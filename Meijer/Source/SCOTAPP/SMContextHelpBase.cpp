//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMContextHelpBase.cpp
//
// TITLE: Class implementation for Contextual Help state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMContextHelp.h"
#include "SMHelpOnWay.h"
#include "SMContinueTrans.h"
#include "SMSmAuthorization.h"		//Tar 194454 RF020702


#define COMP_ID ID_SM               // Base state component
#define T_ID _T("ContextHelpBase")

IMPLEMENT_DYNCREATE(SMContextHelpBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ContextHelp)


DEFINE_TIMESTAMP

//////////////////////////////
SMContextHelpBase::SMContextHelpBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////////////////////
// Initialize()
SMStateBase *SMContextHelpBase::Initialize()
{
  // TAR 397386 - Check if we are in Multi-item picklist before sending state to Security Manager.
  if(!fInMultiSelectPickList)
  {
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ContextHelp") );	// RFC 330050
  }
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_HELPFUL, SCOT_LANGUAGE_PRIMARY));
  if(bPressHelpOnWay)    //Tar 164037 Came from Help on Way screen, automatically return to the previous screen which came to Help Need screen
  {
	  bPressHelpOnWay = false;
      /*RFC 234621 Below comment no longer valid-Customer should return to initial state 
	    if anchorstate is attract then set achorstate to SCANANDBAG
	    if(getAnchorState()==BES_START)
	    return setAndCreateAnchorState(BES_SCANANDBAG);
        else
      */
	  return STATE_RETURN;
  }

SMStateBase *retState = STATE(OperatorPasswordState)::Initialize();

  if (fFromRequestSignature) 
	nTimeOut = 0;					 //TAR#140156 -- turn timeout feature off  5/9/00 LMP and YW

  //ps.CMButton(IDC_BUTTONHELP,BTT_HELP,false);
  ps.SetPSText(PS_WORK1,ps.GetPSText(BTT_STORELOGIN,SCOT_LANGUAGE_PRIMARY));
  ps.SetButtonText(_T("CMButton2StoreLogIn"),PS_WORK1);
  return retState;
}
 

////////////////////////////////////////////////////////
void SMContextHelpBase::showScreen(bool passwordVerified)
{
  // This state does not need to reshow if the password
  // was verified, since we'll be leaving now
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;


  if (passwordVerified)
    return;

  /*TAR#112195  LPM080499 Moved to button <Call For Help>.  
  dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                      DM_TRICOLORLIGHT_BLINK_1HZ,
                      DM_TRICOLORLIGHT_NORMALREQUEST);
  */

   if (inputMode == PROMPTING_FOR_OPERATOR)
     {
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;

		//+SR93 Biometrics support		
		STATE(OperatorPasswordState)::ShowEnterID();
		ps.SetCMLeadthruText( TXT_ENTER_ID );
		ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
		//-SR93 Biometrics support
        
		ps.ShowCMTBText(csTBMessage2Scot);
		ps.ShowCMFrame( false );
        return;
     }
     else
     {  // Display initial Help screen. 
        // NewUI
        ps.SetCMFrame(_T("ContextHelp"));
		ps.SetButtonText(_T("CMButton2"), BTT_STORELOGIN);
        // End NewUI
		DMSayPhrase(CALLFORHELP);  //TAR 156263
     }
  
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;  
  ps.SetCMLeadthruText( LTT_HELPFUL );
  ps.CMButtonFlash(_T("CMButton1"),true);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  ps.Message12(TXT_ENTER_OPERATOR_PROMPT, true);
  ps.ShowCMFrame(false);
  // End NewUI
	
#ifdef _DEBUG
  CString csWork;
  csWork.Format(_T("\n\nCalling state: %s\nBMP file:  %s\nTimeout: %d; StoreMode: %c"),
    csLastNonHelpStateName,
    csLastNonHelpStateBMP,
    nLastNonHelpStateTimeOut,
    (fLastNonHelpStateStoreMode?_T('Y'):_T('N')));
  // ps.Message23(csWork);
#endif
}

SMStateBase *SMContextHelpBase::PSButtonHelp(void)
{
  // Help button acts like Call For Help button (#1)
  return PSButton1();
}

///////////////////////////////////////
SMStateBase *SMContextHelpBase::PSButton1(void) // call for help
{
  if (!fInValidOperatorPassword)  // TAR 252552
  {
    //Save the current light before makiing it RED, so that we can restore it
    // after we handle call for Help.
    dm.saveTriColorLightStatus();

    //TAR#112195  LPM080499
    
    // (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                       DM_TRICOLORLIGHT_BLINK_1HZ,
                       DM_TRICOLORLIGHT_NORMALREQUEST);
                       
    fRequestForHelp = true;

  }
  if (parentClassCurrentlyHandlingButton2() || fInValidOperatorPassword)  // TAR 253541
      return STATE(OperatorPasswordState)::PSButton1();


  RETURNSTATE(HelpOnWay)	//Always return Help On Way Screen
}

///////////////////////////////////////
SMStateBase *SMContextHelpBase::PSButton2(void) // Store Log In
{
  //TAR#112195  LPM080499
  //merged tar 234726 - fixed by Ruby
  //dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
  //                    DM_TRICOLORLIGHT_BLINK_1HZ,
  //                    DM_TRICOLORLIGHT_NORMALREQUEST);

  promptForOperator();
  fFromRequestSignature = false;
  return STATE_NULL;
}

