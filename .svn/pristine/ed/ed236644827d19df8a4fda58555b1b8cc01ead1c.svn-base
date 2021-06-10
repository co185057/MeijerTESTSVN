/////////////////////////////////////////////////////////////////////////////
//
// ServiceTemplate.h
//
// Declaration of TOposService template class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/06/17 Initial version.                                      T. Burk
//

// The project's resource file is included here.  The template uses 
// the folowing IDS_ manifest constants:
//    IDS_CH_UNKNOWN
//    IDS_CH_INTERNAL_PASS
//    IDS_CH_INTERNAL_FAIL
//    IDS_CH_EXTERNAL_PASS
//    IDS_CH_EXTERNAL_FAIL
//    IDS_CH_INTERACTIVE_PASS
//    IDS_CH_INTERACTIVE_FAIL
//    IDS_SO_DESCRIPTION
//    IDS_FILE_VERSION_COMMENT
// Since these are preprocessor replaced, they are not known when the 
// template is instantiated by the compiler ( too late ). I decided
// to include the resource file here instead of a more complicated solution.
// I considered:
//    1) Pass the IDs in to the functions that require them:
//       Check Health & OpenService
//       But this would be a pain.
//    2) Define compiler resolvable names ( const long ) for each and
//       then require the user to define them. This would work, and would
//       allow some flexability in naming, but would require the extra step
//       of defining them.  Also a pain. 
// This scheme requires that the user define strings with the appropriate
// names. 
// This implementation does not create a precompiled header dependency on
// resource.h
// This should also not present any problem with a .DLL that supports 
// multiple Service Object ( Template instantiated more than once ) since
// there is still only one device, and only one set of these is required 
// regardless of the number of service objects in the DLL. 

#include <resource.h>	// A dummy resource file for Opos.lib

static LPCTSTR g_szNCRVersion = _T("NCRVersion");

// Claim Notes:
// Service Objects that do not support the concept of being claimed should 
// do the following:
//    1) After Successful Open, call TOposService::Claim
//    2) In their claim and release methods, return OPOS_E_ILLEGAL
//    3) Check for PIDX_Claimed in GetPropertyNumber, if it is
//       return FALSE, else call TOposService::GetPropertyNumber.
// This allows the framework to enforce that Claim is before enable
// and increases the amount of code that can be boilerplated. Step 2,3
// hides this implementation from the users.
// Release does not need to be called since this is handled by the 
// framework in TOposService::Close


template<class DEVICE_TYPE, class SERVICE_TYPE>
class TOposService 
   {
   public:
      TOposService( LPCTSTR sServiceClassName, 
         TOposDeviceMap<DEVICE_TYPE>& rDeviceMap, const IID& iid );
      TOposService( LPCTSTR sServiceClassName, const IID& iid );
      ~TOposService(  );
	   long COFreezeEvents(BOOL bFreeze);
	   long CheckHealth(long lLevel);
      // CheckHealthEx - Added for better diagnostics, new SOs should use this, 
      // old SOs should migrate to it as needed.
	   long CheckHealthEx(long lLevel);
	   long Claim(long lTimeout);
	   long Release();
	   long Close();
	   long GetPropertyNumber(long lPropIndex);
	   BSTR GetPropertyString(long lPropIndex);
	   long OpenService(LPCTSTR sClass, LPCTSTR sName, LPDISPATCH pDispatch,
         LPCTSTR sTargetClass );
	   void SetPropertyNumber(long lPropIndex, long lProperty);
	   void SetPropertyString(long lPropIndex, LPCTSTR sProperty);
	   long ClearInput();
	   long ClearOutput();
	   long DirectIO(long lCommand, long FAR* plData, BSTR FAR* psData);
      DEVICE_TYPE* GetDevice( void ) 
         { ASSERT( IsOpen() != FALSE ); return m_pDevice; }
      SERVICE_TYPE& GetService( void ) 
			{ ASSERT( m_pService != NULL ); return *m_pService; }
      CmDataCapture& GetDCap( void ) { ASSERT( m_pDc != NULL ); return *m_pDc; }
      BOOL IsOpen( void ) const { return m_pDevice != NULL; }
   // Opos Members
   private:
      int                        m_nPort;
      DEVICE_TYPE*               m_pDevice;
      SERVICE_TYPE*              m_pService;     
      CmDataCapture*             m_pDc;        
      const CString              m_sServiceClassName; // For Data Capture
      const IID&                 m_iidSo;
   private:
      long Enable( BOOL bEnable );
   };


