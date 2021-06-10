/*
-------------------------------------------------------------------------------

 File Name	   : TransBroker.H
 Type			: Application include file for SCOT TB

 Language		: MSVC++
 Operating System: Win32
 Author/Location : SCOT 2.0 Team, RSG-Atlanta

 Copyright (C) NCR Corporation 1998

-------------------------------------------------------------------------------
 Date	| Changes Made									  | Version | By
-------------------------------------------------------------------------------
 10/27/98  Original version									01.00.00  NCR
-------------------------------------------------------------------------------

 This include file can be used by TB provider if _TRANSBROKER_PROVIDER_ is
 defined.

-------------------------------------------------------------------------------
$Archive: /Fastlane/4.4 Patch G/Development/Core/Source/Include/TransBroker.h $
$Revision: 13 $
$Date: 8/12/09 2:35p $
-------------------------------------------------------------------------------
*/

#ifndef _TRANSBROKER_H_
#define _TRANSBROKER_H_

#include	"SCOTMsg.h"
#include    "DevicePropertyChanges.h"
#define SCAN_SDT_RSS 502  //This is a temporily fix for RSS barcode since OPOS doesn't support RSS// RFQ 7477
#ifdef _CPPUNIT
class CDescriptorList;
#endif
//Begin RFC 412256
/* 
  SA250050 - Remove non-OPOS SCAN_SDT_RSS value '502' and replace with the actual OPOS compliant scan type to prevent 
  'out-of-range' errors which are caused by passing an invalid barcode type (i.e. 502) to the TB and sales applications. 
  UnifiedPOS v1.8 adds SCAN_SDT_RSS14 and SCAN_SDT_RSS_EXPANDED which are later changed in UnifiedPOS v1.12 respectively 
  to SCAN_SDT_GS1DATABAR and SCAN_SDT_GS1DATABAR_E. We will use the latter and their values.

#define SCAN_SDT_RSS 502  //This is a temporily fix for RSS barcode since OPOS doesn't support RSS
						   //scan type yet. This line need be removed after OPOS supports RSS scan type in 
						   //"OposScan.h"  //tar 247406
*/   

// tn. Comment out the following two lines since they are defined in the Oposscan.h already (10/18/2011)

//#define SCAN_SDT_GS1DATABAR    131   // GS1 DataBar Omnidirectional (normal or stacked), prev SCAN_SDT_RSS14 
//#define SCAN_SDT_GS1DATABAR_E  132   // GS1 DataBar Expanded (normal or stacked), prev SCAN_SDT_RSS_EXPANDED

//End RFC 412256

