// CustomerCurrentTransaction.h: interface for the CCustomerCurrentTransaction class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CustomerCurrentTransaction_H__INCLUDED_)
#define AFX_CustomerCurrentTransaction_H__INCLUDED_

#include "BaseCurrentTransaction.h"
#include "SingleInstance.h"

class CCustomerCurrentTransaction : public CBaseCurrentTransaction,
                                    public CSingleInstance<CCustomerCurrentTransaction>
{
    friend class CSingleInstance<CCustomerCurrentTransaction>;

public:
    virtual ~CCustomerCurrentTransaction();
	void ResetTransactionDetails(void);

    CString InsertedDiscount() { return m_csInsertedDiscount; }
	void InsertedDiscount(CString csInsertedDiscount);

	BOOL DetectAndUpdateRebateItemAttributeChange(void);
	BOOL IsCurrentItemWithRebateReceipt(void);
    int  CountItemsWithRebateReceipts(void);
    int  CountTotalRebateReceipts(void);
	
	typedef struct _RECEIPTITEMINFO
	{
		CString strGiftRebateInd;
		CString strRebateReceiptNums;
	} RECEIPTITEMINFO, * PRECEIPTITEMINFO;

protected:
    CCustomerCurrentTransaction();  // should not be called outside of this class (instance function)
    

private:
	CMap<int,int, PRECEIPTITEMINFO, PRECEIPTITEMINFO> m_rcptItems;
	void ClearReceiptItems(void);
    CString m_csInsertedDiscount;

    CCustomerCurrentTransaction(const CCustomerCurrentTransaction &);                    // hide copy const
    CCustomerCurrentTransaction & operator =(const CCustomerCurrentTransaction &); // hide assignment
};

#endif // !defined(AFX_CustomerCurrentTransaction_H__INCLUDED_)

