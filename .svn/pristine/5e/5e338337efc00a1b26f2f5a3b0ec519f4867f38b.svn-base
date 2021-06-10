// IXMPrinter.h: interface for the IXMPrinter class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _IXMPRINTER_H
#define _IXMPRINTER_H
#include "XMExport.h"
#include "ScotMsg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * \class IXMPrintNew
 * \RFC2774_21
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
class IXMPrinter  
{
public:
    /**
     * Print receipt.
     * \param[in] data - Text to be sent to SCOTApp for print.
     * \param[in] breceipt - If true, send data to the receipt printer.
     *                       If false, send data to the journal printer.
     *                       Journal printing is not currently supported.
     * \return true on success. false on failure.
     */
    virtual bool PrintNormal(LPCTSTR data, bool breceipt=true) = 0;
    
   /**
     * Print barcode file.
     * \param[in] sFileName - barcode to be printed.
     * \param[in] nWidth - Printed width of the barcode to be performed.
     * \param[in] nAlign - Placement of the barcode
     * \param[in] nStation - The printer station to be used. May be either
     *                     PTR_S_RECEIPT or PTR_S_SLIP
     */
    virtual bool PrintBarCode(LPCTSTR sFileName, long nWidth, long nHeight,
                            long nAlign, long nStation) = 0;
    /**
     * Print bitmap file.
     * \param[in] sFileName - Name of bitmap file to print.
     * \param[in] nWidth - Printed width of the bitmap to be performed.
     * \param[in] nAlign - Placement of the bitmap
     * \param[in] nStation - The printer station to be used. May be either
     *                     PTR_S_RECEIPT or PTR_S_SLIP
     */
    virtual bool PrintBitmap(LPCTSTR sFileName, long nWidth,
                            long nAlign, long nStation) = 0;

    /**
     * Gets Filen where print data is saved.
     *\return File handle for the print data or NULL if there is no file data.
     */
    virtual HFILE GetPrintFile() = 0;

    /**
     * Set Barcode information to the receipt data
	 * \param[in] BarcodeDataBuffer - Barcode Data to be appended to the data receipt
	 * \param[in] nBufferLength - length of barcode data
	 * \param[in] nType	- barcode type
	 * \param[in] nHeight - printed height of the barcode
	 * \param[in] nAlight - placement of the barcode
     */
	virtual void AddBarCodeInfo(LPVOID BarcodeDataBuffer, int nBufferLength, long nType,
                             long nHeight, long nAlign, bool bPrintNumber) = 0;

    /**
     * Set Barcode information to the receipt data
	 * \param[in] csBmpNameWithPath - Bitmap name and it's file path
	 * \param[in] nWidth - width of the bitmap to be printed
	 * \param[in] nAlight - placement of the bitmap
     */
	virtual void AddBitMapInfo(CString csBmpNameWithPath, long nWidth, long nAlign) = 0;

    /* RFC2774_21
     * Determines if Printing is from CashManagement
     * \param[MessageElement*] pMe - Package the print request in this MessageElement
     * \return true on print is fom CM.  false if print is not from CM.
     */
    virtual bool IsPrintMessage(const MessageElement *pMe) const = 0;

	/*
	 * Free the data for the CM receipt printing.
	 * \return true if successful.
     */
    virtual bool ReleaseReceipt() = 0;

	/*
	 * \param[in] Percentage 
	 * \param[in] breceipt - Journal or Receipt printing
	 * \return true if successful.
     */
	virtual bool CutPaper(LONG Percentage, bool breceipt=true) = 0;


	/*
	 * Frees Message Element.
	 * \return true if successful.
     */
	virtual void FreeMessage() = 0;


    IXMPrinter(){};
    virtual ~IXMPrinter(){};

protected:
    IXMPrinter(const IXMPrinter &rhs);       ///< Disable copies.

    void operator=(const IXMPrinter &rhs);  ///< Disable copies.

};

#endif // _IXMPRINTER_H
