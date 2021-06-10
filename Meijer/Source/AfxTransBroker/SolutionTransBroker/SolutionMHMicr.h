// SolutionMHMicr.h: interface for the CSolutionMHMicr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionMHMICR_H__INCLUDED_)
#define AFX_SolutionMHMICR_H__INCLUDED_

#include "BaseMHMicr.h"

class CSolutionMHMicr : public CBaseMHMicr
{
public:
    virtual ~CSolutionMHMicr();
    virtual TBRC EnterMICR( LPCTSTR szRawData, LPCTSTR szTransitNumber, LPCTSTR szAccountNumber, 
        LPCTSTR szBankNumber, LPCTSTR szSerialNumber, LPCTSTR szEPC, 
        long lType, long lCountryCode);
    
    virtual TBRC EnterMICRWithType( TBTENDERTYPE nTenderType, LPCTSTR szRawData, LPCTSTR szTransitNumber, LPCTSTR szAccountNumber, 
        LPCTSTR szBankNumber, LPCTSTR szSerialNumber, LPCTSTR szEPC, 
        long lType, long lCountryCode);
    
protected:
    CSolutionMHMicr();
    
private:
    CSolutionMHMicr(CSolutionMHMicr &);                    // hide copy const
    CSolutionMHMicr & operator =(const CSolutionMHMicr &); // hide assignment
};

#endif 
