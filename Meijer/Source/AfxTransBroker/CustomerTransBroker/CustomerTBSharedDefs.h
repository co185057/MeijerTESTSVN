//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CustomerTBSharedDefs.h
//
// CHANGE HISTORY
//
// CRD 557225 Name: CJ Estrada Date: Mar 4, 2021
// CRD 584194 Name: CJ Estrada Date: Mar 1, 2021
// CRD 525923 Name: CJ Estrada Date: Feb 1, 2021
// CRD 547452 Name: CJ Estrada Date: Jan 13, 2021
// CRD 541144 Name: CJ Estrada Date: Jan 7, 2021
// CRD 541173 Name: CJ Estrada Date: Dec 16, 2020
// CRD 526693 Name: CJ Estrada Date: Dec 14, 2020
// CRD 537503 Name: CJ Estrada Date: Dec 3, 2020
// POS97215 Work Request:61233 Name: Matt Condra Date:May 20, 2014
// POS83168 Work Request:61232 Name: Matt Condra Date:February 11, 2014
// POS81632 Work Request:59313  Name:Matt Condra  Date:December 10, 2013
// POS36319 Work Request:50864 Name:Robert Susanto Date:June 20, 2012
// POS29719 WORK REQUEST:19114 Name: Matt Condra Date: April 20, 2012
// POS29349 WORK REQUEST:19114 Name: Matt Condra Date: March 8, 2012
// POS19072 WORK REQUEST:16656 Name: Matt Condra Date: November 23, 2011
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CustomerTBSharedDefsh
#define _CustomerTBSharedDefsh
#define _MEIJER
/************************************************************************
* POS23514 Work Request:16632 Name: Sandeep Shenoy Date: Nov 9, 2011
* POS11024 Work Request:16632 Name: Sandeep Shenoy Date: Jul 6, 2011
************************************************************************/
#include "..\AfxTransBroker\SolutionTransbroker\SolutionTBSharedDefs.h"

#define TB_CUSTOMERSTATEID         13000   // in CustomerTBSharedDefs.h
#define TBCMD_CUSTOMERID            8000
// *** Remember - when you add a state here, add it to CustomerTBStates below.
// CustomerTBStates is used to trace all return codes.
//#define  TB_FIRST_STATE_HERE             (TBSTATE)(TB_CUSTOMERSTATEID +  1) 

//for DG22
/*
#define  TB_NEED_SCANANDBAG						(TBSTATE)(TB_CUSTOMERSTATEID +  1)
#define  TB_NEEDCOUPONCHANGE                    (TBSTATE)(TB_CUSTOMERSTATEID +  2)
#define  TB_NEEDIMMEDATEAGECHECK                (TBSTATE)(TB_CUSTOMERSTATEID +  3)
*/
#define  TB_LOG_NEGATIVE_MEDIA					(TBSTATE)(TB_CUSTOMERSTATEID +  1)
#define  TB_RESUME_STARTED					    (TBSTATE)(TB_CUSTOMERSTATEID +  2)
#define  TB_RESUME_ITEMS					    (TBSTATE)(TB_CUSTOMERSTATEID +  3)
#define  TB_CANCEL_ITEM					        (TBSTATE)(TB_CUSTOMERSTATEID +  4)
#define  TB_ENABLE_ASSISTMODE                   (TBSTATE)(TB_CUSTOMERSTATEID +  5)
#define  TB_RESUME_ACS_REWARD                   (TBSTATE)(TB_CUSTOMERSTATEID +  6)      //POS23514
#define  TB_INSERT_MEDIA_ITEM                   (TBSTATE)(TB_CUSTOMERSTATEID +  7)      //POS36319
#define  TB_POS_SCANNER_READY                   (TBSTATE)(TB_CUSTOMERSTATEID +  8)      //POS83168
#define  TB_POS_RESTARTING                      (TBSTATE)(TB_CUSTOMERSTATEID +  9)      //CRD185924
#define  TB_POS_NEED_RESTART                    (TBSTATE)(TB_CUSTOMERSTATEID +  10)
#define  TB_POS_RESTART_COMPLETE                (TBSTATE)(TB_CUSTOMERSTATEID +  11)
#define  TB_RESUME_COMPLETE					    (TBSTATE)(TB_CUSTOMERSTATEID +  12)		//CRD 177093

#define  TB_MAXCUSTOMERSTATEID                  (TBSTATE)(TB_CUSTOMERSTATEID + 999)

