//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRAPDataNeeded.CPP
//
// TITLE: Class implementation for RAP Data Needed state
//
//
// AUTHOR:    Erick Kobres
//
// CHANGE HISTORY:
//
// CRD 554916 Name: CJ Estrada Date: Feb 9, 2021
// CRD 517533 Work Request: Sprint 24.1 Name: Dennis Michol Gabutin Date: November 17, 2020
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRAPDataNeeded.h"      // MGV this state
//#include "SMInProgress.h"			// Tar 194454 RF020702

#include "SMSmAssistMode.h"
#include "SMSmAssistMenu.h"
#include "transbrokerssf.h"
#include "SMTakeReceipt.h"
#include "SMTakeChange.h" //SSCOI-31860
#include "SMFinish.h"
#include "SMSmEnhancedAssistMode.h" //POS31050 rrs

#include "SMRecallingItems.h" // RFQ 9545 mobile shopper

#define COMP_ID ID_SM               // base state component
#define T_ID _T("RAPDataNeeded")

#define ACSDATAENTRYINIFILE _T("C:\\SCOT\\CONFIG\\ACSScotDataEntry.ini")

IMPLEMENT_DYNCREATE(SMRAPDataNeeded, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

////////////////////////////////////////////////////
SMRAPDataNeeded::SMRAPDataNeeded(LPCTSTR sDesc, int View)
		: SMRAPDataNeededBase(sDesc, View)
{
  IMPLEMENT_TIMESTAMP
		
}

////////////////////////////////////////
SMRAPDataNeeded::SMRAPDataNeeded()
{
  IMPLEMENT_TIMESTAMP
	//sotf 7428
	//
	if(m_View == DATANEEDED_ASSISTMODE)
	{        
		m_bDataNeededAssistMode = true; 
	}
	
	//POS32142 rrs
	bNeedHoldWeight = false;
	
	bRAPDataNeededBypass = false; // POS50491
}

////////////////////////////////////////
SMStateBase  *SMRAPDataNeeded::Deliver(LPCTSTR sDesc, int View)
{
  SMRAPDataNeededBase::Deliver(sDesc, View);

  return this;
}

////////////////////////////////////////
SMStateBase  *SMRAPDataNeeded::Deliver(LPCTSTR sDesc, int View, LPCTSTR pPtr, const bool storeMode)
{
	trace( L6, _T("storeMode = [%d] "), storeMode);
	bool bStoreMode = storeMode;
	if( (int)storeMode > 1 ) //it means uninitialized
		bStoreMode = false;
	
  SMRAPDataNeededBase::Deliver(sDesc, View, pPtr, bStoreMode);

  return this;
}

SMStateBase *SMRAPDataNeeded::Initialize(void)
{
    SMStateBase *pNextState = STATE_NULL;

    //+ SSCOI-31860
    trace(L6, _T("SSCOI-31860 m_sView = %s SMState::m_bTakeChangeTimerExpired = %d !!!!"), m_sView, m_bTakeChangeTimerExpired);
    if(SMState::m_bTakeChangeTimerExpired && m_sView == _T("TakeChangeTimeout"))
    {
        trace(L6, _T("SSCOI-31860 going to take change"));
        CREATE_AND_DISPENSE(TakeChange)(true);
    }
    //- SSCOI-31860

    CString csPOSStateActive(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSStateActive);
    if( csPOSStateActive == _T("RETURN_COUPON") )
    {
        if (m_bCouponInsertionBypassed)
        {
            // POS33020: Coupon was never inserted in collection bin, so don't display Return Coupon store mode screen.
            trace(L6, _T("No coupon to return"));
            m_bCouponInsertionBypassed = false;
            SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F1}"))); // Perform a softkey #1 (OK) tap so that sales app can continue.

            // POS50491:
            // Clear need-more-data flag so that there are no long delays when customer presses the Finish&Pay button.
            // Make sure UnInitialize() routine is bypassed since this Initialize() routine was bypassed.
            // Turn off red security light.
            g_bTBNeedMoreData = false;
            bRAPDataNeededBypass = true;
            dm.setTriColorLight(DM_TRICOLORLIGHT_RED, DM_TRICOLORLIGHT_OFF, DM_TRICOLORLIGHT_NORMALREQUEST);

            // Current anchor state is more than likely BES_CMDATAENTRY which would lead back to BES_INSERTCOUPON
            // which we don't want to do, so force state back to Scan&Bag.
            trace(L6, _T("Return to ScanAndBag"));
            return setAndCreateAnchorState(BES_SCANANDBAG);
        }
        else
        {
            trace(L6, _T("SMRAPDataNeeded: Coupon to return"));
        }
    }

    // SOTF 6853
    //g_bDataNeededAssistMode = false;

    //if the storelogin flag is set then don't
    //ask the operator to log in again
    //the base only does not ask for a re-login
    //if the View is a DataNeed_MoreData view
    //if(m_View == DATANEEDED_ASSISTMODE)
    //{
    //    g_bDataNeededAssistMode = true;	    
    //}
    // sotf
	CString csVoidPrepaidCardsProp = _T("VoidPrepaidCards");
    CString csIsVoidPrepaidCards;
    SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csVoidPrepaidCardsProp, &csIsVoidPrepaidCards);

    // If voiding prepaid cards or the TB requested
	//FL to go into assistmode
    if ((csIsVoidPrepaidCards == _T("1")) || (m_bDataNeededAssistMode)) // SOTF 6853
    {
		ra.RequestEnableAssistMode(true);
	}

    pNextState = SMRAPDataNeededBase::Initialize();

    // SOTF 6853
    if(m_bDataNeededAssistMode)
    {
        m_View = DATANEEDED_ASSISTMODE;
    }

	//Meijer would like for the wave file to be played when the screen first
	//appears and on state timeouts
	trace(L6, _T("SMRAPDataNeeded:: ra.GetRAPPreviousView() - %d, bEASApproveFromVoid - %d, lUnapprovedEASItems - %d, isPlayedAssistanceAudio - %d"), ra.GetRAPPreviousView(), bEASApproveFromVoid, lUnapprovedEASItems, isPlayedAssistanceAudio);
	if (ra.GetRAPPreviousView() != DATANEEDED_NOVIEW || (!bEASApproveFromVoid && !isPlayedAssistanceAudio))
	{
		if(g_bTBNeedMoreData && m_View != DATANEEDED_GOTOINPROGRESS && !isPlayedAssistanceAudio) //SSCOI-46589: expression added for SSCOI-46589 & SSCOI-46394 
	    //TAR 425002
        //using say security so that the wave file is not interrupted.
        //it will prevent stuttering
        DMSaySecurity( SEC_NONE, ps.GetSoundName(WAITFORASSISTANCE));

		if(lUnapprovedEASItems || lUnapprovedMobileGenericDelayed || bUnapprovedAccountLookupFailed)	//CRD 223409, 554916
			isPlayedAssistanceAudio = true;	//set to true at first
     
	}

    // POS32142 rrs
    CString csPOSState(_T(""));
    SendTBCommand(TBCMD_GET_POSSTATE, &csPOSState);
    if( csPOSState == _T("INVALID_WIC_FREE_ITEM") )
    {
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CardProcessing") );
        bNeedHoldWeight = true;
        SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F2}")));
        trace(L6, _T("SMRAPDataNeeded Start Hold Weight"));
    }
    // e POS32142


    //POS43790 - variable GC/PC denomination    
    CString csCurrentContext = sm_pDataNeededEntry.GetContextToDisplay(); 
    if( ps.RemoteMode() && csCurrentContext.Find(_T("CmDataEntry5Vertical")) != -1 )
    {
        delete getRealStatePointer( pNextState );
        RETURNSTATE( SmAssistMode )
    }
    // e- POS43790
	//(+) VSTS170002
	 CString csPrevState = this->csLastNonHelpStateName;
	 trace(L6, _T("%s LastNonHelpStatename"), csPrevState);
	if(	(m_bMobileTransactionStarted) && (csPrevState == _T("SMAttract") ))
	{
		ra.RequestEnableAssistMode(false);
		trace(L6, _T("disabling Asistmodebutton in RAP when Mobile shopper transaction Starts"));
		
	}
	//(-)VSTS170002

	SendTBCommand(TBCMD_NOTIFY_STOREMODE_PROMPT);	//CRD 338206

    return pNextState;
	//return SMRAPDataNeededBase::Initialize();
}

