	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// FILE:  SMStateBase.CPP
	//
	// TITLE: Class implementation file for base class SMStateBase
	//
	// All states ultimately inherit from SMState.
	// This class is an abstract class (it has at least one
	// pure virtual function and can't be instantiated.)
	//
	// This class provides default processing for
	// all possible messages to be parsed. Message
	// handlers are usually implemented in deriving state
	// but if not they end up here and are usually
	// ignored.
	//
	// The functions in SMState are implemented here and in
	// SMStatePS.cpp, SMStateDM.cpp, SMStateTB.cpp and SMStateSA.cpp
	//
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	#include <stdafx.h>			    	     // MS MFC always needed first
	#include <afxtempl.h>			         // MS MFC template header
    #include <vector>  //tar 381428

	#include "Common.h"			    	     // Common includes
	#include "SMState.h"

   // for BaseState()
	#include "SMStart.h"
	#include "SMAttract.h"
	#include "SMScanAndBag.h"
	#include "SMKeyInCode.h"
	#include "SMTakeCard.h"
	#include "SMSignCharge.h"
	#include "SMFinish.h"
	#include "SMThankYou.h"
	#include "SMEnterCoupons.h"
	#include "SMOutOfService.h"
	#include "SMContinueTrans.h"
	#include "SMSelectFromList.h"
	#include "SMLookUpItem.h"
	#include "SMProduceFavorites.h"
	#include "SMSystemMessage1.h" //TAR 279371
	#include "SMSystemMessage4.h"
	#include "SMSystemMessage5.h"
	#include "SMSystemMessage6.h"
	#include "SMDepositCoupons.h"
	#include "SMDropoffCoupons.h"
	#include "SMSelectCardType.h"
  	#include "SMSelectContainer.h"
  	#include "SMContextHelp.h"
  	#include "SMCardMisRead.h"			//TAR119740	LPM100199  must return to system msg after call for help
	#include "SMSelectTare.h"
	#include "DMProcedures.h"
  	#include "SMCouponNotAllowed.h"		//TAR102019 LPM120799  must return to coupon not allowed screen after call for help
    #include "SMRequestSig.h"
    #include "SMEnterQuantity.h"
    #include "SMEnterPin.h"
    #include "SMEnterWeight.h"
    #include "SMEnterWtForPriceEmbedded.h"
	#include "SMSmMediaStatus.h"
	#include "SMSmSystemFunctions.h"
	#include "SMLaneClosed.h"
    #include "SMEnterCouponValue.h"  // TAR 164014
	#include "SMCouponTooHigh.h"     // TAR 164015
	#include "SMCrateableItem.h"
    #include "SMBagAndEAS.h"		//T.I.D
    #include "SMPickingUpItems.h"   //T.I.D
    #include "SMVoidItem.h"
    #include "SMVoidNoMatch.h"
    #include "SMConfirmVoid.h"
	#include "SMSecMisMatchWeight.h" //T.I.D
	#include "SMSecUnexpectedDecrease.h"
	#include "SMSecUnExpectedIncrease.h"
	#include "SMAcknowledgeSig.h"
	#include "SMCardProcessing.h"
	#include "SMSmConfirmSignature.h"
	#include "SMGetFoodStampState.h"
	#include "SMTakeReceipt.h"

    #include "SMVoidApproval.h"
    #include "SMSetAllItemsAside.h"
    #include "SMVoidTransApproval.h"
    #include "SMConfirmAbort.h"
    #include "SMInsertCoupon.h" //for coupon sensor

	#include "SMLaneClosed.h"
	#include "SMOutOfService2.h"
	#include "SMSecBagBoxThreshold.h"
	#include "SMSecItemRemovedThreshold.h"
	#include "SMSecSkipBaggingThreshold.h"
	#include "SMSecNewItemEntry.h"
	#include "SMCashPayment.h"
	#include "SMCheckBasketReminder.h"
	#include "SMSmAuthorization.h"		//Tar 194454 RF020702
	#include "SMTransportItem.h"
	#include "SMSecBagAreaBackup.h"
	#include "ncrpid.h"
	#include "SMWaitForApproval.h"
    #include "SMWaitForRemoteAssistance.h"
    #include "SMRAPDataNeeded.h"
    #include "SMSecurityLevel3.h"
	#include "SMEnterAlphaNumericInfo.h"
    #include "SMUnDeActivatedItemApproval.h"
    #include "SMItemNotForSale.h"
	#include "SMUnknownItem.h"
	#include "SMTimeRestrictedItem.h"
	#include "SMItemQuantityExceeded.h"
	#include "SMTakeLoyaltyCard.h"
    #include "SMCmDataEntry.h"
	#include "SMAssistedTender.h"
	#include "SCOTTakeawayBelt.h"		// for TAB
    #include "SMSmDataEntry.h"
	#include "SMVoucherPayment.h"
	#include "SMHostOffLine.h"
	#include "SMTenderBalance.h"
	#include "SMDepositGiftCard.h"
	#include "SMInsertGiftCard.h"
	#include "SMCollectGiftCard.h"
	#include "SMCouponNoMatch.h"		//TAR 265390
	#include "SMSelectLanguage.h"
	#include "SMExtensionState.h"       //TAR 312345
	#include "SMCustomMessage.h"		//TAR 277900 Migrated-BM185018
    #include "SMInProgress.h"
	#include "SMSmPrintReceipt.h"
	#include "SMPutBagOnScale.h"//SR085 - added for SR085
	#include "SMSecBagBoxNotValid.h"//SR085 - added for SR085
  #include "SharedSendScot.h" //dp185016 tar 287778 !this file has been added in
                              //$../development/platform/utility/sendscot/
                              // and will also placed/linked in c:\scot\adk\include
	#include "RemoteMessageAPI.h"	// External library ...\platform\RemoteMessage
	#include "VolumeControl.h"
	#include "SMSellBags.h" //SR713
	#include "SMDegradedContinueTrans.h"	//degraded mode tars

//USSF Start
#include "USSFProcedures.h"
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF End

	#define COMP_ID ID_SM			         // Base state component
	#define T_ID	_T("SMStateBase")
	DLLEXPORT extern CSCOTDataEntry cDataEntry;

	IMPLEMENT_DYNCREATE(SMStateBase, CObject)// MFC Runtime class/object information

	extern SMStateBase* pgState2Push;	// 179195 & 183721 LPM091901

	bool bHasToldRAPAboutBeginCashTender = false;

	bool bReportServerOnline = true; //RFC 262343


//---------------------------------------------------------------------
	// Static data member definition
	//---------------------------------------------------------------------
	CPSX   *SMStateBase::m_pAppPSX = NULL;       // NewUI
	int	    SMStateBase::nLastMsgID = 0;        // last message element ID MP received
	int		SMStateBase::nPsxRedrawCount=0;		// PSX's redraw logic has change, app needs to keep track

	CString SMStateBase::csOperator = EMPTY_STRING;       // operator ID for store mode
	CString SMStateBase::csPassword = EMPTY_STRING;       // operator password for store mode
  CString SMStoreModeStateBase::csCashierID = EMPTY_STRING; //dp185016 support glory device-Part 2
    bool    SMStateBase::ms_bIsCurrentItemFromPickList = false; // Current item is a PickList item
    bool    SMStateBase::fInMultiSelectPickList = false;
    bool    SMStateBase::bIsFromHostOffline = false; //from HostOffline state
    bool    SMStateBase::m_bTrxEnded = false;       // already entered TB_TRXEND T/F
    SMStateBase* SMStateBase::m_pSavedStateInTendAccepted = STATE_NULL; //save the state after tender accepted
    bool    SMStateBase::fStateFreeze = false;  // stay in current state T/F
    bool    SMStateBase::fStateAssist = false;  // stay in current state T/F
    bool    SMStateBase::fInSignedOn = false;   // now signed on T/F
    bool    SMStateBase::fInSignedOff = false;  // now signed off T/F
    bool    SMStateBase::fInSuspendTrans = false; // suspend entire order T/F
    bool    SMStateBase::bFromAssistMode = false;   // coming from Assist Mode to System Functions T/F
    SCOT_LANGUAGE_TYPE SMStateBase::m_LanguageUsedPreviously = SCOT_LANGUAGE_PRIMARY;  //Language from previous transaction  

	bool SMStateBase::bIsCouponExceedLimitFinalization = false;	//SR678

	//Tar 124345
	//bool  SMStateBase::bReadyForTakeReceipt = true; //Tar 126024
	//bool  SMStateBase::bTrxClosed = false;
	bool	SMStateBase::fFromRequestSignature = false;	// disable timeout feature if come from signature needed screen
	bool	SMStateBase::fInVoidTrans = false;   // canceling entire order T/F
	bool	SMStateBase::fDoOptions = false;      // need to load options T/F
	bool    SMStateBase::fDoEOD = false;
	bool	SMStateBase::fDoLogsTo4690 = false;  // need to copy log files to 4690 T/F
	bool	SMStateBase::fDoCashBack = false;    // cashback was due T/F
	bool	SMStateBase::fItemIsWaiting = false; // last item was scanned in Attract or FTU
	bool	SMStateBase::fApprovedForRestrictedItems = false;
	bool	SMStateBase::fAgeObtained = false;
	int	    SMStateBase::nCustomerAge = 0;	      // the default customer age
	bool	SMStateBase::fTrxVerif = false;       // Transaction Verification
	bool	SMStateBase::fRequestForHelp = false; // customer asked for help
	bool	SMStateBase::fCantScanCoupon = false; // customer can't scan a coupon
	bool    SMStateBase::fUnDeActivatedItem = false; // Num of undeactivated item on belt
	bool    SMStateBase::fNewItemScannedSinceLastUnDeActivatedItem = false;
	bool    SMStateBase::fBeyondFinishAndPayState = false;
	bool    SMStateBase::fTABSafetyCoverOpenApprovalPending = false;
	bool	SMState::fInATMTransaction = false;
	bool	SMState::fInLoyaltyCardProcessing = false;	// whether TB has any loyalty card data to process
	bool	SMState::fLoyaltyCardAwaitVerification= false;
	bool	SMState::bTrxLimitExceed = false;
	bool	SMState::bTenderLimitExceeded = false;
	bool	SMState::fLoyaltyCardTrx = false;
	bool	SMState::bAlreadyPlayedWelcome = false;
	bool    SMState::bAlreadyPlayedLCReminder = false;	//Tar 200719 BZ 04/02/02
    bool    SMState::fOtherPayment = false;       // other payment initialize false
    int		SMState::nTrxJournalLength = 0;
    int		SMState::nTrxReceiptLength = 0;
    bool    SMState::bLaneClosed = false;
    bool    SMState::bEnterTenderState = false;
    bool	SMState::bInApprovalNeeded = false; //TAR 279092
    bool    SMState::fTenderPending = false;       //RFC 262343
	bool	SMState::bLaneClosedTamperOut = false; //Tar 279371

    bool	SMStateBase::waitingForCardRemoval = false;
	bool	SMStateBase::fPutSCOTinLC = false;
	bool	SMStateBase::fStopSCOT = false;
	bool	SMStateBase::fCancelSuspend = false;
	bool	SMStateBase::bSayAudioAssistedTender = false; //Tar 353054

	bool	SMStateBase::bCompletePrintAndCut = false; //TAR 325292
	bool	SMStateBase::bReprintReceiptFailed = false;

    CString	SMStateBase::csLastDisplay = EMPTY_STRING;
	CString SMStateBase::csTBMessage2Scot = EMPTY_STRING;

	int		SMStateBase::nLastList = 0;	     // last lookup list selected
    int		SMStateBase::nLastDay = 0;       // last day run EOD
	int		SMStateBase::nPreviousDay = 0;
    // BeginCodeWLDBSA11
    bool	SMStateBase::fInWLDBProcess=FALSE;	   // waiting for WLDB to authorize again to enable Scanner
    bool	SMStateBase::fScannerRequested=FALSE;  // this state request Scanner
    // EndCodeWLDBSA11

	// Futher need to look into side effects, need to initialize all elements of array?
	// Found a side effect while working.
	SMStateBase* SMStateBase::sStateFifo[MAX_FIFO_STACK];	// system message fifo stack for Attract

	int	  SMStateBase::aStateFifo = 0;
	int	  SMStateBase::uStateFifo = 0;

	BEST SMStateBase::anchorState = BES_START; // last base entry state
	BEST SMStateBase::TABstateBeforeTransport = BES_EMPTY_STATE; //TAR221089 CHU 112502
	BEST SMStateBase::nSellState = BES_START;  // last sell entry state

    TBTENDERTYPE SMStateBase::nTenderType = TB_TENDER_NONE;	// doing any tender type, no // TAR 107134

	long SMStateBase::lItems = 0;		     // # of line items in trans
	long SMStateBase::lRestrictedItems = 0;  // # of restricted items in this trans
	long SMStateBase::lVoidedItems = 0;	     // # of voided items in this trans
	long SMStateBase::lCoupons = 0;		     // # of coupons in this trans
	long SMStateBase::lDepositedCoupons = 0; // # of coupons deposited
	long SMStateBase::lRegularYellowNeedingApproval = 0;
	long SMStateBase::lUnapprovedRestrictedItems = 0;   // rest'd items not yet approved
	long SMStateBase::lUnapprovedVoidedItems = 0;	// void items need approval
	long SMStateBase::lUnapprovedSecurityAlerts = 0; // # security alert need approval
	long SMStateBase::lUnapprovedCoupons = 0; // coupons need approval
	long SMStateBase::lExceptionItemsRequiringApproval = 0; //exception items which require approval
	long SMStateBase::lNotForSaleItems = 0;
    long SMStateBase::lTimeRestrictedItems = 0;
    long SMStateBase::lQuantityRestrictedItems = 0; //TAR#126778 quantity restricted items should require approval
	long SMStateBase::lUnknownItems = 0;
	long SMStateBase::lUnknownItemsKeyed = 0;	//used to tract keyed unknown items TAR#120988	LPM091499
	long SMStateBase::lVisualVerifyItems = 0;
	long SMStateBase::lUnapprovedVisualVerifyItems = 0;
	long SMStateBase::lUnapprovedHopperSubstitution=0;
	long SMStateBase::lPriceRequiredItems = 0;
	long SMStateBase::lRestrictedNotAllowedItems = 0;
    // 3.1 req 4.4.5 new store-mode flags to track GoBack(now Store Authorization) button on delayed interventions
	bool SMStateBase::bStoreModeGoBack = false;
    bool SMStateBase::bStoreModeApprovals = false;
	bool SMStateBase::bForceApprovalBeforeSuspend = false; //TAR 364905

    long SMStateBase::lCoinVoucherApproval = 0;      // Added for Reports function - customer specific use
    long SMStateBase::lGiftCertificateApproval = 0;  // Added for Reports function - customer specific use
    long SMStateBase::lUserDefinedApproval1 = 0; // Added for Reports function - customer specific use
    long SMStateBase::lUserDefinedApproval2 = 0; // Added for Reports function - customer specific use
    long SMStateBase::lUserDefinedApproval3 = 0; // Added for Reports function - customer specific use
    long SMStateBase::lUserDefinedApproval4 = 0; // Added for Reports function - customer specific use

	long SMStateBase::lVoidQty = 0;		 // qty of item selected for void
	long SMStateBase::lVoidWgt = 0;		 // wgtof item selected for void
	long SMStateBase::lVoidTcd = 0;		 // tare of item selected for void
	long SMStateBase::lVoidPrc = 0;		 // extended price of item selected for void
	long SMStateBase::lVoidUPrc = 0;	 // unti price of item selected for void
	long SMStateBase::lVoidBarcodeType = 0; // item scan code type for void
    bool SMStateBase::m_bIsVoidByScanning = false;

	int	 SMStateBase::nISIPTimeOut = 0;
    int  SMStateBase::nTimeSinceLastHeartbeat = 0;
    //Tar 122723
    bool SMStateBase::bFinish = 0;              // if Finish and Pay is hit

	CString	SMStateBase::csChargeAccount = EMPTY_STRING;  // charge account
	CString	SMStateBase::csChargeExpiry = EMPTY_STRING;   // credit card expiry data YYMM
	CString	SMStateBase::csItemSent = EMPTY_STRING;       // last item code we TRIED to sell
	CString	SMStateBase::csLookupItemCode = EMPTY_STRING; // last item code we TRIED to lookup

	CString	SMStateBase::csBirthDate = EMPTY_STRING;      // last Birthdate input
	CStdioFile SMStateBase::csfReceipt;	        // temporary receipt holding file

	// all these totals come from SMA in cents via the display
	// all are in cents and are +ve amounts
	CString SMStateBase::csErrDescription = EMPTY_STRING;
	long SMStateBase::lErrDescriptionLength = 0;
	long SMStateBase::lTaxDue=0;			// taxes
	long SMStateBase::lChangeDue=0;		    // change due to customer
	long SMStateBase::lBalanceDue=0;		// balance due from customer
	long SMStateBase::lTotalSale=0;		    // total amount of items in sale
	long SMStateBase::lTotalPaid=0;		    // total of all tenders
	long SMStateBase::lLastTender=0;		// last tender amount in cents
	long SMStateBase::lCouponValue=0;	    // from B064 message
	long SMStateBase::lVoidedCoupons=0;	    // number of voided coupons
	long SMStateBase::lItemSalesAmountVoided=0;	// total amount voided (positive)
	long SMStateBase::lCouponAmountVoided=0;    // total coupon amount voided (positive)
	long SMStateBase::lTotalCouponAmount=0;     // total coupons
	int SMStateBase::nCountPossibleDuplicateCoupon=0;	// Number of times for duplicate coupon
	long SMStateBase::lFoodStampsDue=0;		    // total amount of food stamp eligible items
	bool SMStateBase::m_bIsFromLaneClosed=false;	// from LaneClosed state
    bool SMStateBase::m_bIsAttendantModeChanged=false;//TAR371932 & 371918
	long SMStateBase::lEBTAvailableBalance=0;	// total amount Available for EBT
	int SMStateBase::nPINTries=0;	            // Number of times customer entered a PIN
    bool SMStateBase::fCashBack = false;
    bool SMStateBase::bSigCapInProgress = false;
	bool SMStateBase::fSSWtFromAttract = false;	// TAR123172 only used if scan scale wt increase from attract screen LPM101499
	long SMStateBase::lCouponsInAssist = 0;		//102019
	bool SMStateBase::IsEncryptorSetupFail = false; // flag Y/N for EncryptorSetup Fail
	bool SMStateBase::bBadCard = false;             //Tar193038: Set to false for initial value


	// remote approval
	bool SMStateBase::bDropoffRequired = false;
	bool SMStateBase::bDropoffApproved = false;
	bool SMStateBase::bSuspendRequested = false;
    bool SMStateBase::bSuspendApproved = false;
	bool SMStateBase::bApplicationErrorApprovalRequired = false;

	bool SMStateBase::bPartialTenderWithFoodStamps = false;

	CString SMStateBase::csLastNonHelpStateName; // for SMContextHelp
	CString SMStateBase::csLastNonHelpStateBMP;	 // for SMContextHelp
	int  SMStateBase::nLastNonHelpStateTimeOut;	 //
	bool SMStateBase::fLastNonHelpStateStoreMode;
	HFILE SMStateBase::hReceiptFile = NULL;
	HFILE SMStateBase::hJournalFile = NULL;
	HFILE SMStateBase::hSlipFile = NULL;

	bool SMStateBase::bSuspendFailure = false;  //Tar 126382
	bool SMStateBase::m_bReloadingOptions = false; // TAR 352145
	bool SMStateBase::bValidScannedOperator = false; //TAR397106
        bool SMStateBase::g_bSkipBaggingButtonTimerExpires = false; //Sr778

	//Security Maintanance Mode (SMM)
    bool SMStateBase::bSecMaintMode = false;
    bool SMStateBase::bSMMPreScanState = true;	// set to false when item scanned in SMM
	long SMStateBase::lSMMBagScaleEvent = 0;	// incremented by bag scale event from security in SMM
	bool SMStateBase::bSMMFailure = false;		// set to true when Security notifies us it is down
	bool SMStateBase::bSMMFailureActionCancel = false;		// set to true when Security notifies us it's down in the middle of an update action
	long SMStateBase::lSMMglWtExp;				// Global wt expectation flag (no=0, yes=1, default=2) from security in SMM
	long SMStateBase::lSMMglWtSubCk;			// Global wt substitution check flag (no=0, yes=1, default=2) from security in SMM
	long SMStateBase::lSMMglWtTol;				// Global wt tolerance value(value or default = -1) from security in SMM
	long SMStateBase::lSMMglPPWUTol;			// Global PPWU tolerance value(value or default = -1) from security in SMM
	bool SMStateBase::bIsSMMitemTolPPWU;		// is item a PPWU (for tolerance info) from security in SMM
	bool SMStateBase::bIsSMMItemAdded = false;	// is item added to WLDB by security.
	bool SMStateBase::bIsSMMItemDeleted = false; // is item deleted from WLDB by security.
	long SMStateBase::lSMMitemWtExp;			// Item wt expectation flag (no=0, yes=1, default=2) from security in SMM
	long SMStateBase::lSMMitemWtSubCk;			// Item wt substitution check flag (no=0, yes=1, default=2) from security in SMM
	long SMStateBase::lSMMitemWtTol;			// Item wt tolerance value(value or default = -1) from security in SMM
	bool SMStateBase::m_bSecViolationFromAttract = false;  //383748

	bool SMStateBase::bWLDBNotActive = false;	// TAR #142007 to allow SMM to work if WLDB not active
	bool SMStateBase::bOkToDisplayBagScaleInfo = false;	//TAR164827 don't let bag scale wts update the echo box in SMM screens LPM030101
	bool SMStateBase::bgAlreadySubmittedWithOverride = false;
	bool SMStateBase::bEASReEnableScanner = true; // EAS event processed, OK to enable the scanner scale

	CString SMStateBase::CustomerMessage = EMPTY_STRING;
	long SMStateBase::lTrxTotalSavings = 0;
	long SMStateBase::lTrxTotalRewardPoints = 0;
	bool SMStateBase::bPressHelpOnWay = false;
	bool SMStateBase::bAlreadyInSnB = false; //fix screen blinking
	CString SMStateBase::csSavedSecMessage = _T(""); //fix screen blinking
	bool SMStateBase::bIsScaleBroken = false; //tar 183854
	bool SMStateBase::bIsSysMsgScreenState = false; //tar 185972
	bool SMStateBase::bIsFromVoidApproval = false; //in Void Approval screen
	bool SMStateBase::bCancelTransApproval = false; // TAR 298096 - to indicate cancel entire transaction
	bool SMStateBase::bQuietNeedSecApproval = false; //if need security approval when in quiet mode
	bool SMStateBase::bIsTransactionStarted = false; //TAR 219278 CHU 110602 If TBStart() has called: set it to True
    int  SMStateBase::nAdvanceUnexpectedIncreaseCnt = 1; // 3.2 patch B - RFC 257450
    int  SMStateBase::nAdvanceMismatchCnt = 1; // 3.2 patch B - RFC 257499
    SCOT_LANGUAGE_TYPE SMStateBase::m_customerLanguage = SCOT_LANGUAGE_PRIMARY;
	int  SMStateBase::m_View = DATANEEDED_NOVIEW; //tar 252987
	int  SMStateBase::m_PrevView = DATANEEDED_NOVIEW; //tar 252987
	CString SMStateBase::m_sButton = EMPTY_STRING;       // String of button clicked by RAP operator
    CString SMStateBase::m_sInput = EMPTY_STRING;        // Input received from RAP operator
	int  SMStateBase::nSkipBaggingBtnPresses = 0;		//the number of Skip Btn Button Press
	int  SMStateBase::nItemRemovedBtnPresses = 0;	//the number of Done Removing Button Press
	int  SMStateBase::nBagBoxBtnPresses = 0;
	long SMStateBase::lEOT_LGW = -999;								// end of transaction last good weight
    bool SMStateBase::g_bMatchedWtWaitingOnEAS = false;  // Also used to wait for Takeaway Belt ready event - KP
	bool SMStateBase::m_bEASHappened = false;			// cmal121002 a motion no motion pair occured after an item purchase
	bool SMStateBase::bToldRAPMediaLow = false;

	// TAR196659 PUSHING AND POPPING -  changes to use one stack per state LPM022502
	SMStateBase* SMStateBase::g_pDelay_WaitForApprovalStateChange = NULL;
	SMStateBase* SMStateBase::g_pDelay_TBExpectedStateChange = NULL;
	bool SMStateBase::g_bGLOBALScanAndBagInitializeDone = false; // to not disable devices that have not been enabled yet when S&B initialized/uninitialized
	bool SMStateBase::g_bSnBScreenIsShown = false;				// sometimes can pop S&B but the global bAlreadyInSnB is wrong, need another flag
	bool SMStateBase::g_bgTrxVerif = false;		// for WaitForApproval delayed when creating it the 2nd time
	bool SMStateBase::g_bgVoidTrx = false;		// for WaitForApproval delayed when creating it the 2nd time
	bool SMStateBase::g_bgVerifySig = false;		// for WaitForApproval delayed when creating it the 2nd time
	bool SMStateBase::g_bgCheckCashChanger = false;  //Tar 256141
	bool SMStateBase::g_bAllowEnterRemoteAssistMode = true;
	bool SMStateBase::g_bSafetyCoverClosed = true;	//when TAB safety cover opens and closes, App can't check health on the device yet  LPM040502
	bool SMStateBase::g_bZeroWtExpected = false;    //197477(28) TAB
    ENDORSE_STATE SMState::fInEndorsementState = ENDORSE_NONE;
    bool SMState::ms_bKeepRemoteDevices = false;
	static bool bAlreadyDoneOnce = false;			//TAR197477(30)
	bool SMStateBase::g_bAllowRemoteBtn1 = true;
	bool SMStateBase::g_bAssistModeDataNeeded = false;
	bool SMStateBase::g_bReadyForItem = false;					//TAR 363023

	bool SMStateBase::bDoNotResetTimeOutOnDMEvent = false;  // TAR 200294 CMal and NN 041602 Do not reset the timeout when DMEvent is fired
    int SMStateBase::nEscalationRAPTimeOut = 0;           // RJF 5/28/02 RFC 202655 & 202656

	int SMStateBase::nOperatorStoreLoginBarcodePasswordLength=0;            //FZ20020730 NN RFC 187993
	int SMStateBase::nOperatorStoreLoginBarcodeUseridLength=0;              //FZ20020730 NN RFC 187993
	CString SMStateBase::csOperatorStoreLoginBarcodeSignature=EMPTY_STRING; //FZ20020730 NN RFC 187993
	CString SMStateBase::csOperatorStoreLoginBarcodeMask=EMPTY_STRING;      //FZ20020730 NN RFC 187993

	bool bWriteEndOfTransactionToWLDBlog = false;		//NN RFC 210177 Write "End of transaction" to mismatch log only if mismatch happened during the transaction
	extern CString csGLOBALAddThe128Prefix; // CM/NN 090502 TAR 214029
	CString SMStateBase::g_csSavedDeptInfo = EMPTY_STRING;	// SR 141 - RFC 283384    bool SMStateBase::m_bMotionSensorThreadRun = false;		// RFC 219283 DE/RF Touchscreen Motion sensor enables 7883 scanner motor
	bool SMStateBase::m_bMotionSensorThreadRun = false;
    CWinThread* SMStateBase::m_hMotionSensorThread = NULL;	// RFC 219283 DE/RF
    int SMStateBase::m_MotionEnablesScannerInterval=0;		// RFC 219283 DE/RF
	long SMStateBase::g_lUseDefaultTolerance = SMM_TRUE;		// for Std Dev, will need to know if WtInfo determines to use the default tol or exception tol for SMM LPM120402
	int SMStateBase::m_iTolUsedflag = WTINFO_DEFAULT;			// poly-state flag indicating tolerance used: 1 = Std Deviation; 2 = Default; 3 = Maximum was used (not good) - for tracing purposes in the App
	long SMStateBase::g_lDisplayFormRC = 0;		//TAR 220074 BZ 11/27/02 227543
	long SMStateBase::m_lSavedCashChangerStatusCode = -999;	//TAR225069
	bool SMStateBase::fStartTrxInAM = false;
	bool SMStateBase::m_bSignaturePrintingStatus = false; //TAR 195179 & 191490 & 224378
   	long SMStateBase::m_lAgerestrictQueuedtoRAP = 0;      //TAR 196910
    long SMStateBase::m_lItemsScanned = 0;	            //TAR 04/30/02 BZ
	bool SMStateBase::m_bHasSeenTakeReceipt = false;             // TAR239622 CHU 070203
	bool SMStateBase::m_bScanOnAttractAtQuietMode = false;  //TAR234536
	CString SMStateBase::g_csAssistModeStateName = EMPTY_STRING; // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
	bool SMStateBase::m_bMotionSensorEnabledOnAttract = false;    //RFC 265465
	int SMStateBase::m_MotionSensorOnAttractInterval=0;		//RFC 265465
	// SSF will set this function pointer to a function that will create SSF anchor states and return them
	pfnSSFAnchorType SMStateBase::m_pfnSSFCreateAnchor = NULL;  // TAR:242837

  bool SMStateBase::g_bBackToBackDataNeeded = false;  // TAR 265143
  bool SMStateBase::g_bClearOccurred = false;         // TAR 265143
  bool SMStateBase::g_bFromRAP = false;               // TAR 265143
  bool SMStateBase::g_bDelayedSecIntervention = true;
  bool SMStateBase::g_bClearAllDelayedSecIntervention = false;
  bool SMStateBase::g_bOnScannerEnable = false;	//TAR 298537
  bool SMStateBase::g_bOnItemOK = false;    //TAR 298537
  long SMStateBase::g_lNumberOfItemsNotBagInMultiPick = 0;    //Multipick Enhancement
  int  SMStateBase::nCustomMessageLastState = -1; // TAR 312856
  int  SMStateBase::nHopperTrxCount = co.nOperationsHopperFixLaterTrxLimit;
  bool SMStateBase::m_bCurrentIntervention_MisMatch = false;     //TAR340654
  bool SMStateBase::m_bCurrentIntervention_UnexIncrease = false; //TAR340654
  bool SMStateBase::m_bCurrentIntervention_UnexDecrease = false; //TAR340654
  bool SMStateBase::m_bCurrentIntervention_DontBagThreshold = false; //TAR433737
  bool SMStateBase::fCashOutSensorBlocked = false; //TAR 317732
  bool SMStateBase::g_bInterventionApproval = false; //TAR 371932 & tar 371918
  long SMStateBase::m_SafePayLastErrorCode = OPOS_SUCCESS;
  bool SMStateBase::m_bShowMediaStatusButton = false;
  bool SMStateBase::m_fPrintDuplicateTrailer = false;  //RFC 334227
  bool SMStateBase::g_bMultiPickSkipBaggingAllowed = false; // TAR 398289
  BEST SMStateBase::g_savedAnchorStateFromAttract = BES_START; //416582

  //included for TAR 381428
  struct RapOperator {
	  CString csRapName;
	  CString csRapOPID;
  };

  vector<RapOperator> csRapOperatorIDs;
  //end TAR 381428


    COleVariant SMStateBase::defaultFont;
    ItemDetails SMStateBase::itemDetailOrig;
    ItemDetails SMStateBase::itemDetailNew;

	//+SR93 Biometrics
	CString SMStateBase::g_csPassword = _T("") ;
	CString SMStateBase::g_csOperator = _T("") ;
	bool SMStateBase::g_bBiometricACLDevice = false;
	bool SMStateBase::g_bBiometricACLServer = false;
	bool SMStateBase::g_bFPValid = false;
	bool SMStateBase::g_bFPTouched = false;
	bool SMStateBase::g_bBiometricOpened = false;
	//-SR93 Biometrics
	
	//+SR713
	bool SMStateBase::bDoNotShowSellBags = false;
	bool SMStateBase::bIsInPreTenderState = false;
	//-SR713
    BOOL SMStateBase::g_bValidHeadCashier = false;

	long SMStateBase::ms_lExcludedItems = 0;	 // RFC 371075

	extern int nGenerateEntryIDForTB; // used to set entry ID if TB does not provide one
	/////////////////////////////////////////
	// Constructor
	/////////////////////////////////////////
	SMStateBase::SMStateBase()
	: nTimeIdleMsec(0),
      m_bStatePopped(false)
	   {
	  saMsgFuncPtr = PSProceduresBase::SecurityMessageModal;
	  bMoveSAMsgDown = false;
	 if (storeMode() || co.IsInAssistMenus())
		nTimeOut = 0;
	  else
		nTimeOut = co.getTimeOutStateDefault();

    m_bNeedToDisplayAuxScreen = true;
	}

   /////////////////////////////////////////////////////////////
   // Calls createState with the anchor state as a param
   /////////////////////////////////////////////////////////////
   SMStateBase *SMStateBase::createAnchorState()
   {
	   return createState(anchorState);
   }


   /////////////////////////////////////////////////////////////
   // Returns a pointer to a newly allocated state of the type
   // saved as the current anchor state
   /////////////////////////////////////////////////////////////
   SMStateBase *SMStateBase::createState(BEST newState)
   {
     // Returns a major base state -Increased the OO design base
     // return a new SMxxx for the passed in state
	 // START TAR : 242837
	 if ((newState >= BEST_BASE) && m_pfnSSFCreateAnchor)
	 {
		 // use SSF callback function to create SCOTSSF anchor states inside SCOTSSF
		 return m_pfnSSFCreateAnchor(newState);
	 }
	 // END TAR : 242837
	 switch (newState)
	 {
	  case BES_START:
  #ifdef _BUILDLIB_
  #else
	  return GetStartState();			// 1
  #endif
	  case BES_SCANANDBAG:
	   RETURNSTATE(ScanAndBag)			// 2
	  case BES_SELECTFROMLIST:
	   RETURNSTATE(SelectFromList)		// 3
	  case BES_FINISH:
		RETURNSTATE(Finish)				// 4
	  case BES_LOOKUPITEM:
        //return new STATE(LookUpItem
		RETURNSTATE(LookUpItem)				// 5
	  case BES_TAKECARD:
        {
		  CREATE_AND_DISPENSE(TakeCard)(true);	      // 6
        }
        case BES_SIGNCHARGE:
		RETURNSTATE(SignCharge)
	  case BES_ENTERCOUPONS:
		RETURNSTATE(EnterCoupons)		      // 7
	  case BES_SELECTCARDTYPE:
		RETURNSTATE(SelectCardType)
	  case BES_THANKYOU:
		RETURNSTATE(ThankYou)
//@@@INFOPOINT
  	  case BES_SELECTCONTAINER:
  		RETURNSTATE(SelectContainer)
//@@@INFOPOINT
	  case BES_HELPSCREEN:
		RETURNSTATE(ContextHelp)
	  case BES_CARDMISREAD:
		RETURNSTATE(CardMisRead)		//TAR119740	LPM100199  must return to system msg after call for help
	  case BES_SELECTTARE:
		RETURNSTATE(SelectTare)
      case BES_KEYINCODE:
		RETURNSTATE(KeyInCode)
	  case BES_COUPONNOTALLOWED:
		RETURNSTATE(CouponNotAllowed)	//TAR#102019 LPM120799  must return to Coupon Not Allowed screen after call for help
      case BES_REQUESTSIG:              //TAR#132072 -- Track data lost
		RETURNSTATE(RequestSig)
      case BES_ENTERQUANTITY:
        RETURNSTATE(EnterQuantity)
	  case BES_ENTERWEIGHT:             //Tar 154147
        RETURNSTATE(EnterWeight)
	  case BES_ENTERWTFORPRICEEMBEDDED:
		RETURNSTATE(EnterWtForPriceEmbedded)
      case BES_CRATEABLEITEM:
        RETURNSTATE(CrateableItem)
	  case BES_SMMEDIASTATUS:
		RETURNSTATE(SmMediaStatus)
      case BES_SMSYSTEMFUNCTIONS:
		RETURNSTATE(SmSystemFunctions)
        case BES_ENTERPIN:
      {
        CREATE_AND_DISPENSE(EnterPin)(nTenderType, fCashBack); //TAR137419 return to pin entry after help
      }
			case BES_LANECLOSED:
				RETURNSTATE(LaneClosed)
	  case BES_HOSTOFFLINE:
				RETURNSTATE(HostOffLine)
      case BES_OUTOFSERVICE:                //TAR125161 return to initialize when after call for help on init failure
      {
        CREATE_AND_DISPENSE(OutOfService)(1);
      }
			case BES_OUTOFSERVICE2:								//TAR125161 return to initialize when after call for help on init failure
			{
				RETURNSTATE(OutOfService2);
			}
	   case BES_PRODUCEFAVORITES:
       	RETURNSTATE(ProduceFavorites)
       case BES_COUPONTOOHIGH:              // TAR 164015
       	RETURNSTATE(CouponTooHigh)
	   case	BES_COUPONNOMATCH:              //TAR 265390
        RETURNSTATE(CouponNoMatch);
		//TAR 277900+ Migrated-BM185018
		case BES_CUSTOMMESSAGE:
			RETURNSTATE(CustomMessage);
		//TAR 277900-
	   case BES_INSERTCOUPON:       //Insert Coupon anchor state
       	{
          if (bEnterTenderState)
          {
		    CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_ENTERCOUPONS);
          }
          else
          {
		    CREATE_AND_DISPENSE(InsertCoupon)(getAnchorState(), BES_SCANANDBAG);
          }
		}
       case BES_ENTERCOUPONVALUE:           // TAR 164014
       	RETURNSTATE(EnterCouponValue)
	   case BES_BAGANDEAS:
        RETURNSTATE(BagAndEAS)				//T.I.D
	   case BES_PICKINGUPITEMS:
        RETURNSTATE(PickingUpItems)			//T.I.D
	   case BES_SECMISMATCHWEIGHT:
		{
		CREATE_AND_DISPENSE(SecMisMatchWeight)(BES_BAGANDEAS, BES_SCANANDBAG);
		}
	   case BES_SECUNEXPECTEDDECREASE:
		{
		CREATE_AND_DISPENSE(SecUnexpectedDecrease)(getAnchorState(), nSellState);
		}
	   case BES_VOIDAPPROVAL:
		{
		CREATE_AND_DISPENSE(VoidApproval)(BES_STATERETURN, nSellState);
		}
	   case BES_SECUNEXPECTEDINCREASE:
		{
		CREATE_AND_DISPENSE(SecUnExpectedIncrease)(getAnchorState(), nSellState);
		}
       case BES_UNDEACTIVATEDITEM:
	   {
           CREATE_AND_DISPENSE(UnDeActivatedItemApproval)(getAnchorState(), getAnchorState());
	   }
	   case BES_ACKNOWLEDGESIG:
		RETURNSTATE(AcknowledgeSig)
	   case BES_CARDPROCESSING:
		RETURNSTATE(CardProcessing)
	   case BES_CONFIRMSIGNATURE:
		RETURNSTATE(SmConfirmSignature)
	   case BES_GETFOODSTAMPSTATE:
	    RETURNSTATE(GetFoodStampState)
	   case BES_TAKERECEIPT:
		RETURNSTATE(TakeReceipt)
	   case BES_SETALLITEMSASIDE:
		RETURNSTATE(SetAllItemsAside)
	   case BES_VOIDTRANSAPPROVAL:
		{
		CREATE_AND_DISPENSE(VoidTransApproval)(BES_STATERETURN, nSellState);
		}
	   case BES_SECBAGBOXTHRESHOLD:
		{
		CREATE_AND_DISPENSE(SecBagBoxThreshold)(getAnchorState());
		}
	   case BES_SECITEMREMOVEDTHRESHOLD:
		{
		CREATE_AND_DISPENSE(SecItemRemovedThreshold)(getAnchorState());
		}
	   //SR085 +
	   case BES_SECBAGBOXNOTVALID:
		{
		CREATE_AND_DISPENSE(SecBagBoxNotValid)(getAnchorState());
		}
	   //SR085 -
       case BES_SECNEWITEMENTRY:
		{
		CREATE_AND_DISPENSE(SecNewItemEntry)(getAnchorState(), nSellState);
		}
       case BES_SECSKIPBAGGINGTHRESHOLD:
		{
		CREATE_AND_DISPENSE(SecSkipBaggingThreshold)(getAnchorState());
		}
	   case BES_VOIDITEM:
		RETURNSTATE(VoidItem)
	   case BES_VOIDNOMATCH:
		RETURNSTATE(VoidNoMatch)
	   case BES_CONFIRMVOID:
		RETURNSTATE(ConfirmVoid)
	   case BES_CONFIRMABORT: //tar 196574
		RETURNSTATE(ConfirmAbort)
	   case BES_CASHPAYMENT:
		RETURNSTATE(CashPayment)
	   case BES_CHECKBASKET:


		RETURNSTATE(CheckBasketReminder)
	   case BES_DEPOSITCOUPONS:
		RETURNSTATE(DepositCoupons)
	   case BES_DROPOFFCOUPONS:
		RETURNSTATE(DropoffCoupons)
	   case BES_TRANSPORTITEM:
		RETURNSTATE(TransportItem)
       case BES_SECBAGAREABACKUP:
        RETURNSTATE(SecBagAreaBackup);
	   case BES_WAITFORAPPROVAL:
		{
			CREATE_AND_DISPENSE(WaitForApproval) (g_bgTrxVerif, g_bgVoidTrx, g_bgVerifySig);
		}
	   case BES_WAITFORREMOTEASSIST: //tar 197857
		   RETURNSTATE(WaitForRemoteAssistance)
	   case BES_SYSMSGOPPASS:
		   RETURNSTATE(SystemMessageOpPass)
	   case BES_SECURITYLEVEL3:
		   RETURNSTATE(SecurityLevel3)
	   case BES_RAPDATANEEDED:
		   RETURNSTATE(RAPDataNeeded)
	   case BES_ENTERALPHANUMERICINFO:
		   RETURNSTATE(EnterAlphaNumericInfo)
	   case BES_ITEMNOTFORSALE:
		   RETURNSTATE(ItemNotForSale)
	   case BES_UNKNOWNITEM:
		   RETURNSTATE(UnknownItem)
	   case BES_TIMERESTRICTEDITEM:
		   RETURNSTATE(TimeRestrictedItem)
	   case BES_ITEMQUANTITYEXCEEDED:
		   RETURNSTATE(ItemQuantityExceeded)
       case BES_CMDATAENTRY:
           RETURNSTATE(CmDataEntry)
       case BES_ASSISTEDTENDER:
           RETURNSTATE(AssistedTender)
       case BES_STOREAUTHORIZATION:
           RETURNSTATE(SmAuthorization)
	   //TAR221768+ CHU 12/06/02, if we don't have this, we would return to Attract
	   case BES_STATERETURN:
		   return STATE_RETURN;
	   //TAR221768-
       case BES_SMDATAENTRY:
           RETURNSTATE(SmDataEntry)
	   case BES_VOUCHERPAYMENT:	// RFC 213717
		   RETURNSTATE(VoucherPayment)
	   case BES_TENDERBALANCE:
		   RETURNSTATE(TenderBalance)
	   case BES_DEPOSITGIFTCARD:
		   RETURNSTATE(DepositGiftCard)
	   case BES_INSERTGIFTCARD:
		   RETURNSTATE(InsertGiftCard)
	   case BES_COLLECTGIFTCARD:
		   RETURNSTATE(CollectGiftCard)
	   case BES_EXTENSIONSTATE:
		   RETURNSTATE(ExtensionState)
	   case BES_LANGUAGESELECT:
		   RETURNSTATE(SelectLanguage)
	   case BES_SMPRINTRECEIPT:
		   RETURNSTATE(SmPrintReceipt)
//USSF Start
	   case BES_UNIVERSALSSF:
		   return MakeUSSFState();
		   //RETURNSTATE(USSFManager)
//USSF End
	   //SR085 +
	   case BES_PUTBAGONSCALE:
		   RETURNSTATE(PutBagOnScale)
	   //SR085 -
	   //+SR713
	   case BES_SELLBAGS:
			RETURNSTATE(SellBags)
	   //-SR713
       //degraded mode tars
	   case BES_DEGRADEDCONTINUETRANS:
		   RETURNSTATE(DegradedContinueTrans)
	   //degraded mode tars
	 }

