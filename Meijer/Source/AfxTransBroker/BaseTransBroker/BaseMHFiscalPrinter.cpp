// BaseMHFiscalPrinter.cpp: implementation of the CBaseMHFiscalPrinter class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHFiscalPrinter.h"
#include "CustomerCurrentFiscalPrinter.h"
#include "CustomerTBConnectPOS.h"

#define T_ID  _T("CBaseMHFiscalPrinter")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHFiscalPrinter::CBaseMHFiscalPrinter()
{
}

CBaseMHFiscalPrinter::~CBaseMHFiscalPrinter()
{
}

/*************************************************************************
* TBProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. 
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHFiscalPrinter::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("ProcessMessage()"));
    TBSTATE rc = TB_IGNORE;
    CFiscalPrinterMsg *pMsg= dynamic_cast<CFiscalPrinterMsg *> (pMsgObj);
    ASSERT(pMsg);
    if (pMsg) {
        CBaseCurrentFiscalPrinter* pCurr = CCustomerCurrentFiscalPrinter::instance();
        pCurr->Refresh(pMsg);
    }

    switch (pMsg->GetFunctionCode() )
    {
    case cBeginFiscalReceipt:
        rc = HandleBeginFiscalReceipt( pMsg );
        break;
    case cPrintRecItem:
        rc = HandlePrintRecItem( pMsg );
        break;
    case cPrintRecTotal:
        rc = HandlePrintRecTotal( pMsg );
        break;
    case cEndFiscalReceipt:
        rc = HandleEndFiscalReceipt( pMsg );
        break;
    case cDirectIO:
        rc = HandleDirectIO( pMsg );
        break;
    case cBeginNonFiscal:
        rc = HandleBeginNonFiscal( pMsg );
        break;
    case cEndNonFiscal:
        rc = HandleEndNonFiscal( pMsg );
        break;
    case cPrintNormal:
        rc = HandlePrintNormal( pMsg );
        break;
    case cPrintRecItemAdjustment:
        rc = HandlePrintRecItemAdjustment( pMsg );
        break;
    case cPrintRecMessage:
        rc = HandlePrintRecMessage( pMsg );
        break;
    case cPrintRecSubtotal:
        rc = HandlePrintRecSubtotal( pMsg );
        break;
    case cPrintRecSubtotalAdjustment:
        rc = HandlePrintRecSubtotalAdjustment( pMsg );
        break;
    case cPrintRecVoid:
        rc = HandlePrintRecVoid( pMsg );
        break;
    case cPrintRecVoidItem:
        rc = HandlePrintRecVoidItem( pMsg );
        break;
    case cPrintZReport:
        rc = HandlePrintZReport( pMsg );
        break;
    case cBeginTraining:
        rc = HandleBeginTraining( pMsg );
        break;
    case cEndTraining:
        rc = HandleEndTraining( pMsg );
        break;
    case cGetData:
        rc = HandleGetData( pMsg );
        break;
    case cGetDate:
        rc = HandleGetDate( pMsg );
        break;
    case cPrintRecNotPaid:
        rc = HandlePrintRecNotPaid( pMsg );
        break;
    case cPrintRecRefund:
        rc = HandlePrintRecRefund( pMsg );
        break;
    case cResetPrinter:
        rc = HandleResetPrinter( pMsg );
        break;
    case cSetDate:
        rc = HandleSetDate( pMsg );
        break;
    case cSetHeaderLine:
        rc = HandleSetHeaderLine( pMsg );
        break;

    case cInvalidFunction:
    case cDirectIO_Return:
    case cGetData_Return:
    case cGetDate_Return:
    case cOutputComplete_Return:
        tbtrace( TM_INFO, _T("Invalid Function Code [%d]"), pMsg->GetFunctionCode() );
        rc = TB_IGNORE;
        break;

    default:
        tbtrace( TM_INFO, _T("Unknown Function Code [%d]"), pMsg->GetFunctionCode() );
        rc = TB_IGNORE;
        break;
    }

    return rc;
}

/*************************************************************************
* The function code handlers - These functions need to be overridden in the
*       solution-specific level for appropriate handling.
*
* Parameters: 
*  CFiscalPrinterMsg* pMsg- the Fiscal printer message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHFiscalPrinter::HandleDirectIO( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleDirectIO"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleBeginFiscalReceipt( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleBeginFiscalReceipt"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleEndFiscalReceipt( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleEndFiscalReceipt"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintRecItem( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecItem"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintRecTotal( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecTotal"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleBeginNonFiscal( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleBeginNonFiscal"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleEndNonFiscal( CFiscalPrinterMsg* pMsg)
{
    tbtrace( TM_ENTRY, _T("+-HandleEndNonFiscal"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintNormal( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintNormal"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintRecItemAdjustment( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecItemAdjustment"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintRecMessage( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecMessage"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintRecSubtotal( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecSubtotal"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintRecSubtotalAdjustment( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecSubtotalAdjustment"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintRecVoid( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecVoid"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintRecVoidItem( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecVoidItem"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintZReport( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintZReport"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleBeginTraining( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleBeginTraining"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleEndTraining( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleEndTraining"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleGetData( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleGetData"));
    return TB_IGNORE;
}


TBSTATE CBaseMHFiscalPrinter::HandleGetDate( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleGetDate"));
    return TB_IGNORE;
}


TBSTATE CBaseMHFiscalPrinter::HandlePrintRecNotPaid( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecNotPaid"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandlePrintRecRefund( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandlePrintRecRefund"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleResetPrinter( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleResetPrinter"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleSetDate( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleSetDate"));
    return TB_IGNORE;
}

TBSTATE CBaseMHFiscalPrinter::HandleSetHeaderLine( CFiscalPrinterMsg* pMsg )
{
    tbtrace( TM_ENTRY, _T("+-HandleSetHeaderLine"));
    return TB_IGNORE;
}

/*************************************************************************
* SendOutputCompleteResp - send output complete msg to POS
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseMHFiscalPrinter::SendOutputCompleteResp()
{
    tbtraceIO(_T("SendOutputCompleteResp()"));
    CCustomerTBConnectPOS * pTBCon = CCustomerTBConnectPOS::instance();
    CFiscalPrinterMsg oMsg;
    oMsg.Set_OutputComplete_Return();
    pTBCon->SendMsgToPOS( oMsg );
}

/*************************************************************************
* SendDirectIOResp - send direct IO response to POS
*
* Parameters:
*  long Command - 
*  CString &Response - 
*
* Returns: void
*************************************************************************/
void CBaseMHFiscalPrinter::SendDirectIOResp(long Command, const CString &Response)
{
    tbtraceIO(_T("SendDirectIOResp()"));
    CCustomerTBConnectPOS * pTBCon = CCustomerTBConnectPOS::instance();
    CFiscalPrinterMsg oMsg;
    oMsg.SetDirectIO_Return(Command, Response );
    pTBCon->SendMsgToPOS( oMsg );
}

/*************************************************************************
* SendGetDataResp - 
*
* Parameters:
*  long DataItem - 
*  CString ReturnData - 
*
* Returns: void
*************************************************************************/
void CBaseMHFiscalPrinter::SendGetDataResp(long DataItem, const CString ReturnData)
{
    tbtraceIO(_T("SendGetDataResp()"));
    CCustomerTBConnectPOS * pTBCon = CCustomerTBConnectPOS::instance();
    CFiscalPrinterMsg oMsg;
    oMsg.Set_GetData_Return(DataItem, ReturnData);
    pTBCon->SendMsgToPOS( oMsg );
}

/*************************************************************************
* SendGetDateResp - 
*
* Parameters:
*  CString ReturnDate - 
*
* Returns: void
*************************************************************************/
void CBaseMHFiscalPrinter::SendGetDateResp(const CString ReturnDate )
{
    tbtraceIO(_T("SendGetDateResp()"));
    CCustomerTBConnectPOS * pTBCon = CCustomerTBConnectPOS::instance();
    CFiscalPrinterMsg oMsg;
    oMsg.Set_GetDate_Return(ReturnDate);
    pTBCon->SendMsgToPOS( oMsg );
}
