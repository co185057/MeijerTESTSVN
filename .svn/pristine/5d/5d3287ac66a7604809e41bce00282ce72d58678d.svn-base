// CustomerMHFiscalPrinter.cpp: implementation of the CCustomerMHFiscalPrinter class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerMHFiscalPrinter.h"
#include "CustomerCurrentFiscalPrinter.h"
#include "CustomerTBConnectPOS.h"

#define T_ID _T("CCustomerMHFiscalPrinter")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerMHFiscalPrinter::CCustomerMHFiscalPrinter()
{

}

CCustomerMHFiscalPrinter::~CCustomerMHFiscalPrinter()
{

}


TBSTATE CCustomerMHFiscalPrinter::HandleDirectIO( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    long nCmd;
    long lInput;
    CString pString;
    if ( pMsg->GetDirectIO( nCmd, lInput, pString ) )
    {
        tbtrace( TM_INFO, _T("DirectIO( %d, %d, %s )"), 
            nCmd, lInput, pString );
        // TODO:  Provide TB specific handling for DirectIO. A DirectIO_Return
        //        message must be sent to the OPOS Emulation SO.
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid DirectIO"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandleBeginFiscalReceipt( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    bool bPrintHeader;
    if ( pMsg->GetBeginFiscalReceipt( bPrintHeader ) )
    {
        tbtrace( TM_INFO, _T("BeginFiscalReceipt( %s )"),
            bPrintHeader ? _T("true") : _T("false") );
        // TODO:  Provide TB specific handling for BeginFiscalReceipt. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid BeginFiscalReceipt"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandleEndFiscalReceipt( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    bool bPrintHeader;
    if ( pMsg->GetEndFiscalReceipt( bPrintHeader ) )
    {
        tbtrace( TM_INFO, _T("EndFiscalReceipt( %s )"),
            bPrintHeader ? _T("true") : _T("false") );
        // TODO:  Provide TB specific handling for EndFiscalReceipt. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid EndFiscalReceipt"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecItem( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CString csDesc, csUnit;
    CURRENCY Price, UnitPrice;
    long lQty, lVat;
    if ( pMsg->GetPrintRecItem( csDesc, Price, lQty, lVat, UnitPrice, csUnit ) )
    {
        tbtrace( TM_INFO, _T("PrintRecItem( %s, %d:%d, %d, %d, %d:%d, %s )"),
            csDesc, Price.Hi, Price.Lo, lQty, lVat, UnitPrice.Hi, UnitPrice.Lo, csUnit );
        // TODO:  Provide TB specific handling for PrintRecItem. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecItem"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecTotal( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CString csDesc;
    CURRENCY Total, Payment;
    if ( pMsg->GetPrintRecTotal( Total, Payment, csDesc ) )
    {
        tbtrace( TM_INFO, _T("PrintRecTotal( %d:%d, %d:%d, %s )"),
            Total.Hi, Total.Lo, Payment.Hi, Payment.Lo, csDesc );
        // TODO:  Provide TB specific handling for PrintRecTotal. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecTotal"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandleBeginNonFiscal( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    if ( pMsg->GetBeginNonFiscal( ) )
    {
        tbtrace( TM_INFO, _T("BeginNonFiscal"));
        // TODO:  Provide TB specific handling for BeginNonFiscal. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid BeginNonFiscal"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandleEndNonFiscal( CFiscalPrinterMsg* pMsg)
{
    TBSTATE tbRC = TB_IGNORE;
    if ( pMsg->GetEndNonFiscal( ) )
    {
        tbtrace( TM_INFO, _T("EndNonFiscal"));
        // TODO:  Provide TB specific handling for EndNonFiscal. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid EndNonFiscal"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintNormal( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    long Station;
    CString Data;
    if ( pMsg->GetPrintNormal( Station, Data ) )
    {
        tbtrace( TM_INFO, _T("PrintNormal( %d, %s )"), Station, Data );
        // TODO:  Provide TB specific handling for PrintNormal. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintNormal"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecItemAdjustment( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    long AdjustmentType;
    CString Description;
    CURRENCY Amount;
    long VatInfo;
    if ( pMsg->GetPrintRecItemAdjustment( AdjustmentType, Description, Amount, VatInfo ) )
    {
        tbtrace( TM_INFO, _T("PrintRecItemAdjustment( %d, %s, %d:%d, %d)"), 
            AdjustmentType, Description, Amount.Hi, Amount.Lo, VatInfo);
        // TODO:  Provide TB specific handling for PrintRecItemAdjustment. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecItemAdjustment"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecMessage( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CString Message;
    if ( pMsg->GetPrintRecMessage(  Message ) )
    {
        tbtrace( TM_INFO, _T("PrintRecMessage( )"), Message );
        // TODO:  Provide TB specific handling for PrintRecMessage. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecMessage"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecSubtotal( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CURRENCY Amount;
    if ( pMsg->GetPrintRecSubtotal(  Amount ) )
    {
        tbtrace( TM_INFO, _T("PrintRecSubtotal( %d:%d )"), 
            Amount.Hi, Amount.Lo );
        // TODO:  Provide TB specific handling for PrintRecSubtotal. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecSubtotal"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecSubtotalAdjustment( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    long AdjustmentType;
    CString Description;
    CURRENCY Amount;
    if ( pMsg->GetPrintRecSubtotalAdjustment(  AdjustmentType, Description, Amount ) )
    {
        tbtrace( TM_INFO, _T("PrintRecSubtotalAdjustment( %d, %s, %d:%d )"), 
            AdjustmentType, Description, Amount.Hi, Amount.Lo );
        // TODO:  Provide TB specific handling for PrintRecSubtotalAdjustment. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecSubtotalAdjustment"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecVoid( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CString Description;
    if ( pMsg->GetPrintRecVoid( Description ) )
    {
        tbtrace( TM_INFO, _T("PrintRecVoid( %s )"), 
            Description );
        // TODO:  Provide TB specific handling for PrintRecVoid. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecVoid"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecVoidItem( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CString Description;
    CURRENCY Amount;
    long Quantity;
    long AdjustmentType;
    CURRENCY Adjustment;
    long VatInfo;
    if ( pMsg->GetPrintRecVoidItem( Description, Amount, Quantity, AdjustmentType, Adjustment, VatInfo ) )
    {
        tbtrace( TM_INFO, _T("PrintRecVoidItem( %s, %d:%d, %d, %d, %d:%d, %d )"), 
            Description, Amount.Hi, Amount.Lo, Quantity, AdjustmentType, 
            Adjustment.Hi, Adjustment.Lo, VatInfo );
        // TODO:  Provide TB specific handling for PrintRecVoidItem. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecVoidItem"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintZReport( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    if ( pMsg->GetPrintZReport( ) )
    {
        tbtrace( TM_INFO, _T("PrintZReport"));
        // TODO:  Provide TB specific handling for PrintZReport. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintZReport"));
    }
    return tbRC;
}


TBSTATE CCustomerMHFiscalPrinter::HandleBeginTraining( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    if ( pMsg->GetBeginTraining( ) )
    {
        tbtrace( TM_INFO, _T("BeginTraining"));
        // TODO:  Provide TB specific handling for BeginTraining. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid BeginTraining"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandleEndTraining( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    if ( pMsg->GetEndTraining( ) )
    {
        tbtrace( TM_INFO, _T("EndTraining"));
        // TODO:  Provide TB specific handling for EndTraining. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid EndTraining"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandleGetData( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    long nItem;
    CString pString;
    if ( pMsg->Get_GetData( nItem ) )
    {
        tbtrace( TM_INFO, _T("GetData( %d )"), nItem );
        // TODO:  Provide TB specific handling for GetData. A GetData_Return
        //        message must be sent to the OPOS Emulation SO.
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid GetData"));
    }
    return tbRC;
}


TBSTATE CCustomerMHFiscalPrinter::HandleGetDate( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CString pString;
    if ( pMsg->Get_GetDate() )
    {
        tbtrace( TM_INFO, _T("GetDate( )"));
        // TODO:  Provide TB specific handling for GetDate. A GetDate_Return
        //        message must be sent to the OPOS Emulation SO.
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid GetDate"));
    }
    return tbRC;
}


TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecNotPaid( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CString csDesc;
    CURRENCY Amount;

    if ( pMsg->GetPrintRecNotPaid( csDesc, Amount) )
    {
        tbtrace( TM_INFO, _T("PrintRecNotPaid( %s, %d:%d)"), csDesc,
            Amount.Hi, Amount.Lo );
        // TODO:  Provide TB specific handling for PrintRecNotPaid. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecNotPaid"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandlePrintRecRefund( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CString csDesc;
    CURRENCY Amount;
    long Vat;

    if ( pMsg->GetPrintRecRefund( csDesc, Amount, Vat) )
    {
        tbtrace( TM_INFO, _T("PrintRecRefund( %s, %d:%d, %d)"), csDesc,
            Amount.Hi, Amount.Lo, Vat );
        // TODO:  Provide TB specific handling for PrintRecRefund. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid PrintRecRefund"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandleResetPrinter( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    if ( pMsg->Get_ResetPrinter( ) )
    {
        tbtrace( TM_INFO, _T("ResetPrinter"));
        // TODO:  Provide TB specific handling for ResetPrinter. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid ResetPrinter"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandleSetDate( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    CString csDate;
    if ( pMsg->Get_SetDate( csDate ) )
    {
        tbtrace( TM_INFO, _T("SetDate( %s )"), csDate);
        // TODO:  Provide TB specific handling for SetDate. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid SetDate"));
    }
    return tbRC;
}

TBSTATE CCustomerMHFiscalPrinter::HandleSetHeaderLine( CFiscalPrinterMsg* pMsg )
{
    TBSTATE tbRC = TB_IGNORE;
    long Line;
    CString csText;
    bool bDWide;

    if ( pMsg->Get_SetHeaderLine( Line, csText, bDWide ) )
    {
        tbtrace( TM_INFO, _T("SetHeaderLine( %d, %s, %d )"), 
            Line, csText, bDWide );
        // TODO:  Provide TB specific handling for SetHeaderLine. 
    }
    else
    {
        tbtrace( TM_INFO, _T("Invalid SetHeaderLine"));
    }
    return tbRC;
}