#ifdef _BUILDLIB_
RETURNSTATE(ThankYou)
#else
return GetStartState();						  // default action is to go back to the attract state
#endif
   }

////////////////////////////////////////////////////////
CString SMStateBase::getSCOTStateName()
{
	CString csRet = _T("");
    switch (anchorState)
	{
	case BES_START:
	  csRet = ps.GetPSText(LTT_ATTRACT, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_SCANANDBAG:
	  csRet = ps.GetPSText(LTT_BELT_WELCOME, SCOT_LANGUAGE_PRIMARY);//TD 05/08/02 TAR 203156
	  break;
	case BES_SELECTFROMLIST:
	  csRet = ps.GetPSText(LTT_SELECTITEM, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_FINISH:
	  csRet = ps.GetPSText(TTL_SELECTPAYMENTTYPE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_LOOKUPITEM:
	  csRet = ps.GetPSText(TXT_TYPEITEMSDESC, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_TAKECARD:
	  csRet = ps.GetPSText(LTT_TAKECARD, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_SIGNCHARGE:
	  csRet = ps.GetPSText(LTT_SIGNCHARGE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_ENTERCOUPONS:
	  csRet = ps.GetPSText(LTT_CPNENTRY, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_SELECTCARDTYPE:
	  csRet = ps.GetPSText(LTT_SELECTCARD, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_THANKYOU:
	  csRet = ps.GetPSText(LTT_THANKYOU, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_SELECTCONTAINER:
	  csRet = ps.GetPSText(LTT_SELECTCONTAINER, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_HELPSCREEN:
	  csRet = ps.GetPSText(LTT_HELPFUL, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_CARDMISREAD:
	  csRet = ps.GetPSText(TTL_CARD_DECLINED, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_SELECTTARE:
	  csRet = ps.GetPSText(LTT_SELECTITEM, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_KEYINCODE:
	  csRet = ps.GetPSText(LTT_KEYINCODE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_COUPONNOTALLOWED:
	  if (co.csTenderAllowCoupons == _T("N"))	//coupons Never allowed
		csRet = ps.GetPSText(LTT_COUPONNOTACCEPTED, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_REQUESTSIG:
	  csRet = ps.GetPSText(LTT_REQSIG, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_ENTERQUANTITY:
	  csRet = ps.GetPSText(LTT_HOWMANYBUY, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_ENTERWEIGHT:
	  csRet = ps.GetPSText(LTT_PUTONSCALE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_ENTERWTFORPRICEEMBEDDED:
	  csRet = ps.GetPSText(LTT_PUTONSCALE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_CRATEABLEITEM:
	  csRet = ps.GetPSText(TTL_PURCHASECRATE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_ENTERPIN:
	  csRet = ps.GetPSText(LTT_PINENTER, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_LANECLOSED:
	  csRet = ps.GetPSText(LTT_LANECLOSED, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_HOSTOFFLINE:
	  csRet = ps.GetPSText(LTT_HOSTOFFLINE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_OUTOFSERVICE:
	  csRet = ps.GetPSText(LTT_OUTOFSERVICE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_OUTOFSERVICE2:
	  csRet = ps.GetPSText(LTT_OUTOFSERVICE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_PRODUCEFAVORITES:
	  csRet = ps.GetPSText(LTT_PRODUCEFAVORITES, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_COUPONTOOHIGH:
	  csRet = ps.GetPSText(LTT_CPNTOOHIGH, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_COUPONNOMATCH:   //Tar 265390
	  csRet = ps.GetPSText(LTT_CPNNOITEM, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_INSERTCOUPON:  //Insert Coupon anchor state
	  csRet = ps.GetPSText(LTT_INSERTCOUPON, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_ENTERCOUPONVALUE:
	  csRet = ps.GetPSText(LTT_CPNVALUE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_ACKNOWLEDGESIG:
	  csRet = ps.GetPSText(LTT_ACKSIG, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_CARDPROCESSING:
	  csRet = ps.GetPSText(LTT_CARDPROCESSING, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_CONFIRMSIGNATURE:
	  csRet = ps.GetPSText(LTT_SIGNCONFIRM, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_TAKERECEIPT:
	  csRet = ps.GetPSText(LTT_TAKERECEIPT, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_SETALLITEMSASIDE:
	  csRet = ps.GetPSText(LTT_SETALLASIDE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_VOIDITEM:
	  csRet = ps.GetPSText(LTT_CANCELITEMS, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_VOIDNOMATCH:
	  csRet = ps.GetPSText(LTT_CANCELNOMATCH, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_CONFIRMVOID:
	  csRet = ps.GetPSText(LTT_CANCELCONFIRM, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_CASHPAYMENT:
	  csRet = ps.GetPSText(LTT_CASHINSERT, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_CHECKBASKET:
	  csRet = ps.GetPSText(TTL_CHECKYOURCART, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_DEPOSITCOUPONS:
	  csRet = ps.GetPSText(LTT_CPNDEPOSIT, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_DROPOFFCOUPONS:
	  csRet = ps.GetPSText(LTT_DROPOFFCOUPONS, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_TRANSPORTITEM:
	  csRet = ps.GetPSText(LTT_TRANSPORT_ITEM, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_CONFIRMABORT: //tar 196574
	  csRet = ps.GetPSText(LTT_CANCELALL, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_BAGANDEAS:  //tar 196453
	  csRet = ps.GetPSText(MSG_BAGITEM, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_UNDEACTIVATEDITEM:
	  csRet = ps.GetPSText(LTT_UNDEACTIVATEDITEM, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_UNKNOWNITEM: //tar 239365
	  csRet = ps.GetPSText(LTT_UNKNOWNITEM, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_VOUCHERPAYMENT:		// RFC 213717
	  csRet = ps.GetPSText(MSG_VOUCHER_PAYMENT, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_TENDERBALANCE:
	  csRet = ps.GetPSText(LTT_YOURBALANCE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_DEPOSITGIFTCARD:
	  csRet = ps.GetPSText(LTT_DEPOSITGIFTCARD, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_INSERTGIFTCARD:
	  csRet = ps.GetPSText(LTT_DEPOSITGIFTCARD, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_COLLECTGIFTCARD:
	  csRet = ps.GetPSText(LTT_WAITFORASSISTANCE, SCOT_LANGUAGE_PRIMARY);
	  break;
	case BES_LANGUAGESELECT:
	  csRet = ps.GetPSText(LTT_SELECTLANGUAGE, SCOT_LANGUAGE_PRIMARY);
	  break;
    //SR085 +
	case BES_PUTBAGONSCALE:
		csRet = _T("PutBagOnScale"); //need to change this later depending on State Name
		break;
	//SR085 -
    //+degraded mode tars
	case BES_DEGRADEDCONTINUETRANS:
		csRet = _T("ContinueTrans"); 
		break;
	//-degraded mode tars
	default:
		CString csStateName = this->GetRuntimeClass()->m_lpszClassName;
		if(csStateName.Find(_T("CustomMessage")) != -1)
		{
		  csRet = ps.GetPSText(LTT_SYSTEMMSG1, SCOT_LANGUAGE_PRIMARY);
		}
	}
	return csRet;
}

   /////////////////////////////////////////////////////
   // Update RAP with the new state info
   ////////////////////////////////////////////////////
   void SMStateBase::UpdateStateName(CString csStateName)
   {
	   	CString csInstruction, csMediaStatus, csLaneButtonText;
		if (co.IsAttendantModeOn())
		{
			csStateName = ps.GetPSText(MSG_ATTENDANT, SCOT_LANGUAGE_PRIMARY);
		}
		else if (co.IsInAssistMenus())
		{
			csStateName = ps.GetPSText(MSG_ASSISTMODE, SCOT_LANGUAGE_PRIMARY);
		}
		else if((csStateName == EMPTY_STRING) && storeMode())
			csStateName = ps.GetPSText(RA_LANEBTT_STOREMODE, SCOT_LANGUAGE_PRIMARY);

		//TAR200557 if media errors, set bToldRAPMediaLow and haven't told RAP, set to true and tell RAP
		if( dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow() ) //&& !bToldRAPMediaLow)
		{
			csMediaStatus = ps.GetPSText(RA_MEDIA_LOW_ID, SCOT_LANGUAGE_PRIMARY);

		}
		else	// else just update the state name to rap
		{
			csMediaStatus = _T("");
		}
		
		//+jrparro added TAR 386731
		if( bDropoffRequired && !bDropoffApproved ) 
		{ 
			 csLaneButtonText = ps.GetPSText(RA_LANEBTT_COLLECCOUPONS, SCOT_LANGUAGE_PRIMARY); 
		} 
		else 
		{ 
			csLaneButtonText = _T(""); 
		} 
		//-jrparro added TAR 386731

		csInstruction.Format(ps.GetPSText(SCOT_LANE_STATUS, SCOT_LANGUAGE_PRIMARY),
			                 _T(""),
							 csLaneButtonText,	//TAR 386731
							 csStateName,
							 co.IsSecurityDisableOn()==true ? ps.GetPSText(RA_SECONDLINE_SECURITYDISABLED, SCOT_LANGUAGE_PRIMARY) : _T(""),
							 csMediaStatus);
		trace(L7, _T("+ra.OnNormalItem, Message = %s"), csInstruction);
		ra.OnNormalItem(csInstruction);
		trace(L7, _T("-ra.OnNormalItem"));


   }

   // TAR 258195 start
   void SMStateBase::setAnchorState(BEST state)
   {
      static CString csBesNames[] = BES_STRINGS;

      try
      {
         trace(L6, _T("+-setAnchorState(%d - %s)"), state,
                      (state < BES_LASTSTATE) ?
                      csBesNames[state+1] : _T("Non-base state"));
      }
      catch (...)
      {
         trace(L6, _T("+-setAnchorState(%d - %s)"), state,
                      _T("unable to obtain text equivalent"));
      }
      anchorState = state;

      // TAR 310487 Start - reset anchor state for delayed approval
      if (g_pDelay_WaitForApprovalStateChange != NULL)
      {
         DeliverAnchorState(g_pDelay_WaitForApprovalStateChange);
      }
      // TAR 310487 End
   }
   // TAR 258195 end


   // Added for TAR 310487
   void SMStateBase::DeliverAnchorState(SMStateBase *statePtr)
   {
      CString csStateName = statePtr->GetRuntimeClass()->m_lpszClassName;
      bool bCalledDeliver = false;

      // Parallels Deliver() calls made via CREATE_AND_DISPENSE in createState()
      try
      {
         if (csStateName.Find( _T("SecUnexpectedDecrease") ) != -1)
         {
            ((SMSecUnexpectedDecrease *)statePtr)->Deliver(getAnchorState(), nSellState);
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecUnExpectedIncrease") ) != -1)
         {
            ((SMSecUnExpectedIncrease *)statePtr)->Deliver(getAnchorState(), nSellState);
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecNewItemEntry") ) != -1)
         {
            ((SMSecNewItemEntry *)statePtr)->Deliver(getAnchorState(), nSellState);
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("UnDeActivatedItemApproval") ) != -1)
         {
            ((SMUnDeActivatedItemApproval *)statePtr)->Deliver(getAnchorState(), getAnchorState());
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecBagBoxThreshold") ) != -1)
         {
            ((SMSecBagBoxThreshold *)statePtr)->Deliver(getAnchorState());
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecItemRemovedThreshold") ) != -1)
         {
            ((SMSecItemRemovedThreshold *)statePtr)->Deliver(getAnchorState());
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecSkipBaggingThreshold") ) != -1)
         {
            ((SMSecSkipBaggingThreshold *)statePtr)->Deliver(getAnchorState());
            bCalledDeliver = true;
         }
      }
      catch (...)
      {
         trace(L2,_T("+-DeliverAnchorState unable to deliver anchor state %d to %s"),getAnchorState(),csStateName);
      }
      if (bCalledDeliver)
      {
         trace(L6,_T("+-DeliverAnchorState %d for state %s"),getAnchorState(),csStateName);
      }

   }

   //Function is similar to DeliverAnchorState, used when we want to update a state without
   //setting it as the anchor.
   void SMStateBase::DeliverUpdate(BEST state)
   {
      CString csStateName = this->GetRuntimeClass()->m_lpszClassName;
      bool bCalledDeliver = false;
      try
      {
         if (csStateName.Find( _T("SecUnexpectedDecrease") ) != -1)
         {
            ((SMSecUnexpectedDecrease *)this)->Deliver(state, nSellState);
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecUnExpectedIncrease") ) != -1)
         {
            ((SMSecUnExpectedIncrease *)this)->Deliver(state, nSellState);
            bCalledDeliver = true;
         }
		else if (csStateName.Find( _T("SecMisMatchWeight") ) != -1)
         {
            ((SMSecMisMatchWeight *)this)->Deliver(state, nSellState);
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecNewItemEntry") ) != -1)
         {
            ((SMSecNewItemEntry *)this)->Deliver(state, nSellState);
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("UnDeActivatedItemApproval") ) != -1)
         {
            ((SMUnDeActivatedItemApproval *)this)->Deliver(state, state);
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecBagBoxThreshold") ) != -1)
         {
            ((SMSecBagBoxThreshold *)this)->Deliver(state);
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecItemRemovedThreshold") ) != -1)
         {
            ((SMSecItemRemovedThreshold *)this)->Deliver(state);
            bCalledDeliver = true;
         }
         else if (csStateName.Find( _T("SecSkipBaggingThreshold") ) != -1)
         {
            ((SMSecSkipBaggingThreshold *)this)->Deliver(state);
            bCalledDeliver = true;
         }
      }
      catch (...)
      {
         trace(L2,_T("+-DeliverUpdate unable to deliver update state %d to %s"),state,csStateName);
      }
      if (bCalledDeliver)
      {
         trace(L6,_T("+-DeliverUpdate %d for state %s"),state,csStateName);
      }

   }

   /////////////////////////////////////////////////////
   // Sets anchor state and also creates and returns a
   // pointer to a newly allocated state of that type.
   /////////////////////////////////////////////////////
   SMStateBase *SMStateBase::setAndCreateAnchorState(BEST state)
   {
	 //merged TAR231606 to 3.1.1 - fixed by Chen
	 if (    (state == BES_SECITEMREMOVEDTHRESHOLD )
		  || (state == BES_SECSKIPBAGGINGTHRESHOLD )
		  || (state == BES_SECBAGBOXTHRESHOLD ) )
     {
		 //we don't want to set anchor state to one of these state. If we did, we could
		 //get stuck in these states. Because createAnchorState() does:
		 //create_and_despense(one of these state)(getAnchorState()).
		 //Inside of these states, we have:
		 //SMSecSkipBaggingThresholdBase::SMSecSkipBaggingThresholdBase(const BEST PrevState)
		 //{
		 //m_PrevState = PrevState;
	     //m_NextState = BES_SECSKIPBAGGINGTHRESHOLD;
		 //}
		 //So m_PrevState = m_NextState = BES_SECSKIPBAGGINGTHRESHOLD, we can never get out
		 setAnchorState(BES_SCANANDBAG);  // TAR 258195 - ensure tracing occurs
		 return createState(state);
     }
     else
	 {
	    setAnchorState(state);  // TAR 258195 - ensure tracing occurs
	    return createAnchorState();
	 }
   }

    /////////////////////////////////////////////////////
    // OO type helper function which indicates if any Items
    // are sold or pending.
    /////////////////////////////////////////////////////
	bool SMStateBase::isAnySoldVoidedItems(void)
	{

	  // function name is misleading, we do not care about voids in this function
	  // Used the conditional operator
	  // check for any  item sold (count)
	  //				item sold (amount)
	  //				items in progress flag
		// SCOT CORE does not need to do anything if there are no items and no transaction amount
		// SCOT may decide to go to ThankYou screen on return value 'false'
		bool bRetValue = false;
		if ((lItems>0) || (lTotalSale != 0))
		{
			bRetValue = true;
		}
		return bRetValue;
	}


//Fiscal Printer start
/////////////////////////////////////////////////////
// OO type helper function which indicates if any Items
// are sold, voided or pending.
/////////////////////////////////////////////////////
	bool SMStateBase::AreThereAnySoldVoidedItems(void)
	{
	  		bool bRetValue = false;
		if ((lItems>0) || (lTotalSale != 0) || (lVoidedItems>0))
		{
			bRetValue = true;
		}
		return bRetValue;
	}
//Fiscal Printer end

  /////////////////////////////////////////////////////
  // OO type helper function which indicates if any store approvals will
  // be required, i.e. would be required if we went to ready to pay
  /////////////////////////////////////////////////////
	bool SMStateBase::isRestrictedItemApprovalRequired(void)
	{
	  //if (lUnapprovedAlcoholItems || lUnapprovedTobaccoItems)
	  if (lUnapprovedRestrictedItems > 0)
		return true;
      return false;
	}

	bool SMStateBase::isVisualItemApprovalRequired(void)
	{
	  if (lUnapprovedVisualVerifyItems > 0)
		return true;
      return false;
	}

   /////////////////////////////////////////////////////
	bool SMStateBase::isSecurityApprovalRequired(void)
	{
		if(!g_bDelayedSecIntervention)
		{
			return false;
		}
		if (SecMgr.GetOutstandingInterventionCount() > 0)
			return true;
		return false;
	}

    /////////////////////////////////////////////////////
	bool SMStateBase::isVoidRatioApprovalRequired(const bool fUpdateSA)
	{
		bool fReturnVal = false;

	  if (lUnapprovedVoidedItems > 0)
	  {
		//Now we approve each voided item immediately
		//  if ( !SAVoidRatioOK(lTotalSale-lTaxDue,
		//		(lItemSalesAmountVoided + lCouponAmountVoided)) )
		{
		  fReturnVal = true;
		  if (fUpdateSA)
		  {
			lSAYellowNeedingApproval++;
			//ps.AddSecurityLog(ps.GetPSText(MSG_VOID_RATIO_ERROR));
			dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
								DM_TRICOLORLIGHT_ON,
								DM_TRICOLORLIGHT_NORMALREQUEST);
		  }
		}
	  }

		return fReturnVal;
	}

   /////////////////////////////////////////////////////
	bool SMStateBase::isCouponRatioApprovalRequired(const bool fUpdateSA)
	{
	  bool fReturnVal = false;

	  if (lUnapprovedCoupons > 0)					  // added this if...then pd980916
	  {
        //Tar 196365
	    if((CouponTally(lTotalSale, lTotalCouponAmount)) || (nCountPossibleDuplicateCoupon >= co.nOperationsDuplicateCouponMax)||
			  (lExceptionItemsRequiringApproval > 0))
		  {
			  fReturnVal = true;
		  }
	  }
	  return fReturnVal;
	}

   /////////////////////////////////////////////////////
	bool SMStateBase::isAnyApprovalRequired(const bool fUpdateSA)
	{
		bool fReturnVal = false;
		CString csTraceInfo = EMPTY_STRING;
		CString csTemp = EMPTY_STRING;

//USSF START
		if (lUnapprovedUSSFItems > 0)
		{
			fReturnVal = true;
			csTraceInfo = _T("USSF Item Approval Required");
		}
//USSF END

		if (isRestrictedItemApprovalRequired())
		{
			fReturnVal = true;
			csTraceInfo = _T("Restricted Item Approval Required ");
		}

		if (isVisualItemApprovalRequired())
		{
			fReturnVal = true;
			csTraceInfo += _T("Visual Item Approval Required ");
		}

		if (lUnapprovedHopperSubstitution> 0)
		{
			fReturnVal = true;
			csTraceInfo += _T("Hopper Substitution Approval Required ");
		}


		if (isCouponRatioApprovalRequired(fUpdateSA))
		{
			fReturnVal = true;
			csTraceInfo += _T("Coupon Ratio Approval Required ");
		}

		if ((lNotForSaleItems>0)			||
			(lUnknownItems>0)				||
			(lPriceRequiredItems>0)			||
			(lRestrictedNotAllowedItems>0)	||
			(lTimeRestrictedItems>0)		||
			(bSuspendFailure)				||
			(lCouponsInAssist>0)			||		//TAR#102019
			(lQuantityRestrictedItems>0)	||
			(fOtherPayment)				    ||		// for outstanding approvals printing
		    (lUnapprovedVoidedItems>0))
		{
			fReturnVal = true;
			csTemp.Format(_T("Approval Required For: lNotForSaleItems=%d lUnknownItems=%d lPriceRequiredItems=%d lRestrictedNotAllowedItems=%d lTimeRestrictedItems=%d bSuspendFailure=%d lCouponsInAssist=%d lQuantityRestrictedItems=%d fOtherPayment=%d lUnapprovedVoidedItems=%d"),
							  lNotForSaleItems, lUnknownItems, lPriceRequiredItems, lRestrictedNotAllowedItems,
							  lTimeRestrictedItems, bSuspendFailure, lCouponsInAssist, lQuantityRestrictedItems,
							  fOtherPayment, lUnapprovedVoidedItems);
			csTraceInfo += csTemp;
		}

		if (fCantScanCoupon || fRequestForHelp)
		{
			fReturnVal = true;
			csTemp.Format(_T("Approval Required For: fCantScanCoupon=%d; fRequestForHelp=%d"),
							  fCantScanCoupon, fRequestForHelp);
			csTraceInfo += csTemp;
		}

		if (bDropoffRequired && !bDropoffApproved)
		{
			fReturnVal = true;
			csTemp.Format(_T("Approval Required For: bDropoffRequired=%d; bDropoffApproved=%d"),
							  bDropoffRequired, bDropoffApproved);
			csTraceInfo += csTemp;
		}

		if (bSuspendRequested && !bSuspendApproved)
		{
			fReturnVal = true;
			csTemp.Format(_T("Approval Required For: bSuspendRequested=%d; bSuspendApproved=%d"),
							  bSuspendRequested, bSuspendApproved);
			csTraceInfo += csTemp;
		}

		if (fUnDeActivatedItem)
		{
            fReturnVal = true;
			csTraceInfo += _T("UnDeActivated Item Approval Required ");
		}

		// must be last of these tests
		if (isSecurityApprovalRequired())
		{
			fReturnVal = true;
			csTraceInfo += _T("Security Approval Required");
		}

		trace(L6, _T("+-SMStateBase::isAnyApprovalRequired: Return Code=%d Result Info=%s"), fReturnVal, csTraceInfo);
		return fReturnVal;
	}

   /////////////////////////////////////////////////////
   // provide a default UnInitialize() for all states
   /////////////////////////////////////////////////////
	void SMStateBase::UnInitialize(void)
	{
	  return;						   // just return
	}


#define ELEMENT_COUNT(a) (sizeof(a) / sizeof(a[0]))
   /////////////////////////////////////////////////////
    void Reveal(MessageElement *me, CString& csFormattedMessage)
	{
      CString csText;
      CString Msg;
      LPTSTR DM_Devices[] = {
      _T("RECEIPT"),
      _T("JOURNAL"),
      _T("SLIP"),
      _T("PRINTER"),
      _T("CASHDRAWER"),
      _T("CASHCHANGER"),
      _T("KEYLOCK"),
      _T("LINEDISPLAY"),
      _T("MICR"),
      _T("MSR"),
      _T("SCALE"),
      _T("SCANNER"),
      _T("SIGCAP"),
      _T("MISC"),
      _T("ENCRYPTOR"),
      _T("CASHACCEPTOR"),
      _T("COINACCEPTOR"),
      _T("AUXDISPLAY"),
      _T("MOTIONSENSOR"),
      _T("EASDEACTIVATOR"),
      _T("CASHRECYCLER"),
	  _T("POINTCARDRW"),	//4.2 card dispenser
      _T("MAX_CLASSES")};

      LPTSTR TB_Devices[] = {
      _T("UNKNOWN"),
      _T("CMOS"),
      _T("DISPLAYINTERMEDIATE"),
      _T("DISPLAYPRIMARY"),
      _T("DISPLAYSECONDARY"),
      _T("DISPLAYIMMEDIATE"),
      _T("DRAWER1"),
      _T("DRAWER2"),
      _T("EPSMODULE"),
      _T("KEYBOARD"),
      _T("KEYLOCK,			"),
      _T("MSR"),
      _T("PRINTERJOURNAL"),
      _T("PRINTERRECEIPT"),
      _T("PRINTERSLIP"),
      _T("SCALE"),
      _T("SERIAL"),
      _T("SCANNER"),
	  _T("ATMMODULE"),
      _T("UNKNOWN"),
      _T("DISPLAYDATANEEDED"),
	  _T("UICOMMAND")};


        LPTSTR PS_Events[] = {
        _T("UKEVT"),
        _T("HELP"),
        _T("BUTTON1"),
        _T("BUTTON2"),
        _T("BUTTON3"),
        _T("BUTTON4"),
        _T("BUTTON5"),
        _T("BUTTON6"),
        _T("BUTTON7"),
        _T("BUTTON8"),
        _T("MENUKEY1"),
        _T("MENUKEY2"),
        _T("MENUKEY3"),
        _T("MENUKEY4"),
        _T("MENUKEY5"),
        _T("MENUKEY6"),
        _T("MENUKEY7"),
        _T("MENUKEY8"),
        _T("MENUKEY9"),
        _T("MENUKEY10"),
        _T("MENUKEY11"),
        _T("MENUKEY12"),
        _T("MENUKEY13"),
        _T("MENUKEY14"),
        _T("MENUKEY15"),
        _T("RECEIPTUP"),
        _T("RECEIPTDOWN"),
        _T("LISTLOOKUP"),
        _T("HELPTOPICS"),
        _T("ENTERKEY"),
        _T("CANCELKEY"),
        _T("CLEARKEY"),
        _T("NUMERICKEY"),
        _T("DECIMALKEY"),
        _T("ALPHAKEY"),
        _T("VIDEODONE"),
        _T("UKRELEASE"),
        _T("UKSELCHANGE"),
        _T("ASSISTKEY"),
        _T("ACTIVITY"),
        _T("mainMENU"),
        _T("SPACEKEY"),
        _T("GOBACK"),
        _T("REMOTE_DISCONNECTED"),
        _T("KEYSTROKE"),
        _T("BUTTONTOGGLE"),
        _T("BUTTONLANE1STATUS"),
        _T("BUTTONLANE2STATUS"),
        _T("BUTTONLANE3STATUS"),
        _T("BUTTONLANE4STATUS"),
        _T("BUTTONLANE5STATUS"),
        _T("BUTTONLANE6STATUS"),
        _T("BUTTONLANE7STATUS"),
        _T("BUTTONLANE8STATUS"),
        _T("REMOTEBUTTON1"),
        _T("REMOTEBUTTON2"),
        _T("REMOTEBUTTON3"),
        _T("REMOTEBUTTON4"),
        _T("LISTLOOKUPTAB"),
        _T("LISTSCROLL"),
        _T("CONNECTREMOTE"),
        _T("SWITCHCONTEXT")
	    _T("BACKSPACEKEY"),
	    _T("SHIFTKEY"),
	    _T("OTHERSPECIALKEY"),
	    _T("ALPHANUMERICKEY"),
	    _T("RAPSIGNON"),
	    _T("RAPSIGNOFF"),
        _T("SWITCHCONTEXTOTHER"),
        _T("RAPSCANDATA"),
        _T("REMOTEDATA"),
        };


      //TRACE("%s(%d) :%d\n", __FILE__, __LINE__, me->id);
	  TRACE(_T("**SMStateBase::Parse() "));
	  switch (me->id)
	  {
	  case ID_PS:
		if (ELEMENT_COUNT(PS_Events) > me->psInfo.nEvt)
			Msg.Format(_T(" ID_PS : Len=%d nEvt=%d(%s) lParm=%ld\n"),
			  me->nLength, me->psInfo.nEvt, PS_Events[me->psInfo.nEvt], me->psInfo.lParm);
		else
			Msg.Format(_T("DEBUG INFORMATION: ID_PS Event %d Unknown PS Event!!!"), me->psInfo.nEvt);    // TAR 294389
		break;
	  case ID_TRANSBROKER:
		  csText = (LPTSTR)me->pData;
		if (ELEMENT_COUNT(TB_Devices) > me->nDeviceID)
			Msg.Format(_T(" ID_TRANSBROKER : Dev=%d(%s) Len=%d text=[%s]\n"),
			  me->nDeviceID, TB_Devices[me->nDeviceID], me->nLength, csText);
		else
			Msg.Format(_T("DEBUG INFORMATION: ID_TRANSBROKER device %d out of array bounds!!!"), me->nDeviceID);
		break;
	  case ID_DM:
		if (ELEMENT_COUNT(DM_Devices) > me->dmInfo.Class)
			Msg.Format(_T(" ID_DM : Len=%d Msg=%d Class=%d(%s)-%d lCodeOrLength=%ld lDirectIONumber=%ld, lDirectIONumData=%d\n"),
			  me->nLength, me->dmInfo.Msg, me->dmInfo.Class,
			  DM_Devices[me->dmInfo.Class],
			  me->dmInfo.DeviceId,
			  me->dmInfo.lCodeOrLength,
              me->dmInfo.lDirectIONumber,
              me->dmInfo.lDirectIONumData);
		else
			Msg.Format(_T("DEBUG INFORMATION: ID_DM device class %d out of array bounds!!!"), me->dmInfo.Class);
		break;

	  case ID_RA:
		Msg.Format(_T(" ID_RA : msg=%d pData=[%s]\n"),
		  me->raInfo.msg, me->raInfo.pData);
		break;

	  case ID_SECMGR:
		Msg.Format(_T(" ID_SECMGR : nEvt=%d\n"), me->secmgrInfo.nEvt);
		break;

	  case ID_RP:
		Msg.Format(_T(" ID_RP : Msg=%d lParam(num. bytes)=%ld\n"),
		  me->rpInfo.Msg, me->rpInfo.lParam);
		break;

	  case ID_GP:					   // general purpose notify
		Msg.Format(_T(" ID_GP : Dev=%d Len=%d nEvt=%d\n"),
		  me->nDeviceID, me->nLength, me->gpInfo.nEvt);
		break;

	  default:
		Msg.Format(_T("DEBUG INFORMATION: Unknown Message ID %d!!!"), me->id);
		break;
	  }

	  TRACE(_T("%s\n"),Msg);
	  csFormattedMessage = _T(">>> Processing message:") + Msg;
}



/////////////////////////////////////////////////////
// provide the one and only Parse() for all states
// derived states do NOT need their own Parse()!

// the methods this calls are all virtual and may be
// overriden in the current (derived) state so these calls
// will be made to the current state's override method
// if it exists, else fall back on the SMState method

// all methods return a STATE_NULL or a new state.
// STATE_NULL tells the caller to stay in the current state
// A new state tells the caller to change to that new state

/////////////////////////////////////////////////////
SMStateBase *SMStateBase::Parse(MessageElement *me)
{


	//nTimeIdleMsec = 0;
	nLastMsgID = me->id;

	CString csMessageLine;
	Reveal(me, csMessageLine);
    int iErrorIdx = csMessageLine.Find(_T("DEBUG INFORMATION:"));
    if (iErrorIdx > 0)
    {
	  trace(L0, _T("%s"), csMessageLine);
    }
    else
    {
	  trace(L7, _T("%s"), csMessageLine);
    }

	switch (me->id)
	{
	  case ID_PS:
	  {
		  SMStateBase* pRetState = PSParse(me);			 // see SMStatePS.cpp
		  if (pRetState != STATE_NULL)
		  {
		     nTimeIdleMsec = 0;
		  }
	      return pRetState;
	  }

	  case ID_TRANSBROKER:
		return TBParse(me);			 // see SMStateTB.cpp

	  case ID_DM:
		// TAR 200294 CMal and NN 041602 Do not reset the timeout when DMEvent is fired
		// For this TAR changes have been made to SMStateBase.cpp, SMAttractBase.cpp, SMTakeReceiptBase.cpp, and SCOTAPPDlg.cpp
		if (bDoNotResetTimeOutOnDMEvent == false)
      {
         nTimeIdleMsec = 0;
      }
		return DMParse(me);			 // see SMStateDM.cpp

	  case ID_RA:
		nTimeIdleMsec = 0;
		return RAParse(me);			 // see SMStateRA.cpp

      case ID_SECMGR:

          return SASecMgrParse(me);
	  case ID_RP:
		return RPParse(me);			 // see SMStateRP.cpp
	  case ID_CU:
		return CUParse(me);			 // see SMStateRP.cpp

	  case ID_GP:					   // general purpose notify
		nTimeIdleMsec = 0;
		// handle STOP or RESTART right here
		if (me->gpInfo.nEvt==GP_STOP)
		{
		  fStateFreeze = false;
		  fStateAssist = false;
		  fStopSCOT = true;
		  return STATE_STOP;
		}
		if (me->gpInfo.nEvt==GP_RESTART)
		{
		  fStateFreeze = false;
		  fStateAssist = false;
		  fStopSCOT = true;

		  ScotError(RETURN,SM_CAT_STOP_ABORT, SCOTAPP_GP_RESTART);

		  return STATE_RESTART;
		}
		if(me->gpInfo.nEvt==GP_PUTSCOTINLC)
		{
			fPutSCOTinLC = true;
			if(!fInSignedOn && !fInSignedOff && !fSAInTransaction)
			{
               	CString csCurrentStateName;
                try
                {
                    csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
                }
                catch(...)
                {
                    trace(L1, _T("this->GetRuntimeClass() threw Exception!"));
                };
                if(csCurrentStateName.Find(_T("SystemMessage")) != -1)//tar 253778
                {
                    trace(L6, _T("SCOT is at a SystemMessage state. Cannot go to lane closed. Returning STATE_NULL."));
                    return STATE_NULL;
                }
				trace(L6, _T("SCOT is not in the middle of a transaction and remotely go to lane closed."));
                //+RFC 986
                if (co.fStateStartupWaitForTB)
                {
                    setAnchorState(BES_START);
                    TBLaneClosed(true);
                    CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), co.getTimeOutItemTransactionResult());
                }
                else
                {
                    RETURNSTATE(LaneClosed)
                }
                //-RFC 986
			}
			else
			{
				trace(L6, _T("SCOT is in the middle of a transaction and remotely go to lane closed."));
				return STATE_NULL;
			}
		}
		if(me->gpInfo.nEvt==GP_STOPSCOT)
		{
			fStopSCOT = true;
			if(!fInSignedOn && !fInSignedOff && !fSAInTransaction)
			{
				trace(L6, _T("SCOT is not in the middle of a transaction and remotely shut down."));
				fStateFreeze = false;
				fStateAssist = false;
				return STATE_STOP;
			}
			else
			{
				trace(L6, _T("SCOT is in the middle of a transaction and remotely shut down."));
				return STATE_NULL;
			}
		}
		if(me->gpInfo.nEvt==GP_QUERYSCOTSTATE)
		{
			trace(L6, _T("SCOT is remotely checking status."));
			CString csCurrentState;
			csCurrentState = this->GetRuntimeClass()->m_lpszClassName;

			CString csSCOTStatus;
/*    //+dp185016 tar 287778
  		if (fInSignedOn || fInSignedOff || fSAInTransaction)
			{
				// You are in the middle of a transaction
				csSCOTStatus += SCOTINTRANSACTION;
			}
			else
				csSCOTStatus += SCOTNOTINTRANSACTION;
			csSCOTStatus += CURRENTSTATE;
			csSCOTStatus += csCurrentState;

			CTime ctTime = CTime::GetCurrentTime();
			csSCOTStatus += _T(" (");
			csSCOTStatus += ctTime.Format( _T("%d %B %Y - %H:%M:%S") );
			csSCOTStatus += _T(")");

			WriteSCOTStatusToFile(csSCOTStatus);
*/

      TCHAR szSCOTStatus[150];
      if (fInSignedOn || fInSignedOff || fSAInTransaction)
      {
        // You are in the middle of a transaction
        GetPrivateProfileString (SECTION3,KEY82, KEY82, szSCOTStatus, sizeof(szSCOTStatus), szIniFilePath);
      }
      else
        GetPrivateProfileString (SECTION3,KEY81, KEY81, szSCOTStatus, sizeof(szSCOTStatus), szIniFilePath);

      TCHAR szSCOTState[100];
      GetPrivateProfileString (SECTION3,KEY83, KEY83, szSCOTState, sizeof(szSCOTState), szIniFilePath);

      _tcscat( szSCOTStatus, szSCOTState );

      //time CString
      CString csSCOTTime;
      CTime ctTime = CTime::GetCurrentTime();
      csSCOTTime += _T(" (");
      csSCOTTime += ctTime.Format( _T("%d %B %Y - %H:%M:%S") );
      csSCOTTime += _T(")");

      CString csWrite;
      csWrite.Format( szSCOTStatus, csCurrentState, csSCOTTime );
      csWrite = ps.ConvertToEscapeSequence(csWrite, _T("\\n"), _T("\n"));
      WriteSCOTStatusToFile(csWrite);
      //-dp185016
			return STATE_NULL;
		}
	    // RFC 219283 DE/RF Touchscreen Motion sensor enables 7883 scanner motor
		if(me->gpInfo.nEvt==GP_MOTIONEVENT)
		{
			if(dm.IsScannerEnabled())
			{
				trace(L6, _T("Motion is (re)enabling scanner"));
				DMScannerDisable();
				DMScannerEnable();
			}
			else
				trace(L6, _T("Motion ignored, scanner is currently disabled"));
			return STATE_NULL;
		}
		//RFC 265463
		if(me->gpInfo.nEvt==GP_POWERSTOPSCOT)
		{
			fStopSCOT = true;
			SMStateBase *rtnVal;
			if(!fInSignedOn && !fInSignedOff && !fSAInTransaction)
			{
				trace(L6, _T("SCOT is not in the middle of a transaction and shut down due to power outage."));
				fStateFreeze = false;
				fStateAssist = false;
				rtnVal = STATE_STOP;
			}
			else
			{
				trace(L6, _T("SCOT is in the middle of a transaction and shut down due to power outage."));
				rtnVal = STATE_NULL;
			}
			tb.PowerShutdown(); // Allow TB to suspend transaction, etc.
			return rtnVal;
		}
		//RFC 265463
		if(me->gpInfo.nEvt==GP_GETTEXT)
		{
			trace(L6, _T("FastLane request for text ID %d in current language."),
			me->psInfo.wParm);
      //+dp185016 tar 287778
			CString csText;
    	TCHAR szSCOTState[100];
      GetPrivateProfileString (SECTION3,KEY84, KEY84, szSCOTState, sizeof(szSCOTState), szIniFilePath);

      csText.Format( szSCOTState,
                    co.language[m_customerLanguage].csName,
			              co.language[m_customerLanguage].szCode,
			              ps.GetPSText(me->psInfo.wParm));
/*			CString csText;
			csText.Format(GETTEXTOUTPUT,
			              co.language[m_customerLanguage].csName,
			              co.language[m_customerLanguage].szCode,
			              ps.GetPSText(me->psInfo.wParm));
*/
      csText = ps.ConvertToEscapeSequence(csText, _T("\\n"), _T("\n"));
      //-dp185016
			WriteSCOTStatusToFile(csText);
			return STATE_NULL;
		}

		//RFC 265465 start
		if(GP_MOTIONONATTRACT == me->gpInfo.nEvt)
		{
			if(m_bMotionSensorEnabledOnAttract)
			{
				DMSayPhrase(WELCOMEPSB);
				trace(L6, _T("Motion on Attract Screen, Playing WELCOMEPSB"));
			}
			else
				trace(L6, _T("Motion ignored, FastLane is not in Attract Screen"));
			return STATE_NULL;
		}
		//RFC 265465 end

		// allow states to handle other GP codes
		return GPNotify(me->gpInfo.nEvt);	// see SMState.cpp
	} // of switch

	// catch all other unknown messages
	ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_BS_UNKNOWN_MSG,_T("%d"),nLastMsgID);
	return STATE_NULL;
}

   /////////////////////////////////////////////////////
	SMStateBase *SMStateBase::IdleTimer(const long lMsec)
	{
		CTime ctCurrentTime = CTime::GetCurrentTime();
		int nCurrentDay = ctCurrentTime.GetDay();
		if (nPreviousDay != nCurrentDay)
		{
			nPreviousDay = nCurrentDay;
			COleVariant v;
			HKEY  hKey;
			if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\TransactionCount"), 
       0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&hKey, NULL) == ERROR_SUCCESS)
			{
				long lTrxCount = 0;
				RegSetValueEx(hKey, _T("TransactionCount"), 0L, REG_DWORD, (LPBYTE)&lTrxCount, sizeof(DWORD));
				RegSetValueEx(hKey, _T("TransactionDate"), 0, REG_DWORD, (LPBYTE)&nCurrentDay, sizeof(DWORD));
				v = lTrxCount;
				m_pAppPSX->SetTransactionVariable( _T("TransactionCount"), v );
			}
		}
		// If EOD option is to Fastlane control
		if (co.nOperationsEOD == OPTION_LANE_CONTROL)
		{
		//	CTime ctCurrentTime = CTime::GetCurrentTime();
			int nCurrentDay = ctCurrentTime.GetDay();
			// if midnight - run EOD
			if (nLastDay != nCurrentDay)
			{
				// Set the last day that run EOD to current day
				nLastDay = nCurrentDay;
				// Call this function to set TB business date
				rp.RPBusinessDate();
				// if in the middle of transaction - just set a flag
				if ( fInSignedOn || fInSignedOff || fSAInTransaction )
				{
					fDoEOD = true;
				}
				else
				{
					return PerformEOD();
				}

			}
		}

      //RFC 263404
      if (fSAInTransaction || storeMode())
      {
          appBusy.RenewBusyStatus();
      }
      //RFC 263404

	  SMStateBase *stb = TBIdleTimer();
	  if ( stb != STATE_NULL )///DcL-!!!
		return stb;

	  if (nTimeOut==0)
		return STATE_NULL;			  // no time out limit

      nTimeSinceLastHeartbeat += lMsec;
      if ((co.getTimeOutHeartbeatTimer() > 0) &&
          (nTimeSinceLastHeartbeat >= co.getTimeOutHeartbeatTimer()*1000L))
      {
          trace(L7,_T("Sending Heartbeat event"));
          eo.Error(INFO,SM_CAT_STARTUP_INIT, SCOTAPP_HEARTBEAT,
              (NCRSM_SNMP_ACTION | NCRSM_REMOTE_LOG_ACTION),
              _T("%s every %d seconds"),
              _T("Heartbeat1"),
              co.getTimeOutHeartbeatTimer());
          eo.Error(INFO,SM_CAT_STARTUP_INIT, SCOTAPP_HEARTBEAT2,
              (NCRSM_SNMP_ACTION | NCRSM_REMOTE_LOG_ACTION),
              _T("%s every %d seconds"),
              _T("Heartbeat2"),
              co.getTimeOutHeartbeatTimer());
          nTimeSinceLastHeartbeat = 0;
      }

	  nTimeIdleMsec += lMsec;		   // advance idle time
   	  trace(L9,_T("State Timeout:%d, CurrentIdleTimeTotal:%d"),nTimeOut, nTimeIdleMsec/1000L);
	  if ( nTimeIdleMsec < nTimeOut*1000L )
		return STATE_NULL;			  // no timeout yet

	  nTimeIdleMsec = 0;				// reset time counter
//USSF START
	  USSFHOOK(_T("TimedOut"), nextState = TimedOut(), return nextState;);
	  //return TimedOut();				// this is defined for each derived state
//USSF END
	}

   /////////////////////////////////////////////////////
	SMStateBase *SMStateBase::TimedOut(void)
	{
	  // Many states override this function
	  // Basically all customer mode sales
	  // or tender states timeout and go to
	  // STContinueTrans. Store mode never
	  // times out. Help mode always returns
	  // to the caller state

        // RFC 262343 - start
        if (fTenderPending) //If we are in a tender
        {
            ScotError(INFO, SM_CAT_INTERNAL_ERROR, SCOTAPP_TENDER_TIMEOUT);
            fTenderPending = false;
            if (cDMLastTrack)
            {
                delete cDMLastTrack;
                cDMLastTrack = NULL;
            }
        }
        // RFC 262343 - end

	  if ( helpMode() )
		return STATE_RETURN;

	  if ( storeMode() || bPartialTenderWithFoodStamps) // annoy user if left too long Tar 132020
	  {
		//DMSayPhrase("ringin");
		return STATE_NULL;
	  }
      //if (IsSCOTAPPVisible())

      // Fix for TAR 153465
	  if (SCOTIsMinimized())
        return STATE_NULL;
      else
	    RETURNSTATE(ContinueTrans)

	}

    /////////////////////////////////////////////////////
	bool SMStateBase::SCOTIsMinimized()
	{
       bool minimize = false;
       CWnd *pWndPrev = CWnd::FindWindow(NULL, _T("NCR SCOT"));
	   if (pWndPrev && pWndPrev->IsIconic())
	     minimize = true;
	   return minimize;
    }

    /////////////////////////////////////////////////////
	SMStateBase *SMStateBase::GPNotify(const GPEVT)
	{
	  return STATE_NULL;
	}

   /////////////////////////////////////////////////////
   // clean up a message by removing double blanks and if
   // a 2x20 message inserting a blank between the lines
   // split extra long lines into 20 char lines with \n around the 20th pos
   /////////////////////////////////////////////////////
	CString SMStateBase::CleanUpMessage(LPCTSTR szIn,
																	const bool f2x20)
	{


	  CString csWork = szIn;
    //Tar 125179  // Expect 40 character length message
    if(csWork.GetLength()!=40)
    {
      int length = csWork.GetLength();  // message length
      for( int i=0; i<(40-length);i++)
        csWork += _T(" ");    // append space till 40
    }

	  CString csOut = EMPTY_STRING;

	  // add a blank between 20 and 21 on 2x20 lines
	  if (f2x20) csWork = csWork.Left(20) + _T(" ") + csWork.Right(20);

	  // move words from csWork to csOut

	  while (csWork!=EMPTY_STRING)					 // do until no words left
	  {
		csWork.TrimLeft();					 // start by removing leading blanks
		int i = csWork.Find(_T(" "));			 // look for next blank (word separator)
		if (i<0) i = csWork.GetLength();     // none left? must be the last word
		csOut += csWork.Left(i);			 // insert new word
		csOut += _T(" ");						 // insert a blank
		csWork = csWork.Mid(i);				 // advance input pointer
	  }

	  csOut.TrimRight();

	  return csOut;
	}

   /////////////////////////////////////////////////////
   long SMStateBase::CopyFileFromServer(LPCTSTR szFileName, // file name
      					     LPCTSTR szDestinationFilePath, // local file path
						     const bool fForce)            // T/F T-copy always, F-copy if different
   {
	  long rc = tb.CopyFileFromServer(szFileName,szDestinationFilePath, fForce);
	  ProcessTBReturn(rc);
      return rc;
   }

   /////////////////////////////////////////////////////
   long SMStateBase::CopyFileToServer(LPCTSTR szFileName, // file name on NT
	            			 LPCTSTR szSourceFilePath)    //local source  file path
	{
      long rc= tb.CopyFileToServer(szFileName,szSourceFilePath);
	  ProcessTBReturn(rc);
	  return rc;
	}

   /////////////////////////////////////////////////////
   // pop a state off the Fifo stack
   /////////////////////////////////////////////////////
	SMStateBase *SMStateBase::PopStateFifoStack(void)
	{
	  SMStateBase *sExit = STATE_NULL;
	  while (uStateFifo!=aStateFifo)
	  {
		if (sStateFifo[uStateFifo]!=STATE_NULL) break;
		if (++uStateFifo==MAX_FIFO_STACK) uStateFifo = 0;
	  }

	  sExit = sStateFifo[uStateFifo];

	  if (sExit!=STATE_NULL)
	  {
        sExit->SetStatePopped( true );
		sStateFifo[uStateFifo] = STATE_NULL;
		if (++uStateFifo==MAX_FIFO_STACK) uStateFifo = 0;
	  }

	  return sExit;
	}

   /////////////////////////////////////////////////////
   // push a state on the Fifo stack, until in attract
   /////////////////////////////////////////////////////

	SMStateBase *SMStateBase::PushStateFifoStack(SMStateBase *sPush)
	{
	  if (sStateFifo[aStateFifo]==STATE_NULL)
	  {
		sStateFifo[aStateFifo] = sPush;	// add state to fifo stack
		if (++aStateFifo==MAX_FIFO_STACK) aStateFifo = 0;
	  }
	  else
	  {
		delete sPush;
		ScotError(RETURN,SM_CAT_RESOURCE, SCOTAPP_BS_STACK_FULL);
	 }

	  pgState2Push = NULL;
	  return STATE_NULL;							  // report it later from Attract state
	}

   /////////////////////////////////////////////////////
	void SMStateBase::ResetAllTransactionValues(void)
	{
      trace(L6,_T("ResetAllTransactionValues"));
	  ps.ShowAmountDueAmountPaid(0,0);
	  //TAR 364905 Begin
	  trace(L7, _T("+ra.ForceApprovalBeforeSuspend, ForceApprovalBeforeSuspend=false"));
	  ra.ForceApprovalBeforeSuspend(false);
	  trace(L7, _T("-ra.ForceApprovalBeforeSuspend"));
	  //TAR 364905 End
//	  ImmediateInterventionStack.deleteAll();
	  g_bReadyForItem = false;//TAR 363023
     for ( POSITION p = ImmediateInterventionStack.GetHeadPosition(); p != NULL;
                    ImmediateInterventionStack.GetNext( p ) )
	  {
		SecImmediateIntervention *psecImmediateIntervention =
			ImmediateInterventionStack.GetAt( p );
		  delete psecImmediateIntervention ;
	  }
	ImmediateInterventionStack.RemoveAll();

	  fCancelPayment = false;

	  fInVoidTrans = false;
	  bTrxLimitExceed = false;
	  bTenderLimitExceeded = false;
	  fInATMTransaction = false;
	  fLoyaltyCardTrx = false;
	  fInLoyaltyCardProcessing = false;
	  fLoyaltyCardAwaitVerification= false;
	  bAlreadyPlayedWelcome = false;
  	  bAlreadyPlayedLCReminder = false;	//Tar 200719 BZ 04/02/02
	  nTrxReceiptLength = 0;
	  nTrxJournalLength = 0;
	  fInSuspendTrans = false;
	  fTenderDeclined = false;		   //If Tender is declined
      // Tar 124345
      // bReadyForTakeReceipt = true;   // reset//Tar 126024
      // bTrxClosed = false;

	   //TAR193038, We initial this flag to false
	  bBadCard = false;

	  // TAR 206686
	  bToldRAPMediaLow = false;

	  nTenderType = TB_TENDER_NONE;    // TAR 107134, Maintain Correct Tender Type
	  nTenderButtonIndex = -1;         // TAR 205452
      nSavedTenderButtonIndex = -1;     //TAR 259370
	  ResetAllTenderValues();
      fTenderPending = false;

	  lItems = 0;					   // no items sold yet
	  m_lItemsScanned = 0;			   // TAR 202845 04/30/02 BZ: no items scanned yet

	  lRestrictedItems = 0;
	  lUnapprovedRestrictedItems = 0;
	  lRegularYellowNeedingApproval = 0;
	  m_lAgerestrictQueuedtoRAP = 0;


	  lNotForSaleItems = 0;
      lTimeRestrictedItems = 0;
	  lUnknownItems = 0;
	  lUnknownItemsKeyed = 0;
	  lPriceRequiredItems = 0;
	  lRestrictedNotAllowedItems = 0;
	  lVoidedItems = 0;
	  lUnapprovedVoidedItems = 0;
      lQuantityRestrictedItems = 0;
	  fApprovedForRestrictedItems = false;
	  fAgeObtained = false;
	  nCustomerAge = 0;

	  lCoupons = 0;
	  lDepositedCoupons = 0;
	  lUnapprovedCoupons = 0;
	  nPINTries = 0;
      fCashBack = false;
      bSigCapInProgress = false;

	  lVisualVerifyItems = 0;
	  lUnapprovedVisualVerifyItems = 0;
      bStoreModeApprovals = false;
	  bStoreModeGoBack = false;
	  bForceApprovalBeforeSuspend = false; //TAR 364905

	  lSALoggedMsgs  = 0;				 // total of security agent conclusions
	  lSAYellowNeedingApproval = 0;	     // SA yellow light conclusions awaiting approval
	  lSARedNeedingApproval = false;	     // TAR 197015 Indicates threshold limits

	  lVoidedCoupons = 0;				 // total voided coupons
	  lItemSalesAmountVoided = 0;		 // total amount voided (positive)
	  lCouponAmountVoided = 0;			 // total coupon amount voided (positive)
	  lTotalCouponAmount = 0;			 // total coupons
	  nCountPossibleDuplicateCoupon = 0; // Number of times for duplicate coupon
	  lCouponsInAssist = 0;				 // customer tried to scan a coupon when csTenderAllowCoupons = O - goto approval needed at finish

	  IsEncryptorSetupFail = false;      // flag Y/N for EncryptorSetup Fail
	  lVoidPrc = 0;						 // extended price of item selected for void MGG3
	  lVoidUPrc = 0;					 // unit price of item selected for void MGG3
	  lVoidQty = 0;						 // qty for void
	  lVoidWgt = 0;						 // wgt for void
	  lVoidTcd = 0;						 // tare for void
	  lVoidBarcodeType = -1;			 // 0 is a valid code, it means manually entered



	  csBirthDate = EMPTY_STRING;			 // mmddyy for restricted items need in exit code
	  csItemSent = EMPTY_STRING;		     // last item code we TRIED to sell
	  csLookupItemCode = EMPTY_STRING;     // last item code we TRIED to lookup

	  lTaxDue=0;			     // taxes
	  lBalanceDue=0;			 // balance due from customer
	  lChangeDue=0;				 // change due to customer
	  lTotalSale=0;				 // total amount of sale
	  lTotalPaid=0;				 // total of all tenders
	  lFoodStampsDue=0;			 // total amount of food stamp eligible items
	  lEBTAvailableBalance=0;	 // total amount of Available EBT

	  // remote approval
	  bDropoffRequired = false;
	  bDropoffApproved = false;
      bSuspendRequested = false;
      bSuspendApproved = false;
	  bApplicationErrorApprovalRequired = false;

	  bPartialTenderWithFoodStamps = false;
	  fOtherPayment = false;

	  nDMCashInserted = 0;							// no money unaccounted for in acceptor
      nDMLastCoinEvent = 0;
      nDMLastBillEvent = 0;
	  nDMCashTendered = 0;							// no money unaccounted for in tendering
	  nDMCashInDrawer = 0;							// no money unaccounted for tendered
	  nISIPTimeOut = 0;
      	  fDMErrorCashDue = false;						// Tar 214729 RJF 101002
	  m_bTrxEnded= false;							// TAR 314603
	  pStateAfterISIPTimeOut = STATE_NULL;

	  ps.ResetTransactionLists();	   // Clear all receipts
      io.Reset();
	  itemDetailOrig.Reset();
	  itemDetailNew.Reset();
	  io.bLoyaltyCard = false;
      bSuspendFailure = false;         // TAR #126382
	  extern bool bInSuspendCall;
      bInSuspendCall = false;
      bgAlreadySubmittedWithOverride = false;
	  fCantScanCoupon = false;
	  fUnDeActivatedItem = false;
	  //TAR239862 fRequestForHelp = false;
	  fBeyondFinishAndPayState = false;
	  fTABSafetyCoverOpenApprovalPending = false;

	  // Disable the sensormatic and EAS deactivator while at the attract screen
      dm.EASDeactivatorDisable( );
	  g_lWLDBNotApprovedThisTrans = 0;

	  CustomerMessage = EMPTY_STRING;
	  lTrxTotalSavings = 0;
	  lTrxTotalRewardPoints = 0;

	  bPressHelpOnWay = false;
	  nSkipBaggingBtnPresses = 0;
	  nItemRemovedBtnPresses = 0;	//the number of Done Removing Button Press
	  nBagBoxBtnPresses = 0;
	  csPrevCoupon = EMPTY_STRING;
 	  lEOT_LGW = -999;

      lUserDefinedApproval1 = 0;
      lUserDefinedApproval2 = 0;
      lUserDefinedApproval3 = 0;
      lUserDefinedApproval4 = 0;
      fInEndorsementState = ENDORSE_NONE;

	  g_bMatchedWtWaitingOnEAS = false; //TAR213889 CHU 090502 Start with false in case the wrong one set from previous transaction
	  m_bEASHappened = false;		// 219883 motion sensor event causing false matched wt cmal121002
	  bIsTransactionStarted = false; //TAR219278 CHU 110602 If TBStart() has called: set it to True
	  dm.fTakeawayBeltBackup = false; //TAR216748 CHU 111802 Reset this at the end of trans.
	  rp.InitData(); //Reset all Report data
	  nGenerateEntryIDForTB = INVALID_ENTRYID;
	  m_bHasSeenTakeReceipt = false; //TAR239622 CHU 070203
	  bHasToldRAPAboutBeginCashTender = false;
	  m_bScanOnAttractAtQuietMode = false; //TAR234536 CHu 080403
	  g_bgVerifySig = false;
	  nSellState = BES_START;  // TAR 286061
	  fCancelSuspend = false;  // TAR 286061
	  m_nCashRecyclerFailedRejectAmt = 0;  // TAR 315616
	  m_nDMSavedDev = -1;	   // TAR 318477
	  g_bInterventionApproval = false; //TAR371918 & TAR371932
	  m_bCurrentIntervention_MisMatch = false;     //TAR340654
      m_bCurrentIntervention_UnexIncrease = false; //TAR340654
      m_bCurrentIntervention_UnexDecrease = false; //TAR340654
      m_bCurrentIntervention_DontBagThreshold = false; //TAR433737
	  m_SafePayLastErrorCode = OPOS_SUCCESS;
	  m_bShowMediaStatusButton = false;
	  fInMultiSelectPickList = false;
	  g_bClearOccurred = false; //TAR387490
	  //+SR697
	  m_bEOTPrintPending = false;
	  m_bForceReceiptPrint = false; 
	  //-SR697				
	  ms_lExcludedItems = 0;	// RFC 371075	
	  
	  //+SR713
	  bIsInPreTenderState = false;	
	  bDoNotShowSellBags = false; 
	  //-SR713 

	  //TAR 417255
	  TrainingModeObj.SetTransactionApproved(false);

	  g_lNumberOfItemsNotBagInMultiPick = 0;  //Multipick Enhancement

//USSF START
	  lUnapprovedUSSFItems = 0;
	  if (co.fOperationsEnableUSSF)
	   USSFEVENT(_T("ResetTrxnValues"), ;, ;);
//USSF END
	}

   /////////////////////////////////////////////////////
   void SMStateBase::ResetAllTenderValues(void)
   {
      // nTenderType = TB_TENDER_NONE;  // TAR 107134, Maintain Correct Tender Type
      //*szChargeAccount = 0;
	  //*szChargeExpiry = 0;
	  csChargeAccount = EMPTY_STRING;
	  csChargeExpiry = EMPTY_STRING;
	  csDMEncryptedPin = EMPTY_STRING;
	  csErrDescription = EMPTY_STRING;
	  fDoCashBack = false;
	  // fPIPShowCardProcessingScreen = false; //PIP

//USSF START
	  if (co.fOperationsEnableUSSF)
	    USSFEVENT(_T("ResetTenderValues"), ;, ;);
//USSF END
   }

  /////////////////////////////////////////////////////
   CString SMStateBase::CustomerName(LPCTSTR szCardData)
   {
	 CString csCardData;
     CString Name(_T(" "));   // Customer name on track 1.

     if(szCardData && _tcsclen(szCardData))
	 {
	    short int nT1, nT2, nT3, nHeaderLen;
		short int nShortLen = sizeof(short);

        nT1 = nT2 = nT3 = 0;
	    nT1 = (short ) *szCardData;						//length of Track 1
	    nT2 = (short ) *(szCardData + nShortLen);		//length of Track 2
	    nT3 = (short ) *(szCardData + (nShortLen *2));	//length of Track 3
  	    nHeaderLen = nT1 + nT2 + nT3;

        // Customer name only appears on track 1 data.
        if (nT1)
		{
	      LPTSTR szTrackData = new _TCHAR[nHeaderLen+1];
	      memcpy(szTrackData, (szCardData + (3*nShortLen)), nHeaderLen+1);
	      szTrackData[nHeaderLen] = _T('\0');
	      csCardData = szTrackData;
          delete [] szTrackData;
          int iCarretPos = csCardData.Find(_T('^'));
          CString leftover = csCardData.Mid(iCarretPos+1, nHeaderLen-iCarretPos);
          iCarretPos = leftover.Find(_T('^'));
          CString CustomerName = leftover.Mid(0, iCarretPos);
		  if (CustomerName != EMPTY_STRING)
		  {
		    int separator = CustomerName.Find(_T('/'));
            Name = CustomerName.Right(iCarretPos-separator-1) + _T(" ") + \
			   CustomerName.Mid(0, separator);
		  }
		}
	 }
	 return Name;
   }

  /////////////////////////////////////////////////////
   CString SMStateBase::GetAccountNum(LPCTSTR szCardData)
   {
     trace(L6,_T("+GetAccountNum"));
	 CString csCardData, strAcctNum(_T(" "));

	    short int nT1, nT2, nT3, nHeaderLen;
		short int nShortLen = sizeof(short);

        nT1 = nT2 = nT3 = 0;
	    nT1 = (short ) *(  (const char *)szCardData);						//length of Track 1
	    nT2 = (short ) *( ((const char *)szCardData) + nShortLen);		//length of Track 2
	    nT3 = (short ) *( ((const char *)szCardData) + (nShortLen * 2));	//length of Track 3
  	    nHeaderLen = nT1 + nT2 + nT3;

		  LPTSTR szTrackData = new _TCHAR[nHeaderLen+1];
		  memcpy(szTrackData, ((const char *)szCardData + (3*nShortLen)), nHeaderLen*sizeof(_TCHAR));
		  szTrackData[nHeaderLen] = '\0';
		  csCardData = szTrackData;
		  delete [] szTrackData;
//		  CString strTrack ;
//		  strTrack.Format("+GetAccountNum --Length for Track 1: %d, Track 2: %d, Track 3: %d", nT1, nT2, nT3) ;
//		  trace(L6, strTrack);

		 // Acct Number appears either on Track1 or Track2
        if (nT1)  // check Track 1 first
		{
          int iCarretPos = csCardData.Find('B');
          CString leftover = csCardData.Mid(iCarretPos+1, nHeaderLen-iCarretPos);
          iCarretPos = leftover.Find('^');
          strAcctNum = leftover.Mid(0, iCarretPos);
		}
        else if (nT2) // if not Track 1, check Track 2
		{
//          int iCarretPos = csCardData.Find('b');  // no 'b' on track 2
          int iCarretPos = 0;
//          CString leftover = csCardData.Mid(iCarretPos+1, nHeaderLen-iCarretPos);
          CString leftover = csCardData;
          iCarretPos = leftover.Find('=');
          strAcctNum = leftover.Mid(0, iCarretPos);
		}
     trace(L6,_T("-GetAccountNum"));
	 return strAcctNum;
   }
  ////////////////////////////////////////////////////////////////////
  //
  // restrictedItemNotAllowed
  //
  // This function does not make any assumptions about whether or not
  // a birth date has been obtained for the customer yet.  It returns
  // false if the item is not restricted.  It returns true if the
  // customer has not been proven to be old enough to purchase the
  // restricted item type, either because we have no birthdate yet
  // or because we have the age but it's too low.
  //
  ////////////////////////////////////////////////////////////////////
  bool SMStateBase::restrictedItemNotAllowed()
  {
		// not a restricted item
		if (!io.d.fRestricted)
			 return false;
		// the customer age is already obtained and less than the restricted age
		if (fAgeObtained && (nCustomerAge < io.d.nRestrictedAge))
			return true;

		// Either we haven't obtained a valid date of birth for the customer
		// yet, in which case we allow the restricted item for now, or they
		// are already approved for this restricted item type.
		return false;
  }



  ////////////////////////////////////////////////////////////////////
  //
  // restrictedItemMessage
  //
  // This function does not make any assumptions about whether or not
  // a birth date has been obtained for the customer yet.  It
  // supplies an appropriate message to be used in the case that the
  // customer is not old enough to purchase this type of item.
  //
  ////////////////////////////////////////////////////////////////////
  PSTEXT SMStateBase::restrictedItemMessage()
  {
	 if (io.d.fRestricted && !fApprovedForRestrictedItems)
			 return MSG_VOID_RESTRICTED_ALL;

	   return PS_NOTEXT;
	}

    ///////////////////////////////////////////////////////////
	// Create system messages.  See h file for description.
	///////////////////////////////////////////////////////////
	SMStateBase* SMStateBase::createLevel1Message( LPCTSTR szMsg,
											   PSTEXT pstMsg,
											   PSTEXT prompt,
											   long   value,
											   long   device,
											   bool   checkHealth )
	{
		//TAR 279371+
		if(m_bIsFromLaneClosed)
		{
 			CREATE_AND_DISPENSE(SystemMessage1)(szMsg,pstMsg,prompt,value,device,checkHealth);
		}
		//TAR 279371-
		else
		{
			CREATE_AND_DISPENSE(SystemMessage4)(szMsg,pstMsg,prompt,value,device,checkHealth);
		}
	}

	///////////////////////////////////////////////////////////
	SMStateBase* SMStateBase::createLevel2Message( LPCTSTR szMsg,
											   PSTEXT pstMsg,
											   PSTEXT prompt,
											   long   value,
											   long   device,
											   bool   checkHealth )
	{
	  CREATE_AND_DISPENSE(SystemMessage5)(szMsg,pstMsg,prompt,value,device,checkHealth);
	}


	///////////////////////////////////////////////////////////
	SMStateBase* SMStateBase::createLevel3Message( LPCTSTR szMsg,
											   PSTEXT pstMsg,

											   PSTEXT prompt,
											   long   value,
											   long   device,
											   bool   checkHealth )
	{
	  CREATE_AND_DISPENSE(SystemMessage6)(szMsg,pstMsg,prompt,value,device,checkHealth);
	}


   //////////////////////////////////////////////////////////
   //Print a Journal file. hFile is the handler of the journal file
   /////////////////////////////////////////////////////
   long SMStateBase::PrintJournal(HFILE hFile)
   {

     CString csJournalData;	 // To contain journal data
     DWORD	 nRetValue = 0;

     trace(L6,_T("+PrintJournal"));

	 fDMJournalError = false;	//Flag for journal error

	 try {
	   if (hFile!=NULL) {
		  DWORD	len;
		  unsigned char *pBuff;		// TAR 391271 - Changed type to match PrintReceipt().

#if _MFC_VER < 0x0700
          CFile	cFile( hFile );
#else
          CFile	cFile( (HANDLE)hFile );
#endif

		  len = (DWORD)cFile.SeekToEnd();  // length of Journal file
		  cFile.SeekToBegin();

		  if (len) {
			 pBuff = new unsigned char[len+1];     // TAR 391271
			 if (pBuff) {
				memset(pBuff, 0, len+1);
				UINT c = cFile.Read(pBuff, len);   //Put to pBuff
				if (c) {
				   pBuff[len] = 0;

				   //+RFC 368222
                   bool bUnicode = IsReceiptUnicodeData(pBuff, len);  // TAR 391271
                   if (bUnicode)
                   {
					   len /= 2;		// TAR 391271 - Convert from bytes to # chars.
                       CString csWholeJournalData((wchar_t *)pBuff, len);
                       csJournalData = csWholeJournalData;
                   }
                   else
                   {
                       CString csWholeJournalData((char *)pBuff, len);
                       csJournalData = csWholeJournalData;
                   }
				   //-RFC 368222

				   nRetValue = len;
				}
				delete [] pBuff;
			 }
		  }
	   }
	}
	catch(CFileException *e)
	{
		_TCHAR szError[255] = {NULL};
		e->GetErrorMessage( szError, 255 );
		ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_BS_JOURNAL, _T("%d|%s"), hFile, szError);
		e->Delete();
		return nRetValue;
	}
	if (csJournalData.IsEmpty())
	{
		// a valid file handle for a zero length journal file?
		// though this is not an error, app wants to log this event as an info item
		ScotError(INFO,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_NULL_JOURNALDATA, _T("%d|%d"), hFile, 0);
		return 0;
	}
    DMJournalLine(csJournalData);  // Print to Journal printer
	nTrxJournalLength += nRetValue;

    trace(L6,_T("-DMPrintJournal %d"),nRetValue);
    return nRetValue;
   }

   /////////////////////////////////////////////////////
   void SMStateBase::CutReceipt(void)
   {
     trace(L6,_T("Receipt Print Cut"));
	 DMReceiptCut();
     return;
   }

   ////////////////////////////////////////////////////
   // To create list for Outstanding Approvals
   ////////////////////////////////////////////////////
   CString SMStateBase::GetApprovalNeededString(void)
   {
		CString csWork = EMPTY_STRING;
			if ((co.fOperationsPrintPendingApprovalOnSuspend) && (fInSuspendTrans || co.fOperationsTBControlsPrinter) && (isAnyApprovalRequired())) //Fiscal Printer added TBControlsPrinter
			{
				csWork += _T("\n\n");
				csWork += ps.GetPSText(LTT_WAITFORAPP, SCOT_LANGUAGE_PRIMARY);
				csWork += _T("\n");

			    if (isRestrictedItemApprovalRequired())
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_RESTRICTEDITEMS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (isCouponRatioApprovalRequired())
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_COUPONS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (isSecurityApprovalRequired())
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(BTT_SALOGS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (lNotForSaleItems)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_RECALLEDITEMS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (lTimeRestrictedItems)
			    {
			          csWork += (_T("\n* ")+ps.GetPSText(TXT_TIME_RESTRICTED_ITEM, SCOT_LANGUAGE_PRIMARY));
				}
			    if (lVisualVerifyItems)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_VISUALITEMS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (lUnknownItems)
			    {
					  csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_UNKNOWNITEMS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (lPriceRequiredItems)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_PRICENEEDEDITEMS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (lRestrictedNotAllowedItems)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_RESTRICTEDNOTALLOWEDITEMS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (lQuantityRestrictedItems)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_QUANTITYRESTRICTEDITEMS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (fOtherPayment)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_OTHERPAYMENT, SCOT_LANGUAGE_PRIMARY);
				}
			    if (fCantScanCoupon)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_CANTSCANCOUPON, SCOT_LANGUAGE_PRIMARY);
				}
			    if (fRequestForHelp)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_REQUESTFORHELP, SCOT_LANGUAGE_PRIMARY);
				}
			    if (bDropoffRequired && !bDropoffApproved)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_DROPOFFCOUPONS, SCOT_LANGUAGE_PRIMARY);
				}
			    if (lCouponsInAssist)
			    {
			          csWork += _T("\n* ");
			          csWork += ps.GetPSText(TXT_COUPONSONLYINASSIST, SCOT_LANGUAGE_PRIMARY);
				}
				if (bSuspendRequested && !bSuspendApproved)
				{
					  csWork += _T("\n* ");
				      csWork += ps.GetPSText(RA_TRANS_SUSPEND_ID, SCOT_LANGUAGE_PRIMARY);
				}
				if (fUnDeActivatedItem)
				{
					csWork += _T("\n* ");
				    csWork += ps.GetPSText(RA_TC_UNDEACTIVATEDITEM,
						                   SCOT_LANGUAGE_PRIMARY);
				}

			}
		return(csWork);
   }

//+RFC 368222
//////////////////////////////////////////////////////////////
// Determime whether pData is UNICODE or ANSI
//////////////////////////////////////////////////////////////
bool SMStateBase::IsReceiptUnicodeData(unsigned char* pData, DWORD len)
{
    if ((pData[0] == 0xFF) && (pData[1] == 0xFE))
        return true;
    else if (0 == co.csOperationsReceiptFileFormat.CompareNoCase(_T("UNICODE")))
        return true;
    else if (0 == co.csOperationsReceiptFileFormat.CompareNoCase(_T("ANSI")))
        return false;
    // This code was removed because sometimes ANSI data will match up to the check. 
	// This always happen in EnterSignaturesFromFile.
    // Just specify the UNICODE setting in Scotopts when sending a UNICODE file without a UNICODE header. 
    //else if (( len > strlen((const char*)pData) ) && ( 4 > strlen((const char*)pData)))
    //    return true;    //Attempt dynamic detection of UNICODE data without FFFE header
    else
        return false;
}
//-RFC 368222

//////////////////////////////////////////////////////////////
// Print a receipt file, hFile is the handler of the receipt file
//////////////////////////////////////////////////////////////
long SMStateBase::PrintReceipt(HFILE hFile, bool bSaveReceipt)
{
 if ( !co.fOperationsPrintReceiptOnCancelledTransaction && ( fInVoidTrans || (lVoidedItems > 0 && lItems <= 0))  )
 {
	trace(L6, _T("SMStateBase::PrintReceipt() - Block receipt printing for cancelled transaction."));
	return 0;
 }
 CString csReceiptData;
 DWORD  nRetValue = 0;	// return length of the receipt
 CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;
 GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));
 csLReceiptTmpFile.ReleaseBuffer();

 trace(L6,_T("+DMPrintReceipt"));

 fDMReceiptError = false;

 try {
	   if (hFile!=NULL)
	   {
		  DWORD	len;
		  unsigned char* pBuff; // Intentionally NOT a _TCHAR, to allow conversion

#if _MFC_VER < 0x0700
          CFile	cFile( hFile );
#else
          CFile	cFile( (HANDLE)hFile );
#endif

		  len = (DWORD)cFile.SeekToEnd();
		  cFile.SeekToBegin();
		  if (len)
		  {
			 pBuff = new unsigned char[len+1];
			 if (pBuff)
			 {
				memset(pBuff, 0, (len+1));
				UINT c = cFile.Read(pBuff, len);
				if (c)
				{
                   bool bUnicode = IsReceiptUnicodeData(pBuff, len);	// RFC 368222
				   if (   bUnicode
					   && ((pBuff[0] == 0xFF) && (pBuff[1] == 0xFE)) )
				   {   // Unicode file; strip leading 0xFFFE type indicator
				       len = len/2;
				       CString csWholePrintData((wchar_t*) pBuff, len);
				       csWholePrintData = csWholePrintData.Mid(1);
				       len--;
				       csReceiptData = csWholePrintData;
				   }
				   else if (    bUnicode 
					         || (( len > strlen((const char*)pBuff)) && (4 > strlen((const char*)pBuff))) )
				   {
#ifndef UNICODE
					   CString csWholePrintData((char*) pBuff, cFile.GetLength());
					   csReceiptData = csWholePrintData;
#else
					   len = len/2;
                       CString csWholePrintData((wchar_t*) pBuff, len);
					   csReceiptData = csWholePrintData;
#endif
				   }
				   else
				   {   // ANSI file; no conversion needed
				       CString csWholePrintData((char*) pBuff, len);
				       csReceiptData = csWholePrintData;
				   }
				   nRetValue = len;
				}
				delete [] pBuff;
			 }
		  }
	   }
 }
 catch(CFileException *e)
 {
	// may be an invalid file handle
	_TCHAR szError[255] = {NULL};
	e->GetErrorMessage( szError, 255 );
	ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_BS_RECEIPT,_T("%d|%s"),hFile, szError);
	e->Delete();
	return nRetValue;
 }

 if ((co.fUserExitAllEnabled) &&    // if exits enabled and
	(co.fUserExit15Enabled))        //    this exit is enabled
 {
   if (UserExit15(csReceiptData) != 1)   // if userexit return 1, means no print
   {
		// csReceiptData might have been got modified by user exit
		nRetValue = csReceiptData.GetLength();
   }
 }

 if (csReceiptData.IsEmpty())
 {
	// valid file handle but file is of zero length
	// though this is not an error, app wants to log this event as an info item
	ScotError(INFO,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_NULL_RECEIPT, _T("%d|%d"), hFile, 0);
	return 0;
 }

 if (TrainingModeObj.IsTMOn() && bSaveReceipt) //TAR 418786
 {
	 CString csTrainingPrintLine;
	 CString csTrainingLine = ps.GetPSText(MSG_TRAINING_MODE_LINE,SCOT_LANGUAGE_PRIMARY);

	 //+345351
	 int len = csReceiptData.GetLength();
	 if(len >= 3  )
	 {
		 //see if there Paper Cut escape at the end. If so, insert training mode line before Paper Cut
		 CString csTemp = csReceiptData.Right(7);
		 int offset;
		 if((offset = csTemp.Find(_T("\x1B|"))) != -1
			 && !(csTemp.Right(1)).CompareNoCase(_T("P")))
		 {
			 int index = len-7 + offset;

			// check if line feed escape sequence exists. If so, do the insert before line feed sequence also
			CString csTemp2 = csReceiptData.Mid(index - 7, 7);
			int offset2;
			if((offset2 = csTemp2.Find(_T("\x1B|"))) != -1
			 && !(csTemp2.Right(2)).CompareNoCase(_T("lf")))
			{
				index = index-7 + offset2;
			}

			csTrainingPrintLine = GetWideCharToMultiByte(csTrainingLine);			
			csReceiptData.Insert(index, (LPCTSTR)csTrainingPrintLine);
		 }
		 else
		 {
			csTrainingPrintLine = GetWideCharToMultiByte(csTrainingLine);			
			csReceiptData += csTrainingPrintLine;
		 }
	 }
	 else
	 {
		 csTrainingPrintLine = GetWideCharToMultiByte(csTrainingLine);			
		 csReceiptData += csTrainingPrintLine;
	 }
	 //-345351

 }

 CString csPrintApprovalList = GetApprovalNeededString();

 //modify the next if branch to add RFC 334227 - Print Duplicate on all duplicate receipts
 if (csPrintApprovalList.GetLength() || ( co.isPrintDuplicateMsgonPrintLastReceipt() && m_fPrintDuplicateTrailer ) )
 {

	 //+345351
	 int len = csReceiptData.GetLength();
	 if(len >= 3)
	 {
		 //see if the Paper Cut escape at the end. If so, insert csPrintApprovalList before Paper Cut
		 CString csTemp = csReceiptData.Right(7);
		 int offset;
		 if((offset = csTemp.Find(_T("\x1B|"))) != -1
			 && !(csTemp.Right(1)).CompareNoCase(_T("P")))
		 {
			int index = len-7 + offset;
			// check if line feed escape sequence exists. If so, do the insert before line feed sequence also
			CString csTemp2 = csReceiptData.Mid(index - 7, 7);
			int offset2;
			if((offset2 = csTemp2.Find(_T("\x1B|"))) != -1
			 && !(csTemp2.Right(2)).CompareNoCase(_T("lf")))
			{
				index = index-7 + offset2;
			}
			CString csTextLine;
			if ( csPrintApprovalList.GetLength() )
			{
				csTextLine = GetWideCharToMultiByte(csPrintApprovalList);
			}
			if ( co.isPrintDuplicateMsgonPrintLastReceipt() && m_fPrintDuplicateTrailer )
			{
				csTextLine += GetWideCharToMultiByte(ps.GetPSText( TXT_DUPLICATE_RECEIPT ));
			}
			csReceiptData.Insert(index, (LPCTSTR)csTextLine );
 		 }
		 else
		 {
			 if ( csPrintApprovalList.GetLength() )
			 {
				csReceiptData += GetWideCharToMultiByte(csPrintApprovalList);
			 }
			if ( co.isPrintDuplicateMsgonPrintLastReceipt() && m_fPrintDuplicateTrailer )
			{
				csReceiptData += GetWideCharToMultiByte(ps.GetPSText( TXT_DUPLICATE_RECEIPT ));
			}
		 }
	 }
	 else
	 {
		if ( csPrintApprovalList.GetLength() )
		{
			csReceiptData += GetWideCharToMultiByte(csPrintApprovalList);
		}
		if ( co.isPrintDuplicateMsgonPrintLastReceipt() && m_fPrintDuplicateTrailer )
		{
			csReceiptData += GetWideCharToMultiByte(ps.GetPSText( TXT_DUPLICATE_RECEIPT ));
		}
	 }
	 //-345351

	m_fPrintDuplicateTrailer = false;//RFC 334227


 }
 // if there is print data, ...
 nRetValue = csReceiptData.GetLength();

 if (bSaveReceipt)

 {
	 //for Print Last Receipt module
	 if (TrainingModeObj.IsTMOn())
	 {
	  SaveLastReceiptData(csLReceiptTmpFile, ps.GetPSText(MSG_TRAINING_MODE_LINE,SCOT_LANGUAGE_PRIMARY)); //save last receipt data from TM
	 }
	 if (csPrintApprovalList.GetLength())
	 {
		SaveLastReceiptData(csLReceiptTmpFile, csPrintApprovalList); //save approval list
	 }
	 //end of for Print Last Receipt module
 }

 DMReceiptLine(csReceiptData);   // Print to Receipt

  //tar 266257 if csReceiptData contains the "Paper Cut" Esc sequence at the end, SCOTAPP don't cut paper again.
 // Reset nRetValue to 0. The "Paper Cut" Esc sequence's format is "\x1B|#p", "\x1B|#fp" or "\x1B|#sp". # will be replaced
 // by the percentage cut desired.

 if(nRetValue >= 3  )
 {
	 CString csTemp = csReceiptData.Right(7);
	 if(csTemp.Find(_T("\x1B|")) != -1
		 && !(csTemp.Right(1)).CompareNoCase(_T("P")))
	 {
		if(csTemp.Find(_T("\x1B|100P")) == 1)	//Last command is paper Cut
			nRetValue = 1;
		else
			nRetValue = 0;
	 }
 }

 nTrxReceiptLength += nRetValue;

 trace(L6,_T("-DMPrintReceipt %d"),nRetValue);
 return nRetValue;
}

   /////////////////////////////////////////////////////
   void SMStateBase::UpdateDepositedCouponVars()
   {
	 lDepositedCoupons += lCoupons;
	 lCoupons = 0;
	 lUnapprovedCoupons = 0;
	 if(nCountPossibleDuplicateCoupon >= co.nOperationsDuplicateCouponMax)
	 {
		 csPrevCoupon = EMPTY_STRING;
		 nCountPossibleDuplicateCoupon = 0;
	 }
   }

   /////////////////////////////////////////////////////
   SMStateBase *SMStateBase::DepositCouponsIfNecessary()
   {
	  //TAR126499  DropOffCoupons screen now displays before finish state (tendering)
      //           The following if() statement was backwards LPM111899
      if (co.IsInAssistMenus())
      {
          return setAndCreateAnchorState(BES_SCANANDBAG);
      }
      else if (bDropoffRequired)
	  {
		 RETURNSTATE(DropoffCoupons)
	  }
	  else
	  {
	 	 //if (co.fStateHasSidecar)
			 RETURNSTATE(DepositCoupons)
		 /*else
			 {
			   UpdateDepositedCouponVars();
			   return setAndCreateAnchorState(BES_FINISH);
			 }*/
	  }
   }

   /////////////////////////////////////////////////////
   void SMStateBase::GetTrxLimitString(const PSTEXT nTextID, CString& csMsgText)
   {
     long lTrxLimit = 0;
	 _TCHAR szLimit[20] = {_T('\0')};

	 lTrxLimit = tb.GetTransactionLimit();
	 CString csAttractOrigText(ps.GetPSText(nTextID));
	 csMsgText.Format(csAttractOrigText, ps.FmtDollar(lTrxLimit));
   }

//////////////////////////////////////////////////////////////////
// FZ and NN 080102 RFC 187993
void SMStateBase::ReadOperatorLoginBarcodeMask(CString csOperatorLoginBarcodeMask)
{
	trace(L2, _T("+ReadOperatorLoginBarcodeMask  <%s>"), csOperatorLoginBarcodeMask);

	CString csOperatorStoreLoginBarcodeUserid;
	CString csOperatorStoreLoginBarcodePassword;

	// FZ20020730 Check Mask Length
	switch(co.nOperationsOperatorLoginBarcodeType)
	{
		case 0:
			return;
		case 1:
			if(csOperatorLoginBarcodeMask.GetLength()> 11)
			{
				InvalidOperatorLoginBarcodeMask(csOperatorLoginBarcodeMask);
				trace(L2, _T("Invalid mask for EAN13 type barcode <%s>"), co.csOperationsOperatorBarcodeMask);
				return;
			}
			break;
		case 2:
			if(csOperatorLoginBarcodeMask.GetLength()> 24)
			{
				InvalidOperatorLoginBarcodeMask(csOperatorLoginBarcodeMask);
				trace(L2, _T("Invalid mask for code 128 type barcode <%s>"), co.csOperationsOperatorBarcodeMask);
				return;
			}
			break;

		default:
			{
				InvalidOperatorLoginBarcodeMask(csOperatorLoginBarcodeMask);
				trace(L2, _T("Wrong Operator Bacode Scan Type  <%d>"), co.nOperationsOperatorLoginBarcodeType);
				return;
			}
	}

	csOperatorStoreLoginBarcodeMask = csOperatorLoginBarcodeMask;
	csOperatorLoginBarcodeMask.MakeUpper();
	int nSigatureLength = csOperatorLoginBarcodeMask.Find(_T('U'));
	if(nSigatureLength == -1)
	{
		InvalidOperatorLoginBarcodeMask(csOperatorLoginBarcodeMask);
		return;
	}
	csOperatorStoreLoginBarcodeSignature = csOperatorStoreLoginBarcodeMask.Left(nSigatureLength);

    int nPasswordIndex = csOperatorLoginBarcodeMask.Find(_T('P'));
	if(nPasswordIndex == -1)
	{
		InvalidOperatorLoginBarcodeMask(csOperatorLoginBarcodeMask);
		return;
	}

	csOperatorStoreLoginBarcodeUserid = csOperatorLoginBarcodeMask.Mid(nSigatureLength, nPasswordIndex-nSigatureLength);
	nOperatorStoreLoginBarcodeUseridLength = csOperatorStoreLoginBarcodeUserid.GetLength();


	nOperatorStoreLoginBarcodePasswordLength = csOperatorLoginBarcodeMask.GetLength() - nPasswordIndex;
	csOperatorStoreLoginBarcodePassword = csOperatorLoginBarcodeMask.Right(nOperatorStoreLoginBarcodePasswordLength);

	//Need to check the userid only contains U and password only contains P
	csOperatorStoreLoginBarcodeUserid.Remove(_T('U'));
	csOperatorStoreLoginBarcodePassword.Remove(_T('P'));
   if(csOperatorStoreLoginBarcodeUserid !=_T("") || csOperatorStoreLoginBarcodePassword !=_T(""))
   {
	   InvalidOperatorLoginBarcodeMask(csOperatorLoginBarcodeMask);
	   trace(L2, _T("Mask can only have u and p <%s>"), co.csOperationsOperatorBarcodeMask);
   }


	trace(L2, _T("-ReadOperatorLoginBarcodeMask  <%s>"), csOperatorStoreLoginBarcodeMask);

	return;
}
/////////////////////////////////////////////////////////////////////////////
//FZ and NN 080102 RFC 187993 Log error in the event log
void SMStateBase::InvalidOperatorLoginBarcodeMask(CString csOperatorLoginBarcodeMask)
{

	ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_OPERATORLOGINBARCODEMASK, _T("%s"), csOperatorLoginBarcodeMask);
	nOperatorStoreLoginBarcodePasswordLength = 0;
	nOperatorStoreLoginBarcodeUseridLength = 0;
	csOperatorStoreLoginBarcodeSignature = _T("");

	return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// isBarcodeValidOperatorPassword
// Scannable Manager ID/Password check
// Scanned manager ID has the following format
//   F411ooooppppmx
// where
//   F	-- Scanner added indicating type of barcode
//	411	-- old SCOT unique indicator for manager ids
//      OR
//  412 -- new SCOT unique identifier for manager ids (as of 4/18/00)
//   o	-- operator number
//   p	-- encoded password  XOR'ed with decimal 3456
//   m	-- mod 10 check digit calculated
//   x	-- actual JAN 13 check digit -- not sure of algorithm
//
// Returns true/false
//   true		-- scaned item matches manager format
//						 csOperator and csPassword will be set to correct values
//   false	-- scanned item DOES NOT match manager format
bool SMStateBase::isBarcodeValidOperatorPassword(const CString csItemCode, CString *pcsOperatorID)
{
	bool isValid = false;
    CString csOpID, csPswd;
         if (!co.IsAttendantModeOn() && !storeMode())  //TAR 293492 When we are in attendant mode, csOperator and csPassword should already contain valid
                                  // ID/Password, so don't empty when in attendant mode.
								  // + mm185096 TAR 330190 And also in Store Mode
	{
		csOperator = EMPTY_STRING;  // TAR 253541
		csPassword = EMPTY_STRING;  // TAR 253541
	}
    long lTemp;

	//+Tar219151
	//If the bar code starts with "B3908" then only print the
	//first 6 characters so we don't exposer customer data.
	if ((csItemCode.Find(_T("B3980")) != -1) && (co.csUserExitString5 == _T("OmitCustSensitiveDataInTraces")))
	{
	   trace(L2, _T("+isBarcodeValidOperatorPassword  <%s>"), csItemCode.Mid(0,6));
	}
	else  //It's ok.
	{
	   trace(L2, _T("+isBarcodeValidOperatorPassword  <%s>"), csItemCode);
	}
	//-Tar219151


	CString csBarcodeScanType, csLastNum;
	CString csDecrypt = _T("");
	CString csAnalyzeThis = _T("");

    //RFC 240738 allow option for code 128 barcodes to contain leading zeros
    bool fAllowLeadingZeros = ((co.nOperationsOperatorLoginBarcodeType == 2)  && co.fOperationsAllowLeadingZerosInCode128LoginBarCode);

	//append scanner added character to the barcode
	switch(co.nOperationsOperatorLoginBarcodeType) //FZ20020730 RFC 187993
	{
		case 0:
			isValid = false;
			break;
		case 1:
			csBarcodeScanType = _T("F");
			{
				CString cItemCode = csItemCode;
				int index;
				CString csTemp;

                index = csItemCode.Find(_T('~'));
                if (index == -1)
                    break;

				//TAR 349173 - change from Right to Mid.
                csTemp = csItemCode.Mid(index+1);

                index = csTemp.Find(_T('~'));
                if (index == -1)
                    break;

				//TAR 349173 - change from Right to Mid.
                csTemp = csItemCode.Mid(index+1);
                if (csTemp == _T("104"))
                {
                    //if (csItemCode[0] != _T('F'))
                    //    csAnalyzeThis = _T('F') + csItemCode;
					//+TAR 349173
					if (isalpha(csItemCode[0]))
					{
						trace (L6, _T("csItemCode[0] = %c"), csItemCode[0]);
						if (cItemCode[0] != _T('F'))
						{
							//the base doesn't have this. if the first char is alpha but not F,
							//overwrite it with F.
							cItemCode.SetAt(0, _T('F'));
							trace (L6, _T("New item code = %s. Forcing 'F' at the beginning."), cItemCode);
						}
					}
					else
					{
						cItemCode = _T('F') + cItemCode;
						trace (L6, _T("csItemCode[0] is numeric. Prepend 'F' at the beginning"));
					}
					csAnalyzeThis = cItemCode;
					//-TAR 349173
                }
            }
			break;
		case 2:
			csBarcodeScanType = _T("B3");

			{
				int iTemp, iEncryptStrLen = 0, iRegLen;
				CString csTemp, csEncryptOrig, csEncrypt, csTemp2, csTilda;
				bool bFindTilda;

                // remove the B3, so we can find the tilda
				csEncrypt = csItemCode.Right(csItemCode.GetLength() -2);
				csEncryptOrig = csEncrypt;
				// See if printed from a K580 or K590 (128C or not)
				if (csEncryptOrig.Find(csOperatorStoreLoginBarcodeSignature) == 0)
				{
                    break; //not so no need to convert from ascii
				}
                //TAR 214029 CM/NN 090602 Temp workaround for known problem with K590 printer
				// FIND THE TILDA
				iRegLen= csEncrypt.GetLength() / 2;
				bFindTilda = false;
				for (int ij = 1; ij <= iRegLen; ij++)
				{
					csTemp = csEncrypt.Left(2); // remove first 2 digits
					if(csTemp[0] == _T('~'))//RFC 240738
					{
						// found the 'tilda' equivalent & remember it
						bFindTilda = true;
						csTilda = csTemp[0];
						iEncryptStrLen = ij*2;
						break;
					}
					// now remove those 1st 2 digits, so can work on next few
					csTemp = csEncrypt.Right(csEncrypt.GetLength() - 2);
					csEncrypt = csTemp;
				} // find tilda

				// IF Looks OK then dissect
				if ((iEncryptStrLen > 0) && (bFindTilda == true))
				{
					// DECRYPT for K590 the encrypted left parm (up to the tilda)
					csTemp		  = csItemCode.Left(iEncryptStrLen);

					csTemp2       = csItemCode.Right(csItemCode.GetLength() - iEncryptStrLen - 1);
					int iEncryptLastNum = csTemp2.Find(csTilda); // was "~"
					csLastNum       = csTemp2.Right(csTemp2.GetLength() - iEncryptLastNum - 1);

					// remove the B3 from the start
					csEncryptOrig = csTemp.Right(iEncryptStrLen - csBarcodeScanType.GetLength());
					csEncrypt     = csEncryptOrig;
					int iDoubleLen = csEncryptOrig.GetLength();

					if ((iDoubleLen % 2) == 0)
					{
						iRegLen = iDoubleLen / 2;

						// now do the dirty work
						for (int ij = 1; ij <= iRegLen; ij++)
						{
							csTemp = csEncrypt.Left(2); // remove first 2 digits
							iTemp  = _ttoi(csTemp);		// convert to number
                            csTemp.Format(_T("%c"),iTemp); //RFC 240738
							csDecrypt += csTemp;

							// now remove those 1st 2 digits, so can work on next few
							csTemp = csEncrypt.Right(csEncrypt.GetLength() - 2);
							csEncrypt = csTemp;
						}
					}
					// else not an even number of digits for Type C?
				}
				// UnEncrypted Barcode...
				csTemp = csBarcodeScanType + csDecrypt;
				csTemp += _T("~");
				csTemp += csDecrypt;
				csTemp += _T("~");
				csTemp += csLastNum;
				csAnalyzeThis = csTemp;
			}
			break;

		default:
			{
				trace(L2, _T("Wrong Operator Bacode Scan Type  <%d>"), co.nOperationsOperatorLoginBarcodeType);
				isValid = false;
			}
	}//end switch
	if (csAnalyzeThis == _T(""))
	csAnalyzeThis = csItemCode;

	CString csItemCodeNoType = csAnalyzeThis.Right(csAnalyzeThis.GetLength()-csBarcodeScanType.GetLength());


	if (csAnalyzeThis.Find(_T("F411")) == 0)   //old method of encoding barcode, left for backward compatibility
	{
        csOpID = csAnalyzeThis.Mid(4,4);
        lTemp = _ttol( csOpID );
        csOpID.Format(_T("%d"),lTemp);
        csPswd = csAnalyzeThis.Mid(8,4);
		lTemp = _ttol( csPswd );
		lTemp ^= 3456;
        csPswd.Format(_T("%d"), lTemp);
  	    if (TBValidateOperator(csOpID, csPswd)==1)
        {
            isValid = true;
            csOperator = csOpID;
	        csPassword = csPswd;
            if(pcsOperatorID)
            {
                *pcsOperatorID = csOpID;
            }
			trace(L7, _T("+rp.SetOperator"));
            rp.SetOperator(csOperator);
			trace(L7, _T("-rp.SetOperator<%s>"),csOperator );
        }
	}
    else if	(csBarcodeScanType == csAnalyzeThis.Left(csBarcodeScanType.GetLength()))
	{
		if(csItemCodeNoType.GetLength() < csOperatorStoreLoginBarcodeMask.GetLength())
		{
			isValid = false;

		}
		else if(csItemCodeNoType.Left(csOperatorStoreLoginBarcodeSignature.GetLength()) == csOperatorStoreLoginBarcodeSignature)  //FZ20020730 //new method of encoding barcode, fixed password range problem
		{
			//csOpID = csItemCode.Mid(4,4);
			csOpID = csItemCodeNoType.Mid(csOperatorStoreLoginBarcodeSignature.GetLength(), nOperatorStoreLoginBarcodeUseridLength);
			//lTemp = atol( csOpID );
			//csOpID.Format("%d",lTemp);
            if(!fAllowLeadingZeros) //RFC 240738
            {
                while(!csOpID.IsEmpty() && csOpID[0] == _T('0'))
                {
                    csOpID = csOpID.Mid(1);  //remove leading 0s
                }
            }
            else
            {
                csOpID.TrimLeft();
            }
			csPswd = csItemCodeNoType.Mid(csOperatorStoreLoginBarcodeSignature.GetLength()+nOperatorStoreLoginBarcodeUseridLength,nOperatorStoreLoginBarcodePasswordLength);
			int nDigit;
			for (int i=0; i < nOperatorStoreLoginBarcodePasswordLength; i++)  //unmask the password
			{
				//decode alphanumberic password
				if(csPswd[i]<_T('0'))     //Ascii from 0-47
				{
				  nDigit = csPswd[i] - _T(' ') - (i+3);
				  while (nDigit < 0) nDigit += 48;
				  csPswd.SetAt(i, nDigit + _T(' '));
				}
				else if(csPswd[i]>_T('9'))  //Ascii from 58-255
				{
				  nDigit = csPswd[i] - _T(':') - (i+3);
				  while (nDigit < 0) nDigit += 198;
				  csPswd.SetAt(i, nDigit + _T(':'));
				}
				else // 0-9  Ascii from 48-57
				{
				  nDigit = csPswd[i] - _T('0') - (i+3);
				  while (nDigit < 0) nDigit += 10;
				  csPswd.SetAt(i, nDigit + _T('0'));
				}
			}

            if(!fAllowLeadingZeros) //RFC 240738
            {
                while(csPswd.GetLength()>1 && csPswd[0] == _T('0')) // tar 261559
                {
                    csPswd = csPswd.Mid(1);  //remove leading 0s
                }
            }
            else
            {
                csPswd.TrimLeft();
            }
  			if (TBValidateOperator(csOpID, csPswd)==1)
			{
				isValid = true;
				csOperator = csOpID;
				csPassword = csPswd;
                if(pcsOperatorID)
                {
                    *pcsOperatorID = csOpID;
                }
			}
		}
	}

	if (isValid)
	{
		bValidScannedOperator = true;
	}
	else
	{
		bValidScannedOperator = false;
	}

	trace(L2, _T("-isBarcodeValidOperatorPassword  <%d>"), isValid);
	return isValid;
}


SMStateBase* SMStateBase::ReloadOptions(void)
{
	USSFutil.DumpSymbolsByElementID();

	bool bOrigFreezeState = fStateFreeze;
	fStateFreeze = true;
	fDoOptions = true;
	ps.SetDisplayWMFlag(true); //refresh W&M info after reload option - improve screen display
	ps.Echo(PS_BLANK);

//USSF Start
	if (co.fOperationsEnableUSSF)
	   USSFEVENT(_T("ReloadOptions"), if (nextState != STATE_NULL){return nextState;}, ;);
//USSF End 

    //let Transaction Broker reload its options first
    ps.Echo(TB_LOADING);
    DMSayPhrase(KEYPADVALID);
    TBLoadOptions();
    ps.Echo(TB_LOADED);

	// load configuration options files 000 and 'xxx'
	ps.Echo(CO_LOADING);              // again
	DMSayPhrase(KEYPADVALID);

    bool bInAttendantMode = co.IsAttendantModeOn();

	int nTermNumber = TBGetTerminalNumber();
  SCDownload( nTermNumber);  // TAR 287143
	if (!COInitialize(nTermNumber))
	{
		fStateFreeze = false;
		ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_CO_INIT);
		return STATE_RESTART;
	}
	to.SetTimeToFlush( co.GetmsTraceToFlush() );	//THD Performance RFC

	co.InitCustomerBagAllowed(dm.fStateTakeawayBelt); // TAR 393731

    if (bInAttendantMode && !co.fStateAttndModeAllowed)
    { //TAR 300284
        trace(L6, _T("Exiting Attendant mode"));
        if (co.IsRemoteScannerConnected())
        {
            dm.ScannerRemoteAccess(false);
            co.SetRemoteScannerConnected(false);
        }
        rp.SetOperator(csOperator);
        co.SetAttendantID(EMPTY_STRING);
        co.SetAttendantMode(false);
        if (dm.fStateTakeawayBelt)
        {
            CKeyValueParmList parmList;
            BSTR bstrResult;
            CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            bstrResult = NULL;
            SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
        }
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
        rp.ModeChange(rp.m_lAttendantEntryID, ATTENDANT, EXIT);
        DMTakeawayBeltNotify(tabEventAssistModeStop);
        ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());
    }
    else if (bInAttendantMode)
    {
        co.SetAttendantMode(true); //retain attendant mode setting
    }
	ps.Echo(CO_LOADED);

	// RFC 219283 DE/RF Touchscreen Motion sensor enables 7883 scanner motor
	SMOutOfServiceBase dlg;
    dlg.MotionSensorEnd();
    dlg.MotionSensorStart();

	// Update the scale unit
	UpdateScaleUnits();

	if (CheckForDATFile(true) == STATE_ENDED)	//awk.7.01 TAR 173990
	{
		fStateFreeze = false;
		fStateAssist = false;
		return STATE_STOP;
	}


	//load logo in printer memory
    dm.LoadPrintObjects();
	DMCheckForCoinDispenserLowSensor(); //tar 312039

	// now finish loading PS allowing copy of new text files and load 50-keyboard
	ps.Echo(PS_LOADING);
	DMSayPhrase(KEYPADVALID);

	if (!SCOTTENDInitialize(nTermNumber))
	{
		fStateFreeze = false;
		ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_CO_INIT);
		return STATE_RESTART;
	}
	ps.Echo(CO_LOADED);

	// now finish loading PS allowing copy of new text files and load 50-keyboard
	ps.Echo(PS_LOADING);
	DMSayPhrase(KEYPADVALID);

	//if (fDoOptions)
	//	ps.UnInitializeConfigData();

	if (PSInitialize()<0)	// errors handled in called methods
	{
		fStateFreeze = false;
		ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_PS_INIT);
		return STATE_RESTART;
	}
   // TAR 284785 start - ensure custom text has been loaded
	if (co.fStateDualLanguage)
   {
      ps.AssignTextToCtlsInCmCtxs(NONEPRIMARYCONTEXT);
      m_LanguageUsedPreviously=m_customerLanguage;
      ps.SetRemoteLanguage(co.language[0].szCode);
   }
	else
	{
		ps.SetRemoteLanguage(co.csPrimaryLanguageCode);
	}
   // TAR 284785 end
	ps.Echo(PS_LOADED);

    // Reload the options for the RemoteConsole manager component
	ps.Echo(RA_LOADING);
	if (ra.ReloadOptions()<0)	// errors handled in called methods
	{
		fStateFreeze = false;
		ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_RA_INIT);
		return STATE_RESTART;
	}
	ps.Echo(RA_LOADED);


    // load Security Manager configuration file
	ps.Echo(MSG_SEC_MGR_LOADING);
    DMSayPhrase(KEYPADVALID);
    CKeyValueParmList parmList;
   	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_RELOAD_OPTIONS);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
    BSTR bstrResult;
	bstrResult = NULL;
	SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
	SecurityControlParms SCRelaodparms;
	CString csResult(bstrResult);
	SCRelaodparms.ParseKeys(csResult);
	if ( SCRelaodparms.m_csReloadOptionsStatusText != _T("") )
	{
		fStateFreeze = false;
		ScotError(FATAL,SM_CAT_STARTUP_INIT, SCOTAPP_SEC_MGR_PROCEDURE_INIT_FAILED, _T("%s"), SCRelaodparms.m_csReloadOptionsStatusText);
		return STATE_RESTART;
	}
	// Need to look at bstrResult on return
	SysFreeString(bstrResult);


    g_bDelayedSecIntervention = GetSecurityConfig(CONFIG_KEY_DELAY_EXCEPTIONS);
    g_bSubstCkPriceEmbeddedWeights = GetSecurityConfig(CONFIG_KEY_SUBST_CK_PRICE_EMBEDDED_WT);
	trace(L6, _T("SubstCkPriceEmbeddedWeights items = %d."), g_bSubstCkPriceEmbeddedWeights);
	g_bIsForgiveUnexpectedDecreaseDuringTendering = GetSecurityConfig(CONFIG_KEY_FORGIVE_UNEX_DEC_DURING_TENDERING);
	//sr714+ - add condition
	if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_1) != _T("") )
	{
		g_csTareWt1 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_1); 
	}
	if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_2) != _T("") )
	{
		g_csTareWt2 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_2); 
	}
	if( GetSecurityConfigValue(CONFIG_KEY_TARE_WT_3) != _T("") )
	{
		g_csTareWt3 = GetSecurityConfigValue(CONFIG_KEY_TARE_WT_3); 
	}
	//sr714-

	// set the value of ScaleMetric option in the Security Manager
	if (co.fOperationsScaleMetric)
	{
		SetSecurityConfig( CONFIG_KEY_SS_METRIC_SCALE, _T("true"));
	}
	else
	{
		SetSecurityConfig(CONFIG_KEY_SS_METRIC_SCALE, _T("false"));
	}

	if (co.fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen)
	{
		SetSecurityConfig( CONFIG_KEY_REMOVE_ITEM_VIOLATION_AUTO_ADVANCE, _T("true"));
	}
	else
	{
		SetSecurityConfig(CONFIG_KEY_REMOVE_ITEM_VIOLATION_AUTO_ADVANCE, _T("false"));
	}

    ps.Echo(MSG_SEC_MGR_LOADED);

    ShrinkPicklistImages();

	// This section is no longer used !!!!!!!!!!!!!!!!!!
	// Please don't merge back in code .................Tina - Jan 20, 2002
	// security agent initialization
    //	ps.Echo(SA_LOADING);
	//DMSayPhrase(KEYPADVALID);
	//if (ReloadSASIActions()<0)             // errors handled in called methods
	//{
	//	fStateFreeze = false;
    //	trace(L0, "Failed to re-load SASI Actions during load options.");
    //	ScotError(RETURN,SM_CAT_STARTUP_INIT, SCOTAPP_SA_INIT);
	//	return STATE_RESTART;
	//}
	//ps.Echo(SA_LOADED);
	//Sleep(100);

	ps.Echo(BTT_OK);
	Sleep(100);

	fDoOptions = false;
	fStateFreeze = bOrigFreezeState;
	ps.Echo(_T("_"));

	//+4.2 provide subtle on-screen volume control
	//check if ShowVolumeControl in 'Scotopts.dat' is set to "Y".
	if (co.IsShowVolumeControl())
	{
		CVolumeControl VolumeControl(SETDEFAULT); //volume is set to default
	}
	//-4.2 provide subtle on-screen volume control
	if (bLaneClosed)
	{
		// while on lane closed state, SCOT App doesn't want to inform
		// TB about intermediate lane changes
		bLaneClosed = false;
		UnInitialize();
		bLaneClosed = true;
	}
	else
	{
		UnInitialize();
	}
	return Initialize();
}

// TAR 287143 start
//////////////////////////////////////////////////////////////////////////////////////////////////
// caller should provide valid SCOT terminal number
// Security Configuration file is downloaded here,
// return true on successfull initialization
//////////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SCDownload(int nTerminalNumber)
{
	trace(L6, _T("+SCDownload"));

	CString csExt, csFileName;
	CString csOptionFile = SECCONFIG;
	CString csDestPath = co.csConfigFilePath+_T("\\");

	// Get Store Level Security Configuration File
	csExt = _T(".000");
	csFileName = csOptionFile + csExt;
	if (TB_SUCCESS != CopyFileFromServer(csFileName, csDestPath+csFileName, false))	// destination path is null
	{
		trace(L6, _T("Failed to copy file %s from server"), csFileName);
	}

	// Get Terminal Level Security Configuration File
	csExt.Format(_T(".%03d"), nTerminalNumber);
	csFileName = csOptionFile + csExt;
	if (TB_SUCCESS != CopyFileFromServer(csFileName, csDestPath+csFileName, false))	// destination path is null
	{
		trace(L6, _T("Failed to copy file %s from server"), csFileName);
	}

	trace(L6, _T("-SCDownload"));
}
// TAR 287143 end

//////////////////////////////////////////////////////////////////////////////////////////////////
// caller should provide valid SCOT terminal number
// Config object is intialized here,
// bLocal = true   : options files are taken from local disk
// bLocal = false  : options files are taken from host application by calling CopyFileFromServer()
// bLocal defaults to false
// return true on successfull initialization
//////////////////////////////////////////////////////////////////////////////////////////////////
bool SMStateBase::COInitialize(int nTerminalNumber, bool bLocal)
{
	if (!bLocal)
	{
		// get files from server to local path
		CString csOptionFile, csExt, csFileName;
		CString csDestPath = co.csConfigFilePath+_T("\\");
		TBRC nTBCallStatus;

		// get generic SCOT option file from server
		// Here SCOT App assumes that TB knows the config file path
		// since SCOT passes the path as an argument to TB on TBInitialize()
		csOptionFile = SCOTOPTS;
		csExt = _T(".000");
		csFileName = csOptionFile + csExt;

		nTBCallStatus = CopyFileFromServer(csFileName,csDestPath+csFileName,false);	// destination path is null
		/* no need to complain about optional configuration files
		ASSERT(nTBCallStatus == TB_SUCCESS);
		if (nTBCallStatus != TB_SUCCESS)
		{
			return false;
		}
		*/
		// copy terminal specific option file from server
		csExt.Format(_T(".%03d"), nTerminalNumber);
		csFileName = csOptionFile + csExt;
		nTBCallStatus = CopyFileFromServer(csFileName,csDestPath+csFileName,false);	// destination path is null
		/* no need to complain about optional configuration files
		ASSERT(nTBCallStatus == TB_SUCCESS);
		if (nTBCallStatus != TB_SUCCESS)
		{
			return false;
		}
		*/
		// do a local options initialize now
		return COInitialize(nTerminalNumber, true);
	}
	else
	{
		int nRetValue = -1;
		nRetValue = co.Initialize(nTerminalNumber);
		CustomerMessage = co.csCMCustomerMessage;
		bool bRetStatus = (nRetValue >= 0);
		return bRetStatus;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// caller should provide valid SCOT terminal number
// Tender option is intialized here,
// bLocal = true   : tender files are taken from local disk
// bLocal = false  : tender files are taken from host application by calling CopyFileFromServer()
// bLocal defaults to false
// return true on successfull initialization
//////////////////////////////////////////////////////////////////////////////////////////////////
bool SMStateBase::SCOTTENDInitialize(int nTerminalNumber, bool bLocal)
{
	if (!bLocal)
	{
		// get files from server to local path
		CString csOptionFile, csExt, csFileName;
		CString csDestPath = co.csConfigFilePath+_T("\\");
		TBRC nTBCallStatus;

		//New Finalization
		if(co.fStateDualLanguage)
		{
            int i;
            CString csTenderFile[SCOT_LANGUAGE_MAX];
            CString csFileName[SCOT_LANGUAGE_MAX];

            for (i = 0; i < co.m_iNumberOfLanguagesSupported; i++)
            {
			    csTenderFile[i] = co.language[i].csTenderFile;
    		    csTenderFile[i] = csTenderFile[i].Left(csTenderFile[i].GetLength() - 4);
			    csExt = _T(".000");
			    csFileName[i] = csTenderFile[i] + csExt;
			    nTBCallStatus = CopyFileFromServer(csFileName[i],csDestPath+csFileName[i],false);	// destination path is null
			    // copy terminal specific option file from server
			    csExt.Format(_T(".%03d"), nTerminalNumber);
			    csFileName[i] = csTenderFile[i] + csExt;
			    nTBCallStatus = CopyFileFromServer(csFileName[i],csDestPath+csFileName[i],false);	// destination path is null

            }

		}
		else
		{
			csOptionFile = co.csDataTenderFile;
			csExt = _T(".000");
			csFileName = csOptionFile + csExt;

			nTBCallStatus = CopyFileFromServer(csFileName,csDestPath+csFileName,false);	// destination path is null

			// copy terminal specific option file from server
			csExt.Format(_T(".%03d"), nTerminalNumber);
			csFileName = csOptionFile + csExt;
			nTBCallStatus = CopyFileFromServer(csFileName,csDestPath+csFileName,false);	// destination path is null
		}
		// do a local options initialize now
		return SCOTTENDInitialize(nTerminalNumber, true);
	}
	else
	{
		int nRetValue = -1;
		nRetValue = co.TenderInitialize(nTerminalNumber);
		bool bRetStatus = (nRetValue >= 0);
		return bRetStatus;
	}

}



SMStateBase *SMStateBase::SetISIPTimeOut(int nMilliSec)
{
	if (nMilliSec > 0)
	{
		nISIPTimeOut = nMilliSec;
	}
	else
	{
		int nTmpISIPValue = nISIPTimeOut;
		nISIPTimeOut = 0;
		if (fScannerRequested && nTmpISIPValue)
		{
			SAWLDBScannerEnable();
		}
		pStateAfterISIPTimeOut = NULL; // not in use
	}
	return STATE_NULL;
}

int SMStateBase::GetISIPTimeOut()
{
	return nISIPTimeOut;
}

long SMStateBase::DisplayAuxScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh, int nArgs, ...)
{
  long rc = -1;
  trace(L6, _T("+DisplayAuxScreen"));
  if (gpSCOTAuxDisp)
  {
    va_list argptr;
    va_start( argptr, nArgs );

		// TAR 222179 CMal/FFR 120902 Removed following 'if' and added result code checking
		//if (g_lDisplayFormRC == 0) // Display is working, but always call, becuz 1 bad screen
		                           // doesn't mean that others won't work (e.g. Signature,...)
	//{
		rc = gpSCOTAuxDisp->VarShowScreenV(eScreen, bForceRefresh, nArgs, argptr);
		if (rc == -1)  // just means that we're already displaying that screen

			rc = 0;     // valid to be on same screen
		g_lDisplayFormRC = rc;	//TAR 219971, 220074 & 208118 BZ

		va_end(argptr);
	//}

    m_bNeedToDisplayAuxScreen = false;
  }

  trace(L6, _T("-DisplayAuxScreen rc:%d"), rc);
  return rc;
}

void SMStateBase::SaveReceiptData(void)
{
	long rc;
	LPTSTR lpFileName, lpReceiptType;
	BSTR bstrSigString;
	bool   bContinue = true;

    lpFileName = NULL;
    lpReceiptType = NULL;
    bstrSigString = NULL;

	while(bContinue)
	{
		rc = TBLogReceipt(&lpFileName, &bstrSigString, &lpReceiptType);
		if(rc == TB_SUCCESS && lpFileName != NULL)
		{
			rp.SaveReceiptData(lpFileName, bstrSigString, lpReceiptType);
		}
		else
		{
			 bContinue = false;
		}

		if(lpFileName)
		{
			delete [] lpFileName;
		}
		if(lpReceiptType)
		{
			delete [] lpReceiptType;
		}

		if(bstrSigString)
		{
			SysFreeString(bstrSigString);
		}
	}

}


//////////////////////////////////////////
void SMStateBase::SaveSigReceiptData(BSTR pSignatureData)
{
	if( SysStringByteLen( pSignatureData ) != 0 )
	{
		CSigReceiptDataFile m_SigDataFile;

		m_SigDataFile.OpenFile(TRUE) ;

		HFILE hFile = TBGetSlipData();
		if(hFile) //tar 253950
		{
			trace(L7, _T("+rp.Signatures"));
			rp.Signatures(hFile, pSignatureData);
			trace(L7, _T("-rp.Signatures<%d><%d>"), hFile, pSignatureData);
		}
		else
		{
			trace(L6, _T("SKIPPED rp.Signatures because hFile is NULL (No Slip Data)"));
		}

		TBReleaseSlipData(hFile);
		m_SigDataFile.CloseFile() ;
	}
	else
	{
		trace( L6, _T("pSignatureData is empty, cant' send this to Report") );
	}
	
}



//////////////////////////////////////////////////////////////////////////////////////////
// Verify the keyed in birth date is an age old enough for the current restricted item
//////////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::VerifyBirthDate()
{
  CString csEntryDate;
  CString csWork;

  DMSayPhrase(KEYPADVALID);

  int nEntryDateLength = ps.GetInput(csEntryDate);

  if (!nEntryDateLength)
	return STATE_NULL;

  COleDateTime currentDate = COleDateTime::GetCurrentTime();

  int nNoCenturyDigit=0;
  int nNoTotalDigit=0;

  if (co.nLocaleNoCenturyDigit==0) nNoCenturyDigit = 2;
  if (co.nLocaleNoCenturyDigit==1) nNoCenturyDigit = 4;

  nNoTotalDigit = nNoCenturyDigit + 4;

  // Not physically allowed to enter more, only less
  if (nEntryDateLength == nNoTotalDigit)
  {
    int birthMonth=0;
    int birthDay=0;
    int birthYear=0;

    switch (co.nLocaleShortDateOrdering)
    {
      case 0: // Month-Day-Year
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthMonth  = _ttoi(csEntryDate.Left(2));
          birthDay    = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthMonth  = _ttoi(csEntryDate.Left(2));
          birthDay    = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(4));
        }
        break;
      case 1: // Day-Month-Year
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthDay    = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthDay    = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(4));
        }
        break;
      case 2: // Year-Month-Day
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthYear   = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthDay    = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthYear   = _ttoi(csEntryDate.Left(4));
          birthMonth  = _ttoi(csEntryDate.Mid(4,2));
          birthDay    = _ttoi(csEntryDate.Right(2));
        }
        break;
      default:
        break;
    }

    // Until the year 2000, assume the date they entered is 19xx.
    // After that, if the 2 digit year they entered is greater
    // than or equal to the current 2 digit year, assume they
    // mean 19xx. If it's less, assume they mean 20xx.

    if (nNoCenturyDigit==2)
    {
      if ( currentDate.GetYear() < 2000 || birthYear > _ttoi(currentDate.Format(_T("%y"))) )
		birthYear += 1900;
      else
		birthYear += 2000;
    }

    COleDateTime birthDate(birthYear, birthMonth, birthDay, 0, 0, 0);

    // If the date isn't valid (e.g. 11/35/62 or 02/29/98 (not leap yr.)),
    // drop outta here.
    if (birthDate.GetStatus() == COleDateTime::valid)
    {
	  nCustomerAge = ps.ageFromDateTime(birthDate);
      CString msgText;

      csBirthDate = csEntryDate;  // mmddyy for restricted items needed in exit code MGG

	  //ra.SaveBirthdate(birthMonth, birthDay, birthYear);
	  CString csBirthDateEnter;
	  csBirthDateEnter.Format(ps.GetPSText(RA_BIRTHDATEENTER, SCOT_LANGUAGE_PRIMARY), ps.DateFormat(birthMonth, birthDay, birthYear), nCustomerAge);
	  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csBirthDateEnter);
	  ra.OnNormalItem(_T("[Summary]") + csBirthDateEnter);
	  trace(L7, _T("-ra.OnNormalItem"));
	  //+ TAR209317 LPM071002
	  static long lSavedlUnapprovedRestrictedItems = lUnapprovedRestrictedItems;	// Need to save flag value
	  // need to decrement these flags when the birthdate is verified LPM071002
	  //chu&th TAR209660 & TAR209661 +:
	  // if ( lUnapprovedRestrictedItems )
	  // 		lUnapprovedRestrictedItems--;
	  // if ( lRestrictedItems )
	  // 	lRestrictedItems--;
	  // if ( lRegularYellowNeedingApproval )
	  // 	lRegularYellowNeedingApproval--;
	  //CHu&TH TAR209660 & TAR209661 -
	  //- TAR209317 LPM071002


	  //ra.OnAgeRestrictedItem(csBirthDateEnter, io.d.nRestrictedAge, FALSE);
      // Check for under age limits with respect to the types of items
      // purchased that require approval.  Set error msg text as appropriate.
      if ( long ltmpUnapprRestItems = ps.RestrictedItemNotAllowed(nCustomerAge,  lSavedlUnapprovedRestrictedItems) ) //TAR249400
      {
        // both item types need approval - under age
        msgText = ps.GetPSText(MSG_UNDER_RESTRICTED_AGE_LIMIT);

	    //TAR 174877
	    tb.EnterDOB(birthYear, birthMonth, birthDay,nCustomerAge); // inform TB about customer age with date info
	    CString strFmt ;
	    strFmt =_T("tb.EnterDOB--birthYear:%d; birthMonth:%d; birthDay:%d; nCustomerAge:%d") ;
	    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, birthYear, birthMonth, birthDay,nCustomerAge) ;
		//TAR249400+
		//If entered DOB has approved some items, we need to decrement the flags
		if ( ltmpUnapprRestItems < lUnapprovedRestrictedItems )
		{
			lSavedlUnapprovedRestrictedItems = lUnapprovedRestrictedItems;
			lUnapprovedRestrictedItems = ltmpUnapprRestItems;
		 	lRestrictedItems = lRestrictedItems - lSavedlUnapprovedRestrictedItems + ltmpUnapprRestItems;
		 	lRegularYellowNeedingApproval = lRegularYellowNeedingApproval - lSavedlUnapprovedRestrictedItems + ltmpUnapprRestItems;
			m_lAgerestrictQueuedtoRAP = m_lAgerestrictQueuedtoRAP - lSavedlUnapprovedRestrictedItems + ltmpUnapprRestItems;
		}
		//TAR249400-

      }
      else
      {
		// All is well.  One or both types of restricted items
		// are now authorized.  We should never come here again
		// during this transaction.  Even if we have just authorized
		// only one type of restricted item, we have obtained a valid
		// age and can set approved flags for any future use.

		tb.EnterDOB(birthYear, birthMonth, birthDay,nCustomerAge); // inform TB about customer age with date info
	    CString strFmt ;
	    strFmt =_T("tb.EnterDOB--birthYear:%d; birthMonth:%d; birthDay:%d; nCustomerAge:%d") ;
	    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, birthYear, birthMonth, birthDay,nCustomerAge) ;

		// TAR 167991 - DMSayPhrase(KEYPADVALID);
		fAgeObtained = true;
		fApprovedForRestrictedItems = true;
		lUnapprovedRestrictedItems = 0;
		lRegularYellowNeedingApproval = 0;

        // remote approval
        CString csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID);
		//TAR 196910 start
		if (m_lAgerestrictQueuedtoRAP)
		{
		trace(L7, _T("+ra.OnAgeApproval"));
        ra.OnAgeApproval();
		trace(L7, _T("-ra.OnAgeApproval"));
		m_lAgerestrictQueuedtoRAP--;
		}
		//TAR 196910 end

	    RETURNSTATE(SmAuthorization)
      }

      // All is not well - the customer does not meet one or both of the age limits.
      DMSayPhrase(SECURITYMED);	// Issue error audio msg.

      // Format the number entered with slashes to look like a date.
      csWork.Format(_T("%s: %s"), ps.DateFormat(birthMonth, birthDay, birthYear), msgText);

      //+TAR391405
      //return createLevel1Message(csWork,     // char *message
      //                           PS_NOTEXT,  // no canned message
      //                           PS_NOTEXT,  // no prompt
      //                           0,          // no prompt value
      //                           -1,         // no device
      //                           false);	 // no DM check health needed
      ps.SetLeadthruText(csWork, _T(""));
      return STATE_NULL;
      //-TAR391405

    }
    else
    {
      // If we get to here, the date entered is invalid, but the right length
      DMSayPhrase(SECURITYMED);

      // format with slashes to look like a date
      switch (co.nLocaleShortDateOrdering)
      {
          case 0: // Month-Day-Year
          case 1: // Day-Month-Year
              csWork.Format(_T("%s: %s%s%s%s%s"),
		                    ps.GetPSText(MSG_DATEINVALID),
                            csEntryDate.Left(2),
                            co.csLocaleDateSeparator,

		                    csEntryDate.Mid(2,2),
		                    co.csLocaleDateSeparator,
		                    csEntryDate.Right(nNoCenturyDigit));
              break;
          case 2: // Year-Month-Day
              csWork.Format(_T("%s: %s%s%s%s%s"),
		                    ps.GetPSText(MSG_DATEINVALID),
                            csEntryDate.Left(nNoCenturyDigit),
                            co.csLocaleDateSeparator,
		                    csEntryDate.Mid(2,2),
		                    co.csLocaleDateSeparator,
		                    csEntryDate.Right(2));
              break;
          default:
              break;
      }
      
      //+TAR391405 
      //return createLevel1Message(csWork,     // char *message
      //                           PS_NOTEXT,  // no canned message
      //                           PS_NOTEXT,  // no prompt
      //                           0,          // no prompt value
      //                           -1,         // no device
      //                           false);     // no DM check health needed
      ps.SetLeadthruText(csWork, _T(""));	
      return STATE_NULL;
      //-TAR391405
    }
  }

  // if we get here the date entered was an incorrect length
  DMSayPhrase(SECURITYMED);
  csWork.Format(ps.GetPSText(MSG_DATE_FORMAT_INVALID), ps.DateFormat(), csEntryDate);
  
  //+TAR391405
  //return createLevel1Message(csWork,     // char *message
  //                           PS_NOTEXT,  // no canned message
  //                           PS_NOTEXT,  // no prompt
  //                           0,          // no prompt value
  //                           -1,         // no device
  //                           false);     // no DM check health needed
  ps.SetLeadthruText(csWork, _T(""));
  return STATE_NULL;
  //-TAR391405

}

//TAR 192098 to handle dob entered from RAP start
BOOL SMStateBase::VerifyBirthDate(CString csFromAnyDOB,CString csoperatorID,CString cspassword)
{
  CString csEntryDate = csFromAnyDOB;
  CString csWork;

//  DMSayPhrase(KEYPADVALID);

  int nEntryDateLength = csEntryDate.GetLength();

  if (!nEntryDateLength)
	return FALSE;

  COleDateTime currentDate = COleDateTime::GetCurrentTime();

  int nNoCenturyDigit=0;
  int nNoTotalDigit=0;

  if (co.nLocaleNoCenturyDigit==0) nNoCenturyDigit = 2;
  if (co.nLocaleNoCenturyDigit==1) nNoCenturyDigit = 4;

  nNoTotalDigit = nNoCenturyDigit + 4;

  // Not physically allowed to enter more, only less
  if (nEntryDateLength == nNoTotalDigit)
  {
    int birthMonth=0;
    int birthDay=0;
    int birthYear=0;

    switch (co.nLocaleShortDateOrdering)
    {
      case 0: // Month-Day-Year
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthMonth  = _ttoi(csEntryDate.Left(2));
          birthDay    = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthMonth  = _ttoi(csEntryDate.Left(2));
          birthDay    = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(4));
        }
        break;
      case 1: // Day-Month-Year
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthDay    = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthDay    = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(4));
        }
        break;
      case 2: // Year-Month-Day
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthYear   = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthDay    = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthYear   = _ttoi(csEntryDate.Left(4));
          birthMonth  = _ttoi(csEntryDate.Mid(4,2));
          birthDay    = _ttoi(csEntryDate.Right(2));
        }
        break;
      default:
        break;
    }

    // Until the year 2000, assume the date they entered is 19xx.
    // After that, if the 2 digit year they entered is greater
    // than or equal to the current 2 digit year, assume they
    // mean 19xx. If it's less, assume they mean 20xx.

    if (nNoCenturyDigit==2)
    {
      if ( currentDate.GetYear() < 2000 || birthYear > _ttoi(currentDate.Format(_T("%y"))) )
		birthYear += 1900;
      else
		birthYear += 2000;
    }

    COleDateTime birthDate(birthYear, birthMonth, birthDay, 0, 0, 0);

    // If the date isn't valid (e.g. 11/35/62 or 02/29/98 (not leap yr.)),
    // drop outta here.
    if (birthDate.GetStatus() == COleDateTime::valid)
    {
	  nCustomerAge = ps.ageFromDateTime(birthDate);
      CString msgText;

      csBirthDate = csEntryDate;  // mmddyy for restricted items needed in exit code MGG

	  //ra.SaveBirthdate(birthMonth, birthDay, birthYear);
	  CString csBirthDateEnter;
	  csBirthDateEnter.Format(ps.GetPSText(RA_BIRTHDATEENTER, SCOT_LANGUAGE_PRIMARY), ps.DateFormat(birthMonth, birthDay, birthYear), nCustomerAge);
	  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csBirthDateEnter);
	  ra.OnNormalItem(_T("[Summary]") + csBirthDateEnter);
	  trace(L7, _T("-ra.OnNormalItem"));

	  //TAR 190558 start
	  //this is done for assistance report for Age restricted items from RAP
	  static long lSavedlUnapprovedRestrictedItems = 0;
	  long lRAPAgeApproval = 0;
	  lSavedlUnapprovedRestrictedItems = lUnapprovedRestrictedItems;
	  //TAR 190558 end

	  //+ TAR209317 LPM071002
	  //chu&th TAR209660 & TAR209661 +:
	  //If the run birth date is inputed:01012002, the item is approved with the following lines
	  //if ( lUnapprovedRestrictedItems )
	  //	lUnapprovedRestrictedItems--;
	  //if ( lRestrictedItems )
	  //	lRestrictedItems--;
	  //if ( lRegularYellowNeedingApproval )
	  //	lRegularYellowNeedingApproval--;
      //chu&th TAR209660 & TAR209661-

	  //ra.OnAgeRestrictedItem(csBirthDateEnter, io.d.nRestrictedAge, FALSE);
      // Check for under age limits with respect to the types of items
      // purchased that require approval.  Set error msg text as appropriate.
	  // use a tempory flag, don't want to muck with global.
	  long ltmpUnapprRestItems = ps.RestrictedItemNotAllowed(nCustomerAge,  lSavedlUnapprovedRestrictedItems);
	  if (ltmpUnapprRestItems)	//- TAR209317 LPM071002
		{
        // both item types need approval - under age
        msgText = ps.GetPSText(MSG_UNDER_RESTRICTED_AGE_LIMIT);
	    //TAR 174877
	    tb.EnterDOB(birthYear, birthMonth, birthDay,nCustomerAge); // inform TB about customer age with date info
	    CString strFmt ;
	    strFmt =_T("tb.EnterDOB--birthYear:%d; birthMonth:%d; birthDay:%d; nCustomerAge:%d") ;
	    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, birthYear, birthMonth, birthDay,nCustomerAge) ;

	  }
      else
      {
		// All is well.  One or both types of restricted items
		// are now authorized.  We should never come here again
		// during this transaction.  Even if we have just authorized
		// only one type of restricted item, we have obtained a valid
		// age and can set approved flags for any future use.



		tb.EnterDOB(birthYear, birthMonth, birthDay,nCustomerAge); // inform TB about customer age with date info
	    CString strFmt ;
	    strFmt =_T("tb.EnterDOB--birthYear:%d; birthMonth:%d; birthDay:%d; nCustomerAge:%d") ;
	    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, birthYear, birthMonth, birthDay,nCustomerAge) ;

		// TAR 167991 - DMSayPhrase(KEYPADVALID);
		fAgeObtained = true;
		fApprovedForRestrictedItems = true;
		lUnapprovedRestrictedItems = 0;
		lRegularYellowNeedingApproval = 0;

        // remote approval
        CString csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID);

		if (m_lAgerestrictQueuedtoRAP)
		{
			m_lAgerestrictQueuedtoRAP--;

		  /* Tar 362122 - no need to approve again for more than two restricted items
		  if (m_lAgerestrictQueuedtoRAP != 0)
		  {
			trace(L7, _T("+ra.OnAgeApproval"));
			ra.OnAgeApproval();
			trace(L7, _T("-ra.OnAgeApproval"));
		  }
			*/
		}
		if (isAnyApprovalRequired())
		{
			dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
			DM_TRICOLORLIGHT_ON,
			DM_TRICOLORLIGHT_NORMALREQUEST);
		}
		else if(fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
		{
			// If inside the transaction, turn on the green light
			//	if(fInSignedOn || fInSignedOff || fStateAssist || fSAInTransaction)
			dm.setTriColorLight(DM_TRICOLORLIGHT_GREEN,
		    DM_TRICOLORLIGHT_ON,
			DM_TRICOLORLIGHT_NORMALREQUEST);
		}
	    return TRUE;
      }
	  return FALSE;
	}
	return FALSE;
	}

return FALSE;
}
//TAR 192098 to handle dob entered from RAP end

//TAR 196908 start
long SMStateBase::GetAge(CString csFromAnyDOB)
{
  CString csEntryDate = csFromAnyDOB;
  CString csWork;

  int nEntryDateLength = csEntryDate.GetLength();

  if (!nEntryDateLength)
	return (0);

  COleDateTime currentDate = COleDateTime::GetCurrentTime();

  int nNoCenturyDigit=0;
  int nNoTotalDigit=0;

  if (co.nLocaleNoCenturyDigit==0) nNoCenturyDigit = 2;
  if (co.nLocaleNoCenturyDigit==1) nNoCenturyDigit = 4;

  nNoTotalDigit = nNoCenturyDigit + 4;

  // Not physically allowed to enter more, only less
  if (nEntryDateLength == nNoTotalDigit)
  {

    int birthMonth=0;
    int birthDay=0;
    int birthYear=0;

    switch (co.nLocaleShortDateOrdering)
    {
      case 0: // Month-Day-Year
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthMonth  = _ttoi(csEntryDate.Left(2));
          birthDay    = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthMonth  = _ttoi(csEntryDate.Left(2));
          birthDay    = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(4));
        }
        break;
      case 1: // Day-Month-Year
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthDay    = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthDay    = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(4));
        }
        break;
      case 2: // Year-Month-Day
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthYear   = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthDay    = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthYear   = _ttoi(csEntryDate.Left(4));
          birthMonth  = _ttoi(csEntryDate.Mid(4,2));
          birthDay    = _ttoi(csEntryDate.Right(2));
        }
        break;
      default:
        break;
    }

    // Until the year 2000, assume the date they entered is 19xx.
    // After that, if the 2 digit year they entered is greater
    // than or equal to the current 2 digit year, assume they
    // mean 19xx. If it's less, assume they mean 20xx.

    if (nNoCenturyDigit==2)
    {
      if ( currentDate.GetYear() < 2000 || birthYear > _ttoi(currentDate.Format(_T("%y"))) )
		birthYear += 1900;
      else
		birthYear += 2000;
    }

    COleDateTime birthDate(birthYear, birthMonth, birthDay, 0, 0, 0);

    // If the date isn't valid (e.g. 11/35/62 or 02/29/98 (not leap yr.)),
    // drop outta here.
    if (birthDate.GetStatus() == COleDateTime::valid)
    {
	  nCustomerAge = ps.ageFromDateTime(birthDate);
	  return nCustomerAge;
	}

	return (0);
  }
  return (0);
 }
//TAR 196908 end

bool SMStateBase::WriteSCOTStatusToFile(CString cStatus)
{
	// Initialize SCOT status file name.
	CString csFile;
	CString csName=_T("CurState.dat");
	csFile = _T("%DATA_DRIVE%\\scot\\logs\\");
	csFile += csName;
	GET_PARTITION_PATH(csFile, csFile.GetBuffer(_MAX_PATH));
	csFile.ReleaseBuffer();

	SCOTDeleteFile(csFile);   //TAR 348946

	bool bRetCode = true;
	bRetCode = OpenSCOTStatusFile(csFile);
	if (bRetCode)
	{
     try
     {
		filePrint.WriteString(cStatus);
     }
     catch (CFileException *fe)
     {
		bRetCode = false;
		_TCHAR szError[255] = {NULL};
		fe->GetErrorMessage( szError, 255 );
		ScotError(RETURN,SM_CAT_FILE_IO, SCOTAPP_FILEOPEN, _T("%s|%s"), csFile, szError);
		fe->Delete();
     }
	}
	filePrint.Close();
	return bRetCode;
}

bool SMStateBase::OpenSCOTStatusFile(CString cFileName)
{
	bool bRetCode = true;
    CFileException fe;

    if (filePrint.m_hFile == CFile::hFileNull)
    {
       // Open does not throw exceptions
       filePrint.Open(cFileName, CFile::modeCreate | CFile::modeNoTruncate |
                      CFile::shareDenyNone | CFile::modeReadWrite, &fe);
       if (fe.m_cause  != CFileException::none)
       {
		   bRetCode = false;
		   trace(L6, _T("Error openning %s file, reason=%d"), cFileName, fe.m_cause);
		   ScotError(INFO, SM_CAT_FILE_IO, SCOTAPP_FILEOPEN,
		     _T("Error openning %s file"), cFileName);
		   fe.Delete();
       }
    }
    return bRetCode;
}

////////////////////////////////////////////////////////////////////////////
// awk.7.01 Added for TAR 173990
// TAR 215165 Check files for dual language, also check PRICESOUNDCONFIGXXXX.DAT
SMStateBase *SMStateBase::CheckForDATFile(bool bReloadOption)
{
  CFileFind DATFileSearch;
  CString ErrorMsg = _T(" is missing. Please copy this file to Config Folder.");
  CString bmpefile = co.csConfigFilePath + _T("\\") + co.csDataBitmapFile + _T(".DAT");		//SCOTBMPE.DAT
  CString wavefile = co.csConfigFilePath + _T("\\") + co.csDataWaveFile + _T(".DAT");		//SCOTWAVE.DAT
//  CString mascotfile = co.csConfigFilePath + _T("\\SCOTMASCOT.DAT");
  CString scotoptsfile = co.csConfigFilePath + _T("\\SCOTOPTS.DAT");

  CString msgefile[SCOT_LANGUAGE_MAX];
  CString tenderfile[SCOT_LANGUAGE_MAX];
  CString tarefile[SCOT_LANGUAGE_MAX];
  CString pricesoundfile[SCOT_LANGUAGE_MAX];

  int i;

  if(co.fStateDualLanguage)
  {
      for (i = 0; i < co.m_iNumberOfLanguagesSupported; i++)
      {
	     msgefile[i] = co.csConfigFilePath + _T("\\") + co.language[i].csMessageFile;		//Scot message file
         tenderfile[i] = co.csConfigFilePath + _T("\\") + co.language[i].csTenderFile;	//Scot tender file
         tarefile[i] = co.csConfigFilePath + _T("\\") + co.language[i].csTareFile;		//Scot tare file
         pricesoundfile[i] = co.csConfigFilePath + _T("\\") + co.language[i].csPriceSoundConfigFile;		//Scot pricesound file
      }
  }
  else
  {
	  msgefile[0] = co.csConfigFilePath + _T("\\") + co.csDataMessageFile + _T(".DAT");		//SCOTMSGE.DAT
	  tenderfile[0] = co.csConfigFilePath + _T("\\") + co.csDataTenderFile + _T(".DAT");	//SCOTTEND.DAT
	  tarefile[0] = co.csConfigFilePath + _T("\\") + co.csDataTareFile + _T(".DAT");		//SCOTTARE.DAT
	  pricesoundfile[0].Format(_T("%s\\%s%s%s"), co.csConfigFilePath, _T("PriceSoundConfig"), dm.m_csSoundDefaultLanguageCode, _T(".DAT"));		//PRICESOUNDCONFIGXXXX.DAT
  }

  for (i = 0; i < co.m_iNumberOfLanguagesSupported; i++)
  {
      if(!DATFileSearch.FindFile(msgefile[i]))
      {
		    // file doesn't exist

		    ScotError(FATAL,SM_CAT_CONFIGURATION, SCOTAPP_MISS_FILE, _T("%s"), msgefile[i]);
		    if ( bReloadOption != true )
			    AfxMessageBox(msgefile[i] + ErrorMsg, MB_SYSTEMMODAL);
		    return STATE_ENDED;
      }
      if(!DATFileSearch.FindFile(tarefile[i]))
      {
		    // file doesn't exist

		    ScotError(FATAL,SM_CAT_CONFIGURATION, SCOTAPP_MISS_FILE, _T("%s"), tarefile[i]);
		    if ( bReloadOption != true )
			    AfxMessageBox(tarefile[i] + ErrorMsg, MB_SYSTEMMODAL);
		    return STATE_ENDED;
      }
      if(!DATFileSearch.FindFile(tenderfile[i]))
      {
		    // file doesn't exist

		    ScotError(FATAL,SM_CAT_CONFIGURATION, SCOTAPP_MISS_FILE, _T("%s"), tenderfile[i]);
		    if ( bReloadOption != true )
			    AfxMessageBox(tenderfile[i] + ErrorMsg, MB_SYSTEMMODAL);
		    return STATE_ENDED;
      }
/*      if(!DATFileSearch.FindFile(pricesoundfile[i]))
      {
		    // file doesn't exist

		    ScotError(FATAL,SM_CAT_CONFIGURATION, SCOTAPP_MISS_FILE, _T("%s"), pricesoundfile[i]);
		    if ( bReloadOption != true )
			    AfxMessageBox(pricesoundfile[i] + ErrorMsg, MB_SYSTEMMODAL);
		    return STATE_ENDED;
      }*/
  }
  if(!DATFileSearch.FindFile(bmpefile))
  {
	    // file doesn't exist

		ScotError(FATAL,SM_CAT_CONFIGURATION, SCOTAPP_MISS_FILE, _T("%s"), bmpefile);
		if ( bReloadOption != true )
			AfxMessageBox(bmpefile + ErrorMsg, MB_SYSTEMMODAL);
		return STATE_ENDED;
  }
/*  else if(!DATFileSearch.FindFile(mascotfile))
  {
	    // file doesn't exist

		ScotError(FATAL,SM_CAT_CONFIGURATION, SCOTAPP_MISS_FILE, _T("%s"), mascotfile);
		if ( bReloadOption != true )
			AfxMessageBox(mascotfile + ErrorMsg, MB_SYSTEMMODAL);
		return STATE_ENDED;
  } */ //No Longer Used
  else if(!DATFileSearch.FindFile(scotoptsfile))
  {
	    // file doesn't exist

		ScotError(FATAL,SM_CAT_CONFIGURATION, SCOTAPP_MISS_FILE, _T("%s"), scotoptsfile);
		if ( bReloadOption != true )
			AfxMessageBox(_T("Scotopts.dat is missing.  Please copy this file to Config Folder."), MB_SYSTEMMODAL);
		return STATE_ENDED;
  }
  else if(!DATFileSearch.FindFile(wavefile))
  {
	    // file doesn't exist

		ScotError(FATAL,SM_CAT_CONFIGURATION, SCOTAPP_MISS_FILE, _T("%s"), wavefile);
		if ( bReloadOption != true )
			AfxMessageBox(wavefile + ErrorMsg, MB_SYSTEMMODAL);
		return STATE_ENDED;
  }
  DATFileSearch.Close();

  return 0;

} //End TAR 173990 awk.7.01
//////////////////////////////////////////////////

SMStateBase *SMStateBase::SAEndOfTransaction()
{
	if (isAnySoldVoidedItems() || fItemIsWaiting)
	{
		// TAR: 127556, false 'False transaction start' event, ignore it
		ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_GOT_SAFALSETRXSTART, _T("%d|%d"), lTotalSale, fItemIsWaiting);
		SASendSCOTInput(ITEMIZATION);
		fSAInCustMode = false;
		SASendSCOTInput(CUSTOMER_MODE);
		return STATE_NULL;
	}
	//Begin TAR#118659	coded to use the SA 16100 event
	// False Transaction Start detected by security, start Attract clean
	ResetAllTransactionValues();	//better way to clear

    //*** NewUI ***//
	//SMSecList.Reset();

	// remote approval:  sending info to rap with no security items at all causes
	// erroneous state handle message in log and assert failures from RAP
	SCOTDO_INFO securityInfo;
	ra.GetSecurityInfo(&securityInfo);
	if(securityInfo.bApprovalRequired)
	{
	  trace(L7, _T("+ra.OnSecurityApproval"));
	  ra.OnSecurityApproval();
	  trace(L7, _T("-ra.OnSecurityApproval"));
	}
	trace(L7, _T("+ra.OnTransactionEnd"));
	ra.OnTransactionEnd();
	trace(L7, _T("-ra.OnTransactionEnd"));

	ClearStateStackEOT();

	// turn off lane light errors
	dm.turnOffTriColorLight();
	return createAnchorState();
}

///////////////////////////////////////////////////////////////////
//for Print Last Receipt module
void SMStateBase::SaveLastReceiptData(HFILE hFile, CString csFileName)
{
	try
	{
	   if (hFile!=NULL)
	   {
		  DWORD	len;
		  unsigned char* pBuff;

#if _MFC_VER < 0x0700
          CFile	cFile( hFile );
#else
          CFile	cFile( (HANDLE)hFile );
#endif

		  len = (DWORD)cFile.SeekToEnd();
		  cFile.SeekToBegin();


		  if (len)
		  {
			 pBuff = new unsigned char [len+1];
			 if (pBuff)
			 {
				memset(pBuff, 0, len+1);
				UINT c = cFile.Read(pBuff, len);
				if (c)
				{
				   pBuff[len]  = 0;
				   //bool bIsSlipData = false;
				   //if (csFileName == SLIP_FILE)
				   //{
				//		csFileName = LRECEIPT_TMP_FILE;
				//		bIsSlipData = true;
				  // }
                   // +TAR 376809
				   CString csReceiptData;
				   bool bIsUnicode = IsReceiptUnicodeData(pBuff, len);
				   if (   bIsUnicode
					   && ((pBuff[0] == 0xFF) && (pBuff[1] == 0xFE)) )
				   {   // Unicode file; strip leading 0xFFFE type indicator
				       CString csWholePrintData((wchar_t*) pBuff, len/2);
				       csWholePrintData = csWholePrintData.Mid(1);
					   len = len - 2;
				       csReceiptData = csWholePrintData;
				   }
				   else if (    bIsUnicode 
					         || (( len > strlen((const char*)pBuff)) && (4 > strlen((const char*)pBuff))) )
				   {
				       // Unicode file without type indicator
				       CString csWholePrintData((wchar_t*) pBuff, len/2);
				       csReceiptData = csWholePrintData;
				   }
				   else
				   {   // ANSI file; no conversion needed
				       CString csWholePrintData((char*) pBuff, len);
				       csReceiptData = csWholePrintData;
				   }
                   // -TAR 376809

				   CFile cFileSave(csFileName, CFile::modeCreate| CFile ::modeNoTruncate | CFile::modeWrite);//modeNoTruncate
				   int nTmp = (int)cFileSave.SeekToEnd();
			       cFileSave.Write(csReceiptData, csReceiptData.GetLength() * sizeof(_TCHAR)); // TAR 376809
				   //if (bIsSlipData)
				   //{
					//	CString csCutSequence = "\x1B|50fP";
					//	cFileSave.Write(csCutSequence , csCutSequence.GetLength());
				   //}

				   cFileSave.Close();
				}
				delete [] pBuff;
			 }
		  }
	   }
	}
	catch(CFileException *e)
	{
		// may be an invalid file handle
		_TCHAR szError[255] = {NULL};
		e->GetErrorMessage( szError, 255 );
		ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_BS_RECEIPT,_T("%d|%s"),hFile, szError);
		e->Delete();
	}

}

//for Print Last Receipt module
void SMStateBase::SaveLastReceiptData(CString csFileName, CString csData)
{
	try
	{
		CFile cFileSave(csFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);//modeNoTruncate
	    int nTmp = (int)cFileSave.SeekToEnd();
		CString csConvertedData = GetWideCharToMultiByte(csData);
		cFileSave.Write(csConvertedData, csConvertedData.GetLength() * sizeof(_TCHAR));//TAR 392046 - double the size of data to handle UNICODE if the receipt file is set to ANSI
	    cFileSave.Close();
	}
	catch(CFileException *e)
	{
	   _TCHAR szError[255] = {NULL};
	   e->GetErrorMessage( szError, 255 );
	   ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_BS_RECEIPT,_T("%s"), szError);
	   e->Delete();
	}
}

//for Print Last Receipt module
bool SMStateBase::FileExist(CString csFileName)
{
   CFile cFile;
   if(!cFile.Open(csFileName, CFile::modeRead))
   {
	   return false;
   }
   else
   {
	   cFile.Close();
	   return true;
   }
}

//for Print Last Receipt module
void SMStateBase::RemoveFile(CString csFileName)
{
   if (FileExist(csFileName))
	   CFile::Remove(csFileName);
}

//for Print Last Receipt module
void SMStateBase::PrintLastReceipt(CString csReceiptFileName)
{
	   //print receipt
	   DWORD nReceiptLen = 0;
	   HFILE hFile = NULL;
	   CFile cLastReceiptFile;

	   if(!cLastReceiptFile.Open(csReceiptFileName, CFile::modeRead))
	   {
		   return;
	   }

	   hFile = (HFILE)cLastReceiptFile.m_hFile;
	   if (hFile != NULL)
	   {
		   //+RFC 334227
		   CString csDuplicateTxt;
		   if ( co.isPrintDuplicateMsgonPrintLastReceipt() )
		   {
				DWORD nLen = 0;
				nLen = cLastReceiptFile.GetLength();
				if ( nLen > 0)
				{
					csDuplicateTxt = ps.GetPSText( TXT_DUPLICATE_RECEIPT ) ;
					DMReceiptLine( csDuplicateTxt );
					m_fPrintDuplicateTrailer = true;
				}
		   }
		   //-RFC 334227
		   nReceiptLen = PrintReceipt(hFile, 0);  //TAR 392046 - set bSaveReceipt to 0 to not save the receipt when printing last receipt
		   if (nReceiptLen)
		   {
			   CutReceipt();  //cut receipt
		   }
	   }
	   cLastReceiptFile.Close();
}



///////////////////////////////////////////////////////////////////////////
//  When re-loading options only reload the SASI Actions file
///////////////////////////////////////////////////////////////////////////
long SMStateBase::ReloadSASIActions(void)
{
   return 0;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnMatchedWt()
{
	// +218558 EAS motion sensor received in the wrong order must behave differently LPM103002
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	// +203255 Clear the RAP Intervention created for UnMatched/increase/decrease weight
	if(m_View > DATANEEDED_NOVIEW) //tar 252987
	{
		ClearSecurityViolationLight();

		trace(L7, _T("+ra.OnNoRAPDataNeeded for SMStateBase::OnMatchedWt"));
		ra.OnNoRAPDataNeeded();
		trace(L7, _T("-ra.OnNoRAPDataNeeded"));
	}// -203255 End

	g_bWtExpected = false;	//We should never expect wt if we got a matched wt.
	g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries

    if (dm.fStateTakeawayBelt && !co.IsSecurityBypassOn())
        return setAndCreateAnchorState(BES_TRANSPORTITEM);

	//219883 should not use g_bMatchedWtWaitingOnEAS for this LPM121002
	/*if (!m_bEASHappened)	// matched wt and the EAS event did not happen yet
	{
		trace(L2, _T("SMStateBase::OnMatchedWt, waiting for EAS motion sensor, returning to BagAndEAS (Bag Item)"));
		return setAndCreateAnchorState(BES_BAGANDEAS);
	}*/

	trace(L2, _T("bEASReEnableScanner is set to true."));
	bEASReEnableScanner = true;
	//-218558 End
	trace(L2, _T("SMStateBase::OnMatchedWt, returning to ScanAndBag"));
    return setAndCreateAnchorState(BES_SCANANDBAG);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnWtIncreaseNotAllowed()
{
	trace(L2, _T("SMStateBase::OnWtIncreaseNotAllowed, PrevState = %d, NextState = %d"),getAnchorState(), getAnchorState());

	// text here is just the name of the section in scotdataentry.ini.  Don't need to be in SCOTMSGE.DAT
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	CString csCurrentStateName;
	try
	{
		csCurrentStateName = this->GetRuntimeClass()->m_lpszClassName;
	}
	catch(...){};
	if(csCurrentStateName.Find(_T("SystemMessage")) != -1)
	{
        if ((getAnchorState() == BES_FINISH) && (SMFinishBase::issuedCashAcceptorError))
        {
            // weight increase causes to leave current system message which is waiting to be addressed
            // so, make sure operator login is required to see any current cash acceptor error once weight is fixed
            // TAR: 196868
            SMFinishBase::issuedCashAcceptorError = false;
        }
    }
	CREATE_AND_DISPENSE(SecUnExpectedIncrease)(getAnchorState(), getAnchorState());
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnWtDecrease()
{

    //447517+
	//racing condition: WtDecrease is happened before FL goes into store mode but the event is parsed 
	//after FL goes to store mode:
	if (storeMode())
	{
		return STATE_NULL;
	}
	//447517

	// normal case where removing is not allowed.
	DMSaySecurity( SEC_NONE, _T("blank.wav") );
	trace(L2, _T("SMStateBase::OnWtDecrease, PrevState = %d, NextState = %d"), getAnchorState(), getAnchorState());
	//setAnchorState(BES_SECUNEXPECTEDINCREASE); // any incorrect increase should goto unexpected increase screen
	CREATE_AND_DISPENSE(SecUnexpectedDecrease)(getAnchorState(), getAnchorState());
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnBackToLGW(void)
{
	if (bIsInFinalization)	//+TAR228892
	{
		// TAR228892/199449: We want to stay at the finalization state we're in now instead
		// of going to AnchorState. If we go to AnchorState, it is going to return to
		// Finish and Pay main and they've already started payment usually which is BAD
		// so returning a STATE_NULL.  LPM022603 CMal
		trace(L2, _T("SMStateBase::OnBackToLGW. In finalization states, no need to process BackToLGW."));
		//If there is WaitForApproval state waiting, clear it
		if (g_pDelay_WaitForApprovalStateChange != NULL)
		{
			CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;

			delete g_pDelay_WaitForApprovalStateChange;
			g_pDelay_WaitForApprovalStateChange = NULL;
			trace(L6,_T("OnBackToLGW - Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
		}
		return STATE_NULL;

	}	//-TAR228892

	// + TAR 404971
	if(bQuietNeedSecApproval)  
	{
		bQuietNeedSecApproval = false;
	}
	// - TAR 404971	
	
	//TAR228754 if we're in any cancel item screens, stay at the same state
	if(bIsFromVoidApproval)
		return STATE_NULL;
	//TAR228754

	trace(L2, _T("SMStateBase::OnBackToLGW, returning to anchor state"));

	// TAB re-enable scanner and clear Wait For Approval State
	TABReEnableScannerAndClearWaitForApprovalState();

	//219883 should not use g_bMatchedWtWaitingOnEAS like this LPM121002

	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
    SAClearSecurityCondition();

	if ( nSellState == BES_START && getAnchorState() != BES_DEGRADEDCONTINUETRANS )	  //degraded mode tars
		return setAndCreateAnchorState(BES_START);

	return createAnchorState();
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnSecurityScaleFailure(int nFailureType)
{
	//+RFC374303
	if ( co.fAutoRunOn && co.fAutoRunBypassBagging )
	{
                trace(L6, _T("AutoRun and AutoRunBypassBagging are true, returning STATE_NULL"));

		return STATE_NULL;
	}//-RFC374303
	CString csWork = ps.GetPSText(MSG_DEVICESERROR) + ps.GetPSText(TXT_BAG_SCALE) + _T(" ") + ps.GetPSText(MSG_BAGSCALE_ERR_FAULT);
	CString csWork2;

	//tar 255330 -- merged the fix in 3.0.2 here
	//Removed dm.settricolorlight because system message screen will take care of that


	switch (nFailureType)
	{
		case SS_COMM_FAILED :
			    csWork2 = _T("DeviceErrorSecurityScale.xml:SS_COMM_FAILED");  //DeviceError
				break;
		case SS_CLAIM_FAILED :
				csWork2 = _T("DeviceErrorSecurityScale.xml:SS_CLAIM_FAILED");
				break;
		case SS_OPEN_FAILED :				csWork2 = _T("DeviceErrorSecurityScale.xml:SS_OPEN_FAILED");
				break;
        case SS_ENABLE_FAILED :
				csWork2 = _T("DeviceErrorSecurityScale.xml:SS_NOT_ENABLED");
				break;
		case SS_FAILED_OVERWEIGHT :
				csWork2 = _T("DeviceErrorSecurityScale.xml:SS_FAILED_OVERWEIGHT");   //TAR352050
			    //csWork2 = csWork;
				break;
        default :
				csWork2 = csWork + _T(" ") + ps. GetPSText(TXT_UNKNOWN);
				break;
	}

	trace(L6,_T("ErrorMessage=%s, FailureType=%d"), csWork2, nFailureType);


		SMStateBase* sExit =
	          createLevel1Message(csWork2,     // char *message
                                 PS_NOTEXT,  // no canned message
                                 PS_NOTEXT,  // no prompt
                                 0,          // no prompt value
                                 PIDCLASS_BAGSCALE,
                                 false);     // no DM check health needed

	//Notify rap
	trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s, DeviceError=%d"), csWork, PIDCLASS_BAGSCALE);

	// save the previous view
	m_PrevView = m_View;

	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("DeviceFailure"));
	m_View = DATANEEDED_DEVICEERROR;
	CString csSecInfo;
	csWork = ps.GetPSText(MSG_SS_COMM_FAILED);
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csWork,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);

	trace(L7, _T("-ra.OnRAPDataNeeded"));
	return sExit;

}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnDBMgrFailure(long lStatusCode, long lStatusType)
{
	CString csWork = ps.GetPSText(MSG_DEVICESERROR) + ps.GetPSText(TXT_WLDB) + _T(" ") + ps.GetPSText(MSG_WLDB_ERR_FAULT);
    trace(L6,_T("ErrorMessage=%s, Statuscode=%d"),csWork,lStatusCode);

    dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
        DM_TRICOLORLIGHT_BLINK_1HZ,
        DM_TRICOLORLIGHT_NORMALREQUEST);


    SMStateBase* sExit =
	          createLevel1Message(csWork,     // char *message
                                 PS_NOTEXT,  // no canned message
                                 PS_NOTEXT,  // no prompt
                                 0,          // no prompt value
                                 PIDCLASS_WLDBSACONFIG,
                                 false);     // no DM check health needed

	//Notify rap
	trace(L7, _T("+ra.OnRAPDataNeeded, ErrorMessage=%s, DeviceError=%d"), csWork, PIDCLASS_BAGSCALE);

	// save the previous view
	m_PrevView = m_View;

	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("WLDBFailure"));
	m_View = DATANEEDED_DEVICEERROR;
	CString csSecInfo;
	csWork = ps.GetPSText(MSG_DBMC_FAILED);
	csSecInfo.Format(ps.GetPSText(RA_SECVIOLATIONINFO, SCOT_LANGUAGE_PRIMARY),csWork,ps.GetPSText(BUTTON_RA_DEVICE_ERROR, SCOT_LANGUAGE_PRIMARY), _T(""), _T(""), _T(""), _T(""));
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(), m_View);
	trace(L7, _T("-ra.OnRAPDataNeeded"));
	return sExit;
}
//TAR 407200
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnSkipBaggingAllowed(void)
{
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType)
{
	//why don't we just call the state after password in the current state? LPM
	trace(L2, _T("+SMStateBase::OnRAPClearSecurityBtn"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing

	g_lTABMisMatchApprWt = g_lCTW;	//TAR200247 Now have to get RAP appr wt off belt without any more mismatched wts LPM042502
    // NewSec
    OverrideSecurityException(lpszAuthorizationType, KEY_EXCEPTION_OPERATION_CLEAR);

	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
    SAClearSecurityCondition();

	trace(L2, _T("-SMStateBase::OnRAPClearSecurityBtn"));

    if(bIsInFinalization) //tar 286086
    {
        return STATE_RETURN;
    }
	return setAndCreateAnchorState(BES_SCANANDBAG);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnRAPApproveAndUpdateBtn(void)
{
	trace(L2, _T("+SMStateBase::OnRAPApproveAndUpdateBtn"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing

	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
    //SAClearSecurityCondition(); TAR 355855 moved this after calling LogWLDBWeightMismatch so that m_View doesn't get reset to -1

	g_lTABMisMatchApprWt = g_lCTW;	//TAR200247 Now have to get RAP appr wt off belt without any more mismatched wts LPM042502

	g_lRAPApprovedUpdate = true;
	g_bOkToUpdate = true;
	// TAR 196227 - reset this global after RAP has approved the # of new items

	if (DATANEEDED_MISMATCHWEIGHT == m_View)
	{
	    LogWLDBWeightMismatch(g_csActualWeight, g_csExpectedWeight, g_lTolerance);
	}

	SAClearSecurityCondition();//TAR 355855 - moved this here from above so that m_View is not reset to -1 before checking the weight mismatch condition

	//Don't need this line because already set to false in UpdateWLDBandITEMexc()
	//g_lRAPApprovedUpdate = false;	 //Tar 200873 BZ & AB +

    // NewSec
    OverrideSecurityException(_T("RAP"), _T("clear-with-update"));

	//TAR 214691 Reset wt expected and number of entries variables
	g_bWtExpected = false;	//We should never expect wt if button is pressed
	g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries

    BEST nextState;
    if (dm.fStateTakeawayBelt)
        nextState = BES_TRANSPORTITEM;
    else
        nextState = BES_SCANANDBAG;

	setAnchorState(nextState);//TAR329239   - change to setAnchorState from "return setAndCreateAnchorState"
	trace(L2, _T("-SMStateBase::OnRAPApproveAndUpdateBtn"));
	return STATE_NULL; //TAR329239
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnRAPApproveNoUpdateBtn(void)
{
	trace(L2, _T("+SMStateBase::OnRAPApproveNoUpdateBtn"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing


	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
    SAClearSecurityCondition();
	g_lTABMisMatchApprWt = g_lCTW;	//TAR200247 Now have to get RAP appr wt off belt without any more mismatched wts LPM042502

	//TAR 214691 Reset wt expected and number of entries variables
	g_bWtExpected = false;	//We should never expect wt if button is pressed
	g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries




	trace(L2, _T("-SMStateBase::OnRAPApproveNoUpdateBtn"));
	//tar 201544 - should return to BES_TRANSPORTITEM if take away belt is configured
	if (dm.fStateTakeawayBelt)
		return setAndCreateAnchorState(BES_TRANSPORTITEM);
	else
		return setAndCreateAnchorState(BES_SCANANDBAG);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnRAPLightItemBtn(void)
{
	trace(L2, _T("+SMStateBase::OnRAPLightItemBtn"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing

 	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
    SAClearSecurityCondition();

	g_lTABMisMatchApprWt = g_lCTW;	//TAR200247 Now have to get RAP appr wt off belt without any more mismatched wts LPM042502
	//TAR 214691 Reset wt expected and number of entries variables
	g_bWtExpected = false;	//We should never expect wt if button is pressed
	g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries

	trace(L2, _T("-SMStateBase::OnRAPLightItemBtn"));
    return setAndCreateAnchorState(BES_SCANANDBAG);
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnRAPItemNotBaggedBtn(void)
{
	trace(L2, _T("+SMStateBase::OnRAPItemNotBaggedBtn"));
	DMSaySecurity( SEC_NONE, _T("blank.wav") );			//to interrupt the wave file if it's playing

 	// resets lane light, flushes belt, sets re-enable scanner flag, and tells RAP no data is needed
    SAClearSecurityCondition();

	//TAR 214691 Reset wt expected and number of entries variables
	g_bWtExpected = false;	//We should never expect wt if button is pressed
	g_lNumberOfWLDBEntriesThisItem = -1;	// reset the number of WLDB entries

	trace(L2, _T("-SMStateBase::OnRAPItemNotBaggedBtn"));
    return setAndCreateAnchorState(BES_SCANANDBAG);
}

///////////////////////////////////////////
// EAS Motion Sensor detected motion ended
///////////////////////////////////////////
SMStateBase  *SMStateBase::DMdf_EASNoMotion(void)
{
	SASendSCOTInput(PRODUCT_TAG_DEACTIVATED, io.csItemCode, 0, 0, 0, 0, 0);
	m_bEASHappened = true;		//219883 set flag to true for later use LPM121002

	if (g_bMatchedWtWaitingOnEAS)	// if the matched wt occurred first, then return it now...
	{
		g_bMatchedWtWaitingOnEAS = false;
//USSF START
		USSFHOOK(_T("OnMatchedWt"), nextState = OnMatchedWt(), return nextState;);
		//return OnMatchedWt();
//USSF END
	}

	return STATE_NULL;
}

///////////////////////////////////////////////
SMStateBase* SMStateBase::CkStateStackAndReturn(void)
{
  //Check SystemMessage statck first because the priority
  //to see if any states in the stack. If it does, go to
  //that state
   SMStateBase  *sReturnState = PopStateFifoStack();
  	
  if (sReturnState != STATE_NULL)
  {
      CString csStateName = sReturnState->GetRuntimeClass()->m_lpszClassName;
      if (ps.RemoteMode() && !sReturnState->PSRemoteLegalForState()) //TAR408719
      {
          trace(L6,_T("Popped state not legal for remote mode, return %s to stack"),csStateName);
          sReturnState = PushStateFifoStack(sReturnState);
      }
      else
      {
          trace(L6,_T("Popping state %s off stack"),csStateName);
          return sReturnState;
      }
  }

  //Check if there is State from TB waiting, priority 2
  if (g_pDelay_TBExpectedStateChange != NULL && getAnchorState() != BES_OUTOFSERVICE)
  {
    sReturnState = g_pDelay_TBExpectedStateChange;
	g_pDelay_TBExpectedStateChange = NULL;
	CString csStateName = sReturnState->GetRuntimeClass()->m_lpszClassName;
    // TAR 220991:  This is the same as popping off the stack
    sReturnState->SetStatePopped( true );
  	trace(L6,_T("Go to state %s saved in g_pDelay_TBExpectedStateChange"),csStateName);
	return sReturnState;
  }
  //+TAR#385187
  //Keep the state in the stack, if FL is in AssistMenu
  if(co.IsInAssistMenus() && g_pDelay_WaitForApprovalStateChange != NULL )
  {
	   trace(L6,_T("FL is in AssistMenu, keep the state in the stack"));
	   return STATE_NULL;
  }
  //-TAR#385187
  //Check if there is WaitForApproval state waiting, priority 3
  if (g_pDelay_WaitForApprovalStateChange != NULL)
  {
    sReturnState = g_pDelay_WaitForApprovalStateChange;
	g_pDelay_WaitForApprovalStateChange = NULL;
	CString csStateName = sReturnState->GetRuntimeClass()->m_lpszClassName;
    // TAR 220991:  This is the same as popping off the stack
    sReturnState->SetStatePopped( true );
  	trace(L6,_T("Go to state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
	return sReturnState;
  }

  return STATE_NULL;
}

///////////////////////////////////////////////
void SMStateBase::ClearStateStackEOT(void)
{
   //TAR195100+  CHU022502: Pop the states saved in the stack, and delete all saved states
   //Pop all SystemMessage states saved in statck
   SMStateBase  *sReturnState = PopStateFifoStack();
   while (sReturnState != STATE_NULL)
   {
     CString csStateName = sReturnState->GetRuntimeClass()->m_lpszClassName;
	 delete sReturnState;
     trace(L6,_T("Popping state %s off stack"),csStateName);
     sReturnState = PopStateFifoStack();
   }

   //If there is State from TB waiting, clear it
   if (g_pDelay_TBExpectedStateChange != NULL)
   {
     CString csStateName = g_pDelay_TBExpectedStateChange->GetRuntimeClass()->m_lpszClassName;
     delete g_pDelay_TBExpectedStateChange;
	 g_pDelay_TBExpectedStateChange = NULL;
	 trace(L6,_T("Delete the state %s saved in g_pDelay_TBExpectedStateChange"),csStateName);
   }

   //If there is WaitForApproval state waiting, clear it
   if (g_pDelay_WaitForApprovalStateChange != NULL)
   {
     CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
   	 delete g_pDelay_WaitForApprovalStateChange;
	 g_pDelay_WaitForApprovalStateChange = NULL;
	 trace(L6,_T("Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
   }

   //TAR195100-  CHU
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::DMTakeawaySendScaleStatus()
{
	// Takeaway Belt has encountered an POSSIBLE security condition and requests
	// current scale information in order to determine what action to take.
	// Send a Good, Bad, or Zero weight signal to TAB control
    trace(L6, _T("+::DMTakeawaySendScaleStatus"));


    trace(L6, _T("-::DMTakeawaySendScaleStatus"));

	return STATE_NULL;
}

///////////////////////////////////////////
// Received Coupon Sensor Event
///////////////////////////////////////////
SMStateBase  *SMStateBase::OnCouponSensor(void)
{
	return STATE_NULL;
}

///////////////////////////////////////////
// Use Coupon Sensor
///////////////////////////////////////////
bool  SMStateBase::IsUsingCouponSensor(void)
{
	if (co.fStateCouponSensor
		&& (co.csTenderAllowCoupons == _T("A"))
		&& dm.IsCouponSensorAvailable()   )
	{
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////
// Received Motion Cash Out Event
///////////////////////////////////////////
SMStateBase  *SMStateBase::OnMotionCashOutSensor(void)
{
	return STATE_NULL;
}
///////////////////////////////////////////
// Received No Motion Cash Out Event
///////////////////////////////////////////
SMStateBase  *SMStateBase::OnNoMotionCashOutSensor(void)
{
	SMStateBase::fCashOutSensorBlocked = false; //317732
	return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////


SMStateBase* SMStateBase::OnDataBaseInfoComplete(void)
{
	// base  behavior - do nothing
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
void SMStateBase::TBShow2x20(void)
{
	TBShow2x20(_T(""));
}


///////////////////////////////////////////////////////////////////////////
void SMStateBase::TBShow2x20(CString csOpDisplay)
{
   _TUCHAR szDispHi[25] = {0};
   _TUCHAR szDispLow[25] = {0};
   //   unsigned char szDisp[50] = {0};
   _TUCHAR szDisp[300] = {0};   //HD Merging

   if (csOpDisplay.IsEmpty())
   {
	   if (fInEndorsementState)
	   {
		   // when tb gets updated 2x20 messages while it is waiting to endorse a document
		   // ignore updating 2x20 and keep the document description(e.g., "CHECK   $4.44") untouched
		   return;
	   }
	tb.Get2x20((LPCTSTR)szDisp);
   }
   else
   {
	   int nLen = csOpDisplay.GetLength();
	   if (nLen >=50)
	   {
			nLen = 49;
	   }
	   _tcsnccpy((LPTSTR)szDisp, (LPCTSTR)csOpDisplay, nLen);
	   szDisp[50]=NULL;
   }
   if (co.csUserExitString5 != _T("OmitCustSensitiveDataInTraces")) // TAR:219151
   {
   	  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("TBShow2x20: %s"), szDisp) ;
   }
   else
   {
	   // let SSF or Tb decide what information to log based on trx context
   }
   memmove(szDispHi,szDisp,20*sizeof(_TCHAR));
   szDispHi[20]=0x00;
   memmove(szDispLow,&szDisp[20],20*sizeof(_TCHAR));
   szDispLow[20]=0x00;
   if (fStateAssist)
     ps.Echo((LPCTSTR)szDispHi,(LPCTSTR)szDispLow);
   return;
}

///////////////////////////////////////////////////////////////////////////
TenderButton &SMStateBase::GetTenderButton(int iIndex)
{
    ASSERT((iIndex > 0 && iIndex <= MAXBUTTONNUM ) ||
      (nTenderButtonIndex > 0 && nTenderButtonIndex <= MAXBUTTONNUM));
    return co.TenderList[ps.m_languageUsed][iIndex == -1 ? nTenderButtonIndex - 1 : iIndex - 1];
}

///////////////////////////////////////////////////////////////////////////
void SMStateBase::SetTenderButton(int iIndex)
{
    nTenderButtonIndex = iIndex;
}

int SMStateBase::GetButtonIndex(int nTenderType)
{
	for(int i = 0; i < MAXBUTTONNUM; i++)
	{
		if(co.TenderList[ps.GetLanguage()][i].nTenderType == nTenderType)
			return i+1;
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////
// assistmode tender will be mapped to corresponding button if possible
int SMStateBase::AdjustTenderButtonIndex(TBTENDERTYPE nAMTender)
{
	short int nTenderIndex = -1;
    // find associated button section in SCOTTend.dat
	// tender button index starts from 1 (1- cash ...etc.)
	for (short i = 1; (i < MAXBUTTONNUM); ++i)
	{
		TBTENDERTYPE nTmp = (TBTENDERTYPE) GetTenderButton(i).nTenderType;
		trace(L6, _T("Iteration %d got tender type:%d"), i, nTmp);
		if (nTmp == nAMTender)
		{
			nTenderIndex = i;
			trace(L0, _T("Found a match for assisted tender"));
			break;
		}
	}
    // if found, check whether it needs SCOT to capture signature or not
	if (nTenderIndex != -1)
	{
        if ( (nTenderButtonIndex != -1) &&
			 (((GetTenderButton(nTenderIndex).bCaptureSignature ==
             GetTenderButton(nTenderButtonIndex).bCaptureSignature) &&
             GetTenderButton(nTenderIndex).bCaptureSignature) ||

             ((GetTenderButton(nTenderIndex).bVerifySignature ==
             GetTenderButton(nTenderButtonIndex).bVerifySignature) &&
             GetTenderButton(nTenderIndex).bVerifySignature))
           )
        {
            // multiple sig-needed payments
            // leave the responsibility to collect sigs to SCOTSSF/TB
            nTenderIndex = -1;
        }
		SetTenderButton(nTenderIndex);
	}
	return nTenderIndex;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::HandleDataNeeded()
{
    static long rc;  // TB return value
    CString pDetails;

    trace(L6, _T("+SMStateBase::HandleDataNeeded()"));

    if(g_bTBNeedMoreData)
    {
      trace(L6, _T("SMStateBase::HandleDataNeeded(), Notify TB of button click"));

      pDetails = m_sButton;   // "command" parameter for the button pressed
      pDetails += _T(";");
      pDetails += m_sInput;   // data entered
      rc = tb.SendDataNeeded(pDetails);

      ProcessTBReturn(rc);
    }
    else
      trace(L6, _T("SMStateBase::HandleDataNeeded(), TB has already been notified!"));

    g_bTBNeedMoreData = false;  // TAR 238061

    // start of TAR 265143
    if( g_bFromRAP)
    {
      g_bClearOccurred = true;
    }
    else
    {
      SAClearSecurityCondition();
    }
    // end of TAR 265143
    // moved for TAR 265143   SAClearSecurityCondition();	// TAR209439 LPM071102 reset the lane lite

    trace(L6, _T("-SMStateBase::HandleDataNeeded() details=%s"),pDetails);

    return STATE_NULL;
}

  //RFQ 2506 - Add method for PCI-compliant dataneeded handling of MSR data
SMStateBase *SMStateBase::HandleMSRData()
{
    trace(L6,_T("HandleMSRData"));
    
    CString pDetails = _T("MSR_DATA;");
    
    if(g_bTBNeedMoreData)
    {
        if (cDMLastTrack)
        {
            //+codes here are copied from SMStateBase::GetAccountNum
            //use LPTSTR SMStateBase::cDMLastTrack to get the track data
            //get track and validate
            short int nT1, nT2, nT3;
            short int nShortLen = sizeof(short);
            
            nT1 = nT2 = nT3 = 0;
            nT1 = (short ) *(  (const char *)cDMLastTrack);						//length of Track 1
            nT2 = (short ) *( ((const char *)cDMLastTrack) + nShortLen);		//length of Track 2
            nT3 = (short ) *( ((const char *)cDMLastTrack) + (nShortLen * 2));	//length of Track 3
            
            _TCHAR szTrack1[300];
            _TCHAR szTrack2[300];
            _TCHAR szTrack3[300];
            
            int nPosition = 3*nShortLen;
            memcpy(szTrack1, ((const char *)cDMLastTrack + nPosition), nT1*sizeof(_TCHAR));
            szTrack1[nT1] = '\0';
            
            nPosition += nT1 * sizeof(_TCHAR);
            memcpy(szTrack2, ((const char *)cDMLastTrack + nPosition), nT2*sizeof(_TCHAR));
            szTrack2[nT2] = '\0';
            
            nPosition += nT2 * sizeof(_TCHAR);;
            memcpy(szTrack3, ((const char *)cDMLastTrack + nPosition), nT3*sizeof(_TCHAR));
            szTrack3[nT3] = '\0';
            
            //    memcpy(szTrackData + nDataIDLen, ((const char *)cDMLastTrack + (3*nShortLen)), nHeaderLen*sizeof(_TCHAR));
            CString csCardData = _T("track1=");
            csCardData += szTrack1;
            csCardData += _T(";track2=");
            csCardData += szTrack2;
            csCardData += _T(";track3=");
            csCardData += szTrack3;
            csCardData += _T(";");
            
            pDetails += csCardData;
            
            
            
            if (cDMLastTrack)
            {
                delete cDMLastTrack;
                cDMLastTrack = NULL;
            }
        }
        else
        {
            trace(L6, _T("SMStateBase::HandleMSRData - No Card Data on Swipe!!!"));
        }
    }
    else
        trace(L6, _T("SMStateBase::HandleMSRData(), TB has already been notified!"));

    long rc = tb.SendDataNeeded(pDetails);
    
    g_bTBNeedMoreData = false;  // TAR 238061

    // start of TAR 265143
    if( g_bFromRAP)
    {
      g_bClearOccurred = true;
    }
    else
    {
      SAClearSecurityCondition();
    }

    trace(L6, _T("-SMStateBase::HandleDataNeeded() pDetails strlen=%d"),pDetails.GetLength());
    ProcessTBReturn(rc);
    
    DMMSREject();                     // eject DcL-
    DMMSRDisable();                   // disable
    
    return STATE_NULL;
}  //RFQ 2605 End

////////////////////////////////////////////////////////////////////////////
// SMWaitForAssistanceBase state now has available 4 buttons
// These 4 buttons IDC_REMOTEBUTTON1 - IDC_REMOTEBUTTON4 work together with
// the ASSISTANCE_NEEDED_FRAME
// The default for REMOTEBUTTON1 is to handle it as a Done button which
// will disconnect from Assist Mode
// SMSmAssistModeBase also has a similar function
SMStateBase  *SMStateBase::PSRemoteButton1(void)		// RFC 209075 RJF 07/31/02
{
	trace(L6, _T("SMStateBase::PSRemoteButton1()"));
	fStateAssist = false;
	return createAnchorState();
}
void SMStateBase::SetStatePopped( bool bPopped )
{
    m_bStatePopped = bPopped;

}
bool SMStateBase::GetStatePopped()
{
    return m_bStatePopped;
}

bool SMStateBase::IsCashDeviceError(long lDeviceClass)
{

    trace(L6,  _T("+SMStateBase::IsCashDeviceError()"));
    bool bCheckingCashDevice = (
            (lDeviceClass == DMCLASS_CASHCHANGER) ||
            (lDeviceClass == DMCLASS_CASHACCEPTOR) ||
            (lDeviceClass == DMCLASS_COINACCEPTOR) ||
            (lDeviceClass == DMCLASS_CASHRECYCLER)
        );

    trace(L6,  _T("-SMStateBase::IsCashDeviceError() %d"), bCheckingCashDevice );
    return bCheckingCashDevice;
}

bool SMStateBase::IsCardDeviceError(long lDeviceClass)
{
    trace(L6,  _T("+SMStateBase::IsCardDeviceError()"));
    bool bCheckingCardDevice = (lDeviceClass == DMCLASS_MSR );

    trace(L6,  _T("-SMStateBase::IsCardDeviceError() %d"), bCheckingCardDevice );
    return bCheckingCardDevice;
}
//SR742-

////////////////////////////////////////////////////////////////////////////
// TABTransportClearedItem is needed
//	1) When a RAP approval occurs
//	2) When Store Login occurs
//	3) When the SmartScale returns to LastGoodWt
//	4) When the TAB requests the scale's current status
////////////////////////////////////////////////////////////////////////////
void SMStateBase::TABTransportClearedItem(void)
{
	//TAR229916 CHU: Removed obsolete code since we always return good weight
	if (g_pDelay_WaitForApprovalStateChange == NULL)    //TAR202677 CHu
	{
		trace(L6,  _T("SMStateBase::TABTransportClearedItem(): Tell TAB this is good weight."));
		DMTakeawayBeltNotify(tabEventStableGoodWeight);
	}

}

/////////////////////////////////////////////////////////
void SMStateBase::TABReEnableScannerAndClearWaitForApprovalState(void)
{
	if(dm.fStateTakeawayBelt)	// is on a take away system, always re-enable the scanner
	{
		trace(L2, _T("bEASReEnableScanner is set to true."));
		bEASReEnableScanner = true;
	}

	//If there is WaitForApproval state waiting, clear it
	if (g_pDelay_WaitForApprovalStateChange != NULL)
	{
		CString csStateName = g_pDelay_WaitForApprovalStateChange->GetRuntimeClass()->m_lpszClassName;
		delete g_pDelay_WaitForApprovalStateChange;
		g_pDelay_WaitForApprovalStateChange = NULL;
		trace(L6,_T("Delete the state %s saved in g_pDelay_WaitForApprovalStateChange"),csStateName);
	}
}
/////////////////////////////////////////////////////////
//Retrieving the Description from View for Reporting Purposes
/////////////////////////////////////////////////////////
CString SMStateBase::GetDescFromView(int nView)
{
	trace(L7, _T("+SMStateBase::GetDescFromView"));
	CString csView = EMPTY_STRING;
    SCOT_LANGUAGE_TYPE customerLanguage;

    //Report Descriptions are always captured in Primary language
    customerLanguage = ps.GetLanguage();
    ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);

	switch(nView)
	{
	case DATANEEDED_UNEXPECTEDINCREASE:
		csView= ps.GetPSText(MSG_DATANEEDED_UNEXPECTEDINCREASE); // _T("UNEXPECTEDINCREASE");
		break;
	case DATANEEDED_CONFIRMSUSPEND:
		csView = ps.GetPSText(MSG_DATANEEDED_CONFIRMSUSPEND);    // _T("CONFIRMSUSPEND");
		break;
	case DATANEEDED_APPROVEVOIDEDITEM:    // 0 - Voided Item Approval view
		csView= ps.GetPSText(MSG_DATANEEDED_APPROVEVOIDEDITEM);    //_T("APPROVEVOIDEDITEM");
		break;
   case DATANEEDED_MISMATCHWEIGHT:
	    csView = ps.GetPSText(MSG_DATANEEDED_MISMATCHWEIGHT);    //_T("MISMATCHWEIGHT");
		rp.m_bItemAssoc = true;
	    break;
   case DATANEEDED_DONTBAGTHRESHOLDEXCEEDED:
	    csView = ps.GetPSText(MSG_DATANEEDED_DONTBAGTHRESHOLDEXCEEDED);    //_T("DONTBAGTHRESHOLDEXCEEDED");
	    break;
   case DATANEEDED_NEWITEMENTRY:
	    csView = ps.GetPSText(MSG_DATANEEDED_NEWITEMENTRY);    //_T("NEWITEMENTRY");
	    break;
   case DATANEEDED_BAGITEMTIMEREXPIRED:
	    csView = ps.GetPSText(MSG_DATANEEDED_BAGITEMTIMEREXPIRED);    //_T("BAGITEMTIMEREXPIRED");
	    break;
   case DATANEEDED_DONEREMOVINGTHRESHOLDEXCEEDED:
	    csView = ps.GetPSText(MSG_DATANEEDED_DONEREMOVINGTHRESHOLDEXCEEDED);    //_T("DONEREMOVINGTHRESHOLDEXCEEDED");
	    break;
   case DATANEEDED_WALKAWAYTIMEREXPIRED:
	    csView = ps.GetPSText(MSG_DATANEEDED_WALKAWAYTIMEREXPIRED);    //_T("WALKAWAYTIMEREXPIRED");
	   break;
   case DATANEEDED_PRICENOTMATCH:
	    csView = ps.GetPSText(MSG_DATANEEDED_PRICENOTMATCH);    //_T("PRICENOTMATCH");
	    break;
   case DATANEEDED_COUPONTHRESHOLDEXCEEDED:
	    csView = ps.GetPSText(MSG_DATANEEDED_COUPONTHRESHOLDEXCEEDED);    //_T("COUPONTHRESHOLDEXCEEDED");
	    break;
   case DATANEEDED_COUPONDOLLARMAX:
	    csView = ps.GetPSText(MSG_DATANEEDED_COUPONDOLLARMAX);    //_T("COUPONDOLLARMAX");
	   break;
   case DATANEEDED_PRICEPROMPT:
	    csView = ps.GetPSText(MSG_DATANEEDED_PRICEPROMPT);    //_T("PRICEPROMPT");
	    break;
   case DATANEEDED_MOREDATA:
	    csView = ps.GetPSText(MSG_DATANEEDED_MOREDATA);    //_T("MOREDATA");
	    break;
   case DATANEEDED_BAGBOXTHRESHOLDEXCEEDED:
	    csView = ps.GetPSText(MSG_DATANEEDED_BAGBOXTHRESHOLDEXCEEDED);    //_T("BAGBOXTHRESHOLDEXCEEDED");
	    break;
   case DATANEEDED_UNEXPECTEDDECREASE:
	    csView = ps.GetPSText(MSG_DATANEEDED_UNEXPECTEDDECREASE);    //_T("UNEXPECTEDDECREASE");
		break;
   case DATANEEDED_DEVICEERROR:
   case DATANEEDED_WLDBFAILURE:
	    csView = ps.GetPSText(MSG_DATANEEDED_DEVICEERROR);    //_T("DEVICEERROR");
	    break;
   //+dp185016
   case DATANEEDED_OUTOFSYNC_STOP:
	    csView = ps.GetPSText(MSG_DATANEEDED_OUTOFSYNC_STOP);    // need reboot
	    break;
   case DATANEEDED_OUTOFSYNC_ASSIST:
	    csView = ps.GetPSText(MSG_DATANEEDED_OUTOFSYNC_ASSIST);    // need assist
	    break;
   //-dp185016
   case DATANEEDED_QUANTITY: // Updates Item Object's Quantity
	    csView = ps.GetPSText(MSG_DATANEEDED_QUANTITY);    //_T("QUANTITY");
	    break;
   case DATANEEDED_ENTERITEM:         // Requests an item QTY/SKU and enters item
	    csView = ps.GetPSText(MSG_DATANEEDED_ENTERITEM);    //_T("ENTERITEM");
		break;
   case DATANEEDED_ENTERCOUPON:       // Requests a coupon QTY/AMOUNT and updates item
	    csView = ps.GetPSText(MSG_DATANEEDED_ENTERCOUPON);    //_T("ENTERCOUPON");
	   break;
   case DATANEEDED_MEASUREMENT:       // Request a measument of "Description" and updates ???
	    csView = ps.GetPSText(MSG_DATANEEDED_MEASUREMENT);    //_T("MEASUREMENT");
		break;
   case DATANEEDED_ASSEMBLYFEE:
	    csView = ps.GetPSText(MSG_DATANEEDED_ASSEMBLYFEE);    //_T("ASSEMBLYFEE");
	   break;
   case DATANEEDED_VERIFYIDENTITY:   // verify the customer name for BIC card
	    csView = ps.GetPSText(MSG_DATANEEDED_VERIFYIDENTITY);    //_T("VERIFYIDENTITY");
	   break;
   case DATANEEDED_AUTHORIZATIONNUM:  // enter authorization number for credit cards
	    csView = ps.GetPSText(MSG_DATANEEDED_AUTHORIZATIONNUM);    //_T("AUTHORIZATIONNUM");
	   break;
   case DATANEEDED_CSASUSPEND:        // Request suspend confirmation for services
	    csView = ps.GetPSText(MSG_DATANEEDED_CSASUSPEND);    //_T("CSASUSPEND");
	   break;
   case DATANEEDED_DLIDNUMBER:        // enter driver's license or id number
	    csView = ps.GetPSText(MSG_DATANEEDED_DLIDNUMBER);    //_T("DLIDNUMBER");
	   break;
   case DATANEEDED_CANCELLTRANSACTION:// cancel transaction
	    csView = ps.GetPSText(MSG_DATANEEDED_CANCELLTRANSACTION);    //_T("CANCELTRANSACTION");
	   break;
     //Begin RFC 356020
   case DATANEEDED_CHECKONLANE:// check on lane
	   csView = ps.GetPSText(RA_CHECK_ON_LANE);    
	   break;
   case DATANEEDED_CHECKONLANERAP:// check on lane
	   csView = ps.GetPSText(RA_CHECK_ON_LANE);    
	   break;
   //End RFC 356020
   case DATANEEDED_ENTERACCOUNTNUMBER:// Manual Credit Card Number Entry
	    csView = ps.GetPSText(MSG_DATANEEDED_ENTERACCOUNTNUMBER);    //_T("ENTERACCOUNTNUMBER");
	   break;

   case DATANEEDED_ENTEREXPDATE:      // Manual Credit Card Date entry.
	    csView = ps.GetPSText(MSG_DATANEEDED_ENTEREXPDATE);    //_T("ENTEREXPDATE");
	   break;
   case DATANEEDED_PRICEPRMT:       // price prompt
	    csView = ps.GetPSText(MSG_DATANEEDED_PRICEPRMT);    //_T("PRICEPRMT");
	   break;
   case DATANEEDED_ASSISTEDTENDER:    // Tender requiring RAP assistance.
	    csView = ps.GetPSText(MSG_DATANEEDED_ASSISTEDTENDER);    //_T("ASSISTEDTENDER");
	   break;
   case DATANEEDED_CLOSELANE:         // awk.2.6.2 TAR192509

	    csView = ps.GetPSText(MSG_DATANEEDED_CLOSELANE);    //_T("CLOSELANE");
	   break;
   case DATANEEDED_ENDORSEDOCUMENT:   // Endorsement for a document is needed by TB
	    csView = ps.GetPSText(MSG_DATANEEDED_ENDORSEDOCUMENT);    //_T("ENDORSEDOCUMENT");
	   break;
   case DATANEEDED_DOCUMENTNUM:       // Document number (View Name: "DocumentNum")
	    csView = ps.GetPSText(MSG_DATANEEDED_DOCUMENTNUM);    //_T("DOCUMENTNUM");
	   break;
   case DATANEEDED_BAGAREABACKUP:
	    csView = ps.GetPSText(MSG_DATANEEDED_BAGAREABACKUP);    //_T("BAGAREABACKUP");
	   break;
   case DATANEEDED_UNDEACTIVATEDITEM: // Undeactivated item on TABelt detected
       csView = ps.GetPSText(MSG_DATANEEDED_UNDEACTIVATEDITEM);    //_T("UNDEACTIVATEDITEM");
	   break;
   case DATANEEDED_VERIFYSIGNATURE:  // awk.4.11.2 TAR199004
	    csView = ps.GetPSText(MSG_DATANEEDED_VERIFYSIGNATURE);    //_T("VERIFYSIGNATURE");
	   break;
   case DATANEEDED_ASSISTEDITEM:     // RFC 205489 RJF 6/11/02
	    csView = ps.GetPSText(MSG_DATANEEDED_ASSISTEDITEM);    //_T("ASSISTEDITEM");
	   break;
   case DATANEEDED_GIFTCARDLIMIT:	// RFC 211574, approval for gift card purchase
	    csView = ps.GetPSText(MSG_DATANEEDED_GIFTCARDLIMIT);    //_T("GIFTCARDLIMIT");
	   break;
   case DATANEEDED_TAKECHANGETIMEREXPIRED:	// Take Change Timer Expired 3.2 req 3.4.2
	    csView = ps.GetPSText(MSG_DATANEEDED_TAKECHANGETIMEREXPIRED);    //_T("TAKECHANGETIMEREXPIRED");
	   break;
   case DATANEEDED_LASTVIEW:
	   csView = ps.GetPSText(MSG_DATANEEDED_LASTVIEW);    //_T("LASTVIEW");
	   break;
   case DATANEEDED_INSERTCOUPONTIMEREXPIRED:	// Insert Coupon Timer Expired RFC 219178
	    csView = ps.GetPSText(MSG_DATANEEDED_INSERTCOUPONTIMEREXPIRED);    //_T("INSERTCOUPONTIMEREXPIRED");
	   break;
   case DATANEEDED_COLLECTGIFTCARD:
		csView= ps.GetPSText(MSG_DATANEEDED_COLLECTGIFTCARD); // _T("COLLECTGIFTCARD");
		break;
   case DATANEEDED_HOSTOFFLINE:
	    csView = ps.GetPSText(MSG_DATANEEDED_CLOSELANE);    //_T("CLOSELANE");
		break;
   // + TAR 352409
   case DATANEEDED_SUSPENSION_DECLINE:							// _T("ApproveSuspensionDecline"); // TAR 352409
	    csView = ps.GetPSText(MSG_DATANEEDED_APPROVEVOIDEDITEM);
		break;
   // - TAR 352409

   case DATANEEDED_STARTTRAININGTRANS: //cs185088
        csView = ps.GetPSText(MSG_DATANEEDED_TRAININGMODE_TRANS); //_T("TRAINING MODE TRANSACTION")
        break;
   //SR085 +
   case DATANEEDED_BAGBOXNOTVALID: 
        csView = ps.GetPSText(MSG_DATANEEDED_BAGBOXNOTVALID); //_T("INVALID BAG")
        break;
   //SR085 -
	//+SR678
   	case DATANEEDED_MAXCOUPONLIMIT:
		csView = ps.GetPSText(MSG_DATANEEDED_MAXCOUPONLIMIT);
  		break;
	case DATANEEDED_MAXCOUPONLIMITFINALIZATION:
		csView = ps.GetPSText(MSG_DATANEEDED_MAXCOUPONLIMIT);
		break;
   //-SR678
	//+ TAR 391995
	case DATANEEDED_STARTTRANSACTION:
		csView = ps.GetPSText(MSG_DATANEEDED_STARTTRANSACTION);
		break;
	case DATANEEDED_THANKYOUEXPIRED:
		csView = ps.GetPSText(MSG_DATANEEDED_THANKYOU_EXPIRED);
		break;
	case DATANEEDED_STARTTRANSACTIONAM:
		csView = ps.GetPSText(MSG_DATANEEDED_STARTTRANSACTION_AM);
		break;
	case DATANEEDED_SAFETYCOVEROPEN:
		csView = ps.GetPSText(MSG_DATANEEDED_SAFETYCOVER_OPEN);
		break;
	case DATANEEDED_MULTIPACKITEM:
		csView = ps.GetPSText(MSG_DATANEEDED_MULTIPACK_ITEM);
		break;
	//- TAR 391995
   default:
       csView = ps.GetPSText(MSG_DATANEEDED_DEFAULT);
       break;
	 }
	ps.SetLanguage(customerLanguage);
    trace(L7, _T("-SMStateBase::GetDescFromView<%s>"),csView );

	return csView;
 }
/////////////////////////////////////////////////////////
//Retrieving the Category from View for Reporting Purposes
/////////////////////////////////////////////////////////
CString SMStateBase::GetCategory(DWORD m_nRpID)
{
	trace(L7, _T("+SMStateBase::GetCategory"));
	CString csCat = EMPTY_STRING;
    SCOT_LANGUAGE_TYPE customerLanguage;

    //Report Category are always captured in Primary language
    customerLanguage = ps.GetLanguage();
    ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);

	switch(m_nRpID)
	{
	case RA_CAT_OTHER:
		 csCat = ps.GetPSText(MSG_RA_CAT_OTHER);  // _T("OTHER");
	     break;
	case RA_CAT_UNKNOWN:
		 csCat = ps.GetPSText(MSG_RA_CAT_UNKNOWN);  //_T("UNKNOWN");
		 break;
	case RA_CAT_RECEIPT:
	     csCat = ps.GetPSText(MSG_RA_CAT_RECEIPT);  //_T("RECEIPT");
	     break;
	case RA_CAT_DEVERR:
	     csCat = ps.GetPSText(MSG_RA_CAT_DEVERR);  //_T("DEVICEERROR");
	     break;
	case RA_CAT_SECURITY:
         csCat = ps.GetPSText(MSG_RA_CAT_SECURITY);  //_T("SECURITY");
	     break;
	case RA_CAT_VOID:
	     csCat = ps.GetPSText(MSG_RA_CAT_VOID);  //_T("VOID");
	       break;
	case RA_CAT_COLLECTION:
		 csCat = ps.GetPSText(MSG_RA_CAT_COLLECTION);  //_T("COLLECTION");
		 break;
	case RA_CAT_AGERESTRICT:
		 csCat = ps.GetPSText(MSG_RA_CAT_AGERESTRICT);  //_T("AGERESTRICT");
		 break;
	case RA_CAT_EFT:
	     csCat = ps.GetPSText(MSG_RA_CAT_EFT);  //_T("EFT");
	     break;
	case RA_CAT_AUDACTION:
	     csCat = ps.GetPSText(MSG_RA_CAT_AUDACTION);  //_T("AUDACTION");
	     break;
	case RA_CAT_OUTOFSERVICE:
		 csCat = ps.GetPSText(MSG_RA_CAT_OUTOFSERVICE);  //_T("OUTOFSERVICE");
		 break;
	case RA_CAT_MEDIASHORTAGE:
		 csCat = ps.GetPSText(MSG_RA_CAT_MEDIASHORTAGE);  //_T("MEDIASHORTAGE");
		 break;
	case RA_CAT_PRICENEEDED:
		 csCat = ps.GetPSText(MSG_RA_CAT_PRICENEEDED);  //_T("PRICENEEDED");
		 break;
	case RA_CAT_UNKNOWNITEM:
	     csCat = ps.GetPSText(MSG_RA_CAT_UNKNOWNITEM);  //_T("UNKNOWNITEM");

		 rp.m_bItemAssoc = true;
		 break;
	case RA_CAT_RECALLEDITEM:
	     csCat = ps.GetPSText(MSG_RA_CAT_RECALLEDITEM);  //_T("RECALLEDITEM");
		 break;
	case RA_CAT_TIMERESTRICT:
		 csCat = ps.GetPSText(MSG_RA_CAT_TIMERESTRICT);  //_T("TIMERESTRICT");
		 break;
	case RA_CAT_SUSPEND:
		 csCat = ps.GetPSText(MSG_RA_CAT_SUSPEND);  //_T("SUSPEND");
		 break;
	case RA_CAT_CALLFORHELP:
		 csCat = ps.GetPSText(MSG_RA_CAT_CALLFORHELP);  //_T("CALLFORHELP");
		 break;
	case RA_CAT_NEGATIVEDEPT:
		 csCat = ps.GetPSText(MSG_RA_CAT_NEGATIVEDEPT);  //_T("NEGATIVEDEPT");
		 break;
	case RA_CAT_VISUALVERIFY:
	     csCat = ps.GetPSText(MSG_RA_CAT_VISUALVERIFY);  //_T("VISUALVERIFY");
		 break;
	case RA_CAT_SIGCAP:
	     csCat = ps.GetPSText(MSG_RA_CAT_SIGCAP);  //_T("SIGCAP");
	     break;
	case RA_CAT_OTHERPAYMENT:
	     csCat = ps.GetPSText(MSG_RA_CAT_OTHERPAYMENT);  //_T("OTHERPAYMENT");
	     break;
	case RA_CAT_QTYRESTRICT:
	     csCat = ps.GetPSText(MSG_RA_CAT_QTYRESTRICT);  //_T("QTYRESTRICT");
		 break;
	case RA_CAT_DATANEEDED:
         // This one is special, shouldn't be called
	     csCat = _T("DATANEEDED");
	     break;
	case RA_CAT_EASITEMACTIVE:
	     csCat = ps.GetPSText(MSG_RA_CAT_EASITEMACTIVE);  //_T("EASITEMACTIVE");
	     break;
	// + TAR 352409
    case RA_CAT_SUSPENSIONDECLINED:						  //_T("SUSPEND DECLINED"); // TAR 352409
         csCat = ps.GetPSText(MSG_RA_CAT_SUSPENSIONDECLINED);
		 break;
	// - TAR 352409
    // +TAR 400905 
    case RA_CAT_COUPON: 
         csCat = ps.GetPSText(MSG_RA_CAT_COUPON); 
         break; 
    // -TAR 400905 
	}
	ps.SetLanguage(customerLanguage);
    trace(L7, _T("-SMStateBase::GetCategory<%s>"), csCat);
	return csCat;
}

///////////////////////////////////////////////////////////////////////////
// Base function for determining if Assist Mode is done with TAB
///////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::DMTakeawayAMExitDone()
{
    // Base functionality does nothing
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
//229084 created a Base function for getNextStateForGoodItem LPM022503
///////////////////////////////////////////////////////////////////////////
BEST SMStateBase::getNextStateForGoodItem()
{
    if (dm.fStateTakeawayBelt)
        return BES_TRANSPORTITEM;
    else
        return BES_SCANANDBAG;
}

///////////////////////////////////////////////////////////////////////////////////////
// Tar 211746 RJF 041603 red and yellow lights both on problems, needed new function
// to assist in determining whether red light should be turned off
// Created a base function that can check all flags related to red approvals
///////////////////////////////////////////////////////////////////////////////////////
bool SMStateBase::AreAnyRedApprovalsPending()
{
	bool fReturnVal = false;
	if(bIsInFinalization)	// if in finalization, then yellow approvals become red
	{
		if (isRestrictedItemApprovalRequired())
			fReturnVal = true;

		if (isVisualItemApprovalRequired())
			fReturnVal = true;

		if (isCouponRatioApprovalRequired())
			fReturnVal = true;
	}

	if ((lNotForSaleItems>0)			||
		(lUnknownItems>0)				||
		(lPriceRequiredItems>0)			||
		(lRestrictedNotAllowedItems>0)	||
		(lTimeRestrictedItems>0)		||
		(bSuspendFailure)				||
		(lCouponsInAssist>0)			||		//TAR#102019
		(lQuantityRestrictedItems>0)	||
		(fOtherPayment)				    ||		// for outstanding approvals printing
	    (lUnapprovedVoidedItems>0)		||
		(fUnDeActivatedItem))
	{
		fReturnVal = true;
	}
	if (fCantScanCoupon || fRequestForHelp)
		fReturnVal = true;
	if (bDropoffRequired && !bDropoffApproved)
		fReturnVal = true;
	if (bSuspendRequested && !bSuspendApproved)
		fReturnVal = true;
	return fReturnVal;
}

///////////////////////////////////////////////////////////////////
void SMStateBase::SendCurrencyAcceptedToRAP(long lAcceptedAmount)
{
	if(!bHasToldRAPAboutBeginCashTender)
	{
		bHasToldRAPAboutBeginCashTender = true;
		trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(RA_BEGINCASHTENDER, SCOT_LANGUAGE_PRIMARY));
 		ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+
                      ps.GetPSText(RA_BEGINCASHTENDER, SCOT_LANGUAGE_PRIMARY));
		trace(L7, _T("-ra.OnNormalItem"));
	}
	CString csTemp = EMPTY_STRING;
	csTemp.Format(ps.GetPSText(MSG_RA_CASHINSERTED, SCOT_LANGUAGE_PRIMARY), ps.FmtDollar(lAcceptedAmount));
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), csTemp);
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY+ csTemp);
	trace(L7, _T("-ra.OnNormalItem"));
}
///////////////////////////////////////////////////////////////////
bool SMStateBase::LaunchProgram(LPCTSTR szBatchFile, DWORD dwWaitTime, bool bKillIfHung)
{
	bool bRet = false;
	int rc;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.lpReserved=0;
	si.lpDesktop=0;
	si.lpTitle=0;

	if (!CreateProcess(NULL, (LPTSTR) szBatchFile, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
	{
		trace(L6, _T("LaunchProgram - CreateProcess failed to launch program."));
		return false;
	}
    if (WaitForSingleObject(pi.hProcess, dwWaitTime) != WAIT_OBJECT_0)
	{
		if (bKillIfHung)
		{
			trace(L6, _T("LaunchProgram - CreateProcess timed out. Program either did not start or hung, terminating program."));
			rc = TerminateProcess(pi.hProcess, 0);
			if (0 != rc)
			{
				trace(L6, _T("LaunchProgram - Process terminated succesfully"));
			}
			else
			{
				trace(L6, _T("LaunchProgram - Process failed to terminate succesfully"));
			}
		}
		trace(L6, _T("LaunchProgram - CreateProcess timed out. Program did not start or hung."));
	}
	else
	{
		bRet = true;
	}
	CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
	return bRet ? true : false;
}
///////////////////////////////////////////////////////////////////
SMStateBase * SMStateBase::PerformEOD()
{
	TCHAR szEODFileName[_MAX_PATH];
	trace(L7, _T("+PerformEOD"));
	_tmakepath(szEODFileName,NULL,co.csConfigFilePath,_T("eod.bat"), _T(""));
	rp.ModeChange(0,_T("Status"),_T("EOD"));
	rp.m_bForceEnd = true;
	rp.TransactionEnd();
	// Run the EOD.bat file
	bool rc = LaunchProgram(szEODFileName, (DWORD) co.nOperationsEODTimeout, true);
	// If in Store mode just call ReloadOptions function, otherwise
	// display the Out Service state
/*	Tar 253037 {+
  //Don't change screen even in customer mode.
  if (fSAInStoreMode)
	{
		trace(L7, _T("-PerformEOD - In Store Mode"));
		return ReloadOptions();
	}
	else
	{
		trace(L7, _T("-PerformEOD - In Customer Mode"));
		CREATE_AND_DISPENSE(OutOfService)(2);
	}*/
		trace(L7, _T("-PerformEOD "));
		m_bReloadingOptions = true; //tar 352145
		SMStateBase * pRetState = ReloadOptions();
		m_bReloadingOptions = false;
		return pRetState;
  //Tar 253037 -}

}

///////////////////////////////////////////////////////////////////
SMStateBase * SMStateBase::HostOffLine(void)
{
    ScotError(RETURN, SM_CAT_LAN_WAN_COMM, SCOTAPP_TB_HOST_OFFLINE);
	RETURNSTATE(HostOffLine);
}

///////////////////////////////////////////////////////////////////
SMStateBase * SMStateBase::HostOnLine(void)
{
    ScotError(INFO, SM_CAT_LAN_WAN_COMM, SCOTAPP_TB_HOST_ONLINE);
	return STATE_RETURN;
}

////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnTransactionComplete()
{
	//TAR381316+
	if (g_bInterventionApproval)
	{
		//Clear RAP Approval so it tells report about interventionend
		trace(L2, _T("Calling ra.OnNoRAPDataNeeded()"));
		ra.OnNoRAPDataNeeded();  
	}
	//TAR381316-
	
	//TAR 388220
	trace(L6, _T("csLastNonHelpStateName: %s"), csLastNonHelpStateName);
	if( 0 == csLastNonHelpStateName.CompareNoCase(_T("SMThankYou")) )
	{		
		RETURNSTATE(ThankYou)
	}

	return STATE_NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation)
{
   	trace(L6, _T("+OverrideSecurityException Authorization= %s Operation = %s"),lpszAuthorization, lpszOperation);
    CKeyValueParmList parmList;
	// Override the Immediate Intervention - check the ImmediateInterventionStack
	// if one Intervention on the stack - clear it using the Exception id
	// if more than one, then peek the head of stack - if unexpected inc, dec, then clear all
	// if more than one, then peek the head of stack - if threshold intervention, then clear it
	// if more than one, then peek the head of stack - if mismatch intervention, then clear all

    if ( ImmediateInterventionStack.GetCount() )
    {
		SecImmediateIntervention *psecImmediateIntervention;
        // pop the last exception
		psecImmediateIntervention = ImmediateInterventionStack.GetAt(
			ImmediateInterventionStack.GetHeadPosition() );

		if (ImmediateInterventionStack.GetCount() == 1)
		{
		   	trace(L6, _T("OverrideSecurityException ImmediateInterventionCount is 1") );
			// TAR 277635
			// "None" is only specified with bag/box security override - since we have to clear
			// unexpected-inc that caused the initial violation. In this case, g_bLastImmediateInterventionCleared
			// flag should not be set.
			if (CString(lpszAuthorization) == _T("None") )
			{
				g_bLastImmediateInterventionCleared = false;
			}
			else
			{
				g_bLastImmediateInterventionCleared = true;
			}
		   	trace(L6, _T("+OverrideSecurityException g_bLastImmediateInterventionCleared= %d"),g_bLastImmediateInterventionCleared);

			_TCHAR buf[10];
			_itot(psecImmediateIntervention->lExceptionId,buf,10);
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_OSE_EXCEPTION_ID, (_TCHAR *) buf );
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_AUTHORIZATION, lpszAuthorization);
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_OPERATION, lpszOperation);
		}
		else
		{
   			trace(L6, _T("OverrideSecurityException Intervention type= %s"),psecImmediateIntervention->csSecInterventionType);
			if ( (psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE ) ||
				(psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_UNEX_DECREASE ) ||
				(psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_INVALID_BAG))//SR085
			{
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_CATEGORY, KEY_EXCEPTION_CATEGORY_WT_CHANGES );
			}
			else if ( (psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD ) ||
					  (psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD ) ||
  					  (psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD ) ||
  					  (psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_NEW_ITEM_THRESHOLD ) ||
  					  (psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_ZERO_WT_THRESHOLD )  )
			{
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_CATEGORY, KEY_EXCEPTION_CATEGORY_THRESHOLD );
			}
			else if ( psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_MISMATCHED )
			{
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_CATEGORY, KEY_EXCEPTION_CATEGORY_MISMATCH );
			}
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_AUTHORIZATION, lpszAuthorization);
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_OPERATION, lpszOperation);

		}
		_bstr_t bstrParmList((const _TCHAR *)parmList);
//		delete  psecImmediateIntervention; Only makes sense to delete this if we remove it
      // which we did not.
	    SecMgr.OverrideException(lSecMgrCookie, bstrParmList);

		//TAR 275670 When override bag/box threshold, should also override wt-change too (in this case it should be unexpected increase)
      if(psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD
       || psecImmediateIntervention->csSecInterventionType == KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD //dp185016 tar 285550
      )
		{
			parmList.Clear();
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_CATEGORY, KEY_EXCEPTION_CATEGORY_WT_CHANGES );
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_AUTHORIZATION, lpszAuthorization);
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_OPERATION, lpszOperation);
			_bstr_t bstrParmList2((const _TCHAR *)parmList);
			SecMgr.OverrideException(lSecMgrCookie, bstrParmList2);
		}
		//TAR 275670
   }

   trace(L6, _T("-OverrideSecurityException"));

}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation, LPCTSTR lpszCategory)
{
	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_CATEGORY, lpszCategory );
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_AUTHORIZATION, lpszAuthorization);
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_EXCEPTION_OPERATION, lpszOperation);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	SecMgr.OverrideException(lSecMgrCookie, bstrParmList);

}
////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::TellApplicationStateToSecurity(LPCTSTR lpszOperation, LPCTSTR lpszStateName)
{
	trace(L5, _T("+TellApplicationStateToSecurity Operation = %s StateName = %s"), lpszOperation, lpszStateName );

	 CKeyValueParmList parmList;
	 BSTR bstrResult;
		
	if (co.IsSecurityBypassOn())
	{
		trace(L5, _T("Security bypass on,  ignoring state change"));
	}
   else
   	{
	    CKeyValue<const _TCHAR*>::Put(parmList, KEY_AC_SET_APP_STATE, lpszOperation);
	    
		if (co.IsLoginBypassOn())
	    {
		  CKeyValue<const _TCHAR*>::Put(parmList, _T("SmAssistMode"));
	    }
	    else
	    {
		  CKeyValue<const _TCHAR*>::Put(parmList, lpszStateName);
	    }
    
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);
	}
	
    CString csStateName(lpszStateName);
	if(csStateName == _T("SmAssistMode") || csStateName == _T("SmRemoteAssistMode"))
	{
		CString csOpName(lpszOperation); // tar340275 mm185096

        //if (lpszOperation == KEY_AC_SET_APP_STATE_END) // tar340275 mm185096
		if (csOpName == KEY_AC_SET_APP_STATE_END)
        {
			if(!co.IsSecurityBypassOn())
			{
				parmList.Clear();
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
				_bstr_t bstrParmList((const _TCHAR *)parmList);
				bstrResult = NULL;
				SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
            }
			trace(L5,_T(" TellApplicationStateToSecurity: Disconnecting from RAP"));
			CString csRemoteConnectionName = ra.GetRemoteConnectionName();
            m_pAppPSX->GenerateEvent( _T(""), _T(""), _T("RemoteData"), _T("operation=assist-release;"), csRemoteConnectionName );
            ps.DisconnectFromServer( TRUE );
			ra.RequestEnableAssistMode(true);
        }
	}

	trace(L5, _T("-TellApplicationStateToSecurity"));

}
///////////////////////////////////////////////////////////////////
long SMStateBase::ChangeSecurityMaintenanceWtFlags(LPCTSTR lpszOperation, LPCTSTR lpszUPC,LPCTSTR lpszFlag, bool bPriceEmbedded)
{
   	trace(L6, _T("+ChangeSecurityMaintenanceWtFlags Operation= %s UPC = %s Flag = %s PriceEmbedded = %d"),lpszOperation, lpszUPC, lpszFlag, bPriceEmbedded); // TAR 311984
	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, lpszOperation);
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC,lpszUPC );
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_FLAG_VALUE, lpszFlag );
	CKeyValue<bool>::Put(parmList, KEY_SMM_PRICE_EMBEDDED, bPriceEmbedded );
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	long lRc =SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);
	trace(L6, _T("-ChangeSecurityMaintenanceWtFlags"));
	return lRc;
}
///////////////////////////////////////////////////////////////////
long SMStateBase::ChangeSecurityMaintenanceTolerance(LPCTSTR lpszOperation, LPCTSTR lpszUPC, LPCTSTR lpszTolernace, bool bPriceEmbedded)
{
   	trace(L6, _T("+ChangeSecurityMaintenanceTolerance Operation= %s UPC = %s Tolerance = %s PriceEmbedded = %d"),lpszOperation, lpszUPC, lpszTolernace, bPriceEmbedded); // TAR 311984
	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SMM_OPERATION, lpszOperation);
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_UPC,lpszUPC );
	CKeyValue<const _TCHAR*>::Put(parmList,KEY_SMM_TOLERANCE,lpszTolernace );
	CKeyValue<bool>::Put(parmList, KEY_SMM_PRICE_EMBEDDED, bPriceEmbedded );
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	long lRc = SecMgr.SecurityMaintenance(lSecMgrCookie, bstrParmList);
	trace(L6, _T("-ChangeSecurityMaintenanceTolerance"));
	return lRc;
}

///////////////////////////////////////////////////////////////////
SMStateBase* SMStateBase::ReturnDelayedStateForGCTender()
{
	if (m_bTrxEnded)
	{
		m_bTrxEnded = false;
		return TBFinish();
	}
	else
	{
		return m_pSavedStateInTendAccepted;
	}
}
///////////////////////////////////////////////////////////////////
SMStateBase* SMStateBase::TareWeightedItem(void)
{
	return STATE_NULL;
}
///////////////////////////////////////////////////////////////////
SMStateBase* SMStateBase::TareOnly(void)
{
	return STATE_NULL;
}
///////////////////////////////////////////////////////////////////
SMStateBase* SMStateBase::TareWLDBItem(void)
{
	return STATE_NULL;
}
///////////////////////////////////////////////////////////////////
SMStateBase* SMStateBase::TareNone(void)
{
	return STATE_NULL;
}

///////////////////////////////////////////////////////////////////
//SR085 +
SMStateBase* SMStateBase::TareOnlyAtAttract(void)
{
	return STATE_NULL;
}
//SR085 -

///////////////////////////////////////////////////////////////////
void SMStateBase::SetSecurityConfig(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
{
   	trace(L6, _T("+SetSecurityConfig Param name= %s ParamValue = %s"),lpszParamName, lpszParamValue);
	CKeyValueParmList parmList;
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_SET);
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_PARAM, lpszParamName);
	CKeyValue<const _TCHAR*>::Put(parmList, lpszParamValue);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
	BSTR bstrResult;
	bstrResult = NULL;
	SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
	// Need to look at bstrResult on return
	SysFreeString(bstrResult);
	trace(L6, _T("-SetSecurityConfig"));

}

///////////////////////////////////////////////////////////////////
bool SMStateBase::GetSecurityConfig(LPCTSTR lpszParamName)
{
   	trace(L6, _T("+GetSecurityConfig Param name= %s"),lpszParamName);
	bool bRet;
	CKeyValueParmList parmList;
   	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_GET);
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_PARAM, lpszParamName);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
    BSTR bstrResult;
	bstrResult = NULL;
	SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
	SecurityControlParms SCKeyValueparms;
	CString csResult(bstrResult);
	SCKeyValueparms.ParseKeys(csResult);
	int iFind = -1;
	iFind = csResult.Find((_TCHAR)_T(','));
	if(iFind != 0)
	{
		csResult = csResult.Right(1);
		if (csResult == _T("N"))
		{
			bRet = false;
		}
		else
		{
			bRet = true;
		}
	}
	// Need to look at bstrResult on return
	SysFreeString(bstrResult);
	trace(L6, _T("-GetSecurityConfig"));
	return bRet;
}

// SR682 - Return SecurityConfig.xml value as CString
///////////////////////////////////////////////////////////////////
CString SMStateBase::GetSecurityConfigValue(LPCTSTR lpszParamName)
{
   	trace(L6, _T("+GetSecurityConfigValue Param name= %s"),lpszParamName);
	CKeyValueParmList parmList;
   	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_GET);
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_PARAM, lpszParamName);
	_bstr_t bstrParmList((const _TCHAR *)parmList);
    BSTR bstrResult;
	bstrResult = NULL;
	SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
	SecurityControlParms SCKeyValueparms;
	CString csResult(bstrResult);
	SCKeyValueparms.ParseKeys(csResult);
	int iFind = -1;
	iFind = csResult.Find((_TCHAR)_T(','));
	SysFreeString(bstrResult);
	trace(L6, _T("-GetSecurityConfigValue"));
	if(iFind != 0)
		csResult = csResult.Mid(iFind+1);
	return csResult;
}


