//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmMobileAudit.CPP
//
// TITLE: Class implementation for store mode MobileAudit state
//
//
// AUTHOR:    Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmMobileAudit.h"
#include "SMSmAssistMode.h"
#include "PSConfigData.h"
#include "TCPConnectConstants.h"
#include "TCPConnectConstantsSSF.h"
#include "FLTCPDefines.h"
#include "MobileCartAudit.h"
#include "MobileCartAuditDefs.h"
#include "LaneStateData.h"
#include "MobileAuditPSDefs.h"
#include "MobileConstantsSSF.h"


#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmMobileAudit")


extern CMobileCartAudit mobileAudit;


IMPLEMENT_DYNCREATE(SMSmMobileAudit, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMSmMobileAudit::SMSmMobileAudit()
{
    m_bAbortingAudit = false;
    m_bWaitForResponse=false;
    nTimeOut = 0;
    pUserInputLock=NULL;
    m_nResponseTimer = cr.GetIntOption(_T("TimeOut"), _T("MobileAuditResponseTimeout"), 15, _T("ScotOpts"));
    m_bRAPScannerAllowed = cr.GetFlagOption(_T("Operations"),_T("RapScannerAllowedForAudit"), false, _T("Scotopts"));
    m_bRAPScannerAvailable = false;
    m_bRAPScannerConnected = false;
//	m_bWaitingForStatusToConnect = false;    //MEIJER RAP Scanner
    IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////////
SMStateBase* SMSmMobileAudit::Initialize()
{
/*  //MEIJER RAP Scanner
    CString csCompName = SMState::GetLaneName();
    csCompName += _T(";");

    CString csOperation=_T("operation=scanner-status-");
    csOperation = csOperation + csCompName;
    CString csRAPName = SMState::GetRAPConnectionName();

    m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, csRAPName );

        */
    int i;
    trace(L6, _T("SMSmMobileAudit SMDataEntryViewString = %s"), sm_pDataNeededEntry.ToString());
    for (i=0; i<8; i++)
    {
        trace(L6, _T("SMSmMobileAudit    Button %d = %s"), i, sm_pDataNeededEntry.GetButtonText(i));
    }

    SMStateBase* retState = SMSmDataEntry::Initialize();

        trace(L6, _T("SMSmMobileAudit SMDataEntryViewString = %s"), sm_pDataNeededEntry.ToString());
    for (i=0; i<8; i++)
    {
        trace(L6, _T("SMSmMobileAudit    Button %d = %s"), i, sm_pDataNeededEntry.GetButtonText(i));
    }
    ra.OnNoRAPDataNeeded();
    if(co.fStateDisplayNumberOfItemsOnRAP)
    {
        trace(L7, _T("+ra.OnAddOtherEvent COMMAND=EnableNumItems;"));
        ra.OnAddOtherEvent(_T("COMMAND=EnableNumItems;"));
        trace(L7, _T("-ra.OnAddOtherEvent"));
        CString csNumItems = EMPTY_STRING;
        csNumItems.Format(_T("%d"), GetDisplayedItemCount());	// RFC 371075
        trace(L7, _T("+ra.OnNormalItem, Sending the number of items to RAP"));
        ra.OnNormalItem(_T("[NumItems]") + csNumItems);
        trace(L7, _T("-ra.OnNormalItem, NumItems = %s"), csNumItems);
        m_pAppPSX->SetConfigProperty(_T("TotalAmount"), _T(""),UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("TotalText"), _T(""),UI::PROPERTYVISIBLE, VARIANT_TRUE);
    }
    else
    {
        trace(L7, _T("+ra.OnAddOtherEvent COMMAND=DisableNumItems;"));
        ra.OnAddOtherEvent(_T("COMMAND=DisableNumItems;"));
        trace(L7, _T("-ra.OnAddOtherEvent"));
        m_pAppPSX->SetConfigProperty(_T("TotalAmount"), _T(""),UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("TotalText"), _T(""),UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    
    return retState;
}

//////////////////////////////////////////////////
void SMSmMobileAudit::UnInitialize()
{
/*  //MEIJER RAP Scanner
	CString csRAPName;
	
	if(SMSmAssistMode::m_csRAPConnectionName == "")
	{
		SMSmAssistMode::m_csRAPConnectionName = SMState::GetRAPConnectionName();
	}
	
	CString csOperation=_T("operation=release-scanner;");
    m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, SMSmAssistMode::m_csRAPConnectionName );

*/
    SMSmDataEntry::UnInitialize();
    UnlockButtons();        
    ps.Total((long) 0);
    
    if (ps.RemoteMode())
    {
        ra.OnNoRAPDataNeeded();
        ps.DisconnectFromServer(true);
    }
    
    if(co.fStateDisplayNumberOfItemsOnRAP && !fSAInTransaction )
    {
        trace(L7, _T("+ra.OnAddOtherEvent COMMAND=DisableNumItems;"));
        ra.OnAddOtherEvent(_T("COMMAND=DisableNumItems;"));
        trace(L7, _T("-ra.OnAddOtherEvent"));
        m_pAppPSX->SetConfigProperty(_T("TotalAmount"), _T(""),UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("TotalText"), _T(""),UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    
}

//////////////////////////////////////////////////
SMStateBase  *SMSmMobileAudit::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
    trace(L6, _T("+SMSmMobileAudit::PSCmdButton"));
    trace(L6, _T("-SMSmMobileAudit::PSCmdButton"));
    return STATE_NULL;
}


//////////////////////////////////////////////////
SMStateBase *SMSmMobileAudit::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    trace(L6, _T("+-SMSmMobileAudit::OnRAPUnsolicitedDataReceived"));   

/*  +MEIJER RAP Scanner
	CString csData = sData;
	
    if (!dm.IsScannerEnabled())
	{
		trace (L6, _T("-SMSmMobileAudit::OnRAPUnsolicitedDataReceived() -- SSCO scanner disabled. Scanning not allowed"));
        return STATE_NULL;
	}
	else if	((m_bRAPScannerConnected) && (csData.Find(_T("cScanData")) != -1))
    {
		
		int iFind;
		
		trace(L6, _T("+SMSmMobileAudit::OnRAPUnsolicitedDataReceived() Processing scan data..."));
		// 4 Fields: cRScan:Data:Label:Type
		// 4 Fields: cScanData:LastData|0xe1|Label|0xe1|Type
		CString csData1 = csData;
		CString csData2 = _T("");
		CString csData3 = _T("");
		CString csData4 = _T("");
		
		
		// Get First Field (cScanData)
		iFind=csData1.Find((_TCHAR)_T(':'));
		ASSERT (iFind != -1);
		
		csData2 = csData1.Mid(iFind+1);
		ASSERT (csData2.GetLength()>0);
		
		// + Tar 360527
		nDMLastLength = csData2.GetLength();
		//Save scan data
		if (cDMLastData) delete cDMLastData;
		cDMLastData = new TCHAR [nDMLastLength+1];
		// End Merge
		
		// copy the new data to our buffer
		// the MsgObject graciously allocates 1 byte more and
		// tacks on a 0x00 so we can copy len + 1
		memcpy(cDMLastData, csData2, nDMLastLength * sizeof(_TCHAR));
		cDMLastData[nDMLastLength] = NULL;    // set last character as a NULL
		// - Tar 360527
		
		// Get Second Field (Data)
		iFind=csData2.Find((_TCHAR)DM_FS);
		ASSERT (iFind != -1);
		csDMLastBarCode = csData2.Left(iFind);
		
		// Get Third Field (Label)
		csData3 = csData2.Mid(iFind+1);
		ASSERT (csData3.GetLength()>0);
		iFind=csData3.Find((_TCHAR)DM_FS);
		ASSERT (iFind != -1);
		csDMLastBarLabel = csData3.Left(iFind);
		
		// Get Third Field
		csData4 = csData3.Mid(iFind+1);
		ASSERT (csData4.GetLength()>0);
		csDMLastBarType = _ttol(csData4);
		
		// EndCodeLocaleUPC
		//Need to know if Code 39 early
		io.cScanType = csDMLastBarType;
		trace(L6, _T("-SMSmMobileAudit::OnRAPUnsolicitedDataReceived() - DMScanner() called with scan data."));
	
		return DMScanner();         // barcode
    }
*/  //-MEIJER RAP Scanner

	trace(L6, _T("-SMSmMobileAudit::OnRAPUnsolicitedDataReceived"));
    return SMStateBase::OnRAPUnsolicitedDataReceived(sData);
}

SMStateBase * SMSmMobileAudit::PSButton1(void)
{
    trace (L2, _T("SMSmMobileAudit::PSButton1"));

	m_sButton = GetPagedButtonCommand(0);

    return HandleDataNeeded();
}

SMStateBase * SMSmMobileAudit::PSButton2(void)
{
    trace (L2, _T("SMSmMobileAudit::PSButton2"));

	m_sButton = GetPagedButtonCommand(1);

    return HandleDataNeeded();
}

SMStateBase * SMSmMobileAudit::PSButton3(void)
{
    trace (L2, _T("SMSmMobileAudit::PSButton3"));

	m_sButton = GetPagedButtonCommand(2);

    return HandleDataNeeded();
}

SMStateBase * SMSmMobileAudit::PSButton4(void)
{
    trace (L2, _T("SMSmMobileAudit::PSButton4"));
    
    m_sButton = GetPagedButtonCommand(3);

    return HandleDataNeeded();
}

SMStateBase * SMSmMobileAudit::PSButton5(void)
{
    trace (L2, _T("SMSmMobileAudit::PSButton5"));
    
    m_sButton = GetPagedButtonCommand(4);

    return HandleDataNeeded();
}

SMStateBase * SMSmMobileAudit::PSButton6(void)
{
    trace (L2, _T("SMSmMobileAudit::PSButton6"));
    
    m_sButton = GetPagedButtonCommand(5);

    return HandleDataNeeded();
}

SMStateBase * SMSmMobileAudit::PSButton7(void)
{
    trace (L2, _T("SMSmMobileAudit::PSButton7"));
    
    m_sButton = GetPagedButtonCommand(6);

    return HandleDataNeeded();
}

SMStateBase * SMSmMobileAudit::PSButton8(void)
{
    trace (L2, _T("SMSmMobileAudit::PSButton8"));

    m_sButton = GetPagedButtonCommand(7);
 
    return HandleDataNeeded();
}


///////////////////////////////////////////////////////////////////////////////
// Function:    CUParse                                  
// Class:       SMStateBase::CUParse                                             
// Description: 
// Parameters:     
//              
//              
//              
//              
// Returns:     retState                                         
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmMobileAudit::ParseMobile(MessageElement *me)
{
    trace(L6, _T("+SMSmMobileAudit::ParseMobile"));
    int nEvent = me->cuInfo.Msg;
    CString csStateName;
    SMStateBase *retState = STATE_NULL;
    static int AuditReceiptCount = 0;
    
    DMScannerDisable();
    
    trace(L6,_T("+ParseMobile"));
    
    switch(nEvent)
    {
        
    case GP_TCP_RECEIVED:
        {
            
            BYTE* pMsg = (BYTE*) me->psInfo.lParm;
            
            BYTE* pMsgData = &pMsg[sizeof(structMsgHeader)];
            
            structMsgHeader* pHeader = (structMsgHeader*) pMsg;
            
            _bstr_t  bstrData = szEmpty;
            
            trace(L6, _T("GP_TCP_MESSAGE: sAppID = %d, lAppData = %d"), pHeader->sAppID, pHeader->lAppData);
            
            if (pHeader->sAppID == TCP_APPID_MOBILECART)
            {
                int nDataLength=pHeader->length - sizeof(structMsgHeader);
                
                if (IsTextUnicode(pMsgData, nDataLength, NULL))
                {
                    bstrData = (LPCWSTR) pMsgData;
                }
                else
                {
                    bstrData = (LPCSTR) pMsgData;
                }
                
                //TODO:  Change this to L7!!!!
                trace(L6, _T("MOBILECART message - %s"), (LPCTSTR) bstrData);
                
                if (pHeader->lAppData==TCP_MOBILECARTMSG_START_AUDIT_RESPONSE)
                {
                    mobileAudit.ProcessStartAuditResponse(bstrData);
                    
                    RetrieveDataNeededDefinition(szEmpty,
                        (LPCTSTR) mobileAudit.DataNeededViewString(), 
                        ms_MobileAuditDataEntryINI, 
                        (LPCTSTR)mobileAudit.DataNeededSection());
                    
                    retState = Initialize();
                    
                    UpdateTicketInfo();
                }
                else if (pHeader->lAppData==TCP_MOBILECARTMSG_ADD_AUDIT_ITEM_RESPONSE)
                {
                    mobileAudit.ProcessAddAuditItemResponse(bstrData);
                    
                    CString csResponseStatus = (LPCTSTR) mobileAudit.RescanStatus();    
                    
                    AddAuditItemToReceipt();
                    
                    RetrieveDataNeededDefinition(szEmpty, 
                        (LPCTSTR) mobileAudit.DataNeededViewString(), 
                        ms_MobileAuditDataEntryINI, 
                        (LPCTSTR)mobileAudit.DataNeededSection());
                    
                    retState = Initialize();
                    
                    UpdateRescanInfo();
                }
                else if (pHeader->lAppData==TCP_MOBILECARTMSG_AUDIT_END_RESPONSE)
                {
                    mobileAudit.ProcessEndAuditResponse(bstrData);
                    
                    CString csResponseStatus = (LPCTSTR) mobileAudit.RescanStatus(); 
                    
                    m_pAppPSX->ClearReceipt(_T("CMSMReceipt"));
                    
                    ps.Total((long) 0);
                    
                    ra.OnNormalItem(_T("[NumItems]0"));
                   // if (!m_bAbortingAudit)
                    {
                        AddAuditItemToReceipt();
                        
                        RetrieveDataNeededDefinition(szEmpty, 
                            (LPCTSTR) mobileAudit.DataNeededViewString(), 
                            ms_MobileAuditDataEntryINI, 
                            (LPCTSTR)mobileAudit.DataNeededSection());
                        
                        retState = Initialize();
                    }
                }
                else if (pHeader->lAppData==TCP_MOBILECARTMSG_SWITCH_TO_FULL_AUDIT_RESPONSE)
                {
                    mobileAudit.ProcessSwitchToFullAuditResponse(bstrData);
                    
                    m_pAppPSX->ClearReceipt(_T("CMSMReceipt"));
                    
                    ps.Total((long) 0);
                    
                    ra.OnNormalItem(_T("[NumItems]0"));

                    UpdateTicketInfo();
                    if (!m_bAbortingAudit)
                    {
                        RetrieveDataNeededDefinition(szEmpty, 
                            (LPCTSTR) mobileAudit.DataNeededViewString(), 
                            ms_MobileAuditDataEntryINI, 
                            (LPCTSTR)mobileAudit.DataNeededSection());
                        retState = Initialize();
                    }
                    else
                    {
                        mobileCart.SendReadyForTransaction();
                        mobileAudit.SendEndAuditRequest(szActionSend);
                    }
                }
                else if (pHeader->lAppData==TCP_MOBILECARTMSG_CONTINUE_AFTER_ERROR_RESPONSE)
                {
                    mobileAudit.ProcessContinueAfterErrorResponse(bstrData);
                    
                    RetrieveDataNeededDefinition(szEmpty, 
                        (LPCTSTR) mobileAudit.DataNeededViewString(), 
                        ms_MobileAuditDataEntryINI, 
                        (LPCTSTR)mobileAudit.DataNeededSection());
                    
                    retState = Initialize();
                }
                else if (pHeader->sAppID == TCP_APPID_MOBILECART && pHeader->lAppData==TCP_MOBILECARTMSG_CARTPOSTED)
                {                
                    m_bWaitForResponse=false;
                    rp.m_bMobileShopper = true;
                    
                    _bstr_t bstrResult=mobileCart.ProcessCart(bstrData);
                    
                    retState = OnMobileTransactionStart((LPCTSTR)bstrResult);
                }
            }
            
            delete [] pMsg;
            
            me->psInfo.lParm = NULL; 
            
            break;
        }
        
    default:
        retState = SMState::CUParse(me);
    }
    
    UnlockButtons();
    
    trace(L6, _T("-SMSmMobileAudit::ParseMobile"));
    return retState;
}

/*  //+MEIJER RAP Scanner
#define SSF_TXT_BUTTON_HHSCAN_AVAIL 9426
#define SSF_TXT_BUTTON_HHSCAN_NOTAVAIL 9427
#define SSF_TXT_BUTTON_HHSCAN_RELEASE 9428
*/  //-MEIJER RAP Scanner

void SMSmMobileAudit::RetrieveDataNeededDefinition(CString csDescription, CString csView, CString csINI, CString csSection)
{
    trace (L6, _T("+SMSmMobileAudit::RetrieveDataNeededDefinition()"));
    trace (L6, _T("%s"), (LPCTSTR)mobileAudit.TraceObjectState());
    
    CString	        csViewString, csStateName;
    CString         sViewCustomer;
    CSCOTDataEntry  cCustomerDataEntry;
    CPSConfigData   cd;
    
    //Jira SSCOP-831 Start
    SCOT_LANGUAGE_TYPE currentLanguage;
    currentLanguage = ps.GetLanguage();
    ps.SetLanguage(m_customerLanguage);
    //Jira SSCOP-831 End
    
    /* //+MEIJER RAP Scanner
    CString csRAPScannerButton = _T("");
    if (m_bRAPScannerAllowed)
    {
        if (!m_bRAPScannerConnected && m_bRAPScannerAvailable)
        {
            csRAPScannerButton.Format(_T("BUTTON1=cGetHHScanner,%%%d;"), SSF_TXT_BUTTON_HHSCAN_AVAIL);
        }
        else if (!m_bRAPScannerConnected && !m_bRAPScannerAvailable)
        {
            csRAPScannerButton.Format(_T("BUTTON1=,%%%d;"), SSF_TXT_BUTTON_HHSCAN_NOTAVAIL);
        }
        else if (m_bRAPScannerConnected)
        {
            csRAPScannerButton.Format( _T("BUTTON1=cReleaseHHScanner,%%%d;"), SSF_TXT_BUTTON_HHSCAN_RELEASE);
        }
        cd.FormatString(csRAPScannerButton, true);
    }
    */ //-MEIJER RAP Scanner

    m_sDesc = csDescription;
    
    if(!csView.IsEmpty())   //String of DataNeeded Parameters
    {
        CString csTest = csView;
        csTest.TrimRight();
        if (csTest[csTest.GetLength() - 1] != _T(';'))
           csViewString = csView + _T(";");       //Make sure ends in semicolon to allow append RAP Scanner button info
        else
           csViewString = csView;
        cd.FormatString(csViewString, true);   //Primary Language Translation
    }
    
    if(!csINI.IsEmpty())
    {
        if(!csSection.IsEmpty())
        {
            //Dataneeded parameters to be retrieved from INI file
            if (cDataEntry.FromConfigData(csINI,csSection))
            {
                cCustomerDataEntry.FromConfigData(csINI, csSection, false);
            }
            else
            {
                trace(L4, _T("Configuration Error: No Section data for INIfile=%s, Section=%s"),csINI, csSection);
            }
            
            // Adding sView/sViewCustomer after the cDataEntry settings gives 
            // precedence to settings in the INI file. 
            csViewString = cDataEntry.ToString() + csViewString;
        }
        else
        {
            trace(L4, _T("GetDataNeededView error, if INI specified, section is required"));
            ps.SetLanguage(m_customerLanguage); //Jira SSCOP-831
        }
    }
    
    /* //+MEIJER RAP Scanner
    if (csViewString.Find(_T("cGetHHScanner")) != -1)
    {
        csViewString += csRAPScannerButton;
    }
    */ //-MEIJER RAP Scanner

    trace(L6, _T("ViewString: %s"), csViewString);
    ps.SetLanguage(m_customerLanguage); //Jira SSCOP-831
    
    sm_pDataNeededEntry.FromString(csViewString);
    
    m_View = DATANEEDED_MOREDATA;    // in case the view got reset by onRapDataNeeded somewhere
    
    DMTurnOffTriColorLight();
    //-SSCOP-2691
    
    trace (L6, _T("-SMSmMobileAudit::RetrieveDataNeededDefinition()"));
}

#define DESKTOP_NUMBUTTONS 7 //Number of pageable user buttons (excluding SMButton8)
void SMSmMobileAudit::SetUserButton(int nIndex, int nPageOffset)
{
    CString csButtonControlName = EMPTY_STRING;
    csButtonControlName.Format(_T("%s%d"), _T("SMButton"), (nIndex + 1));
    bool bEnabled = !sm_pDataNeededEntry.GetButtonText(nIndex).IsEmpty() && 
                    !sm_pDataNeededEntry.GetButtonCommand(nIndex).IsEmpty();
	if(nIndex==DESKTOP_NUMBUTTONS && co.IsLoginBypassOn() && 
	   sm_pDataNeededEntry.GetButtonCommand(nIndex) == _T("cGoBack"))
	{
		ps.ButtonState(csButtonControlName, false, false);
	}
    else if(sm_pDataNeededEntry.GetButtonText(nIndex) !=_T(""))
        ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(nPageOffset + nIndex), bEnabled);
    else
        ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(nPageOffset + nIndex), false);
}

SMStateBase *SMSmMobileAudit::DMScanner(void)
{
    if(1 == sm_pDataNeededEntry.GetScannerDataFormat())   //Allow Scan if scannerDataFormat is 0 or 2
    {
       trace(L6,_T("ERROR: Was expecting a 2D barcode type!"));
       Beep(500, 500);
       DMScannerEnable();
       return STATE_NULL;
    }

	if(sm_pDataNeededEntry.IsScannerInputEnabled())
	{
		trace(L2, _T("SMSmDataEntry::DMScanner ()"));
		m_sInput = csDMLastBarCode; 
        m_sButton = szButtonScanData;
		USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	}
	else
	{
		return STATE(OperatorPasswordState)::DMScanner();
	}
}
//TAR287018-

/////////////////////////////////////
// SR93.4
// DMScannerEncrypted
// 
// Purpose: Handles 2D barcode types.
//          Encrypted barcode types are listed in the registry as:
//
//          [HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT\CurrentVersion\SCOTAPP]
//          "EncryptedBarcodes"="201,202,203,204,205,206,207"
//
///////////////////////////////////////
// SR93.4+
SMStateBase *SMSmMobileAudit::DMScannerEncrypted(void)
{
    if( 1 == sm_pDataNeededEntry.GetScannerDataFormat() ||
        2 == sm_pDataNeededEntry.GetScannerDataFormat())
    {
        m_sButton = _T("SCANDATAENCRYPTED");
        USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
    }
    else
    {
       trace(L6,_T("ERROR: Was expecting a 2D barcode type!"));
       Beep(500, 500);
       DMScannerEnable();
       return STATE_NULL;
    }
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmMobileAudit::HandleDataNeeded()
{
    SMStateBase* retState = STATE_NULL;
    m_bWaitForResponse=true;
    delete pUserInputLock;
    pUserInputLock = new CPSXUserInputLock(T_ID);
    static long rc;  // TB return value

    CString pDetails;
    
    trace(L6, _T("+SMSmMobileAudit::HandleDataNeeded()"));
    trace(L6, _T("m_sButton=%s, m_sInput=%s"), m_sButton, m_sInput);

    if (m_sButton.Find(szButtonScanData) == 0) //449853
    {
        //<ScanInfo>
        //    <ButtonData>%s</ButtonData>
        //    <BarcodeType>%d</BarcodeType>
        //    <BarcodeLength>%d</BarcodeLength>
        //    <BarcodeData>%s</BarcodeData>
        //    <BarcodeLabelLen>%d</BarcodeLabelLen>
        //    <BarcodeLabel>%s</BarcodeLabel>
        //</ScanInfo>

        trace(L6, _T("SMSmMobileAudit::HandleDataNeeded(), notify audit")); 
        CString csSendBarcodeFormat;
        csSendBarcodeFormat.Empty();
        csSendBarcodeFormat += _T("<ScanInfo><ButtonData>%s</ButtonData><BarcodeType>%d</BarcodeType>");
        csSendBarcodeFormat += _T("<BarcodeLength>%d</BarcodeLength><BarcodeData>%s</BarcodeData>");
        csSendBarcodeFormat += _T("<BarcodeLabelLen>%d</BarcodeLabelLen><BarcodeLabel>%s</BarcodeLabel></ScanInfo>");
       
        pDetails.Format(csSendBarcodeFormat,
            m_sButton,
            csDMLastBarType,
            csDMLastBarCode.GetLength(),
            csDMLastBarCode,
            csDMLastBarLabel.GetLength(),
            csDMLastBarLabel
            );
        
        if (_tcsicmp((LPCTSTR) mobileAudit.ResultMessage(), szPartialRescanFailed) == 0)
        {
            mobileAudit.SendContinueAfterErrorRequest((_bstr_t) csDMLastBarLabel);
        }
        else
        {
            mobileAudit.SendAddAuditItemRequest((_bstr_t) pDetails);
        }
        nTimeOut = m_nResponseTimer;
        m_bWaitForResponse = true;
    }
    else
    {
        CString csSendDataFormat = _T("ButtonData=%s;InputData=%s");
        pDetails.Format(csSendDataFormat, m_sButton, m_sInput);
    }

    if (m_sButton == szButtonFinishMobileCartAudit)
    {
        mobileCart.SendReadyForTransaction();
        
        mobileAudit.SendEndAuditRequest(szActionSend);

        RetrieveDataNeededDefinition(szEmpty, 
                                     (LPCTSTR) mobileAudit.DataNeededViewString(), 
                                     ms_MobileAuditDataEntryINI, 
                                     (LPCTSTR)mobileAudit.DataNeededSection());
        retState = Initialize();

        delete getRealStatePointer(retState);
        
        retState = STATE_NULL;
    }
    else if (m_sButton == szButtonContinueAfterError)
    {
        mobileAudit.SendContinueAfterErrorRequest(mobileAudit.ItemBarcode());
        
        m_sButton.Empty();

        retState = STATE_NULL;
    }
    else if (m_sButton == szButtonContinueFullRescanErr)
    {
        mobileAudit.SendContinueAfterFullRescanError();
        RetrieveDataNeededDefinition(szEmpty, 
                                     (LPCTSTR) mobileAudit.DataNeededViewString(), 
                                     ms_MobileAuditDataEntryINI, 
                                     (LPCTSTR)mobileAudit.DataNeededSection());
        retState = Initialize();

        m_bWaitForResponse=false;

    }
    else if (m_sButton == szButtonFullAudit)
    {
        if (cr.GetFlagOption(_T("Operations"),_T("MobilePartialAuditRecordsClearDuringFullAudit"), true, _T("Scotopts")))
        {
            mobileAudit.ClearAuditFailureItems();
        }
        mobileAudit.SendSwitchToFullAuditRequest(_T("true"));
        retState = STATE_NULL;
    }
    else if (m_sButton == szButtonExitAudit)
    {
        mobileAudit.RequestAbortAudit();

        RetrieveDataNeededDefinition(szEmpty, 
                                     (LPCTSTR) mobileAudit.DataNeededViewString(), 
                                     ms_MobileAuditDataEntryINI, 
                                     (LPCTSTR)mobileAudit.DataNeededSection());

        retState = Initialize();

        m_bWaitForResponse=false;
    }
    else if (m_sButton == szButtonReturnToAudit)
    {
        mobileAudit.ReturnToPartialAuditScreen();

        RetrieveDataNeededDefinition(szEmpty, 
                                     (LPCTSTR) mobileAudit.DataNeededViewString(), 
                                     ms_MobileAuditDataEntryINI, 
                                     (LPCTSTR)mobileAudit.DataNeededSection());

        retState = Initialize();

        m_bWaitForResponse=false;
    }
    else if (m_sButton == szButtonAbortAudit)
    {
        m_bAbortingAudit = true;
        //mobileAudit.SendExitAuditRequest();
        mobileAudit.SendSwitchToFullAuditRequest(_T("true"));
        
       // RetrieveDataNeededDefinition(szEmpty, 
       //                              (LPCTSTR) mobileAudit.DataNeededViewString(), 
       //                              ms_MobileAuditDataEntryINI, 
       //                              (LPCTSTR)mobileAudit.DataNeededSection());

       // retState = Initialize();

        delete getRealStatePointer(retState);

        m_pAppPSX->ClearReceipt(_T("CMSMReceipt"));

        retState = STATE_NULL;
    }
/*	//+MEIJER RAP Scanner
    else if (m_sButton == szGetHHScanner) 
    {
        CString csCompName=_T("");
		m_bWaitingForStatusToConnect = true;

		m_bWaitForResponse=false;

		csCompName = SMState::GetLaneName();
		csCompName += _T(";");
		
		CString csOperation=_T("operation=scanner-status-");
		csOperation = csOperation + csCompName;
		CString csRAPName = SMState::GetRAPConnectionName();
		
		m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, csRAPName );
	
    }
    else if (m_sButton == szReleaseHHScanner) 
    {
        //MEIJER RAP Scanner
        CString csRAPName;
        m_bWaitForResponse=false;
        
        if(SMSmAssistMode::m_csRAPConnectionName == "")
        {
            SMSmAssistMode::m_csRAPConnectionName = SMState::GetRAPConnectionName();
        }
       
        CString csOperation=_T("operation=release-scanner;");
        m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, SMSmAssistMode::m_csRAPConnectionName );

    }
*/ //-MEIJER RAP Scanner

    m_sButton.Empty(); 

    trace(L6, _T("-SMSmMobileAudit::HandleDataNeeded() details=%s"),pDetails);
    return retState;
}

void SMSmMobileAudit::AddAuditItemToReceipt()
{
    //need to get ScanEntryID from mobCart and use it as PSX receipt entry ID
    trace(L6, _T("+SMSmMobileAudit::AddAuditItemToReceipt()")); 
    CString csItemEntryId = (LPCTSTR) mobileAudit.ItemId();
    CString csReceiptSrc = _T("CMSMReceipt");

    m_pAppPSX->CreateReceiptItem(csReceiptSrc, csItemEntryId);

    bool bSuccess = _tcsicmp((LPCTSTR) mobileAudit.Result(), szSuccess) == 0;

    bool bRescanNotInCart = _tcsicmp((LPCTSTR) mobileAudit.ResultMessage(), szPartialRescanFailed) == 0;

    if (!bSuccess && !bRescanNotInCart)
    {
        return;   //Dont put Unknown Item, other errors on receipt.  Just items that affect "remaining" counts.
    }

    CString csDesc = (LPCTSTR) mobileAudit.ItemDescription();

    COleVariant vValue;

	vValue = csDesc;
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);

	vValue = (LPCTSTR) mobileAudit.ItemBarcode();
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);

	vValue = (LPCTSTR) mobileAudit.ItemPrice();
	m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);

    if ( bRescanNotInCart )
    {
        CString csMsgItemEntryId = csItemEntryId + _T("NotInCart");

        m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemEntryId, csItemEntryId);

        CString csNotInCart = ps.GetPSText(MOBILE_AUDIT_NOT_IN_TICKET, SCOT_LANGUAGE_PRIMARY);

        vValue= csNotInCart;

        m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemEntryId, _T("SUB_ITEM_DESCRIPTION"), vValue);
    }

    UpdateRescanInfo();
    m_pAppPSX->UpdateReceiptControls(csReceiptSrc);
    trace(L6, _T("-SMSmMobileAudit::AddAuditItemToReceipt()")); 
}

