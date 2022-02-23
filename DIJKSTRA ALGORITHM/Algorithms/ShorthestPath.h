#if !defined(AFX_SHORTHESTPATH_H__B2FBD9EA_02D0_418E_B42A_97F2ED62C375__INCLUDED_)
#define AFX_SHORTHESTPATH_H__B2FBD9EA_02D0_418E_B42A_97F2ED62C375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShorthestPath.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShorthestPath dialog

class CShorthestPath : public CDialog
{
// Construction
public:
	CShorthestPath(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShorthestPath)
	enum { IDD = IDD_DIALOG_SELECT_NODES };
	long	m_node1;
	long	m_node2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShorthestPath)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShorthestPath)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORTHESTPATH_H__B2FBD9EA_02D0_418E_B42A_97F2ED62C375__INCLUDED_)
