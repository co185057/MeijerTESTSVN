// TotalObj.cpp : Implementation of CTotalObj
#include "stdafx.h"
#include "TotalObj.h"
#include "safearrayvariant.h"

#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CTotalObj, m_TotalDetailsMsg)
#pragma warning(default:4311) 

// CTotalObj

STDMETHODIMP CTotalObj::get_TaxDue(BSTR* pVal)
{
   CComBSTR csVal(m_TotalDetailsMsg.TaxDue());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTotalObj::put_TaxDue(BSTR newVal)
{
   m_TotalDetailsMsg.TaxDue(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTotalObj::get_TotalDue(BSTR* pVal)
{
   CComBSTR csVal(m_TotalDetailsMsg.TotalDue());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTotalObj::put_TotalDue(BSTR newVal)
{
   m_TotalDetailsMsg.TotalDue(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTotalObj::get_TotalDiscount(BSTR* pVal)
{
   CComBSTR csVal(m_TotalDetailsMsg.TotalDiscount());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTotalObj::put_TotalDiscount(BSTR newVal)
{
   m_TotalDetailsMsg.TotalDiscount(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTotalObj::get_ErrorDescription(BSTR* pVal)
{
   CComBSTR csVal(m_TotalDetailsMsg.ErrorDescription());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTotalObj::put_ErrorDescription(BSTR newVal)
{
   m_TotalDetailsMsg.ErrorDescription(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTotalObj::get_FoodStampAmount(BSTR* pVal)
{
   CComBSTR csVal(m_TotalDetailsMsg.FoodStampAmount());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTotalObj::put_FoodStampAmount(BSTR newVal)
{
   m_TotalDetailsMsg.FoodStampAmount(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTotalObj::get_RewardPoints(LONG* pVal)
{
   *pVal=m_TotalDetailsMsg.RewardPoints();
   return S_OK;
}

STDMETHODIMP CTotalObj::put_RewardPoints(LONG newVal)
{
   m_TotalDetailsMsg.RewardPoints(newVal);
   return S_OK;
}
