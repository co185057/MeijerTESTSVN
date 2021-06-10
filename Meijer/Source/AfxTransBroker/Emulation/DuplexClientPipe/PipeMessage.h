// PipeMessage.h: interface for the CPipeMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIPEMESSAGE_H__92BEE472_B76D_49EA_8B3E_476EF75579B1__INCLUDED_)
#define AFX_PIPEMESSAGE_H__92BEE472_B76D_49EA_8B3E_476EF75579B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>

class CPipeMessage  
{
public:
	DWORD Size();
	CPipeMessage();
	CPipeMessage( void* pData, DWORD dwByteLength );

	virtual ~CPipeMessage();

	BOOL GetMessage( void* pBuffer, DWORD dwBufferLength, DWORD* pdwBytesReturned );
	BOOL SetMessage( void* pData, DWORD dwByteLength );
	void Attach( void* pData, DWORD dwByteLength );
	void* Detach( DWORD* pdwByteLength );

private:
	void* m_pData;
	DWORD m_dwByteLength;
};

#endif // !defined(AFX_PIPEMESSAGE_H__92BEE472_B76D_49EA_8B3E_476EF75579B1__INCLUDED_)
