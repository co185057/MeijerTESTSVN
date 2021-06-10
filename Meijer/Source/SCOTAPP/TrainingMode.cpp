//////////////////////////////////////////////////////////////////////
//
// TrainingMode.cpp: implementation of the CSCOTTrainingMode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
#include "TrainingMode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define COMP_ID ID_SM
#define T_ID	_T("TrainingMode")


CSCOTTrainingMode::CSCOTTrainingMode()
{
	
	m_nTMCurrentState = SCOTTM_NOTALLOWED;
	m_bTransApproved = false;
}


CSCOTTrainingMode::~CSCOTTrainingMode()
{
	// nothing
}

bool CSCOTTrainingMode::IsAllowed()
{
	bool bRetState = false;
	if ((tb.sm_lpfnSetTrainingModeState) && (tb.IsAllowed(TB_FT_TRAININGMODE)))
	{
		bRetState = true;
	}
	else
	{
		if (IsTMOn())
		{
			// red alert: **** while in training mode, how can TB doesn't allow training mode!!
			ASSERT(0);
			// log this TB error and but allow SCOT App to leave training mode
			ScotError(RETURN, SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_TB_NOTALLOWED_INTRAINING); 
			// TB can either send TB_LEFTTRAININGMODE or TB_VISUAL_VERIFY state
			bRetState = true;
		}
	}
	return bRetState;
}

bool CSCOTTrainingMode::IsTMOn()
{
	bool bRetState = false;
	if (GetCurrentTMState() == SCOTTM_ON)
	{
		bRetState = true;
	}
	return bRetState;

}

long CSCOTTrainingMode::TurnOn()
{
	SetTMState(SCOTTM_ON);
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(MSG_ENTER_TRAINING_MODE, SCOT_LANGUAGE_PRIMARY));
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + ps.GetPSText(MSG_ENTER_TRAINING_MODE, SCOT_LANGUAGE_PRIMARY)); // inform RAP about entering/leaving training mode
	trace(L7, _T("-ra.OnNormalItem"));
	ps.ShowTrainingModeOnScreen(true);
	return 0;
}

long CSCOTTrainingMode::TurnOff()
{
	SetTMState(SCOTTM_OFF);
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(MSG_LEAVE_TRAINING_MODE, SCOT_LANGUAGE_PRIMARY));
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + ps.GetPSText(MSG_LEAVE_TRAINING_MODE, SCOT_LANGUAGE_PRIMARY)); // inform RAP about entering/leaving training mode
	trace(L7, _T("-ra.OnNormalItem"));
	ps.ShowTrainingModeOnScreen(false);
	return 0;
}

inline void CSCOTTrainingMode::SetTMState(TRAININGMODE_STATE nTrainingState)
{
	ASSERT(IsAllowed());
	m_nTMCurrentState = nTrainingState;
}

inline TRAININGMODE_STATE CSCOTTrainingMode::GetCurrentTMState()
{
	return m_nTMCurrentState;
}

//Has the training mode intervention been approved yet?
bool CSCOTTrainingMode::IsTransactionApproved()
{
	//Always return true if Intervention not required.
	bool bRetValue = true;
	if (co.fOperationsInterventionAtStartOfTrainingModeTransaction)
	{
		bRetValue = m_bTransApproved;
	}
	return bRetValue;
}

void CSCOTTrainingMode::SetTransactionApproved(bool bApproved)
{
	m_bTransApproved = bApproved;
}
