// LineDisplay.cpp: implementation of the CLineDisplay class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "TBService.h"
#include "LineDisplay.h"
#include "TBOposIO.h"
#include "CmBstring.h"

#include "SocketForTBDevice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineDisplay::CLineDisplay(int nPort, int nId,  CmDataCapture& cmDc )
					   :CTBDevice(OPOS_CLASSKEY_DISP,nId,nPort, cmDc),
					   m_HdrVersion(1)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::CLineDisplay") );
}

CLineDisplay::~CLineDisplay()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::~CLineDisplay") );

}

// Function: Open
// Purpose:  Opens the LineDisplay device, loads configuration from the registry
// Inputs:  pService - pointer to the Service object controlling the device
//          oRegistry - pointer to the registry settings for the device
// Outputs:  OPOS_SUCCESS on success
// Assumptions:  The registry settings are located in the key created for the 
//               OPOS profile for the device. The following values can be defined
//    Value - Default - Description
//    CapAudience - DISP_AUD_OPERATER - The audience for the display
//    CapBrightness - TRUE - Brightness control is supported.
//    CapDescriptors - TRUE - Does display support descriptors.
//    CapHMarquee - TRUE - Does display support horizontal marquee windows.
//    CapICharWait - TRUE - Does display support intercharacter wait.
//    CapVMarquee - TRUE - Does display support vertical marquee windows.
//    CapBlink - DISP_CB_BLINKALL - Character blinking capabilities.
//    CapCharacterSet - DISP_CCS_ASCII - Default character set.
//    CharacterSet - DISP_CS_ASCII - Character set for display.
//    DeviceColumns - 20 - Number of columns on the device
//    DeviceDescriptors - 3 - Number of descriptors on this device
//    DeviceRows - 2 - Number of rows on the device.
//    DeviceWindows - 1 - Number of windows supported.
//    CharacterSetList - "" - Character sets supported.
// Comments:  None
//
long CLineDisplay::Open( CTBService* pService, COposRegistry& oRegistry )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::Open") );


	
	// Load OPOS properties
	LineDisplayProperties ldProp;

	ldProp.lCapAudience =
		oRegistry.ReadInt(_T("CapAudience"), 0, DISP_AUD_OPERATOR );
	ldProp.bCapBrightness = 
		oRegistry.ReadInt( _T("CapBrightness"), 0, TRUE );
	ldProp.bCapDescriptors = 
		oRegistry.ReadInt( _T("CapDescriptors"), 0, TRUE );
	ldProp.bCapHMarquee = 
		oRegistry.ReadInt( _T("CapHMarquee"), 0, TRUE );
	ldProp.bCapICharWait = 
		oRegistry.ReadInt( _T("CapICharWait"), 0, TRUE );
	ldProp.bCapVMarquee = 
		oRegistry.ReadInt( _T("CapVMarquee"), 0, TRUE );
	ldProp.lCapBlink = 
		oRegistry.ReadInt( _T("CapBlink"), 0, DISP_CB_BLINKALL );
	ldProp.lCapCharacterSet = 
		oRegistry.ReadInt( _T("CapCharacterSet"), 0, DISP_CCS_ASCII );
	ldProp.lCharacterSet = 
		oRegistry.ReadInt( _T("CharacterSet"), 0, DISP_CS_ASCII );
	ldProp.lDeviceBrightness = 100;
	ldProp.lDeviceColumns = 
		oRegistry.ReadInt( _T("DeviceColumns"), 0, 20 );
	ldProp.lDeviceDescriptors = 
		oRegistry.ReadInt( _T("DeviceDescriptors"), 0, 3 );
	ldProp.lDeviceRows = 
		oRegistry.ReadInt( _T("DeviceRows"), 0, 2 );
	ldProp.lDeviceWindows = 
		oRegistry.ReadInt( _T("DeviceWindows"), 0, 1 );
	oRegistry.ReadString( _T("CharacterSetList"), ldProp.sCharacterSetList, 0 ,
		_T("") );

	m_TBDirectData = oRegistry.ReadInt( _T("TBDirectData"), 0, 0 );

	m_HdrVersion= oRegistry.ReadInt( _T("tbControlsHdrVersion"), 0, 1);
	
	pService->InitializeLineDisplay( ldProp );

	return CTBDevice::Open( pService, oRegistry);
}

