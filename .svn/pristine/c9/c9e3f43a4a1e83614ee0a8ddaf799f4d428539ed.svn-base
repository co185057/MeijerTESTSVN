//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmKeyInWtTol.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMKEYINWTTOLSTATE
#define _SMMKEYINWTTOLSTATE

#include "SMSmmKeyInWtTolBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMSmmKeyInWtTol : public SMSmmKeyInWtTolBase
{
public:
    SMSmmKeyInWtTol();					  // constructor
    virtual SMStateBase * Initialize(void);
    virtual void     UnInitialize(void);

protected:
    virtual SMStateBase * PSEnterKey();
    virtual SMStateBase * PSNumericKey(const long);
    virtual SMStateBase * PSDecimalKey(const long);
    virtual SMStateBase * PSClearKey();
    virtual SMStateBase * PSButton8(void);


    DECLARE_DYNCREATE(SMSmmKeyInWtTol)// MFC Runtime class/object information
};

#endif
