// SolutionMHSlip.cpp: implementation of the CSolutionMHSlip class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionMHSlip.h"
#include "tracesystem.h"
#include "CustomerCurrentSlip.h"
#include "CustomerCurrentSigCap.h"

#define T_ID _T("CSolutionMHSlip")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSolutionMHSlip::CSolutionMHSlip() :
    m_bFront(false)
{
}

CSolutionMHSlip::~CSolutionMHSlip()
{
}

TBRC CSolutionMHSlip::EndorsementCompleted(bool bSuppressSubsequent)
{
	return CCustomerCurrentSlip::instance()->EndorsementCompleted( bSuppressSubsequent );
}



TBRC CSolutionMHSlip::GetDocEndorsementInfo(long   *plNumRemaining,
                                       LPTSTR *ppszDocDescription, 
                                       PTSTR  *ppszDocFront, 
                                       LPTSTR *ppszDocBack)
{
	return CCustomerCurrentSlip::instance()->GetDocEndorsementInfo(plNumRemaining,
													   ppszDocDescription, 
													   ppszDocFront, 
													   ppszDocBack);
}


TBSTATE CSolutionMHSlip::Process_Print( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("Process_Print()"));

#ifdef _DEBUG
    CString theData = pMsg->Data();
    TCHAR* pData = (TCHAR*)((LPCTSTR)theData);
    tbhextrace( TM_INFO, _T("PrtData"), (BYTE*)(pData), pMsg->Data().GetLength() * sizeof(TCHAR) );
#endif
    
    if ( m_bFront )
    {
        CCustomerCurrentSlip::instance()->WriteToSlipFront( pMsg->Data() );
    }
    else
    {
        CCustomerCurrentSlip::instance()->WriteToSlipBack( pMsg->Data() );
    }

	return TB_IGNORE;
}



TBSTATE CSolutionMHSlip::Process_SlipStatus( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("Process_SlipStatus()"));
    // NOTE:  BarcodeType contains the SlipStatus argument
    switch( pMsg->BarcodeType() )
    {
    case SLIPSTATUS_BEGININSERTION:
        tbtrace( TM_INFO, _T("SLIPSTATUS_BEGININSERTION"));
        break;
    case SLIPSTATUS_ENDINSERTION:
        tbtrace( TM_INFO, _T("SLIPSTATUS_ENDINSERTION"));
        break;
    case SLIPSTATUS_BEGINREMOVAL:
        tbtrace( TM_INFO, _T("SLIPSTATUS_BEGINREMOVAL"));
        CCustomerCurrentSlip::instance()->FinishSlip();
        break;
    case SLIPSTATUS_ENDREMOVAL:
        tbtrace( TM_INFO, _T("SLIPSTATUS_ENDREMOVAL"));
        break;
	case SLIPSTATUS_SLIPDESC:
		{
			tbtrace( TM_INFO, _T("SLIPSTATIS_SLIPDESC") );
			CCustomerCurrentSlip::instance()->SetSlipDescription( pMsg->Data() );
		}
		break;
    default:
        tbtrace( TM_ERROR, _T("SlipStatus with unknown status value %d"), pMsg->BarcodeType() );
    }

	return TB_IGNORE;
}


HFILE CSolutionMHSlip::GetSlipData(void)
{
	return CCustomerCurrentSigCap::instance()->GetSlipData();
}

TBRC CSolutionMHSlip::ReleaseSlipData(HFILE hSlipData)
{
	return CCustomerCurrentSigCap::instance()->ReleaseSlipData( hSlipData );
}


