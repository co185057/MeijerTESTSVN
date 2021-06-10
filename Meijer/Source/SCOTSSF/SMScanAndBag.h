//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMScanAndBag.h
//
// TITLE: Class header for Scan and Bag State
//
// $Header:
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCANANDBAGSTATE
#define _SCANANDBAGSTATE

#include "SMScanAndBagBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMScanAndBag : public SMScanAndBagBase
{
public:
    SMScanAndBag();
    //+4.2 - Card Dispenser
#ifdef _TESTSSF
protected:
    #include "Test_CardDispenser\SMScanAndBagTest.h"
#endif
    //-4.2 - Card Dispenser
	static VOID CALLBACK SNDYesNoButtonTimeOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

    virtual SMStateBase *Initialize(void);
    virtual SMStateBase *OnPLAHandInView();
    virtual void     UnInitialize(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnMatchedWt(void);
    virtual bool PSAssistMenuLegalForState();
protected:
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSButton3(void);
    virtual SMStateBase *PSButton4(void);
    virtual SMStateBase *PSButton5(void);
    virtual SMStateBase *PSButton6(void);
    virtual SMStateBase *PSButton7(void); // SOTF5803
	virtual SMStateBase *PSButton8(void); // POS138636
    virtual SMStateBase *DMScanner(void);
    virtual SMStateBase *PSCmdButton(CString csContext, CString csControl, CString csCommand);
    virtual SMStateBase *DMCardReader(void);
    virtual SMStateBase *DMAcceptor(void);
    virtual SMStateBase *DMScale(void);
    virtual SMStateBase  *PSButtonHelp(void);
    virtual SMStateBase* DMTakeawayBagAreaBackup(bool isBackedUp);
    virtual SMStateBase *PSNumericKey(const long);
    virtual SMStateBase *PSEnterKey(void);
    virtual SMStateBase *PSClearKey(void);
    virtual void InitializeBasedOnOption();
    virtual void FlashButtonsOnWeight(bool);
    virtual SMStateBase *TimedOut(void);
    virtual void ChangeLanguage(SCOT_LANGUAGE_TYPE langNew);
    virtual SMStateBase *OnLookupComplete (long byteCount);
    virtual SMStateBase  *PSListLookup(CString);
    virtual long DMSayPhrase(LPCTSTR sPhrase);
    virtual long DMSayPhrase(const int nMsg);
    virtual long DMSaySecurity(const long nMsg, LPCTSTR szWave, HANDLE hWaitEvent =0);
    virtual void TBGetItemDetails(ItemDetails&);
    virtual void ProcessWeightOnScale(void);
    virtual SMStateBase *CUParse(MessageElement *me);
	//#ifdef _SSCOMOBILE
    virtual SMStateBase *OnMobileTransactionStart(CString csTransactionInfo);
	//#endif // _SSCOMOBILE
	

	// POS15766
    virtual SMStateBase *TBParse(MessageElement *me);

	SMStateBase * TransitionToPayment(bool bTBWillDriveToPayment);

	virtual SMStateBase *DMTakeawayReadyForItem(void); // POS78131, SSCOB-1164

private :
    // POS29613 - rrs
    CString strWICEBTCheckFeature;
    CString strWICEBTCMFeature;
	void UpdateVoidRemoveButtonState(CString csPOSState);
	void UpdatemPerksButton(bool enable);

    DECLARE_DYNCREATE(SMScanAndBag)// MFC Runtime class/object information


};

#endif
