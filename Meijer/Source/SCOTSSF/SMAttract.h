//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMAttract.h
//
// TITLE: Class header for Attract State
//
// $Header:
//
// AUTHOR:    Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ATTRACTSTATE
#define _ATTRACTSTATE


#include "SMAttractBase.h"


#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
SMAttract : public SMAttractBase
{

public:
    SMAttract();                      // constructor

    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase  *SAEndOfTransaction(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed();
    virtual SMStateBase *RAParse(MessageElement* );
    virtual SMStateBase *OnLookupComplete (long byteCount);
    virtual bool LoyaltyCheck(CString csLookupType, CString csLookupData);
    virtual void HandleLoyaltyCheckTimedOut();
    virtual SMStateBase* ReloadOptions(void);

	virtual SMStateBase  *PSButtonGoBack(void);    // Price Inquiry

protected:
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton5(void);
    virtual SMStateBase  *PSButton6(void);
    virtual SMStateBase  *PSButton7(void); 
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *DMCardReader(void);
    virtual SMStateBase  *DMScale(void);
    virtual SMStateBase  *PSButtonHelp(void);
    virtual SMStateBase  *DMTakeawayBagAreaBackup(bool isBackedUp);
    virtual void PaintScanAndBagScreen();
    virtual void HandleAttendantModeTimedOut();
    virtual void DisplayHopperSubstitution(void);
    virtual void HandleBackToPrimaryLanguageTimedOut();
    virtual void ReloadThemes();
    virtual void CheckCashDiscrepancyStatus(void);
    virtual SMStateBase *PSCmdButton(CString, CString, CString);
    virtual SMStateBase * transition(TRANSITION_EVENT event, SCOT_LANGUAGE_TYPE languageUsed);
    virtual void DMTurnOffTriColorLight();
    virtual SMStateBase *CUParse(MessageElement *me);
	//#ifdef _SSCOMOBILE
    virtual SMStateBase *OnMobileTransactionStart(CString csTransactionInfo);
	//#endif // _SSCOMOBILE
	virtual SMStateBase *DMScannerEncrypted(void);
#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMAttractTSDemo.h"
#endif
	virtual SMStateBase  *OnLanguageSelection(void);
	void    RefreshUI(void);
	void    RefreshWICButton(void);
	SMStateBase *TBParse(MessageElement*);	//TAR335656
	virtual SMStateBase *OnTransactionComplete(); //SOTF6796 TAR421859

private:
	CString m_csFLConfig;
	int m_nLangSelectionPressed;
  bool m_bIsFirstTimedOut;

  DECLARE_DYNCREATE(SMAttract)// MFC Runtime class/object information
};

#endif