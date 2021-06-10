// LineDisplay.h: interface for the CLineDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEDISPLAY_H__B6C7C3C3_6D5C_48F3_ADD6_0050861A9243__INCLUDED_)
#define AFX_LINEDISPLAY_H__B6C7C3C3_6D5C_48F3_ADD6_0050861A9243__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"

#define MAX_LINE_DISPLAY		512


class CLineDisplay : public CTBDevice  
{
public:
	CLineDisplay(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CLineDisplay();
	virtual CString GetDeviceModel( void ) const { return CString("LineDisplay");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );

	// Operations
	long ClearDescriptors( void );
	long ClearText( void );
	long CreateWindow( long lViewportRow, long lViewportColumn, long lViewportHeight, 
		long lViewportWidth, long lWindowHeight, long lWindowWidth );
	long DestroyWindow( void );
	long DisplayText( LPCTSTR sData, long lAttribute );
	long DisplayTextAt(long lRow, long lColumn, LPCTSTR sData, long lAttribute );
	long SetDescriptor(long lDescriptor, long lAttribute );
	void ProcessRawData( void* pData, DWORD dwByteLen );

private:
	long NotifyTB( BOOL bDescriptor );
	long NotifyTBDirect( BOOL bDescriptor, long lRow, long lCol, CString csData );
	long PackLDData( BOOL bDescriptor, BYTE* pMsg );
	long PackWindow( long lwindow, BYTE* pMsg );
	int  m_HdrVersion;
	int  m_TBDirectData;
public:
	CString m_CreatorClass;
	void HandleClose();
};  //lint !e1712 supress "Default contructor is NEVER used!"

#endif // !defined(AFX_LINEDISPLAY_H__B6C7C3C3_6D5C_48F3_ADD6_0050861A9243__INCLUDED_)
