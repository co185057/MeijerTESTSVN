// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _WIN32_WINNT 0x0500	
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
#include <afxocc.h>
#include <afxctl.h>         // MFC support for ActiveX Controls

#include <comdef.h>         // _bstr_t stuff

#define TRACE_ALWAYS 0xFFFFFFFF
#import <msxml4.dll> raw_interfaces_only

using namespace MSXML2;

using namespace std;
// TODO: reference additional headers your program requires here
#include <string>
#ifdef _UNICODE
   typedef std::wstring StringType;
#else
   typedef std::string StringType;
#endif

#include "xtrace.h"
#include "xnew.h"
#include "baseobject.h"
