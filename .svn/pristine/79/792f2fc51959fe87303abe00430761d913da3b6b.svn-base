// OposSerialInterface.cpp: implementation of the COposSerialInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <winbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static CString GetMtxName( 
   int      nPort, 
   LPCTSTR  sDevName )
   {
   CString sResult;
   sResult.Format( _T("%sSerialPort%dMutex"), sDevName, nPort );
   return sResult;
   }

// class COverlapped
//
// This class objectifies OVERLAPPED

class COverlapped
	{
	public:
	   COverlapped(HANDLE hEvent = NULL):m_bOwned(hEvent==NULL)
		   {
		   memset( &m_Overlapped, 0, sizeof(m_Overlapped) );
		   if ( m_bOwned == FALSE )
			   hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
         m_Overlapped.hEvent = hEvent;
		   }
	   ~COverlapped()
		   {
		   if ( m_bOwned != FALSE 
            && m_Overlapped.hEvent != NULL )
			   CloseHandle( m_Overlapped.hEvent );
		   }
   operator LPOVERLAPPED () { return &m_Overlapped; }
   operator OVERLAPPED () { return m_Overlapped; }
   operator HANDLE () { return m_Overlapped.hEvent; }
	private:
   	OVERLAPPED m_Overlapped;
	   const BOOL m_bOwned;
	};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COposSerialInterface::COposSerialInterface(
   int            nPort,
   CmDataCapture& cmDc,
   LPCTSTR        sMtxName    /* = NULL   */,
   BOOL           bAutoBaud   /* = FALSE  */ )  // Auto baud doesn't work!
   :  CMutex( FALSE, sMtxName, sMtxName != NULL ? GET_GLOBAL_SA() : NULL ),
   m_hCom( INVALID_HANDLE_VALUE  ),
   m_cmDc( cmDc ),
   m_dwSpecialDcMask( 0 ),
   m_nPort( nPort ),
   m_nPollTimeout( 0 ),
   m_dwLastGoodResponse( 0 ),
   m_nOfflinePollTimeout( 0 ),
   m_nFailureTimeout( 0 ),
   m_pPollThread( NULL ),
   m_nRetries( 3 ),
   m_pDevice( NULL ),
   m_bFailed( FALSE ),
   m_dwLastMessage( 0 ),
   m_dwDelayTimer( 65L ),
   m_dwMinResponseTime( 10000L ),
   m_dwMaxResponseTime( 0 ),
   m_evExit( FALSE, TRUE ),      // Manual reset so we can get out cleanly
   m_dwCommMask( 0 ),
#ifdef _DEBUG
   m_dwPollThreadId( 0 ),
#endif
   m_bSuspendPolling( FALSE ),
   m_bBaudRateKnown( bAutoBaud == FALSE )
   {
   memset( &m_dcb, 0, sizeof( m_dcb ) );
   m_dcb.BaudRate = CBR_9600;  
   // Set up timeouts to be something reasonable
   m_commTO.ReadIntervalTimeout = 50;        /* Maximum time between read chars. */
   m_commTO.ReadTotalTimeoutMultiplier = 0;  /* Multiplier of characters.        */
   m_commTO.ReadTotalTimeoutConstant = 1000; /* Constant in milliseconds.        */
   m_commTO.WriteTotalTimeoutMultiplier = 10;/* Multiplier of characters.        */
   m_commTO.WriteTotalTimeoutConstant = 1000;/* Constant in milliseconds.        */
   return;
   }

COposSerialInterface::~COposSerialInterface()
   {
   ASSERT( m_hCom == INVALID_HANDLE_VALUE ); // Application should close explicitly
   Close();
   return;
   }


// COposSerialInterface::Read
//
// Purpose: Read from Serial Port

