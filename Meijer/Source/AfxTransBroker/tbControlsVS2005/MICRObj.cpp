// MICRObj.cpp : Implementation of CMICRObj
#include "stdafx.h"
#include "MICRObj.h"

#include "safearrayVariant.h"
#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CMICRObj, m_MICRMsg)
#pragma warning(default:4311) 

// CMICRObj

STDMETHODIMP CMICRObj::get_TransitNumber(BSTR* pVal)
{
	CComBSTR csVal(m_MICRMsg.TransitNumber());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CMICRObj::put_TransitNumber(BSTR newVal)
{
	m_MICRMsg.TransitNumber(CString(newVal));
	return S_OK;
}

STDMETHODIMP CMICRObj::get_AccountNumber(BSTR* pVal)
{
	CComBSTR csVal(m_MICRMsg.AccountNumber());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CMICRObj::put_AccountNumber(BSTR newVal)
{
	m_MICRMsg.AccountNumber(CString(newVal));
	return S_OK;
}

STDMETHODIMP CMICRObj::get_BankNumber(BSTR* pVal)
{
	CComBSTR csVal(m_MICRMsg.BankNumber());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CMICRObj::put_BankNumber(BSTR newVal)
{
	m_MICRMsg.BankNumber(CString(newVal));
	return S_OK;
}

STDMETHODIMP CMICRObj::get_SerialNumber(BSTR* pVal)
{
	CComBSTR csVal (m_MICRMsg.SerialNumber());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CMICRObj::put_SerialNumber(BSTR newVal)
{
	m_MICRMsg.SerialNumber(CString(newVal));
	return S_OK;
}

STDMETHODIMP CMICRObj::get_CheckType(CheckTypes* pVal)
{
	*pVal= (CheckTypes)m_MICRMsg.CheckType();
	return S_OK;
}

STDMETHODIMP CMICRObj::put_CheckType(CheckTypes newVal)
{
	m_MICRMsg.CheckType((CMicrMsg::CheckTypes)newVal);
	return S_OK;
}

STDMETHODIMP CMICRObj::get_CountryCode(CountryCodes* pVal)
{
	*pVal= (CountryCodes)m_MICRMsg.CountryCode();
	return S_OK;
}

STDMETHODIMP CMICRObj::put_CountryCode(CountryCodes newVal)
{
	m_MICRMsg.CountryCode((CMicrMsg::CountryCodes)newVal);
	return S_OK;
}

STDMETHODIMP CMICRObj::get_Data(BSTR* pVal)
{
	CComBSTR csVal(m_MICRMsg.Data());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CMICRObj::put_Data(BSTR newVal)
{
	m_MICRMsg.Data(CString(newVal));
	return S_OK;
}

