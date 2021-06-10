// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Log.h"

#ifndef _WIN32_WCE
#pragma comment(lib, "version.lib")
#endif

#include <ctime>


/////////////////////////////////////////////////////////////////////////////
//	CLog class
//

// Static members //
CLog* CLog::ms_pLog = NULL;

//************************************************************************ 
//
// Function:		CLog
// Purpose:			Constructor
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CLog::CLog() :
	m_bDebugOutput( FALSE ),
	m_bEnabled( FALSE ),
	m_bFileOutput( FALSE ),
    m_bFlush( FALSE ),
	m_hFile( INVALID_HANDLE_VALUE ),
	m_nLevel( 0 ),
	m_nMaxSizeInKB( 5120 )	
{
	InitializeCriticalSection( &m_sect );

	_tcscpy( m_szFile, _T("") );
}

//************************************************************************ 
//
// Function:		~CLog
// Purpose:			Destructor
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CLog::~CLog()
{
	if( INVALID_HANDLE_VALUE != m_hFile )
	{
		CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
		_tcscpy( m_szFile, _T("") );
	}

	DeleteCriticalSection( &m_sect );
}

//************************************************************************ 
//
// Function:		DestroyLog
//
// Purpose:			Destroys the CLog instance
// 
// Parameters:
// Returns:
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void CLog::DestroyLog()
{
	if( ms_pLog )
	{
		delete ms_pLog;
		ms_pLog  = NULL;
	}
}

//************************************************************************ 
//
// Function:		GetConfig
//
// Purpose:			Get logging information from registry
// 
// Parameters:
// Returns:
// Calls:
// Side Effects:
// Notes:			
//
//************************************************************************
void CLog::GetConfig( LPCTSTR szSubKey )
{
	static const TCHAR DEBUGLEVEL[] = _T("DebugLevel");
	static const TCHAR LOGFILE[] = _T("LogFile");
	static const TCHAR LOGFLUSH[] = _T("LogFlush");
    static const TCHAR LOGKEY[] = _T("\\Log");
    static const TCHAR LOGMAXSIZE[] = _T("LogMaxSize");
    	
	HKEY hKey = NULL;
	DWORD dwLevel = 0, dwMaxSize = 5120;
	TCHAR szFile[MAX_PATH + 1] = _T("");
    BOOL bFlush = FALSE;
    
    TCHAR szLogSubKey[MAX_PATH + 1] = _T("");
    _tcscpy( szLogSubKey, szSubKey );
    _tcscat( szLogSubKey, LOGKEY ); 	
 	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, szLogSubKey, 0, KEY_READ, &hKey ) == ERROR_SUCCESS )
	{
		DWORD dwSize = sizeof( DWORD ), dwType = 0;
		RegQueryValueEx( hKey, DEBUGLEVEL, NULL, &dwType, (BYTE*)&dwLevel, &dwSize );
		
		dwSize = MAX_PATH;
		RegQueryValueEx( hKey, LOGFILE, NULL, &dwType, (BYTE*)szFile, &dwSize );

		dwSize = sizeof( DWORD );
		RegQueryValueEx( hKey, LOGFLUSH, NULL, &dwType, (BYTE*)&bFlush, &dwSize );

        dwSize = sizeof( DWORD );
		RegQueryValueEx( hKey, LOGMAXSIZE, NULL, &dwType, (BYTE*)&dwMaxSize, &dwSize );
		
		RegCloseKey( hKey );
	}
	
	if( dwLevel > 0 )
	{
		CLog::GetLog()->SetEnabled( TRUE );
		CLog::GetLog()->SetLevel( dwLevel );
		
#ifdef _DEBUG
	CLog::GetLog()->SetDebugOutput( TRUE );
#endif

		if( _tcslen( szFile ) )
		{
			CLog::GetLog()->SetFileOutput( TRUE, szFile, FALSE, bFlush, dwMaxSize );
		}
	}
    else
    {
        CLog::GetLog()->SetEnabled( FALSE );
    }
}

