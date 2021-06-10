/////////////////////////////////////////////////////////////////////////////
//
// SharedMemory.cpp
//
// Implementation of CSharedMemory methods.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#include "stdafx.h"

// This file implements (simple) shared memory for OPOS service objects.
// SOs should use this class to implement their shared memory needs.

CSharedMemory::CSharedMemory(
   LPCTSTR sDevName,
   CmDataCapture& cmDc )
   : CMutex( FALSE, CString( sDevName ) + _T("Mutex"), GET_GLOBAL_SA() ),
   m_sName( sDevName ), 
   m_hFile( NULL ), 
   m_nStatus( 0 ), 
   m_pMem( NULL ),
   m_bInitialCreation( FALSE ),
   m_cmDc( cmDc )
   { 
   }

// CSharedMemory::Create
//
// Purpose: Creates shared memory and returns a pointer to it. 
// Notes:   Devices that support more than one service object
//          must only call for the first open.  The derived class (device
//          specific) should reference count.

LPVOID CSharedMemory::Create( 
   DWORD nCount, 
   DWORD nSize,
   LPCTSTR sShareExt /* = _T("SharedData") */ )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("CSharedMemory::Create") );
   ASSERT( m_hFile == NULL );
   ASSERT( m_pMem == NULL );
   ASSERT( nSize * nCount > nSize  );  // Too Large
   ASSERT( nSize * nCount > nCount );  // Too Large
   CSingleLock mtxAccess( this, TRUE );
   m_hFile = CreateFileMapping(
      (HANDLE)0xFFFFFFFF,     // Handle to file to map (-1 = no file)
      NULL,                   // Optional security attributes 
      PAGE_READWRITE,         // Protection for mapping object 
      0,                      // High-order 32 bits of object size  
      nCount * nSize,         // Low-order 32 bits of object size (0 = entire file)
      m_sName + sShareExt );  //  Name of file-mapping object 
   m_nStatus = ::GetLastError();
   if ( m_hFile != NULL )
      {           // If file mapping successful, then map file view to memory.
      m_pMem = MapViewOfFile(
         m_hFile,     // File-mapping object to map into address space  
         FILE_MAP_ALL_ACCESS,// Access mode 
         0,                  // High-order 32 bits of file offset 
         0,                  // Low-order 32 bits of file offset 
         0);                 // Number of bytes to map (0 = entire file)
      if ( m_pMem == NULL )
         {
         m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, 
            _T("Failed to map view of shared file!") );
         DWORD nStatus = GetLastError();
         Free();
         m_nStatus = nStatus;
         }
      else if ( m_nStatus != ERROR_ALREADY_EXISTS )
         {
         m_bInitialCreation = TRUE;
         memset( m_pMem, 0, nCount * nSize );
         }
      else
         m_bInitialCreation = FALSE;      // Not first time created!
      }
   else
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, _T("Failed to create shared file mapping!") );
   return m_pMem;
   }

// CSharedMemory::Free
//
// Purpose: Release allocated shared memory

void CSharedMemory::Free( 
   void )
   {
   CSingleLock mtxAccess( this, TRUE );
   if ( m_pMem != NULL )            // the derived class
      {
      UnmapViewOfFile( m_pMem);
      m_pMem = NULL;
      }
   if ( m_hFile != NULL )
      {
      CloseHandle( m_hFile );
      m_hFile = NULL;
      }
   m_nStatus = 0;
   m_bInitialCreation = FALSE;
   return;
   }



