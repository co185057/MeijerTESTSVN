// CustomerTBConnectPOS.cpp: implementation of the CCustomerTBConnectPOS class.
//
// CRD 541144 Name: CJ Estrada Date: Jan 7, 2021
// POS293815 Work request:76936 Name:Jangala Sunny Date:December 16,2016
// POS81632 Work Request:59313 Name:Matt Condra Date:December 10, 2013
// POS29719 Work Request:19114 Name:Matt Condra Date:April 20, 2012
// POS29349 Work Request:19114 Name:Matt Condra Date:March 8, 2012
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerTBConnectPOS.h"
#include "CustomerMHOptions.h"
#include "TraceSystem.h"
#include "CustomerTrxBroker.h"
#include "CustomerTBSharedDefs.h"
#include "CustomerMHOperator.h"

#define T_ID  _T("CCustomerTBConnectPOS")
extern ACS::IACSHookPtr spHook;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerTBConnectPOS::CCustomerTBConnectPOS()
{
	m_bAutoCleanup= false;
    m_bServerInfoLoaded = false;
    m_csServerName = _T("");
    m_csServerIP = _T("");
}

CCustomerTBConnectPOS::~CCustomerTBConnectPOS()
{
    
}

BOOL CCustomerTBConnectPOS::SetSelectedItem(LPCTSTR szEntryID)
{
	CString csOps = CCustomerMHOptions::instance()->OptionValue(_T("SendSelectedEntryID"));

	if( csOps != _T("N") )
	    return CSolutionTBConnectPOS::SetSelectedItem(szEntryID);
	else
	{
	    return TRUE;
	}
}

