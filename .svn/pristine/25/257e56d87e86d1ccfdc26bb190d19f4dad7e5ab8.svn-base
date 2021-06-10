//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmDataEntryBase.CPP
//
// TITLE: Class implementation for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state 
#else
#include "FakeSMOperatorPasswordStateBase.h"
#include "SMStateConstants.h"
#include "FakeWindowCalls.h"
#endif //_CPPUNIT

               
#include "SMSmDataEntry.h"    // This state

#ifndef _CPPUNIT
#include "SMSmVoidedItems.h"        // Voided items auth
#include "SMSmAuthorization.h"      // Auth state
#include "SMSmEnterBirthdate.h"     // 
#include "SMCmDataEntry.h"
#include "PSConfigData.h"
#include "CmFunctions.h" // SR93.4
#include "SMBagAndEAS.h"

#define HANDHELD_NUMBUTTONS 4 //Number of pageable user buttons (excluding SMButton8)
#define DESKTOP_NUMBUTTONS 7 //Number of pageable user buttons (excluding SMButton8)
#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmDataEntryBase")
//DLLEXPORT extern CSCOTDataEntry cDataEntry;


IMPLEMENT_DYNCREATE(SMSmDataEntryBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmDataEntry)


DEFINE_TIMESTAMP


int  SMSmDataEntryBase::nlaststate = -1;
#endif //_CPPUNIT
bool SMSmDataEntryBase::m_bPickListAssistHandler = false; //SSCOADK-1000
#ifndef _CPPUNIT
bool SMSmDataEntryBase::ms_bRAPHasScanned = false; //SSCOP-3803
#endif //_CPPUNIT


// NewUI removed PSInterface

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmDataEntryBase::SMSmDataEntryBase()
: csCurContext(EMPTY_STRING),
  m_HKStatus(IPSProcedures::HK_NORMAL),
  m_bShiftOn(false),
  m_bAltGrOn(false)
{
#ifndef _CPPUNIT
  IMPLEMENT_TIMESTAMP
#endif //_CPPUNIT

}



//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::Initialize(void)
{
#ifndef _CPPUNIT
    ms_bRAPHasScanned = false; //SSCOP-3803
    int iFind, iInputLength;
    CString csSayPhrase;
    //+SSCOP-752
	if(!ps.RemoteMode())
	{
	  ra.RequestEnableAssistMode(false);
	}//-SSCOP-752
#endif //_CPPUNIT
	bool bSecureCamHandler = false; 
	if(!fInMultiSelectPickList)//SR679
	{
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmDataEntry") ); //382138
	}

    if (m_View == DATANEEDED_MOREDATA)
    {
        if (!g_csDataNeededHandler.IsEmpty() && g_csDataNeededHandler == _T("SecureCam"))
            bSecureCamHandler = true;
        else
            m_bPickListAssistHandler = true;
    }

#ifndef _CPPUNIT
    if(getAnchorState() != BES_SMDATAENTRY) // save off for possible cancel later
        nlaststate = getAnchorState();
    
    m_nBtnPageIndex = 0;
    m_bPageButtons = false;
    inputMode = OPERATOR_PASSWORD_ACCEPTED;
	// Tar 210056 RF 071902 - initialize input fields
    m_sButton.Empty();
    m_sInput.Empty();

	//cDataEntry.FromConfigData(csDataEntryINIFile, m_sView);
#endif _CPPUNIT
    CString csInputString = sm_pDataNeededEntry.GetInputMask();
    CString csDetails = sm_pDataNeededEntry.GetDetailedInstruction();	// RFC 211581 RJF 081602

    COleVariant vValue = VARIANT_FALSE;

#ifndef _CPPUNIT
	if (dm.IsOpen()) //tar 352514
	{
        // (*) SR898 ====================
		DMTurnOffTriColorLight();
        
		// (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                           DM_TRICOLORLIGHT_ON,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
	}
#endif _CPPUNIT
	//RFQ 986
	CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay(); 

    m_bShiftOn = false;
    m_bAltGrOn = false; //SR828
    m_HKStatus = IPSProcedures::HK_NORMAL; // SR828
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

        if (csCurContext.Find(_T( "SmDataEntryAlphanumeric")) != -1)
        {
            csCurContext = ps.AppendContextWithLCID(csCurContext);
        }
        //RFQ 2506 - If in Assist menus,  show store-mode version of specified context
		if (!sm_pDataNeededEntry.IsStoreLogonEnabled() && co.IsInAssistMenus())
		{
			ps.SetCMFrame(csCurContext);  //Show "AM_" contexts if in Basic Assist showing customermode screen
            csCurContext = ps.GetCurrentContext(); //set to the context name used by PSProcedures
		}
		else
		{
			ps.SetFrame(csCurContext);
		}
	}									//end RFQ 986
