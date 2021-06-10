//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateSA.CPP
//
// TITLE: SA SMState functions, implement all SMState::SA* functions
//
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include "Common.h"
#include "SMState.h"
#include "MobileConstantsSSF.h"

#define COMP_ID ID_SM			// base state component
#define T_ID    _T("SMsa")

long SMState::SASendSCOTInput(long lInputID,
                              LPCTSTR strUPCNumber,
                              long lWeight,
                              long lPrice,
                              long lQuantity,
                              long lDepartment,
                              bool bIsPriceEmbedded  )
{
    long lReturn = SMStateBase::SASendSCOTInput(lInputID, strUPCNumber, lWeight, lPrice, lQuantity, lDepartment, bIsPriceEmbedded  );
    switch (nSAPrevSCOTInputID)
    {
    case ITEMIZATION:
        SetTransactionStatus(szTSItemization);
        break;
    case TRANSACTION_COMPLETE: // state
        SetTransactionStatus(szTSTransactionComplete);
        break;
    case TENDER_START:			// state
        SetTransactionStatus(szTSTenderStart);
        break;
    case TRANSACTION_CONTINUE:			// state
        SetTransactionStatus(szTSTransactionContinue);
        break;
    case UPC_SCANNED:			// ok
    case UPC_MANUAL:			// ok
        if(g_bInRecallMode )
        { 
            SetTransactionStatus(szTSMobileItemSaleStarted);
        }
        else
        {
            SetTransactionStatus(szTSItemSaleStarted);
        }
        break;
    case ITEM_SOLD_SCAN:
    case ITEM_SOLD_MANUAL:
        SetTransactionStatus(szTSItemSold);
        break;
    case CUSTOMER_MODE:
        SetTransactionStatus(szTSCustomerMode);
        break;
    case MAINTENANCE_MODE:						// Security Maintenance Mode
    case STORE_MODE:
        SetTransactionStatus(szTSStoreMode);
        break;
    case ITEM_VOID_SCAN:
    case ITEM_VOID_MANUAL:
    case COUPON_VOID_SCAN:
    case COUPON_VOID_MANUAL:
        SetTransactionStatus(szTSItemVoided);
        break;
    case CANCEL_ITEM:
    case COUPON_REFUSED:
        SetTransactionStatus(szTSItemVoided);
        break;
    case BALANCE_PAID:			
        SetTransactionStatus(szTSBalancePaid);
        break;
    }
    mobileCart.SendTransactionStatus(GetTransactionStatus());
    return lReturn;
}

void SMState::SAWLDBScannerEnable(void)
{
    SMStateBase::SAWLDBScannerEnable();
}

void SMState::SAWLDBScannerDisable(void)
{
    SMStateBase::SAWLDBScannerDisable();
}

void SMState::SASMMDisplayTolInfo()
{
    SMStateBase::SASMMDisplayTolInfo();
}

SMStateBase * SMState::SASMMSecOffline()
{
    return SMStateBase::SASMMSecOffline();
}

CString SMState::SASMMConvertTol()
{
    return SMStateBase::SASMMConvertTol();
}

long SMState::SASMMConvertToPPWU()
{
    return SMStateBase::SASMMConvertToPPWU();
}

long SMState::SALbsToKgs(long Weight)
{
    return SMStateBase::SALbsToKgs( Weight);
}

CString SMState::GetExpectedWts()
{
    return SMStateBase::GetExpectedWts();
}

SMStateBase * SMState::FinalizationWtIncrease( BEST nextState)
{
    return SMStateBase::FinalizationWtIncrease(nextState);
}

void SMState::AttractWtDecrease(void)
{
    SMStateBase::AttractWtDecrease();
}

SMStateBase * SMState::FinalizationWtDecreaseNotAllowed( BEST nextState)
{
    return SMStateBase::FinalizationWtDecreaseNotAllowed(nextState);
}

void SMState::FinalizationWtDecrease(void)
{
    SMStateBase::FinalizationWtDecrease();
}

SMStateBase * SMState::VoidingWtIncrease(BEST prevState, BEST nextState)
{
    return SMStateBase::VoidingWtIncrease( prevState,  nextState);
}

SMStateBase * SMState::VoidingWtDecrease(void)
{
    return SMStateBase::VoidingWtDecrease();
}

SMStateBase * SMState::ThresholdViolRAPClearSecBtn(BEST ReturnState)
{
    return SMStateBase::ThresholdViolRAPClearSecBtn( ReturnState);
}

SMStateBase * SMState::ThresholdViolWtIncreaseNotAllowed(BEST prevState, BEST ThreshViolationState)
{
    return SMStateBase::ThresholdViolWtIncreaseNotAllowed( prevState,  ThreshViolationState);
}

SMStateBase * SMState::ThresholdViolWtDecrease(BEST prevState, BEST ThreshViolationState)
{
    return SMStateBase::ThresholdViolWtDecrease( prevState,  ThreshViolationState);
}

SMStateBase * SMState::ClearedSecViolSetReturnState(BEST PrevState, BEST NextState)
{
    return SMStateBase::ClearedSecViolSetReturnState( PrevState,  NextState);
}

void SMState::LogWLDBWeightMismatch(CString csActualWeight, CString csExpectedWeight, long lTolerance)
{
    SMStateBase::LogWLDBWeightMismatch(csActualWeight, csExpectedWeight, lTolerance);
}

