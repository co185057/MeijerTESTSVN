//*************************************************************************************
//  CustomerTrxBroker.cpp	:	Definition file for the CCustomerTrxBroker class
//*************************************************************************************
//
// CHANGE HISTORY:
//
// CRD 541144 Name: CJ Estrada Date: Jan 7, 2021
// POS312463 Work Request:77937 Name:Alex Bronola Date:Feb. 07, 2017
// POS297533 WorkRequest:76936 Name:Alex Bronola Date:Dec 15, 2016
// POS297191,297192,297213 Work Request:76936 Name:Alex Bronola Date:Oct 17, 2016
// POS296369 Work Request:76936 Name:Alex Bronola Date:Oct 12, 2016
// POS294602/POS294487 Work Request:73595 Name:Alex Bronola Date:Sept 27, 2016
// POS293857 Work Request:73595 Name:Alex Bronola Date:Sept 21, 2016
// POS266899 Work Request:73595 Name:Robert Susanto Date:Aug 25, 2016
// POS223750 Work Request:68941 Name:Robert Susanto Date:Jan 11, 2016
// POS227744 Work Request:68940 Name:Robert Susanto Date:Nov 6, 2015
// POS105314 Work Request:61233 Name: Saiprasad Srihasam Date: August 05, 2014
// POS97215 Work Request:61233 Name:Matt Condra Date:May 20, 2014
// POS81632 Work Request:59313 Name:Matt Condra Date:December 10, 2013
// POS72976 Work Request:58657 Name: Robert Susanto Date: August 22, 2013
// POS29719 Work Request:19114 Name: Matt Condra Date: April 19, 2012
// POS24632 WORK REQUEST:19114 Name: Robert Susanto Date: March 22, 2012
// POS29349 Work Request:19114 Name: Matt Condra Date: March 8, 2012
// POS19072 Work Request:16656 Name: Matt Condra Date: November 23, 2011
// POS22935 WORK REQUEST:16656 Name: Matt Condra Date: October 31, 2011
// POS18041 WORK REQUEST:16656 Name: Matt Condra Date: October 3, 2011
// POS23514 Work Request:16632 Name: Sandeep Shenoy Date: Nov 9, 2011
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date:October 27, 2011
// POS11024 Work Request:16632 Name:Robert Susanto Date:October 25, 2011
// POS11024 Work Request:16632 Name:Sandeep Shenoy Date:October 24, 2011
// POS11024 Work Request:16632 Name: Sandeep Shenoy Date: Jul 6, 2011
// POS6135 Work Request:15196 Name:Robert Susanto Date:April 28, 2011
// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxpriv.h>
#include <winbase.h>
#include <shlwapi.h>
 
#include "CustomerTrxBroker.h"
#include "traceSystem.h"
#include "CustomerTBDefs.h"
#include "customerMHDisplay.h"
#include "CurrentPosState.h"
#include "CustomerCurrentReceipt.h"
#include "CustomerMHOptions.h"
#include "CustomerCurrentTotals.h"
#include "CustomerCurrentCashDrawer.h"
#include "CustomerMHCashDrawer.h"
#include "CustomerMsgMap.h"
#include "CustomerMHItem.h"
#include "customerTBProperties.h"
#include "CustomerMHOperator.h"
#include "CustomerCurrentSlip.h"
#include "CustomerMHReceipt.h"
#include "CustomerMHOperator.h"
#include "SafeArrayVariant.h"

//fix for TAR#306687
#include "CustomerCurrentTransaction.h"


#include "customerCurrentItem.h"
#include "scannerMsg.h"

#include "BaseMessages.h"	//CRD 185620
#include "CustomerTBConnectFL.h"
#define CUSTOMER_TBSTATES
#include "CustomerTBSharedDefs.h"
 
#define T_ID    _T("CCustomerTrxBroker")
const CString START_PIPEEXE(_T("c:\\scot\\config\\PreSCOTStart.bat"));
const CString STOP_PIPEEXE(_T("c:\\scot\\config\\PostSCOTStop.bat")); 
const CString ACS_POS(_T("c:\\acs\\bin\\salesw2k.exe")); 

bool CCustomerTrxBroker::m_bPrePOSStartUp = false;
bool CCustomerTrxBroker::m_bLaunchedPOS = false;
int CCustomerTrxBroker::nIndex = 0;                //POS11024

#pragma comment(lib, "shlwapi.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCustomerTrxBroker::CCustomerTrxBroker()
:m_bInAssistMenu(false)
{
    this->m_bAutoCleanup = false;
	IsItemScrolled(false);
}

CCustomerTrxBroker::~CCustomerTrxBroker()
{
}

DWORD WINAPI SigBitmapMonitorProc(LPVOID pvParam)
{
	CCustomerTrxBroker & trxBroker = *(CCustomerTrxBroker*)pvParam;
	DWORD dwRet = trxBroker.SigBitmapMonitor();
	return dwRet;
}

void CCustomerTrxBroker::TerminateSigBitmapMonitor(DWORD dwTimeout)
{
    tbtraceIO(_T("TerminateSigBitmapMonitor"));

    SetEvent(m_hSigBmpMonStopSignal); // setup exit signal

    CCustomerMHOptions & opts = *CCustomerMHOptions::instance();
    CString csSrc = opts.OptionValue(_T("SigBmpSource")); 

    // write to a temp file to kick off the monitor thread so it can end.
    TCHAR szSource[MAX_PATH] = {0};
    PathCombine(szSource, csSrc, _T(".TermSigBitmap"));
    CFile file;
    if( file.Open(szSource, CFile::modeCreate | CFile::modeWrite) )
    {
        file.SeekToEnd();
        file.Write((LPVOID)"x",1); // make a change in case the file exists to generate
        file.Close();
    }
    else
    {
        tbtrace(TM_WARNING, _T("Failed to write temp file '%s'."), szSource);
    }

    DWORD dwCode = 0;
    WaitForSingleObject( m_hSigBmpMonThread, dwTimeout );
    tbtrace(TM_INFO, _T("Waiting for ACSHookThread to complete..."));
    GetExitCodeThread(m_hSigBmpMonThread, &dwCode);

    // If thread is still active, kill it
    if( STILL_ACTIVE == dwCode )
    {
        tbtrace(TM_WARNING, _T("SigBitmapMonitor thread did not die within %d ms as expected. TerminateThread is being called."), dwTimeout);
        TerminateThread(m_hSigBmpMonThread,-1);
    }
    else
    {
        tbtrace(TM_INFO, _T("SigBitmapMonitor Thread terminated gracefully."));
    }

    // cleanup temp file
    DeleteFile(szSource);
   
    CloseHandle(m_hSigBmpMonThread);
    m_hSigBmpMonThread  = NULL;
	CloseHandle(m_hSigBmpMonStopSignal);
	m_hSigBmpMonStopSignal = NULL;
}

DWORD CCustomerTrxBroker::SigBitmapMonitor()
{    
	tbtrace(TM_INFO, _T("SigBitmapMonitor Thread started.") );
	
	// From ACSOpts.dat
	//
	// [SignatureCapture]
	// SigBmpSource=C:\TEMP\
	// SigBmpTarget=C:\scot\data\
	// SigBmpFilePattern=PAM*.tmp
    //
    CCustomerMHOptions * pOpts = CCustomerMHOptions::instance();
	
    CString csSrc = pOpts->OptionValue(_T("SigBmpSource")); 
	CString csDest = pOpts->OptionValue(_T("SigBmpTarget"));
	CString	csSpec = pOpts->OptionValue(_T("SigBmpFilePattern"));

	CreateDirectory(csDest, NULL);
	HANDLE hDir = CreateFile(
					csSrc,
					GENERIC_READ,
					FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
					NULL,
					OPEN_EXISTING,
					FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED, 
					NULL);

	_ASSERT(hDir != NULL);

	const int SIZE = 10000;
	BYTE byBuffer[SIZE];
	ZeroMemory(byBuffer, SIZE*sizeof(BYTE));
	
	DWORD dwBytes = SIZE;
	BOOL bRet;
	
	do
	{
		bRet = ReadDirectoryChangesW(hDir, 
					(LPVOID)byBuffer,
					SIZE,
					FALSE,
					FILE_NOTIFY_CHANGE_LAST_WRITE,
					&dwBytes,
					NULL,
					NULL);

		if( bRet )
		{
			PFILE_NOTIFY_INFORMATION pfni = (PFILE_NOTIFY_INFORMATION)byBuffer;

			//
			// process all entries in the list
			//
			while( pfni )
			{
				LPTSTR pszFileName = NULL;
				LPBYTE  pbyFileName = NULL; 
				if( pfni->FileNameLength > 0 )
				{
					// Retrieve filename into a string since pfni does not null terminate
					DWORD dwFileNameLength= (pfni->FileNameLength+2)/2;
					pbyFileName = new BYTE[pfni->FileNameLength+2];
					ZeroMemory(pbyFileName, pfni->FileNameLength+2);
					memmove(pbyFileName, pfni->FileName, pfni->FileNameLength);	
					pszFileName = (LPTSTR)pbyFileName;

					if( PathMatchSpec(pszFileName, csSpec) )
					{
						// copy the file from its current directory to a safe one
						TCHAR szSource[MAX_PATH] = {0};
						TCHAR szDst[MAX_PATH] = {0};
						
						PathCombine(szSource, csSrc, pszFileName);
						PathCombine(szDst, csDest, pszFileName);
						tbtrace(TM_INFO, _T("Copying file '%s' to '%s' for preservation."), szSource, szDst);
						CopyFile(szSource, szDst, FALSE);
					}
					delete [] pbyFileName;
					pbyFileName = NULL;
					pszFileName = NULL;
				}
				
				if( pfni->NextEntryOffset == 0 )
				{
					pfni = NULL;
				}
				else
				{
					DWORD dwAddress = (DWORD)pfni;
					dwAddress += pfni->NextEntryOffset;
					pfni = (PFILE_NOTIFY_INFORMATION)dwAddress;
				}
			} 
		}

	} while( WAIT_TIMEOUT == WaitForSingleObject(m_hSigBmpMonStopSignal, 0) );

	CloseHandle(hDir);
	
	tbtrace(TM_INFO, _T("SigBitmapMonitor Thread Ended.") );

	return 0;
}

