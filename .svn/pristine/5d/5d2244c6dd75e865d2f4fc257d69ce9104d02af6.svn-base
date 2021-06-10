/////////////////////////////////////////////////////////////////////////////
//
// GenericCO.cpp
//
// Implementation of IGenericCO methods.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//
// Machine generated IDispatch wrapper class(es) created with ClassWizard
// with some modifications by the author.

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// sMethodNames
//
// This array contains the names of the properties and methods that we use 
// from the CO ( in any SO ).  This is needed because the CO may not be the 
// one that was coded along with this SO so the DISPIDs may not be the same 
// as for that CO. 
// The connect function maps from method/property name to DISPID.  
// 
// sMethodNames is only required at the class level, but m_nDispatchID
// is required for each object created, since not all objects will
// necessarily connect to the same CO.  The OPOS spec. should probably 
// specify the standard IDs!

// This code is boilerplate and is not specific to CO, SO. 

#define IDX_CO_DESCRIPTION 0
#define IDX_CO_VERSION     1
#define IDX_SO_DIRECTIO    2
#define IDX_SO_DATA        3
#define IDX_SO_ERROR       4
#define IDX_SO_OUTPUT      5
#define IDX_SO_STATUS      6

static LPCTSTR sMethodNames[] =
   {
   _T("ControlObjectDescription"),   // 0
   _T("ControlObjectVersion"),       // 1
   _T("SODirectIO"),                 // 2
   _T("SOData"),                     // 3
   _T("SOError"),                    // 4
   _T("SOOutputComplete"),           // 5
   _T("SOStatusUpdate")              // 6
   };

BOOL IGenericCO::Connect(
   LPDISPATCH pDispatch,
   CmDataCapture& cmDc, 
   BOOL bAutoRelease /* = FALSE */)
   {
   CDataCapEntryExit DCE( cmDc, _T("IGenericCO::Connect") );
   AttachDispatch( pDispatch, bAutoRelease );
   m_IConnect.Connect( 
      m_lpDispatch, sMethodNames, ARRAY_ELEMENT_COUNT( sMethodNames ), cmDc );
   BOOL bResult = TRUE;
   if ( m_IConnect[IDX_CO_DESCRIPTION] == DISPID_UNKNOWN  // Control must provide
      || m_IConnect[IDX_CO_VERSION] == DISPID_UNKNOWN )   // What about DirectIO?
      {
      bResult = FALSE;
      cmDc.DCPrintf( TRACE_ALWAYS, 
         _T("Missing critical method - Disconnecting!") );
      Disconnect();
      }
   return bResult;
   }

void IGenericCO::Disconnect(
   void )
   {
   m_IConnect.Disconnect();
   ReleaseDispatch();
   return ;
   }




/////////////////////////////////////////////////////////////////////////////
// IGenericCO properties

CString IGenericCO::GetControlObjectDescription()
   {
	CString result;
   ASSERT( CString(sMethodNames[IDX_CO_DESCRIPTION]) == CString(_T("ControlObjectDescription")) );
   ASSERT( m_IConnect[IDX_CO_DESCRIPTION] != DISPID_UNKNOWN );// CO does not support
	GetProperty( 
      m_IConnect[IDX_CO_DESCRIPTION], 
      VT_BSTR, 
      (void*)&result);
	return result;
   }


long IGenericCO::GetControlObjectVersion()
   {
	long result;
   ASSERT( CString(sMethodNames[IDX_CO_VERSION]) == CString(_T("ControlObjectVersion")) );
   ASSERT( m_IConnect[IDX_CO_VERSION] != DISPID_UNKNOWN );
	GetProperty( 
      m_IConnect[IDX_CO_VERSION],  
      VT_I4, 
      (void*)&result);
	return result;
   }

