/////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SCUDEFS.H   
//
// SCOT CUSTOMER DEFINED CONSTANTS
//
// CHANGE HISTORY:
//
// POS33020 Work Request:52635 Name: Matt Condra   Date:November 16, 2012
// POS39475 Work Request:52635 Name: Matt Condra   Date:November 10, 2012
// POS36319 Work Request:50864 Name:Robert Susanto Date:June 20, 2012
// POS30981/POS31054 WORK REQUEST:50864 Name: Robert Susanto Date: May 23, 2012
// POS30981/POS31050 WORK REQUEST:50864 Name: Robert Susanto Date: April 24, 2012
// POS19072 WORK REQUEST:16656 Name: Matt Condra Date: November 23, 2011
// POS18905 WORK REQUEST:16656 Name: Robert Susanto Date: October 10, 2011
// POS11024 Work Request:16632 Name:Sandy Reedy Date:July 14, 2011
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCUDEFS_	// SCOT CUSTOMER DEFINED CONSTANTS
#define _SCUDEFS_


const enum SCUD_BMPS
{
	SSF_BMP1 = SCUD_BASE+1,
	SSF_BMP2,
	SCUDBMPS_LAST
};

const enum SCUD_MSGS
{
	SSF_LTT_INVALID_LOGIN_CONFIG = SCUD_BASE+1,         //9000 + 1
    SSF_TXT_INVALID_LOGIN_CONFIG,                       //2
    SSF_NEEDASSIST_DATAENTRY,                           //3
	SSF_NEEDASSIST_TOPCAPTION,                          //4
	SSF_NEEDASSIST_BOTTOMCAPTION,                       //5
	SSF_NEEDASSIST_SUMMARYINSTRUCTIONS,                 //6
	SSF_TBTXT_PLEASE_TRY_AGAIN,                         //7
	SSF_TBTXT_INVALID_SIGNATURE,                        //8
	SSF_TBTXT_CANT_READ_CARD,                           //9
	SSF_TBTXT_FREQ_SHOPPER_ACTIVATED,                   //10
	SSF_TBLTTXT_FREQ_SHOPPER_ACTIVATED,                 //11
	SSF_LTT_INVALID_TRAINING_LOGIN_CONFIG,              //12
	SSF_TXT_INVALID_TRAINING_LOGIN_CONFIG,              //13
	SSF_LTT_DUPLICATE_TRAINING_LOGIN,                   //14
	SSF_TXT_DUPLICATE_TRAINING_LOGIN,                   //15
	SSF_TXT_COLLECTNEGATIVEMEDIA,						//16
	
	SSF_BTT_PRINTGIFTRECEIPT = SCUD_BASE+50,

    SSF_BTT_PRINTREBATERECEIPT = SCUD_BASE+53,          // 53 - POS18905
        
    SSF_BTT_DUPLICATERECEIPT = SCUD_BASE+57,
	SSF_TXT_VOIDATTEMPT_LINE1 = SCUD_BASE+72,
	SSF_TXT_VOIDATTEMPT_LINE2 = SCUD_BASE+73,
	SSF_TXT_VOIDATTEMPT_LINE3 = SCUD_BASE+74,

	SSF_TXT_GUEST_AGE = SCUD_BASE+104,
	SSF_TXT_GUEST_NODATENEEDED = SCUD_BASE+105,

    SSF_BTT_PRICE_INQUIRY = SCUD_BASE+120,              //9120

	SSF_BTT_NEWTRANSACTION = SCUD_BASE+132,				//132
	SSF_LTT_COMPLETETRX_FINISHPAY,						//133
	SSF_LTT_SELECTOPTION_BEGIN,							//134
	SSF_TXT_SCAN_RECEIPT,								//135
	SSF_LTT_RETRIEVING_RECEIPT,							//136
	SSF_TXT_RETRIEVING_RECEIPT_WAIT,					//137
	SSF_TXT_INVALID_RECEIPT,							//138
	SSF_TXT_TRX_RETRIEVAL,								//139
	SSF_BTT_BACKTO_SHOPPING,							//140
	SSF_TXT_SUPERVISOR_SCREEN = SCUD_BASE + 149,
	SSF_LTT_SUPERVISOR_SCREEN,
	SSF_TXT_SUPERVISOR_PWD_SCREEN,
	SSF_LTT_SUPERVISOR_PWD_SCREEN,
	SSF_LTT_SUPERVISOR_INVALID,
	SSF_LTT2_SUPERVISOR_INVALID,
	SSF_LTT_INSERT_PAYMENT = SCUD_BASE+157,
	SSF_MSG_THANKYOUFORSHOPPINGMEIJER = SCUD_BASE+160,
	SSF_MSG_TAKECHANGETIMEREXPIRED = SCUD_BASE+162,
	SSF_BTT_WIC_CARD = SCUD_BASE+167,
	SSF_MSG_WIC_CARD_PROCESSING,
	SSF_MSG_SCAN_WIC_ITEMS,	

    // EMPLID & Biometric leadthrus                    
    MSG_ENTERID_SCANIDORKEY = SCUD_BASE+187,        // 9187
    MSG_ENTERID_SCANIDONLY,                         // 9188
    MSG_ENTERPASSWORD_ORUSEBIOMETRIC,               // 9189
    SSF_LTT_INCORRECTLOGIN,                         // 9190=Incorrect Login
    SSF_MSG_EMPLIDPASSWORD_INVALID,                 // 9191=\nIncorrect Employee ID and password.\n\nPlease try again.
    SSF_MSG_BADGEPASSWORD_INVALID,                  // 9192=\nIncorrect Employee badge and password.\n\nPlease try again.
    SSF_MSG_EMPLIDBIOMET_INVALID,                   // 9193=\nIncorrect Employee ID and fingerpint.\n\nPlease try again.
    SSF_MSG_BADGEBIOMET_INVALID,                    // 9194=\nIncorrect Employee badge and fingerprint.\n\nPlease try again.
    
	RESERVED_FOR_LTT_PAY_CHECK_ON_LANE = SCUD_BASE + 220,		//9220 - Begin RFC 356020 
	RESERVED_FOR_MSG_INSERT_CHECK_ON_LANE,						//9221
	RESERVED_FOR_LTT_MICR_ERR_TITLE1,							//9222
	RESERVED_FOR_LTT_MICR_ERR_TITLE2,							//9223
	RESERVED_FOR_MSG_MICR_NO_DATA,								//9224
	RESERVED_FOR_MSG_MICR_TIMEOUT,								//9225
	RESERVED_FOR_MSG_MICR_UNEXPECTED,							//9226
	RESERVED_FOR_MSG_MICR_NO_HARDWARE,							//9227
	RESERVED_FOR_MSG_MICR_JAM,									//9228
	RESERVED_FOR_MSG_MICR_LINE_ERROR,							//9229
	RESERVED_FOR_MSG_MICR_NO_CHECK,								//9230
	RESERVED_FOR_MSG_MICR_BAD_CHECK,								//9231
	RESERVED_FOR_RA_CHECK_ON_LANE,								//9232
	RESERVED_FOR_MSG_CHECK_DEVICE_NOT_AVAILABLE,					//9233
    RESERVED_FOR_MSG_MICR_CHECK_JAM,								//9234
	RESERVED_FOR_MSG_MICR_GENERAL_FAILURE,						//9235
	RESERVED_FOR_MSG_MICR_TIMEOUT_TEXT,							//9236
	RESERVED_FOR_MSG_MICR_NOT_CONNECTED,							//9237 - End RFC 356020
    
    SSF_AGE_REQUIRED = 9247,                                     //9247

    SSF_BTT_APPLYREBATECHECK        = 9293,                           // 9293 - POS18905
    SSF_BTT_APPLYREBATECHECK_YES    = 9294,                           // 9294 - POS18905
    SSF_BTT_REBATECHECK             = 9295,                           // 9295 - POS18905
    SSF_MSG_SUPERVISOR_REBATECHECK  = 9296,                           // 9296 - POS18905

    SSF_BTT_APPLYPROMOTIONALITEMS = 9298,           // POS19072: 9298=Apply Promotional Items
    SSF_LTT_APPLYPROMOTIONALITEMS = 9299,           // POS19072: 9299=Apply Promotional Items
    SSF_MSG_APPLYPROMOTIONALITEMS = 9300,           // POS19072: 9300=Are you finished entering your WIC eligible items?
    SSF_LTT_SCANPROMOTIONALITEMS  = 9301,           // POS19072: 9301=Scan Promotional Items
    SSF_MSG_NOTPROMOTIONALITEM    = 9302,           // POS19072: 9302=Item is not a promotional item and cannot be sold in this transaction. Set item aside.
    
   SSF_TXT_MOBILE_AUDIT = 9303,                          //9303   POS11024

   // POS31050 
   SSF_TXT_ENHANCEDAM_LOGOUT            = 9322,
   SSF_TXT_ENHANCEDAM_TOTALITEM         = 9323,
   SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE = 9324,
   SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE_LINE1 = 9325,
   SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE_LINE2 = 9326,
   // e POS31050 

   //POS31051
   SSF_TXT_ENHANCEDAM_TOTALREMOVEDITEM = 9327,
   //e POS31051

   //POS31054 - mobile order lane light rrs
   SSF_TXT_ENHANCEDAM_MOBILE_RESUME = 9328,
   //e pos31054
    SSF_LTT_COINSTAR_BOTTLESLIP = 9329, 			// POS36319
    SSF_TXT_COINSTAR_BOTTLESLIP = 9330, 			// POS36319

    SSF_LTT_HIGHVALUE_PROMO = 9331,             // POS39475
    SSF_TXT_HIGHVALUE_PROMO = 9332,             // POS39475

    SSF_LTT_COUPONCHOICE_SELECTION = 9333,      // POS33020 Coupon Selection
    SSF_TXT_COUPONCHOICE_MPERKS    = 9334,      // POS33020 mPerks Digital Coupon
    SSF_BTT_COUPONCHOICE_MPERKS    = 9335,      // POS33020 Continue with mPerks
    SSF_BTT_COUPONCHOICE_MPERKS2   = 9336,      // POS33020 Paper Coupon Available for Future Use
    SSF_TXT_COUPONCHOICE_VENDOR    = 9337,      // POS33020 Manufacturer/Vendor Coupon
    SSF_BTT_COUPONCHOICE_VENDOR    = 9338,      // POS33020 Continue with Paper Coupon
    SSF_BTT_COUPONCHOICE_VENDOR2   = 9339,      // POS33020 mPerks Available for Future Use
    SSF_TXT_COUPONCHOICE_INSERTCOUPON = 9340,   // POS33020 Please Enter mPerks Or Insert Coupon

	//Please add new entries above this line
	SCUDMSGS_LAST
};

// START TAR : 242837
const enum SSF_BEST // SCOT customer or SSF defined base entry states
{
	SSF_BES_SSFSTATE1 = BEST_BASE+1,
	SSF_BES_LAST
};
// END TAR : 242837

#endif //  _SCUDEFS_	// SCOT CUSTOMER DEFINED CONSTANTS
