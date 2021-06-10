//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateBase.h
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
// AUTHOR:	Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMStateBase
#define _SMStateBase
#pragma warning(disable:4786)
#include "library.h"
#include "AuthenticationClientLibraryAPI.h"   //SR93 Biometrics support
// SSCOADK-2499 Sample SSF will not compile stand-alone
//              Use HeaderGenerator project to build tlh/tli headers if necessary.
//#import "C:\\scot\\controls\\vFormControlObject.dll" // SR875
#include <vFormControlObject.tlh>
#ifndef _CPPUNIT
#include "PipeConnection.h"
#else
#include "FakeItemObject.h"
#include "FakePipeConnection.h"
class CDescriptorList;      // BUG.  This should be inside TransBroker.h !!!
#include "TransBroker.h"    // Need TBRC

#include "TestMacros.h"

#include "SCOTAuxScreenList.h"

#include "DMTriColorLightConstants.h"
#include "devmgr.h"
#include "OposChan.h"
#include "PSProceduresConstants.h"      // Need KEYPADINVALID

#endif //_CPPUNIT
#include "GTObject.h"
#include "SMStateConstants.h"
#include "MobileConstants.h"

#include "ConfigObjectConstants.h"      // Need SCOT_LANGUAGE_TYPE

//#import <TelnetClientSupport.dll> no_namespace
#include <TelnetClientSupport.tlh>
#include "QueuedActionsContainer.h"

//USSF Start
#include <vector>
using namespace std;
#include "SMStateUSSFMacros.h"
//USSF End

class SMStateBase;		// TAR:242025
typedef SMStateBase* (*pfnSSFAnchorType) (int nStateID);	// TAR:242025

#include <map>


//USSF Start
class SMUSSFManager;
class SMUSSFManagerBase;
//USSF End

class CCMInterface; //SR700
class CDataNeededUtil;
class CQueuedActionShowMessageScreen;
class CQueuedActionGenericDelayedIntervention;