#ifndef _CPPUNIT
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
            csCurContext = _T("SmDataEntryWithDetailsList");  // Tar 238987
            ps.SetFrame(csCurContext);
            m_pAppPSX->SetConfigProperty(_T("DetailsList"), csCurContext, UI::PROPERTYVISIBLE, vValue);
        }
    }
    else
	    ps.SetFrame(_T("SmDataEntryWithListBG"));
    ps.Echo(PS_BLANK);
    ps.SetTitle(sm_pDataNeededEntry.GetTopCaption());
	if(csCurContext.Find(_T( "SmDataEntryAlphanumeric")) != -1)
	{
        m_pAppPSX->SetConfigProperty(_T("InstructionScreenTitle"), csCurContext, UI::PROPERTYTEXT, sm_pDataNeededEntry.GetTopCaption());
        m_pAppPSX->SetConfigProperty(_T("Instruction_Line1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), csCurContext, UI::PROPERTYTEXT, sm_pDataNeededEntry.GetSummaryInstruction());


        SetAlphaShiftState(csCurContext, m_bShiftOn);

        ps.PopulateHotKeys(csCurContext);
        ps.PopulateHKSensitiveControls(csCurContext, m_HKStatus, m_bShiftOn, m_bAltGrOn);
        if (m_bCapsInput)
        {
            SetAlphaShiftState(csCurContext, m_bCapsInput);
        }
	}

    // TAR 228468 - Check for duplicate text messages.
    CString csSummary = sm_pDataNeededEntry.GetSummaryInstruction();
    
    if(sm_pDataNeededEntry.GetViewType() == _T("Alpha"))
    {
        ps.SetCMLeadthruText(sm_pDataNeededEntry.GetTopCaption());
        ps.Message12(sm_pDataNeededEntry.GetSummaryInstruction(), false);
    }
    else
    {
	    if (csSummary == m_sDesc || ps.GetPSText(MSG_TM_TRANSACTION, SCOT_LANGUAGE_PRIMARY) == m_sDesc)
        {
            ps.SetLeadthruText(csSummary, _T(""));
        }
        else if (!ps.RemoteMode() && (g_csSecureCamInterventionType == _T("SMART ASSIST TRUST LEVEL")))
        {
            ps.SetLeadthruText(m_sDesc, _T("") );
        }
        else 
        {
	        ps.SetLeadthruText(sm_pDataNeededEntry.GetSummaryInstruction(), m_sDesc );    
        }
    }

	m_pAppPSX->SetTransactionVariable(_T("InstructionText"), csSummary); //TAR 400042
    ps.Prompt(sm_pDataNeededEntry.GetBottomCaption());
    if(!csDetails.IsEmpty())			// RFC 211581 RJF 081602
    {
        ps.ResetDetailsList();
	
		ps.AddDetailsList(csDetails);
		if (csContextToDisplay.IsEmpty()) 
		{
			csCurContext = _T("SmDataEntryWithDetailsList"); // Tar 238987
		}
		vValue = VARIANT_TRUE;
        m_pAppPSX->SetConfigProperty(_T("DetailsList"), csCurContext, UI::PROPERTYVISIBLE, vValue);
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
    if(!csResourcePath.IsEmpty())
    {
        vValue = csResourcePath;
        m_pAppPSX->SetConfigProperty(_T("DataEntryVideo"), csCurContext, UI::PROPERTYVIDEO, vValue);
        //m_pAppPSX->SetConfigProperty(_T("DataEntryVideo"), csCurContext, UI::PROPERTYVIDEO, vValue);
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

#endif //_CPPUNIT
    if(csCurContext == _T("SmDataEntryWithImageControl"))
    {
        //disabling Go Back button in lane

        if (!ps.RemoteMode())
        {
            ps.ButtonState(_T("SMButton8"),false,false);
        }
        if(m_csResourcePath.IsEmpty())
        {
            m_csResourcePath = co.csDataDefaultImagePath;
        }

        if(ra.IsRemoteConnectionHandheld(ra.GetRemoteConnectionName()))
        {
           
            ps.SetButtonText(_T("SMButton8"),BTT_GOBACK);
        }


        GET_PARTITION_PATH(m_csResourcePath, m_csResourcePath.GetBuffer(_MAX_PATH));
	    m_csResourcePath.ReleaseBuffer();
        vValue = m_csResourcePath;
        m_pAppPSX->SetConfigProperty(_T("DataEntryImage"), csCurContext, UI::PROPERTYPICTURE, vValue);
    }
#ifndef _CPPUNIT

    //TAR287018+
	//Let .ini file decide if the scanner is enabled
    //Use DMScannerEnable() and DMScannerDisable to avoid manipulating the bEASReEnableScanner flag.
    //This scanner enable/disable is outside of the normal Scan-to-EnterItem or Scan-to-Login flow.
    if(sm_pDataNeededEntry.IsScannerInputEnabled() && (!ps.RemoteMode())) //SSCOADK-5300 Don't enable local scanner for remote mode
    {
		DMScannerEnable();		  // enable the scanner
        m_pAppPSX->SetTransactionVariable( _T("DataNeededScannerInputEnabled"), VARIANT_TRUE );// SR93.4
	}
    else							// Tar 210186 RF 071902
    {
		DMScannerDisable();		// disable scanner if ini parameter set to 0 or not present
        m_pAppPSX->SetTransactionVariable( _T("DataNeededScannerInputEnabled"), VARIANT_FALSE );// SR93.4
    }
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

	if(1 == sm_pDataNeededEntry.GetScannerDataFormat())
	{
		vValue = VARIANT_FALSE;
		m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollUp"), csCurContext, UI::PROPERTYVISIBLE, vValue);
		m_pAppPSX->SetConfigProperty(_T("SMReceiptScrollDown"), csCurContext, UI::PROPERTYVISIBLE, vValue);
	}
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
    // EndCodeLocale
 // ps.ShowFrame();

  // TAR 264139: Do not play sound here
#if 0
  // Check if they have a specific sound file to play
    csSayPhrase = sm_pDataNeededEntry.GetSayPhrase();
    if(csSayPhrase.IsEmpty())
        DMSayPhrase(INFORMATIONNEEDED);
    else
        DMSayPhrase(csSayPhrase);
#endif

      if(fSAInStoreMode && bSecureCamHandler && !ps.RemoteMode() )
      {
         vValue = VARIANT_FALSE;
         ps.ButtonState(_T("SMButton8"),false,false);
         m_pAppPSX->SetConfigProperty(_T("SMButton8"), csCurContext, UI::PROPERTYVISIBLE, vValue);
      }
 // return STATE_NULL;                // stay in SmDataEntryBase state
  // We need to enable the default button handling for RAP. To do this we will check if there
	// is a default button specified in the data entry view information and if so - we will go
	// ahead and press the button instead of showing the frame. 
	
	if(m_GenericDelayedInterventionStatus && sm_pDataNeededEntry.GetButtonCommand(7) =="cGoBack" )
    {
        vValue = VARIANT_FALSE;
        m_pAppPSX->SetConfigProperty(_T("SMButton8"), csCurContext, UI::PROPERTYVISIBLE, vValue);
        trace(L4, _T("WARNING!! - cGoBack not allowed for Generic Delayed Interventions, button not configured!"));
    }

	int iBtn = ps.RemoteMode() ? sm_pDataNeededEntry.GetDefaultButton() : -1;

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
#endif //_CPPUNIT
		ps.ShowFrame();

    if (!sm_pDataNeededEntry.GetDefaultCommandString().IsEmpty())
    {
        return PSCmdButton(_T(""), _T(""), sm_pDataNeededEntry.GetDefaultCommandString());
    }



		return STATE_NULL;                // stay in SmDataEntry state
#ifndef _CPPUNIT
	
    };
#endif //_CPPUNIT

}

void SMSmDataEntryBase::UnInitialize(void)   //TAR 193566
{ 
#ifndef _CPPUNIT

 //SSCOADK-493
 if (!g_bTBNeedMoreData)
	  setAnchorState(nlaststate); 

  if (m_csCurrentIntervention != NULL)
  {
      QueueDelayedIntervention((CQueuedActionGenericDelayedIntervention *)m_csCurrentIntervention);
      m_csCurrentIntervention = NULL;
      m_GenericDelayedInterventionStatus = GENERIC_DELAYED_STARTING;
  }

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
#endif //_CPPUNIT
}

SMStateBase  *SMSmDataEntryBase::PSButton1(void)
{
trace (L2, _T("SMSmDataEntryBase::PSButton1"));
#ifndef _CPPUNIT
    if( fInValidOperatorPassword == true )
    {
        promptForOperator();  
        return STATE_NULL;
    }

	m_sButton = GetPagedButtonCommand(0);
	// 227275 change saved enable since the item was canceled LPM021103
#endif //_CPPUNIT	

    if (m_sButton == _T("cReturn")) {	bEASReEnableScanner = true; }

    else if ( co.IsPicklistAssistEnable() &&	//SSCOADK-1000
              m_bPickListAssistHandler &&
              m_sButton == _T("cApprove")) 
    {
        COleVariant vReturn;
        m_pAppPSX->SendCommand(UI::CONTROLDISPLAY, UI::COMMANDREMOVECACHEDIMAGE, vReturn, 1, COleVariant(m_csResourcePath));

        rp.m_bIsInterventionApproved = true; //report that intervention was approved
        ra.OnNoRAPDataNeeded();
        ClearSecurityViolationLight();
        
        RETURNSTATE(BagAndEAS);
    }
#ifndef _CPPUNIT
	//USSF START

	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
#endif //_CPPUNIT
}

SMStateBase  *SMSmDataEntryBase::PSButton2(void)
{
#ifndef _CPPUNIT
	m_sButton = GetPagedButtonCommand(1);
#endif //_CPPUNIT
    if ( co.IsPicklistAssistEnable() &&    //SSCOADK-1000
         m_bPickListAssistHandler &&
         m_sButton == _T("cVoid"))
    {
        COleVariant vReturn;
        m_pAppPSX->SendCommand(UI::CONTROLDISPLAY, UI::COMMANDREMOVECACHEDIMAGE, vReturn, 1, COleVariant(m_csResourcePath));

        rp.m_bIsInterventionApproved = false; //SR803 report that intervention was not approved
        ra.OnNoRAPDataNeeded();
        ClearSecurityViolationLight();

        g_bPicklistAssistVoid=true;
        lVoidTcd = io.lTareCode;
        lVoidWgt = io.lWeightEntered;

        //Send Skip Bagging message to SM when the item is voided
#ifndef _CPPUNIT      
        CKeyValueParmList parmList;
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
        _bstr_t bstrParmList((const _TCHAR *)parmList);
#else
        _bstr_t bstrParmList=_T("");
#endif //_CPPUNIT
        BSTR bstrResult;
        bstrResult = NULL;

        SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
        ::SysFreeString(bstrResult);

        return TBItemSale(true, lVoidTcd, m_bIsVoidByScanning, true);
    }

#ifndef _CPPUNIT
	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
#endif //_CPPUNIT
}
#ifndef _CPPUNIT

SMStateBase  *SMSmDataEntryBase::PSButton3(void)
{
	m_sButton = GetPagedButtonCommand(2);
	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
}

SMStateBase  *SMSmDataEntryBase::PSButton4(void)
{
	m_sButton = GetPagedButtonCommand(3);
	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
}

SMStateBase  *SMSmDataEntryBase::PSButton5(void)
{
	m_sButton = GetPagedButtonCommand(4);
	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
}

SMStateBase  *SMSmDataEntryBase::PSButton6(void)
{
	m_sButton = GetPagedButtonCommand(5);
	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
}

SMStateBase  *SMSmDataEntryBase::PSButton7(void)
{
	m_sButton = GetPagedButtonCommand(6);
	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
}

SMStateBase  *SMSmDataEntryBase::PSButton8(void)
{
    m_sButton = sm_pDataNeededEntry.GetButtonCommand(7);
	if(m_sButton == _T("cGoBack") || m_sButton == _T("cGoToLane"))
    {
        //Notes on cGoBack functionality.
        // If a Dataneeded has cGoBack defined for Button 8,
        // The button functions as an "I don't want to handle the
        // the intevention just now,  please put it back on hold"
        // The RAP must redisplay the RED button and the correct 
        // button text, and the lane must go back to the correct
        // "Wait for approval" screen.  This needs to work whether
        // done from the RAP or the Lane. If we
        // are in the Assist Menus, The intervention should redisplay
        // because,  due to the definition of assist menus, it
        // must be handled immediately.
        if (ps.RemoteMode() && !co.IsInAssistMenus())
        {
            ps.DisconnectFromServer(true);
        }
        //inputMode = SHOW_WAIT_FOR_APPROVAL;	//Fix TAR395729
        ra.RequestEnableAssistMode(true);
        if (co.GetfStateDualLanguage())
            ps.SetLanguage(m_customerLanguage); 
        //showScreen(false);	//Fix TAR395729
        //MUST MUST MUST redisplay intervention for cGoBack
        // The system must handle the re-display of the intervention
        // from both the RAP and the Lane.

        SASendSCOTInput(CUSTOMER_MODE);

        g_bClearOccurred = false;  //SSCOADK-1088
        RETURNSTATE(RAPDataNeeded);
    
    }

	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
}
#endif //_CPPUNIT

CString SMSmDataEntryBase::GetPagedButtonCommand(int index)
{
    if (m_bPageButtons && index==m_nNumButtons)
    {
        return _T("cNextButtonPage");
    }
    else
    {
        CString csButton;
        csButton = sm_pDataNeededEntry.GetButtonCommand(m_nBtnPageIndex * m_nNumButtons + index);
        if (csButton.Compare(_T("cSelectedItem"))==0)
        {
            COleVariant vValue;
            CString csSelectedItemId;
            long rc = m_pAppPSX->GetSelectedReceiptItem(_T("CmdList_DataEntry"), csSelectedItemId);
            if (rc == PSX_SUCCESS)
            {
                m_pAppPSX->GetReceiptItemVariable(_T("DataNeededReceipt"), csSelectedItemId, _T("ITEM_CMDDATA"), vValue);
                m_sInput = vValue.bstrVal;
            }
        }
        return csButton;
    }
    
}

///////////////////////////////////////////
// GetButtonCommand
//   Return command for the button 
///////////////////////////////////////////
CString SMSmDataEntryBase::GetButtonCommand(int nButtonIndex)
{
   CString csButton = sm_pDataNeededEntry.GetButtonCommand(nButtonIndex);
   if (csButton.Compare(_T("cSelectedItem"))==0)
   {
       COleVariant vValue;
       CString csSelectedItemId;
       long rc = m_pAppPSX->GetSelectedReceiptItem(_T("CmdList_DataEntry"), csSelectedItemId);
       if (rc == PSX_SUCCESS)
       {
           	m_pAppPSX->GetReceiptItemVariable(_T("DataNeededReceipt"), csSelectedItemId, _T("ITEM_CMDDATA"), vValue);
            m_sInput = vValue.bstrVal;
       }
   }
   return csButton;
}

#ifndef _CPPUNIT

SMStateBase  *SMSmDataEntryBase::HandleRAPData()
{
	trace (L2, _T("SMSmDataEntryBase::HandleRAPData"));
    if (m_sButton == _T("cNextButtonPage"))
    {
        m_nBtnPageIndex=m_nBtnPageIndex?0:1;
        DisplayUserButtons();
        return STATE_NULL;
    }
	
    else
    {
        // print local chits as in AssistMode UnInitialize()
        if(!ps.RemoteMode() && m_sView == _T("AssistedEndorsement"))
        {
            if(fInEndorsementState != ENDORSE_NONE)
            {
                long nEndorsementsPending = 0;
                do
                {
                    LPTSTR szSlipDocumentDescription = NULL;
                    LPTSTR szSlipDocumentFront = NULL;
                    LPTSTR szSlipDocumentBack = NULL;

                    tb.GetDocEndorsementInfo(&nEndorsementsPending,
                    &szSlipDocumentDescription,  &szSlipDocumentFront, &szSlipDocumentBack);

                    if (nEndorsementsPending)
                    {
                        trace (L0, _T("SMSmAssistModeBase::UnInitialize() Printing endorsement data to a chit on the receipt station."));
                        PrintEndorsementChit(szSlipDocumentBack);
                        // Suppress further endorsement message from TB since we are doing these
                        // in a loop and don't need to be event driven at this time.
                        tb.EndorsementCompleted(true);
                    }
                } while (nEndorsementsPending);
                fInEndorsementState = ENDORSE_NONE;
            }
        }
        return STATE(RAPDataNeeded)::HandleRAPData();
    }
}


///////////////////////////////////////////////////////////
// PSNumericKey
///////////////////////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::PSNumericKey(const long lKey)
{
  CString csTmp;
  csTmp = sm_pDataNeededEntry.GetInputMask();
	
  if(csTmp.Find(_T(".")) != -1 || csTmp.Find(_T("v")) != -1) //if mask for input data is a price
  {
     int nEntryLength = ps.GetInput(csTmp);
     if(0 == nEntryLength) //this is first character of price
	 {
		 if(lKey == '0') //throw leading 0 keys away
		 {
			 DMSayPhrase(KEYPADINVALID);
			 return STATE_NULL;
		 }
	 }
  }

  PSEchoInput(lKey);
  return STATE_NULL;
}


//////////////////////////////////////////////
// PSClearKey
//////////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::PSClearKey(void)
{
  //+SR828
  if (csCurContext.Find(_T( "SmDataEntryAlphanumeric")) != -1) //work as backspace
  {
      CString csItemSent;
      long lLen = ps.GetInput(csItemSent);
      if (lLen)
      {
        DMSayPhrase(KEYPADVALID);
        ps.EchoInput(0x08, false, false);   // clear or correction key should clear only one character
      }
      else
      {
        DMSayPhrase(KEYPADINVALID);
      }
  }
  //-SR828
  else // clear
  {
      PSClearAllInput();
      //PSClearInput(); //228461 - clear btn should clear entire input box, not one character at a time LPM021903

      CString csValue;   // TAR 227930
      long lLen = ps.GetInput(csValue); 
      if (lLen == 0) 
         ps.ClearInput();
  }

  return STATE_NULL;
}

//////////////////////////////////////////////
// PSEnterKey
//////////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::PSEnterKey(void)
{
  
  if( ( inputMode == PROMPTING_FOR_OPERATOR ) ||
      ( inputMode == PROMPTING_FOR_PASSWORD ) )
  {
      if (getOperatorPassword())
          return stateAfterPasswordVerify();
      else
      {
          return STATE_NULL;
      }
  }

  CString csEntryData, m_smask;
  int iFind;
  int nEntryDateLength = ps.GetInput(csEntryData);

  if (!nEntryDateLength)
	return STATE_NULL;

  ps.PSHideSMMNumericKeypad(); //TAR 193566
  long lPrice = _ttol((LPCTSTR)csEntryData);

  // Checking the mask for the 'v' in order to determine whether
  // we should pass the data in currency format or a simple number
  m_smask = sm_pDataNeededEntry.GetInputMask();
  iFind = m_smask.Find(_T("v"));
  if (iFind != -1)    // if there is a decimal we need to adjust value to unit measure
     m_sInput = ps.FmtDollar(lPrice, ps.SP_NONE);
  else
     m_sInput = csEntryData;
  //USSF START
  USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
  //return HandleRAPData();
  //USSF END
}


SMStateBase *SMSmDataEntryBase::OnTBNeedData(void)
{
        trace (L6, _T("+SMSmDataEntryBase::OnTBNeedData()"));
	    CString	sDescription, sView, sINI, sViewString, sSection, csStateName;
        long rc;  // TB return value
        //RFQ 2506 - rework OnTBNeedData logic 
        CString sRDNLaneMessage, sRDNLaneLeadthru, sRDNValues;  //Strings for Rap Data Needed customer display
        CString sViewCustomer;
        CSCOTDataEntry cCustomerDataEntry;
        CPSConfigData cd;
        
        
        //Jira SSCOP-831 Start
        SCOT_LANGUAGE_TYPE currentLanguage;
        currentLanguage = ps.GetLanguage();
        ps.SetLanguage(m_customerLanguage);
        //Jira SSCOP-831 End
        
        bool bGotDataNeededInfo = GetDataNeededViewFromTB(sDescription, 
                                                          sView,
                                                          sINI,
                                                          sSection);
	    if(!bGotDataNeededInfo)
        {
            trace(L4, _T("GetDataNeededViewFromTB Failed"));
            ps.SetLanguage(m_customerLanguage); //Jira SSCOP-831
		    return STATE_NULL;
        }
        else
        {
            m_sDesc = sDescription; //Update desc from TB - TAR 264138
        }

        if(!sView.IsEmpty())   //String of DataNeeded Parameters
        {
            sViewString = sView;
            sViewCustomer = sView;
            cd.FormatString(sViewString, true);   //Primary Language Translation
            cd.FormatString(sViewCustomer, false); //Customer Language Translation
        }

        if(!sINI.IsEmpty())
        {
            if(!sSection.IsEmpty())
            {   //Dataneeded parameters to be retrieved from INI file
                if (cDataEntry.FromConfigData(sINI, sSection))
                {
                    cCustomerDataEntry.FromConfigData(sINI, sSection, false);
                }
                else
                {
                    trace(L4, _T("Configuration Error: No Section data for INIfile=%s, Section=%s"),sINI, sSection);
                }
                
                // Adding sView/sViewCustomer after the cDataEntry settings gives 
                // precedence to settings in the INI file. 
                sViewString = cDataEntry.ToString() + sViewString;
                sViewCustomer = cCustomerDataEntry.ToString() + sViewCustomer;

            }
            else
            {
                trace(L4, _T("GetDataNeededView error, if INI specified, section is required"));
                ps.SetLanguage(m_customerLanguage); //Jira SSCOP-831
                return STATE_NULL;
            }
        }
        else if(sView.IsEmpty() )
        {
            trace(L4, _T("GetDataNeededView cancel, INI and View are both empty, TB wants to cancel"));
            // Tar 214594 RJF 092502
            bEASReEnableScanner = true;
            SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
            ps.SetLanguage(m_customerLanguage); //Jira SSCOP-831
            // +SSCOP-3649 - Return to last HelpStack state or anchor state
            //setAnchorState((BEST) nlaststate);
            //return STATE_RESUME;
            //return setAndCreateAnchorState((BEST) nlaststate);
            // -SSCOP-3649
            return setAndCreateAnchorState((BEST) nlaststate);  //sscoadk-1876
        }
        
		cCustomerDataEntry.FromString(sViewCustomer);
        if (!cCustomerDataEntry.IsStoreLogonEnabled() && !co.IsInAssistMenus())
        {
            sViewString = cCustomerDataEntry.ToString();
            trace(L6,_T("Data Needed View configuration parameters %s"),sViewString);
            trace (L6, _T("-SMStateTBBase::OnTBNeedData() with configuration parameters %s"),sViewString);
            CREATE_AND_DISPENSE(CmDataEntry)(sViewString);
        }
        else
        {
            ps.SetLanguage(m_customerLanguage); //Jira SSCOP-831
            sm_pDataNeededEntry.FromString(sViewString);
            m_View = DATANEEDED_MOREDATA;    // in case the view got reset by onRapDataNeeded somewhere

            //+SSCOP-2691 Refresh Lane button and Lane state window at RAP
            CString csLaneButton = sm_pDataNeededEntry.GetSecondaryButtonText();
            if ( (m_View != DATANEEDED_MOREDATA) && (m_View < DATANEEDED_LASTVIEW))
            {
             //always use the viewname as lane button text for fastlane-defined requests
             csLaneButton = m_sViewName;
            }
            else if (!csLaneButton.IsEmpty())    //lane button text not defined
            {
             csLaneButton = m_sViewName;
            }

            CString csLaneButtonText;
            csLaneButtonText.Empty();
            csLaneButtonText = sm_pDataNeededEntry.GetSecondaryButtonText();

            CString csLaneStateOne;
            csLaneStateOne.Empty();
            csLaneStateOne = sm_pDataNeededEntry.GetLaneStateOneText();
            if(csLaneStateOne.IsEmpty())
             csLaneStateOne.Format(ps.GetPSText(TXT_NEEDINFORMATION, SCOT_LANGUAGE_PRIMARY));

            CString csLaneStateTwo;
            csLaneStateTwo.Empty();
            csLaneStateTwo = sm_pDataNeededEntry.GetLaneStateTwoText();

            CString csLaneStateThree;
            csLaneStateThree.Empty();
            csLaneStateThree = sm_pDataNeededEntry.GetLaneStateThreeText();

            CString csSecInfo;
            csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONWITH_DNTYPEINFO, SCOT_LANGUAGE_PRIMARY),
                m_sDesc,
                csLaneButton, //TAR 233927 replaced m_sView w/m_sViewName will display Msg file Name for translation reasons
                csLaneStateOne, 
                csLaneStateTwo, 
                csLaneStateThree,
                m_View);
            //-SSCOP-2691
            //sm_pDataNeededEntry.FromString(sViewString);
            ra.OnRAPDataNeeded(csSecInfo, sm_pDataNeededEntry.ToString(), m_View);
            //+SSCOP-2691 - Turn off the light because we are in Store Mode
            DMTurnOffTriColorLight();
            //-SSCOP-2691

            trace (L6, _T("-SMSmDataEntryBase::OnTBNeedData()"));
            return Initialize();
        }
        
}