// IGenericCO::SODirectIO
//
// Purpose: Dispatch DirectIO event to the caller
// Notes:   We allow the receiver od the DirectIO event to change the long and string
//          data, and return those results to the caller.  The COposDirectIO
//          event does not do anything with those changed.  If this functionality is 
//          necessary, then a class must be derived from COposDirectIO to 
//          do something with the results returned, since the direct IO is queued!

void IGenericCO::SODirectIO(
   long     lEventNumber, 
   long*    plData, 
   CString& sData)
   {
	static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PBSTR;
   ASSERT( CString(sMethodNames[IDX_SO_DIRECTIO]) == CString(_T("SODirectIO")) );
   ASSERT( m_IConnect[IDX_SO_DIRECTIO] != DISPID_UNKNOWN );
   if ( m_IConnect[IDX_SO_DIRECTIO] != DISPID_UNKNOWN )
      {
      BSTR bstrData = sData.AllocSysString( );
                                 // Pass Data from caller as a BSTR
	   InvokeHelper( 
         m_IConnect[IDX_SO_DIRECTIO], 
         DISPATCH_METHOD, 
         VT_EMPTY, NULL, 
         parms,
		   lEventNumber, 
         plData, 
         &bstrData );
      sData = bstrData;          // Get Results Back to caller
      SysFreeString( bstrData );
      }
	return ;
   }


void IGenericCO::SOData(long lStatus)
   {
	static BYTE parms[] = VTS_I4;
   ASSERT( CString(sMethodNames[IDX_SO_DATA]) == CString(_T("SOData")) );
   ASSERT( m_IConnect[IDX_SO_DATA] != DISPID_UNKNOWN );
   if ( m_IConnect[IDX_SO_DATA] != DISPID_UNKNOWN )
      {
	   InvokeHelper( 
         m_IConnect[IDX_SO_DATA], 
         DISPATCH_METHOD, 
         VT_EMPTY, NULL, 
         parms, 
         lStatus);
      }
	return ;
   }


void IGenericCO::SOError(
   long lRC, long lRCEx, long lLocus, long* pResponse)
   {
   static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PI4;
   ASSERT( CString(sMethodNames[IDX_SO_ERROR]) == CString(_T("SOError")) );
   ASSERT( m_IConnect[IDX_SO_ERROR] != DISPID_UNKNOWN );
   if ( m_IConnect[IDX_SO_ERROR] != DISPID_UNKNOWN )
      {
      InvokeHelper(
         m_IConnect[IDX_SO_ERROR], 
         DISPATCH_METHOD, 
         VT_EMPTY, NULL, 
         parms,
         lRC, lRCEx, lLocus, pResponse );
      }
   return;
   }

void IGenericCO::SOOutputComplete(long lOutputID)
   {
   static BYTE parms[] = VTS_I4;
   ASSERT( CString(sMethodNames[IDX_SO_OUTPUT]) == CString(_T("SOOutputComplete")) );
   ASSERT( m_IConnect[IDX_SO_OUTPUT] != DISPID_UNKNOWN );
   if ( m_IConnect[IDX_SO_OUTPUT] != DISPID_UNKNOWN )
      {
      InvokeHelper(
         m_IConnect[IDX_SO_OUTPUT], 
         DISPATCH_METHOD, 
         VT_EMPTY, NULL, 
         parms,
         lOutputID);
      }
   return;
   }

void IGenericCO::SOStatusUpdate(long lStatus)
   {
   static BYTE parms[] = VTS_I4;
   ASSERT( CString(sMethodNames[IDX_SO_STATUS]) == CString(_T("SOStatusUpdate")) );
   ASSERT( m_IConnect[IDX_SO_STATUS] != DISPID_UNKNOWN );
   if ( m_IConnect[IDX_SO_STATUS] != DISPID_UNKNOWN )
      {
      InvokeHelper(
         m_IConnect[IDX_SO_STATUS], 
         DISPATCH_METHOD, 
         VT_EMPTY, NULL, 
         parms,
         lStatus );
      }
   return;
   }


