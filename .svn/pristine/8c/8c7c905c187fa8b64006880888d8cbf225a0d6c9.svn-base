#ifndef _SMSTATE_CONSTANTS_H
#define _SMSTATE_CONSTANTS_H

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
#define FORMATTED_RECEIPT_FILE _T("%DATA_DRIVE%\\Scot\\Data\\FormattedReceiptData.txt")  //SR Personalization Digital Receipt
#define GETTEXTOUTPUT _T("%s (Language ID %s):\n%s") //dp185016 rfc 265463

#define DEFAULT_SECURITY_ANALYSIS_TIMER     10
#define DEFAULT_RECALLING_WAIT_FOR_DONE     240

#ifdef _SCOTSSF_
  #define DLLMODE  DLLEXPORT
#else
  #define DLLMODE  DLLIMPORT
#endif

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
  #ifndef _CPPUNIT
      #define STATE(a)              SM##a
      #define RETURNSTATE(a)          {                     \
            CRuntimeClass* p##a = RUNTIME_CLASS(SM##a);     \
            CObject* po##a = p##a->CreateObject();          \
            return (SMStateBase*)po##a;}
      #define CREATE_AND_DISPENSE(a)                        \
            CRuntimeClass* p##a = RUNTIME_CLASS(SM##a);     \
            CObject* po##a = p##a->CreateObject();          \
            STATE(a)* ClassPtr = (STATE(a)*) po##a;         \
            return ClassPtr->Deliver
      #define CREATE_AND_ASSIGN(a,b)                        \
            CRuntimeClass* p##a = RUNTIME_CLASS(SM##a);     \
            CObject* po##a = p##a->CreateObject();          \
            b = (SMStateBase*)((STATE(a)*) po##a)->Deliver
  #else
      #define STATE(a)              FakeSM##a##Base
      #define RETURNSTATE(a)          {                                       \
            FakeSMStateBase::SetRETURNSTATETestString(_T(RETURNSTRING(a)));     \
            return STATE_NULL;}
      #define CREATE_AND_DISPENSE(a)                        \
            FakeSMStateBase::SetCREATE_AND_DISPENSE_TestString(_T(RETURNSTRING(a)));     \
            return FakeSMStateBase::Deliver
      #define RETURNSTRING(a)  #a
      #define CREATE_AND_ASSIGN(a,b)                        \
            FakeSMStateBase::SetRETURNSTATETestString(_T(RETURNSTRING(a)));     \
            b = FakeSMStateBase::Deliver
  #endif //_CPPUNIT
#endif

// Specifically use for UT to have cleaner code
#ifndef _CPPUNIT
    #define SMCUSTOMERMODESTATE SMCustomerModeState // For now SMCustomerModeState don't have SSF yet.
                                                    // if SMCustomerModeState will have SSF, use STATE() macro above
    #define SCOTDATAENTRY CSCOTDataEntry
    #define PSCONFIGDATA CPSConfigData
    #define CKEYVALUEPARMLIST CKeyValueParmList
    #define CKEYVALUE CKeyValue
    #define CPSXREDRAWLOCK CPSXRedrawLock
    #define ITEMDETAILS ItemDetails
    #define COLLECTORBASE CCollectorBase
#else
    #define SMCUSTOMERMODESTATE FakeSMCustomerModeState
    #define SCOTDATAENTRY CFakeSCOTDataEntry
    #define PSCONFIGDATA CFakePSConfigData
    #define CKEYVALUEPARMLIST CFakeKeyValueParmList
    #define CKEYVALUE CFakeKeyValue
    #define CPSXREDRAWLOCK CFakePSXRedrawLock
    #define ITEMDETAILS FakeItemDetails
    #define COLLECTORBASE CFakeCollectorBase
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
	PRINT_RECEIPT_VISIBLE, //SR832
	WEIGHT_ACCEPT,               //SR814.2 
    LOYALTY_LOOKUP,             //SR815 - Personalization
    // +-SSCOADK-5902
    CM_MONEY_INSERTED_TIMER,
	HHSCANNER_TIMER,			//CRD124746
	WELCOMENAV_TIMER,			//CRD 537487
	MAX_TIMER					// 3.2 patch C - RFC 255737 - Please add all base timer ids above this timer id
} SCOTAPP_TIMERS;

// base entry states that can be returned to
#define BEST int

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
  BES_SWIPECARD, //SR+879
  BES_SAVEPREFERENCES,
  BES_SCANCARD,    
  BES_SELECTPAYMENT,      
  BES_CONTINUETRANS,		    
  BES_EBT, 
  BES_CONFIRMQUANTITY,
  BES_RESTRICTEDNOTALLOWED, //-SR879
  BES_CASHMANAGEMENTEX,
  BES_SECURITYANALYSIS, //SSCOADK-546
  BES_INVALIDBARCODE, //SSCOADK-1210
  BES_RECALLINGITEMS,
  BES_SMMEDITADD,
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
  _T("BES_SWIPECARD"),				 \
  _T("BES_SAVEPREFERENCES"),		 \
  _T("BES_SCANCARD"),		         \
  _T("BES_SELECTPAYMENT"),			 \
  _T("BES_CONTINUETRANS"),			 \
  _T("BES_EBT"),					 \
  _T("BES_CONFIRMQUANTITY"),		 \
  _T("BES_RESTRICTEDNOTALLOWED"),	 \
  _T("BES_CASHMANAGEMENTEX"),        \
  _T("BES_SECURITYANALYSIS"),        \
  _T("BES_INVALIDBARCODE"),        \
  _T("BES_RECALLINGITEMS"),        \
  _T("BES_SMMEDITADD"),        \
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

#define REFRESH_THEMES_CODE 711   //SSCOP-1158

#define PLA_VALIDATION              5000 //SSCOADK-260
#define PLA_LOOKUP                  5001
#define PLA_HANDINVIEW              5002
#define PLA_TRANSITION              5003
#define PLA_HANDINVIEW_AFTERRETRY   5004

//+SR742
typedef  enum
{
 DEGRADEDMODE_OFF = 0,
 DEGRADEDMODE_CARD_ONLY,
 DEGRADEDMODE_CASH_ONLY,
}  DEGRADEDMODES;
//-SR742

typedef enum 
{
  GENERIC_DELAYED_NOT_PROCESSING = 0,
  GENERIC_DELAYED_STARTING,
  GENERIC_DELAYED_PROCESSING,
  GENERIC_DELAYED_COMPLETE,
  GENERIC_DELAYED_COMPLETING_APPROVALS,
} GENERIC_DELAYED_STATUS;



#define TBASYNCDATANEEDEDHANDLER _T("TBAsync")

#endif _SMSTATE_CONSTANTS_H