void SMSmMobileAudit::UpdateTicketInfo()
{
    trace(L6, _T("+SMSmMobileAudit::UpdateTicketInfo()")); 
    CString csReceiptSrc = _T("CMSMReceipt");
    CString csMsgLoyaltyEntryID = _T("MobileTicketLoyaltyID");
    CString csMsgItemEntryId = _T("MobileTicketItems");
    CString csMsgItemAmountId = _T("MobileTicketAmount");
    CString csLoyaltyCardMsg;
    COleVariant vValue;

    m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgLoyaltyEntryID);
    
    m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemEntryId);
    m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemAmountId);

    CString csLoyaltyId = (LPCTSTR) mobileAudit.LoyaltyId();
	CString csLastTwo = csLoyaltyId.Mid(csLoyaltyId.GetLength()-2, 2);
	csLoyaltyId = _T("XXXXXXXX") + csLastTwo;

    csLoyaltyCardMsg.Format(ps.GetPSText(MSG_RA_LOYALTYCARDINFO, SCOT_LANGUAGE_PRIMARY), csLoyaltyId);

    vValue = csLoyaltyCardMsg;
    m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgLoyaltyEntryID, _T("SUB_ITEM_DESCRIPTION"), vValue);

    CString csItemsInTicket;

    csItemsInTicket.Format(ps.GetPSText(MOBILE_AUDIT_TOTAL_ITEMS_IN_ORDER, SCOT_LANGUAGE_PRIMARY), (LPCTSTR) mobileAudit.NumberOfItemsInTicket());

    vValue= csItemsInTicket;

    m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemEntryId, _T("SUB_ITEM_DESCRIPTION"), vValue);
    

    CString csAmountInTicket;

    long lTotalAmount = mobileAudit.GetTotalTicketAmount();

    csAmountInTicket.Format(ps.GetPSText(MOBILE_AUDIT_TOTAL_ORDER_AMOUNT, SCOT_LANGUAGE_PRIMARY), ps.FmtDollar(lTotalAmount));
    
    vValue = csAmountInTicket;

    m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csMsgItemAmountId, _T("SUB_ITEM_DESCRIPTION"), vValue);

    
    m_pAppPSX->UpdateReceiptControls(csReceiptSrc);
    trace(L6, _T("-SMSmMobileAudit::UpdateTicketInfo()")); 

}


