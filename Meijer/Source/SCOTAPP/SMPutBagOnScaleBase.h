//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPutBagOnScaleBase.h
//
// TITLE: Class header for Put Bag On Scale State - Done for SR085 by Shakti Kamal
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PutBagOnScaleBaseSTATE
#define _PutBagOnScaleBaseSTATE

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
  SMPutBagOnScaleBase : public SMCUSTOMERMODESTATE
{
public:
#ifndef _CPPUNIT
  SMPutBagOnScaleBase();
  virtual SMStateBase *Initialize(void);
  virtual void UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); //tar 201569
  virtual SMStateBase* OnBackToLGW(void);
#endif
protected: 
#ifndef _CPPUNIT
 virtual SMStateBase *PSButtonHelp(void);
#endif
  virtual SMStateBase *PSButton2(void);
#ifndef _CPPUNIT
  virtual SMStateBase *PSEnterKey(void);
#endif //_CPPUNIT		

  virtual SMStateBase *TareNone(void);
  virtual SMStateBase *TareOnlyAtAttract(void);
#ifndef _CPPUNIT
  virtual SMStateBase *TimedOut(void);
  
  DECLARE_DYNCREATE(SMPutBagOnScaleBase)// MFC Runtime class/object information
#endif //_CPPUNIT		
};

#endif