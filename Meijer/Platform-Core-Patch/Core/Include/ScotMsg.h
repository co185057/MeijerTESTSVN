/*
-------------------------------------------------------------------------------

 File Name       : SCOTMsg.H
 Type            : Common include file for the SCOT application
                   Message format definitions

 Language        : MSVC++
 Operating System: Win32
 Author/Location : SCOT 2.0 Team & MGV, RSG-Atlanta

 Copyright (C) NCR Corporation 1998

-------------------------------------------------------------------------------
 Date    | Changes Made                                     | Version | By
-------------------------------------------------------------------------------
 10/27/98  Original version                                   01.00.00  NCR/MGV
-------------------------------------------------------------------------------

 This is an application include file.
 
-------------------------------------------------------------------------------
$Archive: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/ScotMsg.h $
$Revision: 3 $
$Date: 10/13/08 11:49a $
-------------------------------------------------------------------------------
*/

#ifndef  _SCOTMSG_H_
#define  _SCOTMSG_H_

#define	SCOT_MESSAGE		  (WM_USER+1)
#define	WMU_SCOTWM_CLOSE	  (WM_USER+101)	// tell scot to shutdown
#define WMU_SCOT_GETDIAGFILES (WM_USER+200) // used by GetDiagFiles.exe

#define  ID_TRANSBROKER    1000           /* id of the TB component */


#include    "SCOTUICMD.h"  // UI request format from external sources(e.g. TB) to FastLane