// +SOTF 6312
// SendAuxMsgTLOGOverride parameters:
// ---------------------------------
// Manager:  ACS Manager ID
// Override: ID of the override/intervention
// Location: 1 = FastLane, 2 = HHRAP, 3 = RAP, 4 = ACS
// WaitTime: Customer wait time from the time the assistance needed 
//           was initiated until the override was completed, in seconds.
// Terminal: The ID of the POS (default to the current terminal instance)
BOOL CCustomerTBConnectPOS::SendAuxMsgTLOGOverride(
              CString csManager, long lOverride, long lLocation, long lWaitTime, long lTerminal, long lWaitTimeMs)
{
    tbtrace(TM_INFO, _T("SendAuxMsgTLOGOverride(%s,%d,%d,%d,%d, %d)"), 
                     csManager, lOverride, lLocation, lWaitTime, lTerminal, lWaitTimeMs ); //SOTF 7242
    try
	{
        // Skip logging if setting is not enabled
        CString csEnabled = CCustomerMHOptions::instance()->OptionValue(_T("AuxMsgTLOGOverrideEnabled"));
        csEnabled.TrimLeft();
        csEnabled.TrimRight();
        if( csEnabled != _T("1") && csEnabled.CompareNoCase(_T("y")) )
        {
            tbtrace(TM_INFO, 
                _T("AuxMsgTLOGOverrideEnabled setting is not enabled. TLOG data will not be sent to POS."));
            return TRUE;
        }    
    
        if( lTerminal == -1 )
        {
            lTerminal = CCustomerMHOptions::instance()->GetTerminalNumber();
        }

        // Build the TLOG AuxMsg:
        // ---------------------
		// ex 1: <TLOGOverride Manager="1001100" Override="30" Location="3" 
        //                     WaitTime="34" FastLane="1" Terminal="101" /> 
        // ex 2: <TLOGOverride Manager="TM51511001" Override="30" Location="2" 
        //                     WaitTime="60" FastLane="1" Terminal="122" /> 
        CString csAuxMsg;

        //SOTF 7242
        csAuxMsg.Format(
            _T("<TLOGOverride Manager=\"%s\" Override=\"%d\" ")
            _T("Location=\"%d\" WaitTime=\"%d\" Terminal=\"%d\" WaitTimeMs=\"%d\" />"), 
            csManager, lOverride, lLocation, lWaitTime, lTerminal, lWaitTimeMs);
        
        GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_TLOGOVERRIDE); // Notification for TLOGOverride value is 1
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, 
            _T("SendAuxMsgTLOGOverride failed to send to the POS. HRESULT = 0x%08X:%s"), 
            cex.Error(), 
            (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}


// -SOTF 6312

//******************************************************************************************************
// 
// Function:   	SendAuxMsgPreSelectedTender
// 
// Purpose:		Send PreSelected Tender Type to POS
//
// Notes :  	For PreSelected Tender Type send a notification ID of 2. 
//		The Tender value may be 3, 6, 11, or 12 indicating CREDIT, DEBIT, EBT FOOD, and EBT CASH respectively. 
// 
//******************************************************************************************************
BOOL CCustomerTBConnectPOS::SendAuxMsgPreSelectedTender(long lTender)
{
	tbtrace(TM_INFO, _T("SendAuxMsgPreSelectedTender(%d)"), lTender);

	try
	{
		CString csAuxMsg;
		csAuxMsg.Format(_T("<PaymentSelection Tender=\"%d\" />"), lTender);
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_PRESELECTEDTENDER);	// Send AuxMsg with the preselected tender and notification id
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, 
            _T("SendAuxMsgPreSelectedTender failed to send to the POS. HRESULT = 0x%08X:%s"), 
            cex.Error(), 
            (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}

BOOL CCustomerTBConnectPOS::SendAuxMsgRemoteAssistMode(bool bRemoteAssistMode)
{
	tbtrace(TM_INFO, _T("SendAuxMsgRemoteAssistMode(%d)"), bRemoteAssistMode);

	try
	{
		CString csAuxMsg;
		csAuxMsg.Format(_T("<AssistMode Remote=\"%d\" />"), bRemoteAssistMode);
		
		// Send AuxMsg with whether we are entering remote Assistmode to the POS
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_REMOTEASSISTMODE);		
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, 
            _T("SendAuxMsgRemoteAssistMode failed to send to the POS. HRESULT = 0x%08X:%s"), 
            cex.Error(), 
            (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}

}

//+SOTF 5572
BOOL CCustomerTBConnectPOS::SendAuxMsgRobotCashFlow(CString csMediaReport)
{
	tbtrace(TM_INFO, _T("SendAuxMsgRobotCashFlow"));
	 CCustomerTrxBroker* pTrx = CCustomerTrxBroker::instance();

	try
	{
		CString csCM = pTrx->TBGetProperty(_T("IsItForCashMonitoring")); //POS281261 by aab.

		tbtrace(TM_INFO, _T("SendAuxMsgRobotCashFlow - %s. IsItForCashMonitoring[%s]"), (LPCTSTR)csMediaReport, csCM);
		// Send AuxMsg with the media report

		if(csCM == _T("1"))
		{
		   tbtrace(TM_INFO, _T("Sending AuxMsg with ID of 10."));
           GetHook()->SendAuxMsg((LPCTSTR)csMediaReport, FLNOTIFY_NOTCASHMONITORING);
		   pTrx->TBSetProperty(_T("IsItForCashMonitoring"), _T("0"));
		}
		else
		   GetHook()->SendAuxMsg((LPCTSTR)csMediaReport, FLNOTIFY_CASHMONITORING);
		
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, 
            _T("SendAuxMsgRobotCashFlow failed to send to the POS. HRESULT = 0x%08X:%s"), 
            cex.Error(), 
            (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}

}
//-SOTF 5572

//SOTF6226 
BOOL CCustomerTBConnectPOS::SendAuxMsgAgeVerifyData(int ageEntry, CString csCustomerDOB,
							CString csUPC, int ageRequired, int itemAllowed, int nCustomerAge)
{
    tbtrace(TM_INFO, _T("SendAuxMsgAgeVerifyData(%d,%s,%s,%d,%d)"), 
                         ageEntry, csCustomerDOB, csUPC, ageRequired, itemAllowed);
    try
	{
        // Build the TLOG AgeVerifyData message:
        // ---------------------
		// ex: <AgeValidation AgeRestrictionEntry="1" dobMonth="01" dobDay="01" dobYear ="99" 
        //                    UPC="12345678901234" AgeRequired="21" ItemAllowed="1" nCustomerAge="10" />
		
		CString dob_mo = _T("00");
		CString dob_da = _T("00");
		CString dob_yr = _T("00");

		if(!csCustomerDOB.IsEmpty() && (csCustomerDOB.GetLength() >= 6))
		{
			dob_mo.Format(_T("%s"),csCustomerDOB.Mid(0,2));
			dob_da.Format(_T("%s"),csCustomerDOB.Mid(2,2));
			dob_yr.Format(_T("%s"),csCustomerDOB.Mid(4,2));
		}

		CString csAuxMsg;
        csAuxMsg.Format(
            _T("<AgeValidation AgeRestrictionEntry=\"%d\" dobMonth=\"%s\" dobDay=\"%s\" dobYear=\"%s\" ")
            _T("UPC=\"%s\" AgeRequired=\"%d\" ItemAllowed=\"%d\" CustomerAge=\"%d\" />"), 
            ageEntry, dob_mo, dob_da, dob_yr, csUPC, ageRequired, itemAllowed, nCustomerAge);
        
        GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_AGEVALIDATION); // Notification for AgeValidation value is 5
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, 
            _T("SendAuxMsgAgeVerifyData failed to send to the POS. HRESULT = 0x%08X:%s"), 
            cex.Error(), 
            (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}
//SOTF6226 

// POS29349
BOOL CCustomerTBConnectPOS::SendAuxMsgBroughtBag(bool bBroughtBag)
{
	tbtrace(TM_INFO, _T("SendAuxMsgBroughtBag(%d)"), bBroughtBag);

	try
	{
		CString csAuxMsg;
		csAuxMsg.Format(_T("<Bag Brought=\"%d\" />"), bBroughtBag);
		
		// Send AuxMsg to sales app indicating whether customer has brought there own shopping bag.
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_BROUGHTBAG);		
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, 
            _T("SendAuxMsgBroughtBag failed to send to the sales app. HRESULT = 0x%08X:%s"), 
            cex.Error(), 
            (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}

// POS29719
BOOL CCustomerTBConnectPOS::SendAuxMsgEOT(bool bEOT)
{
	tbtrace(TM_INFO, _T("SendAuxMsgEOT(%d)"), bEOT);

	try
	{
		CString csAuxMsg;
		csAuxMsg.Format(_T("<EOT Active=\"%d\" />"), bEOT);
		
		// Send AuxMsg to sales app indicating that transaction has been finalized.
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_EOT);		
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, 
            _T("SendAuxMsgEOT failed to send to the sales app. HRESULT = 0x%08X:%s"), 
            cex.Error(), 
            (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}

// Start POS111525 POS111527 CRD 145555
BOOL CCustomerTBConnectPOS::SendAuxMsgFLMS(CString csAuxMsg)
{
	tbtrace(TM_INFO, _T("SendAuxMsgFLMS(%s)"), csAuxMsg); 

	// NOTES: until Audit is move to SSCO lane the type will always be 0 and the TrustLevel 0
	// 
	// Fields
	// a. FLM: Start = 1, End = 0
	// b. Type: Normal = 0, Partial Rescan = 1, Full Rescan = 2
	// c.  TrustLevel: 12345 (numeric, leading zero’s) 
	//
	// i.e. Sample
	//  <FLMSTrans>
	//		<FLMS>1</FLMS>
	//		<Type>0</Type>	
	//	Start CRD 145555
	//		<mPerksBarcode>99999-----------</mPerksBarcode>
	//		<CashierID>1234567</CashierID>
	//		<SessionID>e6657a4a-2954-4bfd-9c07-86de36775c2b</SessionID>
	//	End CRD 145555
	//		<TrustLevel>00000</TrustLevel>
	// 	</FLMSTrans>

	try
	{
		//CString csAuxMsg;
		//csAuxMsg.Format(_T("<FLMSTrans><FLMS>%s</FLMS><Type>%s/Type><mPerksBarcode>%s</mPerksBarcode><CashierID>%s</CashierID><SessionID>%s</SessionID><TrustLevel>%05d</TrustLevel></FLMSTrans>"), iStartEnd, iType, csMperksBarcode, szOperatorID, csSessionID, iTrustLevel);
		// Send AuxMsg to sales app transaction: a FLMS start or end transaction.
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_FLMSBASKET);		
		return TRUE;
	}
	catch(_com_error & cex)
	{
		//Start CRD 299396 - force to get the hook marshaled if it has the error as the called is from different thread application (launchpad)
		tbtrace(TM_INFO, _T("SendAuxMsgFLMS requires hook reference to be marshaled. Marshaling the hook."));
		if( cex.Error() == RPC_E_WRONG_THREAD)
		{			
			try
			{
				GetHook(true)->SendAuxMsg((LPCTSTR)csAuxMsg, 13);
				return TRUE;
			}		
			catch (_com_error & cex2)
			{
				tbtrace(TM_ERROR, _T("SendAuxMsgFLMS failed to send to the POS. HRESULT = 0x%08X:%s"), cex2.Error(), (LPCTSTR)cex2.ErrorMessage());
			}
		}
		//End CRD 299396
		return FALSE;
	}
}
// End POS111525 and POS111527
// start CRD120156
BOOL CCustomerTBConnectPOS::SendAuxMsgTransMode(int iMode)
{
	tbtrace(TM_INFO, _T("SendAuxMsgTransMode(%d)"), iMode); 
	

		// Example
		//
		// Mode:
		//   0 = SSCO
		//   1 = SnD
		//   2 = FLMS
		//   3 = unknown / future use
		//  Default = 0 (SSCO)
		// i.e. Sample
		// <TransMode>
		//           <Mode>1</Mode>
		//</TransMode>
		
		try
	{
		CString csAuxMsg;
		csAuxMsg.Format(_T("<TransMode><Mode>%d</Mode></TransMode>"), iMode);
		// Send AuxMsg to sales app transaction: a TransactionMode.
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_TRANSMODE); // "12" per POS request                            
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("SendAuxMsgTransMode failed to send to the sales app. HRESULT = 0x%08X:%s"),  cex.Error(),  (LPCTSTR)cex.ErrorMessage());
		return FALSE;
	}
}
// end CRD126015

// POS81632
BOOL CCustomerTBConnectPOS::SendAuxMsgTLOGRAPTrack(LPCTSTR szOperatorID, LPCTSTR szTimestamp, int nFunc, int nRAP, int nVRAP, int nOffline)
{
   tbtrace(TM_INFO, _T("SendAuxMsgTLOGRAPTrack(%s,%s,%d,%d,%d, %d)"), szOperatorID, szTimestamp, nFunc, nRAP, nVRAP, nOffline);
   try
   {
      // Build the TLOG AuxMsg:
      // ---------------------
      // ex 1: <RAPTrack Operator="1962608" Time="11/15/2013 23:57:43" Func="1" RAP="2" VRAP="801" Offline="0" />

      CString csAuxMsg;

	  if(CCustomerMHOperator::instance()->IsEmployeeBadgeCode(szOperatorID))
	  {
		  tbtrace(TM_INFO, _T("Got Employee badge instead of Employee ID. Will find out the Empl Id and use it for TLOG"));
		  szOperatorID = GetEmplidFromBadge(szOperatorID);
	  }

      csAuxMsg.Format(
         _T("<RAPTrack Operator=\"%s\" Time=\"%s\" ")
         _T("Func=\"%d\" RAP=\"%d\" VRAP=\"%d\" Offline=\"%d\" />"), 
         szOperatorID, szTimestamp, nFunc, nRAP, nVRAP, nOffline);

      GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_LOGRAPTRACK); // Notification for TLOGRAPTrack value is 9
	  spHook = NULL; // POS293815
      return TRUE;
   }
	catch(_com_error & cex)
	{
      tbtrace(TM_ERROR, 
         _T("SendAuxMsgTLOGRAPTrack failed to send to the POS. HRESULT = 0x%08X:%s"), 
         cex.Error(), 
         (LPCTSTR)cex.ErrorMessage());
      return FALSE;
	}
}

void CCustomerTBConnectPOS::LoadServerInfoFromLogs(void)
{
    tbtraceIO(_T("LoadServerInfoFromLogs"));
    
    // This is a temporary solution, ideally we should modify 
    // POS to receive these through an ACS context 
    m_csServerName = _T("");
    m_csServerIP = _T("");
    m_bServerInfoLoaded = true;

    CString csLoaderFile = _T("C:\\acs\\logs\\aploader.log");
    LPCTSTR pszTokStart = _T("Found server ");
    LPCTSTR pszTokStartIp = _T(" at ");

    CStdioFile file;
    CFileException exc;
    
    if( file.Open(csLoaderFile, CFile::modeRead) )
    {
        // (1) Get the last instance of the 'Found server' token
        CString csInfoLine = _T("");
        int iInfoPos = -1;
        CString csLine = _T("");
        while( file.ReadString(csLine) )
        {
            if( !csLine.IsEmpty() && (-1 != csLine.Find(pszTokStart)) )
            {
                // token found on this line
                csInfoLine = csLine;
                iInfoPos = csLine.Find(pszTokStart);
                tbtrace(TM_INFO, _T("Server Info found in log string: %s"), (LPCTSTR)csInfoLine);
            }
        }
        file.Close();

        // (2) If matched, extract server name & IP address
        if( -1 != iInfoPos )
        {
            // identify start of server name
            int iNameStart = iInfoPos + _tcslen(pszTokStart);
            // identify start of ip address (end of name)
            int iIpStart = csInfoLine.Find(pszTokStartIp);

            if( -1 != iIpStart )
            {
                m_csServerName = csInfoLine.Mid(iNameStart, iIpStart - iNameStart);
                tbtrace(TM_INFO, _T("Server Name: %s"), (LPCTSTR)m_csServerName);
                m_csServerIP = csInfoLine.Mid(iIpStart + _tcslen(pszTokStartIp));
                m_csServerIP.TrimRight();
                tbtrace(TM_INFO, _T("Server IP: %s"), (LPCTSTR)m_csServerIP);
            }
            else
            {
                // didn't find the IP start Token, so try getting the server name only
                int iNameEnd = csInfoLine.Find(_T(" "), iNameStart);
                if( -1 != iNameEnd )
                {
                    m_csServerName = csInfoLine.Mid(iNameStart, iNameEnd - iNameStart);
                    tbtrace(TM_INFO, _T("Server Name: %s"), (LPCTSTR)m_csServerName);
                }
                else
                {
                    tbtrace(TM_ERROR, _T("Unable to locate the server Name entry in the %s file."), (LPCTSTR)csLoaderFile);
                }
                tbtrace(TM_ERROR, _T("Unable to locate the server IP entry in the %s file."), (LPCTSTR)csLoaderFile);
            }
        }
        else
        {
            // Didn't find any server info entries in the file.
            tbtrace(TM_ERROR, _T("Unable to locate the server Name and IP value in the %s file. Server Event Viewer logging will be disabled."), (LPCTSTR)csLoaderFile);
        }
    }
    else
    {
        tbtrace(TM_ERROR, _T("Failed to open %s file. Server Event Viewer logging will be disabled."), (LPCTSTR)csLoaderFile);
    }
}

CString CCustomerTBConnectPOS::GetServerName(void)
{
    if( !m_bServerInfoLoaded )
    {
        LoadServerInfoFromLogs();
    }
    tbtrace(TM_INFO, _T("GetServerName(): %s"), (LPCTSTR)m_csServerName);
    return m_csServerName;
}

CString CCustomerTBConnectPOS::GetServerIP(void)
{
    if( !m_bServerInfoLoaded )
    {
        LoadServerInfoFromLogs();
    }
    tbtrace(TM_INFO, _T("GetServerIP(): %s"), (LPCTSTR)m_csServerIP);
    return m_csServerIP;
}


TBRC CCustomerTBConnectPOS::LogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
                                         const int nComponentID, LPCTSTR szComponentID,
                                         LPCTSTR szModuleID, const int nModuleLineNumber)
{
    // Commented out for now - will need to be implemented 
    // for 20080313-1745  - Meijer Crashes GPF Reporting FIS Complete 
    /*
    if( !GetServerName().IsEmpty() )
    {
        HANDLE hLog = ::OpenEventLog(m_csServerName, _T("Application"));
        if( hLog )
        {
            WORD wType;
            switch(nSeverity)
            {
            case 1: wType = EVENTLOG_ERROR_TYPE; break;
            case 2: wType = EVENTLOG_WARNING_TYPE; break;
            default: wType = EVENTLOG_INFORMATION_TYPE; break; // nSeverity: 3,4,...
            }

            BOOL bRet = ::ReportEvent(
                hLog,           // HANDLE hEventLog
                wType,          // WORD wType
                0,              // WORD wCategory
                9999,           // DWORD dwEventID
                NULL,           // PSID lpUserSid
                1,              // WORD wNumStrings
                0,              // DWORD dwDataSize
                &szErrorText,   // LPCTSTR* lpStrings
                NULL);          // LPVOID lpRawData

            if( !bRet )
            {
                tbtrace(TM_ERROR, _T("ReportEvent() failed: %s"),(LPCTSTR)GetErrorString(GetLastError())); 
            }
            ::CloseEventLog(hLog);
            hLog = NULL;    
        }
        else
        {
            tbtrace(TM_ERROR, _T("OpenEventLog() failed: %s"),(LPCTSTR)GetErrorString(GetLastError())); 
        }
    }*/

    return TB_SUCCESS;
}

CString CCustomerTBConnectPOS::GetErrorString(DWORD dwError)
{
    LPVOID pMsg;

    if( FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &pMsg,
        0,
        NULL ) )
    {
        CString str = (LPCTSTR)pMsg;
        LocalFree(pMsg);
        return str;
    }
    
    return _T("There was an error formatting the last system error for display.");
}

