/////////////////////////////////////////////////////////////////////////////
//
// msr.cpp
//
//   Implementation of IDispatch wrapper class for CMSR
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Fastlane/4.0 Patch D/Development/Core/Source/SCOTAPP/dmx/MSRWrapper.cpp $
//
// $Revision: 4 $
//
// $Date: 6/14/05 3:42p $
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 96/11/15 Initial version.                                    C. Light
//
/////////////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "MsrWrapper.h"
#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMX")

/////////////////////////////////////////////////////////////////////////////
// CMSR



BOOL CMSRWrapper::Create( 
   CWnd* pParentWnd, 
   UINT nID )
	{
   return TOPOSMsr<CGenericOposIF>::Create(NULL, 0, CRect(0,0,0,0), pParentWnd, nID );
   }


/////////////////////////////////////////////////////////////////////////////
// CMSR properties



/////////////////////////////////////////////////////////////////////////////
// CMSR operations

/* virtual */
void CMSRWrapper::OnOposDataEvent( 
   long  nStatus ) 
   {
   long nTrackToRead = GetTracksToRead();
   CString sData;
   sData.Format( _T("%s%c%s%c%s"), 
      nTrackToRead & MSR_TR_1 ? GetTrack1Data() : _T(""), DM_FS, 
      nTrackToRead & MSR_TR_2 ? GetTrack2Data() : _T(""), DM_FS, 
      nTrackToRead & MSR_TR_3 ? GetTrack3Data() : _T("") );
   PostDmEventToApplication( DMCLASS_MSR, m_nDeviceId, sData );
   SetDataEventEnabled( TRUE );
   return ;
   }

/* virtual */
void CMSRWrapper::OnOposErrorEvent( 
	long  nResult, 
	long  nRcEx )
	{
	switch (nResult)  //Tar 236723
	{
		case OPOS_E_OFFLINE:
		case OPOS_E_FAILURE:
			PostDmEventToApplication( DM_ERROR, DMCLASS_MSR, m_nDeviceId, nResult );
			return;
	}
	switch ( nRcEx )
	{
		case NCR_READ_ERROR:
		case NCR_DEVICE_INOP:
		case NCR_IOE_DEVICE_INOP:
		case NCR_CARD_JAM:
		case NCR_SHUTTER_FAILURE:
		case NCR_NO_CARD:  //SR672
		case MCR_GET_TEXTLEN_ERROR:
		case MCR_GET_CRCC_ERROR:
		//case OPOS_E_OFFLINE:
		//case OPOS_E_FAILURE:
			PostDmEventToApplication( DM_ERROR, DMCLASS_MSR, m_nDeviceId, nRcEx );
			return;
		case NCR_IOE_EJECT_CAPTURED:
			return;
	}
	BOOL bDataEventFired = FALSE;
	if (nResult == OPOS_E_EXTENDED
      && GetErrorReportingType()== MSR_ERT_TRACK )
	  {
      bDataEventFired = (nRcEx & 0x00FF0000 ) == 0 //TAR198797: taking nRcEx as account too
         || (nRcEx & 0x0000FF00 ) == 0
         || (nRcEx & 0x000000FF ) == 0;
	  if ( bDataEventFired != FALSE )
         OnOposDataEvent(  0 );
	  else
         nRcEx = OPOS_EMSR_PARITY;	// TAR 198797-2 Simulate an error - device error, system message
	  }
	if ( bDataEventFired == FALSE )
      {
      long nStatus = 0;
      switch ( nRcEx )
        {
        case OPOS_EMSR_START:
        case OPOS_EMSR_END:
        case OPOS_EMSR_PARITY:
        case OPOS_EMSR_LRC:
            //nStatus = DM_ERROR_MSR_READING;
			nStatus = nRcEx;  //SR672
            break;
        default:
			//nStatus = MapOposErrorToDM( nResult ); 
			nStatus =  nResult ;  //SR672
			trace(L6, _T("DMX:OnOposErrorEvent??? MSR-")
				_T("%d (%d,%d) Posting Error event"), m_nDeviceId, nResult, nRcEx );
			break;
        }
      PostDmEventToApplication( DM_ERROR, DMCLASS_MSR, m_nDeviceId, nStatus );
      }
	return;
	}

/* virtual */
void CMSRWrapper::OnOposStatusEvent( 
  long nStatus )
  {
	switch ( nStatus ) 
	{
		case NCR_IOE_SHUTTER_JAMMED_CLOSE:
		case NCR_IOE_INVALID_CARD_INSERTED:
		case NCR_IOE_CARD_REMOVED:
		case MCR_GET_RESPONSE_TIMEOUT:
		case MCR_INITIALIZE_CMD_ERROR:
		case MCR_ENABLE_CMD_ERROR:
		case MCR_DISABLE_CMD_ERROR:
		case MCR_MAGTRACKREAD_CMD_ERROR:
		case MCR_STATUS_CMD_ERROR:
		case MCR_EJECT_CMD_ERROR:
		case MCR_CAPTURE_CMD_ERROR:
		case MCR_REVISION_CMD_ERROR:
		case MCR_RETRIEVE_CMD_ERROR:
			PostDmEventToApplication( DM_STATUS, DMCLASS_MSR, m_nDeviceId, nStatus );
			break;
		case NCR_IOE_CARD_DETECTED_ENTERING:
		case NCR_IOE_CARD_INSERTED_STAGED:
		case MCR_GET_BACK_ONLINE:
		default:
			break;
	}
  return;
  }

/* virtual */
long CMSRWrapper::DoOneTimeInitialization(
   void)
   {
   SetErrorReportingType(MSR_ERT_TRACK);
   return OPOS_SUCCESS;
   }
