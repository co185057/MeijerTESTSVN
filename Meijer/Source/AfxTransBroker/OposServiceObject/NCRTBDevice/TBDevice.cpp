// TBDevice.cpp: implementation of the CTBDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#define T_ID _T("TBDevice")
#define TM_MODULE TM_MODULE_4

#include "NCRTBDevice.h"
#include "TBDevice.h"
#include "TBService.h"
#include "Scanner.h"
#include "LineDisplay.h"
#include "CashDrawer.h"
#include "POSPrinter.h"
#include "Scale.h"
#include "MICR.h"
#include "MSR.h"
#include "FiscalPrinter.h"
#include "KeyLock.h"
#include "ToneIndicator.h"

#include <sys/timeb.h>
#include <time.h>
#include "POSKeyBoard1.h"
#include "socketfortbdevice.h"
#include "TBOposIO.h"
#include "TraceSystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma pack(1)
typedef struct {
	tSpecHeaderV01 Hdr;
} tHdrToPipe;
#pragma pack()

CSocketForTBDevice *ptoTBSocket=NULL;
CSocketForTBDevice *pClientNCRTBTBDeviceSock=NULL;

CTBDevice* pdeviceObjs[256]={0};
	
_globalInfo g_info;

static int nTBDeviceInstanceCnt = 0;
CTraceSystemMgr *pts=NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define PROGID_TBDEVICESO     _T("NCRTBDevice.NCRTBDeviceSO")

CTBDevice::CTBDevice(LPCTSTR   sClass,int idsName,    int  nShareKey, CmDataCapture& cmDc )
:COposDevice(/* OPOS_CLASSKEY_SCAN*/sClass , PROGID_TBDEVICESO, idsName, nShareKey,
      IDS_FIRST_TB_TALLY, IDS_LAST_TB_TALLY, IDS_SO_DESCRIPTION, 0,cmDc ),
	  m_nPort(0), m_pTBService(NULL), m_nTBDeviceId( idsName ),
	  m_bAutoDataEventEnabled(FALSE)
{
	if (! nTBDeviceInstanceCnt++)
	{
      pts= new CTraceSystemMgr(_T("c:\\NCRTBDevice.log"), TM_MODULE);
      pts->StartTrace();
      ptoTBSocket= new CSocketForTBDevice();
      pClientNCRTBTBDeviceSock= new CSocketForTBDevice();
      StartSocketClient();   //used to receive data coming from TB
      g_info.TimerForSO.SetExpiration(pClientNCRTBTBDeviceSock->m_iTimeForPOStoProcessKey);
	}
   pts->Trace(TM_ENTRY, _T("CTBDevice::CTBDevice %s"),sClass );
}

CTBDevice::~CTBDevice() 
{
   pts->Trace(TM_ENTRY, _T("CTBDevice::~CTBDevice %s"), m_sName );
   if (!CSocketForTBDevice::GetKeepSocketsOpen()) {
      if (--nTBDeviceInstanceCnt <= 0) {
          pClientNCRTBTBDeviceSock->Close();
          DWORD dwWait = WaitForSingleObject( g_info.g_hReceiveThread, 10000 );
          if ( WAIT_TIMEOUT == dwWait )
          {
            // we have to hard terminate the receive thread since it is likely  
            // blocked on a socket receive call
            TerminateThread(g_info.g_hReceiveThread, 0);
            Sleep(200);
          }
          pts->Trace(TM_DEBUG, _T("Ending Device Trace"));
          pts->EndTrace();
          delete pts;
          pts=NULL;

          delete ptoTBSocket;
          ptoTBSocket=NULL;
          delete pClientNCRTBTBDeviceSock;
          pClientNCRTBTBDeviceSock=NULL;
      }
   }
   else
      pts->Trace(TM_DEBUG, _T("keeping sockets open"));
}


