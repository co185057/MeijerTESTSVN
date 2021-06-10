#include "StdAfx.h"
#include "PrinterAdapter.h"
#include "DMProcedures.h"
#include <assert.h>

#define COMP_ID ID_DM
#define T_ID _T("CPrinterAdapter")

#include "MethodTrace.h"

// Copyright (c) NCR Corp. 2010


CPrinterAdapter::CPrinterAdapter() : m_hFile(NULL)
{
    TRACE_METHOD(_T("CPrinterAdapter::CPrinterAdapter"));
    
    bitmapinfo.csBmpNameWithPath = _T("");
    bitmapinfo.nAlign = 0;
    bitmapinfo.nWidth = 0;
    barcodeinfo.BarcodeDataBuffer = _T("");
    barcodeinfo.bPrintNumber = false;
    barcodeinfo.nAlign = 0;
    barcodeinfo.nBufferLength = 0;
    barcodeinfo.nHeight = 0;
    barcodeinfo.nType = 0;
}


CPrinterAdapter::~CPrinterAdapter()
{
    TRACE_METHOD(_T("CPrinterAdapter::~CPrinterAdapter"));
}

CPrinterAdapter::CPrinterAdapter(const CPrinterAdapter &rhs)  ///< Disable copies.

{
    throw "ERROR";
}

void CPrinterAdapter::operator=(const CPrinterAdapter &rhs) ///< Disable copies.
{
    if(this == &rhs)
        return;

    throw "ERROR";
} //lint !e1539


bool CPrinterAdapter::PrintNormal(LPCTSTR szData, bool breceipt)
{
    TRACE_METHOD(_T("CPrinterAdapter::PrintNormal()"));

    assert(szData);
	
	bool bReturn = false;
    if (!bitmapinfo.csBmpNameWithPath.IsEmpty())
    {
        dm.PrintBitmap(bitmapinfo.csBmpNameWithPath, bitmapinfo.nAlign);        
    }

    if(dm.ReceiptLine(szData) != 0)
    {
        trace(L6, _T("ERROR: Unable to print receipt."));        
    }    

    if(_tcslen(barcodeinfo.BarcodeDataBuffer) != 0)
    {       
        dm.PrintBarcode(barcodeinfo.BarcodeDataBuffer, barcodeinfo.nType, barcodeinfo.nHeight, barcodeinfo.nAlign, barcodeinfo.bPrintNumber);                     
    }

    if(0 == dm.ReceiptCut())
    {
         bReturn = true;
    }
    else
    {
        trace(L6, _T("ERROR: Unable to cut receipt."));
    }

    return bReturn;
}

void CPrinterAdapter::AddBitMapInfo(CString csBmpNameWithPath, long nWidth, long nAlign)
{
    TRACE_METHOD(_T("CPrinterAdapter::AddBitMapInfo"));

    bitmapinfo.csBmpNameWithPath = csBmpNameWithPath;
    bitmapinfo.nAlign = nAlign;
    bitmapinfo.nWidth = nWidth;    
}

void CPrinterAdapter::AddBarCodeInfo(LPVOID BarcodeDataBuffer, int nBufferLength, long nType,
                             long nHeight, long nAlign, bool bPrintNumber)
{
    TRACE_METHOD(_T("CPrinterAdapter::AddBarCodeInfo"));

    assert(BarcodeDataBuffer);

    barcodeinfo.BarcodeDataBuffer=(LPCTSTR)BarcodeDataBuffer;    
    barcodeinfo.nBufferLength=nBufferLength;
    barcodeinfo.nType=nType;
    barcodeinfo.nHeight=nHeight;
    barcodeinfo.nAlign=nAlign;
    barcodeinfo.bPrintNumber=bPrintNumber;    
}

/**
 * Internal protected method to make freeing of the message
 * easier.
 */
void CPrinterAdapter::FreeMessage(void)
{
    ASSERT(0);
    trace(L6, _T("CPrinterAdapter::FreeMessage ** NOT IMPLEMENTED **"));

}

bool CPrinterAdapter::IsPrintMessage(const MessageElement *pMe) const
{
    trace(L6, _T("CPrinterAdapter::IsPrintMessage ** NOT IMPLEMENTED **"));

    return false;
}

bool CPrinterAdapter::PrintBarCode(LPCTSTR sFileName, long nWidth, long nHeight,
                            long nAlign, long nStation)
{

    ASSERT(0);
    trace(L6, _T("CPrinterAdapter::PrintBarCode ** NOT IMPLEMENTED **"));

    return false;
}


bool CPrinterAdapter::PrintBitmap(LPCTSTR sFileName, long nWidth, 
                            long nAlign, long nStation)
{

    ASSERT(0);
    trace(L6, _T("CPrinterAdapter::PrintBitmap ** NOT IMPLEMENTED **"));
    
    return false;
}

bool CPrinterAdapter::CutPaper(LONG Percentage, bool breceipt)
{

    ASSERT(0);
    trace(L6, _T("CPrinterAdapter::CutPaper ** NOT IMPLEMENTED **"));

    return false;
}

HFILE CPrinterAdapter::GetPrintFile(void)
{

    ASSERT(0);
    trace(L6, _T("CPrinterAdapter::GetPrintFile ** NOT IMPLEMENTED **"));

    return m_hFile;
}


bool CPrinterAdapter::ReleaseReceipt(void)
{

    ASSERT(0);
    trace(L6, _T("CPrinterAdapter::ReleaseReceipt ** NOT IMPLEMENTED **"));

    return true;
}
