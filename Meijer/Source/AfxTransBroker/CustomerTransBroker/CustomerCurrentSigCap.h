// CustomerCurrentSigCap.h: interface for the CCustomerCurrentSigCap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerCurrentSigCap_H__INCLUDED_)
#define AFX_CustomerCurrentSigCap_H__INCLUDED_

#include "SolutionCurrentSigCap.h"
#include "SingleInstance.h"

class CCustomerCurrentSigCap : public CSolutionCurrentSigCap,
                                public CSingleInstance<CCustomerCurrentSigCap>
{
    friend class CSingleInstance<CCustomerCurrentSigCap>;

public:
    virtual ~CCustomerCurrentSigCap();
    TBRC PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData);

protected:
    CCustomerCurrentSigCap();

private:
    CCustomerCurrentSigCap(CCustomerCurrentSigCap &);                    // hide copy const
    CCustomerCurrentSigCap & operator =(const CCustomerCurrentSigCap &); // hide assignment
};

#endif 
