//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    SMStateRp.CPP
//
// TITLE:   RP SMState functions, implement all SMState::RP* functions
//
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include "common.h"
#include "SMState.h"



SMStateBase * SMState::RPParse(MessageElement *me)
{
    return SMStateBase::RPParse(me);
}

long SMState::RPInitialize(void)
{
    return SMStateBase::RPInitialize();
}

long SMState::RPUnInitialize(void)
{
    return SMStateBase::RPUnInitialize();
}

SMStateBase * SMState::OnRPReceived(long byteCount)
{
    return SMStateBase::OnRPReceived(byteCount);
}

SMStateBase * SMState::OnRPDisconnect()
{
    return SMStateBase::OnRPDisconnect();
}

SMStateBase * SMState::SendRPDisconnect()
{
    return SMStateBase::SendRPDisconnect();
}

SMStateBase * SMState::SendRPConnect()
{
    return SMStateBase::SendRPConnect();
}
