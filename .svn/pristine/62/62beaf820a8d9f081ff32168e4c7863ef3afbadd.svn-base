//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRemoveItemFromProduceScale.cpp
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

#include "SMRemoveItemFromProduceScale.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("RemoveItemFromProduceScale")

IMPLEMENT_DYNCREATE(SMRemoveItemFromProduceScale, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////////
SMRemoveItemFromProduceScale::SMRemoveItemFromProduceScale()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMRemoveItemFromProduceScale::Initialize(void)
{
    return SMRemoveItemFromProduceScaleBase::Initialize();
}

void SMRemoveItemFromProduceScale::UnInitialize(void)
{
    SMRemoveItemFromProduceScaleBase::UnInitialize();
}


SMStateBase * SMRemoveItemFromProduceScale::DMScale(void)
{
    return SMRemoveItemFromProduceScaleBase::DMScale();
}

SMStateBase * SMRemoveItemFromProduceScale::TimedOut(void)
{
    return SMRemoveItemFromProduceScaleBase::TimedOut();
}
