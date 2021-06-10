//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmGenericDelayedBase.h
//
// TITLE: Class header for update item state
//
//
// AUTHOR:    Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMGENERICDELAYEDBASESTATE
#define _SMGENERICDELAYEDBASESTATE


#ifndef _CPPUNIT
    #include "SMSmAssistMenu.h"
#else
    #include "FakeSMSmAssistMenuBase.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmGenericDelayedBase : public STATE(SmAssistMenu)
{
public:
  SMSmGenericDelayedBase();

  virtual SMStateBase *Initialize(void);
  virtual void     UnInitialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
  
  static CString m_csLastInterventionID;

protected:

  CString csCurContext;    // Tar 238987
  DECLARE_DYNCREATE(SMSmGenericDelayedBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmGenericDelayed(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmGenericDelayedBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmGenericDelayed objects.  This function is called by
// RETURNSTATE(SmGenericDelayed) instead of calling new STATE(SmGenericDelayed) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmGenericDelayedBase.cpp and SMSmGenericDelayed.cpp
// by using the macro IMPLEMENT_STATE(GenericDelayed)

//DECLARE_STATE(SmGenericDelayed)

#endif  //_SMGENERICDELAYEDBASESTATE
