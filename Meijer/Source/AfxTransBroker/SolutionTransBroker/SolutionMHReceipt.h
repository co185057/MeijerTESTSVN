// SolutionMHReceipt.h: interface for the CSolutionMHReceipt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLUTIONMHRECEIPT_H__INCLUDED_)
#define AFX_SOLUTIONMHRECEIPT_H__INCLUDED_

#include "BaseMHReceipt.h"
#include "PrinterMsg.h"
#include "SolutionMultiBitmap.h"


class CSolutionMHReceipt : public CBaseMHReceipt
{
public:
	virtual ~CSolutionMHReceipt();

    virtual HFILE GetReceiptData(void);

    virtual TBSTATE ProcessMessage(CTBMsgObj *pMsgObj);
    virtual TBRC ReleaseReceiptData(HFILE hReceiptData);

    virtual TBSTATE Process_Print( CPrinterMsg* pMsg );
    virtual TBSTATE Process_SetBitmap( CPrinterMsg* pMsg );
    virtual TBSTATE Process_Barcode( CPrinterMsg* pMsg );
    virtual TBSTATE Process_Bitmap( CPrinterMsg* pMsg );

public:
    inline CSolutionMultiBitmap & GetMultiBitmap() { return m_multiBitmap; }

protected:
	CSolutionMHReceipt();
    CSolutionMultiBitmap m_multiBitmap;

private:
    CSolutionMHReceipt(CSolutionMHReceipt &);                    // hide copy const
    CSolutionMHReceipt & operator =(const CSolutionMHReceipt &); // hide assignment

    virtual bool IsPaperCut(CString csData);
    virtual int ConvertHeightToLines( int nHeight );
    virtual int MapToFLAlign( int nAlign );
    virtual int MapToFLAlignBmp(int nAlign);
    virtual int MapToFLTextPosition( int nPosition );
    virtual int MapToFLSymbology( int nSymbology );
    virtual CString FilterPOSData( CString csRaw );
    bool m_bIgnore;
};


#endif
