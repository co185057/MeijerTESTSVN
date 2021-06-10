/////////////////////////////////////////////////////////////////////////////
//
// SATypes.h
//

/* ---------------------------------------------------------------------------------------- */
/* File: SATypes.h 				Purpose: Include file that defines external inputs/outputs  */
/* Version: 1.1					Author:	 Chris Malchak/Leila Parker (Original W. Perry)     */
/* Used to: define enumerated constants for 'global' and external i/o                       */ 
/* declares: no classes                                                                     */
/* Copyright (c) 1997,1999 NCR.  All rights reserved.                                       */
/* ---------------------------------------------------------------------------------------- */
/* Original WPer080197                                                                      */
/* Updates                                                                                  */
/* CMal091197: Replace ITEM_VOID_SELECTED with CANCEL_ITEM and                              */
/*             also remove SCALE_ & SCANNER_ ENABLED & DISABLED                             */
/* CMal091597: ITEMIZATION should be 35 (enumed to 34)                                      */
/* WPer121197: Added coupon related event ids and types                                     */
/* CMal040899: A-list changes.  PPWU.                                                       */
/* CMal051099: removed 1 input that is not used, added comment to other that is optional    */
/* CMal051099: changed SA_CONLUSION back to SA_SECURITYEXCEPTION (as don't know why Willie  */
/*             changed it but it was different than Bruno expected to still see)            */
/* CMal051799: TAR fix 107519 - TENDER_START was ID=4, now 3.  Allow both.                  */
/* CMal102799: TAR fix 124123 - add 211 timer                                               */
/* --------------------------- version 2.0 -------------------------------------------------*/
/* CMal020800: add in 2.0 related #defines                                                  */
/* CMal022300: re-invented specific failure types with SMM                                  */
/* CMal022400: add new internal messages (INPUT_ID's) for SMM 514-517                       */
/* CMea011502: add SMM_QUERYALL in order to query all records from DB												*/
/* CMea011502: add SMM_QUERYALLPPWU  to query all PPWU records from DB	
// LPM010803:  add WTINFO_ tolerance used tri-state flag //										*/
/* BC185047:   add BALANCE_PAID for SR695
/* ---------------------------------------------------------------------------------------- */



#ifndef SATYPES_H_
#define SATYPES_H_

#define	PRICE_EMBEDDED	9999					// used in quantity field to represent a price embedded item
#define SMM_QUERYALL 999							// used in Query to return all records from DB
#define SMM_QUERYALLPPWU 998					// used in Query to return all PPWU records from DB



	/* ---------------------------------------------------------- */
	/* Output types when firing output events to SCOT : CMal040899 */
	/* ---------------------------------------------------------- */

typedef enum {
	SA_SECURITYEXCEPTION,						// 0 CMal051099 changed back
	SA_STATUS,									// 1
	SA_SECURITYLEVEL,							// 2
	SA_TIMERSTARTED,							// 3 (used internally only)
	SA_CONTEXT,									// 4 (used externally to enable S/S and
	                                            //         internally to update WLDB)
} EVENT_TYPES;



/* --------------------------------------------- */
/* inputs to rules in SendSCOTInput : CMal040899 */
/* for OutputType = SACONCLUSION                 */
/* --------------------------------------------- */


