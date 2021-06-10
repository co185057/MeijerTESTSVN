// CustomerMHItem.h: interface for the CCustomerMHItem class.
//
// POS277295 Work Request:73595 Name:Alexander Bronola/Veerasekhara D 	Date:August 12, 2016
// POS242535 Work Request:68942 Name:Saiprasad Srihasam    Date:April 05, 2016
// POS149568 Work Request:66559 Name:Saiprasad Srihasam    Date:December 18, 2014
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMITEM_H__INCLUDED_)
#define		 AFX_CustomerMITEM_H__INCLUDED_

#include "SolutionMHItem.h"
#include "SingleInstance.h"

class CCustomerMHItem : public CSolutionMHItem, public CSingleInstance<CCustomerMHItem>
{
    friend class CSingleInstance<CCustomerMHItem>;
    
public:
    virtual ~CCustomerMHItem();
    virtual TBRC GetItemDetails(PSItemDetails psDetails);
    virtual TBRC GetItemExtendedDetails(PSItemExtendedDetails psDetails);

    virtual TBSTATE HandleItemKeyed(LPARAM lparam);
    virtual TBSTATE HandleItemScan(LPARAM lparam);
    virtual TBSTATE HandleItemWeight(LPARAM lparam);
    virtual TBSTATE HandleItemQty(LPARAM lparam);
    virtual TBSTATE HandleItemVoid(LPARAM lparam);
	TBSTATE ProcessMessage(CTBMsgObj *pMsg);
    TBSTATE HandleItemEntry(LPARAM lparam);
    TBSTATE ProcessRewardMessage(CTBGenMsg *pMsg);
	TBSTATE ProcessMPerksRewardLine(CTBGenMsg *pMsg); //POS277295 
	void HandleRebateItemAttributeChange(BOOL bAddRebateReceiptLine);
	void RemoveReceiptCMDiscount(long lRewardID, long lRefID);
    void RemovePrevWICTotal(long);  //POS149568
    void AddReceiptCMDiscount(CString csReward, long lRewardID, long lRefID);

    virtual TBRC EnterItem(LPCTSTR szItemCode, const BOOL fVoid, const BOOL fOverride, 
                           const long nQuantity, const long nWeight, const long nTareCode,
                           const long lPrice, const BOOL fQuantityConfirmed, 
                           PSEnterItemDetails psMoreDetails);
	virtual bool IsItemGreenDotBarcodes(LPCTSTR str);
	virtual void BuildGreenDotDataNeeded(LPCTSTR strItem);

    virtual bool IsItemPicklistLightWeightItem(LPCTSTR str);  // POS242535

    // SOTF 8380 Order 2 store - RRS
    virtual bool IsItemOrder2StoreBarcodes(LPCTSTR str, LONG lBarcodeType);	
    // E SOTF 8380 - Order 2 store - RRS
    
    virtual TBRC EnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge);
    virtual bool IsItemWithNoItemDetailsExpected(CString sCode); // SOTF5803

	//SOTF 6226
	virtual bool GetACSDelayedAgeRestricted(void)
	{
		return m_bACSDelayedAgeRestricted;
	}

    virtual long GetHighestAgeRequired(void)
    {
        return m_lHighestAgeRequired;
    }

    virtual void ResetDelayedAgeRequiredTLOGData(void);
    virtual void SetAgeRestrictionItemAllowedStatus(long l)
    {
        m_lAgeRequiredItemAllowed = l;
    }
    virtual void SetAgeRestrictionEntry(long l)
    {
        m_lAgeRestrictionEntry = l;
    }

    virtual void SetHighestAgeRequired(long l)
    {
        m_lHighestAgeRequired = l;
    }

   
    virtual CString GetUPCAgeRequired(void)
    {
        return m_csUPCAgeRequired;
    }

    virtual void SetUPCAgeRequired(CString csTemp)
    {
        m_csUPCAgeRequired = csTemp;
    }
    void GetAgeVerifyData(int &ageEntry, CString *csCustomerDOB, CString *csUPC, int &ageRequired, int &itemAllowed, int &nCustomerAge);
    //-sotf

    //SOTF 7261
    virtual CString GetCustomerDOB(void)
    {
        return m_csHighestCustomerDOB;
    }

    static long m_nRewardLineEntryID;   // POS149568

	virtual bool msgEASItem();	//CRD 232268
	virtual bool msgMobileGenericDelayed();
	static long m_lEntryIDtemp;
	static bool isFLMSTransferring;

	virtual CString ModifyBarcode(CString csItemCode, long lScanCodeType);	//CRD 328873
	
protected:
    CCustomerMHItem();
    
private:
    CCustomerMHItem(const CCustomerMHItem &);                    // hide copy const
    CCustomerMHItem & operator =(const CCustomerMHItem &); // hide assignment

	BOOL DetectIsBogusItem(void);

    // generate unique reward IDs when POS does not provide RewardID 
    // not used for lookup, counter to generate unique number only.
    long m_lFLRewardID; 
	CString m_csGreenDotBarcodes;
    std::vector<CString> m_VecGreenDotBarcodesList;
    CString m_csPiclistLightWeightItems;	// POS242535
    std::vector<CString> m_VecPicklistLightWeightList;	// POS242535


	//SOTF 6226
	bool m_bACSDelayedAgeRestricted;
    long m_lHighestAgeRequired;
    CString m_csCustomerDOB;
    CString m_csUPCAgeRequired;
    long m_lUPCAge;
    long m_lAgeRequiredItemAllowed;
    long m_lAgeRestrictionEntry;
    long m_lCustomerAge;
    //-sotf
    
    //SOTF 7261
    CString m_csHighestCustomerDOB;
    long m_lHighestCustomerAge;
    //sotf

    
};

#endif 
