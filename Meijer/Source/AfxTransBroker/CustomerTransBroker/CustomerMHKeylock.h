// CustomerMHKeylock.h: interface for the CCustomerMHKeylock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHKEYLOCK_H__INCLUDED_)
#define AFX_CustomerMHKEYLOCK_H__INCLUDED_

#include "BaseMHKeylock.h"
#include "SingleInstance.h"

class CCustomerMHKeylock : public CBaseMHKeylock, public CSingleInstance<CCustomerMHKeylock>
{
    friend class CSingleInstance<CCustomerMHKeylock>;

public:
	virtual ~CCustomerMHKeylock();

protected:
   	CCustomerMHKeylock();

private:
    CCustomerMHKeylock(CCustomerMHKeylock &);                    // hide copy const
    CCustomerMHKeylock & operator =(const CCustomerMHKeylock &); // hide assignment

};

#endif 
