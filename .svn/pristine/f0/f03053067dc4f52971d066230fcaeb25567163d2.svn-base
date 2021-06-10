// ScannerObj.cpp : Implementation of CScannerObj
#include "stdafx.h"
#include "ScannerObj.h"
#include "safearrayVariant.h"
#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CScannerObj, m_scannerMsg)
#pragma warning(default:4311) 


STDMETHODIMP CScannerObj::get_BarcodeType(BarcodeTypes* pVal)
{
	*pVal= (BarcodeTypes)m_scannerMsg.BarcodeType();
	return S_OK;
}

STDMETHODIMP CScannerObj::put_BarcodeType(BarcodeTypes newVal)
{
	m_scannerMsg.BarcodeType((CScannerMsg::BarcodeTypes)newVal);
	return S_OK;
}

STDMETHODIMP CScannerObj::get_Data(BSTR* pVal)
{
	CComBSTR csVal(m_scannerMsg.Data());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CScannerObj::put_Data(BSTR newVal)
{
	m_scannerMsg.Data(CString(newVal));
	return S_OK;
}
