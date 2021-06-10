// RecallList.h: interface for the RecallList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECALLLISTBASE_H__119DB42B_9235_464A_8BF0_8AC505B2EA03__INCLUDED_)
#define AFX_RECALLLISTBASE_H__119DB42B_9235_464A_8BF0_8AC505B2EA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "common.h"
#ifndef _CPPUNIT
#include "DllDefine.h"
#else
#include "FakeCommon.h"
#include "OposScan.h"
#endif
#include "MobileConstants.h"
#include <queue>//RFQ 3160


/* This header file is documented with comment fields that are usable by Doxygen */

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 CRecallListBase  
{
public:
    CRecallListBase();
    virtual ~CRecallListBase();

    virtual void Initialize(CString csTerminalNumber);
    virtual void Uninitialize();
    virtual void Reset();
    virtual bool LoadQBBarcodes(const CString&);
    virtual CString GetScanData();
    virtual CString GetScanDataLabel();
    virtual int GetBarcodeType();
    virtual int GetBarcodeQuantity();
    virtual long GetUnitPrice();
    virtual long GetExtendedPrice();
    virtual bool ParseNextItem(bool bPopItem = true);
    virtual bool IsMobileTransaction();

protected:

    virtual bool DecodeBarcode(CString csBarcode);
    virtual bool SplitString(CString , CString , CStringArray* );
    virtual bool Load_FMT_001(CString csItemsBarcode);

    std::queue<CString> g_RecallItemQueue; ///<Queue containing individual barcodes (with barcode type) from scan data
    bool m_bMobileTransaction;                 ///<Boolean value indicating a list of mobile items was loaded
    bool m_bItemListLoaded;           ///<The list was successfully loaded.
    int m_nNumListItems;              ///<The number of items in this list
    int m_nNumItemsTotal;             ///<The number of recall items in this transaction (cumulative);
    long m_lCurrentUnitPrice;         ///<The unit price of the current item
    long m_lCurrentExtendedPrice;     ///<The extended price of the current item
    CString m_csListID;               ///<Unique ID of last list that was loaded
    CString m_csCurrentScanData;      ///<ScanData field - individual barcode including barcode type indicator
    CString m_csCurrentScanDataLabel; ///<Current barcode without barcode type indicator
    int m_nCurrentBarcodeType;        ///<Current barcode type as specified by OposScan.h file
    int m_nCurrentBarcodeQuantity;    ///<Quantity of current barcode to be sold.        
    CString m_csTerminalNumber;       ///<Terminal number sent as parameter to initialize method
    CString m_csFileName;             ///<Filename per transaction. Format "QB_<terminalNumber>_<DateTimeString>.txt
    bool m_bPriceEmbedded;            ///<Boolean indicating current item is a price embedded item.

};

#endif // !defined(AFX_RECALLLISTBASE_H__119DB42B_9235_464A_8BF0_8AC505B2EA03__INCLUDED_)
