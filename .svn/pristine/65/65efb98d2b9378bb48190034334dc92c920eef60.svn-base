// SolutionReceiptData.cpp: implementation of the CSolutionReceiptData class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionMHReceipt.h"
#include "tracesystem.h"
#include "CustomerMHOptions.h"
#include "CustomerCurrentTransaction.h"
#include "CustomerCurrentReceipt.h"
#include "PrinterMsg.h"
#include "OposPtr.h"
#include "ncrdevmgr.h"
#include "CustomerTrxBroker.h"

#define T_ID  _T("CSolutionMHReceipt")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSolutionMHReceipt::CSolutionMHReceipt()
{
    m_bIgnore = true;
    
    if( m_multiBitmap.IsEnabled() )
    {
        m_multiBitmap.Init();
    }
}

CSolutionMHReceipt::~CSolutionMHReceipt()
{
    
}


HFILE CSolutionMHReceipt::GetReceiptData(void)
{
    tbtrace(TM_ENTRY, _T("+-GetReceiptData"));
    return CCustomerCurrentReceipt::instance()->GetReceiptData();   
}


TBRC CSolutionMHReceipt::ReleaseReceiptData(HFILE hReceiptData)
{
    TBRC rc = TB_SUCCESS;
    tbtrace( TM_ENTRY, _T("+-ReleaseReceiptData"));
	
	CCustomerCurrentReceipt::instance()->ReleaseReceiptData( hReceiptData);

    return rc;
}


TBSTATE CSolutionMHReceipt::Process_Print( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("Process_Print()"));

#ifdef _DEBUG
    CString theData = pMsg->Data();
    TCHAR* pData = (TCHAR*)((LPCTSTR)theData);
    tbhextrace( TM_INFO, _T("PrtData"), (BYTE*)(pData), pMsg->Data().GetLength() * sizeof(TCHAR) );
#endif

    CString csData = pMsg->Data();
    CCustomerCurrentReceipt* pReceipt = (CCustomerCurrentReceipt*)CCustomerCurrentReceipt::instance();
    if ( IsPaperCut( csData ) )
    {
        pReceipt->FinishReceipt( m_bIgnore);
        m_bIgnore = false;
    }   
    else
    {
        const LPCTSTR szBitmap = _T("\x1B|1B");
        const LPCTSTR szLineFeed = _T(" \x0A");
        CString csFinal = FilterPOSData( csData );
        if ( !csFinal.IsEmpty() )
        {
            pReceipt->WriteToReceipt( csFinal );
        }
        // The Bitmap prints immediately after the end of the last receipt. 
        // Override the InProgress state so that this print line will not report as a
        // receipt in-progress.
        if ( 0 == csFinal.Compare( szBitmap ) )
        {
            pReceipt->OverrideInProgress( true );
        }
        else
        {
            if ( !csFinal.IsEmpty() && (csFinal != szLineFeed) )
            {
                pReceipt->OverrideInProgress( false );
            }
        }
    }
    return TB_IGNORE;
}

TBSTATE CSolutionMHReceipt::Process_SetBitmap( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("Process_SetBitmap()"));

    if( m_multiBitmap.IsEnabled() )
    {
        int iRet = m_multiBitmap.AddRequest(
                        pMsg->BitmapLogoNumber(),
                        pMsg->StationId(),
                        pMsg->Data(), 
                        (long)(short)pMsg->Width(),     // this is a signed 16-bit value
                        (long)(char)pMsg->Alignment()); // this is a signed 8-bit value
                        //MapToFLAlignBmp((char)pMsg->Alignment())); // this is a signed 8-bit value
        if( iRet == -1 )
        {
            tbtrace(TM_ERROR, _T("Failed to add SetBitmap request to the queue."));
        }
    }
    else
    {
        // ACS uses a standard naming convention for the bitmap files.
        // The setbitmap should always use the default below
        const LPCTSTR szACSDefault = _T("C:\\ACS\\DATA\\PAMLOGO10.DAT");

        if ( 0 != pMsg->Data().CompareNoCase( szACSDefault ) )
        {
            tbtrace(TM_ERROR, _T("SetBitmap called with unexpected file: %s"), (LPCTSTR)pMsg->Data() );
        }
    }
    return TB_IGNORE;
}

TBSTATE CSolutionMHReceipt::Process_Barcode( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("Process_Barcode()"));

#ifdef _DEBUG
    tbtrace(TM_INFO, (LPCTSTR)pMsg->Data() );