CString CCustomerTrxBroker::GetBuildFromHistory(void)
{
    tbtraceIO(_T("GetBuildFromHistory"));

    HKEY hKey;
    _TCHAR  szValue[20];
    DWORD dwValueSize=sizeof(szValue);
    CString csHWType = _T("");

	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT - Platform\\ObservedOptions"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("HWType"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
		   csHWType = szValue;
		}
	}
	
    CString csVersion = _T("");
	LPCTSTR pszTokStart = _T("");
	
	//SSCOIA-39228
	//if(csHWType == _T("SS90"))
	//	pszTokStart = _T("___SS90."); // POS266899 change the installation text to ___FastLane.
	//else
		pszTokStart = _T("___SCO."); // POS266899 change the installation text to ___FastLane.
    
    CString csHistFile = _T("C:\\scot\\install\\InstallHistory.log");

    // SCOTAPP should expose access to the GetPartitionPath method without requiring 
    // additional libs and SCOTAPP headers sich as "FastlaneUtils.h"    
    // CString csHistFile = _T("%DATA_DRIVE%\\scot\\install\\InstallHistory.log");
    //GET_PARTITION_PATH(csHistFile, csHistFile.GetBuffer(_MAX_PATH));
    //csHistFile.ReleaseBuffer();

    CStdioFile file;
    CFileException exc;
    
    if( file.Open(csHistFile, CFile::modeRead) )
    {
        // (1) Get the last instance of the __MEIJER token
        CString strVerLine = _T("");
        int iVerPos = -1;
        CString strLine = _T("");
        int iPos = -1;
        while( file.ReadString(strLine) )
        {
			if( !strLine.IsEmpty() && (-1 != (iPos = strLine.Find(pszTokStart))) )
            {
                strVerLine = strLine;
                iVerPos = strLine.Find(pszTokStart);
            }
        }
        file.Close();

        // (2) If matched, find complete version info __MEIJERX.X.X[.X]...
        if( -1 != iVerPos )
        {
            // identify start
            int iStart = iVerPos + _tcslen(pszTokStart);
            // find end
            int iVerLen = strVerLine.GetLength();
            LPCTSTR psz = strVerLine.GetBuffer(0);
            int iEnd = iStart;

            // POS21905/22935 - instead of checking the digit, we need to check the white space for the end of version token - rrs
            while( (iEnd < iVerLen) && ((psz[iEnd] == _T('.')) || (psz[iEnd] != _T(' '))) ) //_istdigit(psz[iEnd])) )
            {
                iEnd++;
            }
            strVerLine.ReleaseBuffer();

            // did we extract a version?
            if( iEnd != iStart )
            {
                csVersion = strVerLine.Mid(iStart, iEnd-iStart);
            }
            tbtrace(TM_INFO, _T("DG Version is:%s"), csVersion);
        }
    }
    return csVersion;
}
            

// Note that this function may be called more than once without uninitialize 
TBRC CCustomerTrxBroker::TBInitialize(HWND hWnd, LPCTSTR szFilePath)
{
    TBRC rc = TB_SUCCESS;
    tbtraceIO(_T("TBInitialize"));
	
    CString csBuildVersion = GetBuildFromHistory();
	if(csBuildVersion != _T(""))
	{
		csBuildVersion = _T("v") + csBuildVersion;
		tbtrace( TM_INFO, _T("build number = %s"), csBuildVersion);
		TBSetProperty(_T("CurrentBuildNumber"), csBuildVersion,false);
	}

//Read from registry to see if lane is a TAB unit
	_TCHAR szTAB[2];
	LONG   lRC;
	HKEY   hKey;
	DWORD  dwDataSize = sizeof(szTAB);
	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT - Platform\\ObservedOptions"), 0, KEY_QUERY_VALUE, &hKey);

	lRC = RegQueryValueEx(hKey, _T("TakeAwayBelts"), NULL, NULL,(LPBYTE)szTAB, &dwDataSize);
	::RegCloseKey(hKey);

	if(!_tcsicmp(szTAB, _T("Y")))
		TBSetProperty(_T("IsTABUnit"), _T("1"),false);
	else
		TBSetProperty(_T("IsTABUnit"), _T("0"),false);

		
    try {
        CSolutionTrxBroker::TBInitialize(hWnd, szFilePath);

		m_hSigBmpMonStopSignal = CreateEvent(NULL, FALSE, FALSE, NULL);

		DWORD dwThreadId = 0;
		m_hSigBmpMonThread = CreateThread(NULL, 0,SigBitmapMonitorProc, (PVOID)this, 0, &dwThreadId);
		_ASSERT(m_hSigBmpMonThread  != NULL);
    }
    catch(...) {
        tbtrace(TM_ERROR,_T("Initialize Failed"));
        return TB_E_NOTINITIALIZED;
    }

    // +TAR 395487 
    if( m_bLaunchedPOS )
    {
        // POS has previously been started and it looks like we're reinitializing after a device 
        // error so run the prescotstart batch file in preparation for the POS To be started.
#ifndef _DEBUG
        LaunchAppAndWait(START_PIPEEXE, 30000, true);
#endif
        m_bLaunchedPOS = false;
    }
    
    try
    {
	    BOOL bConnect = CCustomerTBConnectPOS::instance()->Connect();
	    ASSERT(bConnect);
	    if ( bConnect )
	    {
		    LaunchApp(ACS_POS); // asynchronous launch
            m_bLaunchedPOS = true;
	    }
    }
    catch(...)
    {
        tbtrace(TM_ERROR,_T("Failed to load POS"));
        return TB_E_NOTINITIALIZED;
    }
    // -TAR 395487 
    return rc;
}


TBRC CCustomerTrxBroker::TBUninitialize(void)
{
    TBRC rc = TB_SUCCESS;

    tbtraceIO(_T("TBUninitialize"));

    try {
        TerminateSigBitmapMonitor(2000); // wait before forcing close.
	
        rc = CSolutionTrxBroker::TBUninitialize();
    }
    catch(...) 
    {
        tbtrace(TM_ERROR,_T("Uninitialize Failed"));
        rc = TB_E_UNEXPECTED;
    }

#ifndef _DEBUG
    // cleanup, it's possible we'll be reinitializing if there's a device error.
    LaunchAppAndWait(STOP_PIPEEXE, 30000, true);
#endif

    try
    {
        // CustomerTBConnectPOS must be handled manually since it sets the
        // AutoCleanup flag to false
        tbtrace(TM_INFO,_T("Destroying the CCustomerTBConnectPOS object..."));
	    CCustomerTBConnectPOS* pConnectPOS = CCustomerTBConnectPOS::instance();
        delete pConnectPOS;
    }
    catch(...)
    {
        tbtrace(TM_ERROR, _T("Exception occured while destroying CCustomerTBConnectPOS."));
        rc = TB_E_UNEXPECTED;
    }

    return rc;
}

TBRC CCustomerTrxBroker::TBLaneClosed(const bool bEnter)
{
    tbtraceIO(_T("TBLaneClosed()"));
	TBRC rc = TB_SUCCESS;


    try 
	{
        
		rc = CSolutionTrxBroker::TBLaneClosed(bEnter);

    }catch(...)
	{
		tbtrace(TM_ERROR,_T("Uninitialize Failed"));
        return TB_E_UNEXPECTED;
	}

    return rc;
}

LPCTSTR CCustomerTrxBroker::GetTBSTATEName(long key)
{

    for (int i=0; CustomerTBStates[i].key != TB_MAXCUSTOMERSTATEID; i++) {
        if (CustomerTBStates[i].key == key)
            return CustomerTBStates[i].szname;
    }
    
    return CSolutionTrxBroker::GetTBSTATEName(key);
}

long CCustomerTrxBroker::GetTBSTATEFromName(LPCTSTR szTBState)
{
    
    for (int i=0; CustomerTBStates[i].key != TB_MAXCUSTOMERSTATEID; i++) {
        if (_tcscmp(CustomerTBStates[i].szname, szTBState) == 0)
            return CustomerTBStates[i].key;
    }

    return CSolutionTrxBroker::GetTBSTATEFromName(szTBState);
}

