/////////////////////////////////////////////////////////////////////////////
//
// msr.h
//
//   Declaration of IDispatch wrapper class for CMSRWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/dmx/MSRWrapper.h $
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

#ifndef __MSR_H__
#define __MSR_H__

#include "NCRMSR.h"
#include "TOPOSMsr.h"


class CMSRWrapper : public TOPOSMsr<CGenericOposIF>
   {
   public:  
      static TClassPtr<CGenericOposIF> CreateDevice( HKEY hKey, long nDeviceId )
         { return new CMSRWrapper( nDeviceId ); }
      virtual void OnOposErrorEvent( long nResult, long nRcEx );  
      virtual void OnOposDataEvent( long nStatus );
      virtual void OnOposStatusEvent( long nStatus );
      virtual CMSRWrapper* GetMSR( void ) { return this; }
      virtual long DoOneTimeInitialization(void);
   protected:
      virtual bool IsInputDevice( void ) const { return true; }
   public:
	   virtual BOOL Create( CWnd* pParentWnd, UINT nID );
   public:  // pure virtuals from CGenericOposIF
      virtual long GetDeviceClass( void ) const 
         { return DMCLASS_MSR; }
   private:
      CMSRWrapper( long nDeviceId ) { m_nDeviceId = nDeviceId; } 
   };


#endif // __MSR_H__
