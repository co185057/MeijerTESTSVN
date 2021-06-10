/////////////////////////////////////////////////////////////////////////////
//
// ScaleWrapper.cpp
//
//   Implementation of IDispatch wrapper class for CScaleWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Fastlane/4.0 Patch D/Development/Core/Source/SCOTAPP/dmx/ScaleWrapper.cpp $
//
// $Revision: 1 $
//
// $Date: 6/14/05 3:40p $
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 96/11/15 Initial version.                                    C. Light
//
/////////////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "ScaleWrapper.h"
#include <oposscal.h>

/////////////////////////////////////////////////////////////////////////////
// CScaleWrapper


BOOL CScaleWrapper::Create( 
   CWnd* pParentWnd, 
   UINT nID )
	{
   return TOPOSScale<CGenericOposIF>::Create(NULL, 0, CRect(0,0,0,0), pParentWnd, nID );
   }


/////////////////////////////////////////////////////////////////////////////
// CScaleWrapper properties


/////////////////////////////////////////////////////////////////////////////
// CScaleWrapper operations


/* virtual */
void CScaleWrapper::SetDeviceEnabled( 
   BOOL bEnable ) 
   { 
   TOPOSScale<CGenericOposIF>::SetDeviceEnabled( bEnable ); 
   if ( TOPOSScale<CGenericOposIF>::GetDeviceEnabled() == FALSE )
      m_pParentWnd->KillTimer( m_nControlId );
   else
      m_pParentWnd->SetTimer( m_nControlId, 250, NULL );
   m_nScaleWeight = -1;
   return;
   }

/////////////////////////////////////////////////////////////////////////////
// CDevMgrCtrl::TranslateScaleFailure
//
// This helper function is used by ScaleReadWeight() and ScaleCheckWeight()
// to translate an OPOS Scale failure into a DM Scale failure.
//
// Possible return values include:
//
// DM_ERROR_SCALE_UNSTABLE: scale is unstable
// DM_ERROR_SCALE_OVERWEIGHT: scale exceeds max capacity
// DM_ERROR_SCALE_UNDERZERO: scale weight is less than zero
// DM_ERROR_SCALE_WEIGHTUNCHANGED: scale weight has not changed since last read
// DM_ERROR_GENERALFAILURE: general device failure

static long TranslateScaleFailure(
   long lRcEx )
   {
	/*  Need change for SR672 */
	long lResult = DM_ERROR_GENERALFAILURE;

	if (lRcEx == OPOS_ESCAL_OVERWEIGHT)
		lResult = DM_ERROR_SCALE_OVERWEIGHT;
	else if (lRcEx == NCR_ESCAL_ZEROWEIGHT)
		lResult = 0;
	else if (lRcEx == NCR_ESCAL_WEIGHTUNCHANGED)
		lResult = DM_ERROR_SCALE_WEIGHTUNCHANGED;
	else if (lRcEx == NCR_ESCAL_UNSTABLE)
		lResult = DM_ERROR_SCALE_UNSTABLE;
	else if (lRcEx == NCR_ESCAL_UNDERZERO)
		lResult = DM_ERROR_SCALE_UNDERZERO;
	return lResult;
	
	//return lRcEx;

   }
   

static int failureCount = 0;
#define SCALE_FAILURE_THRESHOLD 50

/* virtual */
void CScaleWrapper::OnTimer( 
   UINT nIDEvent )
   {
//   ASSERT( nIDEvent == 0 );
	long lWeight = 0;
   BSTR bstrUnused = NULL;
   long lResult = DirectIO(NCRDIO_SCAL_LIVE_WEIGHT, &lWeight, &bstrUnused );
   SysFreeString( bstrUnused  );
	if ( lResult != OPOS_SUCCESS )
	   {
      long nRcEx = GetResultCodeExtended();
		lWeight = TranslateScaleFailure( nRcEx  );
      if ( (lResult  == OPOS_E_TIMEOUT || lResult == OPOS_E_FAILURE) 
         && (nRcEx == NCR_ESCAL_NOTREADY || nRcEx == 0) 
         && ++failureCount < SCALE_FAILURE_THRESHOLD)
         lWeight = m_nScaleWeight;
      }
   else
      failureCount = 0;
	// If we haven't already reported this weight / error, fire an Input Event
	if ( lWeight != m_nScaleWeight)
	   {
      failureCount = 0;
      m_nScaleWeight = lWeight;
      mo.PostDM(DM_DATA, DMCLASS_SCALE, m_nDeviceId, sizeof(VT_I4) , (void*)&m_nScaleWeight);
      }
   return;
   }

/* virtual */
void CScaleWrapper::OnOposStatusEvent( long nStatus ) { }


/* virtual */
long CScaleWrapper::ReadFirmwareVersion( 
   CString& sVersion, 
   long* lVersion )
   {
   BSTR bstrData = NULL;
   long lResult = DirectIO( NCRDIO_SCAL_READROM, lVersion, &bstrData );
   if ( lResult  == OPOS_SUCCESS )
      sVersion = bstrData;
   ::SysFreeString( bstrData );
   return lResult;
   }
