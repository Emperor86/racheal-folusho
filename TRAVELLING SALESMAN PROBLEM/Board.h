/*
 Copyright (c) 2001 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 11.09.2001 13:44:03
 Version: 1.0.0

*/

#ifndef _Board_fb3b122b_bbd9_4cf1_b577_3ea726aa847c
#define _Board_fb3b122b_bbd9_4cf1_b577_3ea726aa847c

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

#ifdef max
#undef max
#endif

#define BOARD_WIDTH  100
#define BOARD_HEIGHT 100

#include "travel.h"

class Board : 
	public CWindowImpl<Board, CStatic>
{
	enum {HEIGHT_STATUS_BAR = 16};
	enum {WM_UPDATE = WM_USER + 101};

	TSPData*	m_ctx;				// travel context
	unsigned*	m_travel;			// the best
	unsigned*	m_travel2;			// a second candidate
	unsigned	m_scalex;			// logical width of this board 
	unsigned	m_scaley;			// logical height of this board 
	bool		m_refresh;			// refresh flag
	bool		m_freeze;			// UI freeze flag
	DWORD		m_timestamp;		// last redraw
	DWORD		m_timer0;			// the time of start of computing
	DWORD		m_timer1;			// last check point
	float		m_length;			// length of best travel
	LONG		m_generation[3];	// generation data
	
	HDC			m_hMemDC;			
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBitmap;
	HPEN		m_hPen1;
	HPEN		m_hPen2;
	HPEN		m_hPen3;
	HBRUSH		m_hBrh1;
	HBRUSH		m_hBrh2;
	HFONT		m_hFont;

	JROptex		m_optex;			
	HANDLE		m_thread;			// main thread	
	
//	static __declspec(thread) LONG m_tls_generation;
		
public:

	Board() 
		: m_optex(4000)
	{
		m_ctx		= 0;
		m_travel	= 0;
		m_travel2	= 0;
		m_scalex	= BOARD_WIDTH;
		m_scaley	= BOARD_HEIGHT;
		m_refresh	= false;
		m_freeze	= false;
		m_hMemDC	= 0;
		m_hBitmap	= 0;
		m_hOldBitmap = 0;
		m_timestamp	= 0;	
		m_timer0	= 0;
		m_length	= std::numeric_limits<float>::max();
		m_generation[0] = 0;
		m_generation[1] = 0;
		m_generation[2] = 0;
				
		m_hPen1	= m_hPen2 = m_hPen3 = 0; 
		m_hBrh1 = m_hBrh2 = 0;

		m_hPen1 = CreatePen(PS_SOLID, 0, RGB(128, 0, 0));
		m_hPen2 = CreatePen(PS_SOLID, 0, RGB(0, 0, 225));
		m_hPen3 = CreatePen(PS_SOLID, 0, RGB(200, 225, 245));
		m_hBrh1 = CreateSolidBrush(RGB(225, 0, 0));
		m_hBrh2 = CreateSolidBrush(RGB(168, 168, 168));

		LOGFONT lf = {0};
				
		lstrcpy(lf.lfFaceName, _T("Tahoma"));		
		HDC hDC = ::GetDC(0);
		lf.lfHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		::ReleaseDC(0, hDC);	
		lf.lfWeight	= FW_NORMAL;		
		
		m_hFont = CreateFontIndirect(&lf);
		_ASSERTE(m_hFont);
		
		VERIFY(DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), 
			GetCurrentProcess(), &m_thread, 0, 0, 0));
	}

	~Board()
	{
		CleanupDC();
		SetContext(0);

		if (m_hPen1) DeleteObject(m_hPen1);
		if (m_hPen2) DeleteObject(m_hPen2);
		if (m_hPen3) DeleteObject(m_hPen3);
		if (m_hBrh1) DeleteObject(m_hBrh1);
		if (m_hBrh2) DeleteObject(m_hBrh2);		
		if (m_hFont) DeleteObject(m_hFont);		

		VERIFY(CloseHandle(m_thread));
	}

 DECLARE_WND_SUPERCLASS("Board", CStatic::GetWndClassName());

 BEGIN_MSG_MAP(_Main)	
	MESSAGE_HANDLER(WM_PAINT,		OnPaint)
	MESSAGE_HANDLER(WM_ERASEBKGND,	OnEraseBknd)
	MESSAGE_HANDLER(WM_SIZE,		OnSize)
	MESSAGE_HANDLER(WM_UPDATE,		OnUpdate)
	MESSAGE_HANDLER(WM_LBUTTONDOWN,	OnLButtonDown)
 END_MSG_MAP()

