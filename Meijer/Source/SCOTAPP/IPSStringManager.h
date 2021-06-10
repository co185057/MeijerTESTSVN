// IPSStringManager.h: interface for the CPSStringManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _IPS_STRING_MANAGER_H
#define _IPS_STRING_MANAGER_H

//class CLibraryHelper;
class IPSStringManager  
{

public:
    IPSStringManager(){}
    virtual ~IPSStringManager(){}
    
    //IPSStringManager(CLibraryHelper &helper);

    virtual LPTSTR GetString(LPCTSTR stringName, const int languageId)=0;
    virtual LPTSTR GetStrings(const int languageId)=0;
    virtual void FreeString(LPTSTR str)=0;
    virtual void LoadAndSaveLanguageStringToMap(const int nLangID, const int nLangCode)=0;
    virtual CString GetUIString(const CString csStringID, const int nLangID)=0;

};

#endif // _IPS_STRING_MANAGER_H