//TAR262985+
bool SMStateBase::OpenSCOTFile(CString csFileName, bool bAbortOnOpenFail, bool bIsDefaultFile)
{
	HFILE hFile = NULL;
	CString csMsg;
	int nOpenMode;
	// open csFileName with usual open mode
	nOpenMode = CFile::modeRead | CFile::shareDenyWrite | CFile::typeText;
	CStdioFile FileWrap;

	//+TAR247967 - changing all upper case MFC Macro TRY/CATCH to C++ try/catch
	try
	{
		hFile =  FileWrap.Open(csFileName, nOpenMode);
	}
	catch( CFileException* e )
	{
		csMsg.Format(_T("File Error:%s could not be opened. Cause:%s \n"), csFileName, e->m_cause);
		TRACE(_T("%s"), csMsg);     // debug output window
		trace(L0, _T("%s"), csMsg); // log file
#ifdef _DEBUG_
		afxDump << _T("File Error:") << csFileName << _T("could not be opened,cause: ") << e.m_cause << _T("\n");
#endif
		e->Delete();
	}
	//-TAR247967

	bool bRetStatus = false;
	if (!hFile && bIsDefaultFile)
	{
		if (csMsg.IsEmpty())
			csMsg.Format(_T("File Error:%s could not find. \n"), csFileName);
		TRACE(_T("%s"), csMsg);     // debug output window
		trace(L0, _T("%s"), csMsg); // log file
		HRESULT rc;
		if (bAbortOnOpenFail)
			rc = ScotError(FATAL,SM_CAT_CONFIGURATION, SCOTAPP_PS_FILE, _T("%s"),csFileName); // event log
		else
			rc = ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_PS_FILE, _T("%s"),csFileName); // event log

		ASSERT(rc);
	}
	else if(hFile)
	{
		FileWrap.Close();
		bRetStatus = true;
	}
	return bRetStatus;
}

