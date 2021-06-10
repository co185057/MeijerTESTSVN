// CustomerCurrentPrinter.h: interface for the CCustomerCurrentPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentPrinter_H__INCLUDED_)
#define AFX_CustomerCurrentPrinter_H__INCLUDED_

#include "BaseCurrentPrinter.h"
#include "SingleInstance.h"

class CCustomerCurrentPrinter : public CBaseCurrentPrinter,
                                public CSingleInstance<CCustomerCurrentPrinter>
{
    friend class CSingleInstance<CCustomerCurrentPrinter>;

public:
    virtual ~CCustomerCurrentPrinter();

protected:
    CCustomerCurrentPrinter();

private:
    CCustomerCurrentPrinter(CCustomerCurrentPrinter &);                    // hide copy const
    CCustomerCurrentPrinter & operator =(const CCustomerCurrentPrinter &); // hide assignment
};

#endif 
