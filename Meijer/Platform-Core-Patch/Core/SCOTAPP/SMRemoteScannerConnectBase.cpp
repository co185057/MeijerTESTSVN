//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMRemoteScannerConnectBase.cpp
//
// Description: A SMRemoteScannerConnectBase accepts/rejects remote scanner connect requests.  An
//   operator/password is required to see this one.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMRemoteScannerConnect.h"   // Header file

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("RemoteScannerConnectBase")

IMPLEMENT_DYNCREATE(SMRemoteScannerConnectBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(RemoteScannerConnect)

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
// Constructor
// sys msg 6 is level 3 with op/passwd
/////////////////////////////////////////////////////
SMRemoteScannerConnectBase::SMRemoteScannerConnectBase
                      (bool bStoreMode)  // DMCheckHealth after msg?
{
  m_bStoreMode = bStoreMode;
  IMPLEMENT_TIMESTAMP
}
////////////////////////////////////////
SMRemoteScannerConnectBase::SMRemoteScannerConnectBase()
{
  m_bStoreMode = false;
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMRemoteScannerConnectBase::~SMRemoteScannerConnectBase()
{
}

////////////////////////////////////////
SMStateBase  *SMRemoteScannerConnectBase::Deliver(bool bStoreMode)
{
  m_bStoreMode = bStoreMode;
  return this;
}
 
SMStateBase * SMRemoteScannerConnectBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("RemoteScannerConnect") );	// RFC 330050
  if ((co.IsLoginBypassOn() && IsLoginBypassAllowed()) || storeMode())
  {
      inputMode = OPERATOR_PASSWORD_ACCEPTED;
      rp.SetOperator(co.GetAttendantID());
      return stateAfterPasswordVerify();
  }
  return STATE(OperatorPasswordState)::Initialize();

}

void SMRemoteScannerConnectBase::UnInitialize()
{
	// TAR:164390
	// leaving this state
	// inform security that SCOT is returning to non-secure mode
    ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn()); //tar 227613 AH 02/13/03
    STATE(OperatorPasswordState)::UnInitialize();
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("RemoteScannerConnect") );	// RFC 330050
}

/////////////////////////////////////////
// PSButton 1 - Store login
/////////////////////////////////////////
SMStateBase *SMRemoteScannerConnectBase::PSButton1(void) // Store Log In
{
  if (inputMode == OPERATOR_PASSWORD_ACCEPTED)
  {
      return PSButton3();  //Button1 in store mode is Accept
  }
  promptForOperator();
  return STATE_NULL;
}
/////////////////////////////////////////
// PSButton 2 - Cancel
/////////////////////////////////////////
SMStateBase  *SMRemoteScannerConnectBase::PSButton2(void)
{
    trace(L6, _T("SMRemoteScannerConnectBase::PSButton2"));
    if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
    {
        return STATE(OperatorPasswordState)::PSButton2();
    }
    //Save current value of Attendant mode,
    // Save orig Attendant mode setting for comparision 
    bool bAttendantMode = co.IsAttendantModeOn();
    //Cancel connection accepted in DMParse
    co.SetRemoteScannerConnected(false);
    //Check if Attendant mode is now false because it was entered using HH Scanner.
    if (!co.IsAttendantModeOn() == bAttendantMode)  
    {
        trace(L6, _T("Remote scanner disconnect exiting attendant mode"));
        ps.SMButton(_T("SMButton7"), MSG_LEAVE_ATTENDANT_MODE, true);
        rp.SetOperator(csOperator);
        co.SetAttendantID(EMPTY_STRING);
        rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
        DMTakeawayBeltNotify(tabEventAssistModeStop);
        rp.m_lAttendantEntryID += 1;
    }
    dm.ScannerRemoteAccess(false);
    return STATE_RETURN;
}
/////////////////////////////////////////
// PSButton 3 - Accept
/////////////////////////////////////////
SMStateBase  *SMRemoteScannerConnectBase::PSButton3(void)
{
    trace(L6, _T("SMRemoteScannerConnectBase::PSButton3"));

    if (!co.IsAttendantModeOn())
    {
        trace(L6, _T("Entering Attendant mode via handheld scanner"));
        ps.SMButton(_T("SMButton7"), MSG_ENTER_ATTENDANT_MODE, true);
        co.SetAttendantID(csOperator);
        rp.SetOperator(csOperator);
        rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, ENTER);
        DMTakeawayBeltNotify(tabEventAssistModeStart);
        rp.m_lAttendantEntryID += 1;
    }
    co.SetRemoteScannerConnected(true);
    // Connection has already been accepted in DMParse
    return STATE_RETURN;
}

