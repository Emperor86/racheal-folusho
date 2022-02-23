// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__DC7F5411_A357_41C1_9942_E1FEC6C4BC5C__INCLUDED_)
#define AFX_STDAFX_H__DC7F5411_A357_41C1_9942_E1FEC6C4BC5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>

#define MAXDOUBLE   0xfffffffffffffffe
//#define MAXLONG		0xfffffffe

#include "Node.h"
#include "Edge.h"
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

typedef vector <CEdge> VTYPE_EDGE;
typedef vector < CNode > VTYPE_NODE;
typedef vector < long > VTYPE_NODE_P;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DC7F5411_A357_41C1_9942_E1FEC6C4BC5C__INCLUDED)