typedef enum {
	VOID_TOTALS = -21,							// -21	(for internal use only)
	SECURITY_LEVEL = -20,						// -20	(for internal use only)
	GROSS_SALES = -19,							// -19	(for internal use only)
	CLEAR_NORMAL_COUPONS = -18,					// -18  (for internal use only)  
	CLEAR_HIGH_RISK_COUPONS = -17,				// -17  (for internal use only)
	ALL_COUPONS = -16,							// -16  (for internal use only)
	NORMAL_COUPONS = -15,						// -15  (for internal use only)  
	HIGH_RISK_COUPONS = -14,					// -14  (for internal use only)
	CLEAR_OTHERS = -13,							// -13  (for internal use only)
	CLEAR_VOIDS = -12,							// -12  (for internal use only)
	UPC_SCANNED = 1,							// 1	
	UPC_MANUAL,									// 2
	//UPC_SELECTED,								// was 3 not used by rules CMal051099
	TENDER_START = 3,							// 3  -  TAR fix 107519 CMal051799
	OLD_TENDER_START = 4,						// reserved 4 TAR fix 107519 CMal051799
	PRODUCT_TAG_DEACTIVATED = 5,				// 5 sensormatic tag
	CUSTOMER_MODE = 14,							// 14
	ITEM_SOLD_SCAN,								// 15
	ITEM_SOLD_MANUAL,							// 16
	ITEM_VOID_SCAN,								// 17
	ITEM_VOID_MANUAL,							// 18
	CANCEL_ITEM,								// 19 was ITEM_VOID_SELECTED CMal 9/11
	COUPON_REDEEMED_SCAN,						// 20
	COUPON_REDEEMED_MANUAL,						// 21
	COUPON_VOID_SCAN,							// 22
	COUPON_VOID_MANUAL,							// 23
	COUPON_REFUSED,								// 24
	TRANSACTION_CANCELLED_MANUAL,				// 25
	TRANSACTION_CANCELLED_AUTOMATIC,			// 26 optional not used by SCOTis/ACS CMal051099
	STORE_MODE = 28,							// 28
	TENDER,										// 29
	TRANSACTION_COMPLETE,						// 30
	TRANSACTION_CONTINUE,						// 31
	BALANCE_PAID,								// 32 
	ITEMIZATION = 35,							// 35
	MAINTENANCE_MODE = 42,						// 42 CMal020800 for ver2.0 SMM
	HOST_TIMEOUT = 50,							// 50
	ITEM_NOT_BAGGED = 111,						// 111  (for internal use only)
	POTENTIAL_WALK_AWAY = 130,					// 130  (for internal use only)
	SCALE_NOT_CLEARED = 160,					// 160  (for internal use only)
	SCALE_UNSTABLE = 170,						// 170  (for internal use only)
	FALSE_TRANSACTION_START = 200,				// 200  (for internal use only)
	SS_TIMER = 211,								// 211  (for internal use only) // CMal102799
	WLDB_ACTIVE = 250,							// 250	(for internal use only)
	STABLE_WEIGHT = 500,						// 500  (for internal use only)
	UNSTABLE_WEIGHT,							// 501  (for internal use only)
	WEIGHT_ENTRY = 510,							// 510  (for internal use only)
	QUERY_WEIGHT,								// 511	(for internal use only)
	UPDATE_WEIGHT,								// 512	(for internal use only)
	PURGE_WEIGHT,								// 513	(for internal use only)
	QUERY_EXCEPTION,							// 514  (for internal use only) // CMal022400
	QUERY_EXCEPTION_RESULTS,					// 515  (for internal use only) // CMal022400
	UPDATE_EXCEPTION,							// 516  (for internal use only) // CMal022400
	UPDATE_EXCEPTION_RESULTS,					// 517  (for internal use only) // CMal022400
	QUERY_SMM_WLDB,								// 518  (for internal use only) // CMal030600
	QUERY_SMM_WLDB_RESULTS,						// 519  (for internal use only) // CMal030600
	UPDATE_SMM_WLDB,							// 520  (for internal use only) // CMal030600
	UPDATE_SMM_WLDB_RESULTS,					// 521  (for internal use only) // CMal030600
	NONSMM_UPC_GETITEMINFO_FROM_SERVER,			// 522  (for internal use only) // CMal032600
	NONSMM_PLU_GETITEMINFO_FROM_SERVER,			// 523  (for internal use only) // CMal032600
	TIMER_STOPPED = 600,						// 600  (for internal use only)
} INPUT_IDS;  



	/* -------------------------------------------------------------- */
	/* General Status values for the entire Security ocx : CMal040899 */
	/* when OutputType = SASTATUS                                     */
	/* -------------------------------------------------------------- */

