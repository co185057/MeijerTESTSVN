// DuplexClientPipe.cpp: implementation of the CDuplexClientPipe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "tracesystem.h"
#include "DuplexClientPipe.h"

#define T_ID   _T("DuplexClientPipe")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDuplexClientPipe::CDuplexClientPipe()
{
	m_bMessageBased = true;
	m_hPipe = INVALID_HANDLE_VALUE;
	m_hTerminate = INVALID_HANDLE_VALUE;
	m_hRead = INVALID_HANDLE_VALUE;
	m_hWrite = INVALID_HANDLE_VALUE;
  m_hReadThread = INVALID_HANDLE_VALUE;
  m_hWriteThread = INVALID_HANDLE_VALUE;
  m_hMonitorThread = INVALID_HANDLE_VALUE;
	m_pSink = NULL;
	m_dwPipeID = 0;
}

CDuplexClientPipe::~CDuplexClientPipe()
{
	if ( m_hTerminate != INVALID_HANDLE_VALUE )
	{
		SetEvent( m_hTerminate );
	}
  WaitForSingleObject( m_hReadThread, 1000);
  WaitForSingleObject( m_hWriteThread, 1000);
  WaitForSingleObject( m_hMonitorThread, 1000);
 	if ( m_hPipe != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hPipe );
	}

	if ( m_hRead != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hRead );
	}

	if ( m_hWrite != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hWrite );
	}

	if ( m_hTerminate != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hTerminate );
	}
}

// Function:  Initialize
// Purpose:  Initialize the pipe processing through the creation of related
//           events, and starting the read and write process.  
// Input:  strPipeName - name for the pipe.  This name will be prefixed with
///                      "\\.\pipe\" in order to form a true pipe name if that
//                       prefix does not already exist on the pipe name.
//         dwPipeID - Identifier for this pipe.  The identifier is used in 
//                    calls back to the create via APipeSink
//         bMessageBased - Indicates if the pipe should be read in message 
//                         mode (TRUE) or byte mode (FALSE).
// Output:  0 - Success
//          Standard Win32 error codes.
// Assumptions:  None
// Comments:  None
//
DWORD CDuplexClientPipe::Initialize( LPCTSTR strPipeName, DWORD dwPipeID, BOOL bMessageBased )
{
	DWORD dwErr;
  CWinThread *thread = NULL;

  m_csPipeName = strPipeName;
  m_dwPipeID = dwPipeID;

	dwErr = SimpleEvent( &m_hTerminate);
	if ( dwErr )
	{
		return dwErr;
	}

	dwErr = SimpleEvent( &m_hRead);
	if ( dwErr )
	{
		return dwErr;
	}

	dwErr = SimpleEvent( &m_hWrite);
	if ( dwErr )
	{
		return dwErr;
	}

  dwErr = WaitForServer( m_csPipeName);
	if ( dwErr )
	{
		return dwErr;
	}

  dwErr = OpenPipe( m_csPipeName);
	if ( dwErr )
	{
		return dwErr;
	}

	dwErr = StartWriterThread();
	if ( dwErr )
	{
		CloseHandle( m_hPipe );
		m_hPipe = INVALID_HANDLE_VALUE;
		return dwErr;
	}

	dwErr = StartReaderThread();
	if ( dwErr )
	{
		SetEvent( m_hTerminate );
		CloseHandle( m_hPipe );
	}
  thread = AfxBeginThread( CDuplexClientPipe::MonitorThread,
    (void*) this, 0,0,0,NULL );
  m_hMonitorThread = thread->m_hThread;

	return dwErr;
}


DWORD CDuplexClientPipe::DoRestart()
{
	DWORD dwErr;
  CWinThread *thread = NULL;
  tbtraceIO(_T("DoRestart"));

  ResetEvent( m_hTerminate );
  dwErr = WaitForServer( m_csPipeName);
	if ( dwErr )
	{
		return dwErr;
	}

  dwErr = OpenPipe( m_csPipeName);
	if ( dwErr )
	{
		return dwErr;
	}

	dwErr = StartWriterThread();
	if ( dwErr )
	{
		CloseHandle( m_hPipe );
		m_hPipe = INVALID_HANDLE_VALUE;
		return dwErr;
	}

	dwErr = StartReaderThread();
	if ( dwErr )
	{
		SetEvent( m_hTerminate );
		CloseHandle( m_hPipe );
	}

  thread = AfxBeginThread( CDuplexClientPipe::MonitorThread,
    (void*) this, 0,0,0,NULL );
  m_hMonitorThread = thread->m_hThread;
	return dwErr;
}

