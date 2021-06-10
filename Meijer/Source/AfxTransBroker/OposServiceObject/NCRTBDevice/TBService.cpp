// TBService.cpp: implementation of the CTBService class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "NCRTBDevice.h"
#include "TBService.h"
#include "keyboardmsg.h"


#include "tboposio.h"
#include "socketfortbdevice.h"
extern CSocketForTBDevice *pClientNCRTBTBDeviceSock;
#include "ExpiringInt.h"

extern _globalInfo g_info;

#define T_ID _T("TBService")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTBService::CTBService(): COposService( _T("TBDeviceSO") )
{
	m_pldWindows = NULL;
	m_pDescriptors = NULL;
	m_hDrawerClosed = INVALID_HANDLE_VALUE;
    m_lOutputId = 0;
    m_lKeyLockPosition = LOCK_KP_LOCK;
    m_lPositionCount = 3;
    m_ToneData.lInterToneWait = 0;
    m_ToneData.lTone1Duration = 0;
    m_ToneData.lTone1Pitch = 0;
    m_ToneData.lTone1Volume = 100;
    m_ToneData.lTone2Duration = 0;
    m_ToneData.lTone2Pitch = 0;
    m_ToneData.lTone2Volume = 100;
    m_bCapPitch = true;
    m_bCapVolume = true;
    m_bToneAsync = FALSE;
}

CTBService::~CTBService()
{

}

//
// Function:  DispatchDataEvent
// Purpose:  Dispatch an OPOS DataEvent to the CO.
// Inputs:  sData - Data for the data event
//          lStatus - Status for the data event
// Assumptions:  None
// Comments:  None
// 
//original void CTBService::DispatchDataEvent( const CString& sData, long lStatus )
//antonio start
void CTBService::DispatchDataEvent( LPCTSTR sData, long lStatus )
//antonio end
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::DispatchDataEvent") );
	CSingleLock csAccess( &m_csAccess, TRUE );

	// Process data based on device ID
	switch( m_nDeviceId )
	{
	case IDS_SCANNER:	
		ProcessScanData( sData );
		break;
	case IDS_MICR:	
   	m_MicrProp.ParseMicrData(sData);
		break;
   case IDS_MSR:
      if ( CSocketForTBDevice::GetSpecVersion() > 1) {
         CMSRMsg msr;
         msr.Parse((BYTE*)sData);
         m_MsrProp.ParseMsrData(msr);
      }
      else {
      // already parsed before event notification, as per documentation.
      //m_MsrProp.ParseMsrData((char*)sData);  
      }
      break;
	case IDS_KEYBOARD:	
		ProcessPOSKeyBoardData( sData );
		break;
	default:
		//AfxMessageBox( "Unknown Device", MB_OK, 0 );
		break;
	}

	csAccess.Unlock();
	COposService::DispatchDataEvent( sData, lStatus );

    if ( IDS_KEYBOARD == m_nDeviceId )
    {
		EnterCriticalSection(&g_info.CritSecAcknowledge);
		g_info.TimerForSO.Start();
        m_cmDc.DCPrintf( 0x01, _T("m_TBCountForKeyboardDataLeft=%d"), pClientNCRTBTBDeviceSock->m_TBCountForKeyboardDataLeft);
		if (--pClientNCRTBTBDeviceSock->m_TBCountForKeyboardDataLeft <= 0)
        {
			if ( !SetEvent(g_info.hConsumed) )
            {
                m_cmDc.DCPrintf( 0x01, _T("SetEvent FAILED!! Err=0x%0X"), GetLastError());
            }
        }
		LeaveCriticalSection(&g_info.CritSecAcknowledge);
    }
	return;
}

//
// Function:  DispatchErrorEvent
// Purpose:  Dispatch an OPOS ErrorEvent to the CO.
// Inputs:  nRc - result code for the error event.
//          nRCEx - extended result code for the error event
//          nLocus - ?? TODO ??
// Outputs: None
// Assumptions:  None
// Comments:  None
// 
void CTBService::DispatchErrorEvent( long nRC, long nRCEx, long nLocus )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::DispatchErrorEvent") );
	COposService::DispatchErrorEvent( nRC, nRCEx, nLocus );
	return;
}

//
// Function:  DispatchOutputCompleteEvent
// Purpose:  Dispatch an OPOS OutputCompleteEvent to the CO.
// Inputs:  lOutputID - ID of the output that was completed.
// Outputs:  None
// Assumptions:  None
// Comments:  None
// 
void CTBService::DispatchOutputCompleteEvent( long lOutputID )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::DispatchOutputCompleteEvent") );
	COposService::DispatchOutputCompleteEvent( lOutputID );
	return;
}

//
// Function:  DispatchStatusUpdateEvent
// Purpose:  Dispatch an OPOS StatusUpdateEvent to the CO.
// Inputs:  lStatus - Status for the status update event
// Assumptions:  None
// Comments:  None
// 
void CTBService::DispatchStatusUpdateEvent( long lStatus )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::DispatchStatusUpdateEvent") );
	COposService::DispatchStatusUpdateEvent( lStatus );
	return;
}

// 
// Function:  InitializeProperties
// Purpose:  Set the initial value of properties maintained by the SO Service
// Inputs:  None
// Outputs:  None
// Assumptions:  The SO Service class maintains the data for all the devices
// Comments:  None
// 
void CTBService::InitializeProperties( void )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::InitializeProperties") );
	// Scanner
	m_sScanData.Empty();
	m_bDecodeData = FALSE;
	m_sScanDataLabel.Empty();
	m_lScanDataType = SCAN_SDT_UNKNOWN;

//antonio start
	//POSKeyBoard
	m_bCapKeyUp = FALSE;
//	m_lEventTypes = 0;
	m_lPOSKeyData = 0;
	m_lPOSKeyEventType = 0;
//antonio end

	COposService::InitializeProperties();
}

//
// Function:  GetLDPropertyString
// Purpose:  Get a string property for the LineDisplay
// Inputs:  lPropIndex - ID of the property value to be returned.
// Outputs:  CString containing the property value.
// Assumptions:  None
// Comments: None
//
CString CTBService::GetLDPropertyString( long lPropIndex )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::GetLDPropertyString") );
	CString sResult;

	if ( lPropIndex == PIDXDisp_CharacterSetList )
	{
		sResult = m_ldProp.sCharacterSetList;
	}

	return sResult;
}