DWORD COposSerialInterface::Read(
   void* cResponse, 
   DWORD dwLength ) 
   {
   ASSERT( m_hCom != INVALID_HANDLE_VALUE );
   // Should not read on other threads since this will wake up the poll thread
   ASSERT( (m_dwCommMask & EV_RXCHAR) == 0
      || m_dwPollThreadId == 0
      || IsPollThread() != FALSE );
   COverlapped OlapInfo;
   // Start reading.  If started and not done, then wait for result.
   DWORD dwBytesRead = 0;
   CSingleLock mtxAccess( this, TRUE );
   BOOL bStatus = ReadFile( m_hCom, cResponse, dwLength, &dwBytesRead, OlapInfo );
   if ( bStatus == FALSE && GetLastError() == ERROR_IO_PENDING )
      {
      bStatus = GetOverlappedResult( m_hCom, OlapInfo, &dwBytesRead, TRUE );
      if ( bStatus == FALSE )
         CancelIo( m_hCom );
      }
// Log this error because the device might not be functioning correctly.
   if ( bStatus == FALSE 
      || dwBytesRead != dwLength )
      ReportLineErrors();
   if ( dwBytesRead != 0 )
      {
      m_dwLastMessage = ::GetTickCount();
      m_cmDc.DCDataPrintf( TRACE_OUTPUT_LEVEL1 | m_dwSpecialDcMask, 
         cResponse, dwBytesRead, _T("Serial Read") );
      }
   return dwBytesRead;
   }

// COposSerialInterface::Write
//
// Purpose: Write to Serial Port


BOOL COposSerialInterface::Write(
   const void* cData, 
   DWORD       dwLength ) 
   {
   ASSERT( m_hCom != INVALID_HANDLE_VALUE );
   // Since we don't want to poll the device too quickly, wait if needed
   DWORD dwNow = ::GetTickCount();
   if ( dwNow - m_dwLastMessage < m_dwDelayTimer )
      Sleep( m_dwDelayTimer - ( dwNow - m_dwLastMessage ) );
   COverlapped OlapInfo;
  // Start the output.  If started and not done, then wait for result.
   DWORD dwBytesSent = 0;
   CSingleLock mtxAccess( this, TRUE );
   m_cmDc.DCDataPrintf( TRACE_OUTPUT_LEVEL1 | m_dwSpecialDcMask, 
      cData, dwLength, _T("Serial Write") );
   BOOL bStatus = WriteFile( m_hCom, cData, dwLength , &dwBytesSent, OlapInfo );
   if ( bStatus == FALSE && GetLastError() == ERROR_IO_PENDING )
      {
      bStatus = GetOverlappedResult( m_hCom, OlapInfo, &dwBytesSent, TRUE );
      if ( bStatus == FALSE )
         CancelIo( m_hCom );
      }
   if ( bStatus == FALSE 
      || dwBytesSent != dwLength )
      {
      DWORD dwError = ::GetLastError();
      m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Serial Write Failed (%x)"), dwError );
      m_pDevice->OnWriteFailure();
      ReportLineErrors();
      }
   return dwBytesSent == dwLength;
   }

BOOL COposSerialInterface::Open(
   CPollableDevice* pDevice )
   {
   ASSERT( m_pDevice == NULL );
   ASSERT( m_hCom == INVALID_HANDLE_VALUE );
   CDataCapEntryExit DCE( m_cmDc, _T("COposSerialInterface::Open") );
   CString sCOM;
   // Use special device name syntax to support com10+
   sCOM.Format( _T("\\\\.\\COM%u"), m_nPort );
   m_hCom = OpenPort( sCOM );    // "Shared"
   if ( m_hCom != INVALID_HANDLE_VALUE )
      m_pDevice = pDevice;
   return m_hCom != INVALID_HANDLE_VALUE;
   }

void COposSerialInterface::Close()
   {
   if ( m_pPollThread != NULL )
      EndService();
   if ( m_hCom != INVALID_HANDLE_VALUE )
      ClosePort( m_hCom );               // "Shared"
   m_hCom = INVALID_HANDLE_VALUE;
   m_pDevice = NULL;
   return;
   }

// COposSerialInterface::SetCommState
//
// Purpose: Set user specified DCB. Pass to the port if it is open.

BOOL COposSerialInterface::SetCommState( 
   const DCB& rDCB )
   {
   BOOL bResult = TRUE;
   DCB temp = rDCB;
   temp.DCBlength = sizeof( DCB );
   if ( m_hCom != INVALID_HANDLE_VALUE )
      {
      bResult = ::SetCommState( m_hCom, &temp );
      if ( bResult == FALSE )
         {
         DWORD dwError = ::GetLastError();
         m_cmDc.DCPrintf( TRACE_ALWAYS, _T("-..Failed SetCommState. GetLastError %d"), dwError );
         }
      }
   if ( bResult != FALSE )
      m_dcb = temp;
   return bResult;
   }

