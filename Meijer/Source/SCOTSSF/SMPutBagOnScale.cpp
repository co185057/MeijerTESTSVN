//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPutBagOnScale.cpp
//
// TITLE: Class implimentation for Put Bag On Scale State - Added for SR085
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes
 
#include "SMState.h"                // MGV base state
#include "SMPutBagOnScale.h"          // MGV this state

#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("PutBagOnScale")

IMPLEMENT_DYNCREATE(SMPutBagOnScale, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMPutBagOnScale::SMPutBagOnScale()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMPutBagOnScale::Initialize(void)
{
    return SMPutBagOnScaleBase::Initialize();
}

void SMPutBagOnScale::UnInitialize(void)
{
    SMPutBagOnScaleBase::UnInitialize();
}

SMStateBase * SMPutBagOnScale::OnWtIncreaseNotAllowed(void)
{
    return SMPutBagOnScaleBase::OnWtIncreaseNotAllowed();
}

SMStateBase * SMPutBagOnScale::PSButtonHelp(void)
{
    return SMPutBagOnScaleBase::PSButtonHelp();
}

//////////////////////////////////////
// PSButton2 - Done Button Processing
//////////////////////////////////////
SMStateBase * SMPutBagOnScale::PSButton2(void)
{
    // POS29349: Let sales app know that customer has brought their own shopping bag.
    SendTBCommand(TBCMD_NOTIFY_BROUGHT_BAG, true);
	
    // Call base functionality for Done button processing.
    SMStateBase *pRetState = SMPutBagOnScaleBase::PSButton2();
	
    return pRetState;
}

SMStateBase * SMPutBagOnScale::PSEnterKey(void)
{
    return SMPutBagOnScaleBase::PSEnterKey();
}

SMStateBase * SMPutBagOnScale::TareNone(void)
{
    return SMPutBagOnScaleBase::TareNone();
}

SMStateBase * SMPutBagOnScale::TareOnlyAtAttract(void)
{
    return SMPutBagOnScaleBase::TareOnlyAtAttract();
}

SMStateBase * SMPutBagOnScale::TimedOut(void)
{
    return SMPutBagOnScaleBase::TimedOut();
}
