/////////////////////////////////////////////////////////////////////////////
//
// OposEvent.cpp
//
// Implementation of COposEventQueue methods.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#include "stdafx.h"

#ifndef _OPOS_ENABLE_INLINES
#define OPOS_INLINE
#include <OposEvent.inl>
#endif 
#include "CMRegistry.h"

//////////////////////////////////////////////////////////////////

const LONG WMU_FIRE_EVENT = WM_USER + 100;

// COposEventQueue::COposEventQueue
//
// Purpose: Constructor


COposEventQueue::COposEventQueue( 
   void )
   : m_sClassName( AfxRegisterWndClass( 0 ) ), // Window has class, but no style!
   m_bEnableAny( FALSE ),
   m_bEnableData( FALSE ),
   m_nDataEvents( 0 ),
   m_pReceiver( NULL ),
   m_pDc( NULL )
   {
   }

// COposEventQueue::~COposEventQueue
//
// Purpose: Destructor

COposEventQueue::~COposEventQueue( 
   void )
   {
   ASSERT( m_qList.GetCount() == 0 );   // Call AbortAll() 
   return;
   }




// COposEventQueue::EnableEvents
//
// Purpose: Adjust the allowed events that may be fired.
// Notes:   If bEnableAny is FALSE, then no events may be fired. 
//          If bEnableData if FALSE, Data end Data Error events may 
//          not be fired. 
//          This function signals the thread to send a window message if:
//          1) Events are enabled.
//          2) Event Queue is not empty
//          3) All Events or Data Events flag is transitioning from FALSE to TRUE
//          Why? to minimize the number of unnecessary windows messages sent. 
//          

void COposEventQueue::EnableEvents( 
   BOOL bEnableAny,
   BOOL bEnableData)
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   CDataCapEntryExit DCE( m_pDc, _T("COposEventQueue::EnableEvents") );

   m_bEnableAny   = bEnableAny;
   m_bEnableData  = bEnableData;
   if ( m_pDc ) m_pDc->DCPrintf( TRACE_ALWAYS, _T("EnableEvents:  %d %d"), bEnableAny, bEnableData);

   /*
   if ( (bEnableAny || bEnableData) && !m_qList.IsEmpty() )
   {
       PostMessage(WMU_FIRE_EVENT, 0, 0 );
   }
   */

   if ( bEnableAny && !m_qList.IsEmpty() )
   {
	   COposEvent* pHead = m_qList.GetHead();
	   BOOL bNeedsData = pHead->NeedsDataEventEnabled();

	   if ( !bNeedsData || (bNeedsData && bEnableData) )
	   {
		   if ( m_pDc ) m_pDc->DCPrintf( TRACE_ALWAYS, _T("EnableEvents:  Posting message to fire events"));
		   PostMessage(WMU_FIRE_EVENT, 0, 0 );
	   }
   }
   return;
   }

// COposEventQueue::BeginService
//
// Purpose: Create the window, start the thread. If the thread cannot be created,
//          which is extremely unlikely, then, an exception is thrown.
//          This is why the window is created before the thread. 

void COposEventQueue::BeginService(
   COposEventReceiver*  pReceiver,
   CmDataCapture*       pDc )      
   {
   ASSERT( pReceiver != NULL );
   m_pDc = pDc;
   CDataCapEntryExit DCE( m_pDc, _T("COposEventQueue::BeginService") );
   CSingleLock csAccess( &m_csAccess, TRUE );

   ASSERT( m_pReceiver == NULL );
   BOOL bResult = CreateEx(
      0,                                  // Extended style.
      m_sClassName,                       // Class name.
      _T("FireEventWindow"),              // Window name.
      0,                                  // Window style,
      0, 0,                               // Initial x,y.
      0, 0,                               // Width, Height.
      NULL,                               // Parent.
      0,                                  // Menu ID.
      0);                                 // Params.
   if ( bResult == FALSE )       
      {
      static LPCTSTR sExText = _T("Failed to Create Event Window");
      pDc->DCPrintf( TRACE_ALWAYS, sExText );
      AfxMessageBox( sExText);
      AfxThrowOleDispatchException( 0x1234, sExText);
      }
   m_pReceiver = pReceiver;      // Must set up for End Service
   m_bEnableAny = m_bEnableData = FALSE;
   return ;
   }


// COposEventQueue::EndService
//
// Purpose: Make sure the service thread is not running.

void COposEventQueue::EndService(
   void )
   {
   CDataCapEntryExit DCE( m_pDc, _T("COposEventQueue::EndService") );
   ASSERT( m_pReceiver != NULL );
   CSingleLock csAccess( &m_csAccess, TRUE );
   AbortAll();
   DestroyWindow();
   m_pReceiver = NULL;
   m_pDc = NULL;
   return;
   }

// COposEventQueue::AddEvent
//
// Purpose: Add an event at the end of the queue.  
//          Signal the thread to send a message, if necessary.
// Note:    The windows message may be sent and received before we place the 
//          event into the queue, but the critical section keeps the 
//          OnFireEvent from sending it until we put it in the queue. 

