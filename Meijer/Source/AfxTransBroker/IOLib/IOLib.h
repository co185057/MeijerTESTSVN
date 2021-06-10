#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
//#include <atlbase.h>
#include <conio.h>
#include <list>
#include <map>
#include <vector>
#include <winsock2.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#define IOBUFSIZE 16*4096

// forwards
class CIOLibDeviceMgr;
/////////////////////////////////////////////////////////////////////
//
// IOLibDeviceManager
// --------------------
// Objects used for interfacing with the Lowes.
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

// class forwards
class CIOLibDevice;

class CIOLib
{
public:
    CIOLib(void) {} 
    virtual ~CIOLib(void) {}

    enum DeviceType
    {
        dtSerial = 1,
        dtSocketServer = 2,
        dtSimulator = 3,
        dtSocketClient = 4
    };

    enum TraceType
    {
        ttInfo = 1,
        ttWarning = 2,
        ttError = 3,
        ttDebug = 4
    };

    static const int SerialBlockSize;
    static const int SocketBlockSize;
    static const int DeviceMgrWatchdogPeriod; // milliseconds
    static const int SocketServerWatchdogPeriod;
    static const DWORD DeviceIdleResetPeriod;
    static const int SocketClientConnectPeriod;
    
    static const long CMD_SOCKET;
    static const long CMD_SOCKET_ACCEPT;
   
    static const long CMD_SERIAL;

    static const long CMD_IO;
    static const long CMD_IO_READ;   
    static const long CMD_IO_WRITE;

    static const long CMD_BOGUS;
    

    typedef std::map<long, CIOLibDevice*> IOLibDEVICEMAP;
    typedef std::vector<HANDLE> HANDLEVECTOR;
};





// includes
#include "IOLibDevice.h"
#include "IOLibDeviceMgr.h"
#include "IOLibSerialDevice.h"
#include "IOLibSocketServer.h"
#include "IOLibSocketClient.h"
#include "IOLibSimulatorDevice.h"



