//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecUnexpectedDecrease.h
//
// TITLE: Class header for SMSecUnexpectedDecrease
//
//
// AUTHOR:    Thanhhieu Pham
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SECUNEXPECTEDDECREASESTATE
#define _SECUNEXPECTEDDECREASESTATE

#include "SMSecUnexpectedDecreaseBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSecUnexpectedDecrease : public SMSecUnexpectedDecreaseBase
{
public:
	SMSecUnexpectedDecrease();
	
  DECLARE_DYNCREATE(SMSecUnexpectedDecrease)// MFC Runtime class/object information
};

#endif