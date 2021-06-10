// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__3B8D9119_87C3_427A_B539_4FBA1701AC2F__INCLUDED_)
#define AFX_LOG_H__3B8D9119_87C3_427A_B539_4FBA1701AC2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MYTRACE CLog::GetLog()->Log
#define PREFIX_LEN		150

const UINT TRACE_API            = 0x00000001; // API method entry and exit
const UINT TRACE_ERROR          = 0x00000002; // Error event - major unrecoverable problem
const UINT TRACE_WARNING        = 0x00000004; // Warning event - minor recoverable problem
const UINT TRACE_INFO	        = 0x00000008; // Informational event - trace some normal activity
const UINT TRACE_EXTENSIVE      = 0x00000010; // Trace event - extensive tracing
const UINT TRACE_PERFORMANCE    = 0x00000020; // Performance tracing
const UINT TRACE_ALL		    = 0xFFFFFFFF; // Trace all

//#include <ATLComTime.h>		// COleDateTime.

/////////////////////////////////////////////////////////////////////////////
//	CLog class
//

/**
 * class CLog
 * \brief A really nice logging class from RemoteMessage.  Don't know who wrote this.
 */
class CLog  
{
public:
	~CLog();
	static void DestroyLog();
	void GetConfig( LPCTSTR szSubKey );
    static CLog* GetLog();
	void Log( UINT nLevel, LPCTSTR szFormat, ... );
    void LogFileVersion( UINT nLevel );
	inline void SetDebugOutput( BOOL bOutput ) { m_bDebugOutput = bOutput; }
	inline void SetEnabled( BOOL bEnabled ) { m_bEnabled = bEnabled; }
	void SetFileOutput( BOOL bOutput, LPCTSTR szFile = NULL, BOOL bOverwrite = FALSE, BOOL bFlush = FALSE, UINT nMaxSizeInKB = 5120 );
	inline void SetLevel( UINT nLevel ) { m_nLevel = nLevel; }

	/*lint -e(534) */
	inline void SetPrefix( LPCTSTR szPrefix ) { _tcsncpy( m_szPrefix, szPrefix, PREFIX_LEN-1 ); m_szPrefix[PREFIX_LEN-1] = NULL; }

protected:
	CLog();

private:
	void VerifyMaxLogSize();
	
	BOOL m_bDebugOutput;
	BOOL m_bEnabled;
	BOOL m_bFileOutput;
    BOOL m_bFlush;
	HANDLE m_hFile;
	UINT m_nLevel;
	UINT m_nMaxSizeInKB;
	CRITICAL_SECTION m_sect;
	TCHAR m_szFile[MAX_PATH + 1];
	TCHAR m_szPrefix[PREFIX_LEN];
	static CLog* ms_pLog;
};

#endif // !defined(AFX_LOG_H__3B8D9119_87C3_427A_B539_4FBA1701AC2F__INCLUDED_)
