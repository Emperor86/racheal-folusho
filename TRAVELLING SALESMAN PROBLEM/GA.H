/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 04.09.2001 11:37:08
 Version: 1.0.0

 GA class 
 implementation of Genetic Algorithm 
 
 example of using

 typedef ga_traits<RandomCRT> traits;
 typedef GA<traits, selection_tournament<traits> > tGA;
 traits::Gene::Context context;
 tGA ga(50, &context);		
 tGA::iterator it = ga.update();
 while(it == ga.end())
 {
	ga.epoch(); 
	it = ga.update(); 
 }
 traits::Gene* gnp = (*it);  
*/

/*	
	the interface of a gene type (GA<>::Gene)

	template <typename R>
	struct gene
	{
		typedef int Fitness;
		typedef R Random;
		typedef SomeCommonGeneData Context;

		gene(Context*, Random&);
		gene(const  gene&);
		gene(const gene& gn, Context* ctx, Random& r)
		gene(const gene&, const gene&);		// crossover
		
		gene& operator =  (const gene&);
		bool  operator == (const gene&) const;
		bool  operator != (const gene&) const;	
		bool  operator <  (const gene&) const;

		friend 
		std::ostream& operator << (std::ostream&, gene&);

		Fitness fitness() const;

		void mutation();		
		bool update();
	};

	the interface of a selection type (GA<>::Selection)
	
	template <typename T>
	struct selection_rr
	{
		typedef T Traits;
		typedef T::Gene Gene;	
		typedef T::Random Random;	
		typedef T::Population Population;	
		typedef T::Pointer Pointer;

		void operator () (Population&, Population&, unsigned, Random&);		
	};

	the interface of traits type (GA<>::Traits)

	template <typename R>
	struct ga_traits
	{
		typedef R Random;
		typedef gene<Random> Gene;				
		typedef thin_ptr<Gene> Pointer;
		typedef std::vector<Pointer> Population;
		typedef CritSectClass CritSect;
	};

	the interface of random type (GA<>::Random)

	struct random
	{	
		random(seed);

		void randomize(seed);
		bool flip();
		bool hazard(unsigned stake, unsigned hi);
		unsigned roll(unsigned hi);
		double rand();
		unsigned operator () (unsigned hi)
	};
*/

#ifndef _GA_49c8b127_edfa_4977_a677_205e3a5c8c82
#define _GA_49c8b127_edfa_4977_a677_205e3a5c8c82

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

template <typename T, typename S>
class GA
{
public:
	typedef T Traits;
	typedef T::Gene Gene;	
	typedef T::Random Random;	
	typedef T::Population Population;	
	typedef T::Pointer Pointer;				// aka thin_ptr<>
	typedef T::CritSect CritSect;	
	typedef S Selection;
	typedef Gene::Fitness Fitness;	
	typedef Gene::Context GeneContext;
	typedef Population::iterator iterator;
	typedef Population::const_iterator const_iterator;
	typedef Population::size_type size_type;
		
	struct tuning
	{	
		unsigned		mutation;		// mutation probability		
		unsigned		crossover;		// crossover probability
		unsigned		elite;			// elite's quota
		bool			eliminate_twins;
	};

	enum
	{
		DEFAULT_MUTATION_PROBABILITY	= 5,	// 1/20
		DEFAULT_INVERSION_PROBABILITY	= 5,	// 1/20
		DEFAULT_CROSSOVER_PROBABILITY	= 99,	// 99/100
		DEFAULT_ELITE_QUOTA				= 10,	// 1/10
	};
	
	GA(unsigned population_size, GeneContext* gene_context, Random* r = 0);
	~GA();

	void init(unsigned size);

	iterator update();			// recompute fitness's values and return optimal gene or end()
	iterator find_best();		// finds the gene with best fitness (lowest value is better)
		
	void epoch(tuning&);		// make next population (selection, crossover, mutation, etc)
	void recombine(unsigned elite_size, unsigned crossover_prob, bool eliminate_twins);
	void mutate(unsigned prob);	
	void migration(GA&, unsigned size);
	void sort();
	
	iterator begin()		{return m_population.begin();}
	iterator end()			{return m_population.end();}
	size_type size() const	{return m_population.size();}
		
private:
	
	Population		m_population;					
	Selection		m_selection;
	Random*			m_random;
	bool			m_random_onwer;
	GeneContext*	m_context;
	CritSect		m_cs;
	bool			m_ordered;
};

template <typename T, typename S>
inline GA<T,S>::GA(unsigned population_size, 
	GA<T,S>::GeneContext* gene_context, GA<T,S>::Random* r)
	: m_random(r ? r : new Random), m_random_onwer(r == 0), m_context(gene_context)
{		
	m_cs.Initialize(4000);
	
	m_population.reserve(population_size);	
	while(population_size--) 
	{
		m_population.push_back(new Gene(gene_context, *m_random));	
	}		

	m_ordered = false;
	// init(population_size);
}

template <typename T, typename S>
inline GA<T,S>::~GA()
{
	std::for_each(m_population.begin(), m_population.end(), 
		std::mem_fun_ref(Population::value_type::destroy));
	if (m_random_onwer) delete m_random;
}

template <typename T, typename S>
inline GA<T,S>::iterator GA<T,S>::update()
{	
	CritSect::Lock lock(m_cs);
	m_ordered = false;
	return std::find_if(m_population.begin(), m_population.end(), std::mem_fun(Gene::update));
}

template <typename T, typename S>
inline GA<T,S>::iterator GA<T,S>::find_best()
{	
	CritSect::Lock lock(m_cs);
	return std::min_element(m_population.begin(), m_population.end());
}

