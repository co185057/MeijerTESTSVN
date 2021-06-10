/////////////////////////////////////////////////////////////////////////////
//
// OposSharedData.h
//
// Declaration of COposSharedData class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//
        
#ifndef OPOSSHAREDDATA_H
#define OPOSSHAREDDATA_H

class COposSharedData : private CSharedMemory
   {
   public:
      COposSharedData( LPCTSTR szName, CmDataCapture& cmDc );
      ~COposSharedData();
      BOOL Open( int nPort );
      void Close( void );
      int GetUniqueId( void ) const { return m_nUniqueId; }
      int ClaimUniqueId( DWORD& dwExistingBits );
      void ReleaseUniqueId( DWORD& dwExistingBits );
   private:
      typedef struct 
         {
         DWORD dwDeviceMask;  // Bit-mask of active SOs.
         } SHARED_DATA;       // This device's shared data
      SHARED_DATA*   m_pSharedData;  
      int            m_nUniqueId;
      DWORD          m_dwUniqueBit; 
   };

#endif