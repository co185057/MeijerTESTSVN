//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterWtForPriceEmbedded.h
//
// TITLE: Class header for enter weight state
//
// AUTHOR:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERWTFORPRICEEMBEDDEDSTATE
#define _ENTERWTFORPRICEEMBEDDEDSTATE

#include "SMEnterWtForPriceEmbeddedBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMEnterWtForPriceEmbedded : public SMEnterWtForPriceEmbeddedBase
{
public:
    SMEnterWtForPriceEmbedded();
    virtual SMStateBase  *Initialize(void);
    virtual bool PSAssistMenuLegalForState();
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *DMScale(void);
    virtual SMStateBase  *PSButtonGoBack(void);
  

  DECLARE_DYNCREATE(SMEnterWtForPriceEmbedded)// MFC Runtime class/object information

};

#endif