void SMSmMobileAudit::UpdateRescanInfo()
{
    CString csNumItems = (LPCTSTR) mobileAudit.NumItemsScanned();

    trace(L7, _T("+ra.OnNormalItem, Sending the number of items to RAP"));
    
    ra.OnNormalItem(_T("[NumItems]") + csNumItems);
    
    //long lAmountRemaining = mobileAudit.GetRemainingAmount();

    ps.Total( mobileAudit.GetScannedItemsAmount() );

    ps.ShowSMTotal(true);
}

SMStateBase * SMSmMobileAudit::Parse(MessageElement *me)
{
    //nTimeIdleMsec = 0;
    nLastMsgID = me->id;

    if (me->id == ID_GP)
    {
        //SSCOMobile
        if (GP_TCP_RECEIVED == me->gpInfo.nEvt ||
            GP_TCP_DISCONNECTED == me->gpInfo.nEvt ||
            GP_TCP_CONNECTED == me->gpInfo.nEvt)
        {
            return ParseMobile(me);
        }
    }
    return SMStateBase::Parse(me);
}

void SMSmMobileAudit::PSParseFinalProcessing(const MessageElement* me, SMStateBase* pRetState)
{
    trace(L7, _T("+SMSmMobileAudit::PSParseFinalProcessing()"));
 
    if (!m_bWaitForResponse)
    {
        UnlockButtons();
    }
    else
    {
        nTimeOut = m_nResponseTimer;
    }
    //Do nothing,  unlock buttons after we get a response.
    trace(L7, _T("-SMSmMobileAudit::PSParseFinalProcessing()"));

}

