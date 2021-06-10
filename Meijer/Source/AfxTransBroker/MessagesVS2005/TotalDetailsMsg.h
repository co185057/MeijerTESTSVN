// TotalDetailsMsg.h: interface for the CTotalDetailsMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOTALDETAILSMSG_H__C28D37BA_F12B_461D_B781_782E1FF7B352__INCLUDED_)
#define AFX_TOTALDETAILSMSG_H__C28D37BA_F12B_461D_B781_782E1FF7B352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tchar.h"
#include "TBMsgObj.h"

class CTotalDetailsMsg : public CTBMsgObj
{
public:
	CTotalDetailsMsg();
	virtual ~CTotalDetailsMsg();

	virtual void Reset();

   inline CString TaxDue()
   {
	   return m_csTaxDue;
   }

   inline void TaxDue(const CString &csTax)
   {
	   m_csTaxDue = csTax;
   }

   inline double TaxDueDbl()
   {
		TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csTaxDue, &endptr);
   }
	inline long TaxDueLong()
   {
		return DecStringToLong((LPCTSTR) m_csTaxDue);
	}

   inline void TaxDue(double dbl)
   {
	   m_csTaxDue = DblToDecString(dbl);
   }

   inline void TaxDue(long lval)
   {
	   m_csTaxDue.Format(_T("%d"), lval);
   }

   inline double TotalDueDbl()
   {
   	TCHAR *endptr;
	   return _tcstod((LPCTSTR)m_csTotalDue , &endptr);
	}

   inline long TotalDueLong()
   {
		return DecStringToLong((LPCTSTR) m_csTotalDue);
	}

   inline void TotalDue(const CString &csTotal)
   {
	   m_csTotalDue = csTotal;
   }
   inline CString TotalDue()
   {
	   return m_csTotalDue;
   }
   inline void TotalDue(double dbl)
   {
	   m_csTotalDue = DblToDecString(dbl);
   }

   inline void TotalDue(long lval)
   {
	   m_csTotalDue.Format(_T("%d"), lval);
   }

   inline double TotalDiscountDbl()
   {
		TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csTotalDiscount, &endptr);
   }

   inline long TotalDiscountLong()
   {
		return DecStringToLong((LPCTSTR) m_csTotalDiscount);
	}

   inline void TotalDiscount(const CString &csDiscount)
   {
	   m_csTotalDiscount = csDiscount;
   }
   inline CString TotalDiscount()
   {
	   return m_csTotalDiscount;
   }

   inline void TotalDiscount(double dbl)
   {
	   m_csTotalDiscount = DblToDecString(dbl);
   }
	
	inline void TotalDiscount(long lval)
   {
	   m_csTotalDiscount.Format(_T("%d"), lval);
   }

   inline CString ErrorDescription()
   {
	   return m_csErrorDescription;
   }

   inline void ErrorDescription(const CString &csDesc)
   {
	   m_csErrorDescription = csDesc;
   }

   inline CString FoodStampAmount()
   {
	   return m_csFoodStampAmount;
   }

   inline void FoodStampAmount(const CString &csAmount)
   {
	   m_csFoodStampAmount = csAmount;
   }
   inline double FoodStampAmountDbl()
   {
   	TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csFoodStampAmount, &endptr);
	}

   inline long FoodStampAmountLong()
   {
		return DecStringToLong((LPCTSTR) m_csFoodStampAmount);
	}

   inline void FoodStampAmount(double dbl)
   {
	   m_csFoodStampAmount = DblToDecString(dbl);
   }

   inline void FoodStampAmount(long lval)
   {
	   m_csFoodStampAmount.Format(_T("%d"), lval);
   }

   inline unsigned int RewardPoints()
   {
	   return m_unRewardPoints;
   }

   inline void RewardPoints(unsigned int unPoints)
   {
	   m_unRewardPoints = unPoints;
   }

protected:
	unsigned int m_unRewardPoints;
	CString m_csFoodStampAmount;
	CString m_csErrorDescription;
	CString m_csTotalDiscount;
	CString m_csTotalDue;
	CString m_csTaxDue;

protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);

};

#endif // !defined(AFX_TOTALDETAILSMSG_H__C28D37BA_F12B_461D_B781_782E1FF7B352__INCLUDED_)