// +SOTF 6312
CString CCustomerTBConnectPOS::GetEmplidFromBadge(LPCTSTR szBadge)
{
    tbtrace(TM_INFO, _T("GetEmplidFromBadge(%s)"), szBadge);
    try
    {
        BSTR bstrEmplid = NULL;
        VARIANT_BOOL vbRet = GetHook()->ExGetEmplidFromBadge(szBadge, &bstrEmplid);
        _bstr_t strEmplid(bstrEmplid);
        if( vbRet == VARIANT_TRUE )
        {
            return (LPCTSTR)strEmplid;
        }
        else
        {
            tbtrace(TM_ERROR, _T("GetEmplidFromBadge call to retrieve Emplid failed."));
            return _T("");
        }
    }
    catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, 
            _T("GetEmplidFromBadge failed to send to the POS. HRESULT = 0x%08X:%s"), 
            cex.Error(), 
            (LPCTSTR)cex.ErrorMessage());
        return _T("");
	}
}
// -SOTF 6312

//Start CRD366323
BOOL CCustomerTBConnectPOS::SendAuxMsgNotifyExitStoreMode(CString nextState)
{
	try
	{
		CString csAuxMsg;
		tbtrace(TM_INFO, _T("SendAuxMsgNotifyExitStoreMode Sending NewState = %s"), nextState);
		csAuxMsg.Format(_T("<NewState ExitAMState='%s' />"), nextState);    
		tbtrace(TM_INFO, csAuxMsg);
		// Send AuxMsg to sales app transaction when SCO exists Store Mode
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_EXITSTOREMODE); // "14" per POS request  
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("SendAuxMsgNotifyExitStoreMode failed to send to the sales app. HRESULT = 0x%08X:%s"),  cex.Error(),  (LPCTSTR)cex.ErrorMessage());
		return FALSE;
	}
}
//End CRD366323

