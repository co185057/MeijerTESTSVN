//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTakeChange.cpp
//
// TITLE: Class impliementation file for Take Change State
//
// This state represents a successful transaction paid for in cash with
// change due back.
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMTakeChange.h"
#include "transbrokerssf.h"
#include "SMTakeReceipt.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("TakeChange")

DLLIMPORT extern HWND hw;

DEFINE_TIMESTAMP

IMPLEMENT_DYNCREATE(SMTakeChange, CObject)// MFC Runtime class/object information

////////////////////////////
SMTakeChange::SMTakeChange(const bool fEOT)
: SMTakeChangeBase(fEOT)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMTakeChange::SMTakeChange()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMTakeChange::Deliver(const bool fEOT)
{
  SMTakeChangeBase::Deliver(fEOT);

  return this;
}

SMStateBase  *SMTakeChange::Initialize(void)
{
    trace(L7, _T("SMTakeChange::Initialize"));    
	trace(L6, _T("SSCOI-31860 Takechange initialize value SMState::m_bTakeChangeTimerExpired = %d"), SMState::m_bTakeChangeTimerExpired);
    //+ SSCOI-31860
    if(SMState::m_bTakeChangeTimerExpired)
    {
        trace(L6, _T("SSCOI-31860 going to timedout 2nd time"));
		SMState::m_bTakeChangeTimerExpired = false;
		m_fBillsDispensed = true;
		return TimedOut();
    }
    //- SSCOI-31860
	
	if ( (GetDMCashTakenInSecState()) && (GetChangeDispensedState()))
	{
		trace(L7, _T("SMTakeChange::GetDMCashTakenInSecState is true"));
        SetDMCashTakenInSecState(false);
		
        // SOTF 8696 - Only goes to TakeReceipt if this is the end of trx. - RRS
        trace(L7, _T("Is it End Of Transaction (%d), AnchorState (%d)"), fEndOfTrans, getAnchorState() );
        if( fEndOfTrans )
        {
            RETURNSTATE(TakeReceipt)
        }
        else
        {
            // if it's not the end of trx, then just go back to the anchor state.
            SetChangeDispensedState(false);
            return createAnchorState();
        }
        // E - SOTF 8696 - RRS
    }else
    {
        SetDMCashTakenInSecState(false);
        SetChangeDispensedState(false);
    }
	
	
	if( GetTBProperty(_T("NewFLState")) != _T("SMTakeChange") )
	{
		SetTBProperty(_T("NewFLState"),_T("SMTakeChange"));
		trace(L7, _T("SMTakeChange::Initialize   - SetTBNewState"));
	}
	
	//PW500001 22May2006 Recommended by Steve Miller.  Coupon sensor appears to be reseting the 
	//                   timeout counter.  Therefore we sometimes become stuck in this state.
	SMStateBase::bDoNotResetTimeOutOnDMEvent = true;
	SMStateBase* cReturnState = SMTakeChangeBase::Initialize();
	
	//Meijer does not use the base FL handling for take change timer
	//They use a RapDataNeeded to alert the cashier that change has been left
	//killing the timer so that it does not fire
	KillTimer(hw,RAP_ESCALATION);
	
	// +TAR 360301, 360302
	if ((TrainingModeObj.IsTMOn() && !co.nOperationsAllowDispenseInTrainingMode)
		|| saveChangeDue == 0)
	{
		delete getRealStatePointer( cReturnState); 
		return STATE_NULL;
	}
	// +TAR 357567
	// if(fIs3xHardware && (!dm.m_csCashRecyclerModel.IsEmpty() || WereBillsDispensed()))
	if(!dm.m_csCashRecyclerModel.IsEmpty() || WereBillsDispensed())  // Remove checking for 3xHardware
	{
		m_fBillsDispensed = true;	
	}
	if(m_fBillsDispensed)
    {
		nTimeOut = co.getTimeOutTakeCash();
        SetChangeDispensedState(true);
    }
	else
		nTimeOut = co.getTimeOutTakeCoins();
	// -TAR 357567
	
	return cReturnState;
}

