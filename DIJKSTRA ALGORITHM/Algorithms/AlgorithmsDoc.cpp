// AlgorithmsDoc.cpp : implementation of the CAlgorithmsDoc class
//

#include "stdafx.h"
#include "Algorithms.h"

#include "AlgorithmsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsDoc

IMPLEMENT_DYNCREATE(CAlgorithmsDoc, CDocument)

BEGIN_MESSAGE_MAP(CAlgorithmsDoc, CDocument)
	//{{AFX_MSG_MAP(CAlgorithmsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsDoc construction/destruction

CAlgorithmsDoc::CAlgorithmsDoc()
{
	// TODO: add one-time construction code here

}

CAlgorithmsDoc::~CAlgorithmsDoc()
{
}

BOOL CAlgorithmsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsDoc serialization

void CAlgorithmsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsDoc diagnostics

#ifdef _DEBUG
void CAlgorithmsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAlgorithmsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsDoc commands
