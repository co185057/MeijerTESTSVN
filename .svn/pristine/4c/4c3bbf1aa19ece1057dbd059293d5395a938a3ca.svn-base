#include "..\stdafx.h"
#include "printer.h"
#include "CashDrawerWrapper.h"
#include "KeylockWrapper.h"
#include "MICRWrapper.h"
#include "EASWrapper.h"
#include "MSRWrapper.h"
#include "ScannerWrapper.h"
#include "MotionSensorWrapper.h"
#include "CashRecyclerWrapper.h"
#include "ScaleWrapper.h"
#include "CardDispenser.h"



// DMX_KEY_ROOT is the key under which all Device Manager keys and values are
// placed. This key contains device class keys. 
// The base key is HKEY_LOCAL_MACHINE
#define DMX_KEY_ROOT    _T("SOFTWARE\\NCR\\DeviceManagerEx")
#define DMX_KEY_DEVICES _T("Devices")
#define COMP_ID ID_DM   // used by the trace() calls to identify this component
#define T_ID    _T("DMp")

#define REMOTE_OPEN_SLEEP       100
#define REMOTE_OPEN_RETRIES     2


struct 
   {
   LPCTSTR                       sDeviceClassName;
   TClassPtr<CGenericOposIF>     (*pfnCreateDevice)( HKEY hKey, long nDeviceId );
   BOOL                          bDoEnable; 
   BOOL                          bIsEnabled;
   UINT                          nThreadId;
   } g_dmClassFactory[] =
   {  
   _T("Receipt"),          CPrinterWrapper::CreateDevice,      FALSE,   FALSE,   0,
   _T("Journal"),          CPrinterWrapper::CreateDevice,      FALSE,   FALSE,   0,
   _T("Slip"),             CPrinterWrapper::CreateDevice,      FALSE,   FALSE,   0,
   _T("Printer"),          CPrinterWrapper::CreateDevice,      FALSE,   FALSE,   0,
   _T("CashDrawer"),       CCashDrawerWrapper::CreateDevice,   FALSE,   FALSE,   1,
   _T("CashChanger"),      NULL,                               FALSE,   FALSE,   0,
   _T("Keylock"),          CKeylockWrapper::CreateDevice,      FALSE,   FALSE,   1,
   _T("LineDisplay"),      NULL,                               FALSE,   FALSE,   0,
   _T("MICR"),             CMICRWrapper::CreateDevice,         TRUE,    FALSE,   1,
   _T("MSR"),              CMSRWrapper::CreateDevice,          TRUE,    FALSE,   1,
   _T("Scale"),            CScaleWrapper::CreateDevice,        TRUE,    FALSE,   2,
   _T("Scanner"),          CScannerWrapper::CreateDevice,      TRUE,    FALSE,   1,
   _T("SignatureCapture"), NULL,                               TRUE,    FALSE,   1,
   _T("MISC"),             NULL,                               FALSE,   FALSE,   0,
   _T("Encryptor"),        NULL,                               TRUE,    FALSE,   1,
   _T("CashAcceptor"),     NULL,                               TRUE,    FALSE,   1,
   _T("CoinAcceptor"),     NULL,                               TRUE,    FALSE,   1,
   _T("AuxDisplay"),       NULL,                               FALSE,   FALSE,   0,
   _T("MotionSensor"),     CMotionSensorWrapper::CreateDevice, FALSE,   FALSE,   1,
   _T("ElectronicArticleSurveillant"), 
                           CEASWrapper::CreateDevice,          TRUE,    FALSE,   1,
   _T("CashRecycler"),      // Standard OPOS CashChanger - not Scot CashChanger
                           CCashRecyclerWrapper::CreateDevice, FALSE,   FALSE,   0,
   _T("PointCardRW"),    CCardDispenser::CreateDevice,       FALSE,   FALSE,     0,  //4.2 card dispenser
   };

#define DMXCLASS_MAX_CLASSES (sizeof( g_dmClassFactory ) / sizeof( g_dmClassFactory[0] ))

static LPCTSTR GetDCString( 
   long nDeviceClass )
   {
   return g_dmClassFactory[ nDeviceClass ].sDeviceClassName;
   }

/////////////////////////////////////////////////////////////////////////////
// ReadRegString
//
// Read and return a string value from the registry
//

CString ReadRegString(
   HKEY     hKey, 
   LPCTSTR  pszValue )
   {
   DWORD dwType;
   DWORD dwLength;

   // Look for value; get data size.
   long nRC = RegQueryValueEx(
      hKey,                   // handle of key to query
      pszValue,               // address of name of value to query
      NULL,                   // reserved
      &dwType,                // address of buffer for value type
      NULL,                   // address of data buffer: just return size.
      &dwLength );            // address of data buffer size

   CString sResult;
   // If success and type is string, get the data.
   if ( nRC == ERROR_SUCCESS && dwType == REG_SZ )
      {
      LPTSTR p = sResult.GetBuffer( dwLength );
      RegQueryValueEx(
         hKey,               // handle of key to query
         pszValue,           // address of name of value to query
         NULL,               // reserved
         &dwType,            // address of buffer for value type
         (PBYTE) p,          // address of data buffer
         &dwLength );        // address of data buffer size
      sResult.ReleaseBuffer();
      }
   return sResult;
   }