SMStateBase *SMSmMobileAudit::OnMobileTransactionStart(CString csTransactionInfo)
{
    SMStateBase* retState = STATE_NULL;
    trace(L6, _T("+SMSmMobileAudit::OnMobileTransactionStart m_bAbortingAudit = %d"), m_bAbortingAudit );

    if (!m_bAbortingAudit)
    {
    if (IsValidMobileTransaction(csTransactionInfo))
    {
        retState = TBStart();
        PaintScanAndBagScreen();
    }
    }
    else
    {

        mobileAudit.ClearAuditFailureItems();  //Server won't accept them here.
        mobileCart.SendFinishTransaction(0, 0);
        retState = setAndCreateAnchorState(BES_START);
    }
    trace(L6, _T("-SMSmMobileAudit::OnMobileTransactionStart") );
    return retState;
}

bool SMSmMobileAudit::IsValidMobileTransaction(CString csTransactionInfo)
{
    trace(L6, _T("+SMSmMobileAudit::IsValidMobileTransaction %s"), csTransactionInfo );

    bool bIsQBItemList = (csTransactionInfo.Find(MOBILE_START) == 0);

    if ( bIsQBItemList )
    {
        bool bAlreadyInRecall = g_bInRecallMode;

        if (!g_bInRecallMode && bIsQBItemList && recallList.LoadQBBarcodes(csTransactionInfo))
        {
            g_bInRecallMode = true;
            g_bEnteringRecallMode = true;
        }

        m_pAppPSX->ClearReceipt(_T("CMSMReceipt"));

        CString csButtonText;

        csButtonText = ps.GetPSText(MSG_LANGUAGENAME);
        rp.SetLanguageName(csButtonText);

        if ( bQuietNeedSecApproval )  //TAR234536
        {
            bQuietNeedSecApproval = false;
            m_bScanOnAttractAtQuietMode = true;
        }
    }
    return bIsQBItemList;
}

