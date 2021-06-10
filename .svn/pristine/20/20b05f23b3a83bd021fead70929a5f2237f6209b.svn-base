//
// Copyright (c) 1997-1998 Mabry Software, Inc.
//

#ifndef _CMSOCKET_H__
#define _CMSOCKET_H__

#ifndef _WINSOCKAPI_
	#include <winsock.h>
#endif


#define WM_SOCKET_NOTIFY		0x0373
#define WM_SOCKET_SSL_NOTIFY    0x0374


		
		
class CMSocket
{
// Construction
public:
	CMSocket(LPTSTR libname, WSADATA* pWsaData);
	BOOL Create(UINT nSocketPort = 0, int nSocketType=SOCK_STREAM,
		long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
		LPTSTR lpszSocketAddress = NULL);
	static LRESULT CALLBACK SocketWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void CreateSocketWindow();

#ifdef _BUILD_VBX
#define SetLastError(n) WSASetLastError(n)
#endif

	void _StartTimer(int tval, int& counter);
	void _StopTimer(int& counter);

// Attributes
public:
	SOCKET	m_hSocket;
	HWND	m_hwndSocket;
	BOOL	m_fBlocking;
	BOOL	m_fIsBlocking;
	BOOL	m_fCancelBlockingCall;
	int		m_nConnectError;
	char	m_Libname[256];

	int		m_TimerID;

	int		m_AcceptTimeout;
	int		m_AcceptTimeoutCounter;

	int		m_ConnectTimeout;
	int		m_ConnectTimeoutCounter;

	int		m_SendTimeout;
	int		m_SendTimeoutCounter;

	int		m_ReceiveTimeout;
	int		m_ReceiveTimeoutCounter;

	operator SOCKET() const;

	BOOL GetPeerName(CString& rPeerAddress, UINT& rPeerPort);
	BOOL GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);

	BOOL getsockName(CString& rSocketAddress, UINT& rSocketPort);
	BOOL getsockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen);
	BOOL GetHostName(CString& rHostName, CString& rHostIP);

	BOOL SetSockOpt(int nOptionName, const void* lpOptionValue,
		int nOptionLen, int nLevel = SOL_SOCKET);
	BOOL getsockOpt(int nOptionName, void* lpOptionValue,
		int* lpOptionLen, int nLevel = SOL_SOCKET);
	BOOL IsBlocking();

	int PASCAL GetLastError();

// Operations
public:

	virtual BOOL Accept(CMSocket& rConnectedSocket,	SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);

	BOOL Bind(UINT nSocketPort, LPTSTR lpszSocketAddress = NULL);

	void CancelBlockingCall() { m_fCancelBlockingCall = TRUE; };

	virtual BOOL Close();

	BOOL Connect(LPTSTR lpszHostAddress, UINT nHostPort);
	BOOL Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);

	BOOL IOCtl(long lCommand, DWORD* lpArgument);

	BOOL Listen(int nConnectionBacklog=5);

	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);

	int ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);
	int ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags = 0);

	enum { receives = 0, sends = 1, both = 2 };
	BOOL ShutDown(int nHow = sends);

	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);

	int SendTo(const void* lpBuf, int nBufLen,
		UINT nHostPort, LPCTSTR lpszHostAddress = NULL, int nFlags = 0);

	BOOL AsyncSelect(long lEvent =
		FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);

	enum { TrueBlocking = 0, PseudoBlocking = 1 };
	short m_BlockingMode;

// Overridable callbacks
protected:
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnTimer(void);

// Implementation
public:
	virtual ~CMSocket();

	BOOL Socket(int nSocketType=SOCK_STREAM, long lEvent =
		FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
		int nProtocolType = 0, int nAddressFormat = PF_INET);
	int Block( int events );
	int BlockingSend(const void* lpBuf, int nBufLen, int nFlags);
	int BlockingSendTo(SOCKET m_hSocket, LPTSTR lpBuf, int nBufLen, int nFlags, SOCKADDR* sockAddr, int nSockAddr);
