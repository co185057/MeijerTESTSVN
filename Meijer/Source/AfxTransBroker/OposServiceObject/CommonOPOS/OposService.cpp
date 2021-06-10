/////////////////////////////////////////////////////////////////////////////
//
// OposService.cpp
//
//   Implementation of COposService methods.
//
//   Copyright (c) 1997 NCR.  All rights reserved.
//
//   Date                   Modification                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#include "stdafx.h"


#ifndef _OPOS_ENABLE_INLINES
#define OPOS_INLINE
#include <OposService.inl>
#endif 

static LPCTSTR g_szErrorProperty = _T("[Error]");
#define RED_HERRING  0x80000000


// Data captuure property by name if in debug!

#ifdef _DEBUG 
static LPCTSTR g_szCommonPropNumNames[ ] =
   {
   _T("PIDX_Claimed"),
   _T("PIDX_DataEventEnabled"),
   _T("PIDX_DeviceEnabled"), 
   _T("PIDX_FreezeEvents"),
   _T("PIDX_OutputID"),
   _T("PIDX_ResultCode"),
   _T("PIDX_ResultCodeExtended"),
   _T("PIDX_ServiceObjectVersion"),
   _T("PIDX_State"),
   _T("PIDX_AutoDisable"),
   _T("PIDX_BinaryConversion"),
   _T("PIDX_DataCount")
   };
static LPCTSTR g_szCommonPropStringNames[ ] =
   {
   _T("PIDX_CheckHealthText"),
   _T("PIDX_DeviceDescription"),
   _T("PIDX_DeviceName"),
   _T("PIDX_ServiceObjectDescription")
   };
#endif


BOOL COposService::DcapProperty( 
   long     lPropIndex,
   long     lPidxBase,     // NUMBER or String
   LPCTSTR  szNames[],
   DWORD    nCountNames)
   {
   BOOL bResult = FALSE;
   if (lPropIndex > lPidxBase && (DWORD)(lPropIndex - lPidxBase - 1) < nCountNames )
      {
      ASSERT( szNames != NULL || nCountNames == 0);
      CString sText;
#ifdef _DEBUG
      ASSERT( szNames != NULL );
      lPropIndex -= lPidxBase + 1;
      ASSERT( szNames[ lPropIndex ] != NULL );
      sText = szNames[ lPropIndex ];
#else
      sText.Format( _T("Index: %ld"), lPropIndex );
#endif
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, _T("Property: %s"), (LPCTSTR)sText );
      bResult = TRUE;
      }
   return bResult;
   }

// COposService::COposService
//
// Purpose: Construct the service object, Set the default DC Prefix

COposService::COposService(
   LPCTSTR  sPrefix )
   : COposEventReceiver( ),
   m_nState( OPOS_S_CLOSED ),
   m_nBinaryConversion( RED_HERRING ),
   m_sPrefix( sPrefix ),
   m_pCheckHealth( NULL ),
   m_bOkToDelete( FALSE ),
	m_bEventFired( FALSE )
   {
   m_cmDc.SetPrefix( m_sPrefix );
   m_cmDc.SetCaptureControl( NCRCAP_OFF );
   }


void COposService::InitializeProperties(
   void )
   {
   ASSERT( m_nState == OPOS_S_CLOSED);
   m_nResultCode = OPOS_E_CLOSED;
   m_bDataEventEnabled = m_bAutoDisable = m_bDeviceEnabled =
      m_bFreezeEvents = m_bClaimed = FALSE;
   m_nResultCodeExtended = m_nCOFreezeEvents = 
      m_nOutputID = m_nSoVersion = 0;
   m_sCheckHealthText = m_sDeviceDescription =
      m_sDeviceName = m_sSoDescription = m_sSoVersion = g_szErrorProperty;
   // Kill the herring
   m_nBinaryConversion = OPOS_BC_NONE;
   return;
   }


// COposService::GetPropertyNumber
//
// Purpose: Get common number property by OPOS index ( PIDX ).

