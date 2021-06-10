
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
#define T_ID _T("SocketClient")

DWORD WINAPI SocketConnectThreadProc(LPVOID pvParam)
{
    ((CIOLibSocketClient *)pvParam)->ConnectSocket();

	return 0;
}

CIOLibSocketClient::CIOLibSocketClient(void)
    : CIOLibDevice(CIOLib::dtSocketClient), m_sClientSocket(INVALID_SOCKET), 
      m_pBuffer(NULL), m_bConnecting(FALSE),
      m_hConnectThread(NULL)
{

    ZeroMemory(&m_olRead, sizeof(m_olRead));
    ZeroMemory(&m_olWrite, sizeof(m_olWrite));
    m_olRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_olWrite[0].hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
    m_olWrite[1].hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
    m_olWrite[2].hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

    m_pBuffer = new BYTE[CIOLib::SocketBlockSize];

    WORD wVersionRequested = MAKEWORD( 2, 2 );
    WSADATA wsaData = {0};
 
    int iRet = WSAStartup( wVersionRequested, &wsaData );
    _ASSERT(!iRet);
}

void CIOLibSocketClient::Uninitialize()
{
    m_bConnecting = FALSE;

    if( m_hConnectThread )
    {
        CloseHandle(m_hConnectThread);
        m_hConnectThread = NULL;
    }

    if( m_sClientSocket != INVALID_SOCKET )
    {
        closesocket(m_sClientSocket);
        Sleep(1000);
        m_sClientSocket = INVALID_SOCKET;
    }

    if( m_olRead.hEvent )
    {
        CloseHandle(m_olRead.hEvent);
        m_olRead.hEvent = NULL;
    }

    if( m_olWrite[0].hEvent )
    {
        CloseHandle(m_olWrite[0].hEvent);
        m_olWrite[0].hEvent = NULL;
    }
    if( m_olWrite[1].hEvent )
    {
        CloseHandle(m_olWrite[1].hEvent);
        m_olWrite[1].hEvent = NULL;
    }
    if( m_olWrite[2].hEvent )
    {
        CloseHandle(m_olWrite[2].hEvent);
        m_olWrite[2].hEvent = NULL;
    }

    if( m_pBuffer )
    {
        delete [] m_pBuffer;
        m_pBuffer = NULL;
    }

    WSACleanup();
}

CIOLibSocketClient::~CIOLibSocketClient(void)
{
    Uninitialize();
}

