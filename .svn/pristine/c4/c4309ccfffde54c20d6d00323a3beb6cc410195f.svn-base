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

class CIOLibSocketServer : public CIOLibDevice
{
public:
    virtual ~CIOLibSocketServer(void);
    virtual BOOL WriteData(PVOID pvData, DWORD dwSize);

protected:
    CIOLibSocketServer();
    
    virtual BOOL Initialize(long lDeviceID, long lPort, CIOLibDeviceMgr & genDevMgr, LPCTSTR szSetting = NULL);
    virtual BOOL ReinitializeInternal();
    virtual void ListenSocket();
    virtual void AcceptConnection();
    virtual long GetNetworkEvents();
    virtual void IdlePeriodHousekeeping();
    
    virtual void ReadData(void);
    inline virtual const PVOID GetReadDataPtr(void) { return m_pBuffer; }

    SOCKET m_sListenSocket;
    SOCKET m_sClientSocket;
    HANDLE m_hListenSignal;
    HANDLE m_hListenThread;
    OVERLAPPED m_olRead;
    OVERLAPPED m_olWrite;
    HANDLE m_hReadSignal;
    HANDLE m_hWriteSignal;
    LPBYTE m_pBuffer;
    BOOL m_bListening;

private:
    void Uninitialize();

    friend class CIOLibDeviceMgr;
    friend DWORD WINAPI SocketListenThreadProc(LPVOID pvParam);
};
