// BaseMHSlip.cpp: implementation of the CBaseMHSlip class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHSlip.h"
#include "tracesystem.h"
#include "TBUtilities.h"
#include "BaseCurrentSlip.h"
#include "BaseObjectManager.h"


#define T_ID _T("CBaseMHSlip")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHSlip::CBaseMHSlip()
{
}

CBaseMHSlip::~CBaseMHSlip()
{
}


TBRC CBaseMHSlip::EndorsementCompleted(bool bSuppressSubsequent)
{
    TBRC rc = TB_SUCCESS;
    LPCTSTR  lpThisName = _T("CBaseMHSlip::EndorsementCompleted()");
    tbtrace( TM_ENTRY, _T("+-%s"), lpThisName);
    ASSERT(0);  // Assert until real functionality is added at this level
    return rc;
}


TBRC CBaseMHSlip::GetDocEndorsementInfo(long *plNumRemaining, LPTSTR *ppszDocDescription,
                                       PTSTR *ppszDocFront, LPTSTR *ppszDocBack)
{
    TBRC rc = TB_SUCCESS;
    LPCTSTR  lpThisName = _T("CBaseMHSlip::GetDocEndorsementInfo()");
    tbtrace( TM_ENTRY, _T("+%s"), lpThisName);
    CBaseCurrentSlip* pCurrSlip = CCustomerCurrentSlip::instance();

    *plNumRemaining = pCurrSlip->GetNumRemaining();
    CopyString(*ppszDocDescription, pCurrSlip->GetDocDescription());
    CopyString(*ppszDocFront, pCurrSlip->GetDocFront());
    CopyString(*ppszDocBack, pCurrSlip->GetDocBack());

    return rc;
}


HFILE CBaseMHSlip::GetSlipData(void)
{
    LPCTSTR  lpThisName = _T("CBaseMHSlip::GetSlipData()");
    tbtrace(TM_ENTRY, _T("+%s"), lpThisName);
    HFILE hFile = NULL;
    tbtrace(TM_ENTRY, _T("-%s"), lpThisName);
    ASSERT(0);  // Assert until real functionality is added at this level
    return hFile;   
}


// This is required functionality for all MH classes
TBSTATE CBaseMHSlip::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("CBaseMHSlip::ProcessMessage()"));
    TBSTATE rc = TB_IGNORE;

    CPrinterMsg *pMsg= dynamic_cast<CPrinterMsg *> (pMsgObj);
    if ( pMsg->StationId() == PTR_STATION_SLIP )
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
            tbtrace( TM_ERROR, _T("Invalid SLIP Printer Function %d"), pMsg->Function() );
        }
    }
    else
    {
        tbtrace( TM_ERROR, _T("Received SLIP message for station %d"), pMsg->StationId() );
        return TB_IGNORE;
    }

    return rc;
}


TBRC CBaseMHSlip::ReleaseSlipData(HFILE hSlipData)
{
    TBRC rc = TB_SUCCESS;
    tbtrace( TM_ENTRY,_T("ReleaseSlipData()"));
    return rc;
}



TBSTATE CBaseMHSlip::Process_CutPaper( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHSlip::Process_CutPaper()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHSlip::Process_Print( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHSlip::Process_Print()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHSlip::Process_Barcode( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHSlip::Process_Barcode()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHSlip::Process_Bitmap( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHSlip::Process_Bitmap()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHSlip::Process_SetBitmap( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHSlip::Process_SetBitmap()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHSlip::Process_SetLogo( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHSlip::Process_SetLogo()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHSlip::Process_SlipStatus( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHSlip::Process_SlipStatus()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

