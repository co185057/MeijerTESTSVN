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
#define T_ID _T("SerialDevice")

CIOLibSerialDevice::CIOLibSerialDevice()
: CIOLibDevice(CIOLib::dtSerial), m_hSerialPort(NULL), m_pBuffer(NULL)
{
    m_pBuffer = new BYTE[CIOLib::SerialBlockSize];

    ZeroMemory(&m_olRead, sizeof(m_olRead));
    ZeroMemory(&m_olWrite, sizeof(m_olWrite));
    m_olRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_olWrite[0].hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    m_olWrite[1].hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    m_olWrite[2].hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
}

CIOLibSerialDevice::~CIOLibSerialDevice(void)
{
    if( INVALID_HANDLE_VALUE != m_hSerialPort )
    {
        CloseHandle(m_hSerialPort);
        m_hSerialPort = INVALID_HANDLE_VALUE;
    }

    if( m_pBuffer )
    {
        delete [] m_pBuffer;
        m_pBuffer = NULL;
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
}

BOOL CIOLibSerialDevice::WriteData(PVOID pvData, DWORD dwSize)
{
    if( m_bReinitializing ) 
	{
        return S_FALSE;
	}

    _ASSERT(INVALID_HANDLE_VALUE != m_hSerialPort);
    //_ASSERT(_CrtIsValidHeapPointer(pvData));

    ResetIdlePeriod();
    
    m_olWrite[m_nPendingWrites].Offset = CIOLib::CMD_IO_WRITE;
    DWORD dwWritten = 0;
    BOOL bRet = WriteFile(m_hSerialPort, pvData, dwSize, &dwWritten, &m_olWrite[m_nPendingWrites]);
    DWORD dwError = GetLastError();
	m_dLastError = dwError;
    if( !bRet)
	{
		if (ERROR_IO_PENDING == dwError)
		{
			m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("Write Serial ASYNC Queued for device:%d"), m_lDeviceID);
		}
		else
		{
			m_pGenDevMgr->Trace(CIOLib::ttError, _T("Write Serial failed for device %d. last error:%d. trying again"), m_lDeviceID, dwError);
			Sleep(1000);
		    BOOL bRet = WriteFile(m_hSerialPort, pvData, dwSize, &dwWritten, &m_olWrite[m_nPendingWrites]);
			DWORD dwError = GetLastError();
			if( !bRet)
			{
				if (ERROR_IO_PENDING == dwError)
				{
					m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("Write2 Serial ASYNC Queued for device:%d"), m_lDeviceID);
				}
				else
				{
					m_pGenDevMgr->Trace(CIOLib::ttError, _T("Write2 Serial failed for device %d. last error:%d. trying again"), m_lDeviceID, dwError);
					return S_FALSE;
				}
			}
			else
			{
				m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("***Write2 Serial SYNC Success, device:%d, Bytes Original: %d, Bytes Written: %d"), m_lDeviceID, dwSize, dwWritten);
				m_pGenDevMgr->Trace(CIOLib::ttWarning, _T("Signaling Complete Event Manually to allow next write"));
				SetEvent(m_olWrite[m_nPendingWrites].hEvent);
			}
		}
	}
	else
	{
		m_pGenDevMgr->Trace(CIOLib::ttInfo, _T("****Write Serial SYNC Success, device:%d, Bytes Original: %d, Bytes Written: %d"), m_lDeviceID, dwSize, dwWritten);
	}
	return S_OK;
}

void CIOLibSerialDevice::ReadData(void)
{
    if( m_bReinitializing ) return;

    ResetIdlePeriod();

    DWORD dwRead = 0;
    ZeroMemory(m_pBuffer, CIOLib::SerialBlockSize);
    m_olRead.Offset = CIOLib::CMD_IO_READ;
    BOOL bRet = ReadFile((HANDLE)m_hSerialPort, m_pBuffer, CIOLib::SerialBlockSize, &dwRead, &m_olRead);
    
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
         m_pGenDevMgr->Trace(CIOLib::ttWarning, _T("**ReadFile SerialDevice completed SYNCHRONOUSLY. Still Expecting OnReadComplete"));
         if (ptbto)
			 ptbto->HexTrace(TM_WARNING,_T("SYNCHRONOUS READ DATA"), m_pBuffer, dwRead);
        // Data was placed in the buffer before the completion port had a chance to be set up
        // It seems that the completion port will not complete unless it's jump started by another command so 
        // let's post a dummy command to get going.
        //BOOL bRet = PostQueuedCompletionStatus(m_pGenDevMgr->m_hCompletionPort, 0, CIOLib::CMD_BOGUS, (LPOVERLAPPED)this);
        //_ASSERT(bRet);
    }
}

