/////////////////////////////////////////////////////////////////////////////
//
// OposDeviceThread.cpp
//
//   Implementation of OposDeviceThread methods.
//
//   Copyright (c) 1998 NCR.  All rights reserved.
//
//   Date                   Modification                          Author
//---------|---------------------------------------------------|-------------
// 98/04/15 Initial version.                                      T. Burk
//
// This class replaces the Dialog thread template
// Now the device and the dialog are combined into a single class, 
// making the implementation simpler.  Two classes do not act as one. They are one!

#include "stdafx.h"

COposDeviceThread::COposDeviceThread(
   UINT           nIDTemplate,
   CmDataCapture& cmDc )
   : m_pDlg( NULL ),
   m_nIDTemplate( nIDTemplate ),
   m_nShowWindow( 0 ),
   m_cmDc( cmDc )
   {
   ASSERT( nIDTemplate != 0 );
   m_bAutoDelete = FALSE;
   return;
   }


BOOL COposDeviceThread::InitInstance()
   {
   CDataCapEntryExit DCE( m_cmDc,  _T("COposDeviceThread::InitInstance") );
   CoInitialize(NULL);
   ASSERT( m_pDlg != NULL );
   BOOL bResult = m_pDlg->Create( m_nIDTemplate );
   if ( bResult != FALSE )
      {
      m_pDlg->EnableWindow( TRUE);
      m_pDlg->ShowWindow( m_nShowWindow );
      ASSERT( bResult != FALSE );
      m_pMainWnd = m_pDlg;
      }
   m_evComplete.SetEvent(); 
   // Always return TRUE so then m_nDisablePumpCount is set. 
	return TRUE;
   }

int COposDeviceThread::ExitInstance()
   {
   ASSERT( m_pDlg != NULL );
   ASSERT( m_bAutoDelete == FALSE );
   m_pDlg->DestroyWindow();
   m_pMainWnd  = m_pDlg = NULL;
   m_bAutoDelete = FALSE;
   CoUninitialize(); 				      // Close the com libray 
   return CWinThread::ExitInstance();
   }

/////////////////////////////////////////////////////////////////////////////
// COposDeviceThread message handlers


BOOL COposDeviceThread::Start( 
   CDialog* pDialog,
   int      nShowWindow   /* = SW_SHOWMINNOACTIVE */ )
   { 
   CDataCapEntryExit DCE( m_cmDc,  _T("COposDeviceThread::Start") );
   ASSERT( pDialog != NULL );      // Must pass Device Dialog 
   ASSERT( m_pDlg == NULL );       // One call only
   m_pDlg = pDialog;
   m_nShowWindow = nShowWindow;
   BOOL bResult = CreateThread();
   if ( bResult != FALSE )
      {
      CSingleLock( &m_evComplete, TRUE );
      ASSERT( m_pDlg != NULL );
      }
   else
      {
      m_cmDc.DCPrintf( TRACE_ALWAYS, _T("CreateThread FAILED") );
      m_pDlg = NULL;
      }
   return bResult;
   }


void COposDeviceThread::Stop(
   void )
   {
   ASSERT( m_bAutoDelete == FALSE );
   PostThreadMessage( WM_QUIT, 0, 0 );
   // Wait on me to stop processing
   CSingleLock( &CThreadSync( this ), TRUE );
   ASSERT( m_pDlg == NULL );       // One call only
   ASSERT ( m_hThread != NULL );
   // MFC does not close the thread handle until the object is deleted. 
   // Since we are re-using this thread, we need to delete it now. 
   CloseHandle( m_hThread );
   m_hThread = NULL;
#ifdef _DEBUG     // 
   // MFC uses m_nDisablePumpCount to track whether the thread has  seen the WM_QUIT message
   // Since we are re-using this thread, we need to reset it.
   ASSERT( m_nDisablePumpCount != 0 );    
   m_nDisablePumpCount = 0;
#endif
   return;
   }

