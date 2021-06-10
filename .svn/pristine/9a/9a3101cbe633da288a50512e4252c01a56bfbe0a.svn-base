// CustomerTBConnectFL.h: interface for the CCustomerTBConnectFL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerTBConnectFL_H__INCLUDED_)
#define AFX_CustomerTBConnectFL_H__INCLUDED_

#include "BaseTBConnectFL.h"
#include "SingleInstance.h"

class CCustomerTBConnectFL : public CBaseTBConnectFL,
                                public CSingleInstance<CCustomerTBConnectFL>
{
    friend class CSingleInstance<CCustomerTBConnectFL>;

public:
    virtual ~CCustomerTBConnectFL();
    virtual TBRC GetMessageInfo(PSMessageInfo psMessageInfo);

protected:
    CCustomerTBConnectFL();

private:
    CCustomerTBConnectFL(const CCustomerTBConnectFL &);                    // hide copy const
    CCustomerTBConnectFL & operator =(const CCustomerTBConnectFL &); // hide assignment
};

#endif 