//
// Function:  GetPropertyString
// Purpose:  Get a string property
// Inputs:  lPropIndex - ID of the property value to be returned.
// Outputs:  CString containing the property value.
// Assumptions:  None
// Comments: None
//
CString CTBService::GetPropertyString( long lPropIndex ) 
{
   m_cmDc.DCPrintf( 0x01, _T("+-CTBService::GetPropertyString(%d)"), lPropIndex);
	CString sResult;
	CSingleLock csAccess( &m_csAccess, TRUE );

    if ( IsValidDispStringPidx(lPropIndex) )
    {
		sResult= GetLDPropertyString( lPropIndex );
        return sResult;
    }

	if ( IsValidPtrStringPidx11(lPropIndex) )
    {
		sResult= m_PrtProp.GetPropertyString(lPropIndex);
        //m_cmDc.DCPrintf( 0x01, _T("+-CTBService::GetPropertyString(%d)=[%s]"), lPropIndex, sResult);
        return sResult;
    }

	if ( IsValidMicrStringPidx(lPropIndex) )
    {
		sResult= m_MicrProp.GetPropertyString(lPropIndex);
        return sResult;
    }

  	if ( IsValidMsrStringPidx(lPropIndex) )
    {
		sResult= m_MsrProp.GetPropertyString(lPropIndex);
        return sResult;
    }

    if ( IsValidFptrStringPidx(lPropIndex) )
    {
        sResult = this->m_FiscalPrinterProp.GetPropertyString(lPropIndex);
        return sResult;
    }

   
    switch( lPropIndex )
    {
    case PIDXScan_ScanData:
       sResult = ConvertAsciiToBinary( m_sScanData );
       break;
    case PIDXScan_ScanDataLabel:        
       sResult = ConvertAsciiToBinary( m_sScanDataLabel );
       break;

    default:
       sResult =  COposService::GetPropertyString( lPropIndex );
       break;
    }
//   commented to avoid logging credit cards
//   m_cmDc.DCPrintf( 0x01, _T("+-CTBService::GetPropertyString(%d)=[%s]"), lPropIndex, sResult);

	return sResult;
}

//
// Function:  GetLDPropertyNumber
// Purpose:  Get a numeric property for the LineDisplay
// Inputs:  lPropIndex - ID of the property value to be returned.
// Outputs:  long value of the property.
// Assumptions:  None
// Comments: None
//
long CTBService::GetLDPropertyNumber( long lPropIndex )
{
	m_cmDc.DCPrintf( 0x01, _T("+-CTBService::GetLDPropertyNumber(%d)"), lPropIndex);
	long lResult;

	switch( lPropIndex )
	{
	case PIDXDisp_CharacterSet:
		lResult = m_ldProp.lCharacterSet;
		break;

	case PIDXDisp_Columns:
		lResult = m_pldWindows[m_ldProp.lCurrentWindow]->Columns();
		break;

	case PIDXDisp_CurrentWindow:
		lResult = m_ldProp.lCurrentWindow;
		break;

	case PIDXDisp_CursorColumn:
	case PIDXDisp_CursorRow:
	case PIDXDisp_CursorUpdate:
	case PIDXDisp_InterCharacterWait:
	case PIDXDisp_MarqueeRepeatWait:
	case PIDXDisp_MarqueeType:
	case PIDXDisp_MarqueeUnitWait:
	case PIDXDisp_MarqueeFormat:
		lResult = m_pldWindows[m_ldProp.lCurrentWindow]->GetPropertyNumber( lPropIndex );
		break;

	case PIDXDisp_DeviceBrightness:
		lResult = m_ldProp.lDeviceBrightness;
		break;

	case PIDXDisp_DeviceColumns:
		lResult = m_ldProp.lDeviceColumns;
		break;

	case PIDXDisp_DeviceDescriptors:
		lResult = m_ldProp.lDeviceDescriptors;
		break;

	case PIDXDisp_DeviceRows:
		lResult = m_ldProp.lDeviceRows;
		break;

	case PIDXDisp_DeviceWindows:
		lResult = m_ldProp.lDeviceWindows;
		break;

	case PIDXDisp_Rows:
		lResult = m_pldWindows[m_ldProp.lCurrentWindow]->Rows();
		break;

	case PIDXDisp_CapBlink:
		lResult = m_ldProp.lCapBlink;
		break;

	case PIDXDisp_CapBrightness:
		lResult = m_ldProp.bCapBrightness;
		break;

	case PIDXDisp_CapCharacterSet:
		lResult = m_ldProp.lCapCharacterSet;
		break;

	case PIDXDisp_CapDescriptors:
		lResult = m_ldProp.bCapDescriptors;
		break;

	case PIDXDisp_CapHMarquee:
		lResult = m_ldProp.bCapHMarquee;
		break;

	case PIDXDisp_CapICharWait:
		lResult = m_ldProp.bCapICharWait;
		break;

	case PIDXDisp_CapVMarquee:
		lResult = m_ldProp.bCapVMarquee;
		break;
	}

	return lResult;
}

//
// Function:  SetLDPropertyNumber
// Purpose:  Set a numeric property for the LineDisplay
// Inputs:  lPropIndex - ID of the property value to be returned.
//          lProperty - new value of the property.
// Outputs:  None
// Assumptions:  None
// Comments: None
//
void CTBService::SetLDPropertyNumber( long lPropIndex, long lProperty )
{
	m_cmDc.DCPrintf( 0x01, _T("+-CTBService::SetLDPropertryNumber(%d,%d)"), lPropIndex, lProperty);
	switch( lPropIndex )
	{
	case PIDXDisp_CharacterSet:
		m_ldProp.lCharacterSet = lProperty;
		break;


	case PIDXDisp_CurrentWindow:
		if ( lProperty < m_ldProp.lDeviceWindows )
		{
			m_ldProp.lCurrentWindow = lProperty;
		}
		break;

	case PIDXDisp_CursorColumn:
	case PIDXDisp_CursorRow:
	case PIDXDisp_CursorUpdate:
	case PIDXDisp_InterCharacterWait:
	case PIDXDisp_MarqueeRepeatWait:
	case PIDXDisp_MarqueeType:
	case PIDXDisp_MarqueeUnitWait:
	case PIDXDisp_MarqueeFormat:
		m_pldWindows[m_ldProp.lCurrentWindow]->SetPropertyNumber( lPropIndex, lProperty );
		break;

	case PIDXDisp_DeviceBrightness:
		m_ldProp.lDeviceBrightness = lProperty;
		break;

	case PIDXDisp_DeviceDescriptors:
	case PIDXDisp_DeviceRows:
	case PIDXDisp_DeviceWindows:
	case PIDXDisp_Rows:
	case PIDXDisp_CapBlink:
	case PIDXDisp_CapBrightness:
	case PIDXDisp_CapCharacterSet:
	case PIDXDisp_CapDescriptors:
	case PIDXDisp_CapHMarquee:
	case PIDXDisp_CapICharWait:
	case PIDXDisp_CapVMarquee:
	case PIDXDisp_Columns:
	case PIDXDisp_DeviceColumns:
		// These are read-only!
		break;
	}
}