void SMRAPDataNeeded::UnInitialize(void)
{
	//(+) VSTS170002
	trace(L6, _T(" Making m_bMobileTransactionStarted as false "));
	SMState::m_bMobileTransactionStarted = false;
	//(-) VSTS170002

	SMRecallingItems::g_nTimeoutRecallingWaitForPos = 9999; // RFQ 9545 

    if (bRAPDataNeededBypass)
    {
        // POS50491: Initialize() routine was bypassed so bypass this UnInitialize() routine.
        trace(L6, _T("UnInitialize() bypassed"));
        return;
    }
	
    // POS32142 rrs
    if( bNeedHoldWeight )
    {
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CardProcessing") );
        bNeedHoldWeight = false;
        trace(L6, _T("SMRAPDataNeeded End Hold Weight"));
    }
    // e POS32142
	
    //POS31050 - don't reset the m_bDataNeededAssistMode during the Stream line intervention
    if( GetHookProperty(_T("PropMobShopOrderPrompt")) != _T("1") )
    {
        //sotf 6853
		m_bDataNeededAssistMode = false;    
    }

	    SMRAPDataNeededBase::UnInitialize();
}

bool SMRAPDataNeeded::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMRAPDataNeededBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase *SMRAPDataNeeded::TimedOut(void)
{
    return SMRAPDataNeededBase::TimedOut();
}

