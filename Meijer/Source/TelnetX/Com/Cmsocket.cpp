
//
// Copyright (c) 1997 Mabry Software, Inc.
//
#include "stdafx.h"
#include "cstr.h"
//#include <windows.h>                     
//#include <crtdbg.h>

#define ASSERT(e, m) _ASSERT(e)

#include "cstr.h"
#define _DEFINE_STATICS
#include "cmsocket.h"
#undef _DEFINE_STATICS

static BOOL fWndClassRegistered = FALSE;
static LPTSTR LPSTRClassName = _T("CMSocketWindow");

#define IfBlockingReturn(ret) \
	if(m_fBlocking && m_fIsBlocking) \
	{								 \
		WSASetLastError(WSAEINPROGRESS); \
		return  ret; \
	}

#define BeginBlocking() \
	m_fIsBlocking = TRUE; \
	m_fCancelBlockingCall = FALSE;

#define EndBlocking() \
	m_fIsBlocking = FALSE;

#if 0
#define StartTimer(which) \
	if( (((which##Counter = which) != 0) || (m_fBlocking != FALSE) ) && \
		(m_TimerID == 0)) \
		m_TimerID = SetTimer(m_hwndSocket, 1, 1000, NULL);

#define StopTimer(which) \
	which##Counter = 0; \
	if( m_fBlocking ) \
	{ \
		KillTimer(m_hwndSocket, m_TimerID); \
		m_TimerID = 0; \
	} 
#else

void CMSocket::_StartTimer(int tval, int& counter)
{
	counter = tval;

	if((tval != 0 || m_fBlocking != FALSE) &&	(m_TimerID == 0)) 
		m_TimerID = SetTimer(m_hwndSocket, 1, 1000, NULL);
}

#define StartTimer(which) _StartTimer(which, which##Counter)

#define StartTimerCB(psock, which) psock->which##Counter = psock->which;

void CMSocket::_StopTimer(int& counter)
{
	counter = 0;
	if( m_fBlocking )
	{
		KillTimer(m_hwndSocket, 1);
		m_TimerID = 0;
	} 
}

#define StopTimer(which) _StopTimer(which##Counter)

#endif

#define StopTimerCB(psock, which) \
	psock->which##Counter = 0;
    
    
void CMSocket::CreateSocketWindow()
{
    WNDCLASS wndclass;

    if(!fWndClassRegistered) 
	{
        memset(&wndclass, 0, sizeof(wndclass));
        wndclass.lpfnWndProc = SocketWindowProc;
        wndclass.hInstance   = _Module.GetModuleInstance();
        wndclass.lpszClassName = LPSTRClassName;
		wndclass.cbWndExtra  = 4;
        RegisterClass(&wndclass);
        fWndClassRegistered = TRUE;
    }
	m_hwndSocket = CreateWindow(LPSTRClassName,
      _T("Mabry Socket Window"),
      WS_POPUP,
      0, 0, 0, 0,
      NULL,
      NULL,
      _Module.GetModuleInstance(),
      NULL);
	if( m_hwndSocket != NULL )
		SetWindowLong(m_hwndSocket, GWL_USERDATA, (LONG)this);
}


LRESULT CALLBACK CMSocket::SocketWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CMSocket * pSocket = (CMSocket *)GetWindowLong(hwnd, GWL_USERDATA);

	ATLTRACE(_T("CMSocket::SocketWindowProc %x\n"), msg);
	if( msg == WM_SOCKET_NOTIFY )
		ATLTRACE(_T("WM_SOCKET_NOTIFY: %d\n"), WSAGETSELECTEVENT(lParam));
	
	//
	// Any messages for blocking sockets that get past the Block function
	// are simply thrown away here.  This could happen, for instance, in
	// the case where a timer message arrives between the time a socket
	// event occurs and is recognized and the timer is disabled.
	//

	if(msg==WM_SOCKET_SSL_NOTIFY)
	{
		StopTimerCB(pSocket,m_ReceiveTimeout);
		if (!IsBadCodePtr((FARPROC)pSocket))
			pSocket->OnReceive(0);
		return 0;
	}

	if(msg == WM_SOCKET_NOTIFY && pSocket->m_fBlocking == FALSE)
	{
		int nErrorCode = WSAGETSELECTERROR(lParam);

		ATLTRACE(_T("WM_SOCKET_NOTIFY: %d\n"), WSAGETSELECTEVENT(lParam));
		switch (WSAGETSELECTEVENT(lParam))
		{
			case FD_READ:
			{
				DWORD nBytes;

				ATLTRACE(_T("FD_READ"));
				StopTimerCB(pSocket, m_ReceiveTimeout);
				if(!pSocket->IOCtl(FIONREAD, &nBytes))
					nErrorCode = pSocket->WSAGetLastError();
				if(nBytes != 0 || nErrorCode != 0)
					pSocket->OnReceive(nErrorCode);
				break;
			}
			case FD_WRITE:
				ATLTRACE(_T("FD_WRITE"));
				StopTimerCB(pSocket, m_SendTimeout);
				pSocket->OnSend(nErrorCode);
				break;
			case FD_OOB:
				ATLTRACE(_T("FD_OOB"));
				StopTimerCB(pSocket, m_ReceiveTimeout);
				pSocket->OnOutOfBandData(nErrorCode);
				break;
			case FD_ACCEPT:
				ATLTRACE(_T("FD_ACCEPT"));
				StopTimerCB(pSocket, m_AcceptTimeout);
				pSocket->OnAccept(nErrorCode);
				break;
			case FD_CONNECT:
				ATLTRACE(_T("FD_CONNECT"));
				StopTimerCB(pSocket, m_ConnectTimeout);
				pSocket->OnConnect(nErrorCode);
				pSocket->m_ConnectTimeout = 0;
				break;
			case FD_CLOSE:
				DWORD nBytes;
				bool drain = true;

				ATLTRACE(_T("FD_CLOSE"));

				while(drain)
				{
					if(!pSocket->IOCtl(FIONREAD, &nBytes))
					{
						pSocket->OnClose(pSocket->WSAGetLastError());
						drain = false;
					} else {
						if(nBytes != 0)
							pSocket->OnReceive(nErrorCode);
						else
						{
							pSocket->OnClose(nErrorCode);
							drain = false;
						}
					}
				} 
				break;
		}
		return 0;
	} 
	if (msg == WM_TIMER && wParam==101)
	{
		pSocket->OnTimer();
		return 0;
	}
	if( msg == WM_TIMER && pSocket->m_fBlocking == FALSE) 
	{
		ATLTRACE(_T("WM_TIMER in CMSocket CALLBACK"));
		if( pSocket->m_AcceptTimeoutCounter > 0 && --(pSocket->m_AcceptTimeoutCounter) == 0 )
			pSocket->OnAccept(WSAETIMEDOUT);
		if( pSocket->m_ConnectTimeoutCounter > 0 && --(pSocket->m_ConnectTimeoutCounter) == 0 )
			pSocket->OnConnect(WSAETIMEDOUT);
		if( pSocket->m_ReceiveTimeoutCounter > 0 && --(pSocket->m_ReceiveTimeoutCounter) == 0 )
			pSocket->OnReceive(WSAETIMEDOUT);
		if( pSocket->m_SendTimeoutCounter > 0 && --(pSocket->m_SendTimeoutCounter ) == 0 )
			pSocket->OnSend(WSAETIMEDOUT);

		if( pSocket->m_AcceptTimeoutCounter == 0 &&
			pSocket->m_ConnectTimeoutCounter == 0 &&
			pSocket->m_SendTimeoutCounter == 0 && 
			pSocket->m_ReceiveTimeoutCounter == 0 )
		{
			KillTimer(pSocket->m_hwndSocket, pSocket->m_TimerID);
			pSocket->m_TimerID = 0;
		}
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

CMSocket::CMSocket(LPTSTR libname, WSADATA* pWsaData)
{
	m_hSocket = INVALID_SOCKET;
	m_fBlocking = TRUE;
	m_fIsBlocking = FALSE;

	m_AcceptTimeout =
	m_AcceptTimeoutCounter =
	m_ConnectTimeout =
	m_ConnectTimeoutCounter =
	m_SendTimeout =
	m_SendTimeoutCounter =
	m_ReceiveTimeout =
	m_ReceiveTimeoutCounter = 0;

	m_BlockingMode = TrueBlocking;

	m_TimerID = 0;

	SocketLibAttach(libname, pWsaData);
	_tcsnccpy(m_Libname, libname, sizeof(m_Libname));
	CreateSocketWindow();
}

CMSocket::~CMSocket()
{
	if (m_hSocket != INVALID_SOCKET)
		Close();
	if( m_TimerID != 0 )
		KillTimer(m_hwndSocket, m_TimerID);
	if( m_hwndSocket != NULL )
		DestroyWindow(m_hwndSocket);

	SocketLibDetach();
}

int CMSocket::Block( int events )
{
	int retval = 0;
	BOOL fLoop = TRUE;
	BOOL fHaveMsg;

	ATLTRACE(_T("CMSocket::Block\n"));
	while(fLoop && hInstWinsock != NULL)
	{
		MSG msg;

		WaitMessage();
		//
		// Pick up socket messages first, this will prevent confusing
		// behaviour if a timeout and event arrive at the same time
		// since the WM_TIMER message would be serviced first if we
		// simply grabbed all messages.
		//
		if(::PeekMessage(&msg, m_hwndSocket, WM_SOCKET_NOTIFY, WM_SOCKET_NOTIFY, PM_REMOVE))
		{
			if( (SOCKET)msg.wParam == m_hSocket && WSAGETSELECTEVENT(msg.lParam) == FD_CLOSE )
			{
				fLoop = FALSE;
				retval = FD_CLOSE;
			} else if(WSAGETSELECTEVENT(msg.lParam) & events) {
				if(events & FD_CONNECT & WSAGETSELECTEVENT(msg.lParam))
					m_nConnectError = WSAGETSELECTERROR(msg.lParam);
				fLoop = FALSE;
				retval = WSAGETSELECTEVENT(msg.lParam) & events;
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			continue;
		} 
		//
		// Only deal with paint and timer messages if "true blocking" enabled
		//
		if( m_BlockingMode == TrueBlocking )
		{
			//
			// Service all paint messages
			//
			if( ::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE) )
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			//
			// And any socket directed timer messages
			//
			fHaveMsg = ::PeekMessage(&msg, m_hwndSocket, WM_TIMER, WM_TIMER, PM_REMOVE);
		} else {
			//
			// Pseudo blocking, take everything
			//
			fHaveMsg = ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		}
		if( fHaveMsg )
		{
			if( msg.message == WM_TIMER )
			{
				if( msg.wParam == (WPARAM)m_TimerID )
				{
					//

					// We can get away with this here since only one timer
					// at a time will be set when in blocking mode.
					//
					if( (m_AcceptTimeoutCounter > 0 && --m_AcceptTimeoutCounter == 0) ||
						(m_ConnectTimeoutCounter > 0 && --m_ConnectTimeoutCounter == 0) ||
						(m_ReceiveTimeoutCounter > 0 && --m_ReceiveTimeoutCounter == 0 ) ||
						(m_SendTimeoutCounter > 0 && --m_SendTimeoutCounter == 0 ) )
					{
						KillTimer(m_hwndSocket, 1);
						m_TimerID = 0;
						WSASetLastError(WSAETIMEDOUT);
						retval = WM_TIMER;
					} else {
						retval = 0;
					}
					fLoop = FALSE;
					break;
				}
			} else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return retval;
}

BOOL CMSocket::Accept(CMSocket& ConnectedSocket, SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{
	SOCKET  hSock;

	IfBlockingReturn(FALSE);

	hSock = accept(m_hSocket, lpSockAddr, lpSockAddrLen);

	if( WSAGetLastError() == WSAEWOULDBLOCK )
	{
		WSASetLastError(WSAEWOULDBLOCK);
		if( hSock == INVALID_SOCKET )
			StartTimer(m_AcceptTimeout);
	}

	if( m_fBlocking )
	{
		BeginBlocking();
		while(WSAGetLastError() == WSAEWOULDBLOCK &&  
			  m_fBlocking && 
			  m_fCancelBlockingCall == FALSE)
		{
			int event = Block(FD_ACCEPT|FD_CLOSE);

			if( event == FD_CLOSE )
			{
				hSock = INVALID_SOCKET;
				WSASetLastError(WSAECONNRESET);
			} else if( event == WM_TIMER ) {
				hSock = INVALID_SOCKET;
				WSASetLastError(WSAETIMEDOUT);
			} else {
				hSock = accept(m_hSocket, lpSockAddr, lpSockAddrLen);
			}
		}
		EndBlocking();
		StopTimer(m_AcceptTimeout);
	}

	if( ConnectedSocket.m_hSocket != INVALID_SOCKET )
		closesocket(ConnectedSocket.m_hSocket);
	ConnectedSocket.m_hSocket = hSock;

	if(hSock == INVALID_SOCKET)
	{
		int dwProblem = WSAGetLastError();
		ConnectedSocket.m_hSocket = INVALID_SOCKET;
		WSASetLastError(dwProblem);
	}
	return(hSock != INVALID_SOCKET);
}

BOOL CMSocket::AsyncSelect(long lEvent)
{
	return WSAAsyncSelect(m_hSocket, m_hwndSocket,	WM_SOCKET_NOTIFY, lEvent) != SOCKET_ERROR;
}


BOOL CMSocket::Bind(UINT nSocketPort, LPTSTR lpszSocketAddress)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;

	if (lpszSocketAddress == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		DWORD lResult = inet_addr(lpszSocketAddress);
		if (lResult == INADDR_NONE)
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
		sockAddr.sin_addr.s_addr = lResult;
	}

	sockAddr.sin_port = htons((u_short)nSocketPort);
	return (SOCKET_ERROR != bind(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr))); 
}

BOOL CMSocket::Close()
{
	BOOL fOk = TRUE;

	if (m_hSocket != INVALID_SOCKET)
	{
		fOk = (closesocket(m_hSocket) != SOCKET_ERROR);
		m_hSocket = INVALID_SOCKET;
	}
	return fOk;
}

BOOL CMSocket::Connect(LPTSTR lpszHostAddress, UINT nHostPort)
{
	SOCKADDR_IN sockAddr;
	BOOL fOk = TRUE;

	ATLTRACE(_T("Connect\n"));
	IfBlockingReturn(FALSE);

	memset(&sockAddr,0,sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszHostAddress);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszHostAddress);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	m_nConnectError = 0;

	fOk = (connect(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) != SOCKET_ERROR);
	if( !fOk && (WSAGetLastError() == WSAEWOULDBLOCK) )
	{
		StartTimer(m_ConnectTimeout);
		if( m_fBlocking )
		{
			BeginBlocking();
			while( m_fCancelBlockingCall == FALSE )
			{
				int event = Block(FD_CONNECT|FD_CLOSE);

				if( event == FD_CLOSE )
				{
					fOk = FALSE;
					WSASetLastError(WSAECONNRESET);
					m_fIsBlocking = FALSE;
					break;
				} else if( event == FD_CONNECT ) {
					if( m_nConnectError != 0 )
					{
						WSASetLastError(m_nConnectError);
						fOk = FALSE;
					} else {
						fOk = TRUE;
					}
					break;
				} else if( event == WM_TIMER ) {
					fOk = FALSE;
					WSASetLastError(20605);
					break;
				}
			}
			StopTimer(m_ConnectTimeout);
			EndBlocking();
		}
	}
	return fOk;
}


BOOL CMSocket::Create(UINT nSocketPort, int nSocketType, long lEvent, LPTSTR lpszSocketAddress)
{
	if( m_fBlocking )
		lEvent = 0x3f;

	if(Socket(nSocketType, lEvent))
	{
		if (Bind(nSocketPort,lpszSocketAddress))
			return TRUE;
		int nResult = WSAGetLastError();
		Close();
		WSASetLastError(nResult);
	}
	return FALSE;
}

BOOL CMSocket::GetPeerName(CString& rPeerAddress, UINT& rPeerPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = GetPeerName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rPeerPort = ntohs(sockAddr.sin_port);
		rPeerAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

BOOL CMSocket::GetHostName(CString& rHostName, CString& rHostIP)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	_TCHAR tmpName[256];
	int i=256;
	PHOSTENT hostinfo;
	BOOL bResult=FALSE;

    if( gethostname ( tmpName, 256 ) == 0)
		if((hostinfo = gethostbyname(tmpName)) != NULL)
		{
			rHostIP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			rHostName=tmpName;
			bResult=TRUE;
		}
		
		return bResult;
}

BOOL CMSocket::getsockName(CString& rSocketAddress, UINT& rSocketPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getsockName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

BOOL CMSocket::IOCtl(long lCommand, DWORD* lpArgument)
{ 
	//
	// Keep track of whether we're in blocking mode or not
	//
	if( lCommand == FIONBIO )
	{
		//
		// We don't allow the mode to change while a blocking
		// call is in progress.
		//
		if( m_fBlocking && IsBlocking() )
		{
			WSASetLastError(WSAEINPROGRESS);
			return FALSE;
		}
		//
		// For blocking sockets we enable all events. Twisted?  You bet.
		//
		if( m_fBlocking = (*lpArgument == 0) )
			return WSAAsyncSelect(m_hSocket, m_hwndSocket, WM_SOCKET_NOTIFY, 0x3f) != SOCKET_ERROR;
	}
	return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument)); 
}

BOOL CMSocket::IsBlocking()
{
	return m_fIsBlocking;
}

int CMSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
{
	int nResult;

	ATLTRACE(_T("CMSocket::Receive\n"));

	if( m_fBlocking && m_fIsBlocking == FALSE )
	{
		BeginBlocking();
		StartTimer(m_ReceiveTimeout);
		while((nResult = recv(m_hSocket, (LPTSTR)lpBuf, nBufLen, nFlags)) == SOCKET_ERROR)
		{
			if(WSAGetLastError() == WSAEWOULDBLOCK)
			{
				int event = Block(FD_READ|FD_OOB|FD_CLOSE);

				if( event != 0 && event != FD_READ && event != FD_OOB )
				{
					nResult = SOCKET_ERROR;
					break;
				}
				if( m_fCancelBlockingCall == TRUE )
				{
					WSASetLastError(WSAEINTR);
					nResult = SOCKET_ERROR;
					break;
				}
			} else {
				nResult = SOCKET_ERROR;
				break;
			}
		}
		StopTimer(m_ReceiveTimeout);
		EndBlocking();
	} else {
		nResult = recv(m_hSocket, (LPTSTR)lpBuf, nBufLen, nFlags);
		if( nResult == SOCKET_ERROR )
			if( WSAGetLastError() == WSAEWOULDBLOCK )
				StartTimer(m_ReceiveTimeout);
	}

	return nResult;
}

int CMSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags)
{
	SOCKADDR_IN sockAddr;
	int nSockAddrLen = sizeof(sockAddr);
	int	nResult;

	IfBlockingReturn(SOCKET_ERROR);

	memset(&sockAddr, 0, sizeof(sockAddr));

	nResult = recvfrom(m_hSocket, (LPTSTR)lpBuf, nBufLen, nFlags, (SOCKADDR*)&sockAddr, &nSockAddrLen);

	if( nResult == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK )
		StartTimer(m_ReceiveTimeout);
	if( m_fBlocking && m_fIsBlocking == FALSE )
	{
		BeginBlocking();
		while((nResult = nResult = recvfrom(m_hSocket, (LPTSTR)lpBuf, nBufLen, nFlags, (SOCKADDR*)&sockAddr, &nSockAddrLen)) == SOCKET_ERROR)
		{
			if(WSAGetLastError() == WSAEWOULDBLOCK)
			{
				int event = Block(FD_READ|FD_OOB|FD_CLOSE);

				if( event != 0 && event != FD_READ && event != FD_OOB )
				{
					nResult = SOCKET_ERROR;
					break;
				}
				if( m_fCancelBlockingCall == TRUE )
				{
					WSASetLastError(WSAEINTR);
					nResult = SOCKET_ERROR;
					break;
				}
			} else {
				nResult = SOCKET_ERROR;
				break;
			}
		}
		StopTimer(m_ReceiveTimeout);
		EndBlocking();
	}
	if(nResult != SOCKET_ERROR)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return nResult;
}

int CMSocket::BlockingSend(const void* lpBuf, int nBufLen, int nFlags)
{
	int nLeft, nWritten;
	_TCHAR far * pBuf = (_TCHAR far *)lpBuf;
	nLeft = nBufLen;

	while(nLeft > 0)
	{
		while((nWritten = send(m_hSocket, (LPTSTR)lpBuf, nBufLen, nFlags)) == SOCKET_ERROR)
		{
			if(WSAGetLastError() == WSAEWOULDBLOCK)
			{
				switch( Block(FD_WRITE | FD_CLOSE) )
				{
					case 0:
					case FD_WRITE:
						break;
					default:
						return SOCKET_ERROR;
				}
				if( m_fCancelBlockingCall )
				{
					WSASetLastError(WSAEINTR);
					return SOCKET_ERROR;
				}
			} else {
				return SOCKET_ERROR;
			}
		}
		nLeft -= nWritten;
		pBuf += nWritten;
	}
	m_fIsBlocking = FALSE;
	return nBufLen;
}

int CMSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	int result;

	ATLTRACE(_T("CMSocket::Send\n"));

	IfBlockingReturn(SOCKET_ERROR);
	
	result = send(m_hSocket, (LPTSTR)lpBuf, nBufLen, nFlags);
	if( result == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK )
	{
		StartTimer(m_SendTimeout);
		if( m_fBlocking )
		{
			BeginBlocking();
			result = BlockingSend( lpBuf, nBufLen, nFlags );
			StopTimer(m_SendTimeout);
			EndBlocking();
		}
	}
	return result;
}

int CMSocket::BlockingSendTo(SOCKET m_hSocket, LPTSTR lpBuf, int nBufLen, int nFlags, SOCKADDR* sockAddr, int nSockAddr)
{
	int nLeft, nWritten;
	_TCHAR far * pBuf = (_TCHAR far *)lpBuf;
	nLeft = nBufLen;

	while(nLeft > 0)
	{
		while((nWritten = sendto(m_hSocket, (LPTSTR)lpBuf, nBufLen, nFlags, (SOCKADDR*)&sockAddr, sizeof(sockAddr))) == SOCKET_ERROR)
		{
			if(WSAGetLastError() == WSAEWOULDBLOCK)
			{
				switch( Block(FD_WRITE | FD_CLOSE) )
				{
					case 0:
					case FD_WRITE:
						break;
					default:
						return SOCKET_ERROR;
				}
				if( m_fCancelBlockingCall )
				{
					WSASetLastError(WSAEINTR);
					return SOCKET_ERROR;
				}
			} else {
				return SOCKET_ERROR;
			}
		}
		nLeft -= nWritten;
		pBuf += nWritten;
	}
	m_fIsBlocking = FALSE;
	return nBufLen;
}

int CMSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress, int nFlags)
{
	SOCKADDR_IN sockAddr;
	int result;

	IfBlockingReturn(SOCKET_ERROR);

	memset(&sockAddr,0,sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;

	if (lpszHostAddress == NULL)
	{
		sockAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	} else {
		sockAddr.sin_addr.s_addr = inet_addr(lpszHostAddress);
		if (sockAddr.sin_addr.s_addr == INADDR_NONE)
		{
			LPHOSTENT lphost;
			lphost = gethostbyname(lpszHostAddress);
			if (lphost != NULL)
				sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			else
			{
				WSASetLastError(WSAEINVAL);
				return FALSE;
			}
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	result = sendto(m_hSocket, (LPTSTR)lpBuf, nBufLen, nFlags, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	if( result == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK )
		StartTimer(m_SendTimeout);
	if( m_fBlocking )
	{
		BeginBlocking();
		result = BlockingSendTo(m_hSocket, (LPTSTR)lpBuf, nBufLen, nFlags, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
		StopTimer(m_SendTimeout);
		EndBlocking();
	}
	return result;

}

BOOL CMSocket::Listen(int nConnectionBacklog)
{
	BOOL ret = (listen(m_hSocket, nConnectionBacklog) == 0);

	if( ret == TRUE )
		StartTimer(m_ConnectTimeout);
	return ret;
}

BOOL CMSocket::Socket(int nSocketType, long lEvent, int nProtocolType, int nAddressFormat)
{
	m_hSocket = socket(nAddressFormat,nSocketType,nProtocolType);
	if (m_hSocket != INVALID_SOCKET)
//	{
//		if( m_fBlocking == FALSE )
			return AsyncSelect(lEvent);
//	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CMSocket Overridable callbacks

void CMSocket::OnReceive(int /*nErrorCode*/)
{
}

void CMSocket::OnSend(int /*nErrorCode*/)
{
}

void CMSocket::OnOutOfBandData(int /*nErrorCode*/)
{
}

void CMSocket::OnAccept(int /*nErrorCode*/)
{
}

void CMSocket::OnConnect(int /*nErrorCode*/)
{
}

void CMSocket::OnClose(int /*nErrorCode*/)
{
}

void CMSocket::OnTimer(void)
{
}


//
// sockstub
//
#define _ACCEPT _T("accept")
#define _BIND _T("bind")
#define _CLOSESOCKET _T("closesocket")
#define _CONNECT _T("connect")
#define _GETHOSTBYADDR _T("gethostbyaddr")
#define _GETHOSTBYNAME _T("gethostbyname")
#define _GETHOSTNAME _T("gethostname")
#define _GETPEERNAME _T("getpeername")
#define _getsOCKNAME _T("getsockname")
#define _getsOCKOPT _T("getsockopt")
#define _HTONL _T("htonl")
#define _HTONS _T("htons")
#define _INET_ADDR _T("inet_addr")
#define _INET_NTOA _T("inet_ntoa")
#define _IOCTLSOCKET _T("ioctlsocket")
#define _LISTEN _T("listen")
#define _NTOHS _T("ntohs")
#define _RECV _T("recv")
#define _RECVFROM _T("recvfrom")
#define _SEND _T("send")
#define _SENDTO _T("sendto")
#define _SETSOCKOPT _T("setsockopt")
#define _SHUTDOWN _T("shutdown")
#define _SOCKET _T("socket")
#define _WSASTARTUP _T("WSAStartup")
#define _WSACLEANUP _T("WSACleanup")
#define _WSAGETLASTERROR _T("WSAGetLastError")
#define _WSAISBLOCKING _T("WSAIsBlocking")
#define _WSAASYNCSELECT _T("WSAAsyncSelect")
#define _WSACANCELBLOCKINGCALL _T("WSACancelBlockingCall")
#define _WSASETBLOCKINGHOOK _T("WSASetBlockingHook")
#define _WSAUNHOOKBLOCKINGHOOK _T("WSAUnhookBlockingHook")
#define _WSASETLASTERROR _T("WSASetLastError")

#ifndef MAKEWORD

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#endif

BOOL CMSocket::SocketLibAttach(LPTSTR LPSTRLibName, WSADATA* pWsaData)
{
   WORD  junk = 0;

	if( LoadWinsock(LPSTRLibName) != NULL)
	{
		WORD wVersionRequested = MAKEWORD(2, 0);

		if( WSAStartup(wVersionRequested, pWsaData) )
		{
			wVersionRequested = MAKEWORD(1, 1);
			if( WSAStartup(wVersionRequested, pWsaData) )
			{
				wVersionRequested = MAKEWORD(1, 0);
				if( WSAStartup(wVersionRequested, pWsaData) )
				return FALSE;
			}
		}
		if( winsockUsage != 0 )
			WSACleanup();
		InterlockedIncrement(&winsockUsage);
	}
	return hInstWinsock != NULL;
}

//
// This MUST be called once for each call to SocketLibAttach
//
void CMSocket::SocketLibDetach()
{                                  
	if( hInstWinsock != NULL )
	{
		InterlockedDecrement(&winsockUsage);
		if( winsockUsage <= 0 )
		{
			WSACleanup();
			FreeLibrary( hInstWinsock );
			hInstWinsock = NULL;

		}
	}
}


HINSTANCE CMSocket::LoadWinsock(LPTSTR lpstrLibName)
{                 
	if( hInstWinsock == NULL )
	{	
		if( (hInstWinsock = LoadLibrary(lpstrLibName)) != NULL )
		{
			(FARPROC&)pfn_accept = GetProcAddress(hInstWinsock, _ACCEPT);
			(FARPROC&)pfn_bind = GetProcAddress(hInstWinsock, _BIND);
			(FARPROC&)pfn_closesocket = GetProcAddress(hInstWinsock, _CLOSESOCKET);
			(FARPROC&)pfn_connect = GetProcAddress(hInstWinsock, _CONNECT);
			(FARPROC&)pfn_gethostbyaddr = GetProcAddress(hInstWinsock, _GETHOSTBYADDR);
			(FARPROC&)pfn_gethostname = GetProcAddress(hInstWinsock, _GETHOSTNAME);
			(FARPROC&)pfn_gethostbyname = GetProcAddress(hInstWinsock, _GETHOSTBYNAME);
			(FARPROC&)pfn_getpeername = GetProcAddress(hInstWinsock, _GETPEERNAME);
			(FARPROC&)pfn_getsockname = GetProcAddress(hInstWinsock, _getsOCKNAME);
			(FARPROC&)pfn_getsockopt = GetProcAddress(hInstWinsock, _getsOCKOPT);
			(FARPROC&)pfn_htonl = GetProcAddress(hInstWinsock, _HTONL);
			(FARPROC&)pfn_htons = GetProcAddress(hInstWinsock, _HTONS);
			(FARPROC&)pfn_inet_addr = GetProcAddress(hInstWinsock, _INET_ADDR);
			(FARPROC&)pfn_inet_ntoa = GetProcAddress(hInstWinsock, _INET_NTOA);
			(FARPROC&)pfn_ioctlsocket = GetProcAddress(hInstWinsock, _IOCTLSOCKET);
			(FARPROC&)pfn_listen = GetProcAddress(hInstWinsock, _LISTEN);
			(FARPROC&)pfn_ntohs = GetProcAddress(hInstWinsock, _NTOHS);
			(FARPROC&)pfn_recv = GetProcAddress(hInstWinsock, _RECV);
			(FARPROC&)pfn_recvfrom = GetProcAddress(hInstWinsock, _RECVFROM);
			(FARPROC&)pfn_send = GetProcAddress(hInstWinsock, _SEND);
			(FARPROC&)pfn_sendto = GetProcAddress(hInstWinsock, _SENDTO);
			(FARPROC&)pfn_setsockopt = GetProcAddress(hInstWinsock, _SETSOCKOPT);
			(FARPROC&)pfn_shutdown = GetProcAddress(hInstWinsock, _SHUTDOWN);
			(FARPROC&)pfn_socket = GetProcAddress(hInstWinsock, _SOCKET);
			(FARPROC&)pfn_WSAStartup = GetProcAddress(hInstWinsock, _WSASTARTUP);
			(FARPROC&)pfn_WSACleanup = GetProcAddress(hInstWinsock, _WSACLEANUP);
			(FARPROC&)pfn_WSAGetLastError = GetProcAddress(hInstWinsock, _WSAGETLASTERROR);
			(FARPROC&)pfn_WSAIsBlocking = GetProcAddress(hInstWinsock, _WSAISBLOCKING);
			(FARPROC&)pfn_WSAAsyncSelect = GetProcAddress(hInstWinsock, _WSAASYNCSELECT);
			(FARPROC&)pfn_WSACancelBlockingCall = GetProcAddress(hInstWinsock, _WSACANCELBLOCKINGCALL);
			(FARPROC&)pfn_WSASetBlockingHook = GetProcAddress(hInstWinsock, _WSASETBLOCKINGHOOK);
			(FARPROC&)pfn_WSASetLastError = GetProcAddress(hInstWinsock, _WSASETLASTERROR);
			(FARPROC&)pfn_WSAUnhookBlockingHook = GetProcAddress(hInstWinsock, _WSAUNHOOKBLOCKINGHOOK);
			
			if( pfn_accept == NULL || pfn_bind == NULL || pfn_closesocket == NULL || pfn_connect == NULL ||
				pfn_gethostbyaddr == NULL || pfn_gethostbyname == NULL || pfn_getpeername == NULL ||
				pfn_getsockname == NULL || pfn_getsockopt == NULL || pfn_htonl == NULL || pfn_htons == NULL ||
				pfn_inet_addr == NULL || pfn_inet_ntoa == NULL || pfn_ioctlsocket == NULL ||
				pfn_listen == NULL || pfn_ntohs == NULL || pfn_recv == NULL || pfn_recvfrom == NULL ||

				pfn_send == NULL || pfn_sendto == NULL || pfn_setsockopt == NULL || pfn_shutdown == NULL ||
				pfn_socket == NULL || pfn_WSAStartup == NULL || pfn_WSACleanup == NULL ||
				pfn_WSAGetLastError == NULL || pfn_WSAAsyncSelect == NULL || pfn_WSASetLastError == NULL ||
				pfn_WSAIsBlocking == NULL || pfn_WSASetBlockingHook == NULL || pfn_WSACancelBlockingCall == NULL ||
				pfn_gethostname == NULL )
			{                           
    			FreeLibrary( hInstWinsock );
    			hInstWinsock = FALSE;
			} 
		}
	}
	return hInstWinsock;
}                            