#ifdef   __cplusplus
extern   "C" {
#endif   /* __cplusplus */

#ifdef  _TRANSBROKER_PROVIDER_
   #define  APIExpose   __declspec(dllexport)
#else
   #define  APIExpose   __declspec(dllimport)
#endif

/*<<<Enums>>>----------------------------------------------------------------*/
typedef enum
{
	TB_AT_CREDIT = 1,
	TB_AT_DEBITSAVING,
	TB_AT_DEBITCHECKING,
	TB_AT_DEBIT,
	TB_FT_ATMCASH
} HCACT;

typedef enum
{
    TB_FT_TENDER = 1,
    TB_FT_ITEMSALE,
    TB_FT_SUSPEND,
    TB_FT_DEBITACCOUNTTYPE,
    TB_FT_EXTENDEDDETAILS,
    TB_FT_TRAININGMODE,
    TB_FT_QUICKITEMENTRY,         // TB supports quick item entry
    TB_FT_ENDORSEMENT,            // TB supports document endorsements
    TB_FT_SECURITYLEVEL,          // TB supports security levels
    TB_FT_MICR,                   // TB supports MICR checks
    TB_FT_CASHDRAWER,             // TB supports cash drawer notifications
    TB_FT_AMSTATUS,               // TB supports assist-mode status messages
    TB_FT_SUPPORT_PIM,            // TB supports Post Item Modification, this feature allows to read szItemID from SItemDetails
    TB_FT_PHRASE_AUDIO,           // TB allows general audio at this time
    TB_FT_SECURITY_AUDIO,         // TB allows security-related audio at this time
    TB_FT_PRICE_AUDIO,            // TB allows price audio at this time
	TB_FT_SIGNONRESULTS,		  // TB allows multiple sign-on results (pass, fail, unknown)
    TB_FT_CASHOPERATION,          // TB allows cash management operation at this time          

    // ***** Core Application developers add new features above this value.  TB developers after this value. *****
    TB_INTEGRATION_FEATURES= 10000,    

} TBFEATURE;

typedef enum
{
	TB_KEYLOCK_UNKNOWN = 0, // Invalid position
	TB_KEYLOCK_L,           // Locked
	TB_KEYLOCK_N,           // Normal/Regular
	TB_KEYLOCK_S,           // Supervisor/Manager
	TB_KEYLOCK_X            // Exception (Diagnostic)
} TBKEYLOCK;

typedef enum
{
	TB_CLOSED = 1,
	TB_CMOS_DOSCLOSE,
	TB_CMOS_DOSOPEN,
	TB_CMOS_DOSREAD,
	TB_CMOS_DOSSEEK,
	TB_CMOS_DOSWRITE,
	TB_CMOS_PURGE,
	TB_COMPLETEPRINT,
	TB_COUPONEXCEEDED,
	TB_COUPONNOTMATCH,
	TB_IGNORE,
	TB_INVALIDITEMVOID,		// not in use	
	TB_ITEMQTYEXCEEDED, // this is a threshold to ask for confirmation - not a per item limit
	TB_ITEMRECALLED,
	TB_ITEMSOLD,
	TB_ITEMUNKNOWN,
	TB_LOYALTYCARD,
	TB_LOYALTYINVALID,
	TB_NEEDOVERRIDE,
	TB_NEEDPRICE,
	TB_NEEDQUANTITY,
	TB_NEEDTARE,
	TB_NEEDWEIGHT,
	TB_OPTIONLOADING,
	TB_PRINTIMMEDIATE,		// not in use
	TB_PRINTIMMEDIATECUT,	// not in use
	TB_READY,
	TB_SECURED,				// not in use
	TB_TENDERACCEPTED,
	TB_TENDERDECLINED,
	TB_TOTAL,
	TB_TRXEND,
	TB_TRXLIMITEXCEEDED,
	TB_TRXVOIDED,
	TB_TRXVOIDEXCEEDED,
	TB_UNKNOWN,
	TB_VOIDLIMITEXCEEDED,
	TB_VOIDNOTMATCH,
	TB_VOIDITEMBEFORECOUPON,
	TB_TRXSUSPENDED,
	TB_TRXSUSPENSIONUNCOMPLETED,
	TB_TENDERDECLINEDKEEPCARD,	// Used for Motorized Card Reader only
	TB_CASHWAUTHORIZED,			// not in use
	TB_CASHWNOTAUTHORIZED,		// not in use
	TB_CASHWCONFIRMED,			// not in use
	TB_CASHWNOTCONFIRMED,		// not in use
	TB_CASHWCANCELLED,			// not in use
	TB_INVALIDPIN,
	TB_EPSOFFLINE,
	TB_EPSSODOK,	// SOD: start of day
	TB_EPSSODNOK,
	TB_EPSEODOK,	// EOD: end of day
	TB_EPSEODNOK,
	TB_TENDERFOODSTAMPINSUFFICIENT,
	TB_FOODSTAMPELIGIBLETOTAL,
	TB_ITEM_QUANTITY_RESTRICTED, // this is a per item limit - not the same as the threshold for getting confirmation
	TB_VISUAL_VERIFY,
	TB_TIMERESTRICTEDITEM,
	TB_TENDERCASHBENEFITINSUFFICIENT,
	TB_ITEMSALECOMPLETED,            // if the item sold is complete. SCOT will enable scanner when it receives this state
	TB_INVALIDAMOUNT,
	TB_EPSONLINE,
	TB_NEWENCRYPTIONKEY,
	TB_INTTRXREPLY,
	TB_ATMPOWERUP,
	TB_ATMOFFLINE,
	TB_ATMONLINE,
	TB_ATMEJPRINTCOMPLETE,
	TB_TENDEREBTINSUFFICIENT,
	TB_MESSAGESCREEN,
	TB_ENTEREDTRAINING,
	TB_LEFTTRAINING,
	TB_TENDERCANCELLED,
	TB_CUSTOMERMESSAGE,			// optional: TB implements this state to support consumer marketing 
	TB_REWARDLINE,
	TB_CRATEABLEITEM,			// to ask the customer if the item is in a crate or not 
	TB_NEEDWEIGHT_BIZERBA,
	TB_SENDMESSAGETOSCOT,
	TB_TRXEND_PRINTANDDISPENSE,
	TB_CLOSED_NOPRINTANDNODISPENSE,
    TB_NEEDMOREDATA,
   TB_NEEDMICR,
   TB_ENDORSEDOCUMENT,
   TB_NEWCASHDRAWERSTATE,
   TB_NEWSECURITYLEVEL,
   TB_DOCUMENTNUM,
   TB_COMPLETEPRINTANDCUT,      // similar to tb_completeprint but issues cut command after receipt printing
   TB_ASSISTMODESTATUSCHANGED,  // Notification to FastLane to check assist mode status.
   TB_PRINTER_VERIFY,
   TB_CREATE_REPORT_ENTRY,		// New TB state for Report FastLane 3.1 Feature
   TB_ASSISTMODETABFLUSH,		// Tar 218330 used with possible flush button in AM
   TB_GETBUSINESSDATE, 
   TB_HOSTOFFLINE,              // TB state for host server offline
   TB_HOSTONLINE,               // TB state for host server online
   TB_CHANGESTATE,              // TB is initiating a state change
   TB_OUTOFSYNC_STOP,
   TB_OUTOFSYNC_ASSISTMODE,
   TB_TENDERVOIDED,	//RFC 275377 State for voiding tender
   TB_LANECLOSED,
   TB_LANEOPEN,
   TB_REPRINTRECEIPT,
   TB_REPRINTRECEIPTFAILED,
   TB_PREFERENCEDATA,                
   TB_PRINTLINE,
   TB_NEEDSIGNATURE,
   TB_NEEDSIGANDCONFIRM,
   TB_CLEARSLIPDATA,
   TB_NEEDSIGNSLIP,
   TB_CASHMANAGEMENTCOMPLETED,
   TB_TENDERSTARTED, // (*) SR797 ====================
   TB_READY_TO_TENDER, //111 KRFL-135
   TB_READY_FOR_ITEM,  //112 KRFL-135
   TB_GENERIC_DELAYED,
   TB_GENERIC_DELAYED_COMPLETE,
   TB_SENDEODCASHREPORT,
   TB_INTEGRATION_STATES = 10000, //** Core Application add new TBStates above this value.  TB developers after this value.
   TB_MAXSTATEID				// please add basic TB states above this ID 
} TBSTATE;
/* When Adding new TBStates,  please make sure Scotrans can compile.  It reads this file to generate a state name list. */

typedef enum
{
	TB_TENDER_COUPON = -2,
	TB_TENDER_OTHERPAYMENT = -1, 
	TB_TENDER_NONE = 0,
	TB_TENDER_CASH = 1,      // 1
	TB_TENDER_CREDIT,        // 2
	TB_TENDER_DEBIT,         // 3
	TB_TENDER_LOYALTY,       // 4
	TB_TENDER_DEBITSAVING,   // 5
	TB_TENDER_DEBITCHECKING, // 6
	TB_TENDER_EBT,           // 7
	TB_TENDER_FOODSTAMPS,    // 8
	TB_TENDER_GIFTCARD,      // 9
	TB_TENDER_PIP_GENERIC,        // 10 Partial Intergrated Pinpad (PIP) tenders
	TB_TENDER_PIP_CREDIT,         // 11
	TB_TENDER_PIP_DEBIT,          // 12
    TB_TENDER_PIP_DEBIT_CASHBACK, // 13
	TB_TENDER_PIP_LOYALTY,        // 14
	TB_TENDER_PIP_DEBITSAVING,    // 15  
	TB_TENDER_PIP_DEBITCHECKING,  // 16
	TB_TENDER_PIP_EBT,            // 17
	TB_TENDER_PIP_FOODSTAMPS,     // 18
	TB_TENDER_PIP_GIFTCARD,       // 19
	TB_TENDER_USERDEFINED1,       // 20 Added for customer specific use
	TB_TENDER_USERDEFINED2,       // 21 
	TB_TENDER_USERDEFINED3,       // 22
	TB_TENDER_USERDEFINED4,       // 23 
	TB_TENDER_USERDEFINED5,       // 24 
	TB_TENDER_USERDEFINED6,       // 25
	TB_TENDER_USERDEFINED7,       // 26
	TB_TENDER_USERDEFINED8,       // 27
	TB_TENDER_USERDEFINED9,       // 28
	TB_TENDER_USERDEFINED10,      // 29
	TB_TENDER_USERDEFINED11,      // 30 Added for customer specific use
	TB_TENDER_USERDEFINED12,      // 31 
	TB_TENDER_USERDEFINED13,      // 32
	TB_TENDER_USERDEFINED14,      // 33 
	TB_TENDER_USERDEFINED15,      // 34 
	TB_TENDER_USERDEFINED16,      // 35
	TB_TENDER_USERDEFINED17,      // 36
	TB_TENDER_USERDEFINED18,      // 37
	TB_TENDER_USERDEFINED19,      // 38
	TB_TENDER_USERDEFINED20,      // 39
    TB_TENDER_CHECK,              // 40
	TB_TENDER_CASH_RAP,           // 41 Added for RAP Tendering
	TB_TENDER_ASSISTTENDER,       // 42 Tar 207043 Feng 20020611
	TB_TENDER_VOUCHERPAYMENT,     // 43 RFC 213717
	TB_TENDER_CHECK_LANE,		  // 44 RFC 356020 // RFQ 7477
	TB_TENDER_CHECK_LANE_RAP,		  // 45 RFC 356020 // RFQ 7477
	TB_LAST_TENDERTYPE = 100      // Please add additional tender type above this ID
} TBTENDERTYPE;


typedef enum
{
	LC_INVALID = 0,
	LC_VALID,
	LC_INSERTED_BEFORE,
	LC_CARDDATA_MISSING		// possible card misread
} TBLOYALTYCARDTYPE;

typedef enum
{
  TB_ANSI = 0,            // Also known as ECB mode
  TB_DIEBOLD
} TBPINBLOCKFORMAT;

typedef enum
{
  TB_PINSYSTEM_MS = 0,     // Master/Session
  TB_PINSYSTEM_DUKPT
} TBPINPADSYSTEM;

typedef enum
{
  TB_PINENCODING_COMPATIBLE = 0,     // Old-style compatability mode - binary if M/S,
                                     // if Dukpt, pinblock is binary, sequence# is ASCII
  TB_PINENCODING_ASCII
} TBPINENCODING;

typedef enum
{
	TB_STARTOFDAY_NONE,
    TB_STARTOFDAY_NEEDSTART,
	TB_STARTOFDAY_READY
}TBSTARTOFDAYSTATE;

typedef enum
   {
   TB_AMSTATUSMAYEXIT = 1,    // Operator may exist assist mode.
   TB_AMSTATUSMAYNOTEXIT, // , // Operator may not exit assist mode.
   TB_AMSTATUSMAYENTER,    // KRFL-387 Operator may enter assist mode.
   TB_AMSTATUSMAYNOTENTER  // KRFL-387 Operator may not enter assist mode.
   }TBASSISTMODESTATUS;


typedef struct
{
    LPTSTR sBarCode;      
    long   lBarCodeType;  

} SMultiLabelBarCodeDetails;
typedef SMultiLabelBarCodeDetails* PSMultiLabelBarCodeDetails;

typedef  enum
{
	ITEMUPDATE_NONE = 0,
	ITEMUPDATE_QTY = 0x0001,    //These values will be logically OR'd for update values
	ITEMUPDATE_PRC = 0x0002,
} ITEM_UPDATE;

typedef enum
{
    TBCMD_ASSISTMENUSET = 1,
    TBCMD_CMDSTRING = 2,   
	TBCMD_REPRINTRECEIPT = 3,
	TBCMD_BALANCE = 4,
	TBCMD_LOAN = 5,
	TBCMD_PICKUP = 6,
	TBCMD_CUSTOMERLANGUAGE = 7,
    TBCMD_CARDDISPENSER = 901,
    TBCMD_GETCASHMANAGEMENTINFO = 902, 
	TBCMD_PRINTOPERATORBARCODE = 903,
    // ***** Core Application developers add new TBCMDs above this value.  TB developers after this value. 
    TBCMD_INTEGRATION_COMMANDS = 10000, 

    //Add new command IDs above this line
    TBCMD_LAST,
} TBCMD_ID;

typedef struct
{
	long    lUnitPrice;
	long    lExtPrice;			// add extended price
	long    lItemScanCodeType;	// 0 - manually entered, any other value - as per the OPOSScan.h standard definition
	LPCTSTR	szOriginalCode;		// original code is the code got from scanner or the code entered by user
	int		nCrateableScreenAnswer;	// Tri-state:If customer answers No=0; Yes=1; Never entered screen = 2;
	int		nVoidItemIndex;
	bool    bPickListItem;
	LPCTSTR szItemID;			// unique item id provided by TB, returned to TB in case of void
    int     nMultiLabelBarcodes;
    PSMultiLabelBarCodeDetails psMLBCDetails;
	int     lUpdateFields;      // Fields that are being updated, bitwise combination of ITEM_UPDATE values
} SEnterItemDetails;  /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */


/*<<<Typedefs>>>-------------------------------------------------------------*/
typedef long TBRC;

typedef struct
{
	long   lTaxDue;			      /* Tax due for a transaction (monetary unit) */
	long   lTotal;			      /* Total for a transaction (monetary unit)   */
	long   lDiscount;			  /* Total discount for a transaction		   */
	LPTSTR szErrDescription;	  /* Textual description  of the error	  	   */
	long   lErrDescriptionLength; /* Lenght of error description			   */
	long   lFoodStampDue;		  /* Food Stamp Eligible Amt for a transaction */
	long   lRewardPoints;		  /* Total reward points earned for this transaction */
    int	   nVersion;    // SR797  /* Version	                               */
	long   lBalanceDue;	// SR797  /* Balance Due                               */	    
} STotalDetails;  /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */

typedef struct
{
	LPTSTR szDescription;		  /* Tender description                        */
	long   lDescriptionLength;	  /* Length of the tender description buffer   */
	long   lTender;			      /* Tender amount submitted by the customer   */
	long   lBalance;			  /* Transaction total? (monetary unit)		   */
	long   lChange;			      /* Change due to the customer (monetary unit)*/
	LPTSTR szErrDescription;	  /* Textual description  of the error		   */
	long   lErrDescriptionLength; /* Lenght of error description			   */
    TBTENDERTYPE nTenderType;     /* Tender type                               */
} STenderDetails;  /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */


/*  WARNING - Scotapp/TB backward compatability dependant on SItemDetails definition */ 
typedef struct
{
	LPTSTR szDescription;		  /* Item description						   */
	long lDescriptionLength;	  /* Length of the description buffer		   */
	LPTSTR szItemCode;			  /* Item code								   */
	long lExtPrice;			      /* Extended price from item sale print line  */
	long lUnitPrice;			  /* Unit price for quantity or weight		   */
	long lQuantity;			      /* Quantity of the item					   */
	long lWeight;				  /* Weight of the item (weight unit)		   */
	long lDealQuantity;		      /* Last deal quantity of the items MGG (?)   */
	long lItemQuantityLimit;	  /* per item quantity limit                   */
	bool fCoupon;				  /* Item is a coupon						   */
	bool fNeedQuantity;		      /* Item requires quantity					   */
	bool fNeedWeight;			  /* Item requires weight					   */
	bool fNeedPrice;			  /* Item requires price					   */
	bool fNotFound;			      /* Item was not found in the PLU database/file */
	bool fNotForSale;			  /* Item is not for sale, recalled			   */
	bool fRestricted;			  /* Item is restricted, proper age is required*/
	int  nRestrictedAge;		  /* The age limitation fot restricted items   */
	long nTareCode;			      /* The tare code for quantity items		   */
	bool fDetails;				  /* Item has detailed information			   */
	BOOL fOverride;			      /* Override is allowed                       */
	BOOL fVoid;					  /* void item or sell item                    */
	BOOL fQuantityConfirmed;      /* item high quantity has been confirmed     */
	BOOL fQuantityLimitExceeded;  /* per item quantity limit has been exceeded */
	bool fVisualVerify;
	long lScanCodeType;			  /* if item is scanned, scan type is set as per OPOSScan.h, 0 - for manual */
    bool bLinkedItem;             /* if it is linked item or not                */
    LPTSTR szErrDescription;	  /* Textual description  of the error			*/
	long lErrDescriptionLength;   /* Length of error description			    */
	long lDepartment;             /* Item's department number TAR 137490        */
	long lEntryID;				  /* unique entry id for each item sold         */
	long lRequireSecBagging;	  /* should the Security System require this item to be placed in the bagging area? -1=use current security settings; TB_OVERRIDE_SECURITY_WITH_NO=No, item not required to be placed in bagging area; TB_OVERRIDE_SECURITY_WITH_YES=yes, item is required to be placed in bagging area */
	long lRequireSecSubstChk;	  /* should the Security System perform substitution check on item & store UPC in WLDB? -1=use current security settings; TB_OVERRIDE_SECURITY_WITH_NO=No, do not perform subst chk on this item; TB_OVERRIDE_SECURITY_WITH_YES=yes, Do perform subst chk on this item */
	bool bSecurityTag;			  /* Item has a security tag					*/
	bool bPickListItem;			  /* Is item from Pick List						*/
	LPTSTR szItemID;			  /* string version of lEntryID, set either szItemID or lEntryID, this field has priority over lEntryID		*/
	bool bZeroWeightItem;         /* TB indicates if this item is zero weight item */
	LPTSTR szExtendedDeptInfo;	  /* string containing the extended department information SR 141 - RFC 283384 */
} SItemDetails;  /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */ 

// Enumeration of the Security System tolerances for overriding exception item data
// Applies to lRequireSecBagging & lRequireSecSubstChk in SITEMDETAILS structure above
typedef enum
{
	TB_LET_SECURITY_DECIDE = -1,		// -1
	TB_OVERRIDE_SECURITY_WITH_NO=2,	
	TB_OVERRIDE_SECURITY_WITH_YES,
	TB_OVERRIDE_SECURITY_WITH_NOWEIGHT,   //RFC255148
	TB_OVERRIDE_SECURITY_BYPASSBAGGINGPROMPT   //SR078
} TB_OVERRIDE_SECURITY_EXCEPTION_ITEMS;

// Crateable item screen answer enum,  No=0, Yes=1, Never entered screen=2
// Applies to nCrateableScreenAnswer in SEnterItemDetails
typedef enum
{
  CRATEABLE_ANSWER_NO,			// Customer answered no, item is not in a crate = 0
  CRATEABLE_ANSWER_YES,			// Customer answered yes, item is in a crate = 1
  CRATEABLE_SCREEN_NOT_ENTERED	// item did not take customer to the Crate-able item screen = 2
}  CRATEABLE_SCREEN_ANSWER;

// enumeration of exception processing codes
typedef enum
{
	XNONE, // Value of ExceptionCode is not to be used
    XAPPROVALTHRESHOLDS,
	XAPPROVALALWAYS,
	XAPPROVALNONE,
	XDONOTPROCESS,
	XVISUALVERIFY
} TBXCODE;

// Structure passed by TB with call to TBGetItemExtendedDetails
typedef struct
{
	LPTSTR szCategory;		// Exception category description
	TBXCODE ExceptionCode;	// Exception processing code
	DWORD dwExceptionFlags; // Other exceptions
	DWORD dwTBOptionFlags;  // For TB use only
	LPTSTR szExceptionWav;  // Wav associated with exception item
} SItemExtendedDetails;  /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */

// Structure passed by TB with call to TBGetMessageInfo
// TAR# 127247.
typedef struct
{
	LPTSTR szLeadThruText;	// Screen leadthru text
	LPTSTR szScreenText;    // Screen message text
	LPTSTR szPlayWave;      // Play wave file
	LPTSTR szE_ReceiptText; // Text to display in E-Receipt as informational line
	bool bHiliteReceiptText; // Specify whether to display text in a different color.
	bool bNeedItemCancel;   // Flag to tell core to cancel item and disable pending security
	bool bApprovalNeeded;   // Flag for Approval Needed, treated as Unknown Item approval and
                            // valid only when 'bNeedItemCancel' is TRUE; 
	bool bGoBackScanAndBag; // From Message Screen, go back ScanAndBag after OK button is pressed.
	LPTSTR szRAP_MessageText; // Text to display on e-receipt at RAP
	bool bHiliteRAPText;    // Flag to have RAP message highilighted
} SMessageInfo;       /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */
typedef struct
{
    LPTSTR szRecordType;	
    LPTSTR szValue;		  
} SCreateReportEntry; /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */
typedef enum
{
	REWARDOP_INSERT = 1,
	REWARDOP_DELETE,
	REWARDOP_NONE
} tRewardLineOpType;

typedef enum
{
	BOTTOM_OF_RECEIPT = 1,
	ABOVE_REF_ENTRYID,
	BELOW_REF_ENTRYID,
	POS_NONE
} tRewardLinePosType;


// Structure passed by TB with call to TBGetRewardLineDetails
typedef struct
{
	LPTSTR szDescription;			/* reward line's description	*/
	long   lValue;					/* reward points for this item  */
	bool   bSuppressValue;			/* show/hide reward points for this item */
	long   lEntryID;				/* unique entry id for this line item */
	long   lEntryIDForReference;	/* entry id of one of previous reward lines */
	bool   bUpdateRewardTotal;		/* update total details if this reward-line affects totals */
	long   lRewardPoints;			/* if bUpdateRewardTotal is true, this value is used to update SCOT display */
	long   lEmphasis;				/* enable or disable different line attributes, future usage */

	tRewardLineOpType	nOperatationType; /* operation to be performed on E-receipt */
	tRewardLinePosType	nPosType;		  /* position of this new reward-line */

	// end of reward details
} SRewardLineDetails;  /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */

// dwExceptionFlags values
#define TBXF_SUPPRESSPRICEDISPLAY 0x01  // Suppress display of price on electronic receipt
#define TBXF_REQUESTSUSPEND       0x02  // Request suspend  

typedef struct
{
	LPCTSTR          pchMSRData;		  /* Buffer containing the MSR data (in)                */
	long	         nMSRDataLength;	  /* Buffer length of the pchMSRData (in)               */
	TBTENDERTYPE     nAccountType;		  /* Account type  (in)                                 */
	LPTSTR           szAccountNumber;	  /* Buffer to contain the account number (out)         */
	long	         nAccountNumberLength;/* Buffer length of the szAccountNumber (out)         */
	LPTSTR           szWorkingKey;		  /* Buffer to contain the working key (out)            */
	long	         nWorkingKeyLength;   /* Buffer length of the working key buffer (out)      */
    TBPINBLOCKFORMAT nPinBlockFormat;     /* Allow TB to specify which PIN block format         */
    TBPINPADSYSTEM   nPinpadSystem;       /* Allow TB to specify which Pinpad System            */
    TBPINENCODING    nPinBlockEncoding;   /* Allow TB to specify if pinblock is binary or ascii */
    long             lUnused3;            /* reserve space for later PIN functionality          */
} SPINWorkingKey;  /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */

// Structure passed by TB with call to TBGetDataNeededView
typedef struct
{
    LPTSTR szDescription;	// leadthru text2
    LPTSTR szView;			// parameters that setup view
    LPTSTR szIniFile;
    LPTSTR szSection;
} SDataNeededView;  /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */

//Cash Management Details
typedef struct
{
	CString csType;				/* Cash operation type - loan/pickup	  				*/
	CString csStatus;			/* Cash operation status - success/fail					*/
	long   lAmount;				/* Loan/pickup amount									*/
	CString csCashList;			/* Cash list - denomination,currency,count,tender type  */      
	CString csMessages;			/* Loan/pickup error messages							*/
	CString csDestination;		/* Destination for messages					            */
	CString csCustomData;	    /* Other customer specific information           		*/
	
} SCashManagementDetails; /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */

typedef SItemDetails*         PSItemDetails;
typedef SItemExtendedDetails* PSItemExtendedDetails;
typedef STenderDetails*       PSTenderDetails;
typedef STotalDetails*        PSTotalDetails;
typedef SPINWorkingKey*       PSPINWorkingKey;
typedef SEnterItemDetails*    PSEnterItemDetails; // this strcuture is used by TBEnterItem API to get more details
typedef SMessageInfo*         PSMessageInfo;
typedef SCreateReportEntry*   PSCreateReportEntry;
typedef SRewardLineDetails*   PSRewardLineDetails;
typedef SDataNeededView*      PSDataNeededView;
typedef SCashManagementDetails* PSCashManagementDetails;

typedef struct
{
	long   lAvailBal;				/* Available Balance*/
} SEBTAvailBalance;  /*  !!!WARNING!!! - Scotapp/TB backward compatability dependancy. Do not change/reorder structure */

typedef SEBTAvailBalance*   PSEBTAvailBalance;

/*<<<Return Statuses>>>------------------------------------------------------
//	Transaction Broker return code ranges :
//	-------------------------------------
//	Positive	Successful or warning errors
//	-1 to -99	Operational errors
//	-100 to -199	Programming errors - should be fixed in the application code
//	-666 and below	Unexpected errors
*/

const TBRC  TB_SUCCESS_IMMEDIATE	  = 1;
const TBRC  TB_SUCCESS				  = 0;
const TBRC  TB_E_INVALIDUSERID		  = -2;
const TBRC  TB_E_INVALIDUSERPASSWORD  = -3;
const TBRC  TB_E_INVALIDSTATE		  = -4;
const TBRC  TB_E_FILENOTFOUND		  = -5;
const TBRC  TB_E_INCOMPLETEPROCEDURE  = -6;
const TBRC  TB_E_INVALIDWINDOW		  = -7;
const TBRC  TB_E_DISKFULL			  = -8;
const TBRC  TB_E_INVALIDPARAM		  = -9;
const TBRC  TB_E_NOHOSTCREDIT		  = -10;
const TBRC  TB_E_NOWORKINGKEY		  = -11;
const TBRC  TB_E_FUNCTIONNOTSUPPORTED = -12;
const TBRC  TB_E_ITEMFAILED           = -13;	// return this code for TBEnterItem() if TB cann't process that item
const TBRC  TB_E_CARDDATA_MISSING     = -14;	// return this code if a card data is not complete w.r.t TB

const TBRC  TB_E_NOTINITIALIZED		  = -100;
const TBRC  TB_E_NOHOST				  = -101;
const TBRC  TB_E_UNEXPECTED			  = -666;

const long INVALID_ENTRYID  = LONG_MIN;

/*<<<Transaction Broker APIs>>>----------------------------------------------*/

#ifdef _UNICODE  // Please note the API signature change as needed for unicode
APIExpose TBRC TBEnterTender(TBTENDERTYPE nTenderType,
					 const LONG lAmount, LPCTSTR szCardData,
					 const int nPINLength, LPCTSTR szEncryptedPIN,
					 const BOOL fOverride, const BOOL fVoid);

APIExpose TBRC TBGet2x20(LPCTSTR szDisplayStr);

APIExpose TBRC TBSendMsg2Host(	// send any message to host application. "Use this function only when TB API does not address your issue."
					LPCTSTR		szSend,				// start of message bytes to send/write, NULL for DWO reads
					const int	iMsgLen,			// number of message bytes to send or DWO reads/writes
					const TBDEVICEID nDeviceClass,	// device to send to DEV_...
					const UCHAR	nFunctionCode = 0,	// fucntion code IOM_FNC_...
					const long	lOptFileHandle = 0,	// response message option for printer messages
					const bool  fOriginatePipeMsg = true);	// set it to false if you want to send a monitor responseAPIExpose TBRC TBSetMonitorAll(BOOL fMonitor);

APIExpose BOOL TBValidateOperator(LPCTSTR szOperatorID, LPCTSTR szOperatorPw);
APIExpose TBRC TBSendSigData(LPCTSTR CustomerName, LPCTSTR cSigData,  DWORD nLength);
#else
// ANSI signature of the above APIs
APIExpose TBRC TBEnterTender(TBTENDERTYPE nTenderType,
					 const LONG lAmount, LPCTSTR szCardData,
					 const int nPINLength, LPCSTR szEncryptedPIN,
					 const BOOL fOverride, const BOOL fVoid);

APIExpose TBRC TBGet2x20(LPCSTR szDisplayStr);

APIExpose TBRC TBSendMsg2Host(	// send any message to host application. "Use this function only when TB API does not address your issue."
					LPCSTR		szSend,				// start of message bytes to send/write, NULL for DWO reads
					const int	iMsgLen,			// number of message bytes to send or DWO reads/writes
					const TBDEVICEID nDeviceClass,	// device to send to DEV_...
					const UCHAR	nFunctionCode = 0,	// fucntion code IOM_FNC_...
					const long	lOptFileHandle = 0,	// response message option for printer messages
					const bool  fOriginatePipeMsg = true);	// set it to false if you want to send a monitor responseAPIExpose TBRC TBSetMonitorAll(BOOL fMonitor);

APIExpose BOOL TBValidateOperator(LPCTSTR szOperatorID, LPCSTR szOperatorPw);
APIExpose TBRC TBSendSigData(LPCTSTR CustomerName, LPCSTR cSigData,  DWORD nLength);
#endif


APIExpose TBRC TBCancelTransaction(const BOOL fOverride);
APIExpose TBRC TBCopyFileFromServer(LPCTSTR szFileName, LPCTSTR
						   szDestinationFilePath, const BOOL fForce);
APIExpose TBRC TBCopyFileToServer(LPCTSTR szFileName, LPCTSTR szSourceFilePath);
APIExpose TBRC TBEnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge);
APIExpose TBRC TBSendWeight(long lWeight);
APIExpose TBRC TBEnterItem(LPCTSTR szItemCode,
						   const BOOL fVoid, const BOOL fOverride,
						   const long nQuantity, const long nWeight,
						   const long nTareCode, const long lPrice, 
						   const BOOL fQuantityConfirmed, PSEnterItemDetails psMoreDetails);