SMStateBase *SMRAPDataNeeded::HandleApproveVoidedItem()
{
    return SMRAPDataNeededBase::HandleApproveVoidedItem();
}

SMStateBase *SMRAPDataNeeded::HandleConfirmSuspend()
{
    return SMRAPDataNeededBase::HandleConfirmSuspend();
}

////////////////////////////////////////
SMStateBase *SMRAPDataNeeded::HandleAssistMode()
{
    SMStateBase *pNextState = createAnchorState();
    trace(L6, _T("+SMRAPDataNeeded::HandleAssistMode()"));
	
    if (m_sButton == _T("cAssistMode"))
	{
		// Sit tight and wait for RAP to throw us into Assist Mode...
		delete getRealStatePointer( pNextState);
		pNextState = STATE_NULL;
	}
    else if (m_sButton != _T("cGoBack"))
	{
		ASSERT(FALSE);
		trace(L0, _T("SMRAPDataNeeded::HandleAssistMode() -- Button data was not valid.  Returning to anchor state."));
        BeginRAPView();
	}
	
    trace(L6, _T("-SMRAPDataNeeded::HandleAssistMode()"));
    delete getRealStatePointer( pNextState);
    return STATE_NULL;
	
}

////////////////////////////////////////
//Price entered at both RAP & ScotApp stored mode are handled by this method.
SMStateBase *SMRAPDataNeeded::HandlePricePrompt()
{
    trace(L6, _T("+SMRAPDataNeeded::HandlePricePrompt()"));
	SAClearSecurityCondition();	// TAR209439 LPM071102 reset the lane lite
	//218558 remove ra code. it is done in SAClear above
	//218558 set scanner flag to re-enabled only in state that isn't handling EAS automatically
	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
	CString btnCommand = m_sButton; //cache the button command because the below base method sets m_sButton to blank as a fix for SSCOB-176
	
    SMStateBase *pState= HandleDataNeeded();
    if( btnCommand == _T("cReturn") && GetHookProperty(_T("IsResumeTrxInProgress")) != _T("1") )
	{
		if (io.csItemCode.GetLength()>0) //tar 183854
		{
			// Tar 120648
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		}
		m_sButton = _T(""); //TD TAR 202158 04/19/02 
        // cleanup old state 
        delete getRealStatePointer(pState);     
		return setAndCreateAnchorState(BES_SCANANDBAG);     //TAR#126112 return to Scan&Bag LPM111699
	}
    return pState; 
	//return SMRAPDataNeededBase::HandlePricePrompt();
}

