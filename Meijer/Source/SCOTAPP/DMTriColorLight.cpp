//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:  DMTriColorLight.cpp
//
//  Description: see h file
//
//	Copyright:		NCR Corporation (c) 1999
//
//////////////////////////////////////////////////////////////////////////////
// Tar 234730 - prevent some of the tricolorlight & pager problems from the dual request logic;
// still allowing the use of DM_TRICOLORLIGHT_SECURITYREQUEST by SSF for backward compatability;
// changing our internal logic to process it the same as DM_TRICOLORLIGHT_NORMALREQUEST
// this will simplify this logic and hopefully pave the way for future enhancements
// still keeping the virtual class, using virtualNormalLight, another virtual instance may be
// used in the future but be careful to not break the continued use of SECURITYREQUEST
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "LaneStateData.h"

#include "Common.h"

#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMTriColorLight")

#include "DMTriColorLight.h"

DMTriColorLight * DMTriColorLight::pThis = NULL;

//--------------------------------
// DMVirtualTriColorLight
//--------------------------------

// Constructor
DMVirtualTriColorLight::DMVirtualTriColorLight()
: redState(DM_TRICOLORLIGHT_OFF),
  yellowState(DM_TRICOLORLIGHT_OFF),
  greenState(DM_TRICOLORLIGHT_OFF)
{
}


//--------------------------------
// DMTriColorLight
//--------------------------------

// Constructor
DMTriColorLight::DMTriColorLight()
: theColor(DM_TRICOLORLIGHT_GREEN),
  theState(DM_TRICOLORLIGHT_OFF),
  bSentRedLightPage(false),			// Start In-Store Message Support
  bSentYellowLightPage(false),
  bSentMediaLowPage(false),
  bSentOutOfServicePage(false),
  m_pPagerThread(NULL),
  resendColor((DMTriColorLightColor)0),
  nRetryMsec(0),
  m_iCount(0)						// End  In-Store Message Support
{
	// Start In-Store Message Support
    pThis = this;    // Set up the static "this" pointer.
	// End  In-Store Message Support
}

// Destructor
DMTriColorLight::~DMTriColorLight()
{
	DWORD dwCount;
	if (m_pPagerThread != NULL)
	{
		try
		{
			// Suspend the thread so it can do no harm while we delete it.
			dwCount = SuspendThread(m_pPagerThread->m_hThread);
			ASSERT(dwCount != (DWORD)-1);
			m_pPagerThread = NULL;
		}
		catch (...)
		{
			// I really don't care, just don't want to die
		}
	}
}

