// BaseMHTransaction.cpp: implementation of the CBaseMHTransaction class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHTransaction.h"
#include "traceSystem.h"
#include "BaseCurrentTransaction.h"
#include "BaseObjectManager.h"
#include "customerMsgMap.h"
#include "customerTrxBroker.h"

#define T_ID    _T("CBaseMHTransaction")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHTransaction::CBaseMHTransaction()
{
}

CBaseMHTransaction::~CBaseMHTransaction()
{
}


TBRC CBaseMHTransaction::CancelTransaction(const BOOL fOverride)
{
   tbtrace(TM_INFO, _T("CancelTransaction()"));
   CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();
   Trx.SetCurrentInProgressState(_T("TB_CANCELTRANSACTION_FLAPI"));
   Trx.TriggerMsgMap();

    return TB_SUCCESS;
}


TBRC CBaseMHTransaction::GetAssistModeStatus(long *plAssistModeStatus)
{
    tbtraceIO(_T("GetAssistModeStatus()"));
    CBaseCurrentTransaction* pTrx = CCustomerCurrentTransaction::instance();
    *plAssistModeStatus = pTrx->GetAssistModeStatus();
    return TB_SUCCESS;
}


long CBaseMHTransaction::GetQuantityLimit(void)
{
    LPCTSTR  lpThisName = _T("GetQuantityLimit()");
    tbtrace(TM_ENTRY, _T("+%s"), lpThisName);
    CBaseCurrentTransaction* pTrx = CCustomerCurrentTransaction::instance();
    long rc = pTrx->GetQuantityLimit();
    tbtrace(TM_ENTRY, _T("-%s returns %d"), lpThisName, rc);
    return rc;
}


long CBaseMHTransaction::GetTransactionLimit(void)
{
    CBaseCurrentTransaction* pTrx = CCustomerCurrentTransaction::instance();
    long rc = pTrx->GetTransactionLimit();
    tbtrace(TM_INFO, _T("-GetTransactionLimit returns %d"), rc);
    return rc;
}

long CBaseMHTransaction::GetCurrentTransactionNumber(void)
{
    CBaseCurrentTransaction* pTrx = CCustomerCurrentTransaction::instance();
    long rc = pTrx->GetCurrentTransactionNumber();
    tbtrace(TM_INFO, _T("-GetCurrentTransactionNumber returns %d"), rc);
    return rc;
}

TBRC CBaseMHTransaction::LaneClosed(const bool bEnter)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
    tbtrace(TM_INFO, _T("LaneClosed()"));
    return TB_SUCCESS;
}


/*************************************************************************
* Finish - FL requests TB end the current sales session. TB_TRXEND state 
*            returned by TB tells FastLane to call this function.
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHTransaction::Finish(void)
{
    tbtraceIO(_T("Finish()"));
    CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();
    Trx.SetCurrentInProgressState(_T("TB_FINISH_FLAPI"));
    Trx.TriggerMsgMap();

    return TB_SUCCESS;
}


// This is required functionality for all MH classes
TBSTATE CBaseMHTransaction::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtrace(TM_INFO, _T("ProcessMessage()"));
    return TB_IGNORE;
}


TBRC CBaseMHTransaction::ResumeTransaction(LPCTSTR szSuspendID)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
    tbtrace(TM_INFO, _T("ResumeTransaction()"));
    return TB_SUCCESS;
}


TBRC CBaseMHTransaction::SetTrainingModeState(bool bTMState)
{
   // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
   /*<%ASIS
   if (bTMState)
   SendReturnStatusMsg(TB_ENTEREDTRAINING);
   else
   SendReturnStatusMsg(TB_LEFTTRAINING);
   return TB_SUCCESS;
   %>*/
   
   tbtrace(TM_INFO, _T("SetTrainingModeState()"));
   CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();
   if (bTMState)
      Trx.SetCurrentInProgressState(_T("TB_TRAININGMODE_IN_FLAPI"));
   else
      Trx.SetCurrentInProgressState(_T("TB_TRAININGMODE_OUT_FLAPI"));
   
   Trx.TriggerMsgMap();
   return TB_SUCCESS;
}


/*************************************************************************
* Start - Called to start a trx. Base TB will set the language code that
*       is passed in and return TB_SUCCESS. Solution-specific TB's should
*       call the base implementation and then do any customer specific functions
*
* Parameters: 
*  LPCTSTR szOverrideCode - The 2 character string denoting the ISO639
*       language code for language that the customer has selected to use in 
*       the transaction. NULL if dual language is not supported.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHTransaction::Start(LPCTSTR szOverrideCode)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    SendReturnStatusMsg( TB_READY );
    %>*/

    tbtraceIO(_T("Start()"));
    CCustomerCurrentTransaction::instance()->SetLanguageUsed(szOverrideCode);
#ifdef _DEBUG
    // in debug, reload the message map before each transaction
    CCustomerMsgMap::instance()->Initialize();
#endif
    return TB_SUCCESS_IMMEDIATE;
}

TBRC CBaseMHTransaction::SuspendTransaction(const BOOL fOverride)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%= SetCancelInfo() %>*/
    /*<%ASIS
    SendReturnStatusMsg( TB_TRXSUSPENDED );
    SendReturnStatusMsg( TB_TRXEND );
    return TB_SUCCESS;
    %>*/
    tbtrace(TM_INFO, _T("SuspendTransaction()"));
    CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();
    Trx.SetCurrentInProgressState(_T("TB_SUSPENDTRANSACTION_FLAPI"));
    Trx.TriggerMsgMap();

    return TB_SUCCESS;
}

TBRC CBaseMHTransaction::SuspendTransactionEx(const BOOL fOverride, LPCTSTR szSuspendReasons)
{
    tbtrace(TM_INFO, _T("SuspendTransactionEx()"));
    return TB_SUCCESS;
}