public:
	static SOCKET (PASCAL FAR * pfn_accept)(SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen);
	static int (PASCAL FAR * pfn_bind)(SOCKET s, const struct sockaddr FAR *addr, int namelen);
	static int (PASCAL FAR * pfn_closesocket)(SOCKET s);
	static int (PASCAL FAR * pfn_connect)(SOCKET s, const struct sockaddr FAR *name, int namelen);
	static struct hostent FAR * (PASCAL FAR * pfn_gethostbyaddr)(const _TCHAR FAR * addr, int len, int type) ;
	static struct hostent FAR * (PASCAL FAR * pfn_gethostbyname)(const _TCHAR FAR * name) ;
	static int (PASCAL FAR * pfn_gethostname)(const _TCHAR FAR * name, int namelen);
	static int (PASCAL FAR * pfn_getpeername)(SOCKET s, struct sockaddr FAR *name, int FAR * namelen) ;
	static int (PASCAL FAR * pfn_getsockname)(SOCKET s, struct sockaddr FAR *name, int FAR * namelen) ;
	static int (PASCAL FAR * pfn_getsockopt)(SOCKET s, int level, int optname, _TCHAR FAR * optval, int FAR *optlen) ;
	static u_long (PASCAL FAR * pfn_htonl)(u_long hostlong) ;
	static u_short (PASCAL FAR * pfn_htons)(u_short hostshort) ;
	static unsigned long (PASCAL FAR * pfn_inet_addr)(const _TCHAR FAR * cp) ;
	static _TCHAR FAR * (PASCAL FAR * pfn_inet_ntoa)(struct in_addr in) ;
	static int (PASCAL FAR * pfn_ioctlsocket)(SOCKET s, long cmd, u_long FAR *argp) ;
	static int (PASCAL FAR * pfn_listen)(SOCKET s, int backlog) ;
	static u_short (PASCAL FAR * pfn_ntohs)(u_short netshort) ;
	static int (PASCAL FAR * pfn_recv)(SOCKET s, _TCHAR FAR * buf, int len, int flags) ;
	static int (PASCAL FAR * pfn_recvfrom)(SOCKET s, _TCHAR FAR * buf, int len, int flags, struct sockaddr FAR *from, int FAR * fromlen) ;
	static int (PASCAL FAR * pfn_send)(SOCKET s, const _TCHAR FAR * buf, int len, int flags) ;
	static int (PASCAL FAR * pfn_sendto)(SOCKET s, const _TCHAR FAR * buf, int len, int flags, const struct sockaddr FAR *to, int tolen) ;
	static int (PASCAL FAR * pfn_setsockopt)(SOCKET s, int level, int optname, const _TCHAR FAR * optval, int optlen) ;
	static int (PASCAL FAR * pfn_shutdown)(SOCKET s, int how) ;
	static SOCKET (PASCAL FAR * pfn_socket)(int af, int type, int protocol) ;
	static int (PASCAL FAR * pfn_WSAStartup)(WORD wVersionRequired, LPWSADATA lpWSAData) ;
	static int (PASCAL FAR * pfn_WSACleanup)(void) ;
	static int (PASCAL FAR * pfn_WSAGetLastError)(void);
	static BOOL (PASCAL FAR * pfn_WSAIsBlocking)(void) ;
	static int (PASCAL FAR * pfn_WSAAsyncSelect)(SOCKET s, HWND hWnd, u_int wMsg, long lEvent) ;
	static int (PASCAL FAR * pfn_WSACancelBlockingCall)(void);
	static int (PASCAL FAR * pfn_WSAUnhookBlockingHook)(void);
	static void (PASCAL FAR * pfn_WSASetLastError)(int iError) ;
	static void (PASCAL FAR * pfn_WSASetBlockingHook)(FARPROC hookfunc);

	static HINSTANCE hInstWinsock;
	static long winsockUsage;