//Begin RFC 356020
bool CDeviceSet::IsSamePrinterReceiptSlip()
{
	bool bRet=false;
    CString sKey = _T("SOFTWARE\\OLEforRetail\\ServiceOPOS\\POSPrinter");
    HKEY hKey;
	CString sRec, sSlip, sRecValue, sSlipValue;
	DWORD dwDataSize = _MAX_PATH;
    LONG nRc = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sKey, 0, KEY_READ, &hKey);	
    if ( nRc == ERROR_SUCCESS )
	{

		  DWORD dwType=0;
		  DWORD dwLength=0;

		  // Look for value; get data size.
		  long nRC = RegQueryValueEx(
									  hKey,                   // handle of key to query
									  _T("ScotRec"),	      // address of name of value to query
									  NULL,                   // reserved
									  &dwType,                // address of buffer for value type
									  NULL,                   // address of data buffer: just return size.
									  &dwLength );            // address of data buffer size

		  // If success and type is string, get the data.
		  if ( nRC == ERROR_SUCCESS && dwType == REG_SZ )
		  {
			LPTSTR p = sRec.GetBuffer( dwLength );
			RegQueryValueEx(
							hKey,					// handle of key to query
							_T("ScotRec"),         // address of name of value to query
							NULL,					// reserved
							&dwType,				// address of buffer for value type
							(PBYTE) p,				// address of data buffer
							&dwLength );			// address of data buffer size
			sRec.ReleaseBuffer();

			sRecValue = p;

		  }

		  nRC = RegQueryValueEx(
									  hKey,                   // handle of key to query
									  _T("ScotSlip"),	      // address of name of value to query
									  NULL,                   // reserved
									  &dwType,                // address of buffer for value type
									  NULL,                   // address of data buffer: just return size.
									  &dwLength );            // address of data buffer size

		  // If success and type is string, get the data.
		  if ( nRC == ERROR_SUCCESS && dwType == REG_SZ )
		  {
			LPTSTR p2 = sSlip.GetBuffer( dwLength );
			RegQueryValueEx(
							hKey,					// handle of key to query
							_T("ScotSlip"),         // address of name of value to query
							NULL,					// reserved
							&dwType,				// address of buffer for value type
							(PBYTE) p2,				// address of data buffer
							&dwLength );			// address of data buffer size
			sSlip.ReleaseBuffer();

			sSlipValue = p2;

		  }


			if (!sRecValue.CompareNoCase(sSlipValue))
				bRet = true;

	}
	RegCloseKey( hKey);
	return bRet;
}
//End RFC 356020


void CDeviceSet::CreateDevices( CString  sKey, CDeviceManagerEx* pDMX,
                                    int nRetries, int nRetrySleep )
   {
   trace(L6, _T("Processing %s registry"), GetDCString( m_nDeviceClass ) );
   sKey += CString('\\') + GetDCString( m_nDeviceClass );
   HKEY hKey;
   LONG nRc = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sKey, 0, KEY_READ, &hKey);
   if ( nRc == ERROR_SUCCESS )
      {
      DWORD dwIndex = 0;
      do {
         TCHAR szDeviceName[MAX_PATH];

         nRc = RegEnumKey( hKey, dwIndex++, szDeviceName, sizeof( szDeviceName ) );
         if ( nRc == ERROR_SUCCESS )
            {
            HKEY hKey1;
            VERIFY( RegOpenKeyEx( hKey, szDeviceName, 0, KEY_READ, &hKey1) == ERROR_SUCCESS );
            CString sConfigure = ReadRegString( hKey1, _T("Configure") ).Left(1);
            if ( sConfigure.CompareNoCase(_T("Y")) == 0 )
            {
				//Begin RFC 356020
				CString sDevProfile(szDeviceName);
				if (!sDevProfile.CompareNoCase(_T("ScotSlip")) && IsSamePrinterReceiptSlip())
				{
					trace(L6, _T("Skip open for ScotSlip -- ScotRec and ScotSlip are configured to the same device") );
				}
				else //End RFC 356020
				{ // RFC 356020
               trace(L6, _T("%s-%s is configured"), 
                  GetDCString( m_nDeviceClass ), szDeviceName );
               long nDeviceId = _ttoi( ReadRegString( hKey1, _T("DeviceId") ) );
               // Find an available device id based on the id passed
               while ( nDeviceId < GetSize()
                  && GetAt( nDeviceId ) != NULL )
                  nDeviceId++;
               SetSize( max( nDeviceId + 1, GetSize() ) );
               ASSERT( GetAt( nDeviceId ) == NULL );
               ASSERT( g_dmClassFactory[ m_nDeviceClass ].pfnCreateDevice != NULL );
               trace(L6, _T("%s-%s is assigned DeviceId = %d"), 
                  GetDCString( m_nDeviceClass ), szDeviceName, nDeviceId );
               trace(L6, _T("%s-%s Create the Wrapper method =  %d"), 
                  GetDCString( m_nDeviceClass ), szDeviceName, 
                  g_dmClassFactory[ m_nDeviceClass ].pfnCreateDevice );
               TClassPtr<CGenericOposIF> pDevice = 
                  g_dmClassFactory[ m_nDeviceClass ].pfnCreateDevice( hKey, nDeviceId );
               ASSERT( pDevice != NULL );
               trace(L6, _T("%s-%s Reading the registry thru (%d)"), 
                  GetDCString( m_nDeviceClass ), szDeviceName, pDevice );
               pDevice->ReadRegistry( hKey1 );
               // Get description from the registry - or used Device Class Text
               CString sDeviceDescription = ReadRegString( hKey1, _T("DeviceDescription") );
               SetAtGrow( nDeviceId, pDevice );
               trace(L6, _T("%s-%s About to open"), 
                  GetDCString( m_nDeviceClass ), szDeviceName );
               // Now determine what thread this device should run on. The default is defined 
               // by the g_dmClassFactory table (by device class). However, the registry 
               // can be used to move individual devices.  
               UINT nThreadId = g_dmClassFactory[ m_nDeviceClass ].nThreadId;
               CString sThreadId = ReadRegString( hKey1, _T("ThreadId") );
               if ( sThreadId.IsEmpty() == FALSE )
                  nThreadId = _ttoi( sThreadId );
			   try
			   {
				   long rc = pDevice->CreateOposControl( 
					  sDeviceDescription, 
					  BASE_DEVID( m_nDeviceClass ) + m_nOffset + nDeviceId,
					  szDeviceName, 
					  pDMX->GetThreadWindow( nThreadId ) );
				   if ( rc == OPOS_SUCCESS )
				   {
					   int nOpenTries = 0; 
					   do   
					   {
						   nOpenTries++;
						   if   ( nOpenTries > 1 )
						   {
							   trace(L6, _T("%s-%s retry remote open"), 
							   GetDCString( m_nDeviceClass ), szDeviceName );
							   pDevice->CloseDevice();
							   Sleep(nRetrySleep);
						   }
						   rc = pDevice->OpenDevice(  );
					   } while ( ( rc != OPOS_SUCCESS ) && ( nOpenTries < nRetries ) );
				   }
				   else
				   {
					   trace(L6, _T("%s-%s CreateOPOSControl failed"), 
						   GetDCString( m_nDeviceClass ), szDeviceName );
					   
				   }
			   }
			   catch(...)
			   {
				  trace(L6, _T("Unexpected exception caught in CDeviceSet::CreateDevices()\n"));
			   }
			   }  //RFC 356020
            }
            RegCloseKey( hKey1 );
            }
         } while ( nRc == ERROR_SUCCESS );
      RegCloseKey( hKey);
      }
   return;
   }


