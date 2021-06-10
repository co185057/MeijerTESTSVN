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

CIOLibSimulatorDevice::CIOLibSimulatorDevice()
: CIOLibDevice(CIOLib::dtSimulator) 
{
        
}


BOOL CIOLibSimulatorDevice::ReinitializeInternal()
{ 
    // close connections and cleanup

    Initialize(m_lDeviceID, m_lPort, *m_pGenDevMgr, m_szSetting);
    return TRUE; 
}

BOOL CIOLibSimulatorDevice::Initialize(long lDeviceID, long lPort, CIOLibDeviceMgr & genDevMgr, LPCTSTR szSetting)
{
    ResetIdlePeriod();

    genDevMgr.OnConnected(*this);
    return TRUE;
}


    
BOOL CIOLibSimulatorDevice::WriteData(PVOID pvData, DWORD dwSize)
{
    ResetIdlePeriod();

	return S_OK;
}

void CIOLibSimulatorDevice::ReadData(void)
{
    ResetIdlePeriod();
}


void CIOLibSimulatorDevice::IdlePeriodHousekeeping()
{
    Reinitialize();
}