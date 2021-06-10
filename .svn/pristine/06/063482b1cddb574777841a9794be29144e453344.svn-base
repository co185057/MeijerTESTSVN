//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMReportsMenu.CPP
//
// TITLE:	Class implementation for reports menu state.
//
//
// AUTHOR:	Tara Duncan 
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"			    // MGV base state
#include "SMSmReportsMenu.h"			// MGV this state

#include "transbrokerssf.h"
#include "SMWaitForApproval.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmReportsMenu")

#define PREVIOUS_RECEIPT_FILE _T("c:\\scot\\data\\PreviousMainReceipt.SLP")


IMPLEMENT_DYNCREATE(SMSmReportsMenu, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////
SMSmReportsMenu::SMSmReportsMenu()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmReportsMenu::Initialize(void)
{
    // + TAR 358892
	if (isRestrictedItemApprovalRequired())
	{
		CREATE_AND_DISPENSE(WaitForApproval)(false,false);
	}
	// - TAR 358892
	return SMSmReportsMenuBase::Initialize();
}

void SMSmReportsMenu::UnInitialize(void)
{
    SMSmReportsMenuBase::UnInitialize();
}

void SMSmReportsMenu::WriteRecord(CString csData)
{
    SMSmReportsMenuBase::WriteRecord(csData);
}

void SMSmReportsMenu::OpenFile(void)
{
    SMSmReportsMenuBase::OpenFile();
}

void SMSmReportsMenu::CloseFile(void)
{
    SMSmReportsMenuBase::CloseFile();
}

void SMSmReportsMenu::GoOffline(void)
{
    SMSmReportsMenuBase::GoOffline();
}

void SMSmReportsMenu::LogError(DWORD eventID, 
                               _bstr_t &functionName, 
                               WORD eventType, 
                               long errorCode,
                               long errorCode2,
                               BSTR Description)
{
    SMSmReportsMenuBase::LogError(eventID, functionName, eventType, errorCode, errorCode2, Description);
}

void SMSmReportsMenu::LogComError(_com_error &e, _bstr_t &functionName)
{
    SMSmReportsMenuBase::LogComError(e, functionName);
}

void SMSmReportsMenu::LogOLEError(COleDispatchException *e, _bstr_t &functionName)
{
    SMSmReportsMenuBase::LogOLEError(e, functionName);
}

SMStateBase * SMSmReportsMenu::PSButton1(void)
{
    return SMSmReportsMenuBase::PSButton1();
}

SMStateBase * SMSmReportsMenu::PSButton2(void)
{
	traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("Print Last Receipt") ) ;
	
	if (FileExist(PREVIOUS_RECEIPT_FILE))
	{
		PrintLastReceipt(PREVIOUS_RECEIPT_FILE);  //print receipt
	}
	return STATE_NULL;
}

SMStateBase * SMSmReportsMenu::PSButton3(void)
{
    CString strOptionName = _T("BypassButtonFuntionality");
    CString strOptionValue;
    SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strOptionValue);
	if(strOptionValue == _T("Y"))
		return STATE_NULL; 
	else return SMSmReportsMenuBase::PSButton3();
}

SMStateBase * SMSmReportsMenu::PSButton4(void)
{
    return SMSmReportsMenuBase::PSButton4();
}

SMStateBase * SMSmReportsMenu::PSButton7(void)
{
    return SMSmReportsMenuBase::PSButton7();
}

SMStateBase * SMSmReportsMenu::PSButton8(void)
{
    return SMSmReportsMenuBase::PSButton8();
}

SMStateBase * SMSmReportsMenu::OnRPReceived(long byteCount)
{
    return SMSmReportsMenuBase::OnRPReceived(byteCount);
}

SMStateBase * SMSmReportsMenu::OnRPDisconnect()
{
    return SMSmReportsMenuBase::OnRPDisconnect();
}

SMStateBase * SMSmReportsMenu::SendRPDisconnect()
{
    return SMSmReportsMenuBase::SendRPDisconnect();
}

SMStateBase * SMSmReportsMenu::SendRPConnect()
{
    return SMSmReportsMenuBase::SendRPConnect();
}

long SMSmReportsMenu::AddToReport(const CString& text, const CString &csOption)
{
    return SMSmReportsMenuBase::AddToReport(text, csOption);
}

void SMSmReportsMenu::ParseString(CString csInString)
{
    SMSmReportsMenuBase::ParseString(csInString);
}

void SMSmReportsMenu::ParseReportLine(CString &csReportLine)
{
    SMSmReportsMenuBase::ParseReportLine(csReportLine);
}

bool SMSmReportsMenu::ParsePrompt(CString &csReportLine, bool &bKeepGoing)
{
    return SMSmReportsMenuBase::ParsePrompt(csReportLine, bKeepGoing);
}

bool SMSmReportsMenu::ParseMenuLine(CString &csReportLine, bool &bKeepGoing)
{
    return SMSmReportsMenuBase::ParseMenuLine(csReportLine, bKeepGoing);
}

bool SMSmReportsMenu::ParseErrorLine(CString &csReportLine)
{
    return SMSmReportsMenuBase::ParseErrorLine(csReportLine);
}

bool SMSmReportsMenu::ParseStartReport(CString &csReportLine)
{
    return SMSmReportsMenuBase::ParseStartReport(csReportLine);
}

bool SMSmReportsMenu::SigReceiptFileFound()
{
    return SMSmReportsMenuBase::SigReceiptFileFound();
}

void SMSmReportsMenu::PrintSigReceiptFile()
{
    SMSmReportsMenuBase::PrintSigReceiptFile();
}

SMStateBase * SMSmReportsMenu::PSReceiptUp(void)
{
    return SMSmReportsMenuBase::PSReceiptUp();
}

SMStateBase * SMSmReportsMenu::PSReceiptDown(void)
{
    return SMSmReportsMenuBase::PSReceiptDown();
}

CString SMSmReportsMenu::HandleColumnarLine(const CString &csData)
{
    return SMSmReportsMenuBase::HandleColumnarLine(csData);
}

CString SMSmReportsMenu::HandleCenterLine(const CString &csData)
{
    return SMSmReportsMenuBase::HandleCenterLine(csData);
}

CString SMSmReportsMenu::HandleOther(const CString &csData)
{
    return SMSmReportsMenuBase::HandleOther(csData);
}