CDeviceConfiguration::CDeviceConfiguration(
   int nOffset /* = 0 */, int nRetries /* = 1 */, int nRetrySleep /* = 100 */ )
   : m_pLocalCfg( NULL ),
   m_nOffset( nOffset ),
   m_nRetries( nRetries ),
   m_nRetrySleep( nRetrySleep )
   {
   Close();
   return;
   }


void CDeviceConfiguration::AssignPrinterDeviceId( 
   long              nDeviceClass,
   CPrinterWrapper*  pPrinter )
   {
   TClassPtr<CGenericOposIF> pDevice;
   // Find the printer passed in the DMCLASS_PRINTER map. 
   for ( int nIndex = 0; nIndex < GetAt( DMCLASS_PRINTER )->GetSize(); nIndex++ )
      {
      if ( GetAt( DMCLASS_PRINTER )->GetAt( nIndex ) == 
         (const CGenericOposIF*)pPrinter )
         {
         pDevice = GetAt( DMCLASS_PRINTER )->GetAt( nIndex );
         break;
         }
      }
   // The printer should be found in the DMCLASS_PRINTER set
   ASSERT( pDevice != NULL );
   long nDeviceId = GetAt( nDeviceClass )->GetSize();
   trace(L6, _T("Assign Printer %d to %s id= %d"), nIndex, 
      GetDCString( nDeviceClass ), nDeviceId );
   pPrinter->CreateStation( nDeviceClass, nDeviceId );
   // Add the wrapper class to the correct set
   GetAt( nDeviceClass )->Add( pDevice );
   return;
   }
   
//SR742+
void CDeviceConfiguration::MSROpenDevice(  LPCTSTR  sConfig, 
   CDeviceManagerEx* pDMX )
{ 
      CDeviceSet* pSet = GetAt( DMCLASS_MSR );
      if ( pSet != NULL )
         {
         CString sKey = DMX_KEY_ROOT + CString('\\') + DMX_KEY_DEVICES + CString('\\') + sConfig;
         pSet->CreateDevices( sKey, pDMX, m_nRetries, m_nRetrySleep );
         }
   return;
}
//SR742-

void CDeviceConfiguration::Open(
   LPCTSTR  sConfig, 
   CDeviceManagerEx* pDMX )
   {

   //***********************************************************************
   // Record Registry configuration of each supported device
   //***********************************************************************
   for ( int nIndex = 0; nIndex < GetSize(); nIndex++ )
      {
      CDeviceSet* pSet = GetAt( nIndex );
      if ( pSet != NULL )
         {
         CString sKey = DMX_KEY_ROOT + CString('\\') + DMX_KEY_DEVICES + CString('\\') + sConfig;
         pSet->CreateDevices( sKey, pDMX, m_nRetries, m_nRetrySleep );
         }
      }
   // This coded sets the printer stations fo rsingle station printers.  
   for ( long nDeviceClass = DMCLASS_RECEIPT; 
      nDeviceClass <= DMCLASS_PRINTER; nDeviceClass++ )
      {
      // Get current set
      TClassPtr<CDeviceSet> pDeviceSet = GetAt( nDeviceClass );

      for ( nIndex = 0; nIndex < pDeviceSet->GetSize(); nIndex++ )
         {
         TClassPtr<CGenericOposIF> pDevice = pDeviceSet->GetAt( nIndex );
         if ( pDevice != NULL
            && pDevice->IsAvailable() != FALSE )
            {
            CPrinterWrapper* pPrinter = pDevice->GetPrinter();
            ASSERT( pPrinter != NULL );
            if ( nDeviceClass != DMCLASS_PRINTER )
               pPrinter->CreateStation( nDeviceClass, nIndex );
            else 
               {
				CString sProfile = pDevice->GetDeviceProfile();
               // This code sets up the receipt/journal/slip stations based on the 
               // abilities of the printers that were loaded. 
               if ( pPrinter->GetCapJrnPresent() != FALSE )
                  AssignPrinterDeviceId( DMCLASS_JOURNAL, pPrinter );
			   //Begin RFC 356020
               if ( pPrinter->GetCapRecPresent() != FALSE 
				   && 
				   (!sProfile.CompareNoCase(_T("SCOTRec")) || !sProfile.CompareNoCase(_T("RAPPOSPrinter")) || !sProfile.CompareNoCase(_T("Emulator - Receipt")) ) )
                  AssignPrinterDeviceId( DMCLASS_RECEIPT, pPrinter );
               if ( pPrinter->GetCapSlpPresent() != FALSE  && (!sProfile.CompareNoCase(_T("ScotSlip")) ||  !sProfile.CompareNoCase(_T("RAPPOSPrinter")) || pDeviceSet->IsSamePrinterReceiptSlip())  )
                  AssignPrinterDeviceId( DMCLASS_SLIP, pPrinter );
			   //End RFC 356020
               }
            }
         }
      }
   return;
   }


//SR742+
void CDeviceConfiguration::MSRCloseDevice()
{
	  // Add device classes that we support
	if ( g_dmClassFactory[ DMCLASS_MSR ].pfnCreateDevice != NULL )
         SetAt( DMCLASS_MSR, new CDeviceSet( DMCLASS_MSR, m_nOffset ) );
    return;
}
//SR742-

void CDeviceConfiguration::Close( 
   void )
   {
   SetSize( DMXCLASS_MAX_CLASSES );
   for ( int nIndex = 0; nIndex < DMXCLASS_MAX_CLASSES; nIndex++ )
      {  // Add device classes that we support
      if ( g_dmClassFactory[ nIndex ].pfnCreateDevice != NULL )
         SetAt( nIndex, new CDeviceSet( nIndex, m_nOffset ) );
      }
   return ;
   }


BOOL CDeviceConfiguration::IsClassSupported(
   long nDeviceClass ) const
   {
   if ( (DWORD)nDeviceClass >= DMXCLASS_MAX_CLASSES )
      CDeviceManagerEx::ThrowDmException( DM_E_INVALIDDEVICECLASS );
   return GetAt( nDeviceClass ) != NULL;
   } 

long CDeviceConfiguration::GetDeviceCount(
   long nDeviceClass ) const
   {
   ASSERT( IsClassSupported( nDeviceClass ) != FALSE );
   long nResult = GetAt( nDeviceClass )->GetSize();
   if ( m_pLocalCfg != NULL )
      nResult = max( nResult, m_pLocalCfg->GetDeviceCount( nDeviceClass  ) );
   return nResult;
   }

