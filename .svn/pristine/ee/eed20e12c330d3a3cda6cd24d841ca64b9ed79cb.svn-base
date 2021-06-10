// CashDrawerObj.cpp : Implementation of CCashDrawerObj
#include "stdafx.h"
#include "CashDrawerObj.h"
#include "safearrayvariant.h"
#include "ImplementMacros.h"

// CCashDrawerObj
#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CCashDrawerObj, m_CashDrawerMsg)
#pragma warning(default:4311) 

STDMETHODIMP CCashDrawerObj::get_Station(CashDrawerStations* pVal)
{
	*pVal= (CashDrawerStations)m_CashDrawerMsg.Station();
	return S_OK;
}

STDMETHODIMP CCashDrawerObj::put_Station(CashDrawerStations newVal)
{
	m_CashDrawerMsg.Station((CCashDrawerMsg::CashDrawerStations)newVal);
	return S_OK;
}

STDMETHODIMP CCashDrawerObj::get_Status(CashDrawerStates* pVal)
{
	*pVal= (CashDrawerStates)m_CashDrawerMsg.Status();
	return S_OK;
}

STDMETHODIMP CCashDrawerObj::put_Status(CashDrawerStates newVal)
{
	m_CashDrawerMsg.Status((CCashDrawerMsg::CashDrawerStates )newVal);
	return S_OK;
}