BOOL CIOLibSerialDevice::Initialize(long lDeviceID, long lPort, CIOLibDeviceMgr & genDevMgr, LPCTSTR szSetting)
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

    TCHAR szPort[20];
    _stprintf(szPort, _T("\\\\.\\COM%d"), lPort);
    //_stprintf(szPort, _T("COM%d"), lPort);
    
    m_hSerialPort = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    _ASSERT(INVALID_HANDLE_VALUE != m_hSerialPort);
    
    //
    // Initialize the DCB settings
    //
    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);

    BOOL bRet = BuildCommDCB(szSetting,&dcb);
    _ASSERT(bRet);

	bRet = SetCommState(m_hSerialPort, &dcb);
    _ASSERT(bRet);

    //
	// configure timeouts Retrieve the time-out parameters for all read and write operations and change the settings
	// 
    COMMTIMEOUTS CommTimeouts = {0};
	GetCommTimeouts(m_hSerialPort, &CommTimeouts);
    
	CommTimeouts.ReadIntervalTimeout = 50;  
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;  
	CommTimeouts.ReadTotalTimeoutConstant = 0;    
	CommTimeouts.WriteTotalTimeoutMultiplier = 10;  
	CommTimeouts.WriteTotalTimeoutConstant = 1000;    

    //
	// Set the time-out parameters for all read and write operations on the port. 
	// 
    bRet = SetCommTimeouts(m_hSerialPort, &CommTimeouts);
	_ASSERT(bRet);

    //
    // Set the event for firing
    //
    bRet = SetCommMask(m_hSerialPort, EV_RXFLAG | EV_TXEMPTY | EV_CTS | EV_DSR | EV_RXCHAR | EV_ERR | EV_BREAK | EV_RING);
    _ASSERT(bRet);

    //
    // Add device to be notified on mgr completion port
    //
    HANDLE hCompletionPort = CreateIoCompletionPort(m_hSerialPort, m_pGenDevMgr->m_hCompletionPort, lDeviceID, 0);
    _ASSERT(hCompletionPort != NULL);

    //
    // Go ahead and perform the first read
    //
	DWORD errors = 0x000008ffL;
	ClearCommError(m_hSerialPort, &errors, NULL);
	ClearCommBreak(m_hSerialPort);
    ReadData();

    m_pGenDevMgr->OnConnected(*this);
    
    
    return TRUE;
}


    
BOOL CIOLibSerialDevice::ReinitializeInternal()
{
    m_bReinitializing = TRUE;

    if( INVALID_HANDLE_VALUE != m_hSerialPort )
    {
        CloseHandle(m_hSerialPort);
        Sleep(1000);
        m_hSerialPort = INVALID_HANDLE_VALUE;
    }

    if( m_pBuffer )
    {
        delete [] m_pBuffer;
        m_pBuffer = NULL;
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

    m_pBuffer = new BYTE[CIOLib::SerialBlockSize];

    ZeroMemory(&m_olRead, sizeof(m_olRead));
    ZeroMemory(&m_olWrite, sizeof(m_olWrite));
    
    m_olRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_olWrite[0].hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    m_olWrite[1].hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    m_olWrite[2].hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);

    BOOL bRet = Initialize(m_lDeviceID, m_lPort, *m_pGenDevMgr, m_szSetting);
    m_bReinitializing = FALSE;
    return bRet;
}

void CIOLibSerialDevice::IdlePeriodHousekeeping()
{
    // nothing to do here!  Serial devices will not be recycled.
    ResetIdlePeriod();

}