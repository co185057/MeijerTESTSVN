//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateTB.h
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SMSTATETB
#define _SMSTATETB


	static bool m_bNeedAssistMode;
	static bool m_bTrxnEndedInHandHeld;
	static bool m_bAssistModeReturnAfterSystemMessage;
	static bool m_bReEnterDOB;
    static bool m_bTakeChangeTimerExpired;  //SSCOI-31860

	static bool m_bRecallModeWaitforItemSold; // RFQ 9545

	virtual bool CanSuspendTransaction(void);
	virtual CString TBGetKeyPOSString(const int nKey);

public:
    virtual USHORT TBGetKeyCode(const int nKey);
    virtual long   TBGetKeyLabel(const int nKey,CString &csLabel);
    virtual USHORT *TBEmulatorKeys(void);
    virtual void   TBLoadAssistModeKeyboard(void);
protected:
    
	static USHORT m_nAssistKeyVirtualKeyValue[200];				  // key Virtual Key Value
	static CString m_csKeyPOSString[200];
    static PSTEXT m_nAssistKeyTextID[200];


public:
    virtual void DoEvents();
    virtual long TBGetNextState(LPCTSTR lpLastStateName, 
                               LPCTSTR lpNextStateName, 
                               bool *pbOverride, 
                               LPTSTR *lpOverrideStateName, 
                               CDescriptorList &dlParameterList);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    //virtual TBRC SendTBCommand(  long lCommandID, ...);

    virtual SMStateBase *TBStart(LPCTSTR = NULL);
    virtual SMStateBase *TBItemSale(const bool, const long, const bool, const bool = false, const long = ITEMUPDATE_NONE);
    virtual SMStateBase *TBEnterItem(const bool=false, int lUpdateFields = ITEMUPDATE_NONE);
    virtual SMStateBase *TBEnterTender(const TBTENDERTYPE, const long=0, const bool=false);
    virtual SMStateBase *TBCancelTransaction(const bool=false);
    virtual SMStateBase *TBSuspendTransaction(const bool = false);
    virtual SMStateBase *TBIdleTimer(void);
    virtual SMStateBase *TBEndOfTransaction(void);
    virtual SMStateBase *TBFinish(void);
    virtual TBRC TBLogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
                           const int nComponentID, LPCTSTR szComponentID,
                           LPCTSTR szModuleID, const int nModuleLineNumber);
    virtual void TBEnterTenderState(const bool bEnter);
    virtual void TBSendSigData(LPCTSTR CustomerName, LPCTSTR cSigData, DWORD nLength);
    virtual SMStateBase* TBGetMessageInfo(const bool bDisableSA = false);
    virtual void TBPrcessPIPTenderType(TBTENDERTYPE&);
    virtual void TenderComplete(void);
    virtual SMStateBase * HandleVoidedTender(void);

    virtual BOOL TBValidateHeadCashier(LPCTSTR szCashierID, LPCTSTR szCashierPW);
    virtual void ProcessPendingActions();
    virtual bool        IsTenderStarted();
    virtual void        SetTenderStarted(bool bTenderStarted);
    virtual SMStateBase *TBTenderSelected(TBTENDERTYPE nTenderType);  
protected:
    virtual  void   ProcessTBReturn(long rc);
    virtual SMStateBase *TBParse(MessageElement* );
    virtual SMStateBase *TBItemDetails(void);
    virtual SMStateBase *TBGetTotalDetails(void);
    virtual SMStateBase *TBParseFoodStampTotal(void);
    virtual void TBGetItemDetails(ItemDetails&);
    virtual void UpdateTotal(void);
    virtual SMStateBase *TBBalanceDetails(void);
    virtual SMStateBase *TBGetTenderDetails(void);
    virtual SMStateBase *TBGetAvailableFoodStampAmount(void);
    virtual SMStateBase *TBGetAvailableBenefitAmount(void);
    virtual SMStateBase *TBRequestFoodStampEligibleTotal(void);

    virtual void TBShow2x20(void);
    virtual void TBShow2x20(CString csOpDisplay);

    virtual CString TBName(void);

    virtual TBRC  TBGetPINWorkingKey(CString &csAct,
        CString &csKey,
        TBTENDERTYPE nTyp,
        TBPINBLOCKFORMAT& nPinBlockFormat,
        TBPINPADSYSTEM& nPinpadSystem,
        TBPINENCODING& nPinBlockEncoding);

    virtual bool TBPromptForDebitAccountType(void);
    virtual long TBRefreshState(void);
    virtual long TBInitialize(HWND hwnd, LPCTSTR szFilePath);
    virtual long TBLoadOptions(void);
    virtual long TBUnInitialize(void);
    virtual CString TBLookupCode(const USHORT uKey);
    virtual long TBGetTerminalNumber(void);
    virtual long TBGetCurrentTransactionNumber(void);
    virtual long TBGetCurrentBusinessDate(CString &csBusinessDate);
    virtual BOOL TBValidateOperator(LPCTSTR,LPCTSTR);

    virtual HFILE TBGetSlipData(void);
    virtual HFILE TBGetReceiptData(void);
    virtual HFILE TBGetJournalData(void);
    virtual CString TBGetDisplay(void);

    virtual void TBReleaseSlipData(HFILE);
    virtual void TBReleaseReceiptData(HFILE);
    virtual void TBReleaseJournalData(HFILE);
    virtual void TBLaneClosed(const bool bEnter);
    virtual TBSTATE TBProcessMessage(MessageElement* me);
    virtual SMStateBase *TBSetTrainingModeState(const bool bTMState);
    virtual long TBSetBusinessDate(LPCTSTR szSetBusinessDate);
    virtual void TBEPSEndOfDay(LPVOID lp=0);
    virtual void TBEPSStartOfDay(LPVOID lp=0);
    virtual void TBNeedStartOfDay(TBSTARTOFDAYSTATE &eState);

    virtual void ResetApprovalValues(const bool=false);

    virtual SMStateBase *OnEndorseDocument(void);
    virtual SMStateBase *OnNewCashDrawerState(void);
    virtual SMStateBase *OnNeedMICR(void);
    virtual SMStateBase *OnNewSecurityLevel(void);
    virtual SMStateBase *OnAssistModeStatusChanged(void);
    virtual SMStateBase *OnTBNeedData(void);
    virtual long TBLogReceipt(LPTSTR*,BSTR *, LPTSTR*);
    virtual long TBPreprocessSignatureData(LPCTSTR,BSTR, LPCTSTR,BOOL*);
    virtual long TBGetGiftCardBalance(long *plBalance);

    virtual SMStateBase *HandleOutOfSyncStop();
    virtual SMStateBase *HandleOutOfSyncAssist();
    virtual void TBSyncTBorPOS();
    virtual SMStateBase *getRealStatePointer(SMStateBase *pState=NULL);
    virtual bool AllowImmediateReceiptPrint(); 
    virtual void EndOfTransactionPrint();
	virtual bool IsCardTender(int nTenderType);
	CString GetHookProperty(CString csProp);
	CString GetTBProperty(CString csProp);
	void SetHookProperty(CString csProp, CString csValue = _T(""));
    void SetTBProperty(CString csProp, CString csValue = _T(""));

	virtual void TBAddMessagetoReceipt();


	bool m_bCustomerState;
	static TBSTATE m_TBLastState;
	static long m_lTBCouponCount;
    static long m_lTBCouponAmount;


#endif
