/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 07.09.2001 17:14:06
 Version: 1.0.0

 Genome for Travelling Salesman Problem 
  
*/

#ifndef _Travel_e3dfe72a_06ea_439d_9d40_08de1bb784a2
#define _Travel_e3dfe72a_06ea_439d_9d40_08de1bb784a2

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

#include "MemTools.h"

/////////////////////////////////////////////////

struct TSPData
{
	struct point
	{
		point(int x = 0, int y = 0) {this->x = x; this->y = y;}
		int x; int y;
	};

	typedef std::vector<point> points;
	typedef std::vector<unsigned> cache;

	mem_pool	m_mem_pool;
	points		m_points;		
//	unsigned	m_size;			// number of points
	cache		m_cache;		// auxiliary member for corssover
	unsigned*	m_map;			// auxiliary member for corssover
	float*		m_routes;		// pre-computed cost of routes

	TSPData() 
	{
	//	m_size = 0;
		m_map  = 0;
		m_routes = 0;
	}

	/*
	TSPData(unsigned size) 
		: // m_size(size), 
		m_routes(0)
	{
		m_points.resize(size);		
		m_cache.resize(size);
		m_map = alloc_travel();
	}
	*/
	
	TSPData(const TSPData& p)
		: // m_size(p.m_size), 
		m_routes(0)
	{
		m_points.assign(p.m_points.begin(), p.m_points.end());
		m_cache.resize(size());
		m_map = alloc_travel();

		m_routes = new float [size() * size()];
		memcpy(m_routes, p.m_routes, size() * size() * sizeof (float));
	}

	~TSPData()
	{
		free_travel(m_map);
		delete [] m_routes;
	}

	void init(unsigned reserved = 0)
	{		
		m_points.clear();
		m_cache.clear();
		free_travel(m_map);
		m_map = 0;
	//	m_size = 0;
		delete [] m_routes;
		m_routes = 0;

		if (reserved > 0)
			m_points.reserve(reserved);
	}
	
	void add(int x, int y)
	{
		m_points.push_back(point(x, y));
	//	m_size++;
	}
	
	void end()
	{
		m_mem_pool.cleanup();
	//	m_size = m_points.size();
		m_cache.resize(size());
		m_map = alloc_travel();

		delete [] m_routes;
		m_routes = new float [size() * size()];

		for (unsigned iy = 0; iy < size(); ++iy)
			for (unsigned ix = 0; ix < size(); ++ix)
				m_routes[ix + iy * size()] = (float)cost(m_points[ix], m_points[iy]); 
	}

	unsigned size() const {return m_points.size();}

	static double cost(point& pt1, point& pt2) 
	{
		int x = abs(pt1.x - pt2.x);
		int y = abs(pt1.y - pt2.y);
		if (!x) return y;
		if (!y) return x;
		return sqrt(x*x + y*y);
	}

	double cost(unsigned p1, unsigned p2)
	{
		return m_routes[p1 + (p2 * size())];
	}

	double cost(unsigned* begin, unsigned* end) 
	{
		double value = 0;
		
		unsigned* p1 = begin;
		unsigned* p2 = begin + 1;
				
		for (; p2 != end; p2++)
		{		
			value += m_routes[*p1 + ((*p2) * size())];
			p1 = p2;
		}
	
		value += m_routes[*p1 + ((*begin) * size())];
		return value;
	}

	unsigned* alloc_travel ()
	{	
		return (unsigned*)m_mem_pool.allocate(size() * sizeof(unsigned));
	}
	
	void free_travel (unsigned* p)
	{		
		m_mem_pool.deallocate(p);
	}		
};

// lightweigth memory optimization

struct TSPBase
{
	/*
	static mem_pool g_mem_pool;
	
	void* operator new (size_t s)
	{ return g_mem_pool.allocate(s); }
	
	void operator delete (void* p)
	{ g_mem_pool.deallocate(p);	}
	*/

	static __declspec( thread ) mem_pool* g_mem_pool;
	
