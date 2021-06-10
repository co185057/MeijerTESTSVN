// RewardLineDetailsMsg.cpp: implementation of the CRewardLineDetailsMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RewardLineDetailsMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRewardLineDetailsMsg::CRewardLineDetailsMsg() :
   CTBMsgObj(TBDEV_REWARD_DETAILS),
	m_cRewardLocation(POS_NONE),
	m_bIsVoided(false),
	m_unRewardPoints(0),
	m_bUpdateRewardTotal(false),
	m_unAssociatedEntryID(0),
	m_unEntryID(0),
	m_bShowRewardPoints(true),
	m_unRewardValue(0)
{
}

CRewardLineDetailsMsg::~CRewardLineDetailsMsg()
{

}

int CRewardLineDetailsMsg::getLength(void)
{
   return 
          2 + m_csRewardDescription.GetLength() * sizeof(TCHAR) +
          sizeof(ULONG) +
          sizeof(BYTE) +
          sizeof(ULONG) +
          sizeof(ULONG) +
          sizeof(BYTE) +
          sizeof(ULONG) +
          sizeof(BYTE) +
          sizeof(BYTE) +
			 GetLengthExtensionData()+
          sizeof(tRouteHeader);
}


void CRewardLineDetailsMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   RewardDescription(ParseL2String());
   RewardValue(ParseULong());
   ShowRewardPoints(ParseByte()?true:false);
   EntryID(ParseULong());
   AssociatedEntryID(ParseULong());
   UpdateRewardTotal(ParseByte()?true:false);
   RewardPoints(ParseULong());
   IsVoided(ParseByte()?true:false);
   RewardLocation((tRewardLinePosType)ParseByte());
	ParseExtensionData();
}

BYTE* CRewardLineDetailsMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeL2String(RewardDescription());
   SerializeULong(RewardValue());
   SerializeByte(ShowRewardPoints());
   SerializeULong(EntryID());
   SerializeULong(AssociatedEntryID());
   SerializeByte(UpdateRewardTotal());
   SerializeULong(RewardPoints());
   SerializeByte(IsVoided());
   SerializeByte((BYTE)RewardLocation());
	SerializeExtensionData();
   return m_pBuf;
}




