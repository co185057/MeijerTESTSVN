// PSStringManager.h: interface for the CPSStringManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PS_STRING_MANAGER_H
#define _PS_STRING_MANAGER_H

#include "IPSStringManager.h"
#include "FPMap.h"
#include <string>
#include <map>
#include <vector>
#ifndef _CPPUNIT
#include "Common.h"
#else
#include "FakeCommon.h"
#endif
using namespace std;

class CLibraryHelper;
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
CPSStringManager  : public IPSStringManager
{

public:
    CPSStringManager();
    CPSStringManager(CLibraryHelper &helper);
    virtual ~CPSStringManager();

    virtual LPTSTR GetString(LPCTSTR stringName, const int languageId);
    virtual LPTSTR GetStrings(const int languageId);
    virtual void FreeString(LPTSTR str);
    virtual void LoadAndSaveLanguageStringToMap(const int nLangID, const int nLangCode);
    virtual CString GetUIString(const CString csStringID, const int nLangID);

protected:
    CPSStringManager(const CPSStringManager &rhs);        //copy constructor
    void operator=(const CPSStringManager &rhs);    //assignment operator
    
    bool Initialize(void);
    void UnInitialize(void);

    vector< map<CString, CString> > m_StringList;
    int m_Languages[SCOT_LANGUAGE_MAX];
private:
    HINSTANCE m_hdll;                     // Handle to SSCOUIStringsCppWrapper DLL.
    CFPMap<std::string, FARPROC> m_fpMap; // Holds our function pointers.
    CLibraryHelper *m_pLibraryHelper;
        
    void LoadStringToMap(const int nLangCode);
    CString GetStringFromMap(const CString csString, const int nLangID);
    CString GetUIStringDLLPath(void);

};

#endif // _PS_STRING_MANAGER_H
