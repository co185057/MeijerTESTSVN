//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmEnterBirthdate.h
//
// TITLE: Class header for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMEnterBirthdateSTATE
#define _SMEnterBirthdateSTATE

#include "SMSmEnterBirthdateBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmEnterBirthdate : public SMSmEnterBirthdateBase
{
public:
  SMSmEnterBirthdate();					  // constructor
  DECLARE_DYNCREATE(SMSmEnterBirthdate)     // MFC Runtime class/object information
};

#endif