CString CDeviceConfiguration::GetDeviceAvailability( 
   long nDeviceClass ) const
   {
   CString sResult;
   if ( IsClassSupported( nDeviceClass ) != FALSE )
      {
      long nNumDevices = GetDeviceCount( nDeviceClass );
      for ( int nIndex = 0; nIndex < nNumDevices; nIndex++ )
         {
         CGenericOposIF* pDevice = GetDevice( nDeviceClass, nIndex );
         if (  pDevice == NULL )
            sResult += DM_DA_NOT_CONFIGURED;
         else if ( pDevice->IsAvailable() != FALSE )
            sResult += DM_DA_LOADED;
         else
            sResult += DM_DA_CONFIGURED_BUT_NOT_LOADED;
         }
      }
   return sResult;
   }


CGenericOposIF* CDeviceConfiguration::GetDevice( 
   long  nDeviceClass, 
   long  nDeviceId ) const
   {
   ASSERT( IsClassSupported( nDeviceClass ) != FALSE );
   CGenericOposIF* pResult = NULL;
   CDeviceSet* pSet = GetAt( nDeviceClass );
   ASSERT( pSet != NULL );
   if ( nDeviceId < pSet->GetSize() )
      pResult = pSet->GetOposDevice( nDeviceId ); 
   if ( pResult == NULL
      && m_pLocalCfg != NULL )
      pResult = m_pLocalCfg->GetDevice( nDeviceClass, nDeviceId );
   return pResult;
   }

// CDeviceManagerEx methods

/* virtual */
void CDeviceManagerEx::Open(
   void )
   {
	// Fix TAR 322422 - Stopping Fastlane from Launchpad causes a Dr.Watson on ADK 4.2.1/ADD 1.08.02 
   m_bShutdownInProgress=FALSE;
   HKEY hKey;
   if ( RegOpenKeyEx( HKEY_LOCAL_MACHINE, DMX_KEY_ROOT, 0, KEY_READ, &hKey) == ERROR_SUCCESS )
      {
      CString sSkip = ReadRegString( hKey, _T("SkipHealthCheck") ).Left(1);
      if ( sSkip.CompareNoCase(_T("Y")) == 0 )
         CGenericOposIF::sm_bSkipStartupCheckHealth = TRUE;
      }
   m_cmDc.ReadRegistry( DMX_KEY_ROOT );
   TClassPtr<DMInterface> pDMI = new DMInterface( *this, m_cmDc );
   pDMI->Initialize(FALSE);
   m_mapIdToDMI.SetAt( 0, pDMI );

   m_cfgLocal.Open( _T("Local"), this );
   m_cfgRemote.LinkTo( &m_cfgLocal );
   // Load NCRRemoteSO.dll and keep it in memory
   if ( m_RemoteSO.m_lpDispatch == NULL )
      m_RemoteSO.CreateDispatch( _T("NCRRemoteSO.KeylockSO") );
   CDevMgr::Open();
   return;
   }
   
//SR742+
void CDeviceManagerEx::ReloadMSR(void)
{
	//close it first then open.
	m_cfgLocal.MSRCloseDevice(); 
	m_cfgLocal.MSROpenDevice( _T("Local"), this );
    return;
}
//SR742-

/* virtual */
void CDeviceManagerEx::Close(
   void )
   {
	// Fix TAR 322422 - Stopping Fastlane from Launchpad causes a Dr.Watson on ADK 4.2.1/ADD 1.08.02 
   m_bShutdownInProgress=TRUE;
   m_cfgLocal.Close();
   m_cfgRemote.Close();
   for ( int nIndex = 0; nIndex < DMXCLASS_MAX_CLASSES; nIndex++ )
      g_dmClassFactory[ nIndex ].bIsEnabled = FALSE;
   CDevMgr::Close();
   m_mapIdToDMI.RemoveAll();
   return;
   }

/* virtual */
void CDeviceManagerEx::Enable(
   long nDeviceId, 
   long nDeviceClass, 
   BOOL fEnable )
   { 
   if ( m_cfgRemote.IsClassSupported( nDeviceClass ) != FALSE ) 
      {
      CGenericOposIF* pDevice = GetDevice( nDeviceClass, nDeviceId );
      pDevice->SetDeviceEnabled( fEnable );
      if ( fEnable != FALSE )
         pDevice->SetDataEventEnabled( TRUE );
      }
   else
      {
      // This only works for device id = zero
      // If the device class supports more than one device, then the 
      // device class should be moved to DMX
      ASSERT( nDeviceId == 0 );
      CDevMgr::Enable( nDeviceId, nDeviceClass, fEnable );
      g_dmClassFactory[ nDeviceClass ].bIsEnabled = fEnable;
      }
   return;
   }

/* virtual */
long CDeviceManagerEx::GetAvailableDevices(
   long nDeviceClass )
   {
   long nCount = 0;
   if ( m_cfgRemote.IsClassSupported( nDeviceClass ) != FALSE ) 
      {
      CString sAvailability = GetDeviceAvailability( nDeviceClass );
      while ( sAvailability.IsEmpty() == FALSE )
         {
         nCount += ( sAvailability[ 0 ] == DM_DA_LOADED );
         sAvailability = sAvailability .Mid( 1 );
         }
      }
   else
      nCount = CDevMgr::GetAvailableDevices( nDeviceClass );
   return nCount;
   }


/* virtual */
CString CDeviceManagerEx::GetDeviceDescription(
   long  nDeviceId, 
   long  nDeviceClass)
   {
   if ( m_cfgRemote.IsClassSupported( nDeviceClass ) != FALSE ) 
      {
      // Don't use CDeviceManagerEx::GetDevice because we don't
      // was a throw when the device is not available
      // that's ok for us
      CGenericOposIF* pDevice = m_cfgRemote.GetDevice( nDeviceClass, nDeviceId  );
      if ( pDevice == NULL )
         ThrowDmException( DM_E_INVALIDDEVICEID );
      CDeviceStatistics result = GetDeviceStatistics( nDeviceClass, nDeviceId );
      return result.csDescription;
      }
   return CDevMgr::GetDeviceDescription(nDeviceId, nDeviceClass);
   }
/* virtual */
void CDeviceManagerEx::ClearInput(
   long nDeviceClass )
   {
   // Add to CGenericOposIF device class for All devices
   CDevMgr::ClearInput( nDeviceClass  );
   return;
   }


