//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCashPaymentBase.h
//
// TITLE: Class header for Cash Payment State
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CASHPAYMENTBASESTATE
#define _CASHPAYMENTBASESTATE

#ifdef _CPPUNIT
   #include "FakeSMCustomerModeState.h"
#else
   #include "SMCustomerModeState.h"
#endif // _CPPUNIT


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMCashPaymentBase : public SMCUSTOMERMODESTATE
{
#ifndef _CPPUNIT
public:
  SMCashPaymentBase();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
#endif
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton1(void);
#endif
  virtual SMStateBase  *DMAcceptor(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *SAEventOccurred(const long);
  virtual SMStateBase  *TimedOut(void);  
  //@@@INFOPOINT
  virtual SMStateBase  *PSCancelKey(void);/// override where needed
  virtual SMStateBase  *PSButtonGoBack(void);
#endif
  virtual long DMSayPhrase(LPCTSTR sPhrase); //sscoadk-5501
  virtual long DMSayPhrase(const int nMsg);  //sscoadk-5501
  virtual long DMSaySecurity(const long nMsg, LPCTSTR szWave, HANDLE hWaitEvent =0); //sscoadk-5501
  //@@@INFOPOINT
  DECLARE_DYNCREATE(SMCashPaymentBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMCashPayment(void);
// or
//    DLLEXPORT SMStateBase *makeSMCashPaymentBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMCashPayment objects.  This function is called by
// RETURNSTATE(CashPayment) instead of calling new STATE(CashPayment) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMCashPaymentBase.cpp and SMCashPayment.cpp
// by using the macro IMPLEMENT_STATE(CashPayment)

//DECLARE_STATE(CashPayment)

#endif