// TOposService<DEVICE_TYPE, SERVICE_TYPE>::TOposService
//
// Purpose: Constructor

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline TOposService<DEVICE_TYPE, SERVICE_TYPE>::TOposService(
   LPCTSTR sServiceClassName, 
   TOposDeviceMap<DEVICE_TYPE>& rDeviceMap,
   const IID& iid )
   : m_pDevice( NULL ),
   m_nPort( 0 ),
   m_pDc( NULL ),
   m_pService( NULL ),
   m_sServiceClassName( sServiceClassName ),
   m_iidSo( iid )
   {
#ifdef _DEBUG
   COposRegistry::AssertThreadingModelSO( iid ); 
   // Add RegisterThreadingModelSO to DllRegisterServer!
#endif
   // Create the service object, set up DCap
   m_pService = new SERVICE_TYPE;
   m_pService->InitializeProperties();
   m_pDc = &m_pService->GetDCap();
   ASSERT( m_pDc != NULL );
   return;
   }

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline TOposService<DEVICE_TYPE, SERVICE_TYPE>::TOposService(
   LPCTSTR sServiceClassName, 
   const IID& iid )
   : m_pDevice( NULL ),
   m_nPort( 0 ),
   m_pDc( NULL ),
   m_pService( NULL ),
   m_sServiceClassName( sServiceClassName ),
   m_iidSo( iid )
   {
#ifdef _DEBUG
   COposRegistry::AssertThreadingModelSO( iid ); 
   // Add RegisterThreadingModelSO to DllRegisterServer!
#endif
   // Create the service object, set up DCap
   m_pService = new SERVICE_TYPE;
   m_pService->InitializeProperties();
   m_pDc = &m_pService->GetDCap();
   ASSERT( m_pDc != NULL );
   return;
   }

// TOposService<DEVICE_TYPE, SERVICE_TYPE>::~TOposService
//
// Purpose: Destructor
// Note:    Opos Service Object lives after this class.  See notes in 
//          COposService.cpp

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline TOposService<DEVICE_TYPE, SERVICE_TYPE>::~TOposService()
   {
   ASSERT( m_pService->IsOpen() == FALSE );
   ASSERT( IsOpen() == FALSE  );
   // The following code handles Non-NCR Control Objects, they might not
   // Close us correctly, so we do it here.
   ASSERT( IsOpen() == FALSE );
   if ( m_pService->IsOpen() != FALSE )
      Close();
   m_pService->Delete();	
   return;
   }


// TOposService<DEVICE_TYPE, SERVICE_TYPE>::CheckHealth
//
// Purpose: Perform health check on the device

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::CheckHealth(
   long lLevel )
   {
   int nTextId = IDS_CH_GENERAL_FAIL;
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::CheckHealth"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_SUCCESS;
   // HC requires device be claimed, and enabled, not processing output. 
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
   if ( IsOpen() == FALSE )
      lResult = OPOS_E_CLOSED;
   else if ( m_pService->IsClaimed() == FALSE )
      lResult = OPOS_E_NOTCLAIMED;
   else if ( m_pService->IsEnabled() == FALSE )
      lResult = OPOS_E_DISABLED;
   else if ( m_pService->IsBusy() != FALSE )
      lResult = OPOS_E_BUSY;
   // Should we go on?
   if ( lResult == OPOS_SUCCESS )
      {
      nTextId = IDS_CH_UNKNOWN;
      switch ( lLevel )
         {
         case OPOS_CH_INTERNAL:
            nTextId = IDS_CH_INTERNAL_PASS;
            lResult = m_pDevice->InternalHealthCheck(); 
            if ( lResult != OPOS_SUCCESS )
               nTextId = IDS_CH_INTERNAL_FAIL;
            break;
         case OPOS_CH_EXTERNAL:
            nTextId = IDS_CH_EXTERNAL_PASS;
            lResult = m_pDevice->ExternalHealthCheck(); 
            if ( lResult != OPOS_SUCCESS )
               nTextId = IDS_CH_EXTERNAL_FAIL;
            break;
         case OPOS_CH_INTERACTIVE:
            nTextId = IDS_CH_INTERACTIVE_PASS;
            lResult = m_pDevice->InteractiveHealthCheck(); 
            if ( lResult != OPOS_SUCCESS )
               nTextId = IDS_CH_INTERACTIVE_FAIL;
            break;
         default:
            lResult = m_pService->SetRC( OPOS_E_ILLEGAL );
            break;
         }
      }
   else
      m_pService->SetRC( lResult );
   m_pService->SetCheckHealthText ( nTextId );
	return lResult;
   }

