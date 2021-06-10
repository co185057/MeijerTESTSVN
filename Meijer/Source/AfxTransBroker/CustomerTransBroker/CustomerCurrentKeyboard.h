// CustomerCurrentKeyboard.h: interface for the CCustomerCurrentKeyboard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentKeyboard_H__INCLUDED_)
#define AFX_CustomerCurrentKeyboard_H__INCLUDED_

#include "BaseCurrentKeyboard.h"
#include "SingleInstance.h"

class CCustomerCurrentKeyboard : public CBaseCurrentKeyboard,
                                public CSingleInstance<CCustomerCurrentKeyboard>
{
    friend class CSingleInstance<CCustomerCurrentKeyboard>;

public:
    virtual ~CCustomerCurrentKeyboard();

protected:
    CCustomerCurrentKeyboard();

private:
    CCustomerCurrentKeyboard(CCustomerCurrentKeyboard &);                    // hide copy const
    CCustomerCurrentKeyboard & operator =(const CCustomerCurrentKeyboard &); // hide assignment
};

#endif 
