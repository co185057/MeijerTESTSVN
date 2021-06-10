// CustomerCurrentTotals.h: interface for the CCustomerCurrentTotals class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentTotals_H__INCLUDED_)
#define AFX_CustomerCurrentTotals_H__INCLUDED_

#include "BaseCurrentTotals.h"
#include "SingleInstance.h"

class CCustomerCurrentTotals : public CBaseCurrentTotals,
                                public CSingleInstance<CCustomerCurrentTotals>
{
    friend class CSingleInstance<CCustomerCurrentTotals>;

public:
    virtual ~CCustomerCurrentTotals();

protected:
    CCustomerCurrentTotals();

private:
    CCustomerCurrentTotals(CCustomerCurrentTotals &);                    // hide copy const
    CCustomerCurrentTotals & operator =(const CCustomerCurrentTotals &); // hide assignment
};

#endif 