typedef enum {
	SA_OK,										// 0
	SA_UNKNOWN,									// 1
	SA_FAILURE,									// 2	
} SA_STATUSES;




	/* -------------------------------------------------------------- */
	/* Qty field input flags to Rules, also may have >= 0 : CMal040899 */
	/* -------------------------------------------------------------- */

typedef enum {
	NOT_ACTIVE = -2,							// -2	(for internal use only)
	LAST_ENTRY = -1,							// -1	(for internal use only)
} INPUT_FLAGS;



	/* --------------------------------------------------------------------------- */
	/* States of Security during an Order - almost same as SCOT states : CMal040899 */
	/* --------------------------------------------------------------------------- */

typedef enum {
	SA_ITEMIZATION,								// 0
	SA_TENDER,									// 1
	SA_TENDER_START,							// 2
	SA_WALK_AWAY,								// 3
	SA_TRANSACTION_COMPLETE,					// 4
} SA_STATES;


	/* -------------------------------------------------------- */
	/* Modes of Security - almost mimic SCOT modes : CMal040899 */
	/* -------------------------------------------------------- */


typedef enum {
	SA_CUSTOMER_MODE,							// 0
	SA_STORE_MODE,								// 1
	SA_SMM_MODE,								// 2 CMal020800 Store Maint Mode
} SA_MODES;


	/* ----------------------------------------------------- */
	/* Security Levels - depend on hour and day : CMal040899 */
	/* ----------------------------------------------------- */


typedef enum {
	SA_ALL = 0,									// 0
	SA_LOW,										// 1
	SA_MEDIUM,									// 2
	SA_HIGH,									// 3
} RULE_LEVELS;


	/* ----------------------------------------------------- */
	/* Coupon Types used for CouponRatioOK      : CMal040899 */
	/* Note: SA_ALL_COUPONS not valid for ClearCouponTallies */
	/* ----------------------------------------------------- */

typedef enum {
	SA_ALL_COUPONS,								// 0
	SA_NORMAL_COUPONS,							// 1
	SA_HIGH_RISK_COUPONS,						// 2
} COUPON_TYPES;



	/* ----------------------------------------------------- */
	/* SMM Result codes.                        : CMal020800 */
	/* Note: mainly used for lResult in SMM Firexxx          */
	/* ----------------------------------------------------- */
	/* CMal022300: re-invented specific failure types        */
	/* CMal051700: if WLDB is not chosen to be used...       */

typedef enum {
	SMM_OK_NOWLDB = -1,							// -1
	SMM_OK = 0,									// 0
	SMM_FAILURE_CONNECTION,						// 1
	SMM_FAILURE_DBPROB,							// 2
	SMM_FAILURE_PARAMPROB,						// 3
	SMM_FAILURE_UNKNOWN,						// 4
} SMM_RESULT;


	/* ----------------------------------------------------- */
	/* SMM Boolean     .                        : CMal020800 */
	/* Note: used for all SMM related booleans (eg. lAppr)   */
	/* ----------------------------------------------------- */

typedef enum {
	SMM_FALSE,   								// 0
	SMM_TRUE,									// 1
} SMM_BOOLEAN;


	/* ----------------------------------------------------- */
	/* SMM Action codes                         : CMal020800 */
	/* Note1: used for lAction code in SMM related updates.  */
	/* ----------------------------------------------------- */

typedef enum {
	SMM_ADD,   									// 0
	SMM_DEL,									// 1
	SMM_CHG,									// 2
} SMM_ACTION;

	/* ----------------------------------------------------- */
	/* SMM Special Item.                        : CMal020800 */
	/* Note1: allows future handling of SCOTapp recognized   */
	/*        special items (eg QuanNeeded).  Right now we   */
	/*        have the PLU server info, so can't do it.      */
	/* Note2: only value used will be regular until Note1.   */
	/* ----------------------------------------------------- */