#ifdef   __cplusplus
extern   "C" {
#endif   /* __cplusplus */

// nDeviceID can be any one of the following values:
typedef enum
{
   TB_DEV_CMOS=1,                // CMOS device
   TB_DEV_DISPLAYINTERMEDIATE,   // intermediate display
   TB_DEV_DISPLAYPRIMARY,        // The primary display
   TB_DEV_DISPLAYSECONDARY,      // The secondary display
   TB_DEV_DISPLAYIMMEDIATE,      // Assistmode display or monitor all display msgs
   TB_DEV_DRAWER1,               // Primary cash drawer
   TB_DEV_DRAWER2,               // Secondary cash drawer
   TB_DEV_EPSMODULE,             // Electronic payment server as a virtual device
   TB_DEV_KEYBOARD,              // Keyboard device
   TB_DEV_KEYLOCK,				 // keylock device
   TB_DEV_MSR,                   // MSR device
   TB_DEV_PRINTERJOURNAL,        // Journal printer device
   TB_DEV_PRINTERRECEIPT,        // Customer receipt printer device
   TB_DEV_PRINTERSLIP,           // Slip printer device
   TB_DEV_SCALE,                 // Scale device
   TB_DEV_SERIAL,				 // Serial device
   TB_DEV_SCANNER,               // Scanner device
   TB_DEV_ATMMODULE,             // Cash Withdrawal dll as a virtual device
   TB_DEV_UNKNOWN,               // Unknown device or event
   TB_DEV_DISPLAYDATANEEDED,     // Data Needed state     RFC 197844
   TB_DEV_UICOMMAND,
   TB_DEV_LASTINLIST = 2000      // please add new devices above this id 
} TBDEVICEID;

typedef enum
{
  TB_REQ_DOSOPEN=1,
  TB_REQ_DOSCLOSE,
  TB_REQ_DOSREAD,
  TB_REQ_DOSWRITE,
  TB_REQ_DOSSEEK,
  TB_REQ_DOSIOCTL
} TBFILEFUNCTIONS;

typedef enum
{
  TB_IOCTL_REQ_PURGE=1
} TBFILESUBFUNCTIONS;

/*<<<Typedefs>>>-------------------------------------------------------------*/
const enum GPEVT
{
  GP_STOP,
  GP_RESTART,
  GP_PUTSCOTINLC,
  GP_STOPSCOT,
  GP_QUERYSCOTSTATE,
  GP_MOTIONEVENT,							// RFC 219283 (& 218455) DE/RF 11/02/02
  GP_POWERSTOPSCOT,                 // RFC 265463 //+dp185016 rfc 265463
  GP_GETTEXT,                        // RFC 265463 //-dp185016
  GP_MOTIONONATTRACT                       // RFC 265465
};

typedef struct
{
  int  nEvt;								  // event
  long lParm;								  // data
  long wParm;                                 // Data
  LPTSTR pszContext;						  // source context of event		
} PSInfo;

// Elementary structure type for information from Device Manager.

typedef struct
{
  int  Msg;				   // status event or input event data or error
  int Class;			   // device class
  int DeviceId;		   // device Id
  long lCodeOrLength;	// error or status code or data length (depends on Msg)
  LPVOID vDPtr;         // pointer to void * data, must be deleted if Length > 0
  long lDirectIONumber; // Event number of direct IO event
  long lDirectIONumData; // Numeric data of direct IO event
} DMInfo;

// Elementary structure type for information from Security Manager.
typedef struct
{
  int  nEvt;								  // event
  LPTSTR szParms;						  // source list of paramters		
} SecMgrInfo;


typedef struct
{
	int Msg;					// This will be the identifying code
	WPARAM wParam;        //					
	LPARAM lParam;		//
 } RPInfo;

// Elementary structure type for information from Customer info (Personalization).
typedef struct
{
  int  Msg;								  // event
  LPTSTR szParms;						  // source list of paramters		
} CUInfo;

// Elementary structure type for information from Remote Approval.
#define RAMSG_OPERLEN   24
// RFC380920: Password field being used to pass complex login 
// structure including biometric data and whether ID was scanned 
// or keyed
#define RAMSG_PASSLEN   4096
//#define RAMSG_PASSLEN   12
#define RAMSG_AUXLEN    128

typedef struct
{
   UINT     msg;
   DWORD    handle;
   int      approvalCode;
   DWORD    dwCookie;
   _TCHAR     operatorID[RAMSG_OPERLEN];
   _TCHAR     password[RAMSG_PASSLEN];
   _TCHAR     auxdata[RAMSG_AUXLEN];
   int      nDataLength;
   LPTSTR   pData;   // Approval data string from RAP alloc by RACallback
} RAInfo;
    

// Elementary structure type for information from Terminal services.
// This is obsolete, use TBInfo instead.
typedef struct
{
  int  nEvt;											  // source of message, display, printer, serial...
  int  Length;										  // data length
  void  *Data;                      // data, 'length' bytes, new'd by TS, delete'd by MP
} TSInfo;

typedef struct
{
  UINT   nDeviceID;  // Device source
  UINT   nLength;    // Data length of the data pointed to by pData
  void   *pData;     // Data buffer pointer, alloc by TB, freed with TBReleaseMessage
} TBInfo;

// Elementary structure type for information for general purposes
typedef struct
{
  GPEVT nEvt;                       // misc GP info
} GPInfo;


// Structure type used for sending generic File System I/O-type requests,
// such as CMOS requests in a block driver mode.
// Note that data buffer and data buffer length will be set in the nLength
// and pData fields of a MessageElement for read/write.  This structure will usually be
// passed in the lParam field of a MessageElement.

typedef struct
{
  bool bRequest;        // TRUE if its a request, FALSE if its a response
  UINT command;         // command type - open, close, read, write, etc.
  UINT handle;          // handle to file
  int  retCode;         // return code of completed command.
  LPARAM lParam;        // can be used by solution-specific TB for whatever

  union
  {
    struct
    { 
      UINT flags;
      UINT pMode;
    } OpenInfo;  // File name for open is in pData in the MessageElement
    struct
    {
      int  offset;
      UINT sMode;
    } SeekInfo;
    struct
    {
      UINT subCommand;
    } IoctlInfo; // Used for special functions.
  };

} FILE_IO_INFO, *PFILE_IO_INFO;



/////////////////////////////////////////////////////////////////////////////
// Message structure used within the Message Object.

typedef struct
{
  int  id;
  union
  {
    PSInfo psInfo;
    DMInfo dmInfo;
    RAInfo raInfo;
    TBInfo tbInfo;
    GPInfo gpInfo;
	RPInfo rpInfo;
	SecMgrInfo secmgrInfo;
    CUInfo cuInfo;

    struct {
      UINT   nDeviceID;  // Device source
      UINT   nLength;    // Data length of the data pointed to by pData
      void   *pData;     // Data buffer pointer, alloc by TB, freed with TBReleaseMessage
    };
  };
  LPARAM lParam;       // Generic LPARAM to be used for whatever.  Used by CMOS for request details
    
} MessageElement, SMessageElement, *PSMessageElement;


#ifdef   __cplusplus
}        /* End of extern "C" { */
#endif   /* __cplusplus */

#endif   /* _SCOTMSG_H_  */
