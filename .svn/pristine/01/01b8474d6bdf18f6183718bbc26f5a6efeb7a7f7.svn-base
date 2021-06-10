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
#pragma once

class CIOLibDeviceMgr
{
public:
    CIOLibDeviceMgr(void);
    virtual ~CIOLibDeviceMgr(void);

    CIOLibDevice & CreateDevice(CIOLib::DeviceType dt, long lDeviceID, long lPort, LPCTSTR szSetting = NULL);
    void WriteData(long lDeviceID, PVOID pvData, DWORD dwSize);
    virtual void OnDataReadComplete(CIOLibDevice & genDev, const LPVOID pvBuffer, DWORD dwBytesReceived) = 0;
    virtual void OnDataWriteComplete(CIOLibDevice & genDev) = 0;
    virtual void OnConnected(CIOLibDevice & genDev) = 0;
    virtual void OnTrace(CIOLib::TraceType tt, LPCTSTR szMessage) = 0;
    virtual void RemoveDevice(long lDeviceID);
    virtual void RemoveAllDevices(void);
	void PrimeReadData(long);
   
protected:
    
    //if lNumThreads > 0, then it specifies the number of threads
    //if lnNumThreads < 0, then it specifies the number of threads per processor (-)
    //if lNumThreads == 0, then it defaults to 1 thread
    void Initialize(long lNumThreads);

private:
    CIOLibDevice *GetDevice(long lDeviceID); 
    std::list<HANDLE> m_listWorkerThreads;
    CIOLib::HANDLEVECTOR m_vecWorkersComplete;
    CIOLib::IOLibDEVICEMAP m_mapDevices;
    DWORD m_dwWorkerThreads;
    HANDLE m_hCompletionPort;
    BOOL m_bShuttingDown;
    CRITICAL_SECTION m_csDevices;
    BOOL m_bBeingDestructed;
    void AddDeviceToCompletionPort(CIOLibDevice & genDev);
    void Trace(CIOLib::TraceType tt, LPCTSTR pszFormat, ...);
    
    typedef struct _WORKERTHREADSTRUCT
    {
        CIOLibDeviceMgr * pDevMgr;
        HANDLE hThreadComplete;
    } WORKERTHREADSTRUCT, *PWORKERTHREADSTRUCT;
    void WorkerThread(HANDLE hThreadComplete);
    
    
    friend class CIOLibSerialDevice;
    friend class CIOLibSocketServer;
    friend class CIOLibSocketClient;
    friend DWORD WINAPI WorkerThreadProc(LPVOID pDevMgr);

};
