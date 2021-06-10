// TenderDetailsMsg.cpp: implementation of the CTenderDetailsMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TenderDetailsMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTenderDetailsMsg::CTenderDetailsMsg() :
   CTBMsgObj(TBDEV_TENDER_DETAILS),
	m_bTenderVoided(false),
	m_bTenderAccepted(false),
	m_tbTenderType(0),
	m_csBalance(_T("")),
	m_csChangeDue(_T("")),
	m_csErrorDescription(_T("")),
	m_csCardRemainingBalance(_T("")),
	m_csTenderAmount(_T(""))
{
}

CTenderDetailsMsg::~CTenderDetailsMsg()
{

}

int CTenderDetailsMsg::getLength(void)
{
   return 
         2 + m_csTenderDescription.GetLength() * sizeof(TCHAR)+
         sizeof(ULONG) +
         2 + m_csTenderAmount.GetLength() * sizeof(TCHAR)+
         2 + m_csBalance.GetLength() * sizeof(TCHAR)+
         2 + m_csChangeDue.GetLength() * sizeof(TCHAR)+
         sizeof(BYTE) +
         sizeof(BYTE) +
         2 + m_csErrorDescription.GetLength() * sizeof(TCHAR)+
         2 + m_csCardRemainingBalance.GetLength() * sizeof(TCHAR)+
			 GetLengthExtensionData() +
         sizeof(tRouteHeader);
}

void CTenderDetailsMsg::Reset()
{
	m_bTenderVoided=false;
	m_bTenderAccepted=false;
	m_tbTenderType=0;
	m_csBalance=_T("");
	m_csChangeDue=_T("");
	m_csErrorDescription=_T("");
	m_csCardRemainingBalance=_T("");
	m_csTenderAmount=_T("");
}

void CTenderDetailsMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 

   TenderDescription(ParseL2String());
   TenderType(ParseULong());
   TenderAmount(ParseL2String());
   Balance(ParseL2String());
   ChangeDue(ParseL2String());
   TenderAccepted(ParseByte()?true:false);
   TenderVoided(ParseByte()?true:false);
   ErrorDescription(ParseL2String());
   CardRemainingBalance(ParseL2String());
	ParseExtensionData();
}


BYTE* CTenderDetailsMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 

   SerializeL2String(m_csTenderDescription);
   SerializeULong(m_tbTenderType);
   SerializeL2String(m_csTenderAmount);
   SerializeL2String(m_csBalance);
   SerializeL2String(m_csChangeDue);
   SerializeByte(m_bTenderAccepted);
   SerializeByte(m_bTenderVoided);
   SerializeL2String(m_csErrorDescription);
   SerializeL2String(m_csCardRemainingBalance);
	SerializeExtensionData();
   return m_pBuf;
}