/* virtual */
void CDeviceManagerEx::CheckHealth(
   long  nDeviceClass, 
   long  nDeviceId, 
   long  Level)
   {
   if ( m_cfgRemote.IsClassSupported( nDeviceClass ) != FALSE ) 
      {
      CGenericOposIF* pDevice = GetDevice( nDeviceClass, nDeviceId );
      long lResult = pDevice->CheckHealth( Level );
      ThrowDmException( nDeviceClass, lResult, pDevice->GetResultCodeExtended() );
      }
   else
      CDevMgr::CheckHealth( nDeviceClass, nDeviceId, Level );
   return;
   }

/* virtual */
CString CDeviceManagerEx::GetDeviceAvailability(
   long nDeviceClass )
   {
   CString sResult;
   // Check and see if we are handling this class, if so get it
   // here, else go to DM
   if ( m_cfgRemote.IsClassSupported( nDeviceClass ) != FALSE ) 
      sResult = m_cfgRemote.GetDeviceAvailability( nDeviceClass );
   else if ( nDeviceClass < DMCLASS_MAX_CLASSES )
      sResult = CDevMgr::GetDeviceAvailability( nDeviceClass );
   return sResult;
   }

/* static */
void CDeviceManagerEx::ThrowDmException( 
   CGenericOposIF* pDevice )
   {
   ThrowDmException( pDevice->GetDeviceClass(), 
      pDevice->GetResultCode(), pDevice->GetResultCodeExtended() );
   }


/* static */
void CDeviceManagerEx::ThrowDmException( 
   CPrintStation* pDevice )
   {
   ThrowDmException( DMCLASS_PRINTER, pDevice->GetPrinter().GetResultCode(), 
      pDevice->GetPrinter().GetResultCodeExtended() );
   }

/* static */
void CDeviceManagerEx::ThrowDmException( 
   long  nDeviceClass,
   long  lResult, 
   long  nRcEx )
   {
#define ANY_VALUE 0
   static const struct 
      {
      long  nDeviceClass;     // 0 means any
      long  nResult;          // 0 means any (when I'm not sure what the value is.) 
      long  nRcEx;            // 0 means any
      SCODE dmCode;
      } mapResultsToException[] =
      {     // Specific DM device status values may be defined in column #4
      ANY_VALUE,        OPOS_SUCCESS,     ANY_VALUE,                 0,
      DMCLASS_PRINTER,  OPOS_E_FAILURE,   NCR_EPTR_SWFAIL,           DM_E_DEVICEFAILURE,
      DMCLASS_PRINTER,  ANY_VALUE,        NCR_EPTR_DE_PORTOPEN,      DM_E_DEVICEFAILURE,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_STATION,          DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_NOCAP,            DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_ROTATEMODE,       DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_CP_PERCENTAGE,    DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BC_SYMBOLOGY,     DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BC_HEIGHT,        DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BC_ALIGNMENT,     DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BC_TEXTPOSITION,  DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BM_BITMAPNUMBER,  DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BM_WIDTH,         DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BM_ALIGNMENT,     DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_BM_BITMAPTYPE,    DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_RP_NOTROTATEMODE, DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_RP_ROTATION,      DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_TP_TRANSMODE,     DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_TP_NOTTRANSMODE,  DM_E_ILLEGAL,
      DMCLASS_PRINTER,  OPOS_E_ILLEGAL,   NCR_EPTR_TP_CONTROL,       DM_E_ILLEGAL,
      DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_LINE_ERROR,      DM_E_DEVICEFAILURE,
      DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_NOCHECK,         DM_E_DEVICEFAILURE,
      DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_NO_DATA,         DM_E_DEVICEFAILURE,
      DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_BAD_DATA,        DM_E_DEVICEFAILURE,
      DMCLASS_MICR,     OPOS_E_FAILURE,   NCR_EMICR_CHECKJAM,        DM_E_DEVICEFAILURE,
      DMCLASS_PRINTER,  OPOS_E_EXTENDED,  OPOS_EPTR_BADFORMAT,       CTL_E_INVALIDFILEFORMAT,
//?   DMCLASS_PRINTER,  OPOS_E_EXTENDED,  OPOS_EPTR_TOOBIG,          DM_E_BITMAPTOOBIG, ?See Dm Code?
      ANY_VALUE,        OPOS_E_ILLEGAL,   ANY_VALUE,                 DM_E_ILLEGAL,
      ANY_VALUE,        OPOS_E_NOEXIST,   ANY_VALUE,                 CTL_E_FILENOTFOUND,
      ANY_VALUE,        OPOS_E_FAILURE,   ANY_VALUE,                 DM_E_DEVICEFAILURE,
      ANY_VALUE,        OPOS_E_BUSY,      ANY_VALUE,                 DM_E_BUSY,
      ANY_VALUE,        OPOS_E_TIMEOUT,   ANY_VALUE,                 DM_E_TIMEOUT,
	  DMCLASS_CASHRECYCLER, OPOS_E_OFFLINE, ANY_VALUE,				 DM_E_DEVICEFAILURE, // SafePay poweroff.
	  DMCLASS_CASHRECYCLER, OPOS_E_DISABLED, ANY_VALUE,				 DM_E_DEVICENOTAVAILABLE, // TAR 356035
      };                                                             
   SCODE scException = DM_E_DEVICEFAILURE;
   LPCTSTR sDesc = _T("Device failure");
   if ( nDeviceClass == DMCLASS_RECEIPT
      || nDeviceClass == DMCLASS_JOURNAL
      || nDeviceClass == DMCLASS_SLIP )
      nDeviceClass = DMCLASS_PRINTER;
   for ( int nIndex = 0; nIndex < sizeof( mapResultsToException )
      / sizeof( mapResultsToException[0] ); nIndex++ )
      {
      if ( mapResultsToException[ nIndex ].nDeviceClass == ANY_VALUE
         || mapResultsToException[ nIndex ].nDeviceClass == nDeviceClass )
         {
         if ( mapResultsToException[ nIndex ].nResult == lResult )
            {
            if ( mapResultsToException[ nIndex ].nRcEx == ANY_VALUE
               || mapResultsToException[ nIndex ].nRcEx == nRcEx )
               {
               scException = mapResultsToException[ nIndex ].dmCode;
               break;
               }
            }
         // Allow extended result codes to be mapped regardless of ResultCode
         else if ( mapResultsToException[ nIndex ].nResult == ANY_VALUE
            && mapResultsToException[ nIndex ].nRcEx == nRcEx )
            {
            scException = mapResultsToException[ nIndex ].dmCode;
            break;
            }
         }
      }
   ThrowDmException( scException  );
   return;
   }

