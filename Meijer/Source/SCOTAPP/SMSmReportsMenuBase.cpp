//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    SMSmReportsMenuBase.CPP
//
// TITLE:   Class implementation for Reports Menu state
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
//                                  // MS MFC template header
#ifndef _CPPUNIT
#include <afxpriv.h>                // for USES_CONVERSION //*** NewUI ***//
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "DMProcedures.h"
#include "SMStoreModeState.h"
#include "SMSmRunReports.h"
#include "SMSmAuthorization.h"
#include "SMSmPrintReceipt.h"
#include "ScotappMsg.h"
#else
#include "SMSmReportsMenuBase.h"
#include "FakeSMStoreModeMenuBase.h"
#include "TestMacros.h"
#endif

#ifndef _CPPUNIT
#define PROMPT       _T("???")
#define EXITCMD      _T("X")
#define EXITSIG      _T("G")
#define SIGNATURE    _T("SS")
#define STARTOFRP    _T("<<<")
#define ENDOFRP      _T(">>>")
#define ERRORLINE    _T("EE")
#define CSFILEPATH   _T("%DATA_DRIVE%\\scot\\report\\")
#define FASTLANEMENU _T("1")
#define MENUENTRY    _T("..")
#define COMP_ID ID_SM              
#define T_ID _T("SmReportsMenuBase")
#define MAX_TRACE    400

static bool bPrintedSignaturesAlready;
extern bool bReportServerOnline;	//RFC 262343
extern BOOL CALLBACK RPCallBack_Handler(const long Msg,
                                      const WPARAM wParam,
                                      const LPARAM lParam);
typedef BOOL (CALLBACK *FPRPEVENTHANDLER)(const long, const WPARAM, const LPARAM);

#endif //_CPPUNIT 


IMPLEMENT_DYNCREATE(SMSmReportsMenuBase, CObject)   // MFC Runtime class/object information

#ifndef _CPPUNIT

#define REPORTINGKEY        _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Reporting")                            
#define REPORTSERVERNAME    _T("ReportServer")
#define REPORTPORT          _T("TelnetPort")
#define DEFAULT_TELNET_PORT   1229
#define SIGNATURE_FILE _T("SignatureReport")
#define STANDARD_REPORT _T("StandardReport")

//Static Function
bool IsDoubleSpacedChar(TCHAR tch)
{
    if ( (tch >= 0x1100 && tch <= 0x11FF) || //Hangul Characters
         (tch == 0x2103)  || //Degree Celcius
         (tch >= 0x2150 && tch <= 0x2199) ||  //some in 2200-29FF    
	     (tch >= 0x2E80 && tch <= 0x30FA) || //Chinese, Japanese and Korean character except 30FB (Katakana Middle Dot)	
	     (tch >= 0x30FC && tch <= 0xA4CF) ||
	     (tch >= 0xAC00 && tch <= 0xFAD9) ||
	     (tch >= 0xFF00 && tch <= 0xFF60) ||
	     (tch >= 0xFFE0 && tch <= 0xFFE6) )
	{
	  return true;
	}
    return false;
}

#endif //_CPPUNIT 


DEFINE_TIMESTAMP

///////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////
SMSmReportsMenuBase::SMSmReportsMenuBase()
#ifndef _CPPUNIT
:m_bSignature(false),
 m_bReportCompletedSuccessfully(false),
 m_bProcessingMenus(false),
 m_bShowLastReceipt(false),
 m_bpressed(false),
 m_lReportStatus(NULL),
 m_bSignaturesPrinted(false),
 m_bErrorLineReceived(false),
 m_lEODInterval(1),
 m_EventSource(NULL),
 m_lReportLineIndex(0),
 m_csErrorLine(_T("")),
 m_csPartialString(_T("")),
 m_bRunEOD(false),
 m_bProcessingReport(false)
#endif //_CPPUNIT 

