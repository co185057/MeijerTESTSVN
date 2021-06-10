// CustomerMHPrinter.h: interface for the CCustomerMHPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHPRINTER_H__INCLUDED_)
#define AFX_CustomerMHPRINTER_H__INCLUDED_

#include "BaseMHPrinter.h"
#include "SingleInstance.h"

class CCustomerMHPrinter : public CBaseMHPrinter, public CSingleInstance<CCustomerMHPrinter>
{
    friend class CSingleInstance<CCustomerMHPrinter>;

public:
    virtual ~CCustomerMHPrinter();
    
protected:
    CCustomerMHPrinter();
    
private:
    CCustomerMHPrinter(CCustomerMHPrinter &);                    // hide copy const
    CCustomerMHPrinter & operator =(const CCustomerMHPrinter &); // hide assignment
};

#endif 
