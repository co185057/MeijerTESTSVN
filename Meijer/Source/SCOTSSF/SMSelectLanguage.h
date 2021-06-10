//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectLanguage.h
//
// TITLE: Class header for Customer Sample State
//
//
// AUTHOR:    John Doe
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTLANGUAGESTATE
#define _SELECTLANGUAGESTATE

#include "SMSelectLanguageBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSelectLanguage : public SMSelectLanguageBase
{
public:
    SMSelectLanguage();
    virtual SMStateBase  *Initialize(void);
    virtual void     UnInitialize(void);

protected:
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *ButtonPressed(int);
    virtual SMStateBase  *PSListLookup(CString);

    DECLARE_DYNCREATE(SMSelectLanguage)// MFC Runtime class/object information
};

#endif
