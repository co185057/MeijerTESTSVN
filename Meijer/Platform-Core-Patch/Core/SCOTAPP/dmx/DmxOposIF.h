
#ifndef _DMXOPOSIF_H_
#define _DMXOPOSIF_H_


#define REMOTE_ID_OFFSET 5
#define BASE_DEVID( n ) (10000 + (n) * REMOTE_ID_OFFSET * 2)
#define MAX_DEVID(x) (BASE_DEVID(x) + REMOTE_ID_OFFSET * 2 - 1)

#define CTRLID_TO_DEVCLASS( n ) ((n - 10000) /  REMOTE_ID_OFFSET / 2)
#define CTRLID_TO_DEVID( n )    ((n - 10000) %  REMOTE_ID_OFFSET)
#define CTRLID_IS_REMOTE( n )   (((n - 10000) %  (REMOTE_ID_OFFSET * 2)) >= REMOTE_ID_OFFSET)

class CMICRWrapper;
class CPrinterWrapper;
class CScannerWrapper;
class CKeylockWrapper;
class CMSRWrapper;
class CCashDrawerWrapper;
class CMotionSensorWrapper;
class CEASWrapper;
class CCashRecyclerWrapper;
class CScaleWrapper;
class CCardDispenser;		//card dispenser

#include "opos.h"

class CDeviceStatistics
   {
   public:
      CDeviceStatistics( void ) 
         : cStatus( DM_DA_NOT_CONFIGURED ),
         fDoEnable( false ),
         fIsEnabled( false )
         {}
      CDeviceStatistics( bool bDoEnable ) 
         : cStatus( DM_DA_NOT_CONFIGURED ),
         fDoEnable( bDoEnable ),
         fIsEnabled( false )
         {}
   public:
      CString csDescription;            // device description
      char cStatus;                     // '1' - ok, '2' - in error, from DeviceAvailability
      bool fDoEnable;                   // device supports enable and disable
      bool fIsEnabled;                  // true or false
   };

#include "DispatchHelper.h"

class CGenericOposIF : public CDispatchHelper
   {
   public:
      static BOOL sm_bSkipStartupCheckHealth;
   public:
      virtual ~CGenericOposIF( void ) { }
      long CreateOposControl( LPCTSTR sDeviceDescription, long nCtrlId, LPCTSTR sProfile, CWnd* pParentWnd );
      long OpenDevice( void );
      long CloseDevice( void );
      long OnCreateDevice( void );
      CDeviceStatistics GetDeviceStatistics( void );
	   virtual BOOL Create( CWnd* pParentWnd, UINT nID ) = 0;
   public:  // safe casting operations. 
      virtual CMICRWrapper* GetMICR( void ) { ASSERT( FALSE ); return NULL; }
      virtual CPrinterWrapper* GetPrinter( void ) { ASSERT( FALSE ); return NULL; }
      virtual CScannerWrapper* GetScanner( void ) { ASSERT( FALSE ); return NULL; }
      virtual CKeylockWrapper* GetKeylock( void ) { ASSERT( FALSE ); return NULL; }
      virtual CMSRWrapper* GetMSR( void ) { ASSERT( FALSE ); return NULL; }
      virtual CCashDrawerWrapper* GetCashDrawer( void ) { ASSERT( FALSE ); return NULL; }
      virtual CMotionSensorWrapper* GetMotionSensor( void ) { return NULL; }
      virtual CEASWrapper* GetEASDeactivator( void ) { return NULL; }
      virtual CCashRecyclerWrapper* GetCashRecycler( void ) { return NULL; }
      virtual CScaleWrapper* GetScale( void ) { return NULL; }
	  virtual CCardDispenser* GetCardDispenser( void ) { return NULL; }   //card dispenser
   public:
      virtual long ReadFirmwareVersion( CString& sVersion, long* lVersion )
         { return OPOS_E_ILLEGAL; }
      virtual void ReadRegistry( HKEY hKey );
      virtual long ClaimDevice( long lTimeout) { return OPOS_SUCCESS; }
      virtual long ReleaseDevice( ) { return OPOS_SUCCESS; }
      virtual long ClearInput(void) { return OPOS_SUCCESS; }
      virtual long ClearOutput(void) { return OPOS_SUCCESS; }
      virtual void OnTimer(UINT nIDEvent) { ASSERT( FALSE ); }
      virtual long DoOneTimeInitialization(void) { return OPOS_SUCCESS; }
   public:  // pure virtuals - must be overridden wrapper class
      virtual BOOL IsAvailable( void ) 
         { return m_deviceStatistics.cStatus == DM_DA_LOADED; }
      virtual long Open( LPCTSTR sProfile ) = 0;
      virtual long Close( void ) = 0;
      virtual BOOL GetDeviceEnabled( void ) = 0;
      virtual void SetDeviceEnabled( BOOL bEnable ) = 0;
      virtual BOOL GetDataEventEnabled( void ) { return FALSE; }
      virtual void SetDataEventEnabled( BOOL bEnable ) {}
      virtual long CheckHealth( long lLevel ) = 0;
      virtual long GetResultCode() = 0;
      virtual long GetDeviceClass( void ) const = 0;
      virtual long GetResultCodeExtended() = 0;
      virtual long DirectIO(long Command, long* pData, BSTR* pString) = 0;
      virtual CString GetDeviceDescription() = 0;
      // Event Translators - 
      // return non-zero (an event id) to be fired, else zero -> no event
      // May translate device class for printer stations (not data event)
      virtual void OnOposErrorEvent( long nResult, long nRcEx )
         { ASSERT( FALSE ); }
      virtual void OnOposStatusEvent( long nStatus )
         { ASSERT( FALSE ); }
      // Only fire an event if all requests have been completed
      // for the printer station. 
      virtual void OnOposOutputCompleteEvent( long nOutputId )
         { ASSERT( FALSE ); }
      virtual void OnOposDataEvent( long nStatus )
         { ASSERT( FALSE ); return; }
      virtual void OnOposDirectIOEvent( long nEventId, long* plData, BSTR* pbstrData )
         { ASSERT( FALSE ); return; }
   protected:
//      CGenericOposIF( );
      virtual bool IsInputDevice( void ) const { return false; }
   protected:
      long MapOposErrorToDM( long nResult );
   public:
      void PostDmEventToApplication( long nDeviceClass, long nDeviceId, 
         const CString& sData);
      void PostDmEventToApplication( long nEventType, long nDeviceClass, long nDeviceId, 
         const CString& sData, long lDirectIONumber,  long lDirectIONumData);
      void PostDmEventToApplication( long nEventType, long nDeviceClass, long nDeviceId, 
         long nStatusType );
	  virtual CString GetDeviceProfile() {return m_sProfile;}   //RFC 356020
   protected:
      CDeviceStatistics m_deviceStatistics;
      long              m_nDeviceId;
      CString           m_sProfile;
      UINT              m_nControlId;
   };

#endif // _DMXOPOSIF_H_
