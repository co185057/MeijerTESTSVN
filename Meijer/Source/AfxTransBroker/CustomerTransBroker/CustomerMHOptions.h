// CustomerMHOptions.h: interface for the CCustomerMHOptions class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CustomerMHOptions_H__INCLUDED_)
#define AFX_CustomerMHOptions_H__INCLUDED_

#include "SolutionMHOptions.h"
#include "CustomerKeyDefines.h"
#include "SingleInstance.h"

class CCustomerMHOptions  : public CSolutionMHOptions, 
                             public CSingleInstance<CCustomerMHOptions>
{
    friend class CSingleInstance<CCustomerMHOptions>;

public:
	virtual ~CCustomerMHOptions();

protected:
	CCustomerMHOptions();

private:
    CCustomerMHOptions(CCustomerMHOptions &);                    // hide copy const
    CCustomerMHOptions & operator =(const CCustomerMHOptions &); // hide assignment
};

#endif 