	void* operator new (size_t s)
	{ return g_mem_pool->allocate(s); }
	
	void operator delete (void* p)
	{ g_mem_pool->deallocate(p);	}
};

template <typename R>
struct TSPGene  : TSPBase
{
	typedef float Fitness;
	typedef R Random;
	typedef TSPData Context;
	
	Context*	m_ctx;
	unsigned*	m_travel;
	Fitness		m_fitness;
	Random&		m_random;

	TSPGene(Context* ctx, Random& r)
		: m_random(r), m_ctx(ctx)
	{		
		m_travel = m_ctx->alloc_travel();

		for (unsigned i = 0; i < m_ctx->size(); i++)
			m_travel[i] = i;	

		std::random_shuffle(m_travel, m_travel + m_ctx->size(), m_random);
	}

	TSPGene(const TSPGene& gn)
		: m_random(gn.m_random), m_travel(0)
	{ this->operator = (gn); }

	TSPGene(const TSPGene& gn, Context* ctx, Random& r)
		: m_random(r), m_ctx(ctx)		
	{ 
		m_travel = m_ctx->alloc_travel();
		m_fitness = gn.m_fitness;
		memcpy(m_travel, gn.m_travel, m_ctx->size() * sizeof(unsigned));								
	}

	// crossover
	TSPGene(const TSPGene& parent1, const TSPGene& parent2)
		: m_random(parent1.m_random), m_ctx(parent1.m_ctx)
	{
		const unsigned BUSY = 0xffffffff;

		m_travel = m_ctx->alloc_travel();
		memset(m_ctx->m_map, 0, m_ctx->size() * sizeof (unsigned));
			
		TSPData::cache::iterator pch = m_ctx->m_cache.begin();		
		unsigned* p		= m_travel;
		unsigned* p1	= parent1.m_travel;
		unsigned* p2	= parent2.m_travel;

		///////////////////////////////////////////
		// Greedy Crossover by Grefenstette
		
		*p = m_random.flip() ? *p1 : *p2;
		m_ctx->m_map[*p] = BUSY;

		while(p < m_travel + m_ctx->size() - 1)
		{
			unsigned v  = *p++;

			for (p1	= parent1.m_travel; p1 < parent1.m_travel + m_ctx->size(); ++p1)
				if (v == *p1) break;
			_ASSERTE(p1 < parent1.m_travel + m_ctx->size());

			for (p2	= parent2.m_travel; p2 < parent2.m_travel + m_ctx->size(); ++p2)
				if (v == *p2) break;
			_ASSERTE(p2 < parent2.m_travel + m_ctx->size());
			
			unsigned v1 = p1 == parent1.m_travel + m_ctx->size() - 1 ? *parent1.m_travel : *(++p1);
			unsigned v2 = p2 == parent2.m_travel + m_ctx->size() - 1 ? *parent2.m_travel : *(++p2);

			if (m_ctx->m_map[v1]) v1 = BUSY;		// already busy				
			if (m_ctx->m_map[v2]) v2 = BUSY;		// already busy

			if (v1 != BUSY && v2 != BUSY)
			{
				_ASSERTE(pch != m_ctx->m_cache.end());

				if (v1 == v2)
				{
					*p = v1;
				}
				else
				{
					double c1 = m_ctx->cost(v, v1);
					double c2 = m_ctx->cost(v, v2);			
					bool b	= c1 < c2;
					*p		= b ? v1 : v2;
					*pch++	= b ? v2 : v1;		// copy to a cache a unused point
				}				
			}
			else if (v1 == BUSY && v2 != BUSY)
				*p = v2;
			else if (v1 != BUSY && v2 == BUSY)
				*p = v1;
			else
			{	
				// todo : random select from cache ?

				_ASSERTE(pch != m_ctx->m_cache.begin());
				do {pch--;
				} while (m_ctx->m_map[*pch]);	// search a first unused point into a cache 
												// there is always an unused point into a cache
				*p = *pch;						// copy from the cache
			}

			m_ctx->m_map[*p] = BUSY;	// mark as busy			
		}
				
		#ifdef _DEBUG
		// checks a validity of travel
		memset(m_ctx->m_map, 0, m_ctx->size() * sizeof (unsigned));
		unsigned value1 = 0, value2 = 0;
		{for (unsigned i = 0; i < m_ctx->size(); i++)
		{
			value1 += m_travel[i];
			value2 += i;
			_ASSERTE(m_ctx->m_map[m_travel[i]] != BUSY);
			m_ctx->m_map[m_travel[i]] = BUSY;
		}}
		_ASSERTE(value1 == value2);
		#endif
	}

