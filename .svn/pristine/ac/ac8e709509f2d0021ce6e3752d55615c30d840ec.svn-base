/////////////////////////////////////////////////////////////////////////////
//
// scanner.h
//
//   Declaration of IDispatch wrapper class for CScannerWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/dmx/ScannerWrapper.h $
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

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "TOPOSScanner.h"

//tn. Add feature of making terminal beep when scanning an item
#define MAKE_BEEP_WHEN_SCAN		_T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP")

class CScannerWrapper : public TOPOSScanner<CGenericOposIF>
   {
   public:
      ~CScannerWrapper();
      static TClassPtr<CGenericOposIF> CreateDevice( HKEY hKey, long nDeviceId );
      virtual void OnOposErrorEvent( long nResult, long nRcEx );  
      virtual void OnOposDataEvent( long nStatus );
      virtual void OnOposDirectIOEvent( long nEventId, long* plData, BSTR* pbstrData );
      virtual CScannerWrapper * GetScanner( void ) { return this; }
      virtual long DoOneTimeInitialization(void);
   public:  // virtuals from CGenericOposIF
      virtual long ReadFirmwareVersion( CString& sVersion, long* lVersion );
      virtual long ClaimDevice( long lTimeout) 
         { return TOPOSScanner<CGenericOposIF>::ClaimDevice( 4500 ); }
      virtual long GetDeviceClass( void ) const 
         { return DMCLASS_SCANNER; }
   protected:
      virtual bool IsInputDevice( void ) const { return true; }
public:
	BOOL Create( CWnd* pParentWnd, UINT nID );
// Attributes
public:
   long SetScannerPacesetterEnabled( BOOL bNewValue );
   BOOL GetPacesetterEnabled( void );
// Operations
public:
   private: 
      CScannerWrapper( long nDeviceId );
   private:
      BOOL  m_bPaceSetterEnabled;
   private:
      bool ReadRegEntryBeepWhenScan();
      void BeepThread();
      static int BeepThreadEntry(void *p);

      CWinThread *m_pBeepThread;
      CEvent m_evtBeep;
      bool m_bBeepThreadExit;
      bool m_bBeepWhenScan;

	  DWORD m_dwFreq;
	  DWORD m_dwDuration;
   };

#endif // __SCANNER_H__
