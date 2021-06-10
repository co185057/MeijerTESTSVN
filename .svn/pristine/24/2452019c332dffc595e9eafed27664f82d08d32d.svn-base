//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateUSSF.CPP
//
// TITLE: PS SMState functions, implement all SMState::PS* functions
//
//
// AUTHOR:  Christopher Swift
//
//////////////////////////////////////////////////////////////////////////////////////////////////

// TAR #429050 START
#include <stdafx.h>                 // MS MFC always needed first
#include "Common.h"
#include "SMState.h"
#include "USSFCommon.h"
#include "USSFProcedures.h"
#include "USSFUtility.h"
USSFUtility ub;
DLLEXPORT USSFUtility &USSFutil = ub;
// TAR #429050 STOP


SMStateBase * SMState::StateEntry()
{
    return SMStateBase::StateEntry();
}

void SMState::StateExit()
{
    SMStateBase::StateExit();
}

SMStateBase * SMState::HookEntry(CString csEventName, long &lHookState, bool &bDone, SMStateBase *newState)
{
    return SMStateBase::HookEntry(csEventName, lHookState, bDone, newState);
}

void SMState::SetActiveState(CString thisState)
{
    SMStateBase::SetActiveState(thisState);
}

void SMState::SetActiveContext(CString thisContext)
{
    SMStateBase::SetActiveContext(thisContext);
}

SMStateBase * SMState::ValidateContext(CString csContextName)
{
    return SMStateBase::ValidateContext(csContextName);
}

SMStateBase * SMState::InitializeUSSF(void)
{
    return SMStateBase::InitializeUSSF();
}

bool SMState::getUSSFDefaultContext(CString &csDefaultContext)
{
    return SMStateBase::getUSSFDefaultContext(csDefaultContext);
}

long SMState::isHandled(CString currentEvent, CString altContext, bool bGAMode)
{
    return SMStateBase::isHandled(currentEvent, altContext, bGAMode);
}

SMStateBase * SMState::MakeNextMove(CString thisContext )
{
    return SMStateBase::MakeNextMove(thisContext );
}

SMStateBase * SMState::ProcessEvent(CString thisEvent, CString thisContext )
{
    return SMStateBase::ProcessEvent(thisEvent , thisContext );
}

SMStateBase * SMState::ProcessEventComplete(CString thisEvent, CString thisContext )
{
    return SMStateBase::ProcessEventComplete(thisEvent , thisContext );
}

SMStateBase * SMState::ProcessEventPartial(CString thisEvent, CString thisContext )
{
    return SMStateBase::ProcessEventPartial(thisEvent , thisContext );
}

bool SMState::isUSSFMode(void)
{
    return SMStateBase::isUSSFMode();
}

CString SMState::GetPendingNextStep(void)
{
    return SMStateBase::GetPendingNextStep();
}

void SMState::SetPendingNextStep(CString step)
{
    SMStateBase::SetPendingNextStep(step);
}

long SMState::StoreUSSFData(CString name, COleVariant value)
{
    return SMStateBase::StoreUSSFData(name, value );
}

long SMState::StoreAPPLData(CString name, COleVariant value, CString prefix )
{
    return SMStateBase::StoreAPPLData(name, value , prefix );
}

void SMState::SetOperationalState(SMStateBase *pOpState)
{
    SMStateBase::SetOperationalState(pOpState);
}

SMStateBase * SMState::PerformAction(CString thisAction, bool &bActionTaken)
{
    return SMStateBase::PerformAction(thisAction, bActionTaken);
}

long SMState::ParseAction(CString thisAction)
{
    return SMStateBase::ParseAction(thisAction);
}

long SMState::ShowPOSDisplay(CString csLine1, CString csLine2)
{
    return SMStateBase::ShowPOSDisplay(csLine1, csLine2);
}

long SMState::ClearPOSDisplay(void)
{
    return SMStateBase::ClearPOSDisplay();
}

long SMState::SendKeySequence(CString csSequence, int iSleep)
{
    return SMStateBase::SendKeySequence(csSequence, iSleep);
}

long SMState::SendKeySequence(unsigned char ucSequence, int iSleep)
{
    return SMStateBase::SendKeySequence(ucSequence, iSleep);
}

SMStateBase * SMState::PureMethod(void)
{
    return SMStateBase::PureMethod();
}

long SMState::SendPOSData(CString csSource)
{
    return SMStateBase::SendPOSData(csSource);
}

CString SMState::getStateName(const SMStateBase * const pState)
{
    return SMStateBase::getStateName(pState);
}

int SMState::DoTranslationFromWCToMB( char *ch, const wchar_t *wChar, size_t count,bool bIgnoreNull )
{
    return SMStateBase::DoTranslationFromWCToMB(ch, wChar, count, bIgnoreNull );
}

SMStateBase * SMState::RunScript(CString thisScript)
{
    return SMStateBase::RunScript(thisScript);
}

SMStateBase * SMState::ManageAction(CString thisAction, bool &bActionTaken)
{
    return SMStateBase::ManageAction(thisAction, bActionTaken);
}

void SMState::MarkActivation(CString reference)
{
    SMStateBase::MarkActivation(reference );
}

SMStateBase * SMState::MakePreviousMove(void)
{
    return SMStateBase::MakePreviousMove();
}

SMStateBase * SMState::MakeReferenceMove(CString reference)
{
    return SMStateBase::MakeReferenceMove( reference );
}

void SMState::SetActivationState(CString state, CString context)
{
    SMStateBase::SetActivationState(state, context);
}

void SMState::SetActivationState(void)
{
    SMStateBase::SetActivationState();
}

void SMState::ClearActivationDB(void)
{
    SMStateBase::ClearActivationDB();
}

void SMState::SaveActivationData(void)
{
    SMStateBase::SaveActivationData();
}
