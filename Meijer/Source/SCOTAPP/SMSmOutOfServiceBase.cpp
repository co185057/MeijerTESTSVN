//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmOutOfServiceBase.CPP
//
// TITLE: Class implementation for store mode Out Of Service state
//
// AUTHOR:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"                 // MGV common includes

#include "SMSmOutOfService.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmOutOfServiceBase")

IMPLEMENT_DYNCREATE(SMSmOutOfServiceBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmOutOfService)


DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSmOutOfServiceBase::SMSmOutOfServiceBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
SMStateBase  *SMSmOutOfServiceBase::Initialize(void)
{
	ps.SetBackground(BMP_OUTOFSERVICE);
  ps.SetTitle(LTT_OUTOFSERVICE, PS_BLANK);
  ps.SetLeadthruText(LTT_SYSFUNCSELECT, PS_BLANK);
// NewUI
	ps.SetButtonText(_T("SMButton1"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton2"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton3"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton4"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton5"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton6"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton7"), PS_NOTEXT);
	ps.SetButtonText(_T("SMButton8"), PS_NOTEXT);
// NewUI
  ps.ShowSMTotal(false);
	ps.ShowFrame();

#if 0
  ps.SMShow( BMP_OUTOFSERVICE,      // just the name string, no path
		LTT_OUTOFSERVICE,               // leadthru text string
		// BMP_SMDEFAULT,							  // background
		// TTL_SYSTEMFUNCTIONS,// title 1
		PS_BLANK,											  // blank title 2
		LTT_SYSFUNCSELECT,						  // leadthrutext 1
		PS_BLANK,											  // blank LTT 2
		false,												  // show total
		PS_NOTEXT,										  // button 1
		PS_NOTEXT,										  // button 2
		PS_NOTEXT,										  // button 3
		PS_NOTEXT,										  // button 4
		PS_NOTEXT,										  // button 5
		PS_NOTEXT,										  // button 6
		PS_NOTEXT,										  // button 7
		PS_NOTEXT);										  // button 8
#endif
  ps.AllowInput(co.GetnOperationsOperatorPWMaxDigit(),true,false);
  ps.Prompt(TXT_ENTER_PASSWORD);
	
  return STATE_NULL;
}

