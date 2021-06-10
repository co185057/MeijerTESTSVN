// Scale.h: interface for the CScale class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALE_H__3EFB4689_D549_41EC_A431_DE8BD4103E76__INCLUDED_)
#define AFX_SCALE_H__3EFB4689_D549_41EC_A431_DE8BD4103E76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"

class CScale : public CTBDevice  
{
public:
	CScale(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CScale();

	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );

// Operations
	long ReadWeight( long* pWeight, long Timeout );
	virtual CString GetDeviceModel( void ) const { return CString("Scale");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.2");} 
	void ProcessRawData( void* pData, DWORD dwByteLen );
	void CommunicationFailure( );

    virtual long DirectIO(long nCommand, long *plData, CString& sData);

};  //lint !e1712 supress "Default contructor is NEVER used!"

#endif // !defined(AFX_SCALE_H__3EFB4689_D549_41EC_A431_DE8BD4103E76__INCLUDED_)