//-------------------------------------------------------------------------
//
// set
//
// This method does basically 3 things: converts from intuitive versions
// of color and state to ncrdevmgr versions, sets the light, and saves
// the light state for future sets.
//
// The separate lights have a priority order: red/yellow/green.  This
// determines what happens when a light is set on if another light is already
// on.  e.g. 1- green is turned on, 2- red is turned on.  Red is higher
// priority, so green goes off.  3- green is turned on.  Green is lower
// priority so no change.  4- Red is turned off.  Green must now be turned
// back on since it was on in step 1.
//
//-------------------------------------------------------------------------
void DMTriColorLight::set(const DMTriColorLightColor		color,
                          const DMTriColorLightState		state,
                          const DMTriColorLightRequestType	request,
								CDevMgr&					dm,
								int							index)
{
    // TAR 115242
    // if a page has been sent and its corresponding light is being turned
    // off then kill the resend timer if it has been started.
    if (state == DM_TRICOLORLIGHT_OFF)
    {
        bool bStopResendTimer = false;
        if (bSentRedLightPage && color == DM_TRICOLORLIGHT_RED)
        {
            if (virtualNormalLight.redState != DM_TRICOLORLIGHT_OFF)
            {
                bSentRedLightPage = false;
                bStopResendTimer = true;
            }
        }
        if (bSentYellowLightPage && color == DM_TRICOLORLIGHT_YELLOW)
        {
            if (virtualNormalLight.yellowState != DM_TRICOLORLIGHT_OFF)
            {
                bSentYellowLightPage = false;
                bStopResendTimer = true;
            }
        }
        if (bSentOutOfServicePage && color == DM_TRICOLORLIGHT_RED)
        {
            if (virtualNormalLight.redState != DM_TRICOLORLIGHT_OFF)
            {
                bSentOutOfServicePage = false;
                bStopResendTimer = true;
            }
        }
        if (bSentMediaLowPage && color == DM_TRICOLORLIGHT_GREEN)
        {
            if (virtualNormalLight.greenState != DM_TRICOLORLIGHT_OFF)
            {
				// CMal/DM102303 - dont reset MediaLowPage if we are using the LimitTrans SCOTopt
				//     as this messes with transaction count.  TAR 216233
				if (co.getGreenFlashingLimitTrans() == 0)
					bSentMediaLowPage = false;
                bStopResendTimer = true;
            }
        }
        if (bStopResendTimer)
        {
           nRetryMsec = 0;
        }
    }


    if (color == DM_TRICOLORLIGHT_GREEN)
      virtualNormalLight.greenState = state;
    else if (color == DM_TRICOLORLIGHT_YELLOW)
      virtualNormalLight.yellowState = state;
    else if (color == DM_TRICOLORLIGHT_RED)
      virtualNormalLight.redState = state;
	
	try
	{
		// Although multiple lights may be on internally, only one physical light
		// can be on.  Without bothering to figure out which physical light is on
		// right now, turn them all off and then turn on the highest priority one
		// that is on internally, if any.
		dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS1, DM_MISC_STATE_OFF);	//red
		dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS2, DM_MISC_STATE_OFF);	//yellow
		dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS3, DM_MISC_STATE_OFF);	//green
		

        // NextGenUI Support
        CLaneStateData laneStateData(ps.GetPSXObject());

		// To set the real light, check the virtual lights in priority order:
		// first red, then yellow, then green.
		if (virtualNormalLight.redState)
		{
			theColor = DM_TRICOLORLIGHT_RED;
			theState = virtualNormalLight.redState;
			dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS1,
				convertToDevMgrState(theState)); // red, with real DM state name
			
			// In-Store Message Support
			processPages( theColor, theState);  // This should only be called for non-off states
			
			// Remote Approval Support
			trace(L7, _T("this tLightState, State=%d"), theState);
			ra.SetLightState(0, 0, convertToDevMgrState(theState));
			trace(L7, _T("-ra.SetLightState"));

            // NextGenUI Support
            laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("immediate")));
		}
		else if (virtualNormalLight.yellowState)
		{
			theColor = DM_TRICOLORLIGHT_YELLOW;
			theState = virtualNormalLight.yellowState;
			dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS2,
				convertToDevMgrState(theState)); // yellow, with real DM state name
			
			// In-Store Message Support
			processPages( theColor, theState);  // This should only be called for non-off states
			
			// Remote Approval Support
			trace(L7, _T("+ra.SetLightState, State=%d"), theState);
			ra.SetLightState(0, convertToDevMgrState(theState), 0);
			trace(L7, _T("-ra.SetLightState"));

            // NextGenUI Support
            laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("delayed")));
		}
		else if (virtualNormalLight.greenState)
		{
			theColor = DM_TRICOLORLIGHT_GREEN;
			theState = virtualNormalLight.greenState;
			dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS3,
				convertToDevMgrState(theState)); // green, with real DM state name
			
			// In-Store Message Support
			processPages( theColor, theState);  // This should only be called for non-off states
			
			// Remote Approval Support
			trace(L7, _T("+ra.SetLightState, State=%d"), theState);
			ra.SetLightState(convertToDevMgrState(theState), 0, 0);
			trace(L7, _T("-ra.SetLightState"));

            // NextGenUI Support
            laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("none")));
		}
	}
	catch(CException *)
	{
		trace(L4, _T("ERROR:  DMTriColorLight::set(); color: %d; state: %d"),
			color, state);
		
		updateRAP();	// Update state of RAP button.
		throw;			// Pass the exception up.
	}
}

