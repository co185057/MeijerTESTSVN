/////////////////////////////////////////////////////////////////////////////
//
// scanner.cpp
//
//   Implementation of IDispatch wrapper class for CScannerWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Fastlane/4.0 Patch D/Development/Core/Source/SCOTAPP/dmx/ScannerWrapper.cpp $
//
// $Revision: 4 $
//
// $Date: 6/14/05 3:42p $
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 96/11/15 Initial version.                                    C. Light
//
/////////////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "ScannerWrapper.h"
#include "opos.h"  //SR672

#define COMP_ID  ID_DM		 // used by the trace() calls to identify this component

#define T_ID     _T("DMX")

/////////////////////////////////////////////////////////////////////////////
// CScannerWrapper
//
//
 

BOOL CScannerWrapper::Create( 
   CWnd* pParentWnd, 
   UINT nID )
	{
   return TOPOSScanner<CGenericOposIF>::Create(NULL, 0, CRect(0,0,0,0), pParentWnd, nID );
   }


/////////////////////////////////////////////////////////////////////////////
// CScannerWrapper properties


/////////////////////////////////////////////////////////////////////////////
// CScannerWrapper operations



/* virtual */
void CScannerWrapper::OnOposDataEvent( 
   long  nStatus )
   {
    // +SR93.4
    long nScanType = GetScanDataType();
    long nBinConvOrig = TOPOSScanner<CGenericOposIF>::GetBinaryConversion();

    if ( IsBarcodeTypeEncrypted(nScanType) )
    {
        TOPOSScanner<CGenericOposIF>::SetBinaryConversion(OPOS_BC_DECIMAL);
    }
    // -SR93.4

    CString sScanData = GetScanData();    
   
   CString sData = sScanData;
   CString sScanDataLabel = GetScanDataLabel();
   if ( sScanDataLabel.IsEmpty() == FALSE )
      sData.Format( _T("%s%c%s%c%d"), 
         sScanData,        DM_FS, 
         sScanDataLabel,   DM_FS, 
         nScanType );
   if (m_bBeepWhenScan)
      m_evtBeep.SetEvent();
   PostDmEventToApplication( DMCLASS_SCANNER, m_nDeviceId, sData );
   SetDataEventEnabled( TRUE );

    // +SR93.4
    if ( IsBarcodeTypeEncrypted(nScanType) )
    {
        TOPOSScanner<CGenericOposIF>::SetBinaryConversion(nBinConvOrig);
    }
    // -SR93.4


   m_bVerify = true;  // TAR 445296:  Safety in case note reset in CheckHealth.

   return ;
   }

/* virtual */
void CScannerWrapper::OnOposErrorEvent( 
   long  nResult, 
   long  nRcEx  )
   {
	//long nStatus = DM_ERROR_GENERALFAILURE;
	long nStatus = nResult;
	/* SR672
	switch( nRcEx )
      {
		case NCR_ESCAN_DATALOSS:			//  Not used by opos but still needs to be translated
			nStatus = DM_ERROR_SCAN_DATALOSS;
			break;
		case NCR_ESCAN_BADTAGSUFFIX:		//  Bad suffix byte 
			nStatus = DM_ERROR_SCAN_BADTAGSUFFIX;
			break;
		case NCR_ESCAN_BADTAGPREFIX:		//  Bad prefix byte
			nStatus = DM_ERROR_SCAN_BADTAGPREFIX;
			break;
		case NCR_ESCAN_BADTAG:				//  Missing "30 38" for scanner/scale tag
			nStatus = DM_ERROR_SCAN_BADTAG;
			break;
		case NCR_ESCAN_SERIAL:				//  Serial line error
			nStatus = DM_ERROR_SCAN_SERIAL;
			break;
      default:
         nStatus = MapOposErrorToDM( nResult );
         if ( nStatus == DM_ERROR_UNEXPECTED )
            trace(L6,_T("DMX:OnOposErrorEvent-> SCANNER - %d (%d,%d) Posting DM_ERROR_UNEXPECTED"), m_nDeviceId, nResult, nRcEx );
         break;
      }*/

	//SR672
	switch( nRcEx )
      {
		case NCR_ESCAN_DATALOSS:			//  Not used by opos but still needs to be translated
		case NCR_ESCAN_BADTAGSUFFIX:		//  Bad suffix byte 	
		case NCR_ESCAN_BADTAGPREFIX:		//  Bad prefix byte	
		case NCR_ESCAN_BADTAG:				//  Missing "30 38" for scanner/scale tag	
		case NCR_ESCAN_SERIAL:				//  Serial line error
			nStatus = nRcEx;
			break;	
		default:        
		  nStatus = nResult;    
		if (nStatus == OPOS_E_FAILURE || nStatus == OPOS_E_ILLEGAL || nStatus == OPOS_E_NOEXIST)
            trace(L6,_T("DMX:OnOposErrorEvent-> SCANNER - %d (%d,%d) Posting Error Event"), m_nDeviceId, nResult, nRcEx );
         break;
      }

   PostDmEventToApplication( DM_ERROR, DMCLASS_SCANNER, m_nDeviceId, nStatus );
   return ;
   }

