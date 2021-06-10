//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPLAEnterWeightBase.CPP
//
// TITLE: Class implementation for enter weight state for VegiCam
//
//
// AUTHOR:    David Moore
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifdef _CPPUNIT
    #include "FakeCommon.h"
#endif


#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#endif //CPPUNIT
#include "SMPLAEnterWeight.h"          // MGV this state
#ifndef _CPPUNIT
#include "SMScaleBroken.h"          // Reset scale
#include "SMProduceFavorites.h"

#include "DMProcedures.h"
DLLEXPORT extern DMProcedures dm;   // Global DM object
#endif //CPPUNIT
#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("PLAEnterWeightBase")

IMPLEMENT_DYNCREATE(SMPLAEnterWeightBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

SMPLAEnterWeightBase *SMPLAEnterWeightBase::m_pEnterWeight = NULL;

//////////////////////////////////////////
SMPLAEnterWeightBase::SMPLAEnterWeightBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
  m_pEnterWeight = this;
}

///////////////////////////////////////
SMStateBase  *SMPLAEnterWeightBase::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("PLAEnterWeight") );


  ra.RequestEnableAssistMode(false); 

  //Update RAP window with SCOT state

  UpdateStateName(ps.GetPSText(LTT_PUTONSCALE, SCOT_LANGUAGE_PRIMARY));
  bIsScaleBroken = false;

  if(!io.bBizerbaScale)
	DMScaleEnable();
	
  bool weightchanged = fDMScaleChanged;
  bool weightChangeDetected = false;//SR814.2
  //if there is something on the scale now, use that weight -- but
  // only if we know item on the scale has changed
  fDMScaleChanged = 0;
  if (lDMScaleWeight > 0 && weightchanged)
  {
       weightChangeDetected = true;
  }
  

  if ((!io.bBizerbaScale)&&(lDMScaleWeight <= 0)) 
  {
	DMSayPhrase(PLACEITEMONSCALE);
  }
 


  ps.SetCMFrame(_T("PLAEnterWeight"));
   #ifndef _CPPUNIT
  if (dm.fStateTakeawayBelt)
  {
	ps.SetCMBackground(BMP_PUTONSCALE_BELT);
  }
  else if(!io.bBizerbaScale)
  {
	ps.SetCMBackground(BMP_PUTONSCALE);
  }
  else
  {
	ps.SetCMBackground(BMP_BIZERBAPUTONSCALE);
  }

  #endif //CPPUNIT 
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid); 
  ps.ShowCMTBText(csTBMessage2Scot);
  csCurContext = _T("PLAEnterWeight");
  COleVariant v;
  ps.CMButton(_T("ButtonHelp"),BTT_HELP,false);

  if(!io.bBizerbaScale)
  {
	v=VARIANT_TRUE;
  }
  else
  {
  	v=VARIANT_FALSE;
	ps.CMButton(_T("ButtonHelp"),BTT_HELP,false); 
    nTimeOut = 0;  //No time out
  }
  m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYVISIBLE, v);

  CString csWork;
  csWork = _T("");
    csWork += _T("\n") + ps.GetPSText(MSG_ENTERWEIGHT);
    if (!WaitForScaleRequired(weightChangeDetected) && !m_bWaitForPicklist) {
        return ProcessWeightOnScale();
    }
  if (!m_bIsPLALookUpTransition)
  {
      m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
  }
  else
  {
      if (m_bWaitForPicklist)
      {
        m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
      }
      else
      {
        m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
      }
  }

  ps.Message12(csWork, true);
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMFrame();

  return STATE_NULL;
}

SMStateBase  *SMPLAEnterWeightBase::PSButtonGoBack(void) // cancel
{
  return PSButton3();    // previous state
}

SMStateBase *SMPLAEnterWeightBase::ProcessWeightOnScale(void)
{
    io.lWeightEntered = lDMScaleWeight;
  
	if(co.fStateAcceptWeightBeepEnable)
	{
		Beep(500, 500);
	}
	
    trace(L6,_T("Scale weight accepted= %d"),lDMScaleWeight);
    fDMScaleChanged = 0;
    g_csPLAItemList = _T("");
    nTimeOut = 10;

    if (m_bIsPLALookUpTransition)
    {
        m_bWaitForPicklist = true;

        SendPLAStartPickList();

        StartNewPLAResponseTimer();
        m_pAppPSX->SetConfigProperty(_T("ButtonGoBack"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("CMButton1Med"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);

        return STATE_NULL;
    }
    else
    {
        return TBItemSale(false,0,false,true);
    }
}

bool SMPLAEnterWeightBase::WaitForScaleRequired(bool bWeightChange)
{
  if(bWeightChange || lDMScaleWeight>0)
  {
    return false;
	
  }

  return true;
}

SMStateBase  *SMPLAEnterWeightBase::DMScale(void) 
{
  // let the base prime the 'virtual' scale

    SMStateBase  *sExit =  STATE(State)::DMScale();

	if (sExit!=STATE_NULL)
		return sExit;
	
  if(!WaitForScaleRequired() && !m_bWaitForPicklist)
  {
	return ProcessWeightOnScale();
  }
	
  if (lDMScaleWeight>=0)
  {
    return STATE_NULL;	//wait for timeout until good weight accepted	
  }
  else
  {
    setAnchorState(BES_SCANANDBAG);
    RETURNSTATE(ScaleBroken)
  }
  
}

SMStateBase  *SMPLAEnterWeightBase::PSButton1(void)
{
    g_csPLAItemList = _T("");
    m_bWaitForPicklist = false;
    RETURNSTATE(ProduceFavorites);
}

SMStateBase  *SMPLAEnterWeightBase::PSButton3(void) // cancel
{
    return setAndCreateAnchorState(BES_SCANANDBAG); 

}

SMStateBase  *SMPLAEnterWeightBase::PSCancelKey(void) // cancel
{
  DMSayPhrase(KEYPADVALID);
  return PSButton3();
}

void SMPLAEnterWeightBase::UnInitialize(void)
{
    m_bIsPLALookUpTransition = false;
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("PLAEnterWeight") );
}

SMStateBase  *SMPLAEnterWeightBase::TimedOut(void)
{
    if (m_bIsPLALookUpTransition)
    {
        g_csPLAItemList = _T("");
        m_bWaitForPicklist = false;
        RETURNSTATE(ProduceFavorites);
    } else
    {
        return PSButton3();
    }
}

SMStateBase *SMPLAEnterWeightBase::OnPLATransition()
{
    g_csPLAItemList = _T("");
    m_bWaitForPicklist = false;
    RETURNSTATE(ProduceFavorites);
}