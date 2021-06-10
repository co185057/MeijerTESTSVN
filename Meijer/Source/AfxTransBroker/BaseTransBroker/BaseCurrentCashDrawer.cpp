// BaseCurrentCashDrawer.cpp: implementation of the CBaseCurrentCashDrawer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentCashDrawer.h"
#include "TraceSystem.h"
#include "ReturnStateMsg.h"
#include "CustomerTBScotMsg.h"
#include "CustomerMHCashDrawer.h"

#define T_ID  _T("CBaseCurrentCashDrawer")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentCashDrawer::CBaseCurrentCashDrawer()
{
    m_lExpectedCashDrawerState = CASHDRAWER_CLOSED;
    Station( CCashDrawerMsg::CashDrawerStations::DRAWER_A );
    Status( CCashDrawerMsg::CashDrawerStates::CD_CLOSED );
}

CBaseCurrentCashDrawer::~CBaseCurrentCashDrawer()
{
    
}


/*************************************************************************
* GetExpectedCashDrawerState - TBAPI
*
* Parameters: 
*  plExpectedState - long * to populate expected state
*
* Returns: TBRC, status of API call
*************************************************************************/
TBRC CBaseCurrentCashDrawer::GetExpectedCashDrawerState(long *plExpectedState)
{
    tbtraceIO(_T("GetExpectedCashDrawerState()"));

    TBRC rc = TB_SUCCESS;
    
    if (!plExpectedState)
    { // not valid pointer 
        tbtrace( TM_WARNING, _T("CBaseCurrentCashDrawerState::GetExpectedCashDrawerState() invalid plExpectedState\n"));
        TBASSERT(false);
        rc = TB_E_INVALIDPARAM;
    }
    else
    {
        *plExpectedState = m_lExpectedCashDrawerState;
        tbtrace( TM_INFO, _T("CBaseCurrentCashDrawer::GetExpectedCashDrawerState state( %d)"), *plExpectedState);
    }
    return rc;
}


/*************************************************************************
* SetCashDrawerState - TBAPI to set the cash drawer state
*
* Parameters: 
*  lState - long value of new cash drawer state
*
* Returns: TBRC, status of API call
*************************************************************************/
TBRC CBaseCurrentCashDrawer::SetCashDrawerState(long lState)
{
    LPCTSTR  lpThisName = _T("CBaseCurrentCashDrawer::SetCashDrawerState()");
    tbtraceIO( _T("CBaseCurrentCashDrawer::SetCashDrawerState"));
    tbtrace( TM_INFO, _T("Expected %d, FastLane CashDrawer state = %d"), 
        m_lExpectedCashDrawerState, lState);


    if ( m_lExpectedCashDrawerState )
    {
        // expected to be open
        if ( lState == CASHDRAWER_CLOSED )
        {
            // FastLane closed the cash drawer and it should be open, ask for open again
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEWCASHDRAWERSTATE, NULL, NULL ); 
        }
        else
        {
            // FastLane opened the drawer,  next expected state is closed.
            m_lExpectedCashDrawerState = CASHDRAWER_CLOSED;
        }
    }
    else
    {
        // expected to be closed
        if ( lState == CASHDRAWER_CLOSED )
        {
            if ( Status() == CCashDrawerMsg::CashDrawerStates::CD_OPEN )
            {
                CCustomerMHCashDrawer::instance()->ClosePOSCashDrawer( Station() );
            }
        }
        else
        {
            tbtrace( TM_ERROR, _T("FastLane Cash Drawer opened when it should be closed!") );
        }
    }
    return TB_SUCCESS;
}

/*************************************************************************
* setExpectedCashDrawerState - set the expected cash drawer state
*
* Parameters: 
*  lState - long value of new expected cash drawer state
*
* Returns: TBRC, status of API call
*************************************************************************/
void CBaseCurrentCashDrawer::setExpectedCashDrawerState( long lState)
{
    tbtraceIO(_T("setExpectedCashDrawerState()"));
    tbtrace( TM_ENTRY, _T("Current State [%d] - Setting to [%d]"), m_lExpectedCashDrawerState, lState);
    m_lExpectedCashDrawerState = lState;
}

/*************************************************************************
* Refresh - casts a CashDrawerMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CCashDrawerMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentCashDrawer::Refresh(CCashDrawerMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
    // lock if other threads can get here
    delete [] m_pExtensionBuf;  // delete old pointer
    *((CCashDrawerMsg *) this) = *pMsg;  // USE DEFAULT COPY 
    m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
    // unlock
}