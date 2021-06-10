/////////////////////////////////////////////////////////////////////////////
//
// TControl.h
//
// Template for implementing OPOS control class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifndef OPOS_CONTROL_H
#define OPOS_CONTROL_H

#include "resource.h"
// The m_bVersionError variable gets set after a version error.  
// This value for the ResultCode property after version unsupported properties and methods
//#define OPOS_VERSION_ERROR  OPOS_E_NOSERVICE
#define OPOS_VERSION_ERROR  OPOS_SUCCESS

template<class TYPE>
class TOposControl
   {
   public:
      TOposControl( const IID& nIID, LPCTSTR sControlClassName, LPCTSTR  szPrefix );
      ~TOposControl();
      TYPE& GetService( void ) { ASSERT( m_bOpened != FALSE ); m_bVersionError = FALSE; return m_iService; }
      BOOL IsOpen( void ) const { return m_bOpened; }
      long Open(LPCTSTR szClass, LPCTSTR sDeviceName, LPDISPATCH lpDispatch);
      BOOL ServiceGetNames( LPCTSTR szClass, CString& sName, CString& sSoName );
      BOOL ServiceConnect( const CString& sSoName );
      void ServiceDisconnect( void );
      long ServiceOpen( LPCTSTR szClass, LPCTSTR szName, LPDISPATCH lpDispatch );
      long Close( void );
      BSTR GetPropertyString( long nPidx, long nSoMajor = 1, long nSoMinor = 0 );
      long GetPropertyLong( long nPidx, long nSoMajor = 1, long nSoMinor = 0 );
      long GetPropertyBool( long nPidx, long nSoMajor = 1, long nSoMinor = 0 );
      void SetPropertyLong( long nPidx, long nNewValue, long nSoMajor = 1, long nSoMinor = 0 );
      void SetPropertyBool( long nPidx, BOOL bNewValue, long nSoMajor = 1, long nSoMinor = 0 );
      void SetPropertyString( long nPidx, LPCTSTR lpszNewValue, long nSoMajor = 1, long nSoMinor = 0 );
      void OnFreezeEvents( BOOL bFreeze );
      long Claim( long lTimeout );
      long Release( void );
      long CheckHealth( long lLevel );
      long DirectIO( long lCommand, long FAR* pData, BSTR FAR* pString);
      long ClearInput( void );
      long ClearOutput( void );
      BSTR GetControlObjectDescription() const;
      long GetControlObjectVersion() const;
      CmDataCapture& GetDCap( void ) { return m_cmDc; }
      void OnDraw( COleControl& ctlControl,CDC* pdc, const CRect& rcBounds, int nBitMapId );
      long SoVersionOk( long nSoMajor, long nSoMinor, BOOL bVersionError = TRUE);
   private:
      CString	m_sCoDescription;
      long	   m_nCoVersion;
      long     m_nSoMajor;                    // Service Object major version.
      long     m_nSoMinor;                    // Service Object minor version.
      BOOL     m_bVersionError;               // TRUE if a version error has occurred.
      const IID& m_iidCo;                     // GUID of the control
// This flag tells us whether or not the CO has been opened.
	   BOOL m_bOpened;
// This variable counts the number of times OnFreezeEvents(TRUE) is called.
      UINT m_nCoFreezeEventCount;
// This object maintains the connection between the CO and the SO
      TYPE m_iService;
// This object is used for Data Capture
      CmDataCapture m_cmDc;
      static LPCTSTR g_szDefaultStringProperty;
   private:
      const CString m_sControlClassName;  // For Data Capture
      operator TYPE();  // Disable!
      operator TYPE&(); // Disable!
   };

template<class TYPE>
LPCTSTR TOposControl<TYPE>::g_szDefaultStringProperty = _T("[Error]");         


// TOposControl<TYPE>::SoVersionOk
// 
// Purpose: Manage m_bVersionError property.
// Notes:   SetProperty and Method calls pass TRUE for bVersionError.  They clear
//          m_bVersionError and will set it if there is a version error.
//          GetProperty calls pass FALSE for bVersionError.  They do not clear
//          m_bVersionError and will set it if there is a version error.
//
// Note to users:
//       When you call GetService(), the version error flag is cleared since you 
//       are probably calling a 1.0 method.  If you are calling a post 1.0 method, 
//       then you must call SoVersionOk(). If this returns OPOS_SUCCESS, then 
//       you may call GetService() because the version error flag was cleared by 
//       SoVersionOk().  
//       
// Note to maintenance:
//       Internally, we call GetService() if we are handling a common method call
//       to achieve the same results as users.  But we use m_iService directly
//       if we know it is clear because we have just called SoVersionOk or GetService(),
//       or when we do not want to clear it.