// TOposService<DEVICE_TYPE, SERVICE_TYPE>::CheckHealthEx
//
// Purpose: Perform health check on the device, get text & RcEx from device

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::CheckHealthEx(
   long lLevel )
   {
   int nTextId = IDS_CH_GENERAL_FAIL;
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::CheckHealth"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_SUCCESS;
   // HC requires device be claimed, and enabled, not processing output. 
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
   if ( IsOpen() == FALSE )
      lResult = OPOS_E_CLOSED;
   else if ( m_pService->IsClaimed() == FALSE )
      lResult = OPOS_E_NOTCLAIMED;
   else if ( m_pService->IsEnabled() == FALSE )
      lResult = OPOS_E_DISABLED;
   else if ( m_pService->IsBusy() != FALSE )
      lResult = OPOS_E_BUSY;
   // Should we go on?
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      {
      lResult = OPOS_E_ILLEGAL;
      nTextId = IDS_CH_UNKNOWN;
      switch ( lLevel )
         {
         case OPOS_CH_INTERNAL:
            nTextId = IDS_CH_INTERNAL_PASS;
            lResult = m_pDevice->InternalHealthCheck( &nTextId, &nRcEx ); 
            ASSERT( lResult != OPOS_SUCCESS || nTextId == IDS_CH_INTERNAL_PASS );
            ASSERT( lResult == OPOS_SUCCESS || nTextId != IDS_CH_INTERNAL_PASS );
            break;
         case OPOS_CH_EXTERNAL:
            nTextId = IDS_CH_EXTERNAL_PASS;
            lResult = m_pDevice->ExternalHealthCheck( &nTextId, &nRcEx ); 
            ASSERT( lResult != OPOS_SUCCESS || nTextId == IDS_CH_EXTERNAL_PASS );
            ASSERT( lResult == OPOS_SUCCESS || nTextId != IDS_CH_EXTERNAL_PASS );
            break;
         case OPOS_CH_INTERACTIVE:
            nTextId = IDS_CH_INTERACTIVE_PASS;
            // InteractiveHealthCheck should show the dialog
            m_pDevice->InteractiveHealthCheck( ); 
            lResult = m_pDevice->InternalHealthCheck( &nTextId, &nRcEx ); 
            ASSERT( lResult != OPOS_SUCCESS || nTextId == IDS_CH_INTERACTIVE_PASS );
            ASSERT( lResult == OPOS_SUCCESS || nTextId != IDS_CH_INTERACTIVE_PASS );
            break;
         }
      }
   m_pService->SetCheckHealthText ( nTextId );
   m_pService->SetRC( lResult, nRcEx );
	return lResult;
   }

