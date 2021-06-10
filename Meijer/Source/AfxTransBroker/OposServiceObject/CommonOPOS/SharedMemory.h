/////////////////////////////////////////////////////////////////////////////
//
// SharedMemory.h
//
// Declaration of CSharedMemory class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H      

#define SO_INVALID_DATACAPTURE_ID -1

class CSharedMemory : public CMutex
   {
   public:
      virtual ~CSharedMemory( ) { Free(); } 
      CSharedMemory(LPCTSTR sDevName, CmDataCapture& cmDc );
      LPVOID Create( DWORD nCount, DWORD nSize,
		  LPCTSTR sShareExt = _T("SharedData") );
      DWORD GetLastError( void ) const { return m_nStatus; }
      void Free( void );
      BOOL IsInitialCreation( void ) const { return m_bInitialCreation; }
   public:
      const CString  m_sName;
   protected:
      CmDataCapture& m_cmDc;
   private:
      DWORD          m_nStatus;
      HANDLE         m_hFile;       // Handle to the Memory
      LPVOID         m_pMem;        // Pointer to the Memory
      BOOL           m_bInitialCreation;
   private:
      DECLARE_COPY_INHIBIT( CSharedMemory );    // Disable Copies
   };


#endif
