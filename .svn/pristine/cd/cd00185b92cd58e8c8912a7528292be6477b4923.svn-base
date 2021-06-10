//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSystemFunctions.CPP
//
// TITLE: Class implementation for store mode System Functions state
//
// AUTHOR:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMSmSystemFunctions.h"
#include "transbrokerssf.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmSystemFunctions")


IMPLEMENT_DYNCREATE(SMSmSystemFunctions, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

SMStateBase* pStartState = NULL;
//////////////////////////////////////////
SMSmSystemFunctions::SMSmSystemFunctions()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmSystemFunctions::Initialize(void)
{
    SMStateBase  *retState= SMSmSystemFunctionsBase::Initialize();
	if(!_tcsicmp(co.csHWType, _T("SS90")))
	{
		m_pAppPSX->SetConfigProperty( _T("SMButton2"), _T("SmSystemFunctions"), UI::PROPERTYSTATE, (long)UI::Disabled );
		m_pAppPSX->SetConfigProperty( _T("SMButton5"), _T("SmSystemFunctions"), UI::PROPERTYSTATE, (long)UI::Disabled );
	}
	return retState;
}

void SMSmSystemFunctions::UnInitialize(void)
{
    SMSmSystemFunctionsBase::UnInitialize();
}

SMStateBase * SMSmSystemFunctions::PSButton1(void)
{
    return SMSmSystemFunctionsBase::PSButton1();
}

SMStateBase * SMSmSystemFunctions::PSButton2(void)
{
    return SMSmSystemFunctionsBase::PSButton2();
}

SMStateBase * SMSmSystemFunctions::PSButton3(void)
{
    return SMSmSystemFunctionsBase::PSButton3();
}

SMStateBase * SMSmSystemFunctions::PSButton4(void)
{
    if (fInSignedOn || fInSignedOff || fSAInTransaction)
    {
        // You are in the middle of a transaction, initialize all at the end of it
        trace(L0,_T("Options load requested"));
        fDoOptions = true;
        ps.Prompt(OPTIONS_LOAD);
        ps.Echo(BTT_OK);
        return STATE_NULL;
    }
    else
    {
        // outside of a transaction, no active transaction in progress
        // do initialize of all right now
        // fDoOptions = false;
        // ps.Prompt(OPTIONS_LOAD);
        // CREATE_AND_DISPENSE(OutOfService)(2);
		SetHookProperty(_T("IsPOSReloadOptionDone"), _T("0"));
		trace(L0,_T("Options reload started..."));
        DisableButtons();  // TAR 313180   
        m_bReloadingOptions = true;
		pStartState = NULL;
        pStartState = ReloadOptions();
		CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
		trace(L6, _T("Posstate=<%s>"), csPOSState);
		if(GetHookProperty(_T("IsPOSReloadOptionDone")) == _T("1") && csPOSState.Find(_T("ITEMMENU_DK")) != -1)
		{
			trace(L0,_T("Options reload finished"));
			m_bReloadingOptions = false;
			ReenableButtons(); // TAR 313180
			if (pStartState)
			{
				return pStartState;
			}
			else
			{
				trace(L0,_T("Options load returning state restart"));
				ASSERT(0);
				return STATE_RESTART;
			}
		}
		else
		{
			//option still loading. Keep the buttons disable and not let operator exit system functions
			trace(L0,_T("POS is still reloading options..."));
			ps.Echo(_T("POS Reloading..."));
			return STATE_NULL; //stay in the same state
		}
    }
}

SMStateBase * SMSmSystemFunctions::PSButton5(void)
{
    return SMSmSystemFunctionsBase::PSButton5();
}

SMStateBase * SMSmSystemFunctions::PSButton6(void)
{
    return SMSmSystemFunctionsBase::PSButton6();
}

SMStateBase * SMSmSystemFunctions::PSButton7(void)
{
    return SMSmSystemFunctionsBase::PSButton7();
}

SMStateBase * SMSmSystemFunctions::PSButton8(void)
{
    return SMSmSystemFunctionsBase::PSButton8();
}

SMStateBase * SMSmSystemFunctions::PSNumericKey(const long lKey)
{
    return SMSmSystemFunctionsBase::PSNumericKey(lKey);
}

SMStateBase * SMSmSystemFunctions::DMEncryptorCompleted(void)
{
    return SMSmSystemFunctionsBase::DMEncryptorCompleted();
}

SMStateBase * SMSmSystemFunctions::PSReceiptUp(void)
{
    return SMSmSystemFunctionsBase::PSReceiptUp();
}

SMStateBase * SMSmSystemFunctions::PSReceiptDown(void)
{
    return SMSmSystemFunctionsBase::PSReceiptDown();
}

void SMSmSystemFunctions::showScreen(const bool traceOn)
{
    SMSmSystemFunctionsBase::showScreen(traceOn);
    
    CString strOptionName = _T("TrainingMode");
    CString strTrngMode;
    SendTBCommand(TBCMD_GET_OPTIONS, &strOptionName, &strTrngMode);
    if(strTrngMode == _T("N"))
        ps.Button(_T("SMButton3"), MSG_ENTER_TRAINING_MODE, false);
	
    bool bEmplid = (SendTBCommand(TBCMD_IS_EMPLID_ENABLED) == TB_SUCCESS_IMMEDIATE);
    if( bEmplid )
    {
        ps.Button(_T("SMButton1"),BTT_BARCODE,false);
    }
}

long SMSmSystemFunctions::CopyLogsToServer(void)
{
    return SMSmSystemFunctionsBase::CopyLogsToServer();
}

bool SMSmSystemFunctions::inEncryptorMode(void)
{
    return SMSmSystemFunctionsBase::inEncryptorMode();
}

void SMSmSystemFunctions::DisableButtons(void)
{
    SMSmSystemFunctionsBase::DisableButtons();
}

void SMSmSystemFunctions::ReenableButtons(void)
{
    SMSmSystemFunctionsBase::ReenableButtons();
}

SMStateBase * SMSmSystemFunctions::OnGoBackButton(void)
{
    return SMSmSystemFunctionsBase::OnGoBackButton();
}

void SMSmSystemFunctions::ShowPrintBarcodeButton()
{
    SMSmSystemFunctionsBase::ShowPrintBarcodeButton();
}

void SMSmSystemFunctions::ShowSecurityMaintButton()
{
    SMSmSystemFunctionsBase::ShowSecurityMaintButton();
}

void SMSmSystemFunctions::ShowModeOfOperationButton()
{
    SMSmSystemFunctionsBase::ShowModeOfOperationButton();
}

void SMSmSystemFunctions::ShowLoadOptionsButton()
{
    SMSmSystemFunctionsBase::ShowLoadOptionsButton();
}

void SMSmSystemFunctions::ShowUtilityButton()
{
    SMSmSystemFunctionsBase::ShowUtilityButton();
}

SMStateBase * SMSmSystemFunctions::IdleTimer(const long lMsec)
{
	trace(L6, _T("Idle timer in system functions"));
	if(m_bReloadingOptions) //option reload in progress
	{
		CString csPOSState = GetTBProperty(_T("SavedLastPosState"));
		trace(L6, _T("Posstate=<%s>"), csPOSState);
		if(GetHookProperty(_T("IsPOSReloadOptionDone")) == _T("1") && csPOSState.Find(_T("ITEMMENU_DK")) != -1) //
		{
			//option reload finished. Enable the buttons to let operator exit system functions
			ps.Echo(_T(""));
			trace(L0,_T("POS options reload finished"));
			m_bReloadingOptions = false;
			ReenableButtons(); // TAR 313180
			if (pStartState)
			{
				return pStartState;
			}
			else
			{
				return STATE_NULL;
			}
		}
		else
		{
			//options still loading. Keep the buttons disable and not let operator exit system functions
			trace(L0,_T("POS is still reloading options..."));
			ps.Echo(_T("POS Reloading..."));
			return STATE_NULL; //stay in the same state
		}
	}
    return SMStateBase::IdleTimer(lMsec);
}