//*** NewUI ***//
SMStateBase  *SMSmDataEntryBase::PSReceiptUp(void)
{
	//SSCOP-3718 Page scrolling implemented through XML
    return STATE_NULL;
}
SMStateBase  *SMSmDataEntryBase::PSReceiptDown(void)
{
	//SSCOP-3718 Page scrolling implemented through XML
	return STATE_NULL;

}

#endif //_CPPUNIT
//*** end of NewUI ***//
//TAR287018+
//If the scanner is enabled based on the requirement of .ini, we will handle the scanner
//data here. Otherwise, we will ask base handler to deal with the scanner data
SMStateBase *SMSmDataEntryBase::DMScanner(void)
{
    // +SR93.4
    if(1 == sm_pDataNeededEntry.GetScannerDataFormat())   //Allow Scan if scannerDataFormat is 0 or 2
    {
       trace(L6,_T("ERROR: Was expecting a 2D barcode type!"));
       Beep(500, 500);
       DMScannerEnable();
       return STATE_NULL;
    }
    // -SR93.4

	if(sm_pDataNeededEntry.IsScannerInputEnabled())
	{
		trace(L2, _T("SMSmDataEntry::DMScanner ()"));
		//(+)SSCOB-19485
		//m_sInput = cDMLastData;
		m_sInput = csDMLastBarCode; 
		//()SSCOB-19485
		//USSF START
		USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
		//return HandleDataNeeded();
		//USSF END
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
SMStateBase *SMSmDataEntryBase::DMScannerEncrypted(void)
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
// SR93.4-

#ifndef _CPPUNIT

void SMSmDataEntryBase::DisplayUserButtons(void)
{
    int i;

    m_nNumButtons=DESKTOP_NUMBUTTONS;

    m_bPageButtons = ra.IsRemoteConnectionHandheld(ra.GetRemoteConnectionName()) && ps.RemoteMode();  //page btns for handheld device
    CString csButtonControlName = EMPTY_STRING;
    if (m_bPageButtons)
    {
        m_nNumButtons = HANDHELD_NUMBUTTONS;
        m_bPageButtons = false;
        //Check if paging is actually necessary
        for (i=0; i<7; i++)
        {
            csButtonControlName.Format(_T("%s%d"), _T("SMButton"), (i + 1));
            if(i >= HANDHELD_NUMBUTTONS && sm_pDataNeededEntry.GetButtonText(i) !=_T(""))
            {
                m_bPageButtons = true;
            }
            ps.Button(csButtonControlName, _T(""), false);
        }


        if  (m_bPageButtons)
        {
            csButtonControlName.Format(_T("%s%d"), _T("SMButton"), (m_nNumButtons + 1));
            ps.Button(csButtonControlName, ps.GetPSText(BTT_NEXT), true);
        }   

    }
    
    int nPageOffset = m_nBtnPageIndex * m_nNumButtons;
    for (i=0; (i+nPageOffset < DESKTOP_NUMBUTTONS) && (i<m_nNumButtons); i++)
    {
		SetUserButton(i, (nPageOffset));
	}
    SetUserButton(DESKTOP_NUMBUTTONS, 0);  //PSButton8 is always non-paged
}

void SMSmDataEntryBase::SetUserButton(int nIndex, int nPageOffset)
{
    CString csButtonControlName = EMPTY_STRING;
    csButtonControlName.Format(_T("%s%d"), _T("SMButton"), (nIndex + 1));
	if(nIndex==DESKTOP_NUMBUTTONS && co.IsLoginBypassOn() && 
	   sm_pDataNeededEntry.GetButtonCommand(nIndex) == _T("cGoBack"))
	{
		ps.ButtonState(csButtonControlName, false, false);
	}
    else if(sm_pDataNeededEntry.GetButtonText(nIndex) !=_T(""))
        ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(nPageOffset + nIndex), true);
    else
        ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(nPageOffset + nIndex), false);
}