//
// Function:  CreateDevice
// Purpose:  Creates an instance of a device specific class derived from
//           the CTBDevice class and returns a pointer to the base class.
// Inputs:  nPort - port for the device to be created.
//          oRegistry - Registry entries related to the OPOS Profile for
//                      the device.
//          cmDC - DataCapture class instance.
// Outputs:  Pointer to the base class (CTBDevice) of the derived device
//           class for handling the device identified in the registry.
// Assumptions:  None
// Comments:  None
//
CTBDevice* CTBDevice::CreateDevice( int nPort, COposRegistry& oRegistry, 
								   CmDataCapture& cmDc)
{
   // Warning: do not use the trace object in this static method unless you
   // create / initialize and cleanup.
	CTBDevice* pDevice = NULL;
	CString sModel = oRegistry.GetClass();

   if ( sModel.CompareNoCase( OPOS_CLASSKEY_SCAN ) == 0 ) {
	   pDevice = new CScanner( nPort, IDS_SCANNER, cmDc);
      pdeviceObjs[TBMSG_SCANNER]= pDevice;
   } 
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_DISP ) == 0 ) {
		pDevice = new CLineDisplay( nPort, IDS_LINEDISPLAY, cmDc );
      pdeviceObjs[TBMSG_DISPLAY1]= pDevice;
   } 
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_CASH ) == 0 ){
		pDevice = new CCashDrawer( nPort, IDS_CASHDRAWER, cmDc );
      pdeviceObjs[TBMSG_CASH_DRAWER]= pDevice;
   } 
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_PTR ) == 0 ){
		pDevice = new CPOSPrinter( nPort, IDS_POSPRINTER, cmDc );
      pdeviceObjs[TBMSG_PRINTER]= pDevice;
   }
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_SCAL ) == 0 ){
		pDevice = new CScale( nPort, IDS_SCALE, cmDc );
      pdeviceObjs[TBMSG_SCALE]= pDevice;
   }
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_MICR ) == 0 ){
		pDevice = new CMICR( nPort, IDS_MICR, cmDc );
      pdeviceObjs[TBMSG_MICR]= pDevice;
   }
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_KBD ) == 0 ){
		pDevice = new CPOSKeyBoard( nPort, IDS_KEYBOARD, cmDc );
      pdeviceObjs[TBMSG_KEYBOARD]= pDevice;
   }
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_MSR ) == 0 ){
		pDevice = new CMSR( nPort, IDS_MSR, cmDc );
      pdeviceObjs[TBMSG_MSR]= pDevice;
   }
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_FPTR) == 0 ){
       pDevice = new CFiscalPrinter( nPort, IDS_FPTR, cmDc );
       pdeviceObjs[TBMSG_FPTR]= pDevice;
   }
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_LOCK) == 0 ){
       pDevice = new CKeyLock( nPort, IDS_KEYLOCK, cmDc );
       pdeviceObjs[TBMSG_KEYLOCK]= pDevice;
   }
   else if ( sModel.CompareNoCase( OPOS_CLASSKEY_TONE) == 0 ){
       pDevice = new CToneIndicator( nPort, IDS_TONEINDICATOR, cmDc );
       pdeviceObjs[TBMSG_TONE]= pDevice;
   }
   else{
      CTraceSystem ts(_T("c:\\ncrTBDevice.log"));
      ts.Trace(TM_ERROR, _T("CreateDevice for unknown:%s"), sModel);
   }
	return pDevice;
}

//
// Function:  GetDevicePort
// Purpose:  Returns the port number for the device.
// Inputs:  oRegistry - Registry entries related to the OPOS Profile for
//                      the device.
//          cmDC - DataCapture class instance.
// Outputs:  The configured port number.
// Assumptions:  The OPOS Profile in the registry contains a Port named 
//               value containing data int the format "COMXX" where XX is 
//               a number.
//               Each device must have a unique port number.
// Comments:  None
//
int CTBDevice::GetDevicePort( COposRegistry& oRegistry, CmDataCapture& cmDc )
{
   // Warning: do not use the trace object in this static method unless you
   // create / initialize and cleanup.
	int nPort = 0;
	nPort = oRegistry.ReadPort();
	return nPort;
}

//
// Function:  InternalHealthCheck
// Purpose:  Stub for OPOS health check functions
// Inputs:  None
// Outputs:  OPOS_SUCCESS
// Assumptions: None
// Comments: None
//
long CTBDevice::InternalHealthCheck( void )
{
	return OPOS_SUCCESS;
}

//
// Function:  ExternalHealthCheck
// Purpose:  Stub for OPOS health check functions
// Inputs:  None
// Outputs:  OPOS_SUCCESS
// Assumptions: None
// Comments: None
//
long CTBDevice::ExternalHealthCheck( void )
{
	return OPOS_SUCCESS;
}

//
// Function:  InteractiveHealthCheck
// Purpose:  Stub for OPOS health check functions
// Inputs:  None
// Outputs:  OPOS_E_ILLEGAL
// Assumptions: Interactive health checks are NOT supported.
// Comments: None
//
long CTBDevice::InteractiveHealthCheck( COposCheckHealth* pCheckHealth )
{
	pts->Trace(TM_ENTRY, _T("CTBDevice::InteractiveHealthCheck") );
	return OPOS_E_ILLEGAL;
}

