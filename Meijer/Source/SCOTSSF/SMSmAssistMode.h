//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmAssistMode.h
//
// TITLE: Class header for Assist mode keyboard entry mode
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSMASSISTMODE
#define _SMSMASSISTMODE

#include "SMSmAssistModeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmAssistMode : public SMSmAssistModeBase
{
public:
    SMSmAssistMode();
    virtual SMStateBase  *Initialize();
    virtual void     UnInitialize();
	virtual SMStateBase *TBParse(MessageElement *me);
    virtual bool PSRemoteLegalForState();
    virtual SMStateBase  *TimedOut(void);
    virtual bool assistMode();
    virtual SMStateBase  *DMRemoteScannerConnect(bool bConnect);

	bool IsKeyAllowed(CString csPOSString, long nKey);
    void ClearApprovalFlags(void);
    void ResetPropertiesBasedOnScreenAndKeys(CString csPOSString, CString csTitleOneText );
    CString GetSoftKeyButtonText(long lBtnNumber);
    bool IsSpecialHandlingScreen(void);
    void AMClearAllInput(void);

	virtual SMStateBase* HandleVoidedTender(void);

	static bool m_bTenderInProgress;
	static CString m_csRAPConnectionName; // POS181195/POS181197

protected:
	virtual SMStateBase  *PSAssistKey(const long);
	virtual SMStateBase  *PSKeystroke(const long wParam, const long lParam);
	virtual SMStateBase  *PSNumericKey(const long);
	virtual SMStateBase  *PSAlphaNumericKey(const long);	
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
	virtual CString  GetDisplayText(CString csFieldName);
    virtual _variant_t  GetFieldValue(CString csFieldName);
    virtual SMStateBase *PSReceiptUp(void); 
    virtual SMStateBase *PSReceiptDown(void);
	virtual void TBShowDisplay(bool bRefresh = false, bool bRepaintLastScreen = false);
    virtual void TBShow2x20(void);
    virtual void TBShow2x20(CString csOpDisplay);
    virtual void DisableKeys();
    virtual void AddSerialNumToReceipt(void);
    void DisableSoftKeys();	
    virtual SMStateBase* DMReceiptPrinter(DWORD dwEvent);
	virtual SMStateBase  *PSEnterKey();
    virtual SMStateBase  *PSClearKey();
    virtual SMStateBase  *PSCancelKey();
	virtual void EnableDisableAllKeys(bool bFlag);
	virtual SMStateBase *DMCashDrawer(DWORD dwEvent);
    virtual SMStateBase  *DMScanner();
    virtual SMStateBase  *DMCardReader(void);
    virtual SMStateBase  *DMScale();
    virtual SMStateBase  *SAEndOfTransaction();
    virtual SMStateBase  *DMAcceptor(void);
    virtual SMStateBase* DMMICR(DWORD dwEvent);
    virtual SMStateBase* OnNeedMICR();
    virtual SMStateBase* DMSlipPrinter(DWORD dwEvent);
    virtual SMStateBase* OnEndorseDocument(void);
    virtual SMStateBase* OnNewCashDrawerState(void);
    
    virtual TBRC  TBKeyPress(const USHORT uKey);
    virtual SMStateBase *PSAlphaKey(const long lKeystroke);
    virtual SMStateBase *OnAssistModeStatusChanged(void);
    virtual SMStateBase *PSButtonToggle(void);
    virtual SMStateBase *PSButtonLaneStatus(long lLaneIndex);
    virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);
    virtual SMStateBase *PSRemoteButton1(void);
    virtual SMStateBase *DMTakeawayAMExitDone(void);
    virtual void TABExitingAssistMode(void);
    virtual SMStateBase *HandleNewContext();
    virtual SMStateBase *AutoRunTimeOut(void); 
    virtual SMStateBase *TBEndOfTransaction(void);
    virtual CString ExtractString(CString strData, CString sStringName);
    virtual SMStateBase *PSConnectRemote(CString&, CString&);
    virtual void ResetFlagsInAM(void); 
    virtual long     DoKeyCode(UCHAR uKey);
    virtual SMStateBase  *IssueChange(void);
    virtual SMStateBase *DepositCouponsIfNecessary();

	void SetOperatorGuidance();
    void PrcMethodMsg();
    void VoidItemMsg();
    virtual void SetDynakeys();
    void SetChangeDueDisplay();
    void TraceDisplayMessage();
    void ShowCashDrawerCloseMessageIfApplicable(bool bShowDisplay = false);
	bool CheckIfPrinterAvailable(bool bRetryPrints);
	CString m_csLastDisplay;
    CString m_csLastTitle1;
    MSXML2::IXMLDOMDocumentPtr m_POSDisplay;
    bool m_bSlipPrintSideFront;
    bool m_bSlipAlreadyFlippedAfterApproval;
    bool m_bSlipActivityIsWaiting;
    bool m_bSlipFlipInProgress;
	
    bool m_bEnhancedAssistMode; // POS31050 rrs
	static CString m_csLaneName; // POS181195/POS181197

	bool m_bIsFromDLScanning;

	static bool isEASOperatorGuidance; //CRD 185620
	//Start CRD 289150
	static bool bShowSmRecallingItems;
	static bool bShowFinishRecall;
	static bool bShowPromptOnRefresh;
	static bool bShowB2BHighValuePromo;
	void AMFinishRecall();
	void AMFinishRecallDone();
	//End CRD 289150

private:
    void SetEntryText( );
	//    CString GetLaneName();	// POS181195/POS181197
    bool m_bTransactionEnding;
    CString m_csPrevLastDisplay;
	long m_lWaitCount;
	CString m_csEntryDate;
    bool m_bRequestingCashDrawerOpen;
	bool m_bUnRemotedPrinterDevices;
    bool m_bPreviousDeviceError;
    CString m_csWICItemList;	//POS154826
	bool m_bWasPOSRestarted;

    DECLARE_DYNCREATE(SMSmAssistMode) // MFC Runtime class/object information
};

#endif