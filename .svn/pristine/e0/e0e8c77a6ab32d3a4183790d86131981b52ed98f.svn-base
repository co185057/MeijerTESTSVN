//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSecurityLevel2Base.cpp
//
// TITLE: Class header for Level 2 Security agent State.
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>								  // MS MFC always needed first
//							  // MS MFC template header

#include "Common.h"								  // Common includes

#include "SMState.h"							  // Base state
#include "SMSecurityLevel2.h"			  

#define COMP_ID ID_SM							  // Base state component
#define T_ID _T("SecurityLevel2Base")

IMPLEMENT_DYNCREATE(SMSecurityLevel2Base, CObject)  // MFC Runtime class/object information

//IMPLEMENT_STATE(SecurityLevel2)

DEFINE_TIMESTAMP

//////////////////////////////////////////////////////////////////
SMSecurityLevel2Base::SMSecurityLevel2Base( CString csBar, CString csPop )
: csBarMsg(csBar), csPopUpMsg(csPop)
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMSecurityLevel2Base::SMSecurityLevel2Base()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMSecurityLevel2Base::Deliver( CString csBar, CString csPop )
{
  csBarMsg = csBar; csPopUpMsg = csPop;

  return this;
}

bool SMSecurityLevel2Base::DeliverDescriptorList(CDescriptorList &dlParameterList)
{
    // There should be two parameters, both strings
    if (dlParameterList.GetDescriptorCount() == 2)
    {
        trace(L6, _T("+SMSecurityLevel2Base::DeliverDescriptorList() - Parameter Count of 2"));
        Deliver(DLParamCString(dlParameterList),    // csBar
                DLParamCString(dlParameterList));   // csPop
        return true;
    }
    else
    {
        trace(L2, _T("+SMSecurityLevel2Base::DeliverDescriptorList() - ERROR - Parameter Count %d"), dlParameterList.GetDescriptorCount());
        return false;
    }
}


///////////////////////////////////////////
SMStateBase  * SMSecurityLevel2Base::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SecurityLevel2") );	// RFC 330050
  // TAR 125995
  SAWLDBScannerDisable();  // Disable the scanner 

  ps.SetPSText(PS_WORK1,csBarMsg);
#ifndef NewUI
  ps.SetCMFrame(DEFAULT_FRAME_WITHOUTECHO);
#endif
  ps.SetCMBackground(BMP_ERRMSGLEVEL2);
  ps.SetCMMascotImage(BMP_MASCOT_ERRMSGLEVEL2);
  ps.SetCMLeadthruText(PS_WORK1);
#ifdef NewUI
  ps.SetButtonText(_T("CMButton1"), BTT_OK);
  //Tar 121312
  ps.CMButtonFlash(_T("CMButton1"),true);
  ps.SetButtonText(_T("CMButton2"), PS_NOTEXT);
  ps.SetButtonText(_T("CMButton3"), PS_NOTEXT);
  ps.SetButtonText(_T("CMButton4"), PS_NOTEXT);
#else
  ps.SetCMButton1Text(BTT_OK);
  //Tar 121312
  ps.CMButtonFlash(IDC_BUTTON1,true);
  ps.SetCMButton2Text(PS_NOTEXT);
  ps.SetCMButton3Text(PS_NOTEXT);
  ps.SetCMButton4Text(PS_NOTEXT);
#endif
  ps.SetCMGobackButtonVisible(false);

  ps.ShowCMFrame();
  ps.ShowCMTotal(false ,true);
  ps.ShowCMTBText(csTBMessage2Scot);	//thp
  if(_tcsclen(csPopUpMsg)>0)
  {
	ps.Message12(csPopUpMsg, true);
  }
	
  // ps.ShowVideoLarge();	        // Added to Security level2 per UI test script was never
  return STATE_NULL;
}

//////////////////////////////////////////
SMStateBase  * SMSecurityLevel2Base::PSButton1(void) // continue
{
  // TAR 125995
  SAWLDBScannerEnable();  // Enable the scanner 
  return STATE_RETURN;
}

// + RFC 330050
void SMSecurityLevel2Base::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SecurityLevel2") );	
}
// - RFC 330050
