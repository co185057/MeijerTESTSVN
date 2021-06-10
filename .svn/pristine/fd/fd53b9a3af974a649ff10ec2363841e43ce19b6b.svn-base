/////////////////////////////////////////////////////////////////////////////
//
// OposRequest.cpp
//
// Implementation of COposRequest methods.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#include "stdafx.h"

// Notes:   1) CList is not thread save. We use a critical section to 
//          guard m_qList and m_bEnabled, but we are not concerned 
//          about synchronizing access to the CWinThread pointer. 
//          This is true since the Service Objects are single threaded
//          with regard to starting and stopping the thread. 

// COposRequestQueue::COposRequestQueue
//
// Purpose: Constructor

COposRequestQueue::COposRequestQueue( 
   CmDataCapture&  cmDc )
   : m_pServiceThread( NULL ),
   m_csAccess( ),
   m_evProcess( FALSE, TRUE ),      // Initially Reset and Manual Reset Event
   m_evExit( FALSE, TRUE ),         // Initially Reset and Manual Reset Event
   m_evEmpty( TRUE, TRUE ),
   m_bEnabled( FALSE ),
   m_bAborted( FALSE ),
   m_bRetry( FALSE ),
   m_bProcessing( FALSE ),
   m_cmDc( cmDc )
   {
   }


// COposRequestQueue::~COposRequestQueue
//
// Purpose: Destructor
// Note:    Data Capture is not available!


COposRequestQueue::~COposRequestQueue( 
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposRequestQueue::~COposRequestQueue") );
   ASSERT( m_bEnabled == FALSE );      // Disable before the destructor!
   ASSERT( m_pServiceThread == NULL ); // Disable before the destructor!
   return;
   }


// COposRequestQueue::ServiceThreadEntry
//
// Purpose: (Static) thread entry point. 


UINT COposRequestQueue::ServiceThreadEntry( 
   LPVOID pObj)
   {
   ASSERT( pObj != NULL );
   return ((COposRequestQueue*)pObj)->ServiceThread();
   }

// COposRequestQueue::ServiceThread
//
// Purpose: The processing thread. Requests are processed until either 
//          the thread is told to exit, processing is disabled 
//          or the queue is empty.  If a request fails, then processing
//          is disabled here. 
// Notes:   CMultiLock is used since only events are used.  
//          Since we unlock the object while the request is processed, anything can happen on 
//          another thread. Some possibilities?
//          1) Nothing.    ( This was the easy case.) 
//          2) ClearOutput    ( request in progress should be aborted. )
//          3) RetryOutput ( request in progress failed, and should be re-processed.) 
//          These cases (2,3) are race conditions, and are handled via the m_bAbort and m_bRetry flags. 
//          The flags are only significant if this thread is in pRequest->Process().
//          If m_bAbort is TRUE upon request completion, then the current request is always thrown 
//          away, even if it failed. The flag remembers that we lost the race, and that the event 
//          thread has already acted upon the error. 
//          This logic also handles the normal ClearOutput() case, with a request in
//          progress. We discard the request because it would have been aborted had it been in the
//          queue. This allows callers to call Device and Service ClearOutputs in either order. 
//          If m_bRetry is TRUE, then the request must have failed ( the assertion ) and the event
//          thread got this object locked before we could re-queue the request, and disable the queue.
//          So m_bRetry remembers the we lost the race, and we need to leave the queue enabled, 
//          and retry the failed request as soon as possible. 
//          The assertion allows for the case that both RetryRequest and ClearOutput were called while
//          the request was being processed

UINT COposRequestQueue::ServiceThread( 
   void )
   {
   CSyncObject* hSync[] = { &m_evExit, &m_evProcess };
   CMultiLock mlEvents( hSync, ARRAY_ELEMENT_COUNT(hSync) );
   CDataCapEntryExit DCE( m_cmDc, _T("COposRequestQueue::ServiceThread") );

   while ( mlEvents.Lock( INFINITE, FALSE) != WAIT_OBJECT_0 )
      {
      CSingleLock csAccess( &m_csAccess, TRUE );
      // While still enabled and not wmpty
      if ( m_bEnabled != FALSE 
         && m_qList.IsEmpty() == FALSE )
         {
         // Get first element from the queue
         COposRequest* pRequest = m_qList.RemoveHead( );
         ASSERT( pRequest != NULL );
         m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, 
            _T("COposRequestQueue::ServiceThread - Process") );
         m_bProcessing  = TRUE;
         m_bAborted     = FALSE;
         m_bRetry       = FALSE;
         // Allow Queue manipulation and processing ( Abort, IsEmpty )
         csAccess.Unlock();
         BOOL bResult = pRequest->Process( m_cmDc );
         csAccess.Lock();
         // Processing remains enabled only if request allows and 
         // we are still still enabled and ClearOutput not called!
         if ( bResult == FALSE 
            && m_bAborted == FALSE )
            {
            m_bEnabled =  m_bRetry;
            m_qList.AddHead( pRequest );
            }
         else
            {
            ASSERT( bResult == FALSE || m_bRetry == FALSE );  
            delete pRequest;         // RetryRequest called when no error occurred!
            }
         m_bProcessing = m_bAborted = m_bRetry = FALSE;
         if ( m_qList.GetCount() == 0 )
            m_evEmpty.SetEvent();
         }
      else
         m_evProcess.ResetEvent();
      }
   return OPOS_SUCCESS;
   }

