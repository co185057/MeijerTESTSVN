// CustomerMHPINPad.h: interface for the CCustomerMHPINPad class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CustomerMHPINPad_H__INCLUDED_)
#define AFX_CustomerMHPINPad_H__INCLUDED_

#include "BaseMHPINPad.h"
#include "CustomerKeyDefines.h"
#include "SingleInstance.h"

class CCustomerMHPINPad  : public CBaseMHPINPad, 
                             public CSingleInstance<CCustomerMHPINPad>
{
    friend class CSingleInstance<CCustomerMHPINPad>;

public:
	virtual ~CCustomerMHPINPad();

protected:
	CCustomerMHPINPad();

private:
    CCustomerMHPINPad(CCustomerMHPINPad &);                    // hide copy const
    CCustomerMHPINPad & operator =(const CCustomerMHPINPad &); // hide assignment
};

#endif 
