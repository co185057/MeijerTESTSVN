// TBProperties.h: interface for the CTBProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMERTBPROPERTIES_H__87331EEF_1E06_44DA_836B_255595E965F1__INCLUDED_)
#define AFX_CUSTOMERTBPROPERTIES_H__87331EEF_1E06_44DA_836B_255595E965F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SolutionTBProperties.h"

class CCustomerTBProperties : public CSolutionTBProperties, 
                      public CSingleInstance<CCustomerTBProperties>
{
   friend class CSingleInstance<CCustomerTBProperties>;
public:
	//virtual CString GetHookProperty(LPCTSTR szPropertyName);
	//virtual void SetHookProperty(LPCTSTR szPropertyName, LPCTSTR szValue);

private:
	CCustomerTBProperties();
	virtual ~CCustomerTBProperties();

   CCustomerTBProperties (const CCustomerTBProperties &);
   CCustomerTBProperties & operator =(const CCustomerTBProperties &);
};

#endif // !defined(AFX_TBPROPERTIES_H__87331EEF_1E06_44DA_836B_255595E965F1__INCLUDED_)
