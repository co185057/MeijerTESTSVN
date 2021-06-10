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
	SMStateBase *PSButtonHelp();
    virtual SMStateBase  *Initialize(void);
	SMStateBase  *PSButtonGoBack(void);
	SMStateBase  *TimedOut(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *DMAcceptor(void);
    virtual void UnInitialize(void);

	SMStateBase *BaseInitialize(void);

	// pos15437
    virtual SMStateBase *TBParse(MessageElement *me);
  DECLARE_DYNCREATE(SMCardProcessing)// MFC Runtime class/object information
};

#endif