TBRC CCustomerTrxBroker::SendTBCommand(long lCommandID, va_list pArgs)
{
    //tbtraceIO( _T("CCustomerTrxBroker::SendTBCommand()"));

    CString csCurTBState = GetCurrentInProgressStateName();
	TBRC tbrc = TB_SUCCESS;
	bool bHookProperty = TRUE;
	
	BOOL bHandled = false;
    try
    {
        switch(lCommandID)
        {
        case TBCMD_ASSISTMENUSET:
		{ 
			m_bInAssistMenu = va_arg(pArgs, bool);
			bHandled = true;
		}
        case TBCMD_POS_READY:
            {
                CString  csCurTBState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();

                // FastLane now ready to communicate with POS, make sure we have started it
                if (!m_bLaunchedPOS)
                {
                    BOOL bConnect = CCustomerTBConnectPOS::instance()->Connect();
                    ASSERT(bConnect);
                    if ( bConnect )
                    {
                        // Perform All the operations that we need before we launch POS Application.
                        //  For example, print all spooled receipts
                        if(!m_bPrePOSStartUp)
                        {
                            PrePOSStartUp();
                            m_bPrePOSStartUp = true;
                            
                        }
                        LaunchApp(ACS_POS);
                        m_bLaunchedPOS = true;
                    }
                }
                if((csCurTBState == _T("TERM_INIT_READY"))  || 
                   (csCurTBState == _T("SIGNOUT_COMPLETE")) ||  
                   (csCurTBState == _T("TB_CLOSELANE")))
                {
                    return 1;  // 1 true
                }else
                {
                    return 0;  // 0 false
                }
                break;
            }
		case TBCMD_GET_TBPROPERTY:
			bHookProperty = FALSE;
		case TBCMD_GET_HOOKPROPERTY:
		{ 
			CString * pstrProp = va_arg(pArgs, CString*);
			CString * pstrValue = va_arg(pArgs, CString*);
			
			*pstrValue = TBGetProperty(*pstrProp, bHookProperty);
			tbtrace( TM_DEBUG, _T("GETPROPERTY: Property = %s: Value = %s:"),*pstrProp, *pstrValue);
			bHandled = true;
		}
		break; 	
		case TBCMD_TMDISCOUNT:
		{
		    CString *csRetStr;
			csRetStr = va_arg(pArgs, CString* );

			csRetStr ->Empty();
			if (_ttol(TBGetProperty(_T("IsTMD_Only"), true)) == 1)
				csRetStr->Insert(0, TBGetProperty(_T("TMDiscount_Total"), true));
			else
				csRetStr->Insert(0, TBGetProperty(_T("TMDiscount_Total_CM"), true));

			csRetStr->TrimLeft();
			csRetStr->TrimRight();

			if(csRetStr->IsEmpty())
			{
				return 0;
			}   
			else
			{
				return 1;
			}
		}
		case TBCMD_GMTOTAL:
		{
			CString *csRetStr;
			csRetStr = va_arg(pArgs, CString* );

			csRetStr ->Empty();
			// only one of the properties will be nonzero
			//CString csTest = TBGetProperty(_T("GM_Total_Subtotal"), true);
			if (_ttol(TBGetProperty(_T("IsTMD_Only"), true)) == 1)
				csRetStr->Insert(0, TBGetProperty(_T("GM_Total_TMD"), true));
			else if (_ttol(TBGetProperty(_T("IsCM_TMDPresent"), true)) == 1)
				csRetStr->Insert(0, TBGetProperty(_T("GM_Total_TMD_CM"), true));
			else if(_ttol(TBGetProperty(_T("IsCMDPresent"), true)) == 1)
				csRetStr->Insert(0, TBGetProperty(_T("GM_Total_Subtotal_CM"), true));
			else 
				csRetStr->Insert(0, TBGetProperty(_T("GM_Total_Subtotal"), true));
				 
			csRetStr->TrimLeft();   
			csRetStr->TrimRight();
			if(csRetStr->IsEmpty())
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	    case TBCMD_RESET_TMDISCOUNT_GMTOTAL:
        {
			TBSetProperty(_T("GM_Total_Subtotal"), _T("0"),true);
            TBSetProperty(_T("GM_Total_TMD"), _T("0"),true);
            TBSetProperty(_T("TMDiscount_Total"), _T("0"),true);
			// +SSCOI-47114
			TBSetProperty(_T("IsTMD_Only"), _T("0"), true);
			TBSetProperty(_T("IsCM_TMDPresent"), _T("0"), true);
			TBSetProperty(_T("IsCMDPresent"), _T("0"), true);
			// -SSCOI-47114
            return 0;
        }
		case TBCMD_CASHDRAWER_FAILED:
			tbtrace( TM_ERROR, _T("SSF instructed Cash Drawer failure"));
		case TBCMD_CASHDRAWER_CLOSE:
		{
			// Let FastLane control the POS cash drawer
			if ( CCustomerCurrentCashDrawer::instance()->Status() ==
				CCashDrawerMsg::CashDrawerStates::CD_OPEN )
			{
				tbtrace( TM_ERROR, _T("SSF instructed Cash Drawer closure"));
				CCustomerMHCashDrawer::instance()->SetCloseImmediate( true );
			}
			return TB_SUCCESS;
		}  
        case TBCMD_ENTER_OPERATOR_VOID_RESTRICTED_ITEM:
        {

            SetCurrentInProgressState(_T("OPERATOR_VOID_RESTRICTED_ITEM_ENTER"));
            TriggerMsgMap();

            return TB_SUCCESS;
        }

        //SOTF 6657
        case TBCMD_NOTIFY_ACS_REMOTE_ASSISTMODE_STATUS:
        {
            bool bRemote = va_arg(pArgs, bool );

            //Notify ACS that we are entering or leaving remote assistmode
	        if( CCustomerMHOptions::instance()->BoolOptionValue( _T("NotifyACSRemoteAssistMode")) )
	        {		        
		        
		        CCustomerTBConnectPOS::instance()->SendAuxMsgRemoteAssistMode(bRemote);
		        
	        }

            return TB_SUCCESS;

        }

        // POS29349
        case TBCMD_NOTIFY_BROUGHT_BAG:
        {
            bool bBroughtBag = va_arg(pArgs, bool );

            // Notify sales app that customer has or has not their own shopping bag.
            CCustomerTBConnectPOS::instance()->SendAuxMsgBroughtBag(bBroughtBag);

            return TB_SUCCESS;
        }

        // POS29719
        case TBCMD_NOTIFY_EOT:
        {
            bool bEOT = va_arg(pArgs, bool );

            // Notify sales app that transaction is finalized.
            CCustomerTBConnectPOS::instance()->SendAuxMsgEOT(bEOT);

            return TB_SUCCESS;
        }

		// Start POS111525 POS111527 CRD 145555
		case TBCMD_NOTIFY_FLMS:
		{
			CString csAuxMsg= va_arg(pArgs, CString);
		
            // Notify sales app that FLMS processing started iStartEnd = 1 or ended iStartEnd = 0.
			// Until Audit process is moved to SSCO iType and TrustLevel will both equal 0
			CCustomerTBConnectPOS::instance()->SendAuxMsgFLMS(csAuxMsg);
			
			return TB_SUCCESS;
		}
		// End POS111525 POS111527	CRD 145555	
		// start CRD126015
		case TBCMD_NOTIFY_TRANSACTIONMODE:
			{
				int iTransMode = va_arg(pArgs, int);
			
				// Notify sales app the transaction mode (currently SSCO, SnD, FLMS, FLMS + SnD) 
				CCustomerTBConnectPOS::instance()->SendAuxMsgTransMode(iTransMode);
				
				return TB_SUCCESS;
			}
		// End CRD126015
		// Start CRD 541144
		case TBCMD_SEND_CITI_ACCOUNT_DETAILS:
			{
				CString csAuxMsg = va_arg(pArgs, CString);
				
				CCustomerTBConnectPOS::instance()->SendAuxMsgCitiAccountDetails(csAuxMsg);
				
				return TB_SUCCESS;
			}
		// End CRD 541144
        // POS97215
        case TBCMD_GET_OPERATOR_LAST_PWD_CHG_TIME:
        {
            LPCTSTR szOperatorID = va_arg(pArgs, LPCTSTR);
            int * pYear   = va_arg(pArgs, int *);
            int * pMonth  = va_arg(pArgs, int *);
            int * pDay    = va_arg(pArgs, int *);
            int * pHour   = va_arg(pArgs, int *);
            int * pMinute = va_arg(pArgs, int *);
            int * pSecond = va_arg(pArgs, int *);

            // Get timestamp that operator's password was last changed.
            int nRet = CCustomerTBConnectPOS::instance()->ACSGetOperatorLastPwdChgTime(szOperatorID, pYear, pMonth, pDay, pHour, pMinute, pSecond);

            return nRet;
        }
		//Start CRD 142850
		case TBCMD_HASEASITEM:
		{
			tbtrace(TM_INFO, _T("TBCMD_HASEASITEM"));
			CCustomerMHItem::instance()->msgEASItem();
			return true;

		}
		//End CRD 142850
		//Start CRD 185620
		case TBCMD_EASCOMPLETE:
		{
			tbtrace(TM_INFO, _T("TBCMD_EASCOMPLETE"));
			CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_GENERIC_DELAYED_COMPLETE, NULL, NULL);
			return true;
		}
		//End CRD 185620

		//Start CRD 232268
		case TBCMD_GET_EASLISTSIZE:
		{			
			int * pnValue = va_arg(pArgs, int*);
			
			* pnValue = CSolutionTBConnectPOS::csEASEntryIDList.GetSize();

			return TB_SUCCESS;
		}
		case TBCMD_GET_EASENTRYID:
		{
			int * pIndex = va_arg(pArgs, int*);
			CString * pstrValue = va_arg(pArgs, CString*);
			
			*pstrValue = CSolutionTBConnectPOS::csEASEntryIDList.GetAt(*pIndex);

			return TB_SUCCESS;
		}
		case TBCMD_CLEAR_EASLIST:
		{
			CSolutionTBConnectPOS::csEASEntryIDList.RemoveAll();
			return true;
		}
		case TBCMD_GET_MOBILEASSISTANCENEEDED:
		{
			int * pnValue = va_arg(pArgs, int*);
			
			* pnValue = CSolutionTBConnectPOS::lMobileAssistanceNeeded;
			
			return TB_SUCCESS;
		}
		case TBCMD_CLEAR_MOBILEASSISTANCENEEDED:
		{
			CSolutionTBConnectPOS::lMobileAssistanceNeeded = 0;
			return true;
		}
		case TBCMD_HASMOBILEASSISTANCENEEDED:
		{
			CCustomerMHItem::instance()->msgMobileGenericDelayed();
			return true;
		}
		case TBCMD_FLMS_TRANSFERRING:
		{
			bool pnValue = va_arg(pArgs, bool);
			CCustomerMHItem::isFLMSTransferring = pnValue;

			return TB_SUCCESS;
		}
		//End CRD 232268
        // POS81632
        case TBCMD_CHECKINOUT_OPERATOR:
        {
            int nFunc = va_arg(pArgs, int);
            LPCTSTR szOperatorID = va_arg(pArgs, LPCTSTR);
            LPCTSTR szOperatorPw = va_arg(pArgs, LPCTSTR);
            int nNewRAPid = va_arg(pArgs, int);
            int * currentRAPid = va_arg(pArgs, int *);
            int * offlineStatus = va_arg(pArgs, int *);

            // Check in/out operator.
            int nRet = CCustomerMHOperator::instance()->ACSCheckInOutOperator(nFunc, szOperatorID, szOperatorPw, nNewRAPid, currentRAPid, offlineStatus);    //POS105314
                

            return nRet;
        }

        // POS81632
        case TBCMD_NOTIFY_CHECKINOUT:
        {
            LPCTSTR szOperatorID = va_arg(pArgs, LPCTSTR);
            LPCTSTR szTimestamp = va_arg(pArgs, LPCTSTR);
            int nFunc = va_arg(pArgs, int);
            int nRAP = va_arg(pArgs, int);
            int nVRAP = va_arg(pArgs, int);
            int nOffline = va_arg(pArgs, int);

            // Notify sales app that RAP Attendant check in/out needs to be tlog'd.
            CCustomerTBConnectPOS::instance()->SendAuxMsgTLOGRAPTrack(szOperatorID, szTimestamp, nFunc, nRAP, nVRAP, nOffline);

            return TB_SUCCESS;
        }

		case TBCMD_HHSCANNER_ACTIVE:
			{
				LPCTSTR szOperatorID = va_arg(pArgs, LPCTSTR);
				LPCTSTR szTimestamp = va_arg(pArgs, LPCTSTR);
				int hhScannerActive = va_arg(pArgs, int);
				
				// Notify sales app that barcode data is coming from hand held scanner
				CCustomerTBConnectPOS::instance()->SendAuxMsgHHScannerActiveFlag(hhScannerActive);
				
				return TB_SUCCESS;
			}

		case TBCMD_ENTER_ASSISTMODE:
		{
			bool bRemote = va_arg(pArgs, bool );
			SetInRemoteAssistMode( bRemote );
			//check if we were about to finish a trxn before we blindly change states
			if(csCurTBState == _T("EXIT_TRAINING_IP"))
			{
				SetCurrentInProgressState(_T("ASSISTMODE_EXIT_TRAINING_IP"));

			}
			else if(csCurTBState == _T("ENTER_TRAINING_IP"))
			{   
				SetCurrentInProgressState(_T("ASSISTMODE_ENTER_TRAINING_IP"));

			}
			else if(csCurTBState == _T("TB_FINISH_FLAPI"))
			{
				SetCurrentInProgressState(_T("ASSISTMODE_FINISH_FLAPI"));
			}
			else if ( csCurTBState == _T("ASSISTMODE_TRXEND") )
			{
				tbrc = TB_SUCCESS_IMMEDIATE;
			}
			else if ( csCurTBState == _T("ASSISTMODE_TRXEND_SIGSLIP") ) 
			{
				TBSTATE tbRet = TB_IGNORE;
				CCustomerMsgMap::instance()->msgAssistModeTrxEnd( tbRet, NULL );
				if ( tbRet != TB_IGNORE )
				{
					CCustomerTBScotMsg::instance()->SendReturnStateMsg( tbRet, NULL, NULL );
				}
			}
			/*else if(csCurTBState == _T("TENDER_CASH") && TBGetProperty(_T("IsPromptForServiceDeskCode"), true) != _T("1"))
			{
                 SetCurrentInProgressState(_T("ASSISTMODE_TENDER_CASH"));
			}*/
			else
			{
				SetCurrentInProgressState(_T("ASSISTMODE_ENTER"));
			}
			TriggerMsgMap();
		}
		return tbrc;

		//fix for TAR#306687
        //SOTF 6226
        case TBCMD_EXIT_OPERATOR_VOID_RESTRICTED_ITEM:
        case TBCMD_EXIT_ASSISTMODE:
	    {
			CString  csCurTBState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
			CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
			CString strFromRTS = pCurTrx->TBGetProperty(_T("PropFromRTS"), true);
			CCustomerCurrentTransaction *pCurrTrx = CCustomerCurrentTransaction::instance();


			SetInRemoteAssistMode( false);
            SetLimitedAssistMode( false );
			CString csFLState = TBGetProperty(_T("FLState"));
			if (csFLState == _T("TENDER"))
			{
				if(csCurTBState == _T("ASSISTMODE_TENDER"))
				{   
				    SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
				    TriggerMsgMap();
				    return 1;
				}
			}
            if( (csCurTBState != _T("ASSISTMODE_TRXEND")) &&
				(csCurTBState != _T("ASSISTMODE_TRXEND_SIGSLIP")) &&
				(csCurTBState != _T("TB_FINISH_FLAPI")) && 
				(csCurTBState != _T("ITEMIZATION")) &&
				(csCurTBState != _T("TB_CLOSELANE")) ) //TAR348914
            {

				CCustomerMHOptions* pOpts = CCustomerMHOptions::instance();
                CString csFLConfig = pOpts->OptionValue(CString(_T("FastLaneConfiguration")));
				
				if(strFromRTS == _T("1") && FALSE == pCurrTrx->DirtyTrx() && csFLConfig == _T("Standard"))
				{
					pCurTrx->TBSetProperty(_T("PropFromRTS"), _T("0"), true);
					if(csCurTBState == _T("ASSISTMODE_ENTER"))
					{
						CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_NEED_SCANANDBAG);
					}
					else
					{
						CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_TRXEND);
					}
				}



                SetCurrentInProgressState(_T("ASSISTMODE_EXIT"));
                TriggerMsgMap();
                return 1;
            }
            return 0;
		}
		//end of fix

		case TBCMD_NOTIFY_EXIT_STOREMODE_TO_SCANANDBAG:
		{
			CCustomerTBConnectPOS::instance()->SendAuxMsgNotifyExitStoreMode("ScanAndBag");
			return TB_SUCCESS;
		}

		case TBCMD_NOTIFY_EXIT_STOREMODE_TO_PAYMENT:
		{
			CCustomerTBConnectPOS::instance()->SendAuxMsgNotifyExitStoreMode("Payment");
			return TB_SUCCESS;
		}
		//End CRD 366323

		//Start CRD 321191
		case TBCMD_NOTIFY_ES_ALERT_CANCELED:
			{
				CCustomerTBConnectPOS::instance()->SendAuxMsgNotifyCancelAlert();
				return TB_SUCCESS;
			}
		//END CRD 321191
		//Start CRD 338206
		case TBCMD_NOTIFY_STOREMODE_PROMPT:
			{
				CCustomerTBConnectPOS::instance()->SendAuxMsgNotifyStoreModePrompt();
				return TB_SUCCESS;
			}
		//End CRD 338206
		case TBCMD_NOTIFY_SCOLOADCONFIRMATION:
			{
				CCustomerTBConnectPOS::instance()->SendAuxMsgNotifySCOLoadConfirmation();
				return TB_SUCCESS;
			}
		case TBCMD_ENTERING_FINISH_AND_PAY:
        {
			bool bDriveToPayment = va_arg(pArgs, bool);
			if( bDriveToPayment )
            {
				SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN_DRIVE_FL"));
				TriggerMsgMap();
            }
            else
            {
                //POS312463
			    CString	csCurrentTBState = GetCurrentInProgressStateName();
				tbtrace( TM_INFO, _T("CurrentProgressState = %s."), csCurrentTBState); 
				if(csCurrentTBState != _T("TB_ENTERTENDERSTATE_IN"))
				{
					tbtrace( TM_INFO, _T("Setting CurrentProgressState to TB_ENTERTENDERSTATE_IN."));
                    SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
					TriggerMsgMap();
				}
            }            
            return true;
            
			//SMarks commented out this code.  The problem is when the customer presses
			//finish and pay and if TB was not in the right state, the system would appear hung
			//finish and pay from scan and bag (ssf override) returns in progess and waits
			//for the tb to make FL go to finish.

			/*CString csCurrentTBState= GetCurrentInProgressStateName();               
			CCustomerCurrentTotals *pTotal = CCustomerCurrentTotals::instance();

            CCurrentPosState &PosState = *CCurrentPosState::instance();
			_bstr_t bstr = PosState.GetValue(_T("PosState"));
			CString csCurPosState = (LPCTSTR)bstr;
			csCurPosState.TrimLeft();
			csCurPosState.TrimRight();

            bool bDriveToPayment = va_arg(pArgs, bool);
			
			CString csTotal = pTotal->TotalDue();
			tbtrace( TM_INFO, _T("Total = %s"),csTotal);

            if(csCurrentTBState == _T("ITEMIZATION") && (bDriveToPayment || (csTotal != _T("0") && csTotal != _T(""))) )
            {
				if( (-1 != csCurPosState.Find(_T("ITEMMENU"))) || (-1 != csCurPosState.Find(_T("PROCESSING"))) )
                {
					tbtrace( TM_INFO, _T("FastLane is entering Finish & Pay, driving TB to do the same."));
				}
                else
                {
                    tbtrace( TM_WARNING, _T("Fastlane & POS are at Finish & Pay Pay while TB is in itemization."));
                }

                if( bDriveToPayment )
                {
					SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN_DRIVE_FL"));
                }
                else
                {
                    SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
                }
                TriggerMsgMap();
                return true;
			}

            return 0; */
		}

		case TBCMD_ENTERING_WIC_FREE_ITEM_PHASE: // POS19072
		{
			// Make sure the ‘WIC Free Item’ softkey on the sale app's Item Menu
			// is pressed so that the Free Item phase is activated.
			SetCurrentInProgressState(_T("TB_ENTERING_WIC_FREE_ITEM_PHASE"));
			TriggerMsgMap();
			return true;
		}

		case TBCMD_RESET_TO_LAST_POSSTATE:
		{	
			CCustomerMsgMap::instance()->resetToLastPOSState();
			bHandled = true;
			return 1;
		}
		case TBCMD_GET_POSSTATE:
        {
            bHandled = true;
            CString csCurrState;
            CCurrentPosState &PosState = *CCurrentPosState::instance();
            _bstr_t csCurPosStateBstr= PosState.GetValue(_T("PosState"));
            CString csCurPosState = (LPCTSTR)csCurPosStateBstr;

			CString *csRetStr;
            csRetStr = va_arg(pArgs, CString* );
            
            csRetStr ->Empty();
            csRetStr->Insert(0, csCurPosState);

            csRetStr->TrimLeft();
            csRetStr->TrimRight();
            return 1;

            break;
		}
		case TBCMD_VALIDATE_SUPERVISOR:
		{
			bHandled = true;
			CString csID, csPwd;
			csID = va_arg(pArgs, CString );
			csPwd = va_arg(pArgs, CString );

			if(csID.IsEmpty() || csPwd.IsEmpty())
			{
				return 0;
			}

			return (CCustomerMHOperator::instance()->ValidateSupervisor(csID, csPwd) ? 1:0);

		}
		//added here to add detection of TenderTypeSelected. this is to serve both
		// with and without streamline electronic payment feature.
		case TBCMD_GET_CUSTOMER_SELECTED_TENDER:
        {
			CString *csRetStr;
            csRetStr = va_arg(pArgs, CString* );
            
            csRetStr ->Empty();
            csRetStr->Insert(0, TBGetProperty(_T("TenderType"), true));
            csRetStr->TrimLeft();
            csRetStr->TrimRight();

			if(csRetStr->IsEmpty())
			{
				csRetStr->Insert(0, TBGetProperty(_T("TenderTypeSelected"), true));
				csRetStr->TrimLeft();
				csRetStr->TrimRight();
			}

            if((csRetStr->IsEmpty()) || (csRetStr->Find(_T("Dept")) != -1 ))
            {
                return 0;
            }
			else
			{
				//don't tell FL we have a tender until we're in the right state
                CString csCurrTBState = GetCurrentInProgressStateName();
                if((-1 == csCurrTBState.Find(_T("TB_ENTERTENDERSTATE_IN"))) && (csCurrTBState != _T("ITEMIZATION")))
					return 1;
                return 0;
            }
		}
		case TBCMD_RESUME_ITEMS:
		{
			tbtrace( TM_INFO, _T("TBCMD_RESUME_ITEMS:+")); // POS18041
			TBSetProperty(_T("IsResumeTrxInProgress"), _T("1"), true);
			TBSetProperty(_T("IsResumeComplete"), _T("0"), true);
         //POS11024: Mobile Shopper: Moved static declare of nIndex to CustomerTrxBroker.h
         //nIndex will be initialized at start of this class and in ResetTransactionDetails()
			//static int nIndex = 0;            //POS11024         

			CCustomerCurrentItem* pCurrItem = CCustomerCurrentItem::instance();
			pCurrItem->ResetItemDetails();
			
			static int nArraySize = 0;         
			if(nIndex == 0) //call only the getsize once
				nArraySize = (int)pCurrItem->m_SItemArray.size();

			if(nIndex == nArraySize)
			{
				tbtrace( TM_INFO, _T("m_SItemArray cleared")); // POS18041
				pCurrItem->m_SItemArray.clear();
				nIndex = 0;
            //POS11024:Reset this flag so that alcohol scanned after resume is complete
            //displays on Scan and Bag correctly and prompts for Age restriction.
            CSolutionTBConnectPOS::ms_bIsItemsNeedTobeBuffered = false;       
				return 0;
			}

			tbtrace( TM_INFO, _T("resuming item no. %d of %d to receipt"),nIndex + 1, nArraySize);

			CItemDetailsMsg pItemMsg;

			CTBGenMsg *pMsg = pCurrItem->m_SItemArray[nIndex];//.GetAt(*nIndex);
			nIndex++;

			pMsg->SetMsgVersion(VERSION_GENOBJ_BIN);
			int nLength = 0;
            
			BYTE* pBuf = pMsg->Serialize( nLength );

         //POS23514:Mobile Shopper-Start
         TBSTATE rc;
         if(pMsg->GetTBMsgId() == TBMSG_REWARD)
         {
            //If this is a CM reward, then call ProcessRewardMessage and exit
            CCustomerMHItem::instance()->ProcessRewardMessage(pMsg);            
				CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_RESUME_ACS_REWARD, NULL, NULL);
            delete[] pBuf;
            return 1;
         }
         
         pItemMsg.Parse( pBuf);
         delete[] pBuf;

			ASSERT(&pItemMsg);
			if (&pItemMsg) {
				pCurrItem->Refresh(&pItemMsg);
			}        
         
			rc = CCustomerMHItem::instance()->ProcessMessage(&pItemMsg);
         if ( TB_ITEMSOLD == rc )
         {
             //SOTF 6822
             rc = TB_IGNORE;
			   CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_RESUME_ITEMS, NULL, NULL);
         }                  
         //POS23514:Mobile Shopper-End
			return 1;
		}
        case TBCMD_TLOG_OVERRIDE:
        {	
            CString csManager = va_arg(pArgs, CString);
            long lOverride = va_arg(pArgs, long);
            long lLocation = va_arg(pArgs, long);
            long lWaitTime = va_arg(pArgs, long);
            long lTerminal = va_arg(pArgs, long);
            long lWaitTimeMs = va_arg(pArgs, long);  //SOTF 7242 
            // send a TLOG override/intervention entry to POS
            CCustomerTBConnectPOS::instance()->SendAuxMsgTLOGOverride(
                    csManager, lOverride, lLocation, lWaitTime, lTerminal, lWaitTimeMs
                    );

            return TB_SUCCESS;
        }
        
		//+SOTF 5572
		case TBCMD_ROBOT_CASH_FLOW:
		{
			
			if( CCustomerMHOptions::instance()->BoolOptionValue( _T("ReportCurrencyToACS")) )
			{
				CString csMediaReport = va_arg(pArgs, CString);
				CCustomerTBConnectPOS::instance()->SendAuxMsgRobotCashFlow(csMediaReport);
			}
			return TB_SUCCESS;
	
		}
		//-SOTF 5572
		case TBCMD_SEND_ASSISTMENU_KEY_PRESS:
		{
			TBRC retCode = SendKeyedData(pArgs);
			if(retCode == TB_SUCCESS)
			{
				CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEEDASSISTMODE, NULL, NULL); 
				return 1;
			}
			return 0;
		}
		// Send PreSelected Tender Type to POS
		case TBCMD_SEND_PRESELECTED_TENDER:
		{
            long lPreSelectedTender = va_arg(pArgs, long);

			if( CCustomerMHOptions::instance()->BoolOptionValue( _T("PreSelectedTenderEnabled")) )
			{				
				CCustomerTBConnectPOS::instance()->SendAuxMsgPreSelectedTender(lPreSelectedTender);                
			}


            //POS223750 - EMV - rrs
            /* // POS227744 Prototype EMV - rrs
            tbtrace(TM_INFO, _T("TBCMD_SEND_PRESELECTED_TENDER=%d"), lPreSelectedTender);
            if( lPreSelectedTender == TB_PRESELECTED_TENDER_CREDIT )
            {
                CString csKey(_T("{PAYMENT}{F8}{F8}{F3}"));
                tbtrace(TM_INFO, _T("TBCMD_SEND_PRESELECTED_TENDER, SendKey=%s"), csKey);
                CCustomerTBConnectPOS::instance()->SendSequence(csKey);               
            }
            // e - POS227744 Prototype EMV - rrs */
            //e - POS223750 - EMV - rrs

			return TB_SUCCESS;
		}
		case TBCMD_MICR_SHOULD_EJECT:
        {
            bool bEject = CCustomerCurrentSlip::instance()->ShouldEjectSlipAfterMicr();
            return bEject ? TB_SUCCESS_IMMEDIATE : TB_SUCCESS;
        }
        case TBCMD_SLIP_SHOULD_EJECT:
        {
            bool bJustFlippedToBack = va_arg(pArgs, bool);
            bool bEject = CCustomerCurrentSlip::instance()->ShouldEjectSlipBeforePrint(bJustFlippedToBack);
            return bEject ? TB_SUCCESS_IMMEDIATE : TB_SUCCESS;
        }
        case TBCMD_SLIP_SHOULD_FLIP:
        {
            bool bIsCurrentSlipSideFront = va_arg(pArgs, bool);
            bool bFlip = CCustomerCurrentSlip::instance()->ShouldFlipSlipBeforePrint(bIsCurrentSlipSideFront);
            return bFlip ? TB_SUCCESS_IMMEDIATE : TB_SUCCESS;
        }
        case TBCMD_SLIP_SHOULD_FLIP_AFTER_MICR:
        {
            bool bFlip = CCustomerCurrentSlip::instance()->ShouldFlipSlipAfterMicr();
            return bFlip ? TB_SUCCESS_IMMEDIATE : TB_SUCCESS;
        }
        case TBCMD_GET_REBATE_RECEIPT_COUNTS:
        {
            CCustomerCurrentTransaction & trx = *CCustomerCurrentTransaction::instance();
            int * piItemCount = va_arg(pArgs, int*);
            int * piRcptCount = va_arg(pArgs, int*);

            if( piItemCount )
            {
                *piItemCount = trx.CountItemsWithRebateReceipts();
            }
            if( piRcptCount )
            {
                *piRcptCount = trx.CountTotalRebateReceipts();
            }
            return TB_SUCCESS;
        }
		case TBCMD_GETDISPLAYXML:
		{
		//TAR376012: Dr. Watson in SCOTAPPU.exe, Funct: ntdll!RtlSizeHeap
		//Eliminate using new and delete to prevent heap corruption.
		//Override GetDisplayXML()

			bHandled = true;
			CString* csDisplay = va_arg(pArgs, CString*);
			*csDisplay = TBGetProperty(_T("POSDisplay"));
			tbtrace(TM_ERROR, _T("XML=%s"), (LPCTSTR)*csDisplay);
			break;
        }
		case TBCMD_ADD_BITMAP_REQUEST:
		{
			//+TAR 371923
			CSolutionMultiBitmap & mbmp = CCustomerMHReceipt::instance()->GetMultiBitmap();
			mbmp.AddRecentUpdatedBitmaps();

			return TB_SUCCESS;
			//-TAR 371923
		}
		case TBCMD_WEIGHTITEMS_NOT_ALLOWED:
		{
			//Let the TBMsg map display customer message and skip selling weight required item
			CCustomerTrxBroker::instance()->SetCurrentInProgressState(_T("ITEMIZATION_WEIGHTITEM_NA"));
			TriggerMsgMap();
			return TB_SUCCESS;
		}
        case TBCMD_BIOMET_DATA:
        {
            LPCTSTR szEncodedData = va_arg(pArgs, LPCTSTR);
            
            try
            {
                // Decode Base64 encoded string and package into a byte array
                MSXML2::IXMLDOMDocumentPtr spDoc(__uuidof(MSXML2::DOMDocument40));
                
                MSXML2::IXMLDOMElementPtr spElem = spDoc->createElement(_T("b64"));
                spDoc->appendChild(spElem);
                spElem->PutdataType(_T("bin.base64"));
                spElem->text = szEncodedData;
                _variant_t vData = spElem->nodeTypedValue;
                
                ULONG lnBytes; 
                BYTE* pBuffer= CSafeArrayVariant::GetNewBufferFromVariant(vData, lnBytes);
                
                CString sBuffer = _T("");
                
                // convert buffer to hex encoded string
                for(int i = 0; i < lnBytes; i++)
                {
                    // high order nibble
                    int high = (pBuffer[i] & 0xF0) >> 4;
                    if( 0 <= high && high <= 9 )
                    {
                        sBuffer += (TCHAR)(_T('0') + high);
                    }
                    else
                    {
                        sBuffer += (TCHAR)(_T('A') + (high-0xA));
                    }
                    
                    // low order nibble
                    int low = (pBuffer[i] & 0x0F);
                    if( 0 <= low && low <= 9 )
                    {
                        sBuffer += (TCHAR)(_T('0') + low);
                    }
                    else
                    {
                        sBuffer += (TCHAR)(_T('A') + (low-0xA));
                    }
                }
                
                delete [] pBuffer; 
                pBuffer = NULL;
                
                // send 250 character segments to ACS and decrement counter
                const int iFieldSize = 250;
                int iSegments = sBuffer.GetLength() / iFieldSize; 
                if( sBuffer.GetLength() % iFieldSize )  iSegments += 1;
                
                tbtrace(TM_INFO, _T("Full Biometric Data to send to POS: (len:%03d) %s"), sBuffer.GetLength(), (LPCTSTR)sBuffer);
                
                for(i = 0; i < iSegments; i++)
                {
                    CString sCount;
                    sCount.Format(_T("%d"), iSegments-1-i);
                    CString sPart = sBuffer.Mid(i*iFieldSize, iFieldSize);
                    tbtrace(TM_INFO, _T("Sending Biometric Data part %s: (len:%03d) %s"), (LPCTSTR)sCount, sPart.GetLength(), (LPCTSTR)sPart);
                    TBSetProperty(_T("PropBiometricData"), sBuffer.Mid(i*iFieldSize, iFieldSize), true);
                    TBSetProperty(_T("PropBiometricCount"), sCount, true);
                    CCustomerTBConnectPOS::instance()->SendInput((LPARAM)_T("ACSIO_BIOMETRIC_ENTERED"));
                }
            }
            catch(_com_error & cex)
            {
                tbtrace(TM_ERROR, _T("Error occured processing biometric data: %0x08X - %s"), cex.Error(), cex.ErrorMessage());
            }
            
            return TB_SUCCESS;
        }
        case TBCMD_IS_EMPLID_ENABLED:
        {
            if( CCustomerMHOptions::instance()->BoolVal(
                    CCustomerTBProperties::instance()->GetProperty(_T("AllowEmplIdSignOn"))) )
            {
                tbtrace(TM_INFO, _T("EMPLID signon is enabled in the POS."));
                return TB_SUCCESS_IMMEDIATE;
            }
            else
            {
                return TB_SUCCESS;
            }
        }
        case TBCMD_IS_EMPLID_ALLOWHANDKEY:
        {
            if( CCustomerMHOptions::instance()->BoolVal(
                    CCustomerTBProperties::instance()->GetProperty(_T("AllowHandKeyEMPLID"))) )
            {
                tbtrace(TM_INFO, _T("Hand keying if EMPLID is permitted by the POS."));
                return TB_SUCCESS_IMMEDIATE;
            }
            else
            {
                tbtrace(TM_INFO, _T("Hand keying if EMPLID is NOT permitted by the POS."));
                return TB_SUCCESS;
            }
        }
        case TBCMD_IS_BIOMET_ENABLED:
        {
            if( CCustomerMHOptions::instance()->BoolVal(
                    CCustomerTBProperties::instance()->GetProperty(_T("UseBiometrics"))) )
            {
                tbtrace(TM_INFO, _T("Biometrics are enabled in the POS."));
                return TB_SUCCESS_IMMEDIATE;
            }
            else
            {
                return TB_SUCCESS;
            }            
        }
        case TBCMD_IS_MEIJER_BADGE_GET_BARCODE:
        {
            CString * psData = va_arg(pArgs, CString*); // full scan data
			CString * psCode = va_arg(pArgs, CString*); // barcode to return
            TBRC rc = TB_SUCCESS;
            *psCode = _T("");

            if( CCustomerMHOperator::instance()->VerifyIsEmployeeBadgeAndGetBarcode(*psData, *psCode) )
            {
                tbtrace(TM_INFO, _T("The barcode scanned was an Employee Badge."));
                return TB_SUCCESS_IMMEDIATE;
            }
            else
            {
                return TB_SUCCESS;
            }
        }
        case TBCMD_SIGNONBUILDER_TOSTRING:
        {
            LPCTSTR pszUserId = va_arg(pArgs, LPCTSTR);
            LPCTSTR pszUserPwd = va_arg(pArgs, LPCTSTR);
            LPCTSTR pszBiometData = va_arg(pArgs, LPCTSTR);
            CString * psPwdString = va_arg(pArgs, CString *);
            
            bool bScanned = CCustomerMHOperator::instance()->IsEmployeeBadgeCode(pszUserId);
            SignOnBuilder sob(pszUserId, pszUserPwd, bScanned, pszBiometData);
            *psPwdString = sob.ToString();

            return TB_SUCCESS;
        }
        case TBCMD_GET_TBSTATENAME:
        {
            TBSTATE stateId = va_arg(pArgs, TBSTATE);
            CString * pcsStateName = va_arg(pArgs, CString *);
            *pcsStateName = CCustomerTrxBroker::instance()->GetTBSTATEName(stateId);
            return TB_SUCCESS;
        }
        case TBCMD_IS_TRXN_ENDING:
		{
			 _bstr_t bstrPosState = CCurrentPosState::instance()->GetValue(_T("PosState"));
			CString csPosState = (LPCTSTR)bstrPosState;
			if ((csCurTBState == _T("TB_FINISH_FLAPI")) ||
				(csCurTBState == _T("ASSISTMODE_TRXEND")) || 
				(csPosState == _T("POS_TRXEND")))
			{
				return 1;
			}
			return 0;
		}        
        // +SOTF 6312
        case TBCMD_GET_EMPLID_FROM_BADGE:
        {
            LPCTSTR pszBadge = va_arg(pArgs, LPCTSTR);
            CString * pcsEmplid = va_arg(pArgs, CString *);
            *pcsEmplid = CCustomerTBConnectPOS::instance()->GetEmplidFromBadge(pszBadge);

            return *pcsEmplid != _T("") ? TB_SUCCESS_IMMEDIATE : TB_SUCCESS;
        }
        case TBCMD_IS_MEIJER_BADGE:
        {
            LPCTSTR pszUserId = va_arg(pArgs, LPCTSTR);
            bool bBadge = CCustomerMHOperator::instance()->IsEmployeeBadgeCode(pszUserId);
            return bBadge ? TB_SUCCESS_IMMEDIATE : TB_SUCCESS;
        }
        // -SOTF 6312

        //SOTF 6226
        case TBCMD_ACS_DELAYED_AGE_RESTRICTION:
        {
            bool bDelayedIntervention = CCustomerMHItem::instance()->GetACSDelayedAgeRestricted();
            return bDelayedIntervention ? 1 : 0;

        }
        case TBCMD_HIGHEST_AGE_REQUIRED:
        {

            CString *csRetStr;
            CString csAge;

            csAge.Format(_T("%d"), CCustomerMHItem::instance()->GetHighestAgeRequired());
            csRetStr = va_arg(pArgs, CString* );

            csRetStr ->Empty();
            csRetStr->Insert(0, csAge);
            csRetStr->TrimLeft();
            csRetStr->TrimRight();

            if((csRetStr->IsEmpty()) || (csAge == _T("0")))
            {
             return 0;
            }   
            else
            {
             return 1;
            }
        }
        case TBCMD_SET_DOB_ENTERED_STATUS:
        {
            CCustomerMHItem::instance()->SetAgeRestrictionEntry( va_arg(pArgs, long ));            
            return TB_SUCCESS;
        }

        case TBCMD_SET_AGE_RESTRICTION_ITEM_ALLOWED_STATUS:
        {
            CCustomerMHItem::instance()->SetAgeRestrictionItemAllowedStatus( va_arg(pArgs, long ));            
            return TB_SUCCESS;
        }

        case TBCMD_SEND_AGE_VERIFY_DATA:
        {
            int ageEntry; 
            CString csCustomerDOB;
            CString csUPC;
            int ageRequired;
            int itemAllowed;
            int nCustomerAge;

            CCustomerMHItem::instance()->GetAgeVerifyData(ageEntry, &csCustomerDOB, &csUPC, ageRequired, itemAllowed, nCustomerAge);
            CCustomerTBConnectPOS::instance()->SendAuxMsgAgeVerifyData(ageEntry, csCustomerDOB, csUPC, ageRequired, itemAllowed, nCustomerAge);           

            return TB_SUCCESS;
        }
        // -sotf
		//POS11024:Mobile Shopper (Added generic appini read function that can be called for other features too)
		case TBCMD_GET_APPINI_ENTRY_VAL:
		{
			bHandled = true;
			CString csSection, csEntry;
			CString *csRetStr;
			csSection = va_arg(pArgs, CString );
			csEntry = va_arg(pArgs, CString );
			csRetStr = va_arg(pArgs, CString* );

			if(csSection.IsEmpty() || csEntry.IsEmpty())
			{
				return 0;
			}
			*csRetStr = CCustomerMHOptions::instance()->AppIniOptionValue (csSection, csEntry);
			tbtrace(TM_INFO, _T("Appini section=%s, Entry=%s, Value=%s"), csSection, csEntry, *csRetStr);
			return TB_SUCCESS;
		}

     }
    }
    catch(...)
    {
        tbtrace(TM_ERROR, _T("An exception occured."));
        return TB_E_UNEXPECTED;
    }
	if(!bHandled)
	{
		return CSolutionTrxBroker::SendTBCommand(lCommandID, pArgs);
	}
	else
	{
		return tbrc;
	}
	
}
///////////////////////////////////////////////////////////////////////////////////
/*
	This function will perform all operations that we need prior to POS start Up.
	Right now we print all spooled receipts at startUp
*/
///////////////////////////////////////////////////////////////////////////////////
void CCustomerTrxBroker::PrePOSStartUp()
{
    tbtraceIO( _T("CCustomerTrxBroker::PrePOSStartUp"));

    CCustomerCurrentReceipt* pMHReceipt = CCustomerCurrentReceipt::instance();
    pMHReceipt->PrintAllSavedReceipts();
	CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_COMPLETEPRINTANDCUT, NULL, NULL );

}

