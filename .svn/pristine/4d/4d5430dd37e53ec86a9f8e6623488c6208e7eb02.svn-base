// Setup.cpp : Defines the entry point for the DLL application.
//

// This .dll will run on the handheld before the installation starts. 
// It is extracted from the cab file and launched by the cab installer.  
// It's job is to stop the LaunchPad and RapNet applications 
// if they are running at the time the install begins.  
// This way, the install can reinstall the apps.  



#include "stdafx.h"
#include "Tlhelp32.h"
#include "Setup.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

TCHAR * LaunchPad = _T( "\\scot\\bin\\LaunchPadNet.exe" );
TCHAR * RAP		  = _T( "\\scot\\bin\\RapNet.exe" );
TCHAR * setupDll  = _T( "Setup.dll" );

void Debug( TCHAR *pMsg )
{
	// MessageBox( NULL, pMsg, setupDll, MB_OK );
	HANDLE hFile = CreateFile( _T("\\Temp\\NCR Handheld Rap Install.log"), GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if	( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD bw = 0;
		SetFilePointer( hFile, 0, 0, FILE_END );
		WriteFile( hFile, pMsg, _tcslen( pMsg ) * sizeof( TCHAR ), &bw, NULL );
		bw = 0;
		WriteFile( hFile, _T("\r\n"), 2 * sizeof( TCHAR ), &bw, NULL );
		CloseHandle( hFile );
	}

}


// Return the handle of the process matching the name given.  
HANDLE ProcessIsRunning( TCHAR * szProcessName )
{
	HANDLE hProcess = 0;
	HANDLE snap = CreateToolhelp32Snapshot( TH32CS_SNAPALL, 0 );
	TCHAR debug[ 512 ];
	TCHAR *szExeName = _tcsrchr( szProcessName, _T( '\\' ) ) + 1;

	PROCESSENTRY32 pe; 

	if	( snap == INVALID_HANDLE_VALUE )
	{
		_stprintf( debug, _T("Toolhelpsnapshot returned -1 <%d>"), GetLastError() );
		Debug( debug );
		return 0;
	}
	
	memset( &pe, 0, sizeof( pe ) );
	pe.dwSize = sizeof( pe );

	_stprintf( debug, _T( "searching for <%s><%s>" ), szProcessName, szExeName );
	Debug( debug );

	if	( Process32First( snap, &pe ) == TRUE )
	{
		_stprintf( debug, _T("Process: <%s>" ), pe.szExeFile );
		// Debug( debug );
		while ( ( _tcsicmp( szProcessName, pe.szExeFile ) != 0 )
				&& ( _tcsicmp( szExeName, pe.szExeFile ) != 0 ) )
		{
			if	( Process32Next( snap, &pe ) == FALSE)
			{
				break;
			}
			_stprintf( debug, _T("Process: <%s>" ), pe.szExeFile );
			// Debug( debug );
		}

		
		// if the file name matches this process, open it 
		if	( ( _tcsicmp( szProcessName, pe.szExeFile ) == 0 )
				|| ( _tcsicmp( szExeName, pe.szExeFile ) == 0 ) )
		{
			_stprintf( debug, _T("Process matched %s - getting handle" ), pe.szExeFile );
			Debug( debug );
			hProcess = OpenProcess( 0, FALSE, pe.th32ProcessID );
			if	( hProcess == NULL )
			{
				_stprintf( debug, _T( "OpenProcess returned %d"), GetLastError() );
				Debug( debug );
				hProcess = 0;
			}
			else
			{
				_stprintf( debug, _T( "Process <%s> Handle <%x>" ), szExeName, hProcess );
				Debug( debug );
			}
		}
	}
	else
	{
		_stprintf( debug, _T("Process32First returned False - %d, snap %x"), GetLastError(), snap );
		Debug( debug );
	}

	if	( hProcess == 0 )
	{

		_stprintf( debug, _T( "<%s> is not running" ), szExeName );
		Debug( debug );
	}


	CloseToolhelp32Snapshot( snap );

	return hProcess;
}


// Kill a running process by using the process handle  
void KillProcess( HANDLE hProcess )
{
	TerminateProcess( hProcess, 0 );
}


// Create a Tag file that LaunchPad will see: UpdSig.dat.
// When LaunchPad sees the file, it will shutdown RapNet 
// and itself.  

void CreateStopTag( TCHAR * szFilePath )
{
	HANDLE hFile;

	hFile = CreateFile( szFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	
	if	( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD bw = 0;
		WriteFile( hFile, "Signal Update to LaunchPad\r\n", 28, &bw, NULL ); 
		CloseHandle( hFile );
	}

}



// Create a Tag file that signals that the install is completed.  

void CreateCompletedTag( TCHAR * szFilePath )
{
	HANDLE hFile;

	hFile = CreateFile( szFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	
	if	( hFile != INVALID_HANDLE_VALUE )
	{
		DWORD bw = 0;
		WriteFile( hFile, "Install Completed\r\n", 28, &bw, NULL ); 
		CloseHandle( hFile );
	}

}



// return true if the file named in the param exists. 
// expecting a full file path.  

bool FileExists( TCHAR * szFilePath )
{
	WIN32_FIND_DATA fd;
	memset( &fd, 0, sizeof( fd ) );

	HANDLE hFind = FindFirstFile( szFilePath, &fd );
	if	( hFind != INVALID_HANDLE_VALUE ) 
	{
		FindClose( hFind );
		return true;
	}
	return false;
}


// Wait up to x milliseconds for a process to exit
// return true if the process has exited.  

bool WaitForExit( HANDLE hProcess, int mtime )
{
	DWORD ExitCode;
	BOOL bResult = GetExitCodeProcess( hProcess, &ExitCode );
	int i = 0;
	while ( ( bResult != FALSE ) && ( i < mtime ) && ( ExitCode == STILL_ACTIVE ) )
	{
		Sleep( 100 );
		i += 100;
		bResult = GetExitCodeProcess( hProcess, &ExitCode );
	}

	return ( bResult == FALSE ? true : ExitCode != STILL_ACTIVE ); 
}



// Default to allow the installation  
codeINSTALL_INIT initialResult = codeINSTALL_INIT_CONTINUE;

codeINSTALL_INIT Install_Init(	
								HWND        hwndParent,
								BOOL        fFirstCall,     // is this the first time this function is being called?
								BOOL        fPreviouslyInstalled,
								LPCTSTR     pszInstallDir )
{
	TCHAR debug[ 512 ];
	
	/*if	( !fFirstCall )
	{
		return initialResult; 
	}
	*/

	HANDLE hProcess = 0;
	bool bStoppedLaunchPad  = false;
	bool bStoppedRap		= false; 
	SYSTEMTIME st;
	GetLocalTime( &st );

	_stprintf( debug, _T( "NCR Handheld Rap install running %.02d/%.02d/%.02d %.02d:%.02d:%.02d\n" ),
							st.wMonth, st.wDay, st.wYear % 100, 
							st.wHour, st.wMinute, st.wSecond ); 

	Debug( debug );
	
	_stprintf( debug, _T("Checking for %s file exists" ), LaunchPad );

	Debug( debug );

	// Stop LaunchPad 
	if	( FileExists( LaunchPad ) )
	{
		Debug( _T("LaunchPad file found") );
		if	( ( hProcess = ProcessIsRunning( LaunchPad ) ) > 0 )
		{
			_stprintf( debug, _T("LaunchPad running - hProcess %0x"), hProcess );
			Debug( debug );
			CreateStopTag( _T( "\\scot\\bin\\UpdSig.dat" ) );
			Debug( _T("Created UpdSig.dat - wait for Lp to Stop" ) );
			bStoppedLaunchPad = WaitForExit( hProcess, 5000 ); // Give LaunchPad time to stop Rap and itself 
			if	( !bStoppedLaunchPad )
			{
				Debug( _T("LaunchPad didn't stop, killing"));
				KillProcess( hProcess );
				bStoppedLaunchPad = true;
			}
			CloseHandle( hProcess );
		}
		else
		{
			Debug( _T("LaunchPad is not running" ) );
			bStoppedLaunchPad = true;
		}
	}
	else
	{
		bStoppedLaunchPad = true;
		Debug( _T( "LaunchPad not installed" ) );
	}

	_stprintf( debug, _T("Checking for <%s> installed"), RAP );
	Debug( debug );

	// Stop Rap 
	if	( FileExists( RAP ) )
	{
		Debug( _T("Rap file found - Check if runnning" ) );
		if	( hProcess = ProcessIsRunning( RAP ) )
		{
			_stprintf( debug, _T("Rap running hProcess %x - killing" ), hProcess );
			KillProcess( hProcess );
			bStoppedRap = true;
			CloseHandle( hProcess );
		}
		else
		{
			Debug( _T("Rap not running") );
			bStoppedRap = true;
		}
	}
	else	// No apps installed, install new 
	{
		Debug( _T( "Rap not installed" ) );
		bStoppedRap	= true;
	}

	// if the apps are still running, don't attempt install 
	if	( ( bStoppedLaunchPad ) && ( bStoppedRap ) )
	{
		initialResult = codeINSTALL_INIT_CONTINUE;
	}
	else
	{
		initialResult = codeINSTALL_INIT_CANCEL;
	}

	return initialResult; 
}


codeINSTALL_EXIT Install_Exit(	
								HWND    hwndParent,
								LPCTSTR pszInstallDir,      // final install directory
								WORD    cFailedDirs,
								WORD    cFailedFiles,
								WORD    cFailedRegKeys,
								WORD    cFailedRegVals,
								WORD    cFailedShortcuts )
{
	/*
	if	(	( cFailedDirs > 0 )		||
			( cFailedFiles > 0 )	||
			( cFailedRegKeys > 0 )	||
			( cFailedRegVals > 0 )  ||
			( cFailedShortcuts > 0 ) ) 
	{
		return codeINSTALL_EXIT_UNINSTALL;
	}
	*/

	TCHAR debug[ 512 ];
	
	// Deleting this file will signal the UpdNotice app to 
	// start a reboot.  

	Debug( _T( "Install completing - checking for updsig.dat" ) );

	if	( FileExists( _T( "\\scot\\bin\\UpdSig.dat" ) ) )
	{
		Debug( _T( "UpdSig.dat found - deleting" ) );
		DeleteFile( _T( "\\scot\\bin\\UpdSig.dat" ) );
	}
	//SignalHHUpdate copied the rapnet.xml file to the temp directory.
	//By copying this file we can do a fresh install every time then 
	//copy rapnet.xml from temp directory if it is there
	//if	( FileExists( _T( "\\temp\\rapnet.xml" ) ) )
	//{
		/* BOOL IsDelete = DeleteFile( _T( "\\scot\\config\\rapnet.xml" ) );
		if (IsDelete == TRUE)
		{
			_stprintf( debug, _T( "Delete successfully rapnet.xml from config\n" )); 
			Debug( debug );

		}*/
	//	Debug( _T( "rapnet.xml found - copying" ) );
	//	BOOL IsCopy = CopyFile (_T( "\\temp\\rapnet.xml"), _T ("\\scot\\config\\rapnet.xml"), FALSE );
	//	if (IsCopy == TRUE)
	//	{
	//	_stprintf( debug, _T( "Successfully copied rapnet.xml to the scot directory\n" )); 
	//	Debug( debug );

	//	}
	//	else
	//	{
	//	_stprintf( debug, _T( "Did not copy rapnet.xml to the scot directory\n" )); 
	//	Debug( debug );

	//	}

		//DeleteFile( _T( "\\temp\\rapnet.xml" ) );
	//}

	// tn. Fix TAR 330198

	// Delete shortcut rapnet.lnk from "startup"
	BOOL IsDelete = DeleteFile( _T( "\\windows\\startup\\rapnet.lnk" ) );
	if (IsDelete == TRUE)
	{
		_stprintf( debug, _T( "Delete successfully shortcut RAPNet.lnk from startup\n" )); 
		Debug( debug );

	}
	else
	{
		_stprintf( debug, _T( "Fail to delete shortcut RAPNet.lnk from startup\n" )); 
		Debug( debug );

	}

	// Delete shortcut rapnet.lnk from "start menu"
	IsDelete = DeleteFile( _T( "\\windows\\start menu\\rapnet.lnk" ) );
	if (IsDelete == TRUE)
	{
		_stprintf( debug, _T( "Delete successfully shortcut RAPNet.lnk from start menu\n" )); 
		Debug( debug );

	}
	else
	{
		_stprintf( debug, _T( "Fail to delete shortcut RAPNet.lnk from start menu\n" )); 
		Debug( debug );

	}
	// Delete shortcut LaunchPad.lnk from "startup"
	IsDelete = DeleteFile( _T( "\\windows\\startup\\LaunchPad.lnk" ) );
	if (IsDelete == TRUE)
	{
		_stprintf( debug, _T( "Delete successfully shortcut LaunchPad.lnk from startup\n" )); 
		Debug( debug );

	}
	else
	{
		_stprintf( debug, _T( "Fail to delete shortcut LaunchPad.lnk from startup\n" )); 
		Debug( debug );

	}

// Delete shortcut Launchpad.lnk from "start menu"
	IsDelete = DeleteFile( _T( "\\windows\\start menu\\LaunchPad.lnk" ) );
	if (IsDelete == TRUE)
	{
		_stprintf( debug, _T( "Delete successfully shortcut LaunchPad.lnk from start menu\n" )); 
		Debug( debug );

	}
	else
	{
		_stprintf( debug, _T( "Fail to delete shortcut LaunchPad.lnk from start menu\n" )); 
		Debug( debug );

	}
    // delete HHRAP from starting Meijer request
	IsDelete = DeleteFile( _T( "\\windows\\startup\\HHRAP.lnk" ) );
	if (IsDelete == TRUE)
	{
		_stprintf( debug, _T( "Delete successfully shortcut HHRAP.lnk from startup\n" )); 
		Debug( debug );

	}
	else
	{
		_stprintf( debug, _T( "Fail to delete shortcut HHRAP.lnk from startup\n" )); 
		Debug( debug );

	}

    // Delete shortcut HHRAP.lnk from "start menu"
	IsDelete = DeleteFile( _T( "\\windows\\start menu\\HHRAP.lnk" ) );
	if (IsDelete == TRUE)
	{
		_stprintf( debug, _T( "Delete successfully shortcut HHRAP.lnk from start menu\n" )); 
		Debug( debug );

	}
	else
	{
		_stprintf( debug, _T( "Fail to delete shortcut LaunchPad.lnk from start menu\n" )); 
		Debug( debug );

	}
	// check for the existence of the reboot program 

	Debug( _T( "Attempting to run HHInstallComplete.exe\n" ) );

	if	( FileExists( _T( "\\scot\\bin\\NOReboot.txt" ) ) )
	{
		BOOL IsDelete = DeleteFile( _T( "\\scot\\bin\\NOReboot.txt" ) );
		if (IsDelete == TRUE)
		{
			_stprintf( debug, _T( "Delete successfully shortcut NOReboot.txt from scot\bin \n" )); 
			Debug( debug );

		}
		else
		{
			_stprintf( debug, _T( "Fail to delete shortcut NOReboot.txt from scot\bin\n" )); 
			Debug( debug );
		}
	}
	else
	{

		if	( !FileExists( _T( "\\scot\\bin\\HHInstallComplete.exe" ) ) )
		{
			Debug( _T( "HHInstallComplete.exe file not found" ) );
		}

		PROCESS_INFORMATION pi;
		memset( &pi, 0, sizeof( pi ) );
		
		BOOL bOk = CreateProcess( _T( "\\scot\\bin\\HHInstallComplete.exe" ), _T(""), 
							NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi );

		if	( bOk == FALSE )
		{
			_stprintf( debug, _T("Create process HHInstallComplete.exe returns %d\n" ), GetLastError() );
			Debug( debug );
		}
		else
		{
			Debug( _T( "Success - CreateProcess on HHInstallComplete\n" ) );
		}
	}
	CreateStopTag( _T( "\\scot\\bin\\CompInstall.dat" ) );
	
	HKEY hKey;
    LONG lRet;

	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\UIControls"), 
               0, KEY_ALL_ACCESS, &hKey);

    RegDeleteKey(hKey, _T("Log"));
    RegCloseKey( hKey );

    lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\NCRDisplayEngine"), 
               0, KEY_ALL_ACCESS, &hKey);

    RegDeleteKey(hKey, _T("Log"));
    RegCloseKey( hKey );



	SYSTEMTIME st;
				
	GetLocalTime( &st );

	_stprintf( debug, _T( "NCR Handheld Rap install ending %.02d/%.02d/%.02d %.02d:%.02d:%.02d\n" ),
							st.wMonth, st.wDay, st.wYear % 100, 
							st.wHour, st.wMinute, st.wSecond ); 

	Debug( debug );



	return codeINSTALL_EXIT_DONE;
}


codeUNINSTALL_INIT Uninstall_Init(
									HWND        hwndParent,
									LPCTSTR     pszInstallDir
								)
{
	return codeUNINSTALL_INIT_CONTINUE;
}


codeUNINSTALL_EXIT Uninstall_Exit( HWND hwndParent )
{
	return codeUNINSTALL_EXIT_DONE;
}



