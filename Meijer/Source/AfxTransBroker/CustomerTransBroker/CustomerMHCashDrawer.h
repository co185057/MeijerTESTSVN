// CustomerMHCashDrawer.h: interface for the CCustomerMHCashDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHCashDrawer_H__INCLUDED_)
#define AFX_CustomerMHCashDrawer_H__INCLUDED_

#include "SolutionMHCashDrawer.h"
#include "SingleInstance.h"

class CCustomerMHCashDrawer : public CSolutionMHCashDrawer, 
                              public CSingleInstance<CCustomerMHCashDrawer>
{
    friend class CSingleInstance<CCustomerMHCashDrawer>;

public:
	virtual TBSTATE ProcessMessage(CTBMsgObj *pMsg);
    virtual ~CCustomerMHCashDrawer();

protected:
    CCustomerMHCashDrawer();

private:
    CCustomerMHCashDrawer(CCustomerMHCashDrawer &);                    // hide copy const
    CCustomerMHCashDrawer & operator =(const CCustomerMHCashDrawer &); // hide assignment

};

#endif 