//Start CRD 145555
#define FLMSTYPE_NORMAL _T("NORMAL")
#define FLMSTYPE_PARTIALRESCAN _T("PARTIALRESCAN")
#define FLMSTYPE_FULLRESCAN _T("FULLRESCAN")

#define FLMS_START _T("START")
#define FLMS_END _T("END")

#define FLMSAUDITRESULT_PASS _T("PASS")
#define FLMSAUDITRESULT_FAIL _T("FAIL")
#define FLMSAUDITRESULT_CANCEL _T("CANCEL")

#define FLMSAUDITRECORD_PART _T("PART")
#define FLMSAUDITRECORD_LAST _T("LAST")

//Audit Result Messages
#define FLMS_ITEMNOTFOUND _T("ItemNotFound")
#define FLMS_PARTIALRESCANFAILED _T("PartialRescanFailed")
#define FLMS_PARTIALRESCANDETAILED _T("PartialRescanDetailed")
#define FLMS_JUSTPASSEDMINIMUMREQUIREMENTS _T("PartialRescanJustPassedMinimumRequirements")

//Tlog results
#define FLMS_RESULT_ITEMNOTFOUND _T("NOTFOUND")
#define FLMS_RESULT_NOTINCART _T("NOTINCART")
#define FLMS_RESULT_FOUND _T("FOUND")
#define FLMS_RESULT_INVALIDBARCODESUPP _T("SSCOInvalidSupp")
#define FLMS_RESULT_INVALIDBARCODEGS1 _T("SSCOInvalidGS1")
#define FLMS_RESULT_INVALIDBARCODEQR _T("SSCOInvalidQR")
#define FLMS_RESULT_INVALIDBARCODE _T("SSCOInvalidBarcode")
//End CRD 145555
// (+) CRD 127513
#define TRANSMODE_SCO _T("0")
#define TRANSMODE_SND _T("1")
#define TRANSMODE_MEIJERGO _T("2")
#define TRANSMODE_MEIJERGO_SND _T("3")
// (-) CRD 127513

#define OPERATORMSGNO_MPERKSOFFLINE			1408	//CRD 232267
#define OPERATORMSGNO_INSPECTEDIDAGE		1778  // SSCOI-54479
#define OPERATORMSGNO_INSPECTEDIDAGEOVER40  1779 // SSCOI-73518
#define OPERATORMSGNO_EASTAGS				1840 //CRD 213258
#define OPERATORMSGNO_MEPAVAILABLE			1852 //CRD 269544
#define OPERATORMSGNO_SASTIMEOUT			1853	//CRD 265301
#define OPERATORMSGNO_RESTRICTEDITEMMOBILE	1860	//CRD 295392 
#define OPERATORMSGNO_ACCOUNTLOOKUPFAILED   1882	//CRD 554916
#define OPERATORMSGNO_GETACCOUNTDETAILEDFAILED 1883	//CRD 557225

#define DF_OPERATORMSG 11
#define DF_MOBILEASSISTANCENEEDED 618
#define DF_AGESELECTOPTION 1010 // SSCOI-54479

#define DELAYED_NONE 0
#define DELAYED_EAS 1
#define DELAYED_GENERIC 2

#ifdef CUSTOMER_TBSTATES
struct {
    long key;
    TCHAR* szname;
}CustomerTBStates[]=
{
   
	//for DG22
	/*
    {TB_NEED_SCANANDBAG,               _T( "TB_NEED_SCANANDBAG")},
    {TB_NEEDCOUPONCHANGE,               _T( "TB_NEEDCOUPONCHANGE")},
    {TB_NEEDIMMEDATEAGECHECK,           _T( "TB_NEEDIMMEDATEAGECHECK")}, */
	{TB_LOG_NEGATIVE_MEDIA,            _T( "TB_LOG_NEGATIVE_MEDIA")},
    {TB_RESUME_STARTED,                _T( "TB_RESUME_STARTED")},
	{TB_RESUME_ITEMS,                  _T( "TB_RESUME_ITEMS")},
	{TB_CANCEL_ITEM,                   _T( "TB_CANCEL_ITEM")},
	{TB_ENABLE_ASSISTMODE,             _T( "TB_ENABLE_ASSISTMODE")},
    {TB_RESUME_ACS_REWARD,             _T( "TB_RESUME_ACS_REWARD")}, //POS23514
    {TB_INSERT_MEDIA_ITEM,             _T( "TB_INSERT_MEDIA_ITEM")}, //POS36319
    {TB_POS_SCANNER_READY,             _T( "TB_POS_SCANNER_READY")}, //POS83168
	{TB_POS_RESTARTING,                _T( "TB_POS_RESTARTING")},    //CRD185924
	{TB_POS_NEED_RESTART,              _T( "TB_POS_NEED_RESTART")},
	{TB_POS_RESTART_COMPLETE,          _T( "TB_POS_RESTART_COMPLETE")},
	{TB_RESUME_COMPLETE,			   _T( "TB_RESUME_COMPLETE")}, //CRD 177093
	

    // All states should be added above TB_MAXCUSTOMERSTATEID
    {TB_MAXCUSTOMERSTATEID,              _T("TB_MAXCUSTOMERSTATEID")}
};

