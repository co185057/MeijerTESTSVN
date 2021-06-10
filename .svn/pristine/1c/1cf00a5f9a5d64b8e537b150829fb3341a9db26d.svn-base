// ScannerMsg.h: interface for the CScannerMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCANNERMSG_H__3E4CDE15_A81A_4D34_9847_48DDB2C8196D__INCLUDED_)
#define AFX_SCANNERMSG_H__3E4CDE15_A81A_4D34_9847_48DDB2C8196D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

#define BT_JAN8 BT_EAN8
#define BT_JAN13 BT_EAN13

class CScannerMsg : public CTBMsgObj
{
public:
   typedef enum { BT_UPCA      = 0x01,
                  BT_UPCA_SUP  = 0X02,
                  BT_UPCE      = 0x03,
                  BT_UPCE_SUP  = 0x04,
                  BT_UPCD1     = 0x05,
                  BT_UPCD2     = 0x06,
                  BT_UPCD3     = 0x07,
                  BT_UPCD4     = 0x08,
                  BT_UPCD5     = 0x09,
                  BT_EAN8      = 0x0A,
                  BT_EAN8_SUP  = 0x0B,
                  BT_EAN13     = 0x0C,
                  BT_EAN13_SUP = 0x0D,
                  BT_EAN128    = 0x0E,
                  BT_STD25     = 0x0F,
                  BT_CODABAR   = 0x10,
                  BT_CODE39    = 0x11,
                  BT_CODE93    = 0x12,
                  BT_CODE128   = 0x13,
                  BT_OCRA      = 0x14,
                  BT_OCRB      = 0x14,
                  BT_GS1DATABAR = 0x20,
                  BT_GS1DATABAR_E = 0x21,
                  BT_PDF417    = 0x40,
                  BT_MAXICODE  = 0x41,
                  BT_A         = 0x65,
                  BT_B         = 0x6E,
                  BT_UNKNOWN   = 0x99
   } BarcodeTypes;
                  
public:
	CScannerMsg();
	virtual ~CScannerMsg();

   inline BarcodeTypes BarcodeType()
   {
      return m_BarcodeType;
   }

   inline void BarcodeType(BarcodeTypes dwType)
   {
      m_BarcodeType = dwType;
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
   BarcodeTypes m_BarcodeType;
   CString m_csData;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);
};

#endif // !defined(AFX_SCANNERMSG_H__3E4CDE15_A81A_4D34_9847_48DDB2C8196D__INCLUDED_)
