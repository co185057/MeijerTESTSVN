// TBGenObj.cpp : Implementation of CTBGenObj
#include "stdafx.h"
#include "TBGenObj.h"
#include "safearrayVariant.h"
#include "ImplementMacros.h"
#include ".\tbgenobj.h"

//#pragma warning(disable:4311) 
//IMPLEMENT_TBSTREAM(CTBGenObj, m_tbGenMsg)
//#pragma warning(default:4311) 

STDMETHODIMP CTBGenObj::StreamOut(LONG* cb, BYTE** pBuffer) 
{ 
   *pBuffer= m_tbGenMsg.Serialize((int &)*cb); 
   return S_OK; 
} 
 
 
STDMETHODIMP CTBGenObj::ParseStream(VARIANT i_varpBuf) 
{ 
	ULONG lnBytes; 
	BYTE* pBuffer= CSafeArrayVariant::GetNewBufferFromVariant(i_varpBuf, lnBytes); 
	m_tbGenMsg.Parse(pBuffer); 
	delete [] pBuffer; 
	return S_OK; 
} 
 
STDMETHODIMP CTBGenObj::ParseBuffer(BYTE *pBuf) 
{ 
	m_tbGenMsg.Parse(pBuf); 
	return S_OK; 
} 

#pragma warning(disable:4311) 
 
STDMETHODIMP CTBGenObj::GetMessage(LONG* o_pTBMsgObj) 
{ 
	*o_pTBMsgObj= (LONG)(CTBMsgObj*)&m_tbGenMsg; 
	return S_OK; 
} 
#pragma warning(default:4311) 


void SetCOMError(const CString &cs)
{
    ICreateErrorInfo * pCreate = NULL;
    IErrorInfo * pInfo = NULL;

    CreateErrorInfo(&pCreate);
    pCreate->SetDescription(_bstr_t(cs));

    pCreate->QueryInterface(IID_IErrorInfo,  (void**)&pInfo);
    SetErrorInfo(0, pInfo);

    pInfo->Release();
    pCreate->Release();
}

// CTBGenObj

STDMETHODIMP CTBGenObj::SetValue(BSTR Name, VARIANT vValue)
{
	try {
		//_bstr_t bstr(Name, false);
		m_tbGenMsg.SetValue(_bstr_t(Name), vValue);
	}
	catch(CString &cs)
	{
		SetCOMError(cs);
		return E_FAIL;
	}
	catch(...)
	{
		return E_FAIL;
	}
	return S_OK;
}
 
STDMETHODIMP CTBGenObj::GetValue(BSTR Name, VARIANT* vValue)
{
	try {
	    _bstr_t bstrName(Name);
		_variant_t v= m_tbGenMsg.GetValue((LPCTSTR) bstrName);
		VariantCopy(vValue, &v);
	}
	catch(CString &cs)
	{
		SetCOMError(cs);
		return E_FAIL;
	}
	catch(...) {
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CTBGenObj::GetValue2(BSTR Name, VARIANT* vValue)
{
    _bstr_t bstrName(Name);
	_variant_t v= m_tbGenMsg.GetValue2((LPCTSTR) bstrName);
	VariantCopy(vValue, &v);
	return S_OK;
}

STDMETHODIMP CTBGenObj::Reset(ResetOptions option)
{
	try {
        m_tbGenMsg.Reset((CTBGenMsg::ResetOptions) option);
	}
	catch(CString &cs)
	{
		SetCOMError(cs);
		return E_FAIL;
	}
	catch(...) {
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CTBGenObj::get_xml(BSTR* pVal)
{
	*pVal = _bstr_t(m_tbGenMsg.getXML()).copy();	

   return S_OK;
}

STDMETHODIMP CTBGenObj::put_xml(BSTR newVal)
{
	try {
		m_tbGenMsg.setXML(newVal);
	}
	catch(CString &cs)
	{
		SetCOMError(cs);
		return E_FAIL;
	}
	catch(...) {
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CTBGenObj::get_MsgId(BYTE* pVal)
{
	*pVal= m_tbGenMsg.GetTBMsgId();
	return S_OK;
}

STDMETHODIMP CTBGenObj::put_MsgId(BYTE newVal)
{
	try {
		m_tbGenMsg.SetTBMsgId(newVal);
	}
	catch(CString &cs)
	{
		SetCOMError(cs);
		return E_FAIL;
	}
	catch(...) {
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CTBGenObj::get_IsXMLOutput(BYTE* pVal)
{
	*pVal= m_tbGenMsg.GetXMLOutPut();
	return S_OK;
}

STDMETHODIMP CTBGenObj::put_IsXMLOutput(BYTE newVal)
{
	m_tbGenMsg.SetXMLOutPut(newVal ? true:false);
	return S_OK;
}

STDMETHODIMP CTBGenObj::get_Validate(BYTE* pVal)
{
	*pVal= m_tbGenMsg.GetValidate();
	return S_OK;
}

STDMETHODIMP CTBGenObj::put_Validate(BYTE newVal)
{
	try {
		m_tbGenMsg.SetValidate(newVal ? true:false);
	}
	catch(CString &cs)
	{
		SetCOMError(cs);
		return E_FAIL;
	}
	catch(...) {
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CTBGenObj::get_Name(BSTR* pVal)
{
    *pVal = _bstr_t((LPCTSTR)m_tbGenMsg.GetName()).copy();	
    return S_OK;
}

STDMETHODIMP CTBGenObj::put_Name(BSTR newVal)
{
	try {
        m_tbGenMsg.SetName(_bstr_t(newVal));
	}
	catch(CString &cs)
	{
		SetCOMError(cs);
		return E_FAIL; 
	}
	catch(...) {
		return E_FAIL;
	}
    return S_OK;
}

