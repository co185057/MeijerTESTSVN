// DataMessage.h: interface for the CDataMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DataMessage_H__92BEE472_B76D_49EA_8B3E_476EF75579B1__INCLUDED_)
#define AFX_DataMessage_H__92BEE472_B76D_49EA_8B3E_476EF75579B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>

#include "ExpiringInt.h"

class CConnectBase;

class CDataMessage  
{
public:
	DWORD Size();
	CDataMessage();
	CDataMessage(CConnectBase *pConnSource, void* pData, DWORD dwByteLength );
	virtual ~CDataMessage();

	BOOL GetMessage( void* pBuffer, DWORD dwBufferLength, DWORD* pdwBytesReturned );
	BOOL SetMessage( CConnectBase *pConnSource, void* pData, DWORD dwByteLength );
//	void Attach( void* pData, DWORD dwByteLength );
	void* Detach( DWORD* pdwByteLength );

   CExpiringInt m_lifeTime;
   CConnectBase *m_pConnSource;

private:
	void* m_pData;
	DWORD m_dwByteLength;
};

#endif // !defined(AFX_DataMessage_H__92BEE472_B76D_49EA_8B3E_476EF75579B1__INCLUDED_)
