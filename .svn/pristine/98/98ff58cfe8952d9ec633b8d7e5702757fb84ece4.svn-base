/////////////////////////////////////////////////////////////////////////////
//
// OposRegistryCO.cpp
//
// Implementation of COposRegistry CO methods.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#include "stdafx.h"

#ifndef _OPOS_ENABLE_INLINES
#define OPOS_INLINE
#include <OposRegistry.inl>
#endif 

// COposRegistry::OpenKeyCO 
// 
// Purpose: Open the OPOS key specified by Class and DeviceName, and 
//          logically map to another key if required.
//          Example:
//          If Class = "Scanner" and Name == "AnyScanner"
//          Then the opens:
//          HKEY_LOCAL_MACHINE\Software\OleForRetail\ServiceOpos\Scanner
//          and looks for a string value AnyScanner
//          If found, then the string value of AnyScanner is the key
//          otherwise, AnyScanner is the key.
//



BOOL COposRegistry::OpenKeyCO( 
   void  ) 
   { 
   BOOL bResult = m_cmRegistry.OpenKey( HKEY_LOCAL_MACHINE, GetBaseKeyName() ); 
   if ( bResult == FALSE )
      m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Registry key open failed (%d)"), 
         m_cmRegistry.GetLastError() );
   else if ( m_sName.IsEmpty() != FALSE )
      {
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, _T("* Default Profile Name is not supported") );
      bResult = FALSE;
      }
   else
      {
      CString sMapName;

      // We can't do ReadString( m_sName, m_sName ) since 
      // CmRegistry::ReadString clears the destination string (arg2) before it
      // reads the registry.  The key (arg1) is then gone. 
      if ( m_cmRegistry.ReadString( m_sName, sMapName ) != FALSE )
         {
         m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL2, 
            _T("*Mapping  \"%s\" to \"%s\""), (LPCTSTR)m_sName, (LPCTSTR)sMapName );
         }
      else
         sMapName = m_sName;
      m_sName = sMapName;  
      bResult = FALSE;
      if ( m_sName.IsEmpty() == FALSE )
         {
         bResult = m_cmRegistry.OpenKey( HKEY_LOCAL_MACHINE, GetKeyName() ); 
         if ( bResult == FALSE )
            m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, 
               _T("* \"%s\" not found"), (LPCTSTR)GetKeyName() );
         }
      else
         m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, _T("* Map to empty profile is not supported") );
      }
   return bResult;
   } 


// CCashAcceptorCO::GetSoName
// 
// Purpose: Determine SO Name (e.g. sResult = "NCRZT1000.CashAcceptor" )

CString COposRegistry::GetSoName(
   void ) 
   {
   CString sResult;

   if ( m_cmRegistry.ReadString( _T(""), sResult ) == FALSE 
      || sResult.IsEmpty() != FALSE )
      {
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, 
         _T("*No Default string for \"%s\""), (LPCTSTR) GetKeyName() );
      }
   else
      {
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, 
         _T("*Good read of SO value- %s"), (LPCTSTR)sResult );
      }
   return sResult;
   }

BOOL COposRegistry::GetFileVersionInfo(
   const IID&  iidData,
   CString&    sVersion,
   CmDataCapture& cmDc )
   {
   CDataCapEntryExit DCE( cmDc, _T("COposRegistry::WriteFileVersionInfo") );
   BOOL bResult = FALSE;
   CString sFileName;
   CmRegistry cmRegistry;
   if ( cmRegistry.OpenKey( HKEY_LOCAL_MACHINE, RegKeyFromGuid( iidData ) ) != FALSE )
      bResult = cmRegistry.ReadString( _T(""), sFileName );
   if ( bResult != FALSE )
      {
      DWORD dwMsIsStupid;
      bResult = FALSE;
      // Note on (LPTSTR)(LPCTSTR)
      // I have done this because there is no safe way to use GetFileVersionInfoSize and
      // GetFileVersionInfo unless the passed buffer is treated as a constant.
      // So why get worked up about it. 
      // From the information given to these functions ( an address ), they are
      // only guarenteed able to write up to the NULL anyway without crashing, so this 
      // is guaranteed as safe as passing a char*.
      int nVersionInfoSize = GetFileVersionInfoSize( (LPTSTR)(LPCTSTR)sFileName, &dwMsIsStupid );
      if ( nVersionInfoSize != 0 )
         {
         CString sVersionInfo;
         bResult = ::GetFileVersionInfo( (LPTSTR)(LPCTSTR)sFileName, NULL, 
            nVersionInfoSize, sVersionInfo.GetBuffer( nVersionInfoSize ) );
         sVersionInfo.ReleaseBuffer();
         if ( bResult != FALSE )    // Extract the file version information
            {     
            UINT 	nNumBytes;
            VS_FIXEDFILEINFO	*pFileInfo;

            bResult = VerQueryValue( (LPVOID)(LPCTSTR)sVersionInfo, 
               _T("\\"), (LPVOID *) &pFileInfo, &nNumBytes);
            if ( bResult != FALSE )
               {
               // The FILEVERSION is normally formatted X.Y.A.B
               // X.Y = the Major.Minor of the NCR OPOS release. The A value is
               // the NCR OPOS point release and the B value is the file patch
               // number.  
               // Format the sVersion string so that the user can print the 
               // NCRVersion to DataCapture and output the version to the registry.

               sVersion.Format( _T("%d.%d.%d.%d"), 
                  HIWORD(pFileInfo->dwFileVersionMS), 
                  LOWORD(pFileInfo->dwFileVersionMS),
                  HIWORD(pFileInfo->dwFileVersionLS),
                  LOWORD(pFileInfo->dwFileVersionLS));
               cmDc.DCPrintf( TRACE_ALWAYS,
                  _T("..NCR Release File Version (NCRVersion) is <%s>"), (LPCTSTR) sVersion );
               }
            else
               cmDc.DCPrintf(TRACE_ALWAYS, _T("*VerQueryValue() Failed.") );

            }
         else
            cmDc.DCPrintf(TRACE_ALWAYS, _T("*GetFileVersionInfo() Failed - %ld."), ::GetLastError() );
         }
      }
   else
  	   cmDc.DCPrintf(TRACE_ALWAYS, _T("*Could not read file name from registry - %ld."), ::GetLastError() );
   return bResult;
   }

