/////////////////////////////////////////////////////////////////////////////
//
// OposSharedData.cpp
//
// Implementation of COposSharedData methods.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//
#include "stdafx.h"

// At a minimum, this shared memory is used to uniquely identify each
// device/port that is shared between two applications.  This is done for 
// data capture..... Really!  This class encapsulates the allocation
// of shared memory and the algorithim to determine the unique idfor DCap. 

// COposSharedData::COposSharedData
// 
// Purpose: Constructor 

COposSharedData::COposSharedData(
   LPCTSTR        szName,
   CmDataCapture& cmDc )
   : CSharedMemory( CString( szName ) + _T("DCap"), cmDc ),
   m_pSharedData( NULL ),
   m_dwUniqueBit( 0 ),
   m_nUniqueId( SO_INVALID_DATACAPTURE_ID )
   {
   CDataCapEntryExit DCE( cmDc, _T("COposSharedData::COposSharedData") );
   return;
   }

// COposSharedData::~COposSharedData
// 
// Purpose: Destructor 

COposSharedData::~COposSharedData(
   void)
   {
   // device should call COposSharedData::Close first
   ASSERT( m_pSharedData == NULL );    
   return;
   }

// COposSharedData::Open
// 
// Purpose: Create shared memory for all devices, all ports so that 
//          the shared memory is an array of structures. If succesful,
//          choose our area based on our port and generate an Id for 
//          Data Capture.     

BOOL COposSharedData::Open( 
   int nPort )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposSharedData::Open") );
   nPort = ( nPort % MAX_SERIAL_PORTS ) + 1;
   BOOL bResult = TRUE;
   if ( m_pSharedData == NULL )
      {
      bResult = FALSE;
      LPVOID pMem = Create( MAX_SERIAL_PORTS, sizeof( SHARED_DATA ) );
      if ( pMem != NULL )
         {
         SHARED_DATA* pSharedData = ((SHARED_DATA*)pMem) + ( nPort - 1 );
         m_nUniqueId = ClaimUniqueId( pSharedData->dwDeviceMask );
         if ( m_nUniqueId != SO_INVALID_DATACAPTURE_ID )
            {
            bResult = TRUE;
            m_pSharedData = pSharedData;
            }
         else
            {
            ASSERT( m_dwUniqueBit == 0 );    // Should be released by 
            CSharedMemory::Free();
            }
         }
      }
   return bResult;
   }


// COposSharedData::Close
//
// Puropse: Release the shared memory and Id. 


void COposSharedData::Close( 
   void )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposSharedData::Close") );
   if ( m_pSharedData != NULL )
      {
      ASSERT( m_nUniqueId != SO_INVALID_DATACAPTURE_ID );
      ReleaseUniqueId( m_pSharedData->dwDeviceMask );
      Free();
      }
   m_pSharedData = NULL;
   return;
   }


// COposSharedData::ClaimUniqueId
//
// Purpose: Use a DWORD in shared memory, specified by the caller in 
//          dwExistingBits to generate a unique Id for each SO loaded 
//          of class and port.  Called when the Service
//          Object is loaded.  The Id is valid from Open to Close, 
//          ( Construction to Destruction) not just
//          from the point of Claim to Release ( otherwise, all SOs would
//          end up with the same Id.   
//          The derived class should reference count if it supports
//          more than one service object class( DataCapture ). 
//          The Id is device specific, so all service objects using a 
//          device object should use the same Id.

int COposSharedData::ClaimUniqueId( 
   DWORD&         dwExistingBits )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("COposSharedData::ClaimUniqueId") );
   int nResult = SO_INVALID_DATACAPTURE_ID;
   int nIndex = 0;
   CSingleLock mtxAccess( this, TRUE );
   ASSERT( m_dwUniqueBit == 0 );       // Call One time only 
   m_dwUniqueBit = 1;
   while ( m_dwUniqueBit != 0          // while not all bits tried
      && (m_dwUniqueBit &  dwExistingBits) != 0)  // And not all bits tried   
      {
      m_dwUniqueBit <<= 1;
      nIndex++;
      }
   if ( m_dwUniqueBit != 0 )           // Found an unused bit?
      {
      dwExistingBits |= m_dwUniqueBit; // Flag in Shared Memory
      nResult = nIndex;
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, 
         _T("Created Unique Id %d"), nIndex );
      }
   else
      m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL4, 
         _T("Too Many Applications for %s"), (LPCTSTR)m_sName );
   return nResult;
   }

// COposSharedData::ReleaseUniqueId
//
// Purpose: Release the Id. 

void COposSharedData::ReleaseUniqueId( 
   DWORD& dwExistingBits )
   {
   CSingleLock mtxAccess( this, TRUE );
   CDataCapEntryExit DCE( m_cmDc, _T("COposSharedData::ReleaseUniqueId") );
   if ( m_dwUniqueBit != 0 )
      {
      dwExistingBits &= ~m_dwUniqueBit;
      m_dwUniqueBit = 0;
      }
   return;
   }