TBRC CCustomerTrxBroker::SetSelectedItem(LPCTSTR szEntryID)
{
    tbtraceIO( _T("CCustomerTrxBroker::SetSelectedItem()"));
    TBRC retCode = TB_SUCCESS;
    try
    {
        IsItemScrolled(true);
        CString csEntryID;
        
		if (szEntryID && szEntryID[0])
		{
            csEntryID = szEntryID;
            
            // strip off MSG prefix for selectable Messages (TMD & Guest Card)
            LPCTSTR szMsg = _T("MSG");
            int iPos = csEntryID.Find(szMsg);
            if( -1 != iPos )
            {
                csEntryID.Delete(iPos, _tcslen(szMsg));
            }

			// Has Selected Item Changed? Check the TBProperty.
			CString csLastEntryID = TBGetProperty(_T("LastEntryID"));
			CString csPropPromo = TBGetProperty(_T("PropCMPromoLinked"),true);
			if ( (csLastEntryID != csEntryID) && (csPropPromo == _T("0")))   //(!((bool)csPropPromo)) ) fix for TAR#308318
			{
				tbtrace(TM_INFO, _T("SetSelectedItem to %s"), csEntryID);
				retCode = CSolutionTrxBroker::SetSelectedItem(csEntryID);				
			}
			else
			{
				tbtrace(TM_INFO, _T("SUPPRESSING Set/Unchanged SetSelectedItem (%s)"), csEntryID);
			}
		}
		else
		{
			tbtrace(TM_INFO, _T("Invalid zero-length Item ID"));
		}
    }
    catch (...)
    {
        retCode = TB_E_UNEXPECTED;
    }
	return retCode;
}

