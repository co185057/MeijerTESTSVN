/////////////////////////////////////////////////////////////////////////////
//
// micr.h
//
//   Declaration of IDispatch wrapper class for CMICRWrapper
//
//   Copyright (c) 2003 NCR.  All rights reserved.
//
// $Revision: 4 $
//
// $Date:
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 10Nov03  Initial version.                                     Stephen McDaniel
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __MICR_H__
#define __MICR_H__

#include "TOPOSMicr.h"

class CMICRWrapper : public TOPOSMicr<CGenericOposIF>
   {
   public:  
      static TClassPtr<CGenericOposIF> CreateDevice( HKEY hKey, long nDeviceId )
         { return new CMICRWrapper( nDeviceId ); }
      virtual void OnOposErrorEvent( long nResult, long nRcEx );  
      virtual CMICRWrapper* GetMICR( void ) { return this; }
      virtual void OnOposDataEvent( long nStatus );
   protected:
      virtual bool IsInputDevice( void ) const { return true; }
      virtual void OnTimer( UINT nIDEvent );
   public:
	   BOOL Create( CWnd* pParentWnd, UINT nID );
   public:  // pure virtuals from CGenericOposIF
      virtual void SetDeviceEnabled( BOOL bEnable );
      virtual long GetDeviceClass( void ) const 
         { return DMCLASS_MICR; }
   // Operations
   public:
   	long EndInsertion();
      long BeginInsertion(long Timeout);
   private:
      CMICRWrapper( long nDeviceId ) { m_nDeviceId = nDeviceId; } 

};

#endif // __MICR_H__

