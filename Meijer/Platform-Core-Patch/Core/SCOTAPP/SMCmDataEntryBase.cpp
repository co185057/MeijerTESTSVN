//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCmDataEntryBase.CPP
//
// TITLE: Class implementation for customer mode data entry state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMCmDataEntry.h"      
#include "SMInProgress.h"           // In progress state
#include "SMRapDataNeeded.h"
#include "PSConfigData.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CmDataEntryBase")

IMPLEMENT_DYNCREATE(SMCmDataEntryBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(CmDataEntry)

DEFINE_TIMESTAMP

CSCOTDataEntry SMCmDataEntryBase::sm_pDataNeededEntry;
int  SMCmDataEntryBase::nlaststate = -1;
bool SMCmDataEntryBase::savebEASReEnableScanner = 0;	// Tar 210186 RF 071902
bool SMCmDataEntryBase::bInSmCmDataEntry = false; //TAR371194
//////////////////////////////////////////
SMCmDataEntryBase::SMCmDataEntryBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::Deliver(LPCTSTR pPtr)
{
    // This will setup the object properties and members with 
    // the data provided by the TB
    sm_pDataNeededEntry.FromString(pPtr);
	// save how scanner flag is set now because we may need to change it 
    // it will be restored when we leave the data entry state
    savebEASReEnableScanner = bEASReEnableScanner;	// Tar 210186 RF 071902
    trace (L2, _T("SMCmDataEntryBase bEASReEnableScanner is set to %d"),bEASReEnableScanner);
    return this;
}
/////////////////////////////////////////
bool SMCmDataEntryBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be One parameter, a string
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMCmDataEntryBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((LPCTSTR)DLParamCString(dlParameterList));   // pPtr
        return true;
    }
    else
    {
        trace(L2, _T("+SMCmDataEntryBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
/////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::Initialize(void)
{
	//TAR371194+
    if (!bInSmCmDataEntry)
	{
		if(!fInMultiSelectPickList)
		{
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CmDataEntry") );
		}
        bInSmCmDataEntry = true; 
    }
    //TAR371194-
    int iFind, iInputLength, iInputMinLen, nButtons=0;
    CString csSayPhrase, csButtonControlName;
	PSXRC rc;

    if(getAnchorState() != BES_CMDATAENTRY) // save off for possible cancel later
        nlaststate = getAnchorState();

	//TAR459883: use g_bTBNeedMoreData instead of nTBLastRealState because nTBLastRealState could be changed to
	//TB_TOTAL
    //if(nTBLastRealState != TB_NEEDMOREDATA) //tar 252987
	if( !g_bTBNeedMoreData ) 
    {
		savebEASReEnableScanner = true;
		return setAndCreateAnchorState((BEST) nlaststate);
    }

	//TAR 343819 
	if(!csDMLastBarCode.IsEmpty())
	{
	    //if something is being scanned at attract, save the bar code		
		csItemSent = csDMLastBarCode;
	}

	// Tar 210056 RF 071902 - initialize input fields
    m_sButton.Empty();
    m_sInput.Empty();

    CString csInputString = sm_pDataNeededEntry.GetInputMask();
    CString csSummary = sm_pDataNeededEntry.GetSummaryInstruction();
    CString csViewType = sm_pDataNeededEntry.GetViewType();
	CString csContextToDisplay = sm_pDataNeededEntry.GetContextToDisplay();
    CString csButtonFont = sm_pDataNeededEntry.GetButtonFont();
	
	fCapsInput = false;
	if((csInputString != _T("")))
    {
        if (csInputString[0] == _TCHAR('A'))
	    {
		    fCapsInput = true;
        }
	}
    bShiftOn = false;
    
    //Update RAP window with SCOT state

	//TAR 344460 begin - Now setting up the RAP Window by taking LaneButtonText, LSWND1, LSWND2 and LSWND3 texts from TBscotdataentry.ini
    
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
			              _T(""), 
						  csLaneButtonText,
						  csLaneStateOne, 
						  csLaneStateTwo, 
						  csLaneStateThree,
                          _T("")
						 );
	UpdateStateName(csSecInfo);
	//UpdateStateName(ps.GetPSText(TXT_NEEDINFORMATION, SCOT_LANGUAGE_PRIMARY));

    //TAR 344460 end
	
	
	if( getAnchorState() != BES_PUTBAGONSCALE )//tar 418401
	{
		setAnchorState(BES_CMDATAENTRY);  //TAR#134884 return to this state from help on way
	}

	// Need to determine how many buttons to use in order to set
	// correct context if we are centering the buttons horizontally

    // TAR 413284 - Change from 8 to 7 so the "Go Back" button is not included in the nButtons count.
	for (int i=0; i<7; i++)
	{
        if(sm_pDataNeededEntry.GetButtonText(i) !=_T(""))
		{
	        nButtons++;
		}
	}
	//tar +408235 
	//call ps.Message12 at the end
	bool bModalMessage = false; //tar 408235
    if (csViewType.IsEmpty())               // check if using ViewType parameter
    { // no, so use inputstring to determine if numbers are needed  
        if (csInputString.IsEmpty())        // buttons
        {
	        if(co.fOperationsCenterCustomerDataNeededButtons)
			{	// dynamic context based on #buttons
                csCurContext.Format(_T("%s%d"), _T("CmDataEntry"), nButtons );
			}
	        else
			{
                csCurContext = _T("CmDataEntry");
			}
			bModalMessage = true; //tar 408235
        }
        else                                // pinpad
        {
            csCurContext = _T("CmDataEntryWithKeyBoard");
			bModalMessage = false; //tar408235
        }
    }
    else if(csViewType == _T("Selection"))      // button selection
    {
	    if(co.fOperationsCenterCustomerDataNeededButtons)
		{	// dynamic context based on #buttons
            csCurContext.Format(_T("%s%d"), _T("CmDataEntry"), nButtons );
		}
	    else
		{
            csCurContext = _T("CmDataEntry");
		}
		bModalMessage = true; //tar 408235
    }
    else if(csViewType == _T("Numeric"))        // pinpad
    {
        csCurContext =_T("CmDataEntryWithKeyBoard");
		bModalMessage = false; //tar 408235
    } 

    else if(csViewType == _T("Alpha"))        // alphanumeric keyboard
    {  
		if(csContextToDisplay.IsEmpty())
		{
			//ps.SetCMFrame(_T("EnterAlphaNumericInfo"));
			csCurContext = _T("EnterAlphaNumericInfo");
		}
		else
		{ 
			csCurContext = csContextToDisplay;
			//ps.SetCMFrame(csContextToDisplay); //commented 7/10/2009 tar 408235
			//ps.SetCMFrame(_T("EnterAlphaNumericInfo"));
		}
		bModalMessage = false; //tar 408235
        dm.PinPadDisable();       // TAR 125995
    }
	ps.SetCMFrame(csCurContext); //tar 408235
	ps.Message12(sm_pDataNeededEntry.GetSummaryInstruction(), bModalMessage); //tar 408235
	//-tar 408235

    // only show bottom right global navigation button if Button7 parameter exists
   
	if(csViewType != _T("Alpha"))
	{
		ps.SetFrame (csCurContext);
	}
    if(sm_pDataNeededEntry.GetButtonText(7) !=_T(""))
    {
        ps.Button(_T("ButtonGoBack"),sm_pDataNeededEntry.GetButtonText(7), true);
    }
	//TAR228222+ chu: make the button invisible 2/18/03
	else
	{
		ps.Button(_T("ButtonGoBack"), PS_NOTEXT, false);  
	}
	//TAR228222-

    ps.SetCMLeadthruText(sm_pDataNeededEntry.GetTopCaption());

    // See if they want the Total to be displayed
    if(sm_pDataNeededEntry.IsTotalHidden())
        ps.ShowCMTotal(false,false);   // hide the line at bottom with total
    else
        ps.ShowCMTotal(false,true);
    ps.ShowCMTBText(csTBMessage2Scot);	//thp

    // See if HideHelp was set
    COleVariant v, vFont;
    if(sm_pDataNeededEntry.IsHelpHidden())
    {
	    v = VARIANT_FALSE;   // hide global help button
    }
    else
    {
        v = VARIANT_TRUE;   // show global help button
    }
    m_pAppPSX->SetConfigProperty(_T("ButtonHelp"), csCurContext, UI::PROPERTYVISIBLE, v);
    if (!csButtonFont.IsEmpty())               // check if using ButtonFont parameter
	{
        rc = m_pAppPSX->GetConfigFont(csButtonFont, vFont);
	    if(PSX_SUCCESS != rc)
		{	// failure so use original font for CMButtonXMidiList
			trace(L4, _T("ButtonFont %s not found"),csButtonFont);
		    vFont = defaultFont;
		}
	}
	else
	{	// setup default font if buttonfont parameter not used
		vFont = defaultFont;
	}

    nButtons=0;
    for (i=0; i<7; i++)
    {
        csButtonControlName = EMPTY_STRING;
        if(co.fOperationsCenterCustomerDataNeededButtons)
        {	// only interesed in centering buttons - not maintaining specific button positions
            csButtonControlName.Format (_T("%s%d%s"), _T("CMButton"), (nButtons+1) , _T("MidiList") );
            if(sm_pDataNeededEntry.GetButtonText(i) !=_T(""))
            {
                ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), true);
                nRelativeButton[nButtons]=i;         
                rc = m_pAppPSX->SetConfigProperty(csButtonControlName, csCurContext, UI::PROPERTYFONT, vFont);
                if(PSX_SUCCESS != rc)
                    trace(L4, _T("Failure Setting Button Font for %s, err=%x"),csCurContext,rc);
                nButtons++;
            }
            //+TAR 403015 
            else
            {
                if(sm_pDataNeededEntry.GetContextToDisplay() != _T(""))
                { //If context is directly specified,  Scotapp should not consolidate buttons
                    nRelativeButton[nButtons]=i;
                    nButtons++;
                }
                m_pAppPSX->SetConfigProperty(csButtonControlName, csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE); 
            }
            
            //-TAR 403015 
        }
        else
        {
            csButtonControlName.Format (_T("%s%d%s"), _T("CMButton"), (i+1) , _T("MidiList") );
            if(sm_pDataNeededEntry.GetButtonText(i) !=_T(""))
            {	// using button parameter setting to determine position - no centering
                ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), true);
                rc = m_pAppPSX->SetConfigProperty(csButtonControlName, csCurContext, UI::PROPERTYFONT, vFont);
                if(PSX_SUCCESS != rc)
                    trace(L4, _T("Failure Setting Button Font for %s, err=%x"),csCurContext,rc);
            }
            else
                ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), false);
        }
    }

    //RFQ 2506 - Build CmdButton and CmdList data if necessary
    BuildCmdButtonList(sm_pDataNeededEntry, csCurContext, _T("CmdBtn_DataEntryList") );
	BuildCmdList(sm_pDataNeededEntry, _T("DataNeededReceipt"));
    ps.ShowFrame(false);        
 
	if(sm_pDataNeededEntry.IsScannerInputEnabled())
	{
		bEASReEnableScanner = true;
		SAWLDBScannerEnable();		  // enable the scanner
	}
	else							// Tar 210186 RF 071902
		SAWLDBScannerDisable();		// disable scanner if ini parameter set to 0 or not present

	if(sm_pDataNeededEntry.IsMSRInputEnabled())   //RFQ 2605 - MSR Input from DataNeeded
	{
		DMMSREnable();		  // enable the MSR
	}
	else							// Tar 210186 RF 071902
		DMMSRDisable();		// disable MSR if ini parameter set to 0 or not present


    iInputMinLen = sm_pDataNeededEntry.GetInputLengthMin(); //RFC 335528

    // allow the EchoInput() func to accept up to max digits
    if(csViewType == _T("Alpha"))
    {    
		bool bHideInput = sm_pDataNeededEntry.IsInputHidden();
        iInputLength = csInputString.GetLength();
        if((iInputLength < MAX_DIGITS) && (iInputLength > 0))
            ps.AllowInput(iInputLength,bHideInput,false, iInputMinLen);
        else
            ps.AllowInput(MAX_DIGITS,bHideInput,false, iInputMinLen);
    }
    else 
	{
		iInputLength = csInputString.GetLength();
		if(iInputLength)
		{
			bool bHideInput = sm_pDataNeededEntry.IsInputHidden();	// Tar 230459
			csInputString.TrimLeft(csInputString.Find(_T('@')));
			iFind = csInputString.Find(_T("v"));
			if (iFind != -1)    // if there is a decimal(v), decrement length
				--iInputLength;

			/* TAR - 344962 
			//Removed following line to allow the
			//input mask to set the maximum length
			if (iInputLength > MAX_DIGITS_CASHAMT)
			{
			   iInputLength = MAX_DIGITS_CASHAMT;
			}
			*/

            if (iFind != -1)    // if there is a decimal we need to adjust format to currency
				ps.AllowInput(iInputLength,bHideInput,true, iInputMinLen);	// Tar 230459
			else
				ps.AllowInput(iInputLength,bHideInput,false, iInputMinLen);	// Tar 230459
		}
	}


    // Check if they have a specific sound file to play
    csSayPhrase = sm_pDataNeededEntry.GetSayPhrase();
    if(csSayPhrase.IsEmpty())
        DMSayPhrase(INFORMATIONNEEDED);
    else
        DMSayPhrase(csSayPhrase);
	
  return STATE_NULL;                // stay in Customer mode Data Entry state
}

