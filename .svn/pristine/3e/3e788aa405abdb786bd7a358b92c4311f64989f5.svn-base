// CustomerMHOperator.h: interface for the CCustomerMHOperator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionMHOPERATOR_H__INCLUDED_)
#define AFX_SolutionMHOPERATOR_H__INCLUDED_

#include "BaseMHOperator.h"
#include "SingleInstance.h"

class CSolutionMHOperator : public CBaseMHOperator
                           
{
 public:
	virtual ~CSolutionMHOperator();

    virtual bool ValidateOperator(LPCTSTR szOperatorID, LPCTSTR szPassword);
    virtual bool ValidateSupervisor(LPCTSTR szOperatorID, LPCTSTR szPassword);

protected:
   	CSolutionMHOperator();
	bool FindOfflineOperator(LPCTSTR csCredentials);

private:
    CSolutionMHOperator(CSolutionMHOperator &);                    // hide copy const
    CSolutionMHOperator & operator =(const CSolutionMHOperator &); // hide assignment

	static CStringArray csaOfflineOperators;
};

#endif 