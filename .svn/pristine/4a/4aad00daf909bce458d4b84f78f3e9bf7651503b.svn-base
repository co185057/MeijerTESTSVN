//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  ConfigReader.H
//
// TITLE:   Class interface definition file (describe class here)
//
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _CONFIGREADER
#define _CONFIGREADER

#ifndef SCOTOPTS
  #define SCOTOPTS _T("SCOTOPTS")
#endif

#ifdef _CPPUNIT
  #include "FakeSMStateBase.h"
  #include "FakeScotWinApp.h"
  #include "FakePSConfigData.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
  #define WINAPP CWinApp
  #define GETPROFILENAME m_pszProfileName
  #define SETPROFILENAME(a) m_pszProfileName = a
#else
  #define GETPROFILENAME GetProfileName()
  #define SETPROFILENAME(a) SetProfileName(a)
  #ifndef _CPPUNIT
    #define WINAPP IScotWinApp 
  #else
    #define WINAPP CFakeScotWinApp
  #endif 
#endif

#include "ConfigObject.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
ConfigReader 
{

public:
    ConfigReader();
    ~ConfigReader();

    bool GetFlagOption(CString csSection, CString csOption, bool fDefaultVal, CString csFileName);
    BOOL SearchAndSetValueFromMap(LPCTSTR szKey, CString& csVal);
    BOOL ClearOptionsMap();

    int GetIntOption(CString csSection, CString csOption, int nDefaultVal, CString csFileName);

    long Initialize(const int nTerminal);

    CString GetStringOption(CString csSection, CString csOption, CString csDefaultVal, CString csFileName);
    CString ConcatSectionOptionStrings(CString csSec, CString csOpt);

    void LoadSectionOptionsToMap(CString csFileName, CString csSection);
    void RetrieveSectionOptions(CString csSource, CString csSection);
    void AddOptionToMap(LPCTSTR szKey, CString csValue);
    void SetTerminalNo(int nValue);

    static int GetTerminalNo();

private:
    WINAPP* GetWinApp();
    static CString GetStringValue(WINAPP* pApp, LPCTSTR szSection, LPCTSTR szOption, CString csDefault, const bool fErrorLog);
    static CString GetStringFromFile(WINAPP* pApp, CString csSec, CString csOpt, CString csDefault, CString csFileName);
    
    static int nTerminalNo;

    WINAPP* m_pApp;

    typedef CMap<CString, LPCTSTR, CString, LPCTSTR> OptionsMap;
    OptionsMap options_map;

};

#endif