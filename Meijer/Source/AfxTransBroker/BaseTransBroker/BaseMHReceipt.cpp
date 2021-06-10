// BaseReceiptData.cpp: implementation of the CBaseReceiptData class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHReceipt.h"
#include "tracesystem.h"

#define T_ID  _T("CBaseMHReceipt")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHReceipt::CBaseMHReceipt()
{
    
}

CBaseMHReceipt::~CBaseMHReceipt()
{
    
}


HFILE CBaseMHReceipt::GetReceiptData(void)
{
    tbtrace(TM_ENTRY, _T("+-GetReceiptData"));
    HFILE hFile = NULL;
    return hFile;   
}


// This is required functionality for all MH classes
TBSTATE CBaseMHReceipt::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_IGNORE;
    tbtraceIO( _T("CBaseMHReceipt::ProcessMessage"));

    CPrinterMsg *pMsg= dynamic_cast<CPrinterMsg *> (pMsgObj);
    if ( pMsg->StationId() == PTR_STATION_RECEIPT )
    {
        switch( pMsg->Function() )
        {
        case PTR_FUNCTION_CUTPAPER:
            rc = Process_CutPaper( pMsg );
            break;
        case PTR_FUNCTION_PRINT:
            rc = Process_Print( pMsg );
            break;
        case PTR_FUNCTION_BARCODE:
            rc = Process_Barcode( pMsg );
            break;
        case PTR_FUNCTION_BITMAP:
            rc = Process_Bitmap( pMsg );
            break;
        case PTR_FUNCTION_SETBITMAP:
            rc = Process_SetBitmap( pMsg );
            break;
        case PTR_FUNCTION_SETLOGO:
            rc = Process_SetLogo( pMsg );
            break;
        case PTR_FUNCTION_SLIPSTATUS:
            rc = Process_SlipStatus( pMsg );
            break;
        default:
            tbtrace( TM_ERROR, _T("Invalid RECEIPT Printer Function %d"), pMsg->Function() );
        }
    }
    else
    {
        tbtrace( TM_ERROR, _T("Received RECEIPT message for station %d"), pMsg->StationId() );
        return TB_IGNORE;
    }
    
    return rc;
}


TBRC CBaseMHReceipt::ReleaseReceiptData(HFILE hReceiptData)
{
    TBRC rc = TB_SUCCESS;
    tbtrace( TM_ENTRY, _T("+-ReleaseReceiptData"));
    return rc;
}



TBSTATE CBaseMHReceipt::Process_CutPaper( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHReceipt::Process_CutPaper()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHReceipt::Process_Print( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHReceipt::Process_Print()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHReceipt::Process_Barcode( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHReceipt::Process_Barcode()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHReceipt::Process_Bitmap( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHReceipt::Process_Bitmap()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHReceipt::Process_SetBitmap( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHReceipt::Process_SetBitmap()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHReceipt::Process_SetLogo( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHReceipt::Process_SetLogo()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHReceipt::Process_SlipStatus( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHReceipt::Process_SlipStatus()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

