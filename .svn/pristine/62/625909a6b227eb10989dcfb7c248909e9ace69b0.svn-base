//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCrateableItemBase.CPP
//
// TITLE: Class implementation for Crate-able Item state
//
// AUTHOR: Leila Parker
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMCrateableItem.h"      
#include "SMSelectContainer.h"      // Select a tare
#include "SMScaleBroken.h"          // Reset scale
#include "SMLookUpItem.h"           
#include "SMSelectTare.h"       // This state

#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("CrateableItemBase")

IMPLEMENT_DYNCREATE(SMCrateableItemBase, CObject)// MFC Runtime class/object information
//IMPLEMENT_STATE(CrateableItem)

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMCrateableItemBase::SMCrateableItemBase()
{
  IMPLEMENT_TIMESTAMP
}

///////////////////////////////////////
SMStateBase  *SMCrateableItemBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("CrateableItem") );
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(TTL_PURCHASECRATE, SCOT_LANGUAGE_PRIMARY));
  setAnchorState(BES_CRATEABLEITEM);  
  DMSayPhrase(PURCHASECRATE);

  ps.SetCMFrame(_T("CrateableItem"));

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  CString csWork;
  csWork.Format(ps.GetPSText(LTT_PURCHASEFULLCRATE), io.d.csDescription);
  ps.Message12(csWork,true);	
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid); //*** NewUI ***//
  ps.ShowCMFrame();
  return STATE_NULL;                // stay in CrateableItemBase state
}

//////////////////////////////////////
SMStateBase  *SMCrateableItemBase::PSButton1(void) // yes
{
  io.d.nCrateableScreenAnswer = CRATEABLE_ANSWER_YES;
  io.lQuantityEntered = FULL_CRATE_QTY;
  return TBEnterItem(true);
}

////////////////////////////////////
SMStateBase  *SMCrateableItemBase::PSButton2(void) // no
{
  io.d.nCrateableScreenAnswer = CRATEABLE_ANSWER_NO;
  return TBEnterItem(true);
}

////////////////////////////////////
void SMCrateableItemBase::UnInitialize()
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("CrateableItem") );
}
