/////////////////////////////////////////////////////////////////////////////
//
// SerialRegistry.cpp
//
// Implementation of CSerialRegistry class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#include "stdafx.h"

static LPCTSTR g_szBaud                = _T("Baud");

static BOOL IsValidBaud(
   int nBaud )
   {
   static int nValidSerialBauds[] = { 
      CBR_110, CBR_300, CBR_1200, CBR_2400, CBR_4800, CBR_9600,  
      CBR_14400, CBR_19200, CBR_38400, CBR_57600, CBR_115200
      };
   BOOL bResult = FALSE;
   for ( int nIndex = 0; nIndex < ARRAY_ELEMENT_COUNT( nValidSerialBauds );  
      nIndex++ )
      {
      if ( nBaud == nValidSerialBauds[ nIndex ] )
         {
         bResult = TRUE;
         break;
         }
      }
   return bResult;
   }



int CSerialRegistry::ReadBaud( 
   int      nDefault    /* = REG_DEFAULT_INT */,
   int      nCommentId  /* = 0               */) 
   {
   CDataCapEntryExit DCE( m_cmDc, _T("CSerialRegistry::ReadBaud") );
   CString  sValue;
   int      nResult;
   if ( m_cmRegistry.ReadString( g_szBaud, sValue ) == FALSE )
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL2, _T("Missing %s"), g_szBaud );
   nResult = _ttoi( sValue );
   // Did not read good baud from registry, did caller provide?
   if ( IsValidBaud( nResult ) == FALSE )
      {
      nResult = nDefault;
      if ( nDefault != REG_DEFAULT_INT )
         m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL2, _T("Assuming Default %s %d"), g_szBaud, nDefault );
      else 
         m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL2, _T("No Default %s Provided"), g_szBaud );
      ASSERT( nDefault == REG_DEFAULT_INT || IsValidBaud( nResult ) != FALSE );
      // pass no default or a valid baud!
      }
   WriteRegInt( g_szBaud, nResult , nCommentId );
   return nResult;
   }
