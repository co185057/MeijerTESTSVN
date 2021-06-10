// SolutionMHCashDrawer.cpp: implementation of the CSolutionMHCashDrawer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionMHCashDrawer.h"
#include "TraceSystem.h"
#include "CustomerTBConnectPOS.h"
#include "CustomerTrxBroker.h"
#include "CustomerMsgMap.h"
#include "CustomerCurrentCashDrawer.h"
#include "CustomerMHOptions.h"
#include "BaseTBSharedDefs.h"
#include "TransBroker.h"
#include "CustomerTBConnectFL.h"

#define T_ID  _T("CSolutionMHCashDrawer")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSolutionMHCashDrawer::CSolutionMHCashDrawer()
{
}

CSolutionMHCashDrawer::~CSolutionMHCashDrawer()
{

}

/*************************************************************************
* ClosePOSCashDrawer - ACS/OPOS specific version of ClosePOSCashDrawer
*
* Parameters:   cdStation - The cash drawer station to be closed.
*
* Returns: TRUE if input messages sent, FALSE otherwise
*************************************************************************/
void CSolutionMHCashDrawer::ClosePOSCashDrawer( CCashDrawerMsg::CashDrawerStations cdStation )
{
    tbtraceIO( _T( "CSolutionMHCashDrawer::ClosePOSCashDrawer()"));
    CCashDrawerMsg myMsg;
    myMsg.Status( CCashDrawerMsg::CashDrawerStates::CD_CLOSED );
    myMsg.Station( cdStation );
    CCustomerTBConnectPOS* pConnect = (CCustomerTBConnectPOS*) CCustomerTBConnectPOS::instance();
    if ( pConnect )
    {
        pConnect->SendMsgToPOS( myMsg );
    }
}


TBSTATE CSolutionMHCashDrawer::ProcessMessage(CTBMsgObj *pMsg)
{
    CCustomerTrxBroker *pTrx = CCustomerTrxBroker::instance();
	long lVal = _ttol(pTrx->TBGetProperty(_T("SignPaperForm"),true));
    if(lVal != 0)
	{
		// Don't close POS cash drawer until AssistMode for Credit Sig Slip 
		CString csState = pTrx->GetCurrentInProgressStateName();
		if ( -1 == csState.Find(_T("ASSISTMODE_")) )
		{
			this->SetCloseImmediate( false );
			pTrx->SetCurrentInProgressState(_T("ASSISTMODE_WAIT_SIGSLIP"));
	        CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEEDASSISTMODE, NULL, NULL );
		}

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
            CString  csCurTBState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
            if ( -1 != csCurTBState.Find(_T("ASSISTMODE_TRXEND")) )
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
    }

    return tbRet;
}

