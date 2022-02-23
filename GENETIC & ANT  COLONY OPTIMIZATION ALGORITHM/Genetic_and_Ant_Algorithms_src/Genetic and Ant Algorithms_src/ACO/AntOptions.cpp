// AntOptions.cpp : implementation file
//

#include "stdafx.h"
//#include "resource.h"
#include "AntOptions.h"
#include "../AIDoc.h"
#include "ACOView.h"
#include "AntConstants.h"
namespace ACO{
extern CAntConstants gAntConstants;

IMPLEMENT_DYNAMIC(CAntOptions, CDialog)
CAntOptions::CAntOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CAntOptions::IDD, pParent)
{
	// TODO: Add your command handler code here
	

	
}

CAntOptions::~CAntOptions()
{
}

void CAntOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITANTNO, txtAntNo);
	DDX_Control(pDX, IDC_EDITCITYNO, txtCityNo);	
	DDX_Control(pDX, IDC_EDITALPHA, txtAlpha);
	DDX_Control(pDX, IDC_EDITBETA, txtBeta);
	DDX_Control(pDX, IDC_EDITRHO, txtRho);

	
	DDX_Control(pDX, IDC_RADIOVMTOUR, optVMTour);   
	DDX_Control(pDX, IDC_RADIOVMTOURGRAPH, optVMTourGraph);

	//My DDX
	char buffer[100];

	itoa(gAntConstants.MAX_CITIES,buffer,10);
	txtCityNo.SetWindowText(buffer);

	itoa(gAntConstants.MAX_ANTS,buffer,10);
	txtAntNo.SetWindowText(buffer);

	itoa(gAntConstants.ALPHA,buffer,10);
	txtAlpha.SetWindowText(buffer);

	itoa(gAntConstants.BETA,buffer,10);
	txtBeta.SetWindowText(buffer);

	sprintf(buffer,"%.2f",gAntConstants.RHO);
	txtRho.SetWindowText(buffer);

	if(gAntConstants.VM==vmAll)
		optVMTourGraph.SetCheck(BST_CHECKED);

	if(gAntConstants.VM == vmTour)
		optVMTour.SetCheck(BST_CHECKED);

	
	
}


BEGIN_MESSAGE_MAP(CAntOptions, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CAntOptions message handlers

void CAntOptions::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
    char buffer[20];
	txtCityNo.GetLine(0,buffer,20);
	gAntConstants.MAX_CITIES = atoi(buffer);

	txtAntNo.GetLine(0,buffer,20);
	gAntConstants.MAX_ANTS=atoi(buffer);

	txtAlpha.GetLine(0,buffer,20);
	gAntConstants.ALPHA=atoi(buffer);

	txtBeta.GetLine(0,buffer,20);
	gAntConstants.BETA=atoi(buffer);

	//GetViewMode State

	if(optVMTour.GetState()==1)
		gAntConstants.VM=vmTour;

	

	if(optVMTourGraph.GetState()==1)
		gAntConstants.VM=vmAll;

	char* endString;	
	txtRho.GetLine(0,buffer,20);	
	gAntConstants.RHO=strtod(buffer,&endString);

	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	CView* pOldActiveView = pMainWnd->GetActiveView();

	//CAIDoc* pCurrentDoc = (CAIDoc*)(this->GetActiveDocument());
	CAIDoc* pCurrentDoc = (CAIDoc*)pOldActiveView->GetDocument();
	pCurrentDoc->SwitchToView(RUNTIME_CLASS(CACOView));

	OnOK();
}

}//namespace
