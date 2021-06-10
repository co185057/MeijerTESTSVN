/////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagementEx.CPP
//
// TITLE: Class implementation for store mode CashManagement state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"
#include "SMSmCashManagementEx.h"
#include "transbrokerssf.h"
#include "CMInterface.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmCashManagementEx")

IMPLEMENT_DYNCREATE(SMSmCashManagementEx, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////
SMSmCashManagementEx::SMSmCashManagementEx()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmCashManagementEx::Initialize(void)
{
    return SMSmCashManagementExBase::Initialize();
}

void SMSmCashManagementEx::UnInitialize(void)
{
    SMSmCashManagementExBase::UnInitialize();
}

SMStateBase * SMSmCashManagementEx::DMAcceptor(void)
{
    return SMSmCashManagementExBase::DMAcceptor();
}

SMStateBase * SMSmCashManagementEx::PSParse( const MessageElement* me)
{	
    return SMSmCashManagementExBase::PSParse( me );
}

SMStateBase * SMSmCashManagementEx::PSButton8(void)
{
	SMStateBase  *pNextState = SMSmCashManagementExBase::PSButton8();
	if (Is5XHardware() || !_tcsicmp(co.csHWType, _T("SCOT6")))
    {		
		if(m_cmMgr.GetNotesTransferStatus())
		{
			trace(L6, _T("Cash Monitoring Counts for Transfer All Notes"));
			CashMonitoringCounts(2); // 2 - Notes transfer
			m_cmMgr.ResetNotesTransferStatus();
		}
		else
		{
			trace(L6, _T("Cash Monitoring Counts for Addcash and Remove cash"));
			CashMonitoringCounts(0); // 0 - Out of transaction
		}
    }
	return pNextState;
}

SMStateBase * SMSmCashManagementEx::PSEnterKey(void)
{
    return SMSmCashManagementExBase::PSEnterKey();
}

SMStateBase * SMSmCashManagementEx::DMParse( const MessageElement* me)
{
    return SMSmCashManagementExBase::DMParse(me);
}

long SMSmCashManagementEx::DMCashAcceptorAccept(void)
{
    return SMSmCashManagementExBase::DMCashAcceptorAccept();
}

SMStateBase * SMSmCashManagementEx::HandleScannerData( const MessageElement* message)
{
	// (+) POS309451 / SSCOI-47853
    trace(L6,_T("+SMState::HandleScannerData()"));
    SMStateBase * pRetState = STATE_NULL;
    CString sBarcodeRaw = cDMLastData;
    CString sBarcode = _T("");
    trace(L6, _T("ScannerData: %s"), sBarcodeRaw);
	    
    if( 
        (TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_IS_EMPLID_ENABLED)) &&
        dm.IsScannerEnabled() &&
        (OperPassStateName() == _T("SMOperatorPasswordState") || OperPassStateName() == _T("SMSecOperatorPWState") ) &&
        (TB_SUCCESS_IMMEDIATE == SendTBCommand(TBCMD_IS_MEIJER_BADGE_GET_BARCODE, &sBarcodeRaw, &sBarcode)) 
		)
    {  
      	DMScannerDisable();	
        if( ((SMOperatorPasswordState*)this)->IsPromptingForOperator() )
        {
            // We've received a badge scan process it, wedge the data into the user field and proceed.
            trace(L6, _T("Employee badge was detected, sending barcode to login screen: %s"), sBarcode);
            PSClearAllInput(true);
            for(int i = 0; i < sBarcode.GetLength(); i++)
            {
                PSAlphaNumericKey(sBarcode[i]);
            }
            pRetState = PSEnterKey();
        }
        else
        {
            trace(L0, _T("Employee badge scanned ignored: SCOTApp is not currently prompting for an operator entry."));
        }        
    }
    else
    {
        pRetState = SMSmCashManagementExBase::HandleScannerData(message);
    }
    trace(L6,_T("-SMState::HandleScannerData()"));
    return pRetState;
	// (-) POS309451 / SSCOI-47853
}

SMStateBase * SMSmCashManagementEx::PSHotKey( CString &csHotKey)
{
    return SMSmCashManagementExBase::PSHotKey( csHotKey);
}

SMStateBase * SMSmCashManagementEx::PSShiftKey(void)
{
    return SMSmCashManagementExBase::PSShiftKey();
}

SMStateBase * SMSmCashManagementEx::createLevel1Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMSmCashManagementExBase::createLevel1Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMSmCashManagementEx::createLevel2Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth,
                                           bool displayHTML)
{
    return SMSmCashManagementExBase::createLevel2Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
}

SMStateBase * SMSmCashManagementEx::createLevel3Message( LPCTSTR szMsg,
                                           PSTEXT pstMsg,
                                           PSTEXT prompt,
                                           long   value,
                                           long   device,
                                           bool   checkHealth, 
                                           bool displayHTML )
{
    return SMSmCashManagementExBase::createLevel3Message( szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML );
}

