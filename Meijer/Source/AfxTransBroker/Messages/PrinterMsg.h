// PrinterMsg.h: interface for the CPrinterMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTERMSG_H__42B90DD7_51ED_4FA0_9193_6EBF52E75215__INCLUDED_)
#define AFX_PRINTERMSG_H__42B90DD7_51ED_4FA0_9193_6EBF52E75215__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

#define PTR_STATION_JOURNAL 0x01
#define PTR_STATION_RECEIPT 0x02
#define PTR_STATION_SLIP    0x04

#define PTR_FUNCTION_CUTPAPER   1
#define PTR_FUNCTION_PRINT      2
#define PTR_FUNCTION_BARCODE    3
#define PTR_FUNCTION_BITMAP     4
#define PTR_FUNCTION_SETBITMAP  5
#define PTR_FUNCTION_SETLOGO    6
#define PTR_FUNCTION_SLIPSTATUS 7

#define SLIPSTATUS_BEGININSERTION 0
#define SLIPSTATUS_ENDINSERTION   1
#define SLIPSTATUS_BEGINREMOVAL   2
#define SLIPSTATUS_ENDREMOVAL     3


class CPrinterMsg : public CTBMsgObj
{
public:
	CPrinterMsg(TBDEVICEID devId=TB_DEV_PRINTERRECEIPT);
	virtual ~CPrinterMsg();

   inline BYTE Function()
   {
      return m_cFunction;
   }

   inline void Function(BYTE nFunc)
   {
      m_cFunction = nFunc;
   }

   inline CString Arguments()
   {
      return m_csArguments;
   }

   inline void Arguments(const CString &csArg)
   {
      m_csArguments = csArg;
   }

   inline unsigned short Height()
   {
      return m_unHeight;
   }

   inline void Height(unsigned short nHeight)
   {
      m_unHeight = nHeight;
   }

   inline unsigned short Width()
   {
      return m_unWidth;
   }

   inline void Width(unsigned short nWidth)
   {
      m_unWidth = nWidth;
   }

   inline BYTE Alignment()
   {
      return m_cAlignment;
   }

   inline void Alignment(BYTE nAlign)
   {
      m_cAlignment = nAlign;
   }

   inline unsigned short TextPosition()
   {
      return m_unTextPosition;
   }

   inline void TextPosition(unsigned short nPos)
   {
      m_unTextPosition = nPos;
   }

   inline BYTE StationId()
   {
      return m_cStationId;
   }

   inline void StationId(BYTE nId)
   {  
      m_cStationId = nId;
   }

   inline CString Data()
   {
      return m_csData;
   }

   inline void Data(const CString &csData)
   {
      m_csData = csData;
   }

   inline BYTE BarcodeType()
   {
      return m_cBarcodeType;
   }

   inline void BarcodeType(BYTE barcodeType)
   {
      m_cBarcodeType= barcodeType;
   }

   inline BYTE BarcodeCharacterPrint()
   {
      return m_cBarcodeCharacterPrint;
   }

   inline void BarcodeCharacterPrint(BYTE barcodeCharacterPrint)
   {
      m_cBarcodeCharacterPrint= barcodeCharacterPrint;
   }

   inline BYTE BitmapLogoNumber()
   {
      return m_cBitmapLogoNumber;
   }

   inline void BitmapLogoNumber(BYTE bitmapLogoNumber)
   {
      m_cBitmapLogoNumber= bitmapLogoNumber;
   }
protected:
   BYTE m_cFunction;
   CString m_csArguments;
   unsigned short m_unHeight;
   unsigned short m_unWidth;
   unsigned short m_unTextPosition;
   BYTE m_cAlignment;
   BYTE m_cStationId;
   BYTE m_cBarcodeType;
   BYTE m_cBarcodeCharacterPrint;
   BYTE m_cBitmapLogoNumber;
   CString m_csData;
protected:
   virtual int getLength(void);
public:
   virtual void Parse(BYTE *pBuf);
   virtual BYTE* Serialize(int &byteLength);
};

#endif // !defined(AFX_PRINTERMSG_H__42B90DD7_51ED_4FA0_9193_6EBF52E75215__INCLUDED_)