//************************************************************************ 
//
// Function:		LogFileVersion
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
void CLog::LogFileVersion( UINT nLevel )
{
    if( !m_bEnabled || ( nLevel & m_nLevel ) == 0 )
	{
		return;
	}
    
    TCHAR szFile[MAX_PATH + 1] = _T("");
	if( GetModuleFileName( NULL, szFile, MAX_PATH ) )
	{
	    DWORD dwArg = 0;
        DWORD dwInfoSize = GetFileVersionInfoSize( (LPTSTR)szFile, &dwArg );
        if( dwInfoSize > 0 )
        {
            BYTE *pBuff = new BYTE[dwInfoSize];
            if( GetFileVersionInfo( (LPTSTR)szFile, 0, dwInfoSize, pBuff ) )
            {
                VS_FIXEDFILEINFO *vInfo = NULL;
                UINT uInfoSize = 0;
                if( VerQueryValue( pBuff, _T("\\"), (LPVOID*)&vInfo, &uInfoSize ) )
                {
                    if( uInfoSize > 0 )
                    {
                        Log( nLevel, _T("File version: %d.%d.%d.%d"), HIWORD( vInfo->dwFileVersionMS ), LOWORD( vInfo->dwFileVersionMS ),
						    HIWORD( vInfo->dwFileVersionLS ), LOWORD( vInfo->dwFileVersionLS ) );
                    }
                    else
                    {
                        Log( nLevel, _T("Version information not available") );
                    }               
                }
                else
                {
                    Log( nLevel, _T("Version information not available") );
                }           
            }
            else
            {
                Log( nLevel, _T("Error retrieving version info") );
            }
        
            delete[] pBuff;
        }
        else
        {
            Log( nLevel, _T("No version info available") );
        }
	}
}

//************************************************************************ 
//
// Function:		GetLog
//
// Purpose:			Gets a pointer to the CLog singleton
// 
// Parameters:		
// 
// Returns:			Pointer to the CLog singleton
//					
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CLog* CLog::GetLog()
{ 
	if( ms_pLog == NULL )
	{
		ms_pLog = new CLog;
	}
	
	return ms_pLog;
}

//************************************************************************ 
//
// Function:		Log
//
// Purpose:			Logs a string to a file and/or debug output
// 
// Parameters:		[in] nLevel - Debug level
//					[in] szFormat - A format string as used in the run-time function printf
//					[in] ... - The names of the variables whose values should be dumped
// 
// Returns:
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************

TCHAR szOutput[1024];
TCHAR szBuffer[512];
void CLog::Log( UINT nLevel, LPCTSTR szFormat, ... )
{
	//_ASSERT(m_bEnabled == TRUE);			// JV

	if( !m_bEnabled || ( nLevel & m_nLevel ) == 0 )
	{
		return;
	}
		
	EnterCriticalSection( &m_sect );
	
	szOutput[0] = szBuffer[0] = 0;
	if( _tcslen( m_szPrefix ) )
	{
		_tcscpy( szOutput, m_szPrefix );
		_tcscat( szOutput, _T(": ") );
	}
	/*
    COleDateTime dt( COleDateTime::GetCurrentTime() );
	_stprintf( szBuffer, _T("%.2d/%.2d %.2d:%.2d:%.2d   %d %X> "), dt.GetMonth(), dt.GetDay(), dt.GetHour(),
		dt.GetMinute(), dt.GetSecond(), GetTickCount(), GetCurrentThreadId() );
    _tcscat( szOutput, szBuffer );
	*/
	time_t theTime;
	time( &theTime );
	struct tm *t = localtime( &theTime );  // convert to local
    
	
	 _stprintf( szBuffer, _T("%d/%d/%4d %d:%d:%d; "), t->tm_mon+1, t->tm_mday, 1900+t->tm_year, t->tm_hour, t->tm_min, t->tm_sec);
	 _tcscat( szOutput, szBuffer );
     _stprintf( szBuffer, _T("%d %X> "), GetTickCount(), GetCurrentThreadId() );
    _tcscat( szOutput, szBuffer );

	va_list args;
	va_start( args, szFormat );
	_vsntprintf( szBuffer, ( sizeof( szBuffer ) / sizeof( TCHAR ) ) - 1, szFormat, args );
	va_end( args );

	_tcscat( szOutput, szBuffer );
	_tcscat( szOutput, _T("\r\n") );
		
	if( m_bDebugOutput )
	{
		OutputDebugString( szOutput );
	}
	
	if( m_bFileOutput )
	{
		if( INVALID_HANDLE_VALUE != m_hFile )
		{
			VerifyMaxLogSize();
						
			DWORD dwBytesWritten = 0;
			WriteFile( m_hFile, szOutput, (DWORD)(_tcslen( szOutput ) * sizeof( TCHAR )), &dwBytesWritten, NULL );
            if( m_bFlush )
            {
				FlushFileBuffers( m_hFile );
            }
		}
	}
	
	LeaveCriticalSection( &m_sect );
}