void SMTakeChange::UnInitialize(void)
{
    SMTakeChangeBase::UnInitialize();
}

bool SMTakeChange::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    return SMTakeChangeBase::DeliverDescriptorList(dlParameterList);
}

SMStateBase  *SMTakeChange::DMCashRemoved(void)
{
    bool bDispense = true;
	
    SetDMCashTakenInSecState(false);
    SetChangeDispensedState(false);
	
	if (true == m_bCancellingCashTender) 
    {
		nTenderType = TB_TENDER_NONE;
        m_bCancellingCashTender = false; // resets the global variable
    } 
	
	if (TrainingModeObj.IsTMOn() && !co.nOperationsAllowDispenseInTrainingMode) // in training mode and option set to not dispense
	{
		bDispense = false;
	}
    if (bDispense)
	{
		DMSetCashTaken();
	}
	if (fEndOfTrans)
	{
		
		RETURNSTATE(TakeReceipt)	// end this transaction
	}
	else
	{
		return createAnchorState();		// remain in the transaction
	}
}

SMStateBase  *SMTakeChange::OnWtDecrease(void)
{
    return SMTakeChangeBase::OnWtDecrease();
}

SMStateBase  *SMTakeChange::OnWtIncreaseNotAllowed(void)
{
    return SMTakeChangeBase::OnWtIncreaseNotAllowed();
}

SMStateBase  *SMTakeChange::TimedOut(void)
{
    fTimedOutOnce = true;
	
	SetTimeoutInTakeChangeState(true);
	
	if (m_fBillsDispensed)
	{	//Determine config setting for DelayPrinting option
		CString csOptionName = _T("DelayPrinting");
		CString csOptDelayPrint;
		SendTBCommand(TBCMD_GET_OPTIONS, &csOptionName, &csOptDelayPrint);
		if( csOptDelayPrint == _T("Y") )
		{	// TAR 372377
			SendTBCommand(TBCMD_SET_TB_STATE,_T("TAKECHANGE_TIMER_EXPIRED"));
			DMSetCashTaken();
			return STATE_NULL;
			
		}
	}
	return DMCashRemoved();
}

SMStateBase  *SMTakeChange::OnMotionCashOutSensor(void)
{
    trace(L7, _T("+SMTakeChange::OnMotionCashOutSensor"));
	if(WereBillsDispensed())
	{
		SetChangeDispensedState(true);
		SMStateBase::fCashOutSensorBlocked = true;  
		trace(L7, _T("CashOutSensorBlocked<%d>"), SMStateBase::fCashOutSensorBlocked);
	}
	
	trace(L7, _T("SMTakeChange::OnMotionCashOutSensor"));
  return STATE_NULL;
}

SMStateBase  *SMTakeChange::OnNoMotionCashOutSensor(void)
{
    return SMTakeChangeBase::OnNoMotionCashOutSensor();
}

void SMTakeChange::Escalation2RAP(void)
{
    SMTakeChangeBase::Escalation2RAP();
}

SMStateBase *SMTakeChange::GPNotify(const GPEVT gpEvt)
{
    return SMTakeChangeBase::GPNotify(gpEvt);
}

SMStateBase  *SMTakeChange::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
    return SMTakeChangeBase::OnRAPClearSecurityBtn(lpszAuthorizationType);
}

SMStateBase  *SMTakeChange::PSButtonGoBack(void)
{
    return SMTakeChangeBase::PSButtonGoBack();
}