// Function:  Uninitialize
// Purpose:  Terminates the pipe processing.
// Input:  None
// Output:  Always returns 0
// Assumptions:  None
// Comments:  None
//
DWORD CDuplexClientPipe::Uninitialize( void )
{
	if ( m_hTerminate != INVALID_HANDLE_VALUE )
	{
		SetEvent( m_hTerminate );
	}
  WaitForSingleObject( m_hReadThread, 1000);
  WaitForSingleObject( m_hWriteThread, 1000);
  WaitForSingleObject( m_hMonitorThread, 1000);
 	if ( m_hPipe != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hPipe );
        m_hPipe = INVALID_HANDLE_VALUE;
	}

	return 0;
}

// Function:  WriteData
// Purpose:  Accepts data to be written to the pipe.  The data is queued
//           for writing to the pipe by the write thread.
// Inputs:  pData - pointer to the data
//          dwByteLength - length of the data in bytes.
// Outputs:  True - Data was successfully queued.
//           False - Data could not be queued.
// Assumptions:  None
// Comments:  A failure to write to the pipe is considered a pipe failure 
//            and is reported through APipeSink 
//
BOOL CDuplexClientPipe::WriteData( void* pData, DWORD dwByteLength )
{
	CPipeMessage* pMsg;

	pMsg = new CPipeMessage();
	if ( pMsg == NULL )
	{
		return false;
	}

	pMsg->SetMessage( pData, dwByteLength );

	return ( m_PipeQueue.Push( pMsg ) );
}

// Function:  WaitForServer
// Purpose:  Allows the calling application to wait for a busy pipe server
//           so that a valid connection can be made.
// Input:  strPipeName - name for the pipe.  This name will be prefixed with
//                      "\\.\pipe\" in order to form a true pipe name if that
//                       prefix does not already exist on the pipe name.
//         dwWaitTime - Amount of time to wait for the server.
// Outputs:  See WaitNamedPipe in MSDN Library
// Assumptions:  None
// Comments:  None
// 
DWORD CDuplexClientPipe::WaitForServer( LPCTSTR strPipeName)
{
	TCHAR strFullPipe[PIPE_NAME_MAX];
  DWORD retVal = 0;

	if ( _tcsnicmp( strPipeName, PIPE_PREFIX, _tcslen(PIPE_PREFIX)) != 0 )
	{
		//Add prefix to pipe name
		_stprintf( strFullPipe, PIPE_NAME_FORMAT, strPipeName );
	}
	else
	{
		_tcscpy(strFullPipe, strPipeName );
	}

  do
  {
	  if ( WaitNamedPipe( strPipeName, NMPWAIT_WAIT_FOREVER ) )
    {
  		return 0;
    }
    retVal = GetLastError();
    Sleep(1000);
  }
  while (ERROR_FILE_NOT_FOUND == retVal);

	return retVal;
}

// Function:  SetPipeSink
// Purpose:  Informs the class of the implementation of the APipeSink class
//           to be called for reads and pipe failures.
// Input:  pSink - pointer to an instance of a class that implements
//                 APipeSink
// Output:  None
// Assumptions:  None
// Comments:  None
//
void CDuplexClientPipe::SetPipeSink( APipeSink* pSink )
{
	m_pSink = pSink;
}