//TAR262985-

//TAR 269159
bool SMStateBase::InvalidCouponPrefix(CString& csItemSent)
{

  int prefixLen = co.csStateInvalidCouponPrefix.GetLength();

  CString newcsItemSent = CString(csItemSent);
  TrimSymbology(newcsItemSent);

  if ( co.csStateInvalidCouponPrefix.Compare(newcsItemSent.Left(prefixLen))==0  && prefixLen!= 0)
  {
	  trace(L2, _T("The item has an invalid prefix."));
	   return true;
  }
  else
  {
	   return false;
  }

}

void SMStateBase::SynchronizeTime()
{
	TCHAR szSyncTimeFileName[_MAX_PATH];
	trace(L7, _T("+SynchronizeTime"));
	_tmakepath(szSyncTimeFileName,NULL,co.csConfigFilePath,_T("SyncTime.bat"), _T(""));
	// Run the SyncTime.bat file
	bool rc = LaunchProgram(szSyncTimeFileName, (DWORD) co.getTimeOutSyncTimeout(), true);
	trace(L7, _T("-SynchronizeTime"));
}

// + TAR 301706
void SMStateBase::SynchronizeStoreloginWithLaunchPad(LPCTSTR szMessage)
{
	// TAR295879
	// Synchronize the Store login with LaunchPad so that if the user turns the keyswitch to
	// Normal (supervisor mode), the signon status will match scotapp.
 	LPCTSTR szTarget = _T("LaunchPadNet.exe");
	if ( 0 != ::FindWindow( NULL, szTarget ))
	{
 		trace( L6, _T("SMStateBase::SynchronizeStoreloginWithLaunchPad() - szMessage %s") , szMessage);
		if ( ! SendProcessMessage(szTarget, szMessage) )
		{
	 		trace( L2, _T("SMStateBase::SynchronizeStoreloginWithLaunchPad() - SendProcessMessage to LaunchPad.exe failed.") );
		}
	}
	// Fix 318447
	// Output the Sign On signature to the report server.
	//+TAR 343053
	CString csTemp(szMessage);
	if(csTemp.Find(_T("set-signed-off")) != -1)//Call this for operator sign OFF's only.
	{
	bool bMakeXMLNow = false;
	if (!rp.IsTransactionInProgress())
	{
		bMakeXMLNow = true;
		rp.TransactionStart(rp.m_csBusinessDate);
	}
	rp.SetOperator(csOperator);
	
	//TAR371933+
    if (!ps.RemoteMode() && !co.IsInAssistMenus()) 
	{
		rp.OperatorLogin(rp.m_lOperatorLoginID, ATTOUT, SCOTAPP);
	}
	//TAR371933-

	rp.m_lOperatorLoginID += 1;
	if (bMakeXMLNow)
	{
		rp.TransactionEnd();
	}
	}
	//-TAR 343053
}
// - TAR 301706
// SSF can override this function if the message in question should be allowed to pass
bool SMStateBase::IgnoreTBMessageInSecurityState()
{
	return true;
}

