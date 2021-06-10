//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSavePreferencesBase.h
//
// TITLE: Class header for Save Preferences
//
//
// AUTHOR:   Barbara Dawkins
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SavePreferencesBASESTATE
#define _SavePreferencesBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMSavePreferencesBase : public SMCustomerModeState
{
public:
	SMSavePreferencesBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  CString csWork;
  CString csCode;
	
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);

private:
    //+SR815.5 Personalized Volume 
    CString MsgPreferredLanguage();
    CString MsgPreferredVolume();
    //-SR815.5 Personalized Volume 
  
  DECLARE_DYNCREATE(SMSavePreferencesBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSavePreferences(void);
// or
//    DLLEXPORT SMStateBase *makeSMSavePreferencesBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSavePreferences objects.  This function is called by
// RETURNSTATE(SavePreferences) instead of calling new STATE(SavePreferences) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSavePreferencesBase.cpp and SMSavePreferences.cpp
// by using the macro IMPLEMENT_STATE(SavePreferences)

//DECLARE_STATE(SavePreferences)

#endif
