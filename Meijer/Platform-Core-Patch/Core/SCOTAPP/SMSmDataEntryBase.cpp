//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmDataEntryBase.CPP
//
// TITLE: Class implementation for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmDataEntry.h"    // This state
#include "SMSmVoidedItems.h"        // Voided items auth
#include "SMSmAuthorization.h"      // Auth state
#include "SMSmEnterBirthdate.h"     // 
#include "SMCmDataEntry.h"
#include "PSConfigData.h"

#define HANDHELD_NUMBUTTONS 4 //Number of pageable user buttons (excluding SMButton8)
#define DESKTOP_NUMBUTTONS 7 //Number of pageable user buttons (excluding SMButton8)
#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmDataEntryBase")
//DLLEXPORT extern CSCOTDataEntry cDataEntry;

IMPLEMENT_DYNCREATE(SMSmDataEntryBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmDataEntry)

DEFINE_TIMESTAMP
int  SMSmDataEntryBase::nlaststate = -1;
bool SMSmDataEntryBase::savebEASReEnableScanner = 0;	// Tar 210186 RF 071902

// NewUI removed PSInterface

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmDataEntryBase::SMSmDataEntryBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
  // save how scanner flag is set now because we may need to change it
  // it will be restored when we leave the data entry state
  savebEASReEnableScanner = bEASReEnableScanner;	// Tar 210186 RF 071902
  trace (L2, _T("SMSmDataEntryBase saving bEASReEnableScanner %d"),bEASReEnableScanner);
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::Initialize(void)
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
	  ps.SetLeadthruText(sm_pDataNeededEntry.GetSummaryInstruction(), m_sDesc );    
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

 // return STATE_NULL;                // stay in SmDataEntryBase state
  // We need to enable the default button handling for RAP. To do this we will check if there
	// is a default button specified in the data entry view information and if so - we will go
	// ahead and press the button instead of showing the frame. 
	
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
		ps.ShowFrame();
		return STATE_NULL;                // stay in SmDataEntry state
	};
}


void SMSmDataEntryBase::UnInitialize(void)   //TAR 193566
{ 
  COleVariant vValue = VARIANT_FALSE; // TAR 350774
  
  ps.PSHideSMMNumericKeypad();
  //TD 04/22/02 TAR 198040 
  if (co.fStateDualLanguage && m_customerLanguage != SCOT_LANGUAGE_PRIMARY)  
  {
	ps.SetLanguage(m_customerLanguage);
  }
  // restore previous scanner flag setting
  bEASReEnableScanner = savebEASReEnableScanner;	// Tar 210186 RF 071902

  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmDataEntry") );	// RFC 330050
  }

  m_pAppPSX->SetConfigProperty(_T("MediaList"), csCurContext, UI::PROPERTYVISIBLE, vValue); // TAR 350774
  DMCheckCashCounts(false);	// TAR 350774 Restore media status.

  trace (L2, _T("SMSmDataEntryBase restoring bEASReEnableScanner to %d"),bEASReEnableScanner);

  if(!fInMultiSelectPickList)//SR679
  {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmDataEntry") ); //382138
  }
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
}

SMStateBase  *SMSmDataEntryBase::PSButton1(void)
{
    if( fInValidOperatorPassword == true )
    {
        promptForOperator();  
        return STATE_NULL;
    }

	m_sButton = GetPagedButtonCommand(0);
	// 227275 change saved enable since the item was canceled LPM021103
	if (m_sButton == _T("cReturn")) {	savebEASReEnableScanner = true; }
	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
}

SMStateBase  *SMSmDataEntryBase::PSButton2(void)
{
	m_sButton = GetPagedButtonCommand(1);
	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
}

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
	if(m_sButton == _T("cGoBack"))
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
        if (co.fStateDualLanguage)
            ps.SetLanguage(m_customerLanguage); 
        //showScreen(false);	//Fix TAR395729
        //MUST MUST MUST redisplay intervention for cGoBack
        // The system must handle the re-display of the intervention
        // from both the RAP and the Lane.
        RETURNSTATE(RAPDataNeeded); 
    
    }
	//USSF START
	USSFHOOK(_T("HandleRAPData"), nextState = HandleRAPData(), return nextState);
	//return HandleRAPData();
	//USSF END
}

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