//TAR 348946 - start
BOOL SMStateBase::SCOTDeleteFile(LPCTSTR lpSourceFileName)
{
	int iStat = DeleteFile(lpSourceFileName);
	if (!iStat)
	{
		long lErr = GetLastError();
		long rc;
		_TCHAR sError[300];

		rc = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, lErr, 0, sError, 299, NULL);
		trace(L2, _T("Error %d(0x%x) on delete - %s"), lErr, lErr, sError);
	}
	return iStat;
}
//TAR 348946 - end

//TAR 364905 Begin
bool SMStateBase::IsForceDelayedApprovalBeforeSuspend()
{
	if (isRestrictedItemApprovalRequired() || isCouponRatioApprovalRequired()
		|| isVisualItemApprovalRequired())
		return true;
	else
		return false;
}
//TAR 364905 End

SMStateBase *SMStateBase:: GenerateTrainModeIntervention ()
{
	trace(L7, _T("+ra.ONRAPDataNeeded, RAP Intervention"));
	m_View = DATANEEDED_STARTTRAININGTRANS;
	CString csSecInfo;
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("StartTrainingModeTrans"));
	csSecInfo.Format(ps.GetPSText(MSG_DATANEEDED_TRAININGMODE_TRANS, SCOT_LANGUAGE_PRIMARY));
	ra.OnRAPDataNeeded(csSecInfo, cDataEntry.ToString(),m_View);
	trace(L7, _T("-ra.ONRAPDataNeeded"));

	CREATE_AND_DISPENSE(RAPDataNeeded)(ps.GetPSText(MSG_TM_TRANSACTION),
		DATANEEDED_STARTTRAININGTRANS);
} 

