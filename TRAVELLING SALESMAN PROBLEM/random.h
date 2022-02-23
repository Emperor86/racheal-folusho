/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 04.09.2001 12:28:02
 Version: 1.0.0
*/

#ifndef _random_d2b4c0ed_bd5f_4ced_92d2_5c484ab91916
#define _random_d2b4c0ed_bd5f_4ced_92d2_5c484ab91916

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////

class RandomCRT
{
public:	
	RandomCRT(unsigned seed = time(0))				{randomize(seed);}

	void randomize(unsigned seed = time(0))			{srand(seed);}

	bool flip()										{return hazard(50, 100);}	
	bool hazard(unsigned stake, unsigned hi)		{return roll(hi) < stake;}
	unsigned roll(unsigned hi)						{_ASSERTE(hi <= RAND_MAX); return ::rand() % hi;}
	double rand()									{return (double)::rand() / RAND_MAX;} 	// return double between 0 .. 1

	unsigned operator () (unsigned hi)				{return roll(hi);}		
};

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

/*
Pseudo Random Number Generator with 64bit Seed
Andy Thomas [www.alpha-india5.com]

The algorithm used here was based upon code appearing on Dan Hirschberg's
website (Professor of Information and Computer Science at UCI), which itself
was based on code appearing in "Random number generators: good ones are hard
to find", by Stephen Park and Keith Miller. 

REFERENCES:
http://www.ics.uci.edu/~dan/
http://www.ics.uci.edu/~dan/class/165/random.c
Also web search on "random number"+"Miller" for everything you ever wanted to
to know about random number generators.
*/

class Random64
{
	static const unsigned __int64 RDX_RANGE;
	static const unsigned __int64 RDX_CONST;
	static const unsigned __int64 RDX_Q;
	static const unsigned __int64 RDX_R;

public:	
	Random64();

	void randomize(unsigned __int64 seed = 0);

	bool flip();
	bool hazard(unsigned stake, unsigned hi);
	unsigned roll(unsigned hi);
	double rand();

	unsigned operator () (unsigned hi)				{return roll(hi);}		

private:
	unsigned __int64 m_seed;
};

inline Random64::Random64()
{ randomize(); }

inline void Random64::randomize(unsigned __int64 seed)
{ m_seed = seed ? seed : (unsigned __int64)((double)GetTickCount() * RDX_RANGE); }

inline bool Random64::flip()
{ return hazard(50, 100); }

inline bool Random64::hazard(unsigned stake, unsigned hi)
{ return roll(hi) < stake; }

inline unsigned Random64::roll(unsigned range)
{
	unsigned __int64 hi = m_seed/RDX_Q;
	unsigned __int64 lo = m_seed - (hi * RDX_Q);
	m_seed = ((RDX_CONST * lo) - (RDX_R * hi));
	if (m_seed == 0) m_seed = RDX_RANGE;

	int rslt = int(m_seed);
	if (rslt < 0) rslt = -rslt;
	return (unsigned)rslt % range;
}

inline double Random64::rand()
{ return (double)roll(0x7FFFFFFF) / 0x7FFFFFFF; }

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

/*
	uses CryptGenRandom API for random number generation
*/

class RandomCP	 
{
public:
	RandomCP();
	~RandomCP();

	void randomize();

	bool flip();
	bool hazard(unsigned stake, unsigned hi);
	unsigned roll(unsigned hi);
	double rand();

	unsigned operator () (unsigned hi)				{return roll(hi);}		

private:
	HCRYPTPROV	m_hProv;
	unsigned	m_data[256];
	unsigned	m_p;

	void fill();
	unsigned next();
};

inline RandomCP::RandomCP() : m_hProv(0)
{
	CryptAcquireContext(&m_hProv, 0, 0, PROV_RSA_FULL, 0); 
	randomize();	
}

inline RandomCP::~RandomCP()
{ if (m_hProv) CryptReleaseContext(m_hProv, 0); }

inline void RandomCP::randomize()
{ 
	BYTE* p = (BYTE*)m_data;
	BYTE* end = p + sizeof(m_data);
	for (; p < end; p++) *p = unsigned char(::rand() % 0xff); 
	m_p = 0;	// force to call fill
}

inline bool RandomCP::flip()
{ return hazard(50, 100); }

inline bool RandomCP::hazard(unsigned stake, unsigned hi)
{ return roll(hi) < stake; }

inline unsigned RandomCP::roll(unsigned hi)
{ return next() % hi; }

inline double RandomCP::rand() 
{ return (double)next() / 0xffffffff; }

inline void RandomCP::fill()
{ 
	if (m_hProv) CryptGenRandom(m_hProv, sizeof(m_data), (BYTE*)m_data); 
	else randomize(); 
	m_p = sizeof(m_data)/sizeof(m_data[0]);
}

inline unsigned RandomCP::next()
{ 
	if (!m_p) fill(); 
	return m_data[--m_p]; 
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

/*
Entropy calculation and analysis of putative random sequences.
Designed and implemented by John "Random" Walker in May 1985.
For additional information and the latest version, 
see http://www.fourmilab.ch/random/
*/

#ifdef _RANDOM_TEST_

#include <math.h>
class RandomTest
{	
	#define MONTEN	6

	int		binary;				// Treat input as a bitstream
	long	ccount[256];		// Bins to count occurrences of values
	long	totalc; 			// Total bytes counted
	double	prob[256];			// Probabilities per bin for entropy

	int mp, sccfirst;
	unsigned int monte[MONTEN];
	long inmont, mcount;
	double a, cexp, incirc, montex, montey, montepi,
	      scc, sccun, sccu0, scclast, scct1, scct2, scct3,
	      ent, chisq, datasum;
	
public:
	RandomTest() {init(FALSE);} 

	void init(int);
	void add(void*, int);
	void end(double *r_ent, double *r_chisq, double *r_mean, double *r_montepicalc, double *r_scc);

	// Calculate probability of observed distribution 
	// occurring from the results of the Chi-Square test
	double chisq_prob(double chisq);								

private:
	#define log2of10 3.32192809488736234787
	double log2(double x) {return log2of10 * log10(x);}
};

/*
the result for 65535 iterations on PC Win2k AMD Duron 900 

crt : Entropy 7.991328 Chi-square 531.996384 (0.010000) Mean 126.461555 
      Monte-Carlo-Pi 3.173411 Serial-Correlation 0.004470 
r64 : Entropy 7.991577 Chi-square 510.167926 (0.010000) Mean 126.982025 
      Monte-Carlo-Pi 3.174510 Serial-Correlation -0.009285 
cpa : Entropy 7.991367 Chi-square 528.160388 (0.010000) Mean 126.535271 
      Monte-Carlo-Pi 3.176341 Serial-Correlation 0.006607 

crt : flip 0.50438697 rand 0.50051069 time 10 
r64 : flip 0.49869535 rand 0.50022764 time 30 
cpa : flip 0.49868009 rand 0.49896928 time 100 
*/

void random_test(int max_cycles, std::ostream& os);

#endif // _RANDOM_TEST_

#endif //_random_d2b4c0ed_bd5f_4ced_92d2_5c484ab91916

