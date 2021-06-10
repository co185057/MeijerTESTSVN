/////////////////////////////////////////////////////////////////////////////
//
// OposSerialInterface.h: 
//
// Declaration of CPollableDevice and COposSerialInterface classes.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 99/04/30 Initial version.                                      T. Burk
//          Finally got tired of copying this code from project
//          to project.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPOSSERIALINTERFACE_H__F3925C28_F0DE_11D2_99B2_00A0C9EDD467__INCLUDED_)
#define AFX_OPOSSERIALINTERFACE_H__F3925C28_F0DE_11D2_99B2_00A0C9EDD467__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Users must define an object to be notified of serial events

#define PURGE_ALL  (PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)

class CPollableDevice
   {
   public:
      // Optional virtuals to override
      virtual void OnWriteFailure( void ) { }
      // 
      virtual void OnLineError( DWORD /* dwErrors */ ) {}
      // Periodic device poll, return TRUE if device responded
      virtual BOOL Poll( void ) { return TRUE; }
      // Special character was received
      virtual void OnRxFlag( void ) {}
      // One of CTS, DSR, RING, RLSD changed
      virtual void OnModemStatusChange( DWORD /* dwEventMask */ ) {}
      // A break was detected
      virtual void OnBreak( void ) {}
      // A character is available to be read
      virtual void OnRxChar( void ) {}
      // Device has not responded repeatedly - appears to be offline
      virtual BOOL OnDeviceTimeout( void ) { return TRUE; }
      // Device did not respond to command.
      virtual void OnNoResponse( void ) {}
      // 
      virtual BOOL IsSameCommand( const void* pCmd1, const void* pCmd2, 
         int nLength ) const
         { return memcmp( pCmd1, pCmd2, nLength ) == 0; }
      virtual BOOL IsSameResponse( const void* pResp1, const void* pResp2, 
         int nLength ) const
         { return memcmp( pResp1, pResp2, nLength ) == 0; }
   };