//TAR395246 remove cash in "take change timeout" state 
SMStateBase *SMRAPDataNeeded::OnNoMotionCashOutSensor(void) 
{
	trace(L6, _T("SMRAPDataNeeded::OnNoMotionCashOutSensor"));
	
	SMStateBase::fCashOutSensorBlocked = false;
	
    //POS56079 - rrs
	if ( GetTimeoutInTakeChangeState() && GetHookProperty(_T("TRXENDDone")) == _T("1"))
	{
		trace(L6, _T("SMRAPDataNeeded::GetTimeoutInTakeChangeState is true and TRXENDDone"));
		RETURNSTATE(TakeReceipt)
	}	
    // e POS56079 - rrs
    return STATE_NULL;
	
}

// POS32142 rrs
//////////////////////////////////////////////////////////////
SMStateBase *SMRAPDataNeeded::OnWtIncreaseNotAllowed(void)
{    
	if( bNeedHoldWeight )
    {
        trace(L6, _T("Ignore The WtIncreaseNotAllowed in SMRAPDataNeeded"));
		return STATE_NULL;           
    }    
	
    return SMRAPDataNeededBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMRAPDataNeeded::OnWtDecrease(void) //tar 197857
{
    if( bNeedHoldWeight )
    {
        trace(L6, _T("Ignore The WtDecrease in SMRAPDataNeeded"));
		return STATE_NULL;        
    }    
	
    return SMRAPDataNeededBase::OnWtDecrease();
}
// e POS32142

SMStateBase *SMRAPDataNeeded::OnMatchedWt(void)
{
    return SMRAPDataNeededBase::OnMatchedWt();
}

SMStateBase *SMRAPDataNeeded::HandleAssistedEndorsement()
{
    return SMRAPDataNeededBase::HandleAssistedEndorsement();
}

SMStateBase *SMRAPDataNeeded::HandleAssistedTender()
{
    return SMRAPDataNeededBase::HandleAssistedTender();
}

SMStateBase *SMRAPDataNeeded::HandleMaxCouponLimit()
{
    return SMRAPDataNeededBase::HandleMaxCouponLimit();
}

SMStateBase *SMRAPDataNeeded::HandleMaxCouponLimitFinalization()
{
    return SMRAPDataNeededBase::HandleMaxCouponLimitFinalization();
}

SMStateBase *SMRAPDataNeeded::HandleInvalidBarcode()
{
    return SMRAPDataNeededBase::HandleInvalidBarcode();
}

SMStateBase *SMRAPDataNeeded::RAParse(MessageElement *me)
{
    return SMRAPDataNeededBase::RAParse(me);
}

SMStateBase *SMRAPDataNeeded::SASecMgrParse( const MessageElement *me)
{
    return SMRAPDataNeededBase::SASecMgrParse(me);
}

void SMRAPDataNeeded::OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation)
{
    SMRAPDataNeededBase::OverrideSecurityException(lpszAuthorization, lpszOperation);
}

bool SMRAPDataNeeded::PSRemoteLegalForState()
{
    return SMRAPDataNeededBase::PSRemoteLegalForState();
}

bool SMRAPDataNeeded::storeMode()
{
    return SMRAPDataNeededBase::storeMode();
}

