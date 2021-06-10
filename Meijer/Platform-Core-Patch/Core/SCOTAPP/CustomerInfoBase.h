//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    CustomerInfoBase.h
//
// TITLE:   Retrieves and tracks Customer data
//
// AUTHOR:  Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// CustomerInfoBase
// This class retrieves Customer data from a Web Service
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CCustomerInfoBase
#define _CCustomerInfoBase

#include "CXMLHTTPEvent.h"
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include "KeyValueHandler.h"


#define KEY_OPERATION      _T("operation")
#define KEY_LOYALTY_LOOKUP _T("LoyaltyLookup")
#define KEY_LOYALTY_ID     _T("ShopID")

DECLARE_KV_HANDLER_CLASS_BEGIN(LookupParmsBase)
   DECLARE_KV_CSTRING(Operation)
   DECLARE_KV_CSTRING(LoyaltyID)
DECLARE_KV_HANDLER_CLASS_END


typedef CMap <CString, LPCTSTR, CString, LPCTSTR> customer_string_map;

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 CCustomerInfoBase  : public CObject, CXMLHTTPEventCallBack
{
public:
    CCustomerInfoBase();
    virtual ~CCustomerInfoBase();
    virtual long Initialize();
	virtual void UnInitialize();
    virtual ClearCustomerInfo();
    virtual void Lookup(_bstr_t bstrLookupParms);
    virtual CString GetPreference(CString csSetting, bool bOrigValue = false);
    virtual void SetPreference(CString csSetting, CString csValue);
    virtual CString GetOption(CString csOption);
    virtual void UpdatePreferences();
    virtual void OnReadyStateChange(long lReadyState);
    virtual void AbortRequest();
    virtual bool PersonalizationActive();

protected:
    virtual long SendRequest( _bstr_t bstrWebAddress, 
                              _bstr_t bstrAction, 
                              _bstr_t bstrRequest);
    virtual bool GetConfigInfo(CString csFileName);
    MSXML2::IXMLHTTPRequestPtr  m_pWebRequest;
    CXMLHTTPEvent* m_pXMLHTTPEvent;
    _bstr_t m_bstrCustomerInfo;  //XML string of customer info
    SCOT_LANGUAGE_TYPE m_preferredLanguage;
    customer_string_map ConfigOpts;
    customer_string_map CustomerPref;
    customer_string_map UpdatedPref;  
    CCriticalSection m_CriticalPreferences;
    CCriticalSection m_CriticalRequest;
    bool m_bOnline;
    virtual void GetCustomerSettings(_bstr_t bstrCustomerXML);
    static bool m_bInitialized;
    CString m_csServerName;


DECLARE_DYNCREATE(CCustomerInfoBase)// MFC Runtime class/object information
};



#endif

