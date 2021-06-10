// CustomerCurrentReceipt.h: interface for the CCustomerCurrentReceipt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentReceipt_H__INCLUDED_)
#define AFX_CustomerCurrentReceipt_H__INCLUDED_

#include "SolutionCurrentReceipt.h"
#include "SingleInstance.h"

class CCustomerCurrentReceipt : public CSolutionCurrentReceipt,
                                public CSingleInstance<CCustomerCurrentReceipt>
{
    friend class CSingleInstance<CCustomerCurrentReceipt>;

public:
    virtual ~CCustomerCurrentReceipt();
    virtual bool FinishReceipt( bool bDiscard = false );
    virtual bool WriteToReceipt( CString csReceiptData );
    virtual void CheckForStoreReceipt( CString csData );  //SOTF 7612
    virtual HFILE GetReceiptData(void);                   //SOTF 8362
    virtual TBRC ReleaseReceiptData(HFILE hReceiptData);  //SOTF 8362
    //virtual bool PurgeAllReceipts( bool bRemoveFiles );   //SOTF 8362

protected:
    CCustomerCurrentReceipt();

private:
    CCustomerCurrentReceipt(CCustomerCurrentReceipt &);                    // hide copy const
    CCustomerCurrentReceipt & operator =(const CCustomerCurrentReceipt &); // hide assignment

    CFile* m_pMainReceiptFile;
    bool m_bGiftReceipt;
    CFile* m_pLastReceiptPrinted; //SOTF 8362 
    
};

#endif 