private:
	
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT rc;
		GetClientRect(&rc);		
		PAINTSTRUCT ps;		
		HDC hDC = BeginPaint(&ps);
		Draw(hDC, rc);
		EndPaint(&ps);
		return 0;
	}

	LRESULT OnEraseBknd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 1;
	}
	
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CleanupDC();
		m_refresh = true;
		return 0;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_freeze) return 0;

		if (m_travel)
		{
			delete [] m_travel;
			m_travel = 0;			
		}

		if (m_travel2)
		{
			delete [] m_travel2;
			m_travel2 = 0;
		}			

		RECT rc;
		GetClientRect(&rc);

		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		int cx = rc.right - rc.left;			
		int cy = rc.bottom - rc.top - HEIGHT_STATUS_BAR;

		if (y < cy)
		{					
			float dx = (float)cx / m_scalex;
			float dy = (float)cy / m_scaley;

			x = (int)((float)x / dx);
			y = (int)((float)y / dy);

			m_ctx->add(x, y);

			m_refresh = true;
			Invalidate();
			UpdateWindow();
		}			
		return 0;
	}

	LRESULT OnUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MSG msg; 	
		while (PeekMessage(&msg, m_hWnd, WM_UPDATE, WM_UPDATE, PM_REMOVE))
			;

		RECT rc;
		GetClientRect(&rc);				
		HDC hDC = GetDC();
		Draw(hDC, rc);
		ReleaseDC(hDC);

		SetThreadPriority(m_thread, THREAD_PRIORITY_NORMAL);
		return 0;		
	}
		
	void CreateMemDC(HDC hDC, RECT& rc)
	{
		CleanupDC();		
		m_hMemDC = CreateCompatibleDC(hDC);
		m_hBitmap = CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top);
		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
		m_refresh = true;
	}

	void CleanupDC()
	{
		if (m_hMemDC)
		{
			SelectObject(m_hMemDC, m_hOldBitmap);
			DeleteDC(m_hMemDC);
			DeleteObject(m_hBitmap);
			m_hMemDC = 0;
		}
	}

	void Draw(HDC hDC, RECT& rc)
	{
		if (!m_hMemDC) 
			CreateMemDC(hDC, rc);
		
		if (m_refresh) 
		{
			DrawDC(m_hMemDC, rc);
			m_refresh = false;
		}			

		BitBlt(hDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 
			m_hMemDC, 0, 0, SRCCOPY);

		m_timestamp = GetTickCount();
	}

	void DrawDC(HDC hDC, RECT& rc)
	{
		JROptex::Lock lock(m_optex);

		FillRect(hDC, &rc, m_hBrh2);
				
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top - HEIGHT_STATUS_BAR;
		float dx = (float)cx / m_scalex;
		float dy = (float)cy / m_scaley;

		if (m_ctx && m_ctx->size())
		{
			char buf[100];

			if (m_travel)
			{					
				DWORD ms = m_timer1 - m_timer0;
				DWORD s  = ms / 1000;				
				DWORD m  = s / 60;
				DWORD h  = m / 60;
				ms -= s * 1000;

				sprintf(buf, 
					"points %u, length %8.8f, generation %u (%u) current %u time %02u:%02u:%02u.%03u", 
					m_ctx->size(), 
					(double)m_length, 
					m_generation[0], m_generation[1], m_generation[2],
					h, m, s, ms);				
				
			}
			else
			{
				sprintf(buf, "points %u", m_ctx->size());				
			}

			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(255, 255, 255));
			HFONT hFont = (HFONT)SelectObject(hDC, m_hFont);
			TextOut(hDC, 10, cy + 1, buf, lstrlen(buf));
			SelectObject(hDC, hFont);

			HPEN hPen = (HPEN)SelectObject(hDC, GetStockObject(WHITE_PEN));
			MoveToEx(hDC, 3, cy, 0);
			LineTo(hDC, cx - 3, cy);
			SelectObject(hDC, hPen);
			
			unsigned i, p, x, y;

			if (m_travel2)
			{
				HPEN hPen = (HPEN)SelectObject(hDC, m_hPen3);

				i = m_ctx->m_points.size() - 1;
				p = m_travel2[i];			
				x = int(dx * m_ctx->m_points[p].x);
				y = int(dy * m_ctx->m_points[p].y);
				MoveToEx(hDC, x, y, 0);

				for (i = 0; i < m_ctx->m_points.size(); i++)
				{
					p = m_travel2[i];			
					x = int(dx * m_ctx->m_points[p].x);
					y = int(dy * m_ctx->m_points[p].y);
					
					LineTo(hDC, x, y);
				}

				SelectObject(hDC, hPen);
			}

			i = m_ctx->m_points.size() - 1;
			p = m_travel ? m_travel[i] : i;			
			x = int(dx * m_ctx->m_points[p].x);
			y = int(dy * m_ctx->m_points[p].y);

			MoveToEx(hDC, x, y, 0);
			
			for (i = 0; i < m_ctx->size(); i++)
			{
				p = m_travel ? m_travel[i] : i;			
				x = int(dx * m_ctx->m_points[p].x);
				y = int(dy * m_ctx->m_points[p].y);
				
				HPEN hPen	= (HPEN)SelectObject(hDC, m_hPen1);
				HBRUSH hBrh = (HBRUSH)SelectObject(hDC, m_hBrh1);

				Ellipse(hDC, x - 3, y - 3, x + 3, y + 3);
								
				if (m_travel)
				{
					SelectObject(hDC, m_hPen2);					
					LineTo(hDC, x, y);
				}

				SelectObject(hDC, hPen);
				SelectObject(hDC, hBrh);
			}									
		}		
	}