{
#ifndef _CPPUNIT

#ifndef NO_LOGGING
    m_EventSource = RegisterEventSource(NULL, _T("SCOTAPP"));
#endif
#endif //_CPPUNIT 

}
#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////
SMSmReportsMenuBase::~SMSmReportsMenuBase()
{

}            
///////////////////////////////////////////////////////////////////////////////
// Function:    Initialize                                 
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  void      
//              
//              
//              
//              
// Returns:      SendRPConnect;                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::Initialize(void)
{	
	CString csSlipFile = SLIP_FILE;
    CString csLReceiptFile = LRECEIPT_FILE;

    GET_PARTITION_PATH(SLIP_FILE, csSlipFile.GetBuffer(_MAX_PATH));
    GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));

    csSlipFile.ReleaseBuffer();
    csLReceiptFile.ReleaseBuffer();

	if (co.fOperationsReprintReceiptByTransNumber && !fSAInTransaction)
	{
		m_nPrintReceiptBttnTextId = BTT_PRINT_RECEIPT;
	}
	else
	{
		m_nPrintReceiptBttnTextId = BTT_PRINTLAST;
	}



	// if EOD is run (which runs the EOD.bat and reloads options), 
	// after reload options is completed it calls the calling state's
	// Initialize() function. In this case we don't want to go through the
	// Initialization routine.
	if (!m_bRunEOD)
	{
		_variant_t vtPointer;
		FPRPEVENTHANDLER fpnEventHandler;
		fpnEventHandler = RPCallBack_Handler;
		vtPointer.plVal = (long *)fpnEventHandler;
		try
		{
			rp.m_bTelnetActive = true;
			m_pTelnetIF.CreateInstance(_T("TelnetClientSupport.TelnetClientIF"));
			m_pTelnetIF->SetCallback(vtPointer);
		}
		catch (_com_error e)
		{
			#ifndef NO_LOGGING
			_bstr_t errString;
			LogComError(e,  (_bstr_t)_T("-SMSmReportsMenuBase::Initialize"));
			GoOffline();
			#endif
		}
		bool bShowReports = false;
		bool bShowLastReceipt = false;  //for Print Last Receipt module
		m_bInitialMenu = true;
		m_bDisplayReport = false;
		m_csRPT = EMPTY_STRING;   
		UpdateStateName(_T(""));
		if (dm.fStateTakeawayBelt)	// do not run belt in store mode  LPM091802 215283
		{
			csItemSent = EMPTY_STRING;       // forget last item code we TRIED to sell 
		}
	    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmReportsMenu") );
		ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
		if(gpSCOTAuxDisp)
			gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);

		bool bReturn2Shopping = false;			// Tar 213624 RJF 090602
		long lVerify = 0;

		dm.saveTriColorLightStatus();	//Tar 373985
		
        // (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                           DM_TRICOLORLIGHT_OFF,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
		// (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                           DM_TRICOLORLIGHT_OFF,
                           DM_TRICOLORLIGHT_NORMALREQUEST);

		// Prompt is blank initially.
		ps.Echo(PS_BLANK);
		ps.SetFrame(_T("SmReportsMenu")); //*** NewUI ***// 
		ps.SetTitle(LTT_REPORTING, PS_BLANK);
		ps.SetLeadthruText(PS_BLANK, PS_BLANK);

		
		// If option to log data for reports is On then we show the 2 report buttons. 
		if (co.fOperationsLogDataForSystemReports)
		{
			ps.SetLeadthruText(LTT_REPORT);//Reporting instructional txt
			ps.Button(_T("SMButton1"), BTT_SELECTREPORT, false);
			ps.Button(_T("SMButton3"), BTT_PRINTSIGN, false);	
			ps.Button(_T("SMButton4"), BTT_RECONNECT, false);
			//For REPORT EOD Button 
			if (co.nOperationsEOD == OPTION_OPERATOR_CONTROL)
				ps.SetButtonText(_T("SMButton7"), BTT_ENDREPORTPERIOD);
			else 
				ps.SetButtonText(_T("SMButton7"), PS_NOTEXT);
		}
		else 
		{
			ps.SetLeadthruText(PS_BLANK, PS_BLANK);
			ps.SetButtonText(_T("SMButton1"), PS_NOTEXT);
			ps.SetButtonText(_T("SMButton3"), PS_NOTEXT);
			ps.SetButtonText(_T("SMButton4"), PS_NOTEXT);
			ps.SetButtonText(_T("SMButton7"), PS_NOTEXT);
		}
		// TAR 236945 - Needed for Remote, this api will check option whether to display or not.
		ps.Weight(lDMScaleWeight);

		//For Print Last Receipt module
		//Fiscal Printer start
		if(! co.fOperationsTBControlsPrinter)
		{
			//Fiscal printer end
			if (FileExist(csLReceiptFile)||FileExist(csSlipFile)) 
			{
				m_bShowLastReceipt = true;
				ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId, m_bShowLastReceipt); 
				//Fiscal Printer start
			}
			else
				ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId, false); 
		}
		ps.Button(_T("SMButton8"), BTT_GOBACK, true);
		ps.ResetReport(_T("SMReportReceipt"));


		// 206331 LPM lane light not blinking after going into media status even if printer is low
		if (dm.GetPrinterPaperLow())
		{
			// (*) SR898 ====================
            DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                               DM_TRICOLORLIGHT_BLINK_1HZ,
                               DM_TRICOLORLIGHT_NORMALREQUEST);  
		}
		if (co.fOperationsLogDataForSystemReports)
		{
			ps.ShowFrame();
			m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_FALSE);
			trace(L6, _T("initialize - REDRAW is set to FALSE") );
			return SendRPConnect();
		}
		else 
		{
			//Reporting not configured
			ps.ResetReport(_T("SMReportReceipt"));
			m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_TRUE);
			trace(L6, _T("initialize - REDRAW is set to TRUE") );
			ps.ShowFrame();
			return STATE_NULL;
		}
   } //if (!m_bRunEOD)
   else
   {
	   // just reset the flag and stay here
	   m_bRunEOD = false;
	   return STATE_NULL;
   }

}
void SMSmReportsMenuBase::UnInitialize()
{
    try
    {
        rp.m_bTelnetActive = false;
        m_pTelnetIF->Disconnect();
	    m_pTelnetIF = NULL; // Delete object
    }
    catch (_com_error e)
    {
    }
 	SMStoreModeStateBase::UnInitialize();
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmReportsMenu") );
	m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_TRUE);
	trace(L6, _T("initialize - REDRAW is set to TRUE") );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton1 Select Report Button                                 
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  void      
//              
//              
//              
//              
// Returns:      return STATE_NULL;                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::PSButton1(void)
{
    printerUtils.ResetReceiptPrinter();
    USES_CONVERSION;
    CString csDesc, csCode;
    COleVariant vValue;
    CString csSelectedIndex;

    csCode = ps.GetSelection(_T("SMReportReceipt"));
    m_pAppPSX->GetReceiptItemVariable(_T("SMReportReceipt"), csCode, _T("ITEM_SMTEXT"), vValue);
    vValue.ChangeType( VT_BSTR );
    m_csCurrentReport = OLE2T(vValue.bstrVal);
    m_pAppPSX->GetReceiptItemVariable(_T("SMReportReceipt"), csCode, _T("ITEM_SMINDEX"), vValue);
    vValue.ChangeType( VT_BSTR );
    csSelectedIndex = OLE2T(vValue.bstrVal);

    m_bpressed= true;
    //TAR 234371
    vValue = VARIANT_FALSE;
    ps.ResetReport(_T("SMReportReceipt"));//Reset Report form
    m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, vValue);
	trace(L6, _T("psbutton1 - REDRAW is set to FALSE") );

    m_bProcessingMenus = false;// Hide button presses if Processing menu selections
    ps.Button(_T("SMButton1"), BTT_SELECTREPORT, false);
    ps.Button(_T("SMButton3"), BTT_PRINTSIGN, false);
    ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId, false);
    //For REPORT EOD Button 
    if (co.nOperationsEOD == OPTION_OPERATOR_CONTROL)
        ps.Button(_T("SMButton7"), BTT_ENDREPORTPERIOD, false);
    try
    {
        m_pTelnetIF->TransmitString(_bstr_t((LPCTSTR)csSelectedIndex));     

    }
    catch(_com_error e)
    {
        #ifndef NO_LOGGING
        _bstr_t errString;
        LogComError(e,  (_bstr_t)_T("-SMSmReportsMenuBase::PSButton1"));
        GoOffline();
        #endif
    }
    ps.Echo(LTT_RP_PROCESSINGREPORT);

    m_bProcessingReport = true; //RFC 262343

    return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton2 Print Last Receipt button                                  
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  void      
//              
//              
//              
//              
// Returns:      return STATE_NULL;                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::PSButton2(void)
{
  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("Print Last Receipt") ) ;

  if (co.fOperationsReprintReceiptByTransNumber && !fSAInTransaction)
  {
	  RETURNSTATE(SmPrintReceipt);
  }
  else
  {
	  // I know -- these should be member variables.
	  CString csSlipFile = SLIP_FILE;
	  CString csLReceiptFile = LRECEIPT_FILE;
	  CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;

	  GET_PARTITION_PATH(SLIP_FILE, csSlipFile.GetBuffer(_MAX_PATH));
	  GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));
	  GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));

	  csSlipFile.ReleaseBuffer();
	  csLReceiptFile.ReleaseBuffer();
	  csLReceiptTmpFile.ReleaseBuffer();
  
	  PrintLastReceipt(csSlipFile);      //print slip receipt
	  if (FileExist(csLReceiptTmpFile))
	  {
		  PrintLastReceipt(csLReceiptTmpFile);  //print receipt
	  }
	  else
	  {
		  PrintLastReceipt(csLReceiptFile);  //print receipt
	  }
	  return STATE_NULL;
  }
}
///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton3 Print Signature Button                                  
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  void      
//              
//              
//              
//              
// Returns:      return STATE_NULL;                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::PSButton3(void)
{
    printerUtils.ResetReceiptPrinter();
    m_bSignature = true;
    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("Print Signature") ) ;
    ps.Button(_T("SMButton1"), BTT_SELECTREPORT, false);
    ps.Button(_T("SMButton3"), BTT_PRINTSIGN, false);
    ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId, false);
    //For REPORT EOD Button 
    if (co.nOperationsEOD == OPTION_OPERATOR_CONTROL)
        ps.Button(_T("SMButton7"), BTT_ENDREPORTPERIOD, false);
    ps.Button(_T("SMButton8"), BTT_GOBACK, false);
    m_bSignaturesPrinted = true;
    m_csCurrentReport = SIGNATURE_FILE;
    try
    {
        // Initiate the signature capture report from the server
        m_pTelnetIF->TransmitString(_bstr_t(SIGNATURE));
    }
    catch(_com_error e)
    {
      #ifndef NO_LOGGING
      _bstr_t errString;
      LogComError(e,  (_bstr_t)_T("-SMSmReportsMenuBase::PSButton3"));
      GoOffline();
      #endif
    }
    ps.Echo(LTT_RP_PROCESSINGREPORT);
    return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    SigReceiptFileFound                                  
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  void      
//              
//              
//              
//              
// Returns:     bool                                        
///////////////////////////////////////////////////////////////////////////////////
bool  SMSmReportsMenuBase::SigReceiptFileFound()
{
   return m_SigReceipt.FileAvailable()  ;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    PrintSigReceiptFile                                  
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  void      
//              
//              
//              
//              
// Returns:     void                                        
///////////////////////////////////////////////////////////////////////////////////
void  SMSmReportsMenuBase::PrintSigReceiptFile()
{
    printerUtils.ResetReceiptPrinter();
    if (FALSE == m_SigReceipt.EnterSignaturesFromFile(m_csCurrentReport))
    {
        // Trace error
        m_pTelnetIF->TransmitString(_bstr_t(EXITCMD));
	    trace(L6, _T("Error entering signature file"));
        ps.SetLeadthruText(LTT_PRINT_RECEIPT_FAILED, PS_BLANK);
    }
    else
    {
        // Attempt to print the file regardless of whether we were successful from host
        if (m_SigReceipt.PrintSigReceiptFile() )
        {
            m_pTelnetIF->TransmitString(_bstr_t(EXITSIG));
	        trace(L6, _T("Signature print spool succeeded"));
            ps.SetLeadthruText(LTT_PRINT_RECEIPT_SUCCESS, PS_BLANK);
        }
        else
        {
            m_pTelnetIF->TransmitString(_bstr_t(EXITCMD));
            trace(L6, _T("Signature print spool failed"));
            ps.SetLeadthruText(LTT_PRINT_RECEIPT_FAILED, PS_BLANK);
        }
    }
    //TAR 195179 & 191490 start
    bPrintedSignaturesAlready = m_bSignaturePrintingStatus;
    if (m_csCurrentReport != EMPTY_STRING)
    {
		STATE(StoreModeState)::SCOTDeleteFile(m_csCurrentReport);   //TAR 348946 - Delete Existing Report File
         m_csCurrentReport = EMPTY_STRING;
         m_csRPT           = EMPTY_STRING;
    }
    if (bPrintedSignaturesAlready)
    {
        ps.Button(_T("SMButton3"),BTT_PRINTSIGN,false);
    }
 ps.Echo(PS_BLANK);
}
///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton4 Reconnect Button                                  
// Class:       SMSmReportsMenuBase                                              
// Description: Will send a RPConnect when button is pressed
// Parameters:  void      
//              
//              
//              
//              
// Returns:      SendRPConnect;                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::PSButton4(void)
{
	// I know -- these should be member variables.
    CString csSlipFile = SLIP_FILE;
    CString csLReceiptFile = LRECEIPT_FILE;

    GET_PARTITION_PATH(SLIP_FILE, csSlipFile.GetBuffer(_MAX_PATH));
    GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));

    csSlipFile.ReleaseBuffer();
    csLReceiptFile.ReleaseBuffer();

    m_bSignature                   = false;
    m_bReportCompletedSuccessfully = false;
    m_bProcessingMenus             = false;
    m_bpressed                     = false;
    m_bErrorLineReceived           = false;
   	m_bDisplayReport               = false;
    if(! co.fOperationsTBControlsPrinter)
    {
	    //Fiscal printer end
       if (FileExist(csLReceiptFile)||FileExist(csSlipFile)) 
	       m_bShowLastReceipt = true;
        else
           m_bShowLastReceipt = false;
    }
    ps.ResetReport(_T("SMReportReceipt"));
