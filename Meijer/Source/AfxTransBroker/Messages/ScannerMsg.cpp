// ScannerMsg.cpp : Implementation of CScannerMsg
#include "stdafx.h"
#include "ScannerMsg.h"


// CScannerMsg
CScannerMsg::CScannerMsg() :
   CTBMsgObj(TB_DEV_SCANNER)
{
}

CScannerMsg::~CScannerMsg()
{
}

int CScannerMsg::getLength(void)
{
   return sizeof(ULONG) +
          2 +
          m_csData.GetLength() * sizeof(TCHAR) +
          sizeof(tRouteHeader) +
			 GetLengthExtensionData();
}

void CScannerMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   BarcodeType((CScannerMsg::BarcodeTypes)ParseULong());
   Data(ParseL2String());
	ParseExtensionData();
}

BYTE* CScannerMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeULong((ULONG)BarcodeType());
   SerializeL2String(Data());
	SerializeExtensionData();
   return m_pBuf;
}
