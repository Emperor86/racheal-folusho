// AlgorithmsView.h : interface of the CAlgorithmsView class
//
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INCLUDES()
#include "dijkstra.h"
//}}AFX_INCLUDES

#if !defined(AFX_ALGORITHMSVIEW_H__90F8503B_B5A6_4EAA_B57A_3A22A2DAB661__INCLUDED_)
#define AFX_ALGORITHMSVIEW_H__90F8503B_B5A6_4EAA_B57A_3A22A2DAB661__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAlgorithmsView : public CFormView
{
protected: // create from serialization only
	CAlgorithmsView();
	DECLARE_DYNCREATE(CAlgorithmsView)

public:
	//{{AFX_DATA(CAlgorithmsView)
	enum { IDD = IDD_ALGORITHMS_FORM };
	CDijkstra	m_Dijkstra;
	//}}AFX_DATA

// Attributes
public:
	CAlgorithmsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgorithmsView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAlgorithmsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAlgorithmsView)
	afx_msg void OnMouseDownDijkstra1(short Button, short Shift, long x, long y);
	afx_msg void OnAddNode();
	afx_msg void OnAddEdge();
	afx_msg void OnShortestPath();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AlgorithmsView.cpp
inline CAlgorithmsDoc* CAlgorithmsView::GetDocument()
   { return (CAlgorithmsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGORITHMSVIEW_H__90F8503B_B5A6_4EAA_B57A_3A22A2DAB661__INCLUDED_)
