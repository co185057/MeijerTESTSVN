// TenderDetailsMsg.h: interface for the CTenderDetailsMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TENDERDETAILSMSG_H__5DB61005_141C_441E_AFEC_50183E44A1A5__INCLUDED_)
#define AFX_TENDERDETAILSMSG_H__5DB61005_141C_441E_AFEC_50183E44A1A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"
#include "tchar.h"

class CTenderDetailsMsg : public CTBMsgObj
{
public:
	CTenderDetailsMsg();
	virtual ~CTenderDetailsMsg();

	inline CString CTenderDetailsMsg::TenderDescription()
   {
	   return m_csTenderDescription;
   }

   inline void TenderDescription(const CString &csDesc)
   {
	   m_csTenderDescription = csDesc;
   }

   inline int TenderType()
   {
	   return m_tbTenderType;
   }

   inline void TenderType(int nType)
   {
	   m_tbTenderType = nType;
   }

   inline double TenderAmountDbl()
   {
		TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csTenderAmount, &endptr);
   }

   inline long TenderAmountLong()
   {
		return DecStringToLong((LPCTSTR) m_csTenderAmount);
	}
   
	inline CString TenderAmount()
   {
	   return m_csTenderAmount;
   }

   inline void TenderAmount(double dbl)
   {
	   m_csTenderAmount = DblToDecString(dbl);
   }

   inline void TenderAmount(long lval)
   {
	   m_csTenderAmount.Format(_T("%d"), lval);
   }

   inline void TenderAmount(const CString &csAmount)
   {
	   m_csTenderAmount = csAmount;
   }

   inline CString Balance()
   {
	   return m_csBalance;
   }

   inline double BalanceDbl()
   {
		TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csBalance, &endptr);
   }

   inline long BalanceLong()
   {
			return DecStringToLong((LPCTSTR) m_csBalance);
	}

   inline void Balance(double dbl)
   {
	   m_csBalance = DblToDecString(dbl);
   }

   inline void Balance(long lval)
   {
	   m_csBalance.Format(_T("%d"), lval);
   }

   inline void Balance(const CString &csBalance)
   {
	   m_csBalance = csBalance;
   }

   inline CString ChangeDue()
   {
	   return m_csChangeDue;
   }

   inline double ChangeDueDbl()
   {
		TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csChangeDue, &endptr);
   }

   inline long ChangeDueLong()
   {
			return DecStringToLong((LPCTSTR) m_csChangeDue);
	}

   inline void ChangeDue(const CString &csChange)
   {
	   m_csChangeDue = csChange;
   }

   inline void ChangeDue(double dbl)
   {
	   m_csChangeDue = DblToDecString(dbl);
   }

   inline void ChangeDue(long lval)
   {
	   m_csChangeDue.Format(_T("%d"), lval);
   }

   inline bool TenderAccepted()
   {
	   return m_bTenderAccepted;
   }

   inline void TenderAccepted(bool bVal)
   {
	   m_bTenderAccepted = bVal;
   }

   inline bool TenderVoided()
   {
	   return m_bTenderVoided;
   }

   inline void TenderVoided(bool bVal)
   {
	   m_bTenderVoided = bVal;
   }

   inline CString CardRemainingBalance()
   {
	   return m_csCardRemainingBalance;
   }

   inline double CardRemainingBalanceDbl()
   {
		TCHAR *endptr;
	   return _tcstod((LPCTSTR) m_csCardRemainingBalance, &endptr);
   }

   inline void CardRemainingBalance(double dbl)
   {
	   m_csCardRemainingBalance = DblToDecString(dbl);
   }

   inline void CardRemainingBalance(long lval)
   {
	   m_csCardRemainingBalance.Format(_T("%d"), lval);
   }

   inline void CardRemainingBalance(const CString &csBalance)
   {
	   m_csCardRemainingBalance = csBalance;
   }

   inline void ErrorDescription(const CString &csDescription)
   {
      m_csErrorDescription= csDescription;
   }

   inline CString ErrorDescription()
   {
      return m_csErrorDescription;
   }

	virtual void Reset();

protected:
	bool m_bTenderVoided;
	bool m_bTenderAccepted;
	int m_tbTenderType;
	CString m_csChangeDue;
	CString m_csCardRemainingBalance;
	CString m_csBalance;
	CString m_csTenderAmount;
	CString m_csTenderDescription;
   CString m_csErrorDescription;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);

};

#endif // !defined(AFX_TENDERDETAILSMSG_H__5DB61005_141C_441E_AFEC_50183E44A1A5__INCLUDED_)
