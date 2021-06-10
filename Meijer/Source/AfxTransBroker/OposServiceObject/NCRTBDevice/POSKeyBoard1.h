// POSKeyBoard1.h: interface for the CPOSKeyBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSKEYBOARD1_H__534228CA_E6C1_4ED5_8118_ADA95AEF26C6__INCLUDED_)
#define AFX_POSKEYBOARD1_H__534228CA_E6C1_4ED5_8118_ADA95AEF26C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"

class CPOSKeyBoard : public CTBDevice  
{
public:
	CPOSKeyBoard(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CPOSKeyBoard();
	virtual CString GetDeviceModel( void ) const { return CString("POSKeyBoard");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );
	void ProcessRawData( void* pData, DWORD dwByteLen );
};  //lint !e1712 supress "Default contructor is NEVER used!"

#endif // !defined(AFX_POSKEYBOARD1_H__534228CA_E6C1_4ED5_8118_ADA95AEF26C6__INCLUDED_)
