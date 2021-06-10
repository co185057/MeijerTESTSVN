// CustomerMHCashDrawer.cpp: implementation of the CCustomerMHCashDrawer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerMHCashDrawer.h"
#include "TraceSystem.h"
#include "CustomerTBConnectPOS.h"
#include "CustomerTrxBroker.h"
#include "CustomerMsgMap.h"
#include "CustomerCurrentCashDrawer.h"
#include "CustomerMHOptions.h"
#include "BaseTBSharedDefs.h"
#include "TransBroker.h"
#include "CustomerTBConnectFL.h"
#include "CurrentPosState.h"

#define T_ID  _T("CCustomerMHCashDrawer")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerMHCashDrawer::CCustomerMHCashDrawer()
{
}

CCustomerMHCashDrawer::~CCustomerMHCashDrawer()
{

}

TBSTATE CCustomerMHCashDrawer::ProcessMessage(CTBMsgObj *pMsg)
{
    CCustomerTrxBroker *pTrx = CCustomerTrxBroker::instance();
    CCustomerCurrentCashDrawer *pCurCashDrawer = CCustomerCurrentCashDrawer::instance();
	CString  csState = pTrx->GetCurrentInProgressStateName();

    CCurrentPosState &PosState = *CCurrentPosState::instance();
    _bstr_t csCurPosStateBstr= PosState.GetValue(_T("PosState"));
    CString csCurPosState = (LPCTSTR)csCurPosStateBstr;
    csCurPosState.TrimLeft();
    csCurPosState.TrimRight();

	long lVal = _ttol(pTrx->TBGetProperty(_T("SignPaperForm"),true));
    if(lVal != 0)
	{
		// Don't close POS cash drawer until AssistMode for Credit Sig Slip 
		if ( -1 == csState.Find(_T("ASSISTMODE_")) )
		{
			this->SetCloseImmediate( false );
			pTrx->SetCurrentInProgressState(_T("ASSISTMODE_WAIT_SIGSLIP"));
	        CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEEDASSISTMODE, NULL, NULL );
		}

	}
    if ( -1 != csState.Find(_T("NO_SALE_STATE")) )
    {
		this->SetCloseImmediate( false );
	}

    TBSTATE tbRet = CBaseMHCashDrawer::ProcessMessage( pMsg );

	if ( tbRet == TB_NEWCASHDRAWERSTATE )
	{
		// Pop Drawer for Meijer at start of first receipt print!
		tbRet = TB_IGNORE;
	}

    if ( CCustomerCurrentCashDrawer::instance()->Status() ==
           CCashDrawerMsg::CashDrawerStates::CD_CLOSED )
    {
        if ( CCustomerMHOptions::instance()->BoolOptionValue( _T("TB_FT_SUPPORT_RAPTENDER")) )
        {
            if ( -1 != csState.Find(_T("ASSISTMODE_TRXEND")) )
            {
                TBSTATE tbTrxEnd = TB_TRXEND;
                CCustomerMsgMap::instance()->msgAssistModeTrxEnd( tbTrxEnd, NULL );
                if ( tbRet != TB_IGNORE )
                {
					if ( tbTrxEnd != TB_IGNORE )
					{ 
	                    CCustomerTBScotMsg::instance()->SendReturnStateMsg( tbRet, NULL, NULL );
					}
                }
				else
				{
					tbRet = tbTrxEnd;
				}
            }
        }
		
		if(csState == _T("NO_SALE_STATE_IP") && (csCurPosState == _T("NO_SALE")))
		{
			pTrx->SetCurrentInProgressState(_T("NO_SALE_STATE_END"));
			pTrx->TriggerMsgMap();
			pCurCashDrawer->setExpectedCashDrawerState( CASHDRAWER_CLOSED );
			tbRet = TB_NEWCASHDRAWERSTATE;
		}
    }

    if (pCurCashDrawer->Status() == CCashDrawerMsg::CashDrawerStates::CD_OPEN )
    {
        //Are we on the NO_Sale screen and in remote AM, then send the cash drawer pop
        //SOTF7368
        if((csCurPosState == _T("NO_SALE") && CCustomerTrxBroker::instance()->InRemoteAssistMode()) ||
            ((csCurPosState.Find(_T("MISCMENU")) != -1) && CCustomerTrxBroker::instance()->InRemoteAssistMode())
			|| csState == _T("NO_SALE_STATE_IP"))
        {
            tbtrace(TM_INFO, _T("On the %s screen, sending POS request fo cashdrawer pop."), csCurPosState);
            pCurCashDrawer->setExpectedCashDrawerState( CASHDRAWER_OPEN );
            // FastLane only needs to know about Opens!
            tbRet = TB_NEWCASHDRAWERSTATE;
        }
    }
	return tbRet;
}

