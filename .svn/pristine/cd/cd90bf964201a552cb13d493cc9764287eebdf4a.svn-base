//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCmDataEntryBase.CPP
//
// TITLE: Class implementation for customer mode data entry state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMInProgress.h"           // In progress state
#include "SMRapDataNeeded.h"
#include "PSConfigData.h"
#include "SMScanAndBag.h" // SR93.4
#include "CmFunctions.h"  // SR 93.4
#include "SMPickingUpItems.h"        //sscoadk-2652

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CmDataEntryBase")
#include "SMCmDataEntry.h"     
#else
#include "SMCmDataEntryBase.h"
#include "SMStateConstants.h"

#endif //_CPPUNIT
IMPLEMENT_DYNCREATE(SMCmDataEntryBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(CmDataEntry)

DEFINE_TIMESTAMP

SCOTDATAENTRY SMCmDataEntryBase::sm_pDataNeededEntry;
#ifndef _CPPUNIT
int  SMCmDataEntryBase::nlaststate = -1;
bool SMCmDataEntryBase::bInSmCmDataEntry = false; //TAR371194
bool SMCmDataEntryBase::ms_bRAPHasScanned = false; //SSCOP-3803
#endif //_CPPUNIT
//////////////////////////////////////////
SMCmDataEntryBase::SMCmDataEntryBase()
#ifndef _CPPUNIT
: csCurContext(EMPTY_STRING),
  m_HKStatus(PSProceduresBase::HK_NORMAL) //SR828
#endif //_CPPUNIT
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::Deliver(LPCTSTR pPtr)
{
    // This will setup the object properties and members with 
    // the data provided by the TB
    sm_pDataNeededEntry.FromString(pPtr);
	g_csDataNeededHandler = sm_pDataNeededEntry.GetDataHandler();
#ifndef _CPPUNIT
    g_csSecureCamInterventionType = sm_pDataNeededEntry.GetReportCategoryName();
#endif //_CPPUNIT
    trace (L2, _T("SMCmDataEntryBase bEASReEnableScanner is set to %d"),bEASReEnableScanner);
    return this;
}

#ifndef _CPPUNIT
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
    ms_bRAPHasScanned = false; //SSCOP-3803

    if(!ps.RemoteMode())
    {
        ra.RequestEnableAssistMode(false);
    }

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

    if(nTBLastRealState != TB_NEEDMOREDATA) //tar 252987
    {
        bEASReEnableScanner = true;
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
    
    //SR801+
    //bDNEnableSecurity will determine if security will be enable based on the the value of 
    // EnableSecurityInCMDataNeeded and the value of EnableSecurity
    //Security of a dataneeded screen will be enable if we have the ff:
    //1. EnableSecurityInCMDataNeeded=Y & EnableSecurity is not set
    //2. EnableSecurity=Y & regardless the value of EnableSecurityInCMDataNeeded
    //since EnableSecurity overrides the value of EnableSecurityInCMDataNeeded option
    bDNEnableSecurity = false;
    //IsSecurityEnable
    if(sm_pDataNeededEntry.IsSecurityEnable()==true)
    {
        bDNEnableSecurity = true;
    }

    trace(L6,_T("EnableSecurityInCMDataNeeded= %d, DataNeeded EnableSecurity = %d, DataNeeded during itemSale=%d" 
            ), co.fOperationsEnableSecurityInCMDataNeeded, sm_pDataNeededEntry.IsSecurityEnable(), fSAInItemSale);
    if (!co.IsAttendantModeOn())  //SSCOADK-1570
    {
        //disable security if bDNEnableSecurity is false 
        //or if dataneeded arrives during itemsale
        if( bDNEnableSecurity==false || fSAInItemSale)
        {
            //If security is disable, hold weight
            trace(L7,_T("CMDataEntry, Setting hold weight=true"));
            CKeyValueParmList parmList;
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_HOLD);
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            BSTR bstrResult;
            bstrResult = NULL;
            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
            SysFreeString(bstrResult);
        
        }
        else
        {

            trace(L7,_T("CMDataEntry, Setting hold weight=false"));
            CKeyValueParmList parmList;
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_RESUME);
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            BSTR bstrResult;
            bstrResult = NULL;
            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
            SysFreeString(bstrResult);
        }
        //SR801-
    }
    fCapsInput = false;
    if((csInputString != _T("")))
    {
        if (csInputString[0] == _TCHAR('A'))
        {
            fCapsInput = true;
        }
    }
    bShiftOn = false;
    bAltGrOn = false; //SR828
    m_HKStatus = PSProceduresBase::HK_NORMAL; // SR828
    
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
            {   // dynamic context based on #buttons
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
        {   // dynamic context based on #buttons
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
            //+SR828
            csCurContext = ps.GetKeyboardContextOfCurrentLang();
            trace(L6, _T("Loaded keyboard %s"), csCurContext);
            //-SR828
        }
        else
        { 
            csCurContext = csContextToDisplay;
            //ps.SetCMFrame(csContextToDisplay); //commented 7/10/2009 tar 408235
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
        ps.SetCMFrame (csCurContext);
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

    //+SR828
    if(csCurContext.Left(8).Compare(_T("Keyboard")) == 0)
    {
        m_pAppPSX->SetConfigProperty(_T("InstructionScreenTitle"), csCurContext, UI::PROPERTYTEXT, sm_pDataNeededEntry.GetTopCaption());
        m_pAppPSX->SetConfigProperty(_T("Instruction_Line1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);

        SetAlphaShiftState(csCurContext, bShiftOn);

        ps.PopulateHotKeys(csCurContext);
        ps.PopulateHKSensitiveControls(csCurContext, m_HKStatus, bShiftOn, bAltGrOn);
    }
    //-SR828
    else
    {
        ps.SetCMLeadthruText(sm_pDataNeededEntry.GetTopCaption());
    }

    // See if they want the Total to be displayed
    if(sm_pDataNeededEntry.IsTotalHidden())
        ps.ShowCMTotal(false,false);   // hide the line at bottom with total
    else
        ps.ShowCMTotal(false,true);
    ps.ShowCMTBText(csTBMessage2Scot);  //thp

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
        {   // failure so use original font for CMButtonXMidiList
            trace(L4, _T("ButtonFont %s not found"),csButtonFont);
            vFont = defaultFont;
        }
    }
    else
    {   // setup default font if buttonfont parameter not used
        vFont = defaultFont;
    }
    CString strControlNameArray1[8]; //rfc375568 & rfc375777
    CString strControlNameArray2[8]; //rfc375568 & rfc375777
    nButtons=0;
    for (i=0; i<7; i++)
    {
        csButtonControlName = EMPTY_STRING;
        if(co.fOperationsCenterCustomerDataNeededButtons)
        {   // only interesed in centering buttons - not maintaining specific button positions
            csButtonControlName.Format (_T("%s%d%s"), _T("CMButton"), (nButtons+1) , _T("MidiList") );
            if(sm_pDataNeededEntry.GetButtonText(i) !=_T(""))
            {
                ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), true);
                nRelativeButton[nButtons]=i;         
                strControlNameArray1[nButtons] = csButtonControlName;//rfc375568 & rfc375577
                trace( L6, _T("strControlNameArray1[%d] is %s)"), nButtons,
                        strControlNameArray1[nButtons]);
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
            {   // using button parameter setting to determine position - no centering
                ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), true);
                strControlNameArray2[i] = csButtonControlName;//rfc375568 & rfc37577
                trace( L6, _T("strControlNameArray1[%d] is %s)"), i,
                    strControlNameArray2[i]);
                rc = m_pAppPSX->SetConfigProperty(csButtonControlName, csCurContext, UI::PROPERTYFONT, vFont);
                if(PSX_SUCCESS != rc)
                    trace(L4, _T("Failure Setting Button Font for %s, err=%x"),csCurContext,rc);
            }
            else
                ps.Button(csButtonControlName, sm_pDataNeededEntry.GetButtonText(i), false);
        }
    }

    ps.ClearInput();//clear echo field

    //RFQ 2506 - Build CmdButton and CmdList data if necessary
    BuildCmdButtonList(sm_pDataNeededEntry, csCurContext, _T("CmdBtn_DataEntryList") );
    BuildCmdList(sm_pDataNeededEntry, _T("DataNeededReceipt"));

	// start RFQ 9545
	if ((csCurContext == _T("mPerksEntry")) && (g_bInRecallMode))
	{
		trace(L6, _T("Skipping Showframe since we are in recall mode FLMS CurrentContext %s"), csCurContext );
	}
	else
	{
     ps.ShowFrame(false);        
	}
	// end RFQ 9545
 
    //Use DMScannerEnable() and DMScannerDisable to avoid manipulating the bEASReEnableScanner flag.
    //This scanner enable/disable is outside of the normal Scan-to-EnterItem or Scan-to-Login flow.
    if(sm_pDataNeededEntry.IsScannerInputEnabled())
    {
        DMScannerEnable();        // enable the scanner
    }
    else                            // Tar 210186 RF 071902
        DMScannerDisable();     // disable scanner if ini parameter set to 0 or not present

    if(sm_pDataNeededEntry.IsMSRInputEnabled())   //RFQ 2605 - MSR Input from DataNeeded
    {
        DMMSREnable();        // enable the MSR
    }
    else                            // Tar 210186 RF 071902
        DMMSRDisable();     // disable MSR if ini parameter set to 0 or not present


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
            bool bHideInput = sm_pDataNeededEntry.IsInputHidden();  // Tar 230459
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
                ps.AllowInput(iInputLength,bHideInput,true, iInputMinLen);  // Tar 230459
            else
                ps.AllowInput(iInputLength,bHideInput,false, iInputMinLen); // Tar 230459
        }
    }


    // Check if they have a specific sound file to play
    csSayPhrase = sm_pDataNeededEntry.GetSayPhrase();
    if(csSayPhrase.IsEmpty())
        DMSayPhrase(INFORMATIONNEEDED);
    else
        DMSayPhrase(csSayPhrase);
    
    //+RFC375577 & RFC375568
    if ( co.GetfAutoRunOn() )
    {
        long wParm = 0;
        CString sAutoRunResponse = sm_pDataNeededEntry.GetAutoRunResponse();
        CString csButtonCtrlNameTemp;
        int nButtonID = sm_pDataNeededEntry.GetButtonID(sAutoRunResponse);
        trace( L6, _T("AutoRunResponse is %d for %s)"), nButtonID, 
        sAutoRunResponse );

        if ( ( nButtonID > -1 ) && ( nButtonID < 4 ) ) //a customer button
        {
            if(co.fOperationsCenterCustomerDataNeededButtons)
            {   
                csButtonCtrlNameTemp = strControlNameArray1[nButtonID];
            }
            else 
            {
                csButtonCtrlNameTemp = strControlNameArray2[nButtonID]; 
            }

            (ps.GetPSXObject())->GenerateEvent(csButtonCtrlNameTemp, 
            ps.GetCurrentContext(), UI::EVENTCLICK, COleVariant(wParm));    
        }
        else if ( ( nButtonID >= 4 ) && ( nButtonID < 8 ) ) //base button
        {
            trace(L6, _T("Cancelling request because data entry AutoRunResponse is %s"),
            sm_pDataNeededEntry.GetButtonText(nButtonID) );
            return PSButtonGoBack();//cancel/decline/go back
        }
        else
        {
            m_sInput = sAutoRunResponse;
            return HandleDataNeeded();
        }
    }//-RFC375577 & RFC375568

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

  //SSCOADK-493
  if (!g_bTBNeedMoreData)
	  setAnchorState(nlaststate); 

  DMScannerDisable();  //Always disable.  Initialize will disable if scanner input not configured. 

  bInSmCmDataEntry = false; //TAR371194 

  bDataNeededSecurity = false;
  if(!fInMultiSelectPickList)
  {
      TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CmDataEntry") );
  }
  if (!co.IsAttendantModeOn())  //SSCOADK-1570
  {
      //SR801+
      //Clear hold weight since it was hold at initialize if bDNEnableSecurity=false
      if(bDNEnableSecurity == false && !fSAInItemSale && !bIsInFinalization)  //SSCOADK-1675
      {
        CKeyValueParmList parmList;
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_RESUME);
        _bstr_t bstrParmList((const _TCHAR *)parmList);
        BSTR bstrResult;
        bstrResult = NULL;
        SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
        SysFreeString(bstrResult);
      }
    //SR801-
  }

   //RFQ 2506 - Clear CmdBtn list and CmdList controls
  COleVariant v = (long) 0;
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
    {   // Need to determine first button
        m_sButton=GetButtonCommand(nRelativeButton[0]);
    }
    else
    {
        m_sButton = GetButtonCommand(0);
    }
    // 227275 change saved enable since the item was canceled LPM021103
    if (m_sButton == _T("cReturn")) {   bEASReEnableScanner = true; }
    //USSF START
    USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
    //return HandleDataNeeded();
    //USSF END
}

