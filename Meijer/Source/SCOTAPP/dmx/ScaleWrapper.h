/////////////////////////////////////////////////////////////////////////////
//
// ScaleWrapper.h
//
//   Declaration of IDispatch wrapper class for CScaleWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Fastlane/4.0 Patch D/Development/Core/Source/SCOTAPP/dmx/ScaleWrapper.h $
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

#ifndef __SCALE_H__
#define __SCALE_H__

#include "TOPOSScale.h"

class CScaleWrapper : public TOPOSScale<CGenericOposIF>
   {
   public:
      virtual void OnOposStatusEvent( long nStatus );
      static TClassPtr<CGenericOposIF> CreateDevice( HKEY hKey, long nDeviceId )
         { return new CScaleWrapper( nDeviceId ); }
      virtual CScaleWrapper* GetScale( void ) { return this; }
   public:  // virtuals from CGenericOposIF
      virtual void SetDeviceEnabled( BOOL bEnable );
      virtual long ReadFirmwareVersion( CString& sVersion, long* lVersion );
      virtual long GetDeviceClass( void ) const 
         { return DMCLASS_SCALE; }
      virtual void OnTimer( UINT nIDEvent );
      virtual bool IsInputDevice( void ) const { return true; }
   public:
	   BOOL Create( CWnd* pParentWnd, UINT nID );
   // Attributes
   public:
// Operations
public:
private:
   CScaleWrapper( long nDeviceId ) : m_nScaleWeight( -1 ) { m_nDeviceId = nDeviceId; } 
private:
   long m_nScaleWeight;
};

#endif // __SCALE_H__
