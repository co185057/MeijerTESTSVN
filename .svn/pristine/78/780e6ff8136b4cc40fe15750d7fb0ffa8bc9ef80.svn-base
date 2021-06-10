//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMKeyInCodeBase.h
//
// TITLE: Class header for Key In Code state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _KEYINCODEBASESTATE
#define _KEYINCODEBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

SMKeyInCodeBase : public SMCustomerModeState
{

public:
  SMKeyInCodeBase();
  virtual SMStateBase  *Initialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize(void);		// RFC 330050
protected:
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnBackToLGW(void);				// Screen refreshes when Security calls OnBackToLGW, looses keyed in data LPM092302 214653
  virtual SMStateBase  *PSButton8(void);				// Basic Assist Mode need it
  virtual SMStateBase  *DMScanner(void);				// Basic Assist Mode need it
  
  DECLARE_DYNCREATE(SMKeyInCodeBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(KeyInCode)

#endif
