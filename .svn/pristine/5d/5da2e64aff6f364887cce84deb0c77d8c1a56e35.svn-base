/////////////////////////////////////////////////////////////////////////////
//
// keylock.cpp
//
//   Implementation of IDispatch wrapper class for CKeylockWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/dmx/KeylockWrapper.cpp $
//
// $Revision: 1 $
//
// $Date: 6/23/08 4:56p $
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

