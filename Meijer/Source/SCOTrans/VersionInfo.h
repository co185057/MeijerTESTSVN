// VersionInfo.h: interface for the CVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERSIONINFO_H__CF700EA2_77E3_11D2_90E2_00609761BB2E__INCLUDED_)
#define AFX_VERSIONINFO_H__CF700EA2_77E3_11D2_90E2_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VER_QUERY_MAX  12

#define VER_COMPANY_NAME      0
#define VER_FILE_DESC         1
#define VER_FILE_VER          2
#define VER_INTERNAL_NAME     3
#define VER_LEGAL_COPYRIGHT   4
#define VER_ORIG_FILE_NAME    5
#define VER_PRODUCT_NAME      6
#define VER_PRODUCT_VER       7
#define VER_SPECIAL_BUILD     8
#define VER_PRIVATE_BUILD     9
#define VER_COMMENTS          10
#define VER_LEGAL_TRADEMARKS  11

class CVersionInfo  
{
public:
	CString GetComponentRegKey();
	CString& GetVersionValue(int verConst);
	BOOL Initialize(HINSTANCE hInstance);
	CVersionInfo();
	virtual ~CVersionInfo();

private:
	HMODULE m_hModule;
	LPVOID m_pVerInfo;
	CString m_ModuleFileName;
  static CString m_KeyTable[VER_QUERY_MAX];
  CString m_VerValues[VER_QUERY_MAX];
};

#endif // !defined(AFX_VERSIONINFO_H__CF700EA2_77E3_11D2_90E2_00609761BB2E__INCLUDED_)