template<class TYPE>
long TOposControl<TYPE>::SoVersionOk( 
   long nSoMajor, 
   long nSoMinor, 
   BOOL bVersionError /* = TRUE */)
   { 
   long lResult = OPOS_SUCCESS;
   if ( bVersionError != FALSE )       // Clear Version Error if Settable Property
      m_bVersionError = FALSE;
   if ( nSoMajor > m_nSoMajor          // Greater major version required or
      || ( nSoMajor == m_nSoMajor && nSoMinor > m_nSoMinor ) )  // Greater Minor?
      {
      lResult = OPOS_VERSION_ERROR;
      // If the current operation can set the ResultCode, then flag the error.
      if ( bVersionError != FALSE )
         m_bVersionError = TRUE;
      }
   return lResult;
   }


template<class TYPE>
TOposControl<TYPE>::TOposControl(
   const IID& nIID,
   LPCTSTR  sControlClassName,
   LPCTSTR  szPrefix )
   : m_nCoVersion( 0 ),
   m_bOpened( FALSE ),
   m_nCoFreezeEventCount( 0 ),
   m_sControlClassName( sControlClassName ),
   m_bVersionError( FALSE ),
   m_nSoMajor( 0 ),
   m_nSoMinor( 0 ),
   m_iidCo( nIID )
   {
   ASSERT( szPrefix != NULL );
   m_sCoDescription.LoadString( IDS_CO_DESCRIPTION );
   int nNlIndex = m_sCoDescription.Find( _T('\n') ) ;
   ASSERT( nNlIndex != -1 );   // Must have newline
   m_nCoVersion = _ttol( m_sCoDescription.Mid( nNlIndex + 1 ) );
   m_sCoDescription = m_sCoDescription.Left( nNlIndex );
   // Initialize data capture.
   m_cmDc.SetCaptureControl( NCRCAP_OFF ); 
   m_cmDc.SetPrefix( szPrefix );
   return;
   }

template<class TYPE>
TOposControl<TYPE>::~TOposControl(
   void) 
   { 
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::~TOposControl") );
   if ( m_bOpened != FALSE )
      Close();
   }

template<class TYPE>
long TOposControl<TYPE>::GetPropertyBool(                               
   long nPidx,
   long nSoMajor /* = 1 */, 
   long nSoMinor /* = 0 */ )
   {                                                              
   BOOL bResult = FALSE;                                          
   // AutoDisable is a 1.2 property
   ASSERT( nPidx != PIDX_AutoDisable || ( nSoMajor == 1 && nSoMinor == 2 ));
                                                                  
	if ( m_bOpened != FALSE 
      && SoVersionOk( nSoMajor, nSoMinor, FALSE ) == OPOS_SUCCESS )
		bResult = m_iService.GetPropertyNumber( nPidx );            
   ASSERT( bResult == TRUE || bResult == FALSE );                 
   return bResult;                                                
   }


template<class TYPE>
void TOposControl<TYPE>::SetPropertyBool(
   long nPidx,
   BOOL bNewValue,
   long nSoMajor /* = 1 */, 
   long nSoMinor /* = 0 */ )
   {
   ASSERT( bNewValue == TRUE || bNewValue == FALSE );
   // AutoDisable is a 1.2 property
   ASSERT( nPidx != PIDX_AutoDisable || ( nSoMajor == 1 && nSoMinor == 2 ));
	if ( m_bOpened != FALSE 
      && SoVersionOk( nSoMajor, nSoMinor, TRUE ) == OPOS_SUCCESS )
		m_iService.SetPropertyNumber( nPidx, (long)bNewValue);
   	// SetModifiedFlag(); // Not required for OPOS controls
   return;
   }

// TOposControl<TYPE>::GetPropertyString
// TOposControl<TYPE>::SetPropertyString
//
// Purpose: Get/Set SO properties, if open!
// Note:    Set does not change ResultCode, since when we are closed,
//          the ResultCode is always OPOS_E_CLOSED.

template<class TYPE>
BSTR TOposControl<TYPE>::GetPropertyString(
   long nPidx,
   long nSoMajor /* = 1 */, 
   long nSoMinor /* = 0 */ )
   {                                                              
   CString sResult;                                               
                                                                  
   if ( m_bOpened != FALSE                                       
      && SoVersionOk( nSoMajor, nSoMinor, FALSE ) == OPOS_SUCCESS )
      sResult = m_iService.GetPropertyString( nPidx );            
   else                                                           
      sResult = g_szDefaultStringProperty;                        
   return sResult.AllocSysString();
   }                                                              

