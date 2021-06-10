//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMState.h
//
// TITLE: Class implementation file for Base State
// This class is used for deriving all actual
// states like STAttract and FirtTimeUserState.
// This state provides default processing for
// all possible messages to be parsed, usually
// an error because these messages should have
// been handled by the derived state and not end
// up here.
//
// This class is an 'abstract' class because it has
// at leas one 'pure virtual' function 'virtual...=0;'
// This means this class cannot be instanciated itself,
// but other classes derived from this once can.
//
// This state automatically accepts these inputs and provides
// a standard action:
// PSKey101   : 101-Keyboard Numerics, ENT, ESC, if PS allows
// DMScale  : Weight from the scale
//
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
// CHANGE HISTORY
//
// CRD 541144 Name: CJ Estrada Date: Jan 7, 2021
// CRD 517533 Name: Dennis Michol Gabutin Date: November 17, 2020
// CRD 503257 Name: CJ Estrada Date: Oct 2, 2020
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATE
#define _SMSTATE

#include "SMStateBase.h"
#ifdef _TRADESHOWDEMO
#define _TRADESHOWDEMO_SMSTATEINCLUDES
#include "..\TradeShowDemo\SMStateTSDemo.h"
#undef _TRADESHOWDEMO_SMSTATEINCLUDES
#endif

#include "SCOTSSFErrorObject.h"
DLLEXPORT extern SCOTSSFErrorObject     eoSSF; 

// Start CRD 145557
typedef struct  
{
	CString UPC;
	long Price;
	CString Status;
	CString AccessMethod;
}AUDIT_ITEMS;  
// End CRD 145557

typedef struct  
{
	CString csItemEntryId;
	CString csDescription;
	CString csItemCode;
	long lQuantityEntered;
	long lWeightEntered;
	long lTareCode;
	long lExtPrice;
	long lUnitPrice;
	long cScanType;
	CString csExcCategory;
	long lSecurityTag;
}EAS_ITEMS;  

// CRD 558065
typedef struct
{
	bool bSend;
	CString csRefNumber;
	CString csCaseNumber;
	long lCreditLimit;
	int iStatus;
	int iUseApprovedCard;
}CITIACCOUNTDETAILS; 