public:
	void SetContext(TSPData* ctx) 
	{		
		if (m_travel)
		{
			delete [] m_travel;
			m_travel = 0;
		}

		if (m_travel2)
		{
			delete [] m_travel2;
			m_travel2 = 0;
		}
			
		m_ctx =	ctx;
		m_length = std::numeric_limits<float>::max();	
		m_timestamp = 0;
		m_generation[0] = 0;
		m_generation[1] = 0;
		m_generation[2] = 0;
		
		if (m_hWnd)
		{
			m_refresh = true;
			Invalidate();
			UpdateWindow();
		}		

		m_timer0 = GetTickCount();
	}
	
	void Update(unsigned* p, float l, long generation, bool bTryEnter, bool bNewGeneration) 
	{
		if (bNewGeneration)
			InterlockedIncrement(&m_generation[2]);
		
		_ASSERTE(m_ctx);

		if (bTryEnter)
		{
			if (!m_optex.TryEnter())
				return;
		}			
		else
		{
			m_optex.Enter();
		}

		m_timer1 = GetTickCount();
	
		if (l < m_length)
		{
			if (!m_travel) 
				m_travel = new unsigned[m_ctx->m_points.size()];

			m_generation[0] = generation;
			m_generation[1] = m_generation[2];
			m_length = l;
			memcpy(m_travel, p, m_ctx->m_points.size() * sizeof (unsigned));
			m_refresh = true;	
			SetThreadPriority(m_thread, THREAD_PRIORITY_HIGHEST);
			PostMessage(WM_UPDATE);			
		}				
		else if ((GetTickCount() - m_timestamp) > 100)
		{				
			if (!m_travel2) 
				m_travel2 = new unsigned[m_ctx->m_points.size()];
			memcpy(m_travel2, p, m_ctx->m_points.size() * sizeof (unsigned));
			m_refresh = true;	
			SetThreadPriority(m_thread, THREAD_PRIORITY_HIGHEST);
			PostMessage(WM_UPDATE);
		}
				
		m_optex.Leave();

	}

	float GetLength() const {return m_length;}
	void SetFreeze(bool b) {m_freeze = b;}
};

#endif //_Board_fb3b122b_bbd9_4cf1_b577_3ea726aa847c