#endif  // if CUSTOMER_TBSTATES


typedef enum
{
    //for DG22
    /*
    TBCMD_DUPLICATE_RECEIPT_PRNT = TBCMD_SOLUTION_TBCMD_LAST,
    TBCMD_GET_OPTIONS,
    TBCMD_GET_AGEREQUIRED,
    TBCMD_MSGVOIDABLE,
    */

    TBCMD_RESUME_ITEMS = TBCMD_SOLUTION_TBCMD_LAST,
    TBCMD_RESET_TO_LAST_POSSTATE,
    TBCMD_GET_POSSTATE,
    TBCMD_CASHDRAWER_CLOSE,
    TBCMD_VALIDATE_SUPERVISOR,
    // DG30
    TBCMD_TLOG_OVERRIDE,
	TBCMD_SEND_ASSISTMENU_KEY_PRESS,
	TBCMD_MICR_SHOULD_EJECT,
    TBCMD_SLIP_SHOULD_EJECT,
    TBCMD_SLIP_SHOULD_FLIP,
    TBCMD_SLIP_SHOULD_FLIP_AFTER_MICR,
	// DG31
	TBCMD_SEND_PRESELECTED_TENDER,
    // DG32
    TBCMD_GET_REBATE_RECEIPT_COUNTS,
	TBCMD_ADD_BITMAP_REQUEST,
    
    // DG34 - EMPLID / Biomet
    TBCMD_BIOMET_DATA,
    TBCMD_IS_EMPLID_ENABLED,
    TBCMD_IS_EMPLID_ALLOWHANDKEY,
    TBCMD_IS_BIOMET_ENABLED,
    TBCMD_IS_MEIJER_BADGE_GET_BARCODE,
    TBCMD_SIGNONBUILDER_TOSTRING,
    
    // DG35
    TBCMD_GET_TBSTATENAME,
    TBCMD_IS_TRXN_ENDING,
	
    // +SOTF 6312
    TBCMD_GET_EMPLID_FROM_BADGE,
    TBCMD_IS_MEIJER_BADGE,
    // -SOTF 6312

	//+SOTF 5572
	TBCMD_ROBOT_CASH_FLOW,
	//-SOTF 5572

    //SOTF 6226
    TBCMD_ACS_DELAYED_AGE_RESTRICTION,
    TBCMD_HIGHEST_AGE_REQUIRED,
    TBCMD_ENTER_OPERATOR_VOID_RESTRICTED_ITEM,
    TBCMD_EXIT_OPERATOR_VOID_RESTRICTED_ITEM,
    TBCMD_SET_AGE_RESTRICTION_ITEM_ALLOWED_STATUS,
	TBCMD_SEND_AGE_VERIFY_DATA,
    TBCMD_SET_DOB_ENTERED_STATUS,
    //SOTF 6226
    
    // SOTF 6657
    TBCMD_NOTIFY_ACS_REMOTE_ASSISTMODE_STATUS,
    //sotf
	//POS11024: Mobile Shopper
	TBCMD_GET_APPINI_ENTRY_VAL,


    // POS19072
    TBCMD_ENTERING_WIC_FREE_ITEM_PHASE,

    // POS29349
    TBCMD_NOTIFY_BROUGHT_BAG,

    // POS29719
    TBCMD_NOTIFY_EOT,

	// CRD111525 CRD111527
	TBCMD_NOTIFY_FLMS,
	// CRD126015
	TBCMD_NOTIFY_TRANSACTIONMODE,

    // POS81632
    TBCMD_CHECKINOUT_OPERATOR,
    TBCMD_NOTIFY_CHECKINOUT,

    // POS97215
    TBCMD_GET_OPERATOR_LAST_PWD_CHG_TIME,

	//CRD 185620
	TBCMD_HASEASITEM,
	TBCMD_EASCOMPLETE,

	//CRD 232268
	TBCMD_GET_EASLISTSIZE,	
	TBCMD_GET_EASENTRYID,
	TBCMD_CLEAR_EASLIST,
	TBCMD_GET_MOBILEASSISTANCENEEDED,
	TBCMD_CLEAR_MOBILEASSISTANCENEEDED,
	TBCMD_HASMOBILEASSISTANCENEEDED,
	TBCMD_FLMS_TRANSFERRING,

	//CRD 366323
	TBCMD_NOTIFY_EXIT_STOREMODE_TO_SCANANDBAG,
	TBCMD_NOTIFY_EXIT_STOREMODE_TO_PAYMENT,
	//CRD 321191
	TBCMD_NOTIFY_ES_ALERT_CANCELED,
	//CRD 338206
	TBCMD_NOTIFY_STOREMODE_PROMPT,

	TBCMD_WEIGHTITEMS_NOT_ALLOWED,
	TBCMD_HHSCANNER_ACTIVE,
	//CRD 541144
	TBCMD_SEND_CITI_ACCOUNT_DETAILS,
	TBCMD_NOTIFY_SCOLOADCONFIRMATION,
    //Add new command IDs above this line
    TBCMD_CUSTOMER_TBCMD_LAST
} CUSTOMER_TBCMD_ID;