// TOposService<DEVICE_TYPE, SERVICE_TYPE>::Claim
// TOposService<DEVICE_TYPE, SERVICE_TYPE>::Release
//
// Purpose: Claim/Release the device.
// Note:    Any non timeout error result from the device is changed
//          to OPOS_E_TIMEOUT, per OPOS spec.

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::Claim(
   long lTimeout ) 
   {
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::Claim"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_SUCCESS;
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
   if ( IsOpen() == FALSE )
      lResult = OPOS_E_CLOSED;
   else if ( m_pService->IsClaimed() != FALSE ) // catch multiple claims
      m_pService->SetRC( OPOS_SUCCESS );
   else
      {
      // may throw, call first so device is not changed!
      m_pService->Claim( );      
      lResult = m_pDevice->Claim( lTimeout );
      if ( lResult != OPOS_SUCCESS )
         {
         m_pService->ReleaseClaim( );
         // Return timeout, but let device set result codes
         lResult = OPOS_E_TIMEOUT;
         }
      }
   return lResult;
   }

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::Release(
   void ) 
   {
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::Release"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_SUCCESS;
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
   if ( IsOpen() == FALSE )
      lResult = OPOS_E_CLOSED;
   else if ( m_pService->IsClaimed() == FALSE )
      lResult = m_pService->SetRC( OPOS_E_ILLEGAL );
   else
      {
      if ( m_pService->IsEnabled() != FALSE )
         Enable( FALSE );
      // Release Device, No events can then be placed in the queue
      m_pDevice->ReleaseClaim( );
      // Then release the service, which flushes the input queue!
      m_pService->ReleaseClaim();
      m_pService->SetRC( OPOS_SUCCESS );
      }
   return lResult;
   }


// TOposService<DEVICE_TYPE, SERVICE_TYPE>::OpenService
//
// Purpose: Open the service and device.
// Notes:   The Service Object developer must provide:
//          1) static DEVICE_TYPE* DEVICE_TYPE::CreateDevice( m_nPort, oRegistry, *m_pDc )
//          which returns a pointer to the created ( new-ed ) device.
//          2) static int DEVICE_TYPE::GetDevicePort( oRegistry, *m_pDc )
//          which returns a id (port) for the device from the registry.


template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::OpenService(
   LPCTSTR     sClass, 
   LPCTSTR     sName, 
   LPDISPATCH  pDispatch,
   LPCTSTR     sTargetClass) 
   {
   long lResult = OPOS_E_ILLEGAL;
   ASSERT( m_pService != NULL );
   m_pService->StartDataCapture( sName );
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::OpenService"),
      TRACE_COM_METHOD_ENTER );
   COposRegistry oRegistry( sClass, sName, *m_pDc );  // Create SO Registry
   
   // Check for multiple OpenService calls by the CO.
   if ( IsOpen() != FALSE )
      {
      // We don't expect that SO exists when not open.
      ASSERT( FALSE );
      m_pDc->DCPrintf( TRACE_OUTPUT_LEVEL4, _T("CO bug! Already opened!") );
      }
   else if ( CString( sClass ).CompareNoCase( sTargetClass ) != 0 )
      m_pDc->DCPrintf( TRACE_OUTPUT_LEVEL4, _T("Invalid Class - %s"), sClass );
   else if ( sName == NULL || *sName == 0 )
      m_pDc->DCPrintf( TRACE_OUTPUT_LEVEL4, _T("Invalid Device Name - \"%s\""), CString( sName ) );
   else
      {
      ASSERT( m_nPort == 0 );
      lResult = OPOS_E_NOSERVICE;
      if ( oRegistry.OpenKey( ) != FALSE )
         {
         ASSERT( oRegistry.ReadBool( _T("Debug") ) == FALSE );
         CString sSoDescription;
         sSoDescription.LoadString( IDS_SO_DESCRIPTION  );
         CString   sVersion;
         COposRegistry::GetFileVersionInfo( m_iidSo, sVersion, *m_pDc );
         oRegistry.WriteRegString( g_szNCRVersion, sVersion, IDS_FILE_VERSION_COMMENT );
         // Now Open the Service - May redirect the registry!
         if ( m_pService->Open( pDispatch, oRegistry, sSoDescription ) != FALSE ) 
            {
            m_pService->SetBuildVersion( sVersion );
            // All devices require Port Number, but is is no used
            // to share devices - devices are never shared. 
            m_nPort = DEVICE_TYPE::GetDevicePort( oRegistry, *m_pDc );
            if ( m_nPort != 0 )
               lResult = OPOS_SUCCESS;
            else
               m_pService->Close();
            }
         }
      }
   if ( lResult == OPOS_SUCCESS )
      {  // If everything still working ok, lets hook up the device
      ASSERT( m_nPort != 0 );
      ASSERT( IsOpen() == FALSE );
      lResult = m_pService->SetRC( OPOS_E_NOSERVICE );
      // or create new device, and if successful, update reference count
      DEVICE_TYPE* pDevice = DEVICE_TYPE::CreateDevice( m_nPort, oRegistry, *m_pDc );
      if ( pDevice == NULL )
         m_pService->Close();
      else
         {     // Device was created successfully, can it open?
         lResult = pDevice->Open( m_pService, oRegistry );
// This code stops device sharing at this level. 
// IsOpen() in DEVICE_TYPE::Open now always returns FALSE, 
// Hydras don't provide sharing of the same device class object. So we are really just detecting 
// whether the existing device class object exists and has been opened already. 
// Non-Hydras failed the DEVICE_TYPE::Open anyway, since they do not have the ability to be shared
         if ( lResult != OPOS_SUCCESS )
            {
            delete pDevice;
            m_pService->Close();
            }
         else     //  Now set the device pointer, all is working
            {
            m_pDevice = pDevice;
#ifdef INITIALIZE_SERVICE
            m_pService->SetDevice( pDevice );
#endif
#ifdef INITIALIZE_DEVICE
            m_pDevice->SetService( m_pService );
#endif
            }
         }
      }
   return lResult;
   }


