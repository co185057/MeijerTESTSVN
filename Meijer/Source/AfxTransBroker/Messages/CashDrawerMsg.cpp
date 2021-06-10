// ashDrawerMsg.cpp: implementation of the CashDrawerMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CashDrawerMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCashDrawerMsg::CCashDrawerMsg(TBDEVICEID devId /*=TB_DEV_CASHDRAWER1*/) :
   CTBMsgObj(devId),
	m_Status(CD_UNKNOWN)
{
      switch(devId) {
      case TB_DEV_DRAWER1:
         m_Station= DRAWER_A;
         break;
      case TB_DEV_DRAWER2:
         m_Station= DRAWER_B;
         break;
      default:
   	   m_Station= DRAWER_UNDEFINED;
         break;
      }
}

CCashDrawerMsg::~CCashDrawerMsg()
{
}

int CCashDrawerMsg::getLength(void)
{
   return sizeof(m_Station) +
          sizeof(m_Status)+
          sizeof(tRouteHeader) +
			 GetLengthExtensionData();

}

void CCashDrawerMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   Station((CCashDrawerMsg::CashDrawerStations) ParseByte());
   Status((CCashDrawerMsg::CashDrawerStates) ParseByte());
	ParseExtensionData();
}

BYTE* CCashDrawerMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeByte((BYTE)Station());
   SerializeByte((BYTE)Status());
	SerializeExtensionData();
   return m_pBuf;
}