SMStateBase  *SMSmDataEntryBase::HandleRAPData()
{
    if (m_sButton == _T("cNextButtonPage"))
    {
        m_nBtnPageIndex=m_nBtnPageIndex?0:1;
        DisplayUserButtons();
        return STATE_NULL;
    }
	
    else
        return STATE(State)::HandleRAPData();
}


///////////////////////////////////////////////////////////
// PSNumericKey
///////////////////////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}


//////////////////////////////////////////////
// PSClearKey
//////////////////////////////////////////////
SMStateBase  *SMSmDataEntryBase::PSClearKey(void)
{
  PSClearAllInput();
  //PSClearInput();	//228461 - clear btn should clear entire input box, not one character at a time LPM021903
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
        
        sDescription.Empty();
        sView.Empty();
        
        sINI.Empty();
        sSection.Empty();
        
        SDataNeededView ViewInfo;
        ViewInfo.szDescription = NULL;
        ViewInfo.szView = NULL;
        ViewInfo.szIniFile = NULL;
        ViewInfo.szSection = NULL;
        
        
        
        rc = tb.GetDataNeededView(&ViewInfo);
        ProcessTBReturn(rc);
	    if(rc!=TB_SUCCESS)
        {
            trace(L4, _T("Exception with GetDataNeededView, return code=%d"), rc);
		    return STATE_NULL;
        }
        else
        {
            m_sDesc = ViewInfo.szDescription; //Update desc from TB - TAR 264138
        }
        if(ViewInfo.szDescription)
        {
            sDescription = ViewInfo.szDescription;
            try{ delete ViewInfo.szDescription;} 
            catch(...){trace(L4,_T("Exception while deleting ViewInfo.szDescription"));}
        }
        
        if(ViewInfo.szView)   //String of DataNeeded Parameters
        {
            sView = ViewInfo.szView;
            sViewString = sView;
            sViewCustomer = sView;
            cd.FormatString(sViewString, true);   //Primary Language Translation
            cd.FormatString(sViewCustomer, false); //Customer Language Translation
            
            try{ delete ViewInfo.szView;}
            catch(...){trace(L4, _T("Exception while deleting ViewInfo.szView"));}
        }
        if(ViewInfo.szIniFile)
        {
            sINI = ViewInfo.szIniFile;
            try{ delete ViewInfo.szIniFile;} 
            catch(...){trace(L4, _T("Exception while deleting ViewInfo.szIniFile"));}
        }
        if(ViewInfo.szSection)
        {
            sSection = ViewInfo.szSection;
            try{ delete ViewInfo.szSection;} 
            catch(...){trace(L4, _T("Exception while deleting ViewInfo.szSection"));}
        }
        if(sINI != _T(""))
        {
            if(sSection != _T(""))
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
                return STATE_NULL;
            }
        }
        else if(sView == _T("") )
        {
            trace(L4, _T("GetDataNeededView cancel, INI and View are both empty, TB wants to cancel"));
            // Tar 214594 RJF 092502
            savebEASReEnableScanner = true;
            SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
            return setAndCreateAnchorState((BEST) nlaststate);
        }
        
        if (!cCustomerDataEntry.IsStoreLogonEnabled() && !co.IsInAssistMenus())
        {
            cCustomerDataEntry.FromString(sViewCustomer);
            sViewString = cCustomerDataEntry.ToString();
            trace(L6,_T("Data Needed View configuration parameters %s"),sViewString);
            trace (L6, _T("-SMStateTBBase::OnTBNeedData() with configuration parameters %s"),sViewString);
            CREATE_AND_DISPENSE(CmDataEntry)(sViewString);
        }
        else
        {
            sm_pDataNeededEntry.FromString(sViewString);
            m_View = DATANEEDED_MOREDATA;    // in case the view got reset by onRapDataNeeded somewhere
			ra.OnRAPDataNeeded(sSection, sViewString, m_View);
            trace (L6, _T("-SMSmDataEntryBase::OnTBNeedData()"));
            return Initialize();
        }
        
}

