// Algorithms.h : main header file for the ALGORITHMS application
//

#if !defined(AFX_ALGORITHMS_H__6E320171_FCE2_4E9F_A7C3_43F0ED5AF5E3__INCLUDED_)
#define AFX_ALGORITHMS_H__6E320171_FCE2_4E9F_A7C3_43F0ED5AF5E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsApp:
// See Algorithms.cpp for the implementation of this class
//

class CAlgorithmsApp : public CWinApp
{
public:
	CAlgorithmsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgorithmsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAlgorithmsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGORITHMS_H__6E320171_FCE2_4E9F_A7C3_43F0ED5AF5E3__INCLUDED_)
