// VersionInfo.cpp: implementation of the CVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <ddeml.h>

//#include "scotra.h"
#include "VersionInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVersionInfo::CVersionInfo()
{
  m_pVerInfo = NULL;
}

CVersionInfo::~CVersionInfo()
{
  if (m_pVerInfo)
  {
    free(m_pVerInfo);
    m_pVerInfo = NULL;
  }
}

BOOL CVersionInfo::Initialize(HINSTANCE hInstance)
{
  BOOL rc = FALSE;
  
  m_hModule = (HMODULE) hInstance;

  int dwrc = GetModuleFileName(m_hModule, m_ModuleFileName.GetBuffer(_MAX_PATH), _MAX_PATH);
  m_ModuleFileName.ReleaseBuffer();
  if (dwrc > 0)
  {
    DWORD size;
    DWORD zero;
    size = GetFileVersionInfoSize((LPTSTR) (LPCTSTR) m_ModuleFileName, &zero);
    if (size > 0)
    {
      m_pVerInfo = (LPVOID) malloc(size);
      if (m_pVerInfo)
      {
        rc = GetFileVersionInfo((LPTSTR) (LPCTSTR) m_ModuleFileName, 0, size, m_pVerInfo);
        if (rc)
        {
          // Now populate the values!
          WORD Langs[7] = { MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
                            LANGIDFROMLCID(GetThreadLocale()),
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            GetUserDefaultLangID(),
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
                            GetSystemDefaultLangID(),
                            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) };
          CString key;
          UINT klen;
          int valNdx, langNdx;

          for (valNdx = 0; valNdx < VER_QUERY_MAX; valNdx++)
          {
            char *value = NULL;
            for (langNdx = 0; ((langNdx < sizeof(Langs)) && (value == NULL)); langNdx++)
            {
              key.Format("\\StringFileInfo\\%04.4X%04.4X\\%s", Langs[langNdx],
                       CP_WINUNICODE, m_KeyTable[valNdx]);
              if (VerQueryValue(m_pVerInfo, (LPTSTR) (LPCTSTR) key, (LPVOID*)&value, &klen))
              {
                LPTSTR buf = m_VerValues[valNdx].GetBuffer(klen + 1);
                strncpy(buf, value, klen);
                buf[klen] = '\0';
                m_VerValues[valNdx].ReleaseBuffer();
                value = buf;
              }
              else
              {
                value = NULL;
              }
            }
          }
        }
        else
        {
          free(m_pVerInfo);
          m_pVerInfo = NULL;
        }
      }
    }
  }
  

  return rc;
}


CString CVersionInfo::m_KeyTable[] = 
{
  _T("CompanyName"),
  _T("FileDescription"),
  _T("FileVersion"),
  _T("InternalName"),
  _T("LegalCopyright"),
  _T("OriginalFilename"),
  _T("ProductName"),
  _T("ProductVersion"),
  _T("SpecialBuild"),
  _T("PrivateBuild"),
  _T("Comments"),
  _T("LegalTrademarks"),
};

CString& CVersionInfo::GetVersionValue(int verConst)
{
  return m_VerValues[verConst];
}

CString CVersionInfo::GetComponentRegKey()
{
  CString regKey;
  regKey.Format("SOFTWARE\\%s\\%s\\CurrentVersion\\%s",
                 GetVersionValue(VER_COMPANY_NAME),
                 GetVersionValue(VER_PRODUCT_NAME),
                 GetVersionValue(VER_INTERNAL_NAME));

  return regKey;
}
