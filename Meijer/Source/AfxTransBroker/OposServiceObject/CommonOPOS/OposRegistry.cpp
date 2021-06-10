/////////////////////////////////////////////////////////////////////////////
//
// OposRegistry.cpp
//
// Implementation of COposRegistry methods.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#include "stdafx.h"

// This class provides some common support routines for the registry.

static const CString g_sReference( _T("->") );

static LPCTSTR g_szUses                = _T("Uses");
static LPCTSTR g_szDescription         = _T("Description");
static LPCTSTR g_szVersion             = _T("Version");
static LPCTSTR g_szPort                = _T("Port");

// COposRegistry::WriteRegString
//
// Purpose: Write string to registry with comment. 

void COposRegistry::WriteRegString(
   LPCTSTR pszValue,
   LPCTSTR sResult,
   int nCommentId /* = 0 */ )
   {
   CString sValue( sResult );

   if ( nCommentId != 0 )
      {
      CString sComment;
      sComment.LoadString( nCommentId );
      if ( *sComment != 0 )
         sValue += _T(' ') + g_sReference + _T(' ') + sComment;
      }
   m_cmRegistry.WriteString( pszValue, sValue );
   return;
   }

// COposRegistry::WriteRegInt
//
// Purpose: Write int to registry as a string with comment. 


void COposRegistry::WriteRegInt(
   LPCTSTR  pszValue,
   int      nResult,
   int      nCommentId  /* = 0   */,
   LPCTSTR  sPrefix     /* = ""  */)     // Useful for "COM"x
   {
   ASSERT( sPrefix != NULL );
   CString sValue;

   if ( nResult == REG_DEFAULT_INT )
      sValue.Format(  _T("%s%s"), sPrefix, REG_DEFAULT_STRING);
   else
      sValue.Format( _T("%s%ld"), sPrefix, nResult );
   WriteRegString( pszValue, sValue, nCommentId );
   return;
   }

// COposRegistry::ReadString
//
// Purpose: Read a string from registry, stripping the comment. 
//          Then write it (or the default ) with comment.

BOOL COposRegistry::ReadString(
   LPCTSTR  pszValue,
   CString& sResult, 
   int      nCommentId  /* = 0                  */,
   LPCTSTR  szDefault   /* = REG_DEFAULT_STRING */ )
   {
   BOOL bResult = m_cmRegistry.ReadString( pszValue, sResult );
   if ( bResult != FALSE 
      && sResult.Find( g_sReference ) != -1 )
      {
      sResult = sResult.Left( sResult.Find( g_sReference ) );
      sResult.TrimRight();
      }
   // If string not found, or if string is REG_DEFAULT_STRING, then set it to the
   // caller's default ( which may be REG_DEFAULT_STRING ). 
   if ( bResult == FALSE 
      || sResult == REG_DEFAULT_STRING )
      sResult = szDefault;
   // Write out the registry string if it is non-default or user has provided a 
   // comment
   if ( sResult.IsEmpty() == FALSE 
      && ( sResult != REG_DEFAULT_STRING
      || nCommentId != 0 ) )
      WriteRegString( pszValue, sResult, nCommentId );
   // If string is REG_DEFAULT_STRING, then return FALSE, since no user string 
   // was specified by the user in the registry.  
   if ( sResult == REG_DEFAULT_STRING )
      bResult = FALSE;
   return bResult;
   }

// COposRegistry::ReadInt
//
// Purpose: Read an int from registry. 
//          Then write it (or the default ) with comment.

int COposRegistry::ReadInt( 
   LPCTSTR  pszValue, 
   int      nCommentId  /* = 0               */, 
   int      nDefault    /* = REG_DEFAULT_INT  */)
   {
   CString sValue;
   int nResult = nDefault;

   if ( m_cmRegistry.ReadString( pszValue, sValue ) != FALSE 
      && sValue.IsEmpty() == FALSE 
      && _istdigit( sValue.GetAt( 0 ) ) != FALSE )
      nResult = _ttoi( sValue );
   WriteRegInt( pszValue, nResult , nCommentId );
   return nResult;
   }


