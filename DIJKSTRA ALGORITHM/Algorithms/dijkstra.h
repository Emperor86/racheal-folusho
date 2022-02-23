#if !defined(AFX_DIJKSTRA_H__7416410A_BF1D_4FFA_9464_E93538E67B50__INCLUDED_)
#define AFX_DIJKSTRA_H__7416410A_BF1D_4FFA_9464_E93538E67B50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CDijkstra wrapper class

class CDijkstra : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDijkstra)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x6f02af52, 0xec0b, 0x4188, { 0xa4, 0x52, 0x3f, 0x1e, 0x39, 0xf1, 0xb3, 0x47 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	void Initialize(long nrvert, short bidir);
	void StartAddNodes();
	void StartAddEdges();
	void ShortestPath(long node1, long node2);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIJKSTRA_H__7416410A_BF1D_4FFA_9464_E93538E67B50__INCLUDED_)
