//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInsertCoupon.CPP
//
// TITLE:	Class implementation for Insert Coupon state
//
// AUTHOR:    Aiqin Hou
//
// POS318778 Work Request:77937 Name: Alex Bronola Date:March 23, 2017
// POS108118 Work Request:65347 Name: Saiprasad Srihasam Date:August 28, 2014
// POS59130 Work Request:54416 Name:Robert Susanto Date:March 26, 2013
// POS55714 Work Request:54416 Name:Robert Susanto Date:March 26, 2013
// POS50856 Work Request:52635 Name:Robert Susanto Date:December 19, 2012
// POS33020 Work Request:52635 Name:Matt Condra    Date:November 16, 2012
// POS40386 Work Request:50864 Name:Robert Susanto Date:August 6, 2012
// POS39374 Work Request:50864 Name:Robert Susanto Date:July 24, 2012
// POS37681 Work Request:50864 Name:Robert Susanto Date:June 27, 2012
// POS36319 Work Request:50864 Name:Robert Susanto Date:June 20, 2012
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMInsertCoupon.h"           // MGV this state

#include "transbrokerssf.h"
#include "SMInProgress.h"


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("InsertCoupon")

IMPLEMENT_DYNCREATE(SMInsertCoupon, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMInsertCoupon::SMInsertCoupon()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
// POS39374 - Text flashing - rrs
SMStateBase  *SMInsertCoupon::Initialize(void)
{
    CPSXRedrawLock  csRedrawLock(_T("Display"), _T("SMInsertCoupon::Initialize"));
	
    if (m_bCouponInsertionBypassed && (m_lInsertedCouponsTotal >= lCoupons))
    {
		// POS33020: There is no coupon to insert, so go thru motions as if coupon was inserted, and then get out of here.
		trace(L6, _T("No coupon to insert so return; lCoupons=%d; m_lInsertedCouponsTotal=%d"), lCoupons, m_lInsertedCouponsTotal);
		SMStateBase *rState = OnCouponSensor();
		return rState;
    }
    //m_bCouponInsertionBypassed = false; // This will be cleared when coupon inserted or when button pressed.
	
    SMStateBase *returnState = SMInsertCouponBase::Initialize();

	nPreviousState = BES_INSERTCOUPON;
	
    //POS39374 - fix the flashing of insert coupon text - rrs
    CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
	
    if( csTBState.Find(_T("NEGATIVE_ITEM")) != -1 )
    {
        ps.SetLeadthruText(SSF_LTT_COINSTAR_BOTTLESLIP);
        ps.Message12(SSF_TXT_COINSTAR_BOTTLESLIP, true); 
    }
    else
    {
        ps.SetLeadthruText(LTT_INSERTCOUPON);
    }
    //e POS39374 - fix the flashing of insert coupon text - rrs
	
    // POS33020: Start
    CString csCurrentContext = ps.GetCurrentContext();
	
    //POS55714    
    CString csPosState = GetHookProperty(_T("__POSState"));    
    //e POS55714    
	
    if ( ( _T("1") == GetHookProperty(_T("EnableDigitalCoupons")) ) &&
		( _T("1") == GetHookProperty(_T("CouponChoiceFeatureEnabled")) )
		)
    {
        // The mPerks feature & coupon choice feature are enabled, so determine if the mPerks button should be active or not.
		
        // Enable mPerks button and shimmering video in case it was disabled previously
        //m_pAppPSX->SetConfigProperty (_T("CMButton7"), csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        //m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);	
		
        //POS55714 & 59130 don't display mperks for bottleslip, coinstar or coupon once and done
        if( !SMState::mPerksActivatedInTransaction && csPosState != _T("RESTRICTED_USE_COUPON_ACCEPTED") && csTBState.Find(_T("NEGATIVE_ITEM")) == -1 )
        {
            // mPerks number not entered yet, so display the mPerks button.
            m_pAppPSX->SetConfigProperty (_T("CMButton7"), csCurrentContext, UI::PROPERTYSTATE, (long)UI::Normal);
            m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
            
            // Display screen text of 'Please Enter mPerks Or Insert Coupon'.
            ps.Message12(SSF_TXT_COUPONCHOICE_INSERTCOUPON, true);
			
            //POS50586 - Don't play the audio in here - play it below in the :DMSaySecurity - rrs
            // Audibly say 'Enter mPerks Or Insert Coupon'. The 98 is defined in scotwave.000 file.
            //DMSayPhrase(98);
            //e - POS50586 - Don't play the audio in here - play it below in the :DMSaySecurity
        }
        else
        {
            // mPerks number already entered, so disable/hide mPerks button.
            m_pAppPSX->SetConfigProperty (_T("CMButton7"), csCurrentContext, UI::PROPERTYSTATE, (long)UI::Disabled);
            m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);	
			
            //POS55714 - rrs
            if( csPosState == _T("RESTRICTED_USE_COUPON_ACCEPTED") )
            {
                ps.Message12(SSF_LTT_COUPON_ONCEANDDONE, true);
                SendTBCommand(TBCMD_SENDKEYEDDATA, CString(_T("{F1}")));
				
                m_pAppPSX->SetConfigProperty (_T("CMButton7"), csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);                
            }
            //e POS55714 - rrs
            
        }
    }
    else
    {
        // The mPerks feature is disabled, so disable/hide mPerks button.
        m_pAppPSX->SetConfigProperty (_T("CMButton7"), csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);	
    }
    // POS33020: End
	
    return returnState;
}