class COposSerialInterface : public CMutex
   {
   public:
	   COposSerialInterface( int nPort, CmDataCapture& cmDc, 
         LPCTSTR sMtxName = NULL, BOOL bAutoBaud = FALSE );
	   virtual ~COposSerialInterface();    
	   BOOL BeginService( DWORD dwPriority = THREAD_PRIORITY_ABOVE_NORMAL );
	   void EndService();
	   BOOL Open( CPollableDevice* pDevice );
	   void Close();
      DWORD SendReceive( const void* sCommand, int nCommandLength, void* sResponseData, int nResponseSize, DWORD dwPurgeFlags = PURGE_ALL );
	   BOOL Write( const void* sData, DWORD dwLength );
	   DWORD Read( void* cResponse,  DWORD dwLength );
      BOOL SetCommState( const DCB& rDCB );
      BOOL GetCommState( LPDCB pDCB );
      BOOL SetCommTimeouts( const COMMTIMEOUTS& rTimeouts );
      BOOL GetCommTimeouts( LPCOMMTIMEOUTS pTimeouts );
      BOOL GetCommModemStatus( LPDWORD lpModemStat ) const; 
      DWORD SetSpecialDcMask( DWORD dwSpecialDcMask )
         {
         DWORD dwResult = m_dwSpecialDcMask;
         m_dwSpecialDcMask = dwSpecialDcMask;
         return dwResult;
         }
      DWORD GetSpecialDcMask( void ) const { return m_dwSpecialDcMask; }
      void SetPollTimeouts( 
         DWORD nPollTimeout, 
         DWORD nOfflinePollTimeout, 
         DWORD nFailureTimeout,
         DWORD nDelayTimer = 0)
         {
         m_nPollTimeout          = nPollTimeout;
         m_nOfflinePollTimeout   = nOfflinePollTimeout;
         m_nFailureTimeout       = nFailureTimeout;
         m_dwDelayTimer          = nDelayTimer;
         }
#ifdef _DEBUG
      BOOL IsPollThread( void ) const { return m_dwPollThreadId == ::GetCurrentThreadId(); }
#endif
      DWORD SetDelayTimeout( DWORD nDelayTimer )
         {
         DWORD dwResult = m_dwDelayTimer;
         m_dwDelayTimer          = nDelayTimer;
         return dwResult;
         }
      void SuspendPolling( BOOL bSuspend ) { m_bSuspendPolling = bSuspend; }
      void SetRetries( DWORD nRetries ) { m_nRetries = nRetries; }
      void SetCommMask( DWORD dwMask ) 
         { 
         m_dwCommMask = dwMask; 
         if ( m_hCom != INVALID_HANDLE_VALUE )
            ::SetCommMask( m_hCom, m_dwCommMask );
         }
      operator HANDLE() const { return m_hCom; }
   private:
	   static UINT PollThreadEntry( LPVOID pvArg );
	   UINT PollThread();
   protected:
      virtual void ClosePort( HANDLE hCom );
      virtual HANDLE OpenPort( LPCTSTR sCOM );
   private:
      DWORD PollDevice( void );
      void  ReportLineErrors( void ) const;
   private:
	   mutable HANDLE          m_hCom;
      CEvent                  m_evExit;
      mutable CmDataCapture&  m_cmDc;
      CPollableDevice*        m_pDevice;
      const int               m_nPort;
      CWinThread*             m_pPollThread;
#ifdef _DEBUG
      DWORD             m_dwPollThreadId;
#endif
      // Timout data
      DWORD             m_dwLastGoodResponse;
      DWORD             m_nPollTimeout;
      DWORD             m_nOfflinePollTimeout;
      DWORD             m_nFailureTimeout;
      DWORD             m_nRetries;
      DWORD             m_dwSpecialDcMask;
      BOOL              m_bFailed;
      BOOL              m_bSuspendPolling;
      // Save these in memory so they may be initialized prior to Open.
      DCB               m_dcb;
      COMMTIMEOUTS      m_commTO;
      mutable DWORD     m_dwLastMessage;
      DWORD             m_dwDelayTimer;
      // Statistics
      DWORD             m_dwMinResponseTime;
      DWORD             m_dwMaxResponseTime;
      DWORD             m_dwCommMask;
      BOOL              m_bBaudRateKnown;
   };

// "Shared" Port additions

typedef struct handle_map_t 
   {
   DWORD   dwPid;       // Process Id
   HANDLE  hHandle;     // Handle
   void Reset( void )
      {
      hHandle = INVALID_HANDLE_VALUE;
      dwPid = 0;                     
      }
   BOOL IsAvailable( void ) const
      {
      ASSERT( dwPid == 0 || hHandle != INVALID_HANDLE_VALUE );
      ASSERT( dwPid != 0 || hHandle == INVALID_HANDLE_VALUE );
      return dwPid == 0 && hHandle == INVALID_HANDLE_VALUE;
      }
   void SetHandle( HANDLE hCom )
      {
      ASSERT( IsAvailable() != FALSE );
      dwPid = GetCurrentProcessId();
      hHandle = hCom;
      }
   struct handle_map_t( ) { Reset(); }
   } HANDLE_MAP_T;

// CSharedSerialInterface
//
// This class is derived from COposSerialInterface to reuse the existing serial code
// The class is derived from CMutex so that callers can synchronize calls across
// multiple methods of the class. ("Java like" implementation)
// 

class CSharedSerialInterface : public COposSerialInterface
   {
   public:
      CSharedSerialInterface( int nPort, LPCTSTR sDevName,
         LPCTSTR sShareName, LPCTSTR sMtxName, DWORD nCount, 
         CmDataCapture& cmDc );
   protected:
      virtual void ClosePort( HANDLE hCom );
      virtual HANDLE OpenPort( LPCTSTR sCOM );
   private:
      HANDLE DuplicateComHandle( void );
   private:
      CSharedMemory  m_SharedHandles;
      const int      m_nHandleCount;
      HANDLE_MAP_T*  m_pHandleMap;
   };

#endif // !defined(AFX_OPOSSERIALINTERFACE_H__F3925C28_F0DE_11D2_99B2_00A0C9EDD467__INCLUDED_)