//////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMStateBase : public CObject
{
#ifndef _CPPUNIT
//USSF Start
  friend SMUSSFManagerBase;
  friend SMUSSFManager;
//USSF End
#endif
  friend CQueuedActionShowMessageScreen;
public:
  SMStateBase();
  static void PipeConnect();
  static bool SendPipeDisconnect();
  static bool SendPipeConnect();
  friend void WINAPI Callback(VARIANT vUser, GTO::GTConnectReceiveEvent tbcre, IDispatch *pObj);
  static bool SetPipeConnection(IPipeConnection *conn);
  static void ClearPipeConnection();
  static void HandlePLAResponseType(_bstr_t strResponse, IGTObject *pGenObj);
  static void HandleConfidenceResponse(IGTObject *pGenObj);
  static void HandlePicklistResponse(IGTObject *pGenObj);
  static void HandleHandInViewResponse(IGTObject *pGenObj);
  static void HandleHandInViewAfterRetryResponse(IGTObject *pGenObj);

  static void StartPLAResponseTimer(long);
  static void StartNewPLAResponseTimer();
  static void RestartPLAResponseTimer(long);
  static VOID CALLBACK PLAResponseTimedOut(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
  static void HandlePLAResponseTimedOut(UINT lEventID);
  static bool CheckToDropPLARequest(_bstr_t strRequestID);

  static bool fInMultiSelectPickList; 

  static long GetDWORDValue(const CString& key,const CString& value);



#ifndef _CPPUNIT

  static VCONTROLOBJECTLib::IFormPtr m_pVForm;// SR875   
  static bool IsReceiptUnicodeData(unsigned char* pData, DWORD len); // RFC 368222

  virtual BOOL SCOTDeleteFile(LPCTSTR lpSourceFileName);   //TAR 348946

  //for Print Last Receipt module
  // + TAR_325292 - These members are now made virtual
  virtual void SaveLastReceiptData(HFILE hFile, CString csFileName); 
  virtual void SaveLastReceiptData(CString csFileName, CString csData);
  virtual void PrintLastReceipt(CString csReceiptFileName);  
  // - TAR_325292

  
  bool FileExist(CString csFileName); 
  void RemoveFile(CString csFileName);
  //end of for Print Last Receipt module
  bool LaunchProgram(LPCTSTR szBatchFile, DWORD dwWaitTime, bool bKillIfHung);
#endif //_CPPUNIT
  static void SynchronizeStoreloginWithLaunchPad(LPCTSTR szMessage); //TAR 301706
#ifndef _CPPUNIT
  virtual SMStateBase * PerformEOD();
  static void TraceWtInfo(); //tar 193806
#endif
  virtual bool IsUsingCouponSensor();    //use coupon sensor option
#ifndef _CPPUNIT
  //TAR 269516 - Has app recieved shutdown message?
  static bool IsStopping() {return fStopSCOT; };

  //virtual SMStateBase *Initialize(void)=0;	// force override
#endif //_CPPUNIT
  virtual SMStateBase *Initialize(void){ return STATE_NULL;};	// force override
  virtual void  UnInitialize(void);
#ifndef _CPPUNIT
  virtual SMStateBase *TimedOut(void);

#endif //_CPPUNIT
  virtual bool	 customerMode()  { return false; }
  virtual bool	 storeMode()	 { return false; }
  virtual bool	 helpMode()	  { return false; }
  virtual bool	 assistMode()	  { return false; }
  virtual bool	 storeModeHelp() { return false; }
  virtual bool	 inProgress()	{ return false; }

#ifndef _CPPUNIT
  virtual SMStateBase *Parse(MessageElement*);	// same parse for all
  virtual SMStateBase *IdleTimer(const long);   // idle between messages
  SMStateBase * SMStateBase::ParseMobile(MessageElement *me);
#endif //_CPPUNIT
  static BEST	 getAnchorState()
 // virtual BEST	 getAnchorState()
	{ return anchorState; }

  static SMStateBase* createAnchorState();
#ifndef _CPPUNIT
  //virtual SMStateBase* createAnchorState();

  virtual CString  CleanUpMessage(LPCTSTR,   // message to cleanup
		const bool);	// is message from a 2x20
  virtual void GetTrxLimitString(const PSTEXT nTextID, CString& csMsgText);
#endif
  virtual bool   restrictedItemNotAllowed();
#ifndef _CPPUNIT
  virtual PSTEXT restrictedItemMessage();
  

#endif //_CPPUNIT
  // CopyFileFrom4690: returns
  //   -1 error, read or control data error, file not copied
  //   -2 warning, same file data/time/size, file not copied
  //   other -ve rc means file error from 4690 like 0x80104010 not found
  //   other >= 0, number of bytes copied
  virtual long CopyFileFromServer(LPCTSTR,	 // ...from 4690 file name
		LPCTSTR,												  // ...to local file name
		const bool);	// ...T/F T-copy always, F-copy if different
#ifndef _CPPUNIT
  virtual long CopyFileToServer(LPCTSTR,	   // ...file on NT to copy to 4690
		LPCTSTR);											  // ...file name to copy

  //-------------------------------------------------------------
  // Create system messages.  There are 3 levels of messages.
  // Level 1 has a continue button, level 2 a cancel button, and
  // level 3 has both.  There are 6 system messages.  1 thru 3
  // correspond to levels 1 thru 3.  Sys msgs 4 thru 6 correspond
  // to 1 thru 3, but they require an operator/password to see
  // them.  The default is for an operator/password to be entered,
  // so levels 1-3 create sys msgs 4-6 at this base level.  This
  // can be overridden for states that do no require an operator/
  // password to see the msg (such as Store Mode states, where an
  // operator/password has presumably already been entered.
  //-------------------------------------------------------------
  virtual SMStateBase* createLevel1Message( LPCTSTR szMsg,
											PSTEXT pstMsg,
											PSTEXT prompt,
											long   value,
											long   device,
											bool   checkHealth,
											bool displayHTML=false); //Sr672
  virtual SMStateBase* createLevel2Message( LPCTSTR szMsg,
											PSTEXT pstMsg,
											PSTEXT prompt,
											long   value,
											long   device,
											bool   checkHealth,
											bool displayHTML=false); //SR672
  virtual SMStateBase* createLevel3Message( LPCTSTR szMsg,
											PSTEXT pstMsg,
											PSTEXT prompt,
											long   value,
											long   device,
											bool   checkHealth,
											bool displayHTML=false); //SR672
  //Tar 124345
  //static bool bReadyForTakeReceipt; //Tar 126024
  //static bool bTrxClosed;

#endif // _CPPUNIT
    static bool ms_bIsCurrentItemFromPickList; // Current item is a PickList item

    static bool bIsFromHostOffline;  //from HostOffline state
    static bool fStateFreeze;   // stay in the current state T/F
    static bool fStateAssist;   // stay in the current state T/F
  static CString csLastNonHelpStateName; // for SMContextHelp
  static CString csLastNonHelpStateBMP; // for SMContextHelp
  //static bool	m_bSecViolationFromAttract;  //383748 // SSCOP-2756
  static int	 nLastNonHelpStateTimeOut;	// for SMContextHelp
  static bool	fLastNonHelpStateStoreMode;	// save for debug info
  //Tar 122722
  static bool bFinish;       // if hit finish and pay button
  static bool fSSWtFromAttract;			// TAR123172 only used if scan scale wt increase from attract screen LPM101499

#ifndef _CPPUNIT
  int	 nTimeOut;								  // seconds in this state
  virtual SMStateBase *DMdf_EASNoMotion();
#endif // _CPPUNIT
  static int nEscalationRAPTimeOut;     // RJF 5/28/02 RFC 202655 & 202656
  static bool bEASReEnableScanner;	// EAS event processed, OK to enable the scanner scale

  // Tar 126382
  static bool bSuspendFailure;  
  static bool bgAlreadySubmittedWithOverride; // submit item only once with override flag-on

	// Shared 'static' state variables
	// the 'static' type allows these to be persistent
	// for access by other derived states, see SMState.cpp
	// these are coded as protected data members of SMState to
	// restrict their visibility to the SMState and its
	// derived states
 
  //Security Maintenance Mode
  static bool bSecMaintMode;	// if in Security Maintenance Mode, scanning allowed
  static bool bAlreadyInSnB;		// Already in Scan and Bag - fix screen blinking
  static CString csSavedSecMessage; //saved security msg - fix screen blinking
  static bool bIsScaleBroken; //tar 183854
  static bool bIsSysMsgScreenState; //tar 185972
  static bool bIsFromVoidApproval; //if in Void Approval screen
  static bool bCancelTransApproval;	// TAR 298096 - to indicate cancel entire transaction
  static bool bQuietNeedSecApproval; //if need security approval when in quiet mode
  static bool bIsTransactionStarted; //Tar219278 CHU 110602: If TBStart() has called: set it to True
  static int nAdvanceUnexpectedIncreaseCnt; // 3.2 patch B - RFC 257450
  static int nAdvanceMismatchCnt;			// 3.2 patch B - RFC 257449
  static bool bInApprovalNeeded; //TAR 279092  
  static bool bLaneClosedTamperOut; //TAR 279371
  static bool fRAPIsInStoreMode;
  static SCOT_LANGUAGE_TYPE m_customerLanguage; //TAR210823 CHU make it public
  // NewUI moved these two flags here to be able to access it in PSProceduress
  static long lFoodStampsDue;	   // for EBT
  // NewUI
  static int nLastDay;				   // Last day run EOD
  static int nPreviousDay;				//The day before system date
  static int nPsxRedrawCount;			// PSX's redraw logic has change, app needs to keep track
  static bool m_bIsFromLaneClosed;	   // from LaneClosed state
  static bool m_bIsAttendantModeChanged;  //TAR371932 & 371918
  static bool bValidScannedOperator;  //TAR 397106
  static bool g_bPicklistAssistVoid;
  static bool bFromAssistMenu;	   // SSCOB-18178 SSCOADK-6286 Assist Menu to know if the System Functions screen came from there or not
  static bool bReturnToRestrictedItems; //SSCOADK-7224
#ifndef _CPPUNIT
  virtual SMStateBase *PushStateFifoStack(SMStateBase *);	// changed to public so MPProcedures.cpp can use it LPM072901  TAR176732
  virtual SMStateBase *OnCouponSensor(); //if received a coupon sensor event
  virtual SMStateBase *OnMotionCashOutSensor();      //if received a Motion cash trough sensor event
  virtual SMStateBase *OnNoMotionCashOutSensor();    //if received a No Motion cash trough sensor event
  virtual SMStateBase *GenerateTrainModeIntervention(void); // Generate Training Mode Intervention cs185088
  virtual bool InvalidCouponPrefix(CString&);      //TAR 269159        
#endif
  static bool OpenSCOTFile(CString csFileName,bool bAbortOnOpenFail, bool bIsDefaultFile); // check for existence of any scot files TAR262985
#ifndef _CPPUNIT
  virtual void SynchronizeTime(); 
#endif // _CPPUNIT
  virtual bool ShrinkPicklistImages();
  static CQueuedActionsContainer m_ActionQueue;  //queue of delayed actions for ScanAndBag
  static CQueuedActionsContainer m_GenericDelayedInterventions;  //Temporary until Generic Delayed implemented in RCM.

#ifndef _CPPUNIT
  virtual CString GetWideCharToMultiByte(const CString &csData);

#endif // _CPPUNIT
  static bool bDataNeededSecurity;//SR801
  static bool g_bSentCashBalancesToReportServer;

  static bool bPOSatmPerksPromotions;
#ifndef _CPPUNIT
#include "SMStateUEBase.h"
#include "SMStateHTBase.h"
#include "SMStateRPBase.h"
#endif //CPPUNIT
#include "SMStatePSBase.h"
#include "SMStateSABase.h"
#include "SMStateTBBase.h"
#include "SMStateCUBase.h"
#include "SMStateDMBase.h"
#include "SMStateRABase.h"
//USSF Start
#include "SMStateUSSFBase.h"
//USSF End

protected:

#ifndef _CPPUNIT
  static CPSX*  m_pAppPSX;          // NewUI - pointer to PSX object
#endif //_CPPUNIT
  static int    nLastMsgID;		   // last message element ID MP received
  static bool   fInATMTransaction; // ATM transaction to withdraw cash, defaults to false
  static bool   fInLoyaltyCardProcessing;  // whether TB has any loyalty card data to process
  static bool   fLoyaltyCardAwaitVerification;  // card swiped before trx start, awaiting verification
  static bool 	bTrxLimitExceed;   // defaults to false
  static bool	fLoyaltyCardTrx;   // defaults to false
  static int	nTrxReceiptLength;	// defaults to zero
  static int	nTrxJournalLength;	// defaults to zero
  static bool   bAlreadyPlayedWelcome; // play welcome wave file once for a transaction
  static bool   bAlreadyPlayedLCReminder; // play LC reminder wave file once for a transaction //Tar 200719 BZ 04/02/02
  static bool   fTrxVerif;		   // Transaction Verification
  static long   lLengthOfPreviousEchoData;
  static bool   fInSignedOn;	   // now signed on T/F What is this for????
  static bool   fInSignedOff;	   // now signed off T/F What is this for????
  static bool   bFromAssistMode;	   // Assist mode to know if the System Functions screen came from there or not

  static SCOT_LANGUAGE_TYPE  m_LanguageUsedPreviously;  //Language from previous transaction  

  //Begin_Suspend
  static bool   fInSuspendTrans;	// suspend whole transaction
  //End_Suspend
  static bool   fOtherPayment;    // Other Payment
  static bool   fInVoidTrans;	   // canceling entire order
  static bool   fDoCashBack;		// cashback was due T/F
  static bool   fDoOptions;		 // need to load options T/F
  static bool   fItemIsWaiting;	 // last item was scanned in Attract or FTU T/F
  static bool   fDoEOD;			  // need to run lane EOD
  static bool   fDoLogsTo4690;	  // need to copy log files to 4690 T/F  
  //  static bool   fApprovedForTobacco; // verified old enough for tobacco
  //  static bool   fApprovedForAlcohol; // verified old enough for alcohol
  static bool	fFromRequestSignature;	// disable timeout feature if come from signature needed screen
  static bool   fUnapprovedRestrictedItemFromCustomerMode;
  static bool   fApprovedForRestrictedItems; // verified old enough for all restricted items
  static bool   fAgeObtained;	   // obtained some age, old enough or not
  static int    nCustomerAge;	   // The age of the customer
  static bool   fRequestForHelp;		// customer asked for help
  static bool   fCantScanCoupon;		// customer can't scan a coupon
  static bool   fUnDeActivatedItem;     // Any Unintervened undeactivated items on takeaway belt
  static bool   fNewItemScannedSinceLastUnDeActivatedItem; 
  static bool   fBeyondFinishAndPayState;
  static bool   fHasEnteredFinishState;
  static bool   fTABSafetyCoverOpenApprovalPending;
  // BeginCodeWLDBSA11
  static bool		fInWLDBProcess;	 // waiting for WLDB to authorize again to enable Scanner
  static bool		fScannerRequested;  // this state request Scanner
  // EndCodeWLDBSA11
  static bool   waitingForCardRemoval;

  // remote support
  static bool	fPutSCOTinLC;	//T if SCOT receive Lane Closed message
  static bool	fStopSCOT;		//T if SCOT receive Stop SCOT message

  // Cancel Suspend transaction
  static bool	fCancelSuspend;
  static bool   bSayAudioAssistedTender; //Tar 353054

  static int	nLastList;		  // last lookup list selected
  static long   lItems;			 // # of line items in this trans
  static long   lRestrictedItems;   // # rest'd items in this trans
  static long   lVoidedItems;	   // # voided itesm in this trans
  static long   lCoupons;		   // # coupons in this trans
  static long   lDepositedCoupons;  // # coupons deposited
  static long   lUnapprovedRestrictedItems;	// # rest'd items need approval
//	static long   lUnapprovedAlcoholItems;	// # rest'd items need approval // Alcohol
  static long   lRegularYellowNeedingApproval; // items that turn on yellow light,
                                               // distinguished from lSAYellowNeedingApproval
  static long   lUnapprovedVoidedItems;	// # void items need approval
  static long   lUnapprovedSecurityAlerts; // # security alert need approval
  static long   lUnapprovedCoupons; // # coupons need approval
  static long   lExceptionItemsRequiringApproval; //Exception Items which require approval
  static long   lNotForSaleItems;   // # in this trans needing approval
  static long   lTimeRestrictedItems;  // # in this trans needing approval
  static long   lUnknownItems;	  // # in this trans needing approval
  static long	lUnknownItemsKeyed;	// # of unknown items keyed only
  static long   lPriceRequiredItems;
  static long   lRestrictedNotAllowedItems;
  static long   lVisualVerifyItems;
  static CMap <int, int, bool, bool> g_mapTransactionVisualVerify;
  static long   lUnapprovedVisualVerifyItems;
  static long   lUnapprovedHopperSubstitution;

  static long   lQuantityRestrictedItems;     // # of times user got quantity restricted message
  static long   lCoinVoucherApproval;   // Added for Reports function - customer specific use
  static long   lGiftCertificateApproval;  // Added for Reports function - customer specific use
  static long   lUserDefinedApproval1;  // Added for Reports function - customer specific use
  static long   lUserDefinedApproval2;  // Added for Reports function - customer specific use
  static long   lUserDefinedApproval3;  // Added for Reports function - customer specific use
  static long   lUserDefinedApproval4;  // Added for Reports function - customer specific use

  static long   lVoidQty;		   // qty of item to void
  static long   lVoidWgt;		   // weight of item to void
  static long   lVoidTcd;		   // tare code of item to void
  static long   lVoidPrc;		   // extended price of item to void
  static long   lVoidUPrc;		   // unit price of item to void
  static long   lVoidBarcodeType;   // scanned item label's type
  static bool   m_bIsVoidByScanning;  // flag to determine if the void was initiated with a scan or manually selected

  	//  static char   szChargeAccount[HC_ACCOUNTLEN+1];	// charge account
	//  static char   szChargeExpiry[5]; // credit card expiry data YYMM
  static CString csChargeAccount;   // charge account
  static CString csChargeExpiry;	// credit card expiry data YYMM
  
  static CString csItemSent;		// last item code we TRIED to sell

  static CString csLookupItemCode;	// last item code we TRIED to lookup
  static CString csOperator;		// operator ID for storemode
  static CString csPassword;		// password storemode
  static CString csCashierID; //dp185016 support glory device-Part 2
  static CString csOperatorStoreLoginBarcodeMask;    //FZ and NN RFC 18799320020730
  static CString csOperatorStoreLoginBarcodeSignature;//FZ and NN RFC 187993
  static int nOperatorStoreLoginBarcodeUseridLength; //FZ and NN RFC 187993
  static int nOperatorStoreLoginBarcodePasswordLength;  //FZ and NN RFC 187993

  static CStdioFile csfReceipt;	 // temporary receipt holding file
  static bool m_bReloadingOptions; //tar 352145


  // all these come from SMA
  // all are +ve amounts in cents
  // the tax due and balance due come directly from Smkt
  // tax and balance come together on the TAX & TOTAL display
  // the balance also comes on the tender paid display lines
  static long lTaxDue;			  // total taxes
  static long lChangeDue;		   // change due to customer in cents


  static long lBalanceDue;		  // balance due remaining in cents
  static long lCouponValue;		 // from B064 message
  static long lEBTAvailableBalance;	// for EBT
  static int nHopperTrxCount;
  // the total sale must be accumulated manually as a sum of
  // item sales To get the total, add lItemTotal plue lTaxDue
  static long lTotalSale;					  // total of items in sale

  // the total tender paid must be accumulated as the
  // sum of tenders
  static long lLastTender;				  // last tender amount
  static long lTotalPaid;					  // total accumulated tender in cents

  static CString csErrDescription;		//Error description
  static long lErrDescriptionLength;	 // length

  static int nPINTries;					  // Number of times customer entered a PIN

  static TBTENDERTYPE nTenderType;

  static bool fCashBack;
  static bool bSigCapInProgress;

  static bool bLaneClosed;		// true - lane closed, false - lane open
  static bool bEnterTenderState;		// true - Enter tender, false - Leave Tender 

  static bool fTenderPending;       // true - we are waiting for the response to a tender (RFC 262343)
  // 3.1 req 4.4.5 new store-mode flags to track GoBack button on delayed interventions

  static bool bStoreModeGoBack;
  static bool bStoreModeApprovals;	// indicates pending approvals
  static bool bForceApprovalBeforeSuspend; //TAR 364905

  static BEST nSellState;		   // last item sell state

  static SMStateBase *sStateFifo[MAX_FIFO_STACK];	// system message fifo stack for Attract
  static int aStateFifo;			// add at count
  static int uStateFifo;			// used at count


  static long lVoidedCoupons;			  // number of voided coupons
  static long lItemSalesAmountVoided; // total amount voided (positive)
  static long lCouponAmountVoided;	// total coupon amount voided (positive)
  static long lCouponsInAssist;     // customer tried to scan a coupon when csTenderAllowCoupons = O - goto approval needed at finish
  static bool IsEncryptorSetupFail;  // flag Y/N for EncryptorSetup Fail
  static long lTotalCouponAmount;   // total coupons
  static int nCountPossibleDuplicateCoupon;  //Count for how many duplicate coupons


  static CMap <CString, LPCTSTR, int, int> g_mapTransactionCoupon;
	/******cm150000: Added in Merge 2******/
//	static long lNormalCouponAmount;	// total normal coupons (Scanned or manual upc input)
//	static long lHighRiskCouponAmount;	// total high rish coupons (price input by user)
	/******cm150000: End Merge 2******/

  static bool bBadCard;            //TAR 193038: true: bad card(misread, bad track data, bad account number
  	//+SR832
	static bool bPrintReceipt; //1 - print receipt, 0 - don't print receipt
	static int nTenderCount; //count the tender type used/selected, more than 1 means mix tender
	static int nLowestPrintThresholdTenderVal; //lowest threshold tender value
	static int nLowestPrintThresholdTenderIndex; //lowest threshold tender index
	//-SR832
	//+SR Personalization Digital Receipt
    static bool bGenerateDigitalReceipt;
    static bool bDoneGenerateDigitalReceipt;
    //-SR Personalization Digital Receipt

  //TB
  static CString csLastDisplay;   //The last display data
  static CString csTBMessage2Scot; //thp
  static HFILE hReceiptFile;				// The handel of the receipt file
  static HFILE hJournalFile;			 // The handel of the Journal file
  static HFILE hSlipFile;				// The handel of the Slip file
#ifndef _CPPUNIT
  virtual void CutReceipt(void);	// Cut the receipt

  virtual void SaveSignatureDataToBitmap(LPCTSTR csBitmapFileName);
  virtual void SaveSigReceiptData(BSTR pSignatureData);   // Save Signature Receipt Data
  virtual void SaveReceiptData(void); 
#endif //_CPPUNIT
  static CString csBirthDate;	   // mmddyy for restricted items

  // remote approval
  static bool bDropoffRequired;
  static bool bDropoffApproved;
  static bool bSuspendRequested;
  static bool bSuspendApproved;
  static bool bApplicationErrorApprovalRequired;
	  
  static bool bPartialTenderWithFoodStamps;
  static bool bPressHelpOnWay;    //Tar 164037 

  // SafePay cash recycler.  If non-zero, this variable contains an error code that
  // may require special handling in SystemMessage.
  static long m_SafePayLastErrorCode;	

  // SafePay cash recycler.  If true, then show media status button in SystemMessage.
  static bool fItemScanned;	//SSCOP-3184 (SSCOP-3139)

#ifndef _CPPUNIT
  // Interface to external cash management module. 
  // TAR 450162:  Change from pointer to object.
  static CCMInterface m_cmMgr;   

  //static SCOT_LANGUAGE_TYPE m_customerLanguage; //TAR210823 CHU make it public

  CStdioFile filePrint;		//Status File


#endif //_CPPUNIT
  virtual SMStateBase *OnNeedData(CString szDescription, CString szView, CString szIniFile=_T(""), CString szSection=_T(""));
  virtual SMStateBase *GenerateRAPDataNeeded(CString sDescription, CString sView, CString sINI=_T(""), CString sSection=_T(""));
  virtual SMStateBase *HandleDataRequest(CString csData = _T(""));
  virtual bool HandleDataNeededTBAsync(CString csData);
#ifndef _CPPUNIT
  virtual SMStateBase *GPNotify(const GPEVT);
  virtual SMStateBase *PopStateFifoStack(void);
#endif
  // Are there any Items Sold or Voided items.
  virtual bool isAnySoldVoidedItems(void);
#ifndef _CPPUNIT
  virtual bool AreThereAnySoldVoidedItems(void);	//Fiscal Printer changes
#endif //_CPPUNIT
  virtual bool isBarcodeValidOperatorPassword(const CString csItemCode, CString *pcsOperatorID = NULL);

  virtual void ReadOperatorLoginBarcodeMask(CString OperatorLoginBarcodeMask);  //FZ20020730 RFC 187993
#ifndef _CPPUNIT
  virtual void InvalidOperatorLoginBarcodeMask(CString csOperatorLoginBarcodeMask); //FZ and NN RFC 187993
  virtual bool isRestrictedItemApprovalRequired(void);
  virtual bool isVisualItemApprovalRequired(void);
  virtual bool isSecurityApprovalRequired(void);
  virtual bool isCouponRatioApprovalRequired(const bool=false);
  virtual bool isVoidRatioApprovalRequired(const bool=false);
#endif
  virtual bool isAnyApprovalRequired(const bool=false);
#ifndef _CPPUNIT
  virtual void ResetAllTransactionValues(void);	// called at end of transaction
  virtual void ResetAllTenderValues(void);  // called after failed tender attempts
  virtual long PrintReceipt(HFILE, const bool=true);		// close and print receipt file,
  virtual long PrintJournal(HFILE);  // print and close journal file.
  virtual CString CustomerName(LPCTSTR trackData); 
  virtual CString GetAccountNum(LPCTSTR trackData); 
  virtual void UpdateDepositedCouponVars();
  virtual SMStateBase *DepositCouponsIfNecessary();
  virtual SMStateBase *SetISIPTimeOut(int nMilliSec);
  virtual int GetISIPTimeOut();
  virtual bool isAnyNonImmediateApprovalRequired(void); //SSCOP-4920

  virtual CString GetApprovalNeededString(void);
  virtual bool WriteSCOTStatusToFile(CString cStatus);	// Write string to Status File
  virtual bool OpenSCOTStatusFile(CString cFileName);	// Open Status File 
  // takeaway belt
  virtual SMStateBase* DMTakeawaySendScaleStatus();		// TakeawayBelt request for scale status
#endif //_CPPUNIT
  virtual SMStateBase* DMTakeawayAMExitDone();			// TakeawayBelt notifying done with assist mode
#ifndef _CPPUNIT
  virtual TenderButton &GetTenderButton(int iIndex = -1);
  virtual void SetTenderButton(int iIndex = -1);
  virtual int  AdjustTenderButtonIndex(TBTENDERTYPE nAMTender);

#endif //_CPPUNIT
  //Security Maintenance Mode
  static bool bSMMPreScanState;
  static long lSMMBagScaleEvent;
  static bool bSMMFailure;
  static bool bSMMFailureActionCancel;
  static bool bIsSMMitemTolPPWU;
  static bool bIsSMMItemAdded;
  static bool bIsSMMItemDeleted;
  static long lSMMglWtExp;
  static long lSMMglWtSubCk;
  static long lSMMglWtTol;
  static long lSMMglPPWUTol;
  static long lSMMitemWtExp;	
  static long lSMMitemWtSubCk;	
  static long lSMMitemWtTol;

  static bool bWLDBNotActive;	// TAR #142007 to allow SMM to work if WLDB not active
  static bool bOkToDisplayBagScaleInfo;
  static COleVariant defaultFont;
  static CString CustomerMessage;
  static long lTrxTotalSavings;
  static long lTrxTotalRewardPoints;
  static int nCustomMessageLastState;
  static long ms_lExcludedItems;        //number of item sold that are excluded from the displayed item count - Rfc 371075};

//private:
  int	      nTimeIdleMsec;	// time counted while idle in msec
  static int  nTimeAttractIdleMsec;	// time counted while idle in msec
  static int  nTimeSinceLastHeartbeat; // time since last heartbeat was sent
  static int  nISIPTimeOut;	// time counted while item sale is in progress
#ifndef _CPPUNIT
  SMStateBase* pStateAfterISIPTimeOut;
#endif //_CPPUNIT
  static BEST  anchorState; // a saved point for states to return to
  static BEST  TABstateBeforeTransport; //TAR221089 CHU 112502 saved so we go back to this state from Transport
#ifndef _CPPUNIT
  bool      m_bStatePopped;  // TAR #223705 to allow multiple SystemMessages on the stack
  static ItemDetails itemDetailOrig; //Item details struction for saving pre-update details
  static ItemDetails itemDetailNew;  //Item details structure for saving details for update;
  
#endif //_CPPUNIT
  static bool bCompletePrintAndCut; // TAR_325292
  static bool bReprintReceiptFailed;
  static bool fCashOutSensorBlocked; //TAR 317732

  static bool m_fPrintDuplicateTrailer;      //RFC 334227

	//+SR93 Biometrics support
	static CString g_csPassword;
	static CString g_csOperator;
	static bool g_bBiometricACLDevice;
	static bool g_bBiometricACLServer;
	static bool g_bFPValid;
	static bool g_bFPTouched;
	static bool g_bBiometricOpened;
	//-SR93 Biometrics support
    static CString m_csResourcePath;  //SSCOADK-261
  
public:
  DECLARE_DYNCREATE(SMStateBase) // MFC Runtime class/object information
#ifndef _CPPUNIT
 
  CString GetOperator() {return  csOperator;}
  CString GetPassword() {return csPassword;}
#endif // _CPPUNIT
  void SCDownload(int nTerminalNumber);  // TAR 287143
#ifndef _CPPUNIT
  //transfer these to RAProcedures
  /*static void addOperatorID(CString csRapName, CString csOpID);  //RFC2054
  static CString getOperatorID(CString csRapName);  //RFC2054
  static void addRAPValidHeadCashier(CString csRapName, bool csOpType);  //SR819 
  static bool getRAPValidHeadCashier(CString csRapName);   //SR819*/
#endif // _CPPUNIT
  bool COInitialize(int nTerminalNumber, bool bLocal = false);
  bool SCOTTENDInitialize(int nTerminalNumber, bool bLocal = false);
  virtual SMStateBase* ReloadOptions(void);
  virtual long DisplayAuxScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh = false, int nArgs = 0, ...);
  bool         m_bNeedToDisplayAuxScreen;
#ifndef _CPPUNIT
  virtual SMStateBase* VerifyBirthDate(void);		// called when enter key is pressed on authorization or restricted items 
  //TAR 192098 196908 196910 start
  virtual BOOL VerifyBirthDate(CString csFromAnyDOB, CString csoperatorID, CString cspassword);
  virtual long GetAge(CString csFromAnyDOB);
  //TAR 192098 196908 196910 end
  virtual SMStateBase* SAEndOfTransaction(void);	// Security issued a 161 so end this transaction  LPM030501 TAR164591
  virtual long ReloadSASIActions(void);	// during reload options, only reload the SASI actions file, not the entire OCX
#endif //_CPPUNIT
  virtual void SetNextGenUICurrentItem(CString = _T(""), CString = _T(""), bool = false, bool = false, int = 0);
  virtual SMStateBase* CheckForDATFile(bool bReloadOption);	 //awk.7.01 TAR 173990
  virtual void SetNextGenUIMode(void);
  virtual SMStateBase* OnMatchedWt(void);				// Smart Scale reported a matched weight
  virtual SMStateBase* OnWtIncreaseNotAllowed(void);	// Smart Scale reported an unmatched wt increase
  virtual SMStateBase* OnWtDecrease(void);				// Smart Scale reported an unmatched wt decrease
  virtual SMStateBase* OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual SMStateBase* OnSecurityScaleFailure(int nFailureType);	// Smart Scale reported a bag scale failure
  virtual SMStateBase* OnDBMgrFailure(long lStatusCode, long lStatusType);	//DB client reported a failure
  virtual void SendNextGenUIApplicationState(LPCTSTR lpNextStateName,
                                 bool pbOverride,
                                 LPCTSTR lpOverrideStateName);
#ifndef _CPPUNIT
  // Undeactivated item on the TakeAway belt (belt 2) detected. 
  virtual SMStateBase* OnUnDeActivatedTheftTag(void);
#endif //_CPPUNIT
  virtual SMStateBase* OnSkipBaggingAllowed(void);	///TAR 407200 Security Manager report Skip Bagging is allowed

  static long lEOT_LGW;
  static bool g_bMatchedWtWaitingOnEAS;
  static bool m_bEASHappened;		// cmal121002 a motion no motion pair occured after an item purchase tar 219883
  static bool bIsCoupon;	//TAR339619
  static int m_View;              // ID of the view to be displayed to RAP operator
  static CString g_csDataNeededHandler;
  static bool bSecurityTrustLevelAlert;
  static CString g_csSecureCamInterventionType;
  // Used to save the previous view, for example if (some device error happens while at 
  // price needed intervention then it will save m_PrevView as DATANEEDED_PRICEPROMPT 
  static int m_PrevView;          // ID of the previous view (if any) to be displayed to RAP operator
  static CString m_sButton;       // String of button clicked by RAP operator
  static CString m_sInput;        // Input received from RAP operator

  static int nSkipBaggingBtnPresses;		//the number of Skip Btn Button Press
  static int nItemRemovedBtnPresses;	//the number of Done Removing Button Press
  static int nBagBoxBtnPresses;
  static long g_lBaggingReq;				// global bagging required for each item
#ifndef _CPPUNIT
  virtual SMStateBase* OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase* OnRAPApproveAndUpdateBtn(void);
  virtual SMStateBase* OnRAPApproveNoUpdateBtn(void);
  virtual SMStateBase* OnRAPLightItemBtn(void);
  virtual SMStateBase* OnRAPItemNotBaggedBtn(void);
  virtual CString GetDescFromView(int nView);  //for Reporting Feature get description from view
  virtual CString GetCategory(DWORD m_nRpID);  //for Reporting Feature get category 
#endif //_CPPUNIT
  static bool bToldRAPMediaLow;		// Tell RAP about media status
  virtual CString getSCOTStateName(void);
  virtual void UpdateStateName(CString csStateName = EMPTY_STRING);
  static SMStateBase* createState(BEST newState);		// fixed multiple problems to be able to pass state to the creation without reseting anchor state LPM 3.0 Sec/App merge
#ifndef _CPPUNIT

  // TAR196659 PUSHING AND POPPING -  changes to use one stack per state LPM022502
  virtual SMStateBase* CkStateStackAndReturn(void);
  virtual void ClearStateStackEOT(void);
  virtual SMStateBase* OnDataBaseInfoComplete(void);
#endif //_CPPUNIT
  virtual SMStateBase *HandleDataNeeded(void);
  static SMStateBase* g_pDelay_TBExpectedStateChange;
  static SMStateBase* g_pDelay_WaitForApprovalStateChange; 
  static bool g_bGLOBALScanAndBagInitializeDone; // to not disable devices that have not been enabled yet when S&B initialized/uninitialized
  static bool g_bSnBScreenIsShown;		// sometimes can pop S&B but the global bAlreadyInSnB is wrong, need another flag
  static bool g_bgTrxVerif;		// for WaitForApproval delayed when creating it the 2nd time
  static bool g_bgVoidTrx;		// for WaitForApproval delayed when creating it the 2nd time
  static bool g_bgVerifySig;	// for WaitForApproval delayed when creating it the 2nd time
  static bool g_bgCheckCashChanger;  // Tar 256141
  static bool g_bAllowEnterRemoteAssistMode;  // for allowing entering Remote Assist Mode
  static bool g_bSafetyCoverClosed;		// TAB LPM
  static bool g_bZeroWtExpected;        // TAR197477 TAB
  static bool ms_bKeepRemoteDevices;
  static ENDORSE_STATE fInEndorsementState;

  static bool g_bAllowRemoteBtn1;				// RFC 209075 
  static bool g_bAssistModeDataNeeded;          // TAR 213074
#ifndef _CPPUNIT
  virtual SMStateBase *HandleMSRData(void);  //RFQ 2605 - MSR handling for Dataneeded
  virtual void Escalation2RAP(void){;}
  virtual SMStateBase* PSRemoteButton1(void);	// RFC 209075 RJF 073102
  virtual void ScanScaleAttract(void){;} //RFC 276235
  virtual void ScanScaleSB(void){;} //RFC 276235
  virtual void TABTransportClearedItem(void);	// consolidate dup lines into one procedure LPM090502
  virtual void TABReEnableScannerAndClearWaitForApprovalState(void); //Re-enable the scanner and clear wait for approval state
  ITelnetClientIFPtr m_pTelnetIF;//FastLane 3.1 Reporting telnet client 
#endif //_CPPUNIT
  virtual int GetButtonIndex(int nTenderType);
  virtual void SendNextGenUITenderAnimation(int iIndex, int nTenderType);
  static CString SMStateBase::g_csSavedDeptInfo; // SR 141 - RFC 283384  static bool m_bMotionSensorThreadRun;			// RFC 219283 RF/DE 112002
  static bool m_bMotionSensorThreadRun;			// RFC 219283 RF/DE 112002
  static CWinThread *m_hMotionSensorThread;
  static int m_MotionEnablesScannerInterval;
  static int m_MotionSensorOnAttractInterval;   //RFC 265465
  static bool m_bMotionSensorEnabledOnAttract; //RFC 265465
  static long g_lUseDefaultTolerance;		// for Std Dev, will need to know if WtInfo determines to use the default tol or exception tol for SMM LPM120402
  static int m_iTolUsedflag;				// poly-state flag indicating tolerance used: 1 = Std Deviation; 2 = Default; 3 = Maximum was used (not good) - for tracing purposes in the App

  //SR742+
  virtual bool IsCashDeviceError(long lDeviceClass);
  virtual bool IsCardDeviceError(long lDeviceClass);
  virtual SMStateBase *EnterDegradedMode(DEGRADEDMODES lModeType); 
  virtual SMStateBase *ExitDegradedMode();  
  //SR742-
  static bool bTenderLimitExceeded;
#ifndef _CPPUNIT
  virtual void SetStatePopped( bool bPopped );
  virtual bool GetStatePopped();
#endif //_CPPUNIT
  static long SMStateBase::g_lDisplayFormRC;  // TAR 219971, 220074, 208118 BZ
  virtual BEST getNextStateForGoodItem();	  // 229084 created a Base function for getNextStateForGoodItem LPM022503
  static long m_lWtComparisonReq;			  // 229344 global wt comparison/substitution check flag for each item false means no comparison & no WLDB updates LPM022703 CMal
#ifndef _CPPUNIT
  virtual bool AreAnyRedApprovalsPending();   // Tar 211746 RJF function to check for all approvals that should turn light red
#endif //_CPPUNIT
  static long m_lSavedCashChangerStatusCode;  // TAR225069  
  static bool fStartTrxInAM;                  // RFC 228102 RF 060503 initiate transaction from RAP
  static bool m_bHasSeenTakeReceipt;          // TAR239622 CHU
  static bool m_bScanOnAttractAtQuietMode;    //TAR234536 CHU 080403
  static CString g_csAssistModeStateName;     // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
  static pfnSSFAnchorType m_pfnSSFCreateAnchor;	// added for SSF implementations  // TAR:242025
#ifndef _CPPUNIT
  void SendCurrencyAcceptedToRAP(long lAcceptedAmount);
#endif //_CPPUNIT
  static bool bDoNotResetTimeOutOnDMEvent;	  // TAR 260288
#ifndef _CPPUNIT
  virtual SMStateBase* HostOffLine(void);
  virtual SMStateBase* HostOnLine(void);
#endif //_CPPUNIT
  static bool m_bReportServerOnline ; // RFC 262343
  virtual SMStateBase* OnTransactionComplete(void);
  virtual void OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation);
  virtual void OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation, LPCTSTR lpszCategory);
  virtual void TellApplicationStateToSecurity(LPCTSTR lpszOperation, LPCTSTR lpszStateName);
