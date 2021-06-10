// BaseMHPrinter.cpp: implementation of the CBaseMHPrinter class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHPrinter.h"
#include "tracesystem.h"
#include "BaseCurrentPrinter.h"
#include "BaseObjectManager.h"


#define T_ID _T("CBaseMHPrinter")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHPrinter::CBaseMHPrinter()
{

}

CBaseMHPrinter::~CBaseMHPrinter()
{

}

TBRC CBaseMHPrinter::GetPrinterError(LPTSTR* psErrorMessage)
{
    CBaseCurrentPrinter* pCurrPrinter = CCustomerCurrentPrinter::instance();
    return pCurrPrinter->GetPrinterError(psErrorMessage);
}


TBRC CBaseMHPrinter::Print(LPCTSTR szPrintFile)
{
    // Incomplete Base TB Function: This function needs to be implemented for the Base TB code.
    /*<%ASIS
    return TB_SUCCESS;
    %>*/
    TBRC rc = TB_E_INCOMPLETEPROCEDURE;
    LPCTSTR  lpThisName = _T("CBaseMHPrinter::Print()");
//    tbtrace( TM_ENTRY, _T("+-%s - Empty Base Functionality"), lpThisName);
    ASSERT(0);  // Assert until real functionality is added at this level
    return rc;
}


// This is required functionality for all MH classes
TBSTATE CBaseMHPrinter::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("CBaseMHPrinter::ProcessMessage()"));

    TBSTATE rc = TB_IGNORE;
	CPrinterMsg *pMsg= dynamic_cast<CPrinterMsg *> (pMsgObj);
	ASSERT(pMsg);
	if (pMsg)
    {
        switch ( pMsg->StationId() )
        {
        case PTR_STATION_JOURNAL:
            rc = CCustomerMHJournal::instance()->ProcessMessage( pMsgObj );
            break;
        case PTR_STATION_RECEIPT:
            rc = CCustomerMHReceipt::instance()->ProcessMessage( pMsgObj );
            break;
        case PTR_STATION_SLIP:
            rc = CCustomerMHSlip::instance()->ProcessMessage( pMsgObj );
            break;
        default:
            tbtrace( TM_ERROR, _T("Printer message for invalid station %d"), pMsg->StationId() );
        }
	}
    return rc;
}

