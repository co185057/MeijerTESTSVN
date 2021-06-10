/////////////////////////////////////////////////////////////////////////////
//
// std_opos.h
//
// Declaration of Standard Opos Stuff.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifndef STD_OPOS_H
#define STD_OPOS_H

#include <Opos.hi>
#include <afxmt.h>
#include <CTally.h>
#include <CmDataCapture.h>
#include <CmRegistry.h>
#include "CmFunctions.h"                // Miscellaneous.

#define ARRAY_ELEMENT_COUNT( x )  ( sizeof( x ) / sizeof( x[0] ) )
#define MAX_SERIAL_PORTS   32
#define ASSERT_BOOLEAN(bTest) ASSERT( bTest == TRUE || bTest == FALSE ); 
#define DECLARE_COPY_INHIBIT(class_name)		\
private:						\
	class_name (const class_name &);		\
	class_name & operator = (const class_name &);


const DWORD TRACE_UNIQUE_POLLS         =  0x00000001;
const DWORD TRACE_COM_PROPGET_ENTER    =  0x00010000;
const DWORD TRACE_COM_PROPGET_EXIT     =  0x00020000;
const DWORD TRACE_COM_PROPPUT_ENTER    =  0x00040000;
const DWORD TRACE_COM_PROPPUT_EXIT     =  0x00080000;
const DWORD TRACE_COM_METHOD_ENTER     =  0x00100000;
const DWORD TRACE_COM_METHOD_EXIT      =  0x00200000;
const DWORD TRACE_COM_EVENT_ENTER      =  0x00400000;
const DWORD TRACE_COM_EVENT_EXIT       =  0x00800000;
const DWORD TRACE_EXT_FUNC_ENTER       =  0x01000000;
const DWORD TRACE_EXT_FUNC_EXIT        =  0x02000000;
const DWORD TRACE_INT_FUNC_ENTER       =  0x04000000;
const DWORD TRACE_INT_FUNC_EXIT        =  0x08000000;
const DWORD TRACE_OUTPUT_LEVEL1        =  0x10000000;
const DWORD TRACE_OUTPUT_LEVEL2        =  0x20000000;
const DWORD TRACE_OUTPUT_LEVEL3        =  0x40000000;
const DWORD TRACE_OUTPUT_LEVEL4        =  0x80000000;
const DWORD TRACE_ALWAYS               =  0xFFFFFFFF;

class CDataCapEntryExit
   {
   public:
      CDataCapEntryExit( 
         CmDataCapture& cmDc, 
         LPCTSTR sName, 
         DWORD dwMask = TRACE_INT_FUNC_ENTER );
      CDataCapEntryExit( CmDataCapture* pDc, 
         LPCTSTR sName, 
         DWORD dwMask = TRACE_INT_FUNC_ENTER );
      void NoTraceExit( void );
      ~CDataCapEntryExit( ) ;
   private:
      DWORD          m_dwMask;
      CmDataCapture* m_pDc;
      const CString  m_sName;
      void TraceEntry( )
         {
         if ( m_pDc != NULL )
            m_pDc->DCPrintf( m_dwMask, _T("+%s"), m_sName ); 
         }
   };


class CThreadSync : public CSyncObject
   {
   // Constructor
   public:
   CThreadSync( CWinThread* pThread )
      : CSyncObject( _T("ThreadUnused") )
      {
      ASSERT( pThread != NULL );
      ASSERT( pThread->m_hThread != INVALID_HANDLE_VALUE );
      ASSERT( pThread->m_hThread != NULL );
	   m_hObject = pThread->m_hThread;
	   if (m_hObject == NULL)
		   AfxThrowResourceException();
      return;
      }
// Implementation
   public:
	   virtual ~CThreadSync() { m_hObject = NULL; }
	   virtual BOOL Unlock() { return TRUE; }
   private:
      DECLARE_COPY_INHIBIT( CThreadSync);    // Disable Copies
   };


extern void CheckThreadObject( CWinThread* pThread );
extern CString StrTok( CString& sData, LPCTSTR  sDelimiter );
extern BOOL ValueInList( long lValue, CString  sList );
extern void BuildNumberListString( CString& sResult, long nUnit );

#include <TClassPtr.h>
#include <OposRegistry.h>
#include <IConnection.h>

#ifndef OPOS_CONTROL
#include <OposEvent.h>
#include <OposRequest.h>
#include <OposOutput.h>
#include <GenericCO.h>
#include <SharedMemory.h>
#include <OposSharedData.h>
#include <OposService.h>
#include <SerialRegistry.h>
#include <TDeviceMap.h>
#include <TService.h>
#include <TCheckHealth.h>
#include <OposDeviceThread.h>
#include <OposDeviceWinThread.h>
#include <OposSerialInterface.h>
#include <WinDriverFacade.h>
#include <OposDiagnostics.h>
#include <OposDevice.h>
#else    // !OPOS_CONTROL
#include <TControl.h>
#endif   // !OPOS_CONTROL

#ifndef _DEBUG
#define OPOS_INLINE inline
#include <std_opos.inl>
#endif 

// I disable this warning because I use this extension
//    CSingleLock( &CThreadSync( this ), TRUE );
//                 ^ Compiler complains about this
// My intent here to wait forever for my thread to stop. The scope of the CThreadSync
// is one line.  I think it is clumsy to use 
// CThreadSync x( this );
// CSingleLock( &x, TRUE ); 
// Note further that the single lock is not named either, since its scope is one line
// I think this is elegant, the compiler thinks I may be an idiot. But the compiler is wrong
// because I am not an idiot. Are you?

#pragma warning(disable:4238)  // nonstandard extension used : class rvalue used as lvalue

#endif // STD_OPOS_H