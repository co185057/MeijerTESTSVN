// BaseMHCashDrawer.cpp: implementation of the CBaseMHCashDrawer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHCashDrawer.h"
#include "TraceSystem.h"
#include "BaseCurrentCashDrawer.h"
#include "BaseObjectManager.h"
#define T_ID  _T("CBaseMHCashDrawer")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHCashDrawer::CBaseMHCashDrawer() :
    m_bCloseImmediate_A(true),
    m_bCloseImmediate_B(true)
{
}

CBaseMHCashDrawer::~CBaseMHCashDrawer()
{
}

/*************************************************************************
* SetCloseImmediate - This is called by TB to specify the handling of 
*                     of CashDrawer messages from the POS
*
* Parameters: 
*  bool bNewCloseImmediate - true or false.
*
* Returns: void
*************************************************************************/
void CBaseMHCashDrawer::SetCloseImmediate(bool bNewCloseImmediate)
{
    if ( m_bCloseImmediate_A != bNewCloseImmediate )
    {
        if ( bNewCloseImmediate )
        {
            CBaseCurrentCashDrawer* pCurr = CCustomerCurrentCashDrawer::instance();
			long lState = 0;
			pCurr->GetExpectedCashDrawerState( &lState );
			if ( CASHDRAWER_OPEN == lState )
			{
				pCurr->setExpectedCashDrawerState( CASHDRAWER_CLOSED );
			}
            if ( pCurr->Status() == CCashDrawerMsg::CashDrawerStates::CD_OPEN  )
            {
                ClosePOSCashDrawer( pCurr->Station() );
            }
        }
        m_bCloseImmediate_A = bNewCloseImmediate;
    }
}


/*************************************************************************
* ClosePOSCashDrawer - EMPTY!!  implementation is TB and Hook specific
*
* Parameters:   cdStation - Cash Drawer station to be closed
*
* Returns: TRUE if input messages sent, FALSE otherwise
*************************************************************************/
void CBaseMHCashDrawer::ClosePOSCashDrawer( CCashDrawerMsg::CashDrawerStations cdStation )
{
    tbtraceIO( _T( "CBaseMHCashDrawer::ClosePOSCashDrawer()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
}

/*************************************************************************
* TBProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. 
*
* Parameters: 
*  CTBMsgObj *pMsg - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHCashDrawer::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("ProcessMessage"));
    TBSTATE rc = TB_IGNORE;
    CCashDrawerMsg *pMsg= dynamic_cast<CCashDrawerMsg *> (pMsgObj);
    CBaseCurrentCashDrawer* pCurr = CCustomerCurrentCashDrawer::instance();

    ASSERT(pMsg);
    if (pMsg) 
    {
        // We only use the primary Cash Drawer in BASE, just close all others.
        if ( pMsg->Station() != CCashDrawerMsg::CashDrawerStations::DRAWER_A )
        {
            if ( pMsg->Status() == CCashDrawerMsg::CashDrawerStates::CD_OPEN )
            {
                // m_bCloseImmediate_B is ALWAYS true in the BASE!!!!!
                if ( m_bCloseImmediate_B )
                {
                    ClosePOSCashDrawer( pMsg->Station() );
                }
            }
            return rc;
        }

        pCurr->Refresh( pMsg );
        tbtrace( TM_INFO, _T("CashDrawer Status is %d"), pCurr->Status() );
        if ( m_bCloseImmediate_A )
        {
            if ( pCurr->Status() == CCashDrawerMsg::CashDrawerStates::CD_OPEN )
            {
                ClosePOSCashDrawer( pMsg->Station() );
            }
			pCurr->setExpectedCashDrawerState( CASHDRAWER_CLOSED );
        }
        else
        {
            // Not closing immediately, Inform FL of new expected CashDrawer State
            if ( pCurr->Status() == CCashDrawerMsg::CashDrawerStates::CD_OPEN )
            {
                pCurr->setExpectedCashDrawerState( CASHDRAWER_OPEN );
                // FastLane only needs to know about Opens!
                rc = TB_NEWCASHDRAWERSTATE;
            }
            else
            {
                pCurr->setExpectedCashDrawerState( CASHDRAWER_CLOSED );
            }
        }
    }
    
    return rc;
}
