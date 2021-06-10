
#if !defined(_LOGFILE_H_)
#define _LOGFILE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <share.h>
#include <stdio.h>
#include <time.h>

const long g_LogFileSize = 3000000;

class CLogFile
{
public:
	CLogFile(LPCTSTR szFileName)
	{
  		_tcscpy_s(m_szFileName, MAX_PATH, szFileName);
		m_hMutex = ::CreateMutex(
			NULL, 
			FALSE, 
         szFileName
			); 
	}

   virtual ~CLogFile()
	{
        if ( m_hMutex )
            ::CloseHandle(m_hMutex);
	}

	void InitializeFileName(LPCTSTR pszFileName)
	{
		_tcscpy_s(m_szFileName, MAX_PATH, pszFileName);
	}

	void LogMessage(LPCTSTR pszMessage)
	{
        FILE* pOutFile;
		::WaitForSingleObject(m_hMutex, INFINITE);
		__try	{
            pOutFile = _tfsopen(m_szFileName, _T("a"), _SH_DENYNO);
			if (pOutFile) {
				fseek(pOutFile, 0L, SEEK_END);

            TCHAR timebuf[20];
			_tstrdate_s(timebuf, sizeof(timebuf)/sizeof(TCHAR));
			timebuf[5] = _T(' ');
			_tstrtime_s(&timebuf[6], (sizeof(timebuf)/sizeof(TCHAR)) -6);

				_ftprintf(pOutFile, _T("%s: %s\n"), timebuf, pszMessage);
            if (ftell(pOutFile) > g_LogFileSize) {
               fflush(pOutFile);
               fclose (pOutFile);

               TCHAR szbakFileName[MAX_PATH];
               _tcscpy_s(szbakFileName, MAX_PATH, m_szFileName);
               _tcscat_s(szbakFileName, MAX_PATH, _T(".bak"));

  				DeleteFile( szbakFileName );
				MoveFile( m_szFileName, szbakFileName );
               pOutFile= _tfsopen(m_szFileName, _T("a"), _SH_DENYNO);
            }
			}
		}
		__finally {
            if ( pOutFile )
                fclose(pOutFile);
			::ReleaseMutex(m_hMutex);
		}
	}
	
private:
	TCHAR            m_szFileName[MAX_PATH];
	HANDLE           m_hMutex;
};

#endif // !defined(_LOGFILE_H_)

