// OposDevice.cpp: implementation of the Base Device class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "OposDevice.h"

// FormatResourceName()
//
// Purpose: local fn used by the constructor to pull create unique resource 
// names based on a format string and a numeric identifier.
//
// This was added for TAR 193039 (ECK 20020205) since after enough creations
// in the original code the name would contain a character that's not valid
// for use within a kernel resource name. ('\')  MFC would then throw a 
// resource exception from within the mutex constructor.

static CString FormatResourceName(CString sFormat, int iIdentifier)
   {
   CString sName;
   sName.Format(sFormat, iIdentifier);
   return sName;
   }

// COposDevice::COposDevice
//
// Purpose: Base constructor for all OPOS device class classes. This is just the 
//          "core" of all the devices I have ever done. The data capture reference is from 
//          the service class, the longest living object. 

COposDevice::COposDevice(
   LPCTSTR        sClass,
   LPCTSTR        sEventSource,
   int            idsName,    // from the resource: "device name\nDescription"
   int            nShareKey,  // Sharing key, for claim sem name
   int            nFirstTally,
   int            nLastTally,
   int            idsSoDescription,
   int            nSmInvalidDirectIO,
   CmDataCapture& cmDc )         
   : m_nDeviceId( idsName ),
   m_ShareArea( sClass, cmDc ),
   m_nShareKey( nShareKey ),
   m_nFirstTally( nFirstTally + 1),
   m_nLastTally( nLastTally - 1 ),
   m_mtxClaim( FALSE, 
      FormatResourceName(CString(_T("NCR") ) + sClass +_T("SOClaimMtx%d"), nShareKey)),
   m_cmDc( cmDc ),
   m_pService( NULL ),
   m_bDeviceEnabled( FALSE ),
   m_bFailed( FALSE ),
   m_bClaimed( FALSE ),
   m_pCheckHealth( NULL ),
   m_idsCheckHealthText( 0 ),
   m_oDiagnostics( sEventSource, idsSoDescription ),
   m_nSmInvalidDirectIO( nSmInvalidDirectIO )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposDevice::COposDevice") );
   }

// COposDevice::Open
//
// Purpose: Common Open code for all devices.

long COposDevice::Open(
   COposService*  pService,
   COposRegistry& oRegistry )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposDevice::Open") );
   ASSERT( pService != NULL );
   // Does anybody have this device open?  This shouldn't happen.
   // 1) TService catches this if the CO opens us twice.
   // 2) TService does not allow a second CO to open us. No sharing of this class. 
   ASSERT( IsOpen() == FALSE );
   long lResult = OPOS_SUCCESS;
   // Set diagnostic profile for event logging throughout
   m_oDiagnostics.SetProfile( oRegistry.GetName() );
   // Allow derived class to read the registry, initialize any other classes they have
   if ( ReadRegistry( oRegistry ) == FALSE 
      || m_ShareArea.Open( m_nShareKey ) == FALSE )
      lResult = OPOS_E_FAILURE;
   else
      {
      m_pService = pService;
      pService->SetDeviceName( m_nDeviceId );
      }
   return pService->SetRC( lResult );
   }


// COposDevice::Close
//
// Purpose: Clean up device.

void COposDevice::Close(
   COposService*  pService )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposDevice::Close") );
   ASSERT( pService == m_pService );
   ASSERT( IsClaimed() == FALSE );
   // Allow derived class to close any open resources.
   OnDeviceClosed();
   m_ShareArea.Close( );
   return;
   }

// COposDevice::Claim
//
// Purpose: Common claim process.

long COposDevice::Claim(
   long lTimeout)
   {
   long lResult = OPOS_E_TIMEOUT;
   CDataCapEntryExit DCE( m_cmDc, _T("COposDevice::Claim") );

   // TService template makes sure we are not called twice.
   ASSERT( m_bClaimed == FALSE );
   // We should be disabled if not claimed. 
   ASSERT( m_bDeviceEnabled == FALSE 
      || IsExclusive() == FALSE );
   CSingleLock mtxClaim( &m_mtxClaim );
   // Anybody else have this device claimed?
   if ( OPOS_CLASSKEY_LOCK == this->GetDeviceModel() )
   {
       lResult = OpenDevice();
       if ( lResult == OPOS_SUCCESS )
       {
           m_bClaimed = TRUE;
       }
   }
   else
   {
       if ( mtxClaim.Lock( lTimeout ) != FALSE )
          {
          lResult = OpenDevice();
          if ( lResult == OPOS_SUCCESS )
             {
             m_bClaimed = TRUE;
             // Add second lock to stick after CSingleLock releases it
             m_mtxClaim.Lock( INFINITE );  // Guaranteed to work!
             }
          }
   }
   return SetRC( lResult );
   }