//-------------------------------------------------
// off
//
// Turn physical light and all virtual lights off
//
//-------------------------------------------------
void DMTriColorLight::off(CDevMgr& dm, int index)
{
	try
	{
		// Real lights
		dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS1, DM_MISC_STATE_OFF);	//red
		dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS2, DM_MISC_STATE_OFF);	//yellow
		dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS3, DM_MISC_STATE_OFF);	//green
		
		// Internal state
		theState = DM_TRICOLORLIGHT_OFF;
		// don't worry about theColor, meaningless at this point.
		
		// Virtual lights
		virtualNormalLight.greenState = DM_TRICOLORLIGHT_OFF;
		virtualNormalLight.yellowState = DM_TRICOLORLIGHT_OFF;
		virtualNormalLight.redState = DM_TRICOLORLIGHT_OFF;
		
		// In-Store Message Support
		ResetPages();
		
		// Remote Approval Support
		trace(L7, _T("+ra.SetLightState"));
		ra.SetLightState(0, 0, 0);
		trace(L7, _T("-ra.SetLightState"));

        // NextGenUI Support
        CLaneStateData laneStateData(ps.GetPSXObject());
        laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("off")));
	}
	catch(CException *)
	{
		trace(L4, _T("ERROR:  DMTriColorLight::off()"));	
		updateRAP(true);	// Update state of RAP button.	
		throw;				// Pass the exception up.
	}
}

//-------------------------------------------------
// backup
//
// Save the current state of the TriColorLight
//
//-------------------------------------------------
void DMTriColorLight::backup()
{
  trace(L6, _T("Backing up lane light color %d"), theColor);
  backupColor = theColor;
  backupState = theState;
  backupVirtualNormalLight = virtualNormalLight;
}

//-----------------------------------------------------------
// restore
//
// Restore the last backed up state of the TriColorLight
//
//-----------------------------------------------------------
void DMTriColorLight::restore(CDevMgr& dm, int index)
{
  // Restore regular and virtual light values
  theColor = backupColor;
  theState = backupState;
  virtualNormalLight = backupVirtualNormalLight;

  try
  {
	  // First turn off all the lights
	  dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS1, DM_MISC_STATE_OFF);	//red
	  dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS2, DM_MISC_STATE_OFF);	//yellow
	  dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS3, DM_MISC_STATE_OFF);	//green
	  
        // NextGenUI Support
        CLaneStateData laneStateData(ps.GetPSXObject());

	  // Turn on the real light
	  if (theColor == DM_TRICOLORLIGHT_RED)
	  {
		  dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS1, theState);	// red
		  
		  // Remote Approval Support
		  trace(L7, _T("+ra.SetLightState, State=%d"), theState);
		  ra.SetLightState(0, 0, convertToDevMgrState(theState));
		  trace(L7, _T("-ra.SetLightState"));

          // NextGenUI Support
          laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("immediate")));
	  }
	  else if (theColor == DM_TRICOLORLIGHT_YELLOW)
	  {
		  dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS2, theState);	// yellow
		  
		  // Remote Approval Support
		  trace(L7, _T("+ra.SetLightState, State=%d"), theState);
		  ra.SetLightState(0, convertToDevMgrState(theState), 0);
		  trace(L7, _T("-ra.SetLightState"));

          // NextGenUI Support
          laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("delayed")));
	  }  
	  else if (theColor == DM_TRICOLORLIGHT_GREEN)
	  {
		  dm.MiscSetIndicator(index, DM_MISC_REMOTE_STATUS3, theState);	// green
		  
		  // Remote Approval Support
		  trace(L7, _T("+ra.SetLightState, State=%d"), theState);
		  ra.SetLightState(convertToDevMgrState(theState), 0, 0);
		  trace(L7, _T("-ra.SetLightState"));

          // NextGenUI Support
          laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("none")));
	  }
  }
  catch(CException *)
  {
	  trace(L4, _T("ERROR:  DMTriColorLight::restore(); theColor: %d; theState: %d"),
		  theColor, theState);
	  
	  updateRAP();	// Update state of RAP button.		
	  throw;			// Pass the exception up.
  }
}
//--------------------------------------------------------------------
// 
// endTransaction
//
// If the skip transaction is used for paging, this function marks the
// end of the transaction
//--------------------------------------------------------------------
void DMTriColorLight::endTransaction()
{
  // Additionally for Media Low Pages reset the corresponding PageSent variable
  // only if GreenFlashingLimitTime is zero and if the transaction count is
  // greater than GreenFlashingLimitTrans else increament transaction count
  if (0 == co.getGreenFlashingLimitTime())
  {
	  // CMal/DM 091903: need to access the SM properties 			if (fInSignedOn || fInSignedOff || fSAInTransaction)

	  if (m_iCount < co.getGreenFlashingLimitTrans())
		  m_iCount++;
	  else
	  {
		  bSentMediaLowPage = false;
		  m_iCount=0;
	  }
  }

}
//--------------------------------------------------------------------
//
// convertToDevMgrState
//
// Convert the state to the real Device Manager version
//--------------------------------------------------------------------
long DMTriColorLight::convertToDevMgrState(DMTriColorLightState state)
{
  switch (state)
  {
	case DM_TRICOLORLIGHT_OFF:
		return DM_MISC_STATE_OFF;
	case DM_TRICOLORLIGHT_ON:
		return DM_MISC_STATE_ON;
	case DM_TRICOLORLIGHT_BLINK_QUARTERHZ:
		return DM_MISC_STATE_QUARTER_HZ;
	case DM_TRICOLORLIGHT_BLINK_HALFHZ:
		return DM_MISC_STATE_HALF_HZ;
	case DM_TRICOLORLIGHT_BLINK_1HZ:
		return DM_MISC_STATE_ONE_HZ;
	case DM_TRICOLORLIGHT_BLINK_2HZ:
		return DM_MISC_STATE_TWO_HZ;
	case DM_TRICOLORLIGHT_BLINK_4HZ:
		return DM_MISC_STATE_FOUR_HZ;
  default:
    return DM_MISC_STATE_OFF;			  // shouldn't get here
  }
}