// COposRegistry::ReadBool
//
// Purpose: Read a bool from registry, write it with comment. 

BOOL COposRegistry::ReadBool(
   LPCTSTR  pszValue,
   int      nCommentId  /* = 0     */,
   BOOL     bDefault    /* = FALSE */) 
   {
   CString sValue;
   BOOL bResult = bDefault;

   if ( m_cmRegistry.ReadString( pszValue, sValue ) != FALSE 
      && sValue.IsEmpty() == FALSE )
      {
      sValue.MakeUpper();
      if ( sValue.GetAt(0)  == _T('T')  ) 
         bResult = TRUE;
      else if ( sValue.GetAt(0)  == _T('F') ) 
         bResult = FALSE;
      }
   WriteRegString( pszValue, bResult ? _T("TRUE") : _T("FALSE"), nCommentId );
   return bResult;
   }


// COposRegistry::Redirect
//
// Purpose: Process the "uses" value in the registry. 

BOOL COposRegistry::Redirect(
   const CString& sClass      /* = _T("") */,
   int            nCommentId  /* = 0      */ )
   {
   BOOL bResult = FALSE;
   //***********************************************************************
   // Extract registry key redirection.
   // -- Uses --  Dfault = not used.  If present, must be a valid key.
   //   If USES value, then go to that key for further information:
   CString sValue;
   if ( m_cmRegistry.ReadString( g_szUses, sValue ) != FALSE 
      && sValue.IsEmpty() == FALSE)
      {
      WriteRegString( g_szUses, sValue, nCommentId );
      m_cmRegistry.CloseKey();

      // If prefixed with '\', then remove it.
      if ( sValue.GetAt(0) == '\\' )
         sValue = sValue.Mid( 1 );
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, _T("*%s <%s>"), 
         g_szUses, (LPCTSTR) sValue );
      int nIndex = sValue.Find( _T('\\') );
      if ( nIndex != -1 )
         {
         m_sClass = sValue.Left( nIndex );   // Change class and device
         m_sName = sValue.Mid( nIndex + 1 );
         }
      else
         m_sClass = sValue;                  // Just change the device class
      // Build registry key and open.
      // Note that we are not supporting a redirection from the uses
      // key.  If we called COposRegistry::OpenKey, then 
      // "Class\Device" might map to yet another key, if Device referenced
      // a string value.
      bResult = m_cmRegistry.OpenKey( HKEY_LOCAL_MACHINE, GetKeyName() );
      if ( bResult == FALSE )
         m_cmDc.DCPrintf( TRACE_ALWAYS, _T("*%s is Invalid, Registry was Closed"), g_szUses );
      }
   else if ( sClass.IsEmpty() == FALSE )    // "Uses" is Required?
      {     // Not found, write suggestion
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, _T("*Missing \"Uses\"") );
      sValue = sClass + _T("\\[keyname]");
      WriteRegString( g_szUses, sValue, nCommentId );
      }
   return bResult;
   }


// COposRegistry::WriteSoDescription
//
// Purpose: Write SO description and version to the registry. 

CString COposRegistry::WriteSoDescription( 
   LPCTSTR sDescription, 
   int     nVersion )
   {
   ASSERT( sDescription != NULL );
   ASSERT( nVersion != 0 );
   CString sVersion;
   sVersion.Format( _T("%d.%d.%d"),
      nVersion / 1000000,           // Millions value.
      ( nVersion / 1000 ) % 1000,   // Thousands value.
      nVersion % 1000 );            // Units value.
   WriteRegString( g_szDescription, sDescription );
   WriteRegString( g_szVersion, sVersion);
   return sVersion;
   }


///////////////////////////////////////////////////////////////

