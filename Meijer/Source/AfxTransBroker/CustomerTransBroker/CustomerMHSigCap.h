// CustomerMHSigCap.h: interface for the CCustomerMHSigCap class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CustomerMHSigCap_H__INCLUDED_)
#define AFX_CustomerMHSigCap_H__INCLUDED_

#include "BaseMHSigCap.h"
#include "CustomerKeyDefines.h"
#include "SingleInstance.h"

class CCustomerMHSigCap  : public CBaseMHSigCap, 
                             public CSingleInstance<CCustomerMHSigCap>
{
    friend class CSingleInstance<CCustomerMHSigCap>;

public:
	virtual ~CCustomerMHSigCap();

protected:
	CCustomerMHSigCap();

private:
    CCustomerMHSigCap(CCustomerMHSigCap &);                    // hide copy const
    CCustomerMHSigCap & operator =(const CCustomerMHSigCap &); // hide assignment
};

#endif 
