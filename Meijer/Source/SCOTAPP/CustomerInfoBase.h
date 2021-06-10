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


#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include "KeyValueHandler.h"
#include "TEventHandler.h"
#include "ICustomerInfo.h"

#define LOOKUP_COMPLETE               1001
#define LOOKUP_TIMEOUT                1002
#define LOOKUP_OPTIONS_RECIEVED       1003


#define KEY_OPERATION      _T("operation")
#define KEY_LOYALTY_LOOKUP _T("LoyaltyLookup")
#define KEY_LOYALTY_ID     _T("ShopID")
#define IDC_PERSONALIZATIONCTRL1    2500

DECLARE_KV_HANDLER_CLASS_BEGIN(LookupParmsBase)
   DECLARE_KV_CSTRING(Operation)
   DECLARE_KV_CSTRING(LoyaltyID)
DECLARE_KV_HANDLER_CLASS_END
/*
#ifdef UNICODE
#ifdef _DEBUG
#import <NCRPersonalizationUD.dll> rename_namespace("PERSN")
#else
#import <NCRPersonalizationU.dll> rename_namespace("PERSN")
#endif
#else
#ifdef _DEBUG
#import <NCRPersonalizationD.dll> rename_namespace("PERSN")
#else
#import <NCRPersonalization.dll> rename_namespace("PERSN")
#endif
#endif
*/
// SSCOADK-2499 Sample SSF will not compile stand-alone
//              Use HeaderGenerator project to build tlh/tli headers if necessary.
#include <NCRPersonalizationU.tlh>

class CCustomerInfoBase;

typedef CMap <CString, LPCTSTR, CString, LPCTSTR> customer_string_map;
// ***** Declare an event handling class using the TEventHandler template. *****
typedef TEventHandlerNamespace::TEventHandler<CCustomerInfoBase, PERSN::IPersonalization, PERSN::_IPersonalizationEvents> PersonalizationEventHandler;


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 CCustomerInfoBase  : public CObject, public ICustomerInfo
{
public:
    CCustomerInfoBase();
    virtual ~CCustomerInfoBase();
    virtual long Initialize();
	virtual void UnInitialize();
    virtual ClearCustomerInfo();
    virtual void Lookup(_bstr_t bstrLookupParms);
    virtual long VerifyAndGetLookupTimer(CString csLookupType, CString csLookupID);
    virtual CString GetPreference(CString csSetting, bool bOrigValue = false);
    virtual void SetPreference(CString csSetting, CString csValue);
    virtual CString GetDataString(CString csSetting);
    virtual void SetDataString(CString csSetting, CString csValue);
    virtual long GetMetricLong(CString csSetting);
    virtual void SetMetricLong(CString csSetting, long lValue);
    virtual void IncrementMetricLong(CString csSetting, long lValue);
    virtual CString GetOption(CString csOption);
    virtual void UpdatePreferences();
    virtual bool PersonalizationActive();
	virtual void RequestOptions();

    HRESULT OnEventPersonalizationObj
	 (
	  PersonalizationEventHandler* pEventHandler,
	  DISPID dispidMember, 
	  REFIID riid,
	  LCID lcid, 
	  WORD wFlags, 
	  DISPPARAMS* pdispparams, 
	  VARIANT* pvarResult,
	  EXCEPINFO* pexcepinfo, 
	  UINT* puArgErr
	 );
     static PersonalizationEventHandler *m_pPersonalizationEventHandler;
protected:
    _bstr_t m_bstrCustomerInfo;  //XML string of customer info
    static bool m_bInitialized;
    static PERSN::IPersonalizationPtr m_pPersonalizationObj;
    static PERSN::_IPersonalizationEventsPtr m_pPersonalizationEventObj;
    static time_t m_tLookupStartTime;
    static time_t m_tLookupCompleteTime;
    CString m_csLookupID;
    DECLARE_DYNCREATE(CCustomerInfoBase)// MFC Runtime class/object information
};



#endif

