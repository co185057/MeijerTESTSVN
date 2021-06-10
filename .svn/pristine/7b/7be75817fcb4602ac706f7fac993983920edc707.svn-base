// TBService.h: interface for the CTBService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TBSERVICE_H__2B5E45C2_37E8_11D5_9C83_0090276FD28D__INCLUDED_)
#define AFX_TBSERVICE_H__2B5E45C2_37E8_11D5_9C83_0090276FD28D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LDWindow.h"
#include "PrinterProperties.h"
#include "MicrProperties.h"
#include "MsrProperties.h"
#include "ExpiringInt.h"
#include "FiscalProperties.h"
#include "ToneIndicatorMsg.h"

class _globalInfo
{
public:
   _globalInfo()
   {
      g_hReceiveThread= INVALID_HANDLE_VALUE;
	   InitializeCriticalSection(&CritSecAcknowledge);
	   hConsumed = CreateEvent(NULL,TRUE,FALSE,NULL);  //manual reset, initialize to FALSE
   }
   ~_globalInfo()
   {
       if ( NULL != hConsumed )
       {
          CloseHandle(hConsumed);
          hConsumed = NULL;
       }
       if ( INVALID_HANDLE_VALUE != g_hReceiveThread )
       {
           CloseHandle(g_hReceiveThread);
           g_hReceiveThread = INVALID_HANDLE_VALUE;
       }
	   DeleteCriticalSection(&CritSecAcknowledge);
   }
   CRITICAL_SECTION CritSecAcknowledge;

   HANDLE hConsumed;
   HANDLE g_hReceiveThread;  // handle of the receive thread
   CExpiringInt TimerForSO;
};

typedef struct
{
	long lCapAudience;
	long lCapBlink;
	BOOL bCapBrightness;
	long lCapCharacterSet;
	BOOL bCapDescriptors;
	BOOL bCapHMarquee;
	BOOL bCapICharWait;
	BOOL bCapVMarquee;
	long lDeviceWindows;
	long lDeviceRows;
	long lDeviceColumns;
	long lDeviceDescriptors;
	long lDeviceBrightness;
	long lCharacterSet;
	long lCurrentWindow;
	CString sCharacterSetList;
} LineDisplayProperties;


class CTBService : public COposService  
{
public:
	CTBService();
	virtual ~CTBService();

	CString	m_sClassName;	// Set by the MFC SO

	void	SetDeviceName( int nId );
	CString ConvertBinary( CString& sData );
	CString ConvertASCII( CString& sData );

	// Scanner related functions
	void ProcessScanData( LPCTSTR strData );

	//line display
	virtual void InitializeLineDisplay( LineDisplayProperties ldProp );
	long ClearDescriptors( void );
	long SetDescriptor( long Descriptor, long Attribute );
	long ClearText();
	long CreateWindow( long Rows, long Columns );
	long DestroyWindow( void );
	long DisplayTextAt(long lRow, long lColumn, LPCTSTR sData );
	BOOL GetDescriptor( long dwDescriptor, BYTE* bValue );
	long GetLDCurrentWindowText( long* lwindow, long* lrows, long* lcolumns, LPTSTR strText );
	long GetLDWindowText( long lwindow, long* lrows, long* lcolumns, LPTSTR strText );
	long GetLDAudience( void );
	void CloseLineDisplay( void );

	//Cash Drawer
	long InitializeCashDrawer( BOOL bCapStatus );
	void CloseCashDrawerService();
	BOOL GetCashDrawerStatusAndEvent( HANDLE* hClosed);

	// POS Printer
	long InitializePOSPrinter( CString sModel, int nSlipToggle );
	void UninitializePOSPrinter( );
	BOOL IsAsync();
	long GetOutputId();
	void IncrementOutputId( );
	long ConvertMeasure( BOOL bHeight, long Station, long nUnits );
	void SetSlipEmpty( BOOL bEmpty );
	long OverridePOSPrinter( COposRegistry& oReg );

	// Scale
	long InitializeScale( BOOL bCapDisplay, long lMaxWeight, long lWeightUnit );
	void CloseScale();
	long GetScaleWeight( long* pWeight, long Timeout );
	void SetScaleWeight( long lWeight );

	// MICR
	long InitializeMICR(CString sRemoveNonDigits);
	void ProcessPOSKeyBoardData(LPCTSTR sData);

    // Fiscal Printer
    long InitializeFiscalPrinter( bool bEuroStatus);
    void SetFiscalProperty( long lPropIndex, long lPropValue );

    // KeyLock
    void InitializeKeyLock( long KeyPosition, long PositionCount );
    bool SetKeyLockPosition( long NewPosition );

    // ToneIndicator
    void InitializeToneIndicator( BOOL bCapPitch, BOOL bCapVolume );

protected:
	virtual CString GetServiceObjectClassName( void ) const 
			{ ASSERT(m_sClassName.IsEmpty() == FALSE );  return m_sClassName; }
	virtual void InitializeProperties( void );
//original	virtual void DispatchDataEvent( const CString& sData, long lStatus );
//antonio start
	virtual void DispatchDataEvent( LPCTSTR sData, long lStatus );
//antonio end
	virtual void DispatchStatusUpdateEvent( long lStatus );
	virtual void DispatchOutputCompleteEvent( long lOutputID );
	virtual void DispatchErrorEvent( long nRC, long nRCEx, long nLocus );
	CString GetPropertyString( long lPropIndex );
public:
	long InitializeMSR();
	long GetPropertyNumber( long lPropIndex );
   CMsrProperties   m_MsrProp;
	long	m_lScaleWeight;
protected:
	void SetPropertyNumber(long lPropIndex, long lProperty);

private:
	int      m_nDeviceId;
	long	m_lOutputId;

	// Scale
	BOOL	m_bCapDisplay;
	long	m_lScaleMaxWeight;
	long	m_lScaleWeightUnit;
	HANDLE	m_hWeightAvailable;

	// POS Printer
	CPrinterProperties	m_PrtProp;

	// Cash Drawer
	HANDLE	m_hDrawerClosed;
	BOOL	m_bCapStatus;

	// Line Display
	long GetLDPropertyNumber( long lPropIndex );
	void SetLDPropertyNumber( long lPropIndex, long lProperty );
	CString GetLDPropertyString( long lPropIndex );
	LineDisplayProperties	m_ldProp;
	CLDWindow**				m_pldWindows;
	BYTE*					m_pDescriptors;

	// Scanner Properties
	CString  m_sScanData;
	BOOL     m_bDecodeData;
	CString  m_sScanDataLabel;
	LONG     m_lScanDataType;

	// MICR
	CMicrProperties  m_MicrProp;

    // Fiscal Printer
    CFiscalProperties m_FiscalPrinterProp;

    // KeyLock
    long    m_lKeyLockPosition;
    long    m_lPositionCount;

    // ToneIndicator
    tToneInfo  m_ToneData;
    BOOL       m_bCapPitch;
    BOOL       m_bCapVolume;
    BOOL       m_bToneAsync;

//antonio start
	//POSKeyBoard
	BOOL	m_bCapKeyUp;
	LONG	m_lEventTypes;
	LONG	m_lPOSKeyData;
	LONG	m_lPOSKeyEventType;
//antonio end
	  
private:
	friend class TOposService<class CTBDevice, class CTBService>;
};

#endif // !defined(AFX_TBSERVICE_H__2B5E45C2_37E8_11D5_9C83_0090276FD28D__INCLUDED_)