#ifndef _CPPUNIT
  virtual long ChangeSecurityMaintenanceWtFlags(LPCTSTR lpszOperation, LPCTSTR lpszUPC,LPCTSTR lpszFlag, bool bPriceEmbedded);
  virtual long ChangeSecurityMaintenanceTolerance(LPCTSTR lpszOperation, LPCTSTR lpszUPC, LPCTSTR lpszTolerance, bool bPriceEmbedded);
  virtual SMStateBase* ReturnDelayedStateForGCTender(); //check trxend and return appropriate state
#endif //_CPPUNIT
  static bool	m_bTrxEnded;							//already entered TB_TRXEND
  static SMStateBase* m_pSavedStateInTendAccepted;		//save the state after tender accepted
  virtual SMStateBase* TareWeightedItem(void);
  virtual SMStateBase* TareOnly(void);
  virtual SMStateBase* TareWLDBItem(void);
  virtual SMStateBase* TareNone(void);
  virtual SMStateBase* TareOnlyAtAttract(void);//SR085
  static bool g_bBackToBackDataNeeded;  // TAR 265143
  static bool g_bClearOccurred;  // TAR 265143
  static bool g_bFromRAP;  // TAR 265143
  virtual void SetSecurityConfig(LPCTSTR lpszParamName, LPCTSTR lpszParamValue);
  static bool g_bDelayedSecIntervention;
  static bool g_bClearAllDelayedSecIntervention;
  static long m_lAgerestrictQueuedtoRAP;       //TAR 196910
  static bool g_bIsForgiveUnexpectedDecreaseDuringTendering;
  static CString g_csTareWt1;	// SR682
  static CString g_csTareWt2;	// SR682
  static CString g_csTareWt3;	// SR714
  virtual bool GetSecurityConfig(LPCTSTR lpszParamName);
  virtual CString GetSecurityConfigValue(LPCTSTR lpszParamName); // SR682
