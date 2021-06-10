// SolutionMHOptions.h: interface for the CSolutionMHOptions class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SolutionMHOptions_H__INCLUDED_)
#define AFX_SolutionMHOptions_H__INCLUDED_

#include "BaseMHOptions.h"

class CSolutionMHOptions  : public CBaseMHOptions
{
public:
	virtual ~CSolutionMHOptions();
	int DetermineTerminalNumber(HKEY hKey);

    // Access to ACS APPINI options
    virtual CString AppIniOptionValue    (const CString & strSection, const CString& strOptName);
    virtual bool    AppIniBoolOptionValue(const CString & strSection, const CString& strOptName);
    virtual int     AppIniIntOptionValue (const CString & strSection, const CString& strOptName);
	virtual bool    IsAllowed(TBFEATURE nFeature);



protected:
	CSolutionMHOptions();

private:
    CSolutionMHOptions(CSolutionMHOptions &);                    // hide copy const
    CSolutionMHOptions & operator =(const CSolutionMHOptions &); // hide assignment
};

#endif 
