//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMKeyInCodeBase.h
//
// TITLE: Class header for Key In Code state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _KEYINCODEBASESTATE
#define _KEYINCODEBASESTATE

#ifndef _CPPUNIT
#include "SMCustomerModeState.h"
#else
#include "FakeSMCustomerModeState.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

SMKeyInCodeBase : public SMCUSTOMERMODESTATE
{

public:
  SMKeyInCodeBase();
#ifndef _CPPUNIT
  virtual SMStateBase  *Initialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize(void);		// RFC 330050
#endif // _CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *PSClearKey(void);
#endif // _CPPUNIT
  virtual SMStateBase  *PSEnterKey(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *PSBackSpaceKey(void); //SR834
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *OnBackToLGW(void);				// Screen refreshes when Security calls OnBackToLGW, looses keyed in data LPM092302 214653
  virtual SMStateBase  *PSButton8(void);				// Basic Assist Mode need it
#endif // _CPPUNIT
  virtual SMStateBase  *DMScanner(void);				// Basic Assist Mode need it
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButtonHelp(void); //SSCOP-986
  virtual SMStateBase  *PSSwitchContext(BOOL bRemote, CString csNewContext); // SSCOP-1754
#endif // _CPPUNIT
  
  DECLARE_DYNCREATE(SMKeyInCodeBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(KeyInCode)

#endif