public:

	BOOL WinsockLoaded();
	LPTSTR Libname() { return m_Libname; };

	SOCKET PASCAL FAR accept(SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen);
	int PASCAL FAR bind (SOCKET s, const struct sockaddr FAR *addr, int namelen);
	int PASCAL FAR closesocket (SOCKET s);
	int PASCAL FAR connect (SOCKET s, const struct sockaddr FAR *name, int namelen);
	struct hostent FAR * PASCAL FAR gethostbyaddr(const _TCHAR FAR * addr, int len, int type);
	struct hostent FAR * PASCAL FAR gethostbyname(const _TCHAR FAR * name);
	int PASCAL FAR gethostname (const _TCHAR FAR *name, int namelen);
	int PASCAL FAR getpeername (SOCKET s, struct sockaddr FAR *name, int FAR * namelen);
	int PASCAL FAR getsockname (SOCKET s, struct sockaddr FAR *name, int FAR * namelen);
	int PASCAL FAR getsockopt (SOCKET s, int level, int optname, _TCHAR FAR * optval, int FAR *optlen);
	u_long PASCAL FAR htonl (u_long hostlong);
	u_short PASCAL FAR htons (u_short hostshort);
	unsigned long PASCAL FAR inet_addr (const _TCHAR FAR * cp);
	_TCHAR FAR * PASCAL FAR inet_ntoa (struct in_addr in);
	int PASCAL FAR ioctlsocket (SOCKET s, long cmd, u_long FAR *argp);
	int PASCAL FAR listen (SOCKET s, int backlog);
	u_short PASCAL FAR ntohs (u_short netshort);
	int PASCAL FAR recv (SOCKET s, _TCHAR FAR * buf, int len, int flags);
	int PASCAL FAR recvfrom (SOCKET s, _TCHAR FAR * buf, int len, int flags, struct sockaddr FAR *from, int FAR * fromlen);
	int PASCAL FAR send (SOCKET s, const _TCHAR FAR * buf, int len, int flags);
	int PASCAL FAR sendto (SOCKET s, const _TCHAR FAR * buf, int len, int flags, const struct sockaddr FAR *to, int tolen);
	int PASCAL FAR setsockopt (SOCKET s, int level, int optname, const _TCHAR FAR * optval, int optlen);
	int PASCAL FAR shutdown (SOCKET s, int how);
	SOCKET PASCAL FAR socket (int af, int type, int protocol);
	int PASCAL FAR WSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData);
	int PASCAL FAR WSACancelBlockingCall(void);
	int PASCAL FAR WSACleanup(void);
	int PASCAL FAR WSAUnhookBlockingHook(void);
	int PASCAL FAR WSAGetLastError(void);
	BOOL PASCAL FAR WSAIsBlocking(void);
	int PASCAL FAR WSAAsyncSelect(SOCKET s, HWND hWnd, u_int wMsg, long lEvent);
	void PASCAL FAR WSASetLastError(int iError);
private:
	BOOL SocketLibAttach(LPTSTR LPSTRLibName, WSADATA* pWsaData);
	void SocketLibDetach();
	HINSTANCE LoadWinsock(LPTSTR LPSTRLibName);
};

#ifdef _DEFINE_STATICS
SOCKET (PASCAL FAR * CMSocket::pfn_accept)(SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen);
int    (PASCAL FAR * CMSocket::pfn_bind)(SOCKET s, const struct sockaddr FAR *addr, int namelen);
int (PASCAL FAR * CMSocket::pfn_closesocket)(SOCKET s);
int (PASCAL FAR * CMSocket::pfn_connect)(SOCKET s, const struct sockaddr FAR *name, int namelen);
struct hostent FAR * (PASCAL FAR * CMSocket::pfn_gethostbyaddr)(const _TCHAR FAR * addr, int len, int type) ;
struct hostent FAR * (PASCAL FAR * CMSocket::pfn_gethostbyname)(const _TCHAR FAR * name) ;
int (PASCAL FAR * CMSocket::pfn_gethostname)(const _TCHAR FAR *name, int namelen);
int (PASCAL FAR * CMSocket::pfn_getpeername)(SOCKET s, struct sockaddr FAR *name, int FAR * namelen) ;
int (PASCAL FAR * CMSocket::pfn_getsockname)(SOCKET s, struct sockaddr FAR *name, int FAR * namelen) ;
int (PASCAL FAR * CMSocket::pfn_getsockopt)(SOCKET s, int level, int optname, _TCHAR FAR * optval, int FAR *optlen) ;
u_long (PASCAL FAR * CMSocket::pfn_htonl)(u_long hostlong) ;
u_short (PASCAL FAR * CMSocket::pfn_htons)(u_short hostshort) ;
unsigned long (PASCAL FAR * CMSocket::pfn_inet_addr)(const _TCHAR FAR * cp) ;
_TCHAR FAR * (PASCAL FAR * CMSocket::pfn_inet_ntoa)(struct in_addr in) ;
int (PASCAL FAR * CMSocket::pfn_ioctlsocket)(SOCKET s, long cmd, u_long FAR *argp) ;
int (PASCAL FAR * CMSocket::pfn_listen)(SOCKET s, int backlog) ;
u_short (PASCAL FAR * CMSocket::pfn_ntohs)(u_short netshort) ;
int (PASCAL FAR * CMSocket::pfn_recv)(SOCKET s, _TCHAR FAR * buf, int len, int flags) ;
int (PASCAL FAR * CMSocket::pfn_recvfrom)(SOCKET s, _TCHAR FAR * buf, int len, int flags, struct sockaddr FAR *from, int FAR * fromlen) ;
int (PASCAL FAR * CMSocket::pfn_send)(SOCKET s, const _TCHAR FAR * buf, int len, int flags) ;
int (PASCAL FAR * CMSocket::pfn_sendto)(SOCKET s, const _TCHAR FAR * buf, int len, int flags, const struct sockaddr FAR *to, int tolen) ;
int (PASCAL FAR * CMSocket::pfn_setsockopt)(SOCKET s, int level, int optname, const _TCHAR FAR * optval, int optlen) ;
int (PASCAL FAR * CMSocket::pfn_shutdown)(SOCKET s, int how) ;
SOCKET (PASCAL FAR * CMSocket::pfn_socket)(int af, int type, int protocol) ;
int (PASCAL FAR * CMSocket::pfn_WSAStartup)(WORD wVersionRequired, LPWSADATA lpWSAData) ;
int (PASCAL FAR * CMSocket::pfn_WSACleanup)(void) ;
int (PASCAL FAR * CMSocket::pfn_WSAGetLastError)(void);
BOOL (PASCAL FAR * CMSocket::pfn_WSAIsBlocking)(void) ;
int (PASCAL FAR * CMSocket::pfn_WSAAsyncSelect)(SOCKET s, HWND hWnd, u_int wMsg, long lEvent) ;
int (PASCAL FAR * CMSocket::pfn_WSACancelBlockingCall)(void);
int (PASCAL FAR * CMSocket::pfn_WSAUnhookBlockingHook)(void);
void (PASCAL FAR * CMSocket::pfn_WSASetLastError)(int iError) ;
void (PASCAL FAR * CMSocket::pfn_WSASetBlockingHook)(FARPROC hookfunc);

