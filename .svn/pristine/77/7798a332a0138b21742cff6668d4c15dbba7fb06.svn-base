#ifndef _PRINTERADAPTER_H
#define _PRINTERADAPTER_H

#include "IXMPrinter.h"

struct structBitmap
{
    CString csBmpNameWithPath;
    long nWidth;
    long nAlign;
};
struct structBarcode
{
    LPCTSTR BarcodeDataBuffer;
    int nBufferLength;
    long nType;
    long nHeight;
    long nAlign;
    bool bPrintNumber;
};
/**
 * \class  CPrinterAdapter
 * \brief  Implementation of IXMPrinter interface
 * \author Renato L. Taer
 * \note   Copyright (c) NCR Corp. 2010
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
class CPrinterAdapter  : public IXMPrinter 
{
public:
    virtual bool PrintNormal(LPCTSTR data, bool breceipt=true);
    virtual bool PrintBarCode(LPCTSTR sFileName, long nWidth, long nHeight,
                            long nAlign, long nStation);
    virtual bool PrintBitmap(LPCTSTR sFileName, long nWidth,
                            long nAlign, long nStation);
    virtual HFILE GetPrintFile();
    virtual void AddBarCodeInfo(LPVOID BarcodeDataBuffer, int nBufferLength, long nType,
                             long nHeight, long nAlign, bool bPrintNumber);
    virtual void AddBitMapInfo(CString csBmpNameWithPath, long nWidth, long nAlign);
    virtual bool IsPrintMessage(const MessageElement *pMe) const;
    virtual bool ReleaseReceipt();
    virtual bool CutPaper(LONG Percentage, bool breceipt=true);
    virtual void FreeMessage();

    CPrinterAdapter();    
    virtual ~CPrinterAdapter();
protected:
    CPrinterAdapter(const CPrinterAdapter &rhs);    ///< Disable copies.
    void operator=(const CPrinterAdapter &rhs); 	///< Disable copies.
	
    HFILE m_hFile;	//handle to the file to be printed
   
    structBitmap bitmapinfo;
    structBarcode barcodeinfo;  
};  

#endif // _PRINTERADAPTER_H