//Start CRD 321191
BOOL CCustomerTBConnectPOS::SendAuxMsgNotifyCancelAlert()
{
	tbtrace(TM_INFO, _T("SendAuxMsgNotifyCancelAlert()")); 
	
	try
	{
		CString csAuxMsg;
		csAuxMsg.Format(_T("<AlertFeedback Canceled='1'/>"));
		// Send AuxMsg to sales app transaction when SCO attendant cancels alert
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_CANCELALERT); // "16" per POS request                            
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("SendAuxMsgNotifyCancelAlert failed to send to the sales app. HRESULT = 0x%08X:%s"),  cex.Error(),  (LPCTSTR)cex.ErrorMessage());
		return FALSE;
	}
}
//End CRD 321191

//Start CRD 338206
BOOL CCustomerTBConnectPOS::SendAuxMsgNotifyStoreModePrompt()
{
	tbtrace(TM_INFO, _T("SendAuxMsgNotifyStoreModePrompt()")); 
	
	try
	{
		CString csAuxMsg;
		csAuxMsg.Format(_T("<AssistMode Prompt='1'/>"));
		// Send AuxMsg to sales app transaction when SCO exists Store Mode
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_STOREMODEPROMPT); // "15" per POS request                            
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("SendAuxMsgNotifyStoreModePrompt failed to send to the sales app. HRESULT = 0x%08X:%s"),  cex.Error(),  (LPCTSTR)cex.ErrorMessage());
		return FALSE;
	}
}
//End CRD 338206