#ifndef _CPPUNIT
  virtual bool IsLoginBypassAllowed() { return true; };
#endif //_CPPUNIT
  static bool m_bSignaturePrintingStatus;      //TAR 195179 & 191490 & 224378
  static long m_lItemsScanned;	               //TAR 04/30/02 BZ
  static bool g_bOnItemOK;			// TAR 298537 Use for TAB only
  static long g_lNumberOfItemsNotBagInMultiPick;			// Multipick Enhancement
  static bool g_bOnScannerEnable;	// TAR 298537 Use for TAB only
  virtual void   setAnchorState(BEST state);
  virtual SMStateBase* setAndCreateAnchorState(BEST state);
#ifndef _CPPUNIT

  virtual void DeliverAnchorState(SMStateBase *StatePtr);
#endif //_CPPUNIT
  virtual void DeliverUpdate(BEST state);
#ifndef _CPPUNIT
  virtual bool IgnoreTBMessageInSecurityState();
#endif // _CPPUNIT
  static bool m_bCurrentIntervention_MisMatch;     //TAR340654
  static bool m_bCurrentIntervention_UnexIncrease; //TAR340654
  static bool m_bCurrentIntervention_UnexDecrease; //TAR340654
  static bool m_bCurrentIntervention_DontBagThreshold; //TAR433737
  static bool g_bReadyForItem;						//TAR 363023