// Purpose: Catch notifications from the remote scanner (wants to connect/has disconnected)
//
// 
/* virtual */
void CScannerWrapper::OnOposDirectIOEvent( 
    long    nEventId, 
    long*   plData, 
    BSTR*   pbstrData )
    {
    PostDmEventToApplication( DM_DIRECTIO, DMCLASS_SCANNER, m_nDeviceId, *pbstrData, nEventId, *plData);
    return;
    }


BOOL CScannerWrapper::GetPacesetterEnabled(
   void )
   {
   return m_bPaceSetterEnabled;
   }

long CScannerWrapper::SetScannerPacesetterEnabled(
   BOOL bNewValue )
   {
	BSTR bstrData = NULL;
   long nType = PACESETTER_DISABLE_MODE_3;
	if ( bNewValue != FALSE )
		nType = PACESETTER_ENABLE_MODE_3;
	long lResult = DirectIO( NCRDIO_SCAN_PACESETTER, &nType, &bstrData );
   if ( lResult == OPOS_SUCCESS )
      m_bPaceSetterEnabled = bNewValue;
   ::SysFreeString( bstrData );
   return lResult;
   }

/* static */
TClassPtr<CGenericOposIF> CScannerWrapper::CreateDevice( 
   HKEY hKey,
   long nDeviceId )
   {
   return new CScannerWrapper( nDeviceId );
   }


/* virtual */
long CScannerWrapper::DoOneTimeInitialization(
   void)
   {
   SetDecodeData(true);               
   SetAutoDisable(true);
   return OPOS_SUCCESS;
   }

int CScannerWrapper::BeepThreadEntry(void *pThis)
{
	CScannerWrapper *p = (CScannerWrapper *)pThis;
	p->BeepThread();
	return 0;
}

void CScannerWrapper::BeepThread()
{
	while (1)
	{
		WaitForSingleObject((HANDLE)m_evtBeep, INFINITE);
		if (m_bBeepThreadExit)
			return;
		//Beep(880, 1000);
		Beep(m_dwFreq, m_dwDuration);
	}

}

bool CScannerWrapper::ReadRegEntryBeepWhenScan()
{
	// tn. Read the BeepScanner from the registry


	HKEY   hMachineKey = HKEY_LOCAL_MACHINE;
	HKEY   hNamesKey;
	DWORD  dwDataSize;
	DWORD  dwDataType; 
	bool bResult = FALSE;

	DWORD dwDefaultFreq = 440;
	DWORD dwDefaultDuration = 20;

	TCHAR szScannerBeep[100]; 

	if (RegOpenKeyEx(hMachineKey, MAKE_BEEP_WHEN_SCAN, 
				   0, KEY_QUERY_VALUE, &hNamesKey) == ERROR_SUCCESS)
	{
		// Read MakeBeep value from registry
		dwDataSize = sizeof(szScannerBeep);
		if(RegQueryValueEx(hNamesKey, _T("MakeBeep"), 0, &dwDataType,  
		 (unsigned char *) szScannerBeep, &dwDataSize) == ERROR_SUCCESS)
		{
			CString strTemp = szScannerBeep;
			strTemp.TrimLeft();
			strTemp.MakeUpper();       
			if(strTemp[0] == 'T' )        
				bResult = TRUE;
		}
	}

	if (bResult)
	{
		  // Read the frequency 
		  dwDataSize = sizeof(DWORD);
		  if(RegQueryValueEx(hNamesKey, _T("SoundFrequency"), 0, &dwDataType, 
							   (unsigned char *) &m_dwFreq, &dwDataSize) != ERROR_SUCCESS)
		  {
				m_dwFreq = dwDefaultFreq;
		  }

		  // Read the duration
  		  dwDataSize = sizeof(DWORD);
		  if(RegQueryValueEx(hNamesKey, _T("SoundDuration"), 0, &dwDataType, 
							   (unsigned char *) &m_dwDuration, &dwDataSize) != ERROR_SUCCESS)
		  {
				m_dwDuration = dwDefaultDuration;
		  }

	}

	RegCloseKey(hNamesKey);
	RegCloseKey(hMachineKey);  

	return bResult;

}

CScannerWrapper::CScannerWrapper( long nDeviceId ) 
   : 
   
   m_bPaceSetterEnabled( FALSE ),
   m_pBeepThread(NULL),
   m_bBeepThreadExit(false),
   m_bBeepWhenScan(FALSE),
   m_dwFreq(440),
   m_dwDuration(25),
   m_bVerify(false)

   {
   m_nDeviceId = nDeviceId;
	m_bBeepWhenScan = ReadRegEntryBeepWhenScan();
	if (m_bBeepWhenScan)
	{
		m_pBeepThread = AfxBeginThread((AFX_THREADPROC)BeepThreadEntry, (void*)this, THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED, 0, NULL);  
		if (m_pBeepThread)
		{
		 m_pBeepThread->m_bAutoDelete = FALSE;
		 m_pBeepThread->ResumeThread();
		}
	}
   GetEncryptedTypes();  //SR 93.4 Gets the Encrypted Barcode Types from the registry.
}