// COposSerialInterface::GetCommState
//
// Purpose: Return the DCB. 

BOOL COposSerialInterface::GetCommState( 
   LPDCB pDCB )
   {
   BOOL bResult = TRUE;
   // Port need not be open to access the DCB, we save it.
   if ( m_hCom != INVALID_HANDLE_VALUE )
      {
      bResult = ::GetCommState( m_hCom, pDCB );
      if ( bResult == FALSE )
         {
         DWORD dwError = ::GetLastError();
         m_cmDc.DCPrintf( TRACE_ALWAYS, _T("-..Failed GetCommState. GetLastError %d"), dwError );
         }
      else if ( m_dcb.DCBlength != 0 ) // User has initialized the DCB
         ASSERT( memcmp( &m_dcb, pDCB, sizeof(DCB) ) == 0 );
      }
   else
      *pDCB = m_dcb;
   m_dcb = *pDCB;
   pDCB->DCBlength = sizeof( DCB );
   return bResult;
   }


// COposSerialInterface::SetCommTimeouts
//
// Purpose: Set user specified timeouts. Pass to the port if it is open.

BOOL COposSerialInterface::SetCommTimeouts( 
   const COMMTIMEOUTS& rTimeouts )
   {
   BOOL bResult = TRUE;
   COMMTIMEOUTS temp = rTimeouts;
   if ( m_hCom != INVALID_HANDLE_VALUE )
      {
      ReportLineErrors( );
      bResult = ::SetCommTimeouts( m_hCom, &temp );
      if ( bResult == FALSE )
         {
         DWORD dwError = ::GetLastError();
         m_cmDc.DCPrintf( TRACE_ALWAYS, _T("-..Failed SetCommTimeouts. GetLastError %d"), dwError );
         }
      }
   // Always save timeouts, even if the serial write fails
   m_commTO = temp;
   return bResult;
   }

// COposSerialInterface::GetCommTimeouts
//
// Purpose: Return the COMMTIMEOUTS. 


BOOL COposSerialInterface::GetCommTimeouts( 
   LPCOMMTIMEOUTS pTimeouts )
   {
   BOOL bResult = TRUE;
   // Port need not be open to access the DCB, we save it.
   if ( m_hCom != INVALID_HANDLE_VALUE )
      {
      ReportLineErrors( );
      if ( ::GetCommTimeouts( m_hCom, pTimeouts ) == FALSE )
         {
         DWORD dwError = ::GetLastError();
         m_cmDc.DCPrintf( TRACE_ALWAYS, _T("-..Failed GetCommTimeouts. GetLastError %d"), dwError );
         }
      else
         ASSERT( memcmp( &m_commTO, pTimeouts, sizeof(COMMTIMEOUTS) ) == 0 );
      }
   // Always return our timeouts, even if the serial read fails
   *pTimeouts = m_commTO;
   return bResult;
   }

// COposSerialInterface::GetCommModemStatus
//
// Purpose: Return the modem status

BOOL COposSerialInterface::GetCommModemStatus( 
   LPDWORD lpModemStat ) const
   {
   ASSERT( m_hCom != INVALID_HANDLE_VALUE );
   BOOL bResult = ::GetCommModemStatus( m_hCom, lpModemStat );
   if ( bResult == FALSE )
      {
      DWORD dwError = ::GetLastError();
      m_cmDc.DCPrintf( TRACE_ALWAYS, _T("-..Failed GetCommModemStatus. GetLastError %d"), dwError );
      }
   return bResult;
   }


// COposSerialInterface::BeginService
//
// Purpose: Starts the polling thread 

BOOL COposSerialInterface::BeginService(
   DWORD dwPriority /* = THREAD_PRIORITY_NORMAL */ )
   {
   m_evExit.ResetEvent(); 
   m_pPollThread = AfxBeginThread( PollThreadEntry, this, dwPriority, 0,
      CREATE_SUSPENDED );
   BOOL bResult = FALSE;
   if ( m_pPollThread != NULL )
      {
      m_pPollThread->m_bAutoDelete = FALSE; // We want to wait 
      m_pPollThread->ResumeThread();
      bResult = TRUE;
      }
   return bResult;
   }

