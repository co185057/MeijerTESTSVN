// CustomerCurrentMicr.h: interface for the CCustomerCurrentMicr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentMicr_H__INCLUDED_)
#define AFX_CustomerCurrentMicr_H__INCLUDED_

#include "BaseCurrentMicr.h"
#include "SingleInstance.h"

class CCustomerCurrentMicr : public CBaseCurrentMicr,
                                public CSingleInstance<CCustomerCurrentMicr>
{
    friend class CSingleInstance<CCustomerCurrentMicr>;

public:
    virtual ~CCustomerCurrentMicr();

protected:
    CCustomerCurrentMicr();

private:
    CCustomerCurrentMicr(CCustomerCurrentMicr &);                    // hide copy const
    CCustomerCurrentMicr & operator =(const CCustomerCurrentMicr &); // hide assignment
};

#endif 
