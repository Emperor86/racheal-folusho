// ACOView.cpp : implementation file
//

#include "stdafx.h"
#include "../AIDemo.h"
#include "../AIDoc.h"
#include "../utils.h"
#include "../MemDC.h"
#include "ACOView.h"
#include "AntConstants.h"
#include "AntOptions.h"



namespace ACO
{
	extern CAntConstants gAntConstants;

	IMPLEMENT_DYNCREATE(CACOView, CAIView)

		CACOView::CACOView()
	{
		//CAntSystem antSystem(30,15);
		antSystem = new CAntSystem(gAntConstants.MAX_CITIES,gAntConstants.MAX_ANTS,
			gAntConstants.ALPHA,gAntConstants.BETA,gAntConstants.RHO);
		antSystem->_curTime=0;
		graph=NULL;
		CGlobals::TIMER.Start(TRUE);
		CGlobals::TIMER.Stop();

		//Set the title
		CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();		
		pMainWnd->SetTitle("Ant Colony Optimization Algorithm");



	}

	CACOView::~CACOView()
	{
		delete antSystem;

	}

	BEGIN_MESSAGE_MAP(CACOView, CAIView)
		ON_WM_PAINT()
		ON_WM_TIMER()
		ON_WM_SIZE()
	END_MESSAGE_MAP()

	void CACOView::ButtonStart()
	{
		CAIView::ButtonStart();
		Invalidate();
	}
	// CACOView drawing

	void CACOView::ButtonStop()
	{

		CAIView::ButtonStop();
		Invalidate();
	}

	void CACOView::ButtonStep()
	{

		CAIView::ButtonStep();
		InvalidateRect(&rectClient);
	}

	void CACOView::ButtonReset()
	{
		//CAIView::ButtonReset();

		if(gAntConstants.VM==vmAll)
		{
			antSystem->Init(&rectTour);
			InvalidateRect(&rectTour);

		}
		else if(gAntConstants.VM==vmTour)
		{
			//Shows only tour
			antSystem->Init(&rectClient);
			InvalidateRect(&rectClient);

		}
		//Invalidate();
		antSystem->_curTime=0;
		antSystem->_bestSoFar=CGlobals::INFINITY;
		CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
		CView* pOldActiveView = pMainWnd->GetActiveView();

		//CAIDoc* pCurrentDoc = (CAIDoc*)(this->GetActiveDocument());
		CAIDoc* pCurrentDoc = (CAIDoc*)pOldActiveView->GetDocument();
		pCurrentDoc->SwitchToView(RUNTIME_CLASS(CACOView));
	}

	void CACOView::ButtonOptions()
	{
		CAIView::ButtonOptions();
		CAntOptions antOpts;
		antOpts.DoModal();
	}
	void CACOView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: add draw code here

	}


	// CACOView diagnostics

#ifdef _DEBUG
	void CACOView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CACOView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}
