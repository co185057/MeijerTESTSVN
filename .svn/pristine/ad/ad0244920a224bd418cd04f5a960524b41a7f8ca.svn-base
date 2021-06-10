/////////////////////////////////////////////////////////////////////////////
//
// cashdrawer.cpp
//
//   Implementation of IDispatch wrapper class for CCashDrawerWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/dmx/CashDrawerWrapper.cpp $
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
#include "CashDrawerWrapper.h"

/////////////////////////////////////////////////////////////////////////////
// CCashDrawerWrapper


BOOL CCashDrawerWrapper::Create( 
   CWnd* pParentWnd, 
   UINT  nID )
	{
   return TOPOSCashDrawer<CGenericOposIF>::Create(NULL, 0, CRect(0,0,0,0), pParentWnd, nID );
   }

/////////////////////////////////////////////////////////////////////////////
// CCashDrawerWrapper properties


/////////////////////////////////////////////////////////////////////////////
// CCashDrawerWrapper operations


/* virtual */
void CCashDrawerWrapper::OnOposStatusEvent( 
   long  nStatus )
   {
	if ( nStatus != FALSE )
      nStatus = DM_DRAWER_OPEN;
   else
      nStatus = DM_DRAWER_CLOSED;
   PostDmEventToApplication( DM_STATUS, DMCLASS_CASHDRAWER, m_nDeviceId, nStatus );
   return;
   }