//
// Function:  GetPropertyNumber
// Purpose:  Get a numeric property
// Inputs:  lPropIndex - ID of the property value to be returned.
// Outputs:  long value of the property.
// Assumptions:  None
// Comments: None
//
long CTBService::GetPropertyNumber( long lPropIndex ) 
{
	m_cmDc.DCPrintf( 0x01, _T("+-CTBService::GetPropertyNumber(%d)"), lPropIndex);
	long lResult;

	CSingleLock csAccess( &m_csAccess, TRUE );

	if ( IsValidDispNumericPidx(lPropIndex)   ||	 IsValidDispCapPidx(lPropIndex) )
		return GetLDPropertyNumber( lPropIndex );

	if ( IsValidPtrNumericPidx11(lPropIndex)  || IsValidPtrCapPidx11(lPropIndex) )
	{
		lResult = m_PrtProp.GetPropertyNumber( lPropIndex );
		//m_cmDc.DCPrintf( 0x01, _T("+-CTBService::GetPropertyNumber(%d)=[%d]"), lPropIndex, lResult);
		return lResult;
	}

	if ( IsValidMicrNumericPidx(lPropIndex) )
		return m_MicrProp.GetPropertyNumber( lPropIndex );

	if ( IsValidMsrNumericPidx12(lPropIndex) )
		return m_MsrProp.GetPropertyNumber( lPropIndex );

    if ( IsValidFptrNumericPidx( lPropIndex ) ||
         IsValidFptrCapPidx( lPropIndex ) )
    {
        return this->m_FiscalPrinterProp.GetPropertyNumber( lPropIndex );
    }

   switch( lPropIndex )
	{
	case PIDXKbd_POSKeyData:
      {
		lResult = m_lPOSKeyData;
	   //_timeb timebuffer;
       //_ftime( &timebuffer );
	   //CTraceSystem ts(_T("c:\\NCRTBDevice.log"));
	   //ts.Trace(TM_DEBUG,_T("KeyBoard Data Key [%d] requested"), lResult);
		break;
      }
	case PIDXKbd_EventTypes:
		lResult = m_lEventTypes;
		break;
	case PIDXKbd_POSKeyEventType:
		lResult = m_lPOSKeyEventType;
		break;
	case PIDXKbd_CapKeyUp:
		lResult = m_bCapKeyUp;
		break;
		//antonio end
	case PIDXScal_CapDisplay:
		lResult = m_bCapDisplay;
		break;
	case PIDXScal_MaximumWeight:
		lResult = this->m_lScaleMaxWeight;
		break;
	case PIDXScal_WeightUnits:
	//case PIDXScal_WeightUnit:
		lResult = m_lScaleWeightUnit;
		break;
	case PIDXScan_ScanDataType:     
		lResult = m_lScanDataType;
		break;
	case PIDXScan_DecodeData:       
		lResult = m_bDecodeData;
		break;
	case PIDXCash_CapStatus:
		lResult = m_bCapStatus;
		break;
	case PIDXCash_DrawerOpened:
		if ( m_bCapStatus )
		{
			if ( WaitForSingleObject( m_hDrawerClosed, 0 ) == WAIT_OBJECT_0 )
			{
				lResult = FALSE;
			}
			else
			{
				lResult = TRUE;
			}
		}
		else
		{
			lResult = FALSE;
		}
		break;
	case PIDX_OutputID:
		// Override OutputID in base class since we cannot set its value
		lResult = m_lOutputId;
		break;

    case PIDXLock_KeyPosition:
        lResult = m_lKeyLockPosition;
        break;

    case PIDXLock_PositionCount:
        lResult = m_lPositionCount;
        break;

    case PIDXTone_AsyncMode:
        lResult = (long)m_bToneAsync;
        break;

    case PIDXTone_Tone1Pitch:
        lResult = m_ToneData.lTone1Pitch;
        break;

    case PIDXTone_Tone1Volume:
        lResult = m_ToneData.lTone1Volume;
        break;

    case PIDXTone_Tone1Duration:
        lResult = m_ToneData.lTone1Duration;
        break;

    case PIDXTone_Tone2Pitch:
        lResult = m_ToneData.lTone2Pitch;
        break;

    case PIDXTone_Tone2Volume:
        lResult = m_ToneData.lTone2Volume;
        break;

    case PIDXTone_Tone2Duration:
        lResult = m_ToneData.lTone2Duration;
        break;

    case PIDXTone_InterToneWait:
        lResult = m_ToneData.lInterToneWait;
        break;

    case PIDXTone_CapPitch:
        lResult = (long)m_bCapPitch;
        break;

    case PIDXTone_CapVolume:
        lResult = (long)m_bCapVolume;
        break;

   default:
		lResult = COposService::GetPropertyNumber( lPropIndex );
		break;
	}
//   m_cmDc.DCPrintf( 0x01, _T("+-CTBService::GetPropertyNumber(%d)=[%d]"), lPropIndex, lResult);
	return lResult;
}

