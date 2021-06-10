// MicrMsg.cpp: implementation of the CMicrMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MicrMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMicrMsg::CMicrMsg() :
   CTBMsgObj(TBDEV_MICR),
   m_CheckType(CHECK_UNKNOWN),
   m_CountryCode(CC_UNK)
{
}

CMicrMsg::~CMicrMsg()
{

}

int CMicrMsg::getLength(void)
{
   return 2 + m_csTransitNumber.GetLength() * sizeof(TCHAR) +
          2 + m_csBankNumber.GetLength() * sizeof(TCHAR) +
          2 + m_csAccountNumber.GetLength() * sizeof(TCHAR) +
          2 + m_csSerialNumber.GetLength() * sizeof(TCHAR) +
          sizeof(BYTE) +
          sizeof(BYTE) +
          2 + m_csData.GetLength() * sizeof(TCHAR) +
			 GetLengthExtensionData()+
          sizeof(tRouteHeader);
}

void CMicrMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   TransitNumber(ParseL2String());
   BankNumber(ParseL2String());
   AccountNumber(ParseL2String());
   SerialNumber(ParseL2String());
   CheckType((CMicrMsg::CheckTypes) ParseByte());
   CountryCode((CMicrMsg::CountryCodes) ParseByte());
   Data(ParseL2String());
	ParseExtensionData();
}

BYTE* CMicrMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   // Need L2String for Transit and Bank Number for proper processing via
   // TBControls in relation to XML GenMsg.

   SerializeL2String(TransitNumber());
   SerializeL2String(BankNumber());
   SerializeL2String(AccountNumber());
   SerializeL2String(SerialNumber());
   SerializeByte((BYTE)CheckType());
   SerializeByte((BYTE)CountryCode());
   SerializeL2String(Data());
	SerializeExtensionData();
   return m_pBuf;
}
