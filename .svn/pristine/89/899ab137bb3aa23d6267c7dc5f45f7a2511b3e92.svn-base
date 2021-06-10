//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAcknowledgeSig.cpp
//
// TITLE: Class implimentation for Acknowledge Signature State
//
//
// AUTHOR:    Yafu Wang
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMAcknowledgeSig.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("AcknowledgeSig")

IMPLEMENT_DYNCREATE(SMAcknowledgeSig, SMAcknowledgeSigBase)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMAcknowledgeSig::SMAcknowledgeSig()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMAcknowledgeSig::Initialize(void)
{
    return SMAcknowledgeSigBase::Initialize();
}

void SMAcknowledgeSig::UnInitialize(void)
{
    SMAcknowledgeSigBase::UnInitialize();
}

SMStateBase * SMAcknowledgeSig::OnWtIncreaseNotAllowed(void)
{
    return SMAcknowledgeSigBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMAcknowledgeSig::OnWtDecrease(void)
{
    return SMAcknowledgeSigBase::OnWtDecrease();
}

BOOL SMAcknowledgeSig::FileToSafeArray( COleVariant& vPngFile, CString& csTempFile )
{
    return SMAcknowledgeSigBase::FileToSafeArray(vPngFile, csTempFile );
}

SMStateBase * SMAcknowledgeSig::PSButton1(void)
{
    return SMAcknowledgeSigBase::PSButton1();
}

SMStateBase * SMAcknowledgeSig::PSButton2(void)
{
    return SMAcknowledgeSigBase::PSButton2();
}

SMStateBase * SMAcknowledgeSig::PSEnterKey(void)
{
    return SMAcknowledgeSigBase::PSEnterKey();
}

SMStateBase * SMAcknowledgeSig::PSButtonHelp(void)
{
    return SMAcknowledgeSigBase::PSButtonHelp();
}

SMStateBase * SMAcknowledgeSig::TimedOut(void)
{
    return SMAcknowledgeSigBase::TimedOut();
}