// 
// Function:  OpenDevice
// Purpose:  Implements the OPOS open of the physical device.  For this
//           SO, this means initializing the communication with the TB.
// Inputs:  None
// Outputs:  OPOS_SUCCESS - The method was successful
//           OPOS_E_FAILURE - Communcation with TB could not be established.
// Assumptions:  The window name used for a device is the name of the OPOS
//               Profile for the device.
// Comments:  None
// 
long CTBDevice::OpenDevice( void )
{
	pts->Trace(TM_ENTRY, _T("CTBDevice::OpenDevice %s"), m_sName );

	int iErrCode=OPOS_SUCCESS;
	iErrCode = ptoTBSocket->GetLastError();
	switch (iErrCode)
	{
	case WSANOTINITIALISED:
		return OPOS_E_FAILURE;
		break;
	default:
		return OPOS_SUCCESS;
	}
}

//
// Function:  CloseDevice
// Purpose:  Implements the OPOS close of the physical device.  For this SO,
//           this means uninitializing the TB Communication.
// Inputs: None
// Outputs: None
// Assumptions:  None
// Comments:  None
//     
void CTBDevice::CloseDevice( void )
{
	pts->Trace(TM_ENTRY, _T("CTBDevice::CloseDevice %s"), m_sName );
//original code	m_Win.Uninitialize();
	return;
}
 
// 
// Function:  OnData
// Purpose:  Handles generation of an OPOS DataEvent
// Inputs:  sData - the data to be included in the event
//          lStatus - the status to be reported by the event
// Outputs: None
// Assumptions:  None
// Comments: None
//          
void CTBDevice::OnData( LPCTSTR sData, long lStatus/*=0*/, long len/*=0*/ )
{ 
	if ( m_pTBService->AutoDisable() != FALSE )
		Enable( FALSE );

   m_pTBService->AddEvent( new COposDataEvent( lStatus, sData, len, m_bAutoDataEventEnabled));

	//_timeb timebuffer;
    //_ftime( &timebuffer );
	//pts->Trace(TM_INFO,_T("Data Event sent to OPOS (Thread %d), @ %d %d"), GetCurrentThreadId(), timebuffer.time, timebuffer.millitm);
	return;
}

// 
// Function:  OnDirectIO
// Purpose:  Handles generation of an OPOS DirectIOEvent
// Inputs:  lEventNumber - the number of the DirectIO Event
//          lData - ??TODO??
//          sData - the data to be included in the event
// Outputs: None
// Assumptions:  None
// Comments: None
//          
void CTBDevice::OnDirectIO( long lEventNumber, long lData, LPCTSTR sData)
{
    pts->Trace(TM_ENTRY, _T("CTBDevice::OnDirectIO") );
	m_pTBService->AddEvent( new COposDirectIOEvent( lEventNumber, lData, sData ) ); 
	return;
}

// 
// Function:  OnInputError
// Purpose:  Handles generation of an OPOS InputErrorEvent
// Inputs:  nRC - error return code
//          nRCEx - extended error return code
// Outputs: None
// Assumptions:  None
// Comments: None
//          
void CTBDevice::OnInputError( long nRC, long nRCEx )
{ 
    pts->Trace(TM_ENTRY, _T("CTBDevice::OnInputError") );
	m_pTBService->AddInputErrorEvent( nRC, nRCEx ); 
	return; 
}

// 
// Function:  OnOutput
// Purpose:  Handles generation of an OPOS OutputCompleteEvent
// Inputs:  nOutputId - ID of the output that was completed.
// Outputs: None
// Assumptions:  None
// Comments: None
//          
void CTBDevice::OnOutput( long nOutputId )
{ 
    pts->Trace(TM_ENTRY, _T("CTBDevice::OnOutputId") );
	m_pTBService->AddEvent( new COposOutputCompleteEvent( nOutputId ) ); 
	return;
}

// 
// Function:  OnStatus
// Purpose:  Handles generation of an OPOS StatusUpdateEvent
// Inputs:  lStatus - the status to be reported by the event
// Outputs: None
// Assumptions:  None
// Comments: None
//          
void CTBDevice::OnStatus( long lStatus )
{
   pts->Trace(TM_ENTRY, _T("CTBDevice::OnStatus:%d"), lStatus );
	m_pTBService->AddEvent( new COposStatusUpdateEvent( lStatus) ); 
	return;
}

