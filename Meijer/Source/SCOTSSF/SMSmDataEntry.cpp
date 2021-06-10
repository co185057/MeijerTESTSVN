//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmDataEntry.CPP
//
// TITLE: Class implementation for store mode Restricted Items state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmDataEntry.h"    // MGV this state
#include "SMSmMobileAudit.h"
#include "FLTCPDefines.h"
#include "MobileCartAudit.h"

#include "transbrokerssf.h"
#include "SMSmAssistMenu.h"
#include "SMSmAssistMode.h"
#include "SMSmMobileAudit.h"	// CDR126194
#include "FLTCPDefines.h"		// CDR126194
#include "MobileCartAudit.h"	// CDR126194
#include "SMFinish.h"
#include "SMInProgress.h"

#include "SMSmMobileAudit.h"	// CDR126194
#include "FLTCPDefines.h"		// CDR126194
#include "MobileCartAudit.h"	// CDR126194

#include "SMSmGenericDelayed.h"	//CRD 223409
#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmDataEntry")
extern CMobileCartAudit mobileAudit; // CDR126194

extern CMobileCartAudit mobileAudit; // CDR126194

IMPLEMENT_DYNCREATE(SMSmDataEntry, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMSmDataEntryTSDemo.cpp"
#endif //_TRADESHOWDEMO

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmDataEntry::SMSmDataEntry()
{
  IMPLEMENT_TIMESTAMP
}

#ifndef _TRADESHOWDEMO
SMStateBase *SMSmDataEntry::Initialize()
{
   int iFind, iInputLength;
    CString csSayPhrase;

	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmDataEntry") ); //382138
	}

    if(getAnchorState() != BES_SMDATAENTRY) // save off for possible cancel later
        nlaststate = getAnchorState();
    
    m_nBtnPageIndex = 0;
    m_bPageButtons = false;
    inputMode = OPERATOR_PASSWORD_ACCEPTED;
	// Tar 210056 RF 071902 - initialize input fields
    m_sButton.Empty();
    m_sInput.Empty();

	//cDataEntry.FromConfigData(csDataEntryINIFile, m_sView);
    CString csInputString = sm_pDataNeededEntry.GetInputMask();
    CString csDetails = sm_pDataNeededEntry.GetDetailedInstruction();	// RFC 211581 RJF 081602
    COleVariant vValue = VARIANT_FALSE;

	if (dm.IsOpen()) //tar 352514
	{
		dm.turnOffTriColorLight();
		dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
							 DM_TRICOLORLIGHT_ON,
							 DM_TRICOLORLIGHT_NORMALREQUEST);
	}

	//RFQ 986
	CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay(); 

    // POS16600 - rrs
    if(csContextToDisplay == _T("TakeChangeTimeOut") )
    {
        // we need to call the OnNoRAPDataNeeded in here so that when the attendant press the ALERT from the RAP
        // or logs in on the lane, we will call the ReportInterventionEnd and the action will be in TLOG
        ClearSecurityViolationLight();//End of Tar 209034 & 209045
       //Clear RAP Approval 
        trace(L2, _T("Calling ra.OnNoRAPDataNeeded() For TakeChangeTimeOut intervention"));
        ra.OnNoRAPDataNeeded();    
    }
    // e POS16600 - rrs*/


	m_bShiftOn = false;
	m_bCapsInput = false;
	if((csInputString != _T("")))
    {
        if (csInputString[0] == _TCHAR('A'))
	    {
		    m_bCapsInput = true;
        }
	}

   if (!csContextToDisplay.IsEmpty())
	{
        // Note: If using Numeric contexts the name should start with 
        // "SmDataEntry" (like "SmDataEntryNumericSmall" to allow the PSEnterClearKeys function
        // to work properly.
		csCurContext = csContextToDisplay;
        //RFQ 2506 - If in Assist menus,  show store-mode version of specified context
		if (!sm_pDataNeededEntry.IsStoreLogonEnabled() && co.IsInAssistMenus())
		{
			ps.SetCMFrame(csCurContext);  //Show "AM_" contexts if in Basic Assist showing customermode screen
		}
		else
		{
			ps.SetFrame(csCurContext);
		}
	}									//end RFQ 986
    else if (csInputString.IsEmpty())
    {
	    if(csDetails.IsEmpty())
        {
            csCurContext = _T("SmDataEntry");  // Tar 238987
            ps.SetFrame(csCurContext);
            m_pAppPSX->SetConfigProperty(_T("SMReceipt"), csCurContext, UI::PROPERTYVISIBLE, vValue);
         }
	    else
        {
            csCurContext = _T("SmDataEntryWithMediaList");  // Tar 238987
            ps.SetFrame(csCurContext);
            m_pAppPSX->SetConfigProperty(_T("MediaList"), csCurContext, UI::PROPERTYVISIBLE, vValue);
        }
    }
    else
	    ps.SetFrame(_T("SmDataEntryWithListBG"));
    ps.Echo(PS_BLANK);
    ps.SetTitle(sm_pDataNeededEntry.GetTopCaption());
	if (csCurContext.Find(_T( "Alphanumeric")) != -1)
	{
		if (m_bCapsInput)
		{
			SetAlphaShiftState(csCurContext, m_bCapsInput);
		}
		else
		{
			SetAlphaShiftState(csCurContext, m_bShiftOn);
		}
	}

    // TAR 228468 - Check for duplicate text messages.
    CString csSummary = sm_pDataNeededEntry.GetSummaryInstruction();
	if (csSummary == m_sDesc)
    {
      ps.SetLeadthruText(csSummary, _T(""));
    }
    else 
    {
	  ps.SetLeadthruText(sm_pDataNeededEntry.GetSummaryInstruction(), _T("") );    
      }
	m_pAppPSX->SetTransactionVariable(_T("InstructionText"), csSummary); //TAR 400042
    ps.Prompt(sm_pDataNeededEntry.GetBottomCaption());
    if(!csDetails.IsEmpty())			// RFC 211581 RJF 081602
    {
        ps.ResetMediaList();
	
		ps.AddMediaList(csDetails);
		if (csContextToDisplay.IsEmpty()) 
		{
			csCurContext = _T("SmDataEntryWithMediaList"); // Tar 238987
		}
		vValue = VARIANT_TRUE;
        m_pAppPSX->SetConfigProperty(_T("MediaList"), csCurContext, UI::PROPERTYVISIBLE, vValue);
    }
    else
    {
		if (csContextToDisplay.IsEmpty()) 
		{ 
			csCurContext = _T("SmDataEntry"); // Tar 238987 
		}
		vValue = VARIANT_TRUE;
        m_pAppPSX->SetConfigProperty(_T("SMReceipt"), csCurContext, UI::PROPERTYVISIBLE, vValue);
    }

	CString csResourcePath = sm_pDataNeededEntry.GetVideoPath();

	//Start CRD 313027
	if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && 
		(
			sm_pDataNeededEntry.GetSectionName() == _T("MiscanPrompt") ||
			sm_pDataNeededEntry.GetSectionName() == _T("ProductSwitchPrompt") ||
			sm_pDataNeededEntry.GetSectionName() == _T("BasketLossPrompt")
		) 
	   )
	{
		//If an everseen alert, then we need to clear Operator Guidance
		CString csOk(_T("{F1}"));
		SendTBCommand(TBCMD_SENDKEYEDDATA, csOk);
		
		//get root video path from hook property
		CString csEverseenVideoPath = _T("");
		csEverseenVideoPath = GetTBProperty(_T("EverseenVideoPathProp"));

		//if video path is not empty, everseen is configured to show a video
		if(csEverseenVideoPath != _T(""))
		{
			CString csEverseenVideoName = _T("");
			csEverseenVideoName = GetHookProperty(_T("OG_Guidance_Prop"));
			csResourcePath.Format(_T("%s\\%s"),  csEverseenVideoPath, csEverseenVideoName);
			trace(L6, _T("Everseen video path - %s"), csResourcePath);
		}
	}
	//End CRD 313027
	
    if(!csResourcePath.IsEmpty())
    {
        vValue = csResourcePath;
        m_pAppPSX->SetConfigProperty(_T("DataEntryVideo"), csCurContext, UI::PROPERTYVIDEO, vValue);
    }

    /* TAR 308320+  lp185019
    for (int i=0; i<8; i++)
    {
		CString csButtonControlName = EMPTY_STRING;
		csButtonControlName.Format(_T("%s%d"), _T("SMButton"), (i+1));
		if(sm_pDataNeededEntry.GetButtonText(i) !=_T(""))
			ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), true);
		else
			ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), false);
	} //-- make use of the DisplayUserButtons() to display button paging also in the HHRAP.
    */
	DisplayUserButtons();  //TAR 308320-  lp185019
    //RFQ 2506 - Build CmdBtn and CmdList control data, if necessary
    BuildCmdButtonList(sm_pDataNeededEntry, csCurContext, _T("CmdBtn_DataEntryList") );
	BuildCmdList(sm_pDataNeededEntry, _T("DataNeededReceipt"));

    if(m_sView == _T("AssistedEndorsement"))
	{
		if(sm_pDataNeededEntry.GetButtonText(0) !=_T(""))
			ps.Button(_T("SMButton1"), _T(""), false);

		ps.ButtonState(_T("SMButton8"),true,true);
		PrintEndorsement();
        ra.OnNoRAPDataNeeded(); // TAR402023         
    }
	if( (m_sView == _T("NegativeMedia")) || (m_sView == _T("GetCoupon")) )
	{
		g_bMatchedWtWaitingOnEAS = false;
		g_bWtExpected = false;	
		g_lNumberOfWLDBEntriesThisItem = -1;

		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		BSTR bstrResult;
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);

		//SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
	}

   if(m_sView == _T("PricePrompt"))  //POS11024 Mobile Shopper
   {
		CString csPropName = _T("AgeRequiredLeadThru");
      CString csTempText;

      SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPropName, &csTempText);
      ps.SetLeadthruText(csSummary, csTempText);
   }

   //Start CRD 313052 - need to set to Assist for Meijer.com Prompt so that SMSmAborted won't show anymore during finishing
   if(m_sView == _T("MeijerComPrompt"))
	   fStateAssist = true;
   //End CRD 313052

   //POS11024 Fix PriceVerify resume display not always showing desc, itemcode, price 
	if(m_sView == _T("PriceVerify") || 
      sm_pDataNeededEntry.GetTopCaption() == ps.GetPSText(9163))
	{	//Show item description and price in PriceVerify screen
		CString csPropName = _T("PriceVerifyItemDesc");
		CString csItemDesc, csItemPrice, csItemUPC;  //POS11024 add ItemUPC

		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPropName, &csItemDesc);
      csPropName = _T("PropVerifyItemPrice");
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPropName, &csItemPrice);
		csSummary = csItemDesc + _T("     $") + csItemPrice; 
      csPropName = _T("PriceVerifyItemUPC");       //POS11024 add ItemUPC
		SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPropName, &csItemUPC);
		ps.SetLeadthruText(csSummary, csItemUPC);    //POS11024 ItemUPC on leadthru line2
	}

    //TAR287018+
	//Let .ini file decide if the scanner is enabled
	if(sm_pDataNeededEntry.IsScannerInputEnabled())
	{
		bEASReEnableScanner = true;
		SAWLDBScannerEnable();		  // enable the scanner
	}
	else							// Tar 210186 RF 071902
		SAWLDBScannerDisable();		// disable scanner if ini parameter set to 0 or not present
	//TAR287018-

	if(sm_pDataNeededEntry.IsMSRInputEnabled())   //RFQ 2605 - MSR Input from DataNeeded
	{
		DMMSREnable();		  // enable the msr
	}
	else							// Tar 210186 RF 071902
		DMMSRDisable();		// disable msr if ini parameter set to 0 or not present


    if(sm_pDataNeededEntry.IsTotalHidden())
        ps.ShowSMTotal(false);  // when false only shows total after data entered
    else
        ps.ShowSMTotal(true);

    // allow the EchoInput() func to accept up to max digits

	iInputLength = csInputString.GetLength();
    if(iInputLength)
    {
	   bool bHideInput = sm_pDataNeededEntry.IsInputHidden();	// Tar 230459
       int iInputMinLen = sm_pDataNeededEntry.GetInputLengthMin(); //RFC 335528
       csInputString.TrimLeft(csInputString.Find(_T('@')));
       iFind = csInputString.Find(_T("v"));
       if (iFind != -1)    // if there is a decimal(v), decrement length
           --iInputLength;

       if(m_sView == _T("PricePrompt"))
	   {
          //Tar 193493
          m_View = DATANEEDED_PRICEPROMPT;
          //Tar 193493
          if (iInputLength > MAX_DIGITS_CASHAMT)
		  {
              iInputLength = MAX_DIGITS_CASHAMT;
          }
          ps.AllowInput(iInputLength,bHideInput,true, iInputMinLen);	// Tar 230459
	   }
	   else
	   {
           if (iFind != -1)    // if there is a decimal we need to adjust format to currency
           {
               if (iInputLength > MAX_DIGITS_CASHAMT)
               {
                   iInputLength = MAX_DIGITS_CASHAMT;
               }

               ps.AllowInput(iInputLength,bHideInput,true, iInputMinLen);	// Tar 230459
           }
           else
               ps.AllowInput(iInputLength,bHideInput,false, iInputMinLen);	// Tar 230459
       }
    }
	
    //POS37397 - send the cYes button to TB that would cause the F1 key to be sent to the POS for the DF_ID=1577
    if( (sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MobileShopperOrderInProgress")) )
    {
        trace(L6, _T("Re-resume prompt - auto press cYES button"));
        return PSButton1();
    }

	// We need to enable the default button handling for RAP. To do this we will check if there
	// is a default button specified in the data entry view information and if so - we will go
	// ahead and press the button instead of showing the frame. This should only be done when going
	// into assist mode on the RAP.
	
	int iBtn = ps.RemoteMode() ? sm_pDataNeededEntry.GetDefaultButton() : -1;

	if( ps.RemoteMode())
    {
       	rp.SetInterventionTerminalNumber(_T("RAP"));
		rp.SetInterventionEndInProgress(true);
		ra.SetIsRemoteApprovalFlag(true);
        // +SOTF6416
        rp.SetOperator(
            ra.getOperatorID(
                ra.GetRemoteConnectionName()
                )
            );
        // -SOTF6416
    }

   //POS11024 Mobile Shopper Audit   
   if( (sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MobileAudit")) ||
	    (sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MobileShopperEasDeact")) ||
       (sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MobileAuditItemAdded")))
   {
      m_pAppPSX->SetConfigProperty( _T("MobileList"), _T("SmDataEntry"), UI::PROPERTYVISIBLE, VARIANT_TRUE );
      m_pAppPSX->SetConfigProperty( _T("SMReceipt"), _T("SmDataEntry"), UI::PROPERTYVISIBLE, VARIANT_FALSE );
      TBShowDisplay(true, true);
   }
   else
   {
      //clear the mobile receipt list to display regular smdataentry screen
      m_pAppPSX->ClearReceipt(_T("MobileList"));
   }

	switch(iBtn)
	{
	case 0:	return PSButton1();
	case 1:	return PSButton2();
	case 2:	return PSButton3();
	case 3:	return PSButton4();
	case 4:	return PSButton5();
	case 5:	return PSButton6();
	case 6:	return PSButton7();
	case 7:	return PSButton8();
	default:
		// EndCodeLocale
		if(!bEASApproveFromVoid)	//CRD 185620 - do not show screen if approving EAS intervention from void
		{
			ps.ShowFrame();
		}
	
		if (!sm_pDataNeededEntry.GetDefaultCommandString().IsEmpty())
		{
			return PSCmdButton(_T(""), _T(""), sm_pDataNeededEntry.GetDefaultCommandString());
		}
		return STATE_NULL;                // stay in SmDataEntry state
	};
}

void SMSmDataEntry::UnInitialize()
{
	//SSCOADK-493
	if (!g_bTBNeedMoreData)
		setAnchorState(nlaststate); 
	
	/* CRD 185620
	if (m_csCurrentIntervention != NULL)
	{
		QueueDelayedIntervention((CQueuedActionGenericDelayedIntervention *)m_csCurrentIntervention);
		m_csCurrentIntervention = NULL;
		m_GenericDelayedInterventionStatus = GENERIC_DELAYED_STARTING;
	}
	*/
	COleVariant vValue = VARIANT_FALSE; // TAR 350774
	
	if(co.IsInAssistMenus() && m_sView == _T("SMDataEntryPLAValidation"))
	{
		co.SetInAssistMenus(false);
	}
	
	ps.PSHideSMMNumericKeypad();
	
	if(sm_pDataNeededEntry.IsScannerInputEnabled())
	{
		DMScannerDisable();
	}
	
#endif //_CPPUNIT
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmDataEntry") );	// RFC 330050
	}
	
#ifndef _CPPUNIT
	m_pAppPSX->SetConfigProperty(_T("DetailsList"), csCurContext, UI::PROPERTYVISIBLE, vValue); // TAR 350774
	DMCheckCashCounts(false);	// TAR 350774 Restore media status.
	
	trace (L2, _T("SMSmDataEntryBase restoring bEASReEnableScanner to %d"),bEASReEnableScanner);
	
	//Clear CmdBtn and CmdList data
	COleVariant v = (long) 0;
	m_pAppPSX->ClearReceipt(_T("DataNeededReceipt"));
	m_pAppPSX->SetConfigProperty( _T("CmdBtn_DataEntryList"), csCurContext, UI::PROPERTYBUTTONCOUNT, v );
	m_pAppPSX->SetControlProperty(_T("CmdBtn_DataEntryList"), UI::PROPERTYBUTTONCOUNT, v );
	if(sm_pDataNeededEntry.IsMSRInputEnabled()) 
	{
		DMMSREject(); 
		DMMSRDisable();
	}
	
	// +SR93.4
	if(sm_pDataNeededEntry.IsScannerInputEnabled())
	{
		m_pAppPSX->SetTransactionVariable( _T("DataNeededScannerInputEnabled"), VARIANT_FALSE );
	}
  // -SR93.4
    //SMSmDataEntryBase::UnInitialize();
}
#endif

bool SMSmDataEntry::PSRemoteLegalForState()
{
    return SMSmDataEntryBase::PSRemoteLegalForState();
}

bool SMSmDataEntry::storeMode()
{
    return SMSmDataEntryBase::storeMode();
}

SMStateBase * SMSmDataEntry::PSButton1(void)
{
    return SMSmDataEntryBase::PSButton1();
}

SMStateBase * SMSmDataEntry::PSButton2(void)
{
    return SMSmDataEntryBase::PSButton2();
}

SMStateBase * SMSmDataEntry::PSButton3(void)
{
    return SMSmDataEntryBase::PSButton3();
}

SMStateBase * SMSmDataEntry::PSButton4(void)
{
    return SMSmDataEntryBase::PSButton4();
}

SMStateBase * SMSmDataEntry::PSButton5(void)
{
    return SMSmDataEntryBase::PSButton5();
}

SMStateBase * SMSmDataEntry::PSButton6(void)
{
    return SMSmDataEntryBase::PSButton6();
}

SMStateBase * SMSmDataEntry::PSButton7(void)
{
    return SMSmDataEntryBase::PSButton7();
}

SMStateBase * SMSmDataEntry::PSButton8(void)
{  
	if(m_sView == _T("AssistedEndorsement"))
	{
		SendTBCommand(TBCMD_SET_TB_STATE,_T("TENDER_PAYMENT_ENDORSEMENT_IP"));
		return createAnchorState();
	}
    else if( m_sView == _T("PricePrompt") && (co.IsAttendantModeOn() || co.IsInAssistMenus()) )
    {
        // TAR 361572: if go back is pressed in attendant mode Price Prompt
        // it should act like Cancel Request which is PSButton1()
        return PSButton1();
    }
	else
    {
        return SMSmDataEntryBase::PSButton8();
    }
}

SMStateBase * SMSmDataEntry::PSNumericKey(const long lKey)
{  
	if( m_View==DATANEEDED_PRICEPROMPT )
    {
        // TAR 424697: do not accept 0 price for price required item
        CString csEntryData;
        int nEntryDataLength = ps.GetInput(csEntryData);
        if( nEntryDataLength==0 && lKey==48 )
        {
            return STATE_NULL;
        }
    }

    return SMSmDataEntryBase::PSNumericKey(lKey);
}

SMStateBase * SMSmDataEntry::PSEnterKey(void)
{
    return SMSmDataEntryBase::PSEnterKey();
}

SMStateBase * SMSmDataEntry::PSClearKey(void)
{
    return SMSmDataEntryBase::PSClearKey();
}

SMStateBase * SMSmDataEntry::PSDecimalKey(const long ch)
{
    return SMSmDataEntryBase::PSDecimalKey(ch);
}

SMStateBase * SMSmDataEntry::PSBackSpaceKey(void)
{
    return SMSmDataEntryBase::PSBackSpaceKey();
}

SMStateBase * SMSmDataEntry::PSSpaceKey(void)
{
    return SMSmDataEntryBase::PSSpaceKey();
}

SMStateBase * SMSmDataEntry::PSShiftKey(void)
{
    return SMSmDataEntryBase::PSShiftKey();
}

SMStateBase * SMSmDataEntry::PSAlphaKey(const long lKey)
{
    return SMSmDataEntryBase::PSAlphaKey(lKey);
}

SMStateBase * SMSmDataEntry::PSAlphaNumericKey(const long lKey)
{
    return SMSmDataEntryBase::PSAlphaNumericKey(lKey);
}

SMStateBase * SMSmDataEntry::DMScanner()
{
	if(m_sView == _T("NegativeMedia"))
	{
				//set these two variables so that the operator
		//can continue to scan items or bottle slips
		//while on the smsmdataentry screen
		g_bTBNeedMoreData = true;
		m_View = DATANEEDED_MOREDATA;
		
		//CString csPropName = _T("ScanNegMediaState");
		//CString csPropValue = _T("1");
		//SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csPropName, &csPropValue);
		
        // Extract item code from scanner data.
		int iFind;
		CString csItemCode = cDMLastData;
		
		iFind=csItemCode.Find((_TCHAR)DM_FS);
		ASSERT (iFind != -1);
		csItemCode = csItemCode.Mid(iFind+1);
		ASSERT (csItemCode.GetLength()>0);
		iFind=csItemCode.Find((_TCHAR)DM_FS);
		ASSERT (iFind != -1);
		csItemCode = csItemCode.Left(iFind);
		
        // pos 11273 - remove this logic so that the scanned data will be sent as scanner data to POS - rrs
		//m_sInput = csItemCode;
        // e pos 11273 - remove this logic so that the scanned data will be sent as scanner data to POS - rrs
        
        // TAR 354932: Negative media item sales in SMDataEntry are 
        // bypassing the standard TBItemSale and this is preventing the 
        // proper initialization of the current item object before
        // item sale causing bad data to be present during processing
        // the item sold. Let's reset the object here:
        io.Reset();
        
        // pos 11273 - remove this logic so that the scanned data will be sent as scanner data to POS - rrs
		//return HandleDataNeeded();       
        // e - pos 11273 - remove this logic so that the scanned data will be sent as scanner data to POS - rrs
	}

	//CRD 313027
	if(m_sView == _T("MiscanPrompt") || m_sView == _T("ProductSwitchPrompt") || m_sView == _T("BasketLossPrompt"))
	{
	   //We need to send the barcode data as is so that TB can send it as scan data, not keyed data
		g_bTBNeedMoreData = true;
		m_View = DATANEEDED_MOREDATA;

		io.Reset();

		m_sInput = cDMLastData;
		USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	}

	
    return SMSmDataEntryBase::DMScanner();
}

SMStateBase * SMSmDataEntry::DMScannerEncrypted()
{
    return SMSmDataEntryBase::DMScannerEncrypted();
}

SMStateBase * SMSmDataEntry::DMCardReader(void)
{
    return SMSmDataEntryBase::DMCardReader();
}

SMStateBase * SMSmDataEntry::OnTBNeedData(void)
{
    return SMSmDataEntryBase::OnTBNeedData();
}

SMStateBase * SMSmDataEntry::HandleRAPData(void)
{
    trace( L6, _T("SMSmDataEntry::HandleRAPData()"));

    SMStateBase* retState = STATE_NULL;
    // start CDR126194
    bool bNeedClear = true;
    CString csDataHandler = g_csDataNeededHandler;
    g_csDataNeededHandler = _T("");
    if (m_sButton == "cMobileCartAudit")
    {
        trace(L6,_T("SMStateBase::OnRAPUnsolicitedDataReceived() Starting remote AM."));
        mobileAudit.SendStartAuditRequest();
        //SAClearSecurityCondition();
        RETURNSTATE(SmMobileAudit);
    }
    if (m_sButton == _T("cClearMobileCartAudit"))
    {
        bNeedClear = true;
    }
   // end CDR126194

	SMStateBase *pNextState = STATE_NULL;

    switch (m_View)
    {
        case DATANEEDED_ASSISTMODE:
        {   
			//ra.RequestEnableAssistMode(false);
			ra.OnNoRAPDataNeeded();

            long lConnMode = ra.GetConnectionMode();
            CString csConnection = ra.GetRemoteConnectionName();
            bool bIsHandHeld = ra.IsRemoteConnectionHandheld(csConnection);
            if( lConnMode == RA_CONNECTION_MODE_DATA_NEEDED ) // need assist mode so send msg.
            {
                m_pAppPSX->GenerateEvent(_T(""), _T(""), _T("RemoteData"), _T("operation=assist-acquire;"), csConnection);
            }

			if( bIsHandHeld && ps.RemoteMode() && co.fStateAssistMenuFromRAPAllowed )
			{
				m_bNeedAssistMode = true;
				RETURNSTATE(SmAssistMenu);
			}
			RETURNSTATE( SmAssistMode )
			break;
        }

		//+TAR 357550
		case DATANEEDED_PRICEPROMPT:
			{
				g_bTBNeedMoreData = true;
				pNextState = SMStateBase::HandleRAPData();
				break;
			}
		//-TAR 357550

        default:
        {
			/*POS11024:Mobile Shopper: Reset prompt for EAS scan validation needed*/
         if(m_sView == _T("MobileShopperEasDeact"))
         {
            SendTBCommand(TBCMD_SET_TBPROPERTY, &CString(TBPROP_RAPDATANEEDED_CUSTENTRY), &CString(_T("")));
            trace(L6, _T("SMSmDataEntry::HandleRAPData(), Reset Scan Validation prompt"));
         }
         if(m_sView == _T("NegativeMedia"))
			{
				//set these two variables so that the operator
				//can continue to scan items or bottle slips
				//while on the smsmdataentry screen
                //SOTF 8105  these variables should not be
                //set if we are remoted and the operator
                //presses the red lane button
                if(!ps.RemoteMode())
                {
				    g_bTBNeedMoreData = true;
				    m_View = DATANEEDED_MOREDATA;
                }
			}

            // SOTF 8673-9067 - RRS
            // this is a workaround to just display the SmAssistMode if the AssistMode button on the rap is pressed
            if(g_bTBNeedMoreData)
            {
                trace(L6, _T("SMSmDataEntry::HandleRAPData(), Handling AssistMode"));

                CString pDetails;
                pDetails = m_sButton;
                if( pDetails.Find(_T("cAssistMode")) >= 0 )  // "command" parameter for the button pressed
                {
                    ra.OnNoRAPDataNeeded();

                    long lConnMode = ra.GetConnectionMode();
                    CString csConnection = ra.GetRemoteConnectionName();
                    bool bIsHandHeld = ra.IsRemoteConnectionHandheld(csConnection);
                    if( lConnMode == RA_CONNECTION_MODE_DATA_NEEDED ) // need assist mode so send msg.
                    {
                        m_pAppPSX->GenerateEvent(_T(""), _T(""), _T("RemoteData"), _T("operation=assist-acquire;"), csConnection);
                    }

			        if( bIsHandHeld && ps.RemoteMode() && co.fStateAssistMenuFromRAPAllowed )
			        {
				        m_bNeedAssistMode = true;
				        RETURNSTATE(SmAssistMenu);
			        }
			        RETURNSTATE( SmAssistMode )
			        break;
                }
                // POS16600 - rrs
                else if(m_sView == _T("TakeChangeTimeout") )
                {
                    // as we have called the OnNoRAPDataNeeded when it displays the TakeChangeTimeOut, we need to reset
                    // the m_View to DATANEEDED_MOREDATA so that the attendant could press the OK button on the lane
                    // and the flow will move on
                    m_View = DATANEEDED_MOREDATA;
                    trace(L2, _T("TakeChangeTimeout OK Button, mView(%s)"), m_View);                    
                }
                // e POS16600 - rrs*/
                //POS65108/POS6300 rrs
                else if( ps.RemoteMode() && GetHookProperty(_T("__POSState")).Find(_T("CELL_PHONE_2_STEP")) != -1 )
                {
                    trace(L6, _T("SMRAPDataNeeded::HandleRAPData() - Handle cellphone activation on AM"));

                    // POS64823 - get an exclusive access for the Remote RAP so that the scanner data can be retrieved
                    ra.OnNoRAPDataNeeded();

                    long lConnMode = ra.GetConnectionMode();
                    CString csConnection = ra.GetRemoteConnectionName();
                    bool bIsHandHeld = ra.IsRemoteConnectionHandheld(csConnection);
                    if( lConnMode == RA_CONNECTION_MODE_DATA_NEEDED ) // need assist mode so send msg.
                    {
                        m_pAppPSX->GenerateEvent(_T(""), _T(""), _T("RemoteData"), _T("operation=assist-acquire;"), csConnection);
                    }			        
                    // e - POS64823 - get an exclusive access for the Remote RAP

                    RETURNSTATE( SmAssistMode )
                }
				//Start CRD 321191
				else if(m_sButton == _T("cCancel") && (m_sView == _T("MiscanPrompt") || m_sView == _T("ProductSwitchPrompt") || m_sView == _T("BasketLossPrompt")))
				{
					promptForOperator();
					return STATE_NULL;
				}
				//End CRD 321191
				//Start CRD 344902
				else if(m_sView == _T("PLUEntry"))
				{
					io.csItemCode = m_sInput;
					io.csOrigCode = m_sInput;
				}
				//End CRD 344902
				
                // e POS65108/POS6300 rrs
            }                
            // E - SOTF 8673-9067 - RRS

            pNextState = SMStateBase::HandleRAPData();
        }
    }
   // start CDR126194
    if (bNeedClear)
    {
        SAClearSecurityCondition();
    }
	// end CDR126194

	if (!fSAInTransaction && (csDataHandler.Find(_T("MobileError")) != -1 )) 
    {
        rp.SetInterventionEndInProgress(true);
    }

    return pNextState;
}

SMStateBase * SMSmDataEntry::PSReceiptUp(void)
{
    return SMSmDataEntryBase::PSReceiptUp();
}

SMStateBase * SMSmDataEntry::PSReceiptDown(void)
{
    return SMSmDataEntryBase::PSReceiptDown();
}

void SMSmDataEntry::DisplayUserButtons(void)
{
    SMSmDataEntryBase::DisplayUserButtons();
}

void SMSmDataEntry::SetUserButton(int nIndex, int nPageOffset)
{
    SMSmDataEntryBase::SetUserButton(nIndex, nPageOffset);
}

CString SMSmDataEntry::GetPagedButtonCommand(int index)
{
    return SMSmDataEntryBase::GetPagedButtonCommand(index);
}

CString SMSmDataEntry::GetButtonCommand(int nButtonIndex)
{
    return SMSmDataEntryBase::GetButtonCommand(nButtonIndex);
}

//+SSCOI-31860
SMStateBase * SMSmDataEntry::PSOnRemoteDisconnected(const long lParm)
{
    trace(L6, _T("SSCOI-31860 m_sView = %s SMState::m_bTakeChangeTimerExpired = %d !!!!"), m_sView, m_bTakeChangeTimerExpired);
	if (m_sView == _T("TakeChangeTimeout"))
    {
        SMState::m_bTakeChangeTimerExpired = true;
        trace(L6, _T("SSCOI-31860 value setting to true SMState::m_bTakeChangeTimerExpired = true"));
    }
	return SMSmDataEntryBase::PSOnRemoteDisconnected(lParm);
}
//-SSCOI-31860

#ifndef _TRADESHOWDEMO
SMStateBase * SMSmDataEntry::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
	//Start CRD 185620 should trigger to go to finish and pay so that the operator message shows up
	if(csCommand == _T("CmdBtn_HandleEASIntervention") || csCommand == _T("CmdBtn_HandleMobileGenericDelayedIntervention")) 
    {
		bGenericDelayedFinishSent = true;	//CRD 232271
        setAnchorState(BES_FINISH);

		SendTBCommand(TBCMD_ENTERING_FINISH_AND_PAY, false);
        
		CREATE_AND_DISPENSE(InProgress)(0,
			helpMode(),
			co.getTimeOutTender());

		return STATE_NULL;
    }
	//End CRD 185620
	
    return SMSmDataEntryBase::PSCmdButton(csContext, csControl, csCommand);
}
#endif

SMStateBase * SMSmDataEntry::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    return SMSmDataEntryBase::OnRAPUnsolicitedDataReceived(sData);
}

void SMSmDataEntry::PrintEndorsement(void)
{
	trace (L6, _T("+SMSmDataEntry::PrintEndorsement()"));
	long nEndorsementsPending=0;
	
	do{
        LPTSTR szSlipDocumentDescription;
        LPTSTR szSlipDocumentFront;
        LPTSTR szSlipDocumentBack;
		
		
		szSlipDocumentDescription = NULL;
		szSlipDocumentFront = NULL;
		szSlipDocumentBack = NULL;
		
		TBGetDocEndorsementInfo(&nEndorsementsPending,
			&szSlipDocumentDescription,  &szSlipDocumentFront, &szSlipDocumentBack);
		
        if(nEndorsementsPending)
		{
			trace (L0, _T("SMState::OnEndorseDocument Printing endorsement data to a chit"));
			PrintEndorsementChit(szSlipDocumentBack);
			// Suppress further endorsement message from TB since we are doing these
			// in a loop and don't need to be event driven at this time.
			TBEndorsementCompleted(true);
		}
        delete szSlipDocumentDescription;
		delete szSlipDocumentFront;
		delete szSlipDocumentBack;
	   }while (nEndorsementsPending);
	
	trace (L6, _T("-SMSmDataEntry::PrintEndorsement()"));
}

SMStateBase *SMSmDataEntry::TBItemDetails(void)
{
	
    SMStateBase *retState = SMState::TBItemDetails();
	
    if(m_sView == _T("NegativeMedia"))
	{
		
		delete getRealStatePointer( retState); 
		return STATE_NULL;
    }
	
    return retState;
}

///////////////////////////////////////////////////////////
//POS11024 Mobile Shopper START
///////////////////////////////////////////////////////////
SMStateBase *SMSmDataEntry::TBParse(MessageElement *me)
{
	if ( me->nDeviceID == TB_DEV_DISPLAYPRIMARY ) 
	{
		if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MobileAudit") )
		{
			m_pAppPSX->SetConfigProperty( _T("MobileList"), _T("SmDataEntry"), UI::PROPERTYVISIBLE, VARIANT_TRUE );
			m_pAppPSX->SetConfigProperty( _T("SMReceipt"), _T("SmDataEntry"), UI::PROPERTYVISIBLE, VARIANT_FALSE );
			TBShowDisplay(true, true);
		}
	}
	return SMSmDataEntryBase::TBParse(me);
}

void SMSmDataEntry::TBShowDisplay(bool bRefresh, bool bRepaintLastScreen)
{
   COleVariant vValue;

   m_POSDisplay.CreateInstance(__uuidof( MSXML2::DOMDocument40) );
   m_POSDisplay->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load

   CString strDisplay;
   SendTBCommand(TBCMD_GETDISPLAYXML, &strDisplay);
   if( m_POSDisplay->loadXML( (_bstr_t) strDisplay ) != VARIANT_TRUE )
	{
	   trace(L1, _T("ERROR: Load display xml failed"));
   } 

   //START Enable or disable buttons based on if POS skey is enabled or not
   CString csLabel, csEnabled, csText, csGrayed, csIcon;
	COleVariant vReturn, v;
	long nButtonCount = 8; 

	COleSafeArray saTextArray, saIconArray, saStateArray;
	COleSafeArray saDataArray;

	saIconArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	saStateArray.Create(VT_I4, 1, (DWORD*)&nButtonCount);
	
	v = saIconArray;

	//save button data list before blasting buttons
   m_pAppPSX->SendCommand( _T("Key24"), UI::COMMANDGETBUTTONDATALIST, vReturn);

	if (vReturn.vt == (VT_ARRAY | VT_VARIANT))
	{
		saDataArray.Attach(vReturn);
	}
 
	long nKeyOffset;
   CString csButton;
	for (int nKey=1; nKey <= nButtonCount; nKey++)
	{
		nKeyOffset=nKey-1;  
		
		csLabel.Format(_T("DK%d_Label"),nKey);
		csEnabled.Format(_T("DK%d_Enabled"), nKey);
		csGrayed.Format(_T("DK%d_Grayed"), nKey);
		csIcon.Format(_T("DK%d_Icon"), nKey);
		
		csText = GetDisplayText(csLabel);  //Dynakey1
		if (csText == _T(""))
		{
			csLabel.Format(_T("DK%d_Label_Alt"), nKey);
			csText = GetDisplayText(csLabel);
			if (csText == _T("")) 
				csText = _T(" ");
		}
      BSTR bstrTemp;

#ifndef UNICODE
      bstrTemp = SysAllocString( T2OLE( csText ) );
#else 
		bstrTemp = SysAllocString( csText );
#endif
		saTextArray.PutElement(&nKeyOffset, bstrTemp );
      SysFreeString( bstrTemp );

	   _variant_t vEnabled = GetFieldValue(csEnabled);  
		_variant_t vGrayed = GetFieldValue(csGrayed);
		csIcon = GetDisplayText(csIcon);
	   csIcon.Replace(_T("|BITMAP|:"),_T("C:\\scot\\Image\\ACSIcons\\"));
		csIcon.Replace(_T(".bmp"),_T(".ico"));
		
#ifndef UNICODE
		bstrTemp = SysAllocString( T2OLE( csIcon ) );
#else
      bstrTemp = SysAllocString( csIcon );
#endif
		saIconArray.PutElement(&nKeyOffset, bstrTemp );
      SysFreeString( bstrTemp );

		long lButtonState;
		if (vEnabled.vt != VT_NULL && vEnabled.boolVal)
			lButtonState = (long) UI::Normal; 
		else if ((vGrayed.vt != VT_NULL) && (vGrayed.intVal == 0 || vGrayed.intVal == 2))
		{
			if (csIcon.Find(_T("checkon")) != -1)
				lButtonState = (long) UI::Highlighted;
			else
				lButtonState = (long) UI::Normal;
		}
		else 
			lButtonState = (long) UI::Disabled;

		saStateArray.PutElement(&nKeyOffset, &lButtonState );
      csButton.Format(_T("SMButton%d"), nKey);
      
      if(lButtonState == (long)UI::Disabled)
      {
         ps.ButtonEnable(csButton, false);
      }
      else
      {
         // disable scanner if MobileAudit done
         ps.ButtonEnable(csButton, true);
         if(nKey == 1 &&
            sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MobileAudit"))
         {     
            SAWLDBScannerDisable();
         }
      }
	}
   //END Enable or disable buttons

   //POS11024: Set up screen data for EAS deactivation
   CString csEASDeactProp = _T("PropEASDeact");         //This property is set whenever DF_Build_rainck context is sent
   CString csEASFlg;
   SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csEASDeactProp, &csEASFlg);

   if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MobileShopperEasDeact") )
   {
       
      //Update leadthru text by concatenating LTT1 and LTT2
      ps.SetTitle(GetDisplayText(_T("Title1")));      
      CString csText = GetDisplayText(_T("LTT1"));
      CString csTextLTT2 = GetDisplayText(_T("LTT2"));
      if(csTextLTT2.GetLength())
      {
         csText = csText + csTextLTT2;        
      }     
      ps.SetLeadthruText(csText);
      if (csEASFlg == _T("1"))
      {
         //Only update the receipt for first iteration
	      SetHookProperty(_T("PropEASDeact"), _T("0"));    //clear here to avoid multiple entries into this logic     

	      m_pAppPSX->ClearReceipt( _T("PrepaidCardList") );        //POS11024:Clear this list before we populate

         CString csPrepaidCardProp = _T("PrepaidCardList");
         CString csPrepaidCard;
         SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPrepaidCardProp, &csPrepaidCard);

         //clear the list so that it does not accumulate next time
         SetHookProperty(_T("PrepaidCardList"), _T(""));

	      m_pAppPSX->ClearReceipt( _T("MobileList") );         

         //csPrepaidCard may have multiple EAS line item joined by "\t". Split them here
         bool bDoesEASListItemExist = true;
         CString csEASListItemDesc, csReceiptID;
         int iToken=0;
         int iReceiptID=0;
     
         while( bDoesEASListItemExist )
         {
            iToken = csPrepaidCard.Find(_T("\t"));
            if( iToken > -1 )
            {
                // extract the description for EAS line item
                csEASListItemDesc = csPrepaidCard.Left(iToken);
                csPrepaidCard = csPrepaidCard.Mid(iToken+1);

                // create the receipt item entry id
                csReceiptID.Format(_T("Item%d"), iReceiptID);
                vValue = csEASListItemDesc;
                iReceiptID++;

                m_pAppPSX->CreateReceiptItem( _T("MobileList"), csReceiptID );
                m_pAppPSX->SetReceiptItemVariable( _T("MobileList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
            }
            else
            {
                csReceiptID.Format(_T("item%d"), iReceiptID);
                vValue = csPrepaidCard;
          
                m_pAppPSX->CreateReceiptItem( _T("MobileList"), csReceiptID );
                m_pAppPSX->SetReceiptItemVariable( _T("MobileList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                bDoesEASListItemExist = false;
            }
         }     
         // update the receipt control
         m_pAppPSX->UpdateReceiptControls( _T("MobileList") );
      }
   }
     
   //Set title, leadthru, and receipt line for MobileAudit   
   if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MobileAudit") ) 
	   
   {     
      ps.SetTitle(GetDisplayText(_T("Title1")));
      //POS29338 set second line blank
      ps.SetLeadthruText(GetDisplayText(_T("Title2")), _T(""));
      vValue = GetDisplayText(_T("LTT2"));

      m_pAppPSX->ClearReceipt( _T("MobileList") );
      m_pAppPSX->CreateReceiptItem(_T("MobileList"), _T("Item0"));
      m_pAppPSX->SetReceiptItemVariable(_T("MobileList"), _T("Item0"), _T("ITEM_DESCRIPTION"),vValue);

      // update the receipt control
      m_pAppPSX->UpdateReceiptControls( _T("MobileList") );
   }   

   //Set title, leadthru, and receipt line for MobileAuditItemAdded
   if(sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && sm_pDataNeededEntry.GetSectionName() == _T("MobileAuditItemAdded"))
   {
      ps.SetTitle(GetDisplayText(_T("Title1")));
      ps.SetLeadthruText(GetDisplayText(_T("OperGuid_Txt")));
      m_pAppPSX->CreateReceiptItem(_T("MobileList"), _T("Item0"));
      m_pAppPSX->UpdateReceiptControls( _T("MobileList") );
   }
   
   ps.ShowFrame();
}

CString SMSmDataEntry::GetDisplayText(CString csFieldName)
{
	_variant_t vFieldValue = GetFieldValue(csFieldName);
	if (vFieldValue.vt == VT_NULL)
		return _T("");
	else
		return vFieldValue.bstrVal;
}

_variant_t  SMSmDataEntry::GetFieldValue(CString csFieldName)
{
	CString csPath;
	csPath.Format(_T("message/fields/field[@name = \"%s\"]"), csFieldName); 
	_bstr_t test = _bstr_t(csPath);
	
	MSXML2::IXMLDOMNodePtr testnode = m_POSDisplay->selectSingleNode(_bstr_t(csPath));
	
	_variant_t nodeValue = _T("");
    nodeValue.vt = VT_NULL;
	if (testnode != NULL)
	{
		_variant_t nodeValue=testnode->GetnodeTypedValue();
		return nodeValue;
	}
	else
	{
		return nodeValue;
	}
} 
//POS11024 Mobile Shopper END

//Start CRD 321191
SMStateBase *SMSmDataEntry::stateAfterPasswordVerify()
{
	trace(L2, _T("+SMSmDataEntry::stateAfterPasswordVerify"));
	SendTBCommand(TBCMD_NOTIFY_ES_ALERT_CANCELED);
	RETURNSTATE( SmAssistMode );
}

SMStateBase  *SMSmDataEntry::PSButtonGoBack(void) // cancel all
{
    if(inputMode == PROMPTING_FOR_PASSWORD)
    {  
        promptForOperator();
		return STATE_NULL;
    }
	RETURNSTATE(SmDataEntry);
}
//Start CRD 321191