long COposService::GetPropertyNumber( 
   long lPropIndex ) 
   {
   long lResult = 0;

   CDataCapEntryExit DCE( m_cmDc, _T("COposService::GetPropertyNumber") );
   DCAPPROPERTY( lPropIndex, PIDX_NUMBER, g_szCommonPropNumNames, 
      ARRAY_ELEMENT_COUNT( g_szCommonPropNumNames ) );
   CSingleLock csAccess( &m_csAccess, TRUE );
   ASSERT( m_pCheckHealth == NULL );
   ASSERT(m_nState != OPOS_S_CLOSED);
   switch( lPropIndex )
      {
      case PIDX_State:                
         lResult = m_nState;
         if ( m_nState == OPOS_S_IDLE
            && IsBusy() != FALSE )
            lResult = OPOS_S_BUSY;
         break;
      case PIDX_ResultCode:           
         lResult = m_nResultCode;
         break;
      case PIDX_Claimed:              
         lResult = m_bClaimed;
         break;      
      case PIDX_DataEventEnabled:     
         lResult = m_bDataEventEnabled;
         break;      
      case PIDX_DeviceEnabled:        
         lResult = m_bDeviceEnabled;
         break;    
      case PIDX_DataCount:
         lResult = m_qEvents.GetDataCount(); 
         break;      
      case PIDX_BinaryConversion:
         lResult = m_nBinaryConversion;
         break;      
      case PIDX_AutoDisable:
         lResult = m_bAutoDisable;
         break;      
      case PIDX_FreezeEvents:         
         lResult = m_bFreezeEvents;
         break;      
      case PIDX_ResultCodeExtended:   
         lResult = m_nResultCodeExtended;
         break;      
      case PIDX_OutputID:
         lResult = m_nOutputID;
         break;
      case PIDX_ServiceObjectVersion:
         lResult = m_nSoVersion;
         break;
      default:
         m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, 
            _T("Bad Index %d"), lPropIndex );
         break;
      }
   return lResult;
   }


// COposService::GetPropertyString
//
// Purpose: Get common string property by OPOS index ( PIDX ).

CString COposService::GetPropertyString( 
   long lPropIndex ) 
   {
   CString sResult;
   CSingleLock csAccess( &m_csAccess, TRUE );
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::GetPropertyString") );
   DCAPPROPERTY( lPropIndex, PIDX_STRING, g_szCommonPropStringNames, 
      ARRAY_ELEMENT_COUNT( g_szCommonPropStringNames ) );

   ASSERT( m_pCheckHealth == NULL );
   ASSERT(m_nState != OPOS_S_CLOSED);
   switch( lPropIndex )
      {
      case PIDX_CheckHealthText:
         sResult = m_sCheckHealthText;
         break;
      case PIDX_DeviceDescription:        
         sResult = m_sDeviceDescription;
         break;
      case PIDX_DeviceName:               
         sResult = m_sDeviceName;
         break;
      case PIDX_ServiceObjectDescription:
         sResult = m_sSoDescription;
         break;
      default:
         m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, 
            _T("Bad Index %d"), lPropIndex );
         sResult = g_szErrorProperty;
      }
   m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, 
      _T("PropertyIndex %d, Value <%s>"), lPropIndex, (LPCTSTR)sResult );
   return sResult;
   }


// COposService::SetPropertyNumber
//
// Purpose: Set common number property by OPOS index ( PIDX ).

void COposService::SetPropertyNumber(
   long lPropIndex, 
   long lProperty)
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   ASSERT( m_pCheckHealth == NULL );
   ASSERT( lPropIndex != PIDX_DeviceEnabled  ); // Go through device 
   ASSERT(m_nState != OPOS_S_CLOSED);
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::SetPropertyNumber") );
   DCAPPROPERTY( lPropIndex, PIDX_NUMBER, g_szCommonPropNumNames, 
      ARRAY_ELEMENT_COUNT( g_szCommonPropNumNames ) );
   SetRC( OPOS_SUCCESS );
   switch ( lPropIndex )
      {
      case PIDX_DataEventEnabled:
         m_bDataEventEnabled = lProperty;
         break;
      case PIDX_BinaryConversion:
         if ( lProperty == OPOS_BC_NONE   
            || lProperty == OPOS_BC_NIBBLE 
            || lProperty == OPOS_BC_DECIMAL)
            m_nBinaryConversion = lProperty;
         else
            SetRC( OPOS_E_ILLEGAL );
         break;      
      case PIDX_AutoDisable:
         m_bAutoDisable = lProperty;
         break;      
      case PIDX_FreezeEvents:
         m_bFreezeEvents = lProperty;
         break;
      default:
         m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, 
            _T("Bad Index %d"), lPropIndex );
         SetRC( OPOS_E_ILLEGAL );
      }
   UpdateEventState();
   return;
   }