//
// Function:  Close
// Purpose:  Closes the LineDisplay device and cleans up resources related
//           to the device.
// Inputs:  pService - pointer to the Service object controlling the device
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CLineDisplay::Close( CTBService* pService )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::Close") );
	pService->CloseLineDisplay();
	CTBDevice::Close( pService );
}

//
// Function:  ClearDescriptors
// Purpose:  Implementation of OPOS ClearDescriptors
// Inputs:  None
// Outputs: OPOS_SUCCESS - The method was successful.
// Assumptions: None
// Comments:  None
//
long CLineDisplay::ClearDescriptors( void )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::ClearDescriptors") );
	long lResult;

	lResult = m_pTBService->ClearDescriptors();

	if ( OPOS_SUCCESS == lResult )
	{
		lResult = NotifyTB( TRUE );
	}

	return lResult;
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
long CLineDisplay::ClearText( void )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::CLearText") );
	long lResult;

	lResult = m_pTBService->ClearText();

	if ( OPOS_SUCCESS == lResult )
	{
		lResult = NotifyTB( FALSE );
	}

	return lResult;
}

//
// Function:  CreateWindow
// Purpose:  Implements the OPOS CreateWindow method.  Creates a window
//           of the specified size.  Viewport arguments are ignored since
//           they really only apply to a real device.
// Inputs:  lViewportRow, lViewportColumn, lViewportHeight, lViewportWidth
//               Ignored in this implementation
//          lWindowHeight - number of rows in the window.
//          lWindowWidth - number of columns in the window.
// Outputs:  OPOS_SUCCESS - the method was successful.
// Assumptions:  Viewport arguments are ignored since this SO reports the
//               full contents of each individual window instead of the
//               single mapped device display.
// Comments:  None
//
long CLineDisplay::CreateWindow( long lViewportRow, long lViewportColumn, long lViewportHeight, 
		long lViewportWidth, long lWindowHeight, long lWindowWidth )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::CreateWindow") );
	return m_pTBService->CreateWindow( lWindowHeight, lWindowWidth );
}

//
// Function:  DestroyWindow
// Purpose:  Implements the OPOS DestroyWindow method.  Cleans up resources
//           related to the currently selected window.
// Inputs:  None
// Outputs:  OPOS_SUCCESS - The method was successful.
// Assumptions:  None
// Comments:  None
//
long CLineDisplay::DestroyWindow( void )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::DestroyWindow") );
	return m_pTBService->DestroyWindow( );
}

// 
// Function:  DisplayText
// Purpose:  Implements the OPOS DisplayText Command. Text is added to 
//           the currently selected window starting at the location specified by 
//           CursorRow and CursorColumn.  Text is added to the window according
//           to the rules defined in the OPOS Application Guide.
// Inputs:  sData - The text to be added to the window
//          lAttribute - Ignored in this implementation.
// Outputs:  OPOS_SUCCESS - The method was successful.
// Assumptions: The text attributes are ignored since they only modify
//              the display characteristics for a real device.
// Comments: None
//
long CLineDisplay::DisplayText( LPCTSTR sData, long lAttribute )
{
	
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::DisplayText") );
	long lResult;
    CmBString bstrData = sData;
    CString csData = bstrData.GetCString();

	if ( m_TBDirectData )
	{
		lResult = NotifyTBDirect(FALSE, 0, 0, csData);
	}
	else
	{
		lResult = m_pTBService->DisplayTextAt( -1, -1, csData );

		if ( OPOS_SUCCESS == lResult )
		{
			lResult = NotifyTB( FALSE );
		}
	}

	return lResult;
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
//          lAttribute - Ignored in this implementation.
// Outputs:
//    OPOS_SUCCESS - The mehtod was successful.
//    OPOS_E_ILLEGAL - The arguments were incorrect.
// Assumptions: The text attributes are ignored since they only modify
//              the display characteristics for a real device.
// Comments:  None
//
long CLineDisplay::DisplayTextAt(long lRow, long lColumn, LPCTSTR sData, long lAttribute )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::DisplayTextAt") );
	long lResult;
    CmBString bstrData = sData;
    CString csData = bstrData.GetCString();

	if ( (lRow < 0) || (lColumn<0) )
	{
		return OPOS_E_ILLEGAL;
	}

	if ( m_TBDirectData )
	{
		lResult = NotifyTBDirect(FALSE, lRow, lColumn, csData );
	}
	else
	{
		lResult = m_pTBService->DisplayTextAt( lRow, lColumn, csData );

		if ( OPOS_SUCCESS == lResult )
		{
			lResult = NotifyTB( FALSE );
		}
	}

	return lResult;
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
long CLineDisplay::SetDescriptor(long lDescriptor, long lAttribute )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::SetDescriptor") );
	long lResult;

	if ( (lAttribute != DISP_SD_ON) ||
		 (lAttribute != DISP_SD_BLINK) ||
		 (lAttribute != DISP_SD_OFF) )
	{
		return OPOS_E_ILLEGAL;
	}

	lResult = m_pTBService->SetDescriptor( lDescriptor, lAttribute );

	if ( OPOS_SUCCESS == lResult )
	{
		lResult = NotifyTB( TRUE );
	}

	return lResult;
}

