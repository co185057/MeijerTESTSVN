// SCOTMultiStringList.h: interface for the CSCOTMultiStringList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTMULTISTRINGLIST_H__D6302F82_938D_11D3_8A82_009027AB32EF__INCLUDED_)
#define AFX_SCOTMULTISTRINGLIST_H__D6302F82_938D_11D3_8A82_009027AB32EF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ScotStringList.h"
#include "DllDefine.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 CSCOTMultiStringList : public CSCOTStringList  
{
public:
	bool GetStatus(int);
	CString GetLabel(int);
	USHORT GetCode(int);
	CString GetFieldValue(int, int);
	CSCOTMultiStringList(LPCTSTR szFileName = NULL, int nListID = -99);
	CSCOTMultiStringList(int nListID);
private:
	CString GetField(CString, int, _TCHAR);
};

#endif // !defined(AFX_SCOTMULTISTRINGLIST_H__D6302F82_938D_11D3_8A82_009027AB32EF__INCLUDED_)