BOOL CIOLibSocketClient::WriteData(PVOID pvData, DWORD dwSize)
{
    if( m_bReinitializing ) 
	{
		m_dLastError = 20999;
		return S_FALSE;
	}

    //_ASSERT(m_sClientSocket != INVALID_SOCKET);
    _ASSERT(_CrtIsValidHeapPointer(pvData));
    _ASSERT(!m_bConnecting); // We should never get in here if there is no socket connection established	

	// issue ::select command to find out if the Write socket is available and ready to receive data 
	// only if the write socket is ready then the data will be sent - RRS
//	bool bSocketIsExist = true;
//	struct timeval timeout = {2,0};
//	fd_set status;
//	FD_ZERO(&status);
//	FD_SET( m_sClientSocket, &status);
//	int iRet = ::select(0, NULL, &status, NULL, &timeout);

//	if( iRet == 0 )
//	{
//		bSocketIsExist = false;
//		m_pGenDevMgr->Trace(CIOLib::ttError, _T("Socket not writable!")) ;
//	}	

	ResetIdlePeriod();

	BOOL bRet=false;
	DWORD dwWritten = 0;
	m_olWrite[m_nPendingWrites].Offset = CIOLib::CMD_IO_WRITE;    
	bRet = WriteFile((HANDLE)m_sClientSocket, pvData, dwSize, &dwWritten, &m_olWrite[m_nPendingWrites]);

	DWORD dwError = GetLastError();
	m_dLastError= dwError;
    if( !bRet)
	{
		if (ERROR_IO_PENDING == dwError)
		{
			m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("WriteFile ASYNC Queued for device %d"), m_lDeviceID);
		}
		else
		{
			m_pGenDevMgr->Trace(CIOLib::ttError, _T("WriteData failed for device %d, last error:%d"), m_lDeviceID, dwError);

			// the connection is broken, try to re-initialize the connections
			long lLoop=0;		
		
			while( !bRet && lLoop < 3 )
			{
			    lLoop++;

				Sleep(1000);
				ResetIdlePeriod();
				m_olWrite[m_nPendingWrites].Offset = CIOLib::CMD_IO_WRITE;
				DWORD dwWritten = 0;

				// try again to find out if the write socket is ready - RRS
				//FD_ZERO(&status);
				//FD_SET( m_sClientSocket, &status);
				//iRet = ::select(0, NULL, &status, NULL, &timeout);

				m_pGenDevMgr->Trace(CIOLib::ttWarning, _T("Resending Serial data device:%d"), m_lDeviceID);

				bRet = WriteFile((HANDLE)m_sClientSocket, pvData, dwSize, &dwWritten, &m_olWrite[m_nPendingWrites]);
				dwError = GetLastError();
				m_dLastError = dwError;

				if( !bRet)
				{
					if (ERROR_IO_PENDING == dwError)
					{
						m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("WriteFile ASYNC Queued for device %d"), m_lDeviceID);
					}
					else
					{
						m_pGenDevMgr->Trace(CIOLib::ttError, _T("WriteData2 failed for device %d, last error:%d"), m_lDeviceID, dwError);
						SetEvent(m_olWrite[m_nPendingWrites].hEvent);
					}
				}
				else
				{
					m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("***WriteFile2 SYNC Success, device:%d, Bytes Original: %d, Bytes Written: %d"), m_lDeviceID, dwSize, dwWritten);
					m_pGenDevMgr->Trace(CIOLib::ttWarning, _T("Signaling Complete Event Manually to allow next write"));
					SetEvent(m_olWrite[m_nPendingWrites].hEvent);
				}
			}

			if( ERROR_NETNAME_DELETED == dwError || ERROR_CONNECTION_ABORTED == dwError || 
				ERROR_SEM_TIMEOUT == dwError || ERROR_NETWORK_UNREACHABLE == dwError)
			{
				bRet = Reinitialize();
//				iRet = 0;
//				if( bRet )
//				{
//					// try again to find out if the write socket is ready - RRS
//					FD_ZERO(&status);
//					FD_SET( m_sClientSocket, &status);
//					iRet = ::select(0, NULL, &status, NULL, &timeout);
//				}
//				m_pGenDevMgr->Trace(CIOLib::ttError, _T("Socket Error, Device ID : %d, Socket Ready : %d, Reinitialize : %d"), m_lDeviceID, iRet, bRet );

				m_pGenDevMgr->Trace(CIOLib::ttError, _T("Socket Error, Device ID : %d, Reinitialize : %d"), m_lDeviceID, bRet );
			}
			return S_FALSE;
		}
    } 	
	else
	{
		m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("***WriteFile SYNC Success, device:%d, Bytes Original: %d, Bytes Written: %d"), m_lDeviceID, dwSize, dwWritten);
	}

	return S_OK;
}
#include <winerror.h>
void CIOLibSocketClient::ReadData(void)
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
        if(ERROR_IO_PENDING != dwError) 
        {
            m_pGenDevMgr->Trace(CIOLib::ttError, _T("ReadData failed for device %d, last error: %d"), m_lDeviceID, dwError);
			//MessageBox(NULL, _T("ReadData failed for device"), _T("debug"), MB_OK);
            _ASSERT(FALSE);
        }
    }
    else // received data synchronously!
    {
         m_pGenDevMgr->Trace(CIOLib::ttWarning, _T("**ReadFile SocketClient completed SYNCHRONOUSLY. Still Expecting OnReadComplete"));
         if (ptbto)
			 ptbto->HexTrace(TM_WARNING,_T("SYNCHRONOUS READ DATA"), m_pBuffer, dwRead);

        // Data was placed in the buffer before the completion port had a chance to be set up
        // It seems that the completion port will not complete unless it's jump started by another command so 
        // let's post a dummy command to get going.
        //BOOL bRet = PostQueuedCompletionStatus(m_pGenDevMgr->m_hCompletionPort, 0, CIOLib::CMD_BOGUS, (LPOVERLAPPED)this);
        //_ASSERT(bRet);
		// MessageBox(NULL, _T("ReadData got into questionable code"), _T("debug"), MB_OK);
    }
}

