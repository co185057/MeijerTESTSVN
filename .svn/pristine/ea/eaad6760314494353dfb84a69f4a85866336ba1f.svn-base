// MSRMsg.cpp: implementation of the CMSRMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSRMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMSRMsg::CMSRMsg() :
  CTBMsgObj(TB_DEV_MSR)
{
}

CMSRMsg::~CMSRMsg()
{

}

int CMSRMsg::getLength(void)
{
   return sizeof(BYTE) +
          sizeof(BYTE) +
          sizeof(BYTE) +
          m_csTracks.GetLength() * sizeof(TCHAR) +
			 GetLengthExtensionData()+
          sizeof(tRouteHeader);
}

void CMSRMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   m_nTrack1Len= ParseByte();
   m_nTrack2Len= ParseByte();
   m_nTrack3Len= ParseByte();

   SetTrack123(m_nTrack1Len, m_nTrack2Len, m_nTrack3Len, ParseString(m_nTrack1Len+ m_nTrack2Len+ m_nTrack3Len));
	ParseExtensionData();
}

BYTE* CMSRMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeByte((BYTE)m_nTrack1Len);
   SerializeByte((BYTE)m_nTrack2Len);
   SerializeByte((BYTE)m_nTrack3Len);
   SerializeString(m_csTracks);
	SerializeExtensionData();
   return m_pBuf;
}
