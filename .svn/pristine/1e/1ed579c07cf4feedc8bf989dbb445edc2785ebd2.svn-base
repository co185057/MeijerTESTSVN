// CustomerCurrentTender.h: interface for the CCustomerCurrentTender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentTender_H__INCLUDED_)
#define AFX_CustomerCurrentTender_H__INCLUDED_

#include "BaseCurrentTender.h"
#include "SingleInstance.h"

#define INVALID_BALANCE 0x7FFFFFFF // largest +ve value for a signed long

class CCustomerCurrentTender : public CBaseCurrentTender,
                                public CSingleInstance<CCustomerCurrentTender>
{
    friend class CSingleInstance<CCustomerCurrentTender>;

public:
    virtual ~CCustomerCurrentTender();
    void Invalidate();
    bool IsValid();

protected:
    CCustomerCurrentTender();

private:
    CCustomerCurrentTender(CCustomerCurrentTender &);                    // hide copy const
    CCustomerCurrentTender & operator =(const CCustomerCurrentTender &); // hide assignment
};

#endif 
