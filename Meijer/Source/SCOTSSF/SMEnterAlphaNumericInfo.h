//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMSelectCardType.H
//
// TITLE:    Class header for Select Card Type State
//
// $Header:
//
// AUTHOR:   Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMTypeJobInformationSTATE
#define _SMTypeJobInformationSTATE

#include "SMEnterAlphaNumericInfoBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMEnterAlphaNumericInfo : public SMEnterAlphaNumericInfoBase
{
public:
    SMEnterAlphaNumericInfo();
    virtual SMStateBase  *Initialize(void);
    virtual void     UnInitialize(void);
    virtual bool PSAssistMenuLegalForState();
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSAlphaNumericKey(const long);
    virtual SMStateBase  *PSAlphaKey(const long);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSSpaceKey(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *PSButtonGoBack(void);

    DECLARE_DYNCREATE(SMEnterAlphaNumericInfo)// MFC Runtime class/object information

};

#endif
