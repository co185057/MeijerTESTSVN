/////////////////////////////////////////////////////////////////////////////
//
// IConnection.cpp
//
// Implementation of IConnection class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#include "stdafx.h"
#include <afxpriv.h>    // For T2OLE macro

// IConnection::Connect
//
// Purpose: Dynamically determine dispatch Ids for an OLE server from a 
//          list of method names.

int IConnection::Connect(
   LPDISPATCH  pDispatch, 
   LPCTSTR     sMethodNames[], 
   int         nCount,
   CmDataCapture& cmDC)
   {
   ASSERT( nCount != 0 );
   ASSERT( m_nCount == 0 );      // Only One Connect
   ASSERT( m_pDispId == NULL);

   m_pDispId   = new DISPID[nCount];
   m_nCount    = nCount;
   int nMissing = 0;
	for ( int nIndex = 0; nIndex < nCount; nIndex++ )
      {     
   	USES_CONVERSION;
		OLECHAR* OLEMethodName = (OLECHAR*) T2COLE(sMethodNames[nIndex]);
      // - Look up the dispatch ID of the name in the control object,
      //     and place it into the CO dispatch ID array.
      //   -> If fails a lookup, then undo our init thus far and return error.
      if ( pDispatch->GetIDsOfNames(
         IID_NULL,
         &OLEMethodName,
         1,
         LOCALE_SYSTEM_DEFAULT,
         &m_pDispId[nIndex] ) != S_OK )
         {
         nMissing++;
         m_pDispId[nIndex] = DISPID_UNKNOWN;
         cmDC.DCPrintf1( TRACE_OUTPUT_LEVEL3, 
            _T("*Couldn't access method \"%s\""), sMethodNames[nIndex] );
         }
      }
   return m_nCount - nMissing;
   }


// IConnection::operator[]
//
// Purpose: Return Dispatch Id for method at nIndex.

DISPID IConnection::operator[](
   int nIndex) const
   {
   ASSERT( nIndex < m_nCount );  // Too Few Names in Connection
   ASSERT( m_pDispId != NULL );  // Not connected 
   return m_pDispId[ nIndex ];
   }