#ifndef _CPPUNIT
  virtual bool IsForceDelayedApprovalBeforeSuspend();
  static FT_ACTION getRegisterAction(FT_STATUS_PT pStatus, void *pParam);  //SR93 Biometrics
  long GetDisplayedItemCount();		// Rfc 371075
  virtual SMStateBase *CouponMaxIntervention();			//SR678	Generate Intervention for Coupon that exceed     maximum limit
  virtual SMStateBase *CouponMaxInterventionFinal();	//SR678 Generate Intervention for Coupon that exceed maximum limit

  virtual SMStateBase *PreTenderState(void);			
  virtual bool IsRemoteErrorAllowed(long lDeviceClass); //TAR408719
#endif // _CPPUNIT
  virtual void StartTimerForSkipBaggingBtn(); // SR778 //427044/427406
  static bool g_bInterventionApproval; //TAR 371932 & tar 371918
  static bool bIsCouponExceedLimitFinalization;			//SR678
  static bool bIsMaxCouponLimitInterventionDone;
  //static bool bInAssistMenuValidHeadCashier;   //SR819: Removing this flag. Implementation changed.  SSCOP-366 & 368  
  //+SR713
  static bool bDoNotShowSellBags;								
  static bool bIsInPreTenderState;
  //-SR713
  static bool g_bSkipBaggingButtonTimerExpires; //SR778
  static BOOL g_bValidHeadCashier;
  static bool g_bMultiPickSkipBaggingAllowed; // TAR 398289
  static BEST g_savedAnchorStateFromAttract; //416582
  //+ Personalization picklist
  static bool g_bHavePicklist; //pick item on picklist state
  static bool g_bHaveSoldFromPicklist; //item sold from picklist
  static CStringArray g_csPicklistItemSoldInfo;
  static CString g_csPreviousPicklist;//save the picklist from personalization server
  //- Personalization picklist
  //+ Dynamic Quickpick
  static CStringArray g_csQuickPickItemSold;
  static bool g_bHaveQuickPick; //pick item on quickpick state
  static bool g_bHaveSoldFromQuickPick; //item sold from quickpick
  static CString g_csPreviousPersonalPicklist;
  //- Dynamic Quickpick
  static bool g_bIsSecurityDisableSubCheckOnly; //SR814.4
  static bool m_bEnableSkipBaggingButtonTimer; //427044/427406
  static bool fItemSalePending; //tar 440596
  static int iItemUPCtype; //tar 440596
  static CString csPendingItemCode; //tar 440596
  static bool ms_bLaneClosedFromRapFlag; //TAR 450342
  static bool Is5XMiniHardware(); // SSCOP-762 To check if hardware used is 5xMini

  static SMStateBase* g_pRememberStateBeforeHelp; //SR879
