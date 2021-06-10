//////////////////////////////////////////////////////////////////////
//
// BaseMHTender.cpp: implementation of the CBaseMHTender class.
//
// CHANGE HISTORY:
//
// POS312463 Work Request:77937 Name:Alex Bronola Date:Feb. 07, 2017
// 
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHTender.h"
#include "traceSystem.h"
#include "TBUtilities.h"
#include "BaseCurrentTender.h"
#include "BaseObjectManager.h"
#include "CustomerTrxBroker.h"
#include "CustomerCurrentTransaction.h"
#include "CustomerTBProperties.h"

#define T_ID    _T("CBaseMHTender")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseMHTender::CBaseMHTender(): 
m_bEnterTenderStateValue(false)
{
}

CBaseMHTender::~CBaseMHTender()
{
}


TBRC CBaseMHTender::EnterTender( TBTENDERTYPE nTenderType, LONG lAmount, LPCTSTR szCardData,
                              int nPINLength, LPCTSTR szEncryptedPIN, BOOL fOverride, BOOL fVoid) 
{
    TBRC rc = TB_SUCCESS;

    tbtrace( TM_INFO, _T("EnterTender::(nTenderType= %d, lAmount= %d, nPINLength= %d, szEncryptedPIN= %s, fOverride= %d, fVoid= %d)"), 
		  nTenderType, lAmount, nPINLength, szEncryptedPIN, fOverride, fVoid);

    CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();
    CBaseCurrentTender* pCurrTender = CCustomerCurrentTender::instance();
    
    pCurrTender->TenderType(nTenderType);
   
    pCurrTender->TenderAmount(lAmount);
    Trx.TBSetProperty(TBPROP_ENTERTENDERAMOUNT, pCurrTender->TenderAmount());

    pCurrTender->SetCardData(szCardData);
    pCurrTender->SetPIN(szEncryptedPIN);
    pCurrTender->SetOverride(fOverride);
    pCurrTender->TenderVoided(fVoid!=FALSE);

	CString csTenderName;

	// get the tender name from TBBaseOpt - properties loaded from there
	// in format Tender[nTenderType]=TENDER_NAME
	//
    
    //if we are in assistmode then change state to an assistmode
    //substate
    csTenderName.Format( _T("Tender%d"), nTenderType); 
    if(Trx.InAssistMode())
    {
        CString csTender = Trx.TBGetProperty( csTenderName);
        csTender.TrimLeft();
        csTender.TrimRight();

        CString csTemp = _T("ASSISTMODE_") + csTender;
        Trx.SetCurrentInProgressState(csTemp);
        
    }else
    {
	    
        Trx.SetCurrentInProgressState( Trx.TBGetProperty( csTenderName));
    }

    Trx.TriggerMsgMap();

	return rc;
}