//
// Function:  SetPropertyNumber
// Purpose:  Set a numeric property
// Inputs:  lPropIndex - ID of the property value to be returned.
//          lProperty - new value of the property.
// Outputs:  None
// Assumptions:  None
// Comments: None
//
void CTBService::SetPropertyNumber( long lPropIndex, long lProperty)
{
	m_cmDc.DCPrintf( 0x01, _T("+-CTBService::SetPropertyNumber(%d,%d)"), lPropIndex, lProperty);
	CSingleLock csAccess( &m_csAccess, TRUE );
	SetRC( OPOS_SUCCESS );

	if ( IsValidDispNumericPidx(lPropIndex) ||
		 IsValidDispCapPidx(lPropIndex) )
	{
		SetLDPropertyNumber( lPropIndex, lProperty );
		return;
	}

	if ( IsValidPtrNumericPidx11(lPropIndex)  ||
		 IsValidPtrCapPidx11(lPropIndex) )
	{
		m_PrtProp.SetPropertyNumber( lPropIndex, lProperty );

		if ( (lPropIndex == PIDXPtr_FlagWhenIdle) &&
			 (m_PrtProp.GetPropertyNumber( PIDXPtr_FlagWhenIdle )) )
		{
			m_PrtProp.SetPropertyNumber( lPropIndex, FALSE );
			this->DispatchStatusUpdateEvent( PTR_SUE_IDLE );
		}
		return;
	}

   if ( IsValidMsrNumericPidx12(lPropIndex) ) 
   {
		m_MsrProp.SetPropertyNumber( lPropIndex, lProperty );
      return;
   }

   if ( IsValidFptrNumericPidx(lPropIndex) )
   {
       this->m_FiscalPrinterProp.SetPropertyNumber( lPropIndex, lProperty );
       return;
   }

	switch( lPropIndex )
	{

	case PIDXScan_DecodeData:       
		m_bDecodeData = lProperty;
		if ( m_bDecodeData == FALSE )
		{
			m_sScanDataLabel.Empty();
			m_lScanDataType = SCAN_SDT_UNKNOWN;
		}
		break;
	case PIDXKbd_EventTypes:
//		AfxMessageBox("want to set EVENTTYPES"); 
		m_lEventTypes = lProperty;
		break;

    case PIDXTone_AsyncMode:
        m_bToneAsync = lProperty ? TRUE : FALSE;
        break;

    case PIDXTone_Tone1Pitch:
        m_ToneData.lTone1Pitch = lProperty;
        break;

    case PIDXTone_Tone1Volume:
        m_ToneData.lTone1Volume = lProperty;
        break;

    case PIDXTone_Tone1Duration:
        m_ToneData.lTone1Duration = lProperty;
        break;

    case PIDXTone_Tone2Pitch:
        m_ToneData.lTone2Pitch = lProperty;
        break;

    case PIDXTone_Tone2Volume:
        m_ToneData.lTone2Volume = lProperty;
        break;

    case PIDXTone_Tone2Duration:
        m_ToneData.lTone2Duration = lProperty;
        break;

    case PIDXTone_InterToneWait:
        m_ToneData.lInterToneWait = lProperty;
        break;
    
    default:
		COposService::SetPropertyNumber(lPropIndex, lProperty);
		break;
	}
	return;
}

//
// Function:  SetDeviceName
// Purpose:  Allows the device class to set the device ID of the device being 
//           controled by the service.
// Inputs: nID - integer id of the device
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CTBService::SetDeviceName( int nId )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::SetDeviceName") );
	m_nDeviceId = nId;
	COposService::SetDeviceName( nId );
}

//
// Function:  InitializeMICR
// Purpose:   Configure resource to allow CTBService instance to service
//            a MICR device.
// Inputs:    String containing MICR strip information
//            (combining "T" for transit number & "A" for account number)
// Outputs:   Always OPOS_SUCCESS
// Assumptions:  No distinctions are made by model type
// Comments:  Calls the MICR property class' Initialize() routine
//
long CTBService::InitializeMICR(CString sRemoveNonDigits)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::InitializeMICR") );
    m_MicrProp.Initialize(&m_cmDc, sRemoveNonDigits);
	return OPOS_SUCCESS;
}

//
// Function:  ProcessScanData
// Purpose:  Processes scanner data received from the TB
// Inputs: strData - the scanner data to be processed.
// Outputs:  None 
// Assumptions:  Processing of the data initializes the scanner data variables
//               that are members of the service class.
// Comments:  None
//
void CTBService::ProcessScanData( LPCTSTR strData )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::ProcessScanData") );
    long    num_symchars = 1;
	CString scandata = strData;

    // Default the ScanDataLabel to the full tag, and the type to unknown.
	m_sScanData = strData;
    m_sScanDataLabel = scandata;
    m_lScanDataType = SCAN_SDT_UNKNOWN;

	if ( ! m_bDecodeData )
	{
		return;
	}

    // Check to see if PaceSetter data is being sent in trailer mode.
    // A 0x43 0x3Y 0x3Z at the end of a tag signifies PaceSetter data.
    // Note that 0x43 is a 'C'.  For parsing of the tag data, we will
    // temporarily remove PaceSetter data.
    if ( scandata.GetAt(scandata.GetLength() - 3) == 'C' )
        scandata = scandata.Left(scandata.GetLength() - 3);
    
    if ( scandata.GetAt(0) == 'A' )
    {
        // Symbology is of type UPC-A
        if ( scandata.GetLength() <= 13 )
            m_lScanDataType = SCAN_SDT_UPCA;
        else
            m_lScanDataType = SCAN_SDT_UPCA_S;
    }
    else if ( scandata.GetAt(0) == 'E' )
    {
        // Symbology is of type UPC-E
        if ( scandata.GetLength() <= 9 ) //TAR 127966 change from 8 to 9 to account for check digit
            m_lScanDataType = SCAN_SDT_UPCE;
        else
            m_lScanDataType = SCAN_SDT_UPCE_S;
    }
    else if ( scandata.GetAt(0) == 'B' )
    {
        num_symchars = 2;
        if ( scandata.GetAt(1) == '1' )
            m_lScanDataType = SCAN_SDT_Code39;
        else if ( scandata.GetAt(1) == '2' )
            m_lScanDataType = SCAN_SDT_ITF;
        else if ( scandata.GetAt(1) == '3' )
            m_lScanDataType = SCAN_SDT_Code128;
        // Start 7448 Support Modification
        //else if ( scandata.GetAt(1) == '4' && pScanner->m_nModel == 8154)
        //    m_lScanDataType = SCAN_SDT_Codabar;
        // End 7448 Support Modification
    }
    else if ( scandata.GetAt(0) == 'F' )
    {
        if ( scandata.GetAt(1) == 'F' )
        {
            num_symchars = 2;
            if ( scandata.GetLength() <= 10 )
                m_lScanDataType = SCAN_SDT_EAN8;
//                    else
//                        m_lScanDataType = SCAN_SDT_EAN8_S;
        }
        else
        {
            if ( scandata.GetLength() <= 14 )
                m_lScanDataType = SCAN_SDT_EAN13;
            else
                m_lScanDataType = SCAN_SDT_EAN13_S;
        }
    }
    else if ( scandata.GetAt(0) == 'D' )
    {
        num_symchars = 2;
        if ( scandata.GetAt(1) == '1' )
            m_lScanDataType = SCAN_SDT_UPCD1;
        else if ( scandata.GetAt(1) == '2' )
            m_lScanDataType = SCAN_SDT_UPCD2;
        else if ( scandata.GetAt(1) == '3' )
            m_lScanDataType = SCAN_SDT_UPCD3;
        else if ( scandata.GetAt(1) == '4' )
            m_lScanDataType = SCAN_SDT_UPCD4;
        else if ( scandata.GetAt(1) == '5' )
            m_lScanDataType = SCAN_SDT_UPCD5;
    }

	// Retail Controls Release 1.5 Modification
	// We are now adding support for EAN 128 which has an 
	// identifier of ]C1.
	else if(scandata.GetAt(0) == ']')
	{
		if((scandata.GetAt(1) == 'C') &&
		   (scandata.GetAt(2) == '1'))
		{
			// We have an EAN-128 label
			num_symchars = 3;
			m_lScanDataType = SCAN_SDT_EAN128;
		}
	}

    // If the tag type was known, then remove the leading symbology character.
    if ( m_lScanDataType != SCAN_SDT_UNKNOWN )
        m_sScanDataLabel = m_sScanData.Mid(num_symchars);

}

