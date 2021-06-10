// ItemDetailsMsg.h: interface for the CItemDetailsMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDETAILSMSG_H__85B2F693_22E9_43A7_99D5_8DF780764866__INCLUDED_)
#define AFX_ITEMDETAILSMSG_H__85B2F693_22E9_43A7_99D5_8DF780764866__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"
#include "tchar.h"

class CItemDetailsMsg : public CTBMsgObj
{

public:
	CItemDetailsMsg();
	virtual ~CItemDetailsMsg();

	virtual void Reset();

   inline CString ItemDescription()
   {
	   return m_csItemDescription;
   }

   inline void ItemDescription(LPCTSTR szDesc)
   {
	   m_csItemDescription = szDesc;
   }

   inline CString ItemCode()
   {
	   return m_csItemCode;
   }

   inline void ItemCode(LPCTSTR szCode)
   {
	   m_csItemCode = szCode;
   }

   inline CString LinkedItemCode()
   {
	   return m_csLinkedItemCode;
   }

   inline void LinkedItemCode(LPCTSTR szCode)
   {
	   m_csLinkedItemCode = szCode;
   }

   inline CString ExtendedPrice()
   {
	   return m_csExtendedPrice;
   }

   inline long ExtendedPriceLong()
   {
	   return DecStringToLong((LPCTSTR) m_csExtendedPrice);
   }

   inline void ExtendedPrice(LPCTSTR szPrice)
   {
	   m_csExtendedPrice = szPrice;
   }

   inline void ExtendedPrice(long lPrice)
   {
	   m_csExtendedPrice.Format(_T("%d"), lPrice);
   }

   inline CString UnitPrice()
   {
	   return m_csUnitPrice;
   }

   inline long UnitPriceLong()
   {
	   return DecStringToLong((LPCTSTR)m_csUnitPrice);
   }

   inline void UnitPrice(LPCTSTR szPrice)
   {
	   m_csUnitPrice = szPrice;
   }

   inline void UnitPrice(long lPrice)
   {
	   m_csUnitPrice.Format(_T("%d"), lPrice);
   }

