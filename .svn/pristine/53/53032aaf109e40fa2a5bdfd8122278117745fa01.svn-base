// KeyLock.h: interface for the CKeyLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYLOCK_H__18C74CFE_BB7E_4421_9519_DD7FD667F7CE__INCLUDED_)
#define AFX_KEYLOCK_H__18C74CFE_BB7E_4421_9519_DD7FD667F7CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"

class CKeyLock : public CTBDevice  
{
public:
	CKeyLock(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CKeyLock();

	virtual CString GetDeviceModel( void ) const { return CString(OPOS_CLASSKEY_LOCK);}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );
	virtual void ProcessRawData( void* pData, DWORD dwByteLen );

    virtual long WaitForKeylockChange(long KeyPosition, long Timeout);

private:
    HANDLE m_hKeyWait;
    long   m_lWaitForPosition;
    long   m_lMaxPositions;
    CCriticalSection m_csBlock;
};

#endif // !defined(AFX_KEYLOCK_H__18C74CFE_BB7E_4421_9519_DD7FD667F7CE__INCLUDED_)
