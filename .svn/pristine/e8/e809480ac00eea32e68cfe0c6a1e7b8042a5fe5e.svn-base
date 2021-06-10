/////////////////////////////////////////////////////////////////////////////
//
// OposService.h
//
// Declaration of COposService class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifndef OPOS_SERVICE_H
#define OPOS_SERVICE_H

#define OPOS_REQUIRE_OPENED  0   // Always required
#define OPOS_REQUIRE_CLAIMED 1
#define OPOS_REQUIRE_ENABLED 2
#define OPOS_REQUIRE_IDLE    4   // Not Busy

// COposService
// This class is provided as a base implementation of a service object.
// You must derive a class specific to the device class that you are
// supporting.  The MFC generated class should have a pointer to 
// the derived class as a member variable. 
// Most functions do not need to be virtual since
// all users of the derived class ( except the OposEvent classes ) know the
// exact type and will call functions in the derived class.

/*
 * long     m_nState;
 *
 *    This long reflects the state of the service object as defined by OPOS
 *    CLOSED, IDLE, BUSY or ERROR 
 *
 * long     m_nOutputID;
 *
 *    Reflects the id assigned do the last output.  Derived class should 
 *    increment for each request.
 *
 * DWORD    m_bDataEventEnabled  : 1;
 *
 *    This bit implements the DataEventEnabled property.  It is set to FALSE
 *    automatically when a data event is dispatched.
 *
 * DWORD    m_bDeviceEnabled     : 1;
 *
 *    This bit implements the DeviceEnabled property.  It is set by the
 *    device class.
 *
 * DWORD    m_bFreezeEvents      : 1;
 *
 *    This bit is set by the application through the control.  When TRUE,
 *    no events can be sent by the SO. ( see UpdateEventState )
 *
 * DWORD    m_bOkToDelete        : 1;
 *
 *    This bit indicates that the SO is being deleted corectly, through its
 *    Delete() method, not through the delete operator. 
 *
 * DWORD    m_bAutoDisable       : 1;
 *
 *    This bit implements the AutoDisable property.  It is set by the 
 *    application and is available to the device. 
 *
 * DWORD    m_bClaimed           : 1;
 *
 *    This bit is set by the device and may be retrieved by the application.
 *    If a derived SO does not support the Claim method, then it should
 *    set claim to TRUE when it is opened so that events may be fired!
 *
 * CString  m_sCheckHealthText;
 *
 *    This string implements the CheckHealthText property. 
 *
 * long     m_nResultCode;
 * long     m_nResultCodeExtended;
 *
 *    These longs implement the ResultCode and ResultCodeExtended properties.
 *    They are normally ( but not always ) set by the device, since the device
 *    determines what the extended result codes are. 
 *
 * long     m_nBinaryConversion;    
 *
 *    This long tracks the method of binary conversion.  It is not used
 *    by this class. 
 *
 * CString  m_sDeviceName;
 * CString  m_sDeviceDescription;
 *    
 *    These strings implement the DeviceName and DeviceDescription properties, 
 *    and are set by the device. 
 *
 * CString  m_sSoDescription;
 * long     m_nSoVersion;
 * CString  m_sSoVersion;
 *
 *    These strings implement the ServiceObjectDescription and 
 *    ServiceObjectVersion properties.  They are set through a string in the
 *    SO's resource file "Service Object Description\nVersion Number"
 *    The "\n" is required. 
 *
 * CCriticalSection     m_csAccess;
 *
 *    A critical section to control access to the device. 
 *
 * COposCheckHealth*    m_pCheckHealth;
 *
 *    A pointer to the health check object, if in an interactive health check.
 *    Events are queued to the health check object rather than to the SO when 
 *    a health check is active.
 *
 * CmDataCapture        m_cmDc;
 *
 *    The ever-present data capture object.  This object ( the SO ) exists
 *    longer than any other object.  So the DC was put here. Multi-function
 *    devices have one DC per SO, so they get their choice of where they
 *    capture to when more than one SO is open. They could select based 
 *    on a priority of their chhosing, or they could base it on what API they
 *    are in, or some other scheme. 
 *
 * IGenericCO           m_iCo;
 *
 *    An interface to the CO, for event dispatching.
 *
 * long                 m_nCOFreezeEvents;
 *
 *    A counter that is set when the container ( not the application ) 
 *    requests that events be frozen.
 *
 * CString              m_sPrefix;
 *
 *    The current data capture prefix.
 *
 * const CString        m_sClass;
 *    
 *    The device class of this service object 
 *    ( determined by the derived class.)
 *
 */