void SMRAPDataNeeded::SetupViewData()
{
   LPCTSTR lpThisName = _T("SMRAPDataNeeded :: SetupViewData()");
    trace( L6, _T("+%s %s 0x%08.8x "), lpThisName, _T("m_View = "), m_View);
    
    
    CSCOTDataEntry de;
    switch (m_View)
    {
	case DATANEEDED_ASSISTMODE:
        {// RFC 197844
            m_sView        = _T("NeedAssist");
            m_sViewName    = _T("NeedAssist");        //TAR 233927
            m_ViewText     = SSF_NEEDASSIST_DATAENTRY;
            m_View         = DATANEEDED_MOREDATA;     //see comment in Initialize() for storelogin
    
            //POS31050 - display Mobile Assistance text rrs
            //POS31052 - handle new audit screen rrs
            //POS31051 - Display the Mobile Transaction Verification Needed for the Mobile Audit Intervention only
            if( GetHookProperty(_T("PropMobShopOrderPrompt")) == _T("1") )
            {
                CString csPosState = GetHookProperty(_T("__POSState"));
            
                DMSaySecurity( SEC_LOW, _T("blank.wav") );

                if( csPosState.Find(_T("MOBILE_AUDIT")) != -1 )
                {       
                    m_ViewText     = 9309;                    
                }
                else
                {
                    m_ViewText     = SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE;
                }    
                
                m_View = DATANEEDED_ASSISTMODE;
                trace(L6, _T("MobileEnhancedAssistMode will be active !!!!"));
            }

			if (de.FromConfigData(ACSDATAENTRYINIFILE, m_sView))
			{
				sm_pDataNeededEntry.FromConfigData(ACSDATAENTRYINIFILE, m_sView);
				trace(L6, _T("%s Successfully created view: <%s> from config file."), lpThisName, m_sView);
			}
			else
			{
				trace( L0, _T("%s Failed to convert view: <%s> from config file.  View name will be used unadorned."), lpThisName, m_sView);
			}
			
			break;
        }     
	case DATANEEDED_MOREDATA:         // RFC 197844
		{
			// view specified - see if we have a customer screen property specified
			// if the view is specified then we won't overwrite it.
			CString strCustProp = _T("");
			SendTBCommand(TBCMD_GET_TBPROPERTY, &CString(TBPROP_RAPDATANEEDED_CUSTENTRY), &strCustProp); 
			SendTBCommand(TBCMD_SET_TBPROPERTY, &CString(TBPROP_RAPDATANEEDED_CUSTENTRY), &CString(_T(""))); 
			
			m_ViewText = strCustProp.IsEmpty() ? TXT_NEEDINFORMATION : _ttol(strCustProp);
			//flag negative media item to skip Assistance Needed wav message
			if(m_ViewText == 9070)
			{	
				bNegativeMediaTBNeedMoreData = true;
			}

			// is this a custom DataNeeded with all scotentry items provided?
			CString strSection = sm_pDataNeededEntry.GetSectionName();
			if( !strSection.IsEmpty() )
			{
				m_sView = strSection;
				
				// no need to reload the configuration file we've already done this
				CString strLaneButton = sm_pDataNeededEntry.GetSecondaryButtonText();
				if( !strLaneButton.IsEmpty() )
				{
					m_sViewName = strLaneButton;
				}
			}
			else
			{
				// run the default handler and attempt to load from the ACS scot data entry file
				SMRAPDataNeededBase::SetupViewData();
				
				//if the TB specified a string to use for m_ViewText then preserve it
				if(!strCustProp.IsEmpty())
				{
					m_ViewText =_ttol(strCustProp);
				}

                //SOTF 7701 && 7955
                /* code is no longer needed with G2
                if ((m_sView != _T("NeedInfo")) && (de.FromConfigData(ACSDATAENTRYINIFILE, m_sView)))
				{
					sm_pDataNeededEntry.FromConfigData(ACSDATAENTRYINIFILE, m_sView);
					trace(L6, _T("%s Successfully created view: <%s> from config file."), lpThisName, m_sView);
				}
				else
				{
					trace( L0, _T("%s Failed to convert view: <%s> from config file.  View name will be used unadorned."), lpThisName, m_sView);
				} */
			}
		}
		break;
	
    default:
        {
            
            SMRAPDataNeededBase :: SetupViewData();
        }
    } // switch
}

