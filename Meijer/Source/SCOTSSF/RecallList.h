// RecallList.h: interface for the RecallList class.
// //UploadTicket Port - Entire file heavily modified
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECALLLIST_H__119DB42B_9235_464A_8BF0_8AC505B2EA03__INCLUDED_)
#define AFX_RECALLLIST_H__119DB42B_9235_464A_8BF0_8AC505B2EA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "common.h"
#include "DllDefine.h"
#include "RecallListBase.h"
#include "MobileConstants.h"
#include <queue>//RFQ 3160

#include "ItemObject.h"
#include "QueuedActionsContainer.h"
#include "QueuedActionRecallItem.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
CRecallList  : public CRecallListBase
{
public:
    CRecallList();
    virtual ~CRecallList();
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
    CString GetDescription();
    void QueueMobileItem(IQueuedAction* pMobileItem);
    void QueueMobileItemSold(CString csEntryID, CString csTrimmedItemCode);
    IQueuedAction* GetNextSoldItem(bool bPopItem);
    int GetNumRecallItems();
    int GetNumSoldItems();

protected:

    virtual bool DecodeBarcode(CString csBarcode);
    virtual bool SplitString(CString , CString , CStringArray* );
    virtual bool Load_FMT_001(CString csItemsBarcode);
    CQueuedActionsContainer* m_MobileRecallQueue;  //queue of Mobile items for the transaction
    CQueuedActionsContainer* m_MobileItemSoldList; //Items sold during the transaction

    bool m_bMobileTransaction;                 ///<Boolean value indicating a list of mobile items was loaded
    int m_nNumListItems;              ///<The number of items in this list
    int m_nNumItemsTotal;             ///<The number of recall items in this transaction (cumulative);
    CQueuedActionRecallItem *pCurrentItem;
    CString m_csCurrentScanData;      ///<ScanData field - individual barcode including barcode type indicator
    CString m_csCurrentScanDataLabel; ///<Current barcode without barcode type indicator
    int m_nCurrentBarcodeType;        ///<Current barcode type as specified by OposScan.h file
    int m_nCurrentBarcodeQuantity;    ///<Quantity of current barcode to be sold.        
    CString m_csTerminalNumber;       ///<Terminal number sent as parameter to initialize method
    CString m_csCurrentDescription;   ///<Description of current item from

};

#endif // !defined(AFX_RECALLLIST_H__119DB42B_9235_464A_8BF0_8AC505B2EA03__INCLUDED_)
