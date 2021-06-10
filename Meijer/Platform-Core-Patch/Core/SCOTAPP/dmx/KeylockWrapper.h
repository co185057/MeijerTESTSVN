/////////////////////////////////////////////////////////////////////////////
//
// keylock.h
//
//   Declaration of IDispatch wrapper class for CKeylockWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/dmx/KeylockWrapper.h $
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

#ifndef __KEYLOCK_H__
#define __KEYLOCK_H__

#include "TOPOSKeylock.h"

class CKeylockWrapper : public TOPOSKeylock<CGenericOposIF>
   {
   public:
      virtual void OnOposStatusEvent( long nStatus );
      static TClassPtr<CGenericOposIF> CreateDevice( HKEY hKey, long nDeviceId )
         { return new CKeylockWrapper( nDeviceId ); }
      virtual CKeylockWrapper* GetKeylock( void ) { return this; }
   public:  // pure virtuals from CGenericOposIF
      virtual long GetDeviceClass( void ) const 
         { return DMCLASS_KEYLOCK; }
   public:
	   BOOL Create( CWnd* pParentWnd, UINT nID );
   private:
      CKeylockWrapper( long nDeviceId ) { m_nDeviceId = nDeviceId; } 
   };

#endif // __KEYLOCK_H__
