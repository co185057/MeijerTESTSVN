// DMPrinterUtils.cpp: implementation of the CDMPrinterUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DMPrinterUtils.h"

#include "Common.h"
#include "CommonNCR.h"
#include "CommonServices.h"
#include "dmx\printer.h"

//SR672+
#include "DeviceErrorLookUpAPI.h" 
#include "OposPtr.h"
#include <OposPtr.hi>
//SR672-

#include "DMXAdapter.h" //SR700
#include "PrinterAdapter.h" //SR700

#define DEVICEID0 0
#define DEVICEID1 1

#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMSSF")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// macro to handle exception processing using multiple catch{}'s
// pass return value if exception caught
#define PROCESS_EXCEPTIONS_EX(i,n)                           \
	catch(COleDispatchException * e)                            \
{                                                              \
  rc = dm.OleDispatchException(e,i,n,msg,__LINE__);                 \
  e->Delete();                                                 \
}                                                              \
	catch(COleException* e)                                     \
{                                                              \
  rc = -2;                                                     \
  dm.OleException(e,i,n,msg,__LINE__);                              \
  e->Delete();                                                 \
}                                                              \
	catch(CException* e)                                        \
{                                                              \
  rc = -2;                                                     \
  dm.Exception(e,i,n,msg,__LINE__);                                 \
  e->Delete();                                                 \
}                                                              \
	catch(...)                                                  \
{                                                              \
  rc = -2;                                                     \
  long le = GetLastError();                                    \
}
// Same but with DeviceId assumed to be zero
#define PROCESS_EXCEPTIONS(i) PROCESS_EXCEPTIONS_EX(i,0)



static _TCHAR szTokenList[] = {BARCODETOK_START, BMPTOK_START, BMPTOK_END,_T('\0')};
static _TCHAR szWork[255];

PRNOBJECTTOKENS FindPrnTokenType(_TCHAR chTokenPrefix);

static long rc;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDMPrinterUtils::CDMPrinterUtils()
{
    
}

CDMPrinterUtils::~CDMPrinterUtils()
{
    
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
long CDMPrinterUtils::ReceiptLine(const CString& csPrintLine) // print a line
{
    return CDMPrinterUtilsBase::ReceiptLine(csPrintLine);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
long CDMPrinterUtils::ReceiptCut(void)
{
    return CDMPrinterUtilsBase::ReceiptCut();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
long CDMPrinterUtils::ProcessPrintLine(int nDeviceClass, const CString& csPrintLine)
{
    return CDMPrinterUtilsBase::ProcessPrintLine(nDeviceClass, csPrintLine);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
long CDMPrinterUtils::ProcessPrintTokens(int nDeviceClass, CString& CurrentPrintLine)
{
    return CDMPrinterUtilsBase::ProcessPrintTokens(nDeviceClass, CurrentPrintLine);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void CDMPrinterUtils::ProcessSetProperty(CString csPropSet, long nDeviceClass)
{
    CDMPrinterUtilsBase::ProcessSetProperty(csPropSet,nDeviceClass);
}


//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
long CDMPrinterUtils::PrintBarcode(CString  csBarcode,
                          long     lBCType, 
                          long     lBCHeight, 
                          long     lBCAlign, 
                          bool     fPrintNumbers, 
                          int      nDeviceClass )
{
     return CDMPrinterUtilsBase::PrintBarcode(csBarcode,
                          lBCType, 
                          lBCHeight, 
                          lBCAlign, 
                          fPrintNumbers, 
                          nDeviceClass );
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
long CDMPrinterUtils::PrintBitmap(CString  csBitmap, 
                         long     lWidth,
                         long     lAlign, 
                         int      nDeviceClass)
{
    return CDMPrinterUtilsBase::PrintBitmap(csBitmap, 
                         lWidth,
                         lAlign, 
                         nDeviceClass);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
CString CDMPrinterUtils::GetEncoded128CBarcode(CString csBarcodeData)
{
    return CDMPrinterUtilsBase::GetEncoded128CBarcode(csBarcodeData);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
PRNOBJECTTOKENS CDMPrinterUtils::FindPrnTokenType(_TCHAR chTokenPrefix)
{
    return CDMPrinterUtilsBase::FindPrnTokenType(chTokenPrefix);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
long CDMPrinterUtils::MapDmBarcodeTypeToOpos(long BarcodeType )
{
    return CDMPrinterUtilsBase::MapDmBarcodeTypeToOpos(BarcodeType );
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
long CDMPrinterUtils::MapDmBarcodeAlignToOpos(long nAlign )
{
    return CDMPrinterUtilsBase::MapDmBarcodeAlignToOpos(nAlign );
}

//////////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////////
void CDMPrinterUtils::DecodeBase64String(CString &csEncodedData)
{
    CDMPrinterUtilsBase::DecodeBase64String(csEncodedData);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Reset printer values that might be set by TB SetProp commands
//////////////////////////////////////////////////////////////////////////////////////////////
void CDMPrinterUtils::ResetReceiptPrinter()
{
    CDMPrinterUtilsBase::ResetReceiptPrinter();
}