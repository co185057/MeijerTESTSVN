//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAcknowledgeSig.h
//
// TITLE: Class header for Request Sig
//
//
// AUTHOR:    Yafu Wang
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ACKNOWLEDGESIGSTATE
#define _ACKNOWLEDGESIGSTATE

#include "SMAcknowledgeSigBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
SMAcknowledgeSig : public SMAcknowledgeSigBase
{
public:
    SMAcknowledgeSig();

    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);

protected:
    virtual BOOL FileToSafeArray( COleVariant& vPngFile, CString& csTempFile );
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSEnterKey(void);
    virtual SMStateBase  *PSButtonHelp(void);

    virtual SMStateBase *TimedOut(void);

    DECLARE_DYNCREATE(SMAcknowledgeSig)// MFC Runtime class/object information
};

#endif