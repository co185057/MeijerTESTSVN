// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__7B07B3D8_5DFC_11D2_9747_00A024B5D0B1__INCLUDED_)
#define AFX_STDAFX_H__7B07B3D8_5DFC_11D2_9747_00A024B5D0B1__INCLUDED_


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#pragma warning(disable:4786)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x501 //Sinan

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <objbase.h> //Sinan

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


#ifdef _REMOTECONNECTION
#define RCDECLSPEC __declspec(dllexport) 
#else
#define RCDECLSPEC __declspec(dllimport) 
#endif

#include <afxmt.h>
#include <afxtempl.h>
#include "Descriptor.h"
#include "DescriptorList.h"



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__7B07B3D8_5DFC_11D2_9747_00A024B5D0B1__INCLUDED_)
