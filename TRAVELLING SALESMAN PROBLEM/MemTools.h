/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 10.09.2001 15:15:08
 Version: 1.0.0

 easy memory pool
*/

#ifndef _MemTools_fe50eb21_dfaa_41c2_baab_c59b1185b7f2
#define _MemTools_fe50eb21_dfaa_41c2_baab_c59b1185b7f2

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MT
#include "JROptex.h"
#endif // _MT

#include <vector>

struct mem_pool
{		
	enum {CHUNK_SIZE = 256};

	mem_pool(HANDLE h = GetProcessHeap()) 
		: m_size(0), m_heap(h)
		#ifdef _MT
		, m_optex(4000)
		#endif // _MT
	{
		m_chunks.reserve(16);
		m_pool.reserve(CHUNK_SIZE);
	}

	~mem_pool()
	{
		run_garbage_collector();

		_ASSERTE(m_pool.empty());
		_ASSERTE(m_chunks.empty());		
	}

	void* allocate(unsigned size)
	{	
		#ifdef _MT
		JROptex::Lock lock(m_optex);	
		#endif // _MT

		if (m_pool.empty())
		{			
			if (!m_size)			// save a block size
				m_size = size;		
						
			unsigned s = m_size;
			#ifdef _DEBUG
			s += sizeof(int);
			#endif // _DEBUG

			void * p = HeapAlloc(m_heap, 0, s * CHUNK_SIZE);
			for (BYTE* t = (BYTE*)p; t < (BYTE*)p + (s * CHUNK_SIZE); t += s)
			{
				m_pool.push_back(t);
				#ifdef _DEBUG
				*(int*)(t + m_size) = 0xdeadbeef;
				#endif // _DEBUG
			}
				
			m_chunks.push_back(p);
		}

		_ASSERTE(size == m_size);

		void* p = m_pool.back();
		m_pool.pop_back();
		return p;		
	}

	void deallocate(void* p)
	{	
		if (!p) return;

		#ifdef _MT
		JROptex::Lock lock(m_optex);	
		#endif // _MT

		_ASSERTE(*(int*)((BYTE*)p + m_size) == 0xdeadbeef);		
		m_pool.push_back(p);
	}

	bool run_garbage_collector()
	{
		#ifdef _MT
		JROptex::Lock lock(m_optex);	
		#endif // _MT

		if (m_pool.size() < CHUNK_SIZE) 
			return false; // impossible to trash any data

		std::sort(m_pool.begin(), m_pool.end());

		bool result = false;
		for (chunks::iterator it = m_chunks.begin(); it != m_chunks.end(); it++)
		{
			pool::iterator first = std::find(m_pool.begin(), m_pool.end(), *it);
			if (first == m_pool.end()) continue;
			if ((first + CHUNK_SIZE) > m_pool.end()) continue;	// ! only for vector
			pool::iterator last = first + CHUNK_SIZE;			

			if (((BYTE*)*(last - 1) - (BYTE*)*first) != 
				(int)((CHUNK_SIZE - 1) * (m_size 
				#ifdef _DEBUG
					+ sizeof(int)
				#endif // _DEBUG
				))) 
				continue;

			m_pool.erase(first, last);
			VERIFY(HeapFree(m_heap, 0, *it));
			*it = 0;
			result = true;			
			if (m_pool.size() < CHUNK_SIZE) break;
		}

		if (result)
		{
			chunks::iterator end = std::remove_if(m_chunks.begin(), m_chunks.end(), 
			//	std::bind2nd(std::equal_to<void*>(), 0));	
				std::logical_not<chunks::value_type>());
			m_chunks.erase(end, m_chunks.end());
		}		

		return result;
	}

	void cleanup()
	{
		#ifdef _MT
		JROptex::Lock lock(m_optex);	
		#endif // _MT

		// warning, use this method carefully!

		for (chunks::iterator it = m_chunks.begin(); it != m_chunks.end(); it++)
			VERIFY(HeapFree(m_heap, 0, *it));
		m_chunks.clear();
		m_pool.clear();
		m_size = 0;
	}

private:
	typedef std::vector<void*> pool;
	typedef std::vector<void*> chunks;

	pool		m_pool;
	chunks		m_chunks;
	unsigned	m_size;
	HANDLE		m_heap;
	#ifdef _MT
	JROptex		m_optex;	
	#endif // _MT
};



#endif //_MemTools_fe50eb21_dfaa_41c2_baab_c59b1185b7f2

