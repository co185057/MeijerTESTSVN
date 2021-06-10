// CustomerCurrentPINPad.h: interface for the CCustomerCurrentPINPad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentPINPad_H__INCLUDED_)
#define AFX_CustomerCurrentPINPad_H__INCLUDED_

#include "BaseCurrentPINPad.h"
#include "SingleInstance.h"

class CCustomerCurrentPINPad : public CBaseCurrentPINPad,
                                public CSingleInstance<CCustomerCurrentPINPad>
{
    friend class CSingleInstance<CCustomerCurrentPINPad>;

public:
    virtual ~CCustomerCurrentPINPad();

protected:
    CCustomerCurrentPINPad();

private:
    CCustomerCurrentPINPad(CCustomerCurrentPINPad &);                    // hide copy const
    CCustomerCurrentPINPad & operator =(const CCustomerCurrentPINPad &); // hide assignment
};

#endif 