template<class TYPE>
void TOposControl<TYPE>::SetPropertyString(                             
   long nPidx,                                                    
   LPCTSTR lpszNewValue,
   long nSoMajor /* = 1 */, 
   long nSoMinor /* = 0 */ )
   {                                                              
   if ( m_bOpened != FALSE 
      && SoVersionOk( nSoMajor, nSoMinor, TRUE ) == OPOS_SUCCESS )
      m_iService.SetPropertyString( nPidx, lpszNewValue);         
   return;                                                        
   }                                                              

// TOposControl<TYPE>::GetPropertyLong
// TOposControl<TYPE>::SetPropertyLong
//
// Purpose: Return SO property, or if closed, return 0, unless the 
//          State property is requested, then return OPOS_S_CLOSED.
//          If ResultCode is requested and we are closed, 
//          then return OPOS_E_CLOSED!

template<class TYPE>
long TOposControl<TYPE>::GetPropertyLong(
   long nPidx,
   long nSoMajor /* = 1 */, 
   long nSoMinor /* = 0 */ )
   {                                                              
   long lResult = 0;                        
   // Binary conversion is a 1.2 property
   ASSERT( nPidx != PIDX_BinaryConversion || ( nSoMajor == 1 && nSoMinor == 2 ));
   // Data Count is a 1.2 property
   ASSERT( nPidx != PIDX_DataCount || ( nSoMajor == 1 && nSoMinor == 2 ));
                                                                  
	if ( m_bOpened != FALSE )
      {
      BOOL bSet = FALSE;
      switch( nPidx )
         {
         case PIDX_ResultCode:
            lResult = OPOS_VERSION_ERROR;
         case PIDX_ResultCodeExtended:
            bSet = m_bVersionError;    // If version error, then don't call SO
            break;         
         }
      if ( bSet == FALSE 
         && SoVersionOk( nSoMajor, nSoMinor, FALSE ) == OPOS_SUCCESS )
		   lResult = m_iService.GetPropertyNumber( nPidx );            
      }
   else if ( nPidx == PIDX_State )
      lResult = OPOS_S_CLOSED;
   else if ( nPidx == PIDX_ResultCode )
      lResult = OPOS_E_CLOSED;
   return lResult;                                                
   }                                                              

template<class TYPE>
void TOposControl<TYPE>::SetPropertyLong(
   long nPidx,                                                    
   long nNewValue,
   long nSoMajor /* = 1 */, 
   long nSoMinor /* = 0 */ )
   {                                                              
	if ( m_bOpened != FALSE 
      && SoVersionOk( nSoMajor, nSoMinor, TRUE ) == OPOS_SUCCESS )
		m_iService.SetPropertyNumber( nPidx, nNewValue );           
   return;                                                        
   }                                                              


template<class TYPE>
BOOL TOposControl<TYPE>::ServiceConnect( 
   const CString& sSoName )
   {
   return m_iService.Connect( sSoName, m_cmDc );
   }

template<class TYPE>
void TOposControl<TYPE>::ServiceDisconnect( 
   void )
   {
   m_iService.Disconnect();                                    
   }

template<class TYPE>
long TOposControl<TYPE>::ServiceOpen( 
   LPCTSTR szClass,
   LPCTSTR szName,
   LPDISPATCH lpDispatch )  
   {
   long lResult =  m_iService.OpenService( szClass, szName, lpDispatch);                       
	if ( lResult == OPOS_SUCCESS )
      {                                                           
      ASSERT( m_bOpened == FALSE );
      m_bOpened = TRUE;            
      // Need to use m_iService so that no version checking is performed.
      // We don't know what the version number is yet!
      LONG nSOVersion = m_iService.GetPropertyNumber( PIDX_ServiceObjectVersion );
      m_nSoMajor = nSOVersion / 1000000;
      m_nSoMinor = ( nSOVersion / 1000 ) % 1000;
      m_cmDc.DCPrintf( TRACE_ALWAYS, ">Open: SO OPOS Compliant Version is %d.%d.%d",
         m_nSoMajor, m_nSoMinor, nSOVersion % 1000 );
      if ( m_nSoMajor != 1 
         || m_iService.ValidateVersionMethods( m_nSoMajor, m_nSoMinor, m_cmDc ) == FALSE )
         {
         m_cmDc.DCPrintf( TRACE_ALWAYS, ">Open: Bad SO Version -- Major Version not 1" );
         Close();
         lResult = OPOS_E_NOSERVICE;
         }
      else if ( m_nCoFreezeEventCount != 0 )                           
         m_iService.COFreezeEvents(TRUE);                         
      }                                                           
   else                                                           
      m_cmDc.DCPrintf( TRACE_ALWAYS, _T("*Failed: SO OpenService") );                         
   return lResult;
   }

