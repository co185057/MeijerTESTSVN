// ScaleMsg.cpp: implementation of the CScaleMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScaleMsg::CScaleMsg() :
   CTBMsgObj(TB_DEV_SCALE),
	m_unWeight(0)
{
}

CScaleMsg::CScaleMsg(unsigned int unWeight) :
   CTBMsgObj(TB_DEV_SCALE),
	m_unWeight(unWeight)
{
}

CScaleMsg::~CScaleMsg()
{

}

int CScaleMsg::getLength(void)
{
   return sizeof(ULONG) +
			 GetLengthExtensionData()+
          sizeof(tRouteHeader);
}

void CScaleMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   Weight(ParseULong());
	ParseExtensionData();
}

BYTE* CScaleMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeULong(Weight());
	SerializeExtensionData();
   return m_pBuf;
}