bool SMStateBase::ShrinkPicklistImages()
{
    ps.Echo(PL_IMAGES_LOADING);
	DMSayPhrase(KEYPADVALID);
	
    TCHAR szFilePath[_MAX_PATH];
	_tcscpy( szFilePath, co.csShrinkPicklistImageCMD ); //RFC381050
	ExpandPartitionStrings(szFilePath, szFilePath);

	bool rc = LaunchProgram(szFilePath, INFINITE, false);

	ps.Echo(PL_IMAGES_LOADED);

	return rc;
}

//+SR93 Biometrics
FT_ACTION getRegisterAction (FT_STATUS_PT pStatus, void *pParam);


FT_ACTION SMStateBase::getRegisterAction (FT_STATUS_PT pStatus, void *pParam)
{
	PSEVT nEvent = PS_UKEVT;
	long lParm = 0;
	long wParm = 13; //enter key	
	CString csMsg;	

	switch(pStatus->code) 
	{
		case FT_IMAGE_READY:
		{
			TRACE(_T("FT_IMAGE_READY\n"));
			break;
		}
        case FT_FINGER_TOUCHING:
		{
			to.Trace(L6,_T("FT_FINGER_TOUCHING"));			
			break;
		}
		case FT_UNSOLICITED_INFO:
		{
			if (pStatus->param1 == RC_DEVICE_PLUGGED)
			{
				g_bBiometricACLDevice = true;
			}
			else if (pStatus->param1 == RC_DEVICE_UNPLUGGED)
			{
				g_bBiometricACLDevice = false;
				//RFC 396351
				CString sCurrContext=ps.GetCurrentContext();
				if(!sCurrContext.CompareNoCase(_T("EnterID")) && (g_csOperator == _T(""))) 
					ps.Message12( MSG_ENTERID_PRESSENTER, false );

			}
			else if (pStatus->param1 == RC_SERVER_READY)
			{
				g_bBiometricACLServer = true;
				//RFC 396351
				if (g_bBiometricACLDevice)
				{
					CString sCurrContext=ps.GetCurrentContext();
					if(!sCurrContext.CompareNoCase(_T("EnterID"))) 
						ps.Message12( MSG_FINGERPRINT_LOGIN, false ); 
				}

			}
			else if (pStatus->param1 == RC_SERVER_NOTREADY)
			{
				g_bBiometricACLServer = false;
				//RFC 396351
				CString sCurrContext=ps.GetCurrentContext();
				if(!sCurrContext.CompareNoCase(_T("EnterID")) && (g_csOperator == _T("")))
					ps.Message12( MSG_ENTERID_PRESSENTER, false );

			}
							
			break;
		}
		case FT_GET_FINGERPRINT_COMPLETE:
		{
			TRACE(_T("FT_GET_FINGERPRINT_COMPLETE\n"));
			to.Trace(L6,_T("FT_GET_FINGERPRINT_COMPLETE with status=%d"),pStatus->param1 );

			if (pStatus->param1 == RC_SUCCESS)
			{									
				BYTE* pFeatures;
				pFeatures = new BYTE[pStatus->param2];	// Fingerprint features
				memcpy(pFeatures,pParam,pStatus->param2);
				int rc = FT_VerifyUser(pFeatures,pStatus->param2);
				to.Trace(L6,_T("FT_VerifyUser() with ret=%d"), rc);
				delete[] pFeatures;	
			}
			else if (g_bBiometricACLDevice == false)
			{				
				ps.SetCMLeadthruText( TXT_ENTER_ID );
				ps.Message12( MSG_ENTERID_PRESSENTER, false );
				ps.AllowInput( MAX_DIGITS_OPERATOR, false, false );
				ps.ShowCMFrame( false );

				g_csOperator = _T("");
				g_csPassword = _T("");
				nEvent = PS_KEYSTROKE;  								
				mo.PostPS( nEvent, lParm, wParm, _T("EnterID") );			
			}
            break;
		}
		case FT_VERIFY_USER_COMPLETE:
		{
			TRACE(_T("FT_VERIFY_USER_COMPLETE\n"));
			to.Trace(L6,_T("FT_VERIFY_USER_COMPLETE with status=%d"),pStatus->param1 );
			if (pStatus->param1 == RC_SUCCESS)
			{
				g_bFPTouched = true;
				g_bFPValid = true;

				//396157
				CString sCurrContext=ps.GetCurrentContext();
				if(sCurrContext.CompareNoCase(_T("SmCashierPassword")) ) //non-head-cashier login
				{
					ps.SetCMFrame(_T("BiometricProcessing"));
					ps.SetCMLeadthruText(LTT_FINGERPRINTPROCESSING);
					ps.Message12(MSG_FINGERPRINTPROCESSING, true);
					ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid); //TAR 407237
					ps.ShowCMFrame( false );
				}

				FT_USERDATA_PT ptr = (FT_USERDATA_PT)pParam;


				try
				{
					
					TCHAR sTempOp[_MAX_PATH + 1];
					TCHAR sTempPwd[_MAX_PATH + 1];
					_tcsncpy(sTempOp, ptr->pUserID, _MAX_PATH);
					_tcsncpy(sTempPwd, ptr->pPwd, _MAX_PATH);
					
					g_csOperator = sTempOp;
					g_csPassword = sTempPwd;
					
					to.Trace(L6,_T("FT_VERIFY_USER_COMPLETE with id: %s"),g_csOperator );
				}
				catch(...)
				{
					to.Trace(L6,_T("FT_VERIFY_USER_COMPLETE exception"));
					g_csOperator = _T("");
					g_csPassword = _T("");
				}
					
				nEvent = PS_KEYSTROKE;
				//396157
				if(sCurrContext.CompareNoCase(_T("SmCashierPassword")) )
					mo.PostPS( nEvent, lParm, wParm, _T("BiometricProcessing") );	
				else
					mo.PostPS( nEvent, lParm, wParm, _T("SmCashierPassword") );

			}			
			else
			{
				//	RC_FP_NO_MATCH				No match found in fingerprint database.
				//	RC_FAILURE					Failure for some unknown reason.
				g_bFPTouched = true;
				g_bFPValid = false;	
				//396157
				CString sCurrContext=ps.GetCurrentContext();
				if(sCurrContext.CompareNoCase(_T("SmCashierPassword")) )
				{
					ps.SetCMFrame(_T("BiometricProcessing"));
					ps.SetCMLeadthruText(LTT_FINGERPRINTPROCESSING);
					ps.Message12(MSG_FINGERPRINTPROCESSING, true);				
					ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid); //TAR 407237
					ps.ShowCMFrame( false );
				}

				g_csOperator = _T("");
				g_csPassword = _T("");
				nEvent = PS_KEYSTROKE;
				//396157
				if(sCurrContext.CompareNoCase(_T("SmCashierPassword")) )
					mo.PostPS( nEvent, lParm, wParm, _T("BiometricProcessing") );
				else
					mo.PostPS( nEvent, lParm, wParm, _T("SmCashierPassword") );

			}
			break;
		}
        case FT_WAITING_FOR_IMAGE:   
		{
            TRACE(_T("FT_WAITING_FOR_IMAGE\n"));
            break;
		}
		default:
			break;
	}

	return FT_ID_CONTINUE;
}
//-SR93 Biometrics