HINSTANCE CMSocket::hInstWinsock = NULL;
long CMSocket::winsockUsage = 0;
#endif

inline CMSocket::operator SOCKET() const
{
	return m_hSocket;
}
inline BOOL CMSocket::GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{
	return (SOCKET_ERROR != getpeername(m_hSocket, lpSockAddr, lpSockAddrLen));
}
inline BOOL CMSocket::getsockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{


	return (SOCKET_ERROR != getsockname(m_hSocket, lpSockAddr, lpSockAddrLen));
}
inline BOOL CMSocket::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel)
{
	return (SOCKET_ERROR != setsockopt(m_hSocket, nLevel, nOptionName, (LPCTSTR)lpOptionValue, nOptionLen));
}
inline BOOL CMSocket::getsockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel)
{
	return (SOCKET_ERROR != getsockopt(m_hSocket, nLevel, nOptionName, (LPTSTR)lpOptionValue, lpOptionLen));
}

inline int PASCAL CMSocket::GetLastError()
{
	return WSAGetLastError();
}

inline BOOL CMSocket::ShutDown(int nHow)
{
	return (SOCKET_ERROR != shutdown(m_hSocket,nHow));
}

//
// sockstub
//

inline BOOL CMSocket::WinsockLoaded()
{
	return hInstWinsock != NULL;
}

inline SOCKET PASCAL FAR CMSocket::accept(SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen)
{
	return CMSocket::pfn_accept(s, addr, addrlen);
}

inline int PASCAL FAR CMSocket::bind (SOCKET s, const struct sockaddr FAR *addr, int namelen)
{
	return pfn_bind(s, addr, namelen);
}

inline int PASCAL FAR CMSocket::closesocket (SOCKET s)
{
	return pfn_closesocket(s);
}

inline int PASCAL FAR CMSocket::connect (SOCKET s, const struct sockaddr FAR *name, int namelen)
{
	return pfn_connect(s, name, namelen);
}

inline struct hostent FAR * PASCAL FAR CMSocket::gethostbyaddr(const _TCHAR FAR * addr, int len, int type)
{
	return pfn_gethostbyaddr(addr, len, type);
}

inline struct hostent FAR * PASCAL FAR CMSocket::gethostbyname(const _TCHAR FAR * name)
{
	return pfn_gethostbyname(name);
}

inline int PASCAL FAR CMSocket::gethostname (const _TCHAR FAR *name, int namelen)
{
	return pfn_gethostname(name, namelen);
}

