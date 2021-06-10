// PinpadMsg.cpp: implementation of the CPinpadMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PinpadMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPinpadMsg::CPinpadMsg() :
   CTBMsgObj(TBDEV_PINPAD),
   m_TransactionType(TT_UNDEFINED),
   m_PinEncoding(PE_UNDEFINED),
   m_PinBlockFormat(PB_UNDEFINED)
{
}

CPinpadMsg::~CPinpadMsg()
{

}

int CPinpadMsg::getLength(void)
{
   return 
          2 + m_csPrimaryAccountNumber.GetLength() * sizeof(TCHAR) +
          sizeof(USHORT) +
          sizeof(BYTE) +
          2 + m_csWorkingKey.GetLength() * sizeof(TCHAR) +
          sizeof(BYTE) +
          3 + m_csTrack1Data.GetLength() * sizeof(TCHAR) +
              m_csTrack2Data.GetLength() * sizeof(TCHAR) +
              m_csTrack3Data.GetLength() * sizeof(TCHAR) +
			 GetLengthExtensionData()+
          sizeof(tRouteHeader);
}

void CPinpadMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   PrimaryAccountNumber(ParseL2String());
   TransactionType((CPinpadMsg::TransactionTypes) ParseUShort());
   PinEncoding((CPinpadMsg::PinEncodingList) ParseByte());
   WorkingKey(ParseL2String());
   PinBlockFormat((CPinpadMsg::PinBlockFormats)ParseByte());
   UINT nlen1=ParseByte();
   UINT nlen2=ParseByte();
   UINT nlen3=ParseByte();
   Track1Data(ParseString(nlen1));
   Track2Data(ParseString(nlen2));
   Track3Data(ParseString(nlen3));
	ParseExtensionData();
}

BYTE* CPinpadMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeL2String(PrimaryAccountNumber());
   SerializeUShort(TransactionType());
   SerializeByte((BYTE)PinEncoding());
   SerializeL2String(WorkingKey());
   SerializeByte((BYTE)PinBlockFormat());
   CString cs1= Track1Data();
   CString cs2= Track2Data();
   CString cs3= Track3Data();
   SerializeByte((BYTE)cs1.GetLength());
   SerializeByte((BYTE)cs2.GetLength());
   SerializeByte((BYTE)cs3.GetLength());
   SerializeString(cs1);
   SerializeString(cs2);
   SerializeString(cs3);
	SerializeExtensionData();
   return m_pBuf;
}