// TOposService<DEVICE_TYPE, SERVICE_TYPE>::Close
//
// Purpose: Close the service object and device.

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::Close() 
   {
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::Close"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_E_CLOSED;
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );  // CO did not delete after SO close?
   if ( IsOpen() != FALSE )
      {
      if ( m_pService->IsClaimed() != FALSE )
         Release( );
      m_pDevice->Close( m_pService );
      delete m_pDevice;
      m_pDevice = NULL;          // Device deleted! We are closed
      m_pService->Close();       // Closed, but not deleted!
      // Called in case the control does not delete us
      // If they re-open, then we are covered!
      m_pService->InitializeProperties();
      lResult = OPOS_SUCCESS;
      }
   return lResult;
   }



// TOposService<DEVICE_TYPE, SERVICE_TYPE>::GetPropertyNumber
// TOposService<DEVICE_TYPE, SERVICE_TYPE>::SetPropertyNumber
//
// Purpose: Get/Set Property Number
// Notes:   If custom work required, such as some properties on the device
//          then do it before calling these functions. 

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::GetPropertyNumber(
   long lPropIndex) 
   {
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::GetPropertyNumber"),
      TRACE_COM_PROPGET_ENTER );
   long lResult = 0;
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
	if ( IsOpen() != FALSE )
		lResult = m_pService->GetPropertyNumber(lPropIndex);
   else if ( lPropIndex == PIDX_State )
      lResult = OPOS_S_CLOSED;
   else if ( lPropIndex == PIDX_ResultCode )
      lResult = OPOS_E_CLOSED;
   return lResult;
   }



template<class DEVICE_TYPE, class SERVICE_TYPE>
inline void TOposService<DEVICE_TYPE, SERVICE_TYPE>::SetPropertyNumber(
   long lPropIndex, 
   long lProperty) 
   {
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::SetPropertyNumber"),
      TRACE_COM_PROPPUT_ENTER );
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
	if ( IsOpen() != FALSE )
      {
      switch( lPropIndex )
         {
         case PIDX_DeviceEnabled:
            m_pService->SetRC( Enable( lProperty ) );
            break;
         default:
            m_pService->SetPropertyNumber(lPropIndex, lProperty);
            break;
         }
      }
   return;
   }


// TOposService<DEVICE_TYPE, SERVICE_TYPE>::GetPropertyString
// TOposService<DEVICE_TYPE, SERVICE_TYPE>::SetPropertyString
//
// Purpose: Get/Set Property String
// Notes:   If custom work required, then do it before calling these 
//          functions. 

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline BSTR TOposService<DEVICE_TYPE, SERVICE_TYPE>::GetPropertyString(
   long lPropIndex) 
   {
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::GetPropertyString"),
      TRACE_COM_PROPGET_ENTER );
   CString sResult;
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
	if ( IsOpen() != FALSE )
      sResult = m_pService->GetPropertyString(lPropIndex);
   else
      sResult = _T("[Error]");
	return sResult.AllocSysString();
   }

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline void TOposService<DEVICE_TYPE, SERVICE_TYPE>::SetPropertyString(
   long lPropIndex, 
   LPCTSTR sProperty ) 
   {
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::SetPropertyString"),
      TRACE_COM_PROPPUT_ENTER );
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
	if ( IsOpen() != FALSE )
      m_pService->SetPropertyString(lPropIndex, sProperty);
   return;
   }