void SMTakeChange::showScreen(bool passwordVerified)
{
    COleVariant v;
    if (passwordVerified)
        return;
    
    if (inputMode == PROMPTING_FOR_OPERATOR)
    {
        STATE(SecOperatorPWState)::ShowEnterID();  //SR93 Biometrics support
        
        // Determine the leadthru text to show
        int iMessage = MSG_ENTERID_PRESSENTER;
        COleVariant v = VARIANT_TRUE;
        bool bEmplid = (SendTBCommand(TBCMD_IS_EMPLID_ENABLED) == TB_SUCCESS_IMMEDIATE);
        if( bEmplid )
        {
            bool bAllowHandKey = (SendTBCommand(TBCMD_IS_EMPLID_ALLOWHANDKEY) == TB_SUCCESS_IMMEDIATE);
            iMessage = bAllowHandKey ? MSG_ENTERID_SCANIDORKEY : MSG_ENTERID_SCANIDONLY;
            v = bAllowHandKey ? VARIANT_TRUE : VARIANT_FALSE;
			
			//(+)POS108118
            if(!bAllowHandKey)
            {
                CString csNewPosition;
                csNewPosition = _T("400,262,520,65");
                m_pAppPSX->SetConfigProperty(_T("LeadthruText"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);
				
                csNewPosition = _T("265,396,510,250");
                m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"), _T("EnterID"), UI::PROPERTYPOSITION, csNewPosition);
				
                m_pAppPSX->SetConfigProperty(_T("LeadthruText"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
                m_pAppPSX->SetConfigProperty(_T("MessageBoxNoEcho"),  _T("EnterID"),UI::PROPERTYTEXTALIGNMENT, _T("0"));
            }
			//(-)POS108118
        }
        
        m_pAppPSX->SetConfigProperty(_T("NumericP1"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP2"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP3"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("NumericP4"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
		
		//(+)POS108118
        m_pAppPSX->SetConfigProperty(_T("Echo"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
        m_pAppPSX->SetConfigProperty(_T("EchoField"), _T("EnterID"), UI::PROPERTYVISIBLE, v);
		//(-)POS108118
        
        ps.Message12(iMessage, false);
        // Leadthru text message set
        
        ps.AllowInput(co.GetnOperationsOperatorIDMaxDigit(),false,false);
        ps.ShowCMFrame(false);
        return;
    }
    else
    {  
        ps.SetCMFrame(_T("TakeChange"));
        v = VARIANT_TRUE;
        m_pAppPSX->SetConfigProperty(_T("ButtonStoreLogIn"), csCurContext, UI::PROPERTYVISIBLE, v);
    }
    
    ps.ShowCMTBText(csTBMessage2Scot);
    ps.ShowCMFrame(false);
}

SMStateBase  *SMTakeChange::stateAfterPasswordVerify()
{
    return SMTakeChangeBase::stateAfterPasswordVerify();
}

// Begin Dynamic Bill Detection	(tar 357567)
bool WereBillsDispensed()
{
	bool bStatus = false;
    long lAmtDispensed = 0;
	
    CString sList = dm.CashChangerGetDispensedCashList();
	int nSemiColon = sList.Find(_T(';'));
	int nLength = sList.GetLength();
	
	if ((nSemiColon >= 0) && ((nLength - 1) > nSemiColon))
	{
		sList += _T(",");		 // append one last comma so the loop below finds the last pair
		CString csWorkingList = sList.Right((nLength) - nSemiColon);
		while (true)
		{
			int i = csWorkingList.Find(_T(','));
			if (i<0) break;
			CString csTrimmedList = csWorkingList.Left(i);  //everything left of the 1st comma
			csWorkingList = csWorkingList.Mid(i+1);	  //reset the working list to exclude the group just processed
			i = csTrimmedList.Find(_T(':'));						
			if (i>0)					//if there's no colon, the list is finished
			{
				int lVal = _ttoi(csTrimmedList.Left(i));	  
				int lNum = _ttoi(csTrimmedList.Mid(i+1));
				lAmtDispensed += (lVal * lNum);	//multiply to find the monetary amt
			}
		}
		
		if (lAmtDispensed > 0)
		{
			bStatus = true;
		}
	}
	return bStatus;
}
// End of Dynamic Bill Detection
