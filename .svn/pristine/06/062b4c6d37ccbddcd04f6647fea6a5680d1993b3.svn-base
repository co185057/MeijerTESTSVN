// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC OLE automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#ifndef _DEBUG
#define _OPOS_ENABLE_INLINES
#endif 

#include "std_opos.h"
#include <afxmt.h>
#include <afxtempl.h>

// CSecurityAttributes 
// 
// Class that encapsulates SECURITY_ATTRIBUTES and has the following features:
// 1. Requires no local variables for the call - like SECURITY_DESCRIPTOR.
//    This allows it to be easily placed in the initializer list of a constructor!
// 2. Doesn't throw stupid exceptions - lets normal error handling take over!
//    And in all likelihood, things may still work!
// 3. Doesn't require a Delete in the users code (TClassPtr returned stays in scope
//    just long enough to make the call to CreateFile, CreateMutex etc.)

class CSecurityAttributes : public SECURITY_ATTRIBUTES
   {
   public:
      static TClassPtr<CSecurityAttributes> GetGlobalSAEx( LPCSTR sFile, int nLine );
   public:
      CSecurityAttributes( LPCTSTR sFile, int nLine );
   private:
      SECURITY_DESCRIPTOR m_Sd;
   };

// A macro to make life all the better!
// This macro returns a TClassPtr<CSecurityAttributes> and passes the file and line
// We want this in release mode too!

#define GET_GLOBAL_SA( ) CSecurityAttributes::GetGlobalSAEx( THIS_FILE, __LINE__ )