inline int PASCAL FAR CMSocket::getpeername (SOCKET s, struct sockaddr FAR *name, int FAR * namelen)
{
	return pfn_getpeername(s, name, namelen);
}

inline int PASCAL FAR CMSocket::getsockname (SOCKET s, struct sockaddr FAR *name, int FAR * namelen)
{
	return pfn_getsockname(s, name, namelen);
}

inline int PASCAL FAR CMSocket::getsockopt (SOCKET s, int level, int optname, _TCHAR FAR * optval, int FAR *optlen)
{
	return pfn_getsockopt(s, level, optname, optval, optlen);
}

inline u_long PASCAL FAR CMSocket::htonl (u_long hostlong)
{
	return pfn_htonl(hostlong);
}

inline u_short PASCAL FAR CMSocket::htons (u_short hostshort)
{
	return pfn_htons(hostshort);
}

inline unsigned long PASCAL FAR CMSocket::inet_addr (const _TCHAR FAR * cp)
{
	return pfn_inet_addr(cp);
}

inline _TCHAR FAR * PASCAL FAR CMSocket::inet_ntoa (struct in_addr in)
{
	return pfn_inet_ntoa(in);
}

inline int PASCAL FAR CMSocket::ioctlsocket (SOCKET s, long cmd, u_long FAR *argp)
{
	return pfn_ioctlsocket(s, cmd, argp);
}

inline int PASCAL FAR CMSocket::listen (SOCKET s, int backlog)
{
	return pfn_listen(s, backlog);
}

inline u_short PASCAL FAR CMSocket::ntohs (u_short netshort)
{
	return pfn_ntohs(netshort);
}

inline int PASCAL FAR CMSocket::recv (SOCKET s, _TCHAR FAR * buf, int len, int flags)
{
	return pfn_recv(s, buf, len, flags);
}

inline int PASCAL FAR CMSocket::recvfrom (SOCKET s, _TCHAR FAR * buf, int len, int flags, struct sockaddr FAR *from, int FAR * fromlen)
{
	return pfn_recvfrom(s, buf, len, flags, from, fromlen);
}

inline int PASCAL FAR CMSocket::send (SOCKET s, const _TCHAR FAR * buf, int len, int flags)
{
	return pfn_send( s, buf, len, flags);
}

inline int PASCAL FAR CMSocket::sendto (SOCKET s, const _TCHAR FAR * buf, int len, int flags, const struct sockaddr FAR *to, int tolen)
{
	return pfn_sendto(s, buf, len, flags, to, tolen);
}

inline int PASCAL FAR CMSocket::setsockopt (SOCKET s, int level, int optname, const _TCHAR FAR * optval, int optlen)
{
	return pfn_setsockopt(s, level, optname, optval, optlen);
}

inline int PASCAL FAR CMSocket::shutdown (SOCKET s, int how)
{
	return pfn_shutdown(s, how);
}

inline SOCKET PASCAL FAR CMSocket::socket (int af, int type, int protocol)
{
	return pfn_socket(af, type, protocol);
}

inline int PASCAL FAR CMSocket::WSAStartup(WORD wVersionRequired, LPWSADATA lpWSAData)
{
	return pfn_WSAStartup(wVersionRequired, lpWSAData);
}

inline int PASCAL FAR CMSocket::WSACancelBlockingCall(void)
{
	return pfn_WSACancelBlockingCall();
}

inline int PASCAL FAR CMSocket::WSACleanup(void)
{
	//
	// Good kludge James!
	//
	if( hInstWinsock == NULL )
		return 0;
	return pfn_WSACleanup();
}

inline int PASCAL FAR CMSocket::WSAGetLastError(void)
{
	return CMSocket::pfn_WSAGetLastError();
}


inline int PASCAL FAR CMSocket::WSAUnhookBlockingHook(void)
{
	return pfn_WSAUnhookBlockingHook();
}

inline BOOL PASCAL FAR CMSocket::WSAIsBlocking(void)
{
	return pfn_WSAIsBlocking();
}

inline int PASCAL FAR CMSocket::WSAAsyncSelect(SOCKET s, HWND hWnd, u_int wMsg, long lEvent)
{
	return pfn_WSAAsyncSelect(s, hWnd, wMsg, lEvent);
}

inline void PASCAL FAR CMSocket::WSASetLastError(int iError)
{
	pfn_WSASetLastError(iError);
}


#endif