void CIOLibSocketClient::ConnectSocket(void)
{
    m_bConnecting = TRUE;
    
    //
    // Create the client socket and attempt a connection
    //
    m_sClientSocket = ::WSASocket(AF_INET,SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
    _ASSERT(INVALID_SOCKET != m_sClientSocket);
    
    //
    // Create the socket binding information
    //
    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = ::htons((unsigned short)m_lPort);

    //
    // Get the host connection address - either by name or IP
    //
    CT2A ansiSetting(m_szSetting);
    sa.sin_addr.s_addr = ::inet_addr(ansiSetting);
    if(sa.sin_addr.s_addr == INADDR_NONE)
    {
        LPHOSTENT pHost = ::gethostbyname(ansiSetting);
        if( pHost )
        {
            sa.sin_addr.s_addr = ((LPIN_ADDR)pHost->h_addr)->s_addr;
        }
        else
        {
            _ASSERT(FALSE);
        }
    }

    int iRet = 0;
    do
    {
        iRet = ::WSAConnect(m_sClientSocket, (const sockaddr*)&sa, sizeof(sockaddr_in), NULL, NULL, NULL, NULL);
        if( SOCKET_ERROR == iRet )
        {
            long lError = WSAGetLastError();
            m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("Attempting to connect client socket to host: %s, port: %d... (Err#:%d)"), m_szSetting, m_lPort, lError);
            Sleep(CIOLib::SocketClientConnectPeriod);
        }
    }
    while( (SOCKET_ERROR == iRet) && m_bConnecting ); // connecting flag can be set to false by destructor
    
    if( m_bConnecting )
    {
        //
        // Add device to be notified on mgr completion port
        //
        HANDLE hCompletionPort = ::CreateIoCompletionPort((HANDLE)m_sClientSocket, m_pGenDevMgr->m_hCompletionPort, m_lDeviceID, 0);
        _ASSERT(hCompletionPort != NULL);

        m_bConnecting = FALSE;

		int iOptVal;
		int iOptLen = sizeof(int);
		iOptVal = 0;


		if (setsockopt(m_sClientSocket, SOL_SOCKET, SO_SNDBUF, (char*)&iOptVal, iOptLen) != SOCKET_ERROR) 
		{
			// MessageBox(NULL, _T("setsockopt ok"), _T("debug"), MB_OK);
		}
		//
        // Go ahead and perform the first read
        //

        ReadData();

        m_pGenDevMgr->OnConnected(*this);

		m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("Socket client connection established to host: %s, port: %d, SEND_BUFFER: %d."), m_szSetting, m_lPort, iOptVal);		

    }
}


BOOL CIOLibSocketClient::Initialize(long lDeviceID, long lPort, CIOLibDeviceMgr & genDevMgr, LPCTSTR szSetting)
{
    ResetIdlePeriod();
    m_lDeviceID = lDeviceID;
    m_lPort = lPort;
    m_pGenDevMgr = &genDevMgr;

    if( szSetting )
    {
        // if the target is empty or contains something different from the parameter passed
        // make a copy
        if( !m_szSetting || (m_szSetting && _tcscmp(m_szSetting, szSetting)) )
        {
            delete [] m_szSetting;
            m_szSetting = new TCHAR[_tcslen(szSetting) + 1];
            _tcscpy(m_szSetting, szSetting);
        }
    }
    else if( m_szSetting )
    {
        // the parameter is null, clear the current parameter if any exist
        delete [] m_szSetting;
        m_szSetting = NULL;
    }

    _ASSERT(!m_hConnectThread);
    DWORD dwThreadID = 0;
    m_hConnectThread = CreateThread(NULL, 0, SocketConnectThreadProc, this, 0, &dwThreadID);
    _ASSERT(m_hConnectThread);

	return TRUE;
}


BOOL CIOLibSocketClient::ReinitializeInternal()
{
	Uninitialize();

    //
    // Reinitialize
    //
    ZeroMemory(&m_olRead, sizeof(m_olRead));
    ZeroMemory(&m_olWrite, sizeof(m_olWrite));
    m_olRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_olWrite[0].hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
    m_olWrite[1].hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
    m_olWrite[2].hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

    m_pBuffer = new BYTE[CIOLib::SocketBlockSize];

    WORD wVersionRequested = MAKEWORD( 2, 2 );
    WSADATA wsaData = {0};
 
    int iRet = WSAStartup( wVersionRequested, &wsaData );
    _ASSERT(!iRet);

    BOOL bRet = Initialize(m_lDeviceID, m_lPort, *m_pGenDevMgr, m_szSetting);
    m_bReinitializing = FALSE;
	return bRet;	    
}

void CIOLibSocketClient::IdlePeriodHousekeeping()
{
    // nothing to do here!  Socket client devices will not be recycled.
    ResetIdlePeriod();

}

DWORD CIOLibSocketClient::GetError()
{
	return m_dLastError;
}

void CIOLibSocketClient::CancelWrite()
{
	CancelIo((HANDLE)m_sClientSocket);
}