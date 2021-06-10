//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File: DMTriColorLight.H
//
//  Description: This class provides an interface for the device manager
//    tricolor light (not the lane light, but the red, yellow, green
//    light.)  All logic for saving the state of the light when changing
//    color, and all interface inconveniences with the DM interface for
//    the light are contained here.
//
//	Copyright:		NCR Corporation (c) 1999
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// Tar 234730 - prevent some of the tricolorlight & pager problems from the dual request logic;
// still allowing the use of DM_TRICOLORLIGHT_SECURITYREQUEST by SSF for backward compatability;
// changing our internal logic to process it the same as DM_TRICOLORLIGHT_NORMALREQUEST
// this will simplify this logic and hopefully pave the way for future enhancements
// still keeping the virtual class, using virtualNormalLight, another virtual instance may be
// used in the future but be careful to not break the continued use of SECURITYREQUEST
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DMTriColorLight
#define _DMTriColorLight

// In-Store Message Support
#include "DMMessageExtension.h"

// Default Messages for In-Store Message Support
#define DEFAULT_LANE_MESSAGE			_T("LANE ##:\0")
#define DEFAULT_RED_MESSAGE				_T("Red\0")
#define DEFAULT_YELLOW_MESSAGE			_T("Yellow\0")
#define DEFAULT_MEDIA_LOW_MESSAGE		_T("Media Low\0")
#define DEFAULT_OUT_OF_SERVICE_MESSAGE	_T("Out Of Service\0")


// TriColor light constants

// colors
enum DMTriColorLightColor
{  DM_TRICOLORLIGHT_GREEN = 1,
   DM_TRICOLORLIGHT_YELLOW = 2,
   DM_TRICOLORLIGHT_RED = 3
};

// states
enum DMTriColorLightState
{
  // Do not add any before OFF, do not change OFF from 0.
  DM_TRICOLORLIGHT_OFF = 0,
	DM_TRICOLORLIGHT_ON = 1,
	DM_TRICOLORLIGHT_BLINK_QUARTERHZ = 2,
	DM_TRICOLORLIGHT_BLINK_HALFHZ = 3,
	DM_TRICOLORLIGHT_BLINK_1HZ = 4,
	DM_TRICOLORLIGHT_BLINK_2HZ = 5,
	DM_TRICOLORLIGHT_BLINK_4HZ = 6
};

// type of request - security agent or other
enum DMTriColorLightRequestType
{
  DM_TRICOLORLIGHT_NORMALREQUEST = 0,
	DM_TRICOLORLIGHT_SECURITYREQUEST = 1	// Tar 234730 retaining for backward compatability of SSF
};

// forward declare for DMVirtualTriColorLight
class DMTriColorLight;

//////////////////////////////////////////////////////////////////////////////
//
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
   DMVirtualTriColorLight : public CObject
{
public:
  // constructor
  DMVirtualTriColorLight();
  // overload =
  DMVirtualTriColorLight& operator=(const DMVirtualTriColorLight& virtualLight)
  {
    redState = virtualLight.redState;
    yellowState = virtualLight.yellowState;
    greenState = virtualLight.greenState;
    return *this;
  }
private:
  // Grant direct access to these private variables to DMTriColorLight
  friend DMTriColorLight;
	
  DMTriColorLightState redState,
                       yellowState,
                       greenState;
};