   inline double QuantityDbl()
   {	
		TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csQuantity, &endptr);
   }

   inline long QuantityLong()
   {	
	   return DecStringToLong(m_csQuantity);
   }

   inline CString Quantity()
   {
	   return m_csQuantity;
   }

   inline void Quantity(LPCTSTR szQty)
   {
	   m_csQuantity = szQty;
   }

   inline void Quantity(long lQty)
   {
	   m_csQuantity.Format(_T("%d"), lQty);
   }

   inline void Quantity(double dblQty)
   {
      m_csQuantity = DblToDecString(dblQty);
   }

   inline CString Weight()
   {
	   return m_csWeight;
   }

   inline long WeightLong()
   {
	   return DecStringToLong((LPCTSTR) m_csWeight);
   }

   inline double WeightDbl()
   {	
		TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csWeight, &endptr);
   }

   inline void Weight(LPCTSTR szWeight)
   {
	   m_csWeight = szWeight;
   }

   inline void Weight(double dblWght)
   {
      m_csWeight= DblToDecString(dblWght);
   }

   inline void Weight(long lWght)
   {
      m_csWeight.Format(_T("%d"), lWght);
   }

   inline bool IsCoupon()
   {
	   return m_bIsCoupon;
   }

   inline void IsCoupon(bool bVal)
   {
	   m_bIsCoupon = bVal;
   }

   inline bool NeedQuantity()
   {
	   return m_bNeedQuantity;
   }

   inline void NeedQuantity(bool bVal)
   {
	   m_bNeedQuantity = bVal;
   }	

   inline bool NeedWeight()
   {
	   return m_bNeedWeight;
   }

   inline void NeedWeight(bool bVal)
   {
	   m_bNeedWeight = bVal;
   }

   inline bool NeedPrice()
   {
	   return m_bNeedPrice;
   }

   inline void NeedPrice(bool bVal)
   {
	   m_bNeedPrice = bVal;
   }

   inline bool UnknownItem()
   {
	   return m_bUnknownItem;
   }

   inline void UnknownItem(bool bVal)
   {
	   m_bUnknownItem = bVal;
   }

   inline bool NotForSale()
   {
	   return m_bNotForSale;
   }

   inline void NotForSale(bool bVal)
   {
	   m_bNotForSale = bVal;
   }

   inline bool IsRestricted()
   {
	   return m_bIsRestricted;
   }

   inline void IsRestricted(bool bVal)
   {
	   m_bIsRestricted = bVal;
   }


   inline unsigned int RestrictedAge()
   {
	   return m_unRestrictedAge;
   }

   inline void RestrictedAge(unsigned int nAge)
   {
	   m_unRestrictedAge = nAge;
   }

   inline unsigned int TareCode()
   {
	   return m_unTareCode;
   }

   inline void TareCode(unsigned int nCode)
   {
	   m_unTareCode = nCode;
   }

   inline bool IsVoided()
   {
	   return m_bIsVoided;
   }

   inline void IsVoided(bool bVal)
   {
	   m_bIsVoided = bVal;
   }

   inline bool VisualVerify()
   {
	   return m_bVisualVerify;
   }

   inline void VisualVerify(bool bVal)
   {
	   m_bVisualVerify = bVal;
   }

   inline bool LinkedItem()
   {
	   return m_bLinkedItem;
   }

   inline void LinkedItem(bool bVal)
   {
	   m_bLinkedItem = bVal;
   }

   inline CString ErrorDescription()
   {
	   return m_csErrorDescription;
   }

   inline void ErrorDescription(LPCTSTR szDescription)
   {
	   m_csErrorDescription = szDescription;
   }

   inline unsigned int Department()
   {
	   return m_unDepartment;
   }

   inline void Department(unsigned int nDept)
   {
	   m_unDepartment = nDept;
   }

   inline unsigned int EntryID()
   {
	   return m_unEntryID;
   }

   inline void EntryID(unsigned int nID)
   {
	   m_unEntryID = nID;
   }

   inline bool PIMItem()
   {
	   return m_bPIMItem;
   }

   inline void PIMItem(bool bVal)
   {
	   m_bPIMItem = bVal;
   }

   inline bool SecurityTag()
   {
	   return m_bSecurityTag;
   }

   inline void SecurityTag(bool bVal)
   {
	   m_bSecurityTag = bVal;
   }

   void TaxExempt(bool i_bTaxExempt)
   {
		int nBit= ((0 == i_bTaxExempt) ? 0 : 1);
		m_nExtension= m_nExtension & 0xFFFFFFFE;
		m_nExtension= m_nExtension | nBit;
   }

   bool TaxExempt()
   {		
		int nBit= m_nExtension & 0x00000001;
		return (0 != nBit);
   }

   void EndorsementRequired(bool i_bEndorsementRequired)
   {
		int nBit= ((0 == i_bEndorsementRequired) ? 0 : 1);
		nBit = nBit << 1;
		m_nExtension= m_nExtension & 0xFFFFFFFD;
		m_nExtension= m_nExtension | nBit;
   }

   bool EndorsementRequired()
   {		
		int nBit= m_nExtension & 0x00000002;
		return (0 != nBit);
   }

   inline double UnitQuantityDbl()
   {	
		TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csUnitQuantity, &endptr);
   }

   inline long UnitQuantityLong()
   {	
	   return DecStringToLong(m_csUnitQuantity);
   }

   inline CString UnitQuantity()
   {
	   return m_csUnitQuantity;
   }

   inline void UnitQuantity(LPCTSTR szUnitQty)
   {
	   m_csUnitQuantity = szUnitQty;
   }

   inline void UnitQuantity(long lUnitQty)
   {
	   m_csUnitQuantity.Format(_T("%d"), lUnitQty);
   }

   inline void UnitQuantity(double dblUnitQty)
   {
      m_csUnitQuantity = DblToDecString(dblUnitQty);
   }


protected:
	bool m_bSecurityTag;
	bool m_bPIMItem;
	unsigned int m_unEntryID;
	unsigned int m_unDepartment;
	CString m_csErrorDescription;
	bool m_bLinkedItem;
	bool m_bVisualVerify;
	bool m_bIsVoided;
	unsigned int m_unTareCode;
	unsigned int m_unRestrictedAge;
	bool m_bIsRestricted;
	bool m_bNotForSale;
	bool m_bUnknownItem;
	bool m_bNeedPrice;
	bool m_bNeedWeight;
	bool m_bNeedQuantity;
	bool m_bIsCoupon;
	CString m_csWeight;
	CString m_csQuantity;
	CString m_csUnitPrice;
	CString m_csExtendedPrice;
	CString m_csItemCode;
	CString m_csItemDescription;
    CString m_csUnitQuantity;
    CString m_csLinkedItemCode;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);
};

#endif // !defined(AFX_ITEMDETAILSMSG_H__85B2F693_22E9_43A7_99D5_8DF780764866__INCLUDED_)