///////////////////////////////////////
SMStateBase *SMContextHelpBase::PSButton3(void) // exit help
{
   if (inputMode == PROMPTING_FOR_PASSWORD) //TAR 117419
   {
     promptForOperator();
   }
   else
   {
     showScreen(false);
	 return STATE_NULL;
   }
   fFromRequestSignature = false;	//TAR#140156  LPM051000
   return STATE_RETURN;
}

///////////////////////////////////////
SMStateBase *SMContextHelpBase::PSButtonGoBack(void) // exit help
{
  fFromRequestSignature = false;	//TAR#140156  LPM051000
  fInValidOperatorPassword = false;	//TAR347701

  //if (m_customerLanguage==SCOT_LANGUAGE_SECONDARY)//TAR184586 TID
	//   ps.SetLanguage(SCOT_LANGUAGE_SECONDARY);
  // TAR 271759: Don't assume only 2 language; ALWAYS set the language
  // back to the saved off one, if we have dual language to begin with.
  if ( co.GetfStateDualLanguage() )
  {
	  if( PROMPTING_FOR_PASSWORD == inputMode ) //Tar 389669 
		{ ps.SetLanguage( SCOT_LANGUAGE_PRIMARY ); }	
	  else
		{ ps.SetLanguage(m_customerLanguage); }  
  }  


    switch(inputMode)
    {  
	  case SHOW_WAIT_FOR_APPROVAL:
        //+SR879
		if	(g_pRememberStateBeforeHelp != NULL)
		{	
			SMStateBase *sReturnState = g_pRememberStateBeforeHelp;

			CString csStateName = g_pRememberStateBeforeHelp->GetRuntimeClass()->m_lpszClassName;
			g_pRememberStateBeforeHelp = NULL;
  			trace(L6,_T("Go to state %s saved in g_pRememberStateBeforeHelp"),csStateName);
		
			return sReturnState;
		}
		else
		{
			return STATE_RETURN;	
		}
		//-SR879  
        break;
      case PROMPTING_FOR_OPERATOR:
        if( getAnchorState() == BES_THANKYOU )//rfc209702
        {
          return STATE_RETURN;  
        }
        else
        {
          inputMode = SHOW_WAIT_FOR_APPROVAL;
        }
	
		if (co.m_bOperationsUseFingerPrintDevice)  
		{
			FT_Abort();
		}
		break;
      case PROMPTING_FOR_PASSWORD:
        inputMode = PROMPTING_FOR_OPERATOR;
		break;
    }
  return PSButton3();
}

/////////////////////////////////////////
SMStateBase *SMContextHelpBase::PSCancelKey(void)
{
  if (parentClassCurrentlyHandlingCancelKey())
    return STATE(OperatorPasswordState)::PSCancelKey();

  return STATE_NULL;
}

//////////////////////////////////////////
SMStateBase *SMContextHelpBase::PSListLookup(CString cItemCode) // list or lookup selection
{
  // no help yet
  DMSayPhrase(PICKLISTINVALID);
  return STATE_NULL;
}

/////////////////////////////////////////////
SMStateBase *SMContextHelpBase::TimedOut(void)
{
  // Fix for TAR 153465
  if (SCOTIsMinimized() || 
      inputMode == PROMPTING_FOR_OPERATOR ||    // TAR 227943
	  inputMode == PROMPTING_FOR_PASSWORD)
    return STATE_NULL;
  //TAR377953+
  else if( getAnchorState() == BES_THANKYOU )
  {
	  return STATE_NULL;
  }
  //TAR377953-
  else
    RETURNSTATE(ContinueTrans)
}

SMStateBase *SMContextHelpBase::stateAfterPasswordVerify(void) 
{ 
	fFromRequestSignature = false;	//TAR#140156  LPM051000
	BEST best = getAnchorState();
	//SR879
	if	(g_pRememberStateBeforeHelp != NULL)
	{	
		CString csStateName = g_pRememberStateBeforeHelp->GetRuntimeClass()->m_lpszClassName;
		delete g_pRememberStateBeforeHelp;
		g_pRememberStateBeforeHelp = NULL;
		trace(L6,_T("Delete the state %s saved in g_pRememberStateBeforeHelp"),csStateName);
	}

	// TAR 397386 - Check if we are in Multi-item picklist before item cancel to Security Manager.
	if(!fInMultiSelectPickList)
	{
        //+ SSCOP-3552
        if(getAnchorState() != BES_START) 
        {
            SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
        }
        //- SSCOP-3552
	}
 	RETURNSTATE(SmAuthorization)
}

//////////////////////////////////////////////////////////////
SMStateBase *SMContextHelpBase::OnWtIncreaseNotAllowed() //tar 197857
{
	//TAR237079 CHU/TP 051303
	if (getAnchorState() == BES_START )
		g_bWtIncAtContextHelpComingFromAttract = true;

	return VoidingWtIncrease(BES_HELPSCREEN, BES_HELPSCREEN);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMContextHelpBase::OnWtDecrease() //tar 197857
{
	return ThresholdViolWtDecrease(BES_HELPSCREEN, BES_HELPSCREEN);
}
// + RFC 330050
void SMContextHelpBase::UnInitialize(void)
{
	// TAR 397386 - Check if we are in Multi-item picklist before sending state to Security Manager.
	if(!fInMultiSelectPickList)
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ContextHelp") );
	}

	STATE(OperatorPasswordState)::UnInitialize();  // TAR 354252
}
// - RFC 330050
