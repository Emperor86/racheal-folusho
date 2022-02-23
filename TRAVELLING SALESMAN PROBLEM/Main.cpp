// Main.cpp : Implementation of _Main
#include "stdafx.h"
#include "Main.h"
#include "AboutBox.h"

/////////////////////////////////////////////////////////////////////////////
// _Main

#define MAXIMUM_POINTS 1000

LRESULT _Main::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rc;
	int cx, cy;

	GetClientRect(&rc);
	cx = rc.right - rc.left;
	cy = rc.bottom - rc.top;

	m_Board.SubclassWindow(GetDlgItem(IDC_BOARD));
	
	m_Board.GetWindowRect(&rc);

	ScreenToClient(&rc);
	
	rc.top		= 0;
	rc.left		= rc.right + GetSystemMetrics(SM_CXEDGE) * 2;
	rc.right	= cx;
			
	m_BarDlg.Create(m_hWnd, rc);
	m_BarDlg.MoveWindow(&rc, TRUE);	

	m_BarDlg.ShowWindow(SW_SHOW);
	m_hStatusBar.Create(m_hWnd, 0, 0, WS_CHILD|WS_VISIBLE);

	int parts[1] = {-1};
	m_hStatusBar.SetParts(1, parts);

	m_hStatusBar.SetText(0, "ready", SBT_NOBORDERS);	

	BOOL b;
	OnNewCircle(0, 0, 0, b);

	SendMessage(WM_SETICON, TRUE,  
		(LPARAM)LoadIcon((HINSTANCE) GetWindowLongPtr(GWLP_HINSTANCE), 
         MAKEINTRESOURCE(IDI_ICON2)));
	SendMessage(WM_SETICON, FALSE, (LPARAM) 
		LoadIcon((HINSTANCE) GetWindowLongPtr(GWLP_HINSTANCE), 
		MAKEINTRESOURCE(IDI_ICON2)));

	return 1;  // Let the system set the focus
}

LRESULT _Main::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SIZE_MINIMIZED) 
		return 0;

	int cx	= LOWORD(lParam);
	int cy	= HIWORD(lParam);

	RECT rcSts, rcBar, rcBorder;		
	
	m_hStatusBar.SendMessage(WM_SIZE);
	
	m_hStatusBar.GetWindowRect(&rcSts);
	m_BarDlg.GetWindowRect(&rcBar);
	m_Board.GetWindowRect(&rcBorder);
	ScreenToClient(&rcBorder);
		
	int dcx = GetSystemMetrics(SM_CXEDGE) * 2;

	SIZE szBoard = {
			cx - (rcBar.right - rcBar.left) - dcx, 
			cy - (rcSts.bottom - rcSts.top) - rcBorder.top};
	SIZE szBar = {cx - szBoard.cx - dcx, szBoard.cy + rcBorder.top};
	
	HDWP hdwp = BeginDeferWindowPos(2);
		m_Board.DeferWindowPos (hdwp, 0, 0, 0, szBoard.cx, szBoard.cy,
			SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
		m_BarDlg.DeferWindowPos(hdwp, 0, szBoard.cx + dcx, 0, szBar.cx, szBar.cy,
			SWP_NOZORDER|SWP_NOACTIVATE);		
	EndDeferWindowPos(hdwp); 

	return 0;
}

///////////////////////////////////////////////

class NewMapDlg : 
	public CDialogImpl<NewMapDlg>
{
public:
	unsigned m_number;

	NewMapDlg() {m_number = 100;}
	
 enum { IDD = IDD_NEW};

 BEGIN_MSG_MAP(NewMapDlg)
	MESSAGE_HANDLER(WM_INITDIALOG,	OnInitDialog)	
	COMMAND_ID_HANDLER(IDOK,		OnClose)
	COMMAND_ID_HANDLER(IDCANCEL,	OnClose)	
 END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SendDlgItemMessage(IDC_SPIN_POINTS, UDM_SETRANGE, 0, MAKELONG(1000, 0));
		SetDlgItemInt(IDC_EDIT_POINTS, m_number, FALSE);
		return 0;
	}

 	LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{ 
		m_number = GetDlgItemInt(IDC_EDIT_POINTS, 0, FALSE);
		EndDialog(wID); 
		return 0;
	}	
};

