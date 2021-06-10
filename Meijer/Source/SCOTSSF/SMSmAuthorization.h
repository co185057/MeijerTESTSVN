//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAuthorization.h
//
// TITLE: Class header for store mode Authorization state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMAUTHORIZATIONSTATE
#define _SMAUTHORIZATIONSTATE

#include "SMSmAuthorizationBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmAuthorization : public SMSmAuthorizationBase
{
public:
    SMSmAuthorization();
	 //+sotf 7384
    virtual SMStateBase  *Initialize(void);
	//-sotf 7384
    virtual void UnInitialize(void);
    virtual SMStateBase *RAParse(MessageElement* );
    virtual SMStateBase *TBParse(MessageElement* me);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton5(void);
	//sotf 6226
    virtual SMStateBase  *PSButton6(void); // Assist Mode
	//-soft
	SMStateBase  *PSButton7(void);		   // Duplicate Receipt
    virtual SMStateBase  *PSButton8(void); // return to shopping
    virtual SMStateBase  *PSReceiptUp(void);
    virtual SMStateBase  *PSReceiptDown(void);
    virtual void         InitUI(void);
    virtual void         InitButtons(void); //TAR351364
    virtual void         InitLeadThruText(void);
    virtual void         InitReceiptArea(void);
    virtual void         InitLanguage(void);
    virtual void         InitDevices(void);
    virtual SMStateBase  *OnGoBackButton(void); 
    virtual void ShowSuspendButton(); // POS39475
    virtual void ShowCloseLaneButton();
    virtual void ShowAssistModeButton();
    virtual void ShowMediaStatusButton();
    virtual void ShowReportingButton();
    virtual void ShowSystemFunctionsButton();
    virtual void ShowGoBackButton();
    virtual SMStateBase  *PSSwitchContext(BOOL bRemote, CString csNewContext);
    virtual void ShowSMInfo();
    virtual void HandleApprovalsInSM();

	virtual SMStateBase  *TimedOut(void);

    DECLARE_DYNCREATE(SMSmAuthorization)// MFC Runtime class/object information
};

#endif