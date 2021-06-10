// MicrMsg.h: interface for the CMicrMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICRMSG_H__8E5D07A2_74FF_4355_B9EC_ABE6CE468F7C__INCLUDED_)
#define AFX_MICRMSG_H__8E5D07A2_74FF_4355_B9EC_ABE6CE468F7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

class CMicrMsg : public CTBMsgObj 
{
public:
   typedef enum { CHECK_PERSONAL = 0x01,
                  CHECK_BUSINESS = 0x02,
                  CHECK_UNKNOWN  = 0x99
   } CheckTypes;

   typedef enum { CC_US  = 0x01,
                  CC_CAN = 0x02,
                  CC_MEX = 0x03,
                  CC_UNK = 0x99
   } CountryCodes;

public:
	CMicrMsg();
	virtual ~CMicrMsg();

   inline CString TransitNumber()
   {
      return m_csTransitNumber;
   }

   inline void TransitNumber(const CString &csNumber)
   {
	  //Transit Number is 9 characters
      m_csTransitNumber = csNumber.Left(9);
   }

   inline CString AccountNumber()
   {
      return m_csAccountNumber;
   }

   inline void AccountNumber(const CString &csNumber)
   {
      m_csAccountNumber = csNumber;
   }

   inline CString BankNumber()
   {
      return m_csBankNumber;
   }

   inline void BankNumber(const CString &csNumber)
   {
	  // BankNumber is 5 characters
      m_csBankNumber = csNumber.Left(5);
   }

   inline CString SerialNumber()
   {
      return m_csSerialNumber;
   }

   inline void SerialNumber(const CString &csNumber)
   {
      m_csSerialNumber = csNumber;
   }

   inline CheckTypes CheckType()
   {
      return m_CheckType;
   }

   inline void CheckType(CheckTypes nType)
   {
      m_CheckType = nType;
   }

   inline CountryCodes CountryCode()
   {
      return m_CountryCode;
   }

   inline void CountryCode(CountryCodes nCode)
   {
      m_CountryCode = nCode;
   }

   inline CString Data()
   {
      return m_csData;
   }

   inline void Data(const CString &csData)
   {
      m_csData = csData;
   }
protected:
   CString m_csTransitNumber;
   CString m_csAccountNumber;
   CString m_csBankNumber;
   CString m_csSerialNumber;
   CheckTypes m_CheckType;
   CountryCodes m_CountryCode;
   CString m_csData;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);
};

#endif // !defined(AFX_MICRMSG_H__8E5D07A2_74FF_4355_B9EC_ABE6CE468F7C__INCLUDED_)
