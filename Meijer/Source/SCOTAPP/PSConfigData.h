// PSConfigData.h: interface for the CPSConfigData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSCONFIGDATA_H__32810645_E285_11D5_8D8F_00E029301EFC__INCLUDED_)
#define AFX_PSCONFIGDATA_H__32810645_E285_11D5_8D8F_00E029301EFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConfigData.h"

// +TAR385554
#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class
#endif
CPSConfigData : public CConfigData  
// -TAR385554
{
public:
	CPSConfigData();
	virtual ~CPSConfigData();
	void GetFormattedString(LPCTSTR sKey, LPCTSTR sValueName, CString&sValue, LPCTSTR sDefault = _T(""), bool bPrimLang = true) const;
	void FormatString(CString&sValue, bool bPrimLang = true) const;
};

#endif // !defined(AFX_PSCONFIGDATA_H__32810645_E285_11D5_8D8F_00E029301EFC__INCLUDED_)