// 
// Function:  NotifyTB
// Purpose:  Notify the TB of a change in descriptors or window contents.
// Inputs:  bDescriptor - If true, the descriptors were changed; otherwise,
//                        the text of the currently selected window changed.
// Outputs:  OPOS_SUCCESS - The method was successful
//           OPOS_E_FAILURE - The method failed.
// Assumptions:  None
// Comments: None
//
long CLineDisplay::NotifyTB( BOOL bDescriptor )
{
	//antonio start

	// Notify the TB

	long				lResult = OPOS_E_FAILURE;
	int					iSent;
	BYTE *				pBuf;
	unsigned int iLength = 0;

    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::NotifyTB") );


	BYTE pMsg[MAX_LINE_DISPLAY + sizeof(tSpecHeaderV01) + 1];		//antonio added bytes for HDR
	memset( pMsg, 0, MAX_LINE_DISPLAY + sizeof(tSpecHeaderV01) + 1);
	OPOSIO_LD_DESCRIPTOR_SOCKET* pPack = (OPOSIO_LD_DESCRIPTOR_SOCKET *)pMsg;
	pPack->Spec.messageID = TBMSG_DISPLAY1;

//this creates a problem in pipeserver. See below
 //pPack->Spec.nLen = MAX_LINE_DISPLAY + sizeof(tSpecHeaderV01);

    pPack->Spec.nVersion = m_HdrVersion;

	
	pBuf = (BYTE *) &pPack->Data;

	iLength = PackLDData( bDescriptor, pBuf );

// this fixes the problem with pipeserver
   int   len = iLength + sizeof(tSpecHeaderV01) + 1;
   pPack->Spec.nLen = len;

 //iSent = Send(pPack, (iLength + sizeof(tSpecHeaderV01) + 1));
   iSent = Send(pPack, len);

	if (iSent != SOCKET_ERROR)
	{
		m_cmDc.DCPrintf( TRACE_ALWAYS, _T("sent=%u;"), iSent);
		lResult = OPOS_SUCCESS;
	}
	else
	{
		m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to write to PipeServer Socket"));
	}
	//antonio end
/*
	HWND hTB;
	DWORD dwLength;
	BYTE pMsg[MAX_LINE_DISPLAY + 9];		//antonio added 9 bytes for OPOSIO_HDR
	LONG lResult = OPOS_E_FAILURE;

	hTB = ::FindWindow( NULL, WINDOWNAME_TB );
	if ( hTB != NULL )
	{
		dwLength = PackLDData( bDescriptor, pMsg );
		if ( m_Win.WriteData( hTB, pMsg, dwLength ) )
		{
			lResult = OPOS_SUCCESS;
		}
		else
		{
			m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to write to TB Window"));
		}
	}
	else
	{
		m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to find the TB window." ) );
	}
*/
	return OPOS_SUCCESS;
}

