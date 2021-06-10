/////////////////////////////////////////////////////////////////////////////
//
// OposOutput.inl
//
//   Inlines of COposOutput Methods.
//
//   Copyright (c) 1997 NCR.  All rights reserved.
//
//   Date                   Modification                          Author
//---------|---------------------------------------------------|-------------
// 97/06/14 Initial version.                                      T. Burk
//
// COposOutput::COposOutput
//
// Purpose: Constructor

#ifdef OPOS_INLINE

OPOS_INLINE COposOutput::COposOutput(
   void )
   : m_nLastOutput( NO_OUTPUT_ID ), 
   m_nLastAssigned( NO_OUTPUT_ID ),
   m_nCurrent( NO_OUTPUT_ID ),
   m_bResult( FALSE ),
   m_evComplete( FALSE, FALSE ),
   m_nError( NO_OUTPUT_ID ),
   m_nRC( OPOS_SUCCESS ),
   m_nRCEx( OPOS_SUCCESS )
   {
   }

// COposOutput::IsRetry
//
// Purpose: Returns TRUE if the Output request was previously attempted.

OPOS_INLINE BOOL COposOutput::IsRetry( 
   void ) const
   {
   ASSERT( m_nCurrent != NO_OUTPUT_ID );
   return m_nCurrent == m_nError;
   }


// COposOutput::GetNextId
//
// Purpose: Updates the OutputId.

OPOS_INLINE DWORD COposOutput::GetNextId( 
   void )
   {
   return ++m_nLastAssigned;
   }

// COposOutput::Claim
//
// Purpose: Resets Variables.

OPOS_INLINE void COposOutput::Reset( 
   void )
   {
   ASSERT( m_nCurrent == NO_OUTPUT_ID );  // No output!
   m_nError = m_nLastOutput = m_nLastAssigned = m_nCurrent  = NO_OUTPUT_ID;
   m_bResult = FALSE;
   m_evComplete.ResetEvent();
   return;
   }

// COposOutput::Complete
//
// Purpose: Complete a request with NO error.

OPOS_INLINE void COposOutput::Complete( 
   long nRC    /* = OPOS_SUCCESS */, 
   long nRCEx  /* = 0 */)
   {
   ASSERT( m_nCurrent != NO_OUTPUT_ID );
   m_nRC    = nRC;
   m_nRCEx  = nRCEx;
   m_nError = m_nCurrent = NO_OUTPUT_ID;
   m_bResult = TRUE;             // No Error!
   if ( nRC != OPOS_SUCCESS ) 
      {
      m_bResult = FALSE;
      if ( m_nCurrent != IMMEDIATE_REQUEST_ID )
         m_nError = m_nCurrent;        
      }
   else if ( m_nCurrent != IMMEDIATE_REQUEST_ID )
      m_nLastOutput = m_nCurrent;
   m_evComplete.SetEvent();
   return ;
   }


// COposOutput::Clear
//
// Purpose: Clears Output

OPOS_INLINE void COposOutput::Clear( 
   void )
   {
   m_nLastOutput = m_nLastAssigned;
   m_nError = NO_OUTPUT_ID;
   m_bResult = FALSE;
   m_nRC    = m_nRCEx  = 0;
   m_evComplete.SetEvent();
   return;
   }

// COposOutput::GetCurrentId
//
// Purpose: Return ID of the request being processed. Or zero if none. 

OPOS_INLINE long COposOutput::GetCurrentId( 
   void ) const
   {
   return m_nCurrent;
   }


OPOS_INLINE void COposOutput::BeginImmediate( 
   void )
   {
   ASSERT( m_nCurrent == NO_OUTPUT_ID);        
   // No Current Request
   m_bResult = FALSE;
   m_evComplete.ResetEvent();
   m_nCurrent = IMMEDIATE_REQUEST_ID;
   return;
   }


OPOS_INLINE BOOL COposOutput::GetResult(  
   long* pnRC, 
   long* pnRCEx )
   { 
   ASSERT( pnRC != NULL );
   ASSERT( pnRCEx != NULL );
   m_nCurrent = NO_OUTPUT_ID;
   *pnRC = m_nRC;
   *pnRCEx = m_nRCEx;
   return m_bResult;
   }

#endif