//////////////////////////////
#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
SMState : public SMStateBase
{

public:
  SMState();

#ifdef _TESTSSF 
    static int m_SSFMoneyCenterGiftCardAmount; // the amount selected from the Select Shoppin Card Amount
    static CString m_csGiftCardTrackData;	   // track data from CARD DISPENSER
    CString GetAccountNumber(void);				// extract account number
    bool IsValidShoppingCard(CString& cspCardData);  //Validate inserted card 
    bool IsCardDispenserOK();	//check the status of card dispenser
    bool WriteGiftCard(bool bNewCard); //Write to card dispenser
#endif

#include "SMStateDM.h"
#include "SMStatePS.h"
#include "SMStateSA.h"
#include "SMStateTB.h"
#include "SMStateUE.h"
#include "SMStateCU.h"
#include "SMStateRP.h"
#include "SMStateUSSF.h"
#include "SMStateRA.h"
	
#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMStateTSDemo.h"
#include "..\TradeShowDemo\SMStateTBTSDemo.h"
#endif

public:
    virtual BOOL SCOTDeleteFile(LPCTSTR lpSourceFileName);

    virtual void SaveLastReceiptData(HFILE hFile, CString csFileName); 
    virtual void SaveLastReceiptData(CString csFileName, CString csData);
    virtual void PrintLastReceipt(CString csReceiptFileName);
    virtual SMStateBase * PerformEOD();
    virtual SMStateBase *Initialize(void);
    virtual void  UnInitialize(void);
    virtual SMStateBase *TimedOut(void);
    virtual bool customerMode();
    virtual bool storeMode();
    virtual bool helpMode();
    virtual bool assistMode();
    virtual bool storeModeHelp();
    virtual bool inProgress();
    virtual SMStateBase *Parse(MessageElement*);
    virtual SMStateBase *IdleTimer(const long);
    virtual CString  CleanUpMessage(LPCTSTR, const bool);
    virtual void GetTrxLimitString(const PSTEXT nTextID, CString& csMsgText);

    virtual bool   restrictedItemNotAllowed();

    virtual PSTEXT restrictedItemMessage();
    virtual long CopyFileFromServer(LPCTSTR, LPCTSTR, const bool);
    virtual long CopyFileToServer(LPCTSTR, LPCTSTR);

    virtual SMStateBase* createLevel1Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);
    virtual SMStateBase* createLevel2Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);
    virtual SMStateBase* createLevel3Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);

    virtual SMStateBase *DMdf_EASNoMotion();
    virtual SMStateBase *PushStateFifoStack(SMStateBase *);
    virtual SMStateBase *OnCouponSensor();
    virtual SMStateBase *OnMotionCashOutSensor();
    virtual SMStateBase *OnNoMotionCashOutSensor();
    virtual SMStateBase *GenerateTrainModeIntervention(void);
    virtual bool InvalidCouponPrefix(CString&);
    virtual void SynchronizeTime(); 
    virtual bool ShrinkPicklistImages();
    virtual CString GetWideCharToMultiByte(const CString &csData);
    virtual SMStateBase* ReloadOptions(void);
    //virtual long DisplayAuxScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh = false, int nArgs = 0, ...);
    virtual SMStateBase* VerifyBirthDate(void);
    virtual BOOL VerifyBirthDate(CString csFromAnyDOB, CString csoperatorID, CString cspassword);
    virtual long GetAge(CString csFromAnyDOB);
    virtual SMStateBase* SAEndOfTransaction(void);
    virtual long ReloadSASIActions(void);
    virtual SMStateBase* CheckForDATFile(bool bReloadOption);
    virtual SMStateBase* OnMatchedWt(void);
    virtual SMStateBase* OnWtIncreaseNotAllowed(void);
    virtual SMStateBase* OnWtDecrease(void);
    virtual SMStateBase* OnBackToLGW(void);
    virtual SMStateBase* OnSecurityScaleFailure(int nFailureType);
    virtual SMStateBase* OnDBMgrFailure(long lStatusCode, long lStatusType);
    virtual SMStateBase* OnUnDeActivatedTheftTag(void);
    virtual SMStateBase* OnSkipBaggingAllowed(void);
    virtual SMStateBase* OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase* OnRAPApproveAndUpdateBtn(void);
    virtual SMStateBase* OnRAPApproveNoUpdateBtn(void);
    virtual SMStateBase* OnRAPLightItemBtn(void);
    virtual SMStateBase* OnRAPItemNotBaggedBtn(void);
    virtual CString GetDescFromView(int nView);
    virtual CString GetCategory(DWORD m_nRpID);
    virtual CString getSCOTStateName(void);
    virtual void UpdateStateName(CString csStateName = EMPTY_STRING);
    virtual SMStateBase* CkStateStackAndReturn(void);
    virtual void ClearStateStackEOT(void);
    virtual SMStateBase* OnDataBaseInfoComplete(void);
    virtual SMStateBase *HandleDataNeeded(void);
    virtual SMStateBase *HandleMSRData(void);
    virtual void Escalation2RAP(void);
    virtual SMStateBase* PSRemoteButton1(void);
    virtual void ScanScaleAttract(void);
    virtual void ScanScaleSB(void);
    virtual void TABTransportClearedItem(void);
    virtual void TABReEnableScannerAndClearWaitForApprovalState(void);
    virtual int GetButtonIndex(int nTenderType);
    virtual bool IsCashDeviceError(long lDeviceClass);
    virtual bool IsCardDeviceError(long lDeviceClass);
    virtual SMStateBase *EnterDegradedMode(DEGRADEDMODES lModeType); 
    virtual SMStateBase *ExitDegradedMode();
    virtual void SetStatePopped( bool bPopped );
    virtual bool GetStatePopped();
    virtual BEST getNextStateForGoodItem();
    virtual bool AreAnyRedApprovalsPending();
    virtual SMStateBase* HostOffLine(void);
    virtual SMStateBase* HostOnLine(void);
    virtual SMStateBase* OnTransactionComplete(void);
    virtual void OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation);
    virtual void OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation, LPCTSTR lpszCategory);
    virtual void TellApplicationStateToSecurity(LPCTSTR lpszOperation, LPCTSTR lpszStateName);
    virtual long ChangeSecurityMaintenanceWtFlags(LPCTSTR lpszOperation, LPCTSTR lpszUPC,LPCTSTR lpszFlag, bool bPriceEmbedded);
    virtual long ChangeSecurityMaintenanceTolerance(LPCTSTR lpszOperation, LPCTSTR lpszUPC, LPCTSTR lpszTolerance, bool bPriceEmbedded);
    virtual SMStateBase* ReturnDelayedStateForGCTender();
    virtual SMStateBase* TareWeightedItem(void);
    virtual SMStateBase* TareOnly(void);
    virtual SMStateBase* TareWLDBItem(void);
    virtual SMStateBase* TareNone(void);
    virtual SMStateBase* TareOnlyAtAttract(void);
    virtual void SetSecurityConfig(LPCTSTR lpszParamName, LPCTSTR lpszParamValue);
    virtual bool GetSecurityConfig(LPCTSTR lpszParamName);
    virtual CString GetSecurityConfigValue(LPCTSTR lpszParamName);
    virtual bool IsLoginBypassAllowed();
    virtual void   setAnchorState(BEST state);
    virtual SMStateBase* setAndCreateAnchorState(BEST state);

    virtual void DeliverAnchorState(SMStateBase *StatePtr);
    virtual void DeliverUpdate(BEST state);
    virtual bool IgnoreTBMessageInSecurityState();
    virtual bool IsForceDelayedApprovalBeforeSuspend();
    virtual SMStateBase *CouponMaxIntervention();
    virtual SMStateBase *CouponMaxInterventionFinal();
    virtual SMStateBase *PreTenderState(void);
    virtual void StartTimerForSkipBaggingBtn();
    virtual bool IsRemoteErrorAllowed(long lDeviceClass);
    virtual void SetStateBeforeHelp(CString &csLastName, CString &csNextStateName);

    virtual SMStateBase *ProcessTBTender(TBTENDERTYPE nTenderType);  
    virtual void DMSetTriColorLight( DMTriColorLightColor color, DMTriColorLightState state, DMTriColorLightRequestType request );
    virtual void DMTurnOffTriColorLight( void );
    virtual void ShowDoNotBagOnEReceipt(void);

    virtual SMStateBase *GenerateTrustLevelDataNeeded(void);
    virtual void SendPLAStartPickList();
    virtual void loadOptionsFromConfigObject();
    virtual SMStateBase *OnGenericDelayedComplete(void);
    virtual bool RAPAssistModeAllowed();

	virtual bool SCOTIsVisible();

	virtual void MeijerModifyScannedBarcode(void);

	// SA250050: overriden in OperatorState and SecOperatorPwState 
    virtual inline CString OperPassStateName() { return _T(""); } // return blank by default

	static bool m_bDataNeededAssistMode; //SOTF 6583 

	static bool m_bNeedYellowLightForMobile; // POS31054

	static bool m_bCouponInsertionBypassed; //POS33020
	static long m_lInsertedCouponsTotal;    //POS33020
	static bool m_bActivateHHScanner;   // POS181195, POS181197
	static bool m_bWICEBTSNBEnable;	// POS138636
	static long lWICOptionloaded;	// POS138637
	static bool m_bEODEnable;	 //POS97087
	static bool m_bSecurityDisabledForMPerks;
	static int nVoidCounts;   //VSTS 131568
	static bool m_bCustomVoidItemMessage; //VSTS 131568
	static bool mPerksActivatedInTransaction;

	static CString GetRAPConnectionName();	// POS181195
	static CString GetLaneName();		// POS181195
	
	void CashMonitoringCounts(int transactionState);  //POS244229
    bool CashRetrieveMap(CString csStrData,int nStringtype); //POS244229
    void CashArray(CString CashLConfig,int csstringtype);//POS244229
	
	static bool m_bAgeItemSoldInAM; //SSCOI-47774, flag for restricted item sold in AM
	static bool m_bIsLoyaltyProcessed; //154609 Lane hung after audit

	static bool bQuantityRequiredActive; // 140001 QTY Required

	static bool m_bResendFLMSitem; // RFQ 9545 property to tell FLMS RecallItems to resend item code
	
	static bool m_bTMDAccepted; //CRD 168992

	static bool bTMDVoided;
	static bool m_bMobileTransactionStarted; //VSTS170002

	
	static bool bHighValueActive;

	static bool bRAPPrinterBitmapStatus;
	static bool bSCOPrinterBitmapStatus;
	static CString csMeijerLogoFilePath;

	static CString GetLastBarcode();
	static CString GetLastBarLabel();
	static CString GetLastBarType();
	static CString m_csTransactionMode;	// CRD126015 transaction mode

	//Start CRD 145555
	static CString csFLMSType;
	static CString csFLMSmPerksBarcode;
	static CString csFLMSAuditResult;
	static CString csMinimumScanRequired;
	static CString csFLMSAuditStartTime;
	static CString csFLMSAuditEndTime;
	static CString csFLMSOperatorID;
	static CString csFLMSSessionID;

	static CArray <AUDIT_ITEMS*, AUDIT_ITEMS*> csFLMSAuditItemList;	//CRD 145557

	void sendFLMSTLog(int iStartEnd, int iPassFailCancel);
	//End CRD 145555

	static bool bDoNotSendStatusRequest; //CRD 1890706
	//Start CRD 185620
	static CMap <int, int, bool, bool> SMState::g_mapTransactionEASItem;	//CRD 142210
	static long lUnapprovedEASItems;
	virtual SMStateBase *OnGenericDelayedIntervention(CDataNeededUtil &dataNeededUtil); 
	static bool bEASApproveFromVoid;

	static EAS_ITEMS sEASItem;
	static bool isPlayedAssistanceAudio;
	static long sProcessingDelayedIntervention;
	
	//End CRD 185620
	static bool bFinishSendingSuspendBarcode;	//CRD 177093
	static bool bFinishRecall;	//SSCOI-55092
	
	static bool bIsItemizationAfterSkipBaggingSent;	//SSCOI-55023
	static long lUnapprovedMobileGenericDelayed;	//CRD 223409
	static bool bIsGenericDelayedProcessed;	//CRD 223409
	static bool bGenericDelayedFinishSent;	//CRD 232271

	bool checkEASItemsList();
	void appendEASMsgToReceipt(long lEntryId);
	void addItemToEASREceipt(CString csEntryId);
	bool checkMobileAssistanceNeededItems();

	static bool g_bIsProcessingEverseenAlert; //CRD 347846

	static CString SS90SupportMeijerCom;
	static CString ShiptBarcodePrefix;
	static CString ShiptBarcodeSuffix;
	bool IsShiptBarcode(CString barcode);
	static bool bResumeCompleteWaitingForItemMenu; //CRD 503257

	void NotifyExitStoreMode(); //CRD 366323
	static CTime StoreAssistRequestedTime;

	static CString csApplyMeijerCardGUID;	//CRD 525312
	static CITIACCOUNTDETAILS sCitiAccountDetails;	// CRD 558065
	static bool bUnapprovedAccountLookupFailed;	// CRD 554916
	static bool validatingMeijerCardWebApp;		// CRD 574057
	static bool bApplyMeijerStartedFromAttract;	// CRD 537481
	void LoadApplyMeijerCardOptions(bool bApplyMeijerComplete);			// CRD 564271