/*************************************************************************
* EnterTenderState - Informs TB when FastLane enters or leaves the tender 
*       state. Appropriate TB action can be solution-specific. This is not 
*       a request from FL, but a statement of fact.
*
* Parameters: 
*  bool bEnter - Flag indicating whether entering (TRUE) or exiting (FALSE)
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHTender::EnterTenderState( const bool bEnter)
{
    tbtraceIO(_T("EnterTenderState()"));
	bool bNeedTriggerMap = true;
	CCustomerTrxBroker *pTrxBroker = CCustomerTrxBroker::instance();
	CCustomerCurrentTransaction *pCurrTrx = CCustomerCurrentTransaction::instance();
	m_bEnterTenderStateValue = bEnter;

    if (bEnter)
	{
		if (FALSE == pCurrTrx->DirtyTrx())
		{
			tbtrace(TM_INFO, _T("Transaction is clean. Not entering tender state."));
			bNeedTriggerMap = false;
		}
		else
		{
			//POS312463 -aab
			CString csCurrentTBState = pTrxBroker->GetCurrentInProgressStateName();

			tbtrace(TM_INFO, _T("CurrentProgressState = %s."), csCurrentTBState);
			if(csCurrentTBState != _T("TB_ENTERTENDERSTATE_IN"))
			{
			   tbtrace(TM_INFO, _T("Setting progress state to, TB_ENTERTENDERSTATE_IN."));
			   pTrxBroker->SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
			}
			else
               bNeedTriggerMap = false; 
		}
	}
    else
        pTrxBroker->SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_OUT"));

	if (bNeedTriggerMap)
		pTrxBroker->TriggerMsgMap();

    return TB_SUCCESS;

}


TBRC CBaseMHTender::EPSEndOfDay(LPVOID lp)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
    TBRC rc = TB_SUCCESS;
    tbtrace( TM_ENTRY,_T("EPSEndOfDay()"));
    return rc;
}


TBRC CBaseMHTender::EPSStartOfDay(LPVOID lp)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
    TBRC rc = TB_SUCCESS;
    tbtrace( TM_ENTRY, _T("+-EPSStartOfDay"));
    return rc;
}



TBRC CBaseMHTender::GetTenderDetails(PSTenderDetails psTenderDetails)
{
    TBRC rc = TB_SUCCESS;

    CBaseCurrentTender* pCurrTender = CCustomerCurrentTender::instance();

    psTenderDetails->lDescriptionLength =
		 CopyString(psTenderDetails->szDescription, pCurrTender->TenderDescription());
    psTenderDetails->lErrDescriptionLength =
		 CopyString(psTenderDetails->szErrDescription, pCurrTender->ErrorDescription());

      //copystring returns a length of 1 when the string is _T("")
    //adjusting the length so that the tender decline reason
    //does not appear when there is no reason
    CString csErrDesc = pCurrTender->ErrorDescription();
    if(csErrDesc == _T(""))
    {
        psTenderDetails->lErrDescriptionLength =0;
    }

    psTenderDetails->lTender               = pCurrTender->TenderAmountLong();
    psTenderDetails->lBalance              = pCurrTender->BalanceLong();
    psTenderDetails->lChange               = pCurrTender->ChangeDueLong();
    psTenderDetails->nTenderType           = (TBTENDERTYPE)pCurrTender->TenderType();

    tbtrace(TM_INFO, _T("GetTenderDetails(): TenderAmt:%d, Balance:%d, Change:%d, TenderType:%d"),
        psTenderDetails->lTender, psTenderDetails->lBalance, psTenderDetails->lChange, psTenderDetails->nTenderType);

    return rc;
}


// This is required functionality for all MH classes
TBSTATE CBaseMHTender::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_IGNORE;
	 CTenderDetailsMsg *pMsg= dynamic_cast<CTenderDetailsMsg *> (pMsgObj);
	 ASSERT(pMsg);
	 if (pMsg) {
	    CBaseCurrentTender* pCurr = CCustomerCurrentTender::instance();
		 pCurr->Refresh(pMsg);
	 }

    tbtrace( TM_ENTRY, _T("+-ProcessMessage"));
	// there is no need to assert here. This implementation is good-enough.
	 //    ASSERT(0);  // Assert until real functionality is added at this level
    return rc;
}


TBLOYALTYCARDTYPE CBaseMHTender::VerifyLoyaltyCard(LPCTSTR szCardData,int nDataLen)
{
    tbtrace( TM_ENTRY, _T("+- VerifyLoyaltyCard- Base Function returning LC_INVALID"));
    return LC_INVALID;
}

/*************************************************************************
* HandleTenderEntry - 
*
* Parameters: 
*  LPARAM - lparam passed from the MessageMap
*
* Returns: TBSTATE - acknowledge the request
*************************************************************************/
TBSTATE CBaseMHTender::HandleTenderEntry(LPARAM lparam)
{
	tbtraceIO(_T("HandleTenderEntry"));
	TBSTATE rc = TB_IGNORE;

	// handle tender entry 

	return rc;
}