void CCustomerTrxBroker::TBSayPhrase(LPCTSTR szWaveFileName)
{
	if(TBGetProperty(_T("IsResumeTrxInProgress"), true) != _T("1"))
	   CSolutionTrxBroker::TBSayPhrase(szWaveFileName);
}

/*************************************************************************
* GetNextState  
* Deliberately avoiding solution version
************************************************************************/
TBRC CCustomerTrxBroker::GetNextState(LPCTSTR lpLastStateName, LPCTSTR lpNextStateName,
                              bool *pbOverride, LPTSTR *lpOverrideStateName, CDescriptorList &dlParameterList)
{
	tbtrace( TM_ENTRY, _T("CCustomerTrxBroker::GetNextState"));
	CCustomerTBProperties & pProp= *CCustomerTBProperties::instance();
	CString csOrigOverrideStateName= pProp.GetProperty(TBPROP_NEWFLSTATE);


    // This check is added to avoid overriding a regular transition inadvertently
    //tbtraceIO( _T("CCustomerTrxBroker::GetNextState()"));
    CString csTemp2 = lpNextStateName;
    TBRC rc = TB_SUCCESS;
    *pbOverride = false;
    if (csTemp2 == _T("SMExtensionState"))
    {
        TBRC rc= CBaseTrxBroker::GetNextState(lpLastStateName,  lpNextStateName,
                               pbOverride, lpOverrideStateName, dlParameterList);
    }
    
    //preserve the FL state so we can use it in the TB if needed
    CString csTemp = *lpOverrideStateName;
    if((*pbOverride) && (!csTemp.IsEmpty()))
    {
        m_csFLNextState = csTemp;
    }
	else
    {
        m_csFLNextState = lpNextStateName;
    }

    //if we are trying to exit FL at a lane close then allow it    
    if((CString(lpLastStateName) == _T("SMLaneClosed"))      && 
       (CString(lpNextStateName) == _T("SMOutOfService")))  
    {
        
        *pbOverride = false;
        return rc;
    }

    //if we are going to or comming out of laneclose put FL in outservices temp. until POS is ready
    if((CString(lpLastStateName) == _T("SMLaneClosed"))      && 
       (CString(lpNextStateName) != _T("SMOutOfService2")))  
    {
        CCustomerTBProperties &tbProp= *CCustomerTBProperties::instance();
        CString csOverrideStateName= _T("SMOutOfService2");

        *pbOverride = true;
        *lpOverrideStateName = new TCHAR[csOverrideStateName.GetLength() +1];
        _tcscpy(*lpOverrideStateName, (LPCTSTR)csOverrideStateName);
        tbProp.SetProperty(TBPROP_NEWFLSTATE, _T(""));
        
        m_csFLNextState = csOverrideStateName;
        return rc;
    }

    _bstr_t bstrPosState = CCurrentPosState::instance()->GetValue(_T("PosState"));
    CString csPosState = (LPCTSTR)bstrPosState;

	tbtrace(TM_INFO, _T("lpLastStateName = %s, lpNextStateName = %s, csOrigOverrideStateName = %s, and csPosState = %s."), 
		lpLastStateName, lpNextStateName, csOrigOverrideStateName, csPosState);


	// For Invalid WIC items, we will get the custom message after the item is sold
	// in the case of specialty items, after the weight, quantity, age are processed
	// ensure you return to ScanAndBag after the custom message is completed
	if( 
        (CString(lpLastStateName) == _T("SMCustomMessage")) && 
		(-1 != csPosState.Find(_T("ITEMMENU_DK1"))) 
      )
	{
		if  ( 
			(CString(lpNextStateName) == _T("SMEnterQuantity")) || 
			(CString(lpNextStateName) == _T("SMEnterWeight"))   ||
            (CString(lpNextStateName) == _T("SMKeyInCode")) 
			)
		{
			CCustomerTBProperties &tbProp= *CCustomerTBProperties::instance();
			CString csOverride = _T("SMScanAndBag");

			*pbOverride = true;
			*lpOverrideStateName = new TCHAR[csOverride.GetLength() +1];
			_tcscpy(*lpOverrideStateName, (LPCTSTR)csOverride);
			tbProp.SetProperty(TBPROP_NEWFLSTATE, _T(""));
			m_csFLNextState = csOverride;
		}
	}
	else if( 
        (CString(lpLastStateName) == _T("SMCmDataEntry")) && 
		(-1 != csPosState.Find(_T("DO_YOU_WANT_MPERKS"))) 
		)
	{		
        tbtrace(TM_INFO, _T("csOrigOverrideStateName = %s."), csOrigOverrideStateName);
		if  ( csOrigOverrideStateName == _T("SMFinish")) 
			
		{
			//POS293857
            tbtrace(TM_INFO, _T("Preventing state change during mPerks prompt."));	
			
			*pbOverride = false;
			pProp.SetProperty(TBPROP_NEWFLSTATE, _T(""));
		}
	}
	else if( 
        (CString(lpLastStateName) == _T("SMCardProcessing")) && 
		(-1 != csPosState.Find(_T("WAITING_ON_PINPAD"))) 
		)
	{		
        tbtrace(TM_INFO, _T("csOrigOverrideStateName = %s."), csOrigOverrideStateName);
		if  ( csOrigOverrideStateName == _T("SMCardProcessing")) 
			
		{
			//POS294602
            tbtrace(TM_INFO, _T("Preventing duplicate state change during CardProcessing."));	
			
			*pbOverride = false;
			pProp.SetProperty(TBPROP_NEWFLSTATE, _T(""));
		}
	}
	else if( /*POS297553 */
        (CString(lpLastStateName) == _T("SMFinish")) && 
		((CString(lpNextStateName) == _T("SMExtensionState") || CString(lpNextStateName) == _T("SMFinish")) )
		)
	{		
        tbtrace(TM_INFO, _T("csOrigOverrideStateName = %s."), csOrigOverrideStateName);
		if  ( csOrigOverrideStateName == _T("SMFinish")) 
			
		{		
            tbtrace(TM_INFO, _T("Preventing from going back and forth to SMFinish."));	
			
			*pbOverride = false;
			pProp.SetProperty(TBPROP_NEWFLSTATE, _T(""));
		}
	}

    CTBInProgress *pTBState = CTBInProgress::instance();
    if (CString(lpNextStateName) == _T("SMScanAndBag")) 
    {
        
        CString csTBStateType= pTBState->GetType(GetCurrentInProgressState());
        if (csTBStateType == _T("ITEMIZATION_IP"))
        {
            CString csCurrentTBState= GetCurrentInProgressStateName();
            tbtrace(TM_WARNING, _T("FastLane leaving TB in an ITEMIZATION_IP. Resync"));
            SetCurrentInProgressState(csCurrentTBState + CString(_T("_")) + lpNextStateName);
            TriggerMsgMap();
        }
        // +SOTF5842
        CString csTBState = GetCurrentInProgressStateName();
        if( !csTBState.Compare(_T("WIC_START_PENDING")) )
        {
            tbtrace(TM_INFO, _T("We are ready to process WICEBT card."));
            SetCurrentInProgressState(_T("WIC_START"));
            TriggerMsgMap();

            CCustomerTBProperties &props= *CCustomerTBProperties::instance();
            CString csOverride = _T("SMInProgress");
            *pbOverride = true;
			*lpOverrideStateName = new TCHAR[csOverride.GetLength() +1];
			_tcscpy(*lpOverrideStateName, (LPCTSTR)csOverride);
            dlParameterList.AddDescriptor( (WORD)false);
            dlParameterList.AddDescriptor( (WORD)false);
            dlParameterList.AddDescriptor( (WORD)120);
			props.SetProperty(TBPROP_NEWFLSTATE, _T(""));
			m_csFLNextState = csOverride;
        }
        // -SOTF5842

        //POS24632 rrs
        if( !csTBState.Compare(_T("TB_DIGITAL_COUPON_PENDING")) )
        {
            tbtrace(TM_INFO, _T("We are ready to process mPerks entry."));
            SetCurrentInProgressState(_T("TB_DIGITAL_COUPON"));
            TriggerMsgMap();

            CCustomerTBProperties &props= *CCustomerTBProperties::instance();
            CString csOverride = _T("SMInProgress");
            *pbOverride = true;
			*lpOverrideStateName = new TCHAR[csOverride.GetLength() +1];
			_tcscpy(*lpOverrideStateName, (LPCTSTR)csOverride);
            dlParameterList.AddDescriptor( (WORD)false);
            dlParameterList.AddDescriptor( (WORD)false);
            dlParameterList.AddDescriptor( (WORD)120);
			props.SetProperty(TBPROP_NEWFLSTATE, _T(""));
			m_csFLNextState = csOverride;
        }
        // e POS24632
    }

    
    if ((CString(lpNextStateName) == _T("SMFinish")) && (TBGetProperty(_T("SignatureRequested"), true)==_T("1")))
    {
        
        //we have the situation where the pinpad is at the sign screen and FL timedout waiting for the POS
        //alert the cashier
        SetCurrentInProgressState(_T("TENDER_SIG_TIMEOUT"));
        TriggerMsgMap();
    }

    return rc;
}

