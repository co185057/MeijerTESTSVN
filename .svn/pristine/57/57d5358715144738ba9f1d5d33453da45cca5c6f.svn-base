//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMDegradedContinueTrans.cpp
//
// TITLE: Class implimentation for Degraded Continue Transaction State
//
// AUTHOR:    Allen Jay Cagaanan
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGE HISTORY:
//
// CRD 460612 Name: Bob Igot Date: July 20, 2020
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////



#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
 
//#include "SMState.h"                // MGV base state
#include "SMDegradedContinueTrans.h"          // MGV this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("DegradedContinueTrans")

IMPLEMENT_DYNCREATE(SMDegradedContinueTrans, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMDegradedContinueTrans::SMDegradedContinueTrans(void)
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMDegradedContinueTrans::Initialize(void)
{
    return SMDegradedContinueTransBase::Initialize();
}

void SMDegradedContinueTrans::UnInitialize(void)
{
    SMDegradedContinueTransBase::UnInitialize();
}

SMStateBase * SMDegradedContinueTrans::OnWtIncreaseNotAllowed(void)
{
    return SMDegradedContinueTransBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMDegradedContinueTrans::TimedOut(void)
{
    return SMDegradedContinueTransBase::TimedOut();
}

SMStateBase * SMDegradedContinueTrans::PSButton1(void)
{
    return SMDegradedContinueTransBase::PSButton1();
}

SMStateBase * SMDegradedContinueTrans::PSButton2(void)
{
	//Start CRD 460612 set fItemIsWaiting to false when PSButton2(void) is called to reset scanned items
	fItemIsWaiting = false;
	//End CRD 460612
    return SMDegradedContinueTransBase::PSButton2();
}

SMStateBase * SMDegradedContinueTrans::DMCardReader(void)
{
    return SMDegradedContinueTransBase::DMCardReader();
}