//  m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_FALSE);
//	trace(L6, _T("psbutton4 - REDRAW is set to FALSE") );

    ps.Button(_T("SMButton4"), BTT_RECONNECT, false);
    return SendRPConnect();
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton7 End of Report Period Button                                  
// Class:       SMSmReportsMenuBase                                              
// Description: OperatorControl option allows for initiation of EOD in response
//              to a button press on the report screen
// Parameters:  void      
//              
//              
//              
//              
// Returns:     RETURNSTATE(STATE_NULL)                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::PSButton7(void)
{
    printerUtils.ResetReceiptPrinter();
    int iYear   = 0;
    int iDay    = 0;
    int iMonth  = 0;
    int i = -1;
    LONG     lRC;
	HKEY     hKey;
	TCHAR  szDateSize[_MAX_PATH+1];
	DWORD dwDateSize=_MAX_PATH;
	CString csRegKey = _T("CurrentDate");
    CTime ctCurrentTime;
    ctCurrentTime = CTime::GetCurrentTime();
    CString csTime, csMonth,csYear,csDay,csBusinessDate;
    CTime ctComputedTime;
    CTime *pctBusinessDate;
    CTime *pctCurrentDate;
    CTimeSpan *ctInterval;
	csTime.Format(_T("%.4d-%.2d-%.2d"),
		        ctCurrentTime.GetYear(),
		        ctCurrentTime.GetMonth(),
		        ctCurrentTime.GetDay());
    // Is there a business date (Should be true unless we haven't
    // run a transaction yet
    if (rp.m_csBusinessDate == EMPTY_STRING)
    {
        //Begin Retrieve Business date from registry entry
        lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Reporting\\ReportEOD"),
				 0, KEY_ALL_ACCESS, &hKey);

        lRC = ::RegQueryValueEx(hKey, csRegKey, NULL, NULL,
			  (LPBYTE)szDateSize, &dwDateSize);

		if (lRC == ERROR_SUCCESS)
        {
           rp.m_csBusinessDate = szDateSize;
           //If business date is empty place current date in registry 
           if (rp.m_csBusinessDate == EMPTY_STRING)
               rp.m_csBusinessDate = csTime;
            
        }
        lRC = ::RegSetValueEx(hKey,
                _T("CurrentDate"), 
                0, 
                REG_SZ,
                ( LPBYTE )(LPCTSTR)rp.m_csBusinessDate,
                rp.m_csBusinessDate.GetLength());
	    if (lRC != ERROR_SUCCESS)
	    {
		trace(L7, _T("SMSmReportsMenuBase::PSButton7 Error writing ReportEOD to registry key"));
	    }
        TBSetBusinessDate((LPCTSTR)rp.m_csBusinessDate);
		::RegCloseKey(hKey);
    }
    else
    {
        // We have a business date coming into the routine
        // Parse the string for the day month year
        csBusinessDate = rp.m_csBusinessDate;
        i = csBusinessDate.Find(_T('-'));
        if (i>=0) 
        {
            csBusinessDate.SetAt(i,_T('-'));
            int j = csBusinessDate.Find(_T('-'));
            csYear = csBusinessDate.Left(j);
            csBusinessDate = csBusinessDate.Mid(j+1);
            j = csBusinessDate.Find(_T('-'));
            if (j>=0) 
            {
                int k = csBusinessDate.Find(_T('-'));
                csMonth = csBusinessDate.Left(k);
                csDay = csBusinessDate.Mid(k+1);
                iYear   = _ttoi(csYear);
                iMonth  = _ttoi(csMonth);
                iDay    = _ttoi(csDay);
            }
            else
            {
		        trace(L6, _T("SMSmReportsMenuBase::PSButton7 business date in wrong format"));
            }
        }
        else
        {
		    trace(L6, _T("SMSmReportsMenuBase::PSButton7 business date in wrong format"));
        }
        pctBusinessDate = new CTime(iYear, iMonth, iDay, 0, 0, 0);
        ctCurrentTime = CTime::GetCurrentTime();
        pctCurrentDate = new CTime(ctCurrentTime.GetYear(), ctCurrentTime.GetMonth(), ctCurrentTime.GetDay(), 0, 0,0);
    
          //If Business date is less than change to Current date  
        if (*pctBusinessDate < *pctCurrentDate)
        {
			m_bRunEOD = true;
            rp.m_csBusinessDate = csTime;
            hKey = NULL;
	        long lResult = 0L;
      	    trace(L7, _T("SMSmReportsMenuBase::PSButton7 PutReportEOD in registry"));
            lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Reporting\\ReportEOD"),
				     0, KEY_SET_VALUE, &hKey);
	        if (lResult != ERROR_SUCCESS)
            {
		        trace(L7, _T("Error opening registry key to store ReportEOD")); 
	        }	 
            lResult = ::RegSetValueEx(hKey,
                    _T("CurrentDate"), 
                    0, 
                    REG_SZ,
                    ( LPBYTE )(LPCTSTR)rp.m_csBusinessDate,
                    rp.m_csBusinessDate.GetLength());
	        if (lResult != ERROR_SUCCESS)
	        {
		        trace(L7, _T("SMSmReportsMenuBase::PSButton7 Error writing ReportEOD to registry key"));
	        }
	        ::RegCloseKey(hKey);
            ps.SetLeadthruText(LTT_REPORTEOD, PS_BLANK);
            //Notify TB of business date change
            TBSetBusinessDate((LPCTSTR)rp.m_csBusinessDate);
         }
        //If Business date equals Current date increment by 1 
        if (*pctBusinessDate == *pctCurrentDate)
        {
			m_bRunEOD = true;
            // Compute and enter next business date
            ctCurrentTime = CTime::GetCurrentTime();
            ctInterval = new CTimeSpan(m_lEODInterval, 0, 0, 0);
            ctComputedTime = ctCurrentTime + *ctInterval;
            csTime.Format(_T("%.4d-%.2d-%.2d"),
		                ctComputedTime.GetYear(),
		                ctComputedTime.GetMonth(),
		                ctComputedTime.GetDay()); 
            rp.m_csBusinessDate = csTime ;
            hKey = NULL;
	        long lResult = 0L;
      	    trace(L7, _T("SMSmReportsMenuBase::PSButton7 PutReportEOD in registry"));
            lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Reporting\\ReportEOD"),
				     0, KEY_SET_VALUE, &hKey);
	        if (lResult != ERROR_SUCCESS)
            {
		        trace(L7, _T("Error opening registry key to store ReportEOD"));
	        }	 
            lResult = ::RegSetValueEx(hKey,
                    _T("CurrentDate"), 
                    0, 
                    REG_SZ,
                    ( LPBYTE )(LPCTSTR)rp.m_csBusinessDate,
                    rp.m_csBusinessDate.GetLength());
	        if (lResult != ERROR_SUCCESS)
	        {
		        trace(L7, _T("SMSmReportsMenuBase::PSButton7 Error writing ReportEOD to registry key"));
	        }
	        ::RegCloseKey(hKey);
            ps.SetLeadthruText(LTT_REPORTEOD, PS_BLANK);
            //Notify TB of business date change
            TBSetBusinessDate((LPCTSTR)rp.m_csBusinessDate);
        }
       //If Business date is greater than current: Invalid condition 
       if (*pctBusinessDate > *pctCurrentDate)
       {
            trace(L7, _T("Invalid condition: Business date greater than current date"));
       }
       delete pctBusinessDate;
       delete pctCurrentDate;
   }

   //TAR 237933
   co.csBusinessDate = (LPCTSTR)rp.m_csBusinessDate;
   //Disable EOD button after initial button press
   ps.Button(_T("SMButton7"), BTT_ENDREPORTPERIOD, false);
   if (m_bRunEOD)
   {
	 // perform EOD, consists of running the EOD.BAT file and reloading
	 // options
     return PerformEOD();
   }
   return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton8 Go-Back Button                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  void      
