/////////////////////////////////////////////////////////////////////////////
//
// OposOutput.cpp
//
//   Implementation of COposOutput Methods.
//
//   Copyright (c) 1997 NCR.  All rights reserved.
//
//   Date                   Modification                          Author
//---------|---------------------------------------------------|-------------
// 97/06/14 Initial version.                                      T. Burk
//

#include "stdafx.h"

#ifndef _OPOS_ENABLE_INLINES
#define OPOS_INLINE
#include <OposOutput.inl>
#endif 


// COposOutput::BeginAsync
//
// Purpose: Remember the start of output processing for request number 
//          nOutputId.

BOOL COposOutput::BeginAsync( 
   long nOutputId )
   {
   ASSERT( nOutputId > NO_OUTPUT_ID );
   ASSERT( nOutputId <= m_nLastAssigned && nOutputId >= m_nLastOutput );  
   // Valid Request Id, not before last processed and was assigned
   ASSERT( m_nCurrent == NO_OUTPUT_ID); // Previous request Complete?
   m_bResult = FALSE;
   if ( nOutputId > m_nLastOutput )
      {
      m_nCurrent = nOutputId;
   // If we are processing an Id that is greater the Id of the last 
   // request assigned when the error occurred, the the error state
   // is exited!
      m_bResult = TRUE;
      m_evComplete.ResetEvent();
      }
   return m_bResult;
   }
