// CashDrawer.h: interface for the CCashDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASHDRAWER_H__70686526_F804_41EC_B697_A09A976A1654__INCLUDED_)
#define AFX_CASHDRAWER_H__70686526_F804_41EC_B697_A09A976A1654__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"

class CSocketForTBDevice;

class CCashDrawer : public CTBDevice  
{
public:
	CCashDrawer(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CCashDrawer();
	virtual CString GetDeviceModel( void ) const { return CString("CashDrawer");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );

	// TB Input
	void ProcessRawData( void* pData, DWORD dwByteLen );

	// Operations
	long OpenDrawer( void );
	long WaitForDrawerClose( long BeepTimeout, long BeepFrequency,
							long BeepDuration, long BeepDelay );
private:
	long NotifyTB( BYTE DrawerState );
	static UINT DrawerCloseProc( void* lpParam );
	long m_lMaxCloseDelay;
	BYTE m_DrawerNumber;
	long m_lAutoCloseTime;

public:
	CString m_CreatorClass;
};  //lint !e1712 supress "Default contructor is NEVER used!"

#endif // !defined(AFX_CASHDRAWER_H__70686526_F804_41EC_B697_A09A976A1654__INCLUDED_)