#endif

    // FastLane Barcode TokenFormat:
    // ____________________________________________________________________________________
    //|			    |	     |        |        | print numbers    |             |            |
    //|  BCTOKSTART | TYPE   | HEIGHT |  ALIGN |   at the bottom  | BarcodeData |  BCTOKEND  | 
    //|_____________|________|________|________|__0 or 1__________|_____________|____________|
    //

    const TCHAR BarcodeStart = '\x04';
    const TCHAR BarcodeEnd = '\x0F';
    CString csHeight;
    CString csAlign;
    CString csSymbology;
    CString csTextPosition;

    // FastLane "Height" is the number of lines.  OPOS is number of units
    // A conversion is required.
    csHeight.Format(_T("%-4d"), this->ConvertHeightToLines( pMsg->Height() ));
    csAlign.Format(_T("%-4d"), this->MapToFLAlign( (char)pMsg->Alignment() ));
    csSymbology.Format(_T("%-4d"), this->MapToFLSymbology((char)pMsg->BarcodeType()) );
    csTextPosition.Format(_T("%d"), this->MapToFLTextPosition((char)pMsg->BarcodeCharacterPrint()) );

    CString csBarcodeData;
    csBarcodeData.Format(_T("%c%s%s%s%s%s%c"), BarcodeStart,
        csSymbology,
        csHeight,
        csAlign,
        csTextPosition,
        (LPCTSTR)pMsg->Data(),
        BarcodeEnd );

    CCustomerCurrentReceipt::instance()->WriteToReceipt( csBarcodeData );

    return TB_IGNORE;
}

TBSTATE CSolutionMHReceipt::Process_Bitmap( CPrinterMsg* pMsg )
{
    tbtraceIO(_T("Process_Bitmap()"));

    // FastLane Bitmap TokenFormat:
    // ___________________________________________________________
    //|		         |	      |        |             |            |
    //|  BMPTOKSTART | WIDTH  |  ALIGN | BitmapData  | BMPTOKEND  | 
    //|______________|________|________|_____________|____________|
    //
    CString csWidth;
    CString csAlign;
    CString csBitmapData;
    const TCHAR cBitmapTokenStart = _T('\x05');
    const TCHAR cBitmapTokenEnd = _T('\x0F');

	CString csNewBitmap = CCustomerCurrentReceipt::instance()->PerserveBitmap( pMsg->Data() );

    // FastLane will ignore the width, so just stuff it with zeros
    csWidth.Format(_T("%-4d"), 0 );
    csAlign.Format(_T("%-4d"), this->MapToFLAlign( pMsg->Alignment() ));
    csBitmapData = cBitmapTokenStart +
                   csWidth +
                   csAlign +
                   csNewBitmap +
                   cBitmapTokenEnd;

    CCustomerCurrentReceipt::instance()->WriteToReceipt( csBitmapData );

    return TB_IGNORE;
}


CString CSolutionMHReceipt::FilterPOSData( CString csRaw )
{
    tbtraceIO(_T("FilterPOSData()"));
    CString csReturn = csRaw;
    const LPCTSTR szPtrStatus = _T("\x1Bv");

    if ( 0 == csRaw.Compare( szPtrStatus ) )
    {
        csReturn.Empty();
    }

    return csReturn;
}


/* OPOS Utility functions */


// Assumes csData only contains an OPOS ESC sequence
bool CSolutionMHReceipt::IsPaperCut( CString csData )
{
    tbtraceIO(_T("IsPaperCut()"));
    bool bCut = false;

    const TCHAR cOposESC = _T('\x1B');

    if ( (csData[0] == cOposESC) && (csData[1] == _T('|')) )
    {
        int nFind = csData.Find(_T("P"), 2 );
        switch( nFind )
        {
            case -1:
                break;
            case 2:
                bCut = true;
                break;
            default:
                {
                    CString csCheck = csData.Mid(2, nFind-2);
                    bool bAllDigit = true;
                    for( int i=0; bAllDigit && (i<csCheck.GetLength()); i++ )
                    {
                        bAllDigit = (_istdigit(csCheck[i]) != 0);
                    }
                    bCut = bAllDigit;
                }
        }
    }
    return bCut;
}

