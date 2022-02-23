// ShorthestPath.cpp : implementation file
//

#include "stdafx.h"
#include "Algorithms.h"
#include "ShorthestPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShorthestPath dialog


CShorthestPath::CShorthestPath(CWnd* pParent /*=NULL*/)
	: CDialog(CShorthestPath::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShorthestPath)
	m_node1 = 0;
	m_node2 = 0;
	//}}AFX_DATA_INIT
}


void CShorthestPath::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShorthestPath)
	DDX_Text(pDX, IDC_EDIT_NODE1, m_node1);
	DDX_Text(pDX, IDC_EDIT_NODE2, m_node2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShorthestPath, CDialog)
	//{{AFX_MSG_MAP(CShorthestPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShorthestPath message handlers

void CShorthestPath::OnOK() 
{
	UpdateData(TRUE);
	
	CDialog::OnOK();
}
