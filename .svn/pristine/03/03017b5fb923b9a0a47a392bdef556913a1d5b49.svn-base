// RewardObj.cpp : Implementation of CRewardObj
#include "stdafx.h"
#include "RewardObj.h"
#include "safeArrayVariant.h"

#include "ImplementMacros.h"

// CCashDrawerObj
#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CRewardObj, m_rewardMsg)
#pragma warning(default:4311) 


STDMETHODIMP CRewardObj::get_RewardValue(LONG* pVal)
{
	*pVal= m_rewardMsg.RewardValue();
	return S_OK;
}

STDMETHODIMP CRewardObj::put_RewardValue(LONG newVal)
{
	m_rewardMsg.RewardValue(newVal);
	return S_OK;
}

STDMETHODIMP CRewardObj::get_RewardPoints(LONG* pVal)
{
	*pVal= m_rewardMsg.RewardPoints();
	return S_OK;
}

STDMETHODIMP CRewardObj::put_RewardPoints(LONG newVal)
{
	m_rewardMsg.RewardPoints(newVal);
	return S_OK;
}

STDMETHODIMP CRewardObj::get_RewardPosition(tRewardLinePosType* pVal)
{
	*pVal= (tRewardLinePosType) m_rewardMsg.RewardLocation();
	return S_OK;
}

STDMETHODIMP CRewardObj::put_RewardPosition(tRewardLinePosType newVal)
{
	m_rewardMsg.RewardLocation((CRewardLineDetailsMsg::tRewardLinePosType)newVal);
	return S_OK;
}

STDMETHODIMP CRewardObj::get_IsVoided(BYTE* pVal)
{
	*pVal= m_rewardMsg.IsVoided();
	return S_OK;
}

STDMETHODIMP CRewardObj::put_IsVoided(BYTE newVal)
{
	m_rewardMsg.IsVoided(newVal != 0);
	return S_OK;
}

STDMETHODIMP CRewardObj::get_EntryId(LONG* pVal)
{
	*pVal= m_rewardMsg.EntryID();
	return S_OK;
}

STDMETHODIMP CRewardObj::put_EntryId(LONG newVal)
{
	m_rewardMsg.EntryID(newVal);
	return S_OK;
}

STDMETHODIMP CRewardObj::get_UpdateRewardTotal(BYTE* pVal)
{
	*pVal= m_rewardMsg.UpdateRewardTotal();
	return S_OK;
}

STDMETHODIMP CRewardObj::put_UpdateRewardTotal(BYTE newVal)
{
	m_rewardMsg.UpdateRewardTotal(newVal!=0);
	return S_OK;
}

STDMETHODIMP CRewardObj::get_Description(BSTR* pVal)
{
	CComBSTR csVal(m_rewardMsg.RewardDescription());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CRewardObj::put_Description(BSTR newVal)
{
	m_rewardMsg.RewardDescription(CString(newVal));
	return S_OK;
}

STDMETHODIMP CRewardObj::get_AssociatedEntryId(LONG* pVal)
{
	*pVal= m_rewardMsg.AssociatedEntryID();
	return S_OK;
}

STDMETHODIMP CRewardObj::put_AssociatedEntryId(LONG newVal)
{
	m_rewardMsg.AssociatedEntryID(newVal);
	return S_OK;
}

STDMETHODIMP CRewardObj::get_ShowValue(SHORT* pVal)
{
	*pVal= m_rewardMsg.ShowRewardPoints();
	return S_OK;
}

STDMETHODIMP CRewardObj::put_ShowValue(SHORT newVal)
{
	m_rewardMsg.ShowRewardPoints(newVal ? true:false);
	return S_OK;
}
