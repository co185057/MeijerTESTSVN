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

#include "IOLib.h"

class CIOLibDevice
{
public:
    
    virtual ~CIOLibDevice(void);
    virtual BOOL WriteData(PVOID pvData, DWORD dwSize) = 0;

    inline BOOL operator==(CIOLibDevice & dev) { return this->m_lDeviceID == dev.m_lDeviceID; }
    inline BOOL operator==(long lDeviceID)     { return this->m_lDeviceID == lDeviceID; }
    inline long GetDeviceID() { return m_lDeviceID; }
    inline long GetDeviceType() { return m_DeviceType; }
	inline DWORD GetError() { return m_dLastError; }

	virtual BOOL Reinitialize();

protected:
    CIOLibDevice(CIOLib::DeviceType dt);
    inline DWORD GetIdlePeriod() { return GetTickCount() - m_dwLastActionStamp; }
    inline void ResetIdlePeriod() { m_dwLastActionStamp = GetTickCount(); }
    

    virtual BOOL Initialize(long lDeviceID, long lPort, CIOLibDeviceMgr & genDevMgr, LPCTSTR szSetting = NULL) = 0;
    virtual void ReadData(void) = 0;
    virtual const PVOID GetReadDataPtr(void) = 0;
//    virtual BOOL Reinitialize();
    virtual BOOL ReinitializeInternal() = 0;

    virtual void IdlePeriodHousekeeping() = 0;

    long m_DeviceType;
    long m_lDeviceID;
    long m_lPort;
    LPTSTR m_szSetting;
    CIOLibDeviceMgr * m_pGenDevMgr;
    DWORD m_dwLastActionStamp;
    BOOL m_bReinitializing;

	DWORD m_dLastError;
    OVERLAPPED m_olRead;
    OVERLAPPED m_olWrite[3];
    BYTE m_WriteBuf[3][IOBUFSIZE];
	int m_nPendingWrites;
    friend class CIOLibDeviceMgr;
};
