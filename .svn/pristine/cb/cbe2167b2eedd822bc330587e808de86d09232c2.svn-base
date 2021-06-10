//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditAdd.CPP
//
// TITLE: Class implementation for store mode Security Maintenance Mode 
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // common includes

#include "SMState.h"                // base state
#include "SMSmmEditAdd.h"				// this state

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmmEditAdd")

IMPLEMENT_DYNCREATE(SMSmmEditAdd, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmmEditAdd::SMSmmEditAdd()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmmEditAdd::Initialize(void)
{
    return SMSmmEditAddBase::Initialize();
}

void SMSmmEditAdd::UnInitialize(void)
{
    SMSmmEditAddBase::UnInitialize();
}

SMStateBase * SMSmmEditAdd::DMScanner(void)
{
    MeijerModifyScannedBarcode();
	return SMSmmEditAddBase::DMScanner();
}

SMStateBase * SMSmmEditAdd::PSButton1(void)
{
    return SMSmmEditAddBase::PSButton1();
}

SMStateBase * SMSmmEditAdd::PSButton2(void)
{
    return SMSmmEditAddBase::PSButton2();
}

SMStateBase * SMSmmEditAdd::PSButton3(void)
{
    return SMSmmEditAddBase::PSButton3();
}

SMStateBase * SMSmmEditAdd::PSButton4(void)
{
    return SMSmmEditAddBase::PSButton4();
}

SMStateBase * SMSmmEditAdd::PSButton5(void)
{
    return SMSmmEditAddBase::PSButton5();
}

SMStateBase * SMSmmEditAdd::PSButton6(void)
{
    return SMSmmEditAddBase::PSButton6();
}

SMStateBase * SMSmmEditAdd::PSButton7(void)
{
    return SMSmmEditAddBase::PSButton7();
}

SMStateBase * SMSmmEditAdd::PSButton8(void)
{
    return SMSmmEditAddBase::PSButton8();
}

SMStateBase * SMSmmEditAdd::PSReceiptUp(void)
{
    return SMSmmEditAddBase::PSReceiptUp();
}

SMStateBase * SMSmmEditAdd::PSReceiptDown(void)
{
    return SMSmmEditAddBase::PSReceiptDown();
}

SMStateBase * SMSmmEditAdd::DMScannerEncrypted(void)
{
    return SMSmmEditAddBase::DMScannerEncrypted();
}
