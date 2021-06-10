//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPutBagOnScaleBase.h
//
// TITLE: Class header for Put Bag On Scale State - Done for SR085 by Shakti Kamal
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PutBagOnScaleBaseSTATE
#define _PutBagOnScaleBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMPutBagOnScaleBase : public SMCustomerModeState
{
public:
	SMPutBagOnScaleBase();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); //tar 201569

protected:
  virtual SMStateBase *PSButtonHelp(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *PSEnterKey(void);
  virtual SMStateBase *TareNone(void);
  virtual SMStateBase *TareOnlyAtAttract(void);
  virtual SMStateBase *TimedOut(void);
  
  DECLARE_DYNCREATE(SMPutBagOnScaleBase)// MFC Runtime class/object information
		
};

#endif