typedef enum {
	SMM_REGULAR, 								// 0
	SMM_QUAN_NEEDED,							// 1 future
	SMM_PRODUCE,								// 2 future
} SMM_SPECIAL_ITEM;

	/* ----------------------------------------------------- */
	/* SMM Tri-state tolerances                 : CMal020800 */
	/* Note1: used for exception item data.                  */
	/* ----------------------------------------------------- */

typedef enum {
	SMM_TRI_DEF = -1,							// -1
	SMM_TRI_NO  = 0,							// 0
	SMM_TRI_YES,								// 1
} SMM_TRI_STATE_TOLERANCE;


	/* ----------------------------------------------------- */
	/* SMM Weight Tolerances.                   : CMal020800 */
	/* Note1: used to indicate usage of default values       */
	/*        instead of hardcoded overrides.                */
	/* ----------------------------------------------------- */

typedef enum {
	SMM_WTTOL_DEF = -1,						// -1
} SMM_WT_TOLERANCE;

	/* ----------------------------------------------------- */
	/* SMM Tolerance Types.                     : CMal020800 */
	/* Note1: indicate the type of tolerance for exception   */
	/*        items.                                         */
	/* Note2: SCOTapp can not give Dept information          */
	/* ----------------------------------------------------- */

typedef enum {
	SMM_TOL_GLOBAL,								// 0
	SMM_TOL_ITEM,								// 1
	SMM_TOL_DEPT,								// 2 future
} SMM_TOLERANCE_TYPES;


	/* ----------------------------------------------------- */
	/* SMM Usage Variables.                     : CMal020800 */
	/* Note1: general usage variables for calls to allow     */
	/*        special processing of data parameters.         */
	/* ----------------------------------------------------- */

typedef enum {
	SMM_ALL_WTS = -5,							// -5 (used on the Delete type of update)
	SMM_SEC_CALC = -2,							// -2 let security calc this value
	SMM_IGNORE = -1,							// -1 ignore this parameter
} SMM_USAGE_VARS;


	/* ----------------------------------------------------- */
	/* Allow Override of Exceptions             : CMal061201 */
	/* Note1: used when calling SendSCOTInputWithOverrides   */
	/* Note2: values 0/1 are already specifics for No/Yes    */
	/* Note3: since these values are in a .h that CoreApp    */
	/*        already shares with TB, we need slightly diff  */
	/*        variables, since we share this with CoreApp.   */
	/* ----------------------------------------------------- */

typedef enum {
	TBTHRUAPP_LET_SECURITY_DECIDE			= -1,	// -1 Let Security work as usual
	TBTHRUAPP_OVERRIDE_SECURITY_WITH_NO		= 2,	//  2 Override any Security setting with No
	TBTHRUAPP_OVERRIDE_SECURITY_WITH_YES	= 3,	//  3 Override any Security setting with Yes
} TBTHRUAPP_OVERRIDE_SECURITY_EXCEPTION;


typedef enum{
	DBM_STATUS = 0,
	DBM_DBLOOKUPSUCCESS,
	DBM_DEPTINFOSUCCESS,
}DBM_MESSAGE;

typedef enum{
	DBMGRSTATUS_OK = 0,
	DBMGRSTATUS_FAIL
}DBMGRSTATUS;

typedef enum{
	WTINFO_STDDEV  = 1,
	WTINFO_DEFAULT = 2,
	WTINFO_MAXERROR = 3,
	WTINFO_EXCEPTIONITEM = 4,
	WTINFO_EXCEPTIONDEPT = 5,
	WTINFO_EXCEPTIONTB = 6
}WTINFO_TOLERANCEUSED;

typedef enum{
	SmOnItemOK = 0,
	SmOnTransactionComplete,
	SmOnSecurityException,
	SmOnSecurityExceptionCleared,
	SmOnSecurityMaintenance,
	SmOnDeviceFailure,
	SmOnInitializationComplete,
	SmOnAppControl,
	SmOnSecurityControl,
	SmItemPickFinish
}SECMGR_MESSAGE;
#endif//SATYPES_H_