/**
 * Function:		updateRAP
 * Purpose:			Updates the state of the RAP button according to the
 *					virtual light settings.
 * Parameters:		boff - If true, sets RAP button to gray.
 *						   Otherwise, the button color is set to the color
 *						   of the highest priority virtual light.
 * Returns:			true on success.  false otherwise.
 * Calls:			
 * Side Effects:
 * Notes:			Created for JPY TAR 328090.  Japan requested the ability
 *					to prevent the appearance of a trilight error screen within
 *					a transaction.
 *
 *					This was accomplished in SSF, but a side effect is that the 
 *					RAP buttons are not updated correctly.  Device manager 
 *					throws an exception at dm.MiscSetIndicator() and as a 
 *					result ra.SetLightState() is never executed.
 *					A try/catch was added to update the RAP buttons if a 
 *					trilight error causes an exception to be thrown.  
 *					This was also added to DMTriColorLight::off() and 
 *					DMTriColorLight::restore().
 */
bool DMTriColorLight::updateRAP(bool boff)
{
	trace(L6, _T("+updateRAP(%d)"), boff);

	try		// ra.SetLightState can throw an exception.
	{		

        // NextGenUI Support
        CLaneStateData laneStateData(ps.GetPSXObject());

		if(boff)	// Turn off everything.
		{  
			ra.SetLightState(0, 0, 0);

            // NextGenUI Support
            laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("off")));

			trace(L7, _T("-updateRAP"));
			return true;
		}
		
		if (virtualNormalLight.redState)
		{
			trace(L7, _T("Setting RAP light state to RED"));
			ra.SetLightState(0, 0, convertToDevMgrState(virtualNormalLight.redState));

            // NextGenUI Support
            laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("immediate")));
		}
		else if (virtualNormalLight.yellowState)
		{
			trace(L7, _T("Setting RAP light state to YELLOW"));
			ra.SetLightState(0, convertToDevMgrState(virtualNormalLight.yellowState), 0);

            // NextGenUI Support
            laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("delayed")));
		}
		else if (virtualNormalLight.greenState)
		{
			trace(L7, _T("Setting RAP light state to GREEN"));
			ra.SetLightState(convertToDevMgrState(virtualNormalLight.greenState), 0, 0);

            // NextGenUI Support
            laneStateData.SendNextGenUIData(_T("NextGenUIInterventionMode"), CString(_T("none")));
		}
	}
	catch(CException *e)
	{
		trace(L6, _T("-updateRAP FAILED"));
		e->Delete();
		return false;
	}
	
	trace(L6, _T("-updateRAP"));
	return true;
}


