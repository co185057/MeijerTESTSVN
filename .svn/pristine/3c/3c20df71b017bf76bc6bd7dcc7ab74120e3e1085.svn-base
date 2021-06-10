/////////////////////////////////////////////////////////////////////////////
//
// keylock.cpp
//
//   Implementation of IDispatch wrapper class for CKeylockWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Fastlane/4.0 Patch D/Development/Core/Source/SCOTAPP/dmx/KeylockWrapper.cpp $
//
// $Revision: 4 $
//
// $Date: 6/14/05 3:41p $
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 96/11/15 Initial version.                                    C. Light
//
/////////////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "KeylockWrapper.h"

/////////////////////////////////////////////////////////////////////////////
// CKeylockWrapper



BOOL CKeylockWrapper::Create( 
   CWnd* pParentWnd, 
   UINT nID )
	{
   return TOPOSKeylock<CGenericOposIF>::Create(NULL, 0, CRect(0,0,0,0), pParentWnd, nID );
   }


/////////////////////////////////////////////////////////////////////////////
// CKeylockWrapper properties


/////////////////////////////////////////////////////////////////////////////
// CKeylockWrapper operations


/* virtual */
void CKeylockWrapper::OnOposStatusEvent( 
   long  nStatus )
   {
	switch(nStatus)
	   {
		case LOCK_KP_LOCK:
			nStatus = DM_KEYLOCK_L;
			break;
		case LOCK_KP_NORM:
			nStatus = DM_KEYLOCK_N;
			break;
		case LOCK_KP_SUPR:
			nStatus = DM_KEYLOCK_S;
			break;
		case 4:
			nStatus = DM_KEYLOCK_X;
			break;
		default:
         ASSERT( FALSE );
         break;
      }
   PostDmEventToApplication( DM_STATUS, DMCLASS_KEYLOCK, m_nDeviceId, nStatus );
   return;
   }

