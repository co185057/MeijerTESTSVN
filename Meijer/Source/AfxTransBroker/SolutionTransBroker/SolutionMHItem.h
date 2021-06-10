// SolutionMHItem.h: interface for the CSolutionMHItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionMITEM_H__INCLUDED_)
#define		 AFX_SolutionMITEM_H__INCLUDED_

#include "BaseMHItem.h"
#include "TBGenMsg.h"

class CSolutionMHItem : public CBaseMHItem
{
    
public:
    virtual ~CSolutionMHItem();

    virtual TBSTATE HandleItemKeyed(LPARAM lparam);
    virtual TBSTATE HandleItemScan(LPARAM lparam);
    virtual TBSTATE HandleItemWeight(LPARAM lparam);
    virtual TBSTATE HandleItemQty(LPARAM lparam);
    virtual TBSTATE HandleItemVoid(LPARAM lparam);
    virtual TBSTATE ProcessMessage(CTBMsgObj *pMsg);
	virtual void SetSpecialHandlingItemList(CString strSpecialItem, CString &csVar);
	virtual bool IsItemACoupon(const int nItemType);
	virtual TBSTATE ProcessRewardMessage(CTBGenMsg *pMsg);
	virtual CString ExtractRewardDesc(CTBGenMsg *pMsg, int nContextNum);
	virtual bool ComputeRewardValueOrPoints(CTBGenMsg *pMsg, int nContextNum, CRewardLineDetailsMsg  *sRewardDetails);
	virtual bool IsMonetaryReward(const int nRewardType);
	virtual bool IsPointReward(const int nRewardType);
	virtual bool IsRewardVoided(CTBGenMsg *pMsg, int nContextNum);
    virtual TBRC SendWeight(long lWeight);
	virtual int  GetNumberOfItemsSold() { return m_nNumberOfItemsSold; }
	virtual void ResetNumberOfItemsSold() { m_nNumberOfItemsSold = 0; }
    virtual TBRC EnterItem(LPCTSTR szItemCode, const BOOL fVoid, const BOOL fOverride, 
                           const long nQuantity, const long nWeight, const long nTareCode,
                           const long lPrice, const BOOL fQuantityConfirmed, 
                           PSEnterItemDetails psMoreDetails);
    virtual TBRC EnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge);

    CString m_csPrevItemCode;

protected:
    CSolutionMHItem();
    
    
private:
    CSolutionMHItem(const CSolutionMHItem &);                    // hide copy const
    CSolutionMHItem & operator =(const CSolutionMHItem &); // hide assignment

	CString m_csCouponTypeList;
	CString m_csMonetaryRewardList;
	CString m_csPointRewardList;
	int m_nNumberOfItemsSold;
    
    
};

#endif 
