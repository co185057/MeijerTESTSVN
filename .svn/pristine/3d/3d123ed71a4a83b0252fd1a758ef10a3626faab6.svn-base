//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFinishBase.h
//
// TITLE: Class header for Finish State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FINISHBASESTATE
#define _FINISHBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMFinishBase : public SMCustomerModeState
{
public:
	SMFinishBase();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
  static bool issuedCashAcceptorError;					//TAR121479	LPM092999

protected:
  virtual SMStateBase  *DMAcceptor(void);
  virtual SMStateBase  *DMCardReader(void);
  virtual SMStateBase  *DMScanner(void);
  virtual SMStateBase  *PSListLookup(CString);// override for list/lookups
  virtual SMStateBase  *SAEventOccurred(const long);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *PSButtonHelp(void);       
  virtual SMStateBase  *checkCashAcceptorCassette();	//TAR121479	LPM092999
  virtual SMStateBase *ButtonPressed(int);
  virtual SMStateBase *StandardCardProcessing(int);
  virtual void ConfigureTenderOptions(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *PSConnectRemote( CString& strMachineName, CString& strRemoteContext );
  virtual bool IsPaymentButtonAvailable();	//SR742
  bool m_bCashTender;
  bool m_bRetFoodStampState;
  
  DECLARE_DYNCREATE(SMFinishBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMFinish(void);
// or
//    DLLEXPORT SMStateBase *makeSMFinishBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMFinish objects.  This function is called by
// RETURNSTATE(Finish) instead of calling new STATE(Finish) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMFinishBase.cpp and SMFinish.cpp
// by using the macro IMPLEMENT_STATE(Finish)

//DECLARE_STATE(Finish)

#endif
