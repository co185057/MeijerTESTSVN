#include "stdafx.h"
#include <atlconv.h>    // For macro

#ifndef _OPOS_ENABLE_INLINES
#define OPOS_INLINE
#include <std_opos.inl>
#endif 

extern void CheckThreadObject( 
   CWinThread* pThread )
   {
   ASSERT( pThread != NULL );
   if ( pThread == NULL )
      {
      static LPCTSTR sExText = _T("Failed to Create Thread");
      AfxMessageBox( sExText);
      AfxThrowOleDispatchException( 0x1235, sExText);
      }
   return ;
   }


// Common StrToken code, but named so as not to clash with it in other projects

extern CString StrTok( 
   CString& sData,
   LPCTSTR  sDelimiter )
   {
   CString sToken = sData.SpanExcluding( sDelimiter );
   sData = sData.Mid( sToken.GetLength() );
   if ( sData.IsEmpty() == FALSE )
      sData = sData.Mid( 1 );
   sToken.TrimLeft();
   sToken.TrimRight();
   return sToken;
   }


extern BOOL ValueInList( 
   long     lValue, 
   CString  sList )
   {
   BOOL bResult = FALSE;
   while ( sList.IsEmpty() == FALSE 
      && bResult == FALSE )
      bResult = _ttoi( StrTok( sList, _T(",") ) ) == lValue;
   return bResult;
   }


// BuildNumberListString
//
// Purpose: Add amount to string. 

extern void BuildNumberListString( 
	CString& sResult,
	long     nUnit )
	{
   if ( ValueInList( nUnit, sResult ) == FALSE )
      {
	   CString sTemp;
	   sTemp.Format(_T("%ld"), nUnit );
	   if ( sResult.IsEmpty() != FALSE )
		   sResult = sTemp;
      else if ( nUnit < _ttoi( sResult ) )
         sResult = sTemp + _T(',') + sResult;
      else 
         {
         CString sWorking = StrTok( sResult, _T(",") );
         while ( sResult.IsEmpty() == FALSE 
            && _ttoi( sResult ) < nUnit )
            sWorking += _T(',') + StrTok( sResult, _T(",") );
         sWorking = sWorking + _T(',') + sTemp;
         if ( sResult.IsEmpty() == FALSE )
            sWorking += _T(',') + sResult;
         sResult = sWorking;
         }
      }
	return;
	}


// CSecurityAttributes::GetGlobalSAEx
//
// Purpose: Return suitable attributes object for Windows NT

/* static */
TClassPtr<CSecurityAttributes> CSecurityAttributes::GetGlobalSAEx( 
   LPCSTR        sFile,
   int            nLine )
   {
   USES_CONVERSION;

   TClassPtr<CSecurityAttributes> pAttributes;
   // If WinNT or Win2K
	if ( GetVersion() < 0x80000000 )
      pAttributes = new CSecurityAttributes( A2T((LPSTR)sFile), nLine );
   return pAttributes;
   }

// CSecurityAttributes::CSecurityAttributes
//
// Purpose: Initialize attributes and security descriptor
 
CSecurityAttributes::CSecurityAttributes( 
   LPCTSTR        sFile,
   int            nLine )
   {
   // Initialize to safe values!!
   lpSecurityDescriptor = NULL;  
   nLength = sizeof(SECURITY_ATTRIBUTES);
   bInheritHandle = FALSE;
   if ( InitializeSecurityDescriptor( &m_Sd, SECURITY_DESCRIPTOR_REVISION ) == FALSE )
      {
      CString s;
 		s.Format( _T("!! InitializeSecurityDescriptor Failed !!\r\n")
			_T("File %s\r\nLine %d\r\nWin32 error #%d"),
			sFile, nLine, GetLastError() );
 		AfxMessageBox( s, MB_TASKMODAL | MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
      }
   else if ( SetSecurityDescriptorDacl( &m_Sd, TRUE, NULL, TRUE ) == FALSE )
      {
      CString s;
 		s.Format( _T("!! SetSecurityDescriptorDacl Failed !!\r\n")
			_T("File %s\r\nLine %d\r\nWin32 error #%d"),
			sFile, nLine, GetLastError() );
 		AfxMessageBox( s, MB_TASKMODAL | MB_OK | MB_ICONSTOP | MB_SETFOREGROUND );
      }
   else     // Everything worked as expected, set Security Descriptor 
 		lpSecurityDescriptor = &m_Sd;
   return;
   }
