#include "StdAfx.h"
#include "safearrayvariant.h"

CSafeArrayVariant::CSafeArrayVariant(void)
: m_psa(NULL)
{
	VariantInit(&m_var);
}

CSafeArrayVariant::~CSafeArrayVariant(void)
{
	if (m_psa)
		SafeArrayDestroy(m_psa);
}

void CSafeArrayVariant::SetBuffer(UCHAR* pBuffer, ULONG ulnBytes)
{
	m_rgsabound[0].lLbound = 0;
	m_rgsabound[0].cElements = ulnBytes;

	if (m_psa)
		SafeArrayDestroy(m_psa);
	m_psa= SafeArrayCreate(VT_UI1, 1, m_rgsabound);
	void* pData;
	HRESULT hresult = SafeArrayAccessData(m_psa, &pData);
	if (!FAILED(hresult)) {
		memcpy(pData, pBuffer, ulnBytes);
		SafeArrayUnaccessData(m_psa);

		m_var.vt= VT_ARRAY | VT_UI1;
		m_var.parray= m_psa;
	}
}

VARIANT & CSafeArrayVariant::GetVariant()
{
	return m_var;
}

void CSafeArrayVariant::SetVariant(VARIANT &var)
{
	::VariantCopy(&m_var, &var);
}

///////////////////////////////////////////////////
// GetBufferFromVariant:
// convert a variant into the provided buffer
// cb indicates the size of buffer
// returns: The number of bytes copied into buffer
////////////////////////////////////////////////////
ULONG CSafeArrayVariant::GetBufferFromVariant(const VARIANT &var, UCHAR* pBuffer, ULONG cb)
{
	void* pData;
   ULONG ulcb=0;
	HRESULT hresult = SafeArrayAccessData(var.parray, &pData);
	if (!FAILED(hresult)) {
   	ulcb= min(cb, var.parray->rgsabound[0].cElements);
		memcpy(pBuffer, pData, ulcb);	
		SafeArrayUnaccessData(var.parray);
	}
   return ulcb;
}

ULONG CSafeArrayVariant::GetBuffer(UCHAR* pBuffer, ULONG cb)
{
   return CSafeArrayVariant::GetBufferFromVariant(m_var, pBuffer, cb);
}

///////////////////////////////////////////////////
// GetNewBufferFromVariant:
// allocate a new buffer and convert a variant into the buffer
// cb is the number of bytes copied into buffer
// returns: The allocated buffer
////////////////////////////////////////////////////
UCHAR *CSafeArrayVariant::GetNewBufferFromVariant(const VARIANT &var, ULONG &cb)
{
	void* pData;
   UCHAR *pBuffer=NULL;
	HRESULT hresult = SafeArrayAccessData(var.parray, &pData);
	if (!FAILED(hresult)) {
   	cb= var.parray->rgsabound[0].cElements;
      pBuffer= new UCHAR[cb];
		memcpy(pBuffer, pData, cb);	
		SafeArrayUnaccessData(var.parray);
	}
   else
      cb=0;
   return pBuffer;
}

UCHAR *CSafeArrayVariant::GetNewBuffer(ULONG &cb)
{
   return CSafeArrayVariant::GetNewBufferFromVariant(m_var, cb);
}

VARIANT CSafeArrayVariant::GetVariantFromBuffer(UCHAR* pBuffer, LONG lnBytes)
{
	SAFEARRAYBOUND m_rgsabound[1];
	m_rgsabound[0].lLbound = 0;
	m_rgsabound[0].cElements = lnBytes;

	SAFEARRAY		*psa;	
	psa= SafeArrayCreate(VT_UI1, 1, m_rgsabound);

	void* pData;
	HRESULT hresult = SafeArrayAccessData(psa, &pData);
	VARIANT		var;
	VariantInit(&var);

	if (!FAILED(hresult)) {
		memcpy(pData, pBuffer, lnBytes);
		SafeArrayUnaccessData(psa);

		var.vt= VT_ARRAY | VT_UI1;
		var.parray= psa;
	}
//	else
//		ATLASSERT(FALSE);
	return var;
}