/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 11.09.2001 16:28:05
 Version: 1.0.0

*/

#ifndef _BarDlg_1c664483_bc8a_4e60_b7b7_6eafabfedd9c
#define _BarDlg_1c664483_bc8a_4e60_b7b7_6eafabfedd9c

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class _Main;

class BarDlg :
	public CDialogImpl<BarDlg >	
{
public:	

	static const unsigned MIN_PROBABILITY;
	static const unsigned MIN_COEVOLUTION;
	static const unsigned MIN_POPULATION;		
	static const unsigned MAX_PROBABILITY;
	static const unsigned MAX_COEVOLUTION;		
	static const unsigned MAX_POPULATION;
	
	/*
	enum 
	{
		MIN_PROBABILITY	= 0,
		MIN_COEVOLUTION	= 1,
		MIN_POPULATION	= 10,		
		MAX_PROBABILITY	= 100,
		MAX_COEVOLUTION	= 16,		
		MAX_POPULATION	= 1000,
	};
	*/

	unsigned m_coevolution;
	unsigned m_population;
	unsigned m_elite;
	unsigned m_mutation;
	unsigned m_migration;	
	unsigned m_crossover;	
	bool	 m_eliminate_twins;	
	unsigned m_selection;
	unsigned m_heuristics;
	_Main*	 m_main;
	
public:
	BarDlg (_Main* p) 
		: m_main(p) 
	{}
	~BarDlg ()  
	{}
		
	enum {IDD = IDD_BAR};

	BEGIN_MSG_MAP(BarDlg )
		MESSAGE_HANDLER(WM_INITDIALOG,	OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE,		OnSize)
		COMMAND_ID_HANDLER(IDC_START,	OnStart)
		COMMAND_ID_HANDLER(IDC_STOP,	OnStop)
		COMMAND_ID_HANDLER(IDC_RESET,	OnReset)
		COMMAND_ID_HANDLER(IDC_CLEAR,	OnClear)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		BOOL b;
				
		SendDlgItemMessage(IDC_SPIN_COEVOLUTION, UDM_SETRANGE, 0, MAKELONG(MAX_COEVOLUTION, MIN_COEVOLUTION));
		SendDlgItemMessage(IDC_SPIN_POPULATION,  UDM_SETRANGE, 0, MAKELONG(MAX_POPULATION, MIN_POPULATION));
	//	SendDlgItemMessage(IDC_SPIN_HEURISTICS,  UDM_SETRANGE, 0, MAKELONG(100, 0));

		SendDlgItemMessage(IDC_COMBO_SELECTION, CB_ADDSTRING, 0, (LPARAM)_T("r. rank"));
		SendDlgItemMessage(IDC_COMBO_SELECTION, CB_ADDSTRING, 0, (LPARAM)_T("r. cost"));
		SendDlgItemMessage(IDC_COMBO_SELECTION, CB_ADDSTRING, 0, (LPARAM)_T("tournament"));

		OnReset(0, 0, 0, b);
		UpdateUI(TRUE);

		return 0;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT rc = {0, 0, LOWORD(lParam), HIWORD(lParam)};
		
		CWindow hFrame(GetDlgItem(IDC_FRAME));	
		rc.bottom -= 2;
		hFrame.SetWindowPos(0, &rc, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);

		return 0;
	}

	LRESULT OnStart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
			
	LRESULT OnReset(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{	
		kb::SystemInfo si;

		SetDlgItemInt(IDC_EDIT_COEVOLUTION, si.dwNumberOfProcessors * 2, FALSE);
		SetDlgItemInt(IDC_EDIT_POPULATION,  100,	FALSE);
		SetDlgItemInt(IDC_EDIT_ELITE,		15,		FALSE);
		SetDlgItemInt(IDC_EDIT_MUTATION,	15,		FALSE);
		SetDlgItemInt(IDC_EDIT_MIGRATION,	5,		FALSE);
		SetDlgItemInt(IDC_EDIT_CROSSOVER,	95,		FALSE);
		SetDlgItemInt(IDC_EDIT_HEURISTICS,	5,		FALSE);
		CheckDlgButton(IDC_CHECK_ELTWINS, BST_CHECKED);
		SendDlgItemMessage(IDC_COMBO_SELECTION, CB_SETCURSEL, 0);		
		return 0;
	}

	BOOL DispatchDialogMessage(MSG* pMsg)
	{
		if (!m_hWnd) return FALSE;

		if (IsDialogMessage(pMsg)) 		
			return TRUE;

		return FALSE;
	}

	void UpdateUI(bool b)
	{
		::EnableWindow(GetDlgItem(IDC_EDIT_COEVOLUTION),b);
		::EnableWindow(GetDlgItem(IDC_EDIT_POPULATION), b);
		::EnableWindow(GetDlgItem(IDC_EDIT_ELITE),		b);
		::EnableWindow(GetDlgItem(IDC_EDIT_MUTATION),	b);
		::EnableWindow(GetDlgItem(IDC_EDIT_MIGRATION),	b);
		::EnableWindow(GetDlgItem(IDC_EDIT_CROSSOVER),	b);
		::EnableWindow(GetDlgItem(IDC_EDIT_HEURISTICS),	b);

		::EnableWindow(GetDlgItem(IDC_SPIN_COEVOLUTION),b);
		::EnableWindow(GetDlgItem(IDC_SPIN_POPULATION), b);
		::EnableWindow(GetDlgItem(IDC_SPIN_ELITE),		b);
		::EnableWindow(GetDlgItem(IDC_SPIN_MUTATION),	b);
		::EnableWindow(GetDlgItem(IDC_SPIN_MIGRATION),	b);
		::EnableWindow(GetDlgItem(IDC_SPIN_CROSSOVER),	b);
		::EnableWindow(GetDlgItem(IDC_SPIN_HEURISTICS),	b);
		
		::EnableWindow(GetDlgItem(IDC_CHECK_ELTWINS), b);
		::EnableWindow(GetDlgItem(IDC_COMBO_SELECTION), b);

		::EnableWindow(GetDlgItem(IDC_START), b);
		::EnableWindow(GetDlgItem(IDC_STOP),  !b);		
		::EnableWindow(GetDlgItem(IDC_CLEAR), b);
				
		HMENU hMenu		= ::GetMenu(GetParent());
		HMENU hFileMenu = GetSubMenu(hMenu, 0);
		HMENU hGAMenu	= GetSubMenu(hMenu, 1);

		EnableMenuItem(hFileMenu, 0, MF_BYPOSITION | (b ? MF_ENABLED : MF_GRAYED));
		
		EnableMenuItem(hGAMenu, IDC_START, MF_BYCOMMAND | (b ? MF_ENABLED : MF_GRAYED));
		EnableMenuItem(hGAMenu, IDC_STOP,  MF_BYCOMMAND | (b ? MF_GRAYED  : MF_ENABLED));
		EnableMenuItem(hGAMenu, IDC_RESET, MF_BYCOMMAND | (b ? MF_ENABLED : MF_GRAYED));
		EnableMenuItem(hGAMenu, IDC_CLEAR, MF_BYCOMMAND | (b ? MF_ENABLED : MF_GRAYED));
	}
};

#endif //_BarDlg_1c664483_bc8a_4e60_b7b7_6eafabfedd9c

