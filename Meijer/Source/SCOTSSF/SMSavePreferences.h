//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSavePreferences.h
//
// TITLE: Class header for Save Preferences
//
//
// AUTHOR:    Barbara Dawkins
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SAVEPREFERENCESSTATE
#define _SAVEPREFERENCESSTATE

#include "SMSavePreferencesBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSavePreferences : public SMSavePreferencesBase
{
public:
    SMSavePreferences();

    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);


  DECLARE_DYNCREATE(SMSavePreferences)// MFC Runtime class/object information
};

#endif //_SAVEPREFERENCESSTATE