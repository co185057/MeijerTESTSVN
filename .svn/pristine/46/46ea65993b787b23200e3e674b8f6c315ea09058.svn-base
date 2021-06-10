// MICR.h: interface for the CMICR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICR_H__6C23E762_E548_4E40_9B31_F06F6ED237DF__INCLUDED_)
#define AFX_MICR_H__6C23E762_E548_4E40_9B31_F06F6ED237DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"

class CMICR : public CTBDevice  
{
public:
	CMICR(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CMICR();
	virtual CString GetDeviceModel( void ) const { return CString("MICR");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );
	virtual void ProcessRawData( void* pData, DWORD dwByteLen );
	virtual void CommunicationFailure();

	// Operations
    long BeginInsertion(long Timeout);
    long BeginRemoval(long Timeout);
    long EndInsertion();
    long EndRemoval();

private:
	long NotifyTB( BYTE* pBuffer, long lBufferLen );
private:
public:
	CString m_CreatorClass;
	void HandleClose();
};  //lint !e1712 supress "Default contructor is NEVER used!"

#endif // !defined(AFX_MICR_H__6C23E762_E548_4E40_9B31_F06F6ED237DF__INCLUDED_)