// DG31:  Preselected tender types
typedef enum
{
    TB_PRESELECTED_TENDER_CREDIT = 3,
	TB_PRESELECTED_TENDER_DEBIT = 6,
	TB_PRESELECTED_TENDER_EBT_FOOD = 11,
	TB_PRESELECTED_TENDER_EBT_CASH = 12
} TB_PRESELECTED_TENDER_TYPE;

// FLNotify IDs
typedef enum
{
	FLNOTIFY_TLOGOVERRIDE = 1,	//1
	FLNOTIFY_PRESELECTEDTENDER,	//2
	FLNOTIFY_REMOTEASSISTMODE,  //3
	FLNOTIFY_CASHMONITORING,    //4
	FLNOTIFY_AGEVALIDATION,		//5
	FLNOTIFY_BROUGHTBAG,		//6
	FLNOTIFY_EOT,				//7
	FLNOTIFY_LOGRAPTRACK = 9,	//9
	FLNOTIFY_NOTCASHMONITORING,	//10
	FLNOTIFY_TRANSMODE = 12,	//12
	FLNOTIFY_FLMSBASKET,		//13
	FLNOTIFY_EXITSTOREMODE,		//14
	FLNOTIFY_STOREMODEPROMPT,	//15
	FLNOTIFY_CANCELALERT,		//16
	FLNOTIFY_HHSCANNERFLAG = 20,		//20
	FLNOTIFY_CITIACCOUNTDETAILS = 22,   //22
	FLNOTIFY_SCOLOADCONFIRMATION = 30	//30
} FLNOTIFY_ID;

typedef enum
{
	WEBAPPSTART =1,
	TMBADGESCANNED,
	VALIDATETM,
	GETACCOUNTDETAILS,
	WEBAPPDISPOSE

} MEIJERWEBAPPSTATUSID;

// CRD 525312
// Web App XML Tags
const CString szCitiApp				= _T("CitiApp");
const CString szAppStatus			= _T("AppStatus");
const CString szRefNumber			= _T("RefNumber");
const CString szCreditLimit			= _T("CreditLimit");
const CString szCaseNumber			= _T("CaseNumber");
const CString szStatus			    = _T("Status");
const CString szAppError		    = _T("AppError");
const CString szUseApprovedCard     = _T("UseApprovedCard");

// App Status
const CString szAppInitialize       = _T("Initialize");
const CString szAppLoadFailed       = _T("LoadFailed");
const CString szAppStarted			= _T("Started");
const CString szAppCanceled			= _T("Canceled");
const CString szAppTMIntervention	= _T("OperatorIntervention");
const CString szDisplayOperatorPW   = _T("DisplayOperatorPW");				// CRD 574057
//const CString szValidateTM			= _T("ValidateTM");					// CRD 574057
const CString szGetAccountDetails	= _T("GetAccountDetails");				// CRD 525923
const CString szProcessAccountDetails = _T("ProcessAccountDetails");		// CRD 525923
const CString szWebException		= _T("WebException");					// CRD 547452
const CString szGetAccountDetailsFailed = _T("GetAccountDetailsFailed");	// CRD 525923
const CString szRedisplayWebApp     = _T("RedisplayWebApp");				// CRD 584194

//Generic Delayed Interventions
const CString szGDAccountLookUpFailed		= _T("Account Lookup Failed");			// CRD 557225
const CString szGDGetAccountDetailsFailed	= _T("Get Account Details Failed");		// CRD 557225


#endif // _CustomerDefsh
