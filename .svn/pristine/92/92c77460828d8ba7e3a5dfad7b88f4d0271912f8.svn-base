#include "StdAfx.h"
#include "connectsocket.h"
#include "commserverdlg.h"

#define T_ID _T("ConnectSocket")

#pragma comment(lib, "wsock32.lib")

#define		MAX_PENDING_CONNECTS	50

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
    Close();
}

bool CConnectSocket::Initialize()
{
	ATLASSERT(m_handle == INVALID_HANDLE_VALUE);
	m_handle = (HANDLE) socket(AF_INET, SOCK_STREAM, 0);
	ATLASSERT(m_handle != INVALID_HANDLE_VALUE);

//	ULONG bufsize = 0;
//	ioctlsocket((SOCKET)m_handle, FIONREAD, &bufsize);

	return (m_handle != INVALID_HANDLE_VALUE);
}

bool CConnectSocket::Open()
{
//	if (m_bIsServer)
//		return Listen();
//	else
//		return Connect();
	return false;
};

void CConnectSocket::Create()
{
	Open();
}

void CConnectSocket::GetPeerName()
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	if (0 == getpeername((SOCKET)m_handle,(SOCKADDR*)&sockAddr, &nSockAddrLen))
	{
		m_connData.peerSocketPort = ntohs(sockAddr.sin_port);
		m_connData.peerSocketAddr = inet_ntoa(sockAddr.sin_addr);
	}
}

BOOL CConnectSocket::GetSockName()
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = ::getsockname((SOCKET)m_handle,(SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult==0)
	{
		m_connData.socketPort = ntohs(sockAddr.sin_port);
		m_connData.socketAddr = inet_ntoa(sockAddr.sin_addr);
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

//	m_bIsServer= true;
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
        shutdown((SOCKET) m_handle, SD_SEND);
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

int CConnectSocket::Receive(void* lpBuf, int nCount, int nFlags/*=0*/, CTraceSystem *pts)
{
  
	int cbBytesRead = recv((SOCKET)m_handle, (char *)lpBuf, nCount, nFlags);
	if (cbBytesRead == SOCKET_ERROR){
		WSAError(pts);
        if (pts)
            pts->Trace(TM_ERROR, _T("Error Reading From Socket"));
	}
    if (nFlags != MSG_PEEK) {
	   ++m_connData.ReadPasses;
	   m_connData.BytesRead += cbBytesRead;
	   Received();
    }
	return cbBytesRead;
}

int CConnectSocket::ReceiveCount(void* lpBuf, int nCount, int nFlags /*=0*/,CTraceSystem *pts)
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
		int ntries=0;
		while ( (nRead == SOCKET_ERROR) && 
			    ((WSAGetLastError() == WSAEWOULDBLOCK) ||
			     (WSAGetLastError() == WSAEINPROGRESS)) &&
				 (ntries++ <100)
			  ) 
		{
			Sleep(80); // try again
            WSAError(pts);
            if (pts)
               pts->Trace(TM_ERROR, _T("ReceiveCount incomplete- trying again"));

			nRead = recv((SOCKET)m_handle, (char *)pBuf, nLeft, nFlags);
		}
		if (nRead == SOCKET_ERROR){
			WSAError(pts);
            if (pts)
               pts->Trace(TM_ERROR, _T("ReceiveCount failed"));
			return WSAGetLastError()*-1;
		} else if (nRead == 0) {
			WSAError(pts);
            if (pts)
			    pts->Trace(TM_ERROR,_T("Error:Socket read returned 0 bytes"));
			return nCount - nLeft;
		}
        if (nFlags & MSG_PEEK) {    // we re-read entire buffer if MSG_PEEK
            if (nRead == nCount)
                nLeft=0; //done
        }
        else {
		    nLeft -= nRead;
		    pBuf += nRead;
        }
	}
	nRead= nCount - nLeft;
   if (nFlags != MSG_PEEK) {
   	++m_connData.ReadPasses;
	   m_connData.BytesRead += nRead;
	   Received();
   }
   return nRead;
}