#endif //_DEBUG


	// CACOView message handlers

	void CACOView::OnPaint()
	{
		CPaintDC dc(this); // device context for painting
		// TODO: Add your message handler code here
		// Do not call CView::OnPaint() for painting messages

		CMemDC memDC(&dc);
		//memDC->Rectangle(&rectTour);
		//memDC->Rectangle(&rectGraph);
		antSystem->DrawEdges(&memDC);
		antSystem->DrawCities(&memDC);
	}

	//-------------------------------------------------
	// UpdateView 
	//-------------------------------------------------
	void CACOView::UpdateView()
	{

		if(antSystem->_ProblemSolved != true)
		{
			//Problem is not solved
			antSystem->_curTime++;
			CGlobals::TIMER.Start(FALSE);
			if ( antSystem->SimulateAnts() == 0 ) 
			{

				//CGlobals::TIMER.Start(TRUE);
				antSystem->UpdateTrails();
				antSystem->RestartAntSystem();	
				CGlobals::TIMER.Stop();
				antSystem->_elapsedTime=CGlobals::TIMER.Elapsedms();		
			}	//if (antSystem->simulateAnts()

			CGlobals::TIMER.Stop();
			antSystem->_elapsedTime=CGlobals::TIMER.Elapsedms();
			

			if(gAntConstants.VM==vmAll)
			{
				this->InvalidateRect(&rectTour,true);
				graph->Invalidate();
			}
			else if(gAntConstants.VM==vmTour)
			{
				this->InvalidateRect(&rectClient,true);
			}		

		}
		else
		{
			//Problem is solved
			CAIView::_isRunning=false;
			CAIView::_isStepping=false;
		}
	}


	//-------------------------------------------------------------------
	// OnTimer
	//------------------------------------------------------------------
	void ACO::CACOView::OnTimer(UINT nIDEvent)
	{
		// TODO: Add your message handler code here and/or call default

		if(_isRunning)
		{
			UpdateView();	  
		}

		if(_isStepping)
		{
			UpdateView();
			_isStepping = false;
		}
		CView::OnTimer(nIDEvent);
	}

	//-------------------------------------------------------------------
	//On Size
	//------------------------------------------------------------------
	void CACOView::OnSize(UINT nType, int cx, int cy)
	{
		CAIView::OnSize(nType, cx, cy);
		if(cx == 0 || cy==0)
			return;

		//Here we calculate the rectangles	
		this->GetClientRect(&rectClient);
		rectClient.DeflateRect(50,0,70,0);

		//calucate the top rectangle rectTour
		rectTour.left= rectClient.left;
		rectTour.top = rectClient.top;

		rectTour.right=rectClient.right;
		rectTour.bottom= (rectClient.bottom-rectClient.top)/2;

		//calculate bottom rectange rectGraph
		rectGraph.left= rectClient.left;
		rectGraph.top = (rectClient.bottom-rectClient.top)/2;

		rectGraph.right= rectClient.right;
		rectGraph.bottom= rectClient.bottom;
		//rectGraph.DeflateRect(0,0,0,0);

		if(gAntConstants.VM==vmAll)
		{

			if(graph != NULL)
			{	
				//CustomiseGraph();
				graph->DestroyWindow();
				graph->Detach();
				delete graph;
				graph = NULL;
				graph = new CChart();
				graph->Create(WS_VISIBLE|WS_CHILD,rectGraph,this,12000);	
				//graph->MoveWindow(&rectGraph,true);
				//graph->SetWindowPos(this,100,rectGraph.top,rectGraph.Width(),rectGraph.Height(),SWP_SHOWWINDOW);
				antSystem->AttachGraph(graph);
				CustomiseGraph();

				//graph->Invalidate(true);
				//graph->ShowWindow(SW_NORMAL);
			}
			else
			{
				graph = new CChart;
				graph->Create(WS_VISIBLE|WS_CHILD,rectGraph,this,12000);	
				antSystem->AttachGraph(graph);
				CustomiseGraph();

			}					

			antSystem->Init(&rectTour);

		}
		else if(gAntConstants.VM==vmTour)
		{
			//Shows only tour
			antSystem->Init(&rectClient);
		}


	}

	//-------------------------------------------------
	//  
	//-------------------------------------------------
	void CACOView::CustomiseGraph()
	{
		graph->SetChartTitle("Ant Colony Optimisation Stats");
		graph->SetChartLabel("","Total Distance");
		graph->m_BGColor=CColor::white;
		graph->SetGridXYNumber(1,1);
		graph->SetAxisStyle(0);
		graph->nSerieCount=2;
		graph->AllocSerie(10000);
		graph->SetRange(0,1000,0,10000);
		graph->mpSerie[0].m_plotColor=CColor::blue;
		graph->mpSerie[1].m_plotColor=CColor::green;


	}

	void CACOView::OnInitialUpdate()
	{
		CAIView::OnInitialUpdate();


	}
}//namespace