// Assist mode may be run legally from the RAP station
bool SMSmDataEntryBase::PSRemoteLegalForState()
{
	return true;
}

// + TAR 332574
bool SMSmDataEntryBase::storeMode() 
{ 
 return true; 
}
// -TAR 332574



/////////////////////////////////////////
// PSDecimalKey
/////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::PSDecimalKey(const long ch)
{
    ps.EchoDecimalInput((_TCHAR)(ch), true);
	long lLen = ps.GetInput(csItemSent);
	return STATE_NULL;
}

//////////////////////////////////////////////////////
// PSAlphaKey
//////////////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::PSAlphaKey(const long lKey)
{
	ps.EchoInput((_TCHAR)lKey, true);	// allow alpha chars in input
	long lLen = ps.GetInput(csItemSent);
    return STATE_NULL;		  	    //remain in same state
}
#endif //_CPPUNIT

SMStateBase  *SMSmDataEntryBase::PSShiftKey(void)
{
    m_bShiftOn=!m_bShiftOn;

    //+SR828
    if (csCurContext.Find(_T( "SmDataEntryAlphanumeric")) != -1)
    { 
        SetAlphaShiftState(csCurContext, m_bShiftOn);

        ps.PopulateHKSensitiveControls(csCurContext, m_HKStatus, m_bShiftOn, m_bAltGrOn);
    }
    //-SR828
    
    return STATE_NULL;              //remain in same state
}