protected:

    virtual void CutReceipt(void);

    virtual void SaveSigReceiptData(BSTR pSignatureData);
    virtual void SaveReceiptData(void); 

    virtual SMStateBase *OnNeedData(CString szDescription, CString szView, CString szIniFile=_T(""), CString szSection=_T(""));
    virtual SMStateBase *GenerateRAPDataNeeded(CString sDescription, CString sView, CString sINI=_T(""), CString sSection=_T(""));
    virtual SMStateBase *HandleDataRequest();
    virtual SMStateBase *GPNotify(const GPEVT);
    virtual SMStateBase *PopStateFifoStack(void);
    virtual bool isAnySoldVoidedItems(void);
    virtual bool AreThereAnySoldVoidedItems(void);
    virtual bool isBarcodeValidOperatorPassword(const CString csItemCode, CString *pcsOperatorID = NULL);
    virtual void ReadOperatorLoginBarcodeMask(CString OperatorLoginBarcodeMask);
    virtual void InvalidOperatorLoginBarcodeMask(CString csOperatorLoginBarcodeMask);
    virtual bool isRestrictedItemApprovalRequired(void);
    virtual bool isVisualItemApprovalRequired(void);
    virtual bool isSecurityApprovalRequired(void);
    virtual bool isCouponRatioApprovalRequired(const bool=false);
    virtual bool isVoidRatioApprovalRequired(const bool=false);
    virtual bool isAnyApprovalRequired(const bool=false);
    virtual void ResetAllTransactionValues(void);
    virtual void ResetAllTenderValues(void);
    virtual long PrintReceipt(HFILE, const bool=true);
    virtual long PrintJournal(HFILE);
    virtual CString CustomerName(LPCTSTR trackData); 
    virtual CString GetAccountNum(LPCTSTR trackData); 
    virtual void UpdateDepositedCouponVars();
    virtual SMStateBase *DepositCouponsIfNecessary();
    virtual SMStateBase *SetISIPTimeOut(int nMilliSec);
    virtual int GetISIPTimeOut();
    virtual bool isAnyNonImmediateApprovalRequired(void);

    virtual CString GetApprovalNeededString(void);
    virtual bool WriteSCOTStatusToFile(CString cStatus);
    virtual bool OpenSCOTStatusFile(CString cFileName);
    virtual SMStateBase* DMTakeawaySendScaleStatus();
    virtual SMStateBase* DMTakeawayAMExitDone();
    virtual TenderButton &GetTenderButton(int iIndex = -1);
    virtual void SetTenderButton(int iIndex = -1);
    virtual int  AdjustTenderButtonIndex(TBTENDERTYPE nAMTender);
    virtual void SetNextGenUICurrentItem(CString = _T(""), CString = _T(""), bool = false, bool = false, int = 0);
    virtual void SetNextGenUIMode(void);
    virtual bool SetItemGDAsyncIDReceiptVariable(const CString& csReceipt, CString& csItemID, CString& csGDAsyncID);
    virtual bool isItemGenericDelayed(CString csItemID, CString &csGenDataNeededID);
	virtual bool isProcessingGenericDelayedIntervention();

	//Start CRD 291249
	virtual bool DigitalCouponActiveWithNoItems (void);         // SOTF6400
    virtual bool IsTrxnCompletelyClean(void);   // SOTF7555
	//End CRD 291249

