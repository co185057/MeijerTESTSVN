// CustomerCurrentFiscalPrinter.h: interface for the CCustomerCurrentFiscalPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentFiscalPrinter_H__INCLUDED_)
#define AFX_CustomerCurrentFiscalPrinter_H__INCLUDED_

#include "BaseCurrentFiscalPrinter.h"
#include "SingleInstance.h"

class CCustomerCurrentFiscalPrinter : public CBaseCurrentFiscalPrinter,
                                public CSingleInstance<CCustomerCurrentFiscalPrinter>
{
    friend class CSingleInstance<CCustomerCurrentFiscalPrinter>;

public:
    virtual ~CCustomerCurrentFiscalPrinter();

protected:
    CCustomerCurrentFiscalPrinter();

private:
    CCustomerCurrentFiscalPrinter(CCustomerCurrentFiscalPrinter &);                    // hide copy const
    CCustomerCurrentFiscalPrinter & operator =(const CCustomerCurrentFiscalPrinter &); // hide assignment
};

#endif 
