// CustomerMHMSR.h: interface for the CCustomerMHMSR class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CustomerMHMSR_H__INCLUDED_)
#define AFX_CustomerMHMSR_H__INCLUDED_

#include "BaseMHMSR.h"
#include "CustomerKeyDefines.h"
#include "SingleInstance.h"

class CCustomerMHMSR  : public CBaseMHMSR, 
                             public CSingleInstance<CCustomerMHMSR>
{
    friend class CSingleInstance<CCustomerMHMSR>;

public:
	virtual ~CCustomerMHMSR();

protected:
	CCustomerMHMSR();

private:
    CCustomerMHMSR(CCustomerMHMSR &);                    // hide copy const
    CCustomerMHMSR & operator =(const CCustomerMHMSR &); // hide assignment
};

#endif 