template <typename T, typename S>
inline void GA<T,S>::epoch(GA<T,S>::tuning &t)
{
	CritSect::Lock lock(m_cs);
	recombine(t.elite, t.crossover, t.eliminate_twins);
	mutate(t.mutation);
}

template <typename T, typename S>
inline void GA<T,S>::recombine(unsigned elite_size, unsigned crossover_prob, bool eliminate_twins)
{
	CritSect::Lock lock(m_cs);

	_ASSERTE(m_population.size());
	_ASSERTE(m_population.size() == m_population.capacity());

	Population::size_type psize = m_population.size();
		
	sort();
	
	if (eliminate_twins)
	{	
		// eliminate similar genes to maintain the diversity in order to avoid the immature convergence			
		unsigned b = 0;
		iterator it = m_population.begin();
		Gene* last = *it++;
		for (; it != m_population.end(); it++)
		{
			Gene* g = (*it);
			if (g->fitness() == last->fitness() && 
				*last == *g)
			{
				// eliminate twins
				(*it).release();
				delete g;
				b++;
			}
			else
			{
				last = g;
			}
		}

		if (b)
		{		
	//		TRACE("WARNING in GA::recombine, eliminate a %u twins from population!\n", b);
			iterator end = std::remove_if(m_population.begin(), m_population.end(),
				std::logical_not<Population::value_type>());
			m_population.erase(end, m_population.end());
		}
	}
	
	// selection and crossover

	if (crossover_prob == 0 || 
		(crossover_prob < 100 && !m_random->hazard(crossover_prob, 100)))
	{
		// todo : 

		unsigned x = psize - m_population.size();
		while (x--) 
			m_population.push_back(new Gene(m_context, *m_random));
		return;
	}
	
	Population children;
	children.reserve(m_population.capacity());
	m_selection(m_population, children, psize - elite_size, *m_random);
			
	#ifdef _DEBUG
	if (children.empty())
	{
		TRACE("WARNING in GA::recombine, a children vector is empty!\n");
	}
	#endif

	
	unsigned x = psize - children.size();
	unsigned nelite = min(m_population.size(), elite_size);
	
	if (x > 0)
	{			
		// save elite	
		if (nelite)
		{				
			children.insert(children.end(), m_population.begin(), m_population.begin() + nelite);
			x -= nelite;
		}

		// add new gene
		while (x--) 
			children.push_back(new Gene(m_context, *m_random));
	}
	
	_ASSERTE(children.size());			
	_ASSERTE(children.size() == psize);
	m_population.swap(children);
	
	// finalize cleanup

	// destroy parent's except elite
	std::for_each(children.begin() + nelite, children.end(), 
		std::mem_fun_ref(Population::value_type::destroy));

	if (nelite)
	{	
		// release elite
		std::for_each(children.begin(), children.begin() + nelite, 
			std::mem_fun_ref(Population::value_type::release));
	}	

	m_ordered = false;
}

template <typename T, typename S>
inline void GA<T,S>::mutate(unsigned prob)
{	
	CritSect::Lock lock(m_cs);

	if (0 == prob) return;
	bool always = prob >= 100;

	for (iterator it = m_population.begin(); it != m_population.end(); it++)
	{	
		Population::value_type& gnp = (*it);
		if (always || m_random->hazard(prob, 100))
			gnp->mutation();
	}

	m_ordered = false;
}

template <typename T, typename S>
inline void GA<T,S>::init(unsigned size)
{
	CritSect::Lock lock(m_cs);

	int x = m_population.size() - size;

	if (x > 0)
	{	
		// removes weak genes from population
		// and forces a mutation of a remain genes

		sort();
		std::for_each(m_population.begin() + x, m_population.end(), std::mem_fun_ref(Population::value_type::destroy));
		m_population.erase(m_population.begin() + x, m_population.end());
		
		for (iterator it = m_population.begin(); it != m_population.begin() + x; it++)
			(*it)->mutation();

		x = size;
	}
	else
	{
		x = -x;			
	}
	
	while(x--) 
		m_population.push_back(new Gene(m_context, *m_random));

	m_ordered = false;
}

template <typename T, typename S>
inline void GA<T,S>::migration(GA<T,S>& ga, unsigned size)
{	
	if (this == &ga) return;

	CritSect::Lock lock1(m_cs);			// warning : deadlock probably can be here
	CritSect::Lock lock2(ga.m_cs);

	_ASSERTE(size <= m_population.size());
//	_ASSERTE(size <= ga.m_population.size());

	if (size > ga.m_population.size())
		return; // oops

	sort();
	ga.sort();
		
	unsigned n, i; 
	iterator it;

	// at first removes weak genes from population
	n = m_population.size() - size;
	std::for_each(m_population.begin() + n, m_population.end(), std::mem_fun_ref(Population::value_type::destroy));
	m_population.erase(m_population.begin() + n, m_population.end());
		
	// and then adds new strong genes from another population
	for (i = 0, it = ga.m_population.begin(); i < size; i++, it++)
	{
		Gene* gp = (*it);	
		m_population.push_back(new Gene(*gp, m_context, *m_random));		
	}

	m_ordered = false;
}

template <typename T, typename S>
inline void GA<T,S>::sort()
{
	CritSect::Lock lock(m_cs);
	if (m_ordered) return;
	std::sort(m_population.begin(), m_population.end());
	m_ordered = true;
}

#endif //_GA_49c8b127_edfa_4977_a677_205e3a5c8c82

