// MSRObj.cpp : Implementation of CMSRObj
#include "stdafx.h"
#include "MSRObj.h"
#include "safearrayVariant.h"
#include "ImplementMacros.h"

// CMSRObj
#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CMSRObj, m_MSRMsg)
#pragma warning(default:4311) 

STDMETHODIMP CMSRObj::get_Track1(BSTR* pVal)
{
	CComBSTR csVal(m_MSRMsg.Track1());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CMSRObj::get_Track2(BSTR* pVal)
{
	CComBSTR csVal(m_MSRMsg.Track2());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CMSRObj::get_Track3(BSTR* pVal)
{
	CComBSTR csVal(m_MSRMsg.Track3());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CMSRObj::SetTrack123(BYTE Track1Position, BYTE Track2Position, BYTE Track3Position, BSTR Track123)
{
	m_MSRMsg.SetTrack123(Track1Position, Track2Position, Track3Position, CString(Track123));
	return S_OK;
}
