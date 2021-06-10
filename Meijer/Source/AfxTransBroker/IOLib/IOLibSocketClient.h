/////////////////////////////////////////////////////////////////////
//
// IOLibDeviceManager
// --------------------
// Devices use I/O completion ports for communication, making the architecture
// scalable to a large amount of devices. The CIOLib, CIOLibDeviceMgr, 
// CIOLibDevice and IOLibDevice derived classes are generic and can be reused in
// other projects as deemed necessary. These classes use STL and Win32 APIs exclusively.
// NO MFC OBJECTS PLEASE!
//
// Author:      Steve Antonakakis
//
// History:     01/17/2005          - Initial release.
//
#pragma once

class CIOLibSocketClient : public CIOLibDevice
{
public:
    virtual ~CIOLibSocketClient();
    virtual BOOL WriteData(PVOID pvData, DWORD dwSize);
	virtual DWORD GetError();

	virtual BOOL ReinitializeInternal();

	virtual void CancelWrite();

protected:
    CIOLibSocketClient();
    virtual BOOL Initialize(long lDeviceID, long lPort, CIOLibDeviceMgr & genDevMgr, LPCTSTR szSetting = NULL);
    virtual void ReadData();
    
    virtual void ConnectSocket();
    virtual void IdlePeriodHousekeeping();

    inline virtual const PVOID GetReadDataPtr() { return m_pBuffer; }

    SOCKET m_sClientSocket;

    LPBYTE m_pBuffer;
    BOOL m_bConnecting;
    HANDLE m_hConnectThread;

	DWORD m_dLastError;

private:
    void Uninitialize();

    friend class CIOLibDeviceMgr;   
    friend DWORD WINAPI SocketConnectThreadProc(LPVOID pvParam);
};
