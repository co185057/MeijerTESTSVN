// CustomerMHReceipt.h: interface for the CCustomerMHReceipt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CustomerMHRECEIPT_H__INCLUDED_)
#define AFX_CustomerMHRECEIPT_H__INCLUDED_

#include "SolutionMHReceipt.h"
#include "SingleInstance.h"

class CCustomerMHReceipt : public CSolutionMHReceipt, public CSingleInstance<CCustomerMHReceipt>
{
    friend class CSingleInstance<CCustomerMHReceipt>;

public:
	virtual ~CCustomerMHReceipt();
    TBSTATE Process_Bitmap( CPrinterMsg* pMsg );

protected:
	CCustomerMHReceipt();

private:
    CCustomerMHReceipt(CCustomerMHReceipt &);                    // hide copy const
    CCustomerMHReceipt & operator =(const CCustomerMHReceipt &); // hide assignment
};

#endif 
