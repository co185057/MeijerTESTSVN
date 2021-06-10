//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectContainerBase.cpp
//
// TITLE: Class implementation for Select Container state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "SMSelectContainer.h"  

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSelectTare.h"           // Select Tare
#include "SMRemoveItemFromProduceScale.h"


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SelectContainerBase")
#endif // _CPPUNIT


IMPLEMENT_DYNCREATE(SMSelectContainerBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SelectContainer)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSelectContainerBase::SMSelectContainerBase()
{
#ifndef _CPPUNIT
  IMPLEMENT_TIMESTAMP
#endif // _CPPUNIT
}
////////////////////////////////////
SMStateBase *SMSelectContainerBase::Initialize(void)
{
  CString csCurContext;
#ifndef _CPPUNIT
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SelectContainer") );	// RFC 330050
  //Update RAP window with SCOT state
  UpdateStateName(ps.GetPSText(LTT_SELECTCONTAINER, SCOT_LANGUAGE_PRIMARY));
  DMSayPhrase(SELECTCONTAINER);
  setAnchorState(BES_SELECTCONTAINER);

  int length1 = _tcsclen(ps.GetPSText(BTT_TARE1));
  int length2 = _tcsclen(ps.GetPSText(BTT_TARE2));
  if((0 == length1) && (0 == length2))
  {
      csCurContext = _T("SelectContainerWith2Tare");
  }
  else if((0 == length1) && (0 < length2))
  {
      csCurContext = _T("SelectContainerWith3TareB");
  }
  else if((0 < length1) && (0 == length2))
  {
      csCurContext = _T("SelectContainerWith3TareA");
  }
  else
  {
      csCurContext = _T("SelectContainer");
  }
  ps.SetCMFrame(csCurContext);
  
  ps.SetButtonText(_T("CMButton1MidiList"), BTT_TARE1);
  ps.SetButtonText(_T("CMButton4MidiList"), BTT_TARE2);
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  CString csWork;
  csWork.Format(ps.GetPSText(MSG_SELECTCONTAINER),
                io.d.csDescription);
#else
  csCurContext = _T("SelectContainer");
#endif // _CPPUNIT
  COleVariant v;
  if(g_bInRecallMode)
  {
      v=VARIANT_FALSE;
      ps.CMButton(_T("ButtonHelp"),BTT_HELP,false); 
  }
  else
  {
      v=VARIANT_TRUE;
  }
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYVISIBLE, v);
#ifndef _CPPUNIT	
  ps.Message12(csWork, true);
  ps.ShowCMFrame();
#endif // _CPPUNIT
  return STATE_NULL;			  // stay in attract state
}


///////////////////////////////////
SMStateBase *SMSelectContainerBase::PSButton1(void)// tare choice 1
{
#ifndef _CPPUNIT
  io.lTareCode = ps.GetTareAt(co.nOperationsTareContainer1);
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectContainer"), UI::PROPERTYSTATE, (long)UI::Disabled); //tar 330491
#endif // _CPPUNIT
  if(g_bInRecallMode)
  {
      RETURNSTATE(RemoveItemFromProduceScale); 
  }
  return TBEnterItem(false);
}
#ifndef _CPPUNIT
///////////////////////////////////
SMStateBase *SMSelectContainerBase::PSButton2(void)// other tare containers
{
  RETURNSTATE(SelectTare)
}
#endif // _CPPUNIT

///////////////////////////////////
SMStateBase *SMSelectContainerBase::PSButton3(void) // no container
{
#ifndef _CPPUNIT
  io.lTareCode = -1;
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectContainer"), UI::PROPERTYSTATE, (long)UI::Disabled); //tar 330491
#endif // _CPPUNIT
  if(g_bInRecallMode)
  {
      RETURNSTATE(RemoveItemFromProduceScale); 
  }
  return TBEnterItem(false);
}

///////////////////////////////////
SMStateBase *SMSelectContainerBase::PSButton4(void) // tare choice 2
{
#ifndef _CPPUNIT
  io.lTareCode = ps.GetTareAt(co.nOperationsTareContainer2);
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), _T("SelectContainer"), UI::PROPERTYSTATE, (long)UI::Disabled); //tar 330491
#endif // _CPPUNIT

  if(g_bInRecallMode)
  {
      RETURNSTATE(RemoveItemFromProduceScale); 
  }
  return TBEnterItem(false);
}
#ifndef _CPPUNIT
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
#endif // _CPPUNIT
