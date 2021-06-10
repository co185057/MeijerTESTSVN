//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectContainerBase.cpp
//
// TITLE: Class implementation for Select Container state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSelectContainer.h"  
#include "SMSelectTare.h"           // Select Tare

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SelectContainerBase")

IMPLEMENT_DYNCREATE(SMSelectContainerBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SelectContainer)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectContainerBase::SMSelectContainerBase()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////
SMStateBase *SMSelectContainerBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SelectContainer") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_SELECTCONTAINER, SCOT_LANGUAGE_PRIMARY));
  DMSayPhrase(SELECTCONTAINER);
  setAnchorState(BES_SELECTCONTAINER);
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectContainer"), UI::PROPERTYVISIBLE, VARIANT_TRUE); //tar 330491

  int length1 = _tcsclen(ps.GetPSText(BTT_TARE1));
  int length2 = _tcsclen(ps.GetPSText(BTT_TARE2));
  if((0 == length1) && (0 == length2))
	  ps.SetCMFrame(_T("SelectContainerWith2Tare"));
  else if((0 == length1) && (0 < length2))
	  ps.SetCMFrame(_T("SelectContainerWith3TareB"));
  else if((0 < length1) && (0 == length2))
	  ps.SetCMFrame(_T("SelectContainerWith3TareA"));
  else
	  ps.SetCMFrame(_T("SelectContainer"));	  
  
  ps.SetButtonText(_T("CMButton1MidiList"), BTT_TARE1);
  ps.SetButtonText(_T("CMButton4MidiList"), BTT_TARE2);
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  CString csWork;
  csWork.Format(ps.GetPSText(MSG_SELECTCONTAINER),
                io.d.csDescription);
	
  ps.Message12(csWork, true);
  ps.ShowCMFrame();
  return STATE_NULL;			  // stay in attract state
}

///////////////////////////////////
SMStateBase *SMSelectContainerBase::PSButton1(void)// tare choice 1
{
  io.lTareCode = ps.GetTareAt(co.nOperationsTareContainer1);
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectContainer"), UI::PROPERTYVISIBLE, VARIANT_FALSE); //tar 330491
  return TBEnterItem(false);
}

///////////////////////////////////
SMStateBase *SMSelectContainerBase::PSButton2(void)// other tare containers
{
  RETURNSTATE(SelectTare)
}

///////////////////////////////////
SMStateBase *SMSelectContainerBase::PSButton3(void) // no container
{
  io.lTareCode = -1;
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectContainer"), UI::PROPERTYVISIBLE, VARIANT_FALSE); //tar 330491
  return TBEnterItem(false);
}

///////////////////////////////////
SMStateBase *SMSelectContainerBase::PSButton4(void) // tare choice 2
{
  io.lTareCode = ps.GetTareAt(co.nOperationsTareContainer2);
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectContainer"), UI::PROPERTYVISIBLE, VARIANT_FALSE); //tar 330491
  return TBEnterItem(false);
}

///////////////////////////////////
SMStateBase *SMSelectContainerBase::PSButtonGoBack(void) // no container
{
  io.lTareCode = -1;
  //Tar 120648
  SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
  if (fInMultiSelectPickList)
  {
	  g_lNumberOfItemsNotBagInMultiPick--;
	  return setAndCreateAnchorState( BES_PRODUCEFAVORITES );
  }
  else
  {
	  // TAR#116940	LPM081899 return to scan and bag, not select from list	//return TBEnterItem(false);
	  return setAndCreateAnchorState(BES_SCANANDBAG);     // goto back to same item entry
  }
}

SMStateBase  *SMSelectContainerBase::PSCancelKey(void)	// override where needed
{
  DMSayPhrase(KEYPADVALID);
  return PSButtonGoBack();
}
// + RFC 330050
void SMSelectContainerBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SelectContainer") );	
}
// - RFC 330050
