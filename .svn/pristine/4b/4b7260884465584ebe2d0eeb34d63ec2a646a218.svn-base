// PinpadObj.cpp : Implementation of CPinpadObj
#include "stdafx.h"
#include "PinPadObj.h"
#include "safearrayVariant.h"
#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CPinpadObj, m_PinpadMsg)
#pragma warning(default:4311) 

// CPinpadObj

STDMETHODIMP CPinpadObj::get_PrimaryAccountNumber(BSTR* pVal)
{
	CComBSTR csVal(m_PinpadMsg.PrimaryAccountNumber());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CPinpadObj::put_PrimaryAccountNumber(BSTR newVal)
{
	m_PinpadMsg.PrimaryAccountNumber(CString(newVal));
	return S_OK;
}

STDMETHODIMP CPinpadObj::get_TransactionType(TransactionTypes* pVal)
{
	*pVal= (TransactionTypes)m_PinpadMsg.TransactionType();
	return S_OK;
}

STDMETHODIMP CPinpadObj::put_TransactionType(TransactionTypes newVal)
{
	m_PinpadMsg.TransactionType((CPinpadMsg::TransactionTypes)newVal);
	return S_OK;
}

STDMETHODIMP CPinpadObj::get_PinEncoding(PinEncodingList* pVal)
{
	*pVal= (PinEncodingList)m_PinpadMsg.PinEncoding();
	return S_OK;
}

STDMETHODIMP CPinpadObj::put_PinEncoding(PinEncodingList newVal)
{
	m_PinpadMsg.PinEncoding((CPinpadMsg::PinEncodingList)newVal);
	return S_OK;
}