//
// Function:  ProcessPOSKeyBoardData
// Purpose:   Processes POSKeyBoard data received from the TB
// Inputs:    strData - the POSKeyBoard data to be processed.
// Outputs:   None 
// Comments:  None
//
void CTBService::ProcessPOSKeyBoardData( LPCTSTR strData )
{
   CDataCapEntryExit DCE( m_cmDc, _T("CTBService::ProcessPOSKeyBoardData") );

   if (CSocketForTBDevice::GetSpecVersion() > 1) {
      CKeyboardMsg kbd;
      kbd.Parse((BYTE *) strData);
	   m_lPOSKeyData = kbd.KeyCode();
	   m_lPOSKeyEventType = kbd.KeyEvent();//kbd.CombinationKeys();
   }
   else {
	   // Use copied OPOS platform code to set propery values for POSKeyBoard
	   OPOSIO_KEYSTROKE *POSKeyBoardData = (OPOSIO_KEYSTROKE *) strData;
   //	m_bCapKeyUp = TRUE;
   //	m_lEventTypes = POSKeyBoardData->flags;
	   m_lPOSKeyData = POSKeyBoardData->scanKeyCode;
	   m_lPOSKeyEventType = POSKeyBoardData->flags;
   }
}

//
// Function:  InitializeLineDisplay
// Purpose:  Initializes the Service object to handle a line display device.
// Inputs:  ldProp - specifies the properties of the LineDisplay
// Outputs:  None
// Assumptions:  None
// Comments:  None
// 
void CTBService::InitializeLineDisplay( LineDisplayProperties ldProp )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::InitializeLineDisplay") );
	m_ldProp = ldProp;

	m_ldProp.lCurrentWindow = 0;

	m_pldWindows = new CLDWindow*[m_ldProp.lDeviceWindows];
	m_pldWindows[0] = new CLDWindow( m_ldProp.lDeviceRows,
									m_ldProp.lDeviceColumns, m_cmDc );
	for( long l=1; l<m_ldProp.lDeviceWindows; l++ )
	{
		m_pldWindows[l] = NULL;
	}
	m_pDescriptors = new BYTE[m_ldProp.lDeviceDescriptors];
	memset( m_pDescriptors, 0, m_ldProp.lDeviceDescriptors );
}

//
// Function:  ClearDescriptors
// Purpose:  Implementation of OPOS ClearDescriptors
// Inputs:  None
// Outputs: OPOS_SUCCESS - The method was successful.
// Assumptions: None
// Comments:  None
//
long CTBService::ClearDescriptors( void )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::ClearDescriptors") );
	memset( m_pDescriptors, 0, m_ldProp.lDeviceDescriptors );
	return OPOS_SUCCESS;
}

//
// Function:  SetDescriptor
// Purpose:  Implements the OPOS SetDescriptor method.  Set the state
//           of the specified descriptor.
// Inputs:  lDescriptor - Identifies the descriptor to be set
//          lAttribute - the new state of the descriptor.
// Outputs:  OPOS_SUCCESS - The method was successful
//           OPOS_E_ILLEGAL - The arguments were incorrect.
// Assumptions:  None
// Comments:  None
//
long CTBService::SetDescriptor( long Descriptor, long Attribute )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::SetDescriptor") );
	if ( Descriptor < m_ldProp.lDeviceDescriptors )
	{
		m_pDescriptors[Descriptor] = (BYTE)Attribute;
		return OPOS_SUCCESS;
	}
	else
	{
		return OPOS_E_ILLEGAL;
	}
}

//
// Function:  ClearText
// Purpose:  Implements the OPOS ClearText method.  Clears the text
//           from the currently selected window.
// Inputs:  None
// Outputs:  OPOS_SUCCESS - The method was successful.
// Assumptions:  None
// Comments:  None
//
long CTBService::ClearText()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::ClearText") );
	CLDWindow* pWnd;
	long lResult = OPOS_E_FAILURE;

	pWnd = m_pldWindows[m_ldProp.lCurrentWindow];
	if ( pWnd )
	{
		lResult = pWnd->ClearText();
	}

	return lResult;
}

