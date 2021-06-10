// PinpadMsg.h: interface for the CPinpadMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PINPADMSG_H__D5FA1D6D_F39F_414E_9A30_01258739CE88__INCLUDED_)
#define AFX_PINPADMSG_H__D5FA1D6D_F39F_414E_9A30_01258739CE88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

class CPinpadMsg : public CTBMsgObj
{
public:
   typedef enum { TT_CREDIT        = 0x02,
                  TT_DEBIT         = 0x03,
                  TT_LOYALTY       = 0x04,
                  TT_DEBIT_SAVINGS = 0x05,
                  TT_EBT           = 0x07,
                  TT_GIFT          = 0x09,
                  TT_UNDEFINED     = 0x99
   } TransactionTypes;

   typedef enum { PE_MASTER_SESSION = 0x00,
                  PE_DUKPT          = 0x01,
                  PE_UNDEFINED      = 0x99
   } PinEncodingList;

   typedef enum { PB_ANSI      = 0x00,
                  PB_DIEBOLD   = 0x01,
                  PB_UNDEFINED = 0x99
   } PinBlockFormats;

public:
	CPinpadMsg();
	virtual ~CPinpadMsg();

   inline CString PrimaryAccountNumber()
   {
      return m_csPrimaryAccountNumber;
   }

   inline void PrimaryAccountNumber(const CString &csNum)
   { 
      m_csPrimaryAccountNumber = csNum;
   }

   inline TransactionTypes TransactionType()
   {
      return m_TransactionType;
   }

   inline void TransactionType(TransactionTypes nType)
   {
      m_TransactionType = nType;
   }

   inline PinEncodingList PinEncoding()
   {
      return m_PinEncoding;
   }

   inline void PinEncoding(PinEncodingList nEncoding)
   {
      m_PinEncoding = nEncoding;
   }

   inline CString WorkingKey()
   {
      return m_csWorkingKey;
   }

   inline void WorkingKey(const CString &csKey)
   {
      m_csWorkingKey = csKey;
   }

   inline PinBlockFormats PinBlockFormat()
   {
      return m_PinBlockFormat;
   }

   inline void PinBlockFormat(PinBlockFormats nFormat)
   {
      m_PinBlockFormat = nFormat;
   }

   inline CString Track1Data()
   {
      return m_csTrack1Data;
   }

   inline void Track1Data(const CString &csData)
   {
      m_csTrack1Data = csData;
   }

   inline CString Track2Data()
   {
      return m_csTrack2Data;
   }

   inline void Track2Data(const CString &csData)
   {
      m_csTrack2Data = csData;
   }

   inline CString Track3Data()
   {
      return m_csTrack3Data;
   }

   inline void Track3Data(const CString &csData)
   {
      m_csTrack3Data = csData;
   }

   inline void SetTrack123(BYTE l1, BYTE l2, BYTE l3, const CString &track123)
   {
      m_csTrack1Data= track123.Left(l1);
      m_csTrack2Data= track123.Mid(l1, l2);
      m_csTrack1Data= track123.Mid(l1+ l2, l3);
   }
protected:
   CString m_csPrimaryAccountNumber;
   TransactionTypes m_TransactionType;
   PinEncodingList m_PinEncoding;
   CString m_csWorkingKey;
   PinBlockFormats m_PinBlockFormat;
   CString m_csTrack1Data;
   CString m_csTrack2Data;
   CString m_csTrack3Data;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);
};

#endif // !defined(AFX_PINPADMSG_H__D5FA1D6D_F39F_414E_9A30_01258739CE88__INCLUDED_)
