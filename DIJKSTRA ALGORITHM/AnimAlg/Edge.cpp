// Edge.cpp: implementation of the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Edge.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEdge::CEdge()
{
	m_red = false;
	srand( (unsigned)time( NULL ) );
	m_cost = rand()%10;
}

CEdge::~CEdge()
{

}
