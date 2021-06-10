//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateDM.h
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATEDM
#define _SMSTATEDM

public:
    virtual bool ReceiptPrintSuccessful();
    virtual void ResetMLBCValues(void);
    virtual CString GetPrinterCharSet();
    virtual void SendNewEscSeq2Printer();
    virtual void SendOrigEscSeq2Printer();
    virtual long DMConfigureScaleDisplay(void);
    virtual void DMConfigureScaleParams(long &divsize, long &constant);
    virtual CString ConvertPrinterString(CString) const;
    virtual CString FormatDeviceInfoInput(long lErrorCode);

protected:
    virtual CString DMGetDeviceErrorHTML(int DeviceClass, CString& sDeviceModel, CString& sDeviceSubModel,
       int StatusEventId, long ExtendedResultCode, int nDataLength, void* sData, bool* bCheckHealth = NULL, int nContext = -1, CString sLCID = _T(""));

    virtual bool getNextHopperStatusLine(CString& currencyMap,
                                       CString& hopperStatusList,
                                       CString& hopperStatusLine, bool bDenominationOnly=false);
    virtual void getHopperStatus(const CString denominationCode,
                               CString& denominationStatus);
    virtual CString getHopperStatusString(void);

    virtual CString getHopperDenominations(long& lErrorCode);
    virtual bool isOnlyHopperSubsApprovalRequired(void);

    virtual SMStateBase *DMParse( const MessageElement* );
    virtual SMStateBase *DMSetScaleWeight(bool &);
    virtual bool DMHookNotifyTenderOver(void);
    virtual bool DMCashRecyclerShouldReject(const CString &csStateName);
    virtual bool DMCashRecyclerIsTenderOver(const CString &csStateName);
    virtual bool DMUnlockNoteCover(void);

    virtual long DMUnInitialize(void);
    virtual long DMInitialize(void);
    virtual CString DMGetDevicesInError(void);
    virtual SMStateBase *DMScanner(void);
    virtual SMStateBase *DMScannerEncrypted(void);
    virtual SMStateBase *DMReceiptPrinter(DWORD dwEvent);
    virtual SMStateBase *DMJournalPrinter(DWORD dwEvent);
    virtual SMStateBase *DMSlipPrinter(DWORD dwEvent);
    virtual SMStateBase *DMMICR(DWORD dwEvent);
    virtual SMStateBase *DMKeyLock(DWORD dwEvent);
    virtual SMStateBase *DMCashDrawer(DWORD dwEvent);
    virtual SMStateBase *DMAcceptor(void);
    virtual SMStateBase *DMCardReader(void);
    virtual SMStateBase *DMCardRemoved(void);
    virtual SMStateBase *DMCashRemoved(void);
    virtual SMStateBase *DMDispenserPositionEvent( long lStat); //4.2 - Card Dispenser
    virtual void DMSlipPrinterPrint(LPCTSTR szPrintData);
    virtual long DMSlipPrinterBarcode(CString csBarcode,long lBCType, 
      long lBCHeight, long lBCAlign, bool fPrintNumbers);
    virtual long DMSlipPrinterPrintBitmap(CString csBitmap, long lBmpAlign);
    virtual bool DMSlipPrinterIsSlipInserted();
    virtual bool DMSlipPrinterIsSlipNearEnd();
    virtual void DMSlipPrinterInsertSlip(bool bInsert);
    virtual long DMSlipPrinterHome();
    virtual long DMSlipPrinterFeedLines(int iLines);

    virtual void PrintEndorsementChit(LPCTSTR szEndorsement) const;
    virtual bool InRemoteDeviceMode(void) const;
    virtual bool IsHopperSubstitutionUsed(bool & bEmpty, bool &bChuteJammed);

    virtual bool DMWasCashTaken(void);
    virtual void DMSetCashTaken(void);

    virtual SMStateBase *DMScale(void);
    virtual void DMReceiptReprint(void);
    virtual void DMJournalReprint(void);

    virtual CString GetGLOBALAddThe128Prefix();
    virtual void DMReceiptLine(const CString& csPrintLine);
    virtual void DMReceiptCut();
    virtual void DMReceiptBitMap(const CString csBmpFile);
    virtual void DMReceiptPrintBarcode(LPCTSTR szBarcode, long     lBCType);
    virtual void DMJournalLine(const CString& csPrintLine);
    virtual void DMReceiptPrintBarcode(CString  csBarcode,
        long     lBCType, 
        long     lBCHeight, 
        long     lBCAlign = DM_ALIGN_CENTER, 
        bool     fPrintNumbers = false, 
        int      nDeviceClass = DMCLASS_RECEIPT );

        virtual SMStateBase * DMHandleStartupError(void);  

    virtual long DMPrintLine(int nDeviceClass,  const CString& csPrintLine);
    virtual long DMPrintBitmap(CString csBitmap, long lBmpAlign, int nDeviceClass = DMCLASS_RECEIPT);
    virtual long DMPrintBarcode(CString csBarcode,long lBCType, long lBCHeight, long lBCAlign, 
        bool fPrintNumbers, int nDeviceClass = DMCLASS_RECEIPT);
    virtual long DMProcessPrintLine(int nDeviceClass, const CString& csPrintLine);
    virtual long DMProcessPrintTokens(int nDeviceClass, CString& CurrentPrintLine);

    virtual long DMDispenseChange(const long, CString& csCashCountsStatus, long *lDispenseErrorCode=NULL);
    virtual long DMDispenseChange(CString& csChangeInDenomination, long *lDispenseErrorCode=NULL);
    virtual long DMReportDispensedCashList();

    virtual long DMPrintCashManagementReceipt( CArray<int,int>& caValueList, CArray<int,int>& caChangeInCount );
    virtual long DMPrintCashManagementReceipt( 
        CArray<CString,CString>& m_caCashAcceptorDenomList, 
        CArray<CString,CString>& m_caCoinAcceptorDenomList,  
        CArray<CString,CString>& m_caCashDispenserDenomList, 
        CArray<CString,CString>& m_caCoinDispenserDenomList,
        CArray<int,int>& m_caCashAcceptorCountList, 
        CArray<int,int>& m_caCoinAcceptorCountList,  
        CArray<int,int>& m_caCashDispenserCountList, 
        CArray<int,int>& m_caCoinDispenserCountList,
        CArray<int,int>& m_caCashAcceptorChangeInCountList,
        CArray<int,int>& m_caCoinAcceptorChangeInCountList,
        CArray<int,int>& m_caCashDispenserChangeInCountList,
        CArray<int,int>& m_caCoinDispenserChangeInCountList );
    virtual CString DMPrintCashmanagementReceiptByDevice( CArray<CString,CString>& m_caDenomList,
        CArray<int,int>& m_caCountList,
        CArray<int,int>& m_caChangeInCountList);
    virtual long SendPrintDataToTB(CString *csAllData);
    virtual CString Justify( const CString csData );
    virtual CString PositionData( CString csFirst, CString csMiddle, CString csLast );

    virtual CString ConvertValueToString( const int nDenom, bool bTrimDecimal );

    virtual long DMSayAmount(const long);
    virtual long DMSayPhrase(LPCTSTR);
    virtual long DMSayPhrase(const int);
    virtual long DMSaySecurity(const long, LPCTSTR, HANDLE hWaitEvent=0);
    virtual long DMCashAcceptorEnable(LPCTSTR);
    virtual long DMAcceptorLight(const int lightState);
    virtual long DMCoinAcceptorLight(const int lightState);
    virtual long DMCashAcceptorAccept(void);
    virtual long DMCashAcceptorReject(void);
    virtual long DMCashAcceptorDisable(void);
    virtual long DMCoinAcceptorEnable(void);
    virtual long DMCoinAcceptorAccept(void);
    virtual long DMCoinAcceptorReject(void);
    virtual long DMCoinAcceptorDisable(void);

    virtual SMStateBase  *DisplayCashDeviceError(void);

    virtual SMStateBase *GetUnloadedCashAcceptorError(void);
    virtual SMStateBase *GetUnloadedCoinAcceptorError(void);
    virtual SMStateBase *GetUnloadedCashChangerError(void);

    virtual long DMScaleEnable(void);
    virtual long DMScaleDisable(void);
    virtual long DMMSREnable(void);
    virtual long DMMSRLight(const int lightState);
    virtual long DMMSREject(void);
    virtual long DMMSRDisable(void);
    virtual long DMMSRCapture(void);
    virtual long DMCouponLight(const int lightState);
    virtual long DMScannerEnable(void);
    virtual long DMScannerDisable(void);
    virtual void DMTakeawayBeltNotify(long Event);
    virtual void DMTakeawayBeltReset();
    virtual long DMEncryptorEnable(void);
    virtual long DMEncryptorMaster(void);
    virtual long DMEncryptorSetup(LPCTSTR szWorkingKey, 
                                TBPINBLOCKFORMAT nPinBlockFormat, 
                                TBPINPADSYSTEM   nPinpadSystem,
                                TBPINENCODING    nPinBlockEncoding);

    virtual long DMEncryptorKeyTest();
    virtual CString DMEncryptorGetPIN(void);
    virtual long DMEncryptorDisable(void);
    virtual long DMEncryptorCancelSecureMode(void);
    virtual long DMPrintVersion(void);
    virtual void DMCashChangerPurgeBills(void);
    virtual long DMCheckHealth(long nDevClass);

    virtual CString DMCheckCashCounts(const bool, 
        CString& csHighCurrencies = (CString)EMPTY_STRING,
        CString& csCoinDispenserAlertStatus = (CString)EMPTY_STRING, 
        CString& csCashDispenserAlertStatus = (CString)EMPTY_STRING); 

    virtual void BillOrCoinErrorStatus(bool *pCoin, bool* pBill);
    virtual CString CashChangerErrorMessage(long lChangeDue, CString addMsg);

    virtual CString GetCashChangerError(void);

    virtual SMStateBase *DMEncryptorPinIsTooShort(void);
    virtual SMStateBase *DMEncryptorSecured(void);
    virtual SMStateBase *DMEncryptorCanceled(void);
    virtual SMStateBase *DMEncryptorCompleted(void);
    virtual SMStateBase *DMSigcapStarted();
    virtual SMStateBase *DMSigcapEnded();
    virtual SMStateBase* DMTakeawayReadyForItem();
    virtual SMStateBase* DMTakeawayBagAreaBackup(bool isBackedUp);
    virtual SMStateBase* DMTakeawaySafetyCoverOpen(bool isOpen);
    virtual void DMReceiptRequestCompleted(long lRequestId);
    virtual void DMJournalRequestCompleted(long lRequestId);
    virtual SMStateBase *DMSigcapFailure();
    virtual SMStateBase *DMCashChangerError(int nDMLastEvent);
    virtual void getPrinterStatusLine(const long lCode, CString& printerStatusLine);
    virtual void getCashAcceptorStatusLine(const long lCode, CString& printerStatusLine);
    virtual void getCoinAcceptorStatusLine(const long lCode, CString& printerStatusLine);
    virtual void ProcessPrinterExtendedResultCode(long &);
    virtual void AddMLBCRecord(MultiLabelBarcodeFormatRecord *pMLBFRecord);
    virtual CString GetItemCodeFromMLBC(void);
    virtual void TurnOnYellowTriColorLight(void);
    virtual int DMCheckForCoinDispenserLowSensor(void);
    virtual SMStateBase *DMCashChangerInOperable();	
    virtual SMStateBase* FillDMEvent2NotifyTB(const MessageElement* pEvent, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut);
    virtual SMStateBase* HandleScannerData( const MessageElement* message);
    virtual SMStateBase* DMRemoteScannerConnect(bool bConnect);
    virtual void DMHandleAcceptorHigh();
    virtual SMStateBase *ClearAllStatesButLast(long lDeviceClass, SMStateBase *sReturnState);
    virtual void ClearAllStates(long lDeviceClass, SMStateBase *sReturnState);
    virtual void CheckAndCleanStateStack(void);
    virtual void PrintFifoStack(void);
    virtual bool IsMINIHardware();
    virtual bool IsSUPERMINI();
    virtual bool Is5XHardware();
    virtual bool IsCashDeviceConfigured();
    virtual bool IsVerifoneConfigured();
    virtual void DisplayVerifoneForm(_bstr_t csForm);
    virtual CString Get563xCassetteParms(long lcode, int nContext);
    virtual void SetCashDiscrepancyStatus(bool bDiscrepancy);
    virtual bool GetCashDiscrepancyStatus(void);

	// SA250050: Processing of FL dynamic bitmap loading on Lane & RAP Printer through SetBitmap
	bool LoadReceiptLogo(CString bitmapFilePath);


