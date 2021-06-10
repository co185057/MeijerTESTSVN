#if !defined(AFX_SOCKETFORTBDEVICE_H__89F55C5C_BDE2_46CB_B2A0_9B4BFE8228FF__INCLUDED_)
#define AFX_SOCKETFORTBDEVICE_H__89F55C5C_BDE2_46CB_B2A0_9B4BFE8228FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketForTBDevice.h : header file
//
#ifdef __SOCKETFORTB
#include "ncrtbdevice.h"
#include "tbdevice.h"
#endif

#define TM_MODULE TM_MODULE_4
#include "TraceSystem.h"

#include "posprinter.h"
#include "micr.h"
#include "linedisplay.h"
#include "cashdrawer.h"
#include "scale.h"
#include "scanner.h"
#include "poskeyboard1.h"
#include "msr.h"

class CNCRTBDeviceApp;
class CPOSPrinter;
class CLineDisplay;
class CCashDrawer;
class CTBDevice;
class CMICR;
class Scale;
class Scanner;
class CPOSKeyBoard;

/////////////////////////////////////////////////////////////////////////////
// CSocketForTBDevice command target

class CSocketForTBDevice 
{
// Attributes
public:

// Operations
public:
	CSocketForTBDevice();
	virtual ~CSocketForTBDevice();

// Overrides
public:
	int m_TBCountForKeyboardDataLeft;
	int m_iTimeForPOStoProcessKey;
	bool m_bAck;
	CString m_csDeviceIDs;
	CString m_csPortNumberWrite;
	CString m_csPortNumber;
	void ProcessRawData( void* pData, DWORD dwByteLen );
	void SetCreatorClass(CString csCreatorClass);
	long m_lPortNumber;
	CString m_csServerName;
    static int m_nHeaderSpecVersion;
    static bool m_bKeepSocketsOpen;
   
    int TBSend(const void *pBuf, int iLength);
    void HandleAck(BYTE *AckToSend, CTraceSystem &ts);
    void StartReadThread();

    static int  GetSpecVersion();
    static bool GetKeepSocketsOpen();
   
    int Receive(BYTE *buf, int len, int flags );

   // Methods taken from MFC CAsyncSocket
	virtual void Close();
    int GetLastError();
	BOOL Create(UINT nSocketPort = 0, int nSocketType=SOCK_STREAM,
		LPCTSTR lpszSocketAddress = NULL);
    BOOL Socket(int nSocketType, int nProtocolType = 0, int nAddressFormat = PF_INET);
	BOOL Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress = NULL);
	BOOL Bind (const SOCKADDR* lpSockAddr, int nSockAddrLen);
	BOOL IOCtl(long lCommand, DWORD* lpArgument);
	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	BOOL Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);

private:
    SOCKET  m_hSocket;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETFORTBDEVICE_H__89F55C5C_BDE2_46CB_B2A0_9B4BFE8228FF__INCLUDED_)
