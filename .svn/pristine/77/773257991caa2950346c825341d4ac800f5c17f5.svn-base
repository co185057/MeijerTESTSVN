//////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Class Name:		DMMessageExt
//
//  Filename:		DMMessageExtension.cpp
//
//  Description:	This class encapulates the current SCOT Message Sending feature.
//					Encapulation will allow future replacement of this feature by
//					allowing an engineer to update this module and its header file.
//					Thus, reducing development time and bug inducement.
//
//	Message System:	Currently, the SCOT Platform's Paging System is being used as
//					the Message Sending solution.
//
//  Author:			John Z. Bush
//
//	Date:			March 23, 1999
//
//	Copyright:		NCR Corporation (c) 1999
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DMMessageExtension.h"
// Platform's Paging System
#include "PageClient.h"
#define COMP_ID ID_SM               // base state component
#define T_ID    _T("DMMessageExt")

// Constructor
DMMessageExt::DMMessageExt()
: bPagerInstalled(false),
  bInitialized(false),              // TAR 117805
  iReinitializeCount(0),            // TAR 117805
  iReinitializeMax(30)              // TAR 117805
{
}
DMMessageExt::~DMMessageExt() //tar 229591
{
#ifndef _UNICODE
  Uninitialize(); // Tell Pager Client Dll to Cleanup Data Capture
#endif
}
//--------------------------------------------------------------------
//
// IsAvailable
//
// This member function encapulates the current messaage platform's
// Availability function.  The platform's Availibility function is only
// called once.  After that, this function only returns the original
// status returned from the first call.
//
// Returns:	A boolean value containing the availability of the current
//			messaging system.  True, if it is available, false if it
//			is not.
//
// In-Store Message Support
//--------------------------------------------------------------------
bool DMMessageExt::IsAvailable()
{
	if(!bInitialized || iReinitializeMax == iReinitializeCount)     // TAR 117805
	{
		if(0 != PagingAvailable())
			bPagerInstalled = true;
		bInitialized = true;
        iReinitializeCount = 0;                                    // TAR 117805
	}
    else                                                           // TAR 117805
        iReinitializeCount++;                                      // TAR 117805

	return bPagerInstalled;
}

//--------------------------------------------------------------------
//
// Send
//
// This member function checks the availability of the message system
// and if it is available it then sends the message out.
//
// Returns:	A boolean value containing the status of whether the message
//			was sent or not.
//
// In-Store Message Support
//--------------------------------------------------------------------
bool DMMessageExt::Send( LPCTSTR Message)
{
	trace(L7, _T("+rp.SetPagerEvent"));
	bool bStatus = false;
#ifndef _UNICODE
	if(!bInitialized)
		IsAvailable();

	if(bPagerInstalled)
    {                                                                          // TAR 117805
		if(0==SendPage( (LPTSTR) Message, 0))
			bStatus = true;
		    
		else if(0==SendPage( (LPTSTR)Message, 0))   // if First Attempt fails, // TAR 117805
			bStatus = true;                         // resend once more        // TAR 117805
    }
	rp.SetPagerEvent(bStatus);
	trace(L7, _T("-rp.SetPagerEvent<%d>"),bStatus);
//    if (bStatus == true)		// TAR 232346 We don't need this check because if the pager communication 
								// is down, we need to reset bPagerInstalled to false
    bPagerInstalled = bStatus; // Indicate that the pager system whether has failed or not.   // TAR 117805
#endif
	return bStatus;
}