#ifndef _CPPUNIT

SMStateBase  *SMSmDataEntryBase::PSAlphaNumericKey(const long lKey)
{
	long ch = lKey;
	if(m_bCapsInput)  // if input field formatting is 'all caps' 
	{
	  ch = toupper(lKey); 	
	}
	else if(m_bShiftOn) // if input field formatting is 'small caps' but shift is On
	{
	  ch = toupper(lKey);
	}
	ps.EchoInput((_TCHAR)ch, true);	// allow alpha chars in input
	long lLen = ps.GetInput(csItemSent);
    return STATE_NULL;		  	    //remain in same state
}

/////////////////////////////////////////////////////
// PSBackSpc
//////////////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::PSBackSpaceKey(void)
{
 
  PSClearInput();
  
  CString csValue;   // TAR 227930
  long lLen = ps.GetInput(csValue);	
  if (lLen == 0) 
     ps.ClearInput();


  return STATE_NULL;  	    //remain in same state
}

/////////////////////////////////////////
// PSSpaceKey
/////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::PSSpaceKey(void)
{
    ps.EchoInput((_TCHAR)(0x20), true);
	return STATE_NULL;
}

SMStateBase *SMSmDataEntryBase::PSOnRemoteDisconnected(const long lParm)
{
    trace (L2, _T("SMSmDataEntryBase::PSOnRemoteDisconnected m_View = %d"),m_View);
	if(g_bTBNeedMoreData)
    {
	  m_saveDataNeededView = m_View;
	}
	return STATE(RAPDataNeeded)::PSOnRemoteDisconnected(lParm);
}

