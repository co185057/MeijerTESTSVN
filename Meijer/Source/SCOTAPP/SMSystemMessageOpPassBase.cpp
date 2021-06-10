//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMSystemMessageOpPassBase.cpp
//
// Description: A base state for system messages that require an operator and
//   password to see them.  Note that this state is essentially both a
//   SMSystemMessage state and a SMOperatorPassword state.  Multiple inheritance
//   was not used because both those states inherit from SMHelpModeState,
//   leading to ambiguities, etc.  Thus a SMSystemMessageOpPassBase state inherits
//   from SMOperatorPassword state and has a SMSystemMessage.  Comment added for TAR122724
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include "LaneStateData.h"
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSystemMessageOpPass.h"
#include "DMProcedures.h"
/******cm150000: Added In Merged Code******/
#include "MPProcedures.h"
/******cm150000: End Merged Code***********/

DLLEXPORT extern DMProcedures dm;             // global DM object

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessageOpPassBase")

/******cm150000: Added In Merged Code******/
DLLEXPORT extern MPProcedures mp;
/******cm150000: End Merged Code***********/

IMPLEMENT_DYNCREATE(SMSystemMessageOpPassBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP


////////////////////////////////////////////
// Constructor
////////////////////////////////////////////
SMSystemMessageOpPassBase::SMSystemMessageOpPassBase(BMPS   bitmap,
                                             PSTEXT title,       // PSTEXT title number or PS_NOTEXT
                                             bool   continueButton,
                                             bool   cancelButton,
                                             LPCTSTR szMessage,   // string text message or NULL
                                             PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                             PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
		                                         long   promptValue, // prompt area sale value
                                             long   deviceClass, // device class, -1 for none
                                             bool   checkHealth, //DMCheckHealth after msg? 
											 bool displayHTML )  //SR672
// Create a SMSystemMessage from the parms given us.
: theSystemMessage(bitmap,
                   title,
                   continueButton,
                   cancelButton,
                   szMessage,
                   psMessage,
                   promptText,
                   promptValue,
                   deviceClass,
                   checkHealth,
				   false,
				   false,
				   false,
				   (BMP_ERRMSGLEVEL1 == bitmap)?true:false,
				   displayHTML)
{
	IMPLEMENT_TIMESTAMP
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
	/******cm150000: Added In Merged Code******/
	csVerificationState = _T("N");
	/******cm150000: End Merged Code***********/
}

////////////////////////////////////////
SMSystemMessageOpPassBase::SMSystemMessageOpPassBase()
{
  IMPLEMENT_TIMESTAMP
	/******cm150000: Added In Merged Code******/
	csVerificationState = _T("N");
	/******cm150000: End Merged Code***********/
}

////////////////////////////////////////
SMStateBase  *SMSystemMessageOpPassBase::Deliver(BMPS   bitmap,
                                             PSTEXT title,       // PSTEXT title number or PS_NOTEXT
                                             bool   continueButton,
                                             bool   cancelButton,
                                             LPCTSTR szMessage,   // string text message or NULL
                                             PSTEXT psMessage,   // PSTEXT message number or PS_NOTEXT
                                             PSTEXT promptText,  // PSTEXT prompt number or PS_NOTEXT
		                                         long   promptValue, // prompt area sale value
                                             long   deviceClass, // device class, -1 for none
                                             bool   checkHealth,
											 bool displayHTML) //SR672
{
    InitSystemMessage(bitmap,
               title,
               continueButton,
               cancelButton,
               szMessage,
               psMessage,
               promptText,
               promptValue,
               deviceClass,
               checkHealth,
			   displayHTML);

  return this;
}
/////////////////////////////////////////
bool SMSystemMessageOpPassBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    
    // There should be ten parameters, a variety of types
    if (dlParameterList.GetDescriptorCount() == 10)
    {
        trace(L6, _T("+SMSystemMessageOpPassBase::DeliverDescriptorList() - Parameter Count of 10"));
        Deliver((BMPS)DLParamDWORD(dlParameterList),    // bitmap
                (PSTEXT)DLParamDWORD(dlParameterList),  // title
                (bool)DLParamWORD(dlParameterList),     // continueButton,
                (bool)DLParamWORD(dlParameterList),     // cancelButton,
                (LPCTSTR)DLParamCString(dlParameterList), // szMessage
                (PSTEXT)DLParamDWORD(dlParameterList),  // psMessage
                (PSTEXT)DLParamDWORD(dlParameterList),  // promptText
                DLParamDWORD(dlParameterList),          // promptValue
                DLParamDWORD(dlParameterList),          // DeviceClass
                (bool)DLParamWORD(dlParameterList));    // checkHealth
        return true;
    }
    else
    {
        trace(L2, _T("+SMSystemMessageOpPassBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


void SMSystemMessageOpPassBase::InitSystemMessage( BMPS   bitmap,
                         PSTEXT title,       // specify PS_NOTEXT for default
                         bool   continueButton,
                         bool   cancelButton,
                         LPCTSTR szMessage,   // NULL if psMessage specified
                         PSTEXT psMessage,   // PS_NOTEXT if szMessage specified
                         PSTEXT promptText,  // PS_NOTEXT for no prompt
		                 long   promptValue, // prompt area sale value
                         long   deviceClass, // device class, -1 for none
                         bool   checkHealth, // DMCheckHealth after msg?
						 bool displayHTML) //SR672
{
  theSystemMessage.Deliver(bitmap,
                   title,
                   continueButton,
                   cancelButton,
                   szMessage,
                   psMessage,
                   promptText,
                   promptValue,
                   deviceClass,
                   checkHealth,
				   false,
				   false,
				   false,
				   (BMP_ERRMSGLEVEL1 == bitmap)?true:false,
				   displayHTML);

}


////////////////////////////////////////
// Initialize
////////////////////////////////////////
SMStateBase * SMSystemMessageOpPassBase::Initialize()
{
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SystemMessageOpPass") );
	}
    // Begin TAR 206353
    // CANNOT enter Remote Assist Mode at this state
    //ra.OnSetTransactionState(RA_STATE_ASSISTNOTOK);
    //+dp185016 permit state change from systemmessage to waitforremoteassistance 
    // if out of sync need assist
    if (!(ps.GetPSText( MSG_POS_TB_OUTOFSYNC_NEEDASSISTMODE, SCOT_LANGUAGE_PRIMARY ) == theSystemMessage.csMessage))
    {
        //-dp185016
        ra.RequestEnableAssistMode(false);   //TAR 213249
        // End TAR 206353
    }

	/******cm150000: Added In Merged Code******/
	csVerificationState = _T("N");
	/******cm150000: End Merged Code***********/
  nTimeOut = theSystemMessage.getTimeout();
  m_bNeedToDisplayAuxScreen = false;	// set this to false to keep the aux screen from displaying
  if (dm.IsOpen())                //TAR#125161 might try to display error msg when dm failed to open
  {
      // (*) SR898 ====================
      DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                         DM_TRICOLORLIGHT_BLINK_1HZ,
                         DM_TRICOLORLIGHT_NORMALREQUEST);
  }
  DMSaySecurity( SEC_LOW, _T("blank.wav") );
  return STATE(OperatorPasswordState)::Initialize();
}

/////////////////////////////////////////////////////////////
// showScreen
/////////////////////////////////////////////////////////////
void SMSystemMessageOpPassBase::showScreen(bool passwordVerified)
{
  SAWLDBScannerEnable();		// enable the scanner

  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;

  if ( !passwordVerified)
  {
     if (inputMode == PROMPTING_FOR_OPERATOR)
     {
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
        
		STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics support		
		ps.SetCMLeadthruText( TXT_ENTER_ID );
		ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
		ps.ShowCMFrame( false );   
				
		return;
     }
     else
     {  // Display System Message
        ps.SetCMFrame(_T("SystemMessageOpPass"));
        ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
        ps.SetButtonText(_T("CMButton1StoreLogIn"),PS_WORK1);
     }
     // NewUI
  }
  else
  {
	SAWLDBScannerDisable();		// disable the scanner	
    theSystemMessage.showScreen(passwordVerified);
  } 

  if (!passwordVerified) 
  {
    ps.LeadThruText(LTT_WAITFORAPP);

    CString msgText;
    msgText = ps.GetPSText(MSG_WAITFORMESSAGE);
    msgText += "\n* ";
    msgText += ps.GetPSText(TXT_SYSTEMMESSAGE);
    ps.Message12(msgText, true);
	SAWLDBScannerDisable();		// disable the scanner	

  }
  
  if( lChangeDue > 0 )	// Tar 381865
	{ ps.ShowChangeDueAmountPaid( lChangeDue, lTotalPaid ); }
  else
	{ ps.ShowAmountDueAmountPaid(lBalanceDue - nDMCashInserted,lTotalPaid + nDMCashInserted); }//SSCOP-3525
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.ShowCMFrame(false);
}

///////////////////////////////////////////////
// PSButton 1 - Continue
///////////////////////////////////////////////
SMStateBase * SMSystemMessageOpPassBase::PSButton1(void)
{
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)   //TAR#139217 when getting wrong id & pswd, return to id entry
  {
    STATE(OperatorPasswordState)::promptForOperator();  
	return STATE_NULL;
  }
  else
  {
	if(dm.fStateTakeawayBelt)	// is on a take away system
	{
		TABTransportClearedItem();	// consolidate dup lines into one procedure LPM090502
	}

   	return theSystemMessage.PSButton1();
  }
}

///////////////////////////////////////////////
// PSButton 2 - Continue
///////////////////////////////////////////////
SMStateBase * SMSystemMessageOpPassBase::PSButton2(void)
{
  if (parentClassCurrentlyHandlingButton2())
    return STATE(OperatorPasswordState)::PSButton2();
  else
  {
	if(dm.fStateTakeawayBelt)	// is on a take away system
	{
		TABTransportClearedItem();	// consolidate dup lines into one procedure LPM090502
	}

	return theSystemMessage.PSButton2();
  }
}

///////////////////////////////////////////////
// PSButton 3 - Cancel
///////////////////////////////////////////////
SMStateBase * SMSystemMessageOpPassBase::PSButton3(void)
{
	return theSystemMessage.PSButton3();
}

/////////////////////////////////////////////
// PSCancelKey
/////////////////////////////////////////////
SMStateBase  *SMSystemMessageOpPassBase::PSCancelKey()
{
  if (parentClassCurrentlyHandlingCancelKey())
    return STATE(OperatorPasswordState)::PSCancelKey();
  else
    return theSystemMessage.PSCancelKey();
}

//////////////////////////////////////////////////////
SMStateBase * SMSystemMessageOpPassBase::stateAfterPasswordVerify()
{ 
    CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendLaneStateData(_T(""), _T(""), false, true);
    //laneStateData.SendNextGenUIData(_T("NextGenUIApplicationState"), (CString) laneStateData.STORE);

    //sscoadk-5351+
    trace(L6, _T("+ra.OnEnterStoreMode in SMSystemMessageOpPassBase::stateAfterPasswordVerify"));
    ra.OnEnterStoreMode(GetOperator(), GetPassword());
    trace(L6, _T("-ra.OnEnterStoreMode"));
    //sscoadk-5351-
    showScreen(true);
    return STATE_NULL;
 
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSystemMessageOpPassBase::OnBackToLGW(void)
{
	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();
	return STATE_NULL;
}

//////////////////////////////////////////////////////////////
SMStateBase *SMSystemMessageOpPassBase::OnWtIncreaseNotAllowed() //tar 197857
{
	return VoidingWtIncrease(BES_SYSMSGOPPASS, BES_SYSMSGOPPASS);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSystemMessageOpPassBase::OnWtDecrease() //tar 197857
{
	return ThresholdViolWtDecrease(BES_SYSMSGOPPASS, BES_SYSMSGOPPASS);
}

///////////////////////////////////////////////////////////////////////////
void SMSystemMessageOpPassBase::UnInitialize(void)      // TAR203166
{
    theSystemMessage.g_csButtonText.Empty();
	m_nDMSavedDev = -1;   //277025
	SMOperatorPasswordStateBase::UnInitialize();
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SystemMessageOpPass") );
	}
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMSystemMessageOpPassBase::OnMatchedWt()
{
	//If there is WaitForApproval state waiting, clear it
	if (g_pDelay_WaitForApprovalStateChange != NULL)
	{
		CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
		trace(L6,_T("Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
	}
	if(dm.fStateTakeawayBelt)	// is on a take away system, always re-enable the scanner
	{
		trace(L2, _T("bEASReEnableScanner is set to true."));
		bEASReEnableScanner = true;
		if (g_bTBReturnedITEMSOLDforSA)	// if the TB has returned ITEM_SOLD 
		{
			setAnchorState(BES_TRANSPORTITEM);
		}
	}
	else
		setAnchorState(BES_SCANANDBAG);

	return STATE_NULL;
}


//+307389
long SMSystemMessageOpPassBase::GetDeviceClass(void)
{
	return theSystemMessage.GetDeviceClass();

}
//-307389


