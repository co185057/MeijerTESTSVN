#ifndef _CQUEUED_ACTION_RESCAN_ITEM
#define _CQUEUED_ACTION_RESCAN_ITEM

#include "QueuedActionRecallItem.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#elif defined _SCOT
class DLLIMPORT
#else
class 
#endif
 CQueuedActionRescanItem : public CQueuedActionRecallItem
{
public:
    CQueuedActionRescanItem(CString csID, 
                           CString csDMLastBarLabel, 
                           CString csDMLastBarCode, 
                           CString csResponseItemCode,
                           CString csResponseErrorString,
                           long lBarcodeType);
    virtual ~CQueuedActionRescanItem();

    virtual void SetResponseItemCode(CString csResponseItemCode) {m_csResponseItemCode = csResponseItemCode;};
    virtual CString GetResponseItemCode() {return m_csResponseItemCode;} ;
    virtual void SetResponseErrorString(CString csResponseErrorString) {m_csResponseErrorString = csResponseErrorString;};
    virtual CString GetResponseErrorString() {return m_csResponseErrorString;};
    virtual void SetAddItemToTransaction(bool bAddItemToTransaction) {m_bAddItemToTransaction = bAddItemToTransaction;};
 

protected:
    CString m_csResponseItemCode;
    CString m_csResponseErrorString;
    bool m_bAddItemToTransaction;
};

#endif // _CQUEUED_ACTION_RESCAN_ITEM