// Function:  ReaderThread
// Purpose:  Reads from the pipe and passes the data to the callback defined
//           by the APipeSink class.
// Inputs:  lpParam - pointer to the CDuplexClientPipe instance that created
//                    the thread
// Outputs:  0 - Success
//           Win32 error code on failure
// Assumptions:  None
// Comments:  None
//
UINT CDuplexClientPipe::ReaderThread( void* lpParam )
{
   STARTtbtrace();   // initialize a trace obj for this thread
   CDuplexClientPipe* pThis = (CDuplexClientPipe*) lpParam;
	BOOL bReading = true;	
	void* pData;
	DWORD dwDataLen;
	DWORD dwErr = 0;
	tbtrace(TM_ENTRY, _T("ReaderThread()"));

	CoInitializeEx(NULL, COINIT_MULTITHREADED); //Sinan
	while ( bReading )
	{
		pData = pThis->ReadPipe( &dwDataLen, &dwErr );
		if ( NULL == pData )
		{
//			SetEvent( pThis->m_hTerminate );
			tbtrace( TM_ML_ENTRY, _T("NULL data read from pipe, error 0x%x"), dwErr );
			bReading = false;
		}
		else
		{
			// Pass the data off for processing
			if ( pThis->m_pSink )
			{
				pThis->m_pSink->ProcessRawHostData( pThis->m_dwPipeID, pData, dwDataLen );
			}

			delete[] pData;
		}
	}
	CoUninitialize(); //Sinan
   ENDtbtrace();  // cleanup trace obj
	return dwErr;
}

// Function: WriterThread
// Purpose:  Reads from the write queue and writes messages to the pipe.
// Inputs:  lpParam - pointer to the CDuplexClientPipe instance that created
//                    the thread
// Outputs:  0 - Success
//           Win32 error code on failure
// Assumptions:  None
// Comments:  None
//
UINT CDuplexClientPipe::WriterThread( void* lpParam )
{
   STARTtbtrace();
	CDuplexClientPipe* pThis = (CDuplexClientPipe*) lpParam;

	HANDLE hWaitEvents[2];
	BOOL	bProcessing = true;
	DWORD	dwWait;
	DWORD   dwErr = 0;
	tbtrace( TM_ML_ENTRY, _T("+WriterThread"));

	hWaitEvents[0] = pThis->m_hTerminate;
	hWaitEvents[1] = pThis->m_PipeQueue.GetDataEvent();

	while( bProcessing )
	{
		dwWait = WaitForMultipleObjects( 2, hWaitEvents, false, INFINITE );
		switch ( dwWait )
		{
		case WAIT_OBJECT_0:
			// Termination
			bProcessing = false;
			break;
		case WAIT_OBJECT_0 + 1:
    {
			// Data available on queue
			CPipeMessage* pMsg = NULL;
			void* pData = NULL; 
			DWORD dwDataLen = 0; 
			DWORD dwWritten= 0;  
			OVERLAPPED oWrite;

			pMsg = pThis->m_PipeQueue.Pop();
      if (pMsg) 
      { 
			  pData = pMsg->Detach( &dwDataLen );
			  delete pMsg;
        if ((pData) && (dwDataLen)) 
        { 
  			  memset( (void*)&oWrite, 0, sizeof(OVERLAPPED) );
  			  oWrite.hEvent = pThis->m_hWrite;

  			  if ( !WriteFile(pThis->m_hPipe, pData, dwDataLen, &dwWritten, &oWrite ) )
          {
			    	dwErr = GetLastError();
		  	  	if ( ERROR_IO_PENDING == dwErr )
            {
			  	  	if ( !GetOverlappedResult(pThis->m_hPipe, &oWrite, &dwWritten, true ))
              {
				  	  	dwErr = GetLastError();
			  		  	SetEvent( pThis->m_hTerminate );
		  			  	bProcessing = false;
              }
            }
	  			  else
            {
			    		SetEvent( pThis->m_hTerminate );
		    			bProcessing = false;
            }
          }
			   delete[] pData;
        }
      } 
			break;
    }
		default:
			// Unexpected condition
			dwErr = GetLastError();
			SetEvent( pThis->m_hTerminate );
			bProcessing = false;
		}
	}
	tbtrace( TM_ML_ENTRY, _T("-WriterThread, error 0x%x"), dwErr );
   ENDtbtrace();
	return dwErr;
}

// Function:  MonitorThread
// Purpose:  
//           
//           
// Inputs:  lpParam - pointer to the CDuplexClientPipe instance that created
//                    the thread
// Outputs:  0 - Success
// Assumptions:  None
// Comments:  None
//
UINT CDuplexClientPipe::MonitorThread( void* lpParam )
{
  STARTtbtrace();
	CDuplexClientPipe* pThis = (CDuplexClientPipe*) lpParam;
	HANDLE hWaitEvents[3];
  DWORD dwWait = 0;

	hWaitEvents[0] = pThis->m_hTerminate;
	hWaitEvents[1] = pThis->m_hReadThread;
  hWaitEvents[2] = pThis->m_hWriteThread;

  // WaitForMultipleObjects returns lowest index (terminate) for simultaneous events
  dwWait = WaitForMultipleObjects( 3, hWaitEvents, false, INFINITE );
  if (WAIT_OBJECT_0 != dwWait) // terminate or thread(s) finished?
  { // one or both of the read/write threads finished
    pThis->Uninitialize(); // issues terminate and pipe close which kills any remaining threads
    pThis->DoRestart();
  }
  ENDtbtrace();
	return 0;
}

