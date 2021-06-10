//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  ScotWinApp.h
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCOTWINAPP
#define _SCOTWINAPP

#include "IScotWinApp.h"
/////////////////////////////////////////////////////////////////////////////
// ScotWinApp command target

class CScotWinApp : public IScotWinApp
{
public:
    CScotWinApp();
    virtual ~CScotWinApp();

    virtual CString GetProfileString(LPCTSTR, LPCTSTR, LPCTSTR = NULL); 
    virtual int GetProfileInt(LPCTSTR, LPCTSTR, int);
    virtual LPCTSTR GetProfileName();
    virtual void SetProfileName(LPCTSTR);

    CWinApp *m_pApp; 
};

#endif

