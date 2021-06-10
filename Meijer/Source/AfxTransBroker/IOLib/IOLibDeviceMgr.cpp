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

DWORD WINAPI WorkerThreadProc(LPVOID p)
{
    _ASSERT(_CrtIsValidHeapPointer(p));
    CIOLibDeviceMgr::PWORKERTHREADSTRUCT pWTS = (CIOLibDeviceMgr::PWORKERTHREADSTRUCT)p;
    CIOLibDeviceMgr *pDevMgr = pWTS->pDevMgr;
    HANDLE hComplete = pWTS->hThreadComplete;
    
    // delete the object
    delete pWTS;
    pWTS = NULL;
    
    // call the member function
    pDevMgr->WorkerThread(hComplete);
    return 0;
}

void CIOLibDeviceMgr::WorkerThread(HANDLE hThreadComplete)
{
    //
    // Block on completion port
    //
    while(!m_bShuttingDown)
    {
        DWORD dwNumBytes = 0;
        ULONG ulKey = 0;
        LPOVERLAPPED pOL = NULL;
        
        // wait for something
        BOOL bRet = GetQueuedCompletionStatus(m_hCompletionPort, &dwNumBytes, &ulKey, &pOL, CIOLib::DeviceMgrWatchdogPeriod);

        DWORD dwError = GetLastError();

		if( bRet )
        {
			if( m_bShuttingDown )
			{
				Trace(CIOLib::ttWarning, _T("Received Some Data while shutting down, Error :  %d."), dwError);
				break;
			}

            //
            // Handle special commands first
            //
            if( CIOLib::CMD_BOGUS == ulKey )
            {
                // 
                // Bogus command nothing to do here.
                //
                CIOLibDevice & dev = (CIOLibDevice&)*pOL;
                Trace(CIOLib::ttWarning, _T("Received a special NOOP completion port from device %d."), dev.GetDeviceID());
            }
            else if( CIOLib::CMD_SOCKET_ACCEPT == ulKey )
            {
                //
                // connection made to server requiring accept
                //
                CIOLibSocketServer & dev = (CIOLibSocketServer&)*pOL;
                dev.AcceptConnection();
            }
            else
            {
                // handle any command being issued
                EnterCriticalSection(&m_csDevices);
                CIOLib::IOLibDEVICEMAP::iterator iter = m_mapDevices.find((long)ulKey);
                if( iter != m_mapDevices.end() )
                {
                    CIOLibDevice * pDev = (CIOLibDevice*)(iter->second);

					LeaveCriticalSection(&m_csDevices); // optimistic protection against race conditions.

                    if( pOL->Offset == CIOLib::CMD_IO_READ )
                    {
                        if( dwNumBytes > 0 )
                        {
							OnDataReadComplete(*pDev, pDev->GetReadDataPtr(), dwNumBytes);
							if( pDev->GetDeviceID() >= 0 )
                                pDev->ReadData();
							else
	                            Trace(CIOLib::ttError, _T("!!!OnDataReadComplete DeviceID: %d"), pDev->GetDeviceID());
                        }
                        else
                        {
                            // reinitialize the device, connection was likely broken
                            Trace(CIOLib::ttWarning, _T("Lost connection to host for device: %d"), ulKey);
                            
                            BOOL bRet = pDev->Reinitialize();
                            _ASSERT(bRet);
                        }
                    }
                    else if( pOL->Offset == CIOLib::CMD_IO_WRITE)
                    {
                        OnDataWriteComplete(*pDev);
                    }
                    else
                    {
                        Trace(CIOLib::ttWarning, _T("Unrecognized overlapped CMD_IO_?? %d."), pOL->Offset);
                    }                    
                }
                else
                {
                    LeaveCriticalSection(&m_csDevices);
                    Trace(CIOLib::ttWarning,  _T("Unrecognized device %d not in device map."), ulKey);
                }
                
            }
       }
        else if( WAIT_TIMEOUT == dwError )
        {
			//
            // we might have been requested to exit
            //

            //
            // Housekeeping: see if there are devices that have been idle 
            // for longer than DeviceIdleResetPeriod, if so we'll recycle their connections
            //
            EnterCriticalSection(&m_csDevices);

            CIOLib::IOLibDEVICEMAP::iterator iter;
            
            for(iter = m_mapDevices.begin(); iter != m_mapDevices.end(); iter++)
            {
                CIOLibDevice * pDev = (CIOLibDevice*)(iter->second);
                if( pDev )
                {
                    if( pDev->GetIdlePeriod() > CIOLib::DeviceIdleResetPeriod ) 
                    {
                        // run this when the idle time has expired
                        Trace(CIOLib::ttInfo,  _T("Housekeeping for Device %d triggered."), pDev->GetDeviceID());
                        pDev->IdlePeriodHousekeeping();
                    }
                }
            }
            LeaveCriticalSection(&m_csDevices);

            continue;
        }
        else if( ERROR_OPERATION_ABORTED == dwError )
        {
            // we were cancelled while waiting
            Trace(CIOLib::ttWarning, _T("GetQueuedCompletionStatus ERROR_OPERATION_ABORTED."), ulKey);
            continue;
        }
        else if( ERROR_NETNAME_DELETED == dwError || ERROR_CONNECTION_ABORTED == dwError || 
			ERROR_SEM_TIMEOUT == dwError || ERROR_NETWORK_UNREACHABLE == dwError)
        {
            Trace(CIOLib::ttWarning, _T("Lost connection to host for device: %d."), ulKey);
            
            // lost connection to host
            EnterCriticalSection(&m_csDevices);
            CIOLib::IOLibDEVICEMAP::iterator iter = m_mapDevices.find((long)ulKey);
            if( iter != m_mapDevices.end() )
            {
                CIOLibDevice * pDev = (CIOLibDevice*)(iter->second);
                LeaveCriticalSection(&m_csDevices);
                // reinitialize the device, connection was likely broken
                BOOL bRet = pDev->Reinitialize();
                _ASSERT(bRet);
            }
            else
            {
                LeaveCriticalSection(&m_csDevices);
            }
            continue;
        }
        else
        {
			Trace(CIOLib::ttError, _T("Lost connection - unknown error device: %d, err:%d"), ulKey, dwError);
            // other error
            _ASSERT(FALSE);
        }

    }
    
    // signal that we've exited the loop
    SetEvent(hThreadComplete);
}


