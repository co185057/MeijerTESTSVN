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
// these are not real states, just flags
#define STATE_NULL	 ((SMStateBase *)(1))
#define STATE_STOP	 ((SMStateBase *)(2))
#define STATE_RETURN   ((SMStateBase *)(3))// return to previous state, skip InProgress
#define STATE_RESUME   ((SMStateBase *)(4))// return to previous state, no skip InProgress
#define STATE_RESTART  ((SMStateBase *)(5))
#define STATE_ENDED	((SMStateBase *)(6))
#define STATE_ERROR	((SMStateBase *)(7))
#define EASEnable		2					// enable the EAS Deactivator
#define FULL_CRATE_QTY  10					// Quantity of a full crate in the emulator
#define DEFAULT_BEHAVIOR	-1				// -1 is used by Security to denote that the default behavior should be used LPM

#define TIMEOUT_SECURITY_WAIT_AT_THANKYOU 100 // waiting time allowed to recieve a 161xx conclution

#define MAX_FIFO_STACK		10
/*//+dp185016 tar 287778 remove these
#define SCOTINTRANSACTION _T("SCOT is in the middle of a transaction.\n")
#define SCOTNOTINTRANSACTION _T("SCOT is not in the middle of a transaction.\n")
#define CURRENTSTATE _T("Current State is ")
*///-dp185016
#define SLIP_FILE _T("%DATA_DRIVE%\\Scot\\Data\\SlipData.dat")  //for Print Last Receipt module
#define LRECEIPT_FILE _T("%DATA_DRIVE%\\Scot\\Data\\LastReceiptData.dat")  //for Print Last Receipt module
#define LRECEIPT_TMP_FILE _T("%DATA_DRIVE%\\Scot\\Data\\TmpLastReceiptData.dat")  //for Print Last Receipt module
#define GETTEXTOUTPUT _T("%s (Language ID %s):\n%s") //dp185016 rfc 265463
//#import <TelnetClientSupport.dll> no_namespace
#include <TelnetClientSupport.tlh>

#ifdef _SCOTSSF_
  #define DLLMODE  DLLEXPORT
#else
  #define DLLMODE  DLLIMPORT
#endif

//USSF Start
#include <vector>
using namespace std;
#include "SMStateUSSFMacros.h"
//USSF End

