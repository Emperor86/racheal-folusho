// AboutBox.h : Declaration of the AboutBox

#ifndef __ABOUTBOX_H_
#define __ABOUTBOX_H_

#include "resource.h"       // main symbols
#include <atlhost.h>

#include "AnimationCtrl.h"

class AboutBox : 
	public CAxDialogImpl<AboutBox>
{
	CComPtr<IPicture> m_spPicture;
	AnimationCtrl m_alogo;

public:
	AboutBox()
	{}

	~AboutBox()
	{}

	enum { IDD = IDD_ABOUTBOX };

BEGIN_MSG_MAP(AboutBox)
	MESSAGE_HANDLER(WM_INITDIALOG,		OnInitDialog)
	MESSAGE_HANDLER(WM_ERASEBKGND,		OnEraseBkgnd)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC,	OnCtlColorStatic)	
	MESSAGE_HANDLER(WM_DESTROY,			OnDestroy)	
	MESSAGE_HANDLER(WM_LBUTTONDOWN,		OnLButtonDown);
	COMMAND_ID_HANDLER(IDOK,			OnOK)
	COMMAND_ID_HANDLER(IDCANCEL,		OnCancel)
END_MSG_MAP()
	
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
	{
		m_alogo.SubclassWindow(GetDlgItem(IDC_LOGO));
	//	m_alogo.Load("D:\\Kot\\k1.gif");
	//	m_alogo.Start(0, 41, 0, 30, 180, 60, false);	
		m_alogo.Load(_T("RT_GIF"), IDR_RT_GIF1);			
		m_alogo.Start(0, 24, 0, 50, 180, 60, false);
		return 1;  // Let the system set the focus
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
	{	
		m_alogo.Stop();
		return 0;
	}

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
	{
		HDC hdc = (HDC)wParam;
		RECT rc;
		GetClientRect(&rc);		
		FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
		FrameRect(hdc, &rc, (HBRUSH)GetStockObject(GRAY_BRUSH));
		return 0;
	}	

	LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
	{
		HDC hdc = (HDC)wParam;
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(170, 188, 170));
		return (LRESULT)(HBRUSH)GetStockObject(BLACK_BRUSH);
	}
	
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)	
	{
		EndDialog(IDOK);
		return 0;
	}	

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}	
};

#endif //__ABOUTBOX_H_