//////////////////////////////////
SMStateBase *SMSmDataEntryBase::DMCardReader(void)
{
  trace(L6,_T("+SMSmDataEntryBase::DMCardReader()"));

  USSFHOOK(_T("HandleDataNeeded"), nextState = HandleMSRData(), return nextState);
}
#endif
/////////////////////////////////////
// RFQ 986
// PSCmdButton
// 
// Purpose: Handle "Command" button press.  This method is called
//          when a click occurs on a button or button list named "CmdBtn_X..."
//          where any sequence of characters can follow the "_"
// Parameters
//   csContext - name of the context 
//   csControl - name of the control
//   csCommand - contents of the control "Data", defined either in a button's "Data"
//               property or a button list's "List" property
//        		<Control Name="CmdBtn_Button1" Type="Button">
//                  <Data>control string</Data>
//              </Control>
//              <Control Name="CmdBtn_List1" Type="ButtonList">
//                  <List>
//                     <Button>Label1,-,control string 1</Button>
//                     <Button>Label1,-,control string 2</Button>
//                  </List>
//              </Control>
///////////////////////////////////////
SMStateBase *SMSmDataEntryBase::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
    trace(L6,_T("+SMStateBase::PSCmdButton(%s, %s, %s)"), csContext, csControl, csCommand);
   
    SMStateBase* retState = STATE_NULL;

    if(g_bTBNeedMoreData)
    {
        if ( csCommand.IsEmpty() )
        {
            DMSayPhrase(KEYPADINVALID);
        }
        else
        {
            trace(L6,_T("PSCmdButton sending \"%s\" to TB"), csCommand);
            SendTBCommand(TBCMD_CMDSTRING, csCommand);
            ps.SetContextWithKeyPressed();  //SSCOP-3531
            CREATE_AND_ASSIGN(RAPDataNeeded, retState)(NULL, DATANEEDED_GOTOINPROGRESS, NULL, storeMode());
        }
    }
    else
        trace(L6, _T("SMStateBase::PSCmdButton(), TB has already been notified!"));
    
    g_bTBNeedMoreData = false;  // TAR 238061
    
    // start of TAR 265143
    if( g_bFromRAP)
    {
        g_bClearOccurred = true;
    }
    else
    {
        SAClearSecurityCondition();
    }
    ClearCurrentGenericDelayedIntervention();

    trace(L6,_T("-SMStateBase::PSCmdButton()"));
    return retState;                // discard any input
}