long CCustomerTrxBroker::SetFLState( LPCTSTR szState/*=NULL*/ )
{
    tbtrace( TM_ENTRY, _T("CCustomerTrxBroker::SetFLState(%s)"), szState);
    
	CString csFLState = TBGetProperty(TBPROP_NEWFLSTATE);
	tbtrace( TM_ENTRY, _T("Current FLState(%s)"), csFLState);

	CString csPosState = CCustomerTrxBroker::instance()->TBGetProperty(_T("__POSState"), true);
	
	/* commented below as it causes issues, POS297191, 297192, 297213 -aab
	//+POS296369 aab
    if(csFLState == _T("SMFinish") && csFLState == szState)
	{
        tbtrace( TM_INFO, _T("Ignoring SetFLState to prevent from looping to SMFinish."));
		return true;
	} */
	//-POS296369 aab
	if( csFLState == _T("SMCashPayment") && csPosState.Find(_T("AGE_VERIFY")) == -1) //POS289891	
    {
        if( m_csFLNextState == _T("SMCashPayment") )
        {
            //let the SSF handle to go back to Cashpayment and don't change state. TAR340967
		    return true;
        }
        else 
        {
		    szState = _T("SMCashPayment"); // in case FL is in another state (e.g.: SMCmDataEntry) 
        }
    }

	TBSetProperty(TBPROP_NEWFLSTATE, szState);
    return CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_CHANGESTATE, NULL, NULL );
}


