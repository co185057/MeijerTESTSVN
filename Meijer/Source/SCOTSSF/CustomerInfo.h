    /////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // FILE:  CustomerInfo.H
    //
    // FUNCTION:  Retrieves customer-specific info for use in personalizing UI 
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CustomerInfo
#define _CustomerInfo
    
#include "CustomerInfoBase.h"

#ifdef _SCOTSSF_
    class DLLEXPORT
#else
    class DLLIMPORT
#endif
CCustomerInfo : public CCustomerInfoBase
{
public:
    CCustomerInfo();
    virtual ~CCustomerInfo();
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

};

#endif
