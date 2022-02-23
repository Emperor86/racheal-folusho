// Dijkstra.h : Declaration of the CDijkstra

#ifndef __DIJKSTRA_H_
#define __DIJKSTRA_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "AnimAlgCP.h"
#include "STDAFX.h"
#include "Graph.h"
#include "Node.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CDijkstra
class ATL_NO_VTABLE CDijkstra : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IDijkstra, &IID_IDijkstra, &LIBID_ANIMALGLib>,
	public CComControl<CDijkstra>,
	public IPersistStreamInitImpl<CDijkstra>,
	public IOleControlImpl<CDijkstra>,
	public IOleObjectImpl<CDijkstra>,
	public IOleInPlaceActiveObjectImpl<CDijkstra>,
	public IViewObjectExImpl<CDijkstra>,
	public IOleInPlaceObjectWindowlessImpl<CDijkstra>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CDijkstra>,
	public IPersistStorageImpl<CDijkstra>,
	public ISpecifyPropertyPagesImpl<CDijkstra>,
	public IQuickActivateImpl<CDijkstra>,
	public IDataObjectImpl<CDijkstra>,
	public IProvideClassInfo2Impl<&CLSID_Dijkstra, &DIID__IDijkstraEvents, &LIBID_ANIMALGLib>,
	public IPropertyNotifySinkCP<CDijkstra>,
	public CComCoClass<CDijkstra, &CLSID_Dijkstra>,
	public CProxy_IDijkstraEvents< CDijkstra >
{
public:
	CDijkstra()
	{
		if(m_lmfont)
			::DeleteObject(m_lmfont);
		LOGFONT	m_FontUse;
		memset(&m_FontUse, 0, sizeof(LOGFONT));
		m_FontUse.lfHeight = 15;
		m_FontUse.lfWeight = FW_NORMAL;
		m_FontUse.lfOutPrecision=OUT_TT_PRECIS;
		m_FontUse.lfClipPrecision=CLIP_CHARACTER_PRECIS;
		m_FontUse.lfQuality=PROOF_QUALITY;
		m_FontUse.lfPitchAndFamily=DEFAULT_PITCH|FF_DONTCARE;
		memcpy(m_FontUse.lfFaceName, "Arial", 5);
		m_FontUse.lfFaceName[6] = '\0';
		m_lmfont = ::CreateFontIndirect(&m_FontUse);

		m_AddEdges = false;
		m_AddNodes = false;
	}
	~CDijkstra()
	{
		ReleaseAll();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DIJKSTRA)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDijkstra)
	COM_INTERFACE_ENTRY(IDijkstra)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_PROP_MAP(CDijkstra)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CDijkstra)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(DIID__IDijkstraEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CDijkstra)
	CHAIN_MSG_MAP(CComControl<CDijkstra>)
	DEFAULT_REFLECTION_HANDLER()
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] = 
		{
			&IID_IDijkstra,
		};
		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IDijkstra
public:
	void ReleaseGraph();
	void RefreshDone(long node1, long node2);
	bool ExistEdge(CNode u, CNode v);
	double GetEdgeVal(CNode u, CNode v);
	void Relax(CNode u, CNode v, double w);
	CNode ExtractMin(VTYPE_NODE& Q);
	HFONT m_lmfont;
	void InitializeSource(CGraph& g, CNode s);
	STDMETHOD(ShortestPath)(/*[in]*/ long node1, /*[in]*/ long node2);
	CGraph g;
	long GetNode(long x, long y);
	void AddEdge(long x, long y);
	STDMETHOD(StartAddEdges)();
	STDMETHOD(StartAddNodes)();
	void Refresh();
	void AddNode(long x, long y);
	void ReleaseAll();
	POINT m_LastLButtonDownPosition;
	void Draw(HDC hdc);
	bool m_AddNodes;
	bool m_AddEdges;
	STDMETHOD(Initialize)(/*[in]*/ long nrvert, /*[in]*/ short bidir);

	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		//RECT& rc = *(RECT*)di.prcBounds;
		//Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		Draw(di.hdcDraw);

		/*SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 3.0 : Dijkstra");
		TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			pszText, 
			lstrlen(pszText));*/

		return S_OK;
	}
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		return 0;
	}
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		long px=((long)LOWORD(lParam));
		long py=((long)HIWORD(lParam));
        m_LastLButtonDownPosition.x=px;
        m_LastLButtonDownPosition.y=py;
		short fwKeys = short(UINT(wParam));
		if(m_AddNodes)
			AddNode(px, py);
		else if(m_AddEdges)
			AddEdge(px, py);
		Fire_MouseDown(1,fwKeys,px,py);
		return 0;
	}
	LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		return 0;
	}
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		if(::IsWindow(m_hWnd))
		{
			::MoveWindow(m_hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, true);
			::ShowWindow(m_hWnd, SW_SHOW);
		}
		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		return 0;
	}
};

#endif //__DIJKSTRA_H_