CScannerWrapper::~CScannerWrapper()
{
	if (m_pBeepThread)
	{
		m_bBeepThreadExit = true;
		m_evtBeep.SetEvent();
		WaitForSingleObject(m_pBeepThread->m_hThread, INFINITE);
		delete m_pBeepThread;
	}
}

/* virtual */
long CScannerWrapper::ReadFirmwareVersion( 
   CString& sVersion, 
   long* lVersion )
   {
   BSTR bstrData = NULL;
   long lResult = DirectIO( NCRDIO_SCAN_ROM_VERSION, lVersion, &bstrData );
   if ( lResult  == OPOS_SUCCESS )
      sVersion = bstrData;
   ::SysFreeString( bstrData );
   return lResult;
   }


/**
 * TAR 445296:  Check if the enable/disable succeeds.  If it doesn't, then post
 *              an error message.
 */
void CScannerWrapper::SetDeviceEnabled( BOOL bEnable )
{
    TOPOSScanner<CGenericOposIF>::SetDeviceEnabled(bEnable);

    if(! m_bVerify)
    {
        trace(L4, _T("Scanner %s not verified."),
                  bEnable ? _T("Enable") : _T("Disable"));
        return;     // ERROR already posted or not init.  Don't send again.
    }

    // Check if enable succeeded.
    long lStatus = GetResultCode();
    if(lStatus == OPOS_E_EXTENDED)
    {
        lStatus = GetResultCodeExtended();
    }

    if(lStatus != OPOS_SUCCESS)
    {
        trace(L4, _T("Scanner SetDeviceEnabled rc=%d.  Posting ERROR."), 
              lStatus);
        m_bVerify = false;
        PostDmEventToApplication( DM_ERROR, DMCLASS_SCANNER, m_nDeviceId, 
                                  lStatus);
    }
}

/**
 * TAR 445296:  Use CheckHealth to reset our error-post flag.
 */
long CScannerWrapper::CheckHealth( long lLevel )
{
    long nRC =TOPOSScanner<CGenericOposIF>::CheckHealth(lLevel);

    trace(L6, _T("CheckHealth(%d) returns %d"), lLevel, nRC);

    if(nRC == OPOS_SUCCESS)
    {
        trace(L6, _T("Turning on enable/disable verification."));
        m_bVerify = true;     // No longer in error condition.
    }

    return nRC;
}

// SR 93.4 Gets the Encrypted Barcode Types from the registry.
void CScannerWrapper::GetEncryptedTypes()                            
{
    trace(L6, _T("+CScannerWrapper::GetEncryptedTypes()"));

    m_lEncryptedTypes.clear();

    // read registry
    HKEY hKeyScotApp = NULL;
    CString sKey(_T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\ScotApp"));
    long nRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sKey, 0, KEY_READ, &hKeyScotApp);

    CString sEncryptedTypes;
    if (ERROR_SUCCESS == nRC)
    {

        DWORD lpType = 0;
        DWORD dwDataSize = 0;
        nRC = RegQueryValueEx(hKeyScotApp, 
                            _T("EncryptedBarcodes"), 
                            0, 
                            &lpType, 
                            NULL, 
                            &dwDataSize);

        if( nRC == ERROR_SUCCESS )
        {
          BYTE* lpData = new BYTE[ dwDataSize ];

          nRC = RegQueryValueEx(hKeyScotApp, 
                                _T("EncryptedBarcodes"), 
                                0, 
                                &lpType, 
                                lpData, 
                                &dwDataSize);

          CString sTmp((LPCTSTR)lpData);
          sEncryptedTypes = sTmp;
          delete [] lpData;
        }
    

      RegCloseKey( hKeyScotApp );
    }

    trace(L6, _T("EncryptedBarcodes -- %s"), sEncryptedTypes);

    // tokenize barcode types

    TCHAR* sTmpEncTypes = new TCHAR[sEncryptedTypes.GetLength() + 1];
    wsprintf(sTmpEncTypes, _T("%s"), (LPCTSTR)sEncryptedTypes);
    TCHAR* sEncType = _tcstok(sTmpEncTypes, _T(","));

    while(NULL != sEncType)
    {
        long lEncryptedBarcode = _ttol(sEncType);
        m_lEncryptedTypes.insert(lEncryptedBarcode);
        sEncType = _tcstok(NULL, _T(","));
    }

    delete [] sTmpEncTypes;

    trace(L6, _T("-CScannerWrapper::GetEncryptedTypes()"));

}

// SR 93.4 Checks from the list of Encrypted Barcode Types if the current barcode type is an encrypted type or not.
bool CScannerWrapper::IsBarcodeTypeEncrypted(long lBarcodeType)    
{
    trace(L6, _T("+CScannerWrapper::IsBarcodeTypeEncrypted()"));

    bool bRet = ( m_lEncryptedTypes.end() != m_lEncryptedTypes.find(lBarcodeType) );

    trace(L6, _T("-CScannerWrapper::IsBarcodeTypeEncrypted() returns %d"), bRet);
    return bRet;

}