BOOL CCustomerTBConnectPOS::SendAuxMsgHHScannerActiveFlag(int hhScannerActive)
{
	tbtrace(TM_INFO, _T("SendAuxMsgHHScannerActiveFlag(%d)"), hhScannerActive);
	CString csAuxMsg;
	csAuxMsg.Format(_T("<Scanner HandHeld=\"%d\" />"), hhScannerActive);
	try
	{		
		// Send AuxMsg to sales app indicating that the data coming through hand held scanner
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_HHSCANNERFLAG);		
		return TRUE;
	}
	catch(_com_error & cex)
	{		
		tbtrace(TM_INFO, _T("SendAuxMsgHHScannerActiveFlag requires hook reference to be marshaled. Marshaling the hook and resending HHScanner flag..."));
		if( cex.Error() == RPC_E_WRONG_THREAD)
		{			
			try
			{
				GetHook(true)->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_HHSCANNERFLAG);
				return TRUE;
			}		
			catch (_com_error & cex2)
			{
				tbtrace(TM_ERROR, _T("SendAuxMsgHHScannerActiveFlag failed to send to the POS. HRESULT = 0x%08X:%s"), cex2.Error(), (LPCTSTR)cex2.ErrorMessage());
			}
		}
		//End CRD 299396
		return FALSE;
	}
}
//End CRD 338206

