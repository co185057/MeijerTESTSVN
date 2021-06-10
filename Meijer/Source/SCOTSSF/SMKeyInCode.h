//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMKeyInCode.h
//
// TITLE: Class header for Key In Code state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _KEYINCODESTATE
#define _KEYINCODESTATE

#include "SMKeyInCodeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMKeyInCode : public SMKeyInCodeBase
{
public:
    SMKeyInCode();
    virtual SMStateBase  *Initialize(void);
    virtual bool PSAssistMenuLegalForState();
    virtual void UnInitialize(void);
protected:
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSBackSpaceKey(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnBackToLGW(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *PSButtonHelp(void);
    virtual SMStateBase  *PSSwitchContext(BOOL bRemote, CString csNewContext);


    DECLARE_DYNCREATE(SMKeyInCode)// MFC Runtime class/object information
};

#endif