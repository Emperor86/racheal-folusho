/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 05.09.2001 12:19:42
 Version: 1.0.0

*/

#ifndef _thin_ptr_08dde7a2_d5d1_4fbb_a29e_9365fa1b04a8
#define _thin_ptr_08dde7a2_d5d1_4fbb_a29e_9365fa1b04a8

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

template <typename T, const bool tAutoDestroy = false >
struct thin_ptr
{
	typedef T wrapped_type;
	
	T* p;

	thin_ptr(T* p) {this->p = p;}
	thin_ptr(const thin_ptr& x) {this->operator=(x);}
	~thin_ptr() { if (tAutoDestroy) destroy(); }
		
	int destroy() {delete p; p = 0; return 0;}
	T*  release() {T* tp = p; p = 0; return tp;}
	
	T* operator -> () const {_ASSERTE(p); return p;}
	T& operator *  () const {_ASSERTE(p); return *p;}
	T** operator & () const {_ASSERTE(p); return &p;}
	operator T*    () const {_ASSERTE(p); return p;}
	T* operator =  (T* p) {this->p = p;}
		
	// for STL containers
	bool operator < (const thin_ptr& x) const	
	{_ASSERTE(p); _ASSERTE(x.p); return *p < *x.p;}
	
	bool operator!() const	{return p == 0;}
};

#endif //_thin_ptr_08dde7a2_d5d1_4fbb_a29e_9365fa1b04a8

