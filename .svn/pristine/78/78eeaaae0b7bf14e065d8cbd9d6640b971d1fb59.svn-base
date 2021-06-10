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
  SMSelectLanguageBase : public SMCUSTOMERMODESTATE
{
public:
    SMSelectLanguageBase ();
    virtual SMStateBase  *Initialize(void);
#ifndef _CPPUNIT
    virtual void     UnInitialize(void);
#endif // _CPPUNIT
protected:
#ifndef _CPPUNIT
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *ButtonPressed(int);
    virtual SMStateBase  *PSListLookup(CString);
#endif // _CPPUNIT
      DECLARE_DYNCREATE(SMSelectLanguageBase)// MFC Runtime class/object information
 };


#endif