// COposSerialInterface::EndService
//
// Purpose: Stops the polling thread 

void COposSerialInterface::EndService()
   {
   if ( m_pPollThread != NULL )
      {
      m_evExit.SetEvent();
      CSingleLock ( &CThreadSync( m_pPollThread ), TRUE );
      delete m_pPollThread;
      m_pPollThread = NULL;
      m_evExit.ResetEvent(); 
      }
   return;
   }

// COposSerialInterface::ReportLineErrors
//
// Purpose: Tell the application about line errors

void COposSerialInterface::ReportLineErrors(
   void ) const
   {
   DWORD dwErrors = 0;                               
   ::ClearCommError( m_hCom, &dwErrors, NULL );        
   dwErrors &= (CE_RXOVER | CE_OVERRUN | CE_RXPARITY | CE_FRAME | CE_BREAK);
   if ( dwErrors != 0)
      {
      m_pDevice->OnLineError( dwErrors );
      ::PurgeComm( m_hCom, PURGE_RXABORT | PURGE_RXCLEAR ); 
      }
   return;
   }

// COposSerialInterface::PollThreadEntry
//
// Purpose: Static thread entry point.


UINT COposSerialInterface::PollThreadEntry(
   void * pvArg)
   {
   ASSERT( pvArg != NULL );
   return ((COposSerialInterface*)pvArg)->PollThread();
   }


// COposSerialInterface::PollDevice
//
// Purpose: Poll the device and handle failure timeout

DWORD COposSerialInterface::PollDevice(
   void )
   {
   // If we are not polling, then we use the max of the two timeouts
   DWORD nPollTimeout = max( m_nPollTimeout, m_nOfflinePollTimeout );
   if ( m_bSuspendPolling == FALSE )
      {
      if ( m_pDevice->Poll() == FALSE )      // Device did not respond
         {
         nPollTimeout = m_nOfflinePollTimeout;  
         // Now check for Failure Timeout
         if ( m_nFailureTimeout != 0 
            && m_bFailed == FALSE
            && ::GetTickCount() - m_dwLastGoodResponse > (DWORD)(m_nFailureTimeout * 1000) )
            {
            m_bFailed = m_pDevice->OnDeviceTimeout( );
            if ( m_bFailed == FALSE ) // Device really there?
               m_dwLastGoodResponse = ::GetTickCount();
            }
         }        
      else 
         {
         nPollTimeout = m_nPollTimeout;  
         m_bFailed = FALSE;
         m_dwLastGoodResponse = ::GetTickCount();
         }
      }
   return nPollTimeout;
   }

// COposSerialInterface::PollThread
//
// Purpose: Object based thread entry point.

