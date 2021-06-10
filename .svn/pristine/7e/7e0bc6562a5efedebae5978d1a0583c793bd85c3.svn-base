// ItemDetailsMsg.cpp: implementation of the CItemDetailsMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemDetailsMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemDetailsMsg::CItemDetailsMsg() :
   CTBMsgObj(TBDEV_ITEM_DETAILS),
	m_bSecurityTag(false),
	m_bPIMItem(false),
	m_unEntryID(0),
	m_unDepartment(0),
	m_bLinkedItem(false),
	m_bVisualVerify(false),
	m_bIsVoided(false),
	m_unTareCode(0),
	m_unRestrictedAge(0),
	m_bIsRestricted(false),
	m_bNotForSale(false),
	m_bUnknownItem(false),
	m_bNeedPrice(false),
	m_bNeedWeight(false),
	m_bNeedQuantity(false),
	m_bIsCoupon(false),
	m_csWeight(),
	m_csQuantity(),
	m_csErrorDescription(),
	m_csUnitPrice(),
	m_csExtendedPrice(),
	m_csItemCode(),
	m_csItemDescription(),
    m_csUnitQuantity(),
    m_csLinkedItemCode()
{
}

CItemDetailsMsg::~CItemDetailsMsg()
{

}

void CItemDetailsMsg::Reset()
{
	m_bSecurityTag=false;
	m_bPIMItem=false;
	m_unEntryID=0;
	m_unDepartment=0;
	m_bLinkedItem=false;
	m_bVisualVerify=false;
	m_bIsVoided=false;
	m_unTareCode=0;
	m_unRestrictedAge=0;
	m_bIsRestricted=false;
	m_bNotForSale=false;
	m_bUnknownItem=false;
	m_bNeedPrice=false;
	m_bNeedWeight=false;
	m_bNeedQuantity=false;
	m_bIsCoupon=false;
	m_csWeight=_T("");
	m_csQuantity=_T("");
	m_csErrorDescription=_T("");
	m_csUnitPrice=_T("");
	m_csExtendedPrice=_T("");
	m_csItemCode=_T("");
	m_csItemDescription=_T("");
    m_csUnitQuantity=_T("");
    m_csLinkedItemCode = _T("");
}


int CItemDetailsMsg::getLength(void)
{
   return 
         2 + m_csItemDescription.GetLength() * sizeof(TCHAR)+
         2 + m_csItemCode.GetLength() * sizeof(TCHAR)+
         2 + m_csExtendedPrice.GetLength() * sizeof(TCHAR)+
         2 + m_csUnitPrice.GetLength() * sizeof(TCHAR)+
         2 + m_csQuantity.GetLength() * sizeof(TCHAR)+
         2 + m_csWeight.GetLength() * sizeof(TCHAR)+
	      sizeof(BYTE) +
	      sizeof(BYTE) +
	      sizeof(BYTE) +
	      sizeof(BYTE) +
	      sizeof(BYTE) +
	      sizeof(BYTE) +
	      sizeof(BYTE) +
	      sizeof(ULONG) +
	      sizeof(ULONG) +
	      sizeof(BYTE) +
	      sizeof(BYTE) +
	      sizeof(BYTE) +
         2 + m_csErrorDescription.GetLength() * sizeof(TCHAR)+
	      sizeof(ULONG) +
	      sizeof(ULONG) +
	      sizeof(BYTE) +
	      sizeof(BYTE) +
          2 + m_csUnitQuantity.GetLength() * sizeof(TCHAR)+
          2 + m_csLinkedItemCode.GetLength() * sizeof(TCHAR)+
			GetLengthExtensionData() +
         sizeof(tRouteHeader); 
}


void CItemDetailsMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first 
   ItemDescription(ParseL2String());
   ItemCode       (ParseL2String());
   ExtendedPrice  (ParseL2String());
   UnitPrice      (ParseL2String());
   Quantity       (ParseL2String());
   Weight         (ParseL2String());
   IsCoupon       (ParseByte()?true:false);
   NeedQuantity   (ParseByte()?true:false);
   NeedWeight     (ParseByte()?true:false);
   NeedPrice      (ParseByte()?true:false);
   UnknownItem    (ParseByte()?true:false);
   NotForSale     (ParseByte()?true:false);
   IsRestricted   (ParseByte()?true:false);
   RestrictedAge  (ParseULong());
   TareCode       (ParseULong());
   IsVoided       (ParseByte()?true:false);
   VisualVerify   (ParseByte()?true:false);
   LinkedItem     (ParseByte()?true:false);
   ErrorDescription(ParseL2String());
   Department     (ParseULong());
   EntryID        (ParseULong());
   PIMItem        (ParseByte()?true:false);
   SecurityTag    (ParseByte()?true:false);
   UnitQuantity   (ParseL2String());
   LinkedItemCode (ParseL2String());
	ParseExtensionData();
}


BYTE* CItemDetailsMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength); // must be called first 
   SerializeL2String(m_csItemDescription);
   SerializeL2String(m_csItemCode);
   SerializeL2String(m_csExtendedPrice);
   SerializeL2String(m_csUnitPrice);
   SerializeL2String(m_csQuantity);
   SerializeL2String(m_csWeight);
   SerializeByte(m_bIsCoupon);
   SerializeByte(m_bNeedQuantity);
   SerializeByte(m_bNeedWeight);
   SerializeByte(m_bNeedPrice);
   SerializeByte(m_bUnknownItem);
   SerializeByte(m_bNotForSale);
   SerializeByte(m_bIsRestricted);
   SerializeULong(m_unRestrictedAge);
   SerializeULong(m_unTareCode);
   SerializeByte(m_bIsVoided);
   SerializeByte(m_bVisualVerify);
   SerializeByte(m_bLinkedItem);
   SerializeL2String(m_csErrorDescription);
   SerializeULong(m_unDepartment);
   SerializeULong(m_unEntryID);
   SerializeByte(m_bPIMItem);
   SerializeByte(m_bSecurityTag);
   SerializeL2String(m_csUnitQuantity);
   SerializeL2String(m_csLinkedItemCode);

	SerializeExtensionData();
   return m_pBuf;
}
