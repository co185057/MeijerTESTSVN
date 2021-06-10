// TenderObj.cpp : Implementation of CTenderObj
#include "stdafx.h"
#include "TenderObj.h"
#include "safearrayvariant.h"

#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CTenderObj, m_TenderDetailsMsg)
#pragma warning(default:4311) 


// CTenderObj

STDMETHODIMP CTenderObj::get_Description(BSTR* pVal)
{
   CComBSTR csVal(m_TenderDetailsMsg.TenderDescription());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTenderObj::put_Description(BSTR newVal)
{
   m_TenderDetailsMsg.TenderDescription(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTenderObj::get_Type(LONG* pVal)
{
   *pVal= m_TenderDetailsMsg.TenderType();
   return S_OK;
}

STDMETHODIMP CTenderObj::put_Type(LONG newVal)
{
   m_TenderDetailsMsg.TenderType(newVal);
   return S_OK;
}

STDMETHODIMP CTenderObj::get_Amount(BSTR* pVal)
{
   CComBSTR csVal(m_TenderDetailsMsg.TenderAmount());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTenderObj::put_Amount(BSTR newVal)
{
   m_TenderDetailsMsg.TenderAmount(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTenderObj::get_Balance(BSTR* pVal)
{
   CComBSTR csVal(m_TenderDetailsMsg.Balance());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTenderObj::put_Balance(BSTR newVal)
{
   m_TenderDetailsMsg.Balance(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTenderObj::get_ChangeDue(BSTR* pVal)
{
   CComBSTR csVal(m_TenderDetailsMsg.ChangeDue());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTenderObj::put_ChangeDue(BSTR newVal)
{
   m_TenderDetailsMsg.ChangeDue(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTenderObj::get_Accepted(BYTE* pVal)
{
   *pVal= m_TenderDetailsMsg.TenderAccepted();
   return S_OK;
}

STDMETHODIMP CTenderObj::put_Accepted(BYTE newVal)
{
   m_TenderDetailsMsg.TenderAccepted(newVal != 0);
   return S_OK;
}

STDMETHODIMP CTenderObj::get_Voided(BYTE* pVal)
{
   *pVal= m_TenderDetailsMsg.TenderVoided();
   return S_OK;
}

STDMETHODIMP CTenderObj::put_Voided(BYTE newVal)
{
   m_TenderDetailsMsg.TenderVoided(newVal != 0);
   return S_OK;
}

STDMETHODIMP CTenderObj::get_CardRemainingBalance(BSTR* pVal)
{
   CComBSTR csVal(m_TenderDetailsMsg.CardRemainingBalance());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTenderObj::put_CardRemainingBalance(BSTR newVal)
{
   m_TenderDetailsMsg.CardRemainingBalance(CString(newVal));
   return S_OK;
}

STDMETHODIMP CTenderObj::get_ErrorDescription(BSTR* pVal)
{
   CComBSTR csVal(m_TenderDetailsMsg.ErrorDescription());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CTenderObj::put_ErrorDescription(BSTR newVal)
{
   m_TenderDetailsMsg.ErrorDescription(CString(newVal));
   return S_OK;
}
