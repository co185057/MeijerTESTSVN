//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  ConfigObject.H
//
// TITLE:   Class interface definition file (describe class here)
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIGOBJECT
#define _CONFIGOBJECT
#include "DLLDefine.h"
#include "traceobject.h"


#define MAX_TENDER_CASHBACK_VALUES      11
#define MAX_TENDER_EBT_CASHBACK_VALUES  11
#define MAX_PRINTOBJECTS	2
#define MAXBUTTONNUM  15     //New Finalization

typedef	struct 
	{
		int nTenderType;
		CString csTenderText;
		CString csTenderBitmap;
		bool bCashBack;
		bool bConfirmTotal;
		bool bNeedPin;
		bool bIntegratedMSR;
		bool bCaptureSignature;
		bool bVerifySignature;
		int nVerifySignatureOverAmount;
		HBITMAP hTenderBitmap;
        int  nAssistedTender; // prompts at RAP to use Assistmode to complete this tender as of now
		int  nButtonNum;	  // NewUI - stores button number
		int  nTenderLimit;    // trx total is checked against this limit and then intervention follows(=0,always intervention, <0 no intervention, -1 is default value)
		bool bCashBackIfVoided; //RFC 275377 MIGRATED-BM185018
	}TenderButton;

typedef struct
{
   TCHAR szCode[5];          //Two letter ISO 639 code for language or 4 character language code
   CString csName;           //Button for language on Attract screen
   CString csMessageFile;    //Customer Text message file
   CString csTareFile;       //Tare description file
   CString csSecMessageFile; //Security message file (ie. SASIActione)
   CString csTenderFile;     //Tender file
   CString csPriceSoundConfigFile;  //Price Sound Config File
   CString csButtonBitmap;   //Bitmap filename used for the language button in the lang selection screen
} SLanguageInfo;

const enum SCOT_LANGUAGE_TYPE
{
   SCOT_LANGUAGE_PRIMARY,
   SCOT_LANGUAGE_SECONDARY,
   SCOT_LANGUAGE_THIRD,
   SCOT_LANGUAGE_FOURTH,
   SCOT_LANGUAGE_FIFTH,
   SCOT_LANGUAGE_SIXTH,

   SCOT_LANGUAGE_MAX  // If adding in more language choices, add them before the max entry
};

typedef struct 
{
	CString csBmpName;
	long    nBmpWidth;
	long    nAlignment;
	float   scalingFactor;           //TAR 271776
} SBmpPrintObject;


