// DuplexClientPipe.h: interface for the CDuplexClientPipe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUPLEXCLIENTPIPE_H__14643B05_1F8A_4702_9E44_4DBCFEF64F8D__INCLUDED_)
#define AFX_DUPLEXCLIENTPIPE_H__14643B05_1F8A_4702_9E44_4DBCFEF64F8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>
#include "PipeQueue.h"
#include "APipeSink.h"

#define BUFFERSIZE_MULTIPLIER 12
#define TB_PIPE_BUFFERSIZE	1024 * BUFFERSIZE_MULTIPLIER
#define PIPE_NAME_MAX		1024
#define PIPE_PREFIX			_T("\\\\.\\pipe\\")
#define PIPE_NAME_FORMAT	_T("\\\\.\\pipe\\%s")

#define READ_THREAD_INDEX 0
#define WRITE_THREAD_INDEX 1

class CDuplexClientPipe 
{
public:
public:
	CDuplexClientPipe();
	virtual ~CDuplexClientPipe();
	DWORD Initialize( LPCTSTR strPipeName, DWORD dwPipeID, BOOL bMessageBased );
	DWORD Uninitialize( void );
	BOOL WriteData( void* pData, DWORD dwByteLength );
	DWORD WaitForServer( LPCTSTR strPipeName);
	void SetPipeSink( APipeSink* pSink );

private:
  DWORD DoRestart();
	static UINT ReaderThread( void* lpParam );
	static UINT WriterThread( void* lpParam );
  static UINT MonitorThread( void *lpParam);
	DWORD OpenPipe( LPCTSTR strPipeName );
	DWORD StartReaderThread( void );
	DWORD StartWriterThread( void  );
	void* ReadPipe( DWORD* dwBytesRead, DWORD* dwStatus );
	void* ReadMoreData( void* pStartData, DWORD* dwBytesRead, DWORD* dwStatus );
	DWORD SimpleEvent( HANDLE* phEvent );

	BOOL        m_bMessageBased;
	HANDLE			m_hPipe;
	HANDLE			m_hTerminate;
	HANDLE			m_hRead;
	HANDLE			m_hWrite;
 	HANDLE      m_hReadThread;
  HANDLE      m_hWriteThread;
  HANDLE      m_hMonitorThread;
	CPipeQueue	m_PipeQueue;
	APipeSink*	m_pSink;
	DWORD			  m_dwPipeID;
  CString     m_csPipeName;
};

#endif // !defined(AFX_DUPLEXCLIENTPIPE_H__14643B05_1F8A_4702_9E44_4DBCFEF64F8D__INCLUDED_)
