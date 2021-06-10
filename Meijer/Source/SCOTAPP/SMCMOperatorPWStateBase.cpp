//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCMOperatorPWStateBase.cpp
//
// Description: This state handles authentication in Cash Management.
//
// Author:  Jeffrey Yu
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"                          // MS MFC always needed first
#ifndef _CPPUNIT
#include "Common.h"

#include "SMCMOperatorPWState.h"
#include "CMInterface.h"
#else
#include "SMCMOperatorPWStateBase.h"
#include "TestMacros.h"
#endif //CPPUNIT
#include "Base64EncodeDecode.h"

#define COMP_ID ID_SM                         // Base state component
#define T_ID _T("SMCMOperatorPWStateBase")

const LPCTSTR cGO_BACK_BUTTON_CTRLID(_T("ButtonGoBack"));

#ifndef _CPPUNIT
#include "MethodTrace.h"
#endif
IMPLEMENT_DYNCREATE(SMCMOperatorPWStateBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////////////
SMCMOperatorPWStateBase::SMCMOperatorPWStateBase()
: m_bAuthenticationOptional(true) 
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////////////
SMStateBase * SMCMOperatorPWStateBase::PSButtonGoBack()
{
    TRACE_METHOD(_T("SMCMOperatorPWStateBase::PSButtonGoBack"));

    if( PROMPTING_FOR_OPERATOR == inputMode)
    {
        if (co.m_bOperationsUseFingerPrintDevice)
        {
            FT_Abort();
        }
        m_cmMgr.CancelAuthenticationRequest();
        return STATE_NULL;
    }

    return STATE(OperatorPasswordState)::PSButtonGoBack();
}

void SMCMOperatorPWStateBase::showScreen(bool passwordVerified)
{
    if (!passwordVerified)
    {
        if (inputMode == PROMPTING_FOR_OPERATOR)
        { 
            ShowEnterID();  

            ps.SetCMLeadthruText(TXT_ENTER_ID);
            ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
            ps.ShowCMTBText(csTBMessage2Scot);
            ps.ShowFrame(false);
        }
    }
    else
    {
        trace(L6, _T("SMCMOperatorPWStateBase::showScreen password verified"));        
    }
}

SMStateBase *SMCMOperatorPWStateBase::PSParse( const MessageElement* pMsg)
{
    TRACE_METHOD(_T("SMCMOperatorPWStateBase::PSParse"));


    switch(pMsg->psInfo.nEvt)
    {
    case PS_AUTHENTICATION_REQUEST_OPTIONAL:
        trace(L6, _T("Got Authentication Request Optional!  CTX: [%s]"), pMsg->psInfo.pszContext);
        m_bAuthenticationOptional = true;
        promptForOperator();
        break;
    case PS_AUTHENTICATION_REQUEST_REQUIRED:
        trace(L6, _T("Got Authentication Request Required!  CTX: [%s]"), pMsg->psInfo.pszContext);
        m_bAuthenticationOptional = false;
        promptForOperator();
        break;
    case PS_AUTHENTICATION_INVALID_ID:
    case PS_AUTHENTICATION_INVALID_PW:
        trace(L6, _T("Got Authentication invalid ID/PW!  CTX: [%s]"), pMsg->psInfo.pszContext);
        displayInvalidMessage();
        break;
    case PS_AUTHENTICATION_SUCCESS:
        inputMode = OPERATOR_PASSWORD_ACCEPTED;
        stateAfterPasswordVerify();
        break;
    default:
        break;
    }

    return STATE(OperatorPasswordState)::PSParse(pMsg);
}

void SMCMOperatorPWStateBase::promptForOperator()
{
    TRACE_METHOD(_T("SMCMOperatorPWStateBase::promptForOperator"));

    STATE(OperatorPasswordState)::promptForOperator(); 

    if(m_bAuthenticationOptional)
    {
        ShowGobackButton();
    }
    else
    {
        HideGoBackButton();
    }
}

void SMCMOperatorPWStateBase::promptForPassword()
{
    TRACE_METHOD(_T("SMCMOperatorPWStateBase::promptForPassword"));

    STATE(OperatorPasswordState)::promptForPassword();

    ShowGobackButton();
}

void SMCMOperatorPWStateBase::ShowGobackButton()
{
    m_pAppPSX->SetConfigProperty(cGO_BACK_BUTTON_CTRLID, m_csFrameName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
}

void SMCMOperatorPWStateBase::HideGoBackButton()
{
    m_pAppPSX->SetConfigProperty(cGO_BACK_BUTTON_CTRLID, m_csFrameName, UI::PROPERTYVISIBLE, VARIANT_FALSE);  
}

//////////////////////////////////////////////
SMStateBase * SMCMOperatorPWStateBase::PSEnterKey()
{
#ifndef _CPPUNIT
    //tar 244851 - workaround for tri-light yellow/green and red on at same time problem
    //turn the yellow/green light off if Red is already on and any red approval is pending
    if (dm.getTriColorLightColor() == DM_TRICOLORLIGHT_RED  && AreAnyRedApprovalsPending())
    {
        // (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                           DM_TRICOLORLIGHT_OFF,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
                           
        // (*) SR898 ====================
        DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                           DM_TRICOLORLIGHT_OFF,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
    }

  // Enter key not valid after valid operator/password entered
  if (inputMode != OPERATOR_PASSWORD_ACCEPTED)
  {
    DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true); //228072 dislay default msg on enter password. LPM021803
    if (getOperatorPassword())
      return stateAfterPasswordVerify();
  }

#endif //CPPUNIT
  return STATE_NULL;
}

bool SMCMOperatorPWStateBase::getOperatorPassword()
{
    TRACE_METHOD(_T("SMCMOperatorPWStateBase::getOperatorPassword"));

    if(GetUserInfo())
    {
        OnUserCredentialsAquired();
    }
    return false;
}

bool SMCMOperatorPWStateBase::OnUserCredentialsAquired()
{
    TRACE_METHOD(_T("SMCMOperatorPWStateBase::OnUserCredentialsAquired"));

    CString csTempId = csOperator;
    CString csTempPw = csPassword;
    if(co.GetfOperationsEncodeOperatorIDPW())
    {
        CString csPrefix(_T("BASE64:"));
        CBase64EncodeDecode base64;
        csTempId = base64.GetBase64EncodeWithPrefix(csPrefix, csTempId);
        csTempPw = base64.GetBase64EncodeWithPrefix(csPrefix, csTempPw);
    }    
     //Send credential to CM and let it validate
    m_cmMgr.SetCredentials(csTempId, csTempPw);

    return false;
}
