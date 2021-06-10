//Global function for parsing file path with environment variable


#ifndef _INC_STD_FASTLANE
#define _INC_STD_FASTLANE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h> 
#include <stdlib.h> 


////////////////////////////////////////////////////////////////////////////////////
//
// Function:	  GetPartitionPath
// 
// Purpose:		  Retrieves partition environment variables value and 
//                replaces supplied variable path to its absolute partition path.
// 
// Parameters:	  [in]  sSrc - A string that is a path that contains 
//                      the environment variables "BOOT_DRIVE", "OS_DRIVE", 
//                      "APP_DRIVE", "DATA_DRIVE" and "TEMP_DRIVE".
//                [out] sDest - Points to a buffer that is to receive 
//                      the absolute partition path.
// 
// Returns:	      true if successful
// 			      false if failed
// 
// Calls:		  wmemset, memset, _tcstok, _tcsinc, _tgetenv, _tcschr, _tcscat
// 
// Side Effects:  None
// 
// Notes:		  1)  This function can only be used on C++ projects and sDest 
//                    argument will be equal to sSrc when the function fails and
//                    no exception is thrown.
//                2)  sDest will retain its original value when it is NULL when
//                    passed to the function, when sSrc is NULL, and when an
//                    exception is thrown.
//                3)  Caller of this function must make sure sDest has memory 
//                    allocation greater than or equal to sSrc so we are safe
//                    with _tcscpy and _tcscat functions.
// 
////////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_PARTITION_DRIVE   _T("C:")
#define DEFAULT_APPLICATION_DIR   _T("scot")
inline bool GetPartitionPath( LPCTSTR sSrc, LPTSTR sDest, DWORD nSize=_MAX_PATH )
{
    bool bResult = false;
    // make sure sSrc or sDest are not NULL before continuing with
    // our operation. In Debug build, uninitialized variables are not
    // equal to NULL, so using sSrc and sDest should be caught by
    // our try/catch operation.
    if ( sSrc != 0 && sDest != 0 && nSize > 0 )
    {
        try
        {
            DWORD nChars = ExpandEnvironmentStrings( sSrc, sDest, nSize );
            if ( nChars != 0 && nChars < nSize )
            {
                // successful
                // now set default partition to C: if variables are not defined.
                // find %APP_DRIVE%, %BOOT_DRIVE%, %OS_DRIVE%, %DATA_DRIVE%, and %TEMP_DRIVE%
                TCHAR* sSearchStrs[] = { 
                    _T("%BOOT_DRIVE%"),
                    _T("%OS_DRIVE%"),
                    _T("%APP_DRIVE%"),
                    _T("%DATA_DRIVE%"),
                    _T("%TEMP_DRIVE%"),
					_T("%APP_DIR%") };
                int nSearch = sizeof(sSearchStrs)/sizeof(*sSearchStrs);
                for ( int i=0; i<nSearch; ++i )
                {
                    bool bDone = false;
                    do
                    {
                        // we need to loop until we have replaced all undefined
                        // environment partition variables.
                        TCHAR* sFound = _tcsstr( sDest, sSearchStrs[i] );
                        if ( sFound != 0 )
                        {
                            // a variable is not defined, lets change it to default value.
                            TCHAR* sTmpBuffer = new TCHAR[ nSize ];
                        #if defined( _UNICODE ) || defined( UNICODE )
                            wmemset( sTmpBuffer, 0, nSize );
                        #else
                            memset( sTmpBuffer, 0, nSize );
                        #endif
                            _tcsncpy( sTmpBuffer, sDest, sFound - sDest );
							if( _tcscmp(sSearchStrs[i], _T("%APP_DIR%")) == 0 )
								_tcscat( sTmpBuffer, DEFAULT_APPLICATION_DIR );
							else
								_tcscat( sTmpBuffer, DEFAULT_PARTITION_DRIVE );
                            _tcscat( sTmpBuffer, sFound + _tcslen(sSearchStrs[i]) );
                            _tcscpy( sDest, sTmpBuffer );
                            delete [] sTmpBuffer;
                            sTmpBuffer = 0;
                        }
                        else
                        {
                            bDone = true;
                        }
                    } while ( false == bDone );
                }
                bResult = true;
            }
            else
            {
                // failed.
                // sDest buffer (nSize) is to small to hold the expanded string.
            }
        }
        catch (...)
        {
            // some problems
            bResult = false;
        }
    }
    return bResult;
}



