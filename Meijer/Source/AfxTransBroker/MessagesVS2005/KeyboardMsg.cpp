// KeyboardMsg.cpp: implementation of the CKeyboardMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeyboardMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyboardMsg::CKeyboardMsg(USHORT usKeyCode, KeyEventType dwEvent, CombinationKeyFlags dwFlags) :
   CTBMsgObj(TB_DEV_KEYBOARD),
	m_usKeyCode(usKeyCode),
	m_KeyEvent(dwEvent),
	m_ComboFlags(dwFlags)
{
}

CKeyboardMsg::CKeyboardMsg() :
   CTBMsgObj(TB_DEV_KEYBOARD),
	m_usKeyCode(0),
	m_KeyEvent((KeyEventType)0),
	m_ComboFlags((CombinationKeyFlags)0)
{
}

CKeyboardMsg::~CKeyboardMsg()
{

}

int CKeyboardMsg::getLength(void)
{
   return sizeof(USHORT) +
          sizeof(BYTE) +
          sizeof(BYTE) +
			 GetLengthExtensionData() +
          sizeof(tRouteHeader);
}


void CKeyboardMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   USHORT usKeyCode= ParseUShort();
   CKeyboardMsg::KeyEventType dwEvent= (CKeyboardMsg::KeyEventType) ParseByte();
   CKeyboardMsg::CombinationKeyFlags dwFlags= (CKeyboardMsg::CombinationKeyFlags) ParseByte();
   
   KeyCode(usKeyCode);
   KeyEvent(dwEvent);
   CombinationKeys(dwFlags);
	ParseExtensionData();
}

BYTE* CKeyboardMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeUShort(KeyCode());
   SerializeByte((BYTE)KeyEvent());
   SerializeByte((BYTE)CombinationKeys());
	SerializeExtensionData();
   return m_pBuf;
}


