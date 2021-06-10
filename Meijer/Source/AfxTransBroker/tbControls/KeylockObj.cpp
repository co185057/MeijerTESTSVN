// CKeylockObj.cpp : Implementation of CCKeylockObj
#include "stdafx.h"
#include "KeylockObj.h"
#include "safearrayvariant.h"

#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CKeylockObj, m_KeylockDetailMsg)
#pragma warning(default:4311) 


// CCKeylockObj

STDMETHODIMP CKeylockObj::get_Position(BYTE* pVal)
{
   *pVal= m_KeylockDetailMsg.Position();
   return S_OK;
}

STDMETHODIMP CKeylockObj::put_Position(BYTE newVal)
{
   m_KeylockDetailMsg.Position(newVal);
   return S_OK;
}
