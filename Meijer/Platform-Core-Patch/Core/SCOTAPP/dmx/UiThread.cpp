// UiThread.cpp : implementation file
//

#include "stdafx.h"
#include "UiThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TRACE_INT_FUNC_ENTER  0x1000000
#define TRACE_INT_FUNC_EXIT   0x2000000
#define TRACE_ALWAYS          0xFFFFFFFF

/////////////////////////////////////////////////////////////////////////////
// CUiThread

// IMPLEMENT_DYNCREATE(CUiThread, CWinThread)

CUiThread::CUiThread(
   CmDataCapture& cmDc )
   : m_cmDc( cmDc ),
   m_evInitComplete( FALSE, FALSE ),
   m_bInitResult ( FALSE )
   {
   ASSERT( m_hThread == NULL );
   }

CUiThread::~CUiThread()
   {
   // You should (must!) call Stop prior to this point. 
   ASSERT( m_hThread == NULL );	// 
   return;
   }

BOOL CUiThread::InitInstance()
   {
	// TODO:  perform and per-thread initialization here
   m_cmDc.DCPrintf( TRACE_INT_FUNC_ENTER, _T("+CUiThread::InitInstance") );
   m_bInitResult = m_pWndIF->InitWindow();
   m_evInitComplete.SetEvent();
   m_cmDc.DCPrintf( TRACE_INT_FUNC_EXIT, _T("-CUiThread::InitInstance") );
   return TRUE;
   }

int CUiThread::ExitInstance()
   {
	// TODO:  perform any per-thread cleanup here
   if( m_bInitResult != FALSE )
      m_pWndIF->ExitWindow();
	return CWinThread::ExitInstance();
   }

BEGIN_MESSAGE_MAP(CUiThread, CWinThread)
	//{{AFX_MSG_MAP(CUiThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUiThread 

BOOL CUiThread::Start( 
   CThreadWndIF*  pWndIF, 
   CWnd*          pMainWnd )
   { 
   m_cmDc.DCPrintf( TRACE_INT_FUNC_ENTER, _T("+CUiThread::Start") );
   ASSERT( pMainWnd != NULL );    	// Must pass a window 
   ASSERT( pWndIF != NULL );
   ASSERT( m_pMainWnd == NULL ); 	// One call only
   m_bInitResult = CreateThread( CREATE_SUSPENDED );
   // If thread was created...
   if ( m_bInitResult == FALSE )
      m_cmDc.DCPrintf( TRACE_ALWAYS, _T("CreateThread FAILED") );
   else 
      {
      m_pMainWnd = pMainWnd;	// Setup 
      m_pWndIF = pWndIF;
      m_bAutoDelete = FALSE;
      ResumeThread();		
      // Now InitInstance is called and we wait on it
      CSingleLock( &m_evInitComplete, TRUE );
      if ( m_bInitResult == FALSE )
         Stop();  // Cleanup
      }
   m_cmDc.DCPrintf( TRACE_INT_FUNC_EXIT, _T("-CUiThread::Start returns %d"), m_bInitResult );
   return m_bInitResult;
   }


void CUiThread::Stop(
   void )
   {
   if ( m_hThread != NULL )
      {
      ASSERT( m_bAutoDelete == FALSE );
      // Tell thread to exit - pass zero as the exit code. 
      PostThreadMessage( WM_QUIT, 0, 0 );
      // Wait on thread 
      WaitForSingleObject( m_hThread, INFINITE );           // 
      // Since we may be re-using this thread, 
      // we need to close the handle it now. 
      CloseHandle( m_hThread );
      m_hThread = NULL;
#ifdef _DEBUG     // 
#if _MFC_VER < 0x0700 
      // MFC uses m_nDisablePumpCount to track whether the thread has  seen the WM_QUIT message
      // Since we are re-using this thread, we need to reset it.
      ASSERT( m_nDisablePumpCount != 0 );    
      m_nDisablePumpCount = 0;
#endif
#endif
      }
   return;
   }




