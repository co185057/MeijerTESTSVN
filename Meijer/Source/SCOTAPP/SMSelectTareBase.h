//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectTareBase.h
//
// TITLE: Class header for select tare state
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTTAREBASESTATE
#define _SELECTTAREBASESTATE

#ifdef _CPPUNIT
   #include "FakeSMCustomerModeState.h"
   #include "FakeSMStateBase.h"
#else
   #include "SMCustomerModeState.h"
#endif // _CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSelectTareBase : public SMCUSTOMERMODESTATE
{
public:
  SMSelectTareBase();                            // constructor
  virtual SMStateBase  *Initialize(void);        // required override
#ifndef _CPPUNIT
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize(void);		// RFC 330050
#endif // _CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton4(void);         // override for button 4
#endif // _CPPUNIT
  virtual SMStateBase  *PSListLookup(CString);      // override for list/lookups
#ifndef _CPPUNIT
  virtual SMStateBase  *PSCancelKey(void);       // override where needed
  //*** NewUI - aiqin ***//
  virtual SMStateBase  *PSListScroll(const long);
  //*** end of NewUI ***//
  //@@@INFOPOINT
  virtual SMStateBase  *PSButtonGoBack(void);         // override for GoBack Key
  //@@@INFOPOINT
  virtual SMStateBase  *PSReceiptUp(void);
#endif // _CPPUNIT
  virtual SMStateBase  *PSReceiptDown(void);
  virtual SMStateBase  *PSButton1(void);
  CString csCurContext;    // Tar 238987
	
  DECLARE_DYNCREATE(SMSelectTareBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSelectTare(void);
// or
//    DLLEXPORT SMStateBase *makeSMSelectTareBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSelectTare objects.  This function is called by
// RETURNSTATE(SelectTare) instead of calling new STATE(SelectTare) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSelectTareBase.cpp and SMSelectTare.cpp
// by using the macro IMPLEMENT_STATE(SelectTare)

//DECLARE_STATE(SelectTare)

#endif
