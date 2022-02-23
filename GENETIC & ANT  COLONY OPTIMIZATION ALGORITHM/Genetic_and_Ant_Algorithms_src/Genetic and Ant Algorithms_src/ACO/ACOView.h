#pragma once
#include "../AIView.h"
#include "../utils.h"
#include <vector>
#include "../MemDC.h"
#include "../Chart.h"
#include "AntConstants.h"
#include "AntSystem.h"

namespace ACO
{
	
//using namespace std;	

	//-------------------------------------------------
	// ACO VIEW 
	//-------------------------------------------------
	class CACOView : public CAIView
	{
		DECLARE_DYNCREATE(CACOView)
	public:
		void ButtonStart();
		void ButtonStop();
		void ButtonStep();
		void ButtonReset();
		void ButtonOptions();
		void UpdateView();
		CAntSystem* antSystem;
		CChart* graph;
		//Rectangles
		CRect rectTour;
		CRect rectGraph;
		CRect rectClient;
		void CustomiseGraph();
	protected:
		CACOView();           // protected constructor used by dynamic creation
		virtual ~CACOView();

	public:
		virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif

	protected:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnPaint();
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		virtual void OnInitialUpdate();
	};



}//namespace ACO