// COposService::SetPropertyString
//
// Purpose: Set common string property by OPOS index ( PIDX ).

void COposService::SetPropertyString(
   long lPropIndex, 
   LPCTSTR sProperty)
   {
   ASSERT( m_pCheckHealth == NULL );
   //CDataCapEntryExit DCE( m_cmDc, _T("COposService::SetPropertyString") );
   m_cmDc.DCPrintf( 0x01, _T("+-COposService::SetPropertyString(%d,%s)"), lPropIndex, sProperty );

   DCAPPROPERTY( lPropIndex, PIDX_STRING, g_szCommonPropStringNames, 
      ARRAY_ELEMENT_COUNT( g_szCommonPropStringNames ) );
   m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, 
      _T("Bad Index %d, Value <%s>"), lPropIndex, sProperty );
   SetRC( OPOS_E_ILLEGAL );
   return;
   }


// COposService::COFreezeEvents
//
// Purpose: Update m_nCOFreezeEvents
// Notes:   Apparently, the code is written this way to cover the case
//          where the control is non-NCR.  NCR controls only call
//          COFreezeEvents when the first freeze occurs, and then counts
//          any additional freezes at the control level without calling
//          COFreezeEvents.  Events are unfrozen when the control freeze
//          count goes to zero.  For NCR controls, m_nCOFreezeEvents can only
//          is either 0 or 1.  Apparently, there is concern that the 
//          non NCR control might not inform the SO of every freeze 
//          ( those before the ::Open perhaps ) and that the count 
//          might be decremented too many times! 

void COposService::COFreezeEvents( 
   BOOL bFreeze )
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   ASSERT( m_pCheckHealth == NULL );

   CDataCapEntryExit DCE( m_cmDc, _T("COposService::COFreezeEvents") );
   if ( bFreeze != FALSE )
      m_nCOFreezeEvents++;
   else if ( m_nCOFreezeEvents != 0 )  
      m_nCOFreezeEvents--;
   UpdateEventState();
   return;
   }

// COposService::Open
//
// Purpose: Open the SO.


BOOL COposService::Open( 
   LPDISPATCH     pDispatch,
   COposRegistry&  oRegistry,  
   const CString& sSoDescription )
   { 
   ASSERT( m_nBinaryConversion != RED_HERRING );
   // If you hit this asertion, then you need to call 
   // InitializeProperties from your derived class InitializeProperties!!! 
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::Open") );
   ASSERT( m_pCheckHealth == NULL );
   ASSERT( m_nState == OPOS_S_CLOSED );
   int nNlIndex = sSoDescription.Find( _T('\n') ) ;
   ASSERT( nNlIndex != -1 );   // Must have newline
   m_nSoVersion = _ttol( sSoDescription.Mid(nNlIndex + 1));

   // TB Emulation is allowed to override the SO version from the registry
   CString sRegSOVersion;
   oRegistry.ReadString(_T("Version"), sRegSOVersion, 0, _T("") );
   if ( !sRegSOVersion.IsEmpty() )
   {
	   // Convert Registry Value A.B.C to 
	   // (A*1000000) + (B*1000) + C
	   int nVersion = 0;
	   int nDec = sRegSOVersion.Find(_T("."));
	   if ( nDec != -1 )
	   {
		   nVersion = 1000000 * _ttol(sRegSOVersion.Left(nDec));
		   sRegSOVersion = sRegSOVersion.Mid(nDec + 1);

		   nDec = sRegSOVersion.Find(_T("."));
		   if ( nDec != -1)
		   {
			   nVersion += (1000 * _ttol(sRegSOVersion.Left(nDec)));
			   sRegSOVersion = sRegSOVersion.Mid(nDec + 1);
		   
			   nVersion += _ttol(sRegSOVersion);
			   m_nSoVersion = nVersion;
		   }
	   }
   }
   
   // Always write Description and version to the registry. 
   m_sSoDescription = sSoDescription.Left( nNlIndex );
   // Set Description and Version into registry.
   m_sSoVersion = oRegistry.WriteSoDescription( m_sSoDescription, m_nSoVersion ); 
   BOOL bResult = FALSE;
   if ( ReadRegistry( oRegistry ) != FALSE )
      bResult = m_iCo.Connect( pDispatch, m_cmDc );
   // FALSE if CO Version and Description Not Available
   if ( bResult != FALSE )
      {
      m_cmDc.DCPrintf( TRACE_ALWAYS, 
         _T("Control Object <%s>, Version %ld"),
         (LPCTSTR) m_iCo.GetControlObjectDescription(),
         m_iCo.GetControlObjectVersion() );
      m_nState = OPOS_S_IDLE;
      m_sCheckHealthText.Empty();
      SetRC();
      }
   return bResult;
   }

