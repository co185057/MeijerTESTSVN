//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  AppRestartCount.h
//
// Description: This is an entirely static class, i.e. there are all
//              class variables and functions, no instance variables or
//              functions, so no object is required.  This class can be
//              used to save a value in the registry that is stored across
//              invocations of SCOTAPP. In this way we can keep track of
//              the number of times SCOTAPP made an attempt to restart
//              itself in case of an exception.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _APPRESTARTCOUNT
#define _APPRESTARTCOUNT

#include "DllDefine.h"
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
   AppRestartCount
{
	
public:
  static long reset();
  static long increment();
  static int  getCount(long& rc);
	
private:
  // constants used internally
  static const HKEY        APPRESTARTCOUNT_KEY;
  static LPCTSTR const APPRESTARTCOUNT_SUBKEY;
  static LPCTSTR const APPRESTARTCOUNT_VALUE;
	
};

#endif