APIExpose TBRC TBEPSStartOfDay(LPVOID lp);
APIExpose TBRC TBEPSEndOfDay(LPVOID lp);
APIExpose TBRC TBFinish(void);
APIExpose TBRC TBGetItemDetails(PSItemDetails psItemDetails);
APIExpose TBRC TBGetItemExtendedDetails(PSItemExtendedDetails psItemExtendedDetails);
APIExpose long TBGetTransactionLimit(void);
APIExpose long TBGetQuantityLimit(void);
APIExpose HFILE TBGetJournalData(void);
APIExpose TBRC TBGetPINWorkingKey(PSPINWorkingKey psPINWorkingKey);
APIExpose HFILE TBGetReceiptData(void);
APIExpose HFILE TBGetSlipData(void);
APIExpose TBRC TBGetTenderDetails(PSTenderDetails psTenderDetails);
APIExpose long TBGetTerminalNumber(void);
APIExpose long TBGetCurrentTransactionNumber(void);
APIExpose TBRC TBGetCurrentBusinessDate(CString &csBusinessDate);
APIExpose TBRC TBGetTotalDetails(PSTotalDetails psTotalDetails);
APIExpose TBRC TBParseFoodStampTotal(PSTotalDetails psTotalDetails);
APIExpose long TBGetVersion(LPTSTR szVersionString, long nBufferLength);
APIExpose TBRC TBInitialize(HWND hWnd, LPCTSTR szFilePath);
APIExpose BOOL TBIsAllowed(TBFEATURE nFeature);
APIExpose TBRC TBKeyPress(const USHORT uKey);
APIExpose TBRC TBLoadOptions(void);
APIExpose TBRC TBLogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
						   const int nComponentID, LPCTSTR szComponentID,
						   LPCTSTR szModuleID, const int nModuleLineNumber);
