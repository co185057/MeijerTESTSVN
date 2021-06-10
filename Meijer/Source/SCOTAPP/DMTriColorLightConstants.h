#ifndef _DMTRICOLORLIGHT_CONSTANTS_H
#define _DMTRICOLORLIGHT_CONSTANTS_H

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

#endif _DMTRICOLORLIGHT_CONSTANTS_H
