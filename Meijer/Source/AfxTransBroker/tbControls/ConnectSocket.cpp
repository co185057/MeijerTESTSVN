#include "StdAfx.h"
#include "connectsocket.h"

#pragma comment(lib, "wsock32.lib")

#define		MAX_PENDING_CONNECTS	50

bool CConnectSocket::m_bSocketsInitialized= false;

CConnectSocket::CConnectSocket(void)
{
}

CConnectSocket::CConnectSocket(LPCTSTR lpszSocketAddr, int nPort)
:CConnectBase()
{
	m_connData.socketPort=nPort;
	m_connData.socketAddr= lpszSocketAddr;
}

CConnectSocket::~CConnectSocket(void)
{
}

bool CConnectSocket::Initialize()
{
   bool bsuccess=true;
   if (!m_bSocketsInitialized)
      bsuccess= InitSockets();
   if (bsuccess) {
//	   ATLASSERT(m_handle == INVALID_HANDLE_VALUE);
	   m_handle = (HANDLE) socket(AF_INET, SOCK_STREAM, 0);
	   ATLASSERT(m_handle != INVALID_HANDLE_VALUE);
   }
	return (m_handle != INVALID_HANDLE_VALUE);
}

BOOL CConnectSocket::GetPeerName(CString& rPeerAddress, UINT& rPeerPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername((SOCKET)m_handle,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rPeerPort = ntohs(sockAddr.sin_port);
		rPeerAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

BOOL CConnectSocket::GetSockName(CString& rSocketAddress, UINT& rSocketPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = ::getsockname((SOCKET)m_handle,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
 //CConnectSocket Operations

bool CConnectSocket::Accept()
{
	HANDLE hSocket;
	do {
		hSocket = (HANDLE) accept((SOCKET)m_handle, NULL, NULL);
		if (hSocket == INVALID_HANDLE_VALUE) {
			DWORD dwProblem = GetLastError();
			SetLastError(dwProblem);
		} else
			OnAccept(hSocket);
	}
	while (hSocket != INVALID_HANDLE_VALUE);
	return (hSocket != INVALID_HANDLE_VALUE);
}

bool CConnectSocket::Listen()
{
	bool bSuccess=true;

	USES_CONVERSION;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;

//	LPSTR lpszAscii = T2A((LPTSTR)lpszSocketAddress);
//	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//	else{
//		DWORD lResult = inet_addr(lpszAscii);
//		if (lResult == INADDR_NONE)	{
//			WSASetLastError(WSAEINVAL);
//			return FALSE;
//		}
//		sockAddr.sin_addr.s_addr = lResult;
//	}
	sockAddr.sin_port = htons((u_short)m_connData.socketPort);
	if (bind((SOCKET)m_handle,(SOCKADDR*)&sockAddr, sizeof(sockAddr))== SOCKET_ERROR) {
		int nResult = GetLastError();
		Close();
		WSASetLastError(nResult);
		bSuccess=false;
	}
	else if (listen((SOCKET)m_handle, MAX_PENDING_CONNECTS ) == SOCKET_ERROR){
		WSAGetLastError();
		bSuccess= false;
	}
	if (!bSuccess)
		Close();
	return bSuccess;
}

void CConnectSocket::Close()
{
	if (m_handle != INVALID_HANDLE_VALUE)
	{
		closesocket((SOCKET)m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
}

bool CConnectSocket::Connect()
{
	USES_CONVERSION;

	ATLASSERT(!m_connData.socketAddr.IsEmpty());
	ATLASSERT(m_handle != INVALID_HANDLE_VALUE);

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = T2A((LPTSTR)m_connData.socketAddr.GetBuffer(0));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAscii);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	sockAddr.sin_port = htons((u_short)m_connData.socketPort);
	return (connect((SOCKET)m_handle, (SOCKADDR*)&sockAddr, sizeof(sockAddr))!= SOCKET_ERROR);
}

bool CConnectSocket::Connect(LPCTSTR lpszSocketAddr, int nPort)
{
	m_connData.socketPort=nPort;
	m_connData.socketAddr= lpszSocketAddr;
   return Connect();
}

int CConnectSocket::Receive(void* lpBuf, int nCount, int nFlags/*=0*/)
{
  
	int cbBytesRead = recv((SOCKET)m_handle, (char *)lpBuf, nCount, nFlags);
	if (cbBytesRead == SOCKET_ERROR){
		WSAError();
  		ATLTRACE(_T("Error Reading From Socket Error #: %d"), WSAGetLastError());
	}
   if (nFlags != MSG_PEEK) {
   	++m_connData.ReadPasses;
	   m_connData.BytesRead += cbBytesRead;
	   Received();
   }
	return cbBytesRead;
}

int CConnectSocket::ReceiveCount(void* lpBuf, int nCount, int nFlags /*=0*/)
{
   /*
   **  A read() from a socket could return without reading all the bytes.
   **  This function retries until all bytes are read.
   */
	int nRead;
	int nLeft = nCount;
	PBYTE pBuf = (PBYTE)lpBuf;
	while(nLeft > 0) {
		nRead = recv((SOCKET)m_handle, (char *)pBuf, nLeft, nFlags);
		if (nRead == SOCKET_ERROR){
			WSAError();
       	ATLTRACE(_T("Error Reading From Socket Error #: %d"), WSAGetLastError());
			return nRead;
		} else if (nRead == 0) {
			WSAError();
			ATLTRACE(_T("Error:Socket read returned 0 bytes...error #: %d"), WSAGetLastError());
			return nCount - nLeft;
		}
		nLeft -= nRead;
		pBuf += nRead;
	}
	nRead= nCount - nLeft;
   if (nFlags != MSG_PEEK) {
   	++m_connData.ReadPasses;
	   m_connData.BytesRead += nRead;
	   Received();
   }
   return nRead;
}

int CConnectSocket::Send(CConnectBase *pConnSource, const void* lpBuf, int nBufLen)
{
	m_connData.WritePasses++;
	int wc = send((SOCKET)m_handle, (char*)lpBuf, nBufLen, 0);
	if (wc == SOCKET_ERROR){ 
		 WSAError();		
         return -1;
    }
    else if(wc == 0) {
         ATLTRACE("Error:socket send() returned 0 bytes");
         return -1;
    }
	if (wc) {
		m_connData.BytesWritten += wc;
	}
	Sent(pConnSource);
   return wc;
}

int CConnectSocket::Send(const void* lpBuf, int nBufLen)
{
   return Send (this, lpBuf, nBufLen);
}

int CConnectSocket::SendCount(const void* lpBuf, int nBufLen)
{
   return SendCount (this, lpBuf, nBufLen);
}

int CConnectSocket::SendCount(CConnectBase *pConnSource, const void* lpBuf, int nBufLen)
{
   char *ptr;
   int   n, wc=0;
   /*  A write() to a socket could return without writing all the bytes.
   **  This function retries until all bytes are written.
   */
   for(n = nBufLen, ptr = (char *)lpBuf ; n > 0 ; n -= wc, ptr += wc){
   	m_connData.WritePasses++;
      if((wc = send((SOCKET)m_handle, ptr, n, 0)) == SOCKET_ERROR){ 
		 WSAError();		
         return -1;
      }
      else if(wc == 0) {
         ATLTRACE("Error:socket send() returned 0 bytes");
         return -1;
      }
   }
	if (wc) {
		m_connData.BytesWritten += wc;
	}
	Sent(pConnSource);
   return wc;
}

BOOL CConnectSocket::AsyncSelect(long lEvent)
{
	return FALSE;
//	ATLASSERT(m_connData.handle != INVALID_HANDLE_VALUE);

//	_AFX_SOCK_THREAD_STATE* pState = _afxSockThreadState;
//	ASSERT(pState->m_connData.handleWindow != NULL);

//	return WSAAsyncSelect(m_connData.handle, pState->m_connData.handleWindow, WM_SOCKET_NOTIFY, lEvent) != SOCKET_ERROR;
}

// this function will request and wait to receive an ack 
// for the specified msgId and withing the msecs period
void CConnectSocket::ProcessAck(CConnectBase *pConnSource, BYTE cmsgId, long msecs)
{
   // first clear any possible data in receive buffer
   ClearBuffer();
   if (msecs > 0) {
      // request an ACK packet by sending just a header.
      tRouteHeader hdr;
      hdr.messageID= cmsgId;
      hdr.nLen= sizeof(hdr);
      hdr.nVersion= m_connData.nHeaderVersion;
      Send(pConnSource, &hdr, sizeof(hdr));

      ReceiveAck(msecs);   // block upto msecs to receive ACK
   }
}

// Wait up to msecs milliseconds to receive an ACK
void CConnectSocket::ReceiveAck(long msecs)
{
   fd_set fdset;
   fdset.fd_count=1;
   fdset.fd_array[0]= (SOCKET)m_handle;
   timeval tm;
   tm.tv_sec=0;
   tm.tv_usec=msecs*1000;  // microseconds
   int ret=select(1, &fdset, NULL, NULL, &tm);
   if (ret==0 || ret== SOCKET_ERROR) {
      ATLTRACE("ACK Timeout!");
      //MessageBox(NULL, _T("ACK Timout"), _T("ACK Timeout"), MB_OK);
   }
   else {
      BYTE buf[sizeof(tRouteHeader)];
      ReceiveCount(buf, sizeof(tRouteHeader));
      //MessageBox(NULL, _T("ACK Received"), _T("ACK Received"), MB_OK);
   }
}

void CConnectSocket::ClearBuffer(void)
{
   fd_set fdset;
   fdset.fd_count=1;
   fdset.fd_array[0]= (SOCKET)m_handle;
   timeval tm;
   tm.tv_sec=0;
   tm.tv_usec=0;
   int ret=select(1, &fdset, NULL, NULL, &tm);
   if (ret==0 || ret== SOCKET_ERROR)
      ATLTRACE("socket select() timeout");
   else {
      BYTE buf[2048];
      while (Receive(buf, sizeof(buf)) > 0);
   }
}

/////////////////////////////////////////////////////////////////////////////
// Overridable callbacks
void CConnectSocket::OnReceive(int /*nErrorCode*/)
{
}

void CConnectSocket::OnSend(int /*nErrorCode*/)
{
}

void CConnectSocket::OnOutOfBandData(int /*nErrorCode*/)
{
}

void CConnectSocket::OnAccept(HANDLE)
{
}

void CConnectSocket::OnConnect(int /*nErrorCode*/)
{
}

void CConnectSocket::OnClose(int /*nErrorCode*/)
{
}

bool CConnectSocket::InitSockets()
{
	WORD wVersionRequested; 
	WSADATA wsaData; 

	int err; 
	wVersionRequested = MAKEWORD(2, 0); 
	err = WSAStartup(wVersionRequested, &wsaData); 
	if (err != 0) {
		ATLTRACE("Could Not Initialize Sockets");
        WSACleanup(); 
        return false; 
	}
	if (LOBYTE( wVersionRequested) < 2) {
		ATLTRACE("Version Not Supported");
		return false; 
	}
	ATLTRACE("Windows Sockets 2.0 Initialized");
   m_bSocketsInitialized=true;
	return true;
}

void CConnectSocket::WSAError()
{
char Log_Error[256];
switch(WSAGetLastError()) {
	case WSAEADDRNOTAVAIL:
	    	sprintf(Log_Error, "The specified address is not available from the local computer");
	        break;
	case WSAEADDRINUSE:
			sprintf(Log_Error, "The specified address is already in use");
 			break;
	case WSAEAFNOSUPPORT:
			sprintf(Log_Error, "Addresses in the specified family cannot be used with this socket");
 			break;
	case WSAECONNABORTED:	
			sprintf(Log_Error, "Connection aborted due to timeout or other failure");
			break;
	case WSAECONNREFUSED:
            sprintf(Log_Error, "Connection refused, no server");
    		break;
	case WSAECONNRESET:				
			sprintf(Log_Error, "Connection was reset by the remote side");
			break;
	case WSAEDESTADDRREQ:	
            sprintf(Log_Error, "A destination address is required");
		    break;
	case WSAEFAULT:
			sprintf(Log_Error, "The namelen argument is incorrect");
 			break;
	case WSAEINPROGRESS:
			sprintf(Log_Error, "A blocking Windows Sockets call is in progress");
 			break;
	case WSAEINTR:
			sprintf(Log_Error, "The blocking call was canceled using WSACancelBlockingCall");
 			break;
	case WSAENETDOWN:
			sprintf(Log_Error, "The Windows Sockets implementation has detected that the network subsystem has failed");
 			break;
	case WSAEINVAL:
			sprintf(Log_Error, "The socket is already bound to an address");
 			break;
	case WSAEISCONN:
		     sprintf(Log_Error, "The socket is already connected");
			 break;
	case WSAEMFILE:
			sprintf(Log_Error, "No more file descriptors are available");
			break;
	case WSAEMSGSIZE:	
            sprintf(Log_Error, "The datagram was too large to fit into the specified buffer and was truncated");
			break;
    case WSAENETUNREACH:
            sprintf(Log_Error, "Network unreachable");
		    break;
	case WSAENOBUFS	:
			sprintf(Log_Error, "No buffer space is available. The socket cannot be connected");
 			break;
	case WSAENOTCONN:	
			sprintf(Log_Error, "The socket is not connected");
 			break;
	case WSAENOTSOCK:	
		    sprintf(Log_Error, "The descriptor is not a socket");
			break;
	case WSAEOPNOTSUPP:	
			sprintf(Log_Error, "MSG_OOB was specified, but the socket is not of type SOCK_STREAM");
			break;
	case WSAEPROTOTYPE:	
			sprintf(Log_Error, "The specified protocol is the wrong type for this socket");
			break;
	case WSAEPROTONOSUPPORT:	
			sprintf(Log_Error, "The specified protocol is not supported");
			break;
	case WSAESHUTDOWN:	
            sprintf(Log_Error, "The socket has been shut down; it is not possible to recv on a socket after shutdown has been invoked with how set to 0 or 2");
			break;
	case WSAESOCKTNOSUPPORT:	
			sprintf(Log_Error, "The specified socket type is not supported in this address family");
			break;
	case WSAETIMEDOUT:
            sprintf(Log_Error, "Connection timed out");
		    break;
	case WSAEWOULDBLOCK:
			sprintf(Log_Error, "The socket is marked as nonblocking and the connection cannot be completed immediately. It is possible to select the socket while it is connecting by selecting it for writing");
			break;
	case WSANOTINITIALISED:
			sprintf(Log_Error, "A successful WSAStartup must occur before using this function");
 			break;
	default:
		    sprintf(Log_Error, "Unknown error returned from WSAGetLastError()");
  }		    
  ATLTRACE(Log_Error);
}

