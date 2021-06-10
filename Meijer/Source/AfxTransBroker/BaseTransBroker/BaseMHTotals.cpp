// BaseMHTotals.cpp: implementation of the CBaseMHTotals class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHTotals.h"
#include "traceSystem.h"
#include "TBUtilities.h"
#include "BaseCurrentTotals.h"
#include "BaseObjectManager.h"


#define T_ID   _T("CBaseMHTotals")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseMHTotals::CBaseMHTotals()
{
    
}

CBaseMHTotals::~CBaseMHTotals()
{
    
}


TBRC CBaseMHTotals::GetAvailableBenefitAmount(PSEBTAvailBalance psEBTAvailBalance)
{
    TBRC rc = TB_SUCCESS;
    LPCTSTR  lpThisName = _T("CBaseMHTotals::GetAvailableBenefitAmount()");
    tbtrace( TM_ENTRY, _T("+%s"), lpThisName);

    CBaseCurrentTotals* pCurrTotals = CCustomerCurrentTotals::instance();
    psEBTAvailBalance->lAvailBal = pCurrTotals->GetAvailableBenefitAmount();

    tbtrace( TM_ENTRY, _T("-%s"), lpThisName);
    return rc;
}


TBRC CBaseMHTotals::GetAvailableFoodStampAmount(PSEBTAvailBalance psEBTAvailBalance)
{
    TBRC rc = TB_SUCCESS;
    tbtraceIO(_T("GetAvailableFoodStampAmount"));

    CBaseCurrentTotals* pCurrTotals = CCustomerCurrentTotals::instance();
    psEBTAvailBalance->lAvailBal = pCurrTotals->FoodStampAmountLong();

    return rc;
}


TBRC CBaseMHTotals::GetTotalDetails(PSTotalDetails psDetails)
{
    tbtrace(TM_ENTRY, _T("+-GetTotalDetails()"));
    
    CBaseCurrentTotals* pCurrTotals = CCustomerCurrentTotals::instance();
    
    psDetails->lTaxDue       = pCurrTotals->TaxDueLong();			     
    psDetails->lTotal        = pCurrTotals->TotalDueLong();
    psDetails->lDiscount     = pCurrTotals->TotalDiscountLong();
    psDetails->lFoodStampDue = pCurrTotals->FoodStampAmountLong();
    psDetails->lRewardPoints = pCurrTotals->RewardPoints();

    psDetails->lErrDescriptionLength =
		 CopyString(psDetails->szErrDescription, pCurrTotals->ErrorDescription());

    return TB_SUCCESS;
}


TBRC CBaseMHTotals::ParseFoodStampTotal(PSTotalDetails psTotalDetails)
{
    tbtrace( TM_ENTRY, _T("ParseFoodStampTotal()"));
    return GetTotalDetails(psTotalDetails);
}


// This is required functionality for all MH classes
TBSTATE CBaseMHTotals::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_TOTAL;
	 CTotalDetailsMsg *pMsg= dynamic_cast<CTotalDetailsMsg *> (pMsgObj);
	 ASSERT(pMsg);
	 if (pMsg) {
	    CBaseCurrentTotals* pCurr = CCustomerCurrentTotals::instance();
		 pCurr->Refresh(pMsg);
	 }

    tbtrace( TM_ENTRY, _T("+-ProcessMessage()"));
    return rc;
}


TBRC CBaseMHTotals::RequestFoodStampEligibleTotal()
{
    tbtrace( TM_ENTRY, _T("RequestFoodStampEligibleTotal"));
    CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_FOODSTAMPELIGIBLETOTAL);
    return TB_SUCCESS;
}
