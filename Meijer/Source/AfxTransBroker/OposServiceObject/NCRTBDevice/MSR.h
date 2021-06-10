// MSR.h: interface for the CMSR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSR_H__67BFCF90_73DF_4736_82F2_F8ED081446D9__INCLUDED_)
#define AFX_MSR_H__67BFCF90_73DF_4736_82F2_F8ED081446D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"

class CMSR : public CTBDevice  
{
public:
	CMSR(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CMSR();

   long Open(CTBService *pService, COposRegistry &oRegistry);
// Operations
	virtual CString GetDeviceModel( void ) const { return CString("MSR");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	void ProcessRawData( void* pData, DWORD dwByteLen );
	void CommunicationFailure( );
   void HandleClose();

public:
};  //lint !e1712 supress "Default contructor is NEVER used!"

#endif // !defined(AFX_MSR_H__67BFCF90_73DF_4736_82F2_F8ED081446D9__INCLUDED_)
