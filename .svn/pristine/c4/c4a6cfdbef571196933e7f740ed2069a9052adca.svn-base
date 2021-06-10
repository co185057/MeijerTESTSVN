// SCOTStringList.h: interface for the CSCOTStringList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCOTSTRINGLIST_H_
#define _SCOTSTRINGLIST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//
#include "DllDefine.h"


typedef	CArray<CString*,CString*> CWrapStringArray;


// WARNING: This function is deprecated in VC7 //
#if _MFC_VER < 0x0700
// Template Collection class helper to delete elements in the array RemoveAll is called
template <> void inline AFXAPI DestructElements <CString*> ( CString** ppStrList, int nCount )
{    
  for ( int i = 0; i < nCount; i++, ppStrList++ )
  {
    if (*ppStrList)
	{
		(*ppStrList)->Empty();
		delete *ppStrList;
	}
  }
}
#endif

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

 CSCOTStringList : public CWrapStringArray  
{
public:
	CSCOTStringList(LPCTSTR szFileName = NULL, int nListID = -99);
	CSCOTStringList(int nListID);
	virtual ~CSCOTStringList();
	virtual BOOL Initialize(LPCTSTR szFileName = NULL);
	void    UnInitialize();
	int		GetSize();
	CString GetFileName();
	CString GetListName();
	int		GetListID();
	// set properties
	bool	SetListName(LPCTSTR szFileName);
	void	SetFileName(LPCTSTR szFileName);
	void	SetListID(int nListID);
	CString& operator [](int index);	// operator [] override
	int Find(CString csFindString);
	static bool IsFileUnicode(LPCTSTR lpInFile);
	
protected:
	CString m_csFileName;
	void RemoveDoubleQuotes(CString& csOrigString);

private:
	int     m_nListSize;
	CString m_csListName;
	int		m_nListID;
	virtual BOOL BuildList();
	bool ExtractStringContent(CString& csOrigString, TCHAR chDilimiter1, TCHAR chDilimiter2 = NULL);
};

#endif // _SCOTSTRINGLIST_H_