CString SMState::BuildNeedMoreDataViewString(CString csView, CString csDescription, CString csLaneStateOneText, 
        CString csLaneStateTwoText, CString csLaneStateThreeText, CString csResourcePath)
{
    return SMStateBase::BuildNeedMoreDataViewString(csView, csDescription, csLaneStateOneText, 
        csLaneStateTwoText, csLaneStateThreeText, csResourcePath);
}

SMStateBase * SMState::OnSecMgrNeedData(CString exceptionType, long lItemID)
{
    return SMStateBase::OnSecMgrNeedData(exceptionType, lItemID);
}

bool SMState::HandleDataNeededSecureCam(CString &csData)
{
    return SMStateBase::HandleDataNeededSecureCam(csData);
}

void SMState::SendSmartAssistExceptionClear(CString csAuthorization, CString csExceptionID)
{
    SMStateBase::SendSmartAssistExceptionClear(csAuthorization, csExceptionID);
}

void SMState::SetAttendantModeStateInSM(bool bAttendantModeState)
{
    SMStateBase::SetAttendantModeStateInSM(bAttendantModeState);
}

CString SMState::FmtWtToString(const long lWgt)
{
    return SMStateBase::FmtWtToString(lWgt);
}

void SMState::UpdateScaleUnits(void)
{
    SMStateBase::UpdateScaleUnits();
}

SMStateBase * SMState::DetermineEASDeactivationState(void)
{
    return SMStateBase::DetermineEASDeactivationState();
}

void SMState::ClearSecurityViolationLight(void)
{
    SMStateBase::ClearSecurityViolationLight();
}

void SMState::CalcObtainAndTraceSMMWtTol(bool bReplaceItemException )
{
    SMStateBase::CalcObtainAndTraceSMMWtTol( bReplaceItemException );
}

void SMState::TellRAPAboutDelaySecurityIntervention()
{
    SMStateBase::TellRAPAboutDelaySecurityIntervention();
}

bool SMState::AnySMDataNeeded()
{
    return SMStateBase::AnySMDataNeeded();
}

SMStateBase * SMState::ProcessSMDataNeeded()
{
    return SMStateBase::ProcessSMDataNeeded();
}

void SMState::SendSecureCamConfigToSecMgr(bool bIsSecureCamEnable)
{
    SMStateBase::SendSecureCamConfigToSecMgr(bIsSecureCamEnable);
}

bool SMState::IsSecurityAnalysisComplete()
{
    return SMStateBase::IsSecurityAnalysisComplete();
}

SMStateBase * SMState::SASecMgrParse( const MessageElement *me)
{
	// start RFQ 9545 FLMS
	int nEvent = me->secmgrInfo.nEvt;
	trace(L6, _T("SMState::SASecMgrParse nEvent = %d"), nEvent);		
	LPTSTR szParms;
	szParms = new TCHAR [_tcslen(me->secmgrInfo.szParms) + sizeof(TCHAR)];
	_tcscpy( szParms, me->secmgrInfo.szParms );
		
	CString csParms;
	csParms = szParms;
	delete [] szParms;
	switch (nEvent) 
	{
			
		case SmOnSecurityControl:
			if (csParms.Find(KEY_OSC_OPERATION_GOTO_SCANANDBAG) > -1)
			{
				// delete [] me->secmgrInfo.szParms;
				if (g_bInRecallMode) // || g_bPressedDone)
				{
					trace(L6, _T("SMState::SASecMgrParse RecallMode KEY_OSC_OPERATION_GOTO_SCANANDBAG is received"));
					setAnchorState(BES_SCANANDBAG);
					//return STATE_NULL;
				}
			}
	}	
	// end RFQ 9545 FLMS

	return SMStateBase::SASecMgrParse(me);
}

long SMState::SecMgrInitialize(void)
{
    return SMStateBase::SecMgrInitialize();
}

long SMState::SecMgrUnInitialize(void)
{
    return SMStateBase::SecMgrUnInitialize();
}

void SMState::SAEnterSecurityCondition()
{
    SMStateBase::SAEnterSecurityCondition();
}

void SMState::SAClearSecurityCondition()
{
    SMStateBase::SAClearSecurityCondition();
}

void SMState::SecMgrPreInitialize(void)
{
    SMStateBase::SecMgrPreInitialize();
}

void SMState::SecMgrPostInitialize(void)
{
    SMStateBase::SecMgrPostInitialize();
}

bool SMState::CouponTally(long TotalSales, long CouponTotals)
{
    return SMStateBase::CouponTally(TotalSales, CouponTotals);
}

bool SMState::IsDuplicateCoupon()
{
    return SMStateBase::IsDuplicateCoupon();
}

SMStateBase * SMState::SAEventOccurred(const long lEvent)
{
    return SMStateBase::SAEventOccurred(lEvent);
}

SMStateBase * SMState::SASMMBagScaleEvent(void)
{
    return SMStateBase::SASMMBagScaleEvent();
}

void SMState::SASMMFormatProductInfo()
{
    SMStateBase::SASMMFormatProductInfo();
}

void SMState::SASMMFormatToleranceInfo()
{
    SMStateBase::SASMMFormatToleranceInfo();
}

void SMState::SASMMFormatWLDBInfo(void)
{
    SMStateBase::SASMMFormatWLDBInfo();
}

CString SMState::AdjustBarCode(void)
{
    return SMStateBase::AdjustBarCode();
}

