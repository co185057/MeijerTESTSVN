// CustomerCurrentCashDrawer.h: interface for the CCustomerCurrentCashDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentCashDrawer_H__INCLUDED_)
#define AFX_CustomerCurrentCashDrawer_H__INCLUDED_

#include "BaseCurrentCashDrawer.h"
#include "SingleInstance.h"

class CCustomerCurrentCashDrawer : public CBaseCurrentCashDrawer,
                                   public CSingleInstance<CCustomerCurrentCashDrawer>
{
    friend class CSingleInstance<CCustomerCurrentCashDrawer>;

public:
    virtual ~CCustomerCurrentCashDrawer();

protected:
    CCustomerCurrentCashDrawer();

private:
    CCustomerCurrentCashDrawer(CCustomerCurrentCashDrawer &);                    // hide copy const
    CCustomerCurrentCashDrawer & operator =(const CCustomerCurrentCashDrawer &); // hide assignment
};

#endif 
