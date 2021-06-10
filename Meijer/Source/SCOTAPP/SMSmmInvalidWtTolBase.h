//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmInvalidWtTolBase.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmmInvalidWtTolBaseSTATE
#define _SmmInvalidWtTolBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmmInvalidWtTolBase : public STATE(StoreModeState)
{
public:
  SMSmmInvalidWtTolBase();					  // constructor
  virtual SMStateBase * Initialize(void);     // required override
  virtual void     UnInitialize(void);				// required override
	
protected:
  virtual SMStateBase  *PSButton1(void);      // button 1

  DECLARE_DYNCREATE(SMSmmInvalidWtTolBase)// MFC Runtime class/object information
};

//DECLARE_STATE(SmmInvalidWtTol)

#endif