/*blocks up to msecs waiting for data*/
int CConnectSocket::WaitReceive(void* lpBuf, int nCount, int msecs, int nFlags)
{
   fd_set fdset;
   fdset.fd_count=1;
   fdset.fd_array[0]= (SOCKET)m_handle;
   timeval tm;
   tm.tv_sec=0;
   tm.tv_usec=msecs*1000;  // microseconds
   int ret=select(1, &fdset, NULL, NULL, &tm);
   if (ret==0 || ret== SOCKET_ERROR) {
      ATLTRACE(_T("No data received within timeout!"));
   }
   else {
      ret= Receive(lpBuf, nCount, nFlags);
   }
	return ret;
}

int CConnectSocket::Send(CConnectBase *pConnSource, const void* lpBuf, int nBufLen, CTraceSystem *pts)
{
   return SendCount(pConnSource, lpBuf, nBufLen, pts);
/*
   m_connData.WritePasses++;
	int wc = send((SOCKET)m_handle, (char*)lpBuf, nBufLen, 0);
	if (wc == SOCKET_ERROR){ 
		 WSAError(pts);		
       return -1;
    }
    else if(wc == 0) {
		 WSAError(pts);		
       return -1;
    }
	if (wc) {
		m_connData.BytesWritten += wc;
	}
	Sent(pConnSource);
   return wc;
*/
}

