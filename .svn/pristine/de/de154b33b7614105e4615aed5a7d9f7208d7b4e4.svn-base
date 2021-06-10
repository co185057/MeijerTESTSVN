// BaseMHJournal.cpp: implementation of the CBaseMHJournal class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHJournal.h"
#include "TraceSystem.h"

#define T_ID  _T("CBaseMHJournal")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHJournal::CBaseMHJournal()
{

}

CBaseMHJournal::~CBaseMHJournal()
{

}


/*************************************************************************
* GetJournalData - Return the file handle of a file that contains journal 
*        presentation data. If TB wants to print some transaction data on 
*        the journal printer attached to the FastLane terminal, this 
*        function should return a valid handle to a file that contains journal 
*        data. The data is transparent to FastLane and is submitted to the 
*        journal without any modifications. The journal is printed either on 
*        request by TB (synchronous print request: TB_COMPLETEPRINT state) 
*        or at the end of each transaction.
*
* Parameters: none
*
* Returns: HFILE  - handle of a file (NULL = no journal data)
*************************************************************************/
HFILE CBaseMHJournal::GetJournalData(void)
{
    tbtrace(TM_ENTRY, _T("+- GetJournalData() - Base class returning NULL always"));
    return NULL;
}

/*************************************************************************
* ProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. Base class does nothing.
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHJournal::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("+- ProcessMessage"));

    TBSTATE rc = TB_IGNORE;
    CPrinterMsg *pMsg= dynamic_cast<CPrinterMsg *> (pMsgObj);
    if ( pMsg->StationId() == PTR_STATION_JOURNAL )
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
            tbtrace( TM_ERROR, _T("Invalid JOURNAL Printer Function %d"), pMsg->Function() );
        }
    }
    else
    {
        tbtrace( TM_ERROR, _T("Received JOURNAL message for station %d"), pMsg->StationId() );
        return TB_IGNORE;
    }

    return rc;
}

/*************************************************************************
* TBReleaseJournalData - Release the presentation data for the journal printer.
*       Base class does not do anything with this yet.
*
* Parameters: None
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHJournal::ReleaseJournalData(HFILE hJournalData)
{
    tbtrace(TM_ENTRY, _T("+- ReleaseJournalData() - Empty Base class returning TB_SUCCESS always"));
    return TB_SUCCESS;
}



TBSTATE CBaseMHJournal::Process_CutPaper( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHJournal::Process_CutPaper()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHJournal::Process_Print( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHJournal::Process_Print()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHJournal::Process_Barcode( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHJournal::Process_Barcode()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHJournal::Process_Bitmap( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHJournal::Process_Bitmap()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHJournal::Process_SetBitmap( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHJournal::Process_SetBitmap()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHJournal::Process_SetLogo( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHJournal::Process_SetLogo()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

TBSTATE CBaseMHJournal::Process_SlipStatus( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("CBaseMHJournal::Process_SlipStatus()"));
    tbtrace( TM_INFO, _T( "EMPTY!!  Implementation is TB and Hook specific!"));
    return TB_IGNORE;
}

