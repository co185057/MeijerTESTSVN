// CustomerMHTransaction.h: interface for the CCustomerMHTransaction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHTRANSACTION_H__INCLUDED_)
#define AFX_CustomerMHTRANSACTION_H__INCLUDED_

#include "SolutionMHTransaction.h"
#include "SingleInstance.h"

class CCustomerMHTransaction : public CSolutionMHTransaction, 
                               public CSingleInstance<CCustomerMHTransaction>
{
    friend class CSingleInstance<CCustomerMHTransaction>;

public:
	virtual ~CCustomerMHTransaction();
    virtual TBRC Start(LPCTSTR szOverrideCode);
    virtual TBRC Finish();
	TBRC SuspendTransaction(const BOOL fOverride);


protected:
	CCustomerMHTransaction();

private:
    CCustomerMHTransaction(const CCustomerMHTransaction &);                    // hide copy const
    CCustomerMHTransaction & operator =(const CCustomerMHTransaction &); // hide assignment

};

#endif 
