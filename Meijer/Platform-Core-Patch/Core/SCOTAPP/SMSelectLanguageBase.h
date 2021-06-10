//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectLanguageBase.h
//
// TITLE: Class header for Customer Language Selection State
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTLANGUAGEBASESTATE
#define _SELECTLANGUAGEBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSelectLanguageBase : public SMCustomerModeState
{
public:
	SMSelectLanguageBase ();
      virtual SMStateBase  *Initialize(void);
      virtual void     UnInitialize(void);
	
protected:
	  virtual SMStateBase  *PSButtonGoBack(void);
	  virtual SMStateBase  *ButtonPressed(int);
	  virtual SMStateBase  *PSListLookup(CString);
      DECLARE_DYNCREATE(SMSelectLanguageBase)// MFC Runtime class/object information
 };


#endif