// Function:  OpenPipe
// Purpose:  Opens a handle to the client end of a named pipe.
// Input:  strPipeName - name for the pipe.  This name will be prefixed with
//                      "\\.\pipe\" in order to form a true pipe name if that
//                       prefix does not already exist on the pipe name.
// Output: 0 - Success
//         Win32 error code on failure
// Assumptions:  The client end of the pipe is opened for both read and write
//               access.  The pipe server must have already created the pipe.
//               Overlapped access is used so that read/write operations can 
//               occur simultaneously.
// Comments:  None
//
DWORD CDuplexClientPipe::OpenPipe( LPCTSTR strPipeName )
{
	TCHAR strFullPipe[PIPE_NAME_MAX];
	SECURITY_ATTRIBUTES sa;

	sa.bInheritHandle = true;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);

	if ( _tcsnicmp( strPipeName, PIPE_PREFIX, _tcslen(PIPE_PREFIX)) != 0 )
	{
		//Add prefix to pipe name
		_stprintf( strFullPipe, PIPE_NAME_FORMAT, strPipeName );
	}
	else
	{
		_tcscpy(strFullPipe, strPipeName );
	}

	m_hPipe = CreateFile( strFullPipe, GENERIC_READ|GENERIC_WRITE, 0, 
		&sa, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL ); 
	if ( m_hPipe == INVALID_HANDLE_VALUE )
	{
		return GetLastError();
	}

	if ( m_bMessageBased )
	{
		DWORD dwMode = PIPE_READMODE_MESSAGE;
		if ( !SetNamedPipeHandleState( m_hPipe, &dwMode, NULL, NULL ) )
		{
			DWORD dwErr = GetLastError();
			CloseHandle( m_hPipe );
			m_hPipe = INVALID_HANDLE_VALUE;
			return dwErr;
		}
	}

	return 0;
}

// Function:  StartReaderThread
// Purpose:  Starts the reader thread
// Inputs: None
// Outputs: 0
// Assumptions:  The thread created deletes itself when the thread exits.
// Comments: None
//
DWORD CDuplexClientPipe::StartReaderThread( void )
{
  CWinThread *thread = NULL;

  thread = AfxBeginThread( CDuplexClientPipe::ReaderThread, 
						(void*) this, 0,0,0,NULL );
  m_hReadThread = thread->m_hThread;
	return 0;
}

// Function:  StartWriterThread
// Purpose:  Starts the writer thread
// Inputs: None
// Outputs: 0
// Assumptions:  The thread created deletes itself when the thread exits.
// Comments: None
//
DWORD CDuplexClientPipe::StartWriterThread( void  )
{
  CWinThread *thread = NULL;

	thread = AfxBeginThread( CDuplexClientPipe::WriterThread, 
						(void*) this, 0,0,0,NULL );
  m_hWriteThread = thread->m_hThread;
	return 0;
}