/////////////////////////////////////////
// PSButton 8 - Cancel
/////////////////////////////////////////
SMStateBase  *SMRemoteScannerConnectBase::PSButton8(void)
{
    return PSButton2();
}

// 207300 //HDMerge-062502 Starts here
bool SMRemoteScannerConnectBase::PSRemoteLegalForState()
{
    return true; 
}

/////////////////////////////////////////////////////////////
// showScreen
/////////////////////////////////////////////////////////////
void SMRemoteScannerConnectBase::showScreen(bool passwordVerified)
{
  trace(L6, _T("SMRemoteScannerConnectBase::showScreen(%d)"), passwordVerified);
  SAWLDBScannerEnable();		// enable the scanner

  saMsgFuncPtr = ps.SecurityMessageModal;

  if (!passwordVerified)
  {
     if (inputMode == PROMPTING_FOR_OPERATOR)
     {
        saMsgFuncPtr = ps.SecurityMessageNonmodal;
        		
		//Tar 393757
		STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics
		ps.SetCMLeadthruText( TXT_ENTER_ID ); //Tar 395105
		ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
		ps.ShowCMFrame( false );   
				
		return;
     }
     // NewUI
  }
  else
  {
      if (ps.RemoteMode() || co.IsInAssistMenus() || storeMode()) 
      {
          // while in remote mode, got remote scanner connect
          showRemoteScreen();
      }
      else
      {
          showLocalScreen();
      }
  } 

  if (!passwordVerified) 
  {
    ps.SetCMFrame(_T("SystemMessageOpPass"));
    ps.LeadThruText(LTT_WAITFORASSISTANCE);

    CString msgText;
    msgText = ps.GetPSText(MSG_WAITFORMESSAGE);
    msgText += "\n* ";
    msgText += ps.GetPSText(MSG_REMOTESCANNER_CONNECT);

    ps.Message12(msgText, true);
	SAWLDBScannerDisable();		// disable the scanner	

  }
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.ShowCMFrame(false); 
}

///////////////////////////////////////
SMStateBase  *SMRemoteScannerConnectBase::stateAfterPasswordVerify(void)
{
	showScreen(true);
    return STATE_NULL; 
}

///////////////////////////////////////
void SMRemoteScannerConnectBase::showLocalScreen(void)
{
	ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
	

    // NewUI
    ps.SetCMFrame(_T("RemoteScannerConnect"));
    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);


	ps.SetCMLeadthruText(LTT_REMOTESCANNER);
    
    ps.ButtonState(_T("CMButton1Med"), false, false);
    ps.ButtonState(_T("CMButton2Med"), true, true);
    ps.ButtonState(_T("CMButton3Med"), true, true);
    ps.Message12(MSG_REMOTESCANNER_TOUCHCONNECT, true);
    
	ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMTBText(csTBMessage2Scot);
	

	ps.ShowCMFrame();
}

///////////////////////////////////////
void SMRemoteScannerConnectBase::showRemoteScreen(void) 
{
    // NewUI
	ps.SetFrame(_T("RemoteSystemMessage"));
	ps.SetTitle(ps.GetPSText(LTT_REMOTESCANNER), EMPTY_STRING);
    
    ps.SetButtonText(_T("SMButton1"), BTT_ACCEPT);
    ps.SetButtonText(_T("SMButton8"), BTT_CANCEL);
    ps.SMInfo(ps.GetPSText(MSG_REMOTESCANNER_TOUCHCONNECT));

	// TAR 236945 - Needed for Remote, this api will check option whether to display or not.
    ps.Weight(lDMScaleWeight);


    // Need to tell RAP to have full screen.
    ra.OnAddOtherEvent(_T("COMMAND=AssistModeSizeToFullScreen;"));  
	ps.ShowFrame();
}

SMStateBase* SMRemoteScannerConnectBase::DMRemoteScannerConnect(bool bConnect)
{
    trace(L6, _T("SMRemoteScannerConnectBase::DMRemoteScannerConnect(%d)"), bConnect);
    SMStateBase *retState = STATE_NULL;

    if (!bConnect)  // Received a disconnect while connecting
    {
        // received disconnect request while connecting
        co.SetRemoteScannerConnected(false);
        dm.ScannerRemoteAccess(false);
        retState = STATE_RETURN;
    }
    return retState;
}