/* static */
void CDeviceManagerEx::ThrowDmException( 
   SCODE scException  )
   {
   static const struct 
      {
      SCODE dmCode;
      LPCTSTR sDesc;
      } mapScToText[] =
      {
      DM_E_ILLEGAL,              _T("Function is illegal"),
      CTL_E_FILENOTFOUND,        _T("File not found"),
      CTL_E_INVALIDFILEFORMAT,   _T("Invalid file format"),
      DM_E_BITMAPTOOBIG,         _T("Bitmap too big for device"),
      DM_E_DEVICEFAILURE,        _T("Device failure"), 
      DM_E_BUSY,                 _T("An asynchronous process is currently using device"),
      DM_E_TIMEOUT,              _T("Device timeout occurred"),
      DM_E_INVALIDDEVICECLASS,   _T("nDeviceClass parameter is invalid"),
      DM_E_INVALIDDEVICEID,      _T("nDeviceId parameter is invalid"),
      DM_E_DEVICENOTCONFIGURED,  _T("Device is not configured"),
      DM_E_DEVICENOTAVAILABLE,   _T("Device is not available"),
      DM_E_INVALIDPARAMETER,     _T("Invalid parameter"),
      DM_E_NOTSUPPORTED,         _T("Function not supported"),
      };
   if ( scException != 0 )
      {
      LPCTSTR sDesc = _T("Unknown Exception Code");
      for ( int nIndex = 0; nIndex < sizeof( mapScToText ) / sizeof( mapScToText[0] );
         nIndex++ )
         {
         if ( mapScToText[ nIndex ].dmCode == scException )
            {
            sDesc = mapScToText[ nIndex ].sDesc;
            break;
            }
            }
      COleDispatchException* pExcept = new COleDispatchException( sDesc, -1, 0 );
      pExcept->m_scError = scException;
      throw pExcept;
      }
   return;
   }


void CDeviceManagerEx::OpenRemote(
   LPCTSTR sKey ) 
   {
   m_cfgRemote.Open( sKey, this );
   }

void CDeviceManagerEx::CloseRemote(
   void ) 
   {
   m_cfgRemote.Close(  );
   }

// CDeviceManagerEx::GetDevice
//
// Purpose: Get a device based on DC/DID.
// Throws:  DM_E_INVALIDDEVICEID - device  not found