APIExpose TBSTATE TBProcessMessage(PSMessageElement psMessage);
APIExpose TBRC TBRefreshState(void);
APIExpose TBRC TBReleaseJournalData(HFILE hJournalData);
APIExpose TBRC TBReleaseMessage(PSMessageElement psMessage);
APIExpose TBRC TBReleaseReceiptData(HFILE hReceiptData);
APIExpose TBRC TBReleaseSlipData(HFILE hSlipData);
APIExpose TBRC TBResumeTransaction(LPCTSTR szSuspendID);
APIExpose TBRC TBSetMonitorAll(BOOL fMonitor);
APIExpose TBRC TBStart(LPCTSTR szOverrideCode);
APIExpose TBRC TBSuspendTransaction(const BOOL fOverride);
APIExpose TBRC TBUninitialize(void);

APIExpose BOOL TBValidOperator(LPCTSTR szOperatorID);
APIExpose BOOL TBValidPassword(LPCTSTR szOperatorPw);

APIExpose TBLOYALTYCARDTYPE TBVerifyLoyaltyCard(LPCTSTR szCardData, int nDataLen);
APIExpose TBRC TBGetAvailableFoodStampAmount(PSEBTAvailBalance psEBTAvailBalance);
APIExpose TBRC TBGetAvailableBenefitAmount(PSEBTAvailBalance psEBTAvailBalance);
APIExpose TBRC TBRequestFoodStampEligibleTotal();
APIExpose TBRC TBSendSignOn(void);
APIExpose TBRC TBSendSignOff(void);
APIExpose TBRC TBSendLockOn(void);						// set the manager's key lock ON
APIExpose TBRC TBSendLockOff(void);						// set the manager's key lock OFF
APIExpose TBRC TBInitHostAppExit(void);
APIExpose TBRC TBLaneClosed(const bool bEnter);   // Enter or Leave lane Closed state
APIExpose TBRC TBEnterTenderState(const bool bEnter);   // Enter or Cancel Tender status
APIExpose TBRC TBSetTrainingModeState(const bool bTMState);   // entering or leaving training mode  
APIExpose TBRC TBSetBusinessDate(LPCTSTR szSetBusinessDate);                                                                                                           
APIExpose TBRC TBGetMessageInfo(PSMessageInfo psMessageInfo);  
APIExpose TBRC TBCreateReportEntry(PSCreateReportEntry psCreateReportEntry);  
APIExpose TBRC TBGetCustomerMessage(LPTSTR* psCustomerMessageText, LPTSTR* psCustomerMessageAudio);
APIExpose TBRC TBGetRewardLineDetails(PSRewardLineDetails psRewardLineDetails);
APIExpose TBRC TBNeedStartOfDay(TBSTARTOFDAYSTATE &eState);
APIExpose TBRC TBPrint(LPCTSTR szPrintFile);
APIExpose TBRC TBSuspendTransactionEX(const BOOL fOverride, LPCTSTR szSuspendReasons);
APIExpose TBRC TBGetPrinterError(LPTSTR* psErrorMessage);
APIExpose TBRC TBGetDataNeededView(PSDataNeededView psDataNeededView);
APIExpose TBRC TBSendDataNeeded(LPCTSTR szDataNeededDetails);
//+dp185016 
APIExpose TBRC TBSyncTBorPOS();
//-dp185016