//************************************************************************ 
//
// Function:		SetFileOutput
//
// Purpose:			Sets logging output to a new file
// 
// Parameters:		[in] bOutput - TRUE if opening new file, FALSE if closing current file
//					[in] szFile - Name of file to open, ignored if bOutput is FALSE
//					[in] bOverwrite - TRUE if overwrite file, ignored if bOutput is FALSE
//					[in] bFlush - TRUE if flush file after every log, ignored if bOutput is FALSE
//					[in] nMaxSizeInKB - Maximum log file size, ignored if bOutput is FALSE
// 
// Returns:
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void CLog::SetFileOutput( BOOL bOutput, LPCTSTR szFile /*= NULL*/, BOOL bOverwrite /*= FALSE*/, BOOL bFlush /*= FALSE*/, UINT nMaxSizeInKB /*= 5120*/ )
{
	EnterCriticalSection( &m_sect );
	
	m_bFileOutput = bOutput;

	if( INVALID_HANDLE_VALUE != m_hFile )
	{
		CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
	}

	if( bOutput )
	{
		m_nMaxSizeInKB = nMaxSizeInKB;
        m_bFlush = bFlush;
		_tcscpy( m_szFile, szFile ? szFile : _T("Log.log") );

#ifdef UNICODE
		// added to ensure unicode text files have the appropriate prefix FFFE to be recognized
		BOOL bSetUnicodeHeader = GetFileAttributes( m_szFile ) == ( (DWORD)-1 );
#endif

		m_hFile = CreateFile( m_szFile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, bOverwrite ? CREATE_ALWAYS : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
        SetFilePointer( m_hFile, 0, NULL, FILE_END );

#ifdef UNICODE
		if( bSetUnicodeHeader )
		{
			DWORD dwWritten = 0;
			// value is a byte value in ANSI - don't convert this to generic text
			WriteFile( m_hFile, "\xFF\xFE", 2, &dwWritten, NULL );
 		}
#endif

	}
	
	if( INVALID_HANDLE_VALUE == m_hFile )
	{
		_tcscpy( m_szFile, _T("") );
	}

	LeaveCriticalSection( &m_sect );
}

//************************************************************************ 
//
// Function:		VerifyMaxLogSize
//
// Purpose:			Verifies the log file size and deletes it if too big
// 
// Parameters:
// Returns:
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void CLog::VerifyMaxLogSize()
{
	if( INVALID_HANDLE_VALUE != m_hFile )
	{
		if( GetFileSize( m_hFile, NULL ) >= ( m_nMaxSizeInKB * 1024 ) )
		{
		    CloseHandle( m_hFile );
            m_hFile = INVALID_HANDLE_VALUE;

            TCHAR szBackup[MAX_PATH + 1] = _T("");
            _tcscpy( szBackup, m_szFile );
            _tcscat( szBackup, _T(".bak") );
            DeleteFile( szBackup );
			MoveFile( m_szFile, szBackup );

            SetFileOutput( TRUE, m_szFile, FALSE, m_bFlush, m_nMaxSizeInKB );
		}		
	}
}