void COposEventQueue::AddEvent( 
   COposEvent* pEvent,
   BOOL        bAddAtHead /* = FALSE */)
   {
   CSingleLock csAccess( &m_csAccess, TRUE );

   CDataCapEntryExit DCE( m_pDc, _T("COposEventQueue::AddEvent") );

    ASSERT( pEvent != NULL );
    if ( bAddAtHead == FALSE )
	{
        m_qList.AddTail( pEvent );
		if (m_pDc) m_pDc->DCPrintf( TRACE_ALWAYS, _T("AddEvent:  Added to Tail 0x%0X"), pEvent);
	}
    else
	{
        m_qList.AddHead( pEvent );
		if (m_pDc) m_pDc->DCPrintf( TRACE_ALWAYS, _T("AddEvent:  Added to Head 0x%0X"), pEvent);
	}
    if ( pEvent->IsDataEvent() != FALSE )
        m_nDataEvents++;

	
	/*if (m_bEnableAny || m_bEnableData)
	{
		if (m_pDc) m_pDc->DCPrintf( TRACE_ALWAYS, _T("AddEvent:  Posting message to fire event"));
		//PostMessage(WMU_FIRE_EVENT, bAddAtHead ? 1:0, (LPARAM)pEvent);
		PostMessage(WMU_FIRE_EVENT, 0, 0 );
	}*/

	if ( m_bEnableAny )
	{
		COposEvent* pHead = m_qList.GetHead();
		BOOL bIsData = pHead->IsDataEvent();
		BOOL bNeedsData = pHead->NeedsDataEventEnabled();
		if ( !bNeedsData || (bNeedsData && m_bEnableData) )
		{
			if ( !bIsData || (bIsData && (m_nDataEvents == 1)) )
			{
				if (m_pDc) m_pDc->DCPrintf( TRACE_ALWAYS, _T("AddEvent:  Posting message to fire event"));
				PostMessage(WMU_FIRE_EVENT, 0, 0 );
			}
			else
			{
				if (m_pDc) m_pDc->DCPrintf( TRACE_ALWAYS, _T("AddEvent:  %d Events Queued, wait for Enable"), m_nDataEvents);
			}

		}
	}

   return;
   }


// COposEventQueue::AddInputErrorEvent
//
// Purpose: This code encapsulates the Input error handling for the SO
//          Existing Data is bracketed by error events:
//          OPOS_EL_INPUT_DATA < existing Data  events > OPOS_EL_INPUT
//          If no data exists, then just the OPOS_EL_INPUT is added to the queue.

void COposEventQueue::AddInputErrorEvent( 
   long lRC, 
   long nRCEx )
   {
   CDataCapEntryExit DCE( m_pDc, _T("COposEventQueue::AddInputErrorEvent") );
   CSingleLock csAccess( &m_csAccess, TRUE );
// Determine lLocus for input based on whether data events are in the queue
   long lLocus = m_nDataEvents == 0 ? OPOS_EL_INPUT : OPOS_EL_INPUT_DATA;
   // Post Event Message for first event if Enabled
   // Since this is a data event, require m_bEnableData too!
   // Create first event, and add to head if Data exists
   COposEvent* pEvent = new COposErrorEvent( lRC, nRCEx, lLocus);
   if ( lLocus == OPOS_EL_INPUT_DATA )
      {
      m_qList.AddHead( pEvent );
      // Now create the tail event
      pEvent = new COposErrorEvent( lRC, nRCEx, OPOS_EL_INPUT);
      }
   AddEvent( pEvent );     // Add to the tail, set thread event
   return;
   }

// COposEventQueue::AbortAll
//
// Purpose: Discard all queued events. This occurs when the SO claim is release,
//          and when the queue is destroyed. 

void COposEventQueue::AbortAll( 
   void )
   {
   CDataCapEntryExit DCE( m_pDc, _T("COposEventQueue::AbortAll") );
   CSingleLock csAccess( &m_csAccess, TRUE );
   while ( m_qList.IsEmpty() == FALSE )
      {
      COposEvent* pEvent = m_qList.RemoveHead( );
      ASSERT( pEvent != NULL );
      delete pEvent;
      }
   m_nDataEvents = 0;
   return;
   }

// COposEventQueue::ClearDataEvents
// 
// Purpose: Clear data and input error events.  In short, anything that requires
//          that Data Event Enabled be true. 

void COposEventQueue::ClearInput( 
   void )
   {
   CDataCapEntryExit DCE( m_pDc, _T("COposEventQueue::ClearDataEvents") );
   CSingleLock csAccess( &m_csAccess, TRUE );
   for ( POSITION p = m_qList.GetHeadPosition( ); p != NULL; )
      {
      POSITION Target = p;          // Save p in target
      COposEvent* pEvent = m_qList.GetNext( p );
      ASSERT( pEvent != NULL );
      if ( pEvent->NeedsDataEventEnabled() != FALSE )
         {
         m_qList.RemoveAt( Target );
         delete pEvent;
         }
      }
   m_nDataEvents = 0;
   return;
   }


