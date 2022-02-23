// Main.h : Declaration of the _Main

#ifndef __MAIN_H_
#define __MAIN_H_

#include "resource.h"       // main symbols
#include "board.h"
#include "bardlg.h"

// Help and App commands
#define ID_APP_ABOUT                    0xE140
#define ID_APP_EXIT                     0xE141

///////////////////////////////////////

struct ga_traits
{
	typedef RandomCRT Random;
	typedef TSPGene<Random> Gene;	
	typedef thin_ptr<Gene> Pointer;
	typedef std::vector<Pointer> Population;
	typedef JROptex CritSect;
};

///////////////////////////////////////

struct GAAdapter
{
	virtual ~GAAdapter() {}

	virtual bool create(unsigned, TSPData*)		= 0;
	virtual void init(unsigned)					= 0;
	virtual void update()						= 0;
	virtual ga_traits::Gene* find_best()		= 0;	
	virtual void recombine(unsigned elite_size, unsigned crossover_prob, bool eliminate_twins) = 0;
	virtual void mutate(unsigned prob)			= 0;		
	virtual void migration(GAAdapter*, unsigned size) = 0;
	virtual unsigned size() const				= 0;
	virtual void advance_heuristics (unsigned)	= 0;
};

template <typename S>
class GAAdapterT : public GAAdapter
{
	typedef GA<ga_traits, S> tGA;
	tGA* m_ga;
	ga_traits::Random m_r;

public:
	GAAdapterT() : m_ga(0) {}
	~GAAdapterT() {delete m_ga;}

	virtual bool create(unsigned population_size, TSPData* context)
	{ 
		m_ga = new tGA(population_size, context, &m_r); 
		return m_ga != 0; 
	}
	
	virtual void init(unsigned size)	
	{ m_ga->init(size); }

	virtual void update()
	{ m_ga->update(); }

	virtual ga_traits::Gene* find_best()
	{ return *m_ga->find_best(); }

	virtual void recombine(unsigned elite_size, unsigned crossover_prob, bool eliminate_twins)
	{ m_ga->recombine(elite_size, crossover_prob, eliminate_twins); } 

	virtual void mutate(unsigned prob)
	{ m_ga->mutate(prob); }

	virtual void migration(GAAdapter* gaa, unsigned size)
	{ 
		GAAdapterT<S>* p = dynamic_cast<GAAdapterT<S>*>(gaa);
		_ASSERTE(p);
		m_ga->migration(*p->m_ga, size); 
	}

	virtual unsigned size() const
	{ return m_ga->size(); }

	virtual void advance_heuristics (unsigned h_size)
	{
		m_ga->sort();		
		tGA::iterator end = m_ga->begin() + h_size; 
		for (tGA::iterator it = m_ga->begin(); it != end; ++it)		
			(*it)->heuristics_2opt();		
		m_ga->update();
	}
};

///////////////////////////////////////

class _Main : 
	public CAxDialogImpl<_Main>
{
	typedef std::list<GAAdapter*> evols;

	CStatusBarCtrl	m_hStatusBar;
	BarDlg			m_BarDlg;
	Board			m_Board;
	TSPData*		m_context;			
	LONG			m_bRun;			// the run flag 
	LONG			m_nCount;		// number of running GA thread
	HANDLE			m_hSignal;		// signal of closing GA threads
	evols			m_evols;		// list of running GA
	JROptex			m_optex;		// critical section
		
public:
	_Main() : 
		m_optex(4000),
		m_BarDlg(this),
		m_context(new TSPData),	
		m_bRun(0),
		m_nCount(0),
		m_hSignal(0)
	{}

	~_Main()
	{
		delete m_context;
		if (m_hSignal) VERIFY(CloseHandle(m_hSignal));
	}

	enum { IDD = IDD_MAIN };

 BEGIN_MSG_MAP(_Main)
	MESSAGE_HANDLER(WM_INITDIALOG,		OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY,			OnDestroy)
	MESSAGE_HANDLER(WM_SIZE,			OnSize)
	COMMAND_ID_HANDLER(IDOK,			OnOK)
	COMMAND_ID_HANDLER(IDCANCEL,		OnCancel)
	COMMAND_ID_HANDLER(ID_NEW_CIRCLE,	OnNewCircle)
	COMMAND_ID_HANDLER(ID_NEW_RANDOM,	OnNewRandom)
	COMMAND_ID_HANDLER(ID_APP_ABOUT,	OnAbout)
	COMMAND_ID_HANDLER(ID_APP_EXIT,		OnCancel)
	COMMAND_ID_HANDLER(ID_APP_EMAIL,	OnSendEmail)
	if (uMsg == WM_COMMAND)
		CHAIN_MSG_MAP_MEMBER(m_BarDlg)
 END_MSG_MAP()

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&); 
	LRESULT OnSize(UINT, WPARAM, LPARAM, BOOL&);	
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
	{
		StopComputing();		
		return 0;
	}				
	LRESULT OnOK(WORD, WORD, HWND, BOOL&)
	{		
		return 0;
	}
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&)
	{		
		DestroyWindow();
		return 0;
	}
	LRESULT OnAbout(WORD, WORD, HWND, BOOL&);
	LRESULT OnSendEmail(WORD, WORD, HWND, BOOL&);
	LRESULT OnNewCircle(WORD, WORD, HWND, BOOL&);	
	LRESULT OnNewRandom(WORD, WORD, HWND, BOOL&);	

	/////////////////
	
	BOOL DispatchDialogMessage(MSG* pMsg)
	{
		if (!m_hWnd) return 0;

		if (IsDialogMessage(pMsg)) 		
			return TRUE;
		
		if (m_BarDlg.DispatchDialogMessage(pMsg)) 		
			return TRUE;

		return FALSE;
	}

	virtual void OnFinalMessage(HWND)
	{	
		PostQuitMessage(1);
	}

	/////////////////
	
	static unsigned WINAPI ThreadProc(VOID* pv);

	void GARun();
	void StartComputing();	
	void StopComputing();	
	void Clear();

	bool IsRunning() const {return m_hSignal != 0;}

};

#endif //__MAIN_H_