SMStateBase *SMRAPDataNeeded::OnRAPDataReceived(LPCTSTR sData)
{
    return SMRAPDataNeededBase::OnRAPDataReceived(sData);
}

void SMRAPDataNeeded::showScreen(bool passwordVerified)
{
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
    int nNumApprovals = 0;
    
    // This state does not need to reshow if the password
    // was verified, since we'll be leaving now
    if (passwordVerified)
    {
        return;
    }
    
    CString csWork;
    
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
        
        STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics support

        ps.SetCMLeadthruText( TXT_ENTER_ID );
        
        // Determine the leadthru text to show
        int iMessage = MSG_ENTERID_PRESSENTER;
        COleVariant v = VARIANT_TRUE;
        bool bEmplid = (SendTBCommand(TBCMD_IS_EMPLID_ENABLED) == TB_SUCCESS_IMMEDIATE);
        if( bEmplid )
        {
            bool bAllowHandKey = (SendTBCommand(TBCMD_IS_EMPLID_ALLOWHANDKEY) == TB_SUCCESS_IMMEDIATE);
            iMessage = bAllowHandKey ? MSG_ENTERID_SCANIDORKEY : MSG_ENTERID_SCANIDONLY;
            v = bAllowHandKey ? VARIANT_TRUE : VARIANT_FALSE;

//(+)POS108118
            if(!bAllowHandKey)
            {
                CString csNewPosition;
                csNewPosition = _T("400,262,520,65");
                m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);

                csNewPosition = _T("265,396,510,250");
                m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);

                m_pAppPSX->SetConfigProperty(_T("LeadthruText"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
                m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
            }
//(-)POS108118
        }
        
        m_pAppPSX->SetConfigProperty(_T("NumericP1"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP2"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP3"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP4"), _T("EnterID"), UI::PROPERTYVISIBLE, v);

//(+)POS108118
        m_pAppPSX->SetConfigProperty(_T("Echo"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("EchoField"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
//(-)POS108118
        
        ps.Message12(iMessage, false);
        ps.AllowInput( co.GetnOperationsOperatorIDMaxDigit(), false, false );
        ps.ShowCMFrame( false );
        return;
    }
    else
    {
        // TK - TAR 199024
        // Disabling scanner when cashier assistance needed screen is displayed.
        DMScannerDisable();
        // TAR 199024
        // Display Wait for Assistance....
        // NewUI
        ps.SetCMFrame(_T("RAPDataNeeded"));
        // End NewUI
        
        // TK - TAR 197541
        // Playing assistance needed wave file
        if(!bNegativeMediaTBNeedMoreData)	// TAR 352860 - skip Assistance needed message after a negative media sale
        {
            DMSaySecurity(SEC_NONE, ps.GetSoundName(ASSISTANCENEEDED));
        }
    }
	//RFQ 2506 - Add Dataneeded override of Leadthru text
	csWork = _T("");
	csWork = sm_pDataNeededEntry.GetRDNLaneLeadthru();
	if (!csWork.GetLength())
		ps.SetCMLeadthruText(LTT_WAITFORASSISTANCE);
	else
		ps.SetCMLeadthruText(csWork);

    if(m_View == DATANEEDED_ENDORSEDOCUMENT)
        ps.SetCMGobackButtonVisible(false);
    else
    {
        ps.CMButton(_T("ButtonStoreLogIn"), BTT_STORELOGIN, true);
        //T.D 05/02/02 TAR 2032663 only need store-login button
        //ps.SetCMGobackButtonVisible(true);
    }
    
    ps.CMButton(_T("ButtonHelp"), BTT_HELP,false);  //Tar 143857 Disable the help button.
    // NewUI

	//RFQ 2506 - Add Dataneeded override of message text
	csWork = _T("");
	csWork = sm_pDataNeededEntry.GetRDNLaneMessage();
	if (!csWork.GetLength())
	{
	    csWork = ps.GetPSText(MSG_WAITFORASSISTANCE);
	    csWork += _T("\n* ") + ps.GetPSText(m_ViewText);
	}
    ps.Message12(csWork, true);
    ps.ShowCMFrame();
	SMState::StoreAssistRequestedTime = CTime::GetCurrentTime();
}

SMStateBase *SMRAPDataNeeded::stateAfterPasswordVerify()
{
	// SOTF 6853
	if(m_bDataNeededAssistMode)
    {
        m_View = DATANEEDED_ASSISTMODE;
        m_bDataNeededAssistMode = false;
    }
	
    switch(m_View)
    {
    case DATANEEDED_ASSISTMODE:
        {    
            SAClearSecurityCondition();
			
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
			
            //POS31050 rrs
            if( !ps.RemoteMode() && GetHookProperty(_T("PropMobShopOrderPrompt")) == _T("1") )
            {
                trace(L6, _T("Activate EnhancedAssistMode") );
                RETURNSTATE(SmEnhancedAssistMode)
            }
			//e POS31050 - display Mobile Assistance text rrs
            RETURNSTATE(SmAssistMode)
				break;
        }
    default:
        {    
            return SMRAPDataNeededBase::stateAfterPasswordVerify();
            break;
        }
    }
	
    return STATE_NULL;  //Make compiler happy
}

SMStateBase *SMRAPDataNeeded::BeginRAPView()
{
// start CRD126194	
	CString csAuditIntervention = sm_pDataNeededEntry.ExtractString(_T("AuditIntervention"));
    if (csAuditIntervention.CompareNoCase(_T("1")) ==0 )
    {
        rp.TransactionStart(rp.m_csBusinessDate);
    }
// end CRD126194


	if (m_View > DATANEEDED_LASTVIEW)
	{
		sm_pDataNeededEntry.FromConfigData(ACSDATAENTRYINIFILE, m_sView); //now done in deliver or initialize
	}

    //POS31050 - Display the Mobile Assistance Message at the RAP and still allow the AssistMode button at the RAP    
    CString csPosState = GetHookProperty(_T("__POSState"));
    if( (csPosState.Find(_T("MOBILE_TRANSACTION_ASSISTANCE")) != -1) )
    {
        CString csMessage = ps.GetPSText(SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE, SCOT_LANGUAGE_PRIMARY);
        ps.AddMessageToReceipt(_T("CMSMReceipt"), csMessage );
        //ra.RequestEnableAssistMode(false);

        dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                            DM_TRICOLORLIGHT_BLINK_1HZ,
							DM_TRICOLORLIGHT_NORMALREQUEST);

        CString csInstruction;
        //932=DESCRIPTION=%s;LANEBUTTONTEXT=%s;LSWND1=%s;LSWND2=%s;LSWND3=%s;
	    csInstruction.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY), _T(""), ps.GetPSText(SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE_LINE1, SCOT_LANGUAGE_PRIMARY), ps.GetPSText(SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE_LINE2, SCOT_LANGUAGE_PRIMARY), _T(""));
	    ra.OnNormalItem(csInstruction);

        return STATE_NULL;
    }
    //e POS31050 - Do not allow Mobile Assistance at RAP    

   //POS11024 Do not allow MobileAudit at RAP
   if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && (sm_pDataNeededEntry.GetSectionName() == _T("MobileAudit")) )
   {
      CString csMessage = ps.GetPSText(SSF_TXT_MOBILE_AUDIT, SCOT_LANGUAGE_PRIMARY);
      ps.AddMessageToReceipt(_T("CMSMReceipt"), csMessage );
      ra.RequestEnableAssistMode(false);

      dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                          DM_TRICOLORLIGHT_BLINK_1HZ,
                          DM_TRICOLORLIGHT_NORMALREQUEST);
      return STATE_NULL;
   }

   //POS37397 Do not allow Mobile Reresume approval at RAP
   if( sm_pDataNeededEntry.GetSectionName().GetLength() > 0 && (sm_pDataNeededEntry.GetSectionName() == _T("MobileShopperOrderInProgress")) )
   {
      CString csMessage = ps.GetPSText(9346, SCOT_LANGUAGE_PRIMARY);
      ps.AddMessageToReceipt(_T("CMSMReceipt"), csMessage );
      ra.RequestEnableAssistMode(false);

      dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                          DM_TRICOLORLIGHT_BLINK_1HZ,
                          DM_TRICOLORLIGHT_NORMALREQUEST);
      return STATE_NULL;
   }

   SMStateBase *pNextState = STATE_NULL;

   ASSERT(!m_sDesc.IsEmpty());
   ASSERT(!m_sView.IsEmpty());
