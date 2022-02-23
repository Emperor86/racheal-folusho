/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 04.09.2001 12:48:55
 Version: 1.0.0

*/

#include "stdafx.h"
#include "random.h"

const unsigned __int64 Random64::RDX_RANGE = 0xFFFFFFFFFFFFFFFF;
const unsigned __int64 Random64::RDX_CONST = 0x00000000000041A7;
const unsigned __int64 Random64::RDX_Q = Random64::RDX_RANGE / Random64::RDX_CONST;
const unsigned __int64 Random64::RDX_R = Random64::RDX_RANGE % Random64::RDX_CONST;

#ifdef _RANDOM_TEST_

/////////////////////////////////////////////////////
// Random Test

void RandomTest::init(int binmode)
{
	int i;

    binary = binmode;	       // Set binary / byte mode

    // Initialise for calculations

    ent = 0.0;				// Clear entropy accumulator
    chisq = 0.0;			// Clear Chi-Square
    datasum = 0.0;			// Clear sum of bytes for arithmetic mean

    mp = 0;					// Reset Monte Carlo accumulator pointer
    mcount = 0; 			// Clear Monte Carlo tries
    inmont = 0; 			// Clear Monte Carlo inside count
    incirc = 65535.0 * 65535.0; // In-circle distance for Monte Carlo

    sccfirst = TRUE;	       // Mark first time for serial correlation
    scct1 = scct2 = scct3 = 0.0; // Clear serial correlation terms

    incirc = pow(pow(256.0, (double) (MONTEN / 2)) - 1, 2.0);

    for (i = 0; i < 256; i++) 	
		ccount[i] = 0;
    
    totalc = 0;
}

void RandomTest::add(void* buf, int bufl)
{
	unsigned char *bp = (unsigned char *)buf;
    int oc, c, bean;

    while (bean = 0, (bufl-- > 0)) 
	{
		oc = *bp++;

		do 
		{
			if (binary) 		
				c = !!(oc & 0x80);			
			else 			
				c = oc;
			
			ccount[c]++;		  // Update counter for this bin
			totalc++;

			// Update inside / outside circle counts for Monte Carlo computation of PI

			if (bean == 0) 
			{
				monte[mp++] = oc;       // Save character for Monte Carlo
				if (mp >= MONTEN)		// Calculate every MONTEN character
				{
					int mj;
					mp = 0;
					mcount++;
					montex = montey = 0;
					for (mj = 0; mj < MONTEN / 2; mj++) 
					{
						montex = (montex * 256.0) + monte[mj];
						montey = (montey * 256.0) + monte[(MONTEN / 2) + mj];
					}
					if ((montex * montex + montey *  montey) <= incirc) 
					{
						inmont++;
					}
				}
			}

			// Update calculation of serial correlation coefficient

			sccun = c;
			if (sccfirst) 
			{
				sccfirst = FALSE;
				scclast = 0;
				sccu0 = sccun;
			} 
			else 
			{
				scct1 = scct1 + scclast * sccun;
			}
			scct2 = scct2 + sccun;
			scct3 = scct3 + (sccun * sccun);
			scclast = sccun;
			oc <<= 1;
		} while (binary && (++bean < 8));
	} // while
}

void RandomTest::end(double *r_ent, double *r_chisq, double *r_mean, double *r_montepicalc, double *r_scc)
{
	int i;

    // Complete calculation of serial correlation coefficient

    scct1 = scct1 + scclast * sccu0;
    scct2 = scct2 * scct2;
    scc = totalc * scct3 - scct2;
    if (scc == 0.0) 
	{
		scc = -100000;
    } 
	else 
	{
       scc = (totalc * scct1 - scct2) / scc;
    }

    // Scan bins and calculate probability for each bin and Chi-Square distribution

    cexp = totalc / (binary ? 2.0 : 256.0);  // Expected count per bin

    for (i = 0; i < (binary ? 2 : 256); i++) 
	{
       prob[i] = (double) ccount[i] / totalc;
       a = ccount[i] - cexp;
       chisq = chisq + (a * a) / cexp;
       datasum += ((double) i) * ccount[i];
    }

    // Calculate entropy

    for (i = 0; i < (binary ? 2 : 256); i++) 
	{
		if (prob[i] > 0.0) 
		{
			ent += prob[i] * log2(1 / prob[i]);
		}
    }

    // Calculate Monte Carlo value for PI from percentage of hits within the circle

    montepi = 4.0 * (((double) inmont) / mcount);

    // Return results through arguments

    *r_ent = ent;
    *r_chisq = chisq;
    *r_mean = datasum / totalc;
    *r_montepicalc = montepi;
    *r_scc = scc;
}