//*** NewUI ***//
SMStateBase  *SMSmDataEntryBase::PSReceiptUp(void)
{
  COleVariant vReturn, vScrollMode;

  vScrollMode = (long) UI::PageUp;
  m_pAppPSX->SendCommand(_T("MediaList"), UI::COMMANDSCROLL, vReturn, 1, vScrollMode);
  return STATE_NULL;
}
SMStateBase  *SMSmDataEntryBase::PSReceiptDown(void)
{
  COleVariant vReturn, vScrollMode;
  vScrollMode = (long) UI::PageDown;
  m_pAppPSX->SendCommand(_T("MediaList"), UI::COMMANDSCROLL, vReturn, 1, vScrollMode);  return STATE_NULL;

}
//*** end of NewUI ***//
//TAR287018+
//If the scanner is enabled based on the requirement of .ini, we will handle the scanner
//data here. Otherwise, we will ask base handler to deal with the scanner data
SMStateBase *SMSmDataEntryBase::DMScanner(void)
{
	if(sm_pDataNeededEntry.IsScannerInputEnabled())
	{
		trace(L2, _T("SMSmDataEntry::DMScanner ()"));
		m_sInput = cDMLastData;
		//USSF START
		USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
		//return HandleDataNeeded();
		//USSF END
	}
	else
	{
		return SMOperatorPasswordStateBase::DMScanner();
	}

}
//TAR287018-

void SMSmDataEntryBase::DisplayUserButtons(void)
{
    int i;

    m_nNumButtons=DESKTOP_NUMBUTTONS;

    m_bPageButtons = ra.IsRemoteConnectionHandheld(ra.GetRemoteConnectionName());  //page btns for handheld device
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
SMStateBase  *SMSmDataEntryBase::PSShiftKey(void)
{
	if(m_bShiftOn)
	{
		m_bShiftOn = false;
	}
	else
	{
		m_bShiftOn = true;
	} 

	CString csContext = csCurContext;
	CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
	if (!csContextToDisplay.IsEmpty())
	{
		csContext = csContextToDisplay;
	}
	if (csContext.Find(_T( "Alphanumeric")) != -1)
	{
		if (m_bCapsInput)
		{
			SetAlphaShiftState(csContext, m_bCapsInput);
		}
		else
		{
			SetAlphaShiftState(csContext, m_bShiftOn);
		}
	}

    return STATE_NULL;		  	    //remain in same state
}


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
	return STATE(State)::PSOnRemoteDisconnected(lParm);
}

/*
/////////////////////////////////////
// PSCmdList
// 
// Purpose: Handle "Click" event on a CmdList_ receipt control  This method is called
//          when a click occurs on a receipt list named "CmdList_X..."
//          where any sequence of characters can follow the "_"
// Parameters
//   csContext - name of the context 
//   csControl - name of the control
//   lIndex - index of the list item clicked
///////////////////////////////////////
SMStateBase *SMSmDataEntryBase::PSCmdList(CString csContext, CString csControl, const long lIndex)
{
    trace(L6,_T("+SMSmDataEntryBase::PSCmdList(%s, %s, %d)"), csContext, csControl, lIndex);

	m_sInput.Format(_T("%d"),lIndex);
	m_sButton=csControl;
  //USSF START
  USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
  //return HandleDataNeeded();
  //USSF END
}
*/

//////////////////////////////////
SMStateBase *SMSmDataEntryBase::DMCardReader(void)
{
  trace(L6,_T("+SMSmDataEntryBase::DMCardReader()"));

  USSFHOOK(_T("HandleDataNeeded"), nextState = HandleMSRData(), return nextState);
}

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
            ps.SetContextWithKeyPressed();	//SSCOB-102
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
    trace(L6,_T("-SMStateBase::PSCmdButton()"));
    return STATE_NULL;                // discard any input
}