//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCardProcessing.h
//
// TITLE: Class header for Confirn Signature
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CardProcessingSTATE
#define _CardProcessingSTATE

#include "SMCardProcessingBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCardProcessing : public SMCardProcessingBase
{

public:
	SMCardProcessing();
  DECLARE_DYNCREATE(SMCardProcessing)// MFC Runtime class/object information
};

#endif