//+RFC 371075
long SMStateBase::GetDisplayedItemCount()
{
      return lItems - ms_lExcludedItems;
}
//-RFC 371075

/**
 * Method:      CouponMaxIntervention
 * Description: Generates the immediate intervention when coupon exceeds the 
 * 		        new limit
 * Parameters:  
 * Returns:     RAPDataNeeded intervention
 * Notes:       Added for SR678_New intervention if coupon exceeds new option     
 *              for monetary value limit
 */

SMStateBase *SMStateBase::CouponMaxIntervention()
{
	CString sDescription, csTemp;
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("MaxCouponApproval"));
	csTemp = ps.GetPSText(MSG_DATANEEDED_MAXCOUPONLIMIT, SCOT_LANGUAGE_PRIMARY) + _T(" ");
    sDescription.Format(csTemp, cDataEntry.ToString(),m_View); 
	CREATE_AND_DISPENSE(RAPDataNeeded)(sDescription,DATANEEDED_MAXCOUPONLIMIT);
}

/**
 * Method:      CouponMaxInterventionFinal
 * Description: Generates the immediate intervention when coupon during  
 *              finalization exceeds the new limit
 * Parameters:  
 * Returns:     RAPDataNeeded intervention
 * Notes:       Added for SR678_New intervention if coupon exceeds new option     
 *              for monetary value limit
 */

