//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMFinish.h
//
// TITLE: Class header for Finish State
//
// $Header:
//
// AUTHOR:    Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FINISHSTATE
#define _FINISHSTATE

#include "SMFinishBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMFinish : public SMFinishBase
{
public:
    SMFinish();
    virtual SMStateBase  *Initialize(void);
	virtual SMStateBase  *TimedOut(void);
	virtual SMStateBase  *PSButton1(void);
	virtual SMStateBase  *PSButton2(void);
	virtual SMStateBase  *PSButton3(void); // Cancel Order RFQ 9554
	virtual SMStateBase  *PSButton5(void); // Language
	virtual SMStateBase  *PSButton7(void); // Volume 
    virtual void     UnInitialize(void);
    virtual bool PSAssistMenuLegalForState();
    virtual bool storeMode();

private:
	bool m_bIsCreditSelected;
	static long m_lTimeOutCtr;

protected:
    virtual SMStateBase  *DMAcceptor(void);
    virtual SMStateBase  *DMCardReader(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *PSListLookup(CString);
    virtual SMStateBase  *SAEventOccurred(const long);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSButtonHelp(void);
    virtual SMStateBase  *checkCashAcceptorCassette();
    virtual SMStateBase *ButtonPressed(int);
    virtual SMStateBase *ProcessTender(TBTENDERTYPE);
    virtual SMStateBase *StandardCardProcessing(int);
    virtual void ConfigureTenderOptions(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *PSConnectRemote( CString& strMachineName, CString& strRemoteContext );
    virtual bool IsPaymentButtonAvailable(); //SR742 - POS186112
	virtual SMStateBase *PSCmdButton(CString csContext, CString csControl, CString csCommand);

  DECLARE_DYNCREATE(SMFinish)// MFC Runtime class/object information
		
};

#endif