//              
//              
//              
//              
// Returns:     RETURNSTATE(SmAuthorization)                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::PSButton8(void)
{
	m_bInitialMenu = true;
    ps.ResetReport(_T("SMReportReceipt"));//Reset Report form
//	m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_FALSE);
//	trace( L6, _T("PSButton8 - REDRAW is set to FALSE "));            
    ps.ShowSMTotal(false);
	if (ps.RemoteMode())
	{
		dm.restoreTriColorLight();//Tar 373985
		return createAnchorState();
	}
	else
    {
        //Return to current store mode menu state instead of SmAuthorization
        return STATE(StoreModeMenu)::CreateStoreModeMenu();
    }
}
///////////////////////////////////////////////////////////////////////////////
// Function:    ParseString                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  csInString      
//              
//              
//              
//              
// Returns:     NONE                                        
///////////////////////////////////////////////////////////////////////////////////
void SMSmReportsMenuBase::ParseString(CString csInString)
{
	bool bkeepGoing = true;;
	int nextPos;
	CString csleftString;
	CString csrightString;
	CString cstempString;
	CString csCR;
	csCR = _TCHAR(13);
	csrightString = m_csPartialString + csInString;
	if (csInString.GetLength() > MAX_TRACE + 3)
	{
	   trace(L6, _T("ParseString Input %d [%s...%s]"), 
            csInString.GetLength(), 
            csInString.Left(MAX_TRACE/2), 
            csInString.Right(MAX_TRACE/2));
	}
	else
	{
	   trace(L6, _T("ParseString Input %d [%s]"), csInString.GetLength(), csInString);
	}

	while (bkeepGoing)
	{
		nextPos = csrightString.Find(csCR);
		if  (nextPos > -1)
		{
			bkeepGoing = true;
			csleftString  = csrightString.Left(nextPos - 1);
			cstempString  = csrightString.Mid(nextPos + 1);
			csrightString = cstempString;
			if (m_bDisplayReport == false)
			{
                // If menu line (..), process it 
                if (!ParseMenuLine(csleftString, bkeepGoing))
                {
                    // Otherwise check for prompt (???) line and process it if it is
                    if (!ParsePrompt(csleftString, bkeepGoing))
                    {
                        // Otherwise check for error line (EE)
                        if (!ParseErrorLine(csleftString))
                        {
                            // Otherwise check for start of report line (<<<)
                            ParseStartReport(csleftString);
                        }

                    }
				}
			}
			else
			{
				CString csConvertedString;
				if (co.csPrimaryLanguageCode == _T("0804") ||
				    co.csPrimaryLanguageCode == _T("0411") ||
				    co.csPrimaryLanguageCode == _T("0412") ||
					co.csPrimaryLanguageCode.CompareNoCase(_T("0C04")) == 0) //Applies to Chinese Mandarin , Japanese and Korean Reports
				{
					trace(L9, _T("SMSmReportsMenuBase::ParseString. Original Size of the String: %d"), csleftString.GetLength());
					//Handle Removal of extra spaces equivalent to the  space occupied by the 2-spaced characters
					//Case 1: if length >= 35, then assume the line contains data positioned in columns
					if (csleftString.GetLength() >= 35)
					{
						csConvertedString = HandleColumnarLine(csleftString);
					}
					//Case 2: Not In Columns and Start with Space, then assume this is Centered
					//    ACTION: remove leading spaces determined by half of the double-spaced chars
					else if (csleftString.GetAt(0) == 0x20) //if LINE starts with space
					{
						csConvertedString = HandleCenterLine(csleftString);
					}
					//Other Cases: remove trailing space for every double-spaced char
					else 
					{
						csConvertedString = HandleOther(csleftString);
					}
					trace(L9, _T("SMSmReportsMenuBase::ParseString. Original Size of the String: %d"), csleftString.GetLength());
				}
				else
				{
					csConvertedString = csleftString;
				}
                ParseReportLine(csConvertedString);
			}

		}
		else
		{
			bkeepGoing = false;
            m_csPartialString = csrightString;
			if (m_csPartialString.GetLength() > MAX_TRACE + 3)
			{
			   trace(L7, _T("ParseString m_csPartialString %d [%s...%s]"), 
				      m_csPartialString.GetLength(), 
			         m_csPartialString.Left(MAX_TRACE/2), 
			         m_csPartialString.Right(MAX_TRACE/2));
			}
			else
			{
			   trace(L7, _T("ParseString m_csPartialString %d [%s]"), 
			         m_csPartialString.GetLength(), m_csPartialString);
			}
		}
   }

}

///////////////////////////////////////////////////////////////////////////////
// Function:    ParseReportLine                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  csReportLine      
//              
//              
//              
//              
// Returns:     void                                        
///////////////////////////////////////////////////////////////////////////////////
void SMSmReportsMenuBase::ParseReportLine(CString &csReportLine)
{
	int nextPos;
    CString	csPage = _TCHAR(12);
    int iPagePosition;
    CString cstempString;

    // Report Text input
	nextPos = csReportLine.Find(ENDOFRP);
	if (nextPos == 0)
	{
        // End of report was received
        // CLOSE FILE, PROCEED TO NEXT MENU
     	m_bDisplayReport = false;
        CloseFile();
        m_bReportCompletedSuccessfully = true;
        //TAR 234371 Need to reset scroll after finish with processing report
        COleVariant vReturn, vIndex = (long)UI::ScrollTop;
        m_pAppPSX->SendCommand( _T("SMReportReceipt"), _T("Scroll"), vReturn, 1, vIndex );
	}

	else
	{
	
        csReportLine = csReportLine.Left(csReportLine.GetLength() );
		iPagePosition = csReportLine.Find(csPage);
		if  (iPagePosition == 0)
        {
            // We found a page character at the beginning of the line
            // Get rid of it
			cstempString  = csReportLine.Mid(1);
			csReportLine = cstempString;

        }
        if (!m_bSignature)
        {
            AddToReport(csReportLine, _T(""));
        }
        WriteRecord(csReportLine);
        // SAVE LINE TO FILE
	}
}
///////////////////////////////////////////////////////////////////////////////
// Function:    ParsePrompt                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  csReportLine      
//              
//              
//              
//              
// Returns:     true if prompt, otherwise false                                       
///////////////////////////////////////////////////////////////////////////////////
bool SMSmReportsMenuBase::ParsePrompt(CString &csReportLine, bool &bKeepGoing)
{
	int nextPos;
    bool bReturnCode = false;

    nextPos = csReportLine.Find(PROMPT);
    if (nextPos != -1)
    {
        bReturnCode = true;
        if (m_bReportCompletedSuccessfully)
        {
            // Disconnect
            try
            {
                if (m_bSignature)
                {
                    PrintSigReceiptFile();
                }
                else
                {
                    m_pTelnetIF->TransmitString(_bstr_t(EXITCMD));
                }
            }
            catch(_com_error e)
            {
                #ifndef NO_LOGGING
                _bstr_t errString;
                LogComError(e,  (_bstr_t)_T("-SMSmReportsMenuBase::ParsePrompt"));
                GoOffline();
                #endif
            }
            bKeepGoing = false;
           
        }
        else if (m_bProcessingMenus)
        {
		    if (m_bInitialMenu)
		    {
			    m_bInitialMenu = false;
                try
                {
                    m_pTelnetIF->TransmitString(_bstr_t(FASTLANEMENU));
                }
                catch(_com_error e)
                {
                    #ifndef NO_LOGGING
                    _bstr_t errString;
                    LogComError(e,  (_bstr_t)_T("-SMSmReportsMenuBase::ParseString"));
                    GoOffline();
                    #endif
                }
			    bKeepGoing = false;
            }
            else
            {

                COleVariant vReturn, vIndex = (long)0;
                m_pAppPSX->SendCommand( _T("SMReportReceipt"), _T("SetCurSel"), vReturn, 1, vIndex );
                vIndex = VARIANT_TRUE;
                m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, vIndex);
				trace(L6, _T("ParsePrompt - REDRAW is set to TRUE,m_bProcessingMenus %d m_bInitialMenu %d "), m_bProcessingMenus,m_bInitialMenu );

            }
            m_bProcessingMenus = false;
            if (co.fOperationsLogDataForSystemReports)
            {
                ps.Button(_T("SMButton1"), BTT_SELECTREPORT, !m_bpressed);
				// +TAR SSCOP-75 kr185069 2011/08/03
				if (!co.m_bOperationsPrintSignatureReceipt){	
					 ps.Button(_T("SMButton3"), BTT_PRINTSIGN, false);
					
				}else{
					 ps.Button(_T("SMButton3"), BTT_PRINTSIGN, !m_bpressed && !m_bSignaturesPrinted);
				}
				// -TAR SSCOP-75 kr185069 2011/08/03               
                ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId,!m_bpressed && m_bShowLastReceipt);
                ps.Button(_T("SMButton4"), BTT_RECONNECT, false);
                //For REPORT EOD Button 
                if (co.nOperationsEOD == OPTION_OPERATOR_CONTROL)
                    ps.Button(_T("SMButton7"), BTT_ENDREPORTPERIOD, !m_bpressed);
            }
        }


    }
    return bReturnCode;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ParseMenuLine                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  csReportLine      