class COposService : protected COposEventReceiver
   {
   public:
      virtual ~COposService( void );
      long     SetRC( long nRC = OPOS_SUCCESS, long nRCEx = 0 );
      void     Delete( void );
      CmDataCapture& GetDCap( void ) { return m_cmDc; }
      void StartDataCapture( LPCTSTR sName );
      void SetDcPrefix(LPCTSTR sTitle, int nPort, int nId );
      BOOL DcapProperty( long lPropIndex, long lPidxBase, LPCTSTR  szNames[], DWORD nCountNames);

      void COFreezeEvents( BOOL bFreeze );
      BOOL Open( LPDISPATCH pDispatch, COposRegistry& oRegistry, 
         const CString& sSoDescription );
      void SetBuildVersion( LPCTSTR sBuildVersion )
         { m_sBuildVersion = sBuildVersion; }
      CString GetBuildVersion( void ) const 
         { return m_sBuildVersion; }
      long Close( void );
      void SetCheckHealthText( int nId );
      // Event Operations should be called from the device only
      void AddEvent( COposEvent* pEvent, BOOL bAddAtHead = FALSE );
      void AddInputErrorEvent( long lRC, long nRCEx );
      BOOL IsOpen( void ) const { return m_nState != OPOS_S_CLOSED; }
      BOOL IsEnabled( void ) const { return m_bDeviceEnabled; }
      BOOL IsClaimed( void ) const { return m_bClaimed;  }
      BOOL AutoDisable( void );
      void SetDeviceName( int nId );
      virtual long InteractiveHealthCheck( COposCheckHealth* pCheckHealth );
      long OutputOk( void );
   protected:  
      // Constructor: You must derive a class to use this class
      COposService( LPCTSTR sPrefix );
		// You must return your Service Object Class through this function:
		virtual CString GetServiceObjectClassName( void ) const = 0;
// Common Operations
// These are protected to make sure that the only valid callers are
// 1. The derived class
// 2. Friends of ( 1. ) ( Should be TOposService<D,S> )
	   long     GetPropertyNumber( long lPropIndex ) ;
	   CString  GetPropertyString( long lPropIndex ) ;
	   void     SetPropertyNumber(long lPropIndex, long lProperty);
	   void     SetPropertyString(long lPropIndex, LPCTSTR sProperty);
      void     Claim( void );
      void     ReleaseClaim( void );
      void AddRequest( COposRequest* pRequest );
      virtual void Enable( BOOL bEnable );
      virtual BOOL ReadRegistry( COposRegistry& oRegistry ) { return TRUE; }
      // Derived must provide to initialize properties
      virtual void InitializeProperties( void ) = 0;
      // Virtuals called on derived class from base class
      virtual void ClearOutput( void );
      virtual void RetryOutput( void );
      virtual BOOL IsExclusive( void ) const { return TRUE; }
      virtual BOOL IsBusy( void ) { return FALSE; }
      // Derived classed with async output should override and clear/retry output
      virtual void ClearInput( void );
      CString ConvertAsciiToBinary( const CString& sData ) const;
      CString ConvertBinaryToAscii( const CString& sData ) const;
      long CheckMethodValid( int nRequirements = OPOS_REQUIRE_OPENED ) const;
// virtual Dispatch Functions from COposEventReceiver
   protected:
      virtual void DispatchErrorEvent( long nRC, long nRCEx, long nLocus );
//original      virtual void DispatchDataEvent( const CString& sData, long lStatus );
//antonio start
	  virtual void DispatchDataEvent( LPCTSTR sData, long lStatus );
//antonio end
      virtual void DispatchStatusUpdateEvent( long lStatus );
      virtual void DispatchDirectIOEvent( long lEventNumber, 
         long* plData, CString& sData);
      virtual void DispatchOutputCompleteEvent( long lOutputID );
   protected:  // Common Members - Accessible to Derived Classes- 
               // use csAccess to synchronize access to these 
      mutable CCriticalSection     m_csAccess;
      COposCheckHealth*    m_pCheckHealth;
      mutable CmDataCapture  m_cmDc;
   private:    // Implementation Members
      DWORD    m_bDataEventEnabled  : 1;
      DWORD    m_bDeviceEnabled     : 1;
	   DWORD    m_bFreezeEvents      : 1;
      DWORD    m_bOkToDelete        : 1;
      DWORD    m_bAutoDisable       : 1;
    	DWORD    m_bClaimed           : 1;
		DWORD		m_bEventFired			: 1;
      long     m_nOutputID;
      CString  m_sCheckHealthText;
	   long     m_nResultCode;
	   long     m_nResultCodeExtended;
      CString  m_sDeviceDescription;
      CString  m_sSoDescription;
      long     m_nSoVersion;
      CString  m_sSoVersion;
      long     m_nState;
      CString  m_sDeviceName;
      CString  m_sBuildVersion;
      long     m_nBinaryConversion;    
   private:
      IGenericCO           m_iCo;
      long                 m_nCOFreezeEvents;
      CString              m_sPrefix;
   private:    // Always called internally, under csAccess protection
      void UpdateEventState( void );
   private:
      DECLARE_COPY_INHIBIT( COposService )    // Disable Copies
   };         

#ifdef _OPOS_ENABLE_INLINES
#define OPOS_INLINE inline
#include "OposService.inl"
#endif

#ifdef _DEBUG 
#define DCAPPROPERTY( a,b,c,d )  DcapProperty( a,b,c,d ) 
#else
#define DCAPPROPERTY( a,b,c,d )  DcapProperty( a,b, NULL, 0 ) 
#endif

#endif