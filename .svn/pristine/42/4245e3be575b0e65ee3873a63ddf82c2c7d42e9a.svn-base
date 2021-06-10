// CustomerCurrentKeylock.h: interface for the CCustomerCurrentKeylock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentKeylock_H__INCLUDED_)
#define AFX_CustomerCurrentKeylock_H__INCLUDED_

#include "BaseCurrentKeylock.h"
#include "SingleInstance.h"

class CCustomerCurrentKeylock : public CBaseCurrentKeylock,
                                public CSingleInstance<CCustomerCurrentKeylock>
{
    friend class CSingleInstance<CCustomerCurrentKeylock>;

public:
    virtual ~CCustomerCurrentKeylock();

protected:
    CCustomerCurrentKeylock();

private:
    CCustomerCurrentKeylock(CCustomerCurrentKeylock &);                    // hide copy const
    CCustomerCurrentKeylock & operator =(const CCustomerCurrentKeylock &); // hide assignment
};

#endif 