SMStateBase  *SMCmDataEntryBase::PSButton2(void)
{
    if(co.fOperationsCenterCustomerDataNeededButtons)
    {   // Need to determine second button
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
    {   // Need to determine first button
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
    {   // Need to determine first button
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
    {   // Need to determine first button
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
    {   // Need to determine first button
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
    {   // Need to determine first button
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

//////////////////////////////////////////////////////
// PSAlphaKey
//////////////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSAlphaKey(const long lKey)
{
    ps.EchoInput((_TCHAR)lKey, true);   // allow alpha chars in input
    long lLen = ps.GetInput(csItemSent);
    return STATE_NULL;              //remain in same state
}

SMStateBase  *SMCmDataEntryBase::PSShiftKey(void)
{
    bShiftOn=!bShiftOn;

    //+SR828
    if(csCurContext.Left(8).Compare(_T("Keyboard")) == 0)
    { 
        SetAlphaShiftState(csCurContext, bShiftOn);

        ps.PopulateHKSensitiveControls(csCurContext, m_HKStatus, bShiftOn, bAltGrOn);
    }
    //-SR828
    
    return STATE_NULL;              //remain in same state
}

//+SR828
/////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSAltGrKey(void)
{
    bAltGrOn=!bAltGrOn;

    if(csCurContext.Left(8).Compare(_T("Keyboard")) == 0)
    {
        ps.PopulateHKSensitiveControls(csCurContext, m_HKStatus, bShiftOn, bAltGrOn);
    }

    return STATE_NULL;
}
//-SR828

//+SR828
//************************************************************************ 
//
// Function:        PSHotKey
// Purpose:         This function handles the button clicks from the hotkeys control
// Parameters:    [in] CString & csHotKey - The name of the hot key that has been clicked
// Returns:       SMStateBase  * 
// Calls:         PopulateHKSensitiveControls
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase  *SMCmDataEntryBase::PSHotKey(CString & csHotKey) 
{
    PSProceduresBase::HKStatus newStatus=PSProceduresBase::HK_NORMAL;
    if(csHotKey==_T("HKAcute"))
    {
        newStatus=PSProceduresBase::HK_ACUTEON;
    }
    else if(csHotKey==_T("HKGrave"))
    {
        newStatus=PSProceduresBase::HK_GRAVEON;
    }
    else if(csHotKey==_T("HKCircumflex"))
    {
        newStatus=PSProceduresBase::HK_CIRCUMFLEXON;
    }
    else if(csHotKey==_T("HKUmlaut"))
    {
        newStatus=PSProceduresBase::HK_UMLAUTON;
    }
    else if(csHotKey==_T("HKTilde"))
    {
        newStatus=PSProceduresBase::HK_TILDEON;
    }

    COleVariant vIndex(0L), vReturn;

    if(m_HKStatus==newStatus)
    {
        m_HKStatus=PSProceduresBase::HK_NORMAL;
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
            if(m_HKStatus == PSProceduresBase::HK_NORMAL)
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

        ps.PopulateHKSensitiveControls(csCurContext, m_HKStatus, bShiftOn, bAltGrOn);
    }

    return STATE_NULL;//remain in same state
}
//-SR828

//+SR828
//************************************************************************ 
//
// Function:        PSCharKey
// Purpose:         This function handles the button clicks from the alphanumeric keys
// Parameters:    [in] TCHAR c - The character on the alphanumeric key that has been clicked
// Returns:       SMStateBase  * 
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
SMStateBase  *SMCmDataEntryBase::PSCharKey(TCHAR c)
{
   ps.EchoInput(c, true, false);

   return STATE_NULL;//remain in same state
}
//-SR828

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
    ps.EchoInput((_TCHAR)ch, true); // allow alpha chars in input
    long lLen = ps.GetInput(csItemSent);
    return STATE_NULL;              //remain in same state
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


  return STATE_NULL;        //remain in same state
}

/////////////////////////////////////////
// PSClearKey
/////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSClearKey(void)
{
  //+SR828
  if(csCurContext.Left(8).Compare(_T("Keyboard")) == 0) //work as backspace
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

/////////////////////////////////////////
// PSSpaceKey
/////////////////////////////////////////
SMStateBase  *SMCmDataEntryBase::PSSpaceKey(void)
{
 //   CString csEntryData;
    ps.EchoInput((_TCHAR)(0x20), true);
//  long lLen = ps.GetInput(csEntryData);
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

#endif //_CPPUNIT
///////////////////////////////////
SMStateBase  *SMCmDataEntryBase::DMScanner()// scanned item
{
  // +SR93.4
  if(1 == sm_pDataNeededEntry.GetScannerDataFormat())  //Allow Scans for 0 and 2 ScannerDataFormat
  {
     trace(L6,_T("ERROR: Was expecting a 2D barcode type!"));
     DMSayPhrase(UNKNOWNITEM);
     DMScannerEnable();
     return STATE_NULL;
  }
  // -SR93.4

  //+-SSCOADK-6429
  m_sInput = cDMLastData;
  //USSF START
  USSFHOOK(_T("HandleDataNeeded"), nextState = HandleDataNeeded(), return nextState);
  //return HandleDataNeeded();
  //USSF END
}

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
// +SR 93.4
SMStateBase  *SMCmDataEntryBase::DMScannerEncrypted()// scanned item  
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
      DMSayPhrase(UNKNOWNITEM);
      DMScannerEnable();
      return STATE_NULL;

  }


}
// -SR 93.4
#ifndef _CPPUNIT

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
    if(fItemIsWaiting)          // Tar 211054 RJF 081402
        return STATE_NULL;      // ignore weight increase if 1st item scanned during attract
    else
    {
        //SR801+
        //
        if(fSAInItemSale || bDNEnableSecurity == false)
        {
            SMStateBase::bDataNeededSecurity = false;
            trace(L7,_T("OnWtIncreaseNotAllowed dont allow immediate security"));
        }
        else
        {
            trace(L7,_T("OnWtIncreaseNotAllowed allow immediate security"));
            SMStateBase::bDataNeededSecurity = true;
        }
        //SR801-

        return VoidingWtIncrease(BES_CMDATAENTRY, BES_CMDATAENTRY);
    }
}

#endif

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMCmDataEntryBase::OnWtDecrease() //tar 197857
{
    if (bIsInFinalization)   //We are at finialization stage
    {
#ifndef _CPPUNIT

        if((!g_bIsForgiveUnexpectedDecreaseDuringTendering 
            //SR801+
            //if options are set to Y even if ForgiveUnexpectedDecreaseDuringTendering
            //is set to false, then its possible to show intervention immediately
            ||(bDNEnableSecurity == true))
            //SR801-
            && !(lBalanceDue < 0.01))
        {
            //SR801+
            if(bDNEnableSecurity == true)
            {
                SMStateBase::bDataNeededSecurity = true;
                trace(L7,_T("OnWtDecrease allow immediate security"));
            }
            else
            {
                trace(L7,_T("OnWtDecrease don't allow immediate security"));
                SMStateBase::bDataNeededSecurity = false;
            }
            //SR801-

            return ThresholdViolWtDecrease(BES_CMDATAENTRY, BES_CMDATAENTRY);
        }
        else
            return STATE_NULL;
#endif
    }
    else
    {
        //SR801+
        if(fSAInItemSale || bDNEnableSecurity == false)
            {
                //if in ItemSale
                //or security is enable 
                SMStateBase::bDataNeededSecurity = false;
                trace(L7,_T("OnWtDecrease don't allow immediate security"));
            }
            else
            {
                trace(L7,_T("OnWtDecrease allow immediate security"));
                SMStateBase::bDataNeededSecurity = true;
   				//sscoadk-2652+
				if (!fSAInItemSale)
				{
					
		            RETURNSTATE(PickingUpItems)
				}
				//sscoadk-2652-
            }
            //SR801-

#ifndef _CPPUNIT
        return ThresholdViolWtDecrease(BES_CMDATAENTRY, BES_CMDATAENTRY);
#else
        return STATE_NULL;
#endif
    }
}
#ifndef _CPPUNIT

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
    trace (L6, _T("+SMCmDataEntryBase::OnTBNeedData()"));
    CString sDescription, sView, sINI, sViewString, sSection, csStateName;
    //RFQ 2506 - rework OnTBNeedData logic for correct display of new RAPDataNeeded text overrides
    CString sRDNLaneMessage, sRDNLaneLeadthru, sRDNValues;  
    CString sViewCustomer;
    CSCOTDataEntry cCustomerDataEntry;
    CPSConfigData cd;
       
    bool bGotDataNeededInfo = GetDataNeededViewFromTB(sDescription, 
                                                      sView,
                                                      sINI,
                                                      sSection);

    if(!bGotDataNeededInfo)
    {
        trace(L4, _T("GetDataNeededViewFromTB failed"));
        return STATE_NULL;
    }
    
    if(!sView.IsEmpty())
    {
        sViewString = sView;
        sViewCustomer = sView;
        cd.FormatString(sViewString, true);
        cd.FormatString(sViewCustomer, false);
    }


    if(!sINI.IsEmpty())
    {
        if(!sSection.IsEmpty())
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
    else if(sView.IsEmpty() )
    {
        if (fSAInItemSale) //Tar 235531 FFR: Data Entry during Item Sale 
        {
            trace(L4, _T("GetDataNeededView cancel, item sale in progress got to InProgress state"));
            bEASReEnableScanner = true;
            setAnchorState((BEST) nlaststate);
            CREATE_AND_DISPENSE(InProgress) (storeMode(), helpMode(),0);
        }
        else
        {           
            trace(L4, _T("GetDataNeededView cancel, INI and View are both empty, TB wants to cancel"));
            // Tar 214594 RJF 092502
            bEASReEnableScanner = true;
            SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
            return setAndCreateAnchorState((BEST) nlaststate);
        }
    }
       
    // no StoreLogon parameter means we default to store mode
    cCustomerDataEntry.FromString(sViewCustomer);
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
        sViewString = sViewString + sRDNValues;
        
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
SMStateBase *SMCmDataEntryBase::OnRAPUnsolicitedDataReceived(LPCTSTR sData)
{
    CString csData = sData;
    bool bSavefItemIsWaiting = fItemIsWaiting;
    bool bSavefInItemSale = fSAInItemSale;
    bool bRestore = false;

    trace(L7, _T("-SMCmDataEntryBase::OnRAPUnsolicitedDataReceived  <%s>"), csData);

    if (csData.Find(_T("cScanData")) != -1)
    {
        trace(L6, _T("-SMCmDataEntry::OnRAPUnsolicitedDataReceived  Inside cScanData"));
        bool bIsScannerAllowed = sm_pDataNeededEntry.IsScannerInputEnabled() && !ms_bRAPHasScanned;
        if ( bIsScannerAllowed && (fItemIsWaiting || fSAInItemSale) )
        {
            trace(L6, _T("-SMCmDataEntryBase::OnRAPUnsolicitedDataReceived  Inside IsScannerAllowed"));
            ms_bRAPHasScanned = true;
            fItemIsWaiting = false;
            fSAInItemSale = false;
            bRestore = true;
        }
    }

    SMStateBase* pState = STATE(State)::OnRAPUnsolicitedDataReceived( sData);

    if (bRestore)
    {
        fItemIsWaiting = bSavefItemIsWaiting;
        fSAInItemSale = bSavefInItemSale;
    }

    return pState;
}
//-SSCOP-3803
#endif //_CPPUNIT

SMStateBase *SMCmDataEntryBase::HandleScannerData(const MessageElement* message)
{
    SMStateBase *sReturnState = STATE(State)::HandleScannerData(message);   
    if(sm_pDataNeededEntry.IsHelpHidden())
    {
        if ( getRealStatePointer(sReturnState) != NULL )    
        { 
            trace(L6, _T("Base is not returning NULL")); 
            CString csStateName = sReturnState->GetRuntimeClass()->m_lpszClassName;
            if (csStateName.Find(_T("SmAuthorization")) != -1 )
            {
                trace(L6, _T("ignore this barcode, it must be an operator barcode"));
                delete sReturnState;
                DMScannerEnable();
                return STATE_NULL;
            }
        }
    }
    return sReturnState; 
}

