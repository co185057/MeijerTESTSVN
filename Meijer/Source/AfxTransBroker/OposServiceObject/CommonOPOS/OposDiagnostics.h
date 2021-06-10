// OposDiagnostics.h: interface for the COposDiagnostics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPOSDIAGNOSTICS_H__DD8AEFF3_23F8_11D3_99BF_00A0C9EDD467__INCLUDED_)
#define AFX_OPOSDIAGNOSTICS_H__DD8AEFF3_23F8_11D3_99BF_00A0C9EDD467__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <NCREventLog.h>

//
// Define the severity codes
//
#define SEVERITY_SM_SUCCESS              0x0
#define SEVERITY_SM_INFORMATION          0x1
#define SEVERITY_SM_WARNING              0x2
#define SEVERITY_SM_ERROR                0x3

// The CONVERT_SEVERITY macro is used for this purpose.
//
#define CONVERT_SM_SEVERITY(X)           \
		(((X>>30)==SEVERITY_SM_SUCCESS)    ? evlTypeAuditSuccess:           \
		(((X>>30)==SEVERITY_SM_INFORMATION)? evlTypeInformation:            \
		(((X>>30)==SEVERITY_SM_WARNING)    ? evlTypeWarning:                \
		(((X>>30)==SEVERITY_SM_ERROR)      ? evlTypeError : evlTypeInformation))))

#define CONVERT_SM_CATEGORY(X)   (WORD)((X>>16) & 0xFFF)														\

class COposDiagnostics
   {
   public:
      COposDiagnostics( LPCTSTR sLogName, int idsSoDescription, LPCTSTR sProfile = NULL );
      void SetProfile( LPCTSTR sProfile ) { m_sProfile = sProfile; }
      CString GetProfile( void ) const { return m_sProfile; }
      void LogEvent( DWORD dwEventId, WORD wNumParams = 0, ... );
      void LogEvent( DWORD dwEventId, LPCTSTR sFormat, ... );
      void LogEventWithData( DWORD dwEventId, DWORD dwDataSize, const LPVOID pData, WORD wNumParams = 0, ... );
      void LogEventWithData( DWORD dwEventId, DWORD dwDataSize, const LPVOID pData, LPCTSTR sFormat, ... );
   private:
      CNCREventLog         m_EventLog;
      CString              m_sSoDescription;
      CString              m_sProfile;
      const CString        m_sLogName;
   private:
      void LogEventWorker( 
         DWORD                dwEventId, 
         const CStringArray&  sInsertionStrings,
         DWORD                dataSize = 0,
         const LPBYTE         dataBuffer = NULL );
      void FillStringArray(
         CStringArray&  sInsertionStrings,
         LPCTSTR        sFormat, 
         va_list        marker );
   };

#endif // !defined(AFX_OPOSDIAGNOSTICS_H__DD8AEFF3_23F8_11D3_99BF_00A0C9EDD467__INCLUDED_)
