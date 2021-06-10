//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScaleBroken.cpp
//
// TITLE: Class implimentation for Scale Broken State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMScaleBroken.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("ScaleBroken")

IMPLEMENT_DYNCREATE(SMScaleBroken, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////////
SMScaleBroken::SMScaleBroken()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMScaleBroken::Initialize(void)
{
    return SMScaleBrokenBase::Initialize();
}

void SMScaleBroken::UnInitialize(void)
{
    SMScaleBrokenBase::UnInitialize();
}

SMStateBase * SMScaleBroken::TimedOut(void)
{
    return SMScaleBrokenBase::TimedOut();
}

SMStateBase * SMScaleBroken::DMScale(void)
{
    return SMScaleBrokenBase::DMScale();
}

SMStateBase * SMScaleBroken::PSButtonGoBack(void)
{
    return SMScaleBrokenBase::PSButtonGoBack();
}

