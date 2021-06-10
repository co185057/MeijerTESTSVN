// CustomerCurrentOperator.h: interface for the CCustomerCurrentOperator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentOperator_H__INCLUDED_)
#define AFX_CustomerCurrentOperator_H__INCLUDED_

#include "BaseCurrentOperator.h"
#include "SingleInstance.h"

class CCustomerCurrentOperator : public CBaseCurrentOperator,
                                public CSingleInstance<CCustomerCurrentOperator>
{
    friend class CSingleInstance<CCustomerCurrentOperator>;

public:
    virtual ~CCustomerCurrentOperator();

protected:
    CCustomerCurrentOperator();

private:
    CCustomerCurrentOperator(CCustomerCurrentOperator &);                    // hide copy const
    CCustomerCurrentOperator & operator =(const CCustomerCurrentOperator &); // hide assignment
};

#endif 