SMStateBase *SMStateBase::CouponMaxInterventionFinal()
{
	CString sDescription, csTemp;
	cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("CouponWarningAtFinalization"));
	csTemp = ps.GetPSText(MSG_DATANEEDED_MAXCOUPONLIMIT, SCOT_LANGUAGE_PRIMARY) + _T(" ");
    sDescription.Format(csTemp, cDataEntry.ToString(),m_View); 
	CREATE_AND_DISPENSE(RAPDataNeeded)(sDescription,DATANEEDED_MAXCOUPONLIMITFINALIZATION);
}

//-SR678

// SR742+
SMStateBase *SMStateBase::ExitDegradedMode()
{
    trace(L6, _T("+SMStateBase::ExitDegradedMode()") );
    dm.SetDegradedModeType(DEGRADEDMODE_OFF);
    co.fTenderCashAllowed = true;
    rp.ModeChange(rp.m_lAttendantEntryID, DEGRADEDMODE, EXIT);

    return STATE_NULL;
}


SMStateBase *SMStateBase::EnterDegradedMode(DEGRADEDMODES lModeType)
{
    trace(L6, _T("+SMStateBase::EnterDegradedMode()") );
    dm.SetDegradedModeType(lModeType);
    rp.ModeChange(rp.m_lAttendantEntryID, DEGRADEDMODE, ENTER);

    return STATE_NULL;
}
// SR742-

CString SMStateBase::getOperatorID(CString csRapName)
{
	for(int i=0;i < csRapOperatorIDs.size();i++)
	{
		
		if(csRapOperatorIDs[i].csRapName == csRapName)
		{
			trace( L7, _T("SMStateBase::getOperatorID() returns RAP %s - %s") , csRapName, csRapOperatorIDs[i].csRapOPID);
			return csRapOperatorIDs[i].csRapOPID;
		}
	}
	return _T("NotFound");
}

void SMStateBase::addOperatorID(CString csRapName, CString csOpID)
{
	trace( L7, _T("SMStateBase::addOperatorID() for RAP %s - %s") , csRapName,csOpID);
		
	if(getOperatorID(csRapName) != _T("NotFound"))
	{
		
		for(int i=0;i < csRapOperatorIDs.size();i++)
		{
		
			if(csRapOperatorIDs[i].csRapName == csRapName) 
			{
				csRapOperatorIDs[i].csRapOPID = csOpID;
			}
		}
	}
	else{
		RapOperator newOperator;
		newOperator.csRapName = csRapName;
		newOperator.csRapOPID = csOpID;
		csRapOperatorIDs.push_back(newOperator);
	}

}

//+SR713
/**
 * Method:      PreTenderState
 * Description: Handle activities that occur between SMScanAndBag and SMFinish are completed.  
 *				SellBags State, Check your Basket, Delayed Interventions,   
 *              
 * Parameters:  
 * Returns:     
 * Notes:       Added for SR713 - Ability to charges for bag at tender time    
 *             
 */


SMStateBase *SMStateBase::PreTenderState()
{

 CString csPreviousStateName = this->csLastNonHelpStateName;

	if(co.fOperationsSellBagsAtFinalize && lItems && !bDoNotShowSellBags && (lItems != lCoupons))
	{
		
		CString sItemCode, sPrice;


		if(co.csOperationsSellBagsItemCode == _T(""))
		{
			SendTBCommand(TBCMD_CMDSTRING, _T("SellBags"));
			CREATE_AND_DISPENSE(InProgress)(false,false,0);
		}
		else
		{
			sItemCode = co.csOperationsSellBagsItemCode;
			sPrice	  = co.csOperationsBagPrice;
			
			CREATE_AND_DISPENSE(SellBags)(sItemCode,-1,sPrice);
		}
	
	}

 else if(co.fStateCheckBasketReminder && lItems &&  csPreviousStateName != _T("SMCheckBasketReminder"))
 {
  //if previous state is ScanAndBag
  if (csPreviousStateName ==  _T("SMScanAndBag") )
   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
  RETURNSTATE(CheckBasketReminder)
 }


 else if (isAnyApprovalRequired(false))
 {
     // TAR + 418788 - If we still have any approvals pending, ensure that the red trilight is turned on and that the
     // yellow trilight is off. This eliminates yellow flashing when we transition from a red to a green light.
     dm.setTriColorLight(DM_TRICOLORLIGHT_RED,
                         DM_TRICOLORLIGHT_BLINK_1HZ,
                         DM_TRICOLORLIGHT_NORMALREQUEST);
     dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                         DM_TRICOLORLIGHT_OFF,
                         DM_TRICOLORLIGHT_NORMALREQUEST);
     // TAR - 418788

   bIsInPreTenderState = false;
   CREATE_AND_DISPENSE(WaitForApproval)(false,false);
  
 }
 else
 {
  bIsInPreTenderState = false;
  return setAndCreateAnchorState(BES_FINISH);
 }
}
//-SR713


bool  SMStateBase::IsRemoteErrorAllowed(long lDeviceClass) //TAR408719
{

	bool bLegal;
	
	switch(lDeviceClass)
	{
	case DMCLASS_SIGCAP:
	case DMCLASS_KEYLOCK:
	case DMCLASS_CASHACCEPTOR:
	case DMCLASS_COINACCEPTOR:
	case DMCLASS_CASHRECYCLER:
	case DMCLASS_CASHCHANGER:
	case DMCLASS_SCALE:
	case DMCLASS_MOTION_SENSOR:
	case DMCLASS_EASDEACTIVATOR:
	case DMCLASS_MISC:
		bLegal = false;
		break;
	default:
		bLegal = true;
	}
	return bLegal;

}

CString SMStateBase::GetWideCharToMultiByte(const CString &csData)
{
#ifdef _UNICODE
	unsigned int nCodePage = _ttoi(m_csPrinterCharSet);
	int nInDataLen = csData.GetLength();

	if ( (nCodePage == 852) && nInDataLen>0 )
	{
		int nOutDataLen = WideCharToMultiByte(nCodePage, 0, csData, nInDataLen, NULL, 0, NULL, NULL);

		char *szData = new char[nOutDataLen + 1];
		memset(szData, 0, nOutDataLen+1 );
		int nDataLen = WideCharToMultiByte(nCodePage, 0, csData, nInDataLen, szData, nOutDataLen+1, NULL, NULL);

		CString csConvertedData(szData, nDataLen);

		trace( L6, _T("SMStateBase::GetWideCharToMultiByte() - InDataLen: %d, OutDataLen: %d, DataLen: %d"), 
			nInDataLen, nOutDataLen, nDataLen);

		delete[] szData;

		return csConvertedData;
	}
	else
	{
		return csData;
	}
#else
	return csData;
#endif
}