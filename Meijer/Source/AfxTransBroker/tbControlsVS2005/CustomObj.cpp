// CustomObj.cpp : Implementation of CCustomObj
#include "stdafx.h"
#include "CustomObj.h"

#include "safearrayVariant.h"
#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CCustomObj, m_CustomMsg)
#pragma warning(default:4311) 

// CCustomObj

STDMETHODIMP CCustomObj::get_CustomerData(VARIANT* pVal)
{
	// ?vs check if all memory leak stuff is taken care of.
	*pVal= CSafeArrayVariant::GetVariantFromBuffer((UCHAR*)m_CustomMsg.CustomerData(), m_CustomMsg.DataLength());
	return S_OK;
}

STDMETHODIMP CCustomObj::put_CustomerData(VARIANT newVal)
{
	// vs? check for memory leaks etc
	ULONG unSize;
	UCHAR *pBuf= CSafeArrayVariant::GetNewBufferFromVariant(newVal, unSize);
	m_CustomMsg.CustomerData((void*)pBuf, unSize);

	return S_OK;
}