// Start of function definitions for In-Store Message Support

//--------------------------------------------------------------------
//
// processPages
//
// Starts sendPage() in a separate thread if a page is to be sent
//
// Returns:	VOID
//
// In-Store Message Support
//
// NOTE:  the parameters passed in are kept globally as theColor & theState
//--------------------------------------------------------------------
void DMTriColorLight::processPages( DMTriColorLightColor color,
								    DMTriColorLightState state)
{
	ASSERT( DM_TRICOLORLIGHT_OFF != state); // Verification that this function
											// is not getting called for offs
	if(DM_TRICOLORLIGHT_OFF != state)	// Tar 234730 should never page on OFF state
	{
      switch (color)
      {  // RFC 266954 - Ensure auxiliary thread started only if a page will be sent
      case DM_TRICOLORLIGHT_RED:
         if (state == DM_TRICOLORLIGHT_ON)
         {
            if (   !co.getOutOfServiceSendPage()
                || bSentOutOfServicePage)
            {
               return;
            }
         }
         else
         {
            if (   !co.getRedSendPage()
                || bSentRedLightPage)
            {
               return;
            }
         }
         break;
      case DM_TRICOLORLIGHT_YELLOW:
         if (   !co.getYellowSendPage()
             || bSentYellowLightPage)
         {
            return;
         }
         break;
      case DM_TRICOLORLIGHT_GREEN:
         if (   co.getGreenFlashingSendPage()
             && !bSentMediaLowPage
             && (state == DM_TRICOLORLIGHT_BLINK_1HZ))
         {
            // Page Limit Functionality in main thread; doesn't work in auxiliary thread
            if( 0 != co.getGreenFlashingLimitTime())
            {
               ::SetTimer( NULL, 0, (co.getGreenFlashingLimitTime() * 60000),
                           pageLimitTimerProc);
            }
         }
         else
         {
            return;
         }
         break;
      default:
         return;
      }
      DWORD dwCount;
      if (m_pPagerThread != NULL)
      {
         try
         {
            // Suspend the thread so it can do no harm while we delete it.
            dwCount = SuspendThread(m_pPagerThread->m_hThread);
            ASSERT(dwCount != (DWORD)-1);
            m_pPagerThread = NULL;
         }
         catch (...)
         {
            // I really don't care, just don't want to die
         }
      }
      nRetryMsec = 0;
      m_pPagerThread = AfxBeginThread((AFX_THREADPROC)sendPageThread, 
                                      this, THREAD_PRIORITY_NORMAL, 0, 0);
   }
}

//--------------------------------------------------------------------
//
// sendPageThread
//
// Sends the page in this auxiliary thread and waits for the associated
// retry timer to go off.
//
// Returns:	0 as thread exit status
//
// In-Store Message Support: runs in auxiliary thread
//--------------------------------------------------------------------
UINT DMTriColorLight::sendPageThread(LPVOID *lpParam)
{
   pThis->sendPage();
   unsigned long lRetryTimer = pThis->nRetryMsec;
   while (lRetryTimer > 0)
   {
      Sleep(lRetryTimer);
      pThis->resendPage();
      lRetryTimer = pThis->nRetryMsec;
   }
   pThis->m_pPagerThread = NULL;
   AfxEndThread(0, TRUE);
   return 0;
}