//              
//              
//              
//              
// Returns:     true if prompt, otherwise false                                       
///////////////////////////////////////////////////////////////////////////////////
bool SMSmReportsMenuBase::ParseMenuLine(CString &csReportLine, bool &bKeepGoing)
{
	int nextPos;
    bool bReturnCode = false;

    // If menu line
    nextPos = csReportLine.Find(MENUENTRY);
	if (nextPos == 0) 
	{
        bReturnCode = true;
        m_bProcessingMenus = true;
        // If first menu, then select fastlane automatically
		if (!m_bInitialMenu)
		{
			csReportLine = csReportLine.Mid(3);
            int ispaceIndex;
            CString csIndex, csText;
            CString csMenuLine = csReportLine;
            // Find the end of the menu number
            ispaceIndex = csMenuLine.Find(_T(" "));
            csIndex = csMenuLine.Left(ispaceIndex);
            //Find the Report Desc to display via screen
            csText = csMenuLine.Mid(ispaceIndex+1);
            // Only add it to the list if the index is not the exit line
            // and is not the signature capture line DEM
            if (( EXITCMD != csIndex) && (SIGNATURE != csIndex))
            {
                AddToReport(csText, csIndex);
               
            }
            
   		}
    }
    return bReturnCode;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ParseErrorLine                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  csReportLine      
//              
//              
//              
//              
// Returns:     true if prompt, otherwise false                                       
///////////////////////////////////////////////////////////////////////////////////
bool SMSmReportsMenuBase::ParseErrorLine(CString &csReportLine)
{
	int nextPos;
    bool bReturnCode = false;
    CString csErrorLine;
    int ispaceIndex;

    // Otherwise check for error line
    nextPos = csReportLine.Find(ERRORLINE);
    if (nextPos != -1)
    {
        bReturnCode = true;
        m_bErrorLineReceived = true;
        // Error line received
        csErrorLine = csReportLine;
        // Find the end of the menu number
        ispaceIndex = csErrorLine.Find(_T(" "));
        //Find the Report Desc to display via screen
        m_csErrorLine = csErrorLine.Mid(ispaceIndex+1);
        trace(L7, _T("ParseErrorLine m_csErrorLine %d [%s]"), m_csErrorLine.GetLength(), m_csErrorLine);
    }
    return bReturnCode;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ParseStartReport                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  csReportLine      
//              
//              
//              
//              
// Returns:     true if prompt, otherwise false                                       
///////////////////////////////////////////////////////////////////////////////////
bool SMSmReportsMenuBase::ParseStartReport(CString &csReportLine)
{
	int nextPos;
    bool bReturnCode = false;

    // If start of report
	nextPos = csReportLine.Find(STARTOFRP);
	if (nextPos != -1) 
	{
        bReturnCode = true;
        // Start of report received
        // OPEN FILE
        OpenFile();
        m_bReportCompletedSuccessfully = false;
		m_bDisplayReport = true;
	}

    return bReturnCode;
}


///////////////////////////////////////////////////////////////////////////////
// Function:    OnRPReceived                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  byteCount      
//              
//              
//              
//              
// Returns:     NONE                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::OnRPReceived(long byteCount)
{
	_bstr_t *bString;
	CString csString;
    BSTR pbstrInputString;
    try 
    {
       m_pTelnetIF->ReceiveString(&pbstrInputString, byteCount);
	   trace(L6, _T("SMSmReportsMenuBase::OnReceived Input [%d]"), byteCount);
    }
    catch (_com_error e)
    {
       #ifndef NO_LOGGING
       _bstr_t errString;
       LogComError(e,  (_bstr_t)_T("-SMSmReportsMenuBase::OnRPReceived"));
       GoOffline();
       #endif
       return STATE_NULL; // TAR 343530
    }
    bString = new _bstr_t(pbstrInputString,false);
	csString = (wchar_t *) (*bString);
    delete bString;      
	ParseString(csString);

	//RFC 262343 - start
	if (!bReportServerOnline && !csString.IsEmpty())
	{
		ScotError(INFO, 
              SM_CAT_INTERNAL_INTERFACE,
              FLR_REPORTSERVER_NOW_ONLINE,
              (NCRSM_SNMP_ACTION | NCRSM_REMOTE_LOG_ACTION));  
		bReportServerOnline = true;
	}
	//RFC 262343 - end
	
	return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    OnRPDisconnect                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  NONE       
//              
//              
//              
//              
// Returns:     NONE                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::OnRPDisconnect()
{
	m_bInitialMenu = true;
    m_csPartialString = _T("");
    if (m_bReportCompletedSuccessfully)
    {
        m_bReportCompletedSuccessfully = false;
        // If we have retrieved the signature file, now is the time to process it
        // otherwise we've finished a report and need to shift to that screen
        if (m_bSignature)
        {
            m_bSignature = false;
            ps.ResetReport(_T("SMReportReceipt"));
            m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_FALSE);
			trace(L6, _T("ONRPDisconnect 1 - REDRAW is set to false") );
            if (m_bErrorLineReceived)
            {
               AddToReport(m_csErrorLine, _T(""));
            }
            m_bErrorLineReceived = false;  // Should already be false 
        	ps.Button(_T("SMButton8"), BTT_GOBACK, true);
            if (co.fOperationsLogDataForSystemReports)
            {
                ps.Button(_T("SMButton1"), BTT_SELECTREPORT, !m_bpressed);
				// +TAR SSCOP-75 kr185069 2011/08/03
				if (!co.m_bOperationsPrintSignatureReceipt){	
					 ps.Button(_T("SMButton3"), BTT_PRINTSIGN, false);
					
				}else{
					 ps.Button(_T("SMButton3"), BTT_PRINTSIGN, !m_bpressed && !m_bSignaturesPrinted);
				}
				// -TAR SSCOP-75 kr185069 2011/08/03
                ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId,!m_bpressed && m_bShowLastReceipt);
                ps.Button(_T("SMButton4"), BTT_RECONNECT, false);
                //For REPORT EOD Button 
                if (co.nOperationsEOD == OPTION_OPERATOR_CONTROL)
                    ps.Button(_T("SMButton7"), BTT_ENDREPORTPERIOD, !m_bpressed);
            }
            return SendRPConnect();							  
        }
        else
        {
            // Check to see if the report completed but no data was returned
            if (m_bErrorLineReceived && m_bpressed)
            {
                // Clear the screen and display the error message displayed
                ps.ResetReport(_T("SMReportReceipt"));
//              m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_FALSE);
//				trace(L6, _T("ParsePrompt 2 - REDRAW is set to FALSE") );
                AddToReport(m_csErrorLine, _T(""));
                if (m_csRPT == EMPTY_STRING)
                {
                    m_csRPT = m_csCurrentReport;
                }
                // Clear the report name to prevent print button from being enabled
                if (m_csCurrentReport != EMPTY_STRING)
                {
					STATE(StoreModeState)::SCOTDeleteFile(m_csCurrentReport);   //TAR 348946 - Delete Existing Report File
                    m_csCurrentReport = EMPTY_STRING;
                }
            }
            m_bErrorLineReceived = false;  // Should already be false 
        	ps.Button(_T("SMButton8"), BTT_GOBACK, true);
            if (co.fOperationsLogDataForSystemReports)
            {
                ps.Button(_T("SMButton1"), BTT_SELECTREPORT, !m_bpressed);
                // +TAR SSCOP-75 kr185069 2011/08/03
				if (!co.m_bOperationsPrintSignatureReceipt){	
					 ps.Button(_T("SMButton3"), BTT_PRINTSIGN, false);
					
				}else{
					 ps.Button(_T("SMButton3"), BTT_PRINTSIGN, !m_bpressed && !m_bSignaturesPrinted);
				}
				// -TAR SSCOP-75 kr185069 2011/08/03
                ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId,!m_bpressed && m_bShowLastReceipt);
                ps.Button(_T("SMButton4"), BTT_RECONNECT, false);
                //For REPORT EOD Button 
                if (co.nOperationsEOD == OPTION_OPERATOR_CONTROL)
                    ps.Button(_T("SMButton7"), BTT_ENDREPORTPERIOD, !m_bpressed);
            }
			m_bProcessingReport = false; //RFC 262343
            RETURNSTATE(SmRunReports);
        }
    }
    // Check to see if an error message was returned to an attempt
    // to print a report
    else if (m_bErrorLineReceived && m_bpressed)
    {
        // Clear the screen and display the error message returned
        ps.ResetReport(_T("SMReportReceipt"));
//      m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_FALSE);
//		trace(L6, _T("ONRPDisconnect 3 - REDRAW is set to false") );

        AddToReport(m_csErrorLine, _T(""));
        if (m_csRPT == EMPTY_STRING)
        {
            m_csRPT = m_csCurrentReport;
        }
        // Clear the report name to prevent print button from being enabled
        if (m_csCurrentReport != EMPTY_STRING)
        {
			STATE(StoreModeState)::SCOTDeleteFile(m_csCurrentReport);   //TAR 348946 - Delete Existing Report File
            m_csCurrentReport = EMPTY_STRING;
        }
    	ps.Button(_T("SMButton8"), BTT_GOBACK, true);
        if (co.fOperationsLogDataForSystemReports)
        {
            ps.Button(_T("SMButton1"), BTT_SELECTREPORT, !m_bpressed);
            // +TAR SSCOP-75 kr185069 2011/08/03
			if (!co.m_bOperationsPrintSignatureReceipt){	
				 ps.Button(_T("SMButton3"), BTT_PRINTSIGN, false);
				
			}else{
				 ps.Button(_T("SMButton3"), BTT_PRINTSIGN, !m_bpressed && !m_bSignaturesPrinted);
			}
			// -TAR SSCOP-75 kr185069 2011/08/03
            ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId,!m_bpressed && m_bShowLastReceipt);
            ps.Button(_T("SMButton4"), BTT_RECONNECT, false);
            //For REPORT EOD Button 
            if (co.nOperationsEOD == OPTION_OPERATOR_CONTROL)
                ps.Button(_T("SMButton7"), BTT_ENDREPORTPERIOD, !m_bpressed);
        }
		m_bProcessingReport = false; //RFC 262343
        RETURNSTATE(SmRunReports);
    }
    else
    {
        // GoOffline resets the buttons, except for go back
        GoOffline();
    }
	ps.Button(_T("SMButton8"), BTT_GOBACK, true);
    return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    GoOffline                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  NONE       