// COposRequestQueue::BeginService
//
// Purpose: Create a thread to service the request queue.


void COposRequestQueue::BeginService(
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposRequestQueue::BeginService") );
   ASSERT( m_pServiceThread == NULL );
   m_evExit.ResetEvent( );
   m_pServiceThread = AfxBeginThread( 
      ServiceThreadEntry, this, THREAD_PRIORITY_ABOVE_NORMAL );
   CheckThreadObject( m_pServiceThread );
   m_pServiceThread->m_bAutoDelete = FALSE;
   return ;
   }

// COposRequestQueue::EndService
//
// Purpose: Stop the thread that is processing requests.
// Note:    Delete queued requests

void COposRequestQueue::EndService(
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposRequestQueue::EndService") );
   ASSERT( m_pServiceThread != NULL );
   m_evProcess.ResetEvent();
   m_evExit.SetEvent( ); // Signal to exit
   CSingleLock ( &CThreadSync( m_pServiceThread ), TRUE );
//	::WaitForSingleObject(  m_pServiceThread->m_hThread, INFINITE );
   delete m_pServiceThread;
   m_pServiceThread = NULL;   
	ClearOutput();
   return ;
   }


// COposRequestQueue::Add
//
// Purpose: Add request to the queue.


void COposRequestQueue::Add( 
   COposRequest* pRequest )
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   CDataCapEntryExit DCE( m_cmDc, _T("COposRequestQueue::Add") );
   m_qList.AddTail( pRequest );
   m_evProcess.SetEvent();
   m_evEmpty.ResetEvent();
   return;
   }


// COposRequestQueue::ClearOutput
//
// Purpose: Abort all queue entries.  
// Note:    This may be called before the processing thread has detected the error, if there was one.
//          m_bAbort makes sure that the processing thread is aware of the abort and discards the
//          current request, even if it failed. 

void COposRequestQueue::ClearOutput( 
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposRequestQueue::ClearOutput") );
   CSingleLock csAccess( &m_csAccess, TRUE );
   // For any requests not being processed by the Thread
   m_evProcess.ResetEvent();
   while ( m_qList.IsEmpty() == FALSE)
      {
      COposRequest* pRequest = m_qList.RemoveHead( );
      ASSERT( pRequest != NULL );
      delete pRequest;
      }
   // If we are processing, then let the thread know that the request was aborted
   // So that it will never be re-queued. It should be discarded!
   m_bAborted = m_bProcessing;
   // Now re-enable the queue if the thread is running
   if ( m_pServiceThread != NULL )
      m_bEnabled = TRUE;
   m_evEmpty.SetEvent();
   return;
   }

// COposRequestQueue::EnableRequests
//
// Purpose: Enable/Disable Requests, and ensure that the thread state matches
//          the enable state.
// Notes:   We cannot have the device locked through m_csAccess if we are 
//          terminating the thread, sunce the thread may not be able to quit
//          if we have the device locked!

void COposRequestQueue::EnableRequests(
   BOOL bEnable )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposRequestQueue::EnableRequests") );

   CSingleLock csAccess( &m_csAccess, TRUE );
   m_bEnabled = bEnable;
   if ( bEnable == FALSE )     
      {
      // Stop the thread if it is running
      if ( m_pServiceThread != NULL )
         {
         // Allow thread to stop running!
         csAccess.Unlock();
         EndService();
         }
      }
   else
      {
      // Start the thread if it is not running
      if ( m_pServiceThread == NULL )
         BeginService();
      m_evProcess.SetEvent(); // Start processing
      }
   return;
   }

// COposRequestQueue::RetryRequests
//
// Purpose: Retry a request that has had an error.  Should only be called in response to an error. 
// Note:    This may be called before the processing thread has detected the error, if the event
//          handler is fast enough.  m_bRetry makes sure that the processing thread is aware that
//          we have already okayed the retry.  


void COposRequestQueue::RetryRequests(
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposRequestQueue::EnableRequests") );
   CSingleLock csAccess( &m_csAccess, TRUE );
   ASSERT( m_pServiceThread != NULL ); // Should be running!
   m_bEnabled     = TRUE;
   m_bRetry       = TRUE;
   m_evProcess.SetEvent();
   return;
   }


// COposRequestQueue::IsBusy
//
// Purpose: 

BOOL COposRequestQueue::IsBusy( 
   void ) const
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   CDataCapEntryExit DCE( m_cmDc, _T("COposRequestQueue::IsBusy") );
   // We are busy if we have requests and are able to process them
   // or if we are processing a request ( may be the last request )
   return m_bProcessing != FALSE
      || (m_qList.IsEmpty() == FALSE && m_bEnabled != FALSE );
   }