//
// Function:  CreateWindow
// Purpose:  Implements the OPOS CreateWindow method.  Creates a window
//           of the specified size.  
// Inputs:  Rows - number of rows in the window.
//          Columns - number of columns in the window.
// Outputs:  OPOS_SUCCESS - the method was successful.
//           OPOS_E_ILLEGAL - invalid arguments.
// Assumptions:  None
// Comments:  None
//
long CTBService::CreateWindow( long Rows, long Columns )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::CreateWindow") );
	long lResult = OPOS_E_ILLEGAL;

	for( int i=0; i<m_ldProp.lDeviceWindows; i++ )
	{
		if ( m_pldWindows[i] == NULL )
		{
			m_pldWindows[i] = new CLDWindow( Rows, Columns, m_cmDc );
			if ( m_pldWindows[i] )
			{
				m_ldProp.lCurrentWindow = i;
				lResult = OPOS_SUCCESS;
			}
			break;
		}
	}

	return lResult;
}

//
// Function:  DestroyWindow
// Purpose:  Implements the OPOS DestroyWindow method.  Cleans up resources
//           related to the currently selected window.
// Inputs:  None
// Outputs:  OPOS_SUCCESS - The method was successful.
//           OPOS_E_ILLEGAL
// Assumptions:  None
// Comments:  None
//
long CTBService::DestroyWindow( void )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::DestroyWindow") );
	if ( (m_ldProp.lCurrentWindow > 0) && 
		 (m_ldProp.lCurrentWindow < m_ldProp.lDeviceWindows ) )
	{
		delete m_pldWindows[m_ldProp.lCurrentWindow];
		m_pldWindows[m_ldProp.lCurrentWindow] = NULL;
		m_ldProp.lCurrentWindow = 0;
	}
	else
	{
		return OPOS_E_ILLEGAL;
	}

	return OPOS_SUCCESS;
}

// 
// Function:  DisplayTextAt
// Purpose:  Implements the OPOS DisplayTextAt method. Text is added to the 
//           currently selected window starting at the specified location.
//           Text is added to the window according to the rules defined in the
//           OPOS Application Guide.
// Inputs:  lRow - The start row for the text
//          lColumn - The start column for the text
//          sData - The text to be displayed
// Outputs:
//    OPOS_SUCCESS - The mehtod was successful.
//    OPOS_E_ILLEGAL - The arguments were incorrect.
// Assumptions: A row,column of (-1,-1) indicates that text is to written
//              to the current cursor location
// Comments:  None
//
long CTBService::DisplayTextAt(long lRow, long lColumn, LPCTSTR sData )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::DisplayTextAt") );
	long lResult;

	CString sBinary = sData;
	CString sRealData = this->ConvertBinary( sBinary );

	if ( (lRow==-1) && (lColumn==-1) )
	{
		lResult = m_pldWindows[m_ldProp.lCurrentWindow]->DisplayText( sRealData );
	}
	else
	{
		lResult = m_pldWindows[m_ldProp.lCurrentWindow]->DisplayTextAt( lRow, lColumn, sRealData );
	}

	return lResult;
}

//
// Function:  GetDescriptor
// Purpose:  Gets the value of the specified descriptor.
// Inputs:  dwDescriptor - Id of the descriptor whose value is desired
//          bValue - to be loaed with the descriptor value
// Outputs: TRUE - Descriptor value returned successfully
//          FALSE - Descriptor does not exist.
// Assumptions:  None
// Comments:  None
//
BOOL CTBService::GetDescriptor( long dwDescriptor, BYTE* bValue )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::GetDescriptor") );
	if ( dwDescriptor < m_ldProp.lDeviceDescriptors )
	{
		*bValue = m_pDescriptors[ dwDescriptor ];
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//
// Function:  GetLDCurrentWindowText
// Purpose:  Get the text and other information from the currently
//           selected window of a LineDisplay
// Inputs:  None
// Outputs:  lwindow - the returned window id
//           lrows - the returned number of rows in the window
//           lcolumns - the returned number of columns in the window
//           strText - the text from the window
// Assumptions:  None
// Comments:  None
//
long CTBService::GetLDCurrentWindowText( long* lwindow, long* lrows, 
								 long* lcolumns, LPTSTR strText )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::GetLDCurrentWindowText") );
	long lResult;
	*lwindow = m_ldProp.lCurrentWindow;
	*lrows = m_pldWindows[m_ldProp.lCurrentWindow]->Rows();
	*lcolumns = m_pldWindows[m_ldProp.lCurrentWindow]->Columns();
	_tcscpy( strText, m_pldWindows[m_ldProp.lCurrentWindow]->Data() );
	lResult = (_tcslen( strText ) + 1) * sizeof( TCHAR);
	return lResult;
}

//
// Function:  GetLDWindowText
// Purpose:  Get the text and other information from the specified window 
//           of a LineDisplay
// Inputs:  lwindow - the window whose data is to be returned.
// Outputs:  lrows - the returned number of rows in the window
//           lcolumns - the returned number of columns in the window
//           strText - the text from the window
//   Return value: 
//      length of the text returned
//      0 if the window has not been created
//      -1 if the window is invalid
//   
// Assumptions:  None
// Comments:  None
//
long CTBService::GetLDWindowText( long lwindow, long* lrows, 
								 long* lcolumns, LPTSTR strText )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::GetLDWindowText") );
	long lResult;
	if ( lwindow < m_ldProp.lDeviceWindows )
	{
		if ( m_pldWindows[lwindow] != NULL )
		{
			m_cmDc.DCPrintf( 0x01, _T("Window %d is a valid window"), lwindow );
			*lrows = m_pldWindows[lwindow]->Rows();
			*lcolumns = m_pldWindows[lwindow]->Columns();
			_tcscpy( strText, m_pldWindows[lwindow]->Data() );
			lResult = (_tcslen( strText ) + 1) * sizeof( TCHAR);
		}
		else
		{
			m_cmDc.DCPrintf( 0x01, _T("Window %d has not been created"), lwindow );
			lResult = 0;
		}
	}
	else
	{
		m_cmDc.DCPrintf( 0x01, _T("Window %d is invalid"), lwindow );
		lResult = -1;
	}
	return lResult;
}

// 
// Function:  GetLDAudience
// Purpose:  Returns the intended audience for the LineDisplay device.
//           A LineDisplay device can be used as an Operator or Customer
//           display.
// Inputs:  None
// Outputs: DISP_AUD_CUSTOMER - Customer Display
//          DISP_AUD_OPERATOR - Operator Display
// Assumptions:  None
// Comments:  None
//
long CTBService::GetLDAudience( void )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::GetLDAudience") );
	return m_ldProp.lCapAudience;
}

