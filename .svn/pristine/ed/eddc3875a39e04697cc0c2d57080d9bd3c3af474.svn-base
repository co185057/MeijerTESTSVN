/////////////////////////////////////////////////////////////////////////////
//
// OposService.inl
//
// Inlines  of COposService class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifdef OPOS_INLINE

// COposService::~COposService
//
// Purpose: Destructor, Always called from the close event.  It is Ok to 
//          EndService now because the DataCapture passed to the queue
//          is still valid and the queue's thread can exit even though 
//          we are in an event handler. 
//          

OPOS_INLINE COposService::~COposService( 
   void ) 
   { 
   ASSERT( m_bOkToDelete != FALSE );
   ASSERT( m_bEventFired == FALSE );
   // Don't delete directly, use COposService::Delete!
   m_qEvents.EndService( ); // Dump all remaining events
   return;
   }


// COposService::UpdateEventState
//
// Purpose: Update event queue with control state
// Note:    At one point, I changed  m_bDataEventEnabled) to 
//          m_bDataEventEnabled && m_bDeviceEnabled) thinking that data events
//          should not be fired when the device is disabled. But this would
//          probably render AutoDisable useless since the application would 
//          be required to re-enable the device to get the data event, which
//          would allow still more input.

OPOS_INLINE void COposService::UpdateEventState( 
   void )
   {
   ASSERT( m_bOkToDelete == FALSE );
   // Must not change event state after ::Delete() is called!
   ASSERT( m_nState != OPOS_S_CLOSED );   // Must be open
   m_qEvents.EnableEvents( 
      m_bClaimed != FALSE           // Must Be Claimed
      && m_bFreezeEvents == FALSE   // Events Not Frozen By App
      && m_nCOFreezeEvents == 0,    // And Not Frozen By the Container
      m_bDataEventEnabled );        // Are Data Events Enabled?
   return;
   }

OPOS_INLINE void COposService::DispatchDirectIOEvent( 
   long     lEventNumber, 
   long*    plData, 
   CString& sData )
   {  
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::DispatchDirectIOEvent") );
	ASSERT( m_bEventFired == FALSE );
	m_bEventFired = TRUE;
   m_iCo.SODirectIO(lEventNumber, plData, sData); 
	ASSERT( m_bEventFired == TRUE );
	m_bEventFired = FALSE;
   return;
   }

OPOS_INLINE void COposService::DispatchStatusUpdateEvent( 
   long lStatus )
   {  
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::DispatchStatusEvent") );
   m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, _T("Status %ld"), lStatus );
	ASSERT( m_bEventFired == FALSE );
	m_bEventFired = TRUE;
   m_iCo.SOStatusUpdate( lStatus ); 
	ASSERT( m_bEventFired == TRUE );
	m_bEventFired = FALSE;
   return;
   }

OPOS_INLINE void COposService::DispatchOutputCompleteEvent( 
   long lOutputID )
   {  
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::DispatchOutputCompleteEvent") );
   m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, _T("OutputID %ld"), lOutputID );
	ASSERT( m_bEventFired == FALSE );
	m_bEventFired = TRUE;
   m_iCo.SOOutputComplete( lOutputID ); 
	ASSERT( m_bEventFired == TRUE );
	m_bEventFired = FALSE;
   return;
   }

OPOS_INLINE void COposService::SetDeviceName( 
   int nId )
   {
   CString sName;
   sName.LoadString( nId );
   int nNlIndex = sName.Find( _T('\n') ) ;
   ASSERT( nNlIndex != -1 );        // Must have newline
   m_sDeviceDescription = sName.Mid( nNlIndex + 1 );
   m_sDeviceName = sName.Left( nNlIndex );
   return;
   }

OPOS_INLINE void COposService::Enable( BOOL bEnable ) 
   { 
   ASSERT( m_pCheckHealth == NULL );
   m_bDeviceEnabled = bEnable; 
   return;
   }       


OPOS_INLINE BOOL COposService::AutoDisable( 
   void )
   { 
   BOOL bResult = FALSE;
   if ( m_bAutoDisable != FALSE 
      && m_pCheckHealth == NULL )
      {
      Enable( FALSE );  // Tell derived class to disable
      bResult = TRUE;
      }
   return bResult; 
   }       

OPOS_INLINE long COposService::SetRC( 
   long nRC    /* = OPOS_SUCCESS */, 
   long nRCEx  /* = 0            */)
   { 
   CSingleLock csAccess( &m_csAccess, TRUE );
   ASSERT( nRC != OPOS_E_EXTENDED || nRCEx != 0 );
   m_nResultCode = nRC; 
   m_nResultCodeExtended = nRCEx; 
   return nRC; 
   }

OPOS_INLINE void COposService::SetCheckHealthText( 
   int nId )
   { 
   m_sCheckHealthText.LoadString( nId ); 
   return;
   }

OPOS_INLINE void COposService::ClearOutput( 
   void )
   { 
   m_qEvents.ClearOutput();  
   ASSERT( m_nState == OPOS_S_ERROR || m_nState == OPOS_S_IDLE);
   m_nState = OPOS_S_IDLE;
   return;
   }

OPOS_INLINE void COposService::ClearInput( 
   void )
   { 
   m_qEvents.ClearInput(); 
   ASSERT( m_nState == OPOS_S_ERROR || m_nState == OPOS_S_IDLE);
   m_nState = OPOS_S_IDLE;
   return ;
   }

OPOS_INLINE void COposService::RetryOutput( 
   void ) 
   {       
   ASSERT( m_nState == OPOS_S_ERROR || m_nState == OPOS_S_IDLE);
   m_nState = OPOS_S_IDLE; 
   }

OPOS_INLINE long COposService::OutputOk(   
   void )
   {
   long lResult = OPOS_SUCCESS;

   if ( IsClaimed() == FALSE )
      lResult = OPOS_E_NOTCLAIMED;
   else if ( IsEnabled() == FALSE )
      lResult = OPOS_E_DISABLED;
   return lResult;
   }


OPOS_INLINE void COposService::AddRequest( 
   COposRequest* pRequest )
   {
   ASSERT( pRequest != NULL );
   ASSERT( pRequest->GetOutputId() > m_nOutputID );
   ASSERT( IsEnabled() != FALSE ); 
   m_nOutputID  = pRequest->GetOutputId(); 
   return ;
   }


OPOS_INLINE CString COposService::ConvertAsciiToBinary(
   const CString& sData ) const
   { 
   CString sResult = sData;
   ConvertOposAsciiToBinary( sResult, m_nBinaryConversion );
   return sResult;  
   }

OPOS_INLINE CString COposService::ConvertBinaryToAscii(
   const CString& sData ) const
   { 
   CString sResult = sData;
   ConvertBinaryToOposAscii( sResult, m_nBinaryConversion );
   return sResult;         
   }


#endif