//              
//              
//              
//              
// Returns:     void                                        
///////////////////////////////////////////////////////////////////////////////////
void SMSmReportsMenuBase::GoOffline(void)
{
	// I know -- these should be member variables.
    CString csSlipFile = SLIP_FILE;
    CString csLReceiptFile = LRECEIPT_FILE;

    GET_PARTITION_PATH(SLIP_FILE, csSlipFile.GetBuffer(_MAX_PATH));
    GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));

    csSlipFile.ReleaseBuffer();
    csLReceiptFile.ReleaseBuffer();

    switch(m_lReportStatus)
    {
    case RP_NOTOPEN:
         break;
    case RP_OPENED:
         CloseFile();
		 STATE(StoreModeState)::SCOTDeleteFile(m_csCurrentReport);   //TAR 348946 - Delete Existing Report File
         m_csCurrentReport = EMPTY_STRING;
         break;
    case RP_CLOSED:
		 STATE(StoreModeState)::SCOTDeleteFile(m_csCurrentReport);   //TAR 348946 - Delete Existing Report File
         m_csCurrentReport = EMPTY_STRING;
         break;
    }
    m_bDisplayReport = false;
   if (co.fOperationsLogDataForSystemReports)
   {
      ps.Button(_T("SMButton1"), BTT_SELECTREPORT, false); //Disable buttons
      ps.Button(_T("SMButton3"), BTT_PRINTSIGN, false);
      ps.Button(_T("SMButton4"), BTT_RECONNECT, true);
     if (co.nOperationsEOD == OPTION_OPERATOR_CONTROL)
         ps.Button(_T("SMButton7"), BTT_ENDREPORTPERIOD, true);
     if(! co.fOperationsTBControlsPrinter)
     {
        if (FileExist(csLReceiptFile)||FileExist(csSlipFile)) 
        {
	        m_bShowLastReceipt = true;
            ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId, m_bShowLastReceipt);
        }
        else
            ps.Button(_T("SMButton2"), m_nPrintReceiptBttnTextId, m_bShowLastReceipt);
     }

     ps.ResetReport(_T("SMReportReceipt"));
     if (m_bErrorLineReceived)
     {
        AddToReport(m_csErrorLine, _T(""));
     }
     AddToReport(ps.GetPSText(MSG_REPORTINGOFFLINE), _T(""));
     m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_TRUE);


	 //RFC 262343 - start
	 if (m_bProcessingReport)
	 {
		ScotError(INFO, SM_CAT_INTERNAL_ERROR,FLR_REPORTSERVER_OFFLINE);  
		bReportServerOnline = false;	
		m_bProcessingReport = false;
	 }
	 //RFC 262343 - end
	 
    }
   else 
   {
      ps.SetButtonText(_T("SMButton1"), PS_NOTEXT);
      ps.SetButtonText(_T("SMButton3"), PS_NOTEXT);
      ps.SetButtonText(_T("SMButton4"), PS_NOTEXT);
      ps.SetButtonText(_T("SMButton7"), PS_NOTEXT);

   }
   m_bErrorLineReceived = false;
    // TAR 320319
	if (m_bSignature)
    {
		m_bSignature = false;
      	ps.Button(_T("SMButton8"), BTT_GOBACK, true);
	}
    // End TAR 320319

}
///////////////////////////////////////////////////////////////////////////////
// Function:    SendRPDisconnect                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  NONE       
//              
//              
//              
//              
// Returns:                                              
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::SendRPDisconnect()
{
	ps.Button(_T("SMButton8"), BTT_GOBACK, true);
    try
    {
       m_pTelnetIF->TransmitString(_bstr_t(EXITCMD));
    }
    catch (_com_error e)
    {
      #ifndef NO_LOGGING
      _bstr_t errString;
      LogComError(e,  (_bstr_t)_T("-SMSmReportsMenuBase::SendRPDisconnect"));
      GoOffline();
      #endif
    }
    return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    SendRPConnect                                   
// Class:       SMSmReportsMenuBase                                              
// Description: 
// Parameters:  NONE       
//              
//              
//              
//              
// Returns:                                              
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmReportsMenuBase::SendRPConnect()
{
    //Begin Retrieve Reporting 3.1 information from registry
    LONG     lRC;
    HRESULT  hr;
    HKEY     hKey;
    TCHAR  szServer [_MAX_PATH+1];
    long  lTelnetPort= (long) DEFAULT_TELNET_PORT;
    CString csRegKey = REPORTPORT;
    DWORD dwServerSize=sizeof(long);
    lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, REPORTINGKEY,
                        0, KEY_QUERY_VALUE, &hKey);
    if (ERROR_SUCCESS == lRC)
    {

        lRC = ::RegQueryValueEx(hKey, csRegKey, NULL, NULL,
                            (BYTE *)&lTelnetPort, &dwServerSize);
        if (lRC != ERROR_SUCCESS)
        {
            lTelnetPort = (long) DEFAULT_TELNET_PORT;
        }
        dwServerSize=_MAX_PATH;
        csRegKey = REPORTSERVERNAME;
        lRC = ::RegQueryValueEx(hKey, csRegKey, NULL, NULL,
                            (LPBYTE)szServer, &dwServerSize);
        ::RegCloseKey(hKey);
        if (lRC != ERROR_SUCCESS)
        {
            ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
            lRC, _T("opening"), REPORTINGKEY);
        }
    }
    else
    {
	   trace(L6, _T("Error opening report registry key"));
    }
    _bstr_t btHostname (szServer);
    try
    {
        hr = m_pTelnetIF->Connect(btHostname, lTelnetPort);
    }
    catch (_com_error e)
    {
	   trace(L6, _T("Exception on telnet connect call, failed to connect"));
        #ifndef NO_LOGGING
        _bstr_t errString;
        LogComError(e,  (_bstr_t)_T("-SMSmReportsMenuBase::SendRPConnect"));
        GoOffline();
        #endif
    }

    return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    WriteRecord                                   