void SMInsertCoupon::UnInitialize(void)
{
    SMInsertCouponBase::UnInitialize();
}

bool SMInsertCoupon::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMInsertCouponBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase  *SMInsertCoupon::OnWtDecrease(void)
{
    return SMInsertCouponBase::OnWtDecrease();
}

SMStateBase  *SMInsertCoupon::OnWtIncreaseNotAllowed(void)
{
    return SMInsertCouponBase::OnWtIncreaseNotAllowed();
}

bool SMInsertCoupon::PSAssistMenuLegalForState()
{
    return SMInsertCouponBase::PSAssistMenuLegalForState();
}

SMStateBase  *SMInsertCoupon::PSButtonGoBack(void)
{
    return SMInsertCouponBase::PSButtonGoBack();
}

SMStateBase  *SMInsertCoupon::TimedOut(void)
{    
    return SMInsertCouponBase::TimedOut();
}

////////////////////////////////////
//POS36319
SMStateBase  *SMInsertCoupon::OnCouponSensor(void)
{
    trace(L6, _T("+OnCouponSensor")); // POS33020
    SMStateBase *returnState = SMInsertCouponBase::OnCouponSensor();
	
    // POS33020: The coupon insertion was bypassed earlier but now has been completed.
    m_bCouponInsertionBypassed = false;
    m_lInsertedCouponsTotal = lCoupons;
	
    //POS40386 - don't change the TBState if the negative total balance is caused by the bottle slip or coinstar
    CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
    if( csTBState.Find(_T("NEGATIVE_ITEM")) != -1 && lBalanceDue > 0 )
    {
        csTBState += _T("_RETDN_cReturn");
        trace(L6, _T("New TB State (%s)"), csTBState);
				
		SendTBCommand(TBCMD_SET_TB_STATE,csTBState); 
		delete getRealStatePointer( returnState); 
		return STATE_NULL;
    }    
	
    trace(L6, _T("-OnCouponSensor")); // POS33020
    return returnState;
}

void SMInsertCoupon::Escalation2RAP(void)
{
    SMInsertCouponBase::Escalation2RAP();
}

SMStateBase *SMInsertCoupon::GPNotify(const GPEVT gpEvt)
{
    return SMInsertCouponBase::GPNotify(gpEvt);
}

SMStateBase  *SMInsertCoupon::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMInsertCouponBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMInsertCoupon::stateAfterPasswordVerify()
{
    return SMInsertCouponBase::stateAfterPasswordVerify();
}

void SMInsertCoupon::showScreen(bool passwordVerified)
{
    COleVariant v;
    long lReturn;
    PSXRC psxReturn;
    
    if (passwordVerified)
        return;
    
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        STATE(SecOperatorPWState)::ShowEnterID();
        
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
        
        lReturn = ps.Message12(iMessage, false);
        // Leadthru text message set
        ASSERT(0 == lReturn);
        lReturn = ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
        ASSERT(lReturn <= MAX_DIGITS_OPERATOR);
        lReturn = ps.ShowCMFrame(false);
        ASSERT(0 == lReturn);
        return;
    }
    else
    {  
        CPSXRedrawLock  csRedrawLock(_T("Display"), _T("SMInsertCoupon::ShowScreen")); //POS39374
        ps.SetCMFrame(_T("InsertCoupon"));
        v = VARIANT_TRUE;
        psxReturn = m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), _T("InsertCoupon"), UI::PROPERTYVISIBLE, v);
        ASSERT(psxReturn == PSX_SUCCESS);
		
        //POS36319 - Display the coinstar and bottle slip screen - rrs
		
        CString csTBState(_T(""));
        SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
        
        if( csTBState.Find(_T("NEGATIVE_ITEM")) != -1 )
        {
            ps.SetLeadthruText(SSF_LTT_COINSTAR_BOTTLESLIP);
            ps.Message12(SSF_TXT_COINSTAR_BOTTLESLIP, true);
			
            DMSaySecurity( SEC_NONE, ps.GetSoundName(ENTERCASHBACKAMOUNT));
			
            m_pAppPSX->SetConfigProperty (_T("CMButton7"), ps.GetCurrentContext(), UI::PROPERTYVISIBLE, VARIANT_FALSE);     //POS59130
        }
        else
        {
            ps.SetLeadthruText(LTT_INSERTCOUPON);
        }
    }
    
    ps.ShowCMTBText(csTBMessage2Scot);
    lReturn = ps.ShowCMFrame(false);
    ASSERT(0 == lReturn);
}

