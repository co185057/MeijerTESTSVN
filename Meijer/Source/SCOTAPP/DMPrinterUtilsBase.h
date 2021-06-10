// DMPrinterUtilsBase.h: interface for the CDMPrinterUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMPRINTERUTILSBASE_H__5A15A867_1F26_4ADF_B49C_5F85094471A1__INCLUDED_)
#define AFX_DMPRINTERUTILSBASE_H__5A15A867_1F26_4ADF_B49C_5F85094471A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _CPPUNIT
#include "DllDefine.h"
#else
#include "TestMacros.h"
#endif
#include "DMProceduresConstants.h"


#ifdef _SCOTSSF_
    class DLLIMPORT
#else
    class DLLEXPORT
#endif
CDMPrinterUtilsBase  
{
public:
	CDMPrinterUtilsBase();
	virtual ~CDMPrinterUtilsBase();

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

    char* ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn, int* sizeout);
    int DecodeBase64Char(unsigned int ch); 
    BOOL Base64Decode(LPCSTR szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen); 
    
    

protected:
    static _TCHAR szTokenList[];

};

inline int CDMPrinterUtilsBase::DecodeBase64Char(unsigned int ch)
{
   // returns -1 if the character is invalid
   // or should be skipped
   // otherwise, returns the 6-bit code for the character
   // from the encoding table
   if (ch >= 'A' && ch <= 'Z')
       return ch - 'A' + 0;	// 0 range starts at 'A'
   if (ch >= 'a' && ch <= 'z')
       return ch - 'a' + 26;	// 26 range starts at 'a'
   if (ch >= '0' && ch <= '9')
       return ch - '0' + 52;	// 52 range starts at '0'
   if (ch == '+')
       return 62;
   if (ch == '/')
       return 63;
   return -1;
}  

inline BOOL CDMPrinterUtilsBase::Base64Decode(LPCSTR szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen)  
{
    // walk the source buffer
    // each four character sequence is converted to 3 bytes
    // CRLFs and =, and any characters not in the encoding table
    // are skiped
    
    if (szSrc == NULL || pnDestLen == NULL)
    {
        ASSERT(FALSE);
        return FALSE;
    }
    
    LPCSTR szSrcEnd = szSrc + nSrcLen;
    int nWritten = 0;
    
    BOOL bOverflow = (pbDest == NULL) ? TRUE : FALSE;
    
    while (szSrc < szSrcEnd &&(*szSrc) != 0)
    {
        DWORD dwCurr = 0;
        int i;
        int nBits = 0;
        for (i=0; i<4; i++)
        {
            if (szSrc >= szSrcEnd)
                break;
            int nCh = DecodeBase64Char(*szSrc);
            szSrc++;
            if (nCh == -1)
            {
                // skip this char
                i--;
                continue;
            }
            dwCurr <<= 6;
            dwCurr |= nCh;
            nBits += 6;
        }
        
        if(!bOverflow && nWritten + (nBits/8) > (*pnDestLen))
            bOverflow = TRUE;
        
        // dwCurr has the 3 bytes to write to the output buffer
        // left to right
        dwCurr <<= 24-nBits;
        for (i=0; i<nBits/8; i++)
        {
            if(!bOverflow)
            {
                *pbDest = (BYTE) ((dwCurr & 0x00ff0000) >> 16);
                pbDest++;
            }
            dwCurr <<= 8;
            nWritten++;
        }
        
    }
    
    *pnDestLen = nWritten;
    
    if(bOverflow)
    {
        if(pbDest != NULL)
        {
            ASSERT(FALSE);
        }
        
        return FALSE;
    }
    
    return TRUE;
}



#endif // !defined(AFX_DMPRINTERUTILSBASE_H__5A15A867_1F26_4ADF_B49C_5F85094471A1__INCLUDED_)