double RandomTest::chisq_prob(double chisq)
{
	//  Table of chi-square Xp values versus corresponding probabilities
	static double chsqt[2][10] = 
	{
		0.5,
		0.25,
		0.1,
		0.05,
		0.025,
		0.01,
		0.005,
		0.001,
		0.0005,
		0.0001,

		0.0,
		0.6745,
		1.2816,
		1.6449,
		1.9600,
		2.3263,
		2.5758,
		3.0902,
		3.2905,
		3.7190
	};

	double chip;

	chip = sqrt(2.0 * chisq) - sqrt(2.0 * (binary ? 1 : 255.0) - 1.0);
	a = fabs(chip);
	for (int i = 9; i >= 0; i--) 
	{
	   if (chsqt[1][i] < a) 
	   {
	      break;
	   }
	}
	chip = (chip >= 0.0) ? chsqt[0][i] : 1.0 - chsqt[0][i];	
	return chip;
}

/////////////////////////////////////////////////////////////////

void random_test(int max_cycles, std::ostream& os)
{	
	RandomCRT rcrt;
	Random64 r64;
	RandomCP rcp;
	
	RandomTest rt[3];	
	
	unsigned char c[3];
	double ent[3];
	double chisq[3];
	double mean[3];
	double montepicalc[3];
	double scc[3];
	double chip[3];

	int i;
	
	for (i = 0; i < max_cycles; i++)
	{
		c[0] = (unsigned char)rcrt.roll(0xff);
		c[1] = (unsigned char)r64.roll(0xff);
		c[2] = (unsigned char)rcp.roll(0xff);

		for (int i = 0; i < 3; i++)
			rt[i].add(&c[i], 1);	
	}
	
	for (i = 0; i < 3; i++)
	{	
		rt[i].end(&ent[i], &chisq[i], &mean[i], &montepicalc[i], &scc[i]);
		chip[i] = rt[i].chisq_prob(chisq[i]);
	}

	TCHAR s[1023];

	sprintf(s, 
		"crt : Entropy %f Chi-square %f (%f) Mean %f \n"
		"      Monte-Carlo-Pi %f Serial-Correlation %f \n"
		"r64 : Entropy %f Chi-square %f (%f) Mean %f \n" 
		"      Monte-Carlo-Pi %f Serial-Correlation %f \n"
		"cpa : Entropy %f Chi-square %f (%f) Mean %f \n" 
		"      Monte-Carlo-Pi %f Serial-Correlation %f \n"
		,
		ent[0], chisq[0], chip[0] * 100, mean[0], montepicalc[0], scc[0],
		ent[1], chisq[1], chip[1] * 100, mean[1], montepicalc[1], scc[1],
		ent[2], chisq[2], chip[2] * 100, mean[2], montepicalc[2], scc[2]
		);

	os << s << std::endl;

	/////////////////////////////////////////////////

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	double   d[3] = {0.0};	
	unsigned b[3] = {0};
	unsigned t[3] = {0};
		
	t[0] = GetTickCount();
	for (i = 0; i < max_cycles; i++)
	{
		d[0] += rcrt.rand();
		b[0] += (rcrt.flip() ? 1 : 0);
	}
	t[0] = GetTickCount() - t[0];
	
	t[1] = GetTickCount();
	for (i = 0; i < max_cycles; i++)
	{
		d[1] += r64.rand();
		b[1] += (r64.flip() ? 1 : 0);
	}
	t[1] = GetTickCount() - t[1];

	t[2] = GetTickCount();
	for (i = 0; i < max_cycles; i++)
	{
		d[2] += rcp.rand();
		b[2] += (rcp.flip() ? 1 : 0);
	}
	t[2] = GetTickCount() - t[2];
		
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

	sprintf(s, 
		"crt : flip %8.8f rand %8.8f time %u \n"
		"r64 : flip %8.8f rand %8.8f time %u \n"
		"cpa : flip %8.8f rand %8.8f time %u \n"
		,
		(double)b[0] / max_cycles, d[0] / max_cycles, t[0],
		(double)b[1] / max_cycles, d[1] / max_cycles, t[1],
		(double)b[2] / max_cycles, d[2] / max_cycles, t[2]
		);
	
	os << s << std::endl;
}

#endif // _RANDOM_TEST_