template<class TYPE>
BOOL TOposControl<TYPE>::ServiceGetNames( 
   LPCTSTR szClass,
   CString& sName,
   CString& sSoName )
   {
   BOOL bResult = FALSE;
   COposRegistry oRegistry( szClass, sName, m_cmDc );              
   if ( oRegistry.OpenKeyCO( ) != FALSE )                              
      {      
      sSoName = oRegistry.GetSoName( );                             
      if ( sSoName.IsEmpty() == FALSE )
         {                                                           
         sName = oRegistry.GetName();                               
         m_cmDc.DCPrintf( TRACE_ALWAYS,                              
            _T(">SO is %s - \"%s\""), (LPCTSTR)sName, (LPCTSTR) sSoName ); 
         bResult = TRUE;
         }              
      }
   return bResult;
   }

// TOposControl<TYPE>::Open
//
// Purpose: Make connection to SO in standard manner!

template<class TYPE>
long TOposControl<TYPE>::Open(
   LPCTSTR szClass,
   LPCTSTR szName,
   LPDISPATCH lpDispatch )  
   {
   // Don't allow a second open to change DC settings
   if ( m_bOpened == FALSE )                                            
      {
      // Will the Key map correctly and open?
      COposRegistry oRegistry( szClass, szName, m_cmDc );              
      if ( oRegistry.OpenKeyCO( ) != FALSE )
         m_cmDc.ReadRegistry( CString( OPOS_ROOTKEY "\\" ) + szClass + "\\" + oRegistry.GetName() );
      }
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::Open"),              
      TRACE_COM_METHOD_ENTER );                                         
   m_cmDc.DCPrintf( TRACE_ALWAYS,  _T("Class <%s>, Name <%s>"), szClass, szName );
#ifdef _DEBUG
	m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Built: %s, Build type: Debug"),__DATE__);
#else
	m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Built: %s, Build type: Release"),__DATE__);
#endif
   // Data Capture the cONTROL dESCRIPTION.
   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("..Control Object Description is <%s>"), (LPCTSTR)m_sCoDescription);
   // Data Capture the Supported OPOS version of the Control.
   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("..Control Compliant With OPOS Spec Version <%d.%d.%d>"), 
      m_nCoVersion / 1000000,
		(m_nCoVersion / 1000) % 1000, 
      m_nCoVersion % 1000);
   CString    sVersion;
   COposRegistry::GetFileVersionInfo( m_iidCo, sVersion, m_cmDc );
   long lResult = OPOS_E_ILLEGAL;                                       
   CString sSoName;
   CString sName = szName;                                                       
   if ( m_bOpened == FALSE )                                            
      {                                                                 
      lResult = OPOS_E_NOEXIST;                                         
      if ( ServiceGetNames( szClass, sName, sSoName) != FALSE )
         {                                                                 
         lResult = OPOS_E_NOSERVICE;                                       
         if ( ServiceConnect( sSoName ) != FALSE )             
            {
            lResult =  ServiceOpen( szClass, sName, lpDispatch);
            if ( lResult != OPOS_SUCCESS )
               ServiceDisconnect();
            }
         else                                                              
            m_cmDc.DCPrintf( TRACE_ALWAYS, _T("*Failed: Create Dispatch") );  
         }                                                                 
      }
   return lResult;
   }

// TOposControl<TYPE>::Close
//
// Purpose: Close the SO in the standard manner!

template<class TYPE>
long TOposControl<TYPE>::Close(
   void )  
   {
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::Close"),          
      TRACE_COM_METHOD_ENTER );                                      
   long lResult = OPOS_E_CLOSED;                                     
                                                                  
   if ( m_bOpened != FALSE )                                         
      {                                                              
      lResult = GetService().Close();                                  
      ASSERT( lResult == OPOS_SUCCESS );                             
      m_iService.Disconnect();                                       
      m_bOpened = FALSE;                                             
      }                                                              
   return lResult;
   }



// TOposControl<TYPE>::OnFreezeEvents
//
// Purpose: Handle OnFreezeEvents in the standard manner!