CIOLibDeviceMgr::CIOLibDeviceMgr(void)
: m_dwWorkerThreads(0), m_hCompletionPort(NULL), m_bShuttingDown(FALSE), m_bBeingDestructed(FALSE)
{
    InitializeCriticalSection(&m_csDevices);
    m_mapDevices.clear();
    m_listWorkerThreads.clear();
    m_vecWorkersComplete.clear();
}

CIOLibDeviceMgr::~CIOLibDeviceMgr(void)
{
    m_bBeingDestructed = TRUE;
    //
    // Iterate through the list and delete all device instances
    //
    EnterCriticalSection(&m_csDevices);
    while( m_mapDevices.size() > 0 )
    {
        CIOLib::IOLibDEVICEMAP::iterator iter = m_mapDevices.begin();
        
        CIOLibDevice * pDev = (CIOLibDevice*)(iter->second);
        if( pDev )
        {
            delete pDev;
        }
        m_mapDevices.erase(iter);
    }
    LeaveCriticalSection(&m_csDevices);
    
    CIOLib::HANDLEVECTOR::iterator iter = m_vecWorkersComplete.begin();
    
    PHANDLE phComplete = &(*iter);
    
    m_bShuttingDown = TRUE;
    
    DWORD dwRet = WaitForMultipleObjects((DWORD)m_vecWorkersComplete.size(), phComplete, TRUE, CIOLib::DeviceMgrWatchdogPeriod*3);
    
    if( WAIT_TIMEOUT == dwRet )
    {
        // not all threads completed although they should have 
        // trace for debugging
        _ASSERT(FALSE);
    }
    
    // 
    // Destroy the thread complete signals
    //
    for(iter = m_vecWorkersComplete.begin(); iter < m_vecWorkersComplete.end(); iter++)
    {
        HANDLE hComplete = (HANDLE)*iter;
        if( hComplete != NULL )
        {
            CloseHandle(hComplete);
        }
    }
    m_vecWorkersComplete.clear();
    
    // Looks like we're not supposed to be cleaning up the main completion port handle
    // as long as all the other reference handles are completed we're okay.
    // the following code causes an -invalid handle crash
    if( m_hCompletionPort != NULL )
    {
        CloseHandle(m_hCompletionPort);
        m_hCompletionPort = NULL;
    }
    DeleteCriticalSection(&m_csDevices);
}

