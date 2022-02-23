/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 25.09.2001 18:08:29
 Version: 1.0.0

*/

#include "StdAfx.h"
#include "BarDlg.h"
#include "Main.h"

const unsigned BarDlg::MIN_PROBABILITY	= 0;
const unsigned BarDlg::MIN_COEVOLUTION	= 1;
const unsigned BarDlg::MIN_POPULATION	= 10;		
const unsigned BarDlg::MAX_PROBABILITY	= 100;
const unsigned BarDlg::MAX_COEVOLUTION	= 16;		
const unsigned BarDlg::MAX_POPULATION	= 1000;

LRESULT BarDlg::OnStart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{	
	m_coevolution	= GetDlgItemInt(IDC_EDIT_COEVOLUTION, 0, FALSE);
	m_population	= GetDlgItemInt(IDC_EDIT_POPULATION,  0, FALSE);
	m_elite			= GetDlgItemInt(IDC_EDIT_ELITE,		0, FALSE);	
	m_migration		= GetDlgItemInt(IDC_EDIT_MIGRATION,	0, FALSE);
	m_heuristics	= GetDlgItemInt(IDC_EDIT_HEURISTICS,	0, FALSE);
	m_mutation		= GetDlgItemInt(IDC_EDIT_MUTATION,	0, FALSE);	
	m_crossover		= GetDlgItemInt(IDC_EDIT_CROSSOVER,	0, FALSE);
	m_eliminate_twins = IsDlgButtonChecked(IDC_CHECK_ELTWINS) == BST_CHECKED;
	m_selection		= SendDlgItemMessage(IDC_COMBO_SELECTION, CB_GETCURSEL);	

	m_coevolution	= std::_MAX(std::_MIN(m_coevolution, MAX_COEVOLUTION), MIN_COEVOLUTION);
	m_population	= std::_MAX(std::_MIN(m_population,  MAX_POPULATION),  MIN_POPULATION);
	m_elite			= std::_MAX(std::_MIN(m_population,  m_elite),  0u);
	m_migration		= std::_MAX(std::_MIN(m_population,  m_migration),  0u);
	m_heuristics	= std::_MAX(std::_MIN(m_population,  m_heuristics),  0u);
	m_mutation		= std::_MAX(std::_MIN(m_mutation,    MAX_PROBABILITY),  MIN_PROBABILITY);
	m_crossover		= std::_MAX(std::_MIN(m_crossover,   MAX_PROBABILITY),  MIN_PROBABILITY);

	SetDlgItemInt(IDC_EDIT_COEVOLUTION, m_coevolution,	FALSE);
	SetDlgItemInt(IDC_EDIT_POPULATION,  m_population,	FALSE);
	SetDlgItemInt(IDC_EDIT_ELITE,		m_elite,		FALSE);
	SetDlgItemInt(IDC_EDIT_MIGRATION,	m_migration	,	FALSE);	
	SetDlgItemInt(IDC_EDIT_HEURISTICS,	m_heuristics,	FALSE);
	SetDlgItemInt(IDC_EDIT_MUTATION,	m_mutation,		FALSE);	
	SetDlgItemInt(IDC_EDIT_CROSSOVER,	m_crossover,	FALSE);

	UpdateUI(FALSE);
	
	m_main->StartComputing();
	return 0;
}

LRESULT BarDlg::OnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{		
	UpdateUI(TRUE);
	m_main->StopComputing();		
	return 0;
}

LRESULT BarDlg::OnClear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_main->Clear();		
	return 0;
}