void SMSmMobileAudit::PaintScanAndBagScreen()
{
#ifndef _CPPUNIT
    CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendLaneStateData(_T("ScanAndBag"), szEmpty, false, false);
    //laneStateData.SendNextGenUIData(_T("NextGenUIApplicationState"), (CString) laneStateData.INTRANSACTION);

    g_bSnBScreenIsShown = true; //TAR195100 CMal/Chu021402 
    bAlreadyInSnB = true;

    if (co.fOperationsDisplayQuickPickItems)
    {
        if (co.fCMRewardInfo)
        {
            ps.SetCMFrame(_T("QuickPickItemsWithReward"));
            csCurContext = _T("QuickPickItemsWithReward");
        }
        else
        {
            ps.SetCMFrame(_T("QuickPickItems"));
            csCurContext = _T("QuickPickItems");
        }
    }
    else
    {
        if (co.fCMRewardInfo)
        {
            ps.SetCMFrame(_T("ScanAndBagWithReward")); //*** NewUI ***//
            csCurContext = _T("ScanAndBagWithReward");
        }
        else
        {
            ps.SetCMFrame(_T("ScanAndBag"));  //*** NewUI ***//
            csCurContext = _T("ScanAndBag");
        }
    }

    // SR880 ++
    if (co.fOperationsCustomerBagAllowedInScanAndBag)
    {
        m_pAppPSX->SetConfigProperty(_T("CMButton1LookUp"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCode"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("CMButton1LookUpIn3ButtonRow"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCodeIn3ButtonRow"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("CMButton6OwnBag"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    }
    else
    {
        m_pAppPSX->SetConfigProperty(_T("CMButton1LookUp"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCode"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("CMButton1LookUpIn3ButtonRow"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("CMButton2KeyInCodeIn3ButtonRow"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("CMButton6OwnBag"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    // SR880 --

    COleVariant v = VARIANT_FALSE;
    m_pAppPSX->SetConfigProperty(_T("LeadthruText"), csCurContext, UI::PROPERTYVISIBLE, v);
    ps.SetLeadthruText(LTT_SCANPROCESSING);
    v = VARIANT_TRUE;
    m_pAppPSX->SetConfigProperty(_T("LeadthruText"), csCurContext, UI::PROPERTYVISIBLE, v);

    ps.ShowCMTotal(true, false);
    m_pAppPSX->SetConfigProperty(_T("ScanBagMessageModalBackGround"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
    m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
    v = szEmpty;
    m_pAppPSX->SetConfigProperty(_T("ScanBagTextArea"), csCurContext, UI::PROPERTYTEXTFORMAT, v);
    m_pAppPSX->SetConfigProperty(_T("ScanBagAVIMessageModalBackGround"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("ScanBagVideo"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    //TAR 355761 -start
    CString csButtonText;
    if (co.GetfStateDualLanguage() &&
        (co.fStateAllowLanguageSelectionDuringTransaction ||
        (custInfo.GetOption(_T("PersonalizeLanguage")) == "Y")))
    {
        if (co.GetNumberOfLanguagesSupported() > 2)
        {
            // TAR 437633 +
            //csButtonText = ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY);
            ps.SetPSText(PS_WORK1, ps.GetPSText(BTT_LANGUAGE, SCOT_LANGUAGE_PRIMARY));
            // TAR 437633 -
        }
        else if (co.GetNumberOfLanguagesSupported() == 2)
        {
            int i = SCOT_LANGUAGE_PRIMARY; //tar 389302/388448
            int iButtonID;

            if (m_customerLanguage == SCOT_LANGUAGE_PRIMARY)
            {
                i = SCOT_LANGUAGE_SECONDARY;
            }
            else if (m_customerLanguage != SCOT_LANGUAGE_SECONDARY)//tar 389302/388448
            {
                //fix TAR388448+
                trace(L2, _T("WARNING: Unexpected value %d for m_customerLanguage;reset to primary language"), m_customerLanguage);
                m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
                //fix TAR388448-
                //i = SCOT_LANGUAGE_PRIMARY;
            }

            //Get the language name text for button
            csButtonText = co.Getlanguage(i).csName;
            if (_istdigit(*csButtonText))
            {
                iButtonID = _ttoi(co.Getlanguage(i).csName);
                csButtonText = ps.GetPSText(iButtonID, (SCOT_LANGUAGE_TYPE)i);
            }
            ps.SetPSText(PS_WORK1, csButtonText);
        }
        else
        {
            ASSERT(FALSE); // shouldn't enter here because at least 2 languages should be supported
        }

        ps.SetButtonText(_T("CMButton5MidiList"), PS_WORK1);
        //make visible the CMButton5MidiList
        m_pAppPSX->SetConfigProperty(_T("CMButton5MidiList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    }
    //TAR 355761 -end

    if (co.GetfStateDualLanguage())
    {
        if (m_LanguageUsedPreviously != m_customerLanguage)
        {
            m_LanguageUsedPreviously = m_customerLanguage;
        }
    }
#endif //_CPPUNIT
    ps.ShowCMFrame();
}

void SMSmMobileAudit::UnlockButtons()
{
    m_bWaitForResponse = false;
    delete pUserInputLock;
    pUserInputLock = NULL;
    
    CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendNextGenUIData(_T("RemoteProxyUserInputCompleted"), true);
    
}

SMStateBase *SMSmMobileAudit::TimedOut(void)
{
    SMStateBase* retState = STATE_NULL;
    if (m_bWaitForResponse)
    {
        nTimeOut = 0;
        CString csMessageView(_T("SUMMARYINSTRUCTION="));
        csMessageView += ps.GetPSText(MOBILE_AUDIT_ERROR_TIMEOUT_TXT, SCOT_LANGUAGE_PRIMARY) + _T(";");
        RetrieveDataNeededDefinition(szEmpty, 
                                     csMessageView,
                                     ms_MobileAuditDataEntryINI, 
                                     _T("MobileAuditError"));
        retState = Initialize();

        UnlockButtons();
        
    }
    return retState;
}

SMStateBase * SMSmMobileAudit::PSParse( const MessageElement* me)
{
    SMStateBase *retState = SMState::PSParse(me);
/* //+MEIJER RAP Scanner
    trace(L6, _T("+SMSmMobileAudit::PSParse"));
	switch(SMStateBase::nPSLastEvt)
    {

    case PS_RAPSCANNERENGAGED:
        {
			trace(L6, _T("SMSmMobileAudit::PSParse PS_RAPSCANNERENGAGED"));

            m_bRAPScannerAvailable = false;
            m_bRAPScannerConnected = true;
            RetrieveDataNeededDefinition(szEmpty, 
                (LPCTSTR) mobileAudit.DataNeededViewString(), 
                ms_MobileAuditDataEntryINI, 
                (LPCTSTR)mobileAudit.DataNeededSection());
            DisplayUserButtons();
        }
        break;
    case PS_RAPSCANNERAVAILABLE:
        {
			trace(L6, _T("SMSmMobileAudit::PSParse PS_RAPSCANNERAVAILABLE"));

			if (m_bWaitingForStatusToConnect)
			{
				m_bWaitingForStatusToConnect = false;
				CString csCompName=_T("");
				
				//MEIJER RAP Scanner
				CString csOperation=_T("operation=request-scanner-");
				
				csCompName = SMState::GetLaneName();
				
				csCompName += _T(";");
				
				csOperation = csOperation + csCompName;
				
				if(SMSmAssistMode::m_csRAPConnectionName == "")
				{
					SMSmAssistMode::m_csRAPConnectionName = SMState::GetRAPConnectionName();
				}
				
				m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), csOperation, SMSmAssistMode::m_csRAPConnectionName );
			}
			else
			{

				m_bRAPScannerAvailable = true;
				m_bRAPScannerConnected = false;
				RetrieveDataNeededDefinition(szEmpty, 
					(LPCTSTR) mobileAudit.DataNeededViewString(), 
					ms_MobileAuditDataEntryINI, 
					(LPCTSTR)mobileAudit.DataNeededSection());
				DisplayUserButtons();
				CString csCompName=_T("");
				
			}
        }
        break;
    case PS_RAPSCANNERUNAVAILABLE:
        {
			trace(L6, _T("SMSmMobileAudit::PSParse PS_RAPSCANNERUNAVAILABLE"));
			if (!m_bRAPScannerConnected)
			{

				m_bRAPScannerAvailable = false;
				m_bRAPScannerConnected = false;
				RetrieveDataNeededDefinition(szEmpty, 
					(LPCTSTR) mobileAudit.DataNeededViewString(), 
					ms_MobileAuditDataEntryINI, 
					(LPCTSTR)mobileAudit.DataNeededSection());
				DisplayUserButtons();
			}
			else
			{
				trace(L6, _T("Ignoring,  Rap scanner is MINE"));
			}
            
        }
        break;

    }
*/ //-MEIJER RAP Scanner
	trace(L6, _T("-SMSmMobileAudit::PSParse"));

    return retState;
}