//#ifdef _SSCOMOBILE
    virtual void InitializeSSCOMobile();
    virtual void UnInitializeSSCOMobile();
    virtual SMStateBase *SellRecalledItem();
    virtual int GetUnsolictedDataBarcodeType(CString csData);
    virtual void SetInRecalledModeReceiptMessage(PSTEXTMSG nReceiptMsg, PSTEXTMSG nInstructionText);
    SMStateBase * ParseMobileSSF(MessageElement *me);
    SMStateBase * OnMobileErrorEvent(CString csMessage);
	
	void    SetTransactionStatus(_bstr_t bstrTransactionStatus);
    _bstr_t GetTransactionStatus();
	
	static long m_lSavedforMobileTotal;
	static long m_lSavedforMobileTransNumber;


	static _bstr_t m_bstrTransactionStatus;
	
    static CQueuedActionsContainer m_ExternalInterventionQueue;  //Queue of Dataneeded requests that cannot interrupt SSCO/POS flow.

	static bool m_bNeedImmediateApproval;
	static long m_lLastSuspendTotal;
    static bool m_bMJPlayedWelcome;
    static bool m_bMJPlayedUsePinpad;
	static bool m_bNeedScanReceipt;	
	static bool m_bIsFromScanReceipt;
	static bool m_bIsFromPaymentGoback;
	static bool m_bIsNewTrxSelected;
	static bool m_bIsFromDisplayingResumeItem;
	static bool m_bMJWtIncreaseAtSmCmDataEntry;   //TAR335785
	static bool bVoidApprovalDelayed;  //VSTS 131568

	static bool m_bIsNoSale;
	static bool m_bIsSuspendFromRap;
	static long m_lUnApprovedRestrictedItemsDuringCancel;  //sotf 6226
    static bool m_bGoBackAfterAgeRestrictedItem;           //sotf 7384

    //bool CashRetrieveMap(CString csStrData,int nStringtype); //POS244229
    //void CashArray(CString CashLConfig,int csstringtype);//POS244229
	static bool m_bShopAndDropEnabled;	// CRD 125377
	static bool m_bSNDmPerkEntered;		// CRD 125377
	static bool m_bSNDTMDEntered;		// CRD 125377
	static bool m_bSNDAgeEntered;		// CRD 125377
	static bool m_bSNDAgeEnteredFound;	// CRD 125377
	static bool m_bSNDAllowItemMenu;	// CRD 125377
	static bool m_bSNDClearUnknownItemC;// CRD 125377
	static bool m_bSNDmPerksCancel;		// CRD 125377
	static bool m_bSNDExitedAM;			// CRD 125377
	static bool m_bSNDSettingAge;		// CRD 125377
	static bool m_bSNDxCancelTrans;		// CRD 125377
	static bool m_bSNDBackToFnP;		// CRD 125377
	static long lSDNRestrictedAgeTimerAmt; // CRD 125377

	
	bool BoolValue(CString csVal);
	void LoadMeijerBiometricOptions();
	void LoadEverseenOptions();	//CRD 313028
	void SendCitiAccountDetailsToPOS(CString csRefNumber, CString csCaseNumber, long lCreditLimit, int iStatus, int iUseApprovedCard);	// CRD 541144
	bool ProcessCitiApplicationDetails();	// CRD 557225

	static BEST nPreviousState; // SSCOI-45180

    static CString ms_MobileAuditDataEntryINI;			// CDR126194
    static CString ms_MobileAuditDataEntryINISection;	// CDR126194
//#endif _SSCOMOBILE

    DECLARE_DYNCREATE(SMState) // MFC Runtime class/object information
};

#endif