int CSolutionMHReceipt::ConvertHeightToLines( int nHeight )
{
    tbtraceIO(_T("ConvertHeightToLines()"));
    float fAnswer = nHeight;
    float fConvert;
    CString csDots = CCustomerTrxBroker::instance()->TBGetProperty(_T("POSPrinterDotsPerLine"));
    fConvert = (float)_ttoi(csDots);
    if ( fConvert <= 0 )
    {
        // Dots per line for 7158/7167 printers
        tbtrace( TM_ERROR, _T("POSPrinterDotsPerLine not configured properly!"));
        fConvert = 24;
    }

    fAnswer /= fConvert;
    fAnswer += 0.5;  // Make sure we round up!

    int nAnswer = (int)fAnswer;
    tbtrace( TM_INFO, _T("Converted value is %d"), nAnswer );
    return (int)fAnswer;
}

int CSolutionMHReceipt::MapToFLAlign( int nAlign )
{
    tbtraceIO(_T("MapToFLAlign()"));
    int nRet = DM_ALIGN_CENTER;

    // NOTE:  The OPOS bitmap alignment values are equivalent to the barcode
    //   alignment values.
    switch( nAlign )
    {
    case PTR_BC_LEFT:
        nAlign = DM_ALIGN_LEFT;
        break;
    case PTR_BC_CENTER:
        nAlign = DM_ALIGN_CENTER;
        break;
    case PTR_BC_RIGHT:
        nAlign = DM_ALIGN_RIGHT;
        break;
    default:
        tbtrace(TM_ERROR, _T("Non-standard barcode alignment, assuming center"));
    }
    return nRet;
}

int CSolutionMHReceipt::MapToFLAlignBmp(int nAlign)
{
    tbtraceIO(_T("MapToFLAlignBmp()"));
    int nRet = DM_ALIGN_CENTER;

    // NOTE:  The OPOS bitmap alignment values are equivalent to the barcode
    //   alignment values.
    switch( nAlign )
    {
    case PTR_BM_LEFT:
        nAlign = DM_ALIGN_LEFT;
        break;
    case PTR_BM_CENTER:
        nAlign = DM_ALIGN_CENTER;
        break;
    case PTR_BM_RIGHT:
        nAlign = DM_ALIGN_RIGHT;
        break;
    default:
        tbtrace(TM_ERROR, _T("Non-standard bitmap alignment, assuming center"));
    }
    return nRet;

}

int CSolutionMHReceipt::MapToFLTextPosition( int nPosition )
{
    tbtraceIO(_T("MapToFLTextPosition()"));
    int nRet = 1;  // Default to print below

    switch( nPosition )
    {
    case PTR_BC_TEXT_NONE:
        nRet = 0;
        break;
    case PTR_BC_TEXT_BELOW:
        break;
    case PTR_BC_TEXT_ABOVE:
        tbtrace(TM_ERROR, _T("FastLane does not support BC_TEXT_ABOVE, printing below"));
        break;
    default:
        tbtrace(TM_ERROR, _T("Undefined text position.  printing below"));
    }

    return nRet;
}

int CSolutionMHReceipt::MapToFLSymbology( int nSymbology )
{
    tbtraceIO(_T("MapToFLSymbology()"));
    int nRet = nSymbology;

    switch( nSymbology )
    {
    case PTR_BCS_UPCA:
        nRet = DM_BCS_UPCA;
        break;
    case PTR_BCS_UPCE:
        nRet = DM_BCS_UPCE;
        break;
    case PTR_BCS_JAN8:
        nRet = DM_BCS_JAN8;
        break;
    case PTR_BCS_JAN13:
        nRet = DM_BCS_JAN13;
        break;
    case PTR_BCS_ITF:
        nRet = DM_BCS_ITF;
        break;
    case PTR_BCS_Codabar:
        nRet = DM_BCS_CODABAR;
        break;
    case PTR_BCS_Code39:
        nRet = DM_BCS_CODE39;
        break;
    case PTR_BCS_Code128:
        nRet = DM_BCS_CODE128;
        break;
    default:
        tbtrace(TM_ERROR, _T("FastLane does NOT support printing symbology %d on receipts!"),
            nSymbology );
    }

    return nRet;
}


// This is required functionality for all MH classes
#define PTR_FUNC_PRINTMATCH 8

TBSTATE CSolutionMHReceipt::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_IGNORE;
    tbtraceIO( _T("ProcessMessage"));

    CPrinterMsg *pMsg= dynamic_cast<CPrinterMsg *> (pMsgObj);


    if (( pMsg->StationId() == PTR_STATION_RECEIPT ) &&
		( pMsg->Function() == PTR_FUNC_PRINTMATCH ))
    {
		CCustomerCurrentReceipt::instance()->AddStoreRecMatch( pMsg->Data() );
    }
	else
	{
		rc = CBaseMHReceipt::ProcessMessage( pMsgObj );
	}
    
    return rc;
}
