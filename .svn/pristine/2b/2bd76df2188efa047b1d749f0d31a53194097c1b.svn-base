//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmPrintReceipt.CPP
//
// TITLE: Class implementation for store mode Print Receipt state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmPrintReceipt.h"

#define COMP_ID ID_SM                                                     // base state component
#define T_ID _T("SmPrintReceipt")


IMPLEMENT_DYNCREATE(SMSmPrintReceipt, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//================================
// constructor
//================================
SMSmPrintReceipt::SMSmPrintReceipt()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmPrintReceipt::Initialize(void)
{
    return SMSmPrintReceiptBase::Initialize();
}

void SMSmPrintReceipt::UnInitialize(void)
{
    SMSmPrintReceiptBase::UnInitialize();
}

SMStateBase * SMSmPrintReceipt::PSButton1(void)
{
    return SMSmPrintReceiptBase::PSButton1();
}

SMStateBase * SMSmPrintReceipt::PSButton2(void)
{
    return SMSmPrintReceiptBase::PSButton2();
}

SMStateBase * SMSmPrintReceipt::PSButton8(void)
{
    return SMSmPrintReceiptBase::PSButton8();
}