// COposDevice::ReleaseClaim
//
// Purpose: Common release process.

void COposDevice::ReleaseClaim(
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposDevice::ReleaseClaim") );
   // TService template makes sure we are not called if we are not claimed.
   ASSERT( m_bClaimed != FALSE );
   CloseDevice();
   m_bClaimed = FALSE;
   m_mtxClaim.Unlock( );
   return ;
   }

// COposDevice::Enable
//
// Purpose: Common enable/disable process.

long COposDevice::Enable(
   BOOL bEnable )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposDevice::Enable") );
   ASSERT( bEnable == TRUE || bEnable == FALSE );
   // TService stops this
   ASSERT( (BOOL)m_bDeviceEnabled != bEnable );
   ASSERT( m_bClaimed != FALSE 
      || IsExclusive() == FALSE );
   long lResult = OPOS_SUCCESS;
   long nRcEx = 0;
   // Note: Disable can't fail!
   if ( bEnable == FALSE )
      OnDisable();
   else 
      lResult = OnEnable( &nRcEx  );
   if ( lResult == OPOS_SUCCESS )
      m_bDeviceEnabled = bEnable;
   return lResult;
   }


// COposDevice::DirectIO
//
// Purpose: Common DirectIO code - log an event


long COposDevice::DirectIO(
   long     nCommand,
   long*    plData,
   CString& sData)
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposDevice::DirectIO") );
   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("*DirectIO Command: %x, plData: %x, sData: %s"),
      nCommand, plData != NULL ? *plData : 0, sData );
   m_oDiagnostics.LogEventWithData( m_nSmInvalidDirectIO, 
      sData.GetLength(), (LPVOID)(LPCTSTR)sData, _T("%d%d%s"), 
      nCommand, plData == NULL ? 0 : *plData, sData );
   return SetRC( OPOS_E_ILLEGAL );
   }


// COposDevice::InteractiveHealthCheck
//
// Purpose: Perform Interactive Check Health using the derived class provided dialog. 
// Note:    There is no standard dialog. 

long COposDevice::InteractiveHealthCheck(
   COposCheckHealth* pCheckHealth /* = NULL */ )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposDevice::InteractiveHealthCheck") );
   long lResult = OPOS_E_ILLEGAL;
   ASSERT( m_pCheckHealth == NULL );
   ASSERT( pCheckHealth != NULL );  // No default health check. Derived must provide.
   m_pCheckHealth = pCheckHealth ;
   m_idsCheckHealthText = 0;
   m_pService->InteractiveHealthCheck( m_pCheckHealth );
   m_pCheckHealth = NULL;
   return SetRC(  InternalHealthCheck() );
   }           

// COposDevice::SetRC
//
// Purpose: Called by derived class to set result codes on the service.
// Note:    Not used much anymore since I now prefer passing pnRcEx to most methods.
//          Used by methods that are called by TService directly: DirectIO etc. 

long COposDevice::SetRC( 
   long nRC    /* = OPOS_SUCCESS */,
   long nRCEx  /* = 0            */) const
   {
   ASSERT( IsOpen() != FALSE );
   // Must not report OPOS_E_EXTENDED without an extended code. 
   ASSERT( nRCEx != 0 || nRC != OPOS_E_EXTENDED );
   m_pService->SetRC( nRC, nRCEx );
   return nRC;
   }


// COposDevice::OnDeviceFailure
//
// Purpose: Called by derived classed when the device fails.

void COposDevice::OnDeviceFailure(
   int nErrorId /* = 0 */ )
   {
   // Only report it once. 
   if ( m_bFailed == FALSE )
      {
      m_bFailed = TRUE;
      if ( nErrorId != 0 )
         m_oDiagnostics.LogEvent( nErrorId );
      OnDeviceFailed();
      }
   return;
   }


