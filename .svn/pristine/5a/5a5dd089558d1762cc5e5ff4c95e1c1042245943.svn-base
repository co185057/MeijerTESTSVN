#include "stdafx.h"
#include "ScotWinApp.h"

CScotWinApp::CScotWinApp()
{
    m_pApp = AfxGetApp();
}

CScotWinApp::~CScotWinApp() 
{
    //delete m_pApp;
    //m_pApp = NULL;
}

CString CScotWinApp::GetProfileString(LPCTSTR szSect, LPCTSTR szKey, LPCTSTR szDflt)
{
    return m_pApp->GetProfileString(szSect, szKey, szDflt);
}

int CScotWinApp::GetProfileInt(LPCTSTR szSect, LPCTSTR szKey, int szDflt)
{
    return m_pApp->GetProfileInt(szSect, szKey, szDflt);
}

LPCTSTR CScotWinApp::GetProfileName()
{
    return m_pApp->m_pszProfileName;
}

void CScotWinApp::SetProfileName(LPCTSTR pszAppProfile)
{
    m_pApp->m_pszProfileName = pszAppProfile;
}