/////////////////////////////////////////////////////////////////////////////
// ConfigObject command target

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 ConfigObject : public CObject
{
private:
  int  nTracingMillisecondsToFlush;		//THD Performance RFC
  int  nTimeOutSyncTimeout;				// timeout value to wait for Sync batch to run
  int  nTimeOutTender;
  int  nTimeOutStateDefault;		// customer mode timeout sec, typical 60 sec
  int  nTimeOutTakeCard;
  int  nTimeOutTakeCash;
  int  nTimeOutTakeCoins;
  //int  security wait at thank you
  int  nTimeOutItemTransactionResult;
  int  nTimeOutContinueTransaction;
  int  nTimeOutHelp;
  int  nTimeOutSignon;
  int  nTimeOutSignoff;
  int  nTimeOutAttract;
  int  nTimeOutItemSaleInProgress;
  int  nTimeOutEASDeactivationScreenTimer;
  int  nTimeOutCouponSensorScreenTimer;
  int  nTimeOutTenderBalanceScreenTimer;
  int  nTimeOutDepositGiftCardScreenTimer;
  int  nTimeOutCheckBasketReminder;
  int  nTimeOutInitialBagItemAudio;
  int  nTimeOutPeriodicBagItemAudio;
  int  nTimeOutInitialRemovingItemsAudio;
  int  nTimeOutPeriodicRemovingItemsAudio;
  int  nTimeOutWalkAwayTimerAudio;
  int  nTimeOutInitialTakeYourItemAudio;
  int  nTimeOutRecurringTakeYourItemAudio;     //RFC209702
  int  nTimeOutSecurityServerTimer;
  int  nTimeOutThankYouTimerBelt;
  int  nTimeOutInitialClearBagAreaAudio;
  int  nTimeOutPeriodicClearBagAreaAudio;
  int  nTimeOutTimeOutTransportItem;
  int  nTimeOutTouchScreenProximityEnablesScannerMotor;
  int  nTimeOutMotionSensorOnAttractInterval;  //RFC 265465
  int  nTimeOutScannerScaleOnAttractInterval; //RFC 276235
  bool fErrorLog;
  int  nTimeOutBagItemEscalationMilliSecs;          // RFC 202655 - in milliseconds
  int  nTimeOutRemovingItemsEscalationMilliSecs;    // RFC 202656 - in milliseconds
  int  nTimeOutDelaySecurityNotificationMilliSecs;  // RFC 223322
  int  nTimeOutDisplayBagItemScreenTimer;			// RFC 233715
  int  nTimeOutPeriodicSecurityAudio;				// RFC 223322
  int  nTimeOutTakeYourItemEscalationMilliSecs;     //RFC209702
  int  nTimeOutHeartbeatTimer;                             // RFC 262339
  int  nTimeOutBusyLeaseTimer;                           //RFC 263404
  int  nTimeOutAttendantModeTimer;
  int  nTimeOutGloryUnitDispenseTimeout;			// TAR 310660
  int  nTimeOutExitSuspendScreenTimer;				// RFC 294962
  int  nTimeOutDisplayIncreaseMisMatchScreenTimer;  //TAR379130/379131
  int  nTimeOutDisplayPutBagOnScaleScreenTimer;//SR085
  int  nTimeOutEnableSkipBaggingButtonTimer; //SR778

  // Begin In-Store Message Support Parameters
  bool fPagingGreenFlashingSendPage, fPagingYellowSendPage;
  bool fPagingRedSendPage, fPagingOutOfServiceSendPage;
  int  nPagingGreenFlashingElapseTime, nPagingYellowElapseTime;
  int  nPagingRedElapseTime, nPagingOutOfServiceElapseTime;
  int  nPagingGreenFlashingLimitTime, nPagingGreenFlashingLimitTrans;
  int  nPagingAvailabilityCount;        // TAR 117805
  // End In-Store Message Support Parameters

  CString* m_pcsMoviePlayOnAttract;		// remember which movie after registry lookup

  void InitializePrintObjects(SBmpPrintObject* pPrintObjList);
  void PullMovieFile( CString& csMovieFileName );

  bool fOperationsManageCardDispenser;	//option to show Card Management button in Media status		
  int  nOperationsCardDispenserBinCapacity;		//card dispenser bin capacity					
  int  nOperationsDispenserRejectBinNearFull;	//card dispenser reject near full. default=15
  int  nOperationsDispenserRejectBinFull;		//card dispenser reject full. default=20
  //This option if set to Y will display the Volume control button in Attract screen
  bool fOperationsShowVolumeControl ;			
  //This value is used to increment the current volume by the percentage of maximum volume value. 
  int nOperationsVolumeControlIncrementLevel; 
  int nTimeOutVolumeControlTimer;	//After this many seconds, set the volume back to default.
  int nTimeOutSingleLanguageButtonAtAttractTimer; //RFQ SR681 After this many seconds, set customer language back to primary
  bool fOperationsPrintDuplicateMsgonPrintLastReceipt;  //RFC 334227
 
public:
	int GetmsTraceToFlush() { return nTracingMillisecondsToFlush; }  //THD Performance RFC - returns MillisecondsToFlush
 // int nOperationsResreictedAge;	  // minimum age in years for purchasing rest'd items
 // int nOperationsTobaccoAge;		  // minimum age in years for purchasing rest'd items // tobacco etc.
  CString csOperationsReceiptFileFormat;		//  RFC 368222 Receipt file format (ANSI or UNICODE)
  CString csOperationsHeadCashierContext;       // store mode menu context for head cashiers
  int nTimeOutCashOutSensorAudioTimer;
  int nOperationsMaxCoupon;		      // max allowed manual coupon value
  int nOperationsMaxCouponLimit;		//SR678 -  holds the value of the coupon limit
  int nOperationsDuplicateCouponMax;
  int nOperationsTotalCoupons$AmtMax;
  int nOperationsUpper$AmtForPercentToApply;
  int nOperationsCouponPercentofGrossSales;
  bool fOperationsAllowSkipBaggingBtn;
  bool fOperationsAllowStoreLoginBtnOnBagItemScreen;	//BZ 06/07/02 RFC 203538 Y/N, Y=true, N=false
  bool fOperationsAllowStoreLoginBtnOnRemoveItemScreen;	//BZ 06/07/02 RFC 203538 Y/N, Y=true, N=false 
  int nOperationsOperatorLoginBarcodeType;    // NN 072902 RFC 187993 0 or 1 or 2.  0=No scannable barcode, 1=EAN13 type, 2=Cose 128 type 
  CString csOperationsOperatorBarcodeMask;    // NN 072902 RFC 187993 
  bool fOperationsAllowLeadingZerosInCode128LoginBarCode; //rfc 240738
  bool fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen; //CHU 07/25/02 RFC 208874
  bool fOperationsAllowContainerBtn;
  bool fOperationsDisableUnexpIncContainerBtnAftFirstItem; 
  bool fOperationsAllowContainerBtnForMismatching;		 // Tar 266250
  bool fOperationsAllowContainerBtnInFinalization;//SR085
  bool fOperationsAllowItemRemovedBtn;
  bool fOperationsAllowAMDoneBtn;		// RFC 209075 RJF 073102
  bool fOperationsShowWeightInfoOnLane;		//SR682
  bool fOperationsSuppressAudioInItemRemoveViolation; // RFC 252471
  bool fOperationsSuppressAudioInUnexpectedIncrease; // 3.2 patch B - RFC 257450
  bool fOperationsSuppressAudioInMismatch;  // 3.2 patch B - RFC 257449
  bool fOperationsShowSkipBaggingOnEReceipt;  // RFC 252473
  int nOperationsMaxGiftCard;         // max allowed gift card value
  bool fOperationsScaleMetric;	      // Y/N, N = false, lbs/100, Y = true kg/1000
  int nOperationsTareContainer1;	  // value 0-10 for SelectContainer button 1
  int nOperationsTareContainer2;	  // value 0-10 for SelectContainer button 2
  bool fOperationsSelectTareOnWeight;		  // Y/N
  bool fOperationsLogWLDBmismatch;		  // RFC 210177 Y/N
  int nOperationsWLDBmismatchMaxFileSize; //RFC 210177default = 5000
  bool fOperationsCheckCashChanger; //Tar 256141
  int nOperationsAutoTareCode;

  bool fOperationsJumpToTarePickList;
  bool fOperationsAllowMultiPick;
  bool fOperationsDisplayPickListItemCode;
  bool fOperationsDisplayQuickPickItems;
  bool m_bOperationsDebitNeedsSignatureCapture;  // Debit tender needs Signature Capture
  bool m_bOperationsDebitNeedsVerifySignature;   // Debit tender needs Verify Signature
  bool m_bOperationsSignatureCapture; // Y/N
  bool m_bOperationsVerifySignature;  // Y/N
  bool m_bOperationsPrintSignatureReceipt; // Y/N
  bool m_bOperationsAllowCancelAll;	  // Y/N
  bool m_bOperationsUseFingerPrintDevice;   //SR93 Biometrics
  bool fOperationsPrintVersion;		  // default is 'Y' = prints app's version info on a receipt and journal also
  bool fOperationsPrintPendingApprovalOnSuspend; // default is 'Y' = print pending approvals on receipt on transaction suspend
  bool fOperationsDisableBasePriceEmbeddedDefinitions;//RFC239992
  bool fOperationsTruncateSupplementalCodeForSecurity; // default is 'Y' = Truncate all Supplemental UPC code before sending to Security
  bool fOperationsPrintReceiptOnCancelledTransaction; //dont print receipt on cancelled transaction
  CString csOperationsEOTReceiptPrintingState; //SR697-Delay receipt printing until finalization intervention complete gc185038
  bool fOperationsAddPaperCutToTransReceipt;  // RFC 368220 - Need option to disable CutReceipt() added to Transaction Receipt ac185097	

//Fiscal Printer start
  bool fOperationsTBControlsPrinter;	//default is 'N' = SCOTAPP controls printer not configured
//Fiscal Printer end
  CString csCurrencyBitmap;		// Used to store the country whose currency bitmap should be used

  bool fOperationsSuspend;            //Suspend Transaction Y/N
  bool fOperationsVoidCouponBeforeSuspend;  // Void coupon before suspend Y/N
  bool fOperationsAllowSuspendIfPartiallyTendered;     //RFC 267820
  bool fOperationsIfSuspendedDispensePartiallyPaidCash;//RFC 267820

  bool fOperationsUPCAPriceEmbeddedVerifier;	// if Price Verifier=Y then UPC A2/CD PriceEmbedded format = 02 iiii V ppppp C
												// else Price Verifier=N then UPC A2/CD PriceEmbedded format = 02 iiiii V pppp C
  bool fOperationsAutoVoidUnderageRestrict;     // Is automaticcally void underage restricted item or need approval. Feng122299
  bool fOperationsSendSignatureDataToTB;                  // If send signature data to TB
  
  //Partial Integrated PINPad
  bool fOperationsPartialIntegratedPINPad;     // Y/N 
  bool fOperationsPIPSCOTPromptCardType ;      // Y/N
  bool fOperationsPIPSCOTPromptCashBack;       // Y/N
  bool fOperationsPIPSCOTPromptConfirmTotal;   // Y/N

  //Integrated MSR and PIP
  bool fOperationsCreditMSR;             // Y/N (Y for integrated MSR, N for PIP device)
  bool fOperationsDebitMSR;              // Y/N (Y for integrated MSR, N for PIP device)
  bool fOperationsDebitSavingsMSR;       // Y/N (Y for integrated MSR, N for PIP device)
  bool fOperationsDebitCheckingMSR;      // Y/N (Y for integrated MSR, N for PIP device)
  bool fOperationsEBTMSR;                // Y/N (Y for integrated MSR, N for PIP device)
  bool fOperationsFoodStampsMSR;         // Y/N (Y for integrated MSR, N for PIP device)
  bool fOperationsGiftCardMSR;           // Y/N (Y for integrated MSR, N for PIP device)
  bool fOperationsPhoneCardMSR;          // Y/N (Y for integrated MSR, N for PIP device)
  bool fOperationsLoyaltyMSR;            // Y/N (Y for integrated MSR, N for PIP device)

  bool fOperationsUse7401MSR;                      // Y/N (Y if integrated MSR is 7401)                    

  int  nOperationsAllowDispenseInTrainingMode; // 0-doesn't allow, 1-allow dispense of change, other values- reserved for future
  bool fOperationsInterventionAtStartOfTrainingModeTransaction; // RAP Intervention in Training Mode cs185088
  bool fOperationsLogDataForSystemReports;     // Used to turn on or off Reports function.
  int  nOperationsNumberOfReportLogsToKeep;
  int  nOperationsRemoteAssistModeToggleDisplay;
  int  nOperationsGiftCardRedemption;
  int  nOperationsMethodOfCollectionForEmptyGiftCard;
  bool fOperationsDisplayGiftCardBalance;
  int  nOperationsEOD;
  long nOperationsEODTimeout;				// timeout value to wait for EOD batch to run
  bool fOperationsCenterCustomerDataNeededButtons;	// Y/N Default to Y
  int nOperationsGloryDispenseRetries;		// TAR 310660

  //USSF START
  bool fOperationsEnableUSSF;
  //USSF END
  
  CString csOperationsTransitionToScanAndBagContexts; // SR730 RFC380486

  bool fOperationsForceDelayedApprovalBeforeSuspend;
  bool fOperationsCustomerBagAllowed;	//SR085 (Y/N)
  bool fOperationsIncludeCouponsInItemCount;	// Rfc 371075

  bool fOperationsAllowPickListSearchButton;
  bool fOperationsAllowFuzzySearch;

  bool fStateSayPrices;						// flag value Y/N
  bool fStateSayPricesDisableInAttendantMode;  // TAR 330578 - Disable SayPrice during AttendantMode
  bool fStateSayPhrases;					// flag value Y/N
  bool fStateAllowTBAudioControl;           // flag value Y/N
  bool fStateLiveVideo;						// flag value Y/N
  bool fStateWelcomeScreenVideo;			// flag value Y/N
  bool fStateSelectListScreenVideo;			// flag value Y/N
  bool fStateGraphicalSelectList;			// flag value Y/N  Default to Y
  bool fStateProduceFavorites;		      	// flag value Y/N  Default to Y
  bool fStateOverridePickDesc;              // flag value Y/N  Default to N
  CString csStateDefaultProduceBMPName;     // Default File Name for Produce Bitmap (Displayed unless a different filename is specified in SCOTPIMG)
  CString csStateInvalidCouponPrefix;         // TAR 269159
  bool fStateDisplayWeightsAndMeasures;		// flag value Y/N
  bool fStateLargeFontOnCurrentItem;		// flag value Y/N
  bool fStateAllowLoyaltyCard;
  bool fStateLoyaltyCardReminder;
  bool fStateLoyaltyCardReminderAfterFirstItemSold; //if Y, play wave file to remind to use loyalty card after first item is sold.
  bool fStateAssistMode;		      // defaults to Y, enable or disable assistmode
  bool fStateLookupItemIsButton1;
  bool fStateAllowLookUpItem;				//TAR 173139
  bool fStateAllowKeyInCode;				//TAR 173138
  bool fStateFlashLookupItemButton;
  bool fStateFlashKeyInCodeButton;
  bool fStateDualLanguage;
  bool fStateSingleLanguageButtonAtAttract;  //RFQ SR681 flag value Y/N default to N
  bool fStateAllowLanguageSelectionDuringTransaction;
  bool fStateHasCardMEI;
  bool fStateAllowInterruptWavFiles;		// Y/N - allow wave files to be interruptable
  bool fStateForceWeighingOFPriceEmbedded;	// Y/N - should Price Embedded items be weighed on the ScannerScale
  bool fStateConfirmationForSuspendingTransaction; // Y/N - go to suspend confirmation screen
  bool fStateCheckBasketReminder;			// Y/N - show an additional screen before F & P screen
  bool fStateCouponSensor;                  // Y/N - turn on coupon sensor
  bool fStateDOBNotNeededButton;            // Y/N - display/not display Date of Birth Not Needed button
  bool fStateDisplayNumberOfItemsOnRAP;		// Y/N - display/not display the number of items in RAP
  CString csStateMoviePlayOnAttract;
  CString csStateMoviePlayOnAttractBelt;
  CString *getMoviePlayOnAttract( void );	// return correct movie based on takeawaybelt or not
  bool fStateSystemFunctionsBtnOnLaneStatusScreen;	//Y/N - change the button location for Retailix 168378 LPM040601
  bool fStateEscalateTakeYourItemsTimeout;  //RFC 209702 
  bool fStatePaintScanAndBagOnInitialScan;  //TAR 262131
  bool fStateForceTrxStartOnSwipe;          //TAR 275219
  bool fOperationsReprintReceiptByTransNumber;	 //Print any receipt by trans number (Y or N)

  //Attendant mode options
  bool fStateAttndModeAllowed;      //Allow or disallow Attendant mode
  bool fStateAttndModeLoginBypass;  //Bypass intervention/storemode login
  bool fStateAttndModeImmedInterventions;  //Handle interventions immediately (Y or N)

  //Assist menu options
  bool fStateAssistMenuFromStoreModeAllowed; //Store mode assist goes to Assist menu
  bool fStateAssistMenuFromRAPAllowed;       //Rap assist button goes to Assist menu
  bool fStateStartupToLaneClosed;  //RFC 986
  bool fStateStartupWaitForTB;   //RFC 986
  
    bool fStateSecurityDisableAllowed; //SR683
    bool fStateSecurityDisableNoReset; //SR683
  
  CString csTracingFocusComps;        // default focus comp ids for tracing, TS,PS,MP,ST...
  int nTracingCurrentLevel;		      // default startup tracing level
  int nTracingFocusLevel;	          // default focus tracing level
  int nTracingMaxLogSize;	          // Maximum size of traces.log file (KB) 
  int nTracingEntriesToFlush;	      // Number of entries to flush to the file 
  bool fTracingTraceOn;                    // Trace Flag

  int nTracingPSAPIInterval;        // Number of seconds between timed PSAPI traces in core.
  bool fTracingPSAPITraceStateInit; // Whether or not to do PSAPI traces around SMState Init funcs.

	bool fTenderCashAllowed;		  // Cash tender is allowed
	bool fTenderCreditAllowed;		  // Credit tender is allowed
	bool fTenderDebitAllowed;		  // Debit tender is allowed
	bool fTenderCreditNeedsPIN;		  // Credit tender needs a PIN number
	bool fTenderDebitNeedsPIN;		  // Debit tender needs a PIN number
	bool fTenderCreditCashBack;		  // Credit tender allows cash back
	bool fTenderDebitCashBack;		  // Debit tender allows cash back
  bool fStateHasSidecar;			  // ATM has a sidecar
  bool fTenderShowUseCoupons;		  // Show the <Use Coupons> button at tender
  CString csTenderAllowCoupons;		  // this terminal accepts coupons in: A=All modes, O=Only in assist mode, N=No modes
  CString csTenderShowUseCoupons;

  int nTenderCashBack[MAX_TENDER_CASHBACK_VALUES];			  // cash back amounts 
  int nTenderCashBackNum;  // number of cash back amounts specified non-zero

  int nTenderEBTCashBack[MAX_TENDER_EBT_CASHBACK_VALUES];	// EBT cash back amounts
  int nTenderEBTCashBackNum;// number of EBT cash back amounts specified non-zero

  CString csTenderDenominations;	  // cash bill types i.e. ";100,500,1000,2000,5000,10000"

  // Add EBT tender support
  bool fTenderEBTFSAllowed;			  // EBT FS tender is allowed
  bool fTenderEBTFSNeedsPIN;		  // EBT FS tender needs a PIN number
//bool fTenderEBTFSCashBack;		  // EBT FS tender allows cash back no longer used
  bool fTenderEBTCBAllowed;			  // EBT CB tender is allowed
  bool fTenderEBTCBNeedsPIN;		  // EBT CB tender needs a PIN number
  bool fTenderEBTCBCashBack;		  // EBT CB tender allows cash back
  bool fTenderAllowCancelWithPartialEBTTender;	// EBT allows customer to cancel when partial payment is made with EBT

  bool fTenderOtherPayment;           // Other Payment 
  bool fTenderSubmitFSTtotalIfMorethanTrxTotal;    // true - submit FoodStampsDue to TBEnterTender if FoodStampsDue is more than lBalanceDue
  bool fTenderIsMotorizedMSR;		  // using Motorized MSR Y or N
  bool fTenderDispenseChangeAtLaneForRemoteTendering;   //For remote tendering transaction, dispense change at Lane side or pop RAP cash drawer at RAP side. The default value is Y

  //bool fTenderPIPPINEncryptorOnly;	  // If using Motorized MSR w/ PIP, and PIP is for entering PIN only future use

  // Enable Disable User Exit calls. Albertsons
  // nAllExitsEnabled = 1 then nXXExitEnabled must be = 1 to enable
  // exit XX.
  // Header in global file 000 (SCOTOPTS.000) or nnn file (SCOTOPTS.nnn) is
  //		[UserExits]
  bool fUserExitAllEnabled;		 // Allow Enable Disable of User Exits T/F

  // T = any User exit enabled will be on. This will allow
  // for all exits to be turned off with one flag and only
  // selected exits to be turned on.
  bool fUserExit00Enabled;		  // Enable exit 00
  bool fUserExit01Enabled;		  // Enable exit 01
  bool fUserExit02Enabled;		  // Enable exit 02
  bool fUserExit03Enabled;		  // Enable exit 03
  bool fUserExit04Enabled;		  // Enable exit 04
  bool fUserExit05Enabled;		  // Enable exit 05
  bool fUserExit06Enabled;		  // Enable exit 06
  bool fUserExit07Enabled;		  // Enable exit 07
  bool fUserExit08Enabled;		  // Enable exit 08
  bool fUserExit09Enabled;		  // Enable exit 09
  bool fUserExit10Enabled;		  // Enable exit 10
  bool fUserExit11Enabled;		  // Enable exit 11
  bool fUserExit12Enabled;		  // Enable exit 12
  bool fUserExit13Enabled;		  // Enable exit 13
  bool fUserExit14Enabled;		  // Enable exit 14
  bool fUserExit15Enabled;		  // Enable exit 15
  bool fUserExit16Enabled;		  // Enable exit 16
  bool fUserExit99Enabled;		  // Enable exit 99

  int nUserExitInt1;				// for use by stores to pass values to user exits
  int nUserExitInt2;
  int nUserExitInt3;
  int nUserExitInt4;
  int nUserExitInt5;

  CString csUserExitString1;		// for use by stores to pass Strings to user exits
  CString csUserExitString2;
  CString csUserExitString3;
  CString csUserExitString4;
  CString csUserExitString5;

  CString csConfigFilePath;			// config files path, initialized in the constructor of co from registry
  CString csDataMessageFile;        // Message file name
  CString csDataBitmapFile;         // Bitmap file name 
  CString csDataSASIActionFile;     // Security action file name
  CString csDataWaveFile;            // Wav file name
  CString csDataTenderFile;			// Tender file name
  CString csDataTareFile;			// Tare file name
//New UI
  CString csDataXMLFile;			// XML file name
//End New UI
  CString csDataEntryINIFile;		// DATAENTRY ini file
  ConfigObject();					// constructor
  long Initialize(const int);		// initialize, terminal number
  long TenderInitialize(const int);	// initialize tender options

  int getTimeOutSyncTimeout()         { return nTimeOutSyncTimeout; }
  int getTimeOutTender()			  { return (nTimeOutTender + nTimeOutSignoff); }
  int getTimeOutStateDefault()		  { return nTimeOutStateDefault; }
  int getTimeOutTakeCard()			  { return nTimeOutTakeCard; }
  int getTimeOutTakeCash()			  { return nTimeOutTakeCash; }
  int getTimeOutTakeCoins()			  { return nTimeOutTakeCoins; }
  int getTimeOutItemTransactionResult() { return nTimeOutItemTransactionResult; }
  int getTimeOutContinueTransaction() { return nTimeOutContinueTransaction; }
  int getTimeOutHelp()				  { return nTimeOutHelp; }
  int getTimeOutSignon()			  { return nTimeOutSignon; }
  int getTimeOutSignoff()			  { return nTimeOutSignoff; }
  int getTimeOutAttract()			  { return nTimeOutAttract; }
  int getTimeOutItemSaleInProgress()	  { return nTimeOutItemSaleInProgress; }
  int getTimeEASDeactivationScreenTimer() { return nTimeOutEASDeactivationScreenTimer; }
  int getTimeOutCouponSensorScreenTimer() { return nTimeOutCouponSensorScreenTimer; }
  int getTimeOutTenderBalanceScreenTimer() { return nTimeOutTenderBalanceScreenTimer; }
  int getTimeOutDepositGiftCardScreenTimer() { return nTimeOutDepositGiftCardScreenTimer; }
  int getTimeOutCashOutSensorAudioTimer() { return nTimeOutCashOutSensorAudioTimer; }
  int getTimeOutCheckBasketReminder() { return nTimeOutCheckBasketReminder; }
  int getTimeOutInitialBagItemAudio() { return nTimeOutInitialBagItemAudio; }
  int getTimeOutPeriodicBagItemAudio() { return nTimeOutPeriodicBagItemAudio; }
  int getTimeOutBagItemEscalation() { return nTimeOutBagItemEscalationMilliSecs; }
  int getTimeOutInitialRemovingItemsAudio() { return nTimeOutInitialRemovingItemsAudio; }
  int getTimeOutPeriodicRemovingItemsAudio() { return nTimeOutPeriodicRemovingItemsAudio; }
  int getTimeOutRemovingItemsEscalation() { return nTimeOutRemovingItemsEscalationMilliSecs; }
  int getTimeOutWalkAwayTimerAudio() { return nTimeOutWalkAwayTimerAudio; }
  int getTimeOutTakeYourItemEscalation() { return nTimeOutTakeYourItemEscalationMilliSecs; }  //RFC209702
  int getTimeOutInitialTakeYourItemAudio() { return nTimeOutInitialTakeYourItemAudio; }
  int getTimeOutRecurringTakeYourItemAudio() { return nTimeOutRecurringTakeYourItemAudio; } //RFC209702
  int getTimeOutSecurityServerTimer() { return nTimeOutSecurityServerTimer; }
  int getTimeOutThankYouTimerBelt() { return nTimeOutThankYouTimerBelt; }
  int getTimeOutInitialClearBagAreaAudio() { return nTimeOutInitialClearBagAreaAudio; }
  int getTimeOutPeriodicClearBagAreaAudio() { return nTimeOutPeriodicClearBagAreaAudio; }
  int getTimeOutTransportItemTimer()  { return nTimeOutTimeOutTransportItem; }
  int getTimeOutTouchScreenEnablesScanner()  { return nTimeOutTouchScreenProximityEnablesScannerMotor; }
  int getTimeOutMotionSensorOnAttract()  { return nTimeOutMotionSensorOnAttractInterval; } //RFC 265465
  int getTimeOutScannerScaleOnAttract()  { return nTimeOutScannerScaleOnAttractInterval; } //RFC 276235
  int getTimeOutDisplayBagItemScreenTimer()  { return nTimeOutDisplayBagItemScreenTimer; }
  int getTimeOutDelaySecurityNotification() { return nTimeOutDelaySecurityNotificationMilliSecs; }
  int getTimeOutPeriodicSecurityAudio() { return nTimeOutPeriodicSecurityAudio; }
  int getTimeOutHeartbeatTimer() {return nTimeOutHeartbeatTimer; } // RFC 262339
  int getTimeOutBusyLeaseTimer() {return nTimeOutBusyLeaseTimer;}  //RFC 263404
  bool getStateEscalateTakeYourItemsTimeout() { return fStateEscalateTakeYourItemsTimeout; } //RFC209702
  int getTimeOutAttendantModeTimer() {return nTimeOutAttendantModeTimer;}
  int getTimeOutGloryUnitDispense() { return nTimeOutGloryUnitDispenseTimeout; }
  int getTimeOutExitSuspendScreenTimer ()  { return nTimeOutExitSuspendScreenTimer; }		//RFC 294962
  int getTimeOutDisplayPutBagOnScaleScreenTimer()  { return nTimeOutDisplayPutBagOnScaleScreenTimer; }//SR085
  int getTimeOutDisplayIncreaseMisMatchScreenTimer() { return nTimeOutDisplayIncreaseMisMatchScreenTimer; }; //TAR379130/379131
  int getTimeOutEnableSkipBaggingButtonTimer() { return nTimeOutEnableSkipBaggingButtonTimer; } //SR778
  
	//BeginCodeLocale

	bool fLocaleIsCurrencySymbolOnEReceiptCol1;		// Currency Symbol display on the e-receipt Col1
	bool fLocaleIsCurrencySymbolOnEReceiptCol2;		// Currency Symbol display on the e-receipt Col2
	bool fLocaleIsTaxLine;		//set default value											 // Display Tax Line
  // Price Display
	bool fLocaleCurrencySettingNotFromRegionalSetting;	// Y/N Regional Setting (Control Panel)
	bool fLocaleDateSettingNotFromRegionalSetting;		// Y/N Regional Setting (Control Panel)
	// If fLocaleCurrencySettingNotFromRegionalSetting=Y, we overwrite with :
	CString csLocaleCurrencySymbol;		// CURRENCYFMT lpCurrencySymbol ($,TL)
	int nLocalePositiveCurrencyFormat;	// CURRENCYFMT PositiveOrder, LCTYPE LOCALE_ICURRENCY
	int nLocaleNegativeCurrencyFormat;	// CURRENCYFMT NegativeOrder, LCTYPE LOCALE_INEGCURR
	CString csLocaleDecimalSymbol;		// CURRENCYFMT lpDecimalSep
	int nLocaleNoOfDigitsAfterDecimal;	// CURRENCYFMT NumDigits
	CString csLocaleDigitGroupingSymbol;// CURRENCYFMT lpThousandSep
	int nLocaleNoOfDigitsInGroup;		// CURRENCYFMT Grouping
	int nLocaleLeadingZero;			    // CURRENCYFMT Leading Zero (not used)

	// if fLocaleDateSettingNotFromRegionalSetting=Y, we overwrite with:
	int nLocaleShortDateOrdering;	        // LOCALE_IDATE	  Short date format-ordering specifier
	int nLocaleNoCenturyDigit;		    // LOCALE_ICENTURY   Specifier for full 4-digit century.
	CString csLocaleDateSeparator;	    // LOCALE_SDATE	  Character(s) for the date separator.
	CString csLocaleShortDateFormat;      // LOCALE_SSHORTDATE Short date formatting string for this locale.

	// Others
	int nLocalePriceCentUnit;			// Cent Unit (=100 pour 2 decimal)
	CString csLocaleSIntlSymbol;		// International Currency Symbol (not used)
	CString csLocaleCashValueList;		// Possible Cash Value of hopper and cassette
	CString csLocaleCashSafeList;		// Minimum thresholds
	CString csLocaleCashLowList;		// Low thresholds for the green blinking
  bool fLocaleCashManagementScreen;     //+dp185016 support glory device
  bool fLocaleCashManagementDisabledInTransaction; //TAR 370108
  bool fLocaleAllowSoftwareUnload;
  bool fLocaleAllowSoftwareRemoval;
  bool fOperationsOperatorTwoLineText;
  bool fOperationsHopperSubstitutionIntervention;
  int nOperationsHopperFixLaterTrxLimit;
  bool fOperationsAllowDegradedMode;	   //SR742 
  CString csLocaleCashHighList;	
	CString csLocaleCashRemoveAmount;		
	CString csLocaleCashCapacity;		//-dp185016 capacity 
	CString csLocaleCashLowListForCoinHopper;
	CString csLocaleCashCapacityForCoinHopper;
	CString csLocaleCashValueListForCoinHopper;

  bool fLocaleCashManagementDisplayCount;        //+dp185016 generic cash management
  bool fLocaleCashManagementAPPTrackCashCount;   
  int nLocaleCashAcceptorCapacity;
  int nLocaleCoinAcceptorCapacity;
  int nLocaleCashAcceptorHighThreshold;
  int nLocaleCoinAcceptorHighThreshold;           //-dp185016
	CString csLocaleCashLowListForNewEnhancedCoinLowSensor; //For Enhanced Coin Low Sensor

  /* + SafePay cash management. */
  int nLocaleNoteCassetteLocation;
  bool fLocaleAllowNoteCoverRemoval;   
  bool fLocaleAllowNoteCassetteRemoval;   
  int nLocaleXferToCassetteMode;
  bool fLocaleAllowBillUnload;   
  /* - SafePay cash management. */

//TAR#133269 Removed hardcoded max currency number
//	#define MAX_LISTVALUE 8    
//	int nCashValueList [MAX_LISTVALUE];	// Possible Cash Value of hopper and cassette
//	int nCashSafeList [MAX_LISTVALUE];	// Minimum thresholds
//	int nCashLowList [MAX_LISTVALUE];	// Low thresholds for the green blinking
	CArray<int,int> nCashValueList;	// Possible Currency Values in the coin and bill hoppers
	CArray<int,int> nCashLowList;	// Low thresholds for the green blinking
	CArray<int,int> nCashHighList;	//+dp185016 support glory device // High thresholds 
	CArray<int,int> nCashCapacity;	
  CArray<int,int> nCashRemoveAmount;
  //-dp185016 capacity
	//+4.2 card dispenser 
    bool IsManageCardDispenser() const { return fOperationsManageCardDispenser ;}
	//returns card dispenser bin capacity
	long getCardDispenserBinCapacity() const  { return nOperationsCardDispenserBinCapacity ; }  
	//returns  card dispenser bin near full. default value is 15
	long getDispenserRejectBinNearFull() const { return nOperationsDispenserRejectBinNearFull ; } 
	//returns  card dispenser bin full. default value is 20
	long getDispenserRejectBinFull() const { return nOperationsDispenserRejectBinFull ; }
	//-4.2 card dispenser 
	//EndCodLocale

	CString csTemplateKeyboard;		    // Keyboard template (for the pick up list)
	CString csTemplateAssistMode;	        // Assist Mode template
	//Begin Tar 210104
	CString csTemplateAlphaNumeric;     //AlphaNumeric keyboard for job information
	//End Tar 210104
	CString csPrintObjectsChitBmp1, csPrintObjectsChitBmp2;

	// Consumer marketing or Reward information options
	bool fCMRewardInfo;					// main option to configure reward information display
	int  nCMCustomerMessage;			// customer name or any message from scotopts.dat or from TB
	CString  csCMCustomerMessage;			// customer name or any message from scotopts.dat or from TB
	bool fCMTotalSavings;				// option to show/hide total savings
	bool fCMTotalRewardPoints;			// option to show/hide total reward points
	bool fCMTotalSavingsOnLeft;			// option to show total savings on left side on reward_info bar

	CString csMachineName;				//ClientMachineName
	bool IsDegradedModeOn() const { return fOperationsAllowDegradedMode ;} //SR742
    bool fMultiLabelBarCodesPrependFirstPrefixToItemCode; //tar 266075

	// Begin In-Store Message Support Parameter Gets
	bool getGreenFlashingSendPage()	const	{ return fPagingGreenFlashingSendPage;}
	bool getYellowSendPage() const			{ return fPagingYellowSendPage;}
	bool getRedSendPage() const				{ return fPagingRedSendPage;}
	bool getOutOfServiceSendPage() const	{ return fPagingOutOfServiceSendPage;}
	int  getGreenFlashingElapseTime() const	{ return nPagingGreenFlashingElapseTime;}
	int  getYellowElapseTime() const		{ return nPagingYellowElapseTime;}
	int  getRedElapseTime() const			{ return nPagingRedElapseTime;}
	int  getOutOfServiceElapseTime() const	{ return nPagingOutOfServiceElapseTime;}
	int  getGreenFlashingLimitTime() const	{ return nPagingGreenFlashingLimitTime;}
	int  getGreenFlashingLimitTrans() const	{ return nPagingGreenFlashingLimitTrans;}
	int  getAvailabilityCount() const	    { return nPagingAvailabilityCount;} // TAR 117805
	CString getBitmapFile() const           { return csDataBitmapFile;}
//with dual languages, this isn't valid anymore
//	CString getMessageFile() const          { return csDataMessageFile;}
	CString getSASIActionFile() const       { return csDataSASIActionFile;}
	// End  In-Store Message Support Parameter Gets
	CString getRapInitiateTransaction() const	{ return csStateRapInitiateTransaction;}
  
    CString csPrimaryLanguageCode;
    SLanguageInfo language[SCOT_LANGUAGE_MAX];
    void LoadLanguageOptions(CWinApp *pApp); // pApp has SCOTOPTS.000/xyz profile
    void LoadCashValueLists(CString csValueList, CString csLowList, CString csHighList, CString csCapacity, CString csCashRemoveAmount); //dp185016 support glory device // High thresholds 
	SBmpPrintObject PrintBmpObjectList[MAX_PRINTOBJECTS];
	//TAR262985 moved to SmstateBase.h
	//static bool OpenSCOTFile(CString csFileName,bool bAbortOnOpenFail, bool bIsDefaultFile); // check for existence of any scot files
	long InitAllOptions(CString& csFileName);
	long InitTenderOptions(CString& csFileName, SCOT_LANGUAGE_TYPE language_used);
	void ScotoptsOverrideSCOTTend();
	
	CStringArray csaTransitionContextList; // SR730 RFC380486

    TenderButton TenderList[SCOT_LANGUAGE_MAX][MAXBUTTONNUM];   //New Finalization tender list
	void ScotoptsOverrideScottend(SCOT_LANGUAGE_TYPE language_used);
	bool bReadCreditNeedsPIN;  //Tar 165410
	bool bReadCreditCashBack;
	bool bReadDebitNeedsPIN;
	bool bReadDebitCashBack;
	bool bReadEBTFSNeedsPIN;
	bool bReadEBTCBNeedsPIN;
	bool bReadEBTCBCashBack;
	bool bReadDebitNeedsSignatureCapture;
	bool bReadDebitNeedsVerifyCapture;
	bool bReadSignatureCapture;
	bool bReadVerifyCapture;
	bool bReadCreditMSR;
	bool bReadDebitMSR;
	bool bReadDebitSavingsMSR;
	bool bReadDebitCheckingMSR;
	bool bReadEBTMSR;
	bool bReadFoodStampMSR;
	bool bReadGiftCardMSR;
	bool bReadLoyaltyMSR;
	bool bReadPIPSCOTPromptCardType;
	bool bReadPIPSCOTPromptCashBack;
	bool bReadPIPSCOTPromptConfirmTotal;
	int nTimeOutRequestSig;
    int getTimeOutRequestSig() { return nTimeOutRequestSig; }
	
   ////// AutoRun Config Options
    bool fAutoRunOn;
    bool fAutoRunBypassBagging;
    long nAutoRunNumberOfTransactionsToRun;
	bool fAutoRunRandomTimer;
    long nAutoRunAttractDelay;
    long nAutoRunItemPurchaseDelay;
    long nAutoRunNumberOfItemsToSell;
    long nAutoRunTotalCashToPay;
    CString csAutoRunItemCodeToSell;
    bool fAutoRunAssistModeOn;
    CString csAutoRunAssistKeyList;
    //////
    // whether to handle EAS Item active as a immediate or delayed or no intervention
	CString csOperationsHandleUnDeActivatedItemAs;
	CString csLocalMachineName;		// NN and BZ Local Machine name RFC 210177
	int		nMaxBarcodesLength;		// TAR 212888 BZ: the max barcodes length
    CString csBusinessDate;			// FastLane 3.1 Reporting Feature
	long lLGWTolerance;	//Fix the problem with learning light wt item when already existing wt entry in WLDB
	long nPasswordEntryKeypad;
    int nTemplatePasswordEntryKeypad;   // FZ 10102002, Alphanumberic Password
    int nTemplateUserIDEntryKeypad;   // FZ 10102002, Alphanumberic Password
	CString csStateRapInitiateTransaction;
    int m_iNumberOfLanguagesSupported;
	bool fUtilityExitButtonVisible; 
	CString csUtilityExitPassword;
	CString csUtilityExitID;
	CString csUtilityDiagID;
	CString csUtilityDiagPassword; 
	CString csUtilityRebootID;
	CString csUtilityRebootPassword;
	CString csUtilityShutdownID;
	CString csUtilityShutdownPassword; 
	CString csUtilityVolumeID;
	CString csUtilityVolumePassword;
    bool    fStateTrainingProgramButtonVisible;
    CString csUtilityTrainingDocument;
    CString csUtilityInternetExplorerEXE;
    CStringArray csaCouponlist;
    long lSaySecSleepVal; // Tar 222910: Changes to read SaySecurity Sleep value from Registry
    bool bIgnoreSleep; // Tar 222910: Changes to read SaySecurity Sleep value from Registry

	//+4.2 provide subtle on-screen volume control
	bool IsShowVolumeControl() const { return fOperationsShowVolumeControl ; }
	int GetVolumeControlIncrementLevel() const { return nOperationsVolumeControlIncrementLevel; }
	int GetVolumeControlTimer() const { return nTimeOutVolumeControlTimer; }
	//-4.2 provide subtle on-screen volume control
	//RFC 334227
	int GetSingleLanguageButtonAtAttractTimer() const {return nTimeOutSingleLanguageButtonAtAttractTimer;}//RFQ681
	bool isPrintDuplicateMsgonPrintLastReceipt() const { return fOperationsPrintDuplicateMsgonPrintLastReceipt; }
	CString csPicklistImagePath;
	CString csShrinkPicklistImageCMD;
// Dynamic configuration options
// The following options can change during runtime.
// They are defined in ConfigDynamic.cpp
public:
    void InitializeDynamicOptions();
    bool IsAttendantModeOn();
    void SetAttendantMode(bool bAttendantMode);
    bool IsLoginBypassOn();
    bool IsSecurityBypassOn();
    bool IsImmedInterventionOn();
    void SetAttendantID(CString csAttendantID);
    CString GetAttendantID();
    bool IsInAssistMenus();
    void SetInAssistMenus(bool bAssistMenus);
	void SetContextPrefixAM(CString csContextPrefixAM);
	CString GetContextPrefix();  //Return active context prefix
	CString GetContextPrefixAM() {return m_csContextPrefixAM;};
	void SetContextPrefixCustom(CString csContextPrefix);
	CString GetContextPrefixCustom() {return m_csContextPrefixCustom;};
    bool IsRemoteScannerConnected();
    void SetRemoteScannerConnected(bool bRemoteScannerConnected);

    void SetSecurityDisable(bool bValue);
    bool IsSecurityDisableOn();

	void InitCustomerBagAllowed(bool bIsTAB);	// TAR 393731
	
	void ParseTransitionContexts(void); // SR730 RFC380486

protected:
    CString m_csAttendantID; //operator who switched system to attendant mode
    bool m_bAttendantMode;
	bool m_bAssistMenus;
	CString m_csContextPrefixAM;
	CString m_csContextPrefixCustom;
    bool m_bRemoteScannerConnected;

    bool m_bSecurityDisable; //SR683
	
//SR81 begin
public:
	bool fOperationsImmediateInterventionForRestrictedItems;
	bool fOperationsImmediateInterventionForVisualVerifyItems;
//SR81 end

//+SR719
public:
	bool fOperationsVolumeControlMuteBypass;
//-SR719

	//+SR713
	bool fOperationsSellBagsAtFinalize;
	CString csOperationsSellBagsItemCode;
	CString csOperationsBagPrice;
	bool fOperationsRepeatSellBags;
	//-SR713

// End of Dynamic configuration options

};

#endif

