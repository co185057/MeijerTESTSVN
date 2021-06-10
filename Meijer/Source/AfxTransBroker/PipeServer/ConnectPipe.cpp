#include "StdAfx.h"
#include "connectpipe.h"
#include "commserverdlg.h"

#define T_ID _T("ConnectPipe")

CConnectPipe::CConnectPipe(LPCTSTR lpszPipeName /*=NULL*/)
:CConnectBase(),
m_PipeName(lpszPipeName)
{
}

CConnectPipe::~CConnectPipe(void)
{
    if (m_connData.connId == SOURCEID_POSWRITEPIPE)
        CConnectBase::m_pPOSDestination=NULL;
    if (m_connData.connId == SOURCEID_TBWRITEPIPE)
        CConnectBase::m_pTBDestination=NULL;
}

bool CConnectPipe::Initialize()
{
	return true;
}

void CConnectPipe::Close()
{
	if (m_handle != INVALID_HANDLE_VALUE) {
//    any of these calls can block forever, preventing a shutdown.
//    commented for now, shuting down anyway.
//		FlushFileBuffers(m_handle);
//		DisconnectNamedPipe( m_handle);
//		CloseHandle( m_handle);

		m_handle = INVALID_HANDLE_VALUE;
	}
}

int CConnectPipe::Receive(void *lpBuf, int nBufLen, int nFlags)
{
	DWORD cbBytesRead=0;
    ReadFile( m_handle, // handle to pipe
              lpBuf,    // buffer to receive data
              nBufLen,              // size of buffer
              &cbBytesRead,         // number of bytes read
              NULL);                // not overlapped I/O
	m_connData.BytesRead += cbBytesRead;
   // don't bump the readPasses here because this is also used for reading header length
   // and we don't want to count that as a separate pass. Caller is responsible for 
   // calling UpdateReceiveStats()
	return cbBytesRead;
}

void CConnectPipe::UpdateReceiveStats()
{
   m_connData.ReadPasses++;
	Received();
}

int CConnectPipe::Send(CConnectBase *pConnSource, const void *lpBuf, int nBufLen, CTraceSystem *pts/*=NULL*/)
{
	DWORD cbWritten=0;
    WriteFile(	m_handle,            // handle to pipe
				lpBuf,           // buffer to write from (strip header)
				nBufLen,                // number of bytes to write
				&cbWritten,             // number of bytes written
				NULL);                  // not overlapped I/O
	m_connData.WritePasses++;
	m_connData.BytesWritten += cbWritten;
	Sent(pConnSource);
	return cbWritten;
}

HANDLE CConnectPipe::CreatePipe()
{
    return CreateNamedPipe( 
		m_connData.pipeName.GetBuffer(0),    // pipe name
        PIPE_ACCESS_DUPLEX,       // read/write access
        PIPE_TYPE_MESSAGE |       // message type pipe
        PIPE_READMODE_MESSAGE |   // message-read mode
        PIPE_WAIT,                // blocking mode
        PIPE_UNLIMITED_INSTANCES, // max. instances
        BUFSIZE,                  // output buffer size
        BUFSIZE,                  // input buffer size
        PIPE_TIMEOUT,             // client time-out
        NULL);                    // no security attribute
}

bool CConnectPipe::Accept()
{
  BOOL fConnected = false;
  HANDLE hPipe = NULL;

  // The main loop creates an instance of the named pipe and
  // then waits for a client to connect to it. When the client
  // connects, a thread is created to handle communications
  // with that client, and the loop is repeated.
	while (!CConnectBase::bTerminate) { 
		hPipe=CreatePipe();
      if (hPipe == INVALID_HANDLE_VALUE) {
			break; 
      }
		// Wait for the client to connect; if it succeeds,
		// the function returns a nonzero value. If the function returns
		// zero, GetLastError returns ERROR_PIPE_CONNECTED.
		fConnected = ConnectNamedPipe( hPipe, NULL) ?TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		if (fConnected)
			OnAccept(hPipe);
      else { // The client could not connect, so close the pipe.
			CloseHandle(hPipe);
			break;
      }
	}
	if (!fConnected) {
		CoInitialize(NULL);
		CTraceSystem ts(CcommServerDlg::m_csTraceFileName);
		ts.Create();
		ts.SetModuleID(TM_MODULE_ID);
		ts.Trace(TM_ERROR, _T("Connecting Pipe Failed"));
		CoUninitialize();
	}
	return true;
}

