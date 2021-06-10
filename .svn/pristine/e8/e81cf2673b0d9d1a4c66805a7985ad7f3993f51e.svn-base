/////////////////////////////////////////////////////////////////////////////
//
// OposOutput.h
//
//   Declaration of COposOutput class.
//
//   Copyright (c) 1997 NCR.  All rights reserved.
//
//   Date                   Modification                          Author
//---------|---------------------------------------------------|-------------
// 97/06/14 Initial version.                                      T. Burk
//

/*
 * Definition of member variables.
 *
 * long              m_nLastOutput;
 *
 *    This member keeps track of the last output Id that was complete 
 *    sucessfully or was cleared or aborted.
 *
 * long              m_nLastAssigned;
 *
 *    This member is the counter for output IDs. It is reset in the claim.
 *
 * long              m_nCurrent;
 * 
 *    This member keeps track of the current output request, and is zero if
 *    no request is being processed.
 *
 * long              m_nError;
 *
 *    This member tracks a request that had an error. It is zero if no
 *    error has occurred. 
 *
 * BOOL              m_bResult;
 *
 *    This member maintains the result of the current request.
 *
 * CEvent            m_evComplete;
 *
 *    This event is set when a request is completed, errored, cleared, or
 *    aborted. 
 *
 */

#ifndef OPOS_OUTPUT_H
#define OPOS_OUTPUT_H

#define IMMEDIATE_REQUEST_ID -1
#define NO_OUTPUT_ID          0

class COposOutput
   {
   public:
      COposOutput(); 
      ~COposOutput() {}
      DWORD GetNextId( void );
      void Reset( void );
      void Clear( void );
      BOOL BeginAsync( long nOutputId );
      void BeginImmediate( void );
      void Complete( long nRC = OPOS_SUCCESS, long nRCEx = 0 );
      BOOL GetResult( long* nRC, long* nRCEx );
      long GetCurrentId( void ) const;
      BOOL IsRetry( void ) const;
      CEvent* GetEvent( void ) { return &m_evComplete; }
   private:
      long              m_nLastOutput;
      long              m_nLastAssigned;
      long              m_nCurrent;
      long              m_nError;
      BOOL              m_bResult;
      CEvent            m_evComplete;
      long              m_nRC;
      long              m_nRCEx;
   };

#ifdef _OPOS_ENABLE_INLINES
#define OPOS_INLINE inline
#include <OposOutput.inl>
#endif

#endif
