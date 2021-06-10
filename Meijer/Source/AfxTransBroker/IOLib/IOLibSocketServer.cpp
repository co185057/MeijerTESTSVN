
/////////////////////////////////////////////////////////////////////
//
// IOLibDeviceManager
// --------------------
// Devices use I/O completion ports for communication, making the architecture
// scalable to a large amount of devices. The CIOLib, CIOLibDeviceMgr, 
// CIOLibDevice and IOLibxxxDevice classes are generic and can be reused in
// other projects as deemed necessary. These classes use STL and Win32 APIs exclusively.
// NO MFC OBJECTS PLEASE!
//
// Author:      Steve Antonakakis
//
// History:     04/28/2004          - Initial beta release.
//
#include "stdafx.h"
#include "IOLib.h"
#include "tracesystem.h"
#define T_ID _T("SocketServer")

DWORD WINAPI SocketListenThreadProc(LPVOID pvParam)
{
    ((CIOLibSocketServer *)pvParam)->ListenSocket();

    return 0;
}


CIOLibSocketServer::CIOLibSocketServer()
    : CIOLibDevice(CIOLib::dtSocketServer), m_sListenSocket(INVALID_SOCKET), 
      m_sClientSocket(INVALID_SOCKET), m_hListenSignal(NULL), m_hListenThread(NULL),
      m_hReadSignal(NULL), m_hWriteSignal(NULL), m_pBuffer(NULL), m_bListening(FALSE)
{
    ZeroMemory(&m_olRead, sizeof(m_olRead));
    ZeroMemory(&m_olWrite, sizeof(m_olWrite));
    m_hReadSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hWriteSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_olRead.hEvent = m_hReadSignal;
    m_olWrite.hEvent = m_hWriteSignal;

    m_pBuffer = new BYTE[CIOLib::SocketBlockSize];

    WORD wVersionRequested = MAKEWORD( 2, 2 );
    WSADATA wsaData = {0};
 
    int iRet = WSAStartup( wVersionRequested, &wsaData );
    _ASSERT(!iRet);
}

CIOLibSocketServer::~CIOLibSocketServer(void)
{
    Uninitialize();
}

void CIOLibSocketServer::Uninitialize()
{
    if( m_sListenSocket != INVALID_SOCKET )
    {
        closesocket(m_sListenSocket);
        Sleep(1000);
        m_sListenSocket = INVALID_SOCKET;
    }

    if( m_sClientSocket != INVALID_SOCKET )
    {
        closesocket(m_sClientSocket);
        m_sClientSocket = INVALID_SOCKET;
    }

    if( m_hListenSignal )
    {
        CloseHandle(m_hListenSignal);
        m_hListenSignal = NULL;
    }

    if( m_hListenThread )
    {
        CloseHandle(m_hListenThread);
        m_hListenThread = NULL;
    }

    if( m_hReadSignal )
    {
        CloseHandle(m_hReadSignal);
        m_hReadSignal = NULL;
    }

    if( m_hWriteSignal )
    {
        CloseHandle(m_hWriteSignal);
        m_hWriteSignal = NULL;
    }

    if( m_pBuffer )
    {
        delete [] m_pBuffer;
        m_pBuffer = NULL;
    }

    WSACleanup();
}



BOOL CIOLibSocketServer::WriteData(PVOID pvData, DWORD dwSize)
{
    if( m_bReinitializing ) 
	{
		return S_FALSE;
	}

    _ASSERT(m_sClientSocket != INVALID_SOCKET);
    _ASSERT(_CrtIsValidHeapPointer(pvData));
    _ASSERT(!m_bListening); // We should never get in here if there is no socket connection established by a client
    
    ResetIdlePeriod();
    m_olWrite.Offset = CIOLib::CMD_IO_WRITE;
    DWORD dwWritten = 0;
    BOOL bRet = WriteFile((HANDLE)m_sClientSocket, pvData, dwSize, &dwWritten, &m_olWrite);
    DWORD dwError = GetLastError();
    if( !(bRet | (ERROR_IO_PENDING == dwError)) )
    {
        m_pGenDevMgr->Trace(CIOLib::ttError, _T("WriteData failed for device %d, last error: %d"), m_lDeviceID, dwError);
        _ASSERT(FALSE);
		return S_FALSE;
    } 
	return S_OK;
}

void CIOLibSocketServer::ReadData(void)
{
    if( m_bReinitializing ) return;
    
    ResetIdlePeriod();
    DWORD dwRead = 0;
    ZeroMemory(m_pBuffer, CIOLib::SocketBlockSize);
    m_olRead.Offset = CIOLib::CMD_IO_READ;
    
    BOOL bRet = ReadFile((HANDLE)m_sClientSocket, m_pBuffer, CIOLib::SocketBlockSize, &dwRead, &m_olRead);
    DWORD dwError = GetLastError();

    if( !bRet )
    {
        // pending error is expected, anything else is a problem
        if( !(ERROR_IO_PENDING == dwError) )
        {
            m_pGenDevMgr->Trace(CIOLib::ttError, _T("ReadData failed for device %d, last error: %d"), m_lDeviceID, dwError);
            _ASSERT(FALSE);
        }
    }
    else // received data synchronously!
    {
         m_pGenDevMgr->Trace(CIOLib::ttWarning, _T("**ReadFile SocketServer completed SYNCHRONOUSLY. Still Expecting OnReadComplete"));
         if (ptbto)
			 ptbto->HexTrace(TM_WARNING,_T("SYNCHRONOUS READ DATA"), m_pBuffer, dwRead);

        // Data was placed in the buffer before the completion port had a chance to be set up
        // It seems that the completion port will not complete unless it's jump started by another command so 
        // let's post a dummy command to get going.
        //BOOL bRet = PostQueuedCompletionStatus(m_pGenDevMgr->m_hCompletionPort, 0, CIOLib::CMD_BOGUS, (LPOVERLAPPED)this);
        //_ASSERT(bRet);
    }

}

