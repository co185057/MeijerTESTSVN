/////////////////////////////////////////////////////////////////////////////
//
// IConnection.h
//
// Declaration of IConnection class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//
#ifndef ICONNECTION_H
#define ICONNECTION_H

class IConnection
   {
   public:
      IConnection( ) 
         : m_pDispId( NULL ),
         m_nCount( 0 )   { }
      int Connect( LPDISPATCH pDispatch, 
         LPCTSTR sEventNames[], int nCount, CmDataCapture& cmDC);
      void Disconnect( void )
         { delete m_pDispId; m_pDispId = NULL; m_nCount = 0; }
      ~IConnection( ) { ASSERT( m_pDispId == NULL ); } // Call Disconnect
      DISPID operator[](int nIndex) const;
   private:
      IConnection( const IConnection& ); // Disable
      IConnection& operator=( const IConnection& );
      DISPID* m_pDispId;
      int m_nCount;
   };

#endif