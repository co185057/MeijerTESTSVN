// common.cpp


#include "stdafx.h"
#include "resource.h"

BOOL ExecuteCommand(LPCTSTR sClass,
					          const CStringArray&  aCommands,
                    CString m_sWorkingDirectory)
{
  BOOL bOk = TRUE;
  for ( int nIndex = 0; nIndex < aCommands.GetSize(); nIndex++ )
  {
    PROCESS_INFORMATION processInfo;
    ZeroMemory( &processInfo, sizeof( PROCESS_INFORMATION ) );
    STARTUPINFO startupInfo = 
	  {
      sizeof( STARTUPINFO ), NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 
      STARTF_USESHOWWINDOW, SW_HIDE, 0, NULL, 0 
    };
    BOOL bResult = CreateProcess( NULL, 
                                  (_TCHAR*)(LPCTSTR)aCommands[nIndex], 
                                  NULL, 
                                  NULL, 
                                  FALSE, 
                                  0, 
                                  NULL, 
                                  m_sWorkingDirectory ,
                                  &startupInfo,
                                  &processInfo );
    DWORD nError = 0;
    if ( bResult == FALSE )
      nError = ::GetLastError();
    else
    {
      WaitForSingleObject( processInfo.hProcess, INFINITE );
      if ( ::GetExitCodeProcess( processInfo.hProcess, &nError ) == FALSE )
        nError = ::GetLastError();
      CloseHandle( processInfo.hProcess );
      CloseHandle( processInfo.hThread );
    }
    if ( nError != 0 )
    {
      LPTSTR lpMsgBuf;
      FormatMessage( 
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, nError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
            (LPTSTR) &lpMsgBuf,    0,    NULL );// Display the string.
      CString sText = aCommands[nIndex] + _T('\n') + CString( lpMsgBuf );
      LocalFree( lpMsgBuf );
      if ( lpMsgBuf == NULL )
        sText.Format( _T("The %s command returned %d."), aCommands[nIndex], nError );     
      AfxMessageBox( sText);
      bOk = FALSE;
    }
  }
  return bOk;
}