UINT COposSerialInterface::PollThread()
   {
   CEvent evSerialWait(FALSE,TRUE);
   CSyncObject* hSync[] = { &m_evExit, &evSerialWait };
   CMultiLock mlEvents( hSync, ARRAY_ELEMENT_COUNT(hSync) );
   DWORD dwResult = -1;
   m_dwLastGoodResponse = ::GetTickCount();
#ifdef _DEBUG
   ASSERT( m_dwPollThreadId == 0 );
   m_dwPollThreadId = ::GetCurrentThreadId();
#endif
   // Want to poll the device when we first start, so it is a do-while 
   ::SetCommMask( m_hCom, m_dwCommMask );
   DWORD nPollTimeout = PollDevice();
   COverlapped OlapInfo( evSerialWait );
   do {
      evSerialWait.ResetEvent();
      DWORD dwEventMask = 0;
      // If WaitCommEvent is not overlapped, then assume events are pending
      if ( m_dwCommMask != 0 )   // Cannot WaitCommEvent with zero event mask
         {
         DWORD dwError = ::GetLastError();
         if ( ::WaitCommEvent( m_hCom, &dwEventMask, OlapInfo ) != FALSE )
            {
            ASSERT( FALSE );           // Just wanted to see if it ever happened.
            m_cmDc.DCPrintf( TRACE_ALWAYS, _T("WaitCommEvent returned TRUE") );
            }
         else if ( dwError != ERROR_IO_PENDING )
            m_cmDc.DCPrintf( TRACE_ALWAYS, _T("WaitCommEvent/GetLastError returned 0x%x"), dwError );
         }
      // Always call the wait so we can exit! 
      dwResult = mlEvents.Lock( nPollTimeout, FALSE );
      // If WaitCommEvent completed, then see what happened...
      if ( dwResult == WAIT_OBJECT_0 + 1 
         || dwEventMask != 0 )
         {            
         // If either of the above conditions is true, then the event must have been set
         // We may have missed it with a small timeout value.  But since dwEventMask != 0,
         // NT must have set the event
//         ASSERT( mlEvents.Lock( 0L, FALSE ) == WAIT_OBJECT_0 + 1 );
         switch( dwEventMask )
            {
            case EV_ERR:
               ReportLineErrors();
               break;
            case EV_CTS:
            case EV_DSR:
            case EV_RING:
            case EV_RLSD:
               m_pDevice->OnModemStatusChange( dwEventMask );
               break;
            case EV_BREAK:
               m_pDevice->OnBreak( );
            case EV_RXCHAR:
               m_pDevice->OnRxChar( );
               break;
            case EV_RXFLAG:
               m_pDevice->OnRxFlag( );
               break;
            case EV_TXEMPTY:
               ASSERT( FALSE );
               break;
            }
         }
      else if ( dwResult == WAIT_TIMEOUT )
         {                         
         // Discovered that we must not be reading or writing on another thread when
         // we cancel the WaitCommEvent call from above. This will cancel IO on 
         // other threads (contrary to the MS docs).
         // But we must call CancelIO or else WaitCommEvent will fail with GetLastError
         // == x57 (INVALID PARAMETER) when called a second time and it has not fired. 

         CSingleLock mtxAccess( this, TRUE );
         CancelIo( m_hCom );
         mtxAccess.Unlock();
         // If the Wait timed out, then we should poll the device
         nPollTimeout = PollDevice();
         }
      } while ( dwResult != WAIT_OBJECT_0 );
#ifdef _DEBUG
   m_dwPollThreadId = 0;
#endif
   return OPOS_SUCCESS;
   }

// CTedeaDevice::SendReceive
// 
// Purpose: Communicate with the device. 

DWORD COposSerialInterface::SendReceive(
   const void* sCommand,
   int         nCommandLength,
   void*       sResponseData,
   int         nResponseSize,
   DWORD       dwPurgeFlags /* = PURGE_ALL */)
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposSerialInterface::SendReceive") );
   CSingleLock mtxAccess( this, TRUE );   // Lock down the device
// Try it a few times
   DWORD nAttempts = m_nRetries + 1;
   DWORD dwBytesRead = 0;
   while ( dwBytesRead == 0
      && nAttempts-- != 0 )
      {
      CSingleLock evExit(&m_evExit, FALSE );
      if ( evExit.Lock( 0 ) != FALSE )
         break;
      if ( dwPurgeFlags != 0 )
         PurgeComm( m_hCom, dwPurgeFlags );
      if ( Write( sCommand, nCommandLength ) != FALSE )
         {                             // Think we sent it?
         DWORD dwWriteTime = ::GetTickCount();
         memset( sResponseData, 0, nResponseSize );
         dwBytesRead = Read( sResponseData, nResponseSize );
         if ( dwBytesRead == 0 )
            {
            if ( m_bBaudRateKnown == FALSE )
               {
               m_dcb.BaudRate /= 2;
               if ( m_dcb.BaudRate == CBR_600 )
                  m_dcb.BaudRate = CBR_38400;
               COposSerialInterface::SetCommState( m_dcb );
               }
            m_pDevice->OnNoResponse();
            }
         else 
            {
            if ( dwBytesRead > 1 )
               m_bBaudRateKnown |= ( dwBytesRead == (DWORD)nResponseSize );
            DWORD dwNow = ::GetTickCount();
            if ( dwNow - dwWriteTime < m_dwMinResponseTime )
               {
               m_dwMinResponseTime = dwNow - dwWriteTime;
               m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL2 | m_dwSpecialDcMask,
                  _T("New minimum response time = %d"), m_dwMinResponseTime );
               }
            else if ( dwNow - dwWriteTime > m_dwMaxResponseTime )
               {
               m_dwMaxResponseTime = dwNow - dwWriteTime;
               m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL2 | m_dwSpecialDcMask, 
                  _T("New maximum response time = %d"), m_dwMaxResponseTime );
               }
            }
         }
      }
   return dwBytesRead;
   }