//--------------------------------------------------------------------
//
// sendPage
//
// For each Tri-Light Color, this member function determines if an
// In-Store page should be sent based on the terminal's configuration
// and the current status of the paging system.
//
// Returns:	VOID
//
// In-Store Message Support: runs in auxiliary thread
//--------------------------------------------------------------------
void DMTriColorLight::sendPage()
{
#if _DEBUG
		if (1)  // CMal102303 - for debug only , go thru Send Pages code
#else 
	  if( MessageSys.IsAvailable()) // NOTE: CMal102303 if a pager is available
#endif
   {
		DMTriColorLightColor color = getColor();
		DMTriColorLightState state = getState();
		switch( color )
		{
		case DM_TRICOLORLIGHT_RED:
			if (state == DM_TRICOLORLIGHT_ON)
			{
				verifyAndSendPage( co.getOutOfServiceSendPage(), bSentOutOfServicePage,
								   sOutOfServiceMessage, co.getOutOfServiceElapseTime(),
								   color);
			}
			else
			{
				verifyAndSendPage( co.getRedSendPage(), bSentRedLightPage,
								   sRedLightMessage, co.getRedElapseTime(),
								   color);
			}
			break;
		case DM_TRICOLORLIGHT_YELLOW:
				verifyAndSendPage( co.getYellowSendPage(), bSentYellowLightPage,
								   sYellowLightMessage, co.getYellowElapseTime(),
								   color);
			break;
		case DM_TRICOLORLIGHT_GREEN:
			if ( co.getGreenFlashingSendPage()			  // Send a MediaLow page, if SendPage is ok
				&& !bSentMediaLowPage					  // & a page has not already been sent
				&& (state == DM_TRICOLORLIGHT_BLINK_1HZ)) // & its a Flashing Green Light
			{
				ASSERT( dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow()); // if this ASSERT fails, a Flashing Green
													  // light was turned on w/o media being low
				MessageSys.Send( sMediaLowMessage);
				bSentMediaLowPage=true;
		        m_iCount=0;

				//tar180336: log the message into the event log
				ScotError(INFO,SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SEND_PAGE,
              _T("%d|%s"), bSentMediaLowPage, sMediaLowMessage);
				

				// Resend Page Functionality
				setupPageResend( co.getGreenFlashingElapseTime(), color);
			}
			break;
		default:
			ASSERT(false); // Currently, there are only three lights
			break;
		}
	}
}




//--------------------------------------------------------------------
//
// setupPageResend
//
// This member function establishes that the Repage Timer has been
// killed and than creates a new Timer based on the value of the 
// elapse time parameter for the current tri-light status.
//
// Returns:	VOID
//
// In-Store Message Support: runs in auxiliary thread
//--------------------------------------------------------------------
void DMTriColorLight::setupPageResend( const int ElapseTime, DMTriColorLightColor color)
{
	ASSERT( DM_TRICOLORLIGHT_GREEN == color ||	// Verification that this function
			DM_TRICOLORLIGHT_YELLOW == color ||	// is not getting called with an
			DM_TRICOLORLIGHT_RED == color);		// invalid color

	if( 0 != ElapseTime)
	{
		pThis->resendColor = color;  // store which tri-light is resending its page
		pThis->nRetryMsec = ElapseTime * 1000;
	}
}
 
//--------------------------------------------------------------------
//
// pagerInService
//
// This function checks the availability of the SCOT Platform's Paging
// System and records an event in the applicaiton log if paging is not
// available at the start of the application.  The function returns a
// boolean for the current status:  true, if available and false, if
// it is not.
//
// Returns:	A boolean value contiaing the current availability of the 
//			Message System.  True if it is available, false if not.
//
// In-Store Message Support
//--------------------------------------------------------------------
bool DMTriColorLight::pagerInService()
{
    bool bStatus = MessageSys.IsAvailable();
    if(bStatus)
    {
        ScotError(INFO,SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_PAGER_INITIALIZED);
    }
    else
    {
       ScotError(INFO,SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_PAGER_UNAVAILABLE);
    }
	return bStatus;
}

