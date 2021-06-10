// ScaleObj.cpp : Implementation of CScaleObj
#include "stdafx.h"
#include "ScaleObj.h"

#include "safearrayVariant.h"
#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CScaleObj, m_ScaleMsg)
#pragma warning(default:4311) 

// CScaleObj

STDMETHODIMP CScaleObj::get_Weight(LONG* pVal)
{
	*pVal= m_ScaleMsg.Weight();
	return S_OK;
}

STDMETHODIMP CScaleObj::put_Weight(LONG newVal)
{
	m_ScaleMsg.Weight(newVal);
	return S_OK;
}
