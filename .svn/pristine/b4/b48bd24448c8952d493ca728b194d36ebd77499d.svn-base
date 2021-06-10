//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSwipeCardBase.H
//
// TITLE:    Class header for Select Card Type State
//
//
// AUTHOR:   Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SWIPECARDBASESTATE
#define _SWIPECARDBASESTATE


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
SMSwipeCardBase : public SMCUSTOMERMODESTATE
{
	protected:
  	TBTENDERTYPE nTndrType;
	long lTenderAmt;
public:
  SMSwipeCardBase(const TBTENDERTYPE, const long );
  SMSwipeCardBase();
  SMStateBase *Deliver(const TBTENDERTYPE, const long );
#ifndef _CPPUNIT
  virtual SMStateBase *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); //tar 194770
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
#endif // _CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual SMStateBase *PSCancelKey(void);
  virtual SMStateBase *PSButton1(void);		//rfc330995
  
  virtual SMStateBase *SAEventOccurred(const long);
  //@@@INFOPOINT
  virtual SMStateBase  *PSButtonGoBack(void);         // override for GoBack Key
  //@@@INFOPOINT
  virtual SMStateBase  *DMCardReader(void);
  virtual SMStateBase  *DMAcceptor(void);               
#endif // _CPPUNIT
  virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);//TAR212390 CHU/Dale 08/20/02

	
  DECLARE_DYNCREATE(SMSwipeCardBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSwipeCard(void);
// or
//    DLLEXPORT SMStateBase *makeSMSwipeCardBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSwipeCard objects.  This function is called by
// RETURNSTATE(SwipeCard) instead of calling new STATE(SwipeCard) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSwipeCardBase.cpp and SMSwipeCard.cpp
// by using the macro IMPLEMENT_STATE(SwipeCard)

//DECLARE_STATE(SwipeCard)

#endif
