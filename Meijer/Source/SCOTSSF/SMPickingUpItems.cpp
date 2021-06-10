//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPickingUpItems.CPP
//
// TITLE: Class header for PickingUpItems
//
// $Header:
//
// AUTHOR:    Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMPickingUpItems.h"           // MGV this state

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("PickingUpItems")

IMPLEMENT_DYNCREATE(SMPickingUpItems, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMPickingUpItems::SMPickingUpItems()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMPickingUpItems::Initialize(void)
{
    return SMPickingUpItemsBase::Initialize();
}

void SMPickingUpItems::UnInitialize(void)
{
    SMPickingUpItemsBase::UnInitialize();
}

SMStateBase  *SMPickingUpItems::TimedOut(void)
{
    return SMPickingUpItemsBase::TimedOut();
}

SMStateBase  *SMPickingUpItems::PSButton1(void)
{
    return SMPickingUpItemsBase::PSButton1();
}

SMStateBase  *SMPickingUpItems::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMPickingUpItemsBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMPickingUpItems::OnWtIncreaseNotAllowed(void)
{
    return SMPickingUpItemsBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMPickingUpItems::OnWtDecrease(void)
{
    return SMPickingUpItemsBase::OnWtDecrease();
}

SMStateBase  *SMPickingUpItems::PSButtonGoBack(void)
{
    return SMPickingUpItemsBase::PSButtonGoBack();
}

void SMPickingUpItems::showScreen(bool passwordVerified)
{
    if (passwordVerified)
        return;
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        // NewUI
        STATE(SecOperatorPWState)::ShowEnterID();   //SR93 Biometrics support			

        ps.ShowNumericKeypad();
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
        // Leadthru text message set

        ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
        ps.ShowCMFrame(false);
        // End NewUI
        return;
    }
    else
    {
        // TAR 200470 - this problem was discovered as a side effect of the show being slow
        // NewUI
        ps.SetCMFrame(_T("PickingUpItems"));
        // the wave and background changes depending on if Belt is installed or not
        if (dm.fStateTakeawayBelt)
            ps.SetCMBackground(BMP_SECVIOLATIONRETURNINGITEMS);
        else
            ps.SetCMBackground(BMP_SECVIOLATIONRETURNINGITEMS);			
        // Show the Store Login button 
        // TAR 237964 Enable Store Log-in button
        m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        // end TAR 237964
        // End NewUI
    }
    
    
    if (dm.fStateTakeawayBelt)
    {
        ps.SetCMLeadthruText(LTT_BELT_ATTENTION);
        ps.Message12(MSG_BELT_ITEM_REMOVED, true);
    }
    else
    {
        ps.SetCMLeadthruText(MSG_BAGGING_ALERT);
        //BZ 06/07/02 TAR 203537 begin
        if(co.fOperationsAllowItemRemovedBtn)
            ps.Message12(MSG_RETURNITEMS_TO_BAGGING,true);
        else
            ps.Message12(MSG_OVERALL_WEIGHT_DECREASE, true);
        //TAR 203537 end 
    }
    
    //BZ 06/07/02 TAR 203537 begin
    COleVariant v;
    if(co.fOperationsAllowItemRemovedBtn)
    {
        v=VARIANT_TRUE;
        ps.SetButtonText(_T("CMButton1Med"), BTT_ITEM_REMOVED);	
    }
    else
    {
        v=VARIANT_FALSE;
    }
    m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, v);
    //TAR 203537 end
    ps.ShowCMTBText(csTBMessage2Scot);	//thp
    // End NewUI
    
    DisplayAuxScreen(AUX_SCREEN_DEFAULT, true); //tar 193987
    ps.ShowCMFrame(false);
}

SMStateBase  *SMPickingUpItems::stateAfterPasswordVerify()
{
    return SMPickingUpItemsBase::stateAfterPasswordVerify();
}

SMStateBase  *SMPickingUpItems::OnBackToLGW(void)
{
    return SMPickingUpItemsBase::OnBackToLGW();
}

SMStateBase  *SMPickingUpItems::SASecMgrParse( const MessageElement* me )
{
    return SMPickingUpItemsBase::SASecMgrParse( me );
}


void SMPickingUpItems::Escalation2RAP(void)
{
    SMPickingUpItemsBase::Escalation2RAP();
}

SMStateBase *SMPickingUpItems::GPNotify(const GPEVT gpEvt)
{
    return SMPickingUpItemsBase::GPNotify(gpEvt);
}