// Class:       SMSmReportsMenuBase                                              
// Description: Write Current Line of Report to Current Reporting File
// Parameters:  CString csRecord     
//              
//              
//              
//              
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void SMSmReportsMenuBase::WriteRecord(CString csRecord)
{
     try
     {
	   CString csDataToWrite = csRecord;
	   csDataToWrite += _T("\n");
	   if (csDataToWrite.GetLength() > MAX_TRACE + 3)
	   {
	       trace(L7, _T("WriteRecord csDataToWrite %d [%s...%s]"), 
	             csDataToWrite.GetLength(), 
	             csDataToWrite.Left(MAX_TRACE/2), 
	             csDataToWrite.Right(MAX_TRACE/2));
	   }
	   else
	   {
	       trace(L7, _T("WriteRecord csDataToWrite %d [%s]"), 
	             csDataToWrite.GetLength(), csDataToWrite);
	   }
//	   m_csfCurrentReport.WriteString(csDataToWrite);
	   m_csfCurrentReport.Write((void *)(LPCTSTR)csDataToWrite, (sizeof(TCHAR) * csDataToWrite.GetLength()));
     }
     catch (CFileException *fe)
     {
    
	   fe->Delete();
	   trace(L6, _T("Error writing record to report log file"));
	   ScotError(INFO, SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_REPORT_FILEWRITE,
		     _T("Error writing record to this file %s."),m_csCurrentReport);
     }
 }
///////////////////////////////////////////////////////////////////////////////
// Function:    OpenFile                                   
// Class:       SMSmReportsMenuBase                                              
// Description: Create Selected Reporting Report File
// Parameters:          
//              
//              
//              
//              
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void SMSmReportsMenuBase::OpenFile(void)
{
    CFileException fe;
    CTime ctCurrentTime;
    ctCurrentTime = CTime::GetCurrentTime();
    CString csTime, csRpt;
    csTime.Format(_T("_%.2d%.2d%.2d%.2d%.2d%.2d.log"),
                    ctCurrentTime.GetMonth(),
                    ctCurrentTime.GetDay(),
                    ctCurrentTime.GetYear(),
                    ctCurrentTime.GetHour(),
                    ctCurrentTime.GetMinute(),
                    ctCurrentTime.GetSecond());
  if (m_csRPT == EMPTY_STRING)
  {
     m_csRPT = m_csCurrentReport;
  }
    csRpt   = m_csCurrentReport + csTime ;
   try
   {
        if (m_csfCurrentReport.m_hFile == CFile::hFileNull)
        {
	        m_csCurrentReport = CSFILEPATH;
			GET_PARTITION_PATH(m_csCurrentReport, m_csCurrentReport.GetBuffer(_MAX_PATH));
			m_csCurrentReport.ReleaseBuffer();

            m_csCurrentReport += STANDARD_REPORT;
            m_lReportStatus = RP_OPENED;
            int nOpenMode = CFile::modeCreate | CFile::shareDenyNone | CFile::modeReadWrite;
	        m_csfCurrentReport.Open(m_csCurrentReport, nOpenMode, &fe);
            trace(L6, _T("Opened report log file"), 0);
	        if (fe.m_cause  != CFileException::none)
	        {
                m_lReportStatus = RP_NOTOPEN;
                trace(L6, _T("Error open report log file"), 0);
	            ScotError(INFO, SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_REPORT_FILEOPEN,
		        _T("Error open report log file %s."),m_csCurrentReport);	  
            }
        }
   }
   catch (CFileException *e)
   {
        e->Delete();
        trace(L6, _T("Error initialize report log file"), 0);
	    ScotError(INFO, SM_CAT_INTERNAL_ERROR, SCOTAPP_SM_REPORT_FILEOPEN,
		_T("Error initializing report log file %s."),m_csCurrentReport);
   }

}
///////////////////////////////////////////////////////////////////////////////
// Function:    CloseFile                                   
// Class:       SMSmReportsMenuBase                                              
// Description: Close Current Reporting File
// Parameters:          
//              
//              
//              
//              
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void SMSmReportsMenuBase::CloseFile(void)
{
  m_csfCurrentReport.Close();
}


//===================================================================
///////////////////////////////////////////////////////////////////////////////
// Function:    LogError                                   
// Class:       CCollectorBase                                              
// Description: Log Errors to Event Log  
// Parameters:  eventID:        
//              functionName
//              errorCode
//              errorCode2
//              Description
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void SMSmReportsMenuBase::LogError(DWORD eventID, 
                                     _bstr_t &functionName, 
                                     WORD eventType, 
                                     long errorCode,
                                     long errorCode2,
                                     BSTR Description)
{
    WORD     stringCount;
    LPCTSTR  errorStringPtr[5];
    _TCHAR   errorCodeString[20];
    _TCHAR   errorCodeString1[20];
    _bstr_t  bDescription;

    #ifndef NO_LOGGING
        if (NULL != m_EventSource)
        {
            if (-1 == errorCode)
            {
                stringCount = 1;
            }
            else if (-1 == errorCode2)
            {
                stringCount = 2;
            }
            else
            {
                stringCount = 4;
            }
            errorStringPtr[0] = functionName;
            _stprintf(errorCodeString, _T("%d"), errorCode);
            errorStringPtr[1] = errorCodeString;

            _stprintf(errorCodeString1, _T("%d"), errorCode2);
            errorStringPtr[2] = errorCodeString1;
            if (-1 != errorCode2)
            {
                if (NULL == Description)
                {
                    bDescription = EMPTY_STRING;
                }
                else
                {
                    bDescription = Description;
                }
                errorStringPtr[3] = (_TCHAR *)bDescription;
            }


            ReportEvent(m_EventSource,          // handle to event log
                        EVENTLOG_ERROR_TYPE,    // event type 
                        SM_CAT_INTERNAL_ERROR,  // event category
                        eventID,                // event identifier
                        NULL,                   // user security identifier
                        stringCount,            // Number of strings to merge
                        0,                      // size of binary data
                        errorStringPtr,         // array of strings to merge
                        NULL);                  // binary data buffer
        }
    #endif
}
///////////////////////////////////////////////////////////////////////////////
// Function:    LogComError                                      
// Class:       CCollectorBase                                              
// Description: Log Com Errors to Event Log
// Parameters:  functionName
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void SMSmReportsMenuBase::LogComError(_com_error &e, 
                                 _bstr_t &functionName)
{

    LPCTSTR  errorStringPtr[5];
    _TCHAR   errorCodeString[40];
    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());


    #ifndef NO_DATACAP
    trace(L7,
        (_bstr_t)_T("%s> E_FAIL COM Exception Caught"), functionName );
    trace(L7, 
        (_bstr_t)_T("Source %s, Desc: %s, Error %08lX"), 
        (bstrSource.length() > 0) ? (LPCTSTR)bstrSource : _T("null"),
        (bstrDescription.length() > 0) ? (LPCTSTR)bstrDescription : _T("null"),
        e.Error());
    trace( L7, 
        (_bstr_t)_T("Message %s"), e.ErrorMessage());

    #endif

    #ifndef NO_LOGGING
        if (NULL != m_EventSource)
        {
            errorStringPtr[0] = functionName;
            errorStringPtr[1] = (bstrSource.length() > 0) ? (LPCTSTR)bstrSource : _T("null");
            errorStringPtr[2] = (bstrDescription.length() > 0) ? (LPCTSTR)bstrDescription : _T("null");
            _stprintf(errorCodeString, _T("%08lX"), e.Error());
            errorStringPtr[3] = errorCodeString;
            errorStringPtr[4] = e.ErrorMessage();

            ReportEvent(m_EventSource,          // handle to event log
                        EVENTLOG_ERROR_TYPE,    // event type 
                        SM_CAT_INTERNAL_ERROR,  // event category
                        FLR_COM_ERROR,          // event identifier
                        NULL,                   // user security identifier
                        5,                      // Number of strings to merge
                        0,                      // size of binary data
                        errorStringPtr,         // array of strings to merge
                        NULL);                  // binary data buffer
        }
    #endif

    return;            
}
///////////////////////////////////////////////////////////////////////////////
// Function:    LogOLEError                                       
// Class:       CCollectorBase                                              
// Description: Log OLE Errors to the Event Log
// Parameters:  functionName
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void SMSmReportsMenuBase::LogOLEError(COleDispatchException *e, 
                                    _bstr_t &functionName)
{

    LPCTSTR  errorStringPtr[5];
    _TCHAR   errorCodeString[40];


    #ifndef NO_DATACAP
    trace( L7,
        _T("%s> E_FAIL OLEDispatch Exception Caught"), functionName );
    trace( L7, 
        _T("Source %s, Desc: %s, Error %08lX"), 
        e->m_strSource, e->m_strDescription, e->m_wCode);

    #endif

    #ifndef NO_LOGGING
        if (NULL != m_EventSource)
        {
            errorStringPtr[0] = functionName;
            errorStringPtr[1] = (LPCTSTR)e->m_strSource;
            errorStringPtr[2] = (LPCTSTR)e->m_strDescription;
            _stprintf(errorCodeString, _T("%08lX"), e->m_wCode);
            errorStringPtr[3] = errorCodeString;

            ReportEvent(m_EventSource,          // handle to event log
                        EVENTLOG_ERROR_TYPE,    // event type 
                        SM_CAT_INTERNAL_ERROR,  // event category
                        FLR_OLE_ERROR,          // event identifier
                        NULL,                   // user security identifier
                        4,                      // Number of strings to merge
                        0,                      // size of binary data
                        errorStringPtr,         // array of strings to merge
                        NULL);                  // binary data buffer
        }
    #endif

    return;            
}
//===================================================================
//End of ReportMenu 
////////////////////////////////////////

