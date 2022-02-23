// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "dijkstra.h"

/////////////////////////////////////////////////////////////////////////////
// CDijkstra

IMPLEMENT_DYNCREATE(CDijkstra, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CDijkstra properties

/////////////////////////////////////////////////////////////////////////////
// CDijkstra operations

void CDijkstra::Initialize(long nrvert, short bidir)
{
	static BYTE parms[] =
		VTS_I4 VTS_I2;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nrvert, bidir);
}

void CDijkstra::StartAddNodes()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDijkstra::StartAddEdges()
{
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CDijkstra::ShortestPath(long node1, long node2)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 node1, node2);
}