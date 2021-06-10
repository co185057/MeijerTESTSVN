// OposDiagnostics.cpp: implementation of the COposDiagnostics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <malloc.h>
#include <OposDiagnostics.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COposDiagnostics::COposDiagnostics( 
   LPCTSTR  sLogName, 
   int      idsSoDescription,
   LPCTSTR  sProfile /* = NULL */ ) 
   : m_sLogName( sLogName ),
   m_sProfile( sProfile )
   { 
   // Set up device diagnostics SO description
   m_sSoDescription.LoadString( idsSoDescription );
   m_sSoDescription = m_sSoDescription.Left( m_sSoDescription.Find( _T('\n') ) );
   }


// COposDiagnostics::LogEvent
//
// Purpose: Log Event, with insertion strings specified by number (all are strings)

void COposDiagnostics::LogEvent( 
   DWORD dwEventId, 
   WORD  wNumParams /* = 0 */, 
   ... )
   {
   if ( dwEventId != 0 )
      {
      CStringArray sInsertionStrings;
   
      va_list marker;
      va_start( marker, wNumParams );
      for(int nIndex = 0; nIndex < wNumParams; nIndex++ )
         sInsertionStrings.Add( va_arg( marker, LPCTSTR ) );
      va_end(marker);
      LogEventWorker( dwEventId, sInsertionStrings ); 
      }
   return;
   }

// COposDiagnostics::LogEvent
//
// Purpose: Log Event, with insertion strings specified from a printf format specifier.

void COposDiagnostics::LogEvent( 
   DWORD    dwEventId, 
   LPCTSTR  sFormat, 
   ... )
   {
   if ( dwEventId != 0 )
      {
      CStringArray sInsertionStrings;
      va_list marker;
      va_start( marker, sFormat );
      FillStringArray( sInsertionStrings, sFormat,  marker );
      LogEventWorker( dwEventId, sInsertionStrings ); 
      }
   return;
   }

// COposDiagnostics::LogEventWithData
//
// Purpose: Log Event with data, with insertion strings specified by number (all are strings)

void COposDiagnostics::LogEventWithData( 
   DWORD          dwEventId, 
   DWORD          dwDataSize, 
   const LPVOID   pData, 
   WORD           wNumParams /* = 0 */, 
   ... )
   {
   if ( dwEventId != 0 )
      {
      CStringArray sInsertionStrings;
   
      va_list marker;
      va_start( marker, wNumParams );
      for(int nIndex = 0; nIndex < wNumParams; nIndex++ )
         sInsertionStrings.Add( va_arg( marker, LPCTSTR ) );
      va_end(marker);
      LogEventWorker( dwEventId, sInsertionStrings, dwDataSize, (LPBYTE)pData ); 
      }
   return;
   }

// COposDiagnostics::LogEventWithData
//
// Purpose: Log Event with data, with insertion strings specified from a printf format specifier.

void COposDiagnostics::LogEventWithData( 
   DWORD          dwEventId, 
   DWORD          dwDataSize, 
   const LPVOID   pData, 
   LPCTSTR        sFormat, 
   ... )
   {
   if ( dwEventId != 0 )
      {
      CStringArray sInsertionStrings;
      va_list marker;
      va_start( marker, sFormat );
      FillStringArray( sInsertionStrings, sFormat,  marker );
      LogEventWorker( dwEventId, sInsertionStrings, dwDataSize, (LPBYTE)pData ); 
      }
   return;
   }

// COposDiagnostics::m_EventLog
//
// Purpose: Log Event with data, with insertion strings specified from a printf format specifier.

void COposDiagnostics::FillStringArray(
   CStringArray&  sInsertionStrings,
   LPCTSTR        sFormat, 
   va_list        marker )
   {
   while( *sFormat != _T('\0') )
      {
      if ( *sFormat == _T('%') )
         {
         CString sData( sFormat );
         int nIndex = sData.Mid( 1 ).Find( _T('%') ) + 1;
         if ( nIndex != 0 )
            {
            sData = sData.Left( nIndex );
            sFormat += nIndex;               // Point to next %
            }
         else
            sFormat += sData.GetLength();    // Point to null terminator
         CString sString;
         sString.Format( sData, va_arg( marker, LPCTSTR ) );
         sInsertionStrings.Add( sString );
         }
      else
         sFormat++;
      }
   va_end(marker);
   return;
   }


// COposDiagnostics::LogEventWorker
//
// Purpose: Log Event with data, with insertion strings specified through a CStringArray.

void COposDiagnostics::LogEventWorker( 
   DWORD                dwEventId, 
   const CStringArray&  sInsertionStrings,
   DWORD                dwDataSize /* = 0    */,
   const LPBYTE         dataBuffer /* = NULL */ )
   {
   // Allocate array space for strings on the stack, and fill from the string array
   LPCTSTR* pStrings = (LPCTSTR*)_alloca( sizeof( LPCTSTR ) * ( sInsertionStrings.GetSize() + 2 ) );
   ASSERT( m_sSoDescription.IsEmpty() == FALSE );
   ASSERT( m_sProfile.IsEmpty() == FALSE );
   pStrings[0] = m_sSoDescription;
   pStrings[1] = m_sProfile;
   for ( int nIndex = 0; nIndex < sInsertionStrings.GetSize(); nIndex++ )
      pStrings[ nIndex + 2 ] = sInsertionStrings[ nIndex ];
   if ( m_EventLog.Open( m_sLogName ) == S_OK )
      {
      m_EventLog.LogEventArray(
         evlClassBasic,                   // SM Event Class, OPOS always uses Basic
         CONVERT_SM_SEVERITY(dwEventId),  // Severity Type
         CONVERT_SM_CATEGORY(dwEventId),  // WORD Catagory, extracted from the facility field of the EventId
         dwEventId,                       // ULONG EventId created from the Message File (*.MC)
         pStrings,                        // LPCTSTR Pointer to Buffer holding insertion strings
         sInsertionStrings.GetSize() + 2, // WORD Number of insertion strings to be put into event message
         dataBuffer,                      // BYTE Array for binary data to be included in event message
         dwDataSize);                     // ULONG Size of binary data array.
      m_EventLog.Close();
      }
   else
      {
      // If unable to use the StoreMinder Event Agent, try to log an event
      // directly with the NT Event Viewer.  If on 95 this will do nothing.
      HANDLE  hEventSource = RegisterEventSource( NULL, m_sLogName );
      if (hEventSource != NULL)
			{
         ReportEvent(
            hEventSource,                       // Handle returned by RegisterEventSource 
            CONVERT_SM_SEVERITY(dwEventId),     // Event type or severity
            CONVERT_SM_CATEGORY(dwEventId),     // Event Category, pulled from the event Facility field
            dwEventId,                          // Event ID
            NULL,                               // User  Security ID (optional) 
            sInsertionStrings.GetSize() + 2,    // Number of insertion Strings 
            dwDataSize,                         // Size of binary data, in bytes
            pStrings,                           // Array of insertion strings
            dataBuffer );                       // Array of binary data
         DeregisterEventSource(hEventSource);
         }
      }
   return;
   }