//
// Function: CloseLineDisplay
// Purpose:  Clean up resources related to a LineDisplay device
// Inputs:  None
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CTBService::CloseLineDisplay( void )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::CloseLineDisplay") );
	for( long l=0; l<m_ldProp.lDeviceWindows; l++ )
	{
		if ( m_pldWindows[l] )
		{
			delete m_pldWindows[l];
			m_pldWindows[l] = NULL;
		}
	}
	delete[] m_pldWindows;
	m_pldWindows = NULL;
	delete[] m_pDescriptors;
	m_pDescriptors = NULL;
}

// 
// Function:  InitializeCashDrawer
// Purpose:  Initialize the service object to service a CashDrawer Device
// Inputs:  bCapStatus - Indicates whether the device supports status reporting
// Outputs:  OPOS_SUCCESS - the method was successful
//           OPOS_E_FAILURE - failed to initialize
// Assumptions: None
// Comments:  None
//
long CTBService::InitializeCashDrawer( BOOL bCapStatus )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::InitializeCashDrawer") );
	m_bCapStatus = bCapStatus;
	m_hDrawerClosed = CreateEvent( NULL, TRUE, TRUE, NULL );
	if ( m_hDrawerClosed == INVALID_HANDLE_VALUE )
	{
		return OPOS_E_FAILURE;
	}
	else
	{
		return OPOS_SUCCESS;
	}
}

//
// Function:  CloseCashDrawerService
// Purpose:  Cleans up resources in the service object related to the operation
//           of a CashDrawer device.
// Inputs:  None
// Outputs: None
// Assumptions:  None
// Comments:  None
//
void CTBService::CloseCashDrawerService()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::CloseCashDrawerService") );

	if ( m_hDrawerClosed != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hDrawerClosed );
		m_hDrawerClosed = INVALID_HANDLE_VALUE;
	}
}

//
// Function:  GetCashDrawerStatusAndEvent
// Purpose:  Gets the CapStatus for the CashDrawer and returns the 
//           Event handle used to track the closed/open state of 
//           the cash drawer.
// Input:  None
// Output:  hClosed - handle to the event for tracking state
//          Returns the value of CapStatus
// Assumptions:  None
// Comments:  None
//
BOOL CTBService::GetCashDrawerStatusAndEvent( HANDLE* hClosed )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::GetCashDrawerStatusAndEvent") );
	*hClosed = m_hDrawerClosed;

	return m_bCapStatus;
}


//
// Function:  IsAsync
// Purpose:  Provides access to the AsyncMode property of the 
//           POS Printer
// Inputs:  None
// Outputs:  Value of AsyncMode property.
// Assumptions:  None
// Comments:  None
// 
BOOL CTBService::IsAsync()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::IsAsync") );
	return m_PrtProp.GetPropertyNumber( PIDXPtr_AsyncMode );
}


//
// Function:  GetOutputId
// Purpose:  Provides access to the OutputID property of the 
//           POS Printer
// Inputs:  None
// Outputs:  Value of OutputID property.
// Assumptions:  None
// Comments:  None
// 
long CTBService::GetOutputId()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::GetOutputId") );
	return this->GetPropertyNumber( PIDX_OutputID );
}

//
// Function:  IncrementOutputId
// Purpose:  Updates the value of the OutputID property of the
//           POS Printer
// Inputs:  None
// Outputs:  Value of OutputID property.
// Assumptions:  None
// Comments:  None
// 
void CTBService::IncrementOutputId( )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::IncrementOutputId") );
	m_lOutputId++;
}

//
// Function:  ConvertMeasure
// Purpose:  Provides access to the conversion functions that allow
//           values to be converted based on the MapMode of the POSPrinter
// Inputs:  bHeight -  True, Conversion is for a height value.  False,
//                     conversion is for a width value.
//          nUnits - value to be converted.
// Outputs:  Converted value
// Assumptions:  None
// Comments:  None
// 
long CTBService::ConvertMeasure( BOOL bHeight, long Station, long nUnits )
{
	if ( bHeight )
	{
		return m_PrtProp.ConvertHeight( Station, nUnits );
	}
	else
	{
		return m_PrtProp.ConvertWidth( Station, nUnits );
	}
}

void CTBService::SetSlipEmpty( BOOL bEmpty )
{
	if (bEmpty)
	{
		this->m_PrtProp.SlipRemoved();
	}
	else
	{
		this->m_PrtProp.SlipInserted();
	}
}

//
// Function:  ConvertBinary
// Purpose:  Converts input strings to the ASCII format based on the
//           value of the BinaryConversion property.
// Inputs: sData - Data to be converted
// Outputs:  the converted data
// Assumptions:  None
// Comments:  None
//
CString CTBService::ConvertBinary( CString& sData )
{
	return this->ConvertBinaryToAscii( sData );
}
//
// Function:  ConvertASCII
// Purpose:  Converts input strings to the binary format based on the
//           value of the BinaryConversion property.
// Inputs: sData - Data to be converted
// Outputs:  the converted data
// Assumptions:  None
// Comments:  None
//
CString CTBService::ConvertASCII( CString& sData )
{
	return this->ConvertAsciiToBinary( sData );
}

//
// Function:  InitializePOSPrinter
// Purpose:  configure resource to allow CTBService instance to service
//           a POSPrinter device.
// Inputs:  sModel - string that identifies the model of the printer
// Outputs:  Always OPOS_SUCCESS
// Assumptions:  The only model currently handled is "Axiohm7156"
// Comments:  None
//
long CTBService::InitializePOSPrinter( CString sModel, int nSlipToggle )
{
   CDataCapEntryExit DCE( m_cmDc, _T("CTBService::InitializePOSPrinter") );
	m_cmDc.DCPrintf( 0x01, _T("Initialize Printer Model (%s)"), sModel );
	m_PrtProp.Initialize( sModel, nSlipToggle );
	m_cmDc.DCPrintf( 0x01, _T("RecPresent is (%d)"), 
   m_PrtProp.GetPropertyNumber( PIDXPtr_CapRecPresent ));
	return OPOS_SUCCESS;
}