LRESULT _Main::OnNewCircle(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
	// create new map (circle)

	unsigned n = 100;

	if (wID) // via menu otherwise called in OnInitDialog
	{			
		NewMapDlg dlg;
		if (IDOK != dlg.DoModal(m_hWnd))
			return 0;
		n = min(dlg.m_number, MAXIMUM_POINTS);
	}
	
	m_context->init(n);

	double a = 360.0 / n;
	double r = 40.0;
	for (double d = 0.0; d < 360.0; d += a)
	{
		int x, y;			
		x = int(50.0 + sin(d) * r);
		y = int(50.0 + cos(d) * r);
		m_context->add(x, y);
	}

	m_context->end();
	m_Board.SetContext(m_context);
	return 0;
}

LRESULT _Main::OnNewRandom(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// create new map (random)

	NewMapDlg dlg;
	if (IDOK != dlg.DoModal(m_hWnd))
		return 0;
		
	TSPData::points v;	
	v.reserve(BOARD_WIDTH * BOARD_HEIGHT);
	
	dlg.m_number = min(dlg.m_number, MAXIMUM_POINTS);
		
	for (unsigned x = 0; x < BOARD_WIDTH; ++x)
		for (unsigned y = 0; y < BOARD_HEIGHT; ++y)	
			v.push_back(TSPData::point(x, y)); 	
		
	ga_traits::Random r(GetTickCount());
	std::random_shuffle(v.begin(), v.end(), r);
	
	m_context->init(dlg.m_number);	
	
//	for (unsigned i = 0; i < dlg.m_number; i++)
//		m_context->add(v[i].x, v[i].y);
	m_context->m_points.assign(v.begin(), v.begin() + dlg.m_number);	
	
	m_context->end();
	m_Board.SetContext(m_context);
	return 0;
}

/////////////////////////////////////////////////////////////

void _Main::GARun()
{
	// thread's subroutine
	
	mem_pool mpool;
	_ASSERTE(TSPBase::g_mem_pool == 0);
	TSPBase::g_mem_pool = &mpool;
	
	TSPData context(*m_context);
	
	unsigned mutation_prob	= m_BarDlg.m_mutation;
	unsigned crossover_prob	= m_BarDlg.m_crossover;
	unsigned migration		= m_BarDlg.m_migration;
	unsigned elite_size		= m_BarDlg.m_elite;
	bool eliminate_twins	= m_BarDlg.m_eliminate_twins;				
	ga_traits::Gene* gn		= 0;
	LONG generation			= 0;	
	float last				= .0f;
	unsigned r_count		= 0;
	unsigned r_max			= 100;
	unsigned m_count		= 0;
	unsigned m_max			= 20;
	unsigned h_count		= 0;
	unsigned h_max			= 0;						// run heuristics every turn
	unsigned h_size			= m_BarDlg.m_heuristics;	
	GAAdapter* gaa			= 0;

	switch (m_BarDlg.m_selection)
	{
	case 0: 
		gaa = new GAAdapterT<selection_roulette_rank<ga_traits> >;
		break;
	case 1: 
		gaa = new GAAdapterT<selection_roulette_cost<ga_traits> >;
		break;
	case 2: 
		gaa = new GAAdapterT<selection_tournament<ga_traits> >;
		break;
	default: 
		_ASSERTE(0);
		return;
	}

	if (!gaa->create(m_BarDlg.m_population, &context))
	{
		_ASSERTE(0);
		delete gaa;
		return;
	}

	m_optex.Enter();
	m_evols.push_back(gaa);		// stores GA into list of running algorithms
	m_optex.Leave();

	gaa->update();				// compute fitness at first

	while (m_bRun)
	{	
		gaa->recombine(elite_size, crossover_prob, eliminate_twins);		
		gaa->update();

		gn = gaa->find_best();				
		m_Board.Update(gn->m_travel, gn->fitness(), ++generation, true, true);
		
		gaa->mutate(mutation_prob);
		gaa->update();

		if (h_size && 
			++h_count > h_max)
		{
			h_count = 0;
			gaa->advance_heuristics(h_size);	// heuristics
		}
		
		gn = gaa->find_best();	
		m_Board.Update(gn->m_travel, gn->fitness(), generation, true, false);

		if (gn->fitness() == last) ++r_count;	// check convergence to a local optimum
		else last = gn->fitness();
		
		if (r_count > r_max) 
		{
			TRACE("_Main.GARun, reinitializes GA for preventing convergence to a local optimum\n");
			r_count = 0;
			r_max  *= 2;
			last = .0f;
			gaa->init(gaa->size() / 2);	// kills 50% population and mutates remainders
										// todo : it is not enough for preventing local optimum		
										// must find another way
			gaa->update();			
		}				

		if (migration &&				// migrates a best specimen between populations
			++m_count > m_max)
		{
			m_count = 0;
			m_optex.Enter();
			if (m_evols.size() > 1)
			{
				evols::iterator it = std::find(m_evols.begin(), m_evols.end(), gaa);
				_ASSERTE(it != m_evols.end());								
				if (++it == m_evols.end()) it = m_evols.begin();
				{
					gaa->migration((*it), migration);
				}						
			}				
			m_optex.Leave();
		}			
	}		
	
	if (gn)	// updates a board before exit
		m_Board.Update(gn->m_travel, gn->fitness(), generation, false, false);

	m_optex.Enter();
	m_evols.remove(gaa);				
	m_optex.Leave();

	delete gaa;
}

