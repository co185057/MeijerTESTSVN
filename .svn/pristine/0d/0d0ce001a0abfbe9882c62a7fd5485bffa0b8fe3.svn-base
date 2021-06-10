// SolutionMHTransaction.cpp: implementation of the CSolutionMHTransaction class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "SolutionMHTransaction.h"
#include "CustomerTrxBroker.h"
#include "CustomerCurrentReceipt.h"
#include "CustomerCurrentSlip.h"
#include "CustomerTBConnectPOS.h"

#define T_ID    _T("CSolutionMHTransaction")

///////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////
CSolutionMHTransaction::CSolutionMHTransaction()
{
}

CSolutionMHTransaction::~CSolutionMHTransaction()
{
}
TBRC CSolutionMHTransaction::Finish(void)
{
    tbtraceIO(_T("Finish()"));
    CCustomerTrxBroker::instance()->SetAnyRewardItems(false);
    return CBaseMHTransaction::Finish();
}

TBRC CSolutionMHTransaction::Start(LPCTSTR szOverrideCode)
{
    CCustomerTrxBroker::instance()->TBSetProperty(_T("TRXENDDone"), _T("0"), true );
    CCustomerTrxBroker::instance()->TBSetProperty(_T("RebateReceiptRequested"), _T("0"), true );
    CCustomerCurrentReceipt::instance()->SetPrintWhenFinished( false );
    CCustomerCurrentSlip::instance()->SetEndorseWhenFinished( false );
	CCustomerTBConnectPOS::ms_bIsItemsNeedTobeBuffered = false;
    return CBaseMHTransaction::Start( szOverrideCode );
}