APIExpose BOOL TBValidateHeadCashier( LPCTSTR szCashierID, LPCTSTR szCashierPw ); //dp185016 support glory device
// Remote Tendering-related devices:

APIExpose TBRC TBEnterMICR(
   LPCTSTR szRawData,         // Raw MICR data from the device.
   LPCTSTR szTransitNumber,   // Bank Transit Number
   LPCTSTR szAccountNumber,   // Customer Account Number
   LPCTSTR szBankNumber,      // ABA Bank number
   LPCTSTR szSerialNumber,    // Check document number
   LPCTSTR szEPC,             // Extended Processigng Code
   long lType,                // Type (1=Personal, 2=Business, 99=Unknown)
   long lCountryCode);        // Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)

APIExpose TBRC TBGetDocEndorsementInfo(
   long *plNumRemaining,      // Number of endorsements reamining.
   LPTSTR *ppszDocDescription,// Operator prompt or document description.
   LPTSTR *ppszDocFront,      // Text to print on front of document.
   LPTSTR *ppszDocBack);      // Text to print on back of document.

APIExpose TBRC TBSetCashDrawerState(
   long lState);               // =Bitmapped (Set=Open, Clear=Closed - lsb is drawer 1, etc.)

APIExpose TBRC TBGetExpectedCashDrawerState(long *plExpectedState);     // Bitmapped (lsb is drawer 1, etc.)

