//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAcknowledgeSig.h
//
// TITLE: Class header for Request Sig
//
//
// AUTHOR:    Yafu Wang
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ACKNOWLEDGESIGSTATE
#define _ACKNOWLEDGESIGSTATE

#include "SMAcknowledgeSigBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMAcknowledgeSig : public SMAcknowledgeSigBase
{
public:
	SMAcknowledgeSig();
	
  DECLARE_DYNCREATE(SMAcknowledgeSig)// MFC Runtime class/object information
};

#endif