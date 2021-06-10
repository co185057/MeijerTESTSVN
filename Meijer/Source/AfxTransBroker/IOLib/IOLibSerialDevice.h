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

class CIOLibSerialDevice : public CIOLibDevice
{
public:
    virtual ~CIOLibSerialDevice(void); 
    virtual BOOL WriteData(PVOID pvData, DWORD dwSize);

protected:
    CIOLibSerialDevice();
    virtual BOOL Initialize(long lDeviceID, long lPort, CIOLibDeviceMgr & genDevMgr, LPCTSTR szSetting = NULL);
    virtual BOOL ReinitializeInternal();
    virtual void IdlePeriodHousekeeping();
    virtual void ReadData(void);
    inline virtual const PVOID GetReadDataPtr(void) { return m_pBuffer; }
    HANDLE m_hSerialPort;
    LPBYTE m_pBuffer;

    friend class CIOLibDeviceMgr;
};