SMStateBase  *SMSmReportsMenuBase::PSReceiptUp(void)
{
    return STATE_NULL;
}

#endif //_CPPUNIT 

SMStateBase  *SMSmReportsMenuBase::PSReceiptDown(void)
{
  // NextGen //
  if (0 != nPSLastParm)
  {
	  CString csItemId;
	  csItemId.Format(_T("%d"), nPSLastParm);
	  ps.GetPSXObject()->SetSelectedReceiptItem(_T("SMReportReceipt"), csItemId);
  }

    return STATE_NULL;
}

#ifndef _CPPUNIT

//************************************************************************ 
//
// Function:		AddToReport
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long SMSmReportsMenuBase::AddToReport(const CString& text, const CString &csOption)
{
	//*** NewUI ***//
	//the configuration in XML for report is not ready yet, the implementation may change
	COleVariant vValue;
	CString csItemEntryId;
    csItemEntryId.Format(_T("%d"), ++m_lReportLineIndex);

	m_pAppPSX->CreateReceiptItem(_T("SMReportReceipt"), csItemEntryId);
	vValue = text;
	m_pAppPSX->SetReceiptItemVariable(_T("SMReportReceipt"), csItemEntryId, _T("ITEM_SMTEXT"), vValue);
    vValue = csOption;
	m_pAppPSX->SetReceiptItemVariable(_T("SMReportReceipt"), csItemEntryId, _T("ITEM_SMINDEX"), vValue);
	m_pAppPSX->UpdateReceiptControls(_T("SMReportReceipt"));

	return 0;
	//*** end of NewUI ***//
}

// End NewUI

CString SMSmReportsMenuBase::HandleColumnarLine(const CString &csData)
{
	trace(L7, _T("SMSmReportsMenuBase::HandleColumnarLine. Orig Data: %s"), csData);
	CString csConvertedString;
	CString csleftString;
    bool bIsStarting      = true;   //FLAG that we are still processing left aligned columns
	bool bIsLastCharSpace = false;
	bool bHasNonSpace     = false;
	int iNonEngCount      = 0;
	csleftString = csData;
	while (csleftString.GetLength() > 0)
	{
		TCHAR tch = csleftString.GetAt(0);
		if (tch != 0x20)
		{
			CString csChar(tch);
			csConvertedString = csConvertedString + csChar;
			csleftString = csleftString.Mid(1);
			bHasNonSpace = true;
			// if Starting, take note of nonEnglishChar
			// if not and char is NotEnglish, 
			//     remove first non occurence of space from right
			if (bIsStarting == true)
			{
				if (IsDoubleSpacedChar(tch))
				{
					iNonEngCount++;
					trace(L9, _T("SMSmReportsMenuBase::ParseString. Taking Note of Non-Eng Char. Total Non Eng: %d"), iNonEngCount);
				}
			}
			else if (IsDoubleSpacedChar(tch))
			{
				int i = csConvertedString.GetLength();
				while (i > 1)
				{
					if (csConvertedString.GetAt(i) == 0x20 && csConvertedString.GetAt(i-1) == 0x20)
					{
						trace(L9, _T("SMSmReportsMenuBase::ParseString. Current converted string: %s"), csConvertedString);
						CString csNewString = csConvertedString.Left(i-1);
						csNewString = csNewString + csConvertedString.Mid(i);											
						csConvertedString = csNewString;											
						trace(L9, _T("SMSmReportsMenuBase::ParseString. Current new converted string: %s"), csConvertedString);
						break;
					}
					i--;
				}
			}
			bIsLastCharSpace = false;
		}
		else if (tch == 0x20)
		{
			if (iNonEngCount > 0 && bIsLastCharSpace == true) //Ensure at least 1 space
			{									
				iNonEngCount--;
				trace(L9, _T("SMSmReportsMenuBase::ParseString. Ignoring space. Total Non Eng space not accounted for: %d"), iNonEngCount);
			}
			else
			{
				CString csChar(tch);
				csConvertedString = csConvertedString + csChar;
			}
			if (bIsLastCharSpace == true || (bHasNonSpace == true && csConvertedString.GetLength() >= 15))
			{
			    //If we encounter 2 successive spaces OR
			    //If we are at 15th position and no non-space character encountered yet, 
				//there is probably no more left aligned columns
				bIsStarting = false;
			}
			csleftString = csleftString.Mid(1);
			bIsLastCharSpace = true;
		}							
	}
	trace(L7, _T("SMSmReportsMenuBase::HandleColumnarLine. Conv Data: %s"), csConvertedString);
	return csConvertedString;
}

CString SMSmReportsMenuBase::HandleCenterLine(const CString &csData)
{
	trace(L7, _T("SMSmReportsMenuBase::HandleCenterLine. Orig Data: %s"), csData);
	CString csConvertedString;
    CString csleftString;
	csleftString = csData;
	//This must be center text
	//Count how many non-English Chars
	//(Divide by 2) then deduct leading spaces. no. of spaces to deduct = quotient
	int iNonEngCount = 0;
	for (int i=0; i<csleftString.GetLength(); i++)
	{
		TCHAR tch = csleftString.GetAt(i);
		if (IsDoubleSpacedChar(tch))
		{
			iNonEngCount++;
		}
	}
	while (csleftString.GetLength() > 0)
	{
		if ((iNonEngCount/2) > 0 && csleftString.GetAt(0) == 0x20)
		{								
			iNonEngCount = iNonEngCount - 2;
		}
		else 
		{
			CString csChar(csleftString.GetAt(0));
			csConvertedString = csConvertedString + csChar;
		}
		csleftString = csleftString.Mid(1);
	}
	trace(L7, _T("SMSmReportsMenuBase::HandleCenterLine. Conv Data: %s"), csConvertedString);
	return csConvertedString;
}

CString SMSmReportsMenuBase::HandleOther(const CString &csData)
{
	trace(L7, _T("SMSmReportsMenuBase::HandleOther. Orig Data: %s"), csData);
	CString csConvertedString;
	CString csleftString;
	csleftString = csData;
	bool bIsLastCharSpace     = false;
	int iNonEngCount          = 0;
	while (csleftString.GetLength() > 0)
	{
		TCHAR tch = csleftString.GetAt(0);
		if (tch == 0x20)
		{
			if (bIsLastCharSpace == true) //Ensure at least 1 space
			{
				if (iNonEngCount > 0)
				{
				    //Ignore this current space
					iNonEngCount--;
				}
				else
				{
                    CString csChar(tch);
					csConvertedString = csConvertedString + csChar;
				}
			}
			else
			{
				CString csChar(tch);
				csConvertedString = csConvertedString + csChar;
			}
			bIsLastCharSpace = true;
		}
		else if (IsDoubleSpacedChar(tch))
		{
		    iNonEngCount++;
		    CString csChar(tch);
			csConvertedString = csConvertedString + csChar;
	        bIsLastCharSpace     = false;
		}
		else
		{
			CString csChar(tch);
			csConvertedString = csConvertedString + csChar;
	        bIsLastCharSpace     = false;
		}
		csleftString = csleftString.Mid(1);
	}
	trace(L7, _T("SMSmReportsMenuBase::HandleOther. Conv Data: %s"), csConvertedString);
	return csConvertedString;
}

#endif //_CPPUNIT 
