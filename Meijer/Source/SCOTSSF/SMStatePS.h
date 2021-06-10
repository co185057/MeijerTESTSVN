//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStatePS.h
//
//
// AUTHOR:    Peter Denhoed
//
// CRD124746 Work Request:Sprint 10.3 Name:Aparna Tunuguntla Date: March 19,2018
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATEPS
#define _SMSTATEPS

public:
    virtual bool PSRemoteLegalForState();
    virtual bool PSAssistMenuLegalForState();

    void TrimSymbologyForSSF(CString& csItemCode);   //UploadTicket Port 

protected:
    virtual SMStateBase *PSParse( const MessageElement* );

    virtual long PSInitialize(void);
    virtual long PSUnInitialize(void);
    virtual SMStateBase *PSButtonHelp(void);
    virtual SMStateBase *PSButtonGoBack(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSButton3(void);
    virtual SMStateBase *PSButton4(void);
    virtual SMStateBase *PSButton5(void);
    virtual SMStateBase *PSButton6(void);
    virtual SMStateBase *PSButton7(void);
    virtual SMStateBase *PSButton8(void);
    virtual SMStateBase *PSMenuKey1(void);
    virtual SMStateBase *PSMenuKey2(void);
    virtual SMStateBase *PSMenuKey3(void);
    virtual SMStateBase *PSMenuKey4(void);
    virtual SMStateBase *PSMenuKey5(void);
    virtual SMStateBase *PSMenuKey6(void);
    virtual SMStateBase *PSMenuKey7(void);
    virtual SMStateBase *PSMenuKey8(void);
    virtual SMStateBase *PSMenuKey9(void);
    virtual SMStateBase *PSMenuKey10(void);
    virtual SMStateBase *PSMenuKey11(void);
    virtual SMStateBase *PSMenuKey12(void);
    virtual SMStateBase *PSMenuKey13(void);
    virtual SMStateBase *PSMenuKey14(void);
    virtual SMStateBase *PSMenuKey15(void);
    virtual SMStateBase *PSAssistKey(const long);
    virtual SMStateBase *PSCmdButton(CString, CString, CString);
    virtual SMStateBase *PSCmdList(CString, CString, const long);

    virtual SMStateBase *PSMainMenuKey(void);
    virtual SMStateBase *PSHelpTopics(void);
    virtual SMStateBase *PSSwitchContext(BOOL, CString);
    virtual SMStateBase *PSListLookup(CString);
    virtual SMStateBase *PSListLookupTab(LPCTSTR);
    virtual SMStateBase *PSListScroll(const long);
    virtual SMStateBase *PSConnectRemote(CString&,CString&);
    virtual SMStateBase *PSAlphaNumericKey(const long);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual SMStateBase *PSVideoDone(void);
    virtual SMStateBase *PSNumericKey(const long);
    virtual SMStateBase *PSBackSpaceKey(void);
    virtual SMStateBase *PSShiftKey(void);
    virtual SMStateBase *PSDecimalKey(const long);
    virtual SMStateBase *PSOtherSpecialKeys(const long);
    virtual SMStateBase *PSEnterKey(void);
    virtual SMStateBase *PSCancelKey(void);
    virtual SMStateBase *PSClearKey(void);
    virtual SMStateBase *PSSpaceKey(void);
    virtual SMStateBase *PSAssistAccepted(void);
    virtual SMStateBase *PSAssistDenied(void);
    virtual SMStateBase *PSKeystroke(const long, const long);
    virtual SMStateBase *PSHotKey(CString &);
    virtual SMStateBase *PSCharKey(TCHAR c);
    virtual SMStateBase *PSAltGrKey(void);

    virtual TCHAR VirtKeyToTChar(UINT vKey, long shiftState);
    virtual long PSEchoInput(const long);
    virtual long PSEchoDecimalInput(const long);
    virtual long PSClearInput(void);
    virtual long PSBackSpaceInput(void);
    virtual SMStateBase *PSClearAllInput(BOOL bSayValid = TRUE);
    virtual SMStateBase *PSAlphaKey(const long);
    virtual void VoidAllCoupons();

    virtual long SetButton(LPCTSTR szControlName,
        LPCTSTR     szContent,
        const bool fEnable);
    virtual void PSSortQtyList(CStringArray &);
    virtual bool VerifyQtyItem(CString);
    virtual bool SCOTIsMinimized();
    virtual void SetFocusToVideo();
    virtual SMStateBase *PSOnRemoteDisconnected(const long lParm);
    virtual SMStateBase *PSButtonToggle(void);
    virtual SMStateBase *PSButtonLaneStatus(long lLaneIndex);
    virtual SMStateBase *OnExternalUIRequest(MessageElement* me);
    virtual bool FindItemID(CString& csGetItemID, CString csReceipt, CString csReceiptSrc, long lExtPrice, long lUnitPrice, CString csItemCode);
    virtual void TrimSymbology(CString& csItemCode);
    virtual SMStateBase *HandleNewContext();
    virtual SMStateBase* PreNotifySCOTEvent2TB(const MessageElement *me, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut);
    virtual SMStateBase* FillPSEvent2NotifyTB(const MessageElement* pEvent, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut);
    virtual void SetAlphaShiftState(CString csContext, bool bShiftOn);
    virtual void BuildCmdButtonList(CSCOTDataEntry& cDataEntry, CString csCtxName, CString csCtlName);
    virtual void BuildCmdList(CSCOTDataEntry& cDataEntry, const CString& csReceiptSrc);
    virtual void PSParseFinalProcessing(const MessageElement* me,  SMStateBase* pRetState);
    virtual bool ShouldDiscardEvent(const PSInfo &psInfo) const;
	static VOID CALLBACK RAPHHScannerTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime); //CRD124746

	// CRD196066: Helper method to start/stop HHScanner Timer
	void StartHHScannerTimer();
	void StopHHScannerTimer();

#endif
	