/////////////////////////////////////////////////////////////////////////////
//
#include "DllDefine.h"
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 DMTriColorLight : public CObject
{

public:
  // constructor
  DMTriColorLight();
  // destructor
  ~DMTriColorLight();

  // Object Initializer ( called by SMOutOfServiceBase::InitializeAll)
  void Initialize();  // In-Store Message Support

  // get, set methods
  DMTriColorLightColor getColor() { return theColor; }
  DMTriColorLightState getState() { return theState; }
	
//-------------------------------------------------------------------------
//
// set()
//
// Sets the light and saves the light state for future sets.
//
// The separate lights have a priority order: red/yellow/green.  This
// determines what happens when a light is set on if another light is already
// on.  e.g. 1- green is turned on, 2- red is turned on.  Red is higher
// priority, so green goes off.  3- green is turned on.  Green is lower
// priority so no change.  4- Red is turned off.  Green must now be turned
// back on since it was on in step 1.
//
// To simply turn the entire light off and reset, use off().
//
//-------------------------------------------------------------------------
  void set(const DMTriColorLightColor       color,
		       const DMTriColorLightState       state,
		       const DMTriColorLightRequestType request,
		             CDevMgr&                   dm,
		             int                        index);

//------------------------------------------------------
// off()
//
// Turn physical light off and delete all saved values.
//
//------------------------------------------------------
  void off(CDevMgr& dm, int index);

//-----------------------------------------------
// backup
//
// Save the current state of the TriColorLight.
//
//-----------------------------------------------
  void backup();

//--------------------------------------------------------
// restore
//
// Restore the last backed up state of the TriColorLight.
//
//--------------------------------------------------------
  void restore(CDevMgr& dm, int index);

private:
	
  long convertToDevMgrState(const DMTriColorLightState state);

// *** Start In-Store Message Support
  // Page Processing Functions 
  void processPages( const DMTriColorLightColor color,
					 const DMTriColorLightState state);

  static UINT sendPageThread(LPVOID *lpParam); // RFC 266954 - send in separate thread
  void sendPage();    // RFC 266954 - runs in separate thread
  // Pager In Service Function
  bool pagerInService();
  // Pager Message Formatting Function
  void pageMessageFormatter();
  // Pager Resend Function
  void resendPage(); // RFC 266954 - converted from pageTimerProc

  // Media Low Page Limit Callback Function
  static void CALLBACK EXPORT pageLimitTimerProc( HWND hwnd, UINT msg,
												  UINT idTimer, DWORD dwTime);
  // Resend Pages Setup Function
  void setupPageResend( const int, DMTriColorLightColor);
  // Page Verification and Send Function
  bool verifyAndSendPage( const bool bSendPage, bool& bPageStatus,
					      LPCTSTR Message, const int ElapseTime,
						  DMTriColorLightColor color);
  // Page Reset Function
  void ResetPages();
// *** End In-Store Message Support

  // TAR 328090:  
  // Method to update the state of the RAP buttons if setting the trilight fails.  
  bool updateRAP(bool boff=false);
	
  // This is the current state of the real (physical) light - the color of the
  // light that's currently on (if any) and whether the light is blinking.
  // If the state is DM_TRICOLORLIGHT_OFF, then color is meaningless since
  // no light is on.
  DMTriColorLightColor theColor;
  DMTriColorLightState theState;
	
  // Virtual lights
  DMVirtualTriColorLight virtualNormalLight;

  // For backups and restores
  DMTriColorLightColor   backupColor;
  DMTriColorLightState   backupState;
  DMVirtualTriColorLight backupVirtualNormalLight;

  // Start of Variables for In-Store Message Support
  DMMessageExt				MessageSys;
  bool						bSentOutOfServicePage, bSentRedLightPage;
  bool						bSentYellowLightPage, bSentMediaLowPage;
  CString					sOutOfServiceMessage, sRedLightMessage;
  CString					sYellowLightMessage, sMediaLowMessage;
  unsigned long			nRetryMsec;  // RFC 266954 - recycled from nTimerID
  int						m_iCount;
  DMTriColorLightColor		resendColor;
  static DMTriColorLight*	pThis;
  TClassPtr<CWinThread>		m_pPagerThread;  // RFC 266954
  // End of Variables for In-Store Message Support

public:
    void endTransaction();
};

/////////////////////////////////////////////////////////////////////////////
// Function Prototypes for In-Store Message Support
CString mergeMessages( const CString&, const PSTEXT, LPTSTR);

#endif _DMTriColorLight
