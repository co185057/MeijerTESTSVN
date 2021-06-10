//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	 SMScanCardBase.CPP
//
// TITLE:	Class implementation for Select Card Type State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>				 // MS MFC always needed first
#include <afxtempl.h>			 // MS MFC template header

#include "Common.h"				 // Common includes

#include "SMState.h"			 // Base state
#include "SMScanCard.h"
#include "SMTakeCard.h"			 // Take card state
#include "SMSelectPayment.h"        // Select payment state
#include "SMSelectDebitType.h"	 // Select debit account type checking or savings
#include "SMWaitForApproval.h"	 // Wait for approval state
#include "SMElectronicBenefitsTransfer.h"	
#include "SMRequestSig.h"
#include "SMSignCharge.h"
#include "SMSelectCardTypeBase.h"
#include "SMEnterPin.h"
#include "SMConfirmDebitAmount.h"
#include "SMConfirmEBTAmount.h"

#define COMP_ID ID_SM			 // Base state component
#define T_ID	_T("ScanCard")

IMPLEMENT_DYNCREATE(SMScanCardBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ScanCard)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMScanCardBase::SMScanCardBase(const TBTENDERTYPE nTyp,const long lAmt)
: nTndrType(nTyp),
  lTenderAmt(lAmt)
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
}

//////////////////////////////////////////
SMScanCardBase::SMScanCardBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
}

////////////////////////////////////////
SMStateBase  *SMScanCardBase::Deliver(const TBTENDERTYPE nTyp,const long lAmt)
{
  nTndrType = nTyp;
  lTenderAmt = lAmt;

  return this;
}
//////////////////////////////////////////
bool SMScanCardBase::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both dwords
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMScanCardBase::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver((TBTENDERTYPE)DLParamDWORD(dlParameterList),    // nTyp
                DLParamDWORD(dlParameterList));                 // lAmt
        return true;
    }
    else
    {
        trace(L2, _T("+SMScanCardBase::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}
//////////////////////////////////////////
SMStateBase *SMScanCardBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ScanCard") );	// RFC 330050
  bEASReEnableScanner = true;
  SAWLDBScannerEnable();			// enable the scanner

  ps.SetCMFrame(_T("ScanCard"));

  DMSayPhrase(SCANYOURCARD);
  ps.Message12(MSG_SCANGIFTCARD, true);
    
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

  ps.ShowCMFrame();
  return STATE_NULL;
}


/////////////////////////////////////////
SMStateBase  *SMScanCardBase::PSButtonGoBack(void)// cancel
{
  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+
                  ps.GetPSText(RA_ENDCARDTENDER_BALANCEDUE, SCOT_LANGUAGE_PRIMARY));
  return setAndCreateAnchorState(BES_FINISH);
}

//////////////////////////////////
SMStateBase *SMScanCardBase::SAEventOccurred(const long)
{
  /******cm150000: Modified In Merge 2******/
  //Security Log button is gone, this should be gone also  if (isSecurityApprovalRequired())
  //{
  //	CREATE_AND_DISPENSE(WaitForApproval)(false,false);
  //}
  return STATE_NULL;
}

/////////////////////////////////////////
void SMScanCardBase::UnInitialize(void)
{
    SAWLDBScannerDisable();			   // disable the scanner
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanCard") );	// RFC 330050
}

/////////////////////////////////////////
SMStateBase  *SMScanCardBase::PSCancelKey(void)/// override where needed
{
  DMSayPhrase(KEYPADVALID);
  return setAndCreateAnchorState(BES_FINISH);
}

/////////////////////////////////////
// DMCardReader - card inserted
/////////////////////////////////////
SMStateBase  *SMScanCardBase::DMScanner(void)
{
	ra.OnCanSuspendNow(false) ;  // un-reversable tender mode. disable suspend
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ps.GetPSText(RA_BEGINCARDTENDER, SCOT_LANGUAGE_PRIMARY));

	ResetAllTenderValues(); //new Finalization
 	
	csItemSent = csDMLastBarCode;
	csDMEncryptedPin.Empty();
	return TBEnterTender(nTndrType, lBalanceDue);	
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMScanCardBase::OnWtIncreaseNotAllowed()	 
{
	return FinalizationWtIncrease(BES_FINISH);	
}
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMScanCardBase::OnWtDecrease()
{
	if(!g_bIsForgiveUnexpectedDecreaseDuringTendering)
		return FinalizationWtDecreaseNotAllowed(BES_FINISH);
	else
		return STATE_NULL;
}
