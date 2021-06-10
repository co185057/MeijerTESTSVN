#if !defined(AFX_SCANNER_H__32D2CF86_4547_11D5_9C83_0090276FD28D__INCLUDED_)
#define AFX_SCANNER_H__32D2CF86_4547_11D5_9C83_0090276FD28D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Scanner.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScanner window

#include "TBDevice.h"

class CScanner : public CTBDevice
{
// Construction
public:
	CScanner(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CScanner();

// Operations
	virtual CString GetDeviceModel( void ) const { return CString("Scanner");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	void ProcessRawData( void* pData, DWORD dwByteLen );
	void CommunicationFailure( );

};  //lint !e1712 supress "Default contructor is NEVER used!"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANNER_H__32D2CF86_4547_11D5_9C83_0090276FD28D__INCLUDED_)