template<class TYPE>
void TOposControl<TYPE>::OnFreezeEvents(
   BOOL bFreeze) 
   {
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::OnFreezeEvents"),
      TRACE_COM_METHOD_ENTER );
	m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, 
      _T(">Container Freeze: Parm %d, Count %d"), bFreeze, 
      m_nCoFreezeEventCount );
   if (bFreeze != FALSE)
      {
      if ( m_nCoFreezeEventCount++ == 0 )    // First Freeze!
         {
         if (m_bOpened != FALSE)
            m_iService.COFreezeEvents( TRUE );
         }
      }
   else if ( --m_nCoFreezeEventCount == 0 )  // Last Freeze!
      {
      if (m_bOpened != FALSE)
         m_iService.COFreezeEvents( FALSE );
      }
   return;
   }


// TOposControl<TYPE>::Claim
// TOposControl<TYPE>::Release
 
template<class TYPE>
long TOposControl<TYPE>::Claim(
   long lTimeout) 
   {
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::Claim"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_E_CLOSED;
	if ( m_bOpened != FALSE )
      lResult = GetService().Claim(lTimeout);
   return lResult;
   }

template<class TYPE>
long TOposControl<TYPE>::Release() 
   {
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::Release"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_E_CLOSED;

	if ( m_bOpened != FALSE )
	   lResult = GetService().Release();
   return lResult;
   }


// TOposControl::CheckHealth
//
// Purpose: Invoke SO method if open. 

template<class TYPE>
long TOposControl<TYPE>::CheckHealth(
   long lLevel) 
   {
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::CheckHealth"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_E_CLOSED;

	if ( m_bOpened != FALSE )
		lResult = GetService().CheckHealth(lLevel);
	return lResult;
   }



// TOposControl::DirectIO
//
// Purpose: Invoke SO method if open. 

template<class TYPE>
long TOposControl<TYPE>::DirectIO(
   long lCommand, 
   long FAR* pData, 
   BSTR FAR* pString) 
   {
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::DirectIO"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_E_CLOSED;

	if ( m_bOpened != FALSE )
		lResult = GetService().DirectIO(lCommand, pData, pString);
	return lResult;
  }

template<class TYPE>
long TOposControl<TYPE>::ClearInput( 
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::ClearInput"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_E_CLOSED;

	if ( m_bOpened != FALSE )
		lResult = GetService().ClearInput( );
	return lResult;
   }

template<class TYPE>
long TOposControl<TYPE>::ClearOutput( 
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, m_sControlClassName + _T("::ClearOutput"),
      TRACE_COM_METHOD_ENTER );
   long lResult = OPOS_E_CLOSED;

	if ( m_bOpened != FALSE )
		lResult = GetService().ClearOutput( );
	return lResult;
   }

// TOposControl<TYPE>::GetControlObjectDescription
// TOposControl<TYPE>::GetControlObjectVersion()

template<class TYPE>
BSTR TOposControl<TYPE>::GetControlObjectDescription() const
   {
   return m_sCoDescription.AllocSysString();
   }

template<class TYPE>
long TOposControl<TYPE>::GetControlObjectVersion() const
   {
   return m_nCoVersion;
   }

// TOposControl<TYPE>::OnDraw
//
// Purpose: Standard OnDraw code for Opos controls!

template<class TYPE>
void TOposControl<TYPE>::OnDraw( 
   COleControl& ctlControl,
   CDC* pdc, 
   const CRect& rcBounds, 
   int nBitMapId )
   {
   int cx, cy;
   ctlControl.GetControlSize(&cx, &cy);
   if (cx != 28 
      || cy != 28 )
      ctlControl.SetControlSize(28, 28);
   CBitmap bitmap;
   BITMAP  bmp;
   CPictureHolder picHolder;

   // Load button bitmap
   bitmap.LoadBitmap( nBitMapId );
   bitmap.GetObject(sizeof(BITMAP), &bmp);
   CRect rcSrcBounds;
   rcSrcBounds.right    = bmp.bmWidth;
   rcSrcBounds.bottom   = bmp.bmHeight;

   // Create picture and render
   picHolder.CreateFromBitmap((HBITMAP)bitmap.m_hObject, NULL, FALSE);

   // Always render the picture using the original size of the bitmap so
   // that the intermediate repaint will still look correct if we have
   // to resize ourselves.
   CRect rcDestBounds(rcBounds.TopLeft(),
      CSize(rcSrcBounds.right, rcSrcBounds.bottom) );
   picHolder.Render(pdc, rcDestBounds, rcSrcBounds);
   return;
   }

#endif