#ifndef _CPPUNIT
  virtual void SetStateBeforeHelp(CString &csLastName, CString &csNextStateName);//Sr879
  virtual SMStateBase *ProcessTBTender(TBTENDERTYPE nTenderType);      
  virtual void SetPSContext();
#endif //_CPPUNIT
  static CString g_csStateBeforeLookUp; //SR841
  // (+) SR797 
  static TBTENDERTYPE m_nTenderSelected;
  // (-) SR797 

  static bool g_bAllowLateBagging; //SR078
  static bool g_bNoBaggingRequiredForTheCurrentItem; //SR078
  static CString csLastStateName;//SSCOP-1439

  static bool ms_bThemeReload; // SSCOP-1158 Flag for delayed theme reload used when there is ongoing transaction

  static BEST nStateBeforeInvalidBarcode;
  static bool g_bCustomerLanguageChanged; //SSCOADK-5989

// (+) SR898 ====================
  
public:
  virtual void DMSetTriColorLight( DMTriColorLightColor color, DMTriColorLightState state, DMTriColorLightRequestType request );
#ifndef _CPPUNIT
  virtual void DMTurnOffTriColorLight( void );
// (-) SR898 ====================
#endif //CPPUNIT
  virtual void ShowDoNotBagOnEReceipt(void);//SR078