// COposSerialInterface::OpenPort
//
// Purpose: This is the original (almost) open serial port code.

HANDLE COposSerialInterface::OpenPort( 
   LPCTSTR sCOM )
   {
   BOOL bResult = FALSE;
   ASSERT( m_hCom == INVALID_HANDLE_VALUE );
   m_hCom = ::CreateFile(
      sCOM,
      GENERIC_READ | GENERIC_WRITE,
      0,                      // comm devices must be opened w/exclusive-access
      GET_GLOBAL_SA(),        // security attrs for shared ports with services
      OPEN_EXISTING,          // comm devices must use OPEN_EXISTING
      FILE_FLAG_OVERLAPPED,   // overlapped I/O
      NULL);                  // hTemplate must be NULL for comm devices
   if ( m_hCom == INVALID_HANDLE_VALUE )
      {
      DWORD dwError = ::GetLastError();
      m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to open %s - <%ld>"), sCOM, dwError );
      }
   else if ( m_dcb.DCBlength != 0 ) // User has initialized the DCB
      bResult = COposSerialInterface::SetCommState( m_dcb );
   else 
      {
      DCB temp;
      // Call our GetCommState, to initialize our DCB
      bResult = COposSerialInterface::GetCommState( &temp );
      }
   if ( bResult != FALSE )
      bResult = SetCommTimeouts( m_commTO );
   if ( m_hCom != INVALID_HANDLE_VALUE
      && bResult == FALSE )
      CloseHandle( m_hCom );
   return m_hCom;
   }


// COposSerialInterface::ClosePort
//
// Purpose: Original non-shared close port code

void COposSerialInterface::ClosePort( 
   HANDLE hCom )
   {
   ASSERT( hCom == m_hCom );
   CloseHandle( hCom );
   return;
   }

// CSharedSerialInterface::CSharedSerialInterface
//
// Purpose: Constructor for shared serial ports


static CString GetSmName( 
   int      nPort, 
   LPCTSTR  sDevName )
   {
   CString sResult;
   sResult.Format( _T("%sSerialPort%dSharedMemory"), sDevName, nPort );
   return sResult;
   }


CSharedSerialInterface::CSharedSerialInterface( 
   int nPort, 
   LPCTSTR sDevName, 
   LPCTSTR sShareName, 
   LPCTSTR sMtxName, 
   DWORD nCount, 
   CmDataCapture& cmDc )
   : COposSerialInterface( nPort, cmDc, sMtxName != NULL ? sMtxName : GetMtxName( nPort, sDevName ) ), 
   m_SharedHandles( sShareName != NULL ? sShareName : GetSmName( nPort, sDevName ), 
      cmDc ),
   m_nHandleCount( nCount ),
   m_pHandleMap( NULL )
   {     
   // sDevName and port should be mangled into a shared memory unique 
   // area and mutex names based on sDevName and port. 
   // MiscIF does not handle this correctly and does not mangle the 
   // port number into the shared memory or mutex name. 
   // so we allow the misbehavior to continue, but only for Ontrak service objects
   // All others must play by the rules! 
   // Note: Ontrak can't have two ontrack boards in use on different ports. 
   // It is also misnamed because it is not a "Claim" construct 
   // - it doesn't detect multiple claims and it is not a semaphore!! 
   ASSERT( sMtxName == NULL || CString( sMtxName ) == _T("MISCIFPORTCLAIMSEM") );
   ASSERT( sShareName == NULL || CString( sShareName ) == _T("OntrakSharedMem") );
   }

// CSharedSerialInterface::ClosePort
//
// Purpose: Close the port, remove our entry from shared memory (if present) 
//          and release the shared memory.

void CSharedSerialInterface::ClosePort( 
   HANDLE hCom )
   {
   ASSERT( m_pHandleMap != NULL );
   DWORD dwPid = GetCurrentProcessId();
   CSingleLock mtxAccess( this, TRUE );
   for ( int nIndex = 0; nIndex < m_nHandleCount; nIndex++ )
      {
      if ( m_pHandleMap[nIndex].hHandle == hCom 
         && m_pHandleMap[nIndex].dwPid == dwPid  )
         {
         m_pHandleMap[ nIndex ].Reset();
         break;
         }
      }
   m_SharedHandles.Free();
   m_pHandleMap = NULL;
   COposSerialInterface::ClosePort( hCom );
   return;
   }


