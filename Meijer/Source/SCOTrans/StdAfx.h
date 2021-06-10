// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8E5F769B_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
#define AFX_STDAFX_H__8E5F769B_A8AC_11D1_90AC_00609761BB2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC OLE automation classes
#include <afxtempl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxmt.h>

#include <afxole.h>
#pragma once
#ifdef _REMOTECONNECTION
#define RCDECLSPEC __declspec(dllexport) 
#else
#define RCDECLSPEC __declspec(dllimport) 
#endif

#include "Descriptor.h"
#include "DescriptorList.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8E5F769B_A8AC_11D1_90AC_00609761BB2E__INCLUDED_)
