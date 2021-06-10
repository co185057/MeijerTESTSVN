// SolutionTBProperties.h: interface for the CSolutionTBProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOLUTIONTBPROPERTIES_H__0799AC3E_9025_496F_9072_FA7A1D301177__INCLUDED_)
#define AFX_SOLUTIONTBPROPERTIES_H__0799AC3E_9025_496F_9072_FA7A1D301177__INCLUDED_

#include "SolutionTBDefs.h"
#include "BaseTBProperties.h"
#include "SingleInstance.h"

class CSolutionTBProperties : public CBaseTBProperties  
{
public:
	virtual CString GetHookProperty(LPCTSTR szPropertyName);
	virtual void SetHookProperty(LPCTSTR szPropertyName, LPCTSTR szValue);
};

#endif // !defined(AFX_SOLUTIONTBPROPERTIES_H__0799AC3E_9025_496F_9072_FA7A1D301177__INCLUDED_)
