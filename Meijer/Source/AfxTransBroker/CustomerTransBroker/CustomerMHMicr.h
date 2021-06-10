// CustomerMHMicr.h: interface for the CCustomerMHMicr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHMICR_H__INCLUDED_)
#define AFX_CustomerMHMICR_H__INCLUDED_

#include "SolutionMHMicr.h"
#include "SingleInstance.h"

class CCustomerMHMicr : public CSolutionMHMicr, public CSingleInstance<CCustomerMHMicr>
{
    friend class CSingleInstance<CCustomerMHMicr>;
    
public:
    virtual ~CCustomerMHMicr();

    virtual TBRC EnterMICR( LPCTSTR szRawData, LPCTSTR szTransitNumber, LPCTSTR szAccountNumber, 
        LPCTSTR szBankNumber, LPCTSTR szSerialNumber, LPCTSTR szEPC, 
        long lType, long lCountryCode);
   
protected:
    CCustomerMHMicr();
    
private:
    CCustomerMHMicr(CCustomerMHMicr &);                    // hide copy const
    CCustomerMHMicr & operator =(const CCustomerMHMicr &); // hide assignment
};

#endif 
