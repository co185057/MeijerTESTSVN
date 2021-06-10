//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLimitedEBTBenefitBase.cpp
//
// TITLE: Class impliementation for Limited EBT Benefit
//
//
// AUTHOR:    Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMLimitedEBTBenefit.h"     // MGV this state
#include "SMTakeCard.h"             // MGV take Card state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("LimitedEBTBenefitBase")

IMPLEMENT_DYNCREATE(SMLimitedEBTBenefitBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(LimitedEBTBenefit)

DEFINE_TIMESTAMP

////////////////////////////////////////
SMLimitedEBTBenefitBase::SMLimitedEBTBenefitBase(const TBTENDERTYPE nTyp, const long lLimit)
: nTndrType(nTyp),
  lTenderLimit(lLimit), csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMLimitedEBTBenefitBase::SMLimitedEBTBenefitBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMLimitedEBTBenefitBase::Deliver(const TBTENDERTYPE nTyp, const long lLimit)
{
  nTndrType = nTyp;
  lTenderLimit = lLimit;

  return this;
}

bool SMLimitedEBTBenefitBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both dwords
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMLimitedEBTBenefitBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((TBTENDERTYPE)DLParamDWORD(dlParameterList),    // nTyp
                DLParamDWORD(dlParameterList));                 // lAmt
        return true;
    }
    else
    {
        trace(L2, _T("+SMLimitedEBTBenefitBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
/////////////////////////////////////////////
SMStateBase  *SMLimitedEBTBenefitBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("LimitedEBTBenefit") );	// RFC 330050	
	//Update RAP window with SCOT state
	UpdateStateName(ps.GetPSText(LTT_FOODSTAMPLIMIT, SCOT_LANGUAGE_PRIMARY));
	bUpdateFoodStampTotal = true; 

	PSTEXT msg = MSG_FOODSTAMPLIMIT;

	if (nTndrType==TB_TENDER_EBT)
	{
		msg = MSG_CASHBENEFITLIMIT;
	}
	
    csCurContext = _T("LimitedEBTBenefit");  //  Tar 238987
	CString csWork;
    csWork.Format(ps.GetPSText(msg),ps.FmtDollar(lTenderLimit));
	//COleVariant v;
    if(!(co.fOperationsPartialIntegratedPINPad && !co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bIntegratedMSR))  
	{
		//Non-PIP
		//v=VARIANT_TRUE;
		ps.SetCMFrame(csCurContext);
		ps.SetButtonText(_T("CMButton1Med"), BTT_YES);
		ps.SetButtonText(_T("CMButton2Med"), BTT_NO);
		ps.Message12(csWork, true);
	}
	else
	{  
		//PIP
		//v=VARIANT_FALSE;
		ps.SetCMFrame(_T("LimitedEBTBenefitOK"));  //  Tar 388223
		ps.SetButtonText(_T("CMButton1Med"), BTT_OK);
		if(nTndrType==TB_TENDER_EBT)
			ps.Message12(MSG_INSUFFICIENTCASHBENEFIT, true);
		else
			ps.Message12(MSG_INSUFFICIENTFOODSTAMP, true);
	}
    //m_pAppPSX->SetConfigProperty(_T("CMButton2Med"), csCurContext, UI::PROPERTYVISIBLE, v);
	//ps.CMButtonFlash(_T("CMButton1Med"), true);
	DMSayPhrase(CARDNOTACCEPTED);
    ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
	ps.ShowCMFrame();

    DisplayAuxScreen(AUX_SCREEN_USE_TOUCHSCREEN, true);	// TAR 228153
    return STATE_NULL;                // stay here
}

SMStateBase  *SMLimitedEBTBenefitBase::PSButton1(void)// yes
{
  // the BALANCE/CHANGE message will change us to either
  // STCashPayment or STTakeChange or STTakeReceipt
  if(!(co.fOperationsPartialIntegratedPINPad && !co.TenderList[ps.m_languageUsed][nTenderButtonIndex-1].bIntegratedMSR))  
       return TBEnterTender(nTndrType, lTenderLimit,0);
  else
	  return setAndCreateAnchorState(BES_FINISH);
}

SMStateBase  *SMLimitedEBTBenefitBase::PSButton2(void)// No, Cancel
{
  setAnchorState(BES_FINISH);
  CREATE_AND_DISPENSE(TakeCard)(false);
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMLimitedEBTBenefitBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_STATERETURN);	
}



///////////////////////////////////////////////////////////////////////////
SMStateBase *SMLimitedEBTBenefitBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_STATERETURN);
	else
		return STATE_NULL;
}
// + RFC 330050
void SMLimitedEBTBenefitBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("LimitedEBTBenefit") );	
}
// - RFC 330050