unsigned WINAPI _Main::ThreadProc(VOID* pv)
{
	_Main* this_ = reinterpret_cast<_Main*>(pv);	
	EXCEPTION_POINTERS* pex; 
	__try 	
	{	
		InterlockedIncrement(&this_->m_nCount);
		__try
		{	
			this_->GARun();
		}
		__finally
		{
			if (!InterlockedDecrement(&this_->m_nCount))
				SetEvent(this_->m_hSignal);
		}		
	}
	__except(pex = GetExceptionInformation(), 1)
	{
		kb::MsgBox(
			MB_ICONWARNING, 
			_T("TSPApp, ThreadProc"),
			_T("Exception 0x%08x in GA thread by address 0x%08x\nPlease inform author via email: konstantin@mail.primorye.ru"),
			pex->ExceptionRecord->ExceptionCode,
			pex->ExceptionRecord->ExceptionAddress);
	}
	return 0;
}

void _Main::StartComputing()
{
	if (IsRunning()) return;

	m_hStatusBar.SetText(0, "prepare to start ...", SBT_NOBORDERS);	
	
	m_hSignal = CreateEvent(0, FALSE, FALSE, 0);
	m_Board.SetFreeze(true);		
	m_context->end();	
	m_Board.SetContext(m_context);	
	m_bRun = 1;
	m_evols.clear();

	for (unsigned i = 0; i < m_BarDlg.m_coevolution; i++)
	{		
		unsigned id;	
		HANDLE h = (HANDLE)_beginthreadex(0, 0, ThreadProc, this, 0, &id);
		_ASSERTE(h);
		VERIFY(CloseHandle(h));
	}

	m_hStatusBar.SetText(0, "computing...", SBT_NOBORDERS);	
}

void _Main::StopComputing()
{
	if (!IsRunning()) return;

	m_hStatusBar.SetText(0, "attempt to stop...", SBT_NOBORDERS);	

	m_bRun = 0;
	VERIFY(WaitForSingleObject(m_hSignal, INFINITE) == WAIT_OBJECT_0);
	VERIFY(CloseHandle(m_hSignal));
	m_hSignal = 0;
	m_Board.SetFreeze(false);

	m_hStatusBar.SetText(0, "ready", SBT_NOBORDERS);	
}

void _Main::Clear()
{
	m_context->init();
	m_Board.SetContext(m_context);
}

///////////////////////////////////////////

LRESULT _Main::OnSendEmail(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
	// use COM
	const TCHAR *url = _T("mailto:konstantin@mail.primorye.ru?subject=TSPApp");
	
	HRESULT hr;
	CComPtr<IUniformResourceLocator> spURL;
	hr = ::CoCreateInstance(CLSID_InternetShortcut, 0, CLSCTX_INPROC_SERVER, IID_IUniformResourceLocator, (void**)&spURL);
	if (SUCCEEDED(hr)) 
	{
		hr = spURL->SetURL(url, IURL_SETURL_FL_GUESS_PROTOCOL);
		if (SUCCEEDED(hr)) 
		{  
			URLINVOKECOMMANDINFO ivci;
			ivci.dwcbSize	= sizeof (URLINVOKECOMMANDINFO);
			ivci.dwFlags	= 0;
			ivci.hwndParent	= m_hWnd;
			ivci.pcszVerb	= _T("open"); 
			
			if (SUCCEEDED(spURL->InvokeCommand (&ivci)))
				return 0;
		}
	}	
	
	// attempt to use Shell

	SHELLEXECUTEINFO sei = {0};
	sei.cbSize	= sizeof SHELLEXECUTEINFO;
	sei.fMask	= 0; //SEE_MASK_FLAG_NO_UI ;
	sei.hwnd	= m_hWnd;
	sei.lpVerb	= _T("open");
	sei.lpFile	= url;
	sei.nShow	= SW_SHOW;

	ShellExecuteEx(&sei);
	
	return 0;
}

LRESULT _Main::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	AboutBox dlg;
	dlg.DoModal(m_hWnd);
	return 0;
}