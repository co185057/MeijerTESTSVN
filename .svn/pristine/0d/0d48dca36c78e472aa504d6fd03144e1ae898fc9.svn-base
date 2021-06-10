// CustomerMHSlip.h: interface for the CCustomerMHSlip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerSLIPDATA_H__INCLUDED_)
#define		 AFX_CustomerSLIPDATA_H__INCLUDED_

#include "SolutionMHSlip.h"
#include "SingleInstance.h"

class CCustomerMHSlip : public CSolutionMHSlip, public CSingleInstance<CCustomerMHSlip>
{
    friend class CSingleInstance<CCustomerMHSlip>;

public:
	virtual ~CCustomerMHSlip();

protected:
	CCustomerMHSlip();

private:
    CCustomerMHSlip(CCustomerMHSlip &);                    // hide copy const
    CCustomerMHSlip & operator =(const CCustomerMHSlip &); // hide assignment
};

#endif 
