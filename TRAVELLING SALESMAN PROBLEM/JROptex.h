/*

 Created: 02.05.2001 18:51:53
 Version: 1.0.0

 custom critical section based on COptex class from Jeffrry Richter bbok
 'Programming Application for MS Window - Fourth Edition' 

 this class was implemented because of lack of in Win9x the TryEnterCriticalSection API
 and for easy support of mulit-CPU machine
*/

#ifndef _JROptex_1e9ff74d_75c0_4b88_ae66_82d6b88ceefd
#define _JROptex_1e9ff74d_75c0_4b88_ae66_82d6b88ceefd

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

class FakeOptex
{
 public:
	struct Lock
	{		
		Lock(FakeOptex&) {}		
	};

	FakeOptex() {}
	FakeOptex(DWORD) {}	
	
	void Initialize(DWORD) {}
	void Delete() {}

	void SetSpinCount(DWORD) {}
	void Enter() {}
	bool TryEnter() {}
	void Leave() {}
};

class JROptex
{
 public:

	 struct Lock
	 {
		JROptex& m_optex;
		Lock(JROptex& optex) 
			: m_optex(optex) 
		{ m_optex.Enter(); }
		~Lock()	
		{ m_optex.Leave(); }	 
	 };

 private:

	DWORD		m_dwSpinCount;
	LONG		m_lLockCount;
	DWORD		m_dwThreadId;
	LONG		m_lRecurseCount;
	HANDLE      m_hevt;
   
    // 0=multi-CPU, 1=single-CPU, -1=not set yet
   static int& fUniprocessorHost()
   {
	   static int x = -1; return x;
   }
   
 public:

	JROptex();	
	JROptex(DWORD dwSpinCount);	
	~JROptex();

	void Initialize(DWORD dwSpinCount);
	void Delete();

	void SetSpinCount(DWORD dwSpinCount);
	void Enter();
	bool TryEnter();
	void Leave();

 private:   

 public:
	 DWORD getThreadID() const {return m_dwThreadId;}
};

///////////////////////////////////////////////////////////////////////////////
// inline

inline JROptex::JROptex()
	: m_hevt(0)
{}

inline JROptex::JROptex(DWORD dwSpinCount)	
	: m_hevt(0)
{ Initialize(dwSpinCount); }

inline void JROptex::Initialize(DWORD dwSpinCount) 
{
	if (fUniprocessorHost() == -1) 
	{
	  // This is the 1st object constructed, get the number of CPUs
		SYSTEM_INFO sinf;
		GetSystemInfo(&sinf);
		fUniprocessorHost() = (sinf.dwNumberOfProcessors == 1);
	}

	m_dwSpinCount	= 0;
	m_lLockCount	= 0;
	m_dwThreadId	= 0;
	m_lRecurseCount	= 0;
	m_hevt			= 0;

	m_hevt = CreateEventA(0, FALSE, FALSE, 0);
	_ASSERTE(m_hevt);
   
	SetSpinCount(dwSpinCount);
}

inline JROptex::~JROptex() 
{
	Delete();
}

inline void JROptex::Delete() 
{
	if (m_hevt)
	{	
		#ifdef _DEBUG
		if (m_dwThreadId != 0) 
		{		 
			_ASSERTE(!"CS shouldn't be destroyed if any thread owns it");
		}

		if (m_dwThreadId == GetCurrentThreadId()) 
		{	
			_ASSERTE(!"CS shouldn't be destroyed if our thread owns it");
		}
		#endif //_DEBUG

		CloseHandle(m_hevt);	
	}
}

inline void JROptex::SetSpinCount(DWORD dwSpinCount) 
{
   // No spinning on single CPU machines
   if (!fUniprocessorHost())
      InterlockedExchangePointer((PVOID*) &m_dwSpinCount, (PVOID)(DWORD_PTR) dwSpinCount);
}

inline void JROptex::Enter() 
{
   // Spin, trying to get the CS
   if (TryEnter()) 
      return;  // We got it, return

   // We couldn't get the CS, wait for it.
   DWORD dwThreadId = GetCurrentThreadId();

   if (InterlockedIncrement(&m_lLockCount) == 1) 
   {  
	  // CS is unowned, let this thread own it once
      m_dwThreadId = dwThreadId;
      m_lRecurseCount = 1;

   } 
   else 
   {
      if (m_dwThreadId == dwThreadId) 
	  {
         // If CS is owned by this thread, own it again
         m_lRecurseCount++;
      } 
	  else 
	  {
         // CS is owned by another thread, wait for it
         WaitForSingleObject(m_hevt, INFINITE);

         // CS is unowned, let this thread own it once
         m_dwThreadId = dwThreadId;
         m_lRecurseCount = 1;
      }
   }
}

inline bool JROptex::TryEnter() 
{
   DWORD dwThreadId = GetCurrentThreadId();

   BOOL fThisThreadOwnsTheCS = FALSE;     // Assume a thread owns the CS
   DWORD dwSpinCount = m_dwSpinCount; // How many times to spin

   do 
   {
      // If lock count = 0, CS is unowned, we can own it
      fThisThreadOwnsTheCS = (0 == 
         InterlockedCompareExchange(&m_lLockCount, 1, 0)); 

      if (fThisThreadOwnsTheCS) 
	  {
         // CS is unowned, let this thread own it once
         m_dwThreadId = dwThreadId;
         m_lRecurseCount = 1;

      } 
	  else 
	  {
         if (m_dwThreadId == dwThreadId) 
		 {
            // If CS is owned by this thread, own it again
            InterlockedIncrement(&m_lLockCount);
            m_lRecurseCount++;
            fThisThreadOwnsTheCS = TRUE;
         }
      }

   } while (!fThisThreadOwnsTheCS && (dwSpinCount-- > 0));

   // Return whether or not this thread owns the CS
   return fThisThreadOwnsTheCS != 0;
}

inline void JROptex::Leave() 
{
	#ifdef _DEBUG
   // 
	if (m_dwThreadId != GetCurrentThreadId()) 
	//	DebugBreak();
	//	_CrtDbgBreak();
		_ASSERTE(!"Only the owning thread can leave the CS");
	#endif

   // Reduce this thread's ownership of the CS
   if (--m_lRecurseCount > 0) 
   {
      // We still own the CS
      InterlockedDecrement(&m_lLockCount);
   } 
   else 
   {
      // We don't own the CS anymore
      m_dwThreadId = 0;

      if (InterlockedDecrement(&m_lLockCount) > 0) 
	  {
         // Other threads are waiting, the auto-reset event wakes one of them
         SetEvent(m_hevt);
      }
   }
}

#endif //_JROptex_1e9ff74d_75c0_4b88_ae66_82d6b88ceefd