//TAR 215932
	// Tar 211874 RJF 3/19/03 don't need to check for just price-needed
	// all cases coming thru rapdataneeded require red to blink
   dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                       DM_TRICOLORLIGHT_BLINK_1HZ,
                       DM_TRICOLORLIGHT_NORMALREQUEST);

//   ASSERT(!ra.GetRAPDataPendingCount());

//   if (!ra.GetRAPDataPendingCount())
//      {
         // Begin TAR 194764
	     //cDataEntry.FromConfigData(DATAENTRYINIFILE, m_sView); //now done in deliver or initialize
         
         CString csLaneButton = sm_pDataNeededEntry.GetSecondaryButtonText();
         if ( (m_View != DATANEEDED_MOREDATA) && (m_View < DATANEEDED_LASTVIEW))
         {
             //always use the viewname as lane button text for fastlane-defined requests
             csLaneButton = m_sViewName;
         }
         else if (csLaneButton == _T(""))    //lane button text not defined
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
						  sm_pDataNeededEntry.GetLaneStateOneText(), 
						  sm_pDataNeededEntry.GetLaneStateTwoText(), 
						  sm_pDataNeededEntry.GetLaneStateThreeText(),
                          m_View
						 );
		// + RFC 256154 - prevent it from sending two RAPDataNeeded events
		if(DATANEEDED_MULTIPACKITEM != m_View)
		{
            CString csReportCategoryName = sm_pDataNeededEntry.GetReportCategoryName();

            //+SR831
            CString csHandlingContext = sm_pDataNeededEntry.GetHandlingContext();
            ra.OnDisableHandlingAtLocation(csHandlingContext); 
            //-SR831

			ra.OnRAPDataNeeded(csSecInfo, 
                               sm_pDataNeededEntry.ToString(), 
                               m_View,
                               csReportCategoryName);
		}
        // End TAR 194764
