// CustomerCurrentMSR.h: interface for the CCustomerCurrentMSR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentMSR_H__INCLUDED_)
#define AFX_CustomerCurrentMSR_H__INCLUDED_

#include "BaseCurrentMSR.h"
#include "SingleInstance.h"

class CCustomerCurrentMSR : public CBaseCurrentMSR,
                                public CSingleInstance<CCustomerCurrentMSR>
{
    friend class CSingleInstance<CCustomerCurrentMSR>;

public:
    virtual ~CCustomerCurrentMSR();

protected:
    CCustomerCurrentMSR();

private:
    CCustomerCurrentMSR(CCustomerCurrentMSR &);                    // hide copy const
    CCustomerCurrentMSR & operator =(const CCustomerCurrentMSR &); // hide assignment
};

#endif 
