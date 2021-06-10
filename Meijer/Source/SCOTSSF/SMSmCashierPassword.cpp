//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SmCashierPassword.CPP
//
// TITLE: Class implementation for store mode Authenticate state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmCashierPassword.h"     // This state

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmEnterBirthdate")

IMPLEMENT_DYNCREATE(SMSmCashierPassword, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmCashierPassword::SMSmCashierPassword()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmCashierPassword::Initialize(void)
{
    return SMSmCashierPasswordBase::Initialize();
}

void SMSmCashierPassword::UnInitialize(void)
{
    SMSmCashierPasswordBase::UnInitialize();
}

SMStateBase * SMSmCashierPassword::PSButton8(void)
{
    return SMSmCashierPasswordBase::PSButton8();
}

SMStateBase * SMSmCashierPassword::PSEnterKey(void)
{
    return SMSmCashierPasswordBase::PSEnterKey();
}

SMStateBase * SMSmCashierPassword::PSClearKey(void)
{
    return SMSmCashierPasswordBase::PSClearKey();
}

SMStateBase * SMSmCashierPassword::PSNumericKey(const long lKey)
{
    return SMSmCashierPasswordBase::PSNumericKey(lKey);
}

void SMSmCashierPassword::ShowCashierIDSceen()
{
    SMSmCashierPasswordBase::ShowCashierIDSceen();
}

void SMSmCashierPassword::promptForCashierData( PROMPT pInputPrompt )
{
    SMSmCashierPasswordBase::promptForCashierData( pInputPrompt );
}

SMStateBase * SMSmCashierPassword::DMScanner(void)
{
    return SMSmCashierPasswordBase::DMScanner();
}

SMStateBase * SMSmCashierPassword::HandleScannerData( const MessageElement* message)
{
    return SMSmCashierPasswordBase::HandleScannerData( message);
}