////////////////////////////////////////////////////////////////////////////////////
//
// Function:	  ExpandPartitionStrings
// 
// Purpose:		  Searches a string for occurrences of different partition
//                environment variables in a string and replaces them with its
//                corresponding absolute path.
// 
// Parameters:	  [in]  sSrc - A string that may contains
//                      the environment variables "BOOT_DRIVE", "OS_DRIVE", 
//                      "APP_DRIVE", "DATA_DRIVE" and "TEMP_DRIVE".
//                [out] sDest - Points to a buffer that is to receive the resulting
//                      string with the absolute partition path.
// 
// Returns:	      true if successful
// 			      false if failed
// 
// Calls:		  _tcscpy, _tcspbrk, _tcsncpy, _tcscat
// 
// Side Effects:  None
// 
// Notes;         1)  This function extends functionality of GetPartitionPath
//                    to replace all partition environment variables in a string.
//                    GetPartitionPath only replaces one partition environment
//                    variable.
//      		  2)  This function can only be used on C++ projects and sDest 
//                    argument will be equal to sSrc when the function fails and
//                    no exception is thrown.
//                3)  sDest will retain its original value when it is NULL when
//                    passed to the function, when sSrc is NULL, and when an
//                    exception is thrown.
//                4)  Caller of this function must make sure sDest has memory 
//                    allocation greater than or equal to sSrc so we are safe
//                    with _tcscpy, _tcsncpy, and _tcscat functions.
// 
////////////////////////////////////////////////////////////////////////////////////
inline bool ExpandPartitionStrings( LPCTSTR sSrc, LPTSTR sDest )
{
    bool bResult = false;
    if ( sSrc != 0 && sDest != 0 )
    {
		
        TCHAR sBuffer[ _MAX_PATH ] = _T("");
        _tcscpy( sBuffer, sSrc );
        LPTSTR sFindStart, sFirstCh, sSecondCh;
        sFindStart = sFirstCh = sSecondCh = sBuffer;
        try
        {
            bool bDone = false;
            while ( false == bDone )
            {
                sFirstCh = _tcspbrk(sFindStart, _T("%"));
                sFindStart = sFirstCh + 1;
                if ( sFirstCh != 0 )
                {
                    sSecondCh = _tcspbrk( sFindStart, _T("%"));
                    if ( sSecondCh != 0 && sSecondCh > sFirstCh )
                    {
                        TCHAR sVar[ _MAX_PATH ] = _T("");
                        _tcsncpy( sVar, sFirstCh, (sSecondCh - sFirstCh) + 1);
                        TCHAR sDestination[ _MAX_PATH ] = _T("");
                        if ( true == GetPartitionPath( sVar, sDestination ) )
                        {
                            TCHAR sTempBuffer[ _MAX_PATH ] = _T("");
                            _tcsncpy( sTempBuffer, sBuffer, sFirstCh - sBuffer );
                            _tcscat( sTempBuffer, sDestination );
                            _tcscat( sTempBuffer, sSecondCh + 1 );
                            _tcscpy( sBuffer, sTempBuffer );
                        }
                    }
                    else
                    {
                        // no more '%' pair
                        bDone = TRUE;
                    }
                }
                else
                {
                    // no '%' pair
                    bDone = TRUE;
                }
            }
            _tcscpy( sDest, sBuffer );
            bResult = true;
        }
        catch( ... )
        {
            bResult = false;
        }
    }
    return bResult;
}

#endif