//
// Function:  OverridePOSPrinter
// Purpose:  Override POSPrinter resources 
// Inputs:  oReg - Access to registry key with Overrides
// Outputs:  Always OPOS_SUCCESS
// Assumptions:  The only model currently handled is "Axiohm7156"
// Comments:  None
//
long CTBService::OverridePOSPrinter( COposRegistry& oReg )
{
   CDataCapEntryExit DCE( m_cmDc, _T("CTBService::OverridePOSPrinter") );
	m_PrtProp.Override( oReg, &m_cmDc );
	return OPOS_SUCCESS;

}


//
// Function:  UninitializePOSPrinter
// Purpose:  Free resources related to the use of the CTBService instance
//           being used to service a POSPrinter Device.
// Inputs:  None
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CTBService::UninitializePOSPrinter( )
{
	m_PrtProp.Uninitialize();
}


//
// Function:  InitializeScale
// Purpose:  configure resource to allow CTBService instance to service
//           a Scale device.
// Inputs:  bCapDisplay - indicates whether scale has an integrated display
//          lMaxWeight - the maximum weight supported by the scale
//          lWeightUnit - the unit for scale weight data
// Outputs:  OPOS_SUCCESS - Scale initialized successfully
//           OPOS_E_FAILURE - Failed to create event for the scale
// Assumptions:  None
// Comments:  None
//
long CTBService::InitializeScale( BOOL bCapDisplay, long lMaxWeight, long lWeightUnit )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::InitializeScale") );
	this->m_bCapDisplay = bCapDisplay;
	this->m_lScaleMaxWeight = lMaxWeight;
	this->m_lScaleWeightUnit = lWeightUnit;
	this->m_lScaleWeight = 0;
	this->m_hWeightAvailable = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_cmDc.DCPrintf( 0x01, _T("Created hWeightAvailable %d"), m_hWeightAvailable );
	if ( m_hWeightAvailable == INVALID_HANDLE_VALUE )
	{
		return OPOS_E_FAILURE;
	}
	else
	{
		return OPOS_SUCCESS;
	}
}

//
// Function:  CloseScale
// Purpose:  Cleans up resources in the service object related to the operation
//           of a Scale device.
// Inputs:  None
// Outputs: None
// Assumptions:  None
// Comments:  None
//
void CTBService::CloseScale()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::CloseScale") );

	if ( m_hWeightAvailable != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hWeightAvailable );
		m_hWeightAvailable = INVALID_HANDLE_VALUE;
	}

}

//
// Function:  GetScaleWeight
// Purpose:  Gets a non-zero stable weight
// Inputs:  pWeight - return location for weight
//          Timeout - time in millizseconds to wait for a stable weight
// Outputs:  OPOS_SUCCESS - a weight was returned
//           OPOS_E_EXTENDED - The weight was greater than the max
//           OPOS_E_TIMEOUT - No weight received before timeout
//           OPOS_E_FAILURE - Wait failed.
// Assumptions:  The TB only sends non-zero weights.
// Comments:  None
//
long CTBService::GetScaleWeight( long* pWeight, long Timeout )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::GetScaleWeight") );
	long lRet = OPOS_SUCCESS;
	DWORD	dwWait;

	dwWait = WaitForSingleObject( m_hWeightAvailable, Timeout );
	m_cmDc.DCPrintf( 0x01, _T("Wait Returned %d"), dwWait );
	switch( dwWait )
	{
	case WAIT_OBJECT_0:
		// Assumes TB always sends non-zero weight
		m_cmDc.DCPrintf( 0x01, _T("ScaleSuccess %d > %d"), m_lScaleWeight, m_lScaleMaxWeight);
		if ( m_lScaleWeight > this->m_lScaleMaxWeight )
		{
			SetRC( OPOS_E_EXTENDED, OPOS_ESCAL_OVERWEIGHT );
			lRet = OPOS_E_EXTENDED;
		}
		else
		{
			SetRC( OPOS_SUCCESS );
			*pWeight = m_lScaleWeight;
		}
		ResetEvent( m_hWeightAvailable );
		break;

	case WAIT_TIMEOUT:
		m_cmDc.DCPrintf( 0x01, _T("Wait Timeout"));
		SetRC( OPOS_E_TIMEOUT );
		lRet = OPOS_E_TIMEOUT;
		break;

	default:
		SetRC( OPOS_E_FAILURE );
		lRet = OPOS_E_FAILURE;
		break;
	}

	return lRet;
}

// 
// Function:  SetScaleWeight
// Purpose:  Allows the TB to set the weight to be reported by the Scale
// Inputs: lWeight - the weight to be reported
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CTBService::SetScaleWeight( long lWeight )
{
	m_lScaleWeight = lWeight;
	m_cmDc.DCPrintf( 0x01, _T("+-CTBService::SetScaleWeight(): new weight is %d"), m_lScaleWeight);

	SetEvent( m_hWeightAvailable );
}


long CTBService::InitializeMSR()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::InitializeMSR") );
    m_MsrProp.Initialize(&m_cmDc);
	return OPOS_SUCCESS;
}


long CTBService::InitializeFiscalPrinter( bool bEuroStatus )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CTBService::InitializeFiscalPrinter") );
    m_FiscalPrinterProp.Initialize( bEuroStatus );
	return OPOS_SUCCESS;
}


void CTBService::SetFiscalProperty( long lPropIndex, long lPropValue )
{
    this->m_FiscalPrinterProp.SetFiscalProperty( lPropIndex, lPropValue );
}


// KeyLock
void CTBService::InitializeKeyLock( long KeyPosition, long PositionCount )
{
    m_lKeyLockPosition = KeyPosition;
    m_lPositionCount = PositionCount;
}

bool CTBService::SetKeyLockPosition( long NewPosition )
{
    bool bRet = true;

    if ( (NewPosition < 1 ) || (NewPosition > m_lPositionCount) )
    {
        bRet = false;
    }
    else
    {
        m_lKeyLockPosition = NewPosition;
    }

    return bRet;
}

//ToneIndicator
void CTBService::InitializeToneIndicator( BOOL bCapPitch, BOOL bCapVolume )
{
    this->m_bCapPitch = bCapPitch;
    this->m_bCapVolume = bCapVolume;
    this->m_bToneAsync = FALSE;
}