//      }
   return pNextState;
}

SMStateBase *SMRAPDataNeeded::PSButtonGoBack()
{
    return SMRAPDataNeededBase::PSButtonGoBack();
}

SMStateBase *SMRAPDataNeeded::HandleRAPData()
{
    LPCTSTR lpThisName = _T("SMRAPDataNeeded :: HandleRAPData()");
    trace( L6, _T("+%s %s 0x%08.8x "), lpThisName, _T("m_View = "), m_View);
    SMStateBase *pNextState = STATE_NULL;
    
	
    switch (m_View)
    {
    case DATANEEDED_ASSISTMODE:
		{
			pNextState = HandleAssistMode();
			break;
        }
    case DATANEEDED_PRICEPROMPT:
		{
			g_bTBNeedMoreData = true;
			pNextState = SMRAPDataNeededBase::HandleRAPData();
			break;
		}        
    default:
        {
			trace(L7, _T("SMRAPDataNeeded::HandleRAPData() - DATANEEDED_MOREDATA handled in Base"));
			pNextState = SMRAPDataNeededBase :: HandleRAPData();
        }
    }
	
    
    
    trace( L6, _T("-%s"), lpThisName);
    io.d.csErrDescription.Empty();
    return pNextState; 
	//return SMRAPDataNeededBase::HandleRAPData();
}

SMStateBase *SMRAPDataNeeded::HandleScannerData( const MessageElement* me)
{
    return SMRAPDataNeededBase::HandleScannerData(me);
}
