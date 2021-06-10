// KeyboardObj.cpp : Implementation of CKeyboardObj
#include "stdafx.h"
#include "KeyboardObj.h"
#include "safearrayvariant.h"

#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CKeyboardObj, m_KeyboardMsg)
#pragma warning(default:4311) 


// CKeyboardObj

STDMETHODIMP CKeyboardObj::get_KeyCode(LONG* pVal)
{
	*pVal= m_KeyboardMsg.KeyCode();
	return S_OK;
}

STDMETHODIMP CKeyboardObj::put_KeyCode(LONG newVal)
{
	m_KeyboardMsg.KeyCode((USHORT)newVal);
	return S_OK;
}

STDMETHODIMP CKeyboardObj::get_KeyEvent(KeyEventType* pVal)
{
	*pVal= (KeyEventType)m_KeyboardMsg.KeyEvent();
	return S_OK;
}

STDMETHODIMP CKeyboardObj::put_KeyEvent(KeyEventType newVal)
{
	m_KeyboardMsg.KeyEvent((CKeyboardMsg::KeyEventType)newVal);
	return S_OK;
}

STDMETHODIMP CKeyboardObj::get_CombinationKeys(CombinationKeyFlags* pVal)
{
	*pVal= (CombinationKeyFlags)m_KeyboardMsg.CombinationKeys();
	return S_OK;
}

STDMETHODIMP CKeyboardObj::put_CombinationKeys(CombinationKeyFlags newVal)
{
	m_KeyboardMsg.CombinationKeys((CKeyboardMsg::CombinationKeyFlags)newVal);
	return S_OK;
}