// COposService::Close
//
// Purpose: Close the SO if it is open.


long COposService::Close(
   void )
   {
   long lResult = OPOS_E_CLOSED;
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::Close") );
   ASSERT( m_pCheckHealth == NULL );
   if ( m_nState != OPOS_S_CLOSED )
      {
      ASSERT( m_bClaimed == FALSE );   // Device should be closed first, 
      m_iCo.Disconnect();
      m_nState = OPOS_S_CLOSED;
      lResult = OPOS_SUCCESS;
      }
   return SetRC( lResult );
   }


// COposService::Claim
//
// Purpose: Mark that the device has been successfully claimed and 
//          update event state.


void COposService::Claim( 
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::Claim") );
   ASSERT( m_bClaimed == FALSE );   // Device should stop this
   ASSERT( m_pCheckHealth == NULL );
   // This may throw, so do it first
   m_qEvents.BeginService( this, &m_cmDc );
   m_bClaimed = TRUE;
   m_nOutputID = 0;        // Reset the output Id when claimed!
   UpdateEventState();
   return ;
   }


// COposService::ReleaseClaim
//
// Purpose: Mark that the device has been successfully released and 
//          update event state.
// Note:    Pending events are discarded. 

void COposService::ReleaseClaim( 
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::ReleaseClaim") );
   ASSERT( m_pCheckHealth == NULL );
   ASSERT( m_bClaimed != FALSE );   // Device should stop this
   // Call derived ClearInput to clear any Data properties.
   ClearInput();
   m_qEvents.EndService();
   m_bClaimed = FALSE;
   m_nOutputID = 0;
   return ;
   }

//////////////////////////////////////////////////////////////////////////////
// Event Related Functions
//
// COposService::AddEvent
//
// Purpose: Add an event to the event queue or health check queue

void COposService::AddEvent( 
   COposEvent* pEvent,
   BOOL        bAddAtHead /* = FALSE */)
   {
	// NO need to lock this, the queue is protected.
   //CSingleLock csAccess( &m_csAccess, TRUE );
   ASSERT( m_nState != OPOS_S_CLOSED );
   ASSERT( IsClaimed() != FALSE );

//   CDataCapEntryExit DCE( m_cmDc, _T("COposService::AddEvent") );
   if ( m_pCheckHealth == NULL )
      m_qEvents.AddEvent( pEvent, bAddAtHead ); 
   else
      {
      ASSERT( bAddAtHead == FALSE );
      m_pCheckHealth->AddEvent( pEvent );
      }
   }

// COposService::AddInputErrorEvent
//
// Purpose: Add input error event to the event queue or health check queue

void COposService::AddInputErrorEvent( 
   long lRC, 
   long nRCEx )
   { 
   CSingleLock csAccess( &m_csAccess, TRUE );
   ASSERT( m_nState != OPOS_S_CLOSED );
   ASSERT( IsClaimed() != FALSE );
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::AddInputErrorEvent") );
   if ( m_pCheckHealth == NULL )
      m_qEvents.AddInputErrorEvent( lRC, nRCEx ); 
   else
      m_pCheckHealth->AddInputErrorEvent( lRC, nRCEx ); 
   return;
   }

// Event Dispatch Functions:
//
// COposService::DispatchErrorEvent
//
// Purpose: Dispatch an error event and process the response.
//
// Notes:   1) It is imperative that the Critical Section csAccess be 
//          unlocked prior to calling the SOError function on the CO. 
//          This is a deadlock case that is common to callback functions.
//          For example, If this thread has the SO locked, and the 
//          application re-enters the SO within the event handler, and
//          accesses a device api, which needs to update the SO, 
//          then if a device thread also tries to update the SO
//          the device thread could own the device CS and be waiting on
//          this thread to finish, but since this thread is trying to wait 
//          on the device, a deadlock occurs.  This could be avoided using
//          a mutex, a short timeout, and a little polling loop.  But that 
//          would be more bogus than a callback function!
//          2) Derived classes must release the critical section before
//          calling the base class. 
//          3) ClearOutput was made virtual instead of returning
//          nResponse to the caller. We expect derived classes to 
//          to flush output queues, then call the base class.

