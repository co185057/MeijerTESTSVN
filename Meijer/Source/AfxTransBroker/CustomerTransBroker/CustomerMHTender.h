// CustomerMHTender.h: interface for the CCustomerMHTender class.
//
// POS36095/POS42532 Work Request:52635 Name:Robert Susanto Date:October 22, 2012
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_CustomerMHTender_H__INCLUDED_)
#define AFX_CustomerMHTender_H__INCLUDED_

#include "SolutionMHTender.h"
#include "SingleInstance.h"

class CCustomerMHTender : public CSolutionMHTender, public CSingleInstance<CCustomerMHTender>
{
    friend class CSingleInstance<CCustomerMHTender>;

public:
    virtual ~CCustomerMHTender();
    TBSTATE HandleMessage300(bool bInItemization = false);
	virtual void HandleTenderAccepted(void);
	virtual TBRC EnterTenderState( const bool bEnter);   // TAR377139
    virtual TBRC GetTenderDetails(PSTenderDetails psTenderDetails);

    ///	POS42532
	//////////////////////////////////////////////////////////////////////
    virtual TBRC EnterTender( TBTENDERTYPE nTenderType, LONG lAmount, LPCTSTR szCardData,
                              int nPINLength, LPCTSTR szEncryptedPIN, BOOL fOverride, BOOL fVoid);

protected:
    CCustomerMHTender();  // should not be called outside of this class (instance function)
	int FindResponseCode(const CString &csResponseCode, TBTENDERTYPE tendertype);
    virtual TBSTATE ProcessMessage(CTBMsgObj *pMsgObj);
    
private:

    CCustomerMHTender(const CCustomerMHTender &);              // hide copy const
    CCustomerMHTender & operator =(const CCustomerMHTender &); // hide assignment
    
};

#endif 

