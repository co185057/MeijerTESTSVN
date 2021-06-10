// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _WIN32_WINNT 0x0500
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//// Windows Header Files:
////#include <windows.h>
//
////#include <atlbase.h>
////#include <atlstr.h>
//
//#ifndef VC_EXTRALEAN
//#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
//#endif
//
//// Modify the following defines if you have to target a platform prior to the ones specified below.
//// Refer to MSDN for the latest info on corresponding values for different platforms.
//#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
//#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
//#endif
//
//#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
//#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 2000 or later.
//#endif						
//
//#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
//#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
//#endif
//
//#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
//#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
//#endif
//
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
//
//#include <afxctl.h>         // MFC support for ActiveX Controls
//#include <afxext.h>         // MFC extensions
//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Comon Controls
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT
//
//// Delete the two includes below if you do not wish to use the MFC
////  database classes
//#include <afxdb.h>			// MFC database classes
//#include <afxdao.h>			// MFC DAO database classes
//
//#include <afxmt.h>
//#include <afxwin.h>
//#include <afxocc.h>
#define VC_EXTRALEAN                // Exclude rarely-used stuff from Windows headers

#include <afxwin.h>                 // MFC core and standard components
#include <afxext.h>                 // MFC extensions
#include <afxdisp.h>                // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                 // MFC support for Windows Common Controls
#endif  // _AFX_NO_AFXCMN_SUPPORT

#include <afxmt.h>

/******cm150000: Added In Merged Code******/
#include <afxtempl.h>								// MS MFC template header


// TODO: reference additional headers your program requires here

#include <string>
#ifdef _UNICODE
   typedef std::wstring StringType;
#else
   typedef std::string StringType;
#endif

#include <stdio.h>
#include <tchar.h>

#include "utils.h"
#include "xtrace.h"
#include "baseobject.h"
#include "xnew.h"

#import <msxml4.dll> raw_interfaces_only

using namespace MSXML2;
using namespace ATL;
using namespace std;
