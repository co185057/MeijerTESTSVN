// DMPrinterUtils.h: interface for the CDMPrinterUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMPRINTERUTILS_H__5A15A867_1F26_4ADF_B49C_5F85094471A1__INCLUDED_)
#define AFX_DMPRINTERUTILS_H__5A15A867_1F26_4ADF_B49C_5F85094471A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DMPrinterUtilsBase.h"

#ifdef _SCOTSSF_
    class DLLEXPORT
#else
    class DLLIMPORT
#endif
CDMPrinterUtils : public CDMPrinterUtilsBase
{
public:
	CDMPrinterUtils();
	virtual ~CDMPrinterUtils();

    virtual long ReceiptLine(const CString& csPrintLine);
    virtual long ReceiptCut(void);            // cut the paper
    virtual long ProcessPrintLine(int nDeviceClass, const CString& csPrintLine);
    virtual long ProcessPrintTokens(int nDeviceClass, CString& CurrentPrintLine);
    virtual void ProcessSetProperty(CString csPropSet, long nDeviceClass);
    virtual long PrintBarcode(CString  csBarcode,
                              long     lBCType, 
                              long     lBCHeight, 
                              long     lBCAlign, 
                              bool     fPrintNumbers, 
                              int      nDeviceClass );
    virtual long PrintBitmap(CString  csBitmap, 
                             long     lWidth,
                             long     lAlign, 
                             int      nDeviceClass);
    virtual CString GetEncoded128CBarcode(CString csBarcodeData);
    virtual PRNOBJECTTOKENS FindPrnTokenType(_TCHAR chTokenPrefix);
    virtual long MapDmBarcodeTypeToOpos(long BarcodeType ); 
    virtual long MapDmBarcodeAlignToOpos(long nAlign ); 
    virtual void DecodeBase64String(CString &csEncodedData);
    virtual void ResetReceiptPrinter();

};

#endif // !defined(AFX_DMPRINTERUTILS_H__5A15A867_1F26_4ADF_B49C_5F85094471A1__INCLUDED_)