TBRC CCustomerTrxBroker::TBGetCurrentBusinessDate(CString &csBusinessDate)
{
	TBRC rc =TB_E_FUNCTIONNOTSUPPORTED;

    tbtraceIO(_T("TBGetCurrentBusinessDate()"));
    CCustomerTBConnectPOS* pTBConnectPOS = CCustomerTBConnectPOS::instance();
 
	long terminal = TBGetTerminalNumber();
	long lJulianBusinessDate;

	if(pTBConnectPOS->ACSExGetBusinessDate(terminal, &lJulianBusinessDate))
    {
		tbtrace( TM_INFO, _T("The value of currentBusinessDate(%d)."), lJulianBusinessDate);	
		rc = TB_SUCCESS;
    }

	CTime ctCurrentTime = CTime::GetCurrentTime();
	int iYear = ctCurrentTime.GetYear();
	int iMonth=0, iDay=0;
	int ddd;

	int days_of_month[12] = {31,0,31,30,31,30,31,31,30,31,30,31};  
	days_of_month[1] = ( iYear%4 ) ? 28 : 29;

	ddd = (int)lJulianBusinessDate;
	
	int i;
    for (i = 0; i < 12; i++)
    {
        if (ddd > days_of_month[i])
        {
            ddd -= days_of_month[i];
        }
        else
        {
            break;
        }
    }

	iDay = ddd;
    iMonth = i+1;

	CString csTimeBD;

	if (rc == TB_SUCCESS)
	{
		csTimeBD.Format(_T("%.4d-%.2d-%.2d"),iYear,iMonth,iDay);
	}
	else
	{
		csTimeBD.Format(_T("%.4d-%.2d-%.2d"),
			ctCurrentTime.GetYear(),
			ctCurrentTime.GetMonth(),
			ctCurrentTime.GetDay());
	}
	
	csBusinessDate = csTimeBD;
            
	return rc;
}