//+SSCOP-3803
//************************************************************************ 
//
// Function:     OnRAPUnsolicitedDataReceived
// 
// Purpose:      This function handles unsolicited data received from RAP
// 
// Parameters:   sData -  the received data
// 
// Returns:      Returned state of either SMStateBase::OnRAPUnsolicitedDataReceived
//               or SMState::OnRAPUnsolicitedDataReceived call
// 
// Calls:        SMStateBase::OnRAPUnsolicitedDataReceived
//               SMState::OnRAPUnsolicitedDataReceived
// 
// Side Effects: Under certain conditions, ms_bRAPHasScanned, fItemIsWaiting,
//               or fSAInItemSale could be changed
//
// Notes:
// 
//************************************************************************ 
#ifndef _CPPUNIT
SMStateBase *SMSmDataEntryBase::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    CString csData = sData;
    bool bSavefItemIsWaiting = fItemIsWaiting;
    bool bSavefInItemSale = fSAInItemSale;
    bool bRestore = false;

    trace(L7, _T("+SMSmDataEntryBase::OnRAPUnsolicitedDataReceived  <%s>"), csData);

    if (csData.Find(_T("cScanData")) != -1)
    {
        trace(L6, _T(" SMSmDataEntryBase::OnRAPUnsolicitedDataReceived  Inside cScanData"));
        bool bIsScannerAllowed = sm_pDataNeededEntry.IsScannerInputEnabled() && !ms_bRAPHasScanned;
        if ( bIsScannerAllowed && (fItemIsWaiting || fSAInItemSale) )
        {
            trace(L6, _T(" SMSmDataEntryBase::OnRAPUnsolicitedDataReceived  Inside IsScannerAllowed"));
            ms_bRAPHasScanned = true;
            fItemIsWaiting = false;
            fSAInItemSale = false;
            bRestore = true;
        }
    }

    SMStateBase* pState = STATE(RAPDataNeeded)::OnRAPUnsolicitedDataReceived( sData);


    if (bRestore)
    {
        fItemIsWaiting = bSavefItemIsWaiting;
        fSAInItemSale = bSavefInItemSale;
    }
    return pState;

}
#endif //_CPPUNIT
//-SSCOP-3803