// COposRegistry::ReadPort
//
// Purpose: Read serial port from the registry
// Notes:   No default for port! Writes REG_DEFAULT_INT when invalid!

int COposRegistry::ReadPort( 
   int      nCommentId  /* = 0               */,
   LPCTSTR  sPrefix     /* = _T("COM")       */)
   {
   ASSERT( sPrefix != NULL );
   CString sValue;
   int nResult = 0;
   CDataCapEntryExit DCE( m_cmDc, _T("COposRegistry::ReadPort") );
   if ( m_cmRegistry.ReadString( g_szPort, sValue ) != FALSE 
      && sValue.IsEmpty() == FALSE)
      {
      // Remove leading prefix if present
      if ( (sValue.Left(3)).CompareNoCase( sPrefix ) == 0 )
         sValue = sValue.Mid( _tcslen( sPrefix ) );   
      sValue.TrimLeft();
      if ( sValue.IsEmpty() == FALSE 
         && _istdigit( sValue.GetAt( 0 )  ))
         nResult = _ttoi( sValue );
      }
   else
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4,_T("Missing %s"), g_szPort );
   if ( nResult <= 0 || nResult > MAX_SERIAL_PORTS )
      nResult = REG_DEFAULT_INT;
   WriteRegInt( g_szPort, nResult, nCommentId, sPrefix );
   return nResult & ~REG_DEFAULT_INT;
   }


#define S_APARTMENT _T("Apartment") 


HRESULT COposRegistry::RegisterThreadingModelSO( 
   const IID& iidData ) 
   {
   HRESULT hResult = E_UNEXPECTED;
   CmRegistry cmRegistry;
   if ( cmRegistry.OpenKey( HKEY_LOCAL_MACHINE, RegKeyFromGuid( iidData ) ) != FALSE 
      && cmRegistry.WriteString( _T("ThreadingModel"), S_APARTMENT ) != FALSE )
      hResult = S_OK;
   return hResult;
   }


#ifdef _DEBUG

void COposRegistry::AssertThreadingModelSO( 
   const IID& iidData ) 
   {
   CmRegistry cmRegistry;
   ASSERT( cmRegistry.OpenKey( HKEY_LOCAL_MACHINE, RegKeyFromGuid( iidData ) ) != FALSE);
   CString sData;
   ASSERT( cmRegistry.ReadString( _T("ThreadingModel"), sData ) != FALSE );
   ASSERT( sData == S_APARTMENT );
   return ;
   }

#endif               


int COposRegistry::ReadIntRange(
   LPCTSTR        szValue,
   int            nDefault /* = REG_DEFAULT_INT */,
   int            nMin     /* = INT_MIN         */,
   int            nMax     /* = INT_MAX         */,
   int            nComment /* = 0               */)
   {
   int lResult = ReadInt( szValue, nComment, nDefault );
   if ( lResult != REG_DEFAULT_INT )
      {
      int lSavedResult = lResult;
      if ( lResult < nMin )
         lResult = nMin;
      else if ( lResult > nMax )
         lResult = nMax;
      if ( lSavedResult != lResult )
         WriteRegInt( szValue, lResult, nComment );
      }
   return lResult;
   }


DWORD COposRegistry::ReadDwordRange(
   LPCTSTR        szValue,
   DWORD          nDefault /* = REG_DEFAULT_INT */,
   DWORD          nMin     /* = 0               */,
   DWORD          nMax     /* = ULONG_MAX       */,
   int            nComment /* = 0               */)
   {
   DWORD dwResult = (DWORD)ReadInt( szValue, nComment, nDefault );
   if ( dwResult != REG_DEFAULT_INT )
      {
      DWORD lSavedResult = dwResult;
      if ( dwResult < nMin )
         dwResult = nMin;
      else if ( dwResult > nMax )
         dwResult = nMax;
      if ( lSavedResult != dwResult )
         WriteRegInt( szValue, (int)dwResult, nComment );
      }
   return dwResult;
   }