// TOposService<DEVICE_TYPE, SERVICE_TYPE>::ClearInput
// TOposService<DEVICE_TYPE, SERVICE_TYPE>::ClearOutput
//
// Purpose: Clear IO
// Notes:   Input is saved at the service, ( in the event queue ) so
//          the device is not involved in ClearInput.
//          The device is called for ClearOutput, since he may need to 
//          abort an outstanding request. 

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::ClearInput() 
   {        // Does not report not claimed!
   long lResult = OPOS_E_CLOSED;
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
	if ( IsOpen() != FALSE )
      {
      lResult = OPOS_E_NOTCLAIMED;
      if ( m_pService->IsClaimed() != FALSE )
         {
         m_pService->ClearInput();
         lResult = OPOS_SUCCESS;
         }
      m_pService->SetRC( lResult  );
      }
   return lResult;
   }


template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::ClearOutput() 
   {        // Does not report not claimed!
   long lResult = OPOS_E_CLOSED;
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
	if ( IsOpen() != FALSE )
      {
      lResult = OPOS_E_NOTCLAIMED;
      if ( m_pService->IsClaimed() != FALSE )
         {
         m_pService->ClearOutput();    // Clear pending
         m_pDevice->ClearOutput();     // Clear Queued
         lResult = OPOS_SUCCESS;
         }
      m_pService->SetRC( lResult );
      }
   return lResult;
   }

// TOposService<DEVICE_TYPE, SERVICE_TYPE>::DirectIO
//
// Purpose: Send DirectIO to device, handle BSTR data
// Note:    Device must be claimed for DirectIO. See Claim Notes above.

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::DirectIO(
   long lCommand, 
   long FAR* pData, 
   BSTR FAR* pString) 
   {
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::DirectIO"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_SUCCESS;
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
	if ( IsOpen() == FALSE )
      lResult = OPOS_E_CLOSED;      // No need to set result code
   else if ( m_pService->IsClaimed() == FALSE )
      lResult = m_pService->SetRC( OPOS_E_NOTCLAIMED );
   else
      {
      // Note: We do not require device to be enabled!
      CString sData;
      if ( pString != NULL )
         sData = *pString;
      lResult = m_pDevice->DirectIO( lCommand, pData, sData );
      if ( pString != NULL )
         sData.SetSysString( pString );
      }
	return lResult;
   }

// TOposService<DEVICE_TYPE, SERVICE_TYPE>::COFreezeEvents
//
// Purpose: Process Freeze Events from the Control

template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::COFreezeEvents(
   BOOL bFreeze) 
   {
   CDataCapEntryExit DCE( m_pDc, m_sServiceClassName + _T("::COFreezeEvents"),
      TRACE_COM_METHOD_ENTER );
   m_pService->COFreezeEvents( bFreeze );
	return 0;
   }

// TOposService<DEVICE_TYPE, SERVICE_TYPE>::Enable
//
// Purpose: Enable/Disable the device
// Note:    Used internally, do not set ResultCode Here!


template<class DEVICE_TYPE, class SERVICE_TYPE>
inline long TOposService<DEVICE_TYPE, SERVICE_TYPE>::Enable( 
   BOOL bEnable )
   {
   long lResult = OPOS_E_NOTCLAIMED;
   // We don't expect that SO exists when not open.
   ASSERT( IsOpen() != FALSE );
   if ( IsOpen() == FALSE )
      lResult = OPOS_E_CLOSED;
   if ( m_pService->IsClaimed() != FALSE )
      {
		lResult = OPOS_SUCCESS;
      if ( m_pService->IsEnabled() != bEnable )
			{
			ClearOutput();                // Discard Output, Leave Input
			lResult = m_pDevice->Enable( bEnable );
			if ( lResult == OPOS_SUCCESS )
				m_pService->Enable( bEnable );
			}
	   ASSERT( lResult == OPOS_SUCCESS || bEnable != FALSE );
		// Disable should always succeed!
		}
   return lResult;
   }
