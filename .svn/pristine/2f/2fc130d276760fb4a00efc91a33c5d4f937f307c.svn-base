//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMBagAndEAS.h
//
// TITLE: Class header for Bag and EAS state
//
// $Header:
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _BAGANDEASSTATE
#define _BAGANDEASSTATE

#include "SMBagAndEASBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMBagAndEAS : public SMBagAndEASBase
{
public:
    SMBagAndEAS();

    virtual SMStateBase  *Initialize();
    virtual void UnInitialize(void);
    virtual bool PSAssistMenuLegalForState();
    virtual bool storeMode();
    virtual SMStateBase *OnPLAHandInView();
	static long GetDWORDValue(const CString& key,const CString& value);

protected:
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnMatchedWt(void);
    virtual SMStateBase  *DMdf_EASNoMotion();
    virtual SMStateBase  *OnBackToLGW(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase *DMScanner(void);
    virtual void Escalation2RAP(void);
    virtual SMStateBase *GPNotify(const GPEVT gpEvt);
    
    virtual SMStateBase *DMTakeawayReadyForItem(void);
    virtual void HandleSkipBaggingButtonTimedOut(); 
    virtual void StartTimerForSkipBaggingBtn(); 
    virtual SMStateBase  *SASecMgrParse( const MessageElement*);
    virtual SMStateBase  *DMTakeawayReadyForNotRequiredBaggedItem();

	//POS40504 - rrs
	virtual void ShowSkipBaggingOnEReceipt(void);
	bool bSkipBaggingButtonPressed;
	//e - POS40504


    DECLARE_DYNCREATE(SMBagAndEAS)// MFC Runtime class/object information



};

#endif