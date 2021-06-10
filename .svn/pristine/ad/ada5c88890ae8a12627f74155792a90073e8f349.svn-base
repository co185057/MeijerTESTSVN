// CustomerCurrentOptions.h: interface for the CCustomerCurrentOptions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentOptions_H__INCLUDED_)
#define AFX_CustomerCurrentOptions_H__INCLUDED_

#include "BaseCurrentOptions.h"
#include "SingleInstance.h"

class CCustomerCurrentOptions : public CBaseCurrentOptions,
                                public CSingleInstance<CCustomerCurrentOptions>
{
    friend class CSingleInstance<CCustomerCurrentOptions>;

public:
    virtual ~CCustomerCurrentOptions();

protected:
    CCustomerCurrentOptions();

private:
    CCustomerCurrentOptions(CCustomerCurrentOptions &);                    // hide copy const
    CCustomerCurrentOptions & operator =(const CCustomerCurrentOptions &); // hide assignment
};

#endif 
