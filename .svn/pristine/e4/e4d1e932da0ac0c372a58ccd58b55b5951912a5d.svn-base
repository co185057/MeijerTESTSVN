//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAssistedTenderBase.cpp
//
// TITLE: Class impliementation for Assisted Tender State
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"                 
#include "SMState.h"                
#include "SMAssistedTender.h"       

#define COMP_ID ID_SM               
#define T_ID    _T("AssistedTenderBase")

DEFINE_TIMESTAMP

////////////////////////////////////////
SMAssistedTenderBase::SMAssistedTenderBase(LPCTSTR sDesc)
   :STATE(RAPDataNeeded)(sDesc, DATANEEDED_ASSISTEDTENDER),
   m_sDesc(sDesc)
{
IMPLEMENT_TIMESTAMP
}

SMAssistedTenderBase::SMAssistedTenderBase()
   :STATE(RAPDataNeeded)()
{
IMPLEMENT_TIMESTAMP
}

//////////////////////////////

SMStateBase  *SMAssistedTenderBase::Deliver(LPCTSTR sDesc)
{
   return STATE(RAPDataNeeded)::Deliver(sDesc, DATANEEDED_ASSISTEDTENDER);
}
//////////////////////////////
bool SMAssistedTenderBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be One parameter, a string
    if (dlParameterList.GetDescriptorCount() == 1)
    {
        trace(L6, _T("+SMAssistedTenderBase::DeliverDescriptorList() - Parameter Count of 1"));
        Deliver((LPCTSTR)DLParamCString(dlParameterList));   // pPtr
        return true;
    }
    else
    {
        trace(L2, _T("+SMAssistedTenderBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}

//////////////////////////////


SMStateBase *SMAssistedTenderBase::Initialize()
   {
   //TAR208534 BZ 06/27/02: Play wave file
   //TAR209636   DMSaySecurity(SEC_NONE, ps.GetSoundName(CHECKTOPAYSTATION));	
	// TAR+ 288942 Disable scanner when paying in check
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("AssistedTender") );	// RFC 330050
	SMStateBase* rState=STATE(RAPDataNeeded)::Initialize();
    EnableHandheldAssistMode(true);
	DMScannerDisable();
   return rState;
   // TAR- 288942 Disable scanner when paying in check
   }

void SMAssistedTenderBase::UnInitialize()
   {
   STATE(RAPDataNeeded)::UnInitialize();
   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("AssistedTender") );		// RFC 330050
   }


void SMAssistedTenderBase::SetupViewData()
   {
   m_sView = _T("AssistedTender");
   m_ViewText = 0;   // ViewText is not used by base class since we handle it here.
   // Call base class to set up the view data based on out view string.
   STATE(RAPDataNeeded)::SetupViewData();
   }


SMStateBase *SMAssistedTenderBase::HandleRAPData()
   {
   SMStateBase *pNextState = STATE_NULL;
   if (m_sButton == _T("cReturn"))
   {
      pNextState = createAnchorState();
	  ClearSecurityViolationLight();  // TAR 237973 Clear the light when clear immediate intervention
	  ra.OnNoRAPDataNeeded();
	  g_bAssistModeDataNeeded = false;
   }

   return pNextState;
   }

SMStateBase *SMAssistedTenderBase::PSButtonGoBack(void)
   {
   trace(L6, _T("+SMAssistedTenderBase::PSButtonGoBack()"));
   ra.OnNoRAPDataNeeded();
   ClearSecurityViolationLight();  
   g_bAssistModeDataNeeded = false;
   bSayAudioAssistedTender = false;
   trace(L6, _T("-SMAssistedTenderBase::PSButtonGoBack()"));
   return createAnchorState();
   }

SMStateBase *SMAssistedTenderBase::PSButton1(void)
   {
   trace(L6, _T("+SMAssistedTenderBase::PSButton1()"));
   promptForOperator();
   trace(L6, _T("-SMAssistedTenderBase::PSButton1()"));
   return STATE_NULL;
   }

void SMAssistedTenderBase::showScreen(bool passwordVerified)
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
    ps.AllowInput(MAX_DIGITS_OPERATOR,false,false);
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
       sMessage.Format(ps.GetPSText(MSG_TAKEPAYMENTTOCASHIER), _T("'")+sTender+_T("'"));//tar 266220
		//TAR209636
	   if (!bSayAudioAssistedTender )  //Tar 353054
	   {
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
		   bSayAudioAssistedTender = true;
	   }
	   

   }
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

SMStateBase *SMAssistedTenderBase::OnWtIncreaseNotAllowed()	 
{
	if(m_View > DATANEEDED_NOVIEW) // 242625 and tar 252987
	{
	  m_saveDataNeededView = m_View;
	}

	return FinalizationWtIncrease(BES_ASSISTEDTENDER);	

}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMAssistedTenderBase::OnWtDecrease()
{
	if(m_View > DATANEEDED_NOVIEW) // 242625 and tar 252987
	{
	  m_saveDataNeededView = m_View;
	}
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering && !(lBalanceDue < 0.01))
		return FinalizationWtDecreaseNotAllowed(BES_ASSISTEDTENDER);
	else
		return STATE_NULL;
}

IMPLEMENT_DYNCREATE(SMAssistedTenderBase, CObject)// MFC Runtime class/object information
