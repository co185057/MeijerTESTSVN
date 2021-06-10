//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRunReports.CPP
//
// TITLE: Class implementation for store mode RunReports state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmRunReports.h"

#define COMP_ID ID_SM                                                     // base state component
#define T_ID _T("SmRunReports")


IMPLEMENT_DYNCREATE(SMSmRunReports, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//================================
// constructor
//================================
SMSmRunReports::SMSmRunReports()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmRunReports::Initialize(void)
{
    return SMSmRunReportsBase::Initialize();
}

void SMSmRunReports::UnInitialize(void)
{
    SMSmRunReportsBase::UnInitialize();
}

void SMSmRunReports::DMReceiptLine(const CString& csPrintLine)
{
    SMSmRunReportsBase::DMReceiptLine(csPrintLine);
}

SMStateBase * SMSmRunReports::PSButton1(void)
{
    return SMSmRunReportsBase::PSButton1();
}

SMStateBase * SMSmRunReports::PSButton8(void)
{
    return SMSmRunReportsBase::PSButton8();
}

long SMSmRunReports::OpenPrintFile(void)
{
    return SMSmRunReportsBase::OpenPrintFile();
}

SMStateBase * SMSmRunReports::PSReceiptUp(void)
{
    return SMSmRunReportsBase::PSReceiptUp();
}

SMStateBase * SMSmRunReports::PSReceiptDown(void)
{
    return SMSmRunReportsBase::PSReceiptDown();
}
