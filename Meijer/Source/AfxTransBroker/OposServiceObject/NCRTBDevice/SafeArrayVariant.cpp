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

void CSafeArrayVariant::GetBufferFromVariant(const VARIANT &var, UCHAR* pBuffer, ULONG& ulnBytes)
{
	void* pData;
	ulnBytes= var.parray->rgsabound[0].cElements;
	HRESULT hresult = SafeArrayAccessData(var.parray, &pData);
	if (!FAILED(hresult)) {
		memcpy(pBuffer, pData, ulnBytes);	
		SafeArrayUnaccessData(var.parray);
	}
}

void CSafeArrayVariant::GetBuffer(UCHAR* pBuffer, ULONG& ulnBytes)
{
	void* pData;
	ulnBytes= m_var.parray->rgsabound[0].cElements;
	HRESULT hresult = SafeArrayAccessData(m_var.parray, &pData);
	if (!FAILED(hresult)) {
		memcpy(pBuffer, pData, ulnBytes);	
		SafeArrayUnaccessData(m_var.parray);
	}
}