// SA250050:
// ---------------------------------------------------
// (START) - Scanner Sharing methods & properties
// ---------------------------------------------------
// This section is added to support the sharing of the 
// scanner device with the LaunchPad application.
#define eventRapOperation           _T("operation")
#define ReleaseScanner              _T("release-scanner")
#define ScannerAvailable            _T("scanner-available")
#define ScannerOpenFailed           _T("scanner-open-failed")
#define ExitRemoteMessageListener   _T("exit-remote-message-listener")

// RemoteMessage requests, piped into DM for processing to post 
// so they can be processed in SCOTAPP thread.
#define DMCLASS_REMOTEMESSAGE            (DMCLASS_MAX_CLASSES + 1)
	
// DeviceId represents the remotemessage requests 
enum {
	DM_REMOTEMESSAGE_RELEASESCANNER,      
	DM_REMOTEMESSAGE_SCANNERAVAILABLE,
	DM_REMOTEMESSAGE_SCANNEROPENFAILED,
	DM_REMOTEMESSAGE_PROCESSBIOMETRICDATA
};

protected:
	static UINT_PTR m_scannerMsgTimer;
    static CString m_scannerMsgTarget;
    static CString m_biometMsgTarget;
    static CString m_currentProcessName;
    static CRITICAL_SECTION m_scannerMsgTargetLock;
    static int m_scannerMsgTimeout;
    static bool m_waitingForScannerMsgResponse;
    static bool m_scannerSharingEnabled;
    static bool m_scannerLoaded;
    static CWinThread * m_remoteMessageListenerThread;
    static HANDLE m_remoteMessageListenerExit;
	static bool m_cashTakenInSecState;
    static bool m_bCashDispensed;
  	static bool m_timeoutInTakeChangeState;

	void JoinThreadOrTerminate(CWinThread * thread, DWORD timeout);
	void RemoteMessageListener();
	static UINT RemoteMessageListenerProc(LPVOID param);
    CString ExtractRapEventData(CString key, CString eventData);
    void DetectAndEnableScannerSharing();
	long OpenAndEnableScannerDirectly(bool forceEnable, bool sendMsgOnFail);
	void ProcessReleaseScannerRequest();
    void ProcessResponseScannerAvailable();
    void ProcessResponseScannerOpenFailed();
    SMStateBase * ProcessBiometricData(CString encodedData);
	long RequestAccessToScanner();
    void ScannerMsgTimeout();
    void SetScannerMsgTimer();
	void ClearScannerMsgTimer();
    bool IsScannerMsgTargetRunning();

	void SetDMCashTakenInSecState(bool value);
	bool GetDMCashTakenInSecState();
	void SetTimeoutInTakeChangeState(bool value);
	bool GetTimeoutInTakeChangeState();
	bool GetChangeDispensedState();
	void SetChangeDispensedState(bool value);

	friend UINT _cdecl RemoteMessageListenerProc(LPVOID pvParam);
    friend VOID CALLBACK ScannerMsgTimeoutProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	
    // ---------------------------------------------------
    // (END) - Scanner Sharing methods & properties
    // ---------------------------------------------------

	void SetCutPercentage(long lCutPercentage);
    static void ThrowDmException( SCODE sc );
    static void ThrowDmException( long  nDeviceClass, long lResult, long nRcEx );
    static long m_lCutPercentage;
#endif