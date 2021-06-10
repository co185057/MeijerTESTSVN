/////////////////////////////////////////////////////////////////////////////
//
// SerialRegistry.h
//
// Declaration of CSerialRegistry class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

#ifndef SERIAL_REGISTRY_H
#define SERIAL_REGISTRY_H

class CSerialRegistry : public COposRegistry
   {
   public:
      CSerialRegistry( LPCTSTR sClass, LPCTSTR sName,
         CmDataCapture& cmDc ) 
         : COposRegistry( sClass, sName, cmDc ) { }
      CSerialRegistry( const COposRegistry& oRegistry)
         : COposRegistry( oRegistry ) {}
      int ReadBaud( int nDefault = REG_DEFAULT_INT, int nCommentId = 0 );
   };


#endif