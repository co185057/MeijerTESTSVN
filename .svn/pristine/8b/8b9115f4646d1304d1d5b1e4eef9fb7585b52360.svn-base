// ExpiringInt.h: interface for the CExpiringInt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPIRINGINT_H__B7D50FB2_2518_4047_BA26_C1A80B88893C__INCLUDED_)
#define AFX_EXPIRINGINT_H__B7D50FB2_2518_4047_BA26_C1A80B88893C__INCLUDED_

#pragma once
#include "sys/timeb.h"
#include "time.h"

class CExpiringInt  
{
public:
	CExpiringInt(unsigned long MilliSeconds=0){
		m_val=0;
		m_dwTickStart = GetTickCount();
		m_expireMilliSec=MilliSeconds;
	};
	virtual ~CExpiringInt(){};

private:
	int m_val;
	unsigned long m_expireMilliSec;
	unsigned long	m_dwTickStart;
	unsigned long	m_dwCurTime;

public:

   unsigned long m_timediff;
	CExpiringInt& operator =(const int newval) {
		m_dwTickStart = GetTickCount();
		m_val=newval;
		return *this;
	};
	bool operator ==(int newval) {
		m_dwCurTime = GetTickCount();
		m_timediff = m_dwCurTime - m_dwTickStart;
		if (m_timediff > m_expireMilliSec)
			return (newval==0);
		return (newval==m_val);
	};
	operator int(){
		m_dwCurTime = GetTickCount();
		m_timediff = m_dwCurTime - m_dwTickStart;
		if (m_timediff > m_expireMilliSec)
			return 0;
		return m_val;
	}

   void SetExpiration(unsigned int MilliSeconds) {
		m_expireMilliSec=MilliSeconds;
   };

   void Start()
	{
	    m_dwTickStart = GetTickCount();
		return;
	};

   long Elapsed() {
	   m_dwCurTime = GetTickCount();
       return (m_dwCurTime - m_dwTickStart);
   }
   unsigned long Remaining() {
	   m_dwCurTime = GetTickCount();
      long lval= m_expireMilliSec - (m_dwCurTime - m_dwTickStart);
      if (lval< 0)
         lval=0;
      return lval;
   }
};

#endif // !defined(AFX_EXPIRINGINT_H__B7D50FB2_2518_4047_BA26_C1A80B88893C__INCLUDED_)
