//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAssistedTender.CPP
//
// TITLE: Class implementation for Finish State
//
// POS108118 Work Request:65347 Name: Saiprasad Srihasam Date:August 28, 2014
// POS18905 WORK REQUEST:16656 Name: Robert Susanto Date: October 11, 2011
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMAssistedTender.h"       // this state
#include "TransBrokerSSF.h"

//pos18905
#include "SMInProgress.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("AssistedTender")

IMPLEMENT_DYNCREATE(SMAssistedTender, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////
// Constructor
////////////////////
SMAssistedTender::SMAssistedTender()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase *SMAssistedTender::Initialize()
{
    return SMAssistedTenderBase::Initialize();
}

void SMAssistedTender::UnInitialize()
{
    SMAssistedTenderBase::UnInitialize();
}

bool SMAssistedTender::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMAssistedTenderBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase *SMAssistedTender::OnWtIncreaseNotAllowed(void)
{
    return SMAssistedTenderBase::OnWtIncreaseNotAllowed();
}

SMStateBase *SMAssistedTender::OnWtDecrease(void)
{
    return SMAssistedTenderBase::OnWtDecrease();
}

bool SMAssistedTender::IsLoginBypassAllowed()
{
    return SMAssistedTenderBase::IsLoginBypassAllowed();
}

void SMAssistedTender::SetupViewData()
{
    SMAssistedTenderBase::SetupViewData();
}

SMStateBase *SMAssistedTender::HandleRAPData()
{
    return SMAssistedTenderBase::HandleRAPData();
}

void SMAssistedTender::showScreen(bool passwordVerified)
{
    saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
    int nNumApprovals = 0;
    
    // This state does not need to reshow if the password
    // was verified, since we'll be leaving now
    if (passwordVerified)
        return;
    // NewUI
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
        STATE(OperatorPasswordState)::ShowEnterID();  //SR93 Biometrics
        ps.SetCMLeadthruText(TXT_ENTER_ID);

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
        ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
        trace(L6, _T("-SMRAPDataNeeded::showScreen() (PROMPTING_FOR_OPERATOR return path)"));
        ps.ShowCMFrame();
        return;
    }
    ps.SetCMFrame(_T("AssistedTender"));
    ps.ShowAmountDueAmountPaid(lBalanceDue, lTotalPaid + nDMCashInserted);
    ps.CMButton(_T("ButtonHelp"), BTT_HELP, false);
    // End NewUI
    
    CString sTender = GetTenderButton().csTenderText;
    CString sLTT, sMessage;    
    
    if (GetTenderButton().nTenderType == TB_TENDER_OTHERPAYMENT)
    {
        sLTT = sTender;
		//(+) POS307806
        //sMessage.Format(ps.GetPSText(MSG_TAKEPAYMENTTOCASHIER), _T("'")+sTender+_T("'"));//tar 266220
		sMessage = ps.GetPSText(MSG_TAKEPAYMENTTOCASHIER);
		//(-) POS307806	
        if (!bSayAudioAssistedTender )  //Tar 353054
        {
            //TAR209636
            DMSaySecurity(SEC_NONE, ps.GetSoundName(PAYMENTTOPAYSTATION));
			bSayAudioAssistedTender = true;
        }
        
    }    
    /*
    else if (bPaperSlip)
    {
	   sLTT = ps.GetPSText(SSF_TXT_RAPPAPERSLIP);
       sMessage = ps.GetPSText(SSF_TXT_WAITFORPAPERSLIP);
       }
    */
    else
    {
        sLTT.Format(ps.GetPSText(LTT_ASSISTEDTENDER), sTender);
        sMessage.Format(ps.GetPSText(MSG_TAKEPAYMENTTOCASHIER), sTender);
        
        if (!bSayAudioAssistedTender )  //Tar 353054
        {
            //TAR209636
            if(GetTenderButton().nTenderType == TB_TENDER_CHECK)
            {
                
                DMSaySecurity(SEC_NONE, ps.GetSoundName(CHECKTOPAYSTATION));
            }
            else
                DMSaySecurity(SEC_NONE, ps.GetSoundName(PAYMENTTOPAYSTATION));
            //TAR209636
            bSayAudioAssistedTender = true;  //Tar 353054
        }
        
        
    }

    // POS18905 - rrs
    if( GetHookProperty(_T("PropHasRebateCheck")) == _T("1") )
    {
         sTender = ps.GetPSText(SSF_BTT_REBATECHECK);
         sLTT = sTender;
         sMessage.Format(ps.GetPSText(SSF_MSG_SUPERVISOR_REBATECHECK), _T("'")+sTender+_T("'"));

         ps.Button(_T("ButtonGoBack"), BTT_CANCEL, true);
    }
    // e - POS18905

    g_bAssistModeDataNeeded = true; // Tar 213074 - Set flag indicating assisted payment
    trace(L7, _T("+rp.m_bRapAssisted"));
    rp.m_bRapAssisted = true;
    CString csRAP;
    csRAP = _T("RAP");
    rp.SetTenderTerminalNumber(csRAP);
    trace(L7, _T("-rp.m_bRapAssisted<%d>"),rp.m_bRapAssisted );
    ps.SetCMLeadthruText(sLTT);
    ps.Message12(_T("\n\n") + sMessage, true);
    ps.ShowCMFrame();
}

// POS18905
SMStateBase *SMAssistedTender::PSButtonGoBack(void)
{
    trace(L6, _T("+SMAssistedTender::PSButtonGoBack()"));
	if( GetHookProperty(_T("PropHasRebateCheck")) == _T("1") )
	{
        CString csPropRebateCheck = _T("PropHasRebateCheck");
		CString csPropRebateCheckValue = _T("0");        
		
		SendTBCommand(TBCMD_SET_HOOKPROPERTY, &csPropRebateCheck, &csPropRebateCheckValue);
        SendTBCommand(TBCMD_ENTERING_FINISH_AND_PAY, true);
		setAnchorState(BES_FINISH);
		
        CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), nTimeOut);
	}
	
   return SMAssistedTenderBase::PSButtonGoBack();
}