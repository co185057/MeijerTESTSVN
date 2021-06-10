//////////////////////////////////////////////////////////////////////////////////
//
// micr.cpp
//
//   Implementation of IDispatch wrapper class for CMICRWrapper
//
//   Copyright (c) 2003 NCR.  All rights reserved.
//
// $Revision: 2 $
//
// $Date:
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 10Nov03  Initial version.                                     Stephen McDaniel
//
//////////////////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "MicrWrapper.h"

// ================================================================ //
// Following Defines extracted from OposMicr.h 
// (alt. is to remove this section and include scot\adk\OposMicr.h)
// ================================================================ //
//#include "OposMicr.h"
//////////////////////////////////////////////////////////////////////
// "ResultCodeExtended" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG OPOS_EMICR_NOCHECK       = 201; // EndInsertion
const LONG OPOS_EMICR_CHECK         = 202; // EndRemoval

// The following were added in Release 1.7
const LONG OPOS_EMICR_BADDATA       = 203;
const LONG OPOS_EMICR_NODATA        = 204;
const LONG OPOS_EMICR_BADSIZE       = 205;
const LONG OPOS_EMICR_JAM           = 206;
const LONG OPOS_EMICR_CHECKDIGIT    = 207;
const LONG OPOS_EMICR_COVEROPEN     = 208;
// ================================================================ //


#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("MICRWrapper")

/////////////////////////////////////////////////////////////////////////////
// CMICRWrapper


BOOL CMICRWrapper::Create(
   CWnd* pParentWnd, 
	UINT  nID )
   {
   return TOPOSMicr<CGenericOposIF>::Create(NULL, 0, CRect(0,0,0,0), pParentWnd, nID );
   }

/////////////////////////////////////////////////////////////////////////////
// CMICRWrapper properties



/////////////////////////////////////////////////////////////////////////////
// CMICRWrapper operations



long CMICRWrapper::EndInsertion()
   {
   long result = TOPOSMicr<CGenericOposIF>::EndInsertion();
   TOPOSMicr<CGenericOposIF>::SetDataEventEnabled( TRUE );
	return result;
   }


long CMICRWrapper::BeginInsertion(
   long Timeout)
   {
   long lResult = TOPOSMicr<CGenericOposIF>::BeginInsertion( 0 );
   if ( lResult == OPOS_SUCCESS )
      lResult = EndInsertion();
   else
      {
      m_pParentWnd->SetTimer( m_nControlId, 200L, NULL ); 
      lResult = OPOS_SUCCESS; 
      }
   return lResult;
   }

/* virtual */
void CMICRWrapper::SetDeviceEnabled( 
   BOOL bEnable )
   {
   m_pParentWnd->KillTimer( m_nControlId );
   TOPOSMicr<CGenericOposIF>::SetDeviceEnabled( bEnable );
   return;
   }


/* virtual */
void CMICRWrapper::OnOposDataEvent( 
								   long  nStatus )
{
	CString sData;
	sData.Format( _T("%s%c%s%c%s%c%s%c%s%c%s%c%s%c%d%c%d"), 
		GetRawData(),        DM_FS, 
		GetAccountNumber(),  DM_FS, 
		GetAmount(),         DM_FS, 
		GetBankNumber(),     DM_FS, 
		GetEpc(),            DM_FS, 
		GetSerialNumber (),  DM_FS, 
		GetTransitNumber(),  DM_FS, 
		GetCheckType(),      DM_FS, 
		GetCountryCode() );
	PostDmEventToApplication( DMCLASS_MICR, m_nDeviceId, sData );
	return;
}

/* virtual */
void CMICRWrapper::OnOposErrorEvent( 
									long nResult, 
									long nRcEx )
{
	long nStatus = 0;
	switch( nRcEx )
	{
	case NCR_EMICR_LINE_ERROR:
		nStatus = DM_MICR_ERR_LINE_ERROR;
		break;
	case NCR_EMICR_NOCHECK:
		nStatus = DM_MICR_ERR_NOCHECK;
		break;
	case NCR_EMICR_NO_DATA:
    case OPOS_EMICR_NODATA:
        nStatus =  DM_MICR_ERR_NO_DATA;
		break;
	case NCR_EMICR_BAD_DATA:
	case OPOS_EMICR_BADDATA:
		nStatus = DM_MICR_ERR_BAD_DATA;
		break;
	case NCR_EMICR_CHECKJAM:
	case OPOS_EMICR_JAM:
        nStatus = DM_MICR_ERR_CHECKJAM;
		break;
	case OPOS_EMICR_COVEROPEN:
		nStatus = DM_MICR_ERR_COVER_OPEN;
		break;
	case OPOS_EMICR_BADSIZE:
		nStatus = DM_MICR_ERR_BADSIZE;
		break;
    case OPOS_EMICR_CHECKDIGIT:
        nStatus = DM_MICR_ERR_CHECKDIGIT;
        break;
    default:
		nStatus = MapOposErrorToDM( nResult );
		if ( nStatus == DM_ERROR_UNEXPECTED )
            trace(L6, _T("DMX:OnOposErrorEvent??? MICR-")
            _T("%d (%d,%d) Posting DM_ERROR_UNEXPECTED"), m_nDeviceId, nResult, nRcEx );
		break;
	}
	PostDmEventToApplication( DM_ERROR, DMCLASS_MICR, m_nDeviceId, nStatus );
	return ;
}




/* virtual */
void CMICRWrapper::OnTimer( 
   UINT nIDEvent )
   {
   long lResult = TOPOSMicr<CGenericOposIF>::BeginInsertion( 0 );
   if ( lResult != OPOS_E_TIMEOUT )
      {
      m_pParentWnd->KillTimer( m_nControlId );
      if ( lResult == OPOS_SUCCESS )
         EndInsertion();
      else 
         {  // Simulate an error event....
         OnOposErrorEvent( lResult, GetResultCodeExtended() );
         }
      }
   return;
   }
