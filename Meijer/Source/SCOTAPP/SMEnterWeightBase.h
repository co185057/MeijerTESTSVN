//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterWeightBase.h
//
// TITLE: Class header for enter weight state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _EnterWeightBaseSTATE
#define _EnterWeightBaseSTATE

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
  SMEnterWeightBase : public SMCUSTOMERMODESTATE
{
public:
	SMEnterWeightBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize();
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSCancelKey(void);
  virtual SMStateBase  *DMScale(void);
  virtual SMStateBase  *PSButtonGoBack(void);
#endif //_CPPUNIT
  CString csCurContext;    // Tar 238987
#ifndef _CPPUNIT 
  //+SR814.2
  virtual SMStateBase  *PSButton1(void);
#endif //_CPPUNIT
  virtual SMStateBase  *ProcessWeightOnScale();
  static VOID CALLBACK EnterWeightTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
  virtual void HandleEnterWeightTimedOut();
  static SMEnterWeightBase* m_pEnterWeight;	
  static bool  m_bInTimeOut;
#ifndef _CPPUNIT 
  virtual SMStateBase  *TimedOut(void);
#endif //_CPPUNIT
  virtual bool ShowAcceptWeightBtnAndStartEnterWeightTimer(bool bWeightChange = false);
  //-SR814.2
  DECLARE_DYNCREATE(SMEnterWeightBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMEnterWeight(void);
// or
//    DLLEXPORT SMStateBase *makeSMEnterWeightBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMEnterWeight objects.  This function is called by
// RETURNSTATE(EnterWeight) instead of calling new STATE(EnterWeight) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMEnterWeightBase.cpp and SMEnterWeight.cpp
// by using the macro IMPLEMENT_STATE(EnterWeight)

//DECLARE_STATE(EnterWeight)

#endif