// Function:  ReadPipe
// Purpose:  Reads data from the pipe
// Inputs:  None
// Outputs:  dwBytesRead - the number of bytes in the returned buffer
//           dwStatus - Status/Error associated with the read attempt
//           Returns a pointer to a data buffer on success or NULL on
//           failure.
// Assumptions:  the buffer returned must be deleted by the caller
// Comments: None
//
void* CDuplexClientPipe::ReadPipe( DWORD* dwBytesRead, DWORD* dwStatus )
{
	void* pBuffer = (void*) new BYTE[TB_PIPE_BUFFERSIZE];
	DWORD dwErr = 0;
	OVERLAPPED oRead;

	if ( NULL == pBuffer )
	{
		*dwBytesRead = 0;
		*dwStatus = ERROR_NOT_ENOUGH_MEMORY;
		return NULL;
	}

	memset( &oRead, 0, sizeof( OVERLAPPED ) );
	oRead.hEvent = m_hRead;

	if ( !ReadFile( m_hPipe, pBuffer, TB_PIPE_BUFFERSIZE,
						dwBytesRead, &oRead ) )
	{
		dwErr = GetLastError();		
		if ( ERROR_IO_PENDING == dwErr )
		{
			// Wait for overlapped IO to complete and reset error based
			// on result
			if( !GetOverlappedResult( m_hPipe, &oRead, dwBytesRead, true ) )
			{
				dwErr = GetLastError();
			}
			else
			{
				dwErr = 0;
			}
		}
		// Make sure we have an entire "message" for message based pipes
		if ( (ERROR_MORE_DATA == dwErr) && m_bMessageBased )
		{
			void* pFull;

			pFull = ReadMoreData( pBuffer, dwBytesRead, dwStatus );
			if (NULL == pFull)
			{
				// Failed Read
				dwErr = *dwStatus;
			}
			else
			{
				dwErr = 0;
				void* pTemp;
				pTemp = pBuffer;
				pBuffer = pFull;
				delete[] pTemp;
			}
		}
	}			
	if ( dwErr != 0 )
  { //Failure		
		delete[] pBuffer;
		*dwBytesRead = 0;
		*dwStatus = dwErr;
		return NULL;
	}
	return pBuffer;
}

// Function:  ReadMoreData
// Purpose:  Reads the remaining data for a message based pipe that received
//           a message too large for the read buffer.
// Inputs:  pStartData - the data that was read by the ReadFile command
//          dwBytesRead - contains the length of the data that was previosly
//                        read from the pipe.
// Outputs:  dwBytesRead - the number of bytes in the returned buffer
//           dwStatus - Status/Error associated with the read attempt
//           Returns a pointer to a data buffer containing the complete
//           message on success or NULL on failure.
// Assumptions:  the buffer returned must be deleted by the caller
// Comments: None
//
void* CDuplexClientPipe::ReadMoreData( void* pStartData, DWORD* dwBytesRead, DWORD* dwStatus )
{
	DWORD dwOrig = *dwBytesRead;
	DWORD dwSize;
	DWORD dwErr;
	OVERLAPPED oRead;

	// find out how much more data is available
	PeekNamedPipe(m_hPipe, NULL, 0, NULL, NULL, &dwSize );

	// Allocate a new buffer to hold the entire message
	BYTE* pBuf2 = new BYTE[dwSize + dwOrig];
	if ( NULL == pBuf2 )
	{
		*dwBytesRead = 0;
		*dwStatus = ERROR_NOT_ENOUGH_MEMORY;
		return NULL;
	}
	memmove( (void*)pBuf2, pStartData, dwOrig );
	memset( &oRead, 0, sizeof( OVERLAPPED ) );
	oRead.hEvent = m_hRead;
	if( ReadFile( m_hPipe, (void*)&pBuf2[dwOrig],
					dwSize, dwBytesRead, &oRead ))
	{
		*dwBytesRead += dwOrig;
		dwErr = 0;
	}
	else
	{
		dwErr = GetLastError();
		if ( ERROR_IO_PENDING == dwErr )
		{
			// Wait for overlapped IO to complete and reset error based
			// on result
			if( !GetOverlappedResult( m_hPipe, &oRead, dwBytesRead, true ) )
			{
				dwErr = GetLastError();
			}
			else
			{
				dwErr = 0;
				*dwBytesRead += dwOrig;
			}
		}
	}
	return (void*)pBuf2;
}

// Function: SimpleEvent
// Purpose:  Creates a simple unnamed event.
// Inputs:  None
// Outputs:  phEvent - set to handle of event that was created.
//           Returns 0 on success, or Win32 CreateEvent error on failure
// Assumptions:  None
// Comments:  None
//
DWORD CDuplexClientPipe::SimpleEvent(HANDLE* phEvent )
{
	DWORD dwErr = 0;

	if ( *phEvent == INVALID_HANDLE_VALUE )
	{
		*phEvent = CreateEvent( NULL, true, false, NULL );
	
		if ( *phEvent == INVALID_HANDLE_VALUE )
		{
			dwErr = GetLastError();
			return dwErr;
		}
	}
	return dwErr;
}


