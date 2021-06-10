/////////////////////////////////////////////////////////////////////////////
//
// Std_opos.inl
//
//   Inlines for CDataCapEntryExit methods.
//
//   Copyright (c) 1997 NCR.  All rights reserved.
//
//   Date                   Modification                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifdef OPOS_INLINE 

OPOS_INLINE CDataCapEntryExit::CDataCapEntryExit( 
   CmDataCapture& cmDc, 
   LPCTSTR sName, 
   DWORD dwMask   /* = TRACE_INT_FUNC_ENTER  */)
   : m_pDc( &cmDc ), 
   m_sName( sName ), 
   m_dwMask( dwMask ) 
   {  
   TraceEntry( );
   }

OPOS_INLINE CDataCapEntryExit::CDataCapEntryExit( 
   CmDataCapture* pDc, 
   LPCTSTR sName, 
   DWORD dwMask   /* = TRACE_INT_FUNC_ENTER  */)
   : m_pDc( pDc ), 
   m_sName( sName ), 
   m_dwMask( dwMask ) 
   {  
   TraceEntry( );
   }
   
OPOS_INLINE void CDataCapEntryExit::NoTraceExit( 
   void )
   {
   if ( m_pDc != NULL )
      m_pDc->DCPrintf( m_dwMask >> 1, _T("*%s Exit NOT Traced"), 
         (LPCTSTR)m_sName ); 
   m_pDc = NULL;
   return;
   }

OPOS_INLINE CDataCapEntryExit::~CDataCapEntryExit( 
   void ) 
   { 
   if ( m_pDc != NULL )
      m_pDc->DCPrintf( m_dwMask << 1, _T("-%s"), (LPCTSTR) m_sName ); 
   return;
   }

#endif

