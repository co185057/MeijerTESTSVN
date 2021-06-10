// CustomerDefinedMsg.cpp: implementation of the CCustomerDefinedMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomerDefinedMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerDefinedMsg::CCustomerDefinedMsg() :
   CTBMsgObj(TBDEV_CUSTOM),
	m_pData(NULL),
	m_nDataSize(0),
	m_byCustomMessageType(0)
{
}

CCustomerDefinedMsg::~CCustomerDefinedMsg()
{
	delete [] m_pData;
}

int CCustomerDefinedMsg::DataLength()
{
	return m_nDataSize;
}

void CCustomerDefinedMsg::DataLength(int *o_pnDataSize)
{
	*o_pnDataSize= m_nDataSize;
}

void* CCustomerDefinedMsg::CustomerData()
{
	return m_pData;
}

void CCustomerDefinedMsg::CustomerData(void *pData, int nSize)
{
	if(m_pData)
		delete [] m_pData;
	m_pData = new BYTE[nSize];
	memcpy(m_pData, pData, nSize);
	m_nDataSize= nSize;
}

int CCustomerDefinedMsg::getLength(void)
{
   return sizeof(tRouteHeader)+
          sizeof(BYTE) +
			 sizeof(USHORT) +
          m_nDataSize;
}

void CCustomerDefinedMsg::Parse(BYTE *pBuf)
{
   ParseHeader(pBuf);         // must be called first
   m_byCustomMessageType= ParseByte();
	m_nDataSize= ParseUShort();
   CustomerData(ParseBinaryBuffer(m_nDataSize), m_nDataSize);
}

BYTE* CCustomerDefinedMsg::Serialize(int &byteLength)
{
   SerializeHeader(byteLength);
   SerializeByte(m_byCustomMessageType);
	SerializeUShort(m_nDataSize);
   SerializeBinaryBuffer((BYTE*) m_pData, m_nDataSize);
   return m_pBuf;
	
}