// COposDevice::OnDeviceOk
//
// Purpose: Called by derived classed when the device is on.

void COposDevice::OnDeviceOk( 
   void )
   {
   if ( m_bFailed != FALSE )
      OnDeviceRecovered();
   m_bFailed = FALSE;
   return;
   }


long COposDevice::GetPropertyNumber( 
   long lPidx ) const
   {  
   long lData = 0;
   switch( lPidx )
      {
      case PIDX_PowerState:                // Get these from the device
         lData = OPOS_PS_UNKNOWN;
         break;
      case PIDX_PowerNotify:
         lData = OPOS_PN_DISABLED;
         break;
      case PIDX_CapPowerReporting:
         lData = OPOS_PR_NONE;
         break;
      default:
         ASSERT( FALSE );  
         break;
      }
   return lData;
   }


void COposDevice::SetPropertyNumber( 
   long lPidx, 
   long lProperty )
   {  
   switch( lPidx )
      {
      case PIDX_PowerNotify:
         m_pService->SetRC( 
            lProperty != OPOS_PN_DISABLED || m_bDeviceEnabled != FALSE 
            ? OPOS_E_ILLEGAL : OPOS_SUCCESS );
         break;
      default:
         ASSERT( FALSE );  
         break;
      }
   return;
   }


// COposDevice::OnDeviceError
//
// Purpose: Device (non-media related) errors reported via this function.

void COposDevice::OnDeviceError(
   long nResult,
   long nRcEx /* = 0 */) const
   {
   m_pService->AddInputErrorEvent( nResult, nRcEx );
   return;
   }


// COposDevice::CheckDeviceOnline
//
// Purpose: Standard helper function used by the CH3 dialog to display a message about the 
//          on/off line status of the device. 

BOOL COposDevice::CheckDeviceOnline(
   CWnd* pWnd,
   int   idsDeviceIsOffline,
   int   idsDeviceIsOnline,
   int   idsDeviceHasFailed )
   {
   struct 
      {
      long  lResult;
      int   idsText;
      } mapStatusToText[] =
         {
         OPOS_SUCCESS,     idsDeviceIsOnline,
         OPOS_E_OFFLINE,   idsDeviceIsOffline,
         OPOS_E_FAILURE,   idsDeviceHasFailed,
         };
   long lResult = InternalHealthCheck();
   for ( int nIndex = 0; nIndex < ARRAY_ELEMENT_COUNT( mapStatusToText ); nIndex++ )
      {
      if ( mapStatusToText[ nIndex ].lResult == lResult )
         break;
      }
   if ( m_idsCheckHealthText == 0 )
      m_idsCheckHealthText = mapStatusToText[ 0 ].idsText;
   // Should be found
   ASSERT( nIndex != ARRAY_ELEMENT_COUNT( mapStatusToText ) ); 
   if ( nIndex != ARRAY_ELEMENT_COUNT( mapStatusToText ) 
      && m_idsCheckHealthText != mapStatusToText[ nIndex ].idsText )
      {
      m_idsCheckHealthText = mapStatusToText[ nIndex ].idsText;
      CString sText;
      sText.LoadString( m_idsCheckHealthText );
      CString sCaption;
      pWnd->GetWindowText( sCaption );
      pWnd->MessageBox( sText, sCaption );
      }
   return lResult;
   }



// COposDevice::ReportTally
//
// Purpose: Tally and data capture abnormal conditions.

void COposDevice::ReportTally( 
   int   nTallyId ) const 
   {
   ASSERT( nTallyId >= m_nFirstTally && nTallyId <= m_nLastTally );
// if ( m_nLastTally >= m_nFirstTally )
//    {  
//    CTally  theTally;
//    theTally.RegisterTallies( oRegistry.GetClass() + _T('-') + oRegistry.GetName(), 
//       m_nFirstTally, m_nLastTally - m_nFirstTally + 1);
//    theTally.IncrementTally ( nTallyId );
//    }
   CString sMessage;
   sMessage.LoadString( nTallyId );
   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Tally: %s"), (LPCTSTR) sMessage );
   return;
   }


void COposDevice::SetPowerState( 
   long nPowerState )
   {
   if ( m_nPowerState != nPowerState )
      {
      // We are not supporting this yet in the framework,
      // but this is a place holder for when the functionality is implemented
      }
   return;
   }