CIOLibDevice * CIOLibDeviceMgr::GetDevice(long lDeviceID) 
{ 
    CIOLibDevice *pDev= NULL;
    CIOLib::IOLibDEVICEMAP::iterator iter = m_mapDevices.find(lDeviceID);
        
    if (iter != m_mapDevices.end() )
    {
        pDev= iter->second; 
    }

    return pDev;
}

void CIOLibDeviceMgr::PrimeReadData(long lDeviceID)
{
    CIOLibDevice *pDev= GetDevice(lDeviceID);
    if (pDev)
    {
        pDev->ReadData();
    }
}

void CIOLibDeviceMgr::WriteData(long lDeviceID, PVOID pvData, DWORD dwSize)
{
    EnterCriticalSection(&m_csDevices);
	__try
		{
		CIOLibDevice *pDev= GetDevice(lDeviceID);
		if (pDev)
		{
			Trace(CIOLib::ttInfo, _T("WriteData dev:%d waiting for write overlap:%d to complete"), lDeviceID, pDev->m_nPendingWrites);
			WaitForSingleObject(pDev->m_olWrite[pDev->m_nPendingWrites].hEvent, INFINITE);
			if (dwSize > IOBUFSIZE)
				Trace(CIOLib::ttError, _T("WriteData Buffer TOO BIG"));
			else
			{
				// first copy the data so that the client is free of managing the buffer
				memcpy(pDev->m_WriteBuf[pDev->m_nPendingWrites], pvData, dwSize);
				pDev->WriteData(pDev->m_WriteBuf[pDev->m_nPendingWrites], dwSize);
				// we have three concurrent buffers.  move on to the next one.
				pDev->m_nPendingWrites=(pDev->m_nPendingWrites + 1) % 3;
			}
		}
		else
		{
			Trace(CIOLib::ttError, _T("WriteData with Unknown DeviceID %d"), lDeviceID);
		}
	}
	__finally {
		LeaveCriticalSection(&m_csDevices);
	}
}

//if lNumThreads > 0, then it specifies the number of threads
//if lnNumThreads < 0, then it specifies the number of threads per processor (-)
//if lNumThreads == 0, then it defaults to 1 thread
void CIOLibDeviceMgr::Initialize(long lNumThreads)
{
    Trace(CIOLib::ttInfo, _T("Initializing IOLib Device Manager"));
    
    //
    // Create the main completion port
    //
    if( lNumThreads < 0 )
    {
        SYSTEM_INFO si = { 0 };
        GetSystemInfo(&si);
        m_dwWorkerThreads = si.dwNumberOfProcessors * (-lNumThreads);
    }
    else
    {
        m_dwWorkerThreads = (DWORD)max(lNumThreads,1);
    }
    
    // let's keep this fixed.  Dev machines with hyperthreading turned will be seen as having
    // two processors and thus 4 worker threads will be created. We want the environment to be the
    // same everywhere.
    m_dwWorkerThreads = 1;//4;

    m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_dwWorkerThreads);
    _ASSERT(m_hCompletionPort != NULL);
    
    
    //
    // Create the worker threads
    //
    for(DWORD dw  = 0; dw < m_dwWorkerThreads; dw++)
    {
        // create the info struct to pass to the worker thread
        PWORKERTHREADSTRUCT pwts = new WORKERTHREADSTRUCT;
        pwts->pDevMgr = this;
        
        // create a completion signal
        pwts->hThreadComplete = CreateEvent(NULL,TRUE, FALSE, NULL);
        m_vecWorkersComplete.push_back(pwts->hThreadComplete);
        
        // create the worker thread
        DWORD dwThreadID = 0;
        HANDLE hThread = CreateThread(NULL, 0, WorkerThreadProc, pwts, 0, &dwThreadID);
        _ASSERT(hThread != NULL);
        m_listWorkerThreads.push_back(hThread);
        
    }
}