void COposService::DispatchErrorEvent( 
   long nResult, 
   long nRCEx,
   long lLocus )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::DispatchErrorEvent") );
   ASSERT( m_pCheckHealth == NULL );   // Must not be in health check dlg
   ASSERT( m_nState != OPOS_S_CLOSED );
   // Must be output error or if input error, must be enabled
   ASSERT( lLocus == OPOS_EL_OUTPUT || m_bDataEventEnabled != FALSE );
   ASSERT( lLocus == OPOS_EL_OUTPUT // should handle non-zero context and 
      || lLocus == OPOS_EL_INPUT_DATA  // pass zero.
      || lLocus == OPOS_EL_INPUT ); // Only Valid OPOS Loci
   // Set up default response based on the locus
   long nResponse = OPOS_ER_CLEAR;  
   if ( lLocus == OPOS_EL_OUTPUT )
      nResponse = OPOS_ER_RETRY;   
   else if ( lLocus == OPOS_EL_INPUT_DATA )
      nResponse = OPOS_ER_CONTINUEINPUT;
   m_nState = OPOS_S_ERROR;         // Place SO into Error State
	ASSERT( m_bEventFired == FALSE );
	m_bEventFired = TRUE;
   m_iCo.SOError( nResult, nRCEx, lLocus, &nResponse );
	ASSERT( m_bEventFired == TRUE );
	m_bEventFired = FALSE;
   // Analyze the user's response
   ASSERT( nResponse == OPOS_ER_CLEAR  // This is an application test!
      || nResponse == OPOS_ER_RETRY
      || nResponse == OPOS_ER_CONTINUEINPUT );
   switch ( lLocus )
      {
      case OPOS_EL_OUTPUT:
         if ( nResponse == OPOS_ER_CLEAR ) 
            ClearOutput();   // Derived class should delete output
         else
            RetryOutput();   // Let derived class retry
         break;
      case OPOS_EL_INPUT:
         m_nState = OPOS_S_IDLE; 
      case OPOS_EL_INPUT_DATA: // Flow through intended
         if ( nResponse == OPOS_ER_CLEAR ) 
            ClearInput();    
         break;
      }
   return ;
   }


// COposService::DispatchDataEvent
//
// Purpose: Dispatch a data event.
// Note:    Synchronization should not be necessary since we are presumably
//          on the main thread, but it does not hurt. 
/* original
void COposService::DispatchDataEvent( 
   const CString& sData,   
   long lStatus )  */
//antonio start
void COposService::DispatchDataEvent( 
   LPCTSTR sData,   
   long lStatus )
//antonio end
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::DispatchDataEvent") );
// Need CS protection on m_bDataEventEnabled flag, UpdateEventState();
   ASSERT( m_nState != OPOS_S_CLOSED );
   //ASSERT( m_bDataEventEnabled != FALSE );
   ASSERT( m_pCheckHealth == NULL );   // Must not be in health check dlg
   m_bDataEventEnabled = FALSE;
   UpdateEventState();
// Now,  dispatch it to the CO
	ASSERT( m_bEventFired == FALSE );
	m_bEventFired = TRUE;
   m_iCo.SOData( lStatus );
	ASSERT( m_bEventFired == TRUE );
	m_bEventFired = FALSE;
   return ;
   }

// COposService::InteractiveHealthCheck
//
// Purpose: Do a HealthCheck.

long COposService::InteractiveHealthCheck( 
   COposCheckHealth* pCheckHealth )
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::InteractiveHealthCheck") );
   ASSERT( m_pCheckHealth == NULL );
   ASSERT( pCheckHealth != NULL );
   m_pCheckHealth = pCheckHealth;
   csAccess.Unlock();
   long lResult = m_pCheckHealth->DoModal(
      m_sDeviceName, m_sDeviceDescription, m_sSoDescription, m_sSoVersion );
   csAccess.Lock();
   m_pCheckHealth = NULL;
   return lResult;
   }


