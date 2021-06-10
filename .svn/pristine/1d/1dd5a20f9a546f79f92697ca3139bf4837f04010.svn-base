//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterPasswordBase.h
//
// TITLE: Class header for Enter Password state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERPASSWORDBASESTATE
#define _ENTERPASSWORDBASESTATE

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

SMEnterPasswordBase : public SMCUSTOMERMODESTATE
{

public:
   SMEnterPasswordBase();
   virtual SMStateBase  *Initialize(void);
   virtual void UnInitialize(void);		// RFC 330050

protected:
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSButtonGoBack(void);

  DECLARE_DYNCREATE(SMEnterPasswordBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(EnterPassword)

#endif