#ifndef _CPPUNIT
  virtual SMStateBase* SMStateBase::ProcessQueuedActions();
#endif //CPPUNIT
  virtual void loadOptionsFromConfigObject();
  virtual SMStateBase *GenerateTrustLevelDataNeeded(void);
  virtual void SendPLAStartPickList();

  static bool m_bWaitForPicklist;
  static IPipeConnection *m_PipeConnection;	//SSCOADK-265
  static CString g_csPLAItemList;
  static CString g_csPLAItemCode;
  static long g_lQty;
  static long g_lPLAResponseTimerID;
  static long g_lPLAPickListLatestRequestID;
  static CMap <UINT, UINT, bool, bool> g_mapPLAResponseTimer;
  static bool m_bIsPLALookUpTransition;
  static bool g_bInterventionsOnStoreModeEntry;
  static bool g_bMobileAppEnabled;
  static bool g_bInRecallMode;
  static bool g_bEnteringRecallMode;
  //static bool g_bRecallItemSold;
  static long ms_lMobileShopperItemCount;
//#endif //_SSCOMOBILE

  virtual void SMStateBase::QueueDelayedIntervention( CQueuedActionGenericDelayedIntervention* pDelayedIntervention);
  virtual SMStateBase *OnGenericDelayedIntervention(CDataNeededUtil &dataNeededUtil);
  virtual SMStateBase *OnGenericDelayedComplete(void);
  virtual bool isGenericDelayedInterventionApprovalRequired(void);
  static GENERIC_DELAYED_STATUS m_GenericDelayedInterventionStatus;


  virtual bool isProcessingGenericDelayedIntervention();
  virtual bool SetItemGDAsyncIDReceiptVariable(const CString& csReceipt, CString& csItemID, CString& csGDAsyncID);
  virtual bool isItemGenericDelayed(CString csItemID, CString &csGenDataNeededID);
  virtual void ClearCurrentGenericDelayedIntervention();

  static IQueuedAction *m_csCurrentIntervention;
  virtual bool RAPAssistModeAllowed();

protected:
  virtual void InitializeSSCOMobile();
  virtual void UnInitializeSSCOMobile();
  virtual SMStateBase *SellRecalledItem();
  virtual int GetUnsolictedDataBarcodeType(CString csData);
  virtual void SetInRecalledModeReceiptMessage(PSTEXTMSG nReceiptMsg, PSTEXTMSG nInstructionText);

};

#endif

