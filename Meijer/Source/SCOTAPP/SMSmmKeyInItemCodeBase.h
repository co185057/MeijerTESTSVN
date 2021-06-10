//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SmmKeyInItemCodeBase.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR:  Rhonah Entize
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmmKeyInItemCodeSTATE
#define _SmmKeyInItemCodeSTATE

#include "SMStoreModeState.h"


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

SMSmmKeyInItemCodeBase : public STATE(StoreModeState)
{
public:
  SMSmmKeyInItemCodeBase();							// constructor
  virtual SMStateBase * Initialize(void);			// required override
  virtual void     UnInitialize(void);				// required override
	
protected:
  virtual SMStateBase * PSEnterKey();				// enter on the pinpad
  virtual SMStateBase * PSNumericKey(const long);	// numbers on the pinpad
  virtual SMStateBase * PSClearKey();				// acts like a backspace on the pinpad
  virtual SMStateBase * PSBackSpaceKey(void);		//backspace key
  virtual SMStateBase * PSButton8(void);			// go back
  
  
  DECLARE_DYNCREATE(SMSmmKeyInItemCodeBase)// MFC Runtime class/object information
};

//DECLARE_STATE(SmmKeyInItemCode)

#endif