//Start CRD 541144
BOOL CCustomerTBConnectPOS::SendAuxMsgCitiAccountDetails(CString csAuxMsg)
{
	//	kbd->fl_notify.id = 17
	//	<AuxMsg MsgSeq="1">	
	//	<CitiApplication RefID='Ijr7utxAnkdPeqG9S2Ge47BiSt2SzwbB2JvZNJA18brGkhzMTr1qhwWtMJzeX7PY'	
	//			CaseNumber='060720ZG0K000'		
	//			CreditLimit='30000'	
	//			Status='0'
	//			Use = '1' />	
	//	</AuxMsg>
				
	//Note:
		//Status='0' -> Citi Application was completed/approved or it was declined by Citi Bank.
		//	(A decline is known because the RefID and CreditLimit values will be empty.)
		//	(A decline however will have a CaseNumber value.)
		//	Status='1' -> Citi Application canceled by customer before final approval.
		//	Use='0'     -> Do not use the approved account in the current transaction.
		//Use='1'     -> Use the approved account in the current transaction.

	tbtrace(TM_INFO, _T("SendAuxMsgCitiAccountDetails(%s)"), csAuxMsg); 
	
	try
	{
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_CITIACCOUNTDETAILS);                           
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("SendAuxMsgNotifyStoreModePrompt failed to send to the sales app. HRESULT = 0x%08X:%s"),  cex.Error(),  (LPCTSTR)cex.ErrorMessage());
		return FALSE;
	}


	return TRUE;
}
//End CRD 541144