SMStateBase  *SMSmDataEntryBase::PSAltGrKey(void)
{
  
    m_bAltGrOn=!m_bAltGrOn;

    if (csCurContext.Find(_T( "SmDataEntryAlphanumeric")) != -1)
    {
        ps.PopulateHKSensitiveControls(csCurContext, m_HKStatus, m_bShiftOn, m_bAltGrOn);
    }

    return STATE_NULL;
}

//************************************************************************ 
//
// Function:		PSCharKey
// Purpose:			This function handles the button clicks from the alphanumeric keys
// Parameters:    [in] TCHAR c - The character on the alphanumeric key that has been clicked
// Returns:	      SMStateBase  * 
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase  *SMSmDataEntryBase::PSCharKey(TCHAR c)
{
   ps.EchoInput(c, true, false);

   return STATE_NULL;//remain in same state
}

//************************************************************************ 
//
// Function:		PSHotKey
// Purpose:			This function handles the button clicks from the hotkeys control
// Parameters:    [in] CString & csHotKey - The name of the hot key that has been clicked
// Returns:	      SMStateBase  * 
// Calls:         PopulateHKSensitiveControls
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase  *SMSmDataEntryBase::PSHotKey(CString & csHotKey) 
{
  IPSProcedures::HKStatus newStatus=IPSProcedures::HK_NORMAL;
  if(csHotKey==_T("HKAcute"))
  {
    newStatus=IPSProcedures::HK_ACUTEON;
  }
  else if(csHotKey==_T("HKGrave"))
  {
    newStatus=IPSProcedures::HK_GRAVEON;
  }
  else if(csHotKey==_T("HKCircumflex"))
  {
    newStatus=IPSProcedures::HK_CIRCUMFLEXON;
  }
  else if(csHotKey==_T("HKUmlaut"))
  {
    newStatus=IPSProcedures::HK_UMLAUTON;
  }
  else if(csHotKey==_T("HKTilde"))
  {
    newStatus=IPSProcedures::HK_TILDEON;
  }

  COleVariant vIndex(0L), vReturn;
  
  if(m_HKStatus==newStatus)
  {
    m_HKStatus=IPSProcedures::HK_NORMAL;
  }
  else
  {
    m_HKStatus=newStatus;
  }


   PSXRC rc=PSX_SUCCESS;
   rc=m_pAppPSX->GetControlProperty( _T("HotKeys"), UI::PROPERTYBUTTONCOUNT, vIndex );
   if(rc==PSX_SUCCESS)
   {
      long nButtonCount = vIndex.lVal;
      if(nButtonCount)
      {
         m_pAppPSX->SetConfigProperty(_T("HotKeys"), csCurContext, UI::PROPERTYREDRAW, VARIANT_FALSE);
         if(m_HKStatus == IPSProcedures::HK_NORMAL)
         {
            for( long i = 0L; i < nButtonCount; i++ )
            {
               COleVariant pvParams[2];
               pvParams[0]=i;
               pvParams[1]=(long) UI::Normal;
               m_pAppPSX->SendCommand( _T("HotKeys"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, pvParams );
            }
         }
         m_pAppPSX->SetConfigProperty(_T("HotKeys"), csCurContext, UI::PROPERTYREDRAW, VARIANT_TRUE);
      }

      ps.PopulateHKSensitiveControls(csCurContext, m_HKStatus, m_bShiftOn, m_bAltGrOn);
   }


   return STATE_NULL;//remain in same state

}

SMStateBase  *SMSmDataEntryBase::PSButtonGoBack(void) // cancel all
{
    return PSButton8();
}
//Start CRD 321191
SMStateBase *SMSmDataEntryBase::stateAfterPasswordVerify()
{
	trace(L2, _T("-SMTakeChangeBase::stateAfterPasswordVerify"));
	return STATE_NULL;
}
//End CRD 321191