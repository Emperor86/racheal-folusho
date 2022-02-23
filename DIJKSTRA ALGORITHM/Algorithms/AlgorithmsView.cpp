// AlgorithmsView.cpp : implementation of the CAlgorithmsView class
//

#include "stdafx.h"
#include "Algorithms.h"

#include "AlgorithmsDoc.h"
#include "AlgorithmsView.h"
#include "ShorthestPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsView

IMPLEMENT_DYNCREATE(CAlgorithmsView, CFormView)

BEGIN_MESSAGE_MAP(CAlgorithmsView, CFormView)
	//{{AFX_MSG_MAP(CAlgorithmsView)
	ON_COMMAND(ID_ADD_NODE, OnAddNode)
	ON_COMMAND(ID_ADD_EDGE, OnAddEdge)
	ON_COMMAND(ID_SHORTEST_PATH, OnShortestPath)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsView construction/destruction

CAlgorithmsView::CAlgorithmsView()
	: CFormView(CAlgorithmsView::IDD)
{
	//{{AFX_DATA_INIT(CAlgorithmsView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CAlgorithmsView::~CAlgorithmsView()
{
}

void CAlgorithmsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgorithmsView)
	DDX_Control(pDX, IDC_DIJKSTRA1, m_Dijkstra);
	//}}AFX_DATA_MAP
}

BOOL CAlgorithmsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CAlgorithmsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsView printing

BOOL CAlgorithmsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAlgorithmsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAlgorithmsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CAlgorithmsView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsView diagnostics

#ifdef _DEBUG
void CAlgorithmsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CAlgorithmsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CAlgorithmsDoc* CAlgorithmsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAlgorithmsDoc)));
	return (CAlgorithmsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAlgorithmsView message handlers

BEGIN_EVENTSINK_MAP(CAlgorithmsView, CFormView)
    //{{AFX_EVENTSINK_MAP(CAlgorithmsView)
	ON_EVENT(CAlgorithmsView, IDC_DIJKSTRA1, 1 /* MouseDown */, OnMouseDownDijkstra1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAlgorithmsView::OnMouseDownDijkstra1(short Button, short Shift, long x, long y) 
{
	//SetPixel(GetDC()->m_hDC, x,y, RGB(0,0,0));
}

void CAlgorithmsView::OnAddNode() 
{
	m_Dijkstra.StartAddNodes();
}

void CAlgorithmsView::OnAddEdge() 
{
	m_Dijkstra.StartAddEdges();
}

void CAlgorithmsView::OnShortestPath() 
{
	CShorthestPath dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_Dijkstra.ShortestPath(dlg.m_node1, dlg.m_node2);
	}
}