CIOLibDevice & CIOLibDeviceMgr::CreateDevice(CIOLib::DeviceType dt, long lDeviceID, long lPort, LPCTSTR szSetting)
{
    Trace(CIOLib::ttInfo,_T("Creating device of type %d with device ID %d"), dt, lDeviceID);

    //
    // Ensure uniqueness of Device ID
    //
    EnterCriticalSection(&m_csDevices);
    CIOLib::IOLibDEVICEMAP::iterator iter = m_mapDevices.find(lDeviceID);
    if( iter != m_mapDevices.end() )
    {
        LeaveCriticalSection(&m_csDevices);
        throw _T("ID already assigned to a device.");
    }
    LeaveCriticalSection(&m_csDevices);
    
    CIOLibDevice * pIODev = NULL;
    switch(dt)
    {
    case CIOLib::dtSerial: 
        pIODev = (CIOLibDevice*)(new CIOLibSerialDevice());
        break;
        
    case CIOLib::dtSocketServer:
        pIODev = (CIOLibDevice*)(new CIOLibSocketServer());
        break;
        
    case CIOLib::dtSimulator:
        pIODev = (CIOLibDevice*)(new CIOLibSimulatorDevice());
        break;

    case CIOLib::dtSocketClient:
        pIODev = (CIOLibDevice*)(new CIOLibSocketClient());
        break;
        
    default:
        _ASSERT(FALSE);
    };

    //
    // Add device to map
    //
    EnterCriticalSection(&m_csDevices);
    m_mapDevices[lDeviceID] = pIODev; 
    LeaveCriticalSection(&m_csDevices);

    BOOL bRet = pIODev->Initialize(lDeviceID, lPort, *this, szSetting);
    _ASSERT(bRet);

    Trace(CIOLib::ttInfo, _T("Device creation complete. Created: %d, Init Status: %d"), (long)(pIODev != NULL), (long)bRet);
    return *pIODev;
}


void CIOLibDeviceMgr::RemoveDevice(long lDeviceID)
{
    Trace(CIOLib::ttInfo, _T("+Removed device %d."), lDeviceID);
    EnterCriticalSection(&m_csDevices);
    CIOLib::IOLibDEVICEMAP::iterator iter = m_mapDevices.find(lDeviceID);
    if( iter != m_mapDevices.end() )
    {
        
        CIOLibDevice * pDev = (CIOLibDevice*)(iter->second);
        
        if( pDev )
        {
            delete pDev;
        }
        m_mapDevices.erase(iter);
    }
    LeaveCriticalSection(&m_csDevices);
    Trace(CIOLib::ttInfo, _T("-Removed device"));
}


void CIOLibDeviceMgr::RemoveAllDevices(void)
{
    Trace(CIOLib::ttInfo, _T("+RemoveAllDevices"));
	m_bShuttingDown = TRUE;
    EnterCriticalSection(&m_csDevices);
    CIOLibDevice * pDev = NULL;
    
    //
    // Iterate through the list and delete all device instances
    //
    while( m_mapDevices.size() > 0 )
    {
        CIOLib::IOLibDEVICEMAP::iterator iter = m_mapDevices.begin();
        
        CIOLibDevice * pDev = (CIOLibDevice*)(iter->second);
        if( pDev )
        {
            delete pDev;
        }
        m_mapDevices.erase(iter);
    }
    LeaveCriticalSection(&m_csDevices);
    Trace(CIOLib::ttInfo, _T("-RemoveAllDevices"));
}

void CIOLibDeviceMgr::Trace(CIOLib::TraceType tt, LPCTSTR pszFormat, ...)
{
    if( pszFormat && *pszFormat && !m_bBeingDestructed )
    {
        va_list ptr;
	    va_start(ptr, pszFormat);

        int iLength = _vsctprintf(pszFormat,ptr) + 1;

        LPTSTR pszTrace = new TCHAR[iLength];
        _vstprintf(pszTrace, pszFormat, ptr);
        if( !m_bBeingDestructed) OnTrace(tt, pszTrace);
        delete [] pszTrace;
	    va_end(ptr);
    }
}