//--------------------------------------------------------------------
//
// pageMessageFormatter
//
// This function initializes "page message" class members with strings
// formatted from text supplied from scotmsgs.dat.  It merges a lane
// information string with a string for a particular tri-light status.
// The function will replace '##' within the lane information string
// with the current terminal's lane number.  If the function fails to
// get the message texts from scotmsgs.dat, it will use the default
// message defined in DMTriColorLight.h.
//
// Returns:	VOID
//
// In-Store Message Support
//--------------------------------------------------------------------
void DMTriColorLight::pageMessageFormatter()
{
	// Get lane information format or use the default
	CString LaneInfo = ps.GetPSText( MSG_LANE_NUMBER);
	if( LaneInfo.IsEmpty() || (0==LaneInfo.Find(_T('?'))))
		LaneInfo = DEFAULT_LANE_MESSAGE;

	// Insert the terminal number into the lane information format
	int nInsertAt = LaneInfo.Find(_T("##"));
	if( nInsertAt != -1)
	{
		// if ## was found, replace ## with terminal number 
		CString sLeft = LaneInfo.Left( nInsertAt);
		CString sTerminal;
		sTerminal.Format(_T("%2li"), TBGetTerminalNumber());
		CString sRight = LaneInfo.Mid( nInsertAt + 2 );
		LaneInfo = sLeft;
		LaneInfo += sTerminal;
		LaneInfo += sRight;
	}

	// Merge lane information and Red message into Red Page Message
	sRedLightMessage = mergeMessages( LaneInfo, MSG_RED, DEFAULT_RED_MESSAGE);

	// Merge lane information and Yellow message into Yellow Page Message
	sYellowLightMessage = mergeMessages( LaneInfo, MSG_YELLOW, DEFAULT_YELLOW_MESSAGE);

	// Merge lane information and MediaLow message into MediaLow Page Message
	sMediaLowMessage = mergeMessages( LaneInfo, MSG_MEDIA_LOW, DEFAULT_MEDIA_LOW_MESSAGE);

	// Merge lane information and Out of Serivce message into Out of Serivce Page Message
	sOutOfServiceMessage = mergeMessages( LaneInfo, MSG_OUTOFSERVICE, DEFAULT_OUT_OF_SERVICE_MESSAGE);
}

//--------------------------------------------------------------------
//
// resendPage
//
// This member function is called when the Resend Pager timer  
// expires.  It is continuously called after each period
// of a configurable amount of elapsed time. It determines which
// tri-light status current needs its page resent and then resends
// the page on the auxiliary thread.
//
// Returns:	VOID
//
// In-Store Message Support: runs in auxiliary thread
//--------------------------------------------------------------------
void DMTriColorLight::resendPage()
{   
    if (NULL != (DMTriColorLight *) pThis)  
    {
		switch( pThis->resendColor)
		{
		case DM_TRICOLORLIGHT_RED:
			if(pThis->bSentRedLightPage)
				pThis->MessageSys.Send( pThis->sRedLightMessage);
			else if(pThis->bSentOutOfServicePage)
				pThis->MessageSys.Send( pThis->sOutOfServiceMessage);
			else                       // Nothing to do
				pThis->nRetryMsec = 0;  // Force end of auxiliary thread
			break;
		case DM_TRICOLORLIGHT_YELLOW:
			if(pThis->bSentYellowLightPage)
				pThis->MessageSys.Send( pThis->sYellowLightMessage);
			else                       // Nothing to do
				pThis->nRetryMsec = 0;  // Force end of auxiliary thread
			break;
		case DM_TRICOLORLIGHT_GREEN:
			if(pThis->bSentMediaLowPage)
				pThis->MessageSys.Send( pThis->sMediaLowMessage);
			else                       // Nothing to do
				pThis->nRetryMsec = 0;  // Force end of auxiliary thread
			break;
		default:
			ASSERT(false); // Currently, there are only three lights
			break;
		}
    }
}                                           


//--------------------------------------------------------------------
//
// pageLimitTimerProc
//
// This static member function is the callback function for the
// Media Low Page Limit Timer.  It is continuosly called after each period
// of a configurable amount of elapsed time. It limits (essentially blocks)
// when a Media Low page can be sent.  A Media Low page can not be sent if
// the elapse time greater than zero and it has not expired.
//
// Returns:	VOID
//
// In-Store Message Support
//--------------------------------------------------------------------
void CALLBACK EXPORT DMTriColorLight::pageLimitTimerProc( HWND hwnd,
														  UINT msg,
														  UINT idTimer,
														  DWORD dwTime)
{   
    if (NULL != (DMTriColorLight *) pThis)  
    {
		::KillTimer( hwnd, idTimer);
		pThis->bSentMediaLowPage = false;
    }
}

