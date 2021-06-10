// CustomerMHTotals.h: interface for the CCustomerMHTotals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHTOTALS_H__INCLUDED_)
#define AFX_CustomerMHTOTALS_H__INCLUDED_

#include "SolutionMHTotals.h"
#include "SingleInstance.h"

class CCustomerMHTotals : public CSolutionMHTotals, public CSingleInstance<CCustomerMHTotals>
{
    friend class CSingleInstance<CCustomerMHTotals>;

public:
    virtual ~CCustomerMHTotals();
	TBSTATE ProcessMessage(CTBMsgObj *pMsgObj);

protected:
	CCustomerMHTotals();

private:
    CCustomerMHTotals(const CCustomerMHTotals &);              // hide copy const
    CCustomerMHTotals & operator =(const CCustomerMHTotals &); // hide assignment
};

#endif 