// CSharedSerialInterface::OpenPort
// 
// Purpose: Open the selected serial port by hook or crook. 
//          Add Pid and Handle to shared memory if successful!

HANDLE CSharedSerialInterface::OpenPort( 
   LPCTSTR sCOM )
   {
   ASSERT( m_pHandleMap == NULL );
   HANDLE hCom = INVALID_HANDLE_VALUE; 
   CSingleLock mtxAccess( this, TRUE );
   // Create shared memory for this shared port. 
   m_pHandleMap = (HANDLE_MAP_T*)m_SharedHandles.Create( m_nHandleCount, sizeof( HANDLE_MAP_T ), _T("") );
   if ( m_pHandleMap != NULL )
      {
      // Must lock shared memory because to SOs may be starting at the same time 
      // in different applications, or on different threads of the same app, 
      // Without this, its possible that SO a creates the mem and another thread
      // accesses the memory before it is initialized.
      // Actually, the mtxAccess above will stop this.
      CSingleLock mtxSharedMemoryAccess( &m_SharedHandles, TRUE );
      // Initialize handles and Pids if initial creation or if the process indicated 
      // by the process id has died
#ifdef _DEBUG        // MFC DEBUG_NEW does not work for placement new
#undef new           // This trick fixes it
#endif               // FYI: This code is instructive - placement new & vector constructors 
      if ( m_SharedHandles.IsInitialCreation() != FALSE )
         new(m_pHandleMap) HANDLE_MAP_T[ m_nHandleCount ];
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
      // Try to open the com port (first open case)
      hCom = COposSerialInterface::OpenPort( sCOM );
      DWORD dwError = GetLastError();
      if ( hCom == INVALID_HANDLE_VALUE 
         && dwError == ERROR_ACCESS_DENIED ) 
         // This is the port already open case open!
         hCom = DuplicateComHandle( );
      if ( hCom == INVALID_HANDLE_VALUE )
         {                          // Port is invalid, free shared memory
         m_SharedHandles.Free();    // and we're gone
         m_pHandleMap = NULL;
         }
      else
         {
         // Everything worked, then save the handle and our process id in the first
         // available cell of the shared array. 
         for ( int nIndex = 0; nIndex < m_nHandleCount; nIndex++ )
            {
            if ( m_pHandleMap[nIndex].IsAvailable() != FALSE )
               {
               m_pHandleMap[ nIndex ].SetHandle( hCom );
               break;
               }
            }
         if ( nIndex == m_nHandleCount )     // No Cells Available?
            {
            ClosePort( hCom );               // Clean-up everything
            hCom = INVALID_HANDLE_VALUE;     // Note: Pid&Handle won't be found in ClosePort!
            }
         }
      }
   return hCom;
   }

// CSharedSerialInterface::DuplicateComHandle
//
// Purpose: Find a valid process that has this com port open. 
// Note:    This method also cleans up any processes that have terminated abnormally.

HANDLE CSharedSerialInterface::DuplicateComHandle( 
   void )
   {
   HANDLE hCom = INVALID_HANDLE_VALUE;
   for ( int nIndex = 0; nIndex < m_nHandleCount; nIndex++)
      {
      // Find a process that has the handle opened already and dup it!
      if ( m_pHandleMap[nIndex].IsAvailable() == FALSE )
         {
         HANDLE hProcess = OpenProcess( PROCESS_DUP_HANDLE, TRUE, m_pHandleMap[nIndex].dwPid );
         if ( hProcess != NULL )
            {
            if ( hCom == INVALID_HANDLE_VALUE )
               DuplicateHandle( hProcess, m_pHandleMap[nIndex].hHandle, 
                  GetCurrentProcess(), &hCom, NULL, FALSE, DUPLICATE_SAME_ACCESS);
            CloseHandle( hProcess );
            }
         else if ( GetLastError() == ERROR_INVALID_PARAMETER )
            m_pHandleMap[nIndex].Reset(); // Process has died! Clean it up!
         }
      }
   return hCom;
   }


