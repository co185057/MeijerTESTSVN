//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmKeyInItemCode.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR: Rhonah Entize
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMKEYINITEMCODESTATE
#define _SMMKEYINITEMCODESTATE

#include "SMSmmKeyInItemCodeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMSmmKeyInItemCode : public SMSmmKeyInItemCodeBase
{
public:
    SMSmmKeyInItemCode();					  // constructor
    virtual SMStateBase * Initialize(void);
    virtual void     UnInitialize(void);

protected:
    virtual SMStateBase * PSEnterKey();
    virtual SMStateBase * PSNumericKey(const long);
    virtual SMStateBase * PSClearKey();
    virtual SMStateBase * PSBackSpaceKey(void);
    virtual SMStateBase * PSButton8(void);
  

    DECLARE_DYNCREATE(SMSmmKeyInItemCode)// MFC Runtime class/object information
};

#endif
