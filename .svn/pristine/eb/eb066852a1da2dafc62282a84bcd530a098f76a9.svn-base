// CustomerDefinedMsg.h: interface for the CCustomerDefinedMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMERDEFINEDMSG_H__E3ECD57A_DDCC_4253_9087_57A5AE35EC94__INCLUDED_)
#define AFX_CUSTOMERDEFINEDMSG_H__E3ECD57A_DDCC_4253_9087_57A5AE35EC94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

#define CUSTOMMSG_TYPE1		1
#define CUSTOMMSG_TYPE2		2
#define CUSTOMMSG_TYPE3		3
#define CUSTOMMSG_TYPE4		4
#define CUSTOMMSG_TYPE5		5
#define CUSTOMMSG_TYPE6		6
#define CUSTOMMSG_TYPE7		7
#define CUSTOMMSG_TYPE8		8
#define CUSTOMMSG_TYPE9		9
#define CUSTOMMSG_TYPE10	10

class CCustomerDefinedMsg : public CTBMsgObj
{
public:
	CCustomerDefinedMsg();
	virtual ~CCustomerDefinedMsg();

	virtual void CustomerData(void* pData, int nSize);
	virtual void* CustomerData();
	int DataLength();
	void DataLength(int *o_pnDataSize);

	int getLength(void);
	void Parse(BYTE *pBuf);
	BYTE* Serialize(int &byteLength);
public:
	BYTE m_byCustomMessageType;
protected:
	void* m_pData;
	int m_nDataSize;
};

#endif // !defined(AFX_CUSTOMERDEFINEDMSG_H__E3ECD57A_DDCC_4253_9087_57A5AE35EC94__INCLUDED_)