// COposEventQueue::ClearOutput
//
// Purpose: Clear Output Errors from the queue

void COposEventQueue::ClearOutput( 
   void )
   {
   CDataCapEntryExit DCE( m_pDc, _T("COposEventQueue::ClearOutputErrors") );
   CSingleLock csAccess( &m_csAccess, TRUE );
   for ( POSITION p = m_qList.GetHeadPosition( ); p != NULL; )
      {
      POSITION Target = p;          // Save p in target
      COposEvent* pEvent = m_qList.GetNext( p );
      ASSERT( pEvent != NULL );
      if ( pEvent->IsOutputEvent() != FALSE )
         {
         m_qList.RemoveAt( Target );
         delete pEvent;
         }
      }
   return;
   }


/////////////////////////////////////////////////////////////////////////////
// COposEventQueue Window Information

BEGIN_MESSAGE_MAP(COposEventQueue, CWnd)
    //{{AFX_MSG_MAP(COposEventQueue)
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
    ON_MESSAGE(WMU_FIRE_EVENT, OnFireEvent)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// COposEventQueue message handlers

// COposEventQueue::OnFireEvent
// Validate that events may be fired, and if the specific event may be fired.
// Data and Input error events require m_bEnableData to be TRUE 
// Note:    1) Can't DataCap Entry/Exit Here using standard mechanism
//          Because the DataCap will be deleted in the final event
//          that is handled. Therefore, we trace entry, but not exit!
//          2) The semAuto semaphore applies to all OPOS
//          service objects in the application ( using this framework )
//          It keeps service objects from re-entering the application
//          when the application handles an event by putting up a 
//          dialog or message box.  Windows continues to call this 
//          function from the dialog box procedure and will force 
//          re-entry into the application code. We stop this by checking
//          this flag.  We do not care which service object has entered
//          the code, just that one has.  All service objects retry
//          periodically to dispatch the event. See the thread code above!

afx_msg LONG COposEventQueue::OnFireEvent(
   UINT uVal, 
   LONG lVal)
{
    CSingleLock csAccess( &m_csAccess, TRUE );
    CDataCapEntryExit DCE( m_pDc, _T("COposEventQueue::OnFireEvent") );
    if (m_pDc) m_pDc->DCPrintf( TRACE_ALWAYS, _T("OnFireEvent: Event is 0x%0X"), lVal );
    COposEvent* pNewEvent = (COposEvent*)lVal;
    bool bAddAtHead = (uVal != 0);

    if ( (NULL == pNewEvent) &&
        (m_qList.IsEmpty()) )
    {
        if( m_pDc ) m_pDc->DCPrintf( TRACE_ALWAYS, _T("OnFireEvent:  No Events to fire") );
        return 0;
    }

    /*if ( pNewEvent )
    {
        ASSERT( pNewEvent != NULL );
        if ( bAddAtHead == FALSE )
            m_qList.AddTail( pNewEvent );
        else
            m_qList.AddHead( pNewEvent );
        if ( pNewEvent->IsDataEvent() != FALSE )
            m_nDataEvents++;
    }*/

    // If no events or events not legal then return without firing.
    if ( (!m_qList.IsEmpty()) 
      && (m_bEnableAny != FALSE) )
    {
        COposEvent* pTempEvent = m_qList.GetHead( );
        ASSERT( pTempEvent != NULL );        // Check the first event
        if ( m_bEnableData == FALSE      // If cannot fire now, then re-queue it
            && pTempEvent->NeedsDataEventEnabled() != FALSE )
        {
            if (m_pDc) m_pDc->DCPrintf( TRACE_ALWAYS, _T("OnFireEvent:  Can't fire Event 0x%0X %d %d"),
                pTempEvent, m_bEnableData, pTempEvent->NeedsDataEventEnabled() );
            //m_qList.AddHead( pEvent );    
        }
        else                             
        {
			COposEvent* pEvent = m_qList.RemoveHead( );
			ASSERT( pEvent != NULL );        // Check the first event
            DCE.NoTraceExit();
            if ( pEvent->IsDataEvent() != FALSE )
                m_nDataEvents--;
            ASSERT( m_nDataEvents >= 0 );
            if (m_pDc) m_pDc->DCPrintf( TRACE_ALWAYS, _T("OnFireEvent:  Dispatching the Event 0x%0X"), pEvent );
			if ( pEvent->IsDataEvent() )
			{
				m_bEnableData = FALSE;
			}
            csAccess.Unlock();               // Allow more events to be added
            pEvent->Dispatch( *m_pReceiver );  
            delete pEvent;                
        }
    }
    else
    {
        if (m_pDc) m_pDc->DCPrintf( TRACE_ALWAYS, _T("No Events or events not legal %d %d"),
        m_qList.IsEmpty(), m_bEnableAny );
    }

    return 0;
}

