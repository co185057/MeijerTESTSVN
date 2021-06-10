//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPLAHandInView.cpp
//
// TITLE: Class implementation for select item state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMPLAHandInView.h"           // this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("PLAEnterWeight")
                                                   

IMPLEMENT_DYNCREATE(SMPLAHandInView, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMPLAHandInView::SMPLAHandInView()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMPLAHandInView::Initialize(void)
{
    return SMPLAHandInViewBase::Initialize();
}

void SMPLAHandInView::UnInitialize(void)
{
    SMPLAHandInViewBase::UnInitialize();
}

SMStateBase * SMPLAHandInView::TimedOut(void)
{
    return SMPLAHandInViewBase::TimedOut();
}

SMStateBase *SMPLAHandInView::OnPLATransition()
{
    return SMPLAHandInViewBase::OnPLATransition();
}
