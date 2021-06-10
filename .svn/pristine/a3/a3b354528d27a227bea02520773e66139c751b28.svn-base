//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SmmKeyInWtTolBase.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmmKeyInWtTolSTATE
#define _SmmKeyInWtTolSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmmKeyInWtTolBase : public STATE(StoreModeState)
{
public:
  SMSmmKeyInWtTolBase();							// constructor
  virtual SMStateBase * Initialize(void);			// required override
  virtual void     UnInitialize(void);				// required override
	
protected:
  virtual SMStateBase * PSEnterKey();				// enter on the pinpad
  virtual SMStateBase * PSNumericKey(const long);	// numbers on the pinpad
  virtual SMStateBase * PSDecimalKey(const long);	// numbers on the pinpad
  virtual SMStateBase * PSClearKey();				// acts like a backspace on the pinpad
  virtual SMStateBase * PSButton8(void);			// go back
	
  DECLARE_DYNCREATE(SMSmmKeyInWtTolBase)// MFC Runtime class/object information
};

//DECLARE_STATE(SmmKeyInWtTol)

#endif
