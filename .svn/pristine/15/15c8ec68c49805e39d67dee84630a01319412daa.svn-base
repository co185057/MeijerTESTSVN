//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPLAHandInViewBase.CPP
//
// TITLE: Class implementation for enter weight state for VegiCam
//
//
// AUTHOR:    Charl Subingsubing
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header


#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "MPProcedures.h"
//#include "SMScaleBroken.h"          // Reset scale
#include "SMProduceFavorites.h"
#include "SMBagAndEAS.h"
#include "SMScanAndBag.h"
#endif //_CPPUNIT
#include "SMPLAHandInView.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("PLAHandInViewBase")

IMPLEMENT_DYNCREATE(SMPLAHandInViewBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP



//////////////////////////////////////////
SMPLAHandInViewBase::SMPLAHandInViewBase()
{
    IMPLEMENT_TIMESTAMP
}

SMStateBase  *SMPLAHandInViewBase::Deliver(const BEST argPrevState)
{
  m_PrevState = argPrevState;
  return this;
}

///////////////////////////////////////
SMStateBase  *SMPLAHandInViewBase::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("PLAHandInView") );


  ra.RequestEnableAssistMode(false); 

  //Update RAP window with SCOT state


  UpdateStateName(ps.GetPSText(LTT_HANDINVIEW, SCOT_LANGUAGE_PRIMARY));

  //Todo: update dmsay
    DMSayPhrase(PLACEITEMONSCALE);

 


  ps.SetCMFrame(_T("PLAHandInView"));

  ps.ShowCMTBText(csTBMessage2Scot);
 

  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);
  ps.ShowCMFrame();

  return STATE_NULL;
}

void SMPLAHandInViewBase::UnInitialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("PLAHandInView") );
}

SMStateBase  *SMPLAHandInViewBase::TimedOut(void)
{
    return OnPLATransition();
}

SMStateBase *SMPLAHandInViewBase::OnPLATransition()
{
    if (m_bWaitForPicklist)
    {
        g_csPLAItemList = _T("");
        m_bWaitForPicklist = false;
        RETURNSTATE(ProduceFavorites);
    }
    else if ( m_PrevState == BES_BAGANDEAS )
    {
        RETURNSTATE(BagAndEAS)
    }
    RETURNSTATE(ScanAndBag)

}


