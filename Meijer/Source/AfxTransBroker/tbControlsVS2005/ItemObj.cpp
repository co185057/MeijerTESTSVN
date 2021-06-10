// ItemObj.cpp : Implementation of CItemObj
#include "stdafx.h"
#include "ItemObj.h"
#include "safearrayvariant.h"

#include "ImplementMacros.h"
#include ".\itemobj.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CItemObj, m_ItemDetailsMsg)
#pragma warning(default:4311) 

// 

STDMETHODIMP CItemObj::get_IsCoupon(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.IsCoupon();
   return S_OK;
}

STDMETHODIMP CItemObj::put_IsCoupon(BYTE newVal)
{
   m_ItemDetailsMsg.IsCoupon(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_IsRestricted(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.IsRestricted();
   return S_OK;
}

STDMETHODIMP CItemObj::put_IsRestricted(BYTE newVal)
{
   m_ItemDetailsMsg.IsRestricted(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_IsVoided(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.IsVoided();
   return S_OK;
}

STDMETHODIMP CItemObj::put_IsVoided(BYTE newVal)
{
   m_ItemDetailsMsg.IsVoided(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_LinkedItem(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.LinkedItem();
   return S_OK;
}

STDMETHODIMP CItemObj::put_LinkedItem(BYTE newVal)
{
   m_ItemDetailsMsg.LinkedItem(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_NeedPrice(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.NeedQuantity();
   return S_OK;
}

STDMETHODIMP CItemObj::put_NeedPrice(BYTE newVal)
{
   m_ItemDetailsMsg.NeedPrice(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_NeedQuantity(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.NeedQuantity();
   return S_OK;
}

STDMETHODIMP CItemObj::put_NeedQuantity(BYTE newVal)
{
   m_ItemDetailsMsg.NeedQuantity(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_NeedWeight(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.NeedWeight();
   return S_OK;
}

STDMETHODIMP CItemObj::put_NeedWeight(BYTE newVal)
{
   m_ItemDetailsMsg.NeedWeight(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_NotForSale(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.NotForSale();
   return S_OK;
}

STDMETHODIMP CItemObj::put_NotForSale(BYTE newVal)
{
   m_ItemDetailsMsg.NotForSale(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_PIMItem(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.PIMItem();
   return S_OK;
}

STDMETHODIMP CItemObj::put_PIMItem(BYTE newVal)
{
   m_ItemDetailsMsg.PIMItem(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_SecurityTag(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.SecurityTag();
   return S_OK;
}

STDMETHODIMP CItemObj::put_SecurityTag(BYTE newVal)
{
   m_ItemDetailsMsg.SecurityTag(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_UnknownItem(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.UnknownItem();
   return S_OK;
}

STDMETHODIMP CItemObj::put_UnknownItem(BYTE newVal)
{
   m_ItemDetailsMsg.UnknownItem(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_VisualVerify(BYTE* pVal)
{
   *pVal= m_ItemDetailsMsg.VisualVerify();
   return S_OK;
}

STDMETHODIMP CItemObj::put_VisualVerify(BYTE newVal)
{
   m_ItemDetailsMsg.VisualVerify(newVal != 0);
   return S_OK;
}

STDMETHODIMP CItemObj::get_ErrorDescription(BSTR* pVal)
{
   CComBSTR csVal(m_ItemDetailsMsg.ErrorDescription());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CItemObj::put_ErrorDescription(BSTR newVal)
{
   m_ItemDetailsMsg.ErrorDescription(CString(newVal));
   return S_OK;
}

STDMETHODIMP CItemObj::get_ExtendedPrice(BSTR* pVal)
{
   CComBSTR csVal(m_ItemDetailsMsg.ExtendedPrice());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CItemObj::put_ExtendedPrice(BSTR newVal)
{
   m_ItemDetailsMsg.ExtendedPrice(CString(newVal));
   return S_OK;
}

STDMETHODIMP CItemObj::get_ItemCode(BSTR* pVal)
{
   CComBSTR csVal(m_ItemDetailsMsg.ItemCode());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CItemObj::put_ItemCode(BSTR newVal)
{
   m_ItemDetailsMsg.ItemCode(CString(newVal));
   return S_OK;
}

STDMETHODIMP CItemObj::get_ItemDescription(BSTR* pVal)
{
   CComBSTR csVal(m_ItemDetailsMsg.ItemDescription());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CItemObj::put_ItemDescription(BSTR newVal)
{
   m_ItemDetailsMsg.ItemDescription(CString(newVal));
   return S_OK;
}

STDMETHODIMP CItemObj::get_Quantity(BSTR* pVal)
{
   CComBSTR csVal(m_ItemDetailsMsg.Quantity());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CItemObj::put_Quantity(BSTR newVal)
{
   m_ItemDetailsMsg.Quantity(CString(newVal));
   return S_OK;
}

STDMETHODIMP CItemObj::get_UnitPrice(BSTR* pVal)
{
   CComBSTR csVal(m_ItemDetailsMsg.UnitPrice());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CItemObj::put_UnitPrice(BSTR newVal)
{
   m_ItemDetailsMsg.UnitPrice(CString(newVal));
   return S_OK;
}

STDMETHODIMP CItemObj::get_Weight(BSTR* pVal)
{
   CComBSTR csVal(m_ItemDetailsMsg.Weight());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CItemObj::put_Weight(BSTR newVal)
{
   m_ItemDetailsMsg.Weight(CString(newVal));
   return S_OK;
}

STDMETHODIMP CItemObj::get_Department(LONG* pVal)
{
   *pVal= m_ItemDetailsMsg.Department();
   return S_OK;
}

STDMETHODIMP CItemObj::put_Department(LONG newVal)
{
   m_ItemDetailsMsg.Department(newVal);
   return S_OK;
}

STDMETHODIMP CItemObj::get_EntryID(LONG* pVal)
{
   *pVal=m_ItemDetailsMsg.EntryID();
   return S_OK;
}

STDMETHODIMP CItemObj::put_EntryID(LONG newVal)
{
   m_ItemDetailsMsg.EntryID(newVal);
   return S_OK;
}

STDMETHODIMP CItemObj::get_RestrictedAge(LONG* pVal)
{
   *pVal=m_ItemDetailsMsg.RestrictedAge();
   return S_OK;
}

STDMETHODIMP CItemObj::put_RestrictedAge(LONG newVal)
{
   m_ItemDetailsMsg.RestrictedAge(newVal);
   return S_OK;
}

STDMETHODIMP CItemObj::get_TareCode(LONG* pVal)
{
   *pVal= m_ItemDetailsMsg.TareCode();
   return S_OK;
}

STDMETHODIMP CItemObj::put_TareCode(LONG newVal)
{
   m_ItemDetailsMsg.TareCode(newVal);
   return S_OK;
}

STDMETHODIMP CItemObj::get_TaxExempt(BYTE* pVal)
{
	*pVal= m_ItemDetailsMsg.TaxExempt();
	return S_OK;
}

STDMETHODIMP CItemObj::put_TaxExempt(BYTE newVal)
{
	m_ItemDetailsMsg.TaxExempt((0 != newVal));
	return S_OK;
}

STDMETHODIMP CItemObj::get_EndorsementRequired(BYTE* pVal)
{
	*pVal= m_ItemDetailsMsg.EndorsementRequired();
	return S_OK;
}

STDMETHODIMP CItemObj::put_EndorsementRequired(BYTE newVal)
{
	m_ItemDetailsMsg.EndorsementRequired((0 != newVal));
	return S_OK;
}

STDMETHODIMP CItemObj::Reset(void)
{
	m_ItemDetailsMsg.Reset();
	return S_OK;
}

STDMETHODIMP CItemObj::get_UnitQuantity(BSTR* pVal)
{
   CComBSTR csVal(m_ItemDetailsMsg.UnitQuantity());
	csVal.CopyTo(pVal);
   return S_OK;
}

STDMETHODIMP CItemObj::put_UnitQuantity(BSTR newVal)
{
   m_ItemDetailsMsg.UnitQuantity(CString(newVal));
   return S_OK;
}
