//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmMediaStatus.CPP
//
// TITLE: Class implementation for store mode Media Status state
//
// AUTHOR: Jeff Connelly
//
//
// POS244229 Work Request:73594 Name:Aparna Tunuguntla Date: June 23, 2016
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmMediaStatus.h"
#include "LaneStateData.h"  // SSCOI-45254

#define COMP_ID ID_SM							  // base state component
#define T_ID _T("SmMediaStatus")


IMPLEMENT_DYNCREATE(SMSmMediaStatus, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//================================
// constructor
//================================
SMSmMediaStatus::SMSmMediaStatus()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmMediaStatus::Initialize(void)
{
    return SMSmMediaStatusBase::Initialize();
}

void SMSmMediaStatus::UnInitialize(void)
{
    SMSmMediaStatusBase::UnInitialize();
}

//(+) POS244229
SMStateBase * SMSmMediaStatus::PSButton1(void)
{
    SMStateBase  *pNextState = SMSmMediaStatusBase::PSButton1();
    if (Is5XHardware() || !_tcsicmp(co.csHWType, _T("SCOT6")))
    {
        trace(L6, _T("Cash Monitoring Counts for Addcash and Remove cash"));
        CashMonitoringCounts(0); //0 - not in transaction
    }

  return pNextState;
}
//(-) POS244229

SMStateBase * SMSmMediaStatus::PSButton2(void)
{
	// SSCOI-45254
	// Removing code below after corrected the migration error. This is a migration problem.
	//CLaneStateData laneStateData(m_pAppPSX);
    //laneStateData.SendNextGenUIData(_T("RemoteProxyUserInputCompleted"), true);
    return SMSmMediaStatusBase::PSButton2();
}

SMStateBase * SMSmMediaStatus::PSButton4(void)
{
    return SMSmMediaStatusBase::PSButton4();
}

SMStateBase * SMSmMediaStatus::PSButton5(void)
{
    return SMSmMediaStatusBase::PSButton5();
}

SMStateBase * SMSmMediaStatus::PSButton6(void)
{
    return SMSmMediaStatusBase::PSButton6();
}

SMStateBase * SMSmMediaStatus::PSButton7(void)
{
    return SMSmMediaStatusBase::PSButton7();
}

SMStateBase * SMSmMediaStatus::PSButton8(void)
{
    return SMSmMediaStatusBase::PSButton8();
}

SMStateBase * SMSmMediaStatus::DMCashChangerInOperable(void)
{
    return SMSmMediaStatusBase::DMCashChangerInOperable();
}

SMStateBase * SMSmMediaStatus::PSReceiptUp(void)
{
    return SMSmMediaStatusBase::PSReceiptUp();
}

SMStateBase * SMSmMediaStatus::PSReceiptDown(void)
{
    return SMSmMediaStatusBase::PSReceiptDown();
}

void SMSmMediaStatus::initializeHopperList()
{
    SMSmMediaStatusBase::initializeHopperList();
}

void SMSmMediaStatus::initializeCurrencyList()
{
    SMSmMediaStatusBase::initializeCurrencyList();
}

void SMSmMediaStatus::initializePrinterList()
{
    SMSmMediaStatusBase::initializePrinterList();
}

void SMSmMediaStatus::initializeCashAcceptorStatusLine()
{
    SMSmMediaStatusBase::initializeCashAcceptorStatusLine();
}

void SMSmMediaStatus::initializeCoinAcceptorStatusLine()
{
    SMSmMediaStatusBase::initializeCoinAcceptorStatusLine();
}

void SMSmMediaStatus::UpdateDeviceStatus()
{
    SMSmMediaStatusBase::UpdateDeviceStatus();
}

long SMSmMediaStatus::DMCheckHealth(long nDevClass)
{
    return SMSmMediaStatusBase::DMCheckHealth(nDevClass);
}

void SMSmMediaStatus::getHopperStatus(const CString denominationCode,
                               CString& denominationStatus)
{
    SMSmMediaStatusBase::getHopperStatus(denominationCode, denominationStatus);
}

CString SMSmMediaStatus::DisplayCoinDispenserStatus(void)
{
    return SMSmMediaStatusBase::DisplayCoinDispenserStatus();
}

void SMSmMediaStatus::InitializeCoinDispenserStatus(void)
{
    SMSmMediaStatusBase::InitializeCoinDispenserStatus();
}

void SMSmMediaStatus::initializeCardDispenserStatus(void)
{
    SMSmMediaStatusBase::initializeCardDispenserStatus();
}

SMStateBase * SMSmMediaStatus::ReloadCashDevices(void)
{
    return SMSmMediaStatusBase::ReloadCashDevices();
}

