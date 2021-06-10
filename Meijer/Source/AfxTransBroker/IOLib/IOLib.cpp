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

const int CIOLib::SerialBlockSize = 65536;
const int CIOLib::SocketBlockSize = 65536;
const int CIOLib::DeviceMgrWatchdogPeriod = 2000; // milliseconds
const int CIOLib::SocketServerWatchdogPeriod = 5000;
const int CIOLib::SocketClientConnectPeriod = 1000;
const DWORD CIOLib::DeviceIdleResetPeriod = 15*60*1000; // milliseconds  (15 minutes)

const long CIOLib::CMD_SOCKET             = 0x67451000;
const long CIOLib::CMD_SOCKET_ACCEPT      = CMD_SOCKET + 1;

const long CIOLib::CMD_SERIAL             = 0x67452000;

const long CIOLib::CMD_IO                 = 0x67453000;
const long CIOLib::CMD_IO_READ            = CMD_IO + 1;   
const long CIOLib::CMD_IO_WRITE           = CMD_IO + 2;

const long CIOLib::CMD_BOGUS              = 0x67454000;