	~TSPGene()
	{
		m_ctx->free_travel(m_travel);
	}

	TSPGene& operator = (const TSPGene& gn)
	{ 
		if (this != &gn)
		{	
			m_ctx->free_travel(m_travel);
			m_ctx = gn.m_ctx;
			m_travel = m_ctx->alloc_travel()			
			m_fitness = gn.m_fitness;
			memcpy(m_travel, gn.m_travel, m_ctx->size() * sizeof(unsigned));			
		}
		return *this; 
	}

	bool operator == (const TSPGene& gn) const
	{ 
		if (this != &gn)
		{
			return memcmp(m_travel, gn.m_travel, m_ctx->size() * sizeof(unsigned)) == 0;
		}		
		return true; 
	}

	bool operator != (const  TSPGene& gn) const
	{ 	return ! this->operator == (gn); }

	bool operator < (const TSPGene& gn) const
	{
		if (this == &gn) return false;
		return fitness() < gn.fitness();
	}

	friend 
	std::ostream& operator << (std::ostream& os, TSPGene& gn);

	void mutation() 
	{
		if (m_random.hazard(20, 100))		
		{
			// random mutation

			unsigned x1 = m_random.roll(m_ctx->size());		
			unsigned x2 = m_random.roll(m_ctx->size());
			if (x1 != x2)
			{		
				std::swap(m_travel[x1], m_travel[x2]);
				return;
			}			
		}
		else
		{	
			// Greedy-Swap Mutation

			update();
			Fitness f = fitness();
						
			for (unsigned i = 0; i < m_ctx->size(); i++)
			{				
				unsigned x1 = m_random.roll(m_ctx->size());
				unsigned x2 = m_random.roll(m_ctx->size());
				if (x1 != x2)
				{		
					std::swap(m_travel[x1], m_travel[x2]);
					update();
					if (f > fitness())
						return;
					// restore back
					std::swap(m_travel[x2], m_travel[x1]);
				}	
			}
		}		
	}

	void heuristics_2opt()
	{
		// 2opt mutation by Hiroaki Sengoku and Ikuo YOSHIHARA
		
		update();
		Fitness f = fitness();
		
		for (unsigned x1 = 0; x1 < m_ctx->size() - 1; ++x1)
		{						
			for (unsigned i = x1 + 1; i < m_ctx->size(); ++i)		
			{	
				std::swap(m_travel[x1], m_travel[i]);
				// check fitness
				update();
				if (f > fitness())				
					f = fitness();
				else
					// restore back
					std::swap(m_travel[i], m_travel[x1]);
			} 			
		}		
	}

	bool update() 
	{
		m_fitness = (float) m_ctx->cost(m_travel, m_travel + m_ctx->size());
		return false; // the optimum is never known
	}

	Fitness fitness() const				 {return m_fitness;}
};

template <typename R>
std::ostream& operator << (std::ostream& os, TSPGene<R>& gn)
{	
	os << "fitness (" << gn.m_fitness << ") "
	   << "travel <";
	for (unsigned* p = gn.m_travel; p < gn.m_travel + gn.m_ctx->size(); p++)
		os << *p << " ";
	os << "> ";
	return os;
}

#endif //_Travel_e3dfe72a_06ea_439d_9d40_08de1bb784a2

