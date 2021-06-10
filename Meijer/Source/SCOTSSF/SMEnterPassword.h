//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterPassword.h
//
// TITLE: Class header for Enter Password state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERPASSWORDSTATE
#define _ENTERPASSWORDSTATE

#include "SMEnterPasswordBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMEnterPassword : public SMEnterPasswordBase
{
public:
    SMEnterPassword();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSButtonGoBack(void);


  DECLARE_DYNCREATE(SMEnterPassword)// MFC Runtime class/object information
};

#endif