//////////////////////////////////////////////////////////////////////////////
// Data Capture Support Functions:
//
// COposService::StartDataCapture
//
// Purpose:    Capture Arguments to ::OpenService

void COposService::StartDataCapture( 
   LPCTSTR sName )
   {
   CString sDeviceKey = CString( OPOS_ROOTKEY _T("\\") ) + GetServiceObjectClassName() + _T("\\") + sName;
   m_cmDc.ReadRegistry( sDeviceKey );
   m_cmDc.DCPrintf( TRACE_ALWAYS,  _T("Class <%s>, Name <%s>"), GetServiceObjectClassName(), sName );
   return;
   }

// COposService::SetDcPrefix
//
// Purpose: Set Data Capture Prefix.

void COposService::SetDcPrefix(
   LPCTSTR sTitle, 
   int nPort, 
   int nId )
   {
   CString sPrefix;
   sPrefix.Format( _T("%s%02ld-%02d"), sTitle, nPort, nId );
   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("*Switching to Prefix: \"%s\""), 
      (LPCTSTR) sPrefix );
   m_cmDc.Flush();
   m_cmDc.SetPrefix( sPrefix );
   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("*Switching from Prefix: \"%s\""),
      (LPCTSTR) m_sPrefix );
   m_sPrefix = sPrefix;
   return;
   }

//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
// The following mechanism is used to delete the service object.
// Although the MFC Service Object class creates the service object
// ( derived from COposService ), it does not delete it using the delete 
// operator.  Instead, it should call delete to avoid one of the 
//          "problems with callbacks"
// 
// The problem occurs when an event is dispatched to the application, 
// within which, the application closes the Service Object.  This is normally
// not a problem, as long as all the participants do not touch their
// member variables after the event's Dispatch function returns.  This is 
// the point of m_iCo.SOxxxxx call.  The COposEventQueue::OnFireEvent
// works correctly in that it does not access its own member variables
// after the Event is dispatched.  We can almost do this, but 
// the definition of m_iCo.SOError requires that we (potentially) clear
// some of our queues, and change our state after the call returns.
// This class is our solution. 
// When we are closed, we abort all events from the queue, and disable 
// events.  Then when the MFC SO is deleted, he should call our Delete()
// method.  This causes us to abort events ( just to be sure ), re-enable
// events, and add an event that will delete us. This event is not dispatched 
// until the application gets back to its main message loop.  This seems to 
// work even if the application itself is closing. When that message is 
// finally dispatched, we are deleted. Then the COposEventQueue::OnFireEvent
// deletes the event and all is properly cleaned up.  Note, however, that
// Data Capture is no longer valid once COposService::Delete is called. 

class COposCloseEvent : public COposEvent
   {
   virtual void Dispatch( COposEventReceiver& rTarget)
      {    delete &rTarget; }
   };

void COposService::Delete( 
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposService::Delete") );
   ASSERT( m_nState == OPOS_S_CLOSED );   // Close before ::Delete
	ASSERT( m_bOkToDelete == FALSE );
	m_bOkToDelete = TRUE;
	m_qEvents.BeginService( this, &m_cmDc );
	m_qEvents.EnableEvents( TRUE, TRUE );
	if ( m_bEventFired != FALSE )
		m_qEvents.AddEvent( new COposCloseEvent( ) ); 
	else
		{
		DCE.NoTraceExit();
		delete this;
		}
   return;
   }


// COposService::CheckMethodValid
// 
// Purpose: Determine method/property set/get validity
// Note:    Result codes not set because result code should not change for property reads

long COposService::CheckMethodValid( 
   int nRequirements /* = OPOS_REQUIRE_OPENED */) const 
   {
   long lResult = OPOS_SUCCESS;
   if ( m_nState == OPOS_S_CLOSED )
      lResult = OPOS_E_CLOSED;
   else if ( ( nRequirements & OPOS_REQUIRE_CLAIMED ) != 0 
      && m_bClaimed == FALSE )
      lResult = OPOS_E_NOTCLAIMED;
   else if ( ( nRequirements & OPOS_REQUIRE_ENABLED ) != 0 )
      {
      if ( IsExclusive() != FALSE 
         && m_bClaimed == FALSE )
         lResult = OPOS_E_NOTCLAIMED;
      else if ( m_bDeviceEnabled == FALSE )
         lResult = OPOS_E_DISABLED;
      }
   return lResult;
   }