APIExpose TBRC TBSetSecurityLevel(
   long lSecurityLevel);      // Security level set by keylock or other auth means.
                              // 0 - Unknown or invalid key position.
                              // 1 - Locked("L")
                              // 2 - No Manager ("R" or "N")
                              // 3 - Manager Override("S")
                              // 4 - diagnostic("X" or "Ex")

APIExpose TBRC TBGetExpectedSecurityLevel(long *plExpectedLevel);

APIExpose TBRC TBGetCurrentSecurityLevel(long *plCurrentLevel);

APIExpose TBRC TBEndorsementCompleted(
   bool bSuppressSubsequent = false);  // Flag to indicate to TB not to fire addl.
                                       // TB_ENDORSEDOCUMENT messages.

APIExpose TBRC TBGetAssistModeStatus(
   long *plAssistModeStatus);          // Pointer to long to receive assist mode status.

APIExpose TBRC TBGetMICRPrompt(LPTSTR *ppszMICRPrompt);

APIExpose TBRC TBGetLoyaltyCardNumber(LPTSTR *pszLoyaltyCardNumber);
APIExpose BOOL TBDisableTenderButton(TBTENDERTYPE nTenderType);
// End Remote Tendering-related devices:
APIExpose TBRC TBGetGiftCardBalance(long *plBalance);
APIExpose TBRC	TBGetExtensionStateSetup(LPTSTR szUICtx, LPTSTR szDevCtx, psPropChangeRequestList pExtChangeList);
APIExpose TBSTATE TBNotifyExtensionStateEvent(psExtStateEventNotifyIn pEventNotifyIn, psExtStateEventNotifyOut pEventNotifyOut);
APIExpose TBRC TBGetNextState(LPCTSTR lpLastStateName, 
                              LPCTSTR lpNextStateName, 
                              bool *pbOverride, 
                              LPTSTR *lpOverrideStateName, 
                              CDescriptorList &dlParameterList);