void CIOLibSocketServer::AcceptConnection(void)
{
    m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("Accepting socket connection for device %d."), m_lDeviceID);
    
    int iRet = 0;
    struct sockaddr sa = {0};
    int iSize = sizeof(sa);
    _ASSERT(m_sClientSocket == INVALID_SOCKET);
    m_sClientSocket = accept(m_sListenSocket, &sa, &iSize); 
    _ASSERT(m_sClientSocket != INVALID_SOCKET);
    
    //
    // Add device to be notified on mgr completion port
    //
    HANDLE hCompletionPort = CreateIoCompletionPort((HANDLE)m_sClientSocket, m_pGenDevMgr->m_hCompletionPort, m_lDeviceID, 0);
    _ASSERT(hCompletionPort != NULL);

    //
    // Perform the first read request
    //
    ReadData();

    m_pGenDevMgr->OnConnected(*this);
}

long CIOLibSocketServer::GetNetworkEvents()
{
    WSANETWORKEVENTS wne = {0};
    int iRet = WSAEnumNetworkEvents(m_sListenSocket, m_hListenSignal, &wne);
    _ASSERT(!iRet);
    return wne.lNetworkEvents;
}

void CIOLibSocketServer::ListenSocket(void)
{
    m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("Listening on socket %d for device %d."), m_lPort, m_lDeviceID);
    
    m_bListening = TRUE;
    int iRet = 0;

    // Create the socket to begin listening
    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons((unsigned short)m_lPort);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
  
    _ASSERT(INVALID_SOCKET == m_sListenSocket);
    m_sListenSocket = WSASocket(AF_INET,SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
    _ASSERT(INVALID_SOCKET != m_sListenSocket);

    if( !bind(m_sListenSocket, (SOCKADDR*)&sa, sizeof(sa)) )
    {
        fd_set fs;
        TIMEVAL tvForever = {20,0};
        FD_ZERO(&fs);
        FD_SET(m_sListenSocket, &fs);
        iRet = listen(m_sListenSocket, 64);
        _ASSERT(!iRet);
    }

    m_hListenSignal = WSACreateEvent();
    _ASSERT(m_hListenSignal);
    iRet = WSAEventSelect(m_sListenSocket, m_hListenSignal, FD_ACCEPT);
    _ASSERT(!iRet);
    
    while( m_bListening )
    {
        DWORD dwRet = WSAWaitForMultipleEvents(1, &m_hListenSignal, TRUE, CIOLib::SocketServerWatchdogPeriod, FALSE);
        if( WAIT_OBJECT_0 == dwRet )
        {
            long lNetworkEvents = GetNetworkEvents();
            if( lNetworkEvents & FD_ACCEPT )
            {
                iRet = listen(m_sListenSocket, 64);
                _ASSERT(!iRet);
                BOOL bRet = PostQueuedCompletionStatus(m_pGenDevMgr->m_hCompletionPort, 0, CIOLib::CMD_SOCKET_ACCEPT, (LPOVERLAPPED)this);
                _ASSERT(bRet);
                m_bListening = FALSE;

            }
            else if( !lNetworkEvents | (lNetworkEvents & (FD_READ | FD_WRITE | FD_CONNECT)) )
            {
                // ignore these
            }
            else
            {
                _ASSERT(FALSE);
            }
        }
        else if( WSA_WAIT_FAILED == dwRet )
        {
            // no longer listening
            m_bListening = FALSE;
        }
    }
}

BOOL CIOLibSocketServer::Initialize(long lDeviceID, long lPort, CIOLibDeviceMgr & genDevMgr, LPCTSTR szSetting)
{
    ResetIdlePeriod();
    m_lDeviceID = lDeviceID;
    m_lPort = lPort;
    m_pGenDevMgr = &genDevMgr;

    _ASSERT(!m_hListenThread);
    DWORD dwThreadID = 0;
    m_hListenThread = CreateThread(NULL, 0, SocketListenThreadProc, this, 0, &dwThreadID);
    _ASSERT(m_hListenThread);

    return TRUE;
}


    
BOOL CIOLibSocketServer::ReinitializeInternal()
{
    m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("Reinitializing socket device %d."), m_lDeviceID);

    Uninitialize();

    ZeroMemory(&m_olRead, sizeof(m_olRead));
    ZeroMemory(&m_olWrite, sizeof(m_olWrite));
    m_hReadSignal = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hWriteSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_olRead.hEvent = m_hReadSignal;
    m_olWrite.hEvent = m_hWriteSignal;

    m_pBuffer = new BYTE[CIOLib::SocketBlockSize];

    WORD wVersionRequested = MAKEWORD( 2, 2 );
    WSADATA wsaData = {0};
 
    int iRet = WSAStartup( wVersionRequested, &wsaData );
    _ASSERT(!iRet);


    BOOL bRet = Initialize(m_lDeviceID, m_lPort, *m_pGenDevMgr);
    m_bReinitializing = FALSE;
    return bRet;
}

void CIOLibSocketServer::IdlePeriodHousekeeping()
{
    if( !m_bListening )
    {
        // We have a connection open to a client and the idle 
        // time has expired. Go ahead and recycle the connection
        //VERIFY(Reinitialize());
        ResetIdlePeriod();
    }
    else
    {
        // Reset the idle timer
        ResetIdlePeriod();
    }
}