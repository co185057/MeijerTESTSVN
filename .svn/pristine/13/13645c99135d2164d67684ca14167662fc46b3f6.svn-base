// KeylockMsg.cpp: implementation of the CKeylockMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeylockMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeylockMsg::CKeylockMsg() :
   CTBMsgObj(TB_DEV_KEYLOCK),
   m_cPosition(0)
{
}

CKeylockMsg::CKeylockMsg(BYTE cPos) :
   CTBMsgObj(TB_DEV_KEYLOCK),
	m_cPosition(cPos)
{
}

CKeylockMsg::~CKeylockMsg()
{

}

int CKeylockMsg::getLength(void)
{
   return sizeof(BYTE) +
			 GetLengthExtensionData() +
          sizeof(tRouteHeader);
}


void CKeylockMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 
   Position(ParseByte());
	ParseExtensionData();
}

BYTE* CKeylockMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 
   SerializeByte(Position());
	SerializeExtensionData();
   return m_pBuf;
}