APIExpose TBRC TBPreprocessData(LPCTSTR lpDataString, LPCTSTR lpEventName, bool *pbAllowDefaultProcessing);
APIExpose TBRC TBLogReceipt(LPTSTR* lpFileName,BSTR* bstrSignatureString, LPTSTR* lpReceiptType);
APIExpose TBRC TBPreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData);
APIExpose TBRC TBPowerShutdown(void); //dp185016 rfc 265463
APIExpose TBRC TBSendTBCommand(long lDataID, va_list pArgs);
APIExpose TBRC TBTenderSelected(TBTENDERTYPE nTenderType); // (*) SR797 ====================
// +RFQ 7477
//Begin RFC 356020
APIExpose TBRC TBEnterMICRWithType(
   TBTENDERTYPE nTenderType,
   LPCTSTR szRawData,         // Raw MICR data from the device.
   LPCTSTR szTransitNumber,   // Bank Transit Number
   LPCTSTR szAccountNumber,   // Customer Account Number
   LPCTSTR szBankNumber,      // ABA Bank number
   LPCTSTR szSerialNumber,    // Check document number
   LPCTSTR szEPC,             // Extended Processigng Code
   long lType,                // Type (1=Personal, 2=Business, 99=Unknown)
   long lCountryCode);        // Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)
//End RFC 356020
// -RFQ 7477
// End Remote Tendering-related devices:

#ifdef   __cplusplus
}		/* End of extern "C" { */
#endif	/* __cplusplus		 */

#endif   /* _TRANSBROKER_H_	 */