//
// Function: Open
// Purpose:  Initializes the SO Service class and opens the device.
// Inputs:  pService - pointer to the service class that controls the device
//          oRegistry - Registry entries in the OPOS profile for the device
// Outputs:  OPOS_SUCCESS - the method was successful.
// Assumptions:  Derived classes that override this function should always
//               call this function in the base class.
// Comments:  None
//
long CTBDevice::Open( CTBService*  pService,
						COposRegistry& oRegistry )
{
	ASSERT( pService != NULL );
	m_pTBService = pService;
	
	m_sName = oRegistry.GetName();
    pts->Trace(TM_ENTRY, _T("CTBDevice::Open %s"), m_sName );

	m_bAutoDataEventEnabled= oRegistry.ReadInt(_T("AutoDataEventEnabled"), 0, FALSE);
	m_pTBService->SetDeviceName( m_nTBDeviceId );

	//
	// New registry configuration option:  ReportPropertyChange
	//    Value:  Comma separated list of property values for the device that are
	//            to be reported to the TB.
	//
	CString csReport = _T("");
	oRegistry.ReadString( _T("ReportPropertyChange"), csReport, 0, _T("") );
	if ( !csReport.IsEmpty() )
	{
		TCHAR* sReport = new TCHAR[csReport.GetLength() + 1];
		memset( sReport, 0, sizeof(TCHAR)*(csReport.GetLength() + 1) );
		_tcscpy(sReport, csReport);

		TCHAR* pTemp;
		pTemp = _tcstok(sReport, _T(","));
		while( pTemp != NULL )
		{
			m_PropertyMap.SetAt( _ttoi(pTemp), NULL);
			pTemp = _tcstok(NULL, _T(","));
		}

		delete[] sReport;
	}

	long lResult = COposDevice::Open(pService, oRegistry);

    return lResult;
}

// 
// Function:  Close
// Purpose:  Closes access to the device
// Inputs:  pService - pointer to the service class that controls the device
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CTBDevice::Close( CTBService* pService )
{
    pts->Trace(TM_ENTRY, _T("CTBDevice::Close") );
	ASSERT( pService == m_pTBService );
	ASSERT( pService != NULL );
	ASSERT( IsClaimed() == FALSE );
	COposDevice::Close(pService);
	m_pTBService = NULL;
    for( int i = 0; i<256; i++ )
    {
        if ( pdeviceObjs[i] == this )
        {
            pdeviceObjs[i] = NULL;
        }
    }
	return ;
}

// 
// Function:  ProcessRawData
// Purpose:  Process raw data sent from the TB.
// Inputs:  pData - pointer to the message contents.  The message formats are 
//                  defined in TBOposIO.h.
//          dwByteLen - Length of the entire message.
// Outputs:  None
// Assumptions:  Unless overridden by a derived class, the default action
//               is to pass the raw data on in an OPOS DataEvent.
// Comments:  None
//    
void CTBDevice::ProcessRawData( void* pData, DWORD dwByteLen )
{
	OnData((LPCTSTR)pData, 0, dwByteLen);
}

// 
// Function:  CommunicationFailure
// Purpose:  Handle a failure of the communication mechanism with the TB
// Inputs:  None
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CTBDevice::CommunicationFailure( )
{
    pts->Trace(TM_ENTRY, _T("CTBDevice::CommunicationFailure") );
}

