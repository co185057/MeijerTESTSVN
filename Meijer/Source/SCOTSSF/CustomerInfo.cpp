//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  CustomerInfo.CPP
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
  
#include "CustomerInfo.h"

  ////////////////////////////////////////////////
CCustomerInfo::CCustomerInfo():
    CCustomerInfoBase()
{
  
}

CCustomerInfo::~CCustomerInfo()
{
}

void CCustomerInfo::UnInitialize()
{
    CCustomerInfoBase::UnInitialize();
    return;
}

long CCustomerInfo::Initialize()
{
    return CCustomerInfoBase::Initialize();
}

void CCustomerInfo::Lookup(_bstr_t bstrLookupParms)
{
    CCustomerInfoBase::Lookup( bstrLookupParms);
    return;
}


long CCustomerInfo::VerifyAndGetLookupTimer(CString csLookupType, CString csLookupID)
{
    return CCustomerInfoBase::VerifyAndGetLookupTimer(csLookupType, csLookupID);
}

CCustomerInfo::ClearCustomerInfo()
{
    return CCustomerInfoBase::ClearCustomerInfo();
}

CString CCustomerInfo::GetPreference(CString csSetting, bool bOrigValue)
{
    return CCustomerInfoBase::GetPreference(csSetting, bOrigValue);
}

void CCustomerInfo::SetPreference(CString csSetting, CString csValue)
{
    CCustomerInfoBase::SetPreference(csSetting, csValue);
    return;
}

CString CCustomerInfo::GetDataString(CString csSetting)
{
    return CCustomerInfoBase::GetDataString(csSetting);
}

void CCustomerInfo::SetDataString(CString csSetting, CString csValue)
{
    CCustomerInfoBase::SetDataString(csSetting, csValue);
    return;
}

long CCustomerInfo::GetMetricLong(CString csSetting)
{
    return CCustomerInfoBase::GetMetricLong(csSetting);
}

void CCustomerInfo::SetMetricLong(CString csSetting, long lValue)
{
    CCustomerInfoBase::SetMetricLong(csSetting, lValue);
    return;
}

void CCustomerInfo::IncrementMetricLong(CString csSetting, long lValue)
{
    CCustomerInfoBase::IncrementMetricLong(csSetting, lValue);
    return;
}

CString CCustomerInfo::GetOption(CString csOption)
{
    return CCustomerInfoBase::GetOption(csOption);
}

bool CCustomerInfo::PersonalizationActive()
{
    return CCustomerInfoBase::PersonalizationActive();
}

void CCustomerInfo::UpdatePreferences()
{
    CCustomerInfoBase::UpdatePreferences();
    return;
}

void CCustomerInfo::RequestOptions()
{
    CCustomerInfoBase::RequestOptions();
    return;
}
