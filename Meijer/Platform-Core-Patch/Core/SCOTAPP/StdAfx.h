// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
//    are changed infrequently
//

#if !defined(AFX_STDAFX_H__C902752C_676E_11D1_82FB_F81607C1B845__INCLUDED_)
#define AFX_STDAFX_H__C902752C_676E_11D1_82FB_F81607C1B845__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif  // _MSC_VER >= 1000

#define VC_EXTRALEAN                // Exclude rarely-used stuff from Windows headers

#pragma warning(disable:4786)

#include <afxwin.h>                 // MFC core and standard components
#include <afxext.h>                 // MFC extensions
#include <afxdisp.h>                // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                 // MFC support for Windows Common Controls
#endif  // _AFX_NO_AFXCMN_SUPPORT

#include <afxmt.h>

/******cm150000: Added In Merged Code******/

#include <afxtempl.h>								// MS MFC template header
#import <msxml4.dll>
using namespace MSXML2;


//cm150000: Merge 2 begin
#include <oposcash.h>
#include <oposlock.h>
#include <oposptr.h>
#include <NCRPtr.h>
#include <oposscan.h>
#include <ncrscan.h>
#include <scotmot.h>

//cm150000: Merge 2 end
#pragma once
#ifdef _REMOTECONNECTION
#define RCDECLSPEC __declspec(dllexport) 
#else
#define RCDECLSPEC __declspec(dllimport) 
#endif

#include "Descriptor.h"
#include "DescriptorList.h"

#include "common.h"
#include "smstate.h"
#include "smstoremodestate.h"
#include "smcustomermodestate.h"
#include "smhelpmodestate.h"
//tar 193806
#ifdef _DEBUG
#import "c:\scot\controls\WeightInfoD.dll" no_namespace, named_guids
#else
#import "c:\scot\controls\WeightInfo.dll" no_namespace, named_guids
#endif
const DWORD BEGIN_EVENTS		= WM_USER+150;
const DWORD WM_LOAD_CONFIG  	= BEGIN_EVENTS+1;
const DWORD WM_SET_CONTEXT   	= BEGIN_EVENTS+2;

/******cm150000: End Merged Code***********/


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif  // !defined(AFX_STDAFX_H__C902752C_676E_11D1_82FB_F81607C1B845__INCLUDED_)
