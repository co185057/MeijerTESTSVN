/////////////////////////////////////////////////////////////////////////////
//
// OposEvent.inl
//
// Inlines of OposEvent.h.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifdef OPOS_INLINE

OPOS_INLINE void COposDataEvent::Dispatch( 
   COposEventReceiver& pTarget )
   { 
//original   pTarget.DispatchDataEvent( m_sData, m_nResult );
//antonio start
	 pTarget.DispatchDataEvent( (LPCTSTR)m_sData, m_nResult );
//antonio end
   return;
   }

OPOS_INLINE void COposStatusUpdateEvent::Dispatch( 
   COposEventReceiver& pTarget )
   {  
   pTarget.DispatchStatusUpdateEvent( m_nResult ); 
   return;
   }

OPOS_INLINE void COposErrorEvent::Dispatch( 
   COposEventReceiver& pTarget )
   { 
   pTarget.DispatchErrorEvent( m_nRC, m_nRCEx, m_nLocus );  
   return;
   }

OPOS_INLINE void COposOutputCompleteEvent::Dispatch( 
   COposEventReceiver& pTarget )
   { 
   pTarget.DispatchOutputCompleteEvent( m_nOutputID );  
   return;
   }

OPOS_INLINE void COposDirectIOEvent::Dispatch( 
   COposEventReceiver& pTarget )
   { 
   // Pass addressed of long and string data, allow container to change them!
   pTarget.DispatchDirectIOEvent( m_nEventNumber, &m_nData, m_sData );  
   return;
   }



// GetDataCount
// Purpose: Return number of data events in the queue, but not data errors

OPOS_INLINE long COposEventQueue::GetDataCount( 
   void )
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   return m_nDataEvents;    // Count Data Events in the queue
   }


// static COposEventQueue::CreateEventSemaphoreName
//
// Purpose: Create the name of a system Semaphore for the process to 
//          serialize Opos Events for one or more service objects.

OPOS_INLINE CString COposEventQueue::CreateEventSemaphoreName( 
   void ) 
   {
   CString sResult;
   sResult.Format( _T("OposEventSem%lx"), ::GetCurrentProcessId() );
   return sResult;
   }


#endif