/////////////////////////////////////////////////////////////////////////////
//
// scanner.h
//
//   Declaration of IDispatch wrapper class for CScannerWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Fastlane/4.0 Patch D/Development/Core/Source/SCOTAPP/dmx/ScannerWrapper.h $
//
// $Revision: 4 $
//
// $Date: 6/14/05 3:42p $
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 96/11/15 Initial version.                                    C. Light
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <set>
#include "TOPOSScanner.h"

using std::set;


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
      virtual void SetDeviceEnabled( BOOL bEnable );
      virtual long CheckHealth( long lLevel );
   protected:
      virtual bool IsInputDevice( void ) const { return true; }
public:
	BOOL Create( CWnd* pParentWnd, UINT nID );
// Attributes
public:
   long SetScannerPacesetterEnabled( BOOL bNewValue );
   BOOL GetPacesetterEnabled( void );
   void GetEncryptedTypes();                            // SR 93.4
   bool IsBarcodeTypeEncrypted(long lBarcodeType);      // SR 93.4
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

      set<long> m_lEncryptedTypes; //SR93.4


      bool m_bVerify;      // TRUE if enable is to be verified.
   };

#endif // __SCANNER_H__
