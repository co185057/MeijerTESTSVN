//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInvalidBarcode.CPP
//
// TITLE:	Class implementation for 2d Invalid Bardode state
//
// AUTHOR:    Rhonah Entize
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMInvalidBarcode.h"           // MGV this state


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("InvalidBarcode")

IMPLEMENT_DYNCREATE(SMInvalidBarcode, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMInvalidBarcode::SMInvalidBarcode()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMInvalidBarcode::Initialize(void)
{
    return SMInvalidBarcodeBase::Initialize();
}

void SMInvalidBarcode::UnInitialize(void)
{
    SMInvalidBarcodeBase::UnInitialize();
}

SMStateBase * SMInvalidBarcode::TimedOut(void)
{
    return SMInvalidBarcodeBase::TimedOut();
}

SMStateBase * SMInvalidBarcode::PSButton1(void)
{
    return SMInvalidBarcodeBase::PSButton1();
}
