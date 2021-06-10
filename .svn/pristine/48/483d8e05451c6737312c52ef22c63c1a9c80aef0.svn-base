//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectFromListBase.h
//
// TITLE: Class implementation for Select From List Screen
//
// AUTHOR:   Feng Zhang
// DATE:   1998.7.14
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSELECTFROMLISTSTATE
#define _SMSELECTFROMLISTSTATE

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
SMSelectFromListBase : public SMCUSTOMERMODESTATE
{
public:
	SMSelectFromListBase();               // constructor
#ifndef _CPPUNIT
  virtual SMStateBase  *Initialize(void);        // required override
	virtual void     UnInitialize(void);

#endif //_CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton3(void);         // override for button 3
  virtual SMStateBase  *PSButton4(void);         // override for button 4
#endif //_CPPUNIT
  virtual SMStateBase  *PSListLookup(CString);      // override for list/lookups
#ifndef _CPPUNIT
  virtual SMStateBase  *PSNumericKey(const long);	// a key 0-9 was pressed
  virtual SMStateBase  *PSEnterKey(void);        // override for Enter key
  virtual SMStateBase  *PSClearKey(void);        // override for clear key
  virtual SMStateBase  *PSCancelKey(void);       // override for cancel key
#endif //_CPPUNIT
  virtual SMStateBase  *DMScanner(void);         // override for scanner --Feng
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButtonGoBack(void);         // override for GoBack Key

#endif //_CPPUNIT
  DECLARE_DYNCREATE(SMSelectFromListBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSelectFromList(void);
// or
//    DLLEXPORT SMStateBase *makeSMSelectFromListBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSelectFromList objects.  This function is called by
// RETURNSTATE(SelectFromList) instead of calling new STATE(SelectFromList) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSelectFromListBase.cpp and SMSelectFromList.cpp
// by using the macro IMPLEMENT_STATE(SelectFromList)

//DECLARE_STATE(SelectFromList)

#endif