//POS37681 - prevent the original audio to be played during bottle slip & coinstar
long SMInsertCoupon::DMSaySecurity(const long nMsg, LPCTSTR szWave, HANDLE hWaitEvent/*=0*/)
{
    long rc = 0;
	
    CString csTBState(_T(""));
    SendTBCommand(TBCMD_GET_TB_STATE, &csTBState);
	
    //POS55714    
    CString csPosState = GetHookProperty(_T("__POSState"));    
    //e POS55714 
	
    if( csTBState.Find(_T("NEGATIVE_ITEM")) != -1 )
    {
        //POS37681 - The CoinStar and Bottle slip uses the ENTERCASHBACKAMOUNT wav index that is not used by the base team
        rc = SMInsertCouponBase::DMSaySecurity(nMsg, ps.GetSoundName(ENTERCASHBACKAMOUNT), hWaitEvent);
    }
    //POS50856 - Don't play default Insert Coupon audio if there is a DigitalCoupon choice - rrs    
    else if ( ( _T("1") == GetHookProperty(_T("EnableDigitalCoupons")) ) &&
		( _T("1") == GetHookProperty(_T("CouponChoiceFeatureEnabled")) ) && szWave == ps.GetSoundName(INSERTCOUPON) )
    {
        // RFQ55714 play normal insertcoupon audio for the once and done coupon
        if( !SMState::mPerksActivatedInTransaction && csPosState != _T("RESTRICTED_USE_COUPON_ACCEPTED") )
        {
            // Audibly say 'Enter mPerks Or Insert Coupon'. The 98 is defined in scotwave.000 file.
            rc = SMInsertCouponBase::DMSaySecurity(nMsg, ps.GetSoundName(98), hWaitEvent);            
        }
        else
        {
            rc = SMInsertCouponBase::DMSaySecurity(nMsg, szWave, hWaitEvent);
        }
    }
    //e POS50856 - Don't play default Insert Coupon audio if there is a DigitalCoupon choice - rrs
    else
    {
        rc = SMInsertCouponBase::DMSaySecurity(nMsg, szWave, hWaitEvent);
    }
	
	return rc;
}

// POS33020: Start
//---------------------------------------------------------------------------------------------------
// Trigger the mPerks Telephone Entry Screen (POS33020)
//---------------------------------------------------------------------------------------------------
SMStateBase *SMInsertCoupon::PSButton7(void)
{
	CString csCurrentContext = ps.GetCurrentContext();
	
	// Disable RAP Assist Mode button
	ra.RequestEnableAssistMode(false);
	
    // Tell security that we are exiting from InsertCoupon state
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, csCurrentContext);
	
	// Disable button after press to prevent multiple key presses; other race conditions
	COleVariant v = (long)UI::Disabled;
	m_pAppPSX->SetConfigProperty (_T("CMButton7"), csCurrentContext, UI::PROPERTYSTATE, v);
	
	m_pAppPSX->SetConfigProperty (_T("mPerksButtonVideo"), csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);	
	
	// Send sequence of softkeys to the sales app to trigger the entry of the mPerks phone number.
	SendTBCommand(TBCMD_SET_TB_STATE,_T("TB_DIGITAL_COUPON"));
	
	// The inserting of the coupon was bypassed when this mPerks button was tapped.
	// So indicate to the system that the coupon needs to be inserted once the mPerks
	// processing is complete.
	m_bCouponInsertionBypassed = true;
	m_lInsertedCouponsTotal = lCoupons - 1;
	trace(L6, _T("Insert coupon bypassed at lCoupons=%d; m_lInsertedCouponsTotal=%d"), lCoupons, m_lInsertedCouponsTotal);
	
	// Let's wait for mPerks phone# entry screen to become active.
	RETURNSTATE(InProgress);
}
// POS33020: End