bool CCustomerTrxBroker::LaunchAppAndWait(LPCTSTR szBatchFile, DWORD dwWaitTime, bool bKillIfHung)
{
    tbtrace(TM_INFO, _T("+LaunchAppAndWait('%s',%d, %s)"), szBatchFile, dwWaitTime, bKillIfHung ? _T("true"):_T("false"));
	bool bRet = false;
	int rc;
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.lpReserved=0;
	si.lpDesktop=0;
	si.lpTitle=0;

	if (CreateProcess(NULL, (LPTSTR) szBatchFile, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
    {

        if (WaitForSingleObject(pi.hProcess, dwWaitTime) != WAIT_OBJECT_0)
	    {
		    if (bKillIfHung)
		    {
			    tbtrace(TM_WARNING, _T("CreateProcess timed out. Program either did not start or hung, terminating program."));
			    rc = TerminateProcess(pi.hProcess, 0);
			    if (0 != rc)
			    {
				    tbtrace(TM_INFO, _T("Process terminated succesfully"));
			    }
			    else
			    {
				    tbtrace(TM_ERROR, _T("Process failed to terminate succesfully"));
			    }
		    }
		    tbtrace(TM_WARNING, _T("LaunchProgram - CreateProcess timed out. Program did not start or hung."));
	    }
	    else
	    {
		    bRet = true;
	    }
	    CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    else
    {
        tbtrace(TM_INFO, _T("CreateProcess failed to launch program."));
    }

    tbtrace(TM_INFO, _T("-LaunchAppAndWait() Return = %d"), (int)bRet);
	return bRet ? true : false;
}


/*************************************************************************
* ReleaseMessageLParam - Frees the message element 
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the message element as passed to
*       the application as a notification message.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CCustomerTrxBroker::ReleaseMessageLParam(PSMessageElement psMessage)
{
    // Cleanup any UI request dynamically allocated memory 
    if (psMessage->nDeviceID == TB_DEV_UICOMMAND)
    {
        try
        {
            S_UI_CMD* pCmd = (S_UI_CMD*)psMessage->lParam;
            if( pCmd )
            {
                for(int i = 0; i < QUERY_MAX; i++)
                {
                    PS_UI_VAR pVar = pCmd->Rcpt.pUiVars[i];
                    if( pVar ) 
                    {
                        pVar->CleanUp(); // TAR 391689             
                        delete pVar;
                    }
                    else
                    {
                        break;
                    }
                }
                delete pCmd;
                psMessage->lParam = NULL;
            }
        }
        catch(...)
        {
            tbtrace(TM_ERROR, _T("CCustomerTrxBroker::ReleaseMessageLParam(): Exception occurred."));
        }
    }
    return CSolutionTrxBroker::ReleaseMessageLParam(psMessage);
}

/* Meijer POS6135 */
TBRC CCustomerTrxBroker::TBLoadOptions(void)
{
    tbtraceIO(_T("TBLoadOptions()"));

    // Load the TB Configuration files and POS Reload Option
	LoadACSOptions();

    return CSolutionTrxBroker::TBLoadOptions();
}


bool CCustomerTrxBroker::LoadACSOptions()
{
	bool retVal = false;
    PROCESS_INFORMATION processInformation = {0};
    STARTUPINFO startupInfo = {0};
    startupInfo.cb = sizeof(startupInfo);

#if 0

#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER  0xFFFFFFFF
#endif

	WCHAR fileName[] = _T("c:/scot/logs/SSCO_OPTIONS.LOG");

	TCHAR filePath[MAX_PATH] = _T("c:/scot/logs");
	TCHAR root[MAX_PATH] = _T("c:/");

	DWORD bufSize = 0;
	DWORD foo = 0;

	GetDiskFreeSpace(
		root,
		&foo,
		&bufSize,
		&foo,
		&foo);

	HANDLE hTouchEvent = FindFirstChangeNotification(
		filePath,
		FALSE,
		FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE);

#endif

	// %APP_DRIVE%

	/* The command line must be in a non-const buffer. CreateProcess may rewrite the contents. 
	That's why the code below copies to a non-const buffer. */
    _TCHAR cmdLine[MAX_PATH] = _T("c:\\windows\\system32\\wscript.exe c:\\scot\\bin\\ssco_options.js //B //NOLOGO");
	int len = wcslen(cmdLine);
	std::vector<_TCHAR> cmdBuf(len + 1);
	std::copy(cmdLine, cmdLine + len, cmdBuf.begin());

    BOOL result = CreateProcess(_T("c:\\windows\\system32\\wscript.exe"), &cmdBuf[0], NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);

    if (!result)
    {
        retVal = false;
    }
    else
    {
        // Wait until the AppLoader object finished copying all the SSCO config files from ACS Server
        //
		HANDLE hStopEvent = processInformation.hProcess;

#if 0
		int lines = 0;
		DWORD fileSize = 0;
		DWORD prevFileSize = 0;
		DWORD filePointer = 0;
		int firstLine = 0;
		
		ULONG readBytes = 0;
		BOOL read = FALSE;
		
		vector<char> buf(bufSize + 1);
		
		int linesRead = 0;
		int index = 0;
#endif

		HANDLE events[] = {hStopEvent, /*hTouchEvent,*/};
		
		
		bool exitLoop = false;
		DWORD sigObject = 0;
		
		while (!exitLoop)
		{
			sigObject = WaitForMultipleObjects(sizeof(events) / sizeof(events[0]), events, FALSE, INFINITE);
			
			if (sigObject == WAIT_OBJECT_0)
			{
				exitLoop = true;
			}
			else if (sigObject == WAIT_OBJECT_0 + 1)
			{
#if 0
				SetLastError(0);
				
				do
				{
					HANDLE hFile = CreateFile(
						fileName,
						GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
					
					if (hFile != INVALID_HANDLE_VALUE)
					{
						fileSize = GetFileSize(
							hFile,
							NULL);
						
						if (fileSize < prevFileSize)
						{
							// Output?
							
							filePointer = SetFilePointer( 
								hFile, 
								0,
								NULL,
								FILE_END);
							
						}
						else if (filePointer)
						{
							filePointer = SetFilePointer( 
								hFile, 
								filePointer,
								NULL,
								FILE_BEGIN);
							
							if (filePointer == INVALID_SET_FILE_POINTER)
							{
								filePointer = SetFilePointer( 
									hFile, 
									0,
									NULL,
									FILE_BEGIN);
							}
						}
						else
						{
							filePointer = SetFilePointer( 
								hFile, 
								0,
								NULL,
								FILE_BEGIN);
						}
						
						
						prevFileSize = fileSize;
						
						read = ReadFile(
							hFile, 
							&buf[0], 
							bufSize, 
							&readBytes, 
							NULL);
						
						CloseHandle(hFile);
						
						if (read && readBytes)
						{
							filePointer += readBytes;
							buf[readBytes] = '\0';							
						}
					}
					
				} while (readBytes == bufSize);

				FindNextChangeNotification(hTouchEvent);
#endif
			}
		}

#if 0
		FindCloseChangeNotification(hTouchEvent);
#endif

        CloseHandle(processInformation.hProcess);
        CloseHandle(processInformation.hThread);

		retVal = true;
    }

	//POS72976 - Reenabled this feature again as POS have the support of virtual operator doing reload option - rrs
    /* POS15456, temporarily we need to not perform RELOAD OPTION until there is a way to update the cashier option class 7 on the POS Server */
	/* Trigger TB message to press "Load Options" key on ACS */
    /* The Key to go to Load Option from ITEM MENU is by pressing {MISC} - {F5-Supervisor} - {F8-MORE} - {F3-Load Option} */    
    CCustomerTrxBroker* pCurTrx = CCustomerTrxBroker::instance();
    CString csCurrentInProgressState = pCurTrx->GetCurrentInProgressStateName();

    if( csCurrentInProgressState != _T("TERM_INIT") )
    {
        pCurTrx->SetCurrentInProgressState(_T("TB_POS_RELOAD_OPTION"));
        pCurTrx->TriggerMsgMap();
    
        Sleep(5000);
        int n=0;
        while( TBGetProperty(_T("IsPOSReloadOptionDone"), true) == _T("0") )
        {
            n++;
            Sleep(5000);

            // wait for 25 sec then just exit
            if( n == 5 )
            {
                tbtrace(TM_ERROR, _T("POS ReloadOptions never finished, exit out."));
                break;
            }
            
        }        
    }
    /* e - POS15456, temporarily we need to not perform RELOAD OPTION until there is a way to update the cashier option class 7 on the POS Server */
	// e - POS72976 - rrs
    
    return retVal;
}
