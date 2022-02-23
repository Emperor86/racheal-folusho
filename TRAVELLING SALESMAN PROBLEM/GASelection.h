/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 05.09.2001 12:45:08
 Version: 1.0.0

 GA selection algorithms
 tournament, roulette cost and roulette rank
*/

#ifndef _GASelection_635d9733_d091_46ef_ae3b_4d73931a0959
#define _GASelection_635d9733_d091_46ef_ae3b_4d73931a0959

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

template <typename T>
struct selection_tournament
{
	typedef T Traits;
	typedef T::Gene Gene;	
	typedef T::Random Random;	
	typedef T::Population Population;
	typedef T::Pointer Pointer;

	void operator () (Population& parents, Population& children, unsigned size, Random& random)
	{		
		unsigned max_try  = size * 4;
		while (children.size() < size && --max_try)
		{
			/// select first parent
			Gene* parent1 = parents[random.roll(parents.size())];
			Gene* parent2 = parents[random.roll(parents.size())];

			// select second parent
			Gene* parent3 = parents[random.roll(parents.size())];
			Gene* parent4 = parents[random.roll(parents.size())];

			// in fact tournament
			parent1 = (*parent1) < (*parent2) ? parent1 : parent2;
			parent2 = (*parent3) < (*parent4) ? parent3 : parent4;
			
			if ((*parent1 != *parent2))
			{
				// breed
				children.push_back(new Gene(*parent1, *parent2));			
			}		
		}
	}
};

template <typename T, typename V>
struct roulette_base
{
	typedef T Traits;
	typedef T::Gene Gene;	
	typedef T::Random Random;	
	typedef T::Population Population;
	typedef T::Pointer Pointer;
	typedef Gene::Fitness Fitness;	
	typedef V Value;	

	template <typename Value>
	struct sector_t
	{		
		sector_t(Gene* gnp, Value value) {this->gnp = gnp; this->value = value;}		
	//	bool operator < (const sector_t& s) const  {return value < s.value;}
		bool operator < (const sector_t& s) const  {return (*gnp) < (*s.gnp);}

		Gene*			gnp;
		Value			value;
	};

	typedef sector_t<Value> sector;
	typedef std::vector<sector> roulette;

	Gene* spin( roulette::iterator first_it, 
				roulette::iterator last_it, 
				Value max_value, 
				Random& rnd)
	{	
		roulette::iterator it;
		Value last = 0;
		Value val  = 0;
	
	//	val = (Value)rnd.rand() * max_value;
		val = (Value)rnd.roll((unsigned)max_value);
	
		for (it = first_it; it != last_it; ++it)
		{		
			sector& x = (*it);
			if (last <= val && val <= x.value) 
				return (*it).gnp;		// a ball fall into a hole				
			last = x.value;
		}

		return 0;	// default
	}
};

template <typename T>
struct selection_roulette_cost : roulette_base<T, float>
{
	void operator () (Population& parents, Population& children, unsigned size, Random& random)
	{
		roulette r;
		r.reserve(parents.capacity());

		float multi_inv = 0.0, temp = 0.0;

		// compute likelihoods and create roulette data
			
		{for (Population::iterator it = parents.begin(); it != parents.end(); ++it)
		{
			Gene* gnp = (*it);		
			multi_inv += 1.0f/(gnp->fitness());
			r.push_back(sector(gnp, 0));
		}}
			
		{for (roulette::iterator it = r.begin(); it != r.end(); ++it)
		{		
			sector& x = (*it);
			x.value = temp = temp + ((1.0f/(x.gnp->fitness()) / multi_inv) * 100.0f);			 
		}}
		
		// in fact selection roulette	
		unsigned max_try  = size * 4;
		while (children.size() < size && --max_try)
		{
			Gene* parent1 = spin(r.begin(), r.end(), 100.0f, random);
			Gene* parent2 = spin(r.begin(), r.end(), 100.0f, random);
			
			if (parent1 && parent2 && (*parent1 != *parent2))
			{
				// breed
				children.push_back(new Gene(*parent1, *parent2));			
			}		
		}
	}
};

template <typename T>
struct selection_roulette_rank : roulette_base<T, unsigned>
{
	void operator () (Population& parents, Population& children, unsigned size, Random& random)
	{
		roulette r;
		r.reserve(parents.capacity());

		// create roulette
		{for (Population::iterator it = parents.begin(); it != parents.end(); ++it)
	//	{for (Population::iterator it = parents.begin(); it != (parents.begin() + size); it++)
		{
			Gene* gnp = (*it);
		//	r.push_back(sector(gnp, gnp->fitness()));
			r.push_back(sector(gnp, 0));
		}}

		// sort
		// std::sort(r.begin(), r.end());

		unsigned temp = 0;
		Fitness last = 0;
		
		// range by rank
		{for (roulette::reverse_iterator it = r.rbegin(); it != r.rend(); ++it)
		{		
			sector& x = (*it);
			
		//	if (x.value != last)
			if (x.gnp->fitness() != last)
			{			
				last = x.gnp->fitness();
				++temp;
			}		
			
			x.value = temp;				
		}}
		
		temp = 0;	 
		{for (roulette::iterator it = r.begin(); it != r.end(); ++it)
		{
			sector& x = (*it);
			temp += x.value;
			x.value = temp;	
		}}

		// in fact selection roulette	
		unsigned max_try  = size * 4;
		while (children.size() < size && --max_try)
		{
			Gene* parent1 = spin(r.begin(), r.end(), temp, random);
			Gene* parent2 = spin(r.begin(), r.end(), temp, random);
			
			if (parent1 && parent2 && (*parent1 != *parent2))
			{
				// breed
				children.push_back(new Gene(*parent1, *parent2));			
			}		
		}
	}
};

#endif //_GASelection_635d9733_d091_46ef_ae3b_4d73931a0959

