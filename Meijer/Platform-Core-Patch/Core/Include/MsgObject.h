//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  MsgObject.H
//
// TITLE:   Class interface definition file (describe class here)
//
//
// AUTHOR:  DcL, Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//

//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MsgObject
#define _MsgObject

#include "SCOTMsg.h"
#include "DLLDefine.h"
#include "TraceObject.h"

#define WMU_LOADCONTROL_CLOSE (WM_USER + 101)
#define WMU_STOP_SCOT (WM_USER + 102)
#define WMU_PUT_SCOT_IN_LC (WM_USER + 103)
#define WMU_QUERY_SCOT_STATE (WM_USER + 104)
// RFC 265463 start
#define WMU_GET_TEXT (WM_USER + 105)
#define WMU_STOP_SCOT_POWER (WM_USER + 106)
// RFC 265463 end

// The Load Controller registry key
// HKEY_LOCAL_MACHINE\SOFTWARE\NCR\SCOT\CurrentVersion\LoadController
// has two values for the Switch application.
//
// The first value is SCOTMainWindowTitle. This value is "SCOTInputWindow".
// The Switch sends a message WMU_LOADCONTROL_CLOSE to the window with this
// title. This message is defined as const long WMU_LOADCONTROL_CLOSE  =
// WM_USER + 101.  The SCOT application uses this message to close all
// devices.
//
// The second value is SCOTUIWindowTitle that is equal "NCR SCOT". The Switch
// application uses the window with title to minimize and maximize the SCOT
// application.
//
// The Scot application has TWO windows. The first  window is for UI and the
// second one is a container window for all OPOS devices. This is a reason
// why the Switch application sends different messages to different windows.
//
   
    
//////////////////////////////////////////////////////////////////////////////////////////////////
// MsgObject class definition

class TraceObject;
class DLLEXPORT
 MsgObject : public CObject
{
private:
//	TraceObject*		pTraceObj; // App sets this member and null values turns off tracing
public:
	// unique messasge ID to be assigned by RegisterWindowMessage()
	int ScotMessageID;
	HWND hw;   // The handle of the window to post message
	TraceObject*		pTraceObj; // App sets this member and null values turns off tracing
    
public:
	MsgObject(HWND, TraceObject* pTO = NULL);		// constructor
	void SetWindowHandle(HWND hwnd) { hw = hwnd; };
	long PostIt(const MessageElement *);
    
    long PostPS( const int, const long, const long, const CString& );
    	
  /*    long PostTS(
        const int,										  // event or device
        const int,										  // length of data
        const void *);                 // data
   */ 	
      long PostDM(
        const int,							// message type
        const int,							// device class
        int,									// device Id 
        const long,                    // error or status code or data length (depends on Msg)
        const void *,                  // data if length > 0
        const long = 0,                // Direct IO event number
        const long = 0);               // Direct IO numeric data
    	
      long PostSecMgr( const int, const CString& );
	

      long PostRA(
        const int,										  // message type
        const int,										  // OutputConclusion
        const int,										  // OutputLevel
		const int,										  // OutputType
        const int);										  // InputID

      long PostRP(
        const int,										  // message type
        const WPARAM,								      // WPARAM
        const LPARAM);								      // LPARAM
      
      long PostGP(const GPEVT);         // misc
      // RFC 265463 start
      long PostGPwithParams(
        const GPEVT,          // message type
        const WPARAM,         // WPARAM
        const LPARAM);        // LPARAM
      // RFC 265463 end
      long PostCU( const int, const CString& );
    
    #endif
};
