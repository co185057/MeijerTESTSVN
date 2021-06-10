// SolutionMHGenMsg.cpp: implementation of the CSolutionMHGenMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SolutionMHGenMsg.h"
#include "CustomerMHDisplay.h"
#include "CustomerMHItem.h"
#include "CustomerMHCashDrawer.h"
#include "CustomerMHPrinter.h"
#include "PrinterMsg.h"

#define T_ID _T("CSolutionMHGenMsg")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSolutionMHGenMsg::CSolutionMHGenMsg()
{

}

CSolutionMHGenMsg::~CSolutionMHGenMsg()
{

}

/*************************************************************************
* ProcessMessage - Base TB just returns TB_IGNORE, implement at customer levels
*
* Parameters: 
*  CTBGenMsg *pGenMsg - the message to be parsed
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CSolutionMHGenMsg::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("ProcessMessage"));
    TBSTATE rc = TB_IGNORE;
    
    CTBGenMsg *pGenMsg= dynamic_cast<CTBGenMsg*> (pMsgObj);
    switch (pGenMsg->GetTBMsgId())
    {
    case TBMSG_DISPLAY1:
        rc = CCustomerMHDisplay::instance()->ProcessMessage(pMsgObj);
        break;
    case TBMSG_REWARD:
        rc = CCustomerMHItem::instance()->ProcessRewardMessage(pGenMsg);
        break;
    case TBMSG_CASH_DRAWER:
        {
            CCashDrawerMsg CDMsg;
            pGenMsg->SetMsgVersion(VERSION_GENOBJ_BIN);
            int nLength = 0;
            BYTE* pBuf = pGenMsg->Serialize( nLength );
            CDMsg.Parse( pBuf );
            delete[] pBuf;
            rc = CCustomerMHCashDrawer::instance()->ProcessMessage( &CDMsg );
        }
        break;
    case TBMSG_PRINTER:
        {
            tbtrace( TM_ERROR, _T("Received Printer GenMsg from FastLane!!!") );
        }
        break;
    default:
        rc = CBaseMHGenMsg::ProcessMessage(pMsgObj);
    }
    
    return rc;
}