// 
// Function:  PackLDData
// Purpose:  Packs the descriptor or window text data into a buffer for
//           transmission to the TB.
// Inputs:  bDescriptor - If true, the descriptors were changed; otherwise,
//                        the text of the currently selected window changed.
//          pMsg - pointer to a buffer to hold the message.
// Outputs:  The length of data written to the buffer.
// Assumptions:  the buffer passed to this method is of sufficient size to
//               hold the data to be sent to the TB.
// Comments:  None
//
long CLineDisplay::PackLDData( BOOL bDescriptor, BYTE* pMsg )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::PackLDData") );
	DWORD	dwDataLen;

	if ( bDescriptor )
	{
		OPOSIO_LD_DESCRIPTOR* pPack = (OPOSIO_LD_DESCRIPTOR*)pMsg;
		BYTE bValue;

		pPack->ldhdr.hdr.deviceID = TBMSG_DISPLAY1;
		pPack->ldhdr.hdr.port = m_nPort;
		pPack->ldhdr.hdr.unit = (BYTE)m_pTBService->GetLDAudience();;
		pPack->ldhdr.hdr.reserved = 0;
		pPack->ldhdr.msgType = LD_DESCRIPTOR_CHANGE;
		pPack->ldhdr.reserved[0] = 0;
		pPack->ldhdr.reserved[1] = 0;
		pPack->ldhdr.reserved[2] = 0;
		pPack->dataLen = 0;
		while( m_pTBService->GetDescriptor( pPack->dataLen, &bValue) )
		{
			pPack->Data[pPack->dataLen] = bValue;
			pPack->dataLen++;
		}
		dwDataLen = LD_DESCRIPTOR_DATA_OFFSET + pPack->dataLen;
	}
	else
	{
		OPOSIO_LD_WINDOW* pWinPack = (OPOSIO_LD_WINDOW*)pMsg;

		pWinPack->ldhdr.hdr.deviceID = TBMSG_DISPLAY1;
		pWinPack->ldhdr.hdr.port = m_nPort;
		pWinPack->ldhdr.hdr.unit = (BYTE)m_pTBService->GetLDAudience();
		pWinPack->ldhdr.hdr.reserved = 0;
		pWinPack->ldhdr.msgType = LD_TEXT_CHANGE;
		pWinPack->ldhdr.reserved[0] = 0;
		pWinPack->ldhdr.reserved[1] = 0;
		pWinPack->ldhdr.reserved[2] = 0;
		pWinPack->window = 0;
		pWinPack->rows = 0;
		pWinPack->columns = 0;
		pWinPack->dataLen = m_pTBService->GetLDCurrentWindowText( &pWinPack->window,
									&pWinPack->rows,
									&pWinPack->columns,
											(LPTSTR)pWinPack->Data );

		dwDataLen = LD_WINDOW_DATA_OFFSET + pWinPack->dataLen;
	}
		

	return dwDataLen;
}

// 
// Function:  PackWindow
// Purpose:  Packs the window text data into a buffer for
//           transmission to the TB.
// Inputs:  lwindow - Identifies which logical window is to be packed
//          pMsg - pointer to a buffer to hold the message.
// Outputs:  The length of data written to the buffer.
//           0 if the window has not been created.
//           -1 if the window is invalid
// Assumptions:  the buffer passed to this method is of sufficient size to
//               hold the data to be sent to the TB.
// Comments:  None
//
long CLineDisplay::PackWindow( long lwindow, BYTE* pMsg )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::PackWindow") );
	OPOSIO_LD_WINDOW* pWinPack = (OPOSIO_LD_WINDOW*)pMsg;
	long lDataLen;

	lDataLen = m_pTBService->GetLDWindowText( lwindow,
								&pWinPack->rows,
								&pWinPack->columns,
								(LPTSTR)pWinPack->Data );
	if (  lDataLen <= 0 )
	{
		return lDataLen;
	}

	pWinPack->dataLen = lDataLen;
	pWinPack->ldhdr.hdr.deviceID = TBMSG_DISPLAY1;
	pWinPack->ldhdr.hdr.port = m_nPort;
	pWinPack->ldhdr.hdr.unit = (BYTE)m_pTBService->GetLDAudience();
	pWinPack->ldhdr.hdr.reserved = 0;
	pWinPack->ldhdr.msgType = LD_TEXT_CHANGE;
	pWinPack->ldhdr.reserved[0] = 0;
	pWinPack->ldhdr.reserved[1] = 0;
	pWinPack->ldhdr.reserved[2] = 0;
	pWinPack->window = 0;
	pWinPack->rows = 0;
	pWinPack->columns = 0;
	lDataLen = LD_WINDOW_DATA_OFFSET + pWinPack->dataLen;

	return lDataLen;
}

