// CustomerMHKeyboard.h: interface for the CCustomerMHKeyboard class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CustomerMHKeyboard_H__INCLUDED_)
#define AFX_CustomerMHKeyboard_H__INCLUDED_

#include "BaseMHKeyboard.h"
#include "CustomerKeyDefines.h"
#include "SingleInstance.h"

class CCustomerMHKeyboard  : public CBaseMHKeyboard, 
                             public CSingleInstance<CCustomerMHKeyboard>
{
    friend class CSingleInstance<CCustomerMHKeyboard>;

public:
	virtual ~CCustomerMHKeyboard();

protected:
	CCustomerMHKeyboard();

private:
    CCustomerMHKeyboard(CCustomerMHKeyboard &);                    // hide copy const
    CCustomerMHKeyboard & operator =(const CCustomerMHKeyboard &); // hide assignment
};

#endif 