BOOL CCustomerTBConnectPOS::SendAuxMsgNotifySCOLoadConfirmation()
{
	tbtrace(TM_INFO, _T("SendAuxMsgNotifySCOLoadConfirmation"));
	CString csAuxMsg;
	CString scoBuildVersion = CCustomerTrxBroker::instance()->TBGetProperty(_T("CurrentBuildNumber"));
	csAuxMsg.Format(_T("<PkgVersion version='%s'/>"), scoBuildVersion);
	tbtrace(TM_INFO, _T("SendAuxMsgNotifySCOLoadConfirmation AuxMsg= %s"), csAuxMsg);
	try
	{		
		// Send AuxMsg to sales app indicating that the data coming through hand held scanner
		GetHook()->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_SCOLOADCONFIRMATION);		
		return TRUE;
	}
	catch(_com_error & cex)
	{		
		tbtrace(TM_INFO, _T("SendAuxMsgNotifySCOLoadConfirmation requires hook reference to be marshaled. Marshaling the hook and resending SCOLoadConfirmation..."));
		if( cex.Error() == RPC_E_WRONG_THREAD)
		{			
			try
			{
				GetHook(true)->SendAuxMsg((LPCTSTR)csAuxMsg, FLNOTIFY_SCOLOADCONFIRMATION);
				return TRUE;
			}		
			catch (_com_error & cex2)
			{
				tbtrace(TM_ERROR, _T("SendAuxMsgNotifySCOLoadConfirmation failed to send to the POS. HRESULT = 0x%08X:%s"), cex2.Error(), (LPCTSTR)cex2.ErrorMessage());
			}
		}
		//End CRD 299396
		return FALSE;
	}
}