//
// Function:  ProcessRawData
// Purpose:  Process messages sent from the TB
// Inputs:  pData - buffer containing the message
//          dwByteLen - the length of the message data
// Outputs:  None
// Assumptions:  The only expected message is a POLL message from the TB.
// Comments:  None
// 
void CLineDisplay::ProcessRawData( void* pData, DWORD dwByteLen )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::ProcessRawData") );
	OPOSIO_HDR* pHdr = (OPOSIO_HDR*) pData;

	if ( pHdr->deviceID == 0xFF )
	{
		// This is a poll msg
		// Send descriptors
		this->NotifyTB( TRUE );
/*
		// Send window contents
		HWND hTB;
		hTB = ::FindWindow( NULL, WINDOWNAME_TB );
		if ( hTB != NULL )
		{
			BYTE pMsg[MAX_LINE_DISPLAY];
			long lwindow = 0;
			long lLength;

			lLength = this->PackWindow( lwindow, pMsg );
			while( lLength != -1 )
			{
				m_cmDc.DCPrintf( 0x01, _T("Processing window %d, length=%d"),
					lwindow, lLength );
				if ( lLength > 0 )
				{
					m_Win.WriteData( hTB, pMsg, lLength );
					memset( pMsg, 0, MAX_LINE_DISPLAY );
				}
				lwindow = lwindow + 1;
				lLength = this->PackWindow( lwindow, pMsg );
			}
		}			*/
	}
	else
	{
		m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Received unexpected Data Message 0x%0X (LD Data events not supported)"), pHdr->deviceID);
		//CTBDevice::ProcessRawData( pData, dwByteLen );
	}
}


void CLineDisplay::HandleClose()
{
//	ClientNCRTBLineDisplaySock.Close();
}

long CLineDisplay::NotifyTBDirect( BOOL bDescriptor, long lRow, long lCol, CString csData )
{
	// Notify the TB

	long				lResult = OPOS_E_FAILURE;
	int					iSent;
	BYTE *				pBuf;
	unsigned int iLength = 0;

    CDataCapEntryExit DCE( m_cmDc, _T("CLineDisplay::NotifyTBDirect") );


	BYTE pMsg[MAX_LINE_DISPLAY + sizeof(tSpecHeaderV01) + 1];		//antonio added bytes for HDR
	memset( pMsg, 0, MAX_LINE_DISPLAY + sizeof(tSpecHeaderV01) + 1);
	OPOSIO_LD_DESCRIPTOR_SOCKET* pPack = (OPOSIO_LD_DESCRIPTOR_SOCKET *)pMsg;
	pPack->Spec.messageID = TBMSG_DISPLAY1;

    pPack->Spec.nVersion = m_HdrVersion;
	
	pBuf = (BYTE *) &pPack->Data;

	OPOSIO_LD_WINDOW* pWinPack = (OPOSIO_LD_WINDOW*)pBuf;

	pWinPack->ldhdr.hdr.deviceID = TBMSG_DISPLAY1;
	pWinPack->ldhdr.hdr.port = m_nPort;
	pWinPack->ldhdr.hdr.unit = (BYTE)m_pTBService->GetLDAudience();
	pWinPack->ldhdr.hdr.reserved = 0;
	pWinPack->ldhdr.msgType = LD_TEXT_CHANGE;
	pWinPack->ldhdr.reserved[0] = 0;
	pWinPack->ldhdr.reserved[1] = 0;
	pWinPack->ldhdr.reserved[2] = 0;
	pWinPack->window = 0;
	pWinPack->rows = lRow;
	pWinPack->columns = lCol;
	_tcscpy( (TCHAR*)pWinPack->Data, csData );
	pWinPack->dataLen = (_tcslen( (TCHAR*)pWinPack->Data ) + 1) * sizeof( TCHAR);

	iLength = LD_WINDOW_DATA_OFFSET + pWinPack->dataLen;

   int   len = iLength + sizeof(tSpecHeaderV01) + 1;
   pPack->Spec.nLen = len;

   iSent = Send(pPack, len);

	if (iSent != SOCKET_ERROR)
	{
		m_cmDc.DCPrintf( TRACE_ALWAYS, _T("sent=%u;"), iSent);
		lResult = OPOS_SUCCESS;
	}
	else
	{
		m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to write to PipeServer Socket"));
	}

	return OPOS_SUCCESS;
}

