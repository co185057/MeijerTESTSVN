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

CIOLibDevice::CIOLibDevice(CIOLib::DeviceType dt) 
    : m_DeviceType(dt), m_lDeviceID(0), m_lPort(0), m_szSetting(NULL), m_pGenDevMgr(NULL), m_dwLastActionStamp(GetTickCount()), m_bReinitializing(FALSE),
	m_nPendingWrites(0),m_dLastError(0)
{

}

CIOLibDevice::~CIOLibDevice() 
{ 
    if( m_szSetting )
    {
        delete [] m_szSetting;
    }
}


BOOL CIOLibDevice::Reinitialize()
{
	// don't call reinitialize if it's still in the process - RRS.
	if( !m_bReinitializing )
	{
		m_bReinitializing = TRUE;
		BOOL bRet = ReinitializeInternal();
		m_bReinitializing = FALSE;
		return bRet;
	}
	return false;
}