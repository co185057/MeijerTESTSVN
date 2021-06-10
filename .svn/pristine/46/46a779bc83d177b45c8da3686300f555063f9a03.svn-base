/////////////////////////////////////////////////////////////////////////////
//
// cashdrawer.h
//
//   Declaration of IDispatch wrapper class for CCashDrawerWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Fastlane/4.0 Patch D/Development/Core/Source/SCOTAPP/dmx/CashDrawerWrapper.h $
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

#ifndef __CASHDRAWER_H__
#define __CASHDRAWER_H__

#include "TOPOSCashDrawer.h"

class CCashDrawerWrapper : public TOPOSCashDrawer<CGenericOposIF>
   {
   public:
      virtual void OnOposStatusEvent( long nStatus );
      static TClassPtr<CGenericOposIF> CreateDevice( HKEY hKey, long nDeviceId )
         { return new CCashDrawerWrapper( nDeviceId ); }
      virtual CCashDrawerWrapper* GetCashDrawer( void ) { return this; }
   public:
	   BOOL Create( CWnd* pParentWnd, UINT nID );
// Attributes
public:
// Operations
public:
   public:  // pure virtuals from CGenericOposIF
      virtual long GetDeviceClass( void ) const 
         { return DMCLASS_CASHDRAWER; }

private:
   CCashDrawerWrapper( long nDeviceId ) { m_nDeviceId  = nDeviceId; } 
};

#endif // __CASHDRAWER_H__
