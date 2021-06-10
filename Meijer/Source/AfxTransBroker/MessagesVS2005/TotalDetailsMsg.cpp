// TotalDetailsMsg.cpp: implementation of the CTotalDetailsMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TotalDetailsMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTotalDetailsMsg::CTotalDetailsMsg() :
   CTBMsgObj(TBDEV_TOTAL_DETAILS),
	m_unRewardPoints(0),
	m_csFoodStampAmount(_T("")),
	m_csErrorDescription(_T("")),
	m_csTotalDiscount(_T("")),
	m_csTotalDue(_T("")),
	m_csTaxDue(_T(""))
{

}

CTotalDetailsMsg::~CTotalDetailsMsg()
{

}

void CTotalDetailsMsg::Reset()
{
	m_unRewardPoints=0;
	m_csFoodStampAmount=_T("");
	m_csErrorDescription=_T("");
	m_csTotalDiscount=_T("");
	m_csTotalDue=_T("");
	m_csTaxDue=_T("");
}

int CTotalDetailsMsg::getLength(void)
{
   return 
          2 + m_csTaxDue.GetLength() * sizeof(TCHAR)+
          2 + m_csTotalDue.GetLength() * sizeof(TCHAR)+
          2 + m_csTotalDiscount.GetLength() * sizeof(TCHAR)+
          2 + m_csErrorDescription.GetLength() * sizeof(TCHAR)+
          2 + m_csFoodStampAmount.GetLength() * sizeof(TCHAR) +
          sizeof(ULONG) +
			 GetLengthExtensionData() +
          sizeof(tRouteHeader);
}


void CTotalDetailsMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   TaxDue(ParseL2String());
   TotalDue(ParseL2String());
   TotalDiscount(ParseL2String());
   ErrorDescription(ParseL2String());
   FoodStampAmount(ParseL2String());
   RewardPoints(ParseULong());
	ParseExtensionData();
}


BYTE* CTotalDetailsMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeL2String(TaxDue());
   SerializeL2String(TotalDue());
   SerializeL2String(TotalDiscount());
   SerializeL2String(ErrorDescription());
   SerializeL2String(FoodStampAmount());
   SerializeULong(RewardPoints());
	SerializeExtensionData();
   return m_pBuf;
}


