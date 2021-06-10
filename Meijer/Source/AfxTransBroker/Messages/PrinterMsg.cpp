// PrinterMsg.cpp: implementation of the CPrinterMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PrinterMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrinterMsg::CPrinterMsg(TBDEVICEID devId /*=TB_DEV_PRINTERRECEIPT*/) :
   CTBMsgObj(devId),
   m_cFunction(0),
   m_unHeight(0),
   m_unWidth(0),
   m_cAlignment(0),
   m_unTextPosition(0),
   m_cStationId(0),
   m_cBarcodeType(0),
   m_cBarcodeCharacterPrint(0),
   m_cBitmapLogoNumber(0)
{
}

CPrinterMsg::~CPrinterMsg()
{

}

int CPrinterMsg::getLength(void)
{
   return sizeof(BYTE) +
          sizeof(USHORT) +
          sizeof(USHORT) +
          sizeof(BYTE) +
          sizeof(BYTE) +
          sizeof(BYTE) +
          sizeof(BYTE) +
          sizeof(BYTE) +
          sizeof(ULONG) + 
		  (m_csData.GetLength() * sizeof(TCHAR)) +
          //sizeof(m_unTextPosition) +
			 GetLengthExtensionData()+
          sizeof(tRouteHeader);
}

void CPrinterMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   Function(ParseByte());
   Height(ParseUShort());
   Width(ParseUShort());
   Alignment(ParseByte());
   StationId(ParseByte());
   BarcodeType(ParseByte());
   BarcodeCharacterPrint(ParseByte());
   BitmapLogoNumber(ParseByte());
   ULONG ulDataLen = ParseULong();
   BYTE* pTemp = new BYTE[ulDataLen + sizeof(TCHAR)];
   memset( pTemp, 0, ulDataLen + sizeof(TCHAR));
   memcpy( pTemp, ParseBinaryBuffer( ulDataLen ), ulDataLen );
   Data((TCHAR*)pTemp);
   delete[] pTemp;
	ParseExtensionData();
}

BYTE* CPrinterMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeByte(Function());
   SerializeUShort(Height());
   SerializeUShort(Width());
   SerializeByte(Alignment());
   SerializeByte(StationId());
   SerializeByte(BarcodeType());
   SerializeByte(BarcodeCharacterPrint());
   SerializeByte(BitmapLogoNumber());
   ULONG ulDataLen = Data().GetLength() * sizeof(TCHAR);
   SerializeULong(ulDataLen);
   LPCTSTR sData = (LPCTSTR)Data();
   SerializeBinaryBuffer((BYTE*)sData, ulDataLen );
	SerializeExtensionData();
   return m_pBuf;
}