CGenericOposIF* CDeviceManagerEx::GetDevice( 
   long nDeviceClass, 
   long nDeviceId )
   {
   CGenericOposIF* pDevice = m_cfgRemote.GetDevice( nDeviceClass, nDeviceId );
   if ( pDevice == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   else if ( pDevice->IsAvailable() == FALSE )
      ThrowDmException( DM_E_DEVICENOTAVAILABLE );
   return pDevice;
   }


// CDeviceManagerEx::GetCashDrawer
//
// Purpose: Get a CashDrawer device by DID.
// Throws:  DM_E_INVALIDDEVICEID - device  not found

CCashDrawerWrapper* CDeviceManagerEx::GetCashDrawer( 
   long nDeviceId )
   {
   CCashDrawerWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_CASHDRAWER, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetCashDrawer( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }


// CDeviceManagerEx::GetKeylock
//
// Purpose: Get a Keylock device by DID. 
// Throws:  DM_E_INVALIDDEVICEID - device  not found

CKeylockWrapper* CDeviceManagerEx::GetKeylock( 
   long nDeviceId )
   {
   CKeylockWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_KEYLOCK, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetKeylock( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }
  
// CDeviceManagerEx::GetMICR
//
// Purpose: Get a MICR device by DID. If the device is not found, 
//          then the DM_E_INVALIDDEVICEID is thrown.

CMICRWrapper* CDeviceManagerEx::GetMICR(
   long nDeviceId )
   {
   CMICRWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_MICR, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetMICR( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }

//+4.2 card dispenser
// CDeviceManagerEx::GetCardDispenser
//
// Purpose: Get a Card Dispenser device by DID. If the device is not found, 
//          then the DM_E_INVALIDDEVICEID is thrown.

CCardDispenser* CDeviceManagerEx::GetCardDispenser(long nDeviceId )
   {
   CCardDispenser* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_CARDDISPENSER, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetCardDispenser( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }
//-4.2 card dispenser
// CDeviceManagerEx::GetScanner
//
// Purpose: Get a Scanner device by DID. 
// Throws:  DM_E_INVALIDDEVICEID - device  not found

CScannerWrapper* CDeviceManagerEx::GetScanner(
   long nDeviceId )
   {
   CScannerWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_SCANNER, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetScanner( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }

// CDeviceManagerEx::GetPrinter
//
// Purpose: Get a Printer (DMCLASS_PRINTER) device by DID. 
// Throws:  DM_E_INVALIDDEVICEID - device  not found
// Note:    I do not anticipate that the application will need this method.

CPrinterWrapper* CDeviceManagerEx::GetPrinter(
   long nDeviceClass,
   long nDeviceId )
   {
   CPrinterWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( nDeviceClass, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetPrinter( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }

// CDeviceManagerEx::GetPrintStation
//
// Purpose: Get a Print Station (DMCLASS_RECEIPT, DMCLASS_JOURNAL, DMCLASS_SLIP) 
//          device by DC/DID. 
// Throws:  DM_E_INVALIDDEVICEID - device  not found

CPrintStation* CDeviceManagerEx::GetPrintStation(
   long nDeviceClass,
   long nDeviceId )
   {
   CPrintStation* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( nDeviceClass, nDeviceId );
   if ( pDevice != NULL )
   {
	   CPrinterWrapper* pPrinter = pDevice->GetPrinter( );
	   if ( pPrinter != NULL )
	   {
		   if (nDeviceClass == DMCLASS_PRINTER) //Error reporting using generic printer class
		   {
			   for (int nStation=0; nStation < 3 && pResult == NULL; nStation ++)
			   {
				   pResult = pPrinter->GetPrintStation( nStation );
				   if (pResult != NULL && pResult->m_nDeviceId != nDeviceId)
				   {
					   pResult = NULL;
				   }
			   }
		   }
		   else  //Should be real print station
		   {
			   pResult = pPrinter->GetPrintStation(nDeviceClass);
		   }
	   }
   }
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }


// CDeviceManagerEx::GetMSR
//
// Purpose: Get a MSR device by DID. 
// Throws:  DM_E_INVALIDDEVICEID - device  not found

CMSRWrapper* CDeviceManagerEx::GetMSR(
   long nDeviceId )
   {
   CMSRWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_MSR, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetMSR( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }

// CDeviceManagerEx::GetMotionSensor
//
// Purpose: Get a Motion Sensor device by DID. 
// Throws:  DM_E_INVALIDDEVICEID - device  not found

CMotionSensorWrapper* CDeviceManagerEx::GetMotionSensor(
   long nDeviceId )
   {
   CMotionSensorWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_MOTION_SENSOR, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetMotionSensor( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }

// CDeviceManagerEx::GetMotionSensor
//
// Purpose: Get an EAS device by DID. 
// Throws:  DM_E_INVALIDDEVICEID - device  not found

CEASWrapper* CDeviceManagerEx::GetEASWrapper(
   long nDeviceId )
   {
   CEASWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_EASDEACTIVATOR, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetEASDeactivator( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }

// CDeviceManagerEx::IsDeviceRemote
//

BOOL CDeviceManagerEx::IsDeviceRemote( 
   long  nDeviceClass, 
   long  nDeviceId ) const
   {
   return m_cfgRemote.IsDeviceRemote( nDeviceClass, nDeviceId );
   }

// CRemoteDeviceConfiguration::IsDeviceRemote
//

BOOL CRemoteDeviceConfiguration::IsDeviceRemote( 
   long  nDeviceClass, 
   long  nDeviceId ) const
   {
   ASSERT( IsClassSupported( nDeviceClass ) != FALSE );
   CGenericOposIF* pResult = NULL;
   CDeviceSet* pSet = GetAt( nDeviceClass );
   ASSERT( pSet != NULL );
   return  nDeviceId < pSet->GetSize();
   }

// How many times to retry the open of a remote device
int CRemoteDeviceConfiguration::GetRemoteOpenRetries()
{
    int nRetries = REMOTE_OPEN_RETRIES;     // = 2;
	HKEY hKey = NULL;

	long rc = RegOpenKey( HKEY_LOCAL_MACHINE, _T( "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\DMConfig" ), &hKey );

	if	( rc == ERROR_SUCCESS )
	{
		  DWORD dwType;
		  DWORD dwLength;
		  CString s;

		  // Look for value; get data size.
		  long nRC = RegQueryValueEx(
									  hKey,                   // handle of key to query
									  _T("RemoteOpenRetries"),	      // address of name of value to query
									  NULL,                   // reserved
									  &dwType,                // address of buffer for value type
									  NULL,                   // address of data buffer: just return size.
									  &dwLength );            // address of data buffer size

		  // If success and type is string, get the data.
		  if ( nRC == ERROR_SUCCESS && dwType == REG_SZ )
		  {
			LPTSTR p = s.GetBuffer( dwLength );
			RegQueryValueEx(
							hKey,					// handle of key to query
							_T("RemoteOpenRetries"),         // address of name of value to query
							NULL,					// reserved
							&dwType,				// address of buffer for value type
							(PBYTE) p,				// address of data buffer
							&dwLength );			// address of data buffer size
			s.ReleaseBuffer();

            int iRetries = _ttoi( ( LPCTSTR ) s );

            if  ( iRetries < 0 ) 
                iRetries = 2;

            if  ( iRetries > 5 )
                iRetries = 5;

            nRetries = iRetries;
          }

        RegCloseKey( hKey );
   }

    return nRetries; 
}

// How much time to sleep between retry remote device opens 
int CRemoteDeviceConfiguration::GetRemoteOpenSleep()
{
    int nSleep = REMOTE_OPEN_SLEEP;     // = 100;
	HKEY hKey = NULL;

	long rc = RegOpenKey( HKEY_LOCAL_MACHINE, _T( "SOFTWARE\\NCR\\SCOT\\CurrentVersion\\DMConfig" ), &hKey );

	if	( rc == ERROR_SUCCESS )
	{
		  DWORD dwType;
		  DWORD dwLength;
		  CString s;

		  // Look for value; get data size.
		  long nRC = RegQueryValueEx(
									  hKey,                   // handle of key to query
									  _T("RemoteOpenSleep"),	  // address of name of value to query
									  NULL,                   // reserved
									  &dwType,                // address of buffer for value type
									  NULL,                   // address of data buffer: just return size.
									  &dwLength );            // address of data buffer size

		  // If success and type is string, get the data.
		  if ( nRC == ERROR_SUCCESS && dwType == REG_SZ )
		  {
			LPTSTR p = s.GetBuffer( dwLength );
			RegQueryValueEx(
							hKey,					// handle of key to query
							_T("RemoteOpenSleep"),         // address of name of value to query
							NULL,					// reserved
							&dwType,				// address of buffer for value type
							(PBYTE) p,				// address of data buffer
							&dwLength );			// address of data buffer size
			s.ReleaseBuffer();

            int iSleep = _ttoi( ( LPCTSTR ) s );

            if  ( iSleep < 50 ) 
                iSleep = 50;

            if  ( iSleep > 2000 )
                iSleep = 2000;

            nSleep = iSleep;
          }

        RegCloseKey( hKey );
   }
    return nSleep;
}
      
// CDeviceManagerEx::GetDeviceByCtrlId
//

CGenericOposIF* CDeviceManagerEx::GetDeviceByCtrlId( 
   long nCtrlId )
   {
   CGenericOposIF* pDevice = NULL;
   long nDeviceClass = CTRLID_TO_DEVCLASS( nCtrlId );
   long nDeviceId = CTRLID_TO_DEVID( nCtrlId );

   if ( CTRLID_IS_REMOTE( nCtrlId ) != FALSE )
      pDevice = m_cfgRemote.GetDevice( nDeviceClass, nDeviceId );
   else
      pDevice = m_cfgLocal.GetDevice( nDeviceClass, nDeviceId );
   ASSERT( pDevice != NULL );
   return pDevice;
   }

// CDeviceManagerEx::GetMaxClasses
//

long CDeviceManagerEx::GetMaxClasses( 
   void ) const
   {
   return DMXCLASS_MAX_CLASSES;
   }

// CDeviceManagerEx::GetDeviceStatistics
//

CDeviceStatistics CDeviceManagerEx::GetDeviceStatistics( 
   long nDeviceClass, 
   long nDeviceId /* = 0 */)
   {     
   CDeviceStatistics result;
   if ( m_cfgRemote.IsClassSupported( nDeviceClass ) != FALSE ) 
      {
      CGenericOposIF* pDevice = m_cfgRemote.GetDevice( nDeviceClass, nDeviceId );
      if ( pDevice != NULL )
         result = pDevice->GetDeviceStatistics( );
      }
   else if ( nDeviceClass < DMCLASS_MAX_CLASSES )
      {
      // This only works for device id = zero
      // If the device class supports more than one device, then the 
      // device class should be moved to DMX
      try
         {
         CString sAvailable = GetDeviceAvailability( nDeviceClass );
         if ( sAvailable.GetLength() > nDeviceId )
            {
            result.cStatus = sAvailable.GetAt( nDeviceId );
            result.fDoEnable = g_dmClassFactory[ nDeviceClass ].bDoEnable != FALSE;
            if ( result.cStatus != DM_DA_NOT_CONFIGURED )
               {
               result.csDescription = GetDeviceDescription( nDeviceId, nDeviceClass);
               result.fIsEnabled = g_dmClassFactory[ nDeviceClass ].bIsEnabled != FALSE;
               }
            }
         }
      catch( ... )      // Application likes to call Disable after DM is closed
         {              // Supported devices work fine, but DM throws an exception.
         }              // This code catches the exception and returns the default
      }                 // "statistics" --> Someone should fix the application!!!
   if ( result.csDescription.IsEmpty() != FALSE )
      result.csDescription = GetDCString( nDeviceClass );
   return result;
   }


// CDeviceManagerEx::GetCashRecycler
//
// Purpose: Get a Recycler device by DID. 
// Throws:  DM_E_INVALIDDEVICEID - device  not found

CCashRecyclerWrapper* CDeviceManagerEx::GetCashRecycler(
   long nDeviceId )
   {
   CCashRecyclerWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_CASHRECYCLER, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetCashRecycler( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }


CScaleWrapper* CDeviceManagerEx::GetScaleWrapper( 
   long nDeviceId )
   {
   CScaleWrapper* pResult = NULL;
   CGenericOposIF* pDevice = GetDevice( DMCLASS_SCALE, nDeviceId );
   if ( pDevice != NULL )
      pResult = pDevice->GetScale( );
   if ( pResult == NULL )
      ThrowDmException( DM_E_INVALIDDEVICEID );
   return pResult;
   }

CWnd* CDeviceManagerEx::GetThreadWindow( 
   UINT nThreadId )
   {
   TClassPtr<DMInterface> pDMI;
   m_mapIdToDMI.Lookup( nThreadId, pDMI );
   if ( pDMI == NULL )
      {
      pDMI = new DMInterface( *this, m_cmDc );
      pDMI->Initialize();
      m_mapIdToDMI.SetAt( nThreadId, pDMI );
      }
   return pDMI ;
   }


BOOL CDeviceManagerEx::OnOposErrorEvent( 
   UINT  nCtrlId, 
   long  nResult, 
   long  nRcEx, 
   long  nErrorLocus, 
   long FAR* pnErrorResponse )
   {
	// +TAR 374673 
	// Expand fix for TAR 322422 to other event handlers
   if ( m_bShutdownInProgress )
		return TRUE;
    // -TAR 374673 

   CGenericOposIF* pDevice = GetDeviceByCtrlId( nCtrlId );
   ASSERT( pDevice != NULL );

   if(pDevice != NULL)  // TAR 418789
   {
      pDevice->OnOposErrorEvent( nResult, nRcEx );
   }

   // always clear all output after an error
   if ( nErrorLocus == OPOS_EL_OUTPUT )
      *pnErrorResponse = OPOS_ER_CLEAR;
   return TRUE;
   }


BOOL CDeviceManagerEx::OnOposDataEvent( 
   UINT nCtrlId, 
   long nStatus )
   {
	// +TAR 374673 
	// Expand fix for TAR 322422 to other event handlers
   if ( m_bShutdownInProgress )
		return TRUE;
    // -TAR 374673 

   CGenericOposIF* pDevice = GetDeviceByCtrlId( nCtrlId );
   ASSERT( pDevice != NULL );

   if(pDevice != NULL)  // TAR 418789
   {
		pDevice->OnOposDataEvent( nStatus );
   }

   return TRUE;
   }

BOOL CDeviceManagerEx::OnOposStatusUpdateEvent( 
   UINT  nCtrlId, 
   long  nStatus )
   {
	// Fix TAR 322422 - Stopping Fastlane from Launchpad causes a Dr.Watson on ADK 4.2.1/ADD 1.08.02 
   if ( m_bShutdownInProgress )
		return TRUE;
   CGenericOposIF* pDevice = GetDeviceByCtrlId( nCtrlId );
   ASSERT( pDevice != NULL );

   if(pDevice != NULL)  // TAR 418789
   {
	   pDevice->OnOposStatusEvent( nStatus );
   }

   return TRUE;
   }

// CDeviceManagerEx::OnOposOutputCompleteEvent
//
// Purpose: Pass the event back to the printer (wrapper) that fired this event
//          The wrapper will then pass the event back to us with the correct DMalike 
//          parameters. 

BOOL CDeviceManagerEx::OnOposOutputCompleteEvent( 
   UINT  nCtrlId, 
   long  nOutputId )
   {
	// +TAR 374673 
	// Expand fix for TAR 322422 to other event handlers
   if ( m_bShutdownInProgress )
		return TRUE;
    // -TAR 374673 

   CGenericOposIF* pDevice = GetDeviceByCtrlId( nCtrlId );
   ASSERT( pDevice != NULL );

   if(pDevice != NULL)  // TAR 418789
   {
		pDevice->OnOposOutputCompleteEvent( nOutputId );
   }

   return TRUE;
   }

BOOL CDeviceManagerEx::OnOposDirectIOEvent( 
   UINT    nCtrlId, 
   long    nEventId, 
   long*   plData, 
   BSTR*   pbstrData )
   {
	// +TAR 374673 
	// Expand fix for TAR 322422 to other event handlers
   if ( m_bShutdownInProgress )
		return TRUE;
    // -TAR 374673 

   CGenericOposIF* pDevice = GetDeviceByCtrlId( nCtrlId );
   ASSERT( pDevice != NULL );

   if(pDevice != NULL)  // TAR 418789
   {
  	  pDevice->OnOposDirectIOEvent( nEventId, plData, pbstrData );
   }

   return TRUE;
}


