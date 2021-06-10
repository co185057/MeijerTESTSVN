//Global function for parsing file path with environment variable

#include "stdafx.h"
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
bool GetPartitionPath( LPCTSTR sSrc, LPTSTR sDest )
{
    bool bResult = false;
    // make sure sSrc or sDest are not NULL before continuing with
    // our operation. In Debug build, uninitialized variables are not
    // equal to NULL, so using sSrc and sDest should be caught by
    // our try/catch operation.
    if ( sSrc != 0 && sDest != 0 )
    {
        try
        {
            TCHAR sSrcCpy[ _MAX_PATH ];
#if defined( _UNICODE ) || defined( UNICODE )
            wmemset( sSrcCpy, 0, sizeof( sSrcCpy ) / sizeof( *sSrcCpy ) );
#else
            memset( sSrcCpy, 0, sizeof( sSrcCpy ) / sizeof( *sSrcCpy ) );
#endif
            _tcsncpy( sSrcCpy, sSrc, _tcslen( sSrc ) );    
            LPTSTR sVarPartition = _tcstok( sSrcCpy, _T("\\") );
            if ( sVarPartition != 0 )
            {
                // remove '%' character in sVarPartition
                LPTSTR sSource = sVarPartition;
                LPTSTR sEnd = sSource + _tcslen( sVarPartition );
                LPTSTR sDestination = sVarPartition;
                while ( sSource != sEnd )
                {
                    if ( *sSource != _T('%') )
                    {
                        *sDestination = *sSource;
                        sDestination = _tcsinc( sDestination );
                    }
                    sSource = _tcsinc( sSource );
                }
                *sDestination = _T('\0');
                if ( sSource - sDestination == 2     // we remove two '%' characters
                    // there must be letters between the two '%' characters
                    && _tcslen( sVarPartition ) > 0 
                    // must be one of the five defined partition variables
                    && ( _tcsicmp( sVarPartition, _T("APP_DRIVE") ) == 0    
                    || _tcsicmp( sVarPartition, _T("DATA_DRIVE") ) == 0 
                    || _tcsicmp( sVarPartition, _T("TEMP_DRIVE") ) == 0 
                    || _tcsicmp( sVarPartition, _T("BOOT_DRIVE") ) == 0 
                    || _tcsicmp( sVarPartition, _T("OS_DRIVE") ) == 0 ) ) 
                {
                    // caller must make sure sDest has memory allocation
                    // greater than or equal to sSrc so we are safe
                    // with _tcscpy and _tcscat functions.
                    LPTSTR sVar = 0;
                    sVar = _tgetenv( sVarPartition );
                    if ( sVar == 0 )
                    {
                        sVar = DEFAULT_PARTITION_DRIVE;
                    }
                    if ( _tcslen( sVar ) <= _tcslen( sVarPartition ) + 2 )  // +2 for the two '%'
                    {
                        LPTSTR sRight = (LPTSTR)_tcschr( sSrc, _T('\\') );
                        // sRight should be NULL when sSrc is only "%APP_DRIVE%".
                        // We report true for bResult below if _tcscpy is successful
                        // even if sRight is NULL. We're taking value first for
                        // sRight before we do a copy to sDest, this way, our
                        // function can be used in the following manner:
                        //
                        // TCHAR sTemp[ _MAX_PATH ] = _T("%APP_DRIVE%");
                        // GetPartitionPath( sTemp, sTemp );
                        // 
                        // Note: Above example is only ok when sTemp is not pointing
                        //       to a const string, otherwise, statements below
                        //       will throw exceptions.
                        bResult = _tcscpy( sDest, sVar ) != 0;
                        if ( bResult == true && sRight != 0 )
                        {
                            bResult = _tcscat( sDest, sRight ) != 0;
                        }
                    }
                }
            }
            if ( bResult == false )
            {
                _tcscpy( sDest, sSrc );
            }
        }
        catch ( ... )
        {
            // We got an access violation here...
            // sSrc or sDest is uninitialized variable.
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
bool ExpandPartitionStrings( LPCTSTR sSrc, LPTSTR sDest )
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


