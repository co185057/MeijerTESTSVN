// OposDevice.h: interface for the COposDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OposDevice_H__4AE92974_9BB1_11D3_A286_000000000000__INCLUDED_)
#define AFX_OposDevice_H__4AE92974_9BB1_11D3_A286_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COposDevice
   {
   public:
      long Open( COposService* pService, COposRegistry& oRegistry );
      void Close( COposService* pService );
      long Claim( long Timeout );
      void ReleaseClaim( void );
      long Enable( BOOL bEnable );
      virtual long InteractiveHealthCheck( COposCheckHealth* pCheckHealth = NULL );
      // These are the "old style" check health APIs
      // New projects ise TService::CheckHealthEx and return useful
      // CheckHealthText string ids and RcEx values. 
      virtual long InternalHealthCheck( void ) {  ASSERT( FALSE ); return OPOS_E_ILLEGAL; }
      virtual long ExternalHealthCheck( void ) {  ASSERT( FALSE ); return OPOS_E_ILLEGAL; }
      virtual void SetPropertyNumber( long lPidx, long lProperty );
      virtual void SetPropertyString( long lPidx, LPCTSTR  sProperty )
         {  ASSERT( FALSE ); }
      virtual CString GetPropertyString( long lPidx ) const
         {  ASSERT( FALSE ); return CString(); }
      virtual long GetPropertyNumber( long lPidx ) const;
      virtual long DirectIO( long nCommand, long* plData, CString& sData );
      virtual BOOL IsBusy( void ) const { return FALSE; }
      virtual CString GetDeviceModel( void ) const = 0;
      virtual CString GetDeviceVersion( void ) const = 0;
      virtual BOOL CheckHealth3Active( void ) const { return m_pCheckHealth != NULL; }
      CString GetBuildVersion( void ) const 
         { return m_pService->GetBuildVersion(); }
      virtual void OnDeviceError( long nResult, long nRcEx = 0 ) const;
      void SetPowerState( long nPowerState );
   public:
      virtual void ClearOutput( void ) { }
      void OnDeviceOk( void );
      void OnDeviceFailure( int nErrorId = 0 );
      BOOL CheckDeviceOnline( CWnd* pWnd, int idsDeviceIsOffline,
         int idsDeviceIsOnline, int idsDeviceHasFailed );
      COposDiagnostics& GetEventLog( void ) const { return m_oDiagnostics; }
   protected:
	   COposDevice(
         LPCTSTR        sClass,
         LPCTSTR        sEventSource,
         int            idsName,       // from the resource: "device name\nDescription"
         int            nShareKey,     // Sharing key, for claim sem name
         int            nFirstTally,   // Common Tally implementation
         int            nLastTally,
         int            idsSoDescription,
         int            nSmInvalidDirectIO,
         CmDataCapture& cmDc );        // Device Id for multi-Pinpad device
      virtual ~COposDevice()
         {
         }
      long SetRC( long nRC = 0, long nRCEx = 0 ) const;
      BOOL IsOpen( void ) const    { return m_pService != NULL; }
      BOOL IsClaimed( void ) const { return m_bClaimed; }
      BOOL IsEnabled( void ) const { return m_bDeviceEnabled; }
      void ReportTally( int nTallyId ) const;
   protected:     // virtuals overriden by derived class 
      virtual long OpenDevice( void ) = 0;
      virtual void CloseDevice( void ) = 0;
      virtual BOOL ReadRegistry( COposRegistry& oRegistry ) { return TRUE; }
      virtual long OnEnable( long* pnRcEx ) { return OPOS_SUCCESS; }
      virtual void OnDisable( ) { }
      virtual void OnDeviceClosed( ) { }
      virtual BOOL IsExclusive( void ) const { return TRUE; }
      virtual void OnDeviceRecovered( void ) const { }
      virtual void OnDeviceFailed( void ) const { }
   protected:
      mutable CmDataCapture&     m_cmDc;
      mutable COposDiagnostics   m_oDiagnostics;
//      mutable CTally             m_Tally;
   private:
      COposService*           m_pService;
      COposSharedData         m_ShareArea;         // Shared data for Dcap Id
      CMutex                  m_mtxClaim;    
   private:
      DWORD                   m_bDeviceEnabled           : 1;
      DWORD                   m_bClaimed                 : 1;
      DWORD                   m_bFailed                  : 1;
      const int               m_nDeviceId;
      const int               m_nFirstTally;
      const int               m_nLastTally;
      const int               m_nSmInvalidDirectIO;
      int                     m_idsCheckHealthText;
      long                    m_nPowerState;
   protected:
      const int               m_nShareKey;
      COposCheckHealth*       m_pCheckHealth;
   };

#endif // !defined(AFX_OposDevice_H__4AE92974_9BB1_11D3_A286_000000000000__INCLUDED_)