void CTBDevice::StartSocketClient()
{
   if (!pClientNCRTBTBDeviceSock->Create()) {
      pts->Trace(TM_ERROR,_T("**StartSocketClient Socket create failed!!"));
   }

   //will need to change the next line to connect to the right port on PipeServer's side, the WRITER
	//which sends TB Data back to the SO. What is here noe is for testing only

   DWORD dwBlocking= 0;
   pClientNCRTBTBDeviceSock->IOCtl(FIONBIO, &dwBlocking); // set to blocking mode

   if (pClientNCRTBTBDeviceSock->Connect(pClientNCRTBTBDeviceSock->m_csServerName, _ttoi(pClientNCRTBTBDeviceSock->m_csPortNumberWrite)))
	{
		CString csMessage;// = "TBDevice can talk to you now";
		int iLength = 0;//csMessage.GetLength();
      if (pClientNCRTBTBDeviceSock->m_bAck)
         csMessage = "1," + pClientNCRTBTBDeviceSock->m_csDeviceIDs;
      else
         csMessage = "0," + pClientNCRTBTBDeviceSock->m_csDeviceIDs;
		iLength = csMessage.GetLength();
		
		if (iLength > 0)
		{
			int iComa = 0;
			int iNumberOfDevices = 0;
			do
			{
				iComa = csMessage.Find(_T(","), iComa);
				iNumberOfDevices++;
			}
			while (iComa++ != -1);

			int iilength = sizeof(tSpecHeaderV01)+ iNumberOfDevices;
			ASSERT(iilength > sizeof(tSpecHeaderV01));

			tHdrToPipe *pPipeHdr= (tHdrToPipe*) new BYTE[iilength];

			BYTE *ptr=(BYTE*)&pPipeHdr[1];

			pPipeHdr->Hdr.nLen = iilength;
			pPipeHdr->Hdr.messageID=0xFF;
			pPipeHdr->Hdr.nVersion=1;

			CString csMessage2;
			int iDeviceID = 0;
			int iLength2 = iLength;

			for(int i=0; i<= (iNumberOfDevices);)
			{
				if((iComa = csMessage.Find(_T(","), 0)) == -1) break;
				csMessage2 = csMessage.Left(iComa);
				csMessage = csMessage.Right(iLength2 - (iComa+1));
				iLength2 = iLength2 - (csMessage2.GetLength() + 1);
				_stscanf(csMessage2,_T("%x"),&iDeviceID);
				ptr[i++] = iDeviceID;
			}

			if(csMessage.GetLength() >= 1)
			{
				_stscanf(csMessage,_T("%x"),&iDeviceID);
				ptr[i] = iDeviceID;
			}
		
         // Sleep to allow pipeserver time to be ready for our msg
			Sleep(300);
			pClientNCRTBTBDeviceSock->Send((BYTE *)pPipeHdr,iilength);

		   delete [] (BYTE*)pPipeHdr;

         pClientNCRTBTBDeviceSock->StartReadThread();
		}
	}
   // start the outgoing socket
   if (ptoTBSocket->Create()) {
      dwBlocking= 0;
      ptoTBSocket->IOCtl(FIONBIO, &dwBlocking); // set to blocking mode
	   if (ptoTBSocket->Connect(ptoTBSocket->m_csServerName, _ttoi(ptoTBSocket->m_csPortNumber)))
		   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("toTBSocket socket connected to PipeServer" ) );
	   else
		   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("toTBSocket Failed to connect to PipeServer" ) );
   } else
		m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to create toTBSocket" ) );
}

int CTBDevice::Send(const void *pBuf, int iLength)
{
   return ptoTBSocket->TBSend(pBuf, iLength);
}

// COposDevice::DirectIO
//
// Purpose: Common DirectIO code - log an event


long CTBDevice::DirectIO(
   long     nCommand,
   long*    plData,
   CString& sData)
   {
   CDataCapEntryExit DCE( m_cmDc, _T("CTBDevice::DirectIO") );
   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("*DirectIO Command: %x, plData: %x, sData: %s"),
      nCommand, plData != NULL ? *plData : 0, sData );
   m_cmDc.DoDCDataPrintf( TRACE_ALWAYS, sData, (sData.GetLength()*sizeof(TCHAR)), _T("HEXDUMP DATA"));
   return SetRC( OPOS_SUCCESS );
   }

// 
// Function:  CheckPropertyNumber
// Purpose:  Check if a property change is to be reported to the TB
// Inputs:  lProperty - the property number to check
// Outputs:  True - report to TB, False - Do not report to TB
// Assumptions:  None
// Comments:  None
//
bool CTBDevice::CheckPropertyNumber( long lProperty )
{
   CDataCapEntryExit DCE( m_cmDc, _T("CTBDevice::CheckPropertyNumber") );
	void* vValue;

	bool bRet = m_PropertyMap.Lookup( lProperty, vValue) ? true: false;

	return bRet;
}

//
// Function:  ReportPropertyNumber
// Purpose:  Report a property to the TB.  This method does nothing.  The actual
//           reporting must be implemented in the device class.
// Inputs:  lProperty - the property number to be reported
//          lValue - the property value to report
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
// ReportPropertyString is not required since there are no known string properties
// that can be set for the supported devices
//
void CTBDevice::ReportPropertyNumber( long lProperty, long lValue )
{
	// Must be implemented on a per device basis
}

