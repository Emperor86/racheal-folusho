// Node.cpp: implementation of the CNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Node.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNode::CNode()
{
	m_cost = 1;
}

CNode::~CNode()
{
}

CNode CNode::Copy()
{
	CNode ret;
	ret.m_cost = m_cost;
	ret.m_NodeNr = m_NodeNr;
	ret.m_p.x = m_p.x;
	ret.m_p.y = m_p.y;
	
	return ret;
}