//--------------------------------------------------------------------
//
// Initialize
//
// This member function initializes the In-Store Message System by
// checking the availability of the Message System: currently the
// SCOT Platform's Paging System.  The initialization process is
// continued by formatting the messages that will be sent.
//
// Returns:	VOID
//
// In-Store Message Support
//--------------------------------------------------------------------
void DMTriColorLight::Initialize()
{
    MessageSys.SetReinitializeMaximum(co.getAvailabilityCount());  // TAR 117805
	pagerInService();  // Check availabity of Paging System
	pageMessageFormatter();	// Initializes "page message" class members from
							// text supplied by scotmsgs.dat or with default strings 
}

//--------------------------------------------------------------------
//
// verifyAndSendPage
//
// This member function verifies that a message should be sent and then
// sends the message.  If the Send function fails, then this function
// will attempt to resend the message once.  It also sets up a Timer and
// Callback function for resending a message to the Message System.
//
// Returns:	A boolean value containing the status of sending message.
//			True, if the message was sent, false if it was not.
//
// In-Store Message Support: runs in auxiliary thread
//--------------------------------------------------------------------
bool DMTriColorLight::verifyAndSendPage( const bool bSendPage, bool& bPageStatus,
										 LPCTSTR Message, const int ElapseTime,
										 DMTriColorLightColor color)
{
	bool	bSentStatus=false;

	if ( bSendPage			// Send an "Out of Service" page,
		&& !bPageStatus)    //  if SendPage is ok & a page has
							//  not already been sent
	{		
		bSentStatus = MessageSys.Send( Message);
        // TAR 117805, removal of the send attempt at sending a page
        
        ScotError(INFO,SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SEND_PAGE,
              _T("%d|%s"), bSentStatus, Message);

		// Resend Page Functionallity
		setupPageResend( ElapseTime, color);

		bPageStatus = true;
	}

	return bSentStatus;
}

//--------------------------------------------------------------------
//
// ResetPages
//
// This member function resets the page sent flags and the page resend
// timer.  The Media Low page sent flag is reset based on configuration
// options.  This function also maintains the transaction count for
// the Media Low Page Transaction Limit.
//
// Returns:	VOID
//
// In-Store Message Support
//--------------------------------------------------------------------
void DMTriColorLight::ResetPages()
{
  try
  {
    // For Red and Yellow Pages reset the corresponding
    // PageSent variable at the end of every transaction
    bSentRedLightPage = false;			
    bSentYellowLightPage = false;
    bSentOutOfServicePage = false;			

    // For Media Low Pages reset the corresponding PageSent variable only if
    // GreenFlashingLimitTime and GreenFlashingLimitTrans are both zero
    if ((0 == co.getGreenFlashingLimitTime()) && (0 == co.getGreenFlashingLimitTrans()))
	    bSentMediaLowPage = false;
    // NOTE: The Media Low PageSent variable will also be reset by a call to the 
    // pageLimitTimerProc, which is called after the amount of GreenFlashingLimitTime
    // has elapsed.

    // Kill the Resend Page Timer if the timer has been activated
    // and if the all the SentPage variables have been reset.
    // Note: CMal/DM  This is also called when a tranaction is ended.  When the 
    // green light is turned off BECAUSE there is not a media low condition.
  if( !bSentRedLightPage && !bSentOutOfServicePage
     && !bSentYellowLightPage && !bSentMediaLowPage)
  {
     nRetryMsec = 0;
	    }
  }
  catch(...)
  {
		ASSERT(0);
		trace(L6, _T("Unexpected exception caught in DMTriColorLight::ResetPages()"));
  }
}

//--------------------------------------------------------------------
//
// mergeMessages
//
// This fumction merges the lane information string with the current
// color's Page Message or its default string.
//
// Returns:	A CString object containing the merged message
//
// In-Store Message Support
//--------------------------------------------------------------------
CString mergeMessages( const CString& lane, const PSTEXT MSG, LPTSTR DefaultMSG)
{
	CString sResult = lane;
	sResult += _T(" ");
	CString TempMessage = ps.GetPSText( MSG);
	if( TempMessage.IsEmpty() || (0==TempMessage.Find(_T('?'))))
		sResult += DefaultMSG;
	else
		sResult += TempMessage;

	return sResult;
}

// End of function definitions for In-Store Message Support
