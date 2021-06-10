/////////////////////////////////////////////////////////////////////////////
//
// THealthCheck.h
//
// Declaration of COposCheckHealth class and 
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#include "Resource.h"

class COposCheckHealth : public COposEventReceiver
   {
   public:
      COposCheckHealth( void ) : COposEventReceiver( ) { }
      virtual long DoModal(
         LPCTSTR sDeviceName, 
         LPCTSTR sDeviceDescription, 
         LPCTSTR sSoDescription, 
         LPCTSTR nSoVersion ) = 0;
      virtual ~COposCheckHealth() {  }
      void AddEvent( COposEvent* pEvent )
         { m_qEvents.AddEvent( pEvent ); }
      void AddInputErrorEvent( long lRC, long nRCEx )
         { m_qEvents.AddInputErrorEvent( lRC, nRCEx ); }
   private:
      friend class COposService;
      COposCheckHealth( const COposCheckHealth& );        // Disable Copies
      COposCheckHealth& operator=( const COposCheckHealth& );

   };


template<class DEVTYPE, class DLGTYPE>
class TOposCheckHealth : public COposCheckHealth 
   {
   public:
      TOposCheckHealth( DEVTYPE* pDevice );
      virtual long DoModal(
         LPCTSTR sDeviceName, 
         LPCTSTR sDeviceDescription, 
         LPCTSTR sSoDescription, 
         LPCTSTR sSoVersion );
      // Dispatch events to the dialog
      virtual void DispatchErrorEvent( long nRC, long nRCEx, long nLocus);
      virtual void DispatchDataEvent( const CString& sData, long nResult );
      virtual void DispatchStatusUpdateEvent( long nResult );
      virtual void DispatchDirectIOEvent( 
         long lEventNumber, long* plData, CString& sData );
      virtual void DispatchOutputCompleteEvent( long nOutputID );
      virtual ~TOposCheckHealth( ) {}
   private:
      DLGTYPE*             m_pDlg;
      DEVTYPE*             m_pDevice;
   private:
      TOposCheckHealth( const TOposCheckHealth& );           // Disable Copies
      TOposCheckHealth& operator=( const TOposCheckHealth& );// Disable Copies

   };


template<class DEVTYPE, class DLGTYPE>
TOposCheckHealth<DEVTYPE, DLGTYPE>::TOposCheckHealth( 
   DEVTYPE* pDevice ) 
   : COposCheckHealth( ), 
   m_pDevice( pDevice ), 
   m_pDlg( NULL ) 
   {  
   ASSERT( pDevice != NULL );
   }

template<class DEVTYPE, class DLGTYPE>
long TOposCheckHealth<DEVTYPE, DLGTYPE>::DoModal(
   LPCTSTR sDeviceName, 
   LPCTSTR sDeviceDescription, 
   LPCTSTR sSoDescription, 
   LPCTSTR sSoVersion )
   {
   ASSERT( m_pDlg == NULL );
   ASSERT( sDeviceName != NULL );
   ASSERT( sDeviceDescription != NULL );
   ASSERT( sSoDescription != NULL );
   ASSERT( sSoVersion != NULL );
   DLGTYPE dlg( m_pDevice );
   dlg.m_sDeviceName         =  sDeviceName;
   dlg.m_sDeviceDescription  =  sDeviceDescription;
   dlg.m_sSoDescription      =  sSoDescription;
   dlg.m_sSoVersion.LoadString( IDS_VERSION_TEXT );
   dlg.m_sSoVersion += _T(" ");
   dlg.m_sSoVersion += sSoVersion;
   long lResult = OPOS_SUCCESS;
   // Need to fire events to ourselves on the main thread!
   m_qEvents.BeginService( this );     // May Throw
   m_pDlg = &dlg;
   m_qEvents.EnableEvents( TRUE, TRUE );
	int nModalResult = dlg.DoModal();
   if ( nModalResult == -1 || nModalResult == IDABORT )
      lResult = OPOS_E_FAILURE; // Dialog could not be shown!
   m_qEvents.EndService(  );
   m_pDlg = NULL;
   return lResult;
   }
      // Dispatch events to the dialog
template<class DEVTYPE, class DLGTYPE>
void TOposCheckHealth<DEVTYPE, DLGTYPE>::DispatchDataEvent( 
   const CString& sData, 
   long nResult )
   { 
   ASSERT( m_pDlg != NULL ); 
   m_pDlg->DispatchDataEvent( sData, nResult );
   return; 
   }

template<class DEVTYPE, class DLGTYPE>
void TOposCheckHealth<DEVTYPE, DLGTYPE>::DispatchStatusUpdateEvent( 
   long nResult )
   { 
   ASSERT( m_pDlg != NULL ); 
   m_pDlg->DispatchStatusUpdateEvent( nResult );
   return; 
   }


template<class DEVTYPE, class DLGTYPE>
void TOposCheckHealth<DEVTYPE, DLGTYPE>::DispatchDirectIOEvent( 
   long lEventNumber, 
   long* plData, 
   CString& sData )
   { 
   ASSERT( m_pDlg != NULL ); 
   m_pDlg->DispatchDirectIOEvent( lEventNumber, plData, sData );
   return; 
   }

template<class DEVTYPE, class DLGTYPE>
void TOposCheckHealth<DEVTYPE, DLGTYPE>::DispatchErrorEvent( 
   long nRC, 
   long nRCEx, 
   long nLocus)
   { 
   ASSERT( m_pDlg != NULL ); 
   m_pDlg->DispatchErrorEvent( nRC, nRCEx, nLocus );
   return; 
   }

template<class DEVTYPE, class DLGTYPE>
void TOposCheckHealth<DEVTYPE, DLGTYPE>::DispatchOutputCompleteEvent( 
   long nOutputID )
   { 
   ASSERT( m_pDlg != NULL ); 
   m_pDlg->DispatchOutputCompleteEvent( nOutputID );
   return; 
   }
