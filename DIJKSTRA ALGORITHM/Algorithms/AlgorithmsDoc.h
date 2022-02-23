// AlgorithmsDoc.h : interface of the CAlgorithmsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALGORITHMSDOC_H__3086D9E2_AECD_490D_AD79_E70C288F84F8__INCLUDED_)
#define AFX_ALGORITHMSDOC_H__3086D9E2_AECD_490D_AD79_E70C288F84F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAlgorithmsDoc : public CDocument
{
protected: // create from serialization only
	CAlgorithmsDoc();
	DECLARE_DYNCREATE(CAlgorithmsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgorithmsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAlgorithmsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAlgorithmsDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGORITHMSDOC_H__3086D9E2_AECD_490D_AD79_E70C288F84F8__INCLUDED_)
