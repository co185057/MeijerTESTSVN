#ifndef _CQUEUED_ACTION_RECALL_ITEM
#define _CQUEUED_ACTION_RECALL_ITEM

#include "IQueuedAction.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#elif defined _SCOT
class DLLIMPORT
#else
class 
#endif
 CQueuedActionRecallItem : public IQueuedAction
{
public:
    CQueuedActionRecallItem(CString csID, 
                           CString csDMLastBarLabel, 
                           CString csDMLastBarCode, 
                           long lBarcodeType);
    virtual ~CQueuedActionRecallItem();
    virtual SMStateBase* HandleQueuedAction(SMStateBase* pCurrentState);
    virtual void SetActionID(CString csID);
    virtual CString GetScanData() {return m_csDMLastBarCode;} ;
    virtual CString GetScanDataLabel() {return m_csDMLastBarLabel;};
    virtual long GetBarcodeType() {return m_lBarcodeType;};

    virtual void SetDescription(CString csDescription) {m_csDescription = csDescription;};
    virtual CString GetDescription() {return m_csDescription;};

    virtual void SetOrigin(long lOrigin) {m_lOrigin=lOrigin;};
    virtual long GetOrigin() {return m_lOrigin;};

    virtual void SetIsLoyaltyID(bool bIsLoyaltyID) {m_bIsLoyaltyID = bIsLoyaltyID;};
    virtual bool GetIsLoyaltyID() {return m_bIsLoyaltyID;};

    virtual void SetItemProperty(long lItemProperty) {m_lItemProperty = lItemProperty;};
    virtual long GetItemProperty() {return m_lItemProperty;};

    virtual void SetAgeRestricted(bool bAgeRestricted) {m_bAgeRestricted = bAgeRestricted;};
    virtual bool GetAgeRestricted() {return m_bAgeRestricted;};

    virtual void SetNetPrice(long lNetPrice) {m_lNetPrice = lNetPrice;};
    virtual long GetNetPrice() {return m_lNetPrice;};

    virtual void SetRescanItem(long lRescanItem) {m_lRescanItem = lRescanItem;};
    virtual long GetRescanItem() {return m_lRescanItem;};

    virtual void SetQty(long lQty) {m_lQty = lQty;};
    virtual long GetQty() {return m_lQty;};

protected:
    CString m_cDMLastData;
    CString m_csDMLastBarLabel;
    CString m_csDMLastBarCode;
    CString m_csDescription;
    long m_lBarcodeType;
    CString m_EntryID;
    bool m_bScannedFromMobile;
    bool m_bIsLoyaltyID;
    long m_lItemProperty;
    long m_lOrigin;
    bool m_bAgeRestricted;
    long m_lRescanItem;
    long m_lNetPrice;
    long m_lQty;
};

#endif // _CQUEUED_ACTION_RECALL_ITEM