int CConnectSocket::SendCount(CConnectBase *pConnSource, const void* lpBuf, int nBufLen, CTraceSystem *pts)
{
   char *ptr;
   int   n, wc=0;
   /*  A write() to a socket could return without writing all the bytes.
   **  This function retries until all bytes are written.
   */
   for(n = nBufLen, ptr = (char *)lpBuf ; n > 0 ; n -= wc, ptr += wc){
      m_connData.WritePasses++;
      if((wc = send((SOCKET)m_handle, ptr, n, 0)) == SOCKET_ERROR){ 
         int ntries=0;
         while ((wc == SOCKET_ERROR) && 
                (WSAGetLastError() == WSAEWOULDBLOCK) &&
     				 (ntries++ <100)) {
            WSAError(pts);
            if (pts)
               pts->Trace(TM_WARNING, _T("SendCount WSAEWOULDBLOCK- retrying after 100ms"));
            Sleep(50); // try again
            wc = send((SOCKET)m_handle, ptr, n, 0);
         }
         if(wc == SOCKET_ERROR) {
            WSAError(pts);
            if (pts)
               pts->Trace(TM_ERROR, _T("SendCount failed"));
            return -1;
         }
      }
      else if(wc <= 0) {
         WSAError(pts);
         return -1;
      }
      if (wc > 0) {
         m_connData.BytesWritten += wc;
      }
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
      ATLTRACE(_T("ACK Timeout!"));
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
      BYTE buf[1024];
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
   bool bSuccess= true;

   CoInitialize(NULL);
   CTraceSystem ts(CcommServerDlg::m_csTraceFileName);
   ts.Create();
   ts.SetModuleID(TM_MODULE_ID);

	wVersionRequested = MAKEWORD(2, 0); 
	int err = WSAStartup(wVersionRequested, &wsaData); 
	if (err != 0) {
      ts.Trace(TM_ERROR, _T("WSAStartup Error. Could not initialize sockets. err:%d"), err);
      WSACleanup(); 
      bSuccess= false; 
	}
   else if (LOBYTE( wVersionRequested) < 2) {
	   ts.Trace(TM_ERROR, _T("WSAStartup Error. Version < 2"));
		bSuccess= false; 
	}
   else
      ts.Trace(TM_INFO, _T("Windows Sockets 2.0 Initialized"));
   CoUninitialize();
	return bSuccess;
}

void CConnectSocket::WSAError(CTraceSystem *pts)
{
   CString cs;
   switch(WSAGetLastError()) {
   case WSAEADDRNOTAVAIL:
      cs= _T("WSAEADDRNOTAVAIL:The specified address is not available from the local computer");
      break;
   case WSAEADDRINUSE:
      cs= _T("WSAEADDRINUSE:The specified address is already in use");
      break;
   case WSAEAFNOSUPPORT:
      cs= _T("WSAEAFNOSUPPORT:Addresses in the specified family cannot be used with this socket");
      break;
   case WSAECONNABORTED:	
      cs= _T("WSAECONNABORTED:Connection aborted due to timeout or other failure");
      break;
   case WSAECONNREFUSED:
      cs= _T("WSAECONNREFUSED:Connection refused, no server");
      break;
   case WSAECONNRESET:				
      cs= _T("WSAECONNRESET:Connection was reset by the remote side");
      break;
   case WSAEDESTADDRREQ:	
      cs= _T("WSAEDESTADDRREQ:A destination address is required");
      break;
   case WSAEFAULT:
      cs=  _T("WSAEFAULT:The namelen argument is incorrect");
      break;
   case WSAEINPROGRESS:
      cs= _T("WSAEINPROGRESS:A blocking Windows Sockets call is in progress");
      break;
   case WSAEINTR:
      cs= _T("WSAEINTR:The blocking call was canceled using WSACancelBlockingCall");
      break;
   case WSAENETDOWN:
      cs=  _T("WSAENETDOWN:The Windows Sockets implementation has detected that the network subsystem has failed");
      break;
   case WSAEINVAL:
      cs= _T("WSAEINVAL:The socket is already bound to an address");
      break;
   case WSAEISCONN:
      cs= _T("WSAEISCONN:The socket is already connected");
      break;
   case WSAEMFILE:
      cs= _T("WSAEMFILE:No more file descriptors are available");
      break;
   case WSAEMSGSIZE:	
      cs= _T("WSAEMSGSIZE:The datagram was too large to fit into the specified buffer and was truncated");
      break;
   case WSAENETUNREACH:
      cs=  _T("SAENETUNREACH:Network unreachable");
      break;
   case WSAENOBUFS:
      cs=  _T("WSAENOBUFS:No buffer space is available. The socket cannot be connected");
      break;
   case WSAENOTCONN:	
      cs=  _T("WSAENOTCONN:The socket is not connected");
      break;
   case WSAENOTSOCK:	
      cs=  _T("WSAENOTSOCK:The descriptor is not a socket");
      break;
   case WSAEOPNOTSUPP:	
      cs=  _T("WSAEOPNOTSUPP:MSG_OOB was specified, but the socket is not of type SOCK_STREAM");
      break;
   case WSAEPROTOTYPE:	
      cs=  _T("WSAEPROTOTYPE:The specified protocol is the wrong type for this socket");
      break;
   case WSAEPROTONOSUPPORT:	
      cs= _T("WSAEPROTONOSUPPORT:The specified protocol is not supported");
      break;
   case WSAESHUTDOWN:	
      cs= _T("WSAESHUTDOWN:The socket has been shut down; it is not possible to recv on a socket after shutdown has been invoked with how set to 0 or 2");
      break;
   case WSAESOCKTNOSUPPORT:	
      cs= _T("WSAESOCKTNOSUPPORT:The specified socket type is not supported in this address family");
      break;
   case WSAETIMEDOUT:
      cs=  _T("WSAETIMEDOUT:Connection timed out");
      break;
   case WSAEWOULDBLOCK:
      cs= _T("WSAEWOULDBLOCK:The socket is marked as nonblocking and the connection cannot be completed immediately. It is possible to select the socket while it is connecting by selecting it for writing");
      break;
   case WSANOTINITIALISED:
      cs=  _T("WSANOTINITIALISED:A successful WSAStartup must occur before using this function");
      break;
   default:
      cs.Format(_T("Unknown error returned from WSAGetLastError():%d"), WSAGetLastError());
   }		  
   if (pts)
      pts->Trace(TM_ERROR, (LPCTSTR)cs);
}