/////////////////////////////////////////
// PSDecimalKey
/////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSDecimalKey(const long ch)
{
    ps.EchoDecimalInput((_TCHAR)(ch), true);
	long lLen = ps.GetInput(csItemSent);
	return STATE_NULL;
}


void SMCmDataEntryBase::UnInitialize(void)   //TAR 193566
{ 
  // restore previous scanner flag setting
  bEASReEnableScanner = savebEASReEnableScanner;	// Tar 210186 RF 071902
  trace (L2, _T("SMCmDataEntryBase restoring bEASReEnableScanner to %d"),bEASReEnableScanner);
  bInSmCmDataEntry = false; //TAR371194 
  if(!fInMultiSelectPickList)
  {
	  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CmDataEntry") );
  }
   //RFQ 2506 - Clear CmdBtn list and CmdList controls
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

////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSButton1(void)
{
	if(co.fOperationsCenterCustomerDataNeededButtons)
	{	// Need to determine first button
        m_sButton=GetButtonCommand(nRelativeButton[0]);
    }
	else
	{
	    m_sButton = GetButtonCommand(0);
	}
	// 227275 change saved enable since the item was canceled LPM021103
	if (m_sButton == _T("cReturn")) {	savebEASReEnableScanner = true; }
	//USSF START
	USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	//return HandleDataNeeded();
	//USSF END
}

SMStateBase  *SMCmDataEntryBase::PSButton2(void)
{
	if(co.fOperationsCenterCustomerDataNeededButtons)
	{	// Need to determine second button
        m_sButton=GetButtonCommand(nRelativeButton[1]);
    }
	else
	{
	    m_sButton = GetButtonCommand(1);
	}
	//USSF START
	USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	//return HandleDataNeeded();
	//USSF END
}

////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSButton3(void) 
{

	if(co.fOperationsCenterCustomerDataNeededButtons)
	{	// Need to determine first button
        m_sButton=GetButtonCommand(nRelativeButton[2]);
    }
	else
	{
	    m_sButton = GetButtonCommand(2);
	}
	//USSF START
	USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	//return HandleDataNeeded();
	//USSF END
}

SMStateBase  *SMCmDataEntryBase::PSButton4(void)
{
	if(co.fOperationsCenterCustomerDataNeededButtons)
	{	// Need to determine first button
        m_sButton=GetButtonCommand(nRelativeButton[3]);
    }
	else
	{
	    m_sButton = GetButtonCommand(3);
	}
	//USSF START
	USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	//return HandleDataNeeded();
	//USSF END
}

SMStateBase  *SMCmDataEntryBase::PSButton5(void)
{
	if(co.fOperationsCenterCustomerDataNeededButtons)
	{	// Need to determine first button
        m_sButton=GetButtonCommand(nRelativeButton[4]);
    }
	else
	{
	    m_sButton = GetButtonCommand(4);
	}
	//USSF START
	USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	//return HandleDataNeeded();
	//USSF END
}

SMStateBase  *SMCmDataEntryBase::PSButton6(void)
{
	if(co.fOperationsCenterCustomerDataNeededButtons)
	{	// Need to determine first button
        m_sButton=GetButtonCommand(nRelativeButton[5]);
    }
	else
	{
	    m_sButton = GetButtonCommand(5);
	}
	//USSF START
	USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	//return HandleDataNeeded();
	//USSF END
}

SMStateBase  *SMCmDataEntryBase::PSButton7(void)
{
	if(co.fOperationsCenterCustomerDataNeededButtons)
	{	// Need to determine first button
        m_sButton=GetButtonCommand(nRelativeButton[6]);
    }
	else
	{
	    m_sButton = GetButtonCommand(6);
	}
	//USSF START
	USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	//return HandleDataNeeded();
	//USSF END
}


////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSButtonGoBack(void) // cancel all
{
    // for customer mode, button7 in the ini determines the 
    // bottom right global navigation button
	m_sButton = GetButtonCommand(7);
	//USSF START
	USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
	//return HandleDataNeeded();
	//USSF END
}

//////////////////////////////////////////////////////
// PSNumericKey
//////////////////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}

//////////////////////////////////////////////////////
// PSAlphaKey
//////////////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSAlphaKey(const long lKey)
{
	ps.EchoInput((_TCHAR)lKey, true);	// allow alpha chars in input
	long lLen = ps.GetInput(csItemSent);
    return STATE_NULL;		  	    //remain in same state
}
SMStateBase  *SMCmDataEntryBase::PSShiftKey(void)
{

	if(bShiftOn)
	{
		bShiftOn = false;
	}
	else
	{
		bShiftOn = true;
	}
	
    return STATE_NULL;		  	    //remain in same state
}


SMStateBase  *SMCmDataEntryBase::PSAlphaNumericKey(const long lKey)
{
	long ch = lKey;
	if(fCapsInput)  // if input field formatting is 'all caps' 
	{
	  ch = toupper(lKey); 	
	}
	else if(bShiftOn) // if input field formatting is 'small caps' but shift is On
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
SMStateBase  *SMCmDataEntryBase::PSBackSpaceKey(void)
{
 
  PSClearInput();
  
  CString csValue;   // TAR 227930
  long lLen = ps.GetInput(csValue);	
  if (lLen == 0) 
     ps.ClearInput();


  return STATE_NULL;  	    //remain in same state
}

/////////////////////////////////////////
// PSClearKey
/////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSClearKey(void)
{
  PSClearAllInput();
  //PSClearInput();	//228461 - clear btn should clear entire input box, not one character at a time LPM021903

  CString csValue;   // TAR 227930
  long lLen = ps.GetInput(csValue);	
  if (lLen == 0) 
     ps.ClearInput();

  return STATE_NULL;
}

/////////////////////////////////////////
// PSSpaceKey
/////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSSpaceKey(void)
{
 //   CString csEntryData;
    ps.EchoInput((_TCHAR)(0x20), true);
//	long lLen = ps.GetInput(csEntryData);
	return STATE_NULL;
}

/////////////////////////////////////////
// PSEnterKey
/////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSEnterKey(void)// enter key pressed, sell this item
{
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
  USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
  //return HandleDataNeeded();
  //USSF END
}

SMStateBase  *SMCmDataEntryBase::PSOtherSpecialKeys(const long ch)
{
      ps.EchoDecimalInput((_TCHAR)(ch), true); // EchoDecimalInput used because EchoInput 
      long lLen = ps.GetInput(csItemSent);         // won't allow/display non-alphabetic and non-numeric chars 
      return STATE_NULL;
}

///////////////////////////////////
SMStateBase  *SMCmDataEntryBase::DMScanner()// scanned item
{
  m_sInput = csDMLastBarCode;
  //USSF START
  USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
  //return HandleDataNeeded();
  //USSF END
}

//////////////////////////////////
// TimedOut
//////////////////////////////////
SMStateBase  *SMCmDataEntryBase::TimedOut(void)
{
  return STATE_NULL;
}

//////////////////////////////////////////////////////////////
SMStateBase *SMCmDataEntryBase::OnWtIncreaseNotAllowed() //tar 197857
{
	if(fItemIsWaiting)			// Tar 211054 RJF 081402
		return STATE_NULL;		// ignore weight increase if 1st item scanned during attract
	else
		return VoidingWtIncrease(BES_CMDATAENTRY, BES_CMDATAENTRY);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCmDataEntryBase::OnWtDecrease() //tar 197857
{
	if (bIsInFinalization)   //We are at finialization stage
	{
		if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
			return ThresholdViolWtDecrease(BES_CMDATAENTRY, BES_CMDATAENTRY);
		else
			return STATE_NULL;
	}
	else
		return ThresholdViolWtDecrease(BES_CMDATAENTRY, BES_CMDATAENTRY);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCmDataEntryBase::OnMatchedWt()
{
	return STATE_NULL;
}
// TAR 272200
SMStateBase *SMCmDataEntryBase::OnBackToLGW(void)
{
	trace(L2, _T("SMCmDataEntryBase::OnBackToLGW. no need to process BackToLGW."));
	//If there is WaitForApproval state waiting, clear it
	if (g_pDelay_WaitForApprovalStateChange != NULL)
	{
		CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;

		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
		trace(L6,_T("OnBackToLGW - Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
	}

	return STATE_NULL;
}

SMStateBase *SMCmDataEntryBase::OnTBNeedData(void)
{
    trace (L6, _T("+SMStateTBBase::OnTBNeedData()"));
    CString sDescription, sView, sINI, sViewString, sSection, csStateName;
    //RFQ 2506 - rework OnTBNeedData logic for correct display of new RAPDataNeeded text overrides
    CString sRDNLaneMessage, sRDNLaneLeadthru, sRDNValues;  
    CString sViewCustomer;
    CSCOTDataEntry cCustomerDataEntry;
    CPSConfigData cd;
    long rc;
    
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
    
    if(ViewInfo.szDescription)
    {
        sDescription = ViewInfo.szDescription;
        try{ delete ViewInfo.szDescription;}
        catch(...){trace(L4,_T("Exception while deleting ViewInfo.szDescription"));}
    }
    
    if(ViewInfo.szView)
    {
        sView = ViewInfo.szView;
        sViewString = sView;
        sViewCustomer = sView;
        cd.FormatString(sViewString, true);
        cd.FormatString(sViewCustomer, false);
        
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
        {
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
        if (fSAInItemSale) //Tar 235531 FFR: Data Entry during Item Sale 
        {
            trace(L4, _T("GetDataNeededView cancel, item sale in progress got to InProgress state"));
            savebEASReEnableScanner = true;
            setAnchorState((BEST) nlaststate);
            CREATE_AND_DISPENSE(InProgress) (storeMode(), helpMode(),0);
        }
        else
        {			
            trace(L4, _T("GetDataNeededView cancel, INI and View are both empty, TB wants to cancel"));
            // Tar 214594 RJF 092502
            savebEASReEnableScanner = true;
            SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
            return setAndCreateAnchorState((BEST) nlaststate);
        }
    }
       
	
    // no StoreLogon parameter means we default to store mode
	cCustomerDataEntry.FromString(sViewCustomer); //429316/426288
    if (cCustomerDataEntry.IsStoreLogonEnabled() && !co.IsInAssistMenus())
    {
        
        //Build customer-language messages for RapDataNeeded screen.
        sRDNLaneMessage = cCustomerDataEntry.GetRDNLaneMessage();
        sRDNLaneLeadthru = cCustomerDataEntry.GetRDNLaneLeadthru();
        
        sRDNValues = _T("");
        if (sRDNLaneLeadthru != _T(""))
        {
            sRDNValues += _T("RDNLANELEADTHRU=") + sRDNLaneLeadthru + _T(";");
        }
        if (sRDNLaneMessage != _T(""))
        {
            sRDNValues += _T("RDNLANEMESSAGE=") + sRDNLaneMessage + _T(";");
        }
        // Data entry requires store mode
        // Force primary language for store mode/RAP data
        sViewString = sRDNValues + sViewString;
        
        trace (L6, _T("-SMCMDataEntryBase::OnTBNeedData() with configuration parameters %s"),sViewString);
        CREATE_AND_DISPENSE(RAPDataNeeded)(sDescription, DATANEEDED_MOREDATA, sViewString, storeMode());
    }
    else
        //-RFC 251336
    {
        sm_pDataNeededEntry.FromString(sViewCustomer);
        trace (L6, _T("-SMCmDataEntryBase::OnTBNeedData()"));
        return Initialize();
    }
    
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
SMStateBase *SMCmDataEntryBase::PSCmdList(CString csContext, CString csControl, const long lIndex)
{
	CString csEntryId;
	csEntryId.Format(_T("%d"),lIndex);
    trace(L6,_T("+SMCmDataEntryBase::PSCmdList(%s, %s, %d)"), csContext, csControl, lIndex);
    COleVariant vValue;
	m_pAppPSX->GetReceiptItemVariable(_T("DataNeededReceipt"), csEntryId, _T("ITEM_CMDDATA"), vValue);

	m_sInput = vValue.bstrVal;
	m_sButton=csControl;
  //USSF START
  USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
  //return HandleDataNeeded();
  //USSF END
}
*/
//////////////////////////////////
SMStateBase *SMCmDataEntryBase::DMCardReader(void)
{
  trace(L6,_T("+SMCmDataEntryBase::DMCardReader()"));

  USSFHOOK(_T("HandleDataNeeded"), nextState = HandleMSRData(), return nextState);
}

///////////////////////////////////////////
// GetButtonCommand
//   Return command for the button, taking
//   into account v
///////////////////////////////////////////
CString SMCmDataEntryBase::GetButtonCommand(int nButtonIndex)
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