#ifdef _BUILDLIB_
  #define SMSTATE(a,b)			SM##a##b
  #define STATE(a)				SMSTATE(a,Base)
  #define RETURNSTATE(a)		 {							\
		CRuntimeClass* p##a = RUNTIME_CLASS(SM##a##Base);	\
		CObject* po##a = p##a->CreateObject();				\
		return (SMStateBase*)po##a;}								
  #define CREATE_AND_DISPENSE(a)							\
		CRuntimeClass* p##a = RUNTIME_CLASS(SM##a##Base);	\
		CObject* po##a = p##a->CreateObject();				\
		STATE(a)* ClassPtr = (STATE(a)*) po##a;				\
        return ClassPtr->Deliver
  #define CREATE_AND_ASSIGN(a,b)							\
		CRuntimeClass* p##a = RUNTIME_CLASS(SM##a##Base);	\
		CObject* po##a = p##a->CreateObject();				\
		b = (SMStateBase*)((STATE(a)*) po##a)->Deliver
#else
  #define STATE(a)				SM##a
  #define RETURNSTATE(a)          {						\
		CRuntimeClass* p##a = RUNTIME_CLASS(SM##a);		\
		CObject* po##a = p##a->CreateObject();			\
		return (SMStateBase*)po##a;}								
  #define CREATE_AND_DISPENSE(a)						\
		CRuntimeClass* p##a = RUNTIME_CLASS(SM##a);		\
		CObject* po##a = p##a->CreateObject();			\
		STATE(a)* ClassPtr = (STATE(a)*) po##a;			\
        return ClassPtr->Deliver
  #define CREATE_AND_ASSIGN(a,b)						\
		CRuntimeClass* p##a = RUNTIME_CLASS(SM##a);		\
		CObject* po##a = p##a->CreateObject();			\
		b = (SMStateBase*)((STATE(a)*) po##a)->Deliver	
#endif

typedef enum {
   ENDORSE_NONE,
   ENDORSE_INPROGRESS,
   ENDORSE_WAITING
} ENDORSE_STATE;

typedef enum {
    SCOTAPP_NORMAL=1,
    MEM_USAGE,
    RAP_ESCALATION,             // RFC 202655 & 202656 - separate escalation timer from audio
	ITEM_REMOVE_VIOLATION,
	UNEXPECTED_INCREASE,		// 3.2 patch B - RFC 257450
	MISMATCH,					// 3.2 patch B - RFC 257449
	ATTENDANT_MODE,
	SCANSCALEATTRACT,			// 3.2 patch D - RFC 276235
	SCANSCALESB,				// 3.2 patch D - RFC 276235
	VOLUME_CONTROL,				// 4.2 provide subtle on-screen volume control
	LANGUAGE_CONTROL,           // RFQ SR681
	SHOWSCREEN,                 //TAR379130/379131
	SKIPBAG_ENABLE,				//SR778

	MAX_TIMER					// 3.2 patch C - RFC 255737 - Please add all base timer ids above this timer id
} SCOTAPP_TIMERS;

// base entry states that can be returned to
#define BEST int
class SMStateBase;		// TAR:242025
typedef SMStateBase* (*pfnSSFAnchorType) (int nStateID);	// TAR:242025


// All methods herein return a STATE_NULL or a new state.
// STATE_NULL tells the caller to stay in the current state.
// A new state tells the caller to change to that new state.

// Provide traps for all known inputs

// These base methods should not normally be called
// if they are called an error in logic has occured.
// base state definition, has a virtual handler for all
// possible input sources and conditions

// base entry states that can be returned to within SCOTApp (SSF list of base entry states can be in SCUDefs.h)
 const enum BESTLIST
{
  BES_STATERETURN = -1,
  BES_START,
  BES_SCANANDBAG,
  BES_SELECTFROMLIST,
  BES_ENTERCOUPONS,
  BES_EMPTY_STATE,               //TAR221089 CHU 112502 since nobody uses it, TAB is used now 
  BES_LOOKUPITEM,
  BES_FINISH,
  BES_TAKECARD,
  BES_SIGNCHARGE,
  BES_SELECTCARDTYPE,
  BES_THANKYOU,
  BES_SELECTCONTAINER,
  BES_HELPSCREEN,
  BES_CARDMISREAD,				  //TAR119740	LPM100199  must return to system msg after call for help
  BES_SELECTTARE,				  //TAR125623  return to select tare screen after call for help 
  BES_OUTOFSERVICE,				  //TAR123447 return to out of service, in case of call for help after init error
  BES_COUPONNOTALLOWED,			  //TAR102019	LPM120799  must return to Coupon Not Allowed screen after call for help
  BES_KEYINCODE,
  BES_REQUESTSIG,
  BES_ENTERQUANTITY,
  BES_ENTERPIN,                   //TAR137419 return to enter pin screen after help
  BES_PRODUCEFAVORITES,
  BES_ENTERWEIGHT,
  BES_ENTERWTFORPRICEEMBEDDED,      //Tar 154147
  BES_CRATEABLEITEM,
  BES_SMMEDIASTATUS,
  BES_SMSYSTEMFUNCTIONS,
  BES_COUPONTOOHIGH,               // TAR 164015
  BES_COUPONNOMATCH,				//TAR 265390
  BES_INSERTCOUPON,                // Insert Coupon Anchor state
  BES_ENTERCOUPONVALUE,            // TAR 164014
  BES_BAGANDEAS,				   //T.I.D
  BES_SECVIOLATION,				   //T.I.D
  BES_PICKINGUPITEMS,			   //T.I.D
  BES_SECMISMATCHWEIGHT,		   //T.I.D
  BES_SECUNEXPECTEDDECREASE,
  BES_VOIDAPPROVAL,
  BES_SECUNEXPECTEDINCREASE,
  BES_ACKNOWLEDGESIG,
  BES_CARDPROCESSING,
  BES_CONFIRMSIGNATURE,
  BES_GETFOODSTAMPSTATE,
  BES_TAKERECEIPT,
  BES_SETALLITEMSASIDE,
  BES_VOIDTRANSAPPROVAL,
  /******cm150000: Added In Merged Code******/
  BES_OUTOFSERVICE2,
  BES_LANECLOSED,
  /******cm150000: End Merged Code***********/
  BES_HOSTOFFLINE,
  BES_SECBAGBOXTHRESHOLD,
  BES_SECITEMREMOVEDTHRESHOLD,
  BES_SECNEWITEMENTRY,
  BES_SECSKIPBAGGINGTHRESHOLD,
  BES_VOIDITEM,
  BES_VOIDNOMATCH,
  BES_CONFIRMVOID,
  BES_CASHPAYMENT,
  BES_CHECKBASKET,
  BES_DEPOSITCOUPONS,
  BES_DROPOFFCOUPONS,
  BES_TRANSPORTITEM,
  BES_SECBAGAREABACKUP,
  BES_CONFIRMABORT,
  BES_WAITFORAPPROVAL,
  BES_WAITFORREMOTEASSIST,
  BES_SYSMSGOPPASS,
  BES_SECURITYLEVEL3,
  BES_RAPDATANEEDED,
  BES_ENTERALPHANUMERICINFO,
  BES_UNDEACTIVATEDITEM,
  BES_ITEMNOTFORSALE,
  BES_UNKNOWNITEM,
  BES_TIMERESTRICTEDITEM,
  BES_ITEMQUANTITYEXCEEDED,
  BES_CMDATAENTRY,
  BES_ASSISTEDTENDER,
  BES_STOREAUTHORIZATION,
  BES_SMDATAENTRY,
  BES_VOUCHERPAYMENT,
  BES_TENDERBALANCE,
  BES_DEPOSITGIFTCARD,
  BES_INSERTGIFTCARD,
  BES_COLLECTGIFTCARD,
  BES_EXTENSIONSTATE,
  BES_LANGUAGESELECT,
  BES_CUSTOMMESSAGE,	//TAR 277900 Migrated-BM185018
  BES_SMPRINTRECEIPT,
//USSF Start
  BES_UNIVERSALSSF,
//USSF End
  BES_PUTBAGONSCALE, //SR085 - new state added for SR085
  BES_SECBAGBOXNOTVALID, //SR085 - new state added for SR085
  BES_SELLBAGS,		//SR713
  BES_DEGRADEDCONTINUETRANS,	//degraded mode tars
  BES_LASTSTATE      // add new state above this line; also add new entry to BES_STRINGS below!
};

// TAR 258195 - define string values for friendly traces
#define BES_STRINGS                  \
{                                    \
  _T("BES_STATERETURN"),             \
  _T("BES_START"),                   \
  _T("BES_SCANANDBAG"),              \
  _T("BES_SELECTFROMLIST"),          \
  _T("BES_ENTERCOUPONS"),            \
  _T("BES_EMPTY_STATE"),             \
  _T("BES_LOOKUPITEM"),              \
  _T("BES_FINISH"),                  \
  _T("BES_TAKECARD"),                \
  _T("BES_SIGNCHARGE"),              \
  _T("BES_SELECTCARDTYPE"),          \
  _T("BES_THANKYOU"),                \
  _T("BES_SELECTCONTAINER"),         \
  _T("BES_HELPSCREEN"),              \
  _T("BES_CARDMISREAD"),             \
  _T("BES_SELECTTARE"),              \
  _T("BES_OUTOFSERVICE"),            \
  _T("BES_COUPONNOTALLOWED"),        \
  _T("BES_KEYINCODE"),               \
  _T("BES_REQUESTSIG"),              \
  _T("BES_ENTERQUANTITY"),           \
  _T("BES_ENTERPIN"),                \
  _T("BES_PRODUCEFAVORITES"),        \
  _T("BES_ENTERWEIGHT"),             \
  _T("BES_ENTERWTFORPRICEEMBEDDED"), \
  _T("BES_CRATEABLEITEM"),           \
  _T("BES_SMMEDIASTATUS"),           \
  _T("BES_SMSYSTEMFUNCTIONS"),       \
  _T("BES_COUPONTOOHIGH"),           \
  _T("BES_COUPONNOMATCH"),           \
  _T("BES_INSERTCOUPON"),            \
  _T("BES_ENTERCOUPONVALUE"),        \
  _T("BES_BAGANDEAS"),               \
  _T("BES_SECVIOLATION"),            \
  _T("BES_PICKINGUPITEMS"),          \
  _T("BES_SECMISMATCHWEIGHT"),       \
  _T("BES_SECUNEXPECTEDDECREASE"),   \
  _T("BES_VOIDAPPROVAL"),            \
  _T("BES_SECUNEXPECTEDINCREASE"),   \
  _T("BES_ACKNOWLEDGESIG"),          \
  _T("BES_CARDPROCESSING"),          \
  _T("BES_CONFIRMSIGNATURE"),        \
  _T("BES_GETFOODSTAMPSTATE"),       \
  _T("BES_TAKERECEIPT"),             \
  _T("BES_SETALLITEMSASIDE"),        \
  _T("BES_VOIDTRANSAPPROVAL"),       \
  _T("BES_OUTOFSERVICE2"),           \
  _T("BES_LANECLOSED"),              \
  _T("BES_HOSTOFFLINE"),             \
  _T("BES_SECBAGBOXTHRESHOLD"),      \
  _T("BES_SECITEMREMOVEDTHRESHOLD"), \
  _T("BES_SECNEWITEMENTRY"),         \
  _T("BES_SECSKIPBAGGINGTHRESHOLD"), \
  _T("BES_VOIDITEM"),                \
  _T("BES_VOIDNOMATCH"),             \
  _T("BES_CONFIRMVOID"),             \
  _T("BES_CASHPAYMENT"),             \
  _T("BES_CHECKBASKET"),             \
  _T("BES_DEPOSITCOUPONS"),          \
  _T("BES_DROPOFFCOUPONS"),          \
  _T("BES_TRANSPORTITEM"),           \
  _T("BES_SECBAGAREABACKUP"),        \
  _T("BES_CONFIRMABORT"),            \
  _T("BES_WAITFORAPPROVAL"),         \
  _T("BES_WAITFORREMOTEASSIST"),     \
  _T("BES_SYSMSGOPPASS"),            \
  _T("BES_SECURITYLEVEL3"),          \
  _T("BES_RAPDATANEEDED"),           \
  _T("BES_ENTERALPHANUMERICINFO"),   \
  _T("BES_UNDEACTIVATEDITEM"),       \
  _T("BES_ITEMNOTFORSALE"),          \
  _T("BES_UNKNOWNITEM"),             \
  _T("BES_TIMERESTRICTEDITEM"),      \
  _T("BES_ITEMQUANTITYEXCEEDED"),    \
  _T("BES_CMDATAENTRY"),             \
  _T("BES_ASSISTEDTENDER"),          \
  _T("BES_STOREAUTHORIZATION"),      \
  _T("BES_SMDATAENTRY"),             \
  _T("BES_VOUCHERPAYMENT"),          \
  _T("BES_TENDERBALANCE"),           \
  _T("BES_DEPOSITGIFTCARD"),         \
  _T("BES_INSERTGIFTCARD"),          \
  _T("BES_COLLECTGIFTCARD"),         \
  _T("BES_EXTENSIONSTATE"),          \
  _T("BES_LANGUAGESELECT"),          \
  _T("BES_CUSTOMMESSAGE"),           \
  _T("BES_SMPRINTRECEIPT"),          \
  _T("BES_UNIVERSALSSF"),			 \
  _T("BES_PUTBAGONSCALE"),           \
  _T("BES_SECBAGBOXNOTVALID"),       \
  _T("BES_SELLBAGS"),				 \
  _T("BES_DEGRADEDCONTINUETRANS"),	 \
}


//-------------------------------------
// BSIGNORE macro - ignore some events
//-------------------------------------
#define BSIGNORE(a)				  \
  SMStateBase *SMStateBase::a(void)		  \
{									\
  trace(L8,_T("Base state ignore ") _T(#a)); \
  return STATE_NULL;				 \
}

//----------------------------------------------------------------------
// TIMESTAMP macros
// For timestamping the first occurrence of the instantiation of a state
// object, put DEFINE_TIMESTAMP somewhere in the cpp file above the
// constructor, and put IMPLEMENT_TIMESTAMP inside the constructor.
//----------------------------------------------------------------------
#define DEFINE_TIMESTAMP   \
static bool fTimeStamp = false;

#define IMPLEMENT_TIMESTAMP     \
if (!fTimeStamp)			    \
{						        \
  trace(L0, _T(__TIMESTAMP__));     \
  fTimeStamp = true;		    \
}						        \

#define TARE_NUMBER1 1			// determine which tare to use
#define TARE_NUMBER2 2			// determine which tare to use

//+SR742
typedef  enum
{
 DEGRADEDMODE_OFF = 0,
 DEGRADEDMODE_CARD_ONLY,
 DEGRADEDMODE_CASH_ONLY,
}  DEGRADEDMODES;
//-SR742

//USSF Start
class SMUSSFManager;
class SMUSSFManagerBase;
//USSF End

//////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMStateBase : public CObject
{

//USSF Start
  friend SMUSSFManagerBase;
  friend SMUSSFManager;
//USSF End

public:
  SMStateBase();

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
  void SynchronizeStoreloginWithLaunchPad(LPCTSTR szMessage); //TAR 301706
  virtual SMStateBase * PerformEOD();
  static void TraceWtInfo(); //tar 193806
  bool IsUsingCouponSensor();    //use coupon sensor option

  //TAR 269516 - Has app recieved shutdown message?
  static bool IsStopping() {return fStopSCOT; };

  //virtual SMStateBase *Initialize(void)=0;	// force override
  virtual SMStateBase *Initialize(void){ return STATE_NULL;};	// force override
  virtual void	 UnInitialize(void);
  virtual SMStateBase *TimedOut(void);

  virtual bool	 customerMode()  { return false; }
  virtual bool	 storeMode()	 { return false; }
  virtual bool	 helpMode()	  { return false; }
  virtual bool	 assistMode()	  { return false; }
  virtual bool	 storeModeHelp() { return false; }
  virtual bool	 inProgress()	{ return false; }

  virtual SMStateBase *Parse(MessageElement*);	// same parse for all
  virtual SMStateBase *IdleTimer(const long);   // idle between messages

  static BEST	 getAnchorState()
 // virtual BEST	 getAnchorState()
	{ return anchorState; }
  static SMStateBase* createAnchorState();
  //virtual SMStateBase* createAnchorState();

  virtual CString  CleanUpMessage(LPCTSTR,   // message to cleanup
		const bool);	// is message from a 2x20
  virtual void GetTrxLimitString(const PSTEXT nTextID, CString& csMsgText);

  virtual bool   restrictedItemNotAllowed();

  virtual PSTEXT restrictedItemMessage();
  

  // CopyFileFrom4690: returns
  //   -1 error, read or control data error, file not copied
  //   -2 warning, same file data/time/size, file not copied
  //   other -ve rc means file error from 4690 like 0x80104010 not found
  //   other >= 0, number of bytes copied
  virtual long CopyFileFromServer(LPCTSTR,	 // ...from 4690 file name
		LPCTSTR,												  // ...to local file name
		const bool);	// ...T/F T-copy always, F-copy if different
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
											bool   checkHealth );
  virtual SMStateBase* createLevel2Message( LPCTSTR szMsg,
											PSTEXT pstMsg,
											PSTEXT prompt,
											long   value,
											long   device,
											bool   checkHealth );
  virtual SMStateBase* createLevel3Message( LPCTSTR szMsg,
											PSTEXT pstMsg,
											PSTEXT prompt,
											long   value,
											long   device,
											bool   checkHealth );
  //Tar 124345
  //static bool bReadyForTakeReceipt; //Tar 126024
  //static bool bTrxClosed;

	static bool fInMultiSelectPickList; 
	static bool ms_bIsCurrentItemFromPickList; // Current item is a PickList item
    static bool bIsFromHostOffline;  //from HostOffline state
	static bool	fStateFreeze;	// stay in the current state T/F
  static bool	fStateAssist;	// stay in the current state T/F
  static CString csLastNonHelpStateName; // for SMContextHelp
  static CString csLastNonHelpStateBMP; // for SMContextHelp
  static bool	m_bSecViolationFromAttract;  //383748
  static int	 nLastNonHelpStateTimeOut;	// for SMContextHelp
  static bool	fLastNonHelpStateStoreMode;	// save for debug info
  //Tar 122722
  static bool bFinish;       // if hit finish and pay button
  static bool fSSWtFromAttract;			// TAR123172 only used if scan scale wt increase from attract screen LPM101499
  int	 nTimeOut;								  // seconds in this state
  virtual SMStateBase *DMdf_EASNoMotion();
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
  virtual SMStateBase *PushStateFifoStack(SMStateBase *);	// changed to public so MPProcedures.cpp can use it LPM072901  TAR176732
  virtual SMStateBase *OnCouponSensor(); //if received a coupon sensor event
  virtual SMStateBase *OnMotionCashOutSensor();      //if received a Motion cash trough sensor event
  virtual SMStateBase *OnNoMotionCashOutSensor();    //if received a No Motion cash trough sensor event
  virtual SMStateBase *GenerateTrainModeIntervention(void); // Generate Training Mode Intervention cs185088
  virtual bool InvalidCouponPrefix(CString&);      //TAR 269159        
  static bool OpenSCOTFile(CString csFileName,bool bAbortOnOpenFail, bool bIsDefaultFile); // check for existence of any scot files TAR262985
  virtual void SynchronizeTime(); 
  virtual bool ShrinkPicklistImages();
  virtual CString GetWideCharToMultiByte(const CString &csData);

#include "SMStateTBBase.h"
#include "SMStatePSBase.h"
#include "SMStateDMBase.h"
#include "SMStateSABase.h"
#include "SMStateUEBase.h"
#include "SMStateHTBase.h"
#include "SMStateRABase.h"
#include "SMStateRPBase.h"
#include "SMStateCUBase.h"

//USSF Start
#include "SMStateUSSFBase.h"
//USSF End

protected:
  static CPSX*  m_pAppPSX;          // NewUI - pointer to PSX object
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
  static bool   fDoEOD;			  // need to run lane EOD
  static bool   fDoLogsTo4690;	  // need to copy log files to 4690 T/F
  static bool   fItemIsWaiting;	 // last item was scanned in Attract or FTU T/F
  //  static bool   fApprovedForTobacco; // verified old enough for tobacco
  //  static bool   fApprovedForAlcohol; // verified old enough for alcohol
  static bool   fApprovedForRestrictedItems; // verified old enough for all restricted items
  static bool   fAgeObtained;	   // obtained some age, old enough or not
  static int	nCustomerAge;	   // The age of the customer
  static bool	fFromRequestSignature;	// disable timeout feature if come from signature needed screen

  static bool   fRequestForHelp;		// customer asked for help
  static bool   fCantScanCoupon;		// customer can't scan a coupon
  static bool   fUnDeActivatedItem;     // Any Unintervened undeactivated items on takeaway belt
  static bool   fNewItemScannedSinceLastUnDeActivatedItem; 
  static bool   fBeyondFinishAndPayState;
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
	/******cm150000: Added in Merge 2******/
//	static long lNormalCouponAmount;	// total normal coupons (Scanned or manual upc input)
//	static long lHighRiskCouponAmount;	// total high rish coupons (price input by user)
	/******cm150000: End Merge 2******/

  static bool bBadCard;            //TAR 193038: true: bad card(misread, bad track data, bad account number


  //TB
  static CString csLastDisplay;   //The last display data
  static CString csTBMessage2Scot; //thp
  static HFILE hReceiptFile;				// The handel of the receipt file
  static HFILE hJournalFile;			 // The handel of the Journal file
  static HFILE hSlipFile;				// The handel of the Slip file
  virtual void CutReceipt(void);	// Cut the receipt

  virtual void SaveSigReceiptData(BSTR pSignatureData);   // Save Signature Receipt Data
  virtual void SaveReceiptData(void); 
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
  static bool m_bShowMediaStatusButton;	

  //static SCOT_LANGUAGE_TYPE m_customerLanguage; //TAR210823 CHU make it public

  CStdioFile filePrint;		//Status File

  virtual SMStateBase *GPNotify(const GPEVT);
  virtual SMStateBase *PopStateFifoStack(void);
  // Are there any Items Sold or Voided items.
  virtual bool isAnySoldVoidedItems(void);
  virtual bool AreThereAnySoldVoidedItems(void);	//Fiscal Printer changes
  virtual bool isBarcodeValidOperatorPassword(const CString csItemCode, CString *pcsOperatorID = NULL);
  virtual void ReadOperatorLoginBarcodeMask(CString OperatorLoginBarcodeMask);  //FZ20020730 RFC 187993
  virtual void InvalidOperatorLoginBarcodeMask(CString csOperatorLoginBarcodeMask); //FZ and NN RFC 187993
  virtual bool isRestrictedItemApprovalRequired(void);
  virtual bool isVisualItemApprovalRequired(void);
  virtual bool isSecurityApprovalRequired(void);
  virtual bool isCouponRatioApprovalRequired(const bool=false);
  virtual bool isVoidRatioApprovalRequired(const bool=false);
  virtual bool isAnyApprovalRequired(const bool=false);
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

  virtual CString GetApprovalNeededString(void);
  virtual bool WriteSCOTStatusToFile(CString cStatus);	// Write string to Status File
  virtual bool OpenSCOTStatusFile(CString cFileName);	// Open Status File 
  // takeaway belt
  virtual SMStateBase* DMTakeawaySendScaleStatus();		// TakeawayBelt request for scale status
  virtual SMStateBase* DMTakeawayAMExitDone();			// TakeawayBelt notifying done with assist mode
  virtual TenderButton &GetTenderButton(int iIndex = -1);
  virtual void SetTenderButton(int iIndex = -1);
  virtual int  AdjustTenderButtonIndex(TBTENDERTYPE nAMTender);

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
  static int  nTimeSinceLastHeartbeat; // time since last heartbeat was sent
  static int  nISIPTimeOut;	// time counted while item sale is in progress
  SMStateBase* pStateAfterISIPTimeOut;
  static BEST  anchorState; // a saved point for states to return to
  static BEST  TABstateBeforeTransport; //TAR221089 CHU 112502 saved so we go back to this state from Transport
  bool      m_bStatePopped;  // TAR #223705 to allow multiple SystemMessages on the stack
  static ItemDetails itemDetailOrig; //Item details struction for saving pre-update details
  static ItemDetails itemDetailNew;  //Item details structure for saving details for update;
  
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
  
public:
  DECLARE_DYNCREATE(SMStateBase) // MFC Runtime class/object information
  CString GetOperator() {return  csOperator;}
  CString GetPassword() {return csPassword;}
  void SCDownload(int nTerminalNumber);  // TAR 287143
  static void addOperatorID(CString csRapName, CString csOpID);  //RFC2054
  static CString getOperatorID(CString csRapName);  //RFC2054
  bool COInitialize(int nTerminalNumber, bool bLocal = false);
  bool SCOTTENDInitialize(int nTerminalNumber, bool bLocal = false);
  virtual SMStateBase* ReloadOptions(void);
  virtual long DisplayAuxScreen(SCOT_AUX_SCREEN eScreen, bool bForceRefresh = false, int nArgs = 0, ...);
  bool         m_bNeedToDisplayAuxScreen;
  virtual SMStateBase* VerifyBirthDate(void);		// called when enter key is pressed on authorization or restricted items 
  //TAR 192098 196908 196910 start
  virtual BOOL VerifyBirthDate(CString csFromAnyDOB, CString csoperatorID, CString cspassword);
  virtual long GetAge(CString csFromAnyDOB);
  //TAR 192098 196908 196910 end
  virtual SMStateBase* SAEndOfTransaction(void);	// Security issued a 161 so end this transaction  LPM030501 TAR164591
  virtual long ReloadSASIActions(void);	// during reload options, only reload the SASI actions file, not the entire OCX
  virtual SMStateBase* CheckForDATFile(bool bReloadOption);	 //awk.7.01 TAR 173990
  virtual SMStateBase* OnMatchedWt(void);				// Smart Scale reported a matched weight
  virtual SMStateBase* OnWtIncreaseNotAllowed(void);	// Smart Scale reported an unmatched wt increase
  virtual SMStateBase* OnWtDecrease(void);				// Smart Scale reported an unmatched wt decrease
  virtual SMStateBase* OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual SMStateBase* OnSecurityScaleFailure(int nFailureType);	// Smart Scale reported a bag scale failure
  virtual SMStateBase* OnDBMgrFailure(long lStatusCode, long lStatusType);	//DB client reported a failure
  // Undeactivated item on the TakeAway belt (belt 2) detected. 
  virtual SMStateBase* OnUnDeActivatedTheftTag(void);
  virtual SMStateBase* OnSkipBaggingAllowed(void);	///TAR 407200 Security Manager report Skip Bagging is allowed
  static long lEOT_LGW;
  static bool g_bMatchedWtWaitingOnEAS;
  static bool m_bEASHappened;		// cmal121002 a motion no motion pair occured after an item purchase tar 219883
  static bool bIsCoupon;	//TAR339619
  static int m_View;              // ID of the view to be displayed to RAP operator
  // Used to save the previous view, for example if (some device error happens while at 
  // price needed intervention then it will save m_PrevView as DATANEEDED_PRICEPROMPT 
  static int m_PrevView;          // ID of the previous view (if any) to be displayed to RAP operator
  static CString m_sButton;       // String of button clicked by RAP operator
  static CString m_sInput;        // Input received from RAP operator
  static int nSkipBaggingBtnPresses;		//the number of Skip Btn Button Press
  static int nItemRemovedBtnPresses;	//the number of Done Removing Button Press
  static int nBagBoxBtnPresses;
  static long g_lBaggingReq;				// global bagging required for each item
  virtual SMStateBase* OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
  virtual SMStateBase* OnRAPApproveAndUpdateBtn(void);
  virtual SMStateBase* OnRAPApproveNoUpdateBtn(void);
  virtual SMStateBase* OnRAPLightItemBtn(void);
  virtual SMStateBase* OnRAPItemNotBaggedBtn(void);
  virtual CString GetDescFromView(int nView);  //for Reporting Feature get description from view
  virtual CString GetCategory(DWORD m_nRpID);  //for Reporting Feature get category 
  static bool bToldRAPMediaLow;		// Tell RAP about media status
  virtual CString getSCOTStateName(void);
  virtual void UpdateStateName(CString csStateName = EMPTY_STRING);
  static SMStateBase* createState(BEST newState);		// fixed multiple problems to be able to pass state to the creation without reseting anchor state LPM 3.0 Sec/App merge

  // TAR196659 PUSHING AND POPPING -  changes to use one stack per state LPM022502
  virtual SMStateBase* CkStateStackAndReturn(void);
  virtual void ClearStateStackEOT(void);
  static SMStateBase* g_pDelay_TBExpectedStateChange;  
  static SMStateBase* g_pDelay_WaitForApprovalStateChange; 
  static bool g_bGLOBALScanAndBagInitializeDone; // to not disable devices that have not been enabled yet when S&B initialized/uninitialized
  static bool g_bSnBScreenIsShown;		// sometimes can pop S&B but the global bAlreadyInSnB is wrong, need another flag
  static bool g_bgTrxVerif;		// for WaitForApproval delayed when creating it the 2nd time
  static bool g_bgVoidTrx;		// for WaitForApproval delayed when creating it the 2nd time
  static bool g_bgVerifySig;	// for WaitForApproval delayed when creating it the 2nd time
  static bool g_bgCheckCashChanger;  // Tar 256141
  static bool g_bAllowEnterRemoteAssistMode;  // for allowing entering Remote Assist Mode
  virtual SMStateBase* OnDataBaseInfoComplete(void);
  static bool g_bSafetyCoverClosed;		// TAB LPM
  static bool g_bZeroWtExpected;        // TAR197477 TAB
  static bool ms_bKeepRemoteDevices;
  static ENDORSE_STATE fInEndorsementState;
  virtual SMStateBase *SMStateBase::HandleDataNeeded(void);
  virtual SMStateBase *SMStateBase::HandleMSRData(void);  //RFQ 2605 - MSR handling for Dataneeded
  virtual void Escalation2RAP(void){;}
  virtual SMStateBase* PSRemoteButton1(void);	// RFC 209075 RJF 073102
  virtual void ScanScaleAttract(void){;} //RFC 276235
  virtual void ScanScaleSB(void){;} //RFC 276235
  static bool g_bAllowRemoteBtn1;				// RFC 209075 
  static bool g_bAssistModeDataNeeded;          // TAR 213074
  virtual void TABTransportClearedItem(void);	// consolidate dup lines into one procedure LPM090502
  virtual void TABReEnableScannerAndClearWaitForApprovalState(void); //Re-enable the scanner and clear wait for approval state
  ITelnetClientIFPtr m_pTelnetIF;//FastLane 3.1 Reporting telnet client 

  static CString SMStateBase::g_csSavedDeptInfo; // SR 141 - RFC 283384  static bool m_bMotionSensorThreadRun;			// RFC 219283 RF/DE 112002
  static bool m_bMotionSensorThreadRun;			// RFC 219283 RF/DE 112002
  static CWinThread *m_hMotionSensorThread;
  static int m_MotionEnablesScannerInterval;
  static int m_MotionSensorOnAttractInterval;   //RFC 265465
  static bool m_bMotionSensorEnabledOnAttract; //RFC 265465
  static long g_lUseDefaultTolerance;		// for Std Dev, will need to know if WtInfo determines to use the default tol or exception tol for SMM LPM120402
  static int m_iTolUsedflag;				// poly-state flag indicating tolerance used: 1 = Std Deviation; 2 = Default; 3 = Maximum was used (not good) - for tracing purposes in the App
  virtual int GetButtonIndex(int nTenderType);
  //SR742+
  virtual bool IsCashDeviceError(long lDeviceClass);
  virtual bool IsCardDeviceError(long lDeviceClass);
  virtual SMStateBase *EnterDegradedMode(DEGRADEDMODES lModeType); 
  virtual SMStateBase *ExitDegradedMode();	
  //SR742-
  static bool bTenderLimitExceeded;
  virtual void SetStatePopped( bool bPopped );
  virtual bool GetStatePopped();
  static long SMStateBase::g_lDisplayFormRC;  // TAR 219971, 220074, 208118 BZ
  virtual BEST getNextStateForGoodItem();	  // 229084 created a Base function for getNextStateForGoodItem LPM022503
  static long m_lWtComparisonReq;			  // 229344 global wt comparison/substitution check flag for each item false means no comparison & no WLDB updates LPM022703 CMal
  virtual bool AreAnyRedApprovalsPending();   // Tar 211746 RJF function to check for all approvals that should turn light red
  static long m_lSavedCashChangerStatusCode;  // TAR225069  
  static bool fStartTrxInAM;                  // RFC 228102 RF 060503 initiate transaction from RAP
  static bool m_bHasSeenTakeReceipt;          // TAR239622 CHU
  static bool m_bScanOnAttractAtQuietMode;    //TAR234536 CHU 080403
  static CString g_csAssistModeStateName;     // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
  static pfnSSFAnchorType m_pfnSSFCreateAnchor;	// added for SSF implementations  // TAR:242025
  void SendCurrencyAcceptedToRAP(long lAcceptedAmount);
  static bool bDoNotResetTimeOutOnDMEvent;	  // TAR 260288
  virtual SMStateBase* HostOffLine(void);
  virtual SMStateBase* HostOnLine(void);
  static bool m_bReportServerOnline ; // RFC 262343
  virtual SMStateBase* OnTransactionComplete(void);
  virtual void OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation);
  virtual void OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation, LPCTSTR lpszCategory);
  virtual void TellApplicationStateToSecurity(LPCTSTR lpszOperation, LPCTSTR lpszStateName);
  virtual long ChangeSecurityMaintenanceWtFlags(LPCTSTR lpszOperation, LPCTSTR lpszUPC,LPCTSTR lpszFlag, bool bPriceEmbedded);
  virtual long ChangeSecurityMaintenanceTolerance(LPCTSTR lpszOperation, LPCTSTR lpszUPC, LPCTSTR lpszTolerance, bool bPriceEmbedded);
  virtual SMStateBase* ReturnDelayedStateForGCTender(); //check trxend and return appropriate state
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
  static bool g_bIsForgiveUnexpectedDecreaseDuringTendering;
  static CString g_csTareWt1;	// SR682
  static CString g_csTareWt2;	// SR682
  static CString g_csTareWt3;	// SR714
  virtual bool GetSecurityConfig(LPCTSTR lpszParamName);
  virtual CString GetSecurityConfigValue(LPCTSTR lpszParamName); // SR682
  virtual bool IsLoginBypassAllowed() { return true; };
  static bool m_bSignaturePrintingStatus;      //TAR 195179 & 191490 & 224378
  static long m_lAgerestrictQueuedtoRAP;       //TAR 196910
  static long m_lItemsScanned;	               //TAR 04/30/02 BZ
  static bool g_bOnItemOK;			// TAR 298537 Use for TAB only
  static long g_lNumberOfItemsNotBagInMultiPick;			// Multipick Enhancement
  static bool g_bOnScannerEnable;	// TAR 298537 Use for TAB only
  virtual void	 setAnchorState(BEST state);
  virtual SMStateBase* setAndCreateAnchorState(BEST state);
  virtual void DeliverAnchorState(SMStateBase *StatePtr);
  virtual void DeliverUpdate(BEST state);
  virtual bool IgnoreTBMessageInSecurityState();
  static bool m_bCurrentIntervention_MisMatch;     //TAR340654
  static bool m_bCurrentIntervention_UnexIncrease; //TAR340654
  static bool m_bCurrentIntervention_UnexDecrease; //TAR340654
  static bool m_bCurrentIntervention_DontBagThreshold; //TAR433737
  static bool g_bReadyForItem;						//TAR 363023
  virtual bool IsForceDelayedApprovalBeforeSuspend();
  static FT_ACTION getRegisterAction(FT_STATUS_PT pStatus, void *pParam);  //SR93 Biometrics
  long GetDisplayedItemCount();		// Rfc 371075
  static bool g_bInterventionApproval; //TAR 371932 & tar 371918
  virtual SMStateBase *CouponMaxIntervention();			//SR678	Generate Intervention for Coupon that exceed     maximum limit
  virtual SMStateBase *CouponMaxInterventionFinal();	//SR678 Generate Intervention for Coupon that exceed maximum limit
  static bool bIsCouponExceedLimitFinalization;			//SR678
  static bool bIsMaxCouponLimitInterventionDone;
  //+SR713
  virtual SMStateBase *PreTenderState(void);			
  static bool bDoNotShowSellBags;								
  static bool bIsInPreTenderState;
  //-SR713
  static BOOL g_bValidHeadCashier;
  static bool g_bSkipBaggingButtonTimerExpires; //SR778
  virtual void StartTimerForSkipBaggingBtn(){;} // SR778
  static bool g_bMultiPickSkipBaggingAllowed; // TAR 398289
  static BEST g_savedAnchorStateFromAttract; //416582
  virtual bool IsRemoteErrorAllowed(long lDeviceClass); //TAR408719
};

#endif

