/////////////////////////////////////////////////////////////////////////////
//
// COposRequest.h
//
// Declaration of COposRequest and COposRequestQueue classes.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifndef OPOS_REQUEST_H
#define OPOS_REQUEST_H


class COposRequest
   {
   public:
      virtual ~COposRequest( ){}
      virtual BOOL Process( CmDataCapture& pDc ) = 0;  
   protected:     
      long GetOutputId( void ) const
         { ASSERT( m_nOutputId != 0 ); return m_nOutputId; } // Don't Ask!
      COposRequest(long nOutputId ) 
         : m_nOutputId( nOutputId  ) 
         { ASSERT( nOutputId != 0 );}
      COposRequest( ) : m_nOutputId( 0  ) { }

   private:
      long m_nOutputId;
   private:
      COposRequest( const COposRequest& );  // Disable Copies
      COposRequest& operator=( const COposRequest& );
      // Only COposService can ask for Id, to update its m_nOutputID!
      friend class COposService; 
   };


class COposRequestQueue
   {
   public:
      COposRequestQueue( CmDataCapture& cmDc );
      ~COposRequestQueue( void );
      void Add( COposRequest* pRequest );
      void ClearOutput( void );
      void EnableRequests( BOOL bEnable );
      void RetryRequests( void );
      BOOL IsBusy( void ) const;
      BOOL IsEnabled( void ) const { return m_pServiceThread != NULL; }
      void WaitForOutputComplete( void ) const { CSingleLock( &m_evEmpty, TRUE ); }
   private: // Data
      mutable CCriticalSection m_csAccess;      // Access
      CEvent            m_evProcess;            // Process
      CEvent            m_evExit;               // Exit
      mutable CEvent    m_evEmpty;              // Empty
      BOOL              m_bEnabled     : 1;
      BOOL              m_bProcessing  : 1;
      BOOL              m_bAborted     : 1;
      BOOL              m_bRetry       : 1;
      CList < COposRequest*, COposRequest* > m_qList;
      CWinThread*       m_pServiceThread;       // Thread
      CmDataCapture&    m_cmDc;
   private: // Functions
      static UINT ServiceThreadEntry( LPVOID );
      UINT ServiceThread( void );
      void BeginService( void );
      void EndService( void );
	};


#endif