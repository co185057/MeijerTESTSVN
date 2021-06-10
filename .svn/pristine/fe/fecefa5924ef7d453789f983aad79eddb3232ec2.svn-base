//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  ConfigObject.cpp
//
// TITLE:
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <direct.h>

#include "Common.h"								  // MGV headers
#include "SMState.h"								  // MGV headers

// BeginCodeLocale
#include "math.h"
// EndCodeLocale

#define COMP_ID ID_CO
#define T_ID _T("CO")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UNSET_KEYVALUE	_T("?")

#define CURRENCY_CODE_SIZE 11		//used to store Currency Code size
#define CURRENCY_BITMAP_DEFAULT _T("USD") //used to store Default bitmap for currency


// TAR #200002 HC 032602 Start
// For testing/debugging, show a pop up message if return code from TB is not
// TB_SUCCESS or TB_SUCCESS_IMMEDIATE in SMStateTBBase's ProcessTBReturn
// if bShowTBErrors is true and boolean value for TB error is also true
bool bShowTBErrors = false;
bool bShowTBInvalidUserID = false;
bool bShowTBInvalidUserPW = false;
bool bShowTBInvalidState = false;
bool bShowTBFileNotFound = false;
bool bShowTBIncompleteProcedure = false;
bool bShowTBInvalidWindow = false;
bool bShowTBDiskFull = false;
bool bShowTBInvalidParam = false;
bool bShowTBNoHostCredit = false;
bool bShowTBNoWorkingKey = false;
bool bShowTBNotInitialized = false;
bool bShowTBNoHost = false;
bool bShowTBFunctionNotSupported = false;
bool bShowTBUnexpected = false;
bool bShowTBItemFailed = false;
bool bShowTBCardDataMissing = false;
bool bShowTBOther = false;
CString csGLOBALAddThe128Prefix; // CM/NN 090502 214029
// TAR #200002 HC 032602 End

////////////////////////////////////////////////////////////////////////////////
static void CheckRange(	int &lValue,
						const int lMin,
						const int lMax)
{
  if (lValue<lMin) lValue = lMin;
  else if (lValue>lMax) lValue = lMax;
}

////////////////////////////////////////////////////////////////////////////////
static CString GetOptStr(	CWinApp* pApp,
							LPCTSTR szSect,
							LPCTSTR szKey,
							LPCTSTR szDflt,
							const bool fError)
{
	CString csWork(UNSET_KEYVALUE);
	if (pApp)
	{
		csWork = pApp->GetProfileString(szSect,szKey,UNSET_KEYVALUE);
	}
	if (csWork==UNSET_KEYVALUE)
	{
		if (fError)
		{

			ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_MISSING_OPTION,_T("%s|%s|%s"),
				szSect,szKey,szDflt);

		}
        trace(L6, _T("Config option not found for [%s]-%s, using default %s"), szSect, szKey, szDflt);

		csWork = szDflt;
	}


	return csWork;
};

////////////////////////////////////////////////////////////////////////////
static int GetOptInt(	CWinApp* pApp,
						LPCTSTR szSect,
						LPCTSTR szKey,
						const int nDflt,
						const bool fError)
{

	int nWork = -99999;
	if (pApp)
	{
		nWork = pApp->GetProfileInt(szSect,szKey,-99999);
	}
	if (nWork==-99999)
	{
		if (fError)
		{

			ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_MISSING_OPTION,_T("%s|%s|%d"),
				szSect,szKey,nDflt);

		}
        trace(L6, _T("Config option not found for [%s]-%s, using default \"%d\""), szSect, szKey, nDflt);

		nWork = nDflt;
	}
	return nWork;
};

// ---------------------------------------------------------------------------
// CMal091703 TAR 248007 start 
// Purpose: Brand new function to clean working arrays from SCOTopts of a 
//          specified value (e.g. all entries with a value of zero).  Since
//          we now save all original values (including zeroes), because
//          we need the zero values to temporarily serve as place-holders.
//          Part of the fix that allows .000 to Add/Replace/Remove from .DAT.
// ---------------------------------------------------------------------------
static int CleanOptIntArray(	
								  LPCTSTR szSect,
								  LPCTSTR szKeyBase,			// SCOTopts value definition
								  const int iMaxSize,		// array Max Size (was numValues)
								  int values[],				// original array
								  const int iTargetValue)	// the value to purge

{
	int arrayNdx = -1, optInt = 0;
	if (iMaxSize > 0)
	{
		int *tempValues = new int[iMaxSize];	// temporary storage for working array

		// fill temp array with original array values
		for (int ij = 0; ij < iMaxSize; ij++)
			tempValues[ij] = values[ij];

		// only pick out the values that differ than the iTargetValue
		while (optInt < iMaxSize)
		{
			if (tempValues[optInt] != iTargetValue)
			{
				arrayNdx++;
				values[arrayNdx] = tempValues[optInt];
			}
			optInt++; 
		}
		delete(tempValues);
	}
	else
	{
		arrayNdx = -1; // nothing to clean
	
	}
	
	ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_NUM_ARRAY_OPTIONS,_T("%s %s|%d"),	szSect,szKeyBase,arrayNdx + 1);
	return arrayNdx + 1; // return number of values still in the array
}  // CMal091703 TAR 248007 : end new function

// ---------------------------------------------------------------------------
// Make a list of similar int values and store them into an array.
// E.g. - get nCashBack1, nCashBack2, and nCashBack3 into an
// int array nCashBack[3];
// Note CMal090503: looks in SCOTopts.xxx to extract similar values
// CMal090503 TAR 232965: reworked this function to accomplish replace of .000
// CMal091703 TAR 248007: redefined goal to allow add/remove from .000
//              but this leaves zero entries in the array.  Must clean later.
// ---------------------------------------------------------------------------
static int GetOptIntArray(	CWinApp* pApp,
								  LPCTSTR szSect,						// e.g. Tender (where to look in .DAT)
								  LPCTSTR szKeyBase,					// e.g. CashBack (where to look in .DAT)
								  const int numValues,				// Max values in array
								  int values[],						// 1 dim array to hold values
								  const int nDflt,					// reading def value 
								  const bool fError,					// care if value is missing from .DAT
								  const bool fStoreValIfDefault) // reading.DAT or not (.00x)
{
	int arrayValsNotZero = 0;  // CMal091703 TAR 248007 
	int arrayNdx = -1, optInt = 0;
	while (optInt < numValues)
	{
		int iValueInFile;  CString csKey;
		
		// 1st. create keyname e.g. CashBack1
		csKey.Format(_T("%s%d"), szKeyBase, optInt+1);		
		
		// 2nd. see if target file has that key
		iValueInFile = GetOptInt(pApp,szSect, csKey, nDflt, fError); 
		
		// if store regardless of value OR if value is Non-Zero
		// CMal090503: 2 problem here.  1. it resets Number of entries in Array (the return val) if
		//				   SCOTopts.000 has less CashBackX values specified than SCOTopts.000
		//					2. this will not "line-up" the values in the array, but instead overwrites
		
	
		// The main part of TAR 232965 replacement code follows...

		// Now if fStoreValIfDefault means called from .DAT
		if (fStoreValIfDefault == true)
		{
			// Called from the .DAT, store all of those values if not zero.
			// must be synchronous CashBack1 CashBack2 ... 
			// Note: this values array is 1 dimensional (an int represents Position)

			// NEW WAY TAR 248007 CMal 091703: since we have a 1 dim array, keep position too
			if (iValueInFile != nDflt)
				arrayValsNotZero++;
			values[++arrayNdx] = iValueInFile;							// add value in the array
		}
		else // process overrides
		{
			// Called from other SCOTopts file, such as .000.  
			// OLD WAY --- Store at SAME place in array as the .DAT and only if not zero
			// NEW WAY 091703 TAR 248007 : must be able to A) Remove, B) Replace, C) Add
			//                 where all values specified in the override are relevant.
			
			++arrayNdx; // count each place

			// see if original array has value.  
			// means it existed in the previous .DAT files.
			if (values[arrayNdx] > 0)
			{
				// it existed from previous .DAT

				// does it exist in the override as non-zero value
				if (iValueInFile > 0)
				{
					// yes in override too, (replace original value)
					values[arrayNdx] = iValueInFile;
					arrayValsNotZero++;
				}
				else if (iValueInFile == 0)
				{
					// exists in override, but as Zero value (remove original)
					values[arrayNdx] = 0;							 
				}
				else if (iValueInFile == nDflt)
				{
					// no override exists (leave original value alone)
					arrayValsNotZero++;
				}
				else
				{
					// unknown value, likely negative.  Ignore this override.
					arrayValsNotZero++;
				}
			}
			else
			{
				if ((iValueInFile != nDflt) && // value not defined in .000
					 (iValueInFile > 0))			 // value defined, but this means remove original (if exists)
				{
					// add the override, it wasn't in the original .DAT
					values[arrayNdx] = iValueInFile;
					arrayValsNotZero++;					
				}

			}
		}
		optInt++; 
		// end - CMal091703 TAR 248007 now have Add/Replace/Remove ability
	}
	
	return arrayValsNotZero; // CMal091703 removed arrayNdx + 1; // return number of values stored
}
// CMal090503 TAR 232965: end rework

////////////////////////////////////////////////////////////////////////////
static bool GetOptFlg(	CWinApp* pApp,
						LPCTSTR szSect,
						LPCTSTR szKey,
						const bool fDflt,
						const bool fError, bool *bRead =false) //bRead = Y if reading succeed
{
	CString  csWork = GetOptStr(pApp,szSect,szKey,UNSET_KEYVALUE,fError);
	if (csWork==UNSET_KEYVALUE)
	{
		csWork = (fDflt?_T("Y"):_T("N"));
	}
	else
	{
		if(bRead)
		   *bRead = true;
	}

	csWork.MakeUpper();
	if (csWork.Left(1)==_T('1')) return true;
	if (csWork.Left(1)==_T('Y')) return true;
	return false;
};

////////////////////////////////////////////////////////////////////////////

#define _GET_STR(a,b) cs##a##b = GetOptStr(pApp, _T(#a), _T(#b), cs##a##b,fErrorLog);
#define _GET_INT(a,b) n##a##b = GetOptInt(pApp, _T(#a), _T(#b),n##a##b,fErrorLog);
#define _GET_INT_ARRAY(a,b,c,d,e) n##a##b##Num = GetOptIntArray(pApp, _T(#a), _T(#b),c,n##a##b,d,fErrorLog,e);
#define _CLEAN_INT_ARRAY(a,b,c,d) n##a##b##Num = CleanOptIntArray( _T(#a), _T(#b),c,n##a##b,d);
#define _GET_FLG(a,b) f##a##b = GetOptFlg(pApp, _T(#a), _T(#b),f##a##b,fErrorLog);
#define _GET_FLG2(a,b,c) f##a##b = GetOptFlg(pApp, _T(#a), _T(#b), f##a##b,fErrorLog, c);

#define _GET_BFLG(a,b) m_b##a##b = GetOptFlg(pApp, _T(#a), _T(#b),m_b##a##b,fErrorLog);  // alternative version of _GET_FLG reading a entry to m_b*
#define _GET_BFLG2(a,b,c) m_b##a##b = GetOptFlg(pApp, _T(#a), _T(#b), m_b##a##b,fErrorLog, c);  // alternative version of _GET_FLG reading a entry to m_b*


////////////////////////////
// Constructor
////////////////////////////
ConfigObject::ConfigObject()
	:				// large constructor initialization list
	fErrorLog(false),					  	  // assume INI errors are not logged
	//nTracingNumEntries(0),					  // default startup tracing number
	nTracingCurrentLevel(0),				  // default startup tracing level
	nTracingFocusLevel(0),					  // default focus tracing level
	csTracingFocusComps(_T("")),				  // default focus comp ids for tracing, TS,PS,MP,ST...
	nTracingMaxLogSize(0) ,					// default MaxLogSize
	nTracingEntriesToFlush(0) ,				// Number of entries being flushed to the file each time
	nTracingPSAPIInterval(900),				// Number of seconds between timed PSAPI traces in core
	fTracingPSAPITraceStateInit(false),		// Whether or not to do PSAPI traces around SMState Init funcs.
	fTracingTraceOn(false),
	fTenderCashAllowed(false),				  // Cash tender is allowed
	fTenderCreditAllowed(false),			  // Credit tender is allowed
	fTenderDebitAllowed(false),				  // Debit tender is allowed
	fTenderCreditNeedsPIN(false),			  // Credit tender needs a PIN number
	fTenderDebitNeedsPIN(true),				// Debit tender needs a PIN number
	fTenderCreditCashBack(false),			  // Credit tender allows cash back
	fTenderDebitCashBack(true),				// Debit tender allows cash back
	nTenderCashBackNum(0),					  // no cash back amounts yet
	fTenderOtherPayment(false),               // no other payment
	fTenderSubmitFSTtotalIfMorethanTrxTotal(false),  // submit lFoodStampsDue if lFoodStampsDue > lBalanceDue
	csTenderDenominations(_T("")),				  // no cash denominations defined or asssumed
	//nOperationsAlcoholAge(99),			  // must be 99 years age restriction
	//nOperationsTobaccoAge(99),			  // must be 99 years age restriction // Tobacco

	//  Add EBT support
	fTenderEBTFSAllowed(false),				  // EBT FS tender is allowed
	fTenderEBTCBAllowed(false),				  // EBT CB tender is allowed
	fTenderEBTFSNeedsPIN(true),					// EBT FS tender needs a PIN number
	fTenderEBTCBNeedsPIN(true),					// EBT CB tender needs a PIN number
	//fTenderEBTFSCashBack(false),				// EBT FS used to allow cash back - no longer used, not valid
	fTenderEBTCBCashBack(false),				//Tar 246679	// EBT CB tender allows cash back
	fTenderAllowCancelWithPartialEBTTender(false),			  // EBT
	nTenderEBTCashBackNum(0),					// no cash back amounts yet
	fTenderIsMotorizedMSR(false),				// using Motorized MSR, Y or N
	fTenderDispenseChangeAtLaneForRemoteTendering(true),   //For remote tendering transaction, dispense change at Lane side or pop RAP cash drawer at RAP side. The default value is Y
	//fTenderPIPPINEncryptorOnly(false),			// If using Motorized MSR w/ PIP, and PIP is for entering PIN only future use
	//
	csOperationsReceiptFileFormat(_T("")),    // RFC 368222
    csOperationsHeadCashierContext(_T("")),
	fOperationsScaleMetric(false),		      // N=false, Y=true
	nOperationsTareContainer1(0),			  // no tare for SelectContainer button 1
	nOperationsTareContainer2(0),			  // no tare for SelectContainer button 2
	nOperationsMaxCoupon(500),				  // max allowed manual coupon value
	nOperationsMaxCouponLimit(500),			  // SR678 max allowed coupon value for either scanned or keyed in  coupons
	nOperationsDuplicateCouponMax(2),		  // number of duplicate coupons allowed per transaction
	nOperationsTotalCoupons$AmtMax(225),	  // Total coupon dollar amount allowed per transaction
	nOperationsUpper$AmtForPercentToApply(2000), //The upper limit used to determine when to use coupons by $ amount versus coupon % of gross sales. 100=$1
	nOperationsCouponPercentofGrossSales(15), // % or gross sales that the coupon dollar amount can be
	fOperationsAllowSkipBaggingBtn(true),	  // At Bag Item, allow <Skip Bagging> btn, Y/N.
	//BZ 06/07/02 RFC 203538 begin
	fOperationsAllowStoreLoginBtnOnBagItemScreen(true),	  // On Bag&EAS screen, allow <Store Login> btn, Y/N.
	fOperationsAllowStoreLoginBtnOnRemoveItemScreen(true),// On PickUpItem screen, allow <Store Login> btn, Y/N.
	//RFC 203538 end
	nOperationsOperatorLoginBarcodeType(1),  // NN 072902 RFC 187993 0 or 1 or 2.  0=No scannable barcode, 1=EAN13 type, 2=Cose 128 type
    fOperationsAllowLeadingZerosInCode128LoginBarCode(false), //rfc 240738
    csOperationsOperatorBarcodeMask(_T("")),     // NN 072902 RFC 187993
    fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen(false), //CHU 07/25/02 RFC 208874
	fOperationsAllowContainerBtn(true),		  // At Bagging Alert, allow <Container> button
	fOperationsDisableUnexpIncContainerBtnAftFirstItem(false), //dont prompt im using my own bag at unexpected increase
	fOperationsAllowContainerBtnForMismatching(true),		  // Tar 266250
	fOperationsAllowContainerBtnInFinalization(false),//SR085
	fOperationsAllowItemRemovedBtn(true),	  // At Removing Items, allow <Item Removed> btn, Y/N.
    nOperationsMaxGiftCard(0),                // max allowed gift card value
	fOperationsAllowAMDoneBtn(false),		  // allow Done button on WaitForRemoteAssistance RFC 209075 RF 073102
	fOperationsShowWeightInfoOnLane(false),		//SR682-Show weight & item number info on lane during intervention states
	fOperationsSuppressAudioInItemRemoveViolation(true), //RFC 252471
	fOperationsSuppressAudioInUnexpectedIncrease(false), // 3.2 patch B - RFC 257450
	fOperationsSuppressAudioInMismatch(false), // 3.2 patch B - RFC 257449
	fOperationsShowSkipBaggingOnEReceipt(true),// RFC 252473
	fStateSayPrices(false),					  // flag value T/F, 0-no, other-yes
    fStateSayPricesDisableInAttendantMode(false), // flag value T/F TAR 330578 - Disable SayPrice during attendant mode
	fStateSayPhrases(true),					  // flag value T/F, 0-no, other-yes
    fStateAllowTBAudioControl(false),         // flag value T/F, 0-no, other-yes
	fStateLiveVideo(false),					  // flag value T/F, 0-no, other-yes
	fStateWelcomeScreenVideo(true),           // flag value T/F, 0-no, other-yes
	fStateSelectListScreenVideo(true),        // flag value T/F, 0-no, other-yes
	fStateGraphicalSelectList(true),
	fStateOverridePickDesc(false),
	csStateDefaultProduceBMPName(_T("PHOTONOTAVAILABLE.BMP")),
	csStateInvalidCouponPrefix(_T("")),          //TAR 269159
	fStateProduceFavorites(true),
	fStateDisplayWeightsAndMeasures(true),    // flag value T/F, 0-no, other-yes
	fStateLargeFontOnCurrentItem(false),
	fStateConfirmationForSuspendingTransaction(true),  // flag value T/F, 0-no, other-yes
	fStateCheckBasketReminder(false),		  // flag value T/F, 0-no, other-yes
	fStateCouponSensor(false),                // flag value T/F, T - turn on coupon sensor
	fStateDOBNotNeededButton(true),           // flag value T/F, T - display Date of Birth Not Needed button
	fStateDisplayNumberOfItemsOnRAP(true),	  // flag value T/F, T - display the number of items in RAP      
	fStateAllowLoyaltyCard(false),			  // flag value T/F, 0-no, other-yes
	fStateLoyaltyCardReminder(false),		  // flag value T/F, 0-no, other-yes
	fStateLoyaltyCardReminderAfterFirstItemSold(false),
	fStateAssistMode(true),
	fStateLookupItemIsButton1(true),
	fStateAllowLookUpItem(true),			//TAR 173139
	fStateAllowKeyInCode(true),				//TAR 173138
	fStateFlashLookupItemButton(true),
	fStateFlashKeyInCodeButton(true),
	fStateDualLanguage(false),
	fStateSingleLanguageButtonAtAttract(false),
	fStateAllowLanguageSelectionDuringTransaction(false),
	fStateHasCardMEI(true),
	fStateAllowInterruptWavFiles(true),	  // flag value T/F - allow wave files to be interruptable
	fStateForceWeighingOFPriceEmbedded(false),	// flag value T/F - Price Embedded items be weighed on the ScannerScale
    fStateEscalateTakeYourItemsTimeout(false), //rfc209702
	fStateSystemFunctionsBtnOnLaneStatusScreen(false),	// flag value T/F to change location of System Function button for Retailix 168378 LPM040601
	fStatePaintScanAndBagOnInitialScan(true),           // TAR 262131
	fStateForceTrxStartOnSwipe(false),
    fStateAttndModeAllowed(true),
    fStateAttndModeLoginBypass(true),
    fStateAttndModeImmedInterventions(true),
	fOperationsCheckCashChanger (false),    // TAR 256141
	fOperationsReprintReceiptByTransNumber(false),
    fStateAssistMenuFromStoreModeAllowed(false),
	fStateAssistMenuFromRAPAllowed(false),
    fStateStartupToLaneClosed(false),  //RFC 986
    fStateStartupWaitForTB(false),     //RFC 986
    nOperationsEOD(1),				   // flag for EOD feature FastLane 3.1
	nOperationsEODTimeout(60000),	   // timeout value to wait for EOD batch file to run
	//USSF START
	fOperationsEnableUSSF(false),
	//USSF END
	
	csOperationsTransitionToScanAndBagContexts(_T("")),	// SR730 RFC380486

	fStateSecurityDisableAllowed(false),
	fStateSecurityDisableNoReset(false),
 
    // TAR 211420
    csTemplateKeyboard(_T("")),		                // Keyboard template (for the pick up list)
	csTemplateAssistMode(_T("")),	                // Assist Mode template
	csTemplateAlphaNumeric(_T("")),                 // AlphaNumeric keyboard for job information
    nTemplatePasswordEntryKeypad(0),     // FZ 10102002, Alphanumberic Password
	nTemplateUserIDEntryKeypad(0),       // FZ 10102002, Alphanumberic Password

	nTimeOutSyncTimeout(60000),	   // timeout value to wait for SyncTime batch file to run	
	nTimeOutTender(120),
	nTimeOutStateDefault(60),
	nTimeOutTakeCard(15),
	nTimeOutTakeCash(15),
	nTimeOutTakeCoins(3),
	nTimeOutItemTransactionResult(15),
	nTimeOutContinueTransaction(65),
	nTimeOutHelp(10),
	nTimeOutSignon(30),
	nTimeOutSignoff(30),
	nTimeOutAttract(30),
	nTimeOutItemSaleInProgress(0),
	nTimeOutEASDeactivationScreenTimer(10),		//for re-playing the audio msg while on the EAS screen
	nTimeOutCouponSensorScreenTimer(10),	    //for re-playing the audio msg while on the Insert Coupon screen
	nTimeOutDepositGiftCardScreenTimer(0),	    //for re-playing the audio msg while on the Deposit Gift Card screen
	nTimeOutTenderBalanceScreenTimer(0),	    //for re-playing the audio msg while on the Tender Balance screen
    nTimeOutCashOutSensorAudioTimer(15),	    //for re-playing the audio msg while on the Take Change screen
	nTimeOutCheckBasketReminder(30),			//for going forward
	nTimeOutRequestSig(60),			            //for going forward
	nTimeOutInitialBagItemAudio(10),			//After this many seconds, play the audio prompt when entering the Bag Item screen.  0=immediate
	nTimeOutPeriodicBagItemAudio(10),			//for re-playing the audio msg while on the Bag Item screen
	nTimeOutInitialRemovingItemsAudio(10),		//After this many seconds, play the audio prompt when entering the Picking Up Items screen.  0=immediate
	nTimeOutPeriodicRemovingItemsAudio(10),		//for re-playing the audio msg while on the Picking Up Items screen
	nTimeOutWalkAwayTimerAudio(10),				//After this many seconds, play the audio prompt to continue purchasing
	nTimeOutInitialTakeYourItemAudio(1),        //After this many seconds, first take your item audio is played
	nTimeOutRecurringTakeYourItemAudio(15),     //After ThankYouTimerAudio is exprired, TakeYouItem is going to be played repeatly based on this timer RFC209702
	nTimeOutSecurityServerTimer(2),				//After this many seconds, application will call lastchance (local security) lookup
    nTimeOutThankYouTimerBelt(10),              //What this many seconds on the Thank You screen if on a Takeaway Belt system
	nTimeOutInitialClearBagAreaAudio(0),		//After this many seconds, play the audio prompt when entering the Bag Area Backup screen.  0=immediate
	nTimeOutPeriodicClearBagAreaAudio(20),		//for re-playing the audio msg while on the Bag Area Backup screen
	nTimeOutTimeOutTransportItem(15),
	nTimeOutTouchScreenProximityEnablesScannerMotor(0), //time interval between waking up 7883 scanner motor triggered by touchscreen motion sensor, >0=timer, 0=feature off
	nTimeOutMotionSensorOnAttractInterval(0), //RFC 265465
	nTimeOutScannerScaleOnAttractInterval(0),//RFC 276235
	nTimeOutDisplayBagItemScreenTimer(5),		//After this many seconds, display Bag Item Screen
	// RFC 202655/202656 new timers set in milliseconds for RAP escalation
    nTimeOutBagItemEscalationMilliSecs(13000),			//for sending Bag Item message to RAP (in milliseconds)
    nTimeOutRemovingItemsEscalationMilliSecs(12000),		//for sending Picking Up Items message to RAP (in milliseconds)
    nTimeOutDelaySecurityNotificationMilliSecs(10000),  //time to wait before changing trilight to signal security problem
	nTimeOutPeriodicSecurityAudio(15),          //After this many seconds, play the security audio prompt
	nTimeOutTakeYourItemEscalationMilliSecs(30000),		//for sending Fogotten Item message to RAP (in milliseconds) RFC209702
    nTimeOutHeartbeatTimer(1800),                              //RFC 262399
    nTimeOutBusyLeaseTimer(10),                                //RFC 263404
	nTimeOutAttendantModeTimer(300),
	nTimeOutGloryUnitDispenseTimeout(500),				// TAR 310660
	nTimeOutExitSuspendScreenTimer (5),		//After this many seconds, exit Suspend Screen RFC 294962
    nTimeOutDisplayPutBagOnScaleScreenTimer(5), //SR085 - After this many seconds, display PutBagOnScale Screen
	nTimeOutDisplayIncreaseMisMatchScreenTimer(0),		//After this many seconds, display Mismatch or UnexpectedIncrease Screen //TAR379130/379131
	nTimeOutEnableSkipBaggingButtonTimer(0), //SR778 - enable skip bagging button after timer expires (in milliseconds).

    // BeginCodeLocale
	fLocaleIsCurrencySymbolOnEReceiptCol1(true),		// Currency Symbol display on the e-receipt Col1
	fLocaleIsCurrencySymbolOnEReceiptCol2(false),		// Currency Symbol display on the e-receipt Col2
	fLocaleIsTaxLine(true),		//set default value											 // Display Tax Line

	// Price Display
	fLocaleCurrencySettingNotFromRegionalSetting(false),	// Y/N Regional Setting (Control Panel)
	fLocaleDateSettingNotFromRegionalSetting(false),		  // Y/N Regional Setting (Control Panel)

	// If fLocaleCurrencySettingNotFromRegionalSetting=Y, we overwrite with :
	csLocaleCurrencySymbol(_T("$")),			    // CURRENCYFMT lpCurrencySymbol ($,TL)
	nLocalePositiveCurrencyFormat(1),	    // CURRENCYFMT PositiveOrder, LCTYPE LOCALE_ICURRENCY
	nLocaleNegativeCurrencyFormat(1),	    // CURRENCYFMT NegativeOrder, LCTYPE LOCALE_INEGCURR
	csLocaleDecimalSymbol(_T(".")),		        // CURRENCYFMT lpDecimalSep
	nLocaleNoOfDigitsAfterDecimal(2),	    // CURRENCYFMT NumDigits
	csLocaleDigitGroupingSymbol(_T(",")),     // CURRENCYFMT lpThousandSep
	nLocaleNoOfDigitsInGroup(0),				  // CURRENCYFMT Grouping
	nLocaleLeadingZero(0),			          // CURRENCYFMT Leading Zero (not used)

	// if fLocaleDateSettingNotFromRegionalSetting=Y, we overwrite with:
	nLocaleShortDateOrdering(0),	  // LOCALE_IDATE	  Short date format-ordering specifier
	nLocaleNoCenturyDigit(0),		    // LOCALE_ICENTURY   Specifier for full 4-digit century.
	csLocaleDateSeparator(_T("/")),	    // LOCALE_SDATE	  Character(s) for the date separator.
	csLocaleShortDateFormat(_T("")),     // (nit used) LOCALE_SSHORTDATE Short date formatting string for this locale.

	// Others
	nLocalePriceCentUnit(1),		    // Cent Unit (=100 pour 2 decimal)
	csLocaleSIntlSymbol(_T("")),			  // International Currency Symbol (not used)
	csLocaleCashValueList(_T("")),			// Possible Cash Value of hopper and cassette
	csLocaleCashSafeList(_T("")),				// Minimum thresholds
	csLocaleCashLowList(_T("")),				// Low thresholds for the green blinking
  fLocaleCashManagementScreen(false),     //+dp185016 support glory device // High thresholds 
  fLocaleCashManagementDisabledInTransaction(false), //TAR 370108
  fLocaleAllowSoftwareUnload(false),
  fLocaleAllowSoftwareRemoval(false),
  fOperationsOperatorTwoLineText(false),
  fOperationsHopperSubstitutionIntervention(true),
  csLocaleCashHighList(_T("")),				
  csLocaleCashRemoveAmount(_T("")),				
  csLocaleCashCapacity(_T("")),				//-dp185016 capacity 
  csLocaleCashLowListForCoinHopper(_T("")),
  csLocaleCashCapacityForCoinHopper(_T("")),
  csLocaleCashValueListForCoinHopper(_T("")),
  fLocaleCashManagementDisplayCount(true),    //+dp185016 generic cash management
  fLocaleCashManagementAPPTrackCashCount(true),
  nLocaleCashAcceptorCapacity(500),
  nLocaleCoinAcceptorCapacity(1000),
  nLocaleCashAcceptorHighThreshold(400),
  nLocaleCoinAcceptorHighThreshold(800),      //-dp185016
  csLocaleCashLowListForNewEnhancedCoinLowSensor(_T("")),
	// + SafePay cash management.
	nLocaleNoteCassetteLocation(0),
	fLocaleAllowNoteCoverRemoval(false),   
	fLocaleAllowNoteCassetteRemoval(false),   
	nLocaleXferToCassetteMode(0),
	fLocaleAllowBillUnload(false),
	// - SafePay cash management.
   //+4.2 card dispenser
	fOperationsManageCardDispenser(false),				
	nOperationsCardDispenserBinCapacity(0),
	nOperationsDispenserRejectBinNearFull(0),
	nOperationsDispenserRejectBinFull(0),
	//-4.2 card dispenser
	//EndCodLocale

	nOperationsHopperFixLaterTrxLimit(5),
	fStateHasSidecar(true),			// ATM has a sidecar
	csStateRapInitiateTransaction(_T("N")),		// RFC 228102
	//Enable Disable User Exits. Albertsons
	fUserExitAllEnabled(false),// Allow Enable Disable of User Exits 0 = no exits on
	// 1 = any User exit enabled will be on. This will allow
	// for all exits to be turned off with one flag and only
	// selected exits to be turned on.
	fUserExit00Enabled(false),
	fUserExit01Enabled(false),
	fUserExit02Enabled(false),
	fUserExit03Enabled(false),
	fUserExit04Enabled(false),
	fUserExit05Enabled(false),
	fUserExit06Enabled(false),
	fUserExit07Enabled(false),
	fUserExit08Enabled(false),
	fUserExit09Enabled(false),
	fUserExit10Enabled(false),
	fUserExit11Enabled(false),
	fUserExit12Enabled(false),
	fUserExit13Enabled(false),
	fUserExit14Enabled(false),
	fUserExit15Enabled(false),
	fUserExit16Enabled(false),
	fUserExit99Enabled(false),
	nUserExitInt1(0),									  // used to pass long's to user exits
	nUserExitInt2(0),
	nUserExitInt3(0),
	nUserExitInt4(0),
	nUserExitInt5(0),
	csUserExitString1(_T("")),			  // used to pass string's to user exits
	csUserExitString2(_T("")),
	csUserExitString3(_T("")),
	csUserExitString4(_T("")),
	csUserExitString5(_T("")),
	fOperationsSelectTareOnWeight(false),
	nOperationsAutoTareCode(0),
	fOperationsJumpToTarePickList(false),
	fOperationsDisplayPickListItemCode(false),
	fOperationsAllowMultiPick(false),
	fOperationsDisplayQuickPickItems(false),
   fOperationsAllowPickListSearchButton(true),
   fOperationsAllowFuzzySearch(true),
	fOperationsPrintVersion(true),
	fOperationsPrintPendingApprovalOnSuspend(true),	// append outstanding approvals to receipt on suspend
// fiscal printer start
	fOperationsTBControlsPrinter(false), // default is SCOTAPP controls printer
// fiscal printer end
	fOperationsSuspend(true),					  //Suspend
    fOperationsAllowSuspendIfPartiallyTendered(false), //RFC 267820
    fOperationsIfSuspendedDispensePartiallyPaidCash(true), //RFC 267820
	fOperationsDisableBasePriceEmbeddedDefinitions(false),//RFC239992
	fOperationsTruncateSupplementalCodeForSecurity(true),  //Truncate supplemental code before sending UPC to Security
	fOperationsVoidCouponBeforeSuspend(false),
	fOperationsAutoVoidUnderageRestrict(true),
	fOperationsSendSignatureDataToTB(false),
    fOperationsPartialIntegratedPINPad(false),
    fOperationsPIPSCOTPromptCardType(false),
    fOperationsPIPSCOTPromptCashBack(false),
    fOperationsPIPSCOTPromptConfirmTotal(false),
	fOperationsCreditMSR(false),
	fOperationsDebitMSR(false),
	fOperationsDebitSavingsMSR(false),
	fOperationsDebitCheckingMSR(false),
	fOperationsEBTMSR(false),
	fOperationsFoodStampsMSR(false),
	fOperationsGiftCardMSR(false),
	fOperationsPhoneCardMSR(false),
	fOperationsLoyaltyMSR(false),
	fOperationsUse7401MSR(true),
    nOperationsRemoteAssistModeToggleDisplay(0),
    nOperationsGiftCardRedemption(0),
    nOperationsMethodOfCollectionForEmptyGiftCard(0),
	fOperationsDisplayGiftCardBalance(false),
	fOperationsCenterCustomerDataNeededButtons(true), // RFC 266301
	fOperationsAllowDegradedMode(false), 	//SR742
	m_bOperationsDebitNeedsSignatureCapture(false), // Debit tender needs Signature Capture
	m_bOperationsDebitNeedsVerifySignature(false),  // Debit tender needs Verify Signature
	m_bOperationsSignatureCapture(false),
	m_bOperationsVerifySignature(false),
	m_bOperationsPrintSignatureReceipt(false),
	m_bOperationsAllowCancelAll(true),            //TAR 102011
	nOperationsAllowDispenseInTrainingMode(0),
	fOperationsInterventionAtStartOfTrainingModeTransaction(false), // RAP Intervention in Training Mode Transaction cs185088
    fOperationsLogDataForSystemReports(true),
    nOperationsNumberOfReportLogsToKeep(30),
	nOperationsGloryDispenseRetries(60),			// TAR 310660
	fOperationsCustomerBagAllowed(false), //SR085
	fOperationsIncludeCouponsInItemCount (true),	// Rfc 371075
	fPagingGreenFlashingSendPage(true),	// Start of In-Store Message Parameters
	fPagingYellowSendPage(true),
	fPagingRedSendPage(true),
	fPagingOutOfServiceSendPage(true),
	nPagingGreenFlashingElapseTime(0),
	nPagingYellowElapseTime(0),
	nPagingRedElapseTime(90),           // 1.5 minutes
	nPagingOutOfServiceElapseTime(600), // 10 minutes
	nPagingGreenFlashingLimitTime(30),
	nPagingGreenFlashingLimitTrans(15),                                     // TAR 117805
	nPagingAvailabilityCount(50),	// End of In-Store Message Parameters   // TAR 117805
	csDataMessageFile(_T("SCOTMSGE")),              // Message filename
	csDataBitmapFile(_T("SCOTBMPE")),               // Bitmap filename
	csDataSASIActionFile(_T("SASIActione")),        // Security action file
	csDataWaveFile(_T("SCOTWAVE")),					// Wav filename
	csDataTenderFile(_T("SCOTTEND")),				// Tender filename
	csDataTareFile(_T("SCOTTARE")),					// Tare filename
	csDataXMLFile(_T("FASTLANE3.1")),				// XML filename
	fCMRewardInfo(false),				// main option to configure reward information display
	fCMTotalSavings(false),				// option to show/hide total savings
	fCMTotalRewardPoints(false),		// option to show/hide total reward points
	fCMTotalSavingsOnLeft(true),			// option to show 'TotalSavings' on left on reward_info bar
	nCMCustomerMessage(-1),
    fMultiLabelBarCodesPrependFirstPrefixToItemCode(false), //TAR 266075
    fAutoRunOn(false),
    fAutoRunBypassBagging(true),
    nAutoRunNumberOfTransactionsToRun(100),
	fAutoRunRandomTimer(true),
    nAutoRunAttractDelay(0),
    nAutoRunItemPurchaseDelay(0),
    nAutoRunNumberOfItemsToSell(5),
    nAutoRunTotalCashToPay(1445),
    csAutoRunItemCodeToSell(_T("1111")),
    fAutoRunAssistModeOn(false),
    csAutoRunAssistKeyList(_T("")),
	csOperationsHandleUnDeActivatedItemAs(_T("N")),
	m_pcsMoviePlayOnAttract( NULL ),
	fUtilityExitButtonVisible(_T("")), 
    csUtilityExitPassword(_T("")), 
    csUtilityExitID(_T("")), 
    csUtilityDiagID(_T("")), 
    csUtilityDiagPassword(_T("")),  
    csUtilityRebootID(_T("")), 
    csUtilityRebootPassword(_T("")), 
    csUtilityShutdownID(_T("")), 
    csUtilityShutdownPassword(_T("")),  
    csUtilityVolumeID(_T("")), 
    csUtilityVolumePassword(_T("")),
    csUtilityTrainingDocument(_T("")),
    csUtilityInternetExplorerEXE(_T("")),
    m_iNumberOfLanguagesSupported(1),
    fStateTrainingProgramButtonVisible(false),
	//+4.2 provide subtle on-screen volume control
	fOperationsShowVolumeControl (false),    
	nOperationsVolumeControlIncrementLevel(20),
	nTimeOutVolumeControlTimer (15),
	nTimeOutSingleLanguageButtonAtAttractTimer(30),//RFQ SR681
	//-4.2 provide subtle on-screen volume control
	nTracingMillisecondsToFlush(0),  //THD Performance RFC
	fOperationsPrintDuplicateMsgonPrintLastReceipt(false), //RFC 334227
	fOperationsPrintReceiptOnCancelledTransaction(true), //sr218 dont print receipt on cancelled transaction, default = Y
	fOperationsForceDelayedApprovalBeforeSuspend(false),
	
	//SR81
	fOperationsImmediateInterventionForRestrictedItems(false),
	fOperationsImmediateInterventionForVisualVerifyItems(false),
	//SR81
	
	//+SR719
	fOperationsVolumeControlMuteBypass(false),
	//-SR719
	//+SR713
	fOperationsSellBagsAtFinalize(false),  
	csOperationsSellBagsItemCode(_T("")), 
	csOperationsBagPrice(_T("")),		
	fOperationsRepeatSellBags(false),	  
	//-SR713
	csOperationsEOTReceiptPrintingState(_T("Any")), //Delay receipt printing until finalization intervention complete gc185038
	fOperationsAddPaperCutToTransReceipt(true)	// RFC 368220 - Need option to disable CutReceipt() added to Transaction Receipt ac185097
	{
		LONG     lRC;
		HKEY     hKey;
		_TCHAR  szFilePath[_MAX_PATH+1];
		_TCHAR  szMachineName[_MAX_PATH+1];
		DWORD dwDataSize=_MAX_PATH;
		long lBarcodeLength = 0L; //RFC 212888 FFR/CMal
		long lTolerance = 0;	//Fix the problem with learning light wt item when already existing wt entry in WLDB
		DWORD dwType	= REG_DWORD; //RFC 212888 FFR/CMal
		CString csDefaultFilePath = _T("ScotConfigFilePath");
		CString csDefaultClientName = _T("ClientName");
		lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP"),
				 0, KEY_QUERY_VALUE, &hKey);
		ASSERT(lRC==ERROR_SUCCESS);

		// Get the file path
		lRC = RegQueryValueEx(hKey, csDefaultFilePath, NULL, NULL,
				  (LPBYTE)szFilePath, &dwDataSize);
		ASSERT(lRC==ERROR_SUCCESS ) ;

		dwDataSize = _MAX_PATH;
		// Get the client computer name
		lRC = RegQueryValueEx(hKey, csDefaultClientName, NULL, NULL,
				  (LPBYTE)szMachineName, &dwDataSize);

		if (lRC == ERROR_SUCCESS)
			csMachineName = szMachineName;

		::RegCloseKey(hKey); // Close key

		//NN RFC 210177 - Retrieve local machine name from registry
		_TCHAR  szLocalMachineName[_MAX_PATH+1];
		DWORD dwMachineNameSize=_MAX_PATH;
		CString csRegKeyName = _T("ComputerName");

		// CMal082803 - TAR 219659 change reg read from ControlSet001 to CurrentControlSet
		lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName"),
				 0, KEY_QUERY_VALUE, &hKey);
		//ASSERT(lRC==ERROR_SUCCESS);

		lRC = RegQueryValueEx(hKey, csRegKeyName, NULL, NULL,
			  (LPBYTE)szLocalMachineName, &dwMachineNameSize);

		if (lRC == ERROR_SUCCESS)
			csLocalMachineName = szLocalMachineName;

		::RegCloseKey(hKey);
		//RFC 210177 end
        //Begin Retrieve Reporting 3.1 feature business date information
		TCHAR  szDateSize[_MAX_PATH+1];
		DWORD dwDateSize=_MAX_PATH;
		CString csRegKey = _T("CurrentDate");
        lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Reporting\\ReportEOD"),
				 0, KEY_QUERY_VALUE, &hKey);

		lRC = RegQueryValueEx(hKey, csRegKey, NULL, NULL,
			  (LPBYTE)szDateSize, &dwDateSize);

		if (lRC == ERROR_SUCCESS)
			csBusinessDate = szDateSize;

		::RegCloseKey(hKey);
		//End Reporting 3.1 feature
		//RFC 212888 begin - Retrieve the intended max length of barcodes
		//char  szBarcodeLength[_MAX_PATH+1];
		DWORD dwBarcodeLength=sizeof(long);
		csRegKeyName = _T("DWORDOverrideMaxBarcodeLength");

		int iMaxTestedBarcodeLen = 46;

		lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT"), 0, KEY_QUERY_VALUE, &hKey);

		if (lRC == ERROR_SUCCESS)
		{
				// should have found the main key, now check

				lRC = RegQueryValueEx(hKey, csRegKeyName, NULL, &dwType, (BYTE *)&lBarcodeLength, &dwBarcodeLength);

				if (lRC == ERROR_SUCCESS)
				{
					// found the actual optional Reg value, ensure not over what we tested.

					if (lBarcodeLength > iMaxTestedBarcodeLen) // only can be 0 or positive on DWORD
						nMaxBarcodesLength = iMaxTestedBarcodeLen;	//default to max 46 characters
					else
						nMaxBarcodesLength = lBarcodeLength;	// use the value from the reg
				}
				else
					nMaxBarcodesLength = iMaxTestedBarcodeLen;	//No value found - default to max 46 characters


				::RegCloseKey(hKey); // close this Reg key
		}
		else
		{
			// in case can't find the Main reg value, set it to the default
			nMaxBarcodesLength = iMaxTestedBarcodeLen;	//default to max 46 characters
		}
		//RFC 212888 end - moved by FFR/CMal090402
		//RFC 214029 CM/NN090502 begin - Retrieve prefix value for 128 barcode printing
		_TCHAR  szPrefixValue[_MAX_PATH+1];
		//char  szLocalMachineName[_MAX_PATH+1];
		DWORD dwPrefixSize=_MAX_PATH;
		//DWORD dwPrefixLength=sizeof(long);
		csRegKeyName = _T("STRINGOverridePrefixOn128");

		lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT"), 0, KEY_QUERY_VALUE, &hKey);

		if (lRC == ERROR_SUCCESS)
		{
				// should have found the main key, now check

				//lRC = RegQueryValueEx(hKey, csDefaultCli, NULL, NULL,(LPBYTE)szMachineName, &dwDataSize);
				lRC = RegQueryValueEx(hKey, csRegKeyName, NULL, NULL,(LPBYTE)szPrefixValue, &dwPrefixSize);

				if (lRC == ERROR_SUCCESS)
				{
					// found the actual optional Reg value, ensure not over what we tested.
					if (szPrefixValue[0] == NULL)
						csGLOBALAddThe128Prefix = _T("");
					else
					{
						csGLOBALAddThe128Prefix = szPrefixValue; // use registry value

						// ensure that this value is expected
						if ((csGLOBALAddThe128Prefix.GetLength() != 0) &&
							(csGLOBALAddThe128Prefix != _T("{A")) &&
							(csGLOBALAddThe128Prefix != _T("{B")) &&
							(csGLOBALAddThe128Prefix != _T("{C")) )
						{
						    //csGLOBALAddThe128Prefix = "{C";	//not expected, make it default 
							csGLOBALAddThe128Prefix = _T("{A");	//not expected, make it default {A, alphanumeric password
						}
					}
				}
				else
				{
					// couldn't find the entry
					csGLOBALAddThe128Prefix = _T("{A"); //"{C";	//default 
				}


				::RegCloseKey(hKey); // close this Reg key
		}
		else
		{
			// in case can't find the Main reg value, set it to the default
			csGLOBALAddThe128Prefix = _T("{A"); //"{C";	//default 
		}
		// RFC 214029 CM/NN090502 end 
 
		//Fix the problem with learning light wt item when already existing wt entry in WLDB
		DWORD dwTolerance=sizeof(long);
		csRegKeyName = _T("LGWTolerance");

		lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Controls\\SmartScale"), 0, KEY_QUERY_VALUE, &hKey);

		if (lRC == ERROR_SUCCESS)
		{
				// should have found the main key, now check

				lRC = RegQueryValueEx(hKey, csRegKeyName, NULL, &dwType, (BYTE *)&lTolerance, &dwTolerance);

				if (lRC == ERROR_SUCCESS)
				{
					// found the actual optional Reg value, ensure not over what we tested.
					lLGWTolerance = lTolerance;
				}
				else
				{
					trace(L6, _T("Cannot find the value of LGWTolerance in the Registry and we will use this value to be 30"));
					lLGWTolerance = 30;
				}


				::RegCloseKey(hKey); // close this Reg key
		}

		GET_PARTITION_PATH(szFilePath, csConfigFilePath.GetBuffer(_MAX_PATH));
		csConfigFilePath.ReleaseBuffer();
	
	  // Registry entry for primary language code
	  static const TCHAR cszPrimaryCode[] = _T("PrimaryCode");
	  static const TCHAR cszRegScotLanguage[] = _T("SOFTWARE\\NCR\\SCOT\\Installation\\Language");
	
	  hKey = NULL;
	  CString csPrimaryCode;
	  long rc;

	  rc = RegOpenKeyEx( HKEY_LOCAL_MACHINE, cszRegScotLanguage, 0, KEY_READ, &hKey );
	  if( rc == ERROR_SUCCESS )
	  {
		   DWORD dwRegType;
		   DWORD dwDataSize;
		   
		   rc = RegQueryValueEx(hKey,cszPrimaryCode,0,&dwRegType, NULL, &dwDataSize);
		   if(rc == ERROR_SUCCESS && dwRegType == REG_SZ)
		   {
			   LPTSTR pBuffer = csPrimaryCode.GetBuffer(dwDataSize);
			   rc = RegQueryValueEx(hKey,cszPrimaryCode,NULL,&dwRegType,(LPBYTE)pBuffer,&dwDataSize); 
			   csPrimaryCode.ReleaseBuffer();
		   }
		   RegCloseKey(hKey);
	  }

	  csPrimaryLanguageCode = csPrimaryCode;

		// Tar 222910 start: Read SaySecuritySleepValue. Open the registry
		long lSleepValue = 0;
		DWORD dwSaySecSleepVal=sizeof(long);
		csRegKeyName = "SaySecuritySleepVal";
		bIgnoreSleep = false; // We will ignore sleep if 0 or Hex 999999 is defined
		lSaySecSleepVal = 100; // Default sleep value
		lRC = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTSOUND"), 0, KEY_QUERY_VALUE, &hKey);
		if (lRC == ERROR_SUCCESS)
		{
			lRC = RegQueryValueEx(hKey, csRegKeyName, 0, &dwType, (BYTE *)&lSleepValue, &dwSaySecSleepVal);
			if (lRC == 0)
			{
				// if the value for the key SaySecuritySleepVal is 0x000000 OR 0x999999
				// we will ignore sleep. 999999 is a number we came up with just in case, if we would like to skip sleep
				// and we are ignoring 0 as 0 for sleep may result is loop.
				lSaySecSleepVal = lSleepValue;
				if (lSleepValue == 10066329) 
				{
					bIgnoreSleep = true; // skip the sleep - Key Value was 0x999999
					trace(L6, _T("In ConfigObject: SaySecSleepVal was set to Hex 0x999999 which means do not sleep") );	
				}
				else
				{
					if (lSaySecSleepVal >= 10000)  // dont allow bigger than 10 sec
					{
						trace(L6, _T("In ConfigObject: SaySecSleepVal was too big %d ms (over 10sec) reset to .1 sec "), lSaySecSleepVal); 
						lSaySecSleepVal = 100;		// bulletproof step
					}

					bIgnoreSleep = false; // Really want to sleep for some period
				}
			}
			::RegCloseKey(hKey); // close this Reg key
		}
		else
		{
			// use default value, but say it in the trace
			trace(L6, _T("In ConfigObject: SaySecSleepVal set to default .1 sec - no reg override exist") );	
		}

		if (lSaySecSleepVal != 0)
		{
			trace(L6, _T("In ConfigObject: SaySecSleepVal is %d ms. "), lSaySecSleepVal);	
		}
		else
		{
			trace(L6, _T("In ConfigObject: SaySecSleepVal is Zero ms (NT interprets as give up thread control). "), lSaySecSleepVal); 
		}
		//Tar 222910 end

		//RFC381050+
		//Retrieve registry info for picklist image path
		_TCHAR  szPicklistImagePath[_MAX_PATH+1];
		dwDataSize =_MAX_PATH;
		csRegKeyName = _T("Default Image Path");

        lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT - PickList"),
				 0, KEY_QUERY_VALUE, &hKey);

		if (lRC == ERROR_SUCCESS)
		{
			lRC = RegQueryValueEx(hKey, csRegKeyName, NULL, NULL,
				  (LPBYTE)szPicklistImagePath, &dwDataSize);

			if (lRC == ERROR_SUCCESS)
				csPicklistImagePath = szPicklistImagePath;
			else
				csPicklistImagePath = SCOTPICKLISTIMAGE;

			::RegCloseKey(hKey);
		}
		else
		{
			csPicklistImagePath = SCOTPICKLISTIMAGE;
		}

		//Retrieve registry info for shrinking picklist images
		_TCHAR  szShrinkImageCMD[_MAX_PATH+1];
		dwDataSize =_MAX_PATH;
		csRegKeyName = _T("ShrinkImageCommand");

        lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT - PickList"),
				 0, KEY_QUERY_VALUE, &hKey);

		if (lRC == ERROR_SUCCESS)
		{
			lRC = RegQueryValueEx(hKey, csRegKeyName, NULL, NULL,
				  (LPBYTE)szShrinkImageCMD, &dwDataSize);

			if (lRC == ERROR_SUCCESS)
				csShrinkPicklistImageCMD = szShrinkImageCMD;
			else
				csShrinkPicklistImageCMD = SHRINKPICKLISTIMAGECMD;

			::RegCloseKey(hKey);
		}
		else
		{
			csShrinkPicklistImageCMD = SHRINKPICKLISTIMAGECMD;
		}
		//RFC381050-

		GET_PARTITION_PATH(DATAENTRYINIFILE, csDataEntryINIFile.GetBuffer(_MAX_PATH));
		csDataEntryINIFile.ReleaseBuffer();
	
		int i;
		for (i = 0; i < MAX_TENDER_CASHBACK_VALUES; i++)
			nTenderCashBack[i] = 0;

		for (i = 0; i < MAX_TENDER_EBT_CASHBACK_VALUES; i++)
			nTenderEBTCashBack[i] = 0;
	}

/////////////////////////////////////////////////////////////////////////////
// ConfigObject initialization
// use the cWinApp profile functions to read configuration options
// SCOT will use 2 options files
// 1) is a file SCOT:SCOTOPTS.000. This file contains options
// common to all SCOT terminals
// 2) is a file SCOT:SCOTOPTS.num. This file contains options
// unique to this SCOT terminal
// Note: any option that appear in 1) may also appear in 2). options
// in 2) override those in 1).

// the '000' and 'xxx' options file reside on the SCOT terminal
/////////////////////////////////////////////////////////////////////////////
long ConfigObject::Initialize(const int nTerminal)
{
	CString csFile1;
	CString csFile2;
	CString csFile3;

    // TAR #200002 HC 032602 Start
    // For testing/debugging, show a pop up message if return code from TB
    // is not TB_SUCCESS or TB_SUCCESS_IMMEDIATE if optional registry
    // entries are set to 1.
    long lResult = 0L;
    HKEY hKey = NULL;
    DWORD dwType = REG_DWORD;
    DWORD dwSize = sizeof(long);
    long lValue = 0L;
    // TAR #200002 HC 032602 End

	//Reset these values

	 bReadCreditNeedsPIN = false;
	 bReadCreditCashBack = false;
	 bReadDebitNeedsPIN = false;
	 bReadDebitCashBack = false;
	 bReadEBTFSNeedsPIN = false;
	 bReadEBTCBNeedsPIN = false;
	 bReadEBTCBCashBack = false;
	 bReadDebitNeedsSignatureCapture = false;
	 bReadDebitNeedsVerifyCapture = false;
	 bReadSignatureCapture = false;
	 bReadVerifyCapture = false;
	 bReadCreditMSR = false;
	 bReadDebitMSR = false;
	 bReadDebitSavingsMSR = false;
	 bReadDebitCheckingMSR = false;
	 bReadEBTMSR = false;
	 bReadFoodStampMSR = false;
	 bReadGiftCardMSR = false;
	 bReadLoyaltyMSR = false;
	 bReadPIPSCOTPromptCardType = false;
	 bReadPIPSCOTPromptCashBack = false;
	 bReadPIPSCOTPromptConfirmTotal = false;

	 //currency bitmap start
		LONG	lRC;
//		HKEY	hKey;
		CString csCountryCode = _T("CountryCode");	//Value to look for under key
		DWORD dwDataSize=CURRENCY_CODE_SIZE;
		_TCHAR  szCountryCode[CURRENCY_CODE_SIZE+1];

		lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\Installation\\Currency"),
				 0, KEY_QUERY_VALUE, &hKey);
		ASSERT(lRC==ERROR_SUCCESS);

		if (!lRC==ERROR_SUCCESS)
		{
			ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_MISSING_OPTION,_T("%s|%s|%s"),
				_T("RegistryKey"), _T("Currency"), CURRENCY_BITMAP_DEFAULT);
			trace(L0, _T("Currency Key was not found in the Registry, default currency bitmap will be used"));
			co.csCurrencyBitmap = CURRENCY_BITMAP_DEFAULT;
		}
		else
		{
			dwDataSize = CURRENCY_CODE_SIZE;


			lRC = RegQueryValueEx(hKey, csCountryCode, NULL, NULL,
				  (LPBYTE)szCountryCode, &dwDataSize);
			ASSERT(lRC==ERROR_SUCCESS ) ;

			if (lRC == ERROR_SUCCESS)
			{
				co.csCurrencyBitmap = szCountryCode;
				if (!co.csCurrencyBitmap.GetLength())
				{

					ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_MISSING_OPTION,_T("%s|%s|%s"),
					_T("RegistryKeyValue"), _T("CountryCode"), CURRENCY_BITMAP_DEFAULT);
					trace(L0, _T("CountryCode value used for currency was NULL in the Registry, default currency bitmap will be used"));

					co.csCurrencyBitmap = CURRENCY_BITMAP_DEFAULT;
				}
				else if((co.csCurrencyBitmap != _T("USD")) &&
					(co.csCurrencyBitmap != _T("CDN")) &&
					(co.csCurrencyBitmap != _T("GBPe")) &&
					(co.csCurrencyBitmap != _T("GBPs")) &&
					(co.csCurrencyBitmap != _T("GBPc")) &&
					(co.csCurrencyBitmap != _T("GBPr")) &&
					(co.csCurrencyBitmap != _T("DEM")) &&
					(co.csCurrencyBitmap != _T("NLG")) &&
					(co.csCurrencyBitmap != _T("EUR")) &&     //Invalid country code Tar 176050
                    (co.csCurrencyBitmap != _T("AUD")) &&
                    (co.csCurrencyBitmap != _T("JPY")) &&
					(co.csCurrencyBitmap != _T("NZD")) )
				{
					CString csTemp = _T("CountryCode <") + co.csCurrencyBitmap + _T(">");
					ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_INVALID_CURRENCY_COUNTRY_CODE,_T("%s|%s|%s"),
							_T("SOFTWARE\\NCR\\SCOT\\Installation\\Currency"), csTemp, _T("USD"));
					trace(L0, _T("CountryCode %s in Currency Key in the Registry is invalid, default currency USD bitmap will be used"), co.csCurrencyBitmap );

					co.csCurrencyBitmap = CURRENCY_BITMAP_DEFAULT;
				}



			}
			else
			{
				ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_MISSING_OPTION,_T("%s|%s|%s"),
					_T("RegistryKeyValue"), _T("CountryCode"), CURRENCY_BITMAP_DEFAULT);
				trace(L0, _T("CountryCode subkey used for currency was not found in the Registry, default currency bitmap will be used"));
				co.csCurrencyBitmap = CURRENCY_BITMAP_DEFAULT;
			}

		}

		::RegCloseKey(hKey);



	 //currency bitmap end


	//Move all config files from SCOT to SCOT\CONFIG
	csFile1.Format(csConfigFilePath + _T("\\") + SCOTOPTS +_T(".DAT"));
	csFile2.Format(csConfigFilePath + _T("\\") + SCOTOPTS + _T(".%03d"),0);
	csFile3.Format(csConfigFilePath + _T("\\") + SCOTOPTS + _T(".%03d"),nTerminal);

	// see if scotopts.dat exists or not

	// read each option from scotopts.dat
	// set the option value from defualt to configured value
	// if option is set in scotopts.dat, set the option value from the current value to configured value
    g_MultiLabelBarcodes.RemoveAllRecords();

	// repeat step3 for all options
	if (SMStateBase::OpenSCOTFile(csFile1, true, true))
	{
		if (cstoreCode.NumberOfInstoreCodesLoaded())
		cstoreCode.RemoveAllCList();					//remove list of In-store codes
		InitAllOptions(csFile1);
	}

	// see if scotopts.000 exists or not
	// free the current profile name buffer and set the new options file name
	if (SMStateBase::OpenSCOTFile(csFile2, false, false))
	{
		InitAllOptions(csFile2);
	}

	// see if scotopts.xyz exists or not
	// free the current profile name buffer and set the new options file name
	if (SMStateBase::OpenSCOTFile(csFile3, false, false))
	{
		InitAllOptions(csFile3);
	}

	// CMal091703 TAR 248007  - final step of allowing Add/replace/remove via .000 
	//									 must clean away the temporary 'zero' placeholders
	int iSeeNum = -99;
	int iValueToClean = 0;

	// sets co.nTenderCashBackNum & cleans co.nTenderCashBack array 
	_CLEAN_INT_ARRAY(Tender,CashBack, MAX_TENDER_CASHBACK_VALUES, iValueToClean);	
	iSeeNum = nTenderCashBackNum;  // allow debug
	
	// sets co.nTenderEBTCashBackNum & cleans co.nTenderEBTCashBack array
	_CLEAN_INT_ARRAY(Tender,EBTCashBack, MAX_TENDER_EBT_CASHBACK_VALUES, iValueToClean);	
	iSeeNum = nTenderEBTCashBackNum;  // allow debug

	// CMal091703 TAR 248007 - end 


//NN RFC 210177
  misMatchTrace.nCurrentLevel = 1;              // current trace level -1,0...9
  misMatchTrace.nFocusLevel = 7;                // focused trace level
  misMatchTrace.m_nEntriesToFlush=1;         // running count indicating number of lines before flush
  misMatchTrace.m_bTraceOn = co.fOperationsLogWLDBmismatch;
  misMatchTrace.m_nMaxLogSize = co.nOperationsWLDBmismatchMaxFileSize;                // Maximum size of traces.log(KB).  Eventually, it should replace MaxNumberofEntries ;
  misMatchTrace.lFocusComps |= ID_SM;
 //NN RFC 210777 end

	fErrorLog = false;  //RFC 262774 - trace errors instead

	CheckRange(nTimeOutSyncTimeout,0,180000);
	CheckRange(nTimeOutTender,0,180);
	CheckRange(nTimeOutStateDefault,0,180);
	CheckRange(nTimeOutTakeCard,0,180);
	CheckRange(nTimeOutTakeCash,0,180);
	CheckRange(nTimeOutTakeCoins,0,180);
	CheckRange(nTimeOutItemTransactionResult,0,180);
	CheckRange(nTimeOutContinueTransaction,0,180);
	CheckRange(nTimeOutHelp,0,180);
	CheckRange(nTimeOutSignon,0,180);
	CheckRange(nTimeOutSignoff,0,180);
	CheckRange(nTimeOutAttract,0,180);
	CheckRange(nTimeOutItemSaleInProgress,0,180);
	CheckRange(nTimeOutEASDeactivationScreenTimer,0,180);
    CheckRange(nTimeOutCouponSensorScreenTimer,0,180);
	CheckRange(nTimeOutDepositGiftCardScreenTimer,0,180);
	CheckRange(nTimeOutTenderBalanceScreenTimer,0,180);
	CheckRange(nTimeOutCashOutSensorAudioTimer,0,180);
	CheckRange(nTimeOutCheckBasketReminder,0,180);
    CheckRange(nTimeOutRequestSig,0,180);


	CheckRange(nTimeOutInitialBagItemAudio,0,180);
	CheckRange(nTimeOutPeriodicBagItemAudio,0,180);
	CheckRange(nTimeOutBagItemEscalationMilliSecs,0,180000);          // checking against milliseconds
	CheckRange(nTimeOutInitialRemovingItemsAudio,0,180);
	CheckRange(nTimeOutPeriodicRemovingItemsAudio,0,180);
	CheckRange(nTimeOutRemovingItemsEscalationMilliSecs,0,180000);    // checking against milliseconds
	CheckRange(nTimeOutTakeYourItemEscalationMilliSecs,0,180000);   //RFC209702
	CheckRange(nTimeOutWalkAwayTimerAudio,0,180);
	CheckRange(nTimeOutInitialTakeYourItemAudio,0,180);
	CheckRange(nTimeOutRecurringTakeYourItemAudio, 0, 180);  //RFC209702
	CheckRange(nTimeOutSecurityServerTimer,0,10); //for attract quiet mode
	CheckRange(nTimeOutThankYouTimerBelt,0,180);
	CheckRange(nTimeOutInitialClearBagAreaAudio,0,180);
	CheckRange(nTimeOutPeriodicClearBagAreaAudio,0,180);
	CheckRange(nTimeOutTimeOutTransportItem,0,180);
    CheckRange(nTimeOutHeartbeatTimer,0, 86400);   //RFC 262339
    CheckRange(nTimeOutBusyLeaseTimer,0,1440);          //RFC 263404
	CheckRange(nTimeOutTouchScreenProximityEnablesScannerMotor,0,120);
	CheckRange(nTimeOutDisplayBagItemScreenTimer,0,180);
	CheckRange(nTimeOutDelaySecurityNotificationMilliSecs,0,180000);
	CheckRange(nTimeOutPeriodicSecurityAudio,0,180);
	CheckRange(nTimeOutAttendantModeTimer,0, 32767);
	CheckRange(nTimeOutGloryUnitDispenseTimeout, 0, 2000);	// TAR 310660
	CheckRange(nOperationsGloryDispenseRetries, 0, 100);	// TAR 310660
	CheckRange(nTimeOutExitSuspendScreenTimer,0,180);		// RFC 294962
	CheckRange(nLocaleNoteCassetteLocation, 0, 1);			// SafePay
	CheckRange(nLocaleXferToCassetteMode, 0, 3);			// SafePay
	CheckRange(nTimeOutDisplayPutBagOnScaleScreenTimer,0,180);//SR085
	CheckRange(nTimeOutDisplayIncreaseMisMatchScreenTimer,0,180); //TAR379130/379131
	CheckRange(nTimeOutEnableSkipBaggingButtonTimer,0,180000);//SR778

	trace(L0,_T("Common Timeout Syncronize Time - amount time to wait for the SyncTime batch file to run: %d"), nTimeOutSyncTimeout);
	trace(L0,_T("Common  Trace level = %d, Focus level = %d, Focus IDs = %s, Max Log Size = %d"),
	//	nTracingNumEntries,
	nTracingCurrentLevel,
	nTracingFocusLevel,
	csTracingFocusComps,
	nTracingMaxLogSize);

	trace(L0,_T("Tender, SubmitFSTtotalIfMorethanTrxTotal= %d"), fTenderSubmitFSTtotalIfMorethanTrxTotal);
	trace(L0,_T("Tender, IsMotorizedMSR = %d"), fTenderIsMotorizedMSR);
	trace(L0,_T("Tender, fTenderDispenseChangeAtLaneForRemoteTendering = %d"), fTenderDispenseChangeAtLaneForRemoteTendering);   //For remote tendering transaction, dispense change at Lane side or pop RAP cash drawer at RAP side. The default value is Y

	//trace(L0,"Tender, PIPPINEncryptorOnly = %d", fTenderPIPPINEncryptorOnly);	// future use
	trace(L0,_T("Show Key In Code button %d"), fStateAllowKeyInCode);	//TAR 173138/9
    trace(L0,_T("Show Look Up Item button %d"), fStateAllowLookUpItem);	//TAR 173138/9
	trace(L0,_T("Common TimeOut Tender %d"), nTimeOutTender);
	trace(L0,_T("Common TimeOut StateDefault %d"), nTimeOutStateDefault);
	trace(L0,_T("Common TimeOut TakeCard %d"), nTimeOutTakeCard);
	trace(L0,_T("Common TimeOut TakeCash %d"), nTimeOutTakeCash);
	trace(L0,_T("Common TimeOut TakeCoins %d"), nTimeOutTakeCoins);
	trace(L0,_T("Common TimeOut ItemTransactionResult %d"), nTimeOutItemTransactionResult);
	trace(L0,_T("Common TimeOut ContinueTransaction %d"), nTimeOutContinueTransaction);
	trace(L0,_T("Common TimeOut Help %d"), nTimeOutHelp);
	trace(L0,_T("Common TimeOut Signon %d"), nTimeOutSignon);
	trace(L0,_T("Common TimeOut Signoff %d"), nTimeOutSignoff);
	trace(L0,_T("Common TimeOut Attract %d"), nTimeOutAttract);
	trace(L0,_T("Common TimeOut Item sale in progress %d"), nTimeOutItemSaleInProgress);
	trace(L0,_T("Common TimeOut EAS Deactivation Audio %d"), nTimeOutEASDeactivationScreenTimer);
	trace(L0,_T("Common TimeOut Coupon Sensor Audio %d"), nTimeOutCouponSensorScreenTimer);
	trace(L0,_T("Common TimeOut Tender Balance Audio %d"), nTimeOutTenderBalanceScreenTimer);
	trace(L0,_T("Common TimeOut Deposit Gift Card Audio %d"), nTimeOutDepositGiftCardScreenTimer);
    trace(L0,_T("Common TimeOut Cash Out Sensor Audio %d"), nTimeOutCashOutSensorAudioTimer);
	trace(L0,_T("Common TimeOut Check Basket Reminder %d"), nTimeOutCheckBasketReminder);
	trace(L0,_T("Common TimeOut Request Sig %d"), nTimeOutRequestSig);
	trace(L0,_T("Common TimeOut SAInProgress %d"), nTimeOutTimeOutTransportItem);
	trace(L0,_T("Common TimeOut Initial Bag Item Audio %d"), nTimeOutInitialBagItemAudio);
	trace(L0,_T("Common TimeOut Periodic Bag Item Audio %d"), nTimeOutPeriodicBagItemAudio);
	trace(L0,_T("Common TimeOut Bag Item RAP Escalation %d"), nTimeOutBagItemEscalationMilliSecs);
	trace(L0,_T("Common TimeOut Initial Removing Items Audio %d"), nTimeOutInitialRemovingItemsAudio);
	trace(L0,_T("Common TimeOut Periodic Removing Items Audio %d"), nTimeOutPeriodicRemovingItemsAudio);
	trace(L0,_T("Common TimeOut Removing Items RAP Escalation %d"), nTimeOutRemovingItemsEscalationMilliSecs);
	trace(L0,_T("Common TimeOut Take Your Item RAP Escalation %d"), nTimeOutTakeYourItemEscalationMilliSecs); //RFC209702
	trace(L0,_T("Common TimeOut Walk Away Timer Audio %d"), nTimeOutWalkAwayTimerAudio);
	trace(L0,_T("Common TimeOut Take Your Item Audio %d"), nTimeOutInitialTakeYourItemAudio);
	trace(L0,_T("Common TimeOut Take Your Item Recurring Audio %d"), nTimeOutRecurringTakeYourItemAudio);  //RFC209702

	trace(L0,_T("Common TimeOut Security Server Timer %d"),nTimeOutSecurityServerTimer);
	trace(L0,_T("Common TimeOut Thank You Timer Belt %d"), nTimeOutThankYouTimerBelt);
	trace(L0,_T("Common TimeOut Initial Clear Bag Area Audio %d"), nTimeOutInitialClearBagAreaAudio);
	trace(L0,_T("Common TimeOut Periodic Clear Bag Area Audio %d"), nTimeOutPeriodicClearBagAreaAudio);
	trace(L0,_T("Common TimeOut TouchScreen Proximity Enables Scanner Motor %d"), nTimeOutTouchScreenProximityEnablesScannerMotor);
	trace(L0,_T("Common TimeOut Motion Sensor On Attract Interval %d"), nTimeOutMotionSensorOnAttractInterval); //RFC 265465
	trace(L0,_T("Common Timeout Scanner Scale On Attract Interval %d"), nTimeOutScannerScaleOnAttractInterval);//RFC 276235
	trace(L0,_T("Common TimeOut Display Bag Item Screen %d"), nTimeOutDisplayBagItemScreenTimer);
    trace(L0,_T("Common TimeOut Delay Security Notification %d"), nTimeOutDelaySecurityNotificationMilliSecs);
	trace(L0,_T("Common TimeOut Security Audio Timer %d"), nTimeOutPeriodicSecurityAudio);
    trace(L0,_T("Common TimeOut Attendant Mode Timer %d"), nTimeOutAttendantModeTimer);
	trace(L0,_T("Common TimeOut Exit Suspend Screen %d"), nTimeOutExitSuspendScreenTimer);		//RFC 294962
	trace(L0,_T("Common TimeOut Display PutBagOnScale Screen %d"), nTimeOutDisplayPutBagOnScaleScreenTimer);//SR085
	trace(L0,_T("Common TimeOut Display Mismatch or UnexpectedIncrease Screen %d"), nTimeOutDisplayIncreaseMisMatchScreenTimer);  //TAR379130/379131
    trace(L0,_T("Common TimeOut Enable Skip Bagging Button Timer %d"), nTimeOutEnableSkipBaggingButtonTimer); //SR778
	
	trace(L0,_T("Common SayPrices = %d,  SayPhrases = %d,  AllowTBAudioControl = %d, SayPricesAttendantMode = %d"),
	fStateSayPrices,
	fStateSayPhrases,
    fStateAllowTBAudioControl, 
    !fStateSayPricesDisableInAttendantMode);

    trace(L0, _T("Common TrainingProgramButtonVisible = %d"), fStateTrainingProgramButtonVisible);

	trace(L0, _T("Center CMDataNeeded buttons? %d"),fOperationsCenterCustomerDataNeededButtons);
	trace(L0, _T("UseFingerPrintDevice = %d"),m_bOperationsUseFingerPrintDevice);  //SR93 Biometrics
	trace(L0, _T("DebitNeedsSignatureCapture = %d"), m_bOperationsDebitNeedsSignatureCapture);
	trace(L0, _T("DebitNeedsVerifySignature = %d"), m_bOperationsDebitNeedsVerifySignature);
	trace(L0, _T("CreditNeedsSignatureCapture = %d"), m_bOperationsSignatureCapture);
	trace(L0, _T("CreditNeedsVerifyCapture = %d"), m_bOperationsVerifySignature);

	trace(L0,_T("Common LiveVideo = %d"),fStateLiveVideo);
	trace(L0,_T("Common WelcomeScreenVideo = %d"),fStateWelcomeScreenVideo);
	trace(L0,_T("Common SelectListScreenVideo = %d"),fStateSelectListScreenVideo);
	trace(L0,_T("Common GraphicalSelectList = %d"),fStateGraphicalSelectList);
	trace(L0,_T("Common ProduceFavorites = %d"),fStateProduceFavorites);
	trace(L0,_T("Common OverridePickDesc = %d"),fStateOverridePickDesc);
	trace(L0,_T("Default Produce Bitmap = %s"),csStateDefaultProduceBMPName);
	trace(L0,_T("The invalid Prefix is = %s"),csStateInvalidCouponPrefix); //TAR 269159
	trace(L0,_T("Common Display Weights and Measures = %d"),fStateDisplayWeightsAndMeasures);
	trace(L0,_T("Common LargeFontOnCurrentItem = %d"),fStateLargeFontOnCurrentItem);
	trace(L0,_T("Common AllowInterruptWavFiles = %d"),fStateAllowInterruptWavFiles);
	trace(L0,_T("Common ForcePPWUWeighing = %d"),fStateForceWeighingOFPriceEmbedded);
	trace(L0,_T("Common Suspend Confirmation Screen = %d"), fStateConfirmationForSuspendingTransaction);
	trace(L0, _T("Check Basket Reminder Screen  = %d"), fStateCheckBasketReminder);
	trace(L0, _T("Coupon Sensor = %d"), fStateCouponSensor);
	trace(L0, _T("Date of Birth Not Needed = %d"), fStateDOBNotNeededButton);
	trace(L0, _T("Display The Number Of Items On RAP = %d"), fStateDisplayNumberOfItemsOnRAP);
	trace(L0,_T("Movie file on Attract = %s"),csStateMoviePlayOnAttract);
	trace(L0,_T("Movie file on Attract for Takeaway Belt = %s"),csStateMoviePlayOnAttractBelt);
	trace(L0,_T("Change location of System Functions button = %d"), fStateSystemFunctionsBtnOnLaneStatusScreen);
	trace(L0,_T("RapInitiateTransaction = %s"), csStateRapInitiateTransaction);
    trace(L0,_T("Escalate TakeYourItemsTimeout = %d"), fStateEscalateTakeYourItemsTimeout);
	trace(L0,_T("PaintScanAndBagOnInitialScan= %d"), fStatePaintScanAndBagOnInitialScan);
	trace(L0,_T("Allow Language Selection During Transaction? = %d"), fStateAllowLanguageSelectionDuringTransaction);
    trace(L0,_T("ForceTrxStartOnSwipe = %d"), fStateForceTrxStartOnSwipe);
    trace(L0,_T("AttndModeAllowed = %d"), fStateAttndModeAllowed);
    trace(L0,_T("AttndModeLoginBypass = %d"), fStateAttndModeAllowed);
    trace(L0,_T("AttndModeImmedInterventions = %d"), fStateAttndModeAllowed);
	trace(L0, _T("Check Cash Changer? %d"), fOperationsCheckCashChanger);    // TAR 256141
    trace(L0,_T("OperationsReprintReceiptByTransNumber = %d"), fOperationsReprintReceiptByTransNumber);
    trace(L0,_T("OperatorTwoLineText = %d"), fOperationsReprintReceiptByTransNumber);
	trace(L0,_T("HopperSubstitutionIntervention = %d"), fOperationsHopperSubstitutionIntervention);
	trace(L0,_T("AssistMenuFromStoreModeAllowed = %d"), fStateAssistMenuFromStoreModeAllowed);
	trace(L0,_T("AssistMenuFromRAPAllowed = %d"), fStateAssistMenuFromRAPAllowed); 
	trace(L0,_T("StartupToLaneClosed = %d"), fStateStartupToLaneClosed);  //RFC 986
	trace(L0,_T("StartupWaitForTB = %d"), fStateStartupWaitForTB);		  //RFC 986
	trace(L0,_T("HopperFixLaterTrxLimit = %d"), nOperationsHopperFixLaterTrxLimit);
	trace(L0,_T("AllowDegradedMode = %d"), fOperationsAllowDegradedMode);	//SR742
    
    // file 2 has: (* means is optional, overrides file 2 if present)

	// *[State]
	// *SayPrices  = Y			flag value Y/N
	// *SayPhrases = Y		flag value Y/N
    // *AllowTBAudioControl = N
	// *LiveVideo  = N;		flag value Y/N
	// *WelcomeScreenVideo = Y
	// *SelectListScreenVideo = Y
	// *LargeFontOnCurrentItem = N
	// *AssistMode = Y;		flag value Y/N
	// *fStateLookupItemIsButton1 = Y
	// *fStateFlashLookupItemButton = Y
	// *fStateFlashKeyInCodeButton = Y

	// [TimeOut] (timeout values in seconds)
	// Tender				 wait for supermarket tender processing
	// StateDefault		   default for base state
	// TakeCard			   wait to eat the card
	// TakeCash			   wait to take back paper money
	// TakeCoins			  wait to take back coin change
	// ItemTransactionResult  wait for supermarket item processing
	// ContinueTransaction	wait at continue transaction screen
	// Help				   wait at help screen
	// Signon				 wait to sign on from Attract
	// Signoff				wait to sign off from Attract if already signed on
	// Attract				Attract screen timeout
	// Item sale in progress  time out value for item sale complete
	// Check Basket Reminder  wait at Check Basket Reminder Screen
	// FirstTimeUser		  First Time User state

	// *[Tracing]
	// *NumEntries=0		  number of entries from start up
	// *CurrentLevel=0		general tracing level at IPL
	// *FocusLevel=0		  focused trasing level at startup
	// *FocusComps=""		 focus Comp_IDs, TB,PS,MP,ST,DM... see errorobject.h
	///////////////////////////////////////////////////////////

	trace(L0,_T("IsCurrencySymbolOnEReceiptCol1 %d"), fLocaleIsCurrencySymbolOnEReceiptCol1);
	trace(L0,_T("IsCurrencySymbolOnEReceiptCol2 %d"), fLocaleIsCurrencySymbolOnEReceiptCol2);
	trace(L0,_T("IsTaxLine %d"), fLocaleIsTaxLine);
	trace(L0,_T("CashValueList %s"), csLocaleCashValueList);
	//trace(L0,"CashSafeList %s", csLocaleCashSafeList);
	trace(L0,_T("CashManagementScreen %d"), fLocaleCashManagementScreen );
	trace(L0,_T("CashManagementDisabledInTransaction %d"), fLocaleCashManagementDisabledInTransaction ); //TAR 370108
	trace(L0,_T("AllowSoftwareUnload %d"), fLocaleAllowSoftwareUnload );
	trace(L0,_T("AllowSoftwareRemoval %d"), fLocaleAllowSoftwareRemoval );
	trace(L0,_T("CashLowList %s"), csLocaleCashLowList);
	trace(L0,_T("CashHighList %s"), csLocaleCashHighList);  //+dp185016 support glory device // High thresholds 
	trace(L0,_T("CashCapacity %s"), csLocaleCashCapacity);  //-dp185016 capacity
	trace(L0,_T("CashLowListForCoinHopper %s"), csLocaleCashLowListForCoinHopper);  
	trace(L0,_T("CashCapacityForCoinHopper %s"), csLocaleCashCapacityForCoinHopper);  
	trace(L0,_T("CashValueListForCoinHopper %s"), csLocaleCashValueListForCoinHopper); 

	trace(L0,_T("CashRemoveAmount %s"), csLocaleCashRemoveAmount);  //-dp185016 capacity
	trace(L0,_T("fLocaleCashManagementDisplayCount %d"), fLocaleCashManagementDisplayCount);  //+dp185016 generic cash management
	trace(L0,_T("fLocaleCashManagementAPPTrackCashCount %d"), fLocaleCashManagementAPPTrackCashCount);  
	trace(L0,_T("nLocaleCashAcceptorCapacity %d"), nLocaleCashAcceptorCapacity);  
	trace(L0,_T("nLocaleCoinAcceptorCapacity %d"), nLocaleCoinAcceptorCapacity);  
	trace(L0,_T("nLocaleCashAcceptorHighThreshold %d"), nLocaleCashAcceptorHighThreshold);  
	trace(L0,_T("nLocaleCoinAcceptorHighThreshold %d"), nLocaleCoinAcceptorHighThreshold);  //-dp185016
	trace(L0,_T("AllowNoteCoverRemoval: %d"), fLocaleAllowNoteCoverRemoval); 
	trace(L0,_T("AllowNoteCassetteRemoval: %d"),fLocaleAllowNoteCassetteRemoval); 
	trace(L0,_T("NoteCassetteLocation: %d"), nLocaleNoteCassetteLocation); 
	trace(L0,_T("XferToCassetteMode: %d"),   nLocaleXferToCassetteMode);  
	trace(L0,_T("AllowBillUnload: %d"),      fLocaleAllowBillUnload);  
	trace(L0, _T("CashLowListForNewEnhancedCoinLowSensor %s"), csLocaleCashLowListForNewEnhancedCoinLowSensor);
	

	// ---------------------------------------------------------
	// Read Coin Dispenser model
	// ---------------------------------------------------------
	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\ADD\\CoinDispenser"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		DWORD dwValueSize=60;   //Tar 237697
		_TCHAR  szValue[61];
	

		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("DetectedDevice"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
           CString csWork = szValue;
		   csWork.MakeUpper();
		   trace(L6, _T("ReadDevicesModel(). The model of Coin Dispenser in registry is %s."), csWork);
		   if (csWork.Find(_T("TELEQUIPHOPPER")) != -1)
		   {
			    // use options for hopper
			   if(csLocaleCashLowListForCoinHopper != _T(""))
					csLocaleCashLowList = csLocaleCashLowListForCoinHopper;
			   if(csLocaleCashCapacityForCoinHopper != _T(""))
					csLocaleCashCapacity = csLocaleCashCapacityForCoinHopper;
			   if(csLocaleCashValueListForCoinHopper != _T(""))
					csLocaleCashValueList = csLocaleCashValueListForCoinHopper;
		   }

		}
	}
	else
	{
		trace(L6, _T("ReadDevicesModel() did not find the ADD registry key of CoinDispenser model."));
	}
	::RegCloseKey(hKey);

  //LoadCashValueLists(csLocaleCashValueList, csLocaleCashLowList);
  LoadCashValueLists(csLocaleCashValueList, csLocaleCashLowList, csLocaleCashHighList, csLocaleCashCapacity, csLocaleCashRemoveAmount ); //dp185016 support glory device // High thresholds 

	//
	// Important Notes
	// ===============
	//
	// ; if CurrencySettingNotFromRegionalSetting=N or abscent of the scotopts.000 file
	// ; we take all values from Regional Setting of Control Panel, that is the default.
	//
	// ; if CurrencySettingNotFromRegionalSetting=Y
	// ; we take all values from this file.
	//
	// CurrencySettingNotFromRegionalSetting=Y
	//
	// ; ---------------------------------------------------------------------------
	// ; Sample TURKISH Settings (if  CurrencySettingNotFromRegionalSetting=Y)
	// ; ---------------------------------------------------------------------------
	// ; CurrencySymbol=TL
	// ; PositiveCurrencyFormat=3
	// ; NegativeCurrencyFormat=8
	// ; DecimalSymbol=.
	// ; NoOfDigitsAfterDecimal=0
	// ; DigitGroupingSymbol=,
	// ; NoOfDigitsInGroup=3
	// ; ------------------------------------------------------

	// ; HELP on Positive/Negative CurrencyFormat
	// ; ------------------------------------------------------
	// ; Values for the field PositiveCurrencyFormat
	// ; ------------------------------------------------------
	// ; 0 Prefix, no separation
	// ; 1 Suffix, no separation
	// ; 2 Prefix, 1-char. separation
	// ; 3 Suffix, 1-char. separation

	// ; ------------------------------------------------------
	// ; Values for the field NegativeCurrencyFormat
	// ; ------------------------------------------------------
	// ; 0 ($1.1)
	// ; 1 -$1.1
	// ; 2 $-1.1
	// ; 3 $1.1-
	// ; 4 (1.1$)
	// ; 5 -1.1$
	// ; 6 1.1-$
	// ; 7 1.1$-
	// ; 8 -1.1 $ (space before $)
	// ; 9 -$ 1.1 (space after $)
	// ; 10 1.1 $- (space before $)
	// ; 11 $ 1.1- (space after $)
	// ; 12 $ -1.1 (space after $)
	// ; 13 1.1- $ (space before $)
	// ; 14 ($ 1.1) (space after $)
	// ; 15 (1.1 $) (space before $)
	// ; ------------------------------------------------------

	// EndCodeLocale

	//If there is no scale, turn price embedded weighing off.
	/*tar 193558
	if(!dm.IsScaleEnable())
	{
		fStateForceWeighingOFPriceEmbedded = false;

		fStateSubstCkPriceEmbeddedWeights = false;
		fStateDisplayWeightsAndMeasures = false;
		ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_FORCED_TO_OVERRIDE_OPTION, "%s", "ForceWeighingOFPriceEmbedded"); // event log
		trace(L2, "Scot forced to override ForceWeighingOFPriceEmbedded option.");
	}
*/
    if (!fStateLiveVideo)
	{
	  fStateWelcomeScreenVideo= false;
	  fStateSelectListScreenVideo = false;
	}

	if (fCMRewardInfo)
	{
		//ps.SetCMReceipt(&CMRewardReceipt); //*** NewUI ***//
		if (-1 == nCMCustomerMessage)
		{
			// so, customer name/message has to hide
			csCMCustomerMessage.Empty();
		}
		else
		{
			csCMCustomerMessage = ps.GetPSText(nCMCustomerMessage);
		}
	}
	else
	{
		//ps.SetCMReceipt(&CMReceipt); //*** NewUI ***//
	}

	if (getMoviePlayOnAttract()->GetLength())
	{
		// Koteswara:
		// Movie file playing on attract screen resulted to some poor performance
		// while performing transactions. So, on Tim Mason's request, this feature is temporarily
		// disabled.

		// ASUI delivered a fix for ASGraphic.ocx which is supposed to fix the problem in addition
		// to disabling audio for the movie. When this feature has to be enabled, please use the ASUI
		// fix and retest the performance issues.

		// testing ....
		// `
		//co.csStateMoviePlayOnAttract.Empty();
	}
	// get movie file from TB to play on attract screen
	PullMovieFile( csStateMoviePlayOnAttract );
	PullMovieFile( csStateMoviePlayOnAttractBelt );
	if( getMoviePlayOnAttract()->GetLength())
		fStateWelcomeScreenVideo= false;

/*	Made the following code a private method
	if (getMoviePlayOnAttract().GetLength())
	{
		if (0 == getMoviePlayOnAttract().CompareNoCase("N"))
		{
			// MediaPlay option is off, allow displaying video feedback
			setMoviePlayOnAttract(_T(""));
		}
		else
		{
			CString csDestNameWithPath = csConfigFilePath + "\\" + getMoviePlayOnAttract();
			TBRC nTBCallStatus = tb.CopyFileFromServer(getMoviePlayOnAttract(),csDestNameWithPath,false);	// destination path is null


			CFileFind FileSearchObj;
			if (!FileSearchObj.FindFile(csDestNameWithPath))
			{
				// movie file doesn't exist, log it, don't load it and don't show it
				ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_PS_FILE, "%s",getMoviePlayOnAttract()); // event log
				setMoviePlayOnAttract(_T(""));
				// trace it
			}
			else
			{
				// attract movie file exists, no video on attract
				co.fStateWelcomeScreenVideo= false;
				setMoviePlayOnAttract( csDestNameWithPath );
			}
			FileSearchObj.Close();
		}
	}
*/

	//begin TAR#102019	allow coupons has 3 options now; A=All modes, O=Only in assist mode, N=No modes
    if (csTenderAllowCoupons == _T("N"))
	{
	   nOperationsMaxCoupon = 0;
	   fTenderShowUseCoupons = false;	// used to determine if the button USECOUPONS is visible at Finish&Pay
	}


    // get unique timeout values (in seconds)
	CheckRange(nTimeOutSyncTimeout,0,180000);
    CheckRange(nTimeOutTender,0,180);
    CheckRange(nTimeOutStateDefault,0,180);
    CheckRange(nTimeOutTakeCard,0,180);
    CheckRange(nTimeOutTakeCash,0,180);
    CheckRange(nTimeOutTakeCoins,0,180);
    CheckRange(nTimeOutItemTransactionResult,0,180);
    CheckRange(nTimeOutContinueTransaction,0,180);
    CheckRange(nTimeOutHelp,0,180);
    CheckRange(nTimeOutSignon,0,180);
    CheckRange(nTimeOutSignoff,0,180);
    CheckRange(nTimeOutAttract,0,180);
    CheckRange(nTimeOutItemSaleInProgress,0,180);

    CheckRange(nTimeOutEASDeactivationScreenTimer,0,180);
	CheckRange(nTimeOutCouponSensorScreenTimer,0,180);
	CheckRange(nTimeOutDepositGiftCardScreenTimer,0,180);
	CheckRange(nTimeOutTenderBalanceScreenTimer,0,180);
    CheckRange(nTimeOutCashOutSensorAudioTimer,0,180);
	CheckRange(nTimeOutCheckBasketReminder,0,180);
	CheckRange(nTimeOutRequestSig,0,180);
	CheckRange(nTimeOutInitialBagItemAudio,0,180);
	CheckRange(nTimeOutPeriodicBagItemAudio,0,180);
	CheckRange(nTimeOutBagItemEscalationMilliSecs,0,180000);         // checking against milliseconds
	CheckRange(nTimeOutTakeYourItemEscalationMilliSecs,0,180000);  //RFC209702
	CheckRange(nTimeOutInitialRemovingItemsAudio,0,180);
	CheckRange(nTimeOutPeriodicRemovingItemsAudio,0,180);
	CheckRange(nTimeOutRemovingItemsEscalationMilliSecs,0,180000);   // checking against milliseconds
	CheckRange(nTimeOutWalkAwayTimerAudio,0,180);
	CheckRange(nTimeOutInitialTakeYourItemAudio,0,180);
	CheckRange(nTimeOutRecurringTakeYourItemAudio, 0, 180);   //RFC209702
	CheckRange(nTimeOutSecurityServerTimer,0,180);
	CheckRange(nTimeOutThankYouTimerBelt,0,180);
	CheckRange(nTimeOutInitialClearBagAreaAudio,0,180);
	CheckRange(nTimeOutPeriodicClearBagAreaAudio,0,180);
	CheckRange(nTimeOutTimeOutTransportItem,0,180);
	CheckRange(nTimeOutTouchScreenProximityEnablesScannerMotor,0,120);
	CheckRange(nTimeOutMotionSensorOnAttractInterval,0,120);  //RFC 265465
	CheckRange(nTimeOutScannerScaleOnAttractInterval,0,6000); //RFC 276235
	CheckRange(nTimeOutDisplayBagItemScreenTimer,0,180);
    CheckRange(nTimeOutDelaySecurityNotificationMilliSecs,0,180000);
	CheckRange(nTimeOutPeriodicSecurityAudio,0,180);
	CheckRange(nTimeOutAttendantModeTimer,0, 32767);
	CheckRange(nTimeOutExitSuspendScreenTimer,0,180);		//RFC 294962
	CheckRange(nTimeOutDisplayIncreaseMisMatchScreenTimer,0,180); //TAR379130/379131
	CheckRange(nTimeOutEnableSkipBaggingButtonTimer,0,180000);//SR778
    
	// Check range of number of report logs to keep. (TAR 157528)
    CTime ct = CTime::GetCurrentTime();
    COleDateTime t(ct.GetYear(), 12, 31, 1, 12, 0);
    int daysOfYear = t.GetDayOfYear();
	CheckRange(nOperationsNumberOfReportLogsToKeep,8,daysOfYear);

   CheckRange(nOperationsRemoteAssistModeToggleDisplay,0,3);
  
    trace(L0,_T("Final Timeout Syncronize Time - amount time to wait for the SyncTime batch file to run: %d"), nTimeOutSyncTimeout);  
    trace(L0,_T("Final TimeOut Tender %d"), nTimeOutTender);
    trace(L0,_T("Final TimeOut StateDefault %d"), nTimeOutStateDefault);
    trace(L0,_T("Final TimeOut TakeCard %d"), nTimeOutTakeCard);
    trace(L0,_T("Final TimeOut TakeCash %d"), nTimeOutTakeCash);
    trace(L0,_T("Final TimeOut TakeCoins %d"), nTimeOutTakeCoins);
    trace(L0,_T("Final TimeOut ItemTransactionResult %d"), nTimeOutItemTransactionResult);
    trace(L0,_T("Final TimeOut ContinueTransaction %d"), nTimeOutContinueTransaction);
    trace(L0,_T("Final TimeOut Help %d"), nTimeOutHelp);
    trace(L0,_T("Final TimeOut Signon %d"), nTimeOutSignon);
    trace(L0,_T("Final TimeOut Signoff %d"), nTimeOutSignoff);
    trace(L0,_T("Final TimeOut Attract %d"), nTimeOutAttract);
    trace(L0,_T("Final TimeOut Item sale in progress %d"), nTimeOutItemSaleInProgress);
    trace(L0,_T("Final TimeOut EAS Deactivation Audio %d"), nTimeOutEASDeactivationScreenTimer);
	trace(L0,_T("Final TimeOut Coupon Sensor Audio %d"), nTimeOutCouponSensorScreenTimer);
	trace(L0,_T("Final TimeOut Tender Balance Audio %d"), nTimeOutTenderBalanceScreenTimer);
	trace(L0,_T("Final TimeOut Deposit Gift Card Audio %d"), nTimeOutDepositGiftCardScreenTimer);
    trace(L0,_T("Final TimeOut Cash Out Sensor Audio %d"), nTimeOutCashOutSensorAudioTimer);
	trace(L0,_T("Final TimeOut Check Basket Reminder %d"), nTimeOutCheckBasketReminder);
	trace(L0,_T("Final TimeOut Request Sig %d"), nTimeOutRequestSig);
	trace(L0,_T("Final TimeOut Initial Bag Item Audio %d"), nTimeOutInitialBagItemAudio);
	trace(L0,_T("Final TimeOut Periodic Bag Item Audio %d"), nTimeOutPeriodicBagItemAudio);
	trace(L0,_T("Final TimeOut Bag Item RAP Escalation %d"), nTimeOutBagItemEscalationMilliSecs);
	trace(L0,_T("Final TimeOut Take Your Item RAP Escalation %d"), nTimeOutTakeYourItemEscalationMilliSecs); //RFC209702
	trace(L0,_T("Final TimeOut Initial Removing Items Audio %d"), nTimeOutInitialRemovingItemsAudio);
	trace(L0,_T("Final TimeOut Periodic Removing Items Audio %d"), nTimeOutPeriodicRemovingItemsAudio);
	trace(L0,_T("Final TimeOut Removing Items RAP Escalation %d"), nTimeOutRemovingItemsEscalationMilliSecs);
	trace(L0,_T("Final TimeOut Walk Away Timer Audio %d"), nTimeOutWalkAwayTimerAudio);
	trace(L0,_T("Final TimeOut Initial Take Your Item Audio %d"), nTimeOutInitialTakeYourItemAudio);
	trace(L0,_T("Final TimeOut Take Your Item Recurring Audio %d"), nTimeOutRecurringTakeYourItemAudio);  //RFC209702
	trace(L0,_T("Final TimeOut Security Server Timer %d"),nTimeOutSecurityServerTimer);
	trace(L0,_T("Final TimeOut Thank You Timer Belt %d"), nTimeOutThankYouTimerBelt);
	trace(L0,_T("Final TimeOut Initial Clear Bag Area Audio %d"), nTimeOutInitialClearBagAreaAudio);
	trace(L0,_T("Final TimeOut Periodic Clear Bag Area Audio %d"), nTimeOutPeriodicClearBagAreaAudio);
	trace(L0,_T("Final TimeOut SAInProgress %d"), nTimeOutTimeOutTransportItem);
	trace(L0,_T("Final TimeOut TouchScreen Proximity Enables Scanner Motor %d"), nTimeOutTouchScreenProximityEnablesScannerMotor);
	trace(L0,_T("Final TimeOut Motion Sensor Enables Attract Interval %d"), nTimeOutMotionSensorOnAttractInterval);//RFC 265465
	trace(L0,_T("Final Timeout Scanner Scale Enables Attract Interval %d"), nTimeOutScannerScaleOnAttractInterval);//RFC276235
	trace(L0,_T("Final TimeOut Display Bag Item Screen %d"), nTimeOutDisplayBagItemScreenTimer);
    trace(L0,_T("Final TimeOut Delay Security Notification %d"), nTimeOutDelaySecurityNotificationMilliSecs);
	trace(L0,_T("Final TimeOut Security Audio Timer %d"), nTimeOutPeriodicSecurityAudio);
	trace(L0,_T("Final TimeOut Attendant Mode Timer %d"), nTimeOutAttendantModeTimer);
	trace(L0,_T("Final TimeOut GloryDispenseRetries %d"), nOperationsGloryDispenseRetries);	// TAR 310660
	trace(L0,_T("Final TimeOut Exit Suspend Screen %d"), (nTimeOutExitSuspendScreenTimer));		//RFC 294962
	trace(L0,_T("Final TimeOut Display PutBagOnScale Screen %d"), nTimeOutDisplayPutBagOnScaleScreenTimer);//SR085
	trace(L0,_T("Final TimeOut Enable Skip Bagging Button timer %d"), nTimeOutEnableSkipBaggingButtonTimer); //SR778

    // set the required bit flags in the tracing mask for focused comp ids
    if (csTracingFocusComps.Find(_T("MP"))>=0) to.lFocusComps |= ID_MP;
    if (csTracingFocusComps.Find(_T("SM"))>=0) to.lFocusComps |= ID_SM;
    if (csTracingFocusComps.Find(_T("HC"))>=0) to.lFocusComps |= ID_HC;
    if (csTracingFocusComps.Find(_T("PS"))>=0) to.lFocusComps |= ID_PS;
    if (csTracingFocusComps.Find(_T("DM"))>=0) to.lFocusComps |= ID_DM;
    if (csTracingFocusComps.Find(_T("SA"))>=0) to.lFocusComps |= ID_SA;
    if (csTracingFocusComps.Find(_T("TB"))>=0) to.lFocusComps |= ID_TB;
    if (csTracingFocusComps.Find(_T("EO"))>=0) to.lFocusComps |= ID_EO;
    if (csTracingFocusComps.Find(_T("TO"))>=0) to.lFocusComps |= ID_TO;
    if (csTracingFocusComps.Find(_T("MO"))>=0) to.lFocusComps |= ID_MO;
    if (csTracingFocusComps.Find(_T("CO"))>=0) to.lFocusComps |= ID_CO;
    if (csTracingFocusComps.Find(_T("UE"))>=0) to.lFocusComps |= ID_UE;
    if (csTracingFocusComps.Find(_T("TI"))>=0) to.lFocusComps |= ID_TI;
    if (csTracingFocusComps.Find(_T("RP"))>=0) to.lFocusComps |= ID_RP;

    trace(L0,_T("Final Trace level = %d, Focus level = %d, Focus IDs = 0x0%x"),
	           to.nCurrentLevel,to.nFocusLevel,to.lFocusComps);

    trace(L0,_T("Final Metric=%d"), fOperationsScaleMetric);

    //  trace(L0,"Final Restricted AAge=%d, TAge=%d",
    //	  nOperationsAlcoholAge,
    //	  nOperationsTobaccoAge);
	trace(L0, _T("Coupon Max Limit %d"), nOperationsMaxCouponLimit);	//SR678
	trace(L0, _T("Duplicate Coupon Max %d"), nOperationsDuplicateCouponMax);
	trace(L0, _T("Total Coupons $ Amt Max %d"), nOperationsTotalCoupons$AmtMax);
	trace(L0, _T("Total Coupons $ Amt ForPercentToApply %d"), nOperationsUpper$AmtForPercentToApply);
	trace(L0, _T("Coupon Percent of Gross Sales %d"), nOperationsCouponPercentofGrossSales);
	trace(L0, _T("Allow Skip Bagging Btn? %d"), fOperationsAllowSkipBaggingBtn);

	//BZ 06/07/02 RFC 203538 begin
	trace(L0, _T("Allow Store Login Btn on BagItem screen? %d"), fOperationsAllowStoreLoginBtnOnBagItemScreen);
	trace(L0, _T("Allow Store Login Btn on PickupItem Screen? %d"), fOperationsAllowStoreLoginBtnOnRemoveItemScreen);
	//RFC 203538 end
    trace(L0, _T("The Operator barcode type is: %s") , csOperationsOperatorBarcodeMask); //RFC 187993
	trace(L0, _T("The Operator Login Barcode Type (0=None, 1=EAN13, 2=Code128) is: %d") , nOperationsOperatorLoginBarcodeType);
	trace(L0, _T("AllowLeadingZerosInCode128LoginBarCode is: %d"), fOperationsAllowLeadingZerosInCode128LoginBarCode);//rfc 240738
    
	trace(L0, _T("Show item removed Btn on Item Remove Violation screen? %d"), fOperationsShowItemRemovedButtonOnRemoveItemViolationScreen); //CHU 07/25/02 RFC 208874
	trace(L0, _T("Allow Container Btn? %d"), fOperationsAllowContainerBtn);
	trace(L0, _T("Allow Container Btn For Mismatch Weight? %d"), fOperationsAllowContainerBtnForMismatching); //Tar 266250
	trace(L0, _T("Allow Container Btn after Finish and Pay? %d"), fOperationsAllowContainerBtnInFinalization);//SR085
	trace(L0, _T("Allow Item Removed Btn? %d"), fOperationsAllowItemRemovedBtn);
	trace(L0, _T("Allow Assistance Needed Done Btn? %d"), fOperationsAllowAMDoneBtn);	// RFC 209075 RF 073102
	trace(L0, _T("Show Weight Info On Lane? %d"), fOperationsShowWeightInfoOnLane);// SR682
	trace(L0, _T("Suppress Audio in Item Remove Violation %d"), fOperationsSuppressAudioInItemRemoveViolation);
	trace(L0, _T("Suppress Audio in Unexpected Increase %d"), fOperationsSuppressAudioInUnexpectedIncrease);
	trace(L0, _T("Suppress Audio in Mismatch %d"), fOperationsSuppressAudioInMismatch);
	
	trace(L0, _T("Show Skip Bagging on Electronic Receipt %d"), fOperationsShowSkipBaggingOnEReceipt); // RFC 252473
	trace(L0, _T("Weighted Item need tare? %d"), fOperationsSelectTareOnWeight);
	trace(L0, _T("Jump To Tare Pick List? %d"), fOperationsJumpToTarePickList);
	trace(L0, _T("Allow Multi-Selection Picklist? %d"), fOperationsAllowMultiPick);
	trace(L0, _T("Display Pick List Item Code? %d"), fOperationsDisplayPickListItemCode);
	trace(L0, _T("Display Quick Pick Items? %d"), fOperationsDisplayQuickPickItems);
	trace(L0, _T("Suspend transaction? %d"), fOperationsSuspend);
    trace(L0, _T("AllowSuspendIfPartiallyTendered? %d"), fOperationsAllowSuspendIfPartiallyTendered); //RFC 267820
    trace(L0, _T("IfSuspendedDispensePartiallyPaidCash? %d"), fOperationsIfSuspendedDispensePartiallyPaidCash); //RFC 267820
	trace(L0, _T("Print Outstanding Approvals at bottom of receipt? %d"), fOperationsPrintPendingApprovalOnSuspend);
// fiscal printer start
	trace(L0, _T("Fiscal Printer configured? %d"), fOperationsTBControlsPrinter);
// fiscal printer end
   trace(L0, _T("Remote Assist Mode Toggle Display Option %d"), nOperationsRemoteAssistModeToggleDisplay);
    trace(L0, _T("Gift Card Tender Option %d"), nOperationsGiftCardRedemption);
	trace(L0, _T("Method of Gift Card Collection %d"), nOperationsMethodOfCollectionForEmptyGiftCard);
	trace(L0, _T("Display Gift Card Option %d"), fOperationsDisplayGiftCardBalance);
	trace(L0, _T("Currency bitmap will be displayed for: %s"), csCurrencyBitmap);
        trace(L0, _T("Disable base price embedded definitions? %d"), fOperationsDisableBasePriceEmbeddedDefinitions);//RFC239992
	trace(L0, _T("Truncate Barcode Supplemental codes? %d"), fOperationsTruncateSupplementalCodeForSecurity);
	trace(L0, _T("Void coupon before suspend transaction? %d"), fOperationsVoidCouponBeforeSuspend);
	trace(L0, _T("Auto void underage resricted item? %d"),fOperationsAutoVoidUnderageRestrict);   //Feng122299
	trace(L0, _T("Send Signature Data to TB? %d"),fOperationsSendSignatureDataToTB);
	trace(L0, _T("Price Embedded Verifier for UPC A2/CD format = 02 iiii V ppppp C? %d"),fOperationsUPCAPriceEmbeddedVerifier);
    trace(L0, _T("EOD option allows BusinessDate to be Configured: %d"),nOperationsEOD);
    trace(L0, _T("EOD Timeout value is the amount time to wait for the EOD batch file to run: %d"),nOperationsEODTimeout);

	//USSF START
    trace(L0, _T("Enable USSF: %d"), fOperationsEnableUSSF);
	//USSF END
	
	// SR730 RFC380486
	trace(L0, _T("List of contexts which will transition to Scan and Bag before showing Bag Item: %s"), csOperationsTransitionToScanAndBagContexts);

	// Partial Integrated PINPad
	trace(L0, _T("Use partial integrated PINPad? %d"),fOperationsPartialIntegratedPINPad);
	trace(L0, _T("PIP SCOT prompt for card type? %d"),fOperationsPIPSCOTPromptCardType);
	trace(L0, _T("PIP SCOT prompt for cash back? %d"),fOperationsPIPSCOTPromptCashBack);
	trace(L0, _T("PIP SCOT prompt for confirming total? %d"),fOperationsPIPSCOTPromptConfirmTotal);

	// PIP with Integrated MSR options
	trace(L0,_T("Use Integrated MSR for Credit? %d"),fOperationsCreditMSR);
	trace(L0,_T("Use Integrated MSR for Debit? %d"),fOperationsDebitMSR);
	trace(L0,_T("Use Integrated MSR for Debit Savings? %d"),fOperationsDebitSavingsMSR);
	trace(L0,_T("Use Integrated MSR for Debit Checking? %d"),fOperationsDebitCheckingMSR);
	trace(L0,_T("Use Integrated MSR for EBT? %d"),fOperationsEBTMSR);
	trace(L0,_T("Use Integrated MSR for Food Stamps? %d"),fOperationsFoodStampsMSR);
	trace(L0,_T("Use Integrated MSR for Gift Card? %d"),fOperationsGiftCardMSR);
	trace(L0,_T("Use Integrated MSR for Phone Card? %d"),fOperationsPhoneCardMSR);
	trace(L0,_T("Use Integrated MSR for Loyalty Card? %d"),fOperationsLoyaltyMSR);

	trace(L0,_T("GloryUnitDispenseTimeout %d"), nTimeOutGloryUnitDispenseTimeout); // TAR 310660
	//Integrated MSR device info
	trace(L0,_T("Is Integrated MSR 7401? %d"),fOperationsUse7401MSR);

	trace(L0,_T("Final Tare settings Container1=%d, Container2=%d"),
               nOperationsTareContainer1,
	           nOperationsTareContainer2);
	trace(L0, _T("Number of In-Store Barcodes Loaded=%d"), cstoreCode.NumberOfInstoreCodesLoaded());	//In-Store barcodes
	trace(L0, _T("Number of Multi-Label Barcodes Loaded=%d"), g_MultiLabelBarcodes.GetNumberOfMultiLabelBarcodes());	
	trace(L0,_T("Final SayPrices = %d,  SayPhrases = %d,  AllowTBAudioControl = %d, SayPricesInAttendantMode = %d"), fStateSayPrices,fStateSayPhrases,fStateAllowTBAudioControl, !fStateSayPricesDisableInAttendantMode );
    trace(L0,_T("Final TrainingProgramButtonVisible = %d"), fStateTrainingProgramButtonVisible);
	trace(L0,_T("Final fStateLiveVideo = %d"),fStateLiveVideo);
	trace(L0,_T("Final LiveVideo for Attract= %d"),fStateWelcomeScreenVideo);
	trace(L0,_T("Final LiveVideo for Select From List= %d"),fStateSelectListScreenVideo);
	trace(L0,_T("Common Display Weights and Measures = %d"),fStateDisplayWeightsAndMeasures);
	trace(L0,_T("Large Font on Current Item = %d"),fStateLargeFontOnCurrentItem);
	trace(L0,_T("LogDataForSystemReports = %d"),fOperationsLogDataForSystemReports);
	trace(L0,_T("NumberOfReportLogsToKeep = %d"),nOperationsNumberOfReportLogsToKeep);
	trace(L0,_T("CustomerBagAllowed = %d"),fOperationsCustomerBagAllowed);//SR085
	trace(L0,_T("IncludeCouponsInItemCount = %d"),fOperationsIncludeCouponsInItemCount);//RFC 371075

	trace(L0,_T("Exits Enabled = %d"),fUserExitAllEnabled);

	if (fUserExitAllEnabled)
	{
		trace(L0,_T("00  Exit  Enabled = %d"),fUserExit00Enabled);
		trace(L0,_T("01  Exit  Enabled = %d"),fUserExit01Enabled);
		trace(L0,_T("02  Exit  Enabled = %d"),fUserExit02Enabled);
		trace(L0,_T("03  Exit  Enabled = %d"),fUserExit03Enabled);
		trace(L0,_T("04  Exit  Enabled = %d"),fUserExit04Enabled);
		trace(L0,_T("05  Exit  Enabled = %d"),fUserExit05Enabled);
		trace(L0,_T("06  Exit  Enabled = %d"),fUserExit06Enabled);
		trace(L0,_T("07  Exit  Enabled = %d"),fUserExit07Enabled);
		trace(L0,_T("08  Exit  Enabled = %d"),fUserExit08Enabled);
		trace(L0,_T("09  Exit  Enabled = %d"),fUserExit09Enabled);
		trace(L0,_T("10  Exit  Enabled = %d"),fUserExit10Enabled);
		trace(L0,_T("11  Exit  Enabled = %d"),fUserExit11Enabled);
		trace(L0,_T("12  Exit  Enabled = %d"),fUserExit12Enabled);
		trace(L0,_T("13  Exit  Enabled = %d"),fUserExit13Enabled);
		trace(L0,_T("14  Exit  Enabled = %d"),fUserExit14Enabled);
		trace(L0,_T("15  Exit  Enabled = %d"),fUserExit15Enabled);
		trace(L0,_T("16  Exit  Enabled = %d"),fUserExit16Enabled);
		trace(L0,_T("99  Exit  Enabled = %d"),fUserExit99Enabled);
		trace(L0,_T("User Int 1 value = %d"),nUserExitInt1);
		trace(L0,_T("User Int 2 value = %d"),nUserExitInt2);
		trace(L0,_T("User Int 3 value = %d"),nUserExitInt3);
		trace(L0,_T("User Int 4 value = %d"),nUserExitInt4);
		trace(L0,_T("User Int 5 value = %d"),nUserExitInt5);
		trace(L0,_T("User Str 1 value = %s"),csUserExitString1);
		trace(L0,_T("User Str 2 value = %s"),csUserExitString2);
		trace(L0,_T("User Str 3 value = %s"),csUserExitString3);
		trace(L0,_T("User Str 4 value = %s"),csUserExitString4);
		trace(L0,_T("User Str 5 value = %s"),csUserExitString5);
	}

	InitializePrintObjects(PrintBmpObjectList);	// preload receipt bitmaps
	// Get data file names
	trace(L0,_T("Message file = %s"), csDataMessageFile);
	trace(L0,_T("Bitmap file = %s"), csDataBitmapFile);
	trace(L0,_T("SASIActionFile = %s"), csDataSASIActionFile);
	trace(L0,_T("Wave file = %s"), csDataWaveFile);
	trace(L0, _T("Tender file = %s"), csDataTenderFile);
	trace(L0, _T("Tare file = %s"), csDataTareFile);
	trace(L0, _T("XML file = %s"), csDataXMLFile);
	trace(L0,_T("LiveVideo= %d"), fStateLiveVideo);
	trace(L0,_T("WelcomeScreenVideo %d"), fStateWelcomeScreenVideo);
	trace(L0,_T("SelectListScreenVideo %d"), fStateSelectListScreenVideo);
	trace(L0,_T("Display Weights and Measures = %d"),fStateDisplayWeightsAndMeasures);
	trace(L0,_T("LargeFontOnCurrentItem = %d"), fStateLargeFontOnCurrentItem);
	trace(L0,_T("RapInitiateTransaction = %s"), csStateRapInitiateTransaction);

    // TAR 211420
	trace(L0,_T("Keyboard     = %s"), csTemplateKeyboard);     // Keyboard template (for pick up list)
	trace(L0,_T("AssistMode   = %s"), csTemplateAssistMode);   // Assist Mode template
	trace(L0,_T("AlphaNumeric = %s"), csTemplateAlphaNumeric); // AlphaNumeric keyboard for job info.

	trace(L0, _T("PasswordEntryKeypad = %d"), nTemplatePasswordEntryKeypad); // FZ 10102002, Alphanumberic Password
	trace(L0, _T("UserIDEntryKeypad = %d"), nTemplateUserIDEntryKeypad); // FZ 10102002, Alphanumberic Password
	// TAR 134992 -- when the terminal option file exists, tracing options are not honored
	CheckRange(nTracingCurrentLevel,0,9);
	CheckRange(nTracingFocusLevel,0,9);
	CheckRange(nTracingMaxLogSize,0,10000);      // not exceed 10MB
	CheckRange(nTracingEntriesToFlush,1,10000);  // 10000 entries
	CheckRange(nTracingPSAPIInterval,1,(3600 * 24));


	trace(L0,_T("ExitButtonVisible = %d"), fUtilityExitButtonVisible); 
	trace(L0,_T("ExitPassword = %s"), csUtilityExitPassword); 
	trace(L0,_T("ExitID = %s"), csUtilityExitID); 
	trace(L0,_T("DiagID = %s"), csUtilityDiagID); 
	trace(L0,_T("DiagPassword = %s"), csUtilityDiagPassword); 
	trace(L0,_T("RebootID = %s"), csUtilityRebootID); 
	trace(L0,_T("RebootPassword = %s"), csUtilityRebootPassword); 
	trace(L0,_T("ShutdownID = %s"), csUtilityShutdownID); 
	trace(L0,_T("ShutdownPassword = %s"), csUtilityShutdownPassword); 
	trace(L0,_T("VolumeID = %s"), csUtilityVolumeID); 
	trace(L0,_T("VolumePassword = %s"), csUtilityVolumePassword); 
    trace(L0,_T("TrainingDocument = %s"), csUtilityTrainingDocument);
    trace(L0,_T("InternetExplorerEXE = %s"), csUtilityInternetExplorerEXE);

	trace(L0,_T(" Final TraceOn = %d, Trace level = %d, Focus level = %d, Focus IDs = %s, Max Log Size = %d KB"),
	         fTracingTraceOn,
	         nTracingCurrentLevel,
	         nTracingFocusLevel,
	         csTracingFocusComps,
	         nTracingMaxLogSize);

	to.nCurrentLevel = nTracingCurrentLevel;
	to.nFocusLevel = nTracingFocusLevel;
	to.m_nMaxLogSize = nTracingMaxLogSize;
	to.m_nEntriesToFlush = nTracingEntriesToFlush ;
	to.m_bTraceOn = fTracingTraceOn ;
	// Do we want to overwrite the default Regional Setting ?
	if (!fLocaleCurrencySettingNotFromRegionalSetting)
	{
		// Get the current regional setting from "Control Panel"
		#define StringSize 20			// Max size for a price

		LCID Locale;																		// Specifies the locale for which the currency string is to be formatted
		Locale=LOCALE_USER_DEFAULT;
		CString lpLCData;
		int cchData=StringSize;
		LCTYPE LCType;

		LCType = LOCALE_ICURRDIGITS;							// The number of decimal digits
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		nLocaleNoOfDigitsAfterDecimal = _ttoi (lpLCData);
		lpLCData.ReleaseBuffer(0);

		LCType = LOCALE_ILZERO;									// Specifier for leading zeros in decimal fields
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		nLocaleLeadingZero	= _ttoi (lpLCData);
		lpLCData.ReleaseBuffer(0);

		LCType = LOCALE_SMONGROUPING;							// Size of each group of digits to the left of the decimal
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		nLocaleNoOfDigitsInGroup = _ttoi(lpLCData);
		lpLCData.ReleaseBuffer(0);

		LCType = LOCALE_SMONDECIMALSEP;							// Character(s) used as the decimal separator
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		csLocaleDecimalSymbol = lpLCData.GetBuffer(cchData);
		lpLCData.ReleaseBuffer(0);

		LCType = LOCALE_SMONTHOUSANDSEP;						// Character(s) used as the thousand separator

		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		csLocaleDigitGroupingSymbol = lpLCData.GetBuffer(cchData);
		lpLCData.ReleaseBuffer(0);

		LCType = LOCALE_ICURRENCY;								// Positive Currency Mode
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		nLocalePositiveCurrencyFormat = _ttoi (lpLCData);
		lpLCData.ReleaseBuffer(0);

		LCType = LOCALE_INEGCURR;								// Negative Currency Mode
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		nLocaleNegativeCurrencyFormat = _ttoi (lpLCData);
		lpLCData.ReleaseBuffer(0);

		LCType = LOCALE_SCURRENCY;								// String used as the local monetary symbol
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		csLocaleCurrencySymbol = lpLCData.GetBuffer(cchData);
		lpLCData.ReleaseBuffer(0);

		// Others
		LCType = LOCALE_SINTLSYMBOL;							// String used as the int monetary symbol
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		csLocaleSIntlSymbol = lpLCData.GetBuffer(cchData);
		lpLCData.ReleaseBuffer(0);
	}

	//Feng 991228 nLocalePriceCentUnit = (long) pow ((double) 10,(double) nLocaleNoOfDigitsAfterDecimal);
	nLocalePriceCentUnit = 1;
	int nTemp =nLocaleNoOfDigitsAfterDecimal;
	while(nTemp)
	{
		nLocalePriceCentUnit*=10;
		nTemp--;
	}

	// Do we want to overwrite the default Regional Setting ?
	if (!fLocaleDateSettingNotFromRegionalSetting)
	{
		#define StringSize 20
		CString lpLCData;
		LCTYPE LCType;
		LCID Locale;																		// Specifies the locale for which the currency string is to be formatted
		int cchData=StringSize;

		Locale=LOCALE_USER_DEFAULT;
		// Read Date Short Format
		LCType = LOCALE_IDATE;								    // Short date format-ordering specifier.
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		nLocaleShortDateOrdering = _ttoi(lpLCData);
		lpLCData.ReleaseBuffer(0);

		// Read Century
		LCType = LOCALE_ICENTURY;								// Specifier for full 4-digit century.
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		nLocaleNoCenturyDigit = _ttoi(lpLCData);
		lpLCData.ReleaseBuffer(0);

		// Read Character Separator
		LCType = LOCALE_SDATE;								    // Character(s) for the date separator.
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		csLocaleDateSeparator = lpLCData.GetBuffer(cchData);
		lpLCData.ReleaseBuffer(0);

		// Read Short date formatting	 (not used)
		LCType = LOCALE_SSHORTDATE;								// Character(s) for the date separator.
		GetLocaleInfo(	Locale,									// locale identifier
						LCType,									// type of information
						lpLCData.GetBuffer(cchData),			// address of buffer for information
						cchData);								// size of buffer
		csLocaleShortDateFormat = lpLCData.GetBuffer(cchData);
		lpLCData.ReleaseBuffer(0);
	}

	{   // ***BEGIN*** unconditional block

		// validate the csTenderDenominations e.g. 1,2,5,10,20,50,100
		// max of 7 denominations supported.
		long d[7] = {0};  // init to all zeros
		// Get the values from the string as numbers.
		int i = _stscanf(csTenderDenominations,
				   _T("%d,%d,%d,%d,%d,%d,%d"),
				   &d[0],&d[1],&d[2],&d[3],&d[4],&d[5],&d[6]);

		// Find how many commas are in the list.  A valid list separated by
		// commas contains one less comma than numbers.
		int commas(0), numberDenominations(0);
		for (i=0;i<csTenderDenominations.GetLength();i++)
		{
		  if (csTenderDenominations[i] == _T(','))
			commas++;
		}
		numberDenominations = commas + 1;

		// If too many, log error.
		if (numberDenominations > 7)
		{
		  ScotError(RETURN,SM_CAT_INPUT_PARAMETER, SCOTAPP_TOO_MANY_TENDER_TYPE,_T("%s"),
				csTenderDenominations);
		}

		for (i=0;i<numberDenominations;i++)
		{
		  // BeginCodeLocale
		  // This list represents the only valid denomination we support.
		  //if ( d[i] == 1						  // ones
		  //  || d[i] == 2						  // twos
		  //  || d[i] == 5						  // fives
		  //  || d[i] == 10						  // tens
		  //  || d[i] == 20						  // twenties
		  //  || d[i] == 50						  // fifties
		  //  || d[i] == 100 )				  // hundreds
		  // d[i] *= 100; // convert dollars to cents


		  // In case of possible negative value for coins
		  // we test if d[i] is a positive value

		  if (d[i] > 0)
		  {
			d[i] *= nLocalePriceCentUnit; // convert dollars to cents
		  }

		}

		CString workString(_T(";"));
		// char denomination[6]; // max chars in a denomination
		_TCHAR denomination[10]; // max chars in a denomination
		//  Convert all non-zero values to ASCII and concatenate to string
		for (i=0; i<7; i++)
		{
		  if (d[i] > 0)
		  {
			_ltot(d[i],denomination,10);
			workString += denomination;
			workString += _T(",");
		  }
		}
		// Trim last comma we added
		csTenderDenominations = workString.GetBufferSetLength(workString.GetLength()-1);

		trace(L0,_T("Tender Denominations <%s>"),csTenderDenominations);


	} // ***END of unconditional block***


    // TAR #200002 HC 032602 Start
    // For testing/debugging, get registry values to see if a pop up message
    // is shown in SMStateTBBase.cpp's ProcessTBReturn method if return code
    // from TB is not TB_SUCCESS or TB_SUCCESS_IMMEDIATE
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                           _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\TBReturnCodes"),
                           0, KEY_ALL_ACCESS, &hKey);
	if (lResult == ERROR_SUCCESS)
	{
		lResult = RegQueryValueEx(hKey, _T("AMasterKey_ShowMsgOnNonSuccess"), 0,
                                  &dwType, (BYTE *)&lValue, &dwSize);
		if ((lResult == ERROR_SUCCESS) && (lValue == 1))
		{
			// The optional registry value is set to 1, so show pop up
			// message to alert user about error return code from TB
            // if boolean value for TB error is also true
            bShowTBErrors = true;

			// Continue checking the registry values for the individual
            // TB errors only if bShowTBErrors is true, if bShowTBErrors
            // is false, the following values will not be used anyways
		    lResult = RegQueryValueEx(hKey, _T("InvalidUserID"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBInvalidUserID = true;
            else
                bShowTBInvalidUserID = false;

		    lResult = RegQueryValueEx(hKey, _T("InvalidUserPW"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBInvalidUserPW = true;
            else
                bShowTBInvalidUserPW = false;

            lResult = RegQueryValueEx(hKey, _T("InvalidState"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBInvalidState = true;
            else
                bShowTBInvalidState = false;

            lResult = RegQueryValueEx(hKey, _T("FileNotFound"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBFileNotFound = true;
            else
                bShowTBFileNotFound = false;

            lResult = RegQueryValueEx(hKey, _T("IncompleteProcedure"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBIncompleteProcedure = true;
            else
                bShowTBIncompleteProcedure = false;

            lResult = RegQueryValueEx(hKey, _T("InvalidWindow"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBInvalidWindow = true;
            else
                bShowTBInvalidWindow = false;

            lResult = RegQueryValueEx(hKey, _T("DiskFull"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBDiskFull = true;
            else
                bShowTBDiskFull = false;

            lResult = RegQueryValueEx(hKey, _T("InvalidParam"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBInvalidParam = true;
            else
                bShowTBInvalidParam = false;

            lResult = RegQueryValueEx(hKey, _T("NoHostCredit"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBNoHostCredit = true;
            else
                bShowTBNoHostCredit = false;

            lResult = RegQueryValueEx(hKey, _T("NoWorkingKey"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBNoWorkingKey = true;
            else
                bShowTBNoWorkingKey = false;

            lResult = RegQueryValueEx(hKey, _T("NotInitialized"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBNotInitialized = true;
            else
                bShowTBNotInitialized = false;

            lResult = RegQueryValueEx(hKey, _T("NoHost"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBNoHost = true;
            else
                bShowTBNoHost = false;

            lResult = RegQueryValueEx(hKey, _T("FunctionNotSupported"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBFunctionNotSupported = true;
            else
                bShowTBFunctionNotSupported = false;

            lResult = RegQueryValueEx(hKey, _T("Unexpected"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBUnexpected = true;
            else
                bShowTBUnexpected = false;

            lResult = RegQueryValueEx(hKey, _T("ItemFailed"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBItemFailed = true;
            else
                bShowTBItemFailed = false;

            lResult = RegQueryValueEx(hKey, _T("CardDataMissing"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBCardDataMissing = true;
            else
                bShowTBCardDataMissing = false;


            lResult = RegQueryValueEx(hKey, _T("Other"), 0,
                                      &dwType, (BYTE *)&lValue, &dwSize);
		    if ((lResult == ERROR_SUCCESS) && (lValue == 1))
                bShowTBOther = true;
            else
                bShowTBOther = false;
        }
        else
        {
            bShowTBErrors = false;
        }
	}
    else
    {
        bShowTBErrors = false;
    }
	RegCloseKey(hKey);
	// TAR #200002 HC 032602 End

   trace(L0,_T("CM, RewardInfo        = %d"), fCMRewardInfo);
	trace(L0,_T("CM, CustomerMessage   = %s"), csCMCustomerMessage);
	trace(L0,_T("CM, TotalSavings      = %d"), fCMTotalSavings);
	trace(L0,_T("CM, TotalRewardPoints = %d"), fCMTotalRewardPoints);
	trace(L0,_T("CM, TotalSavingsOnLeft= %d"), fCMTotalSavingsOnLeft);	

    trace(L0,_T("MultiLabelBarCodes, PrependFirstPrefixToItemCode=%d"),fMultiLabelBarCodesPrependFirstPrefixToItemCode); //TAR 266075

    trace(L0,_T("AutoRun, On=%d"),fAutoRunOn);
    trace(L0,_T("AutoRun, BypassBagging=%d"),fAutoRunBypassBagging);
    trace(L0,_T("AutoRun, NumberOfTransactionsToRun=%d"),nAutoRunNumberOfTransactionsToRun);
	trace(L0,_T("AutoRun, RandomTimer=%d"),fAutoRunRandomTimer);
    trace(L0,_T("AutoRun, AttractDelay=%d"),nAutoRunAttractDelay);
    trace(L0,_T("AutoRun, ItemPurchaseDelay=%d"),nAutoRunItemPurchaseDelay);
    trace(L0,_T("AutoRun, NumberOfItemsToSell=%d"),nAutoRunNumberOfItemsToSell);
    trace(L0,_T("AutoRun, TotalCashToPay=%d"),nAutoRunTotalCashToPay);
    trace(L0,_T("AutoRun, ItemCodeToSell=%s"),csAutoRunItemCodeToSell);
    trace(L0,_T("AutoRun, AssistModeOn=%d"),fAutoRunAssistModeOn);
    trace(L0,_T("AutoRun, AssistKeyList=%s"),csAutoRunAssistKeyList);
    InitializeDynamicOptions();

	return 0;
}

void ConfigObject::LoadLanguageOptions(CWinApp *pApp)
{
   
   if(fStateDualLanguage)
   {
      CString csEntry, csLanguageCode, csOptionName;
      int nStrIndex;
      int nLanguageMax=1;
      if(fStateDualLanguage)
        nLanguageMax=SCOT_LANGUAGE_MAX;

      for (int i=SCOT_LANGUAGE_PRIMARY; i<nLanguageMax; i++)
      {
         csOptionName.Format(_T("%s%d"),_T("Language"),i+1);
         if (pApp)
         {
           csEntry = pApp->GetProfileString(_T("State"), csOptionName);
         }
         if (csEntry.IsEmpty())
         {
           //TAR 137954////
           //If scotopts.000 is set for dual language and scotopts.xxx does not override
           //the following code will prevent the app from trying to read the language options
           //from scotopts.XXX and causing an error
           if(_tcsclen(language[i].szCode)>1)
	           continue;

           ////////////////
           // If you only have one language defined with dual language set, report error and reset dual language 
           if (m_iNumberOfLanguagesSupported <= 1)
           {
             ScotError(RETURN,SM_CAT_CONFIGURATION, SCOTAPP_DUAL_LANGUAGE_BAD_SETUP,_T("%s|%s"),
	             _T("State"),(LPCTSTR)csOptionName);
             fStateDualLanguage = false;
           }
           break;
         }


         //Get language code - ISO639 2 character code for this language
         nStrIndex = csEntry.Find(_T(','));
         csLanguageCode = csEntry.Left(nStrIndex);
         csLanguageCode.TrimLeft();
         csLanguageCode.TrimRight();


         //language code maybe 4 digits. for example, dutch is 0413, German is 0407
         if (csLanguageCode.GetLength() == 2)
         {
            language[i].szCode[0] = csLanguageCode[0];
            language[i].szCode[1] = csLanguageCode[1];
            language[i].szCode[2] = 0;
         }
         else if(csLanguageCode.GetLength() == 4)
         {
            language[i].szCode[0] = csLanguageCode[0];
            language[i].szCode[1] = csLanguageCode[1];
            language[i].szCode[2] = csLanguageCode[2];
            language[i].szCode[3] = csLanguageCode[3];
            language[i].szCode[4] = 0;
         }
         else //error, language code is not 2 characters
         {
             ScotError(RETURN,SM_CAT_CONFIGURATION, SCOTAPP_DUAL_LANGUAGE_BAD_SETUP,_T("%s|%s"),
	           _T("State"),(LPCTSTR)csOptionName);
             fStateDualLanguage = false;
             break;
         }

          //get language name
         csEntry = csEntry.Right(csEntry.GetLength() - nStrIndex - 1);
         nStrIndex = csEntry.Find(_T(','));
         language[i].csName = csEntry.Left(nStrIndex);
         language[i].csName.TrimLeft();
         language[i].csName.TrimRight();

         //get message file for language
         csEntry = csEntry.Right(csEntry.GetLength() - nStrIndex - 1);
         nStrIndex = csEntry.Find(_T(','));
         language[i].csMessageFile = csEntry.Left(nStrIndex);
         language[i].csMessageFile.TrimLeft();
         language[i].csMessageFile.TrimRight();

         //get Tare description file for this language
         csEntry = csEntry.Right(csEntry.GetLength() - nStrIndex - 1);
         nStrIndex = csEntry.Find(_T(','));
         language[i].csTareFile = csEntry.Left(nStrIndex);
         language[i].csTareFile.TrimLeft();
         language[i].csTareFile.TrimRight();

         //get security message file for this language
         csEntry = csEntry.Right(csEntry.GetLength() - nStrIndex - 1);
         nStrIndex = csEntry.Find(_T(','));
         language[i].csSecMessageFile = csEntry.Left(nStrIndex);
         language[i].csSecMessageFile.TrimLeft();
         language[i].csSecMessageFile.TrimRight();
        //get sound file directory for this language - no longer used
      //    csEntry = csEntry.Right(csEntry.GetLength() - nStrIndex - 1);
      //    language[i].csSoundPath = csEntry;
      //    language[i].csSoundPath.TrimLeft();
      //    language[i].csSoundPath.TrimRight();

         //get tender file for language
         if(nStrIndex > 0)
         {

            csEntry = csEntry.Right(csEntry.GetLength() - nStrIndex - 1);
            nStrIndex = csEntry.Find(_T(','));
            //if there is no value which set after tender file, don't care anything after that
            if(nStrIndex > 0)
               language[i].csTenderFile = csEntry.Left(nStrIndex);
            else
               language[i].csTenderFile = csEntry;
            language[i].csTenderFile.TrimLeft();
            language[i].csTenderFile.TrimRight();
         }
         else
         {
            language[i].csSecMessageFile = csEntry;
            if(csLanguageCode == _T("EN"))
               language[i].csTenderFile = _T("ScotTend.dat");
            else if(csLanguageCode == _T("ES"))
               language[i].csTenderFile = _T("TendSpan.dat");
            else if(csLanguageCode == _T("FR"))
               language[i].csTenderFile = _T("TendFren.dat");
            else if(csLanguageCode == _T("UK"))
			      language[i].csTenderFile = _T("TendUK.dat");
            else
               language[i].csTenderFile = _T("Tend") + csLanguageCode + _T(".dat");
         }

          //get PriceSoundConfig file for language
          //if there is no value which set for PriceSoundConfig file, set it to null string.
          //else get the value.
          if(nStrIndex > 0)
          {
	          csEntry = csEntry.Right(csEntry.GetLength() - nStrIndex - 1);
	          language[i].csPriceSoundConfigFile = csEntry;
	          language[i].csPriceSoundConfigFile.TrimLeft();
	          language[i].csPriceSoundConfigFile.TrimRight();
          }
          else
          {
	          //TAR 215165
	          language[i].csPriceSoundConfigFile = _T("PriceSoundConfig") + csLanguageCode + _T(".DAT");
          }

       //get the bitmap filname for the language's button in 'Select Language' screen
         language[i].csButtonBitmap = ps.GetBmpName(BMP_LANGUAGE1+i);


         // Save the real number of languages supported
         if (i+1 > m_iNumberOfLanguagesSupported)
         {
          m_iNumberOfLanguagesSupported = i + 1;
         }

         trace(L0, _T("Language%d code=%s, button=%s, message file=%s, tare file=%s, security file=%s, tender file=%s, price sound config file=%s"),
              i, language[i].szCode, language[i].csName, language[i].csMessageFile,
              language[i].csTareFile, language[i].csSecMessageFile, language[i].csTenderFile,
           language[i].csPriceSoundConfigFile);
      }
   }
   else
   {
      language[SCOT_LANGUAGE_PRIMARY].szCode[0] = csPrimaryLanguageCode[0];
      language[SCOT_LANGUAGE_PRIMARY].szCode[1] = csPrimaryLanguageCode[1];
      language[SCOT_LANGUAGE_PRIMARY].szCode[2] = csPrimaryLanguageCode[2];
      language[SCOT_LANGUAGE_PRIMARY].szCode[3] = csPrimaryLanguageCode[3];
      language[SCOT_LANGUAGE_PRIMARY].szCode[4] = 0;
   }
}


//void ConfigObject::LoadCashValueLists(CString csValueList, CString csLowList)
//dp185016 support glory device // High thresholds 
void ConfigObject::LoadCashValueLists(CString csValueList, CString csLowList, CString csHighList, CString csCashCapacity, CString csCashRemoveAmount)
{
  int nStrIndex = 0;
  int nStrAmount;
    //be sure we reload all options
    nCashValueList.RemoveAll();
    nCashLowList.RemoveAll();
    nCashHighList.RemoveAll();
    nCashCapacity.RemoveAll();
    nCashRemoveAmount.RemoveAll();

  // Get the values from the string as numbers.
  while (!csValueList.IsEmpty() && !csLowList.IsEmpty())
  {
      nStrIndex = csValueList.Find(_T(','));
      if (nStrIndex != -1)

      {

          nStrAmount = _ttoi( csValueList.Left(nStrIndex) );
          if (nStrAmount != 0)
              nCashValueList.Add( nStrAmount );
          csValueList = csValueList.Mid(nStrIndex+1);
      }
      else if (nStrIndex == -1)
      {
          nStrAmount = _ttoi( csValueList );
          if (nStrAmount != 0)
              nCashValueList.Add( nStrAmount );
          csValueList.Empty();
      }

      nStrIndex = csLowList.Find(_T(','));
      if (nStrIndex != -1)
      {
          nStrAmount = _ttoi( csLowList.Left(nStrIndex) );
          //if (nStrAmount != 0) //for tar 191904, allow 0 for cash low value
          nCashLowList.Add( nStrAmount );
          csLowList = csLowList.Mid(nStrIndex+1);
      }
      else if (nStrIndex == -1)
      {
          nStrAmount = _ttoi( csLowList );
          //if (nStrAmount != 0) //tar 301309
          nCashLowList.Add( nStrAmount );
          csLowList.Empty();
      }
        nStrIndex = csHighList.Find(_T(','));
        if (nStrIndex != -1)
        {
            nStrAmount = _ttoi( csHighList.Left(nStrIndex) );
            nCashHighList.Add( nStrAmount );
            csHighList = csHighList.Mid(nStrIndex+1);
        }
        else if (nStrIndex == -1)
        {
            nStrAmount = _ttoi( csHighList );
            nCashHighList.Add( nStrAmount );

            //get purge high list. the count is after the ';'

            nStrIndex = csHighList.Find(_T(';'));
            if( nStrIndex != -1 )
            {
              csHighList = csHighList.Mid(nStrIndex+1);
              nStrAmount = _ttoi( csHighList );
              nCashHighList.Add( nStrAmount );
            }
            csHighList.Empty();

        }

        nStrIndex = csCashCapacity.Find(_T(','));
        if (nStrIndex != -1)
        {
            nStrAmount = _ttoi( csCashCapacity.Left(nStrIndex) );
            nCashCapacity.Add( nStrAmount );
            csCashCapacity = csCashCapacity.Mid(nStrIndex+1);
        }
        else if (nStrIndex == -1)
        {
            nStrAmount = _ttoi( csCashCapacity );
            nCashCapacity.Add( nStrAmount );
            //get purge capacity count list. the count is after the ';'

            nStrIndex = csCashCapacity.Find(_T(';'));
            if( nStrIndex != -1 )
            {
              csCashCapacity = csCashCapacity.Mid(nStrIndex+1);
              nStrAmount = _ttoi( csCashCapacity );
              nCashCapacity.Add( nStrAmount );
            }
            csCashCapacity.Empty();
        }
        nStrIndex = csCashRemoveAmount.Find(_T(','));
        if (nStrIndex != -1)
        {
            nStrAmount = _ttoi( csCashRemoveAmount.Left(nStrIndex) );
            nCashRemoveAmount.Add( nStrAmount );
            csCashRemoveAmount = csCashRemoveAmount.Mid(nStrIndex+1);
        }
        else if (nStrIndex == -1)
        {
            nStrAmount = _ttoi( csCashRemoveAmount );
            nCashRemoveAmount.Add( nStrAmount );
            csCashRemoveAmount.Empty();
        }


  }


  if(!(csValueList.IsEmpty() && csLowList.IsEmpty() 
    && csHighList.IsEmpty() && csCashCapacity.IsEmpty() && csCashRemoveAmount.IsEmpty())
    && nCashCapacity.GetSize() != nCashHighList.GetSize() 
    && nCashCapacity.GetSize() <= nCashValueList.GetSize())
  {
      ScotError(RETURN,	SM_CAT_INTERNAL_ERROR, SCOTAPP_FAIL_PARSE, _T("%s|%s|%s|%s|%s"),
            csLocaleCashValueList, csLocaleCashLowList,csHighList, csCashCapacity, csCashRemoveAmount);
  }

}
/*
Initialize tender options and override these options with scot options.
*/
long ConfigObject::TenderInitialize(const int nTerminal)
{
	CString csTenderFile1, csTenderFile2, csTenderFile3;  //new Finalization
    int i;

	if(fStateDualLanguage) //Dual language
	{

        CString csTenderFile[SCOT_LANGUAGE_MAX];
        CString csTenderFile1[SCOT_LANGUAGE_MAX];
        CString csTenderFile2[SCOT_LANGUAGE_MAX];
        CString csTenderFile3[SCOT_LANGUAGE_MAX];

        // Set up the file names
        for (i=0; i< m_iNumberOfLanguagesSupported; i++)
        {
            csTenderFile[i] = language[i].csTenderFile;
            csTenderFile[i] = csTenderFile[i].Left(csTenderFile[i].GetLength() - 4);
		    csTenderFile1[i].Format(csConfigFilePath + _T("\\%s.DAT"), csTenderFile[i]);
		    csTenderFile2[i].Format(csConfigFilePath + _T("\\%s.%03d"), csTenderFile[i], 0);
		    csTenderFile3[i].Format(csConfigFilePath + _T("\\%s.%03d"), csTenderFile[i], nTerminal);
        }

        // Load the base options
        for (i=0; i< m_iNumberOfLanguagesSupported; i++)
        {
		    if (SMStateBase::OpenSCOTFile(csTenderFile1[i], true, true))
		    {
			    InitTenderOptions(csTenderFile1[i], (SCOT_LANGUAGE_TYPE)i);
            }
            else
            {
                // Quit if it isn't defined
                break;
            }
        }

        for (i=0; i< m_iNumberOfLanguagesSupported; i++)
        {
		    // see if ScotTend.000 exists or not
		    // free the current profile name buffer and set the new options file name
		    if (SMStateBase::OpenSCOTFile(csTenderFile2[i], false, false))
		    {
			    InitTenderOptions(csTenderFile2[i], (SCOT_LANGUAGE_TYPE)i);
            }
            // Not breaking here to keep compatible with original TAR 227432 fix
		    // see if scottend.xyz exists or not
		    // free the current profile name buffer and set the new options file name
		    if (SMStateBase::OpenSCOTFile(csTenderFile3[i], false, false))
		    {
			    InitTenderOptions(csTenderFile3[i], (SCOT_LANGUAGE_TYPE)i);
            }
            // Not breaking here to keep compatible with original TAR 227432 fix
		    ScotoptsOverrideScottend((SCOT_LANGUAGE_TYPE)i);
        }


	}
	else
	{
		csTenderFile1.Format(csConfigFilePath + _T("\\%s.DAT"), csDataTenderFile);
		csTenderFile2.Format(csConfigFilePath + _T("\\%s.%03d"), csDataTenderFile, 0);
		csTenderFile3.Format(csConfigFilePath + _T("\\%s.%03d"), csDataTenderFile, nTerminal);
		//new Finalization
		if (SMStateBase::OpenSCOTFile(csTenderFile1, true, true))
		{
			InitTenderOptions(csTenderFile1, SCOT_LANGUAGE_PRIMARY);
		}

		// see if ScotTend.000 exists or not
		// free the current profile name buffer and set the new options file name
		if (SMStateBase::OpenSCOTFile(csTenderFile2, false, false))
		{
			InitTenderOptions(csTenderFile2, SCOT_LANGUAGE_PRIMARY);
		}

		// see if scottend.xyz exists or not
		// free the current profile name buffer and set the new options file name
		if (SMStateBase::OpenSCOTFile(csTenderFile3, false, false))
		{
			InitTenderOptions(csTenderFile3, SCOT_LANGUAGE_PRIMARY);
		}

		ScotoptsOverrideScottend(SCOT_LANGUAGE_PRIMARY);
	}
	return 0;
}




// not yet completely implemented
void ConfigObject::InitializePrintObjects(SBmpPrintObject* pPrintObjList)
{
	LPTSTR pKeynames[] = {_T("ChitBmp1"), _T("ChitBmp2")};
	CString pKeyValues[MAX_PRINTOBJECTS];
	pKeyValues[0] = csPrintObjectsChitBmp1;
	pKeyValues[1] = csPrintObjectsChitBmp2;

	if (!pPrintObjList)
		return;	// nothing to do to initialize if the list is not preallocated

	for(int i = 0; i < MAX_PRINTOBJECTS; ++i)
	{
		CString csWork = 	pKeyValues[i];
		if (!csWork.IsEmpty())
		{
			try
			{
				int tmpWidth, tmpAlign;
				CString csFileName;
				float tmpSF;   //TAR 271776

				int nTmpFileNameEndPos = csWork.Find(_T(','));
				if (nTmpFileNameEndPos != -1)
				{
					csFileName = csWork.Left(nTmpFileNameEndPos);
					csWork = csWork.Mid(nTmpFileNameEndPos+1);
				}
				_stscanf(csWork,_T("%d,%d,%f"), &tmpAlign, &tmpWidth, &tmpSF);  //TAR 271776
				if (csFileName.GetLength())
				{
					pPrintObjList[i].csBmpName = csFileName;
					pPrintObjList[i].nBmpWidth = tmpWidth;
					pPrintObjList[i].nAlignment = tmpAlign;
					pPrintObjList[i].scalingFactor = tmpSF;   //TAR 271776
				}
			} // try
			catch (...)
			{
				trace(L0, _T("InitializePrintObjects: doesnt have proper value for %s"),pKeynames[i]);
				ASSERT(0);
			} // catch
		}
	} // for loop
}

//New Finalization
long ConfigObject::InitTenderOptions(CString& csFileName, SCOT_LANGUAGE_TYPE language_used)
{
		// load the profile
	int i;
	CWinApp* pApp = AfxGetApp();
//	CString csAppProfile = pApp->m_pszProfileName;
	LPCTSTR pszAppProfile = pApp->m_pszProfileName;
	pApp->m_pszProfileName = csFileName;


	for (int iReset = 0; iReset < MAXBUTTONNUM; iReset++)
	{
		TenderList[language_used][iReset].nTenderType = 0;
		TenderList[language_used][iReset].csTenderText = _T("");
		TenderList[language_used][iReset].csTenderBitmap = _T("");
		TenderList[language_used][iReset].bCashBack = 0;
		TenderList[language_used][iReset].bConfirmTotal = 0;
		TenderList[language_used][iReset].bNeedPin = 0;
		TenderList[language_used][iReset].bIntegratedMSR = 0;
		TenderList[language_used][iReset].bCaptureSignature = 0;
		TenderList[language_used][iReset].bVerifySignature = 0;
		TenderList[language_used][iReset].nVerifySignatureOverAmount = -1;
		TenderList[language_used][iReset].nAssistedTender = 0;
		TenderList[language_used][iReset].nButtonNum = 0;
		TenderList[language_used][iReset].bCashBackIfVoided=0; //RFC 275377 MIGRATED-BM185018
		// TenderLimit = 0 means always intervention
		// TenderLimit = -1 means no intervention
		// TenderLimit > 0 means intervention if current transaction total exceeds this limit
		// TenderLimit here is in base units of currency (cents in US)
		TenderList[language_used][iReset].nTenderLimit = -1; // no intervention by default, 0- always
	}

	for (i = 0;  i < MAXBUTTONNUM; i++)
	{

		CString csSection;
		csSection.Format(_T("%s%d"), _T("button"), (i+1));
		TenderList[language_used][i].nTenderType = GetOptInt(pApp, csSection, _T("TenderType"), TenderList[language_used][i].nTenderType, fErrorLog);

		TenderList[language_used][i].csTenderText = GetOptStr(pApp, csSection, _T("Text"),TenderList[language_used][i].csTenderText, fErrorLog);
		TenderList[language_used][i].csTenderBitmap = GetOptStr(pApp, csSection, _T("Bitmap"), TenderList[language_used][i].csTenderBitmap, fErrorLog);
		TenderList[language_used][i].bCashBack = GetOptFlg(pApp, csSection, _T("CashBack"), TenderList[language_used][i].bCashBack, fErrorLog);
		TenderList[language_used][i].bConfirmTotal = GetOptFlg(pApp, csSection, _T("ConfirmTotal"), TenderList[language_used][i].bConfirmTotal, fErrorLog);
		TenderList[language_used][i].bNeedPin = GetOptFlg(pApp, csSection, _T("NeedPin"), TenderList[language_used][i].bNeedPin, fErrorLog);
		TenderList[language_used][i].bIntegratedMSR = GetOptFlg(pApp, csSection, _T("IntegratedMSR"), TenderList[language_used][i].bIntegratedMSR, fErrorLog);
		TenderList[language_used][i].bCaptureSignature = GetOptFlg(pApp, csSection, _T("CaptureSignature"), TenderList[language_used][i].bCaptureSignature, fErrorLog);
		TenderList[language_used][i].bVerifySignature = GetOptFlg(pApp, csSection, _T("VerifySignature"), TenderList[language_used][i].bVerifySignature, fErrorLog);
		TenderList[language_used][i].nVerifySignatureOverAmount = GetOptInt(pApp, csSection, _T("VerifySignatureOverAmount"), TenderList[language_used][i].nVerifySignatureOverAmount, fErrorLog);
		TenderList[language_used][i].nAssistedTender = GetOptInt(pApp, csSection, _T("AssistedTender"), TenderList[language_used][i].nAssistedTender, fErrorLog);
		TenderList[language_used][i].nButtonNum = i+1;
		TenderList[language_used][i].nTenderLimit = GetOptInt(pApp, csSection, _T("TenderLimit"), TenderList[language_used][i].nTenderLimit, fErrorLog);
		TenderList[language_used][i].bCashBackIfVoided = GetOptFlg(pApp, csSection, _T("CashBackIfVoided"), TenderList[language_used][i].bCashBackIfVoided, fErrorLog);// RFC 275377 MIGRATED-BM185018
	}
	pApp->m_pszProfileName = 	pszAppProfile;
	return 1;
}



void ConfigObject::ScotoptsOverrideScottend(SCOT_LANGUAGE_TYPE language_used)
{

	for (int i = 0;  i < MAXBUTTONNUM; i++)
	{
		if(TenderList[language_used][i].nTenderType == TB_TENDER_CREDIT || TenderList[language_used][i].nTenderType == TB_TENDER_PIP_CREDIT)  //credit card
		{
			if( bReadCreditNeedsPIN)
				TenderList[language_used][i].bNeedPin = fTenderCreditNeedsPIN;
			if(bReadCreditCashBack)
				TenderList[language_used][i].bCashBack = fTenderCreditCashBack;
			if(bReadSignatureCapture)
				TenderList[language_used][i].bCaptureSignature =m_bOperationsSignatureCapture;
			if(bReadVerifyCapture)
				TenderList[language_used][i].bVerifySignature = m_bOperationsVerifySignature;
			if(bReadCreditMSR)
				TenderList[language_used][i].bIntegratedMSR = fOperationsCreditMSR;
		}

		if(TenderList[language_used][i].nTenderType == TB_TENDER_DEBIT || TenderList[language_used][i].nTenderType == TB_TENDER_PIP_DEBIT || TenderList[language_used][i].nTenderType == TB_TENDER_PIP_DEBIT_CASHBACK)  //debit card
		{
			if(bReadDebitNeedsPIN)
				TenderList[language_used][i].bNeedPin = fTenderDebitNeedsPIN;
			if(bReadDebitCashBack)
				TenderList[language_used][i].bCashBack = fTenderDebitCashBack;
			if(bReadDebitNeedsSignatureCapture)
				TenderList[language_used][i].bCaptureSignature =m_bOperationsDebitNeedsSignatureCapture;
			if(bReadDebitNeedsVerifyCapture)
				TenderList[language_used][i].bVerifySignature = m_bOperationsDebitNeedsVerifySignature ;
			if(bReadDebitMSR)
				TenderList[language_used][i].bIntegratedMSR = fOperationsDebitMSR;
		}

		if(TenderList[language_used][i].nTenderType == TB_TENDER_FOODSTAMPS || TenderList[language_used][i].nTenderType == TB_TENDER_PIP_FOODSTAMPS)  //food stamp
		{
			if(bReadEBTFSNeedsPIN)
				TenderList[language_used][i].bNeedPin = fTenderEBTFSNeedsPIN;
			if(bReadFoodStampMSR)
				TenderList[language_used][i].bIntegratedMSR = fOperationsFoodStampsMSR;
		}


		if(TenderList[language_used][i].nTenderType == TB_TENDER_EBT || TenderList[language_used][i].nTenderType == TB_TENDER_PIP_EBT)  //Cash Benefit
		{
			if(bReadEBTCBNeedsPIN)
				TenderList[language_used][i].bNeedPin = fTenderEBTCBNeedsPIN;
			if(bReadEBTCBCashBack)
				TenderList[language_used][i].bCashBack = fTenderEBTCBCashBack;
			if(bReadEBTMSR)
				TenderList[language_used][i].bIntegratedMSR = fOperationsEBTMSR;
		}

		if(TenderList[language_used][i].nTenderType == TB_TENDER_DEBITSAVING || TenderList[language_used][i].nTenderType == TB_TENDER_PIP_DEBITSAVING)  //debit saving
		{
			if(bReadDebitSavingsMSR )
				TenderList[language_used][i].bIntegratedMSR = fOperationsDebitSavingsMSR;
		}

		if(TenderList[language_used][i].nTenderType == TB_TENDER_DEBITCHECKING || TenderList[language_used][i].nTenderType == TB_TENDER_PIP_DEBITCHECKING)  //debit checking
		{
			if( bReadDebitCheckingMSR)
				TenderList[language_used][i].bIntegratedMSR = fOperationsDebitCheckingMSR;
		}

		if(TenderList[language_used][i].nTenderType == TB_TENDER_GIFTCARD || TenderList[language_used][i].nTenderType == TB_TENDER_PIP_GIFTCARD)  //GiftCard
		{
			if(bReadGiftCardMSR)
				TenderList[language_used][i].bIntegratedMSR = fOperationsGiftCardMSR;
		}
		if(TenderList[language_used][i].nTenderType == TB_TENDER_LOYALTY || TenderList[language_used][i].nTenderType == TB_TENDER_PIP_LOYALTY)  //GiftCard
		{
			if(bReadLoyaltyMSR)
				TenderList[language_used][i].bIntegratedMSR = fOperationsLoyaltyMSR;
		}
	}

	return;
}



long ConfigObject::InitAllOptions(CString& csFileName)
{
	// load the profile
	CWinApp* pApp = AfxGetApp();
//	CString csAppProfile = pApp->m_pszProfileName;
	LPCTSTR pszAppProfile = pApp->m_pszProfileName;
	pApp->m_pszProfileName = csFileName;

	// CMal090503: TAR 232965  Part of the reason for this problem is that 
	// we need to know if this is SCOTOpts.dat or one of the secondary files
	CString csFoundDotDat;  BOOL bThisIsDotDat = FALSE;
	csFoundDotDat = csFileName;
	csFoundDotDat.MakeUpper();
	if (csFoundDotDat.Find(_T("SCOTOPTS.DAT")) > 0)
		bThisIsDotDat = TRUE;
	// CMal090503 TAR + End - know your audience.

	// [Tender]
	// CashAllowed=N			Cash tender allowed
	// CreditAllowed=N		Credit tender allowed
	// CreditNeedsPIN=N		Credit needs a pin number
	// CreditCashBack=N		Credit allows cash back
	// DebitAllowed=N			Debit tender allowed
	// DebitNeedsPIN=N		Debit needs a pin number
	// DebitCashBack=N		Debit allows cash back
	// CashBack1=0			Cash Back amount 1
	// CashBack2=0			Cash Back amount 2
	// CashBack3=0			Cash Back amount 3
	// CashBack4=0			Cash Back amount 4
	// CashBack5=0			Cash Back amount 5
	// CashBack6=0			Cash Back amount 6

	// Add EBT support

	// EBTFSAllowed=N			Debit tender allowed
	// EBTFSNeedsPIN=N		Debit needs a pin number
	// EBTCBAllowed=N			Debit tender allowed
	// EBTCBNeedsPIN=N		Debit needs a pin number
	// EBTCBCashBack=N		Debit allows cash back

	// EBTCashBack1=0		 Cash Back amount 1
	// EBTCashBack2=0		 Cash Back amount 2
	// EBTCashBack3=0		 Cash Back amount 3
	// EBTCashBack4=0		 Cash Back amount 4
	// EBTCashBack5=0		 Cash Back amount 5
	// EBTCashBack6=0		 Cash Back amount 6

	// [Operations]
	// AlcoholAge=99				  age in years to buy alcohol items
	// TobaccoAge=99				  age in years to buy tobacco items
	// ScaleMetric=N		  Y/N, N=false Y=true
	// TareContainer1		 0-10 for tare value for button 1 on SelectContainer
	// TareContainer2		 0-10 for tare value for button 2 on SelectContainer
	// MaxQuantity = 50	   max allowed manual input qty
	// MaxCoupon = 500		max allowed manual coupon value
	// MaxGiftCard = 0

	// [State]
	// SayPrices = Y		    flag value Y/N
	// SayPhrases = Y		    flag value Y/N
    // AllowTBAudioControl = N
	// LiveVideo = N		    flag value Y/N
	// WelcomeScreenVideo = Y
	// SelectListScreenVideo = Y
	// LargeFontOnCurrentItem = N
	// AllowLoyaltyCard = N  disable magnetic-read loyalty cards
	// LoyaltyCardReminder = N display loyalty card reminder on finish and pay screen
	// AssistMode = Y       flag value Y/N
	// fStateLookupItemIsButton1 = Y
	// fStateFlashLookupItemButton= Y
	// fStateFlashKeyInCodeButton = Y
	// [TimeOut] (timeout values in seconds)
	// Tender				 wait for supermarket tender processing
	// StateDefault		   default for base state
	// TakeCard			   wait to eat the card
	// TakeCash			   wait to take back paper money
	// TakeCoins			  wait to take back coin change
	// ItemTransactionResult  wait for supermarket item processing
	// ContinueTransaction	wait at continue transaction screen
	// Help				   wait at help screen
	// Signon				 wait to sign on from Attract
	// Signoff				wait to sign off from Attract if already signed on
	// Attract				Attract screen timeout
	// Item sale in progress	time out value for item sale complete
	// Check Basket Reminder  wait at Check Basket Reminder Screen
	// FirstTimeUser		  First Time User state
	//
	// [Tracing]
	// NumEntries=0		   number of entries from start up
	// CurrentLevel=0		 general tracing level at IPL
	// FocusLevel=0		   focused trasing level at startup
	// FocusComps=""		  focus Comp_IDs, TB,PS,MP,ST,DM... see errorobject.h
	///////////////////////////////////////////////////////////
	_GET_FLG(Tender,CashAllowed);			// load option co.fTenderCashAllowed
	_GET_FLG(Tender,CreditAllowed);		// load option co.fTenderCreditAllowed
	_GET_FLG2(Tender,CreditNeedsPIN, &bReadCreditNeedsPIN);      // load option co.fTenderCreditNeedsPIN
	_GET_FLG2(Tender,CreditCashBack, &bReadCreditCashBack);      // load option co.fTenderCreditCashBack
	_GET_FLG(Tender,DebitAllowed);		// load option co.fTenderDebitAllowed
	_GET_FLG2(Tender,DebitNeedsPIN, &bReadDebitNeedsPIN);	    // load option co.fTenderDebitNeedsPIN
	_GET_FLG2(Tender,DebitCashBack, &bReadDebitCashBack);		// load option co.fTenderDebitCashBack

	
	// TAR 232965: having non-matching CashBack entries in .000 as compared to
	//				   SCOTOpts.000, only mistaken remembers the .000.
	//					CMal090503: rewrite this code to better accomplish the goal
	if (bThisIsDotDat == TRUE)
	{
		// this Macro call sets nTenderCashBackNum
		_GET_INT_ARRAY(Tender,CashBack, MAX_TENDER_CASHBACK_VALUES, 0, true);	// load option co.nTenderCashBack array

	}
	else // one of the SCOTOpts.000 or other overrides
	{

		int nSaveCashbackNum = nTenderCashBackNum;  // save the .DAT number of CashBack options

		// this Macro call sets nTenderCashBackNum & can't fill in more values than the .DAT
		// CMal091703 TAR 248007 - want to see the missing values as -99, as 0 means remove.
		_GET_INT_ARRAY(Tender,CashBack, MAX_TENDER_CASHBACK_VALUES, -99, false);	// load option co.nTenderCashBack array
		
		// CMal091703 TAR 248007 - now GetOptIntArray sets nTenderCashBackNum correctly
	}
	// TAR 232965: end CMal090503

	ASSERT(nTenderCashBackNum <= MAX_TENDER_CASHBACK_VALUES);

	_GET_STR(Tender,Denominations);	    // load option co.csTenderDenominations
	_GET_STR(Tender,AllowCoupons);		// option value A=All modes, O=Only in assist mode, N=No modes	TAR#102019	LPM090799
	_GET_FLG(Tender,ShowUseCoupons);      // option value Y=Enable, N=Disable
	//  Add EBT support
	_GET_FLG(Tender,EBTFSAllowed);		// load option co.fTenderEBTFSAllowed
	_GET_FLG2(Tender,EBTFSNeedsPIN, &bReadEBTFSNeedsPIN);	    // load option co.fTenderEBTFSNeedsPIN
	//_GET_FLG(Tender,EBTFSCashBack);		// used to load option co.fTenderEBTFSCashBack, no longer used

	_GET_FLG(Tender,EBTCBAllowed);		// load option co.fTenderEBTCBAllowed
	_GET_FLG2(Tender,EBTCBNeedsPIN, &bReadEBTCBNeedsPIN);	    // load option co.fTenderEBTCBNeedsPIN
	_GET_FLG2(Tender,EBTCBCashBack, &bReadEBTCBCashBack);		// load option co.fTenderEBTCBCashBack

	_GET_FLG(Tender,AllowCancelWithPartialEBTTender);		// load option co.fTenderAllowCancelWithPartialEBTTender


	// TAR 232965: having non-matching CashBack entries in .000 as compared to
	//				   SCOTOpts.000, only mistaken remembers the .000.
	//					CMal090503: rewrite this code to better accomplish the goal
	if (bThisIsDotDat == TRUE)
	{
		// this Macro call sets nTenderEBTCashBackNum
		_GET_INT_ARRAY(Tender,EBTCashBack, MAX_TENDER_EBT_CASHBACK_VALUES, 0, true);	// load option co.nTenderEBTCashBack array
	}
	else // one of the SCOTOpts.000 or other overrides
	{

		int nSaveEBTCashbackNum = nTenderEBTCashBackNum;  // save the .DAT number of CashBack options

		// this Macro call sets nTenderEBTCashBackNum & can't fill in more values than the .DAT
		// CMal091703 TAR 248007 - want to see the missing values as -99, as 0 means remove.
		_GET_INT_ARRAY(Tender,EBTCashBack, MAX_TENDER_CASHBACK_VALUES, -99, false);	// load option co.nTenderEBTCashBack array
		
		// CMal091703 TAR 248007 - now GetOptIntArray sets nTenderCashBackNum correctly

	}
	// TAR 232965: end CMal090503

	ASSERT(nTenderEBTCashBackNum <= MAX_TENDER_EBT_CASHBACK_VALUES);

	_GET_FLG(Tender,OtherPayment);		// load option co.fTenderOtherPayment
	_GET_FLG(Tender,SubmitFSTtotalIfMorethanTrxTotal);
	_GET_FLG(Tender,IsMotorizedMSR);			// for motorized MSR
	_GET_FLG(Tender,DispenseChangeAtLaneForRemoteTendering);   //For remote tendering transaction, dispense change at Lane side or pop RAP cash drawer at RAP side. The default value is Y
	//_GET_FLG(Tender,PIPPINEncryptorOnly); // future use

	// get common operations  options
	//  _GET_INT(Operations,AlcoholAge);	// age in years to buy res'd items
	//  _GET_INT(Operations,TobaccoAge);	// age in years to buy res'd items //Tobacco
	_GET_FLG(Operations,ScaleMetric);		// N=false Y=true
	_GET_INT(Operations,MaxCoupon);		// max allowed manual coupon value
	_GET_INT(Operations,MaxCouponLimit);	//SR678 - max allowed coupon for scanned or keyed in coupon
	_GET_INT(Operations,DuplicateCouponMax);
	_GET_INT(Operations,TotalCoupons$AmtMax);
	_GET_INT(Operations,Upper$AmtForPercentToApply);
	_GET_INT(Operations,CouponPercentofGrossSales);
	_GET_FLG(Operations,AllowSkipBaggingBtn);
	_GET_FLG(Operations,AllowStoreLoginBtnOnBagItemScreen);		//BZ 06/07/02 RFC 203538
	_GET_FLG(Operations,AllowStoreLoginBtnOnRemoveItemScreen);	//BZ 06/07/02 RFC 203538
	_GET_INT(Operations,OperatorLoginBarcodeType);   //NN 072902 RFC 187993
	_GET_STR(Operations,OperatorBarcodeMask);        //NN 073002 RFC 187993
    _GET_FLG(Operations,AllowLeadingZerosInCode128LoginBarCode);//rfc 240738
	_GET_FLG(Operations,ShowItemRemovedButtonOnRemoveItemViolationScreen);  //CHU 07/25/02 RFC 208874
	_GET_FLG(Operations,AllowContainerBtn);
	_GET_FLG(Operations,DisableUnexpIncContainerBtnAftFirstItem); //don't prompt ownbag at unex inc
	_GET_FLG(Operations,AllowContainerBtnForMismatching); //Tar 266250
	_GET_FLG(Operations,AllowContainerBtnInFinalization);//SR085
	_GET_FLG(Operations,AllowItemRemovedBtn);
	_GET_FLG(Operations,AllowAMDoneBtn);
	_GET_FLG(Operations,SuppressAudioInItemRemoveViolation); // RFC 252471
	_GET_FLG(Operations,SuppressAudioInUnexpectedIncrease); // 3.2 patch B - RFC 257450
	_GET_FLG(Operations,SuppressAudioInMismatch); // 3.2 patch B - RFC 257449
	_GET_FLG(Operations,ShowSkipBaggingOnEReceipt);			// RFC 252473
	_GET_INT(Operations,MaxGiftCard);   // max allowed gift card value
	_GET_FLG(Operations,PrintVersion);	// prints version info of SCOT App on a receipt and journal if any
	_GET_FLG(Operations,PrintPendingApprovalOnSuspend);	// append outstanding approvals to receipt on suspend
	_GET_FLG(Operations,ReprintReceiptByTransNumber);
	_GET_FLG(Operations,OperatorTwoLineText);
	_GET_FLG(Operations,HopperSubstitutionIntervention);
	_GET_INT(Operations,HopperFixLaterTrxLimit);
	_GET_FLG(Operations, PrintReceiptOnCancelledTransaction); //dont print receipt on cancelled transaction
	_GET_STR(Operations,EOTReceiptPrintingState); //Delay receipt printing until finalization intervention complete gc185038
	_GET_FLG(Operations, AddPaperCutToTransReceipt);  // RFC 368220 - Need option to disable CutReceipt() added to Transaction Receipt ac185097
// fiscal printer start
	_GET_FLG(Operations,TBControlsPrinter); // TB controls printer ?
// fiscal printer end
    _GET_FLG(Operations,DisableBasePriceEmbeddedDefinitions);//RFC239992
	_GET_FLG(Operations,TruncateSupplementalCodeForSecurity);  // Truncate supplemental code before sending UPC to Security
	_GET_FLG(Operations,SelectTareOnWeight);	// Weighted Item need Tare?
	_GET_INT(Operations,AutoTareCode);
	// load option co.csOperationsHandleUnDeActivatedItemAs
	_GET_STR(Operations,HandleUnDeActivatedItemAs);
	//RFC 210177 begin
	_GET_FLG(Operations,LogWLDBmismatch);
	_GET_INT(Operations,WLDBmismatchMaxFileSize);
	//RFC 210177 end
	_GET_FLG(Operations,CheckCashChanger);  //tar 256141
	_GET_FLG(Operations,ShowWeightInfoOnLane); //SR682
	_GET_FLG(Operations,AllowDegradedMode);	//SR742
	// Get Glory-specific options.
	_GET_INT(Operations, GloryDispenseRetries);       // Load nOperationsGloryDispenseRetries. TAR 310660
    //Load In-Store barcodes start

	CString CSCustomCodeValue;
	_TCHAR CSCustomCode[20] = _T("CustomCode");
	int count = 1;
	_stprintf(CSCustomCode,_T("CustomCode%d"), count++);
	CSCustomCodeValue = pApp->GetProfileString(_T("InStoreBarcodes"), CSCustomCode, UNSET_KEYVALUE);
		while ((CSCustomCodeValue != UNSET_KEYVALUE) && (CSCustomCodeValue.GetLength()))
		{
			cstoreCode.AddInstoreCodeCList(CSCustomCodeValue);
			trace(L0, _T("%s is : %s"), CSCustomCode, CSCustomCodeValue);
			_stprintf(CSCustomCode,_T("CustomCode%d"), count++);
			CSCustomCodeValue = pApp->GetProfileString(_T("InStoreBarcodes"), CSCustomCode, UNSET_KEYVALUE);
		}
    //Load In-Store Barcodes end
    ///////////////////////////////////////////////////////
    //START Load Multi-Label Barcodes 
	CString csRecord,csEntry;
	int nCount = 1;
    do {
        csRecord.Format(_T("MultiLabelFormat%d"),nCount);
        csEntry = pApp->GetProfileString(_T("MultiLabelBarCodes"), csRecord, UNSET_KEYVALUE);
        if( UNSET_KEYVALUE == csEntry )  
            break;
        if(g_MultiLabelBarcodes.ProcessRecords(nCount++, csEntry))
		    trace(L0, _T("%s is : %s"), csRecord, csEntry);
        else
            trace(L0, _T("!!ERROR!! g_MultiLabelBarcodes.ProcessRecords() %s is : %s"), csRecord, csEntry);
    } while(true);
    //END Load Multi-Label Barcodes 

    // Load Coupons start
	CString csCoupon, csCouponValue;
	int nCoupon = 1;
	csCoupon.Format(_T("CustomCoupon%d"),nCoupon);
    csCouponValue = pApp->GetProfileString(_T("Coupons"), csCoupon, UNSET_KEYVALUE);
	while( (csCouponValue != UNSET_KEYVALUE) && (csCouponValue.GetLength()) )
	{
		csaCouponlist.SetAtGrow(nCoupon-1,csCouponValue);
		trace(L0, _T("%s is : %s"), csCoupon, csCouponValue);
		csCoupon.Format(_T("CustomCoupon%d"), ++nCoupon);
		csCouponValue = pApp->GetProfileString(_T("Coupons"), csCoupon, UNSET_KEYVALUE);
	}
	// END Load coupons

	_GET_INT(Operations,EOD);				     //Load FastLane 3.1 EOD feature 
	_GET_INT(Operations,EODTimeout);			 
	_GET_FLG(Operations,JumpToTarePickList);	// Bypass First Select Container Screen
	_GET_FLG(Operations,AllowMultiPick);
	_GET_FLG(Operations,DisplayPickListItemCode);
	_GET_FLG(Operations,DisplayQuickPickItems);
	_GET_FLG(Operations,AllowPickListSearchButton);
    _GET_FLG(Operations,AllowFuzzySearch);

	_GET_INT(Operations,TareContainer1);	// tare value 0-10
	_GET_INT(Operations,TareContainer2);	// tare value 0-10
	_GET_FLG(Operations,UPCAPriceEmbeddedVerifier);		// Is Price Embedded Verifier=Y then UPC A2/CD PriceEmbedded format = 02 iiii V ppppp C  (leading zero optional)

	//Begin_Suspend
	_GET_FLG(Operations, Suspend);               // flag if suspend transaction
	_GET_FLG(Operations, VoidCouponBeforeSuspend);  // flag if need void coupon before suspend
    _GET_FLG(Operations, AllowSuspendIfPartiallyTendered); //RFC 267820
    _GET_FLG(Operations, IfSuspendedDispensePartiallyPaidCash); //RFC 267820
	//End_Suspend
	_GET_FLG(Operations,AutoVoidUnderageRestrict);  //Feng122299
	_GET_FLG(Operations,SendSignatureDataToTB);

   	// Partial Integrated PINPad
    _GET_FLG(Operations,PartialIntegratedPINPad);
    _GET_FLG2(Operations,PIPSCOTPromptCardType, &bReadPIPSCOTPromptCardType);
	_GET_FLG2(Operations,PIPSCOTPromptCashBack, &bReadPIPSCOTPromptCashBack);
	_GET_FLG2(Operations,PIPSCOTPromptConfirmTotal, &bReadPIPSCOTPromptConfirmTotal);

	//PIP with Integrated MSR options
	_GET_FLG2(Operations,CreditMSR, &bReadCreditMSR);
	_GET_FLG2(Operations,DebitMSR, &bReadDebitMSR);
	_GET_FLG2(Operations,DebitSavingsMSR, &bReadDebitSavingsMSR);
	_GET_FLG2(Operations,DebitCheckingMSR, &bReadDebitCheckingMSR);
	_GET_FLG2(Operations,EBTMSR, &bReadEBTMSR);
	_GET_FLG2(Operations,FoodStampsMSR, &bReadFoodStampMSR);
	_GET_FLG2(Operations,GiftCardMSR, &bReadGiftCardMSR);
	_GET_FLG(Operations,PhoneCardMSR);
	_GET_FLG2(Operations,LoyaltyMSR, &bReadLoyaltyMSR);

	//Integrated MSR info
	_GET_FLG(Operations,Use7401MSR);

	_GET_BFLG2(Operations, DebitNeedsSignatureCapture, &bReadDebitNeedsSignatureCapture);  // load option co.m_bOperationsDebitNeedsSignatureCapture
	_GET_BFLG2(Operations, DebitNeedsVerifySignature, &bReadDebitNeedsVerifyCapture);   // load option co.m_bOperationsDebitNeedsVerifySignature
	_GET_BFLG2(Operations, SignatureCapture, &bReadSignatureCapture);  // flag if signature capture is configured
	_GET_BFLG2(Operations, VerifySignature, &bReadVerifyCapture);	  // flag if signature is to be verified.
	_GET_BFLG(Operations, PrintSignatureReceipt);  // flag if signature capture is configured
	_GET_BFLG(Operations, AllowCancelAll);	  // flag if Cancel All is allowed, TAR 102011
	_GET_INT(Operations,  AllowDispenseInTrainingMode); // 0-doesn't allow, 1-allow dispense of change, other values- reserved for future
	_GET_FLG(Operations, InterventionAtStartOfTrainingModeTransaction);  // RAP Intervention in each Training Mode Transaction cs185088
	trace(L0, _T("InterventionAtStartOfTrainingModeTransaction = %d"), fOperationsInterventionAtStartOfTrainingModeTransaction);
	_GET_FLG(Operations,  LogDataForSystemReports); // Flag whether to log for Reports
	_GET_INT(Operations,  NumberOfReportLogsToKeep);
	_GET_FLG(Operations,  CenterCustomerDataNeededButtons);  // flag whether to center the buttons for customer mode dataneeded msgs
	_GET_BFLG(Operations, UseFingerPrintDevice);   //SR93 Biometrics

   // How to display Toggle option on Remote Assist Mode:
   // 0 (default) = Disable Toggle Functionality and display nothing.
   // 1 = Display Lane Number only if lane needs intervention.
   // 2 = Display Lane Status only if lane needs intervention.
   // 3 = Always display lane number, but display lane status when intervention required.
   _GET_INT(Operations, RemoteAssistModeToggleDisplay);
   _GET_INT(Operations, GiftCardRedemption);
   _GET_INT(Operations, MethodOfCollectionForEmptyGiftCard);
   _GET_FLG(Operations, DisplayGiftCardBalance);
	_GET_FLG(Operations, CustomerBagAllowed);//SR085
	_GET_FLG(Operations, IncludeCouponsInItemCount);	//RFC 371075

	_GET_STR(State,MoviePlayOnAttract);		  // load option co.csStateMoviePlayOnAttract
	_GET_STR(State,MoviePlayOnAttractBelt);	  // load option co.csStateMoviePlayOnAttractBelt
	_GET_FLG(State,AllowInterruptWavFiles);	  // flag value Y/N
	_GET_FLG(State,SayPrices);			      // flag value Y/N
    _GET_FLG(State,SayPricesDisableInAttendantMode); // flag value Y/N 
	_GET_FLG(State,SayPhrases);			      // flag value Y/N
    _GET_FLG(State,AllowTBAudioControl);      // flag value Y/N
	_GET_FLG(State,LiveVideo);			      // flag value Y/N
	_GET_FLG(State,WelcomeScreenVideo);	      // flag value Y/N
	_GET_FLG(State,SelectListScreenVideo);	  // flag value Y/N
	_GET_FLG(State,GraphicalSelectList);		// flag value Y/N
   _GET_FLG(State,ProduceFavorites);		// flag value Y/N
	_GET_FLG(State,OverridePickDesc);		// flag value Y/N
   _GET_STR(State,DefaultProduceBMPName);
    _GET_STR(State,InvalidCouponPrefix);       //TAR269159
	_GET_FLG(State,DisplayWeightsAndMeasures);// flag value Y/N
	_GET_FLG(State,LargeFontOnCurrentItem);	  // flag value Y/N

	_GET_FLG(State,HasSidecar);			      // flag value 0-no, other-yes
	_GET_FLG(State,AllowLoyaltyCard);		  // flag value Y/N
	_GET_FLG(State,LoyaltyCardReminder);	  // flag value Y/N
	_GET_FLG(State,LoyaltyCardReminderAfterFirstItemSold);	  // flag value Y/N
	_GET_FLG(State,AssistMode);               // flag value Y/N
	_GET_FLG(State,LookupItemIsButton1);      // flag value Y/N
	_GET_FLG(State,AllowKeyInCode);	  // flag if AllowKeyInCode is allowed, TAR 173139 Y/N
	_GET_FLG(State,AllowLookUpItem);		// flag if AllowLookUpItem is allowed, TAR 173139
	_GET_FLG(State,FlashLookupItemButton);    // flag value Y/N
	_GET_FLG(State,FlashKeyInCodeButton);     // flag value Y/N
	_GET_FLG(State,ForceWeighingOFPriceEmbedded);	// flag value Y/N
	_GET_FLG(State,ConfirmationForSuspendingTransaction); //flag value Y/N
	_GET_FLG(State,CheckBasketReminder); //flag value Y/N
	_GET_FLG(State,CouponSensor);        //flag value Y/N
	_GET_FLG(State,DOBNotNeededButton);       //flag value Y/N
	_GET_FLG(State,DisplayNumberOfItemsOnRAP);	//flag value Y/N
	_GET_FLG(State,DualLanguage);             // flag value Y/N
	_GET_FLG(State,SingleLanguageButtonAtAttract); //RFQ SR681 flag value Y/N
	_GET_FLG(State,AllowLanguageSelectionDuringTransaction);  // flag value Y/N
   if (!fStateDualLanguage)
   {
      m_iNumberOfLanguagesSupported = 1;
		fStateSingleLanguageButtonAtAttract = false;
   }
   LoadLanguageOptions(pApp);

	_GET_FLG(State, HasCardMEI);             // flag value Y/N
	_GET_FLG(State, SystemFunctionsBtnOnLaneStatusScreen);	// flag value Y/N
    _GET_FLG(State, EscalateTakeYourItemsTimeout); //rfc209702
	_GET_FLG(State, PaintScanAndBagOnInitialScan);   // TAR 262131
	_GET_FLG(State, ForceTrxStartOnSwipe);           // TAR 275219
    _GET_FLG(State, AttndModeAllowed);
    _GET_FLG(State, AttndModeLoginBypass);
    _GET_FLG(State, AttndModeImmedInterventions);
    _GET_FLG(State, AssistMenuFromStoreModeAllowed);
	_GET_FLG(State, AssistMenuFromRAPAllowed);
	_GET_FLG(State, StartupToLaneClosed);
	_GET_FLG(State, StartupWaitForTB);

	_GET_FLG(State, SecurityDisableAllowed); // SR683
	_GET_FLG(State, SecurityDisableNoReset); // SR683

	// get common timeout values (in seconds)
	_GET_INT(TimeOut,SyncTimeout);
	_GET_INT(TimeOut,Tender);
	_GET_INT(TimeOut,StateDefault);
	_GET_INT(TimeOut,TakeCard);
	_GET_INT(TimeOut,TakeCash);
	_GET_INT(TimeOut,TakeCoins);
	_GET_INT(TimeOut,ItemTransactionResult);
	_GET_INT(TimeOut,ContinueTransaction);
	_GET_INT(TimeOut,Help);
	_GET_INT(TimeOut,Signon);
	_GET_INT(TimeOut,Signoff);
	_GET_INT(TimeOut,Attract);
	_GET_INT(TimeOut,ItemSaleInProgress);
	_GET_INT(TimeOut,EASDeactivationScreenTimer);
	_GET_INT(TimeOut,CouponSensorScreenTimer);
	_GET_INT(TimeOut,TenderBalanceScreenTimer);
	_GET_INT(TimeOut,DepositGiftCardScreenTimer);
    _GET_INT(TimeOut,CashOutSensorAudioTimer);
	_GET_INT(TimeOut,CheckBasketReminder);
	_GET_INT(TimeOut,RequestSig);
	_GET_INT(TimeOut,InitialBagItemAudio);
	_GET_INT(TimeOut,PeriodicBagItemAudio);
	_GET_INT(TimeOut,BagItemEscalationMilliSecs);
	_GET_INT(TimeOut,TakeYourItemEscalationMilliSecs); //RFC209702

	_GET_INT(TimeOut,InitialRemovingItemsAudio);
	_GET_INT(TimeOut,PeriodicRemovingItemsAudio);
	_GET_INT(TimeOut,RemovingItemsEscalationMilliSecs);
	_GET_INT(TimeOut,WalkAwayTimerAudio);
	_GET_INT(TimeOut,InitialTakeYourItemAudio);
	_GET_INT(TimeOut,RecurringTakeYourItemAudio); //RFC209702
	_GET_INT(TimeOut,SecurityServerTimer);
	_GET_INT(TimeOut,ThankYouTimerBelt);
	_GET_INT(TimeOut,InitialClearBagAreaAudio);
	_GET_INT(TimeOut,PeriodicClearBagAreaAudio);
	_GET_INT(TimeOut,TouchScreenProximityEnablesScannerMotor);
	_GET_INT(TimeOut,MotionSensorOnAttractInterval);  //RFC 265465
	_GET_INT(TimeOut,ScannerScaleOnAttractInterval); //RFC 276235
	_GET_INT(TimeOut,DisplayBagItemScreenTimer);
	_GET_INT(TimeOut,DelaySecurityNotificationMilliSecs);
	_GET_INT(TimeOut,PeriodicSecurityAudio);
	_GET_INT(TimeOut,TimeOutTransportItem);  //TAR284378
    _GET_INT(TimeOut,HeartbeatTimer);
    _GET_INT(TimeOut,BusyLeaseTimer);
	_GET_INT(TimeOut,AttendantModeTimer);
	_GET_INT(TimeOut, GloryUnitDispenseTimeout);  // Load nTimeOutGloryUnitDispenseTimeout  TAR 310660
	_GET_INT(TimeOut,ExitSuspendScreenTimer);		//RFC 294962
	_GET_INT(TimeOut,DisplayPutBagOnScaleScreenTimer);//SR085
	_GET_INT(TimeOut,DisplayIncreaseMisMatchScreenTimer);		//TAR379130/379131
	_GET_INT(TimeOut,EnableSkipBaggingButtonTimer); //SR778


	fErrorLog = false;

	// get common tracing options
	//  _GET_INT(Tracing,NumEntries);	      // load option co.nTracingNumEntries
	_GET_INT(Tracing,CurrentLevel);	  // load optn co.nTracingCurrentLevel
	_GET_INT(Tracing,FocusLevel);	      // load option co.nTracingFocusLevel
	_GET_STR(Tracing,FocusComps);	      // load option co.csTracingFocusComps
	_GET_INT(Tracing,MaxLogSize);	      // load option co.nTracingMaxLogSize
	_GET_INT(Tracing,EntriesToFlush);	  // load option co.nTracingEntriesToFlush
	_GET_FLG(Tracing,TraceOn);	        // load option co.fTraceOn
	_GET_INT(Tracing,PSAPIInterval);	  // load option co.nTracingPSAPIInterval
	_GET_FLG(Tracing,PSAPITraceStateInit);

	_GET_INT(Tracing, MillisecondsToFlush);  //THD Performance RFC
	// Enable Disable User Exits using default file. Albertsons
	_GET_FLG(UserExit, AllEnabled);	  // Allow Enable Disable of User Exits 0 = no exits on
	// 1 = any User exit enabled will be on.This allowa
	// for all exits to be turned off with one flag and
	// selected exits to be turned on.

	_GET_FLG(UserExit, 00Enabled);	  // Enable exit 00
	_GET_FLG(UserExit, 01Enabled);	  // Enable exit 01
	_GET_FLG(UserExit, 02Enabled);	  // Enable exit 02
	_GET_FLG(UserExit, 03Enabled);	  // Enable exit 03
	_GET_FLG(UserExit, 04Enabled);	  // Enable exit 04
	_GET_FLG(UserExit, 05Enabled);	  // Enable exit 05
	_GET_FLG(UserExit, 06Enabled);	  // Enable exit 06
	_GET_FLG(UserExit, 07Enabled);	  // Enable exit 07
	_GET_FLG(UserExit, 08Enabled);	  // Enable exit 08
	_GET_FLG(UserExit, 09Enabled);	  // Enable exit 09
	_GET_FLG(UserExit, 10Enabled);	  // Enable exit 10
	_GET_FLG(UserExit, 11Enabled);	  // Enable exit 11
	_GET_FLG(UserExit, 12Enabled);	  // Enable exit 12
	_GET_FLG(UserExit, 13Enabled);	  // Enable exit 13
	_GET_FLG(UserExit, 14Enabled);	  // Enable exit 14
	_GET_FLG(UserExit, 15Enabled);	  // Enable exit 15

	_GET_FLG(UserExit, 16Enabled);	  // Enable exit 16
	_GET_FLG(UserExit, 99Enabled);	  // Enable exit 99

	_GET_INT(UserExit, Int1);
	_GET_INT(UserExit, Int2);
	_GET_INT(UserExit, Int3);
	_GET_INT(UserExit, Int4);
	_GET_INT(UserExit, Int5);

	_GET_STR(UserExit, String1);
	_GET_STR(UserExit, String2);
	_GET_STR(UserExit, String3);
	_GET_STR(UserExit, String4);
	_GET_STR(UserExit, String5);

	// Start of In-Store Message Support
	_GET_FLG(Paging, GreenFlashingSendPage);		// fPagingGreenFlashingSendPage
	_GET_FLG(Paging, YellowSendPage);				// fPagingYellowSendPage
	_GET_FLG(Paging, RedSendPage);				// fPagingRedSendPage
	_GET_FLG(Paging, OutOfServiceSendPage);		// fPagingOutOfServiceSendPage
	_GET_INT(Paging, GreenFlashingElapseTime);	// nPagingGreenFlashingElapseTime
	_GET_INT(Paging, YellowElapseTime);			// nPagingYellowElapseTime
	_GET_INT(Paging, RedElapseTime);				// nPagingRedElapseTime
	_GET_INT(Paging, OutOfServiceElapseTime);		// nPagingOutOfServiceElapseTime
	_GET_INT(Paging, GreenFlashingLimitTime);		// nPagingGreenFlashingLimitTime
	_GET_INT(Paging, GreenFlashingLimitTrans);	// nPagingGreenFlashingLimitTrans
	_GET_INT(Paging, AvailabilityCount);      	// nPagingAvailibilityCount     // TAR 117805
	
	// TAR 266195 - Allow backwards compatibility for misspelled option
	int nAvailabilityCount = GetOptInt(pApp, _T("Paging"), _T("AvailibilityCount"), -1, false);
	if (nAvailabilityCount >= 0)
	{
		nPagingAvailabilityCount = nAvailabilityCount;
	}

	// End of In-Store Message Support
	// BeginCodeLocale
	_GET_FLG(Locale, IsCurrencySymbolOnEReceiptCol1);
	_GET_FLG(Locale, IsCurrencySymbolOnEReceiptCol2);
	_GET_FLG(Locale, IsTaxLine);
  //+dp185016 support glory device
  _GET_FLG(Locale, CashManagementScreen);
  _GET_FLG(Locale, CashManagementDisabledInTransaction);// TAR 370108
  _GET_FLG(Locale, AllowSoftwareUnload);
  _GET_FLG(Locale, AllowSoftwareRemoval);
	_GET_STR(Locale, CashHighList);
	_GET_STR(Locale, CashCapacity);	
	_GET_STR(Locale, CashCapacityForCoinHopper);	
	_GET_STR(Locale, CashLowListForCoinHopper);
	_GET_STR(Locale, CashValueListForCoinHopper);
	_GET_STR(Locale, CashRemoveAmount);	

  //-dp185016

  //+dp185016 generic cash management
	_GET_FLG(Locale, CashManagementDisplayCount);
	_GET_FLG(Locale, CashManagementAPPTrackCashCount);
	_GET_INT(Locale, CashAcceptorCapacity);
	_GET_INT(Locale, CoinAcceptorCapacity);
	_GET_INT(Locale, CashAcceptorHighThreshold);
	_GET_INT(Locale, CoinAcceptorHighThreshold);
  //-dp185016

	/* + SafePay cash management. */
	_GET_INT(Locale, NoteCassetteLocation);
	_GET_FLG(Locale, AllowNoteCoverRemoval);   
	_GET_FLG(Locale, AllowNoteCassetteRemoval);   
	_GET_INT(Locale, XferToCassetteMode);
	_GET_FLG(Locale, AllowBillUnload);   
	/* - SafePay cash management. */

	// Special Display Price (please read the following notes)
	_GET_FLG(Locale, CurrencySettingNotFromRegionalSetting);
	_GET_STR(Locale, CurrencySymbol);
	_GET_INT(Locale, PositiveCurrencyFormat);
	_GET_INT(Locale, NegativeCurrencyFormat);
	_GET_STR(Locale, DecimalSymbol);
	_GET_INT(Locale, NoOfDigitsAfterDecimal);
	_GET_STR(Locale, DigitGroupingSymbol);
	_GET_INT(Locale, NoOfDigitsInGroup);

	//+4.2 card dispenser
	_GET_FLG(Operations, ManageCardDispenser);  
	_GET_INT(Operations, CardDispenserBinCapacity);
	_GET_INT(Operations, DispenserRejectBinNearFull);
	_GET_INT(Operations, DispenserRejectBinFull);
	//-4.2 card dispenser
	//get message and SASIACTION files
	_GET_STR(Data, MessageFile);
	_GET_STR(Data, SASIActionFile);
	_GET_STR(Data, TenderFile);
	_GET_STR(Data, TareFile);
// NEW UI
	_GET_STR(Data, XMLFile);
// END NEW UI


	_GET_STR(PrintObjects, ChitBmp1);
	_GET_STR(PrintObjects, ChitBmp2);

	// consumer marketing or reward information options
	_GET_FLG(CM, RewardInfo);
	_GET_INT(CM, CustomerMessage);
	_GET_FLG(CM, TotalSavings);
	_GET_FLG(CM, TotalRewardPoints);
	_GET_FLG(CM, TotalSavingsOnLeft);

    _GET_FLG(MultiLabelBarCodes, PrependFirstPrefixToItemCode); //TAR 266075

    _GET_FLG(AutoRun, On);
    _GET_FLG(AutoRun, BypassBagging);
    _GET_INT(AutoRun, NumberOfTransactionsToRun);
    _GET_FLG(AutoRun, RandomTimer);
    _GET_INT(AutoRun, AttractDelay);
    _GET_INT(AutoRun, ItemPurchaseDelay);
    _GET_INT(AutoRun, NumberOfItemsToSell);
    _GET_INT(AutoRun, TotalCashToPay);
    _GET_STR(AutoRun, ItemCodeToSell);
    _GET_FLG(AutoRun, AssistModeOn);
    _GET_STR(AutoRun, AssistKeyList);

	_GET_INT(Locale, ShortDateOrdering);  // LOCALE_IDATE	  Short date format-ordering specifier
	_GET_INT(Locale, NoCenturyDigit);	 // LOCALE_ICENTURY   Specifier for full 4-digit century.
	_GET_STR(Locale, DateSeparator);	  // LOCALE_SDATE	  Character(s) for the date separator.
	_GET_STR(Locale, ShortDateFormat);	// LOCALE_SSHORTDATE Short date formatting string for this locale.

	_GET_FLG(Locale, DateSettingNotFromRegionalSetting);

	// Others
	_GET_STR(Locale, SIntlSymbol);			// Not used yet

	_GET_STR(Locale, CashValueList);		// Possible Cash Value of hopper and cassette
	//	_GET_STR(Locale, CashSafeList);			// Minimum thresholds - no longer used
	_GET_STR(Locale, CashLowList);			// Low thresholds for the green blinking
	_GET_STR(Locale, CashLowListForNewEnhancedCoinLowSensor);
	_GET_STR(Template, Keyboard);
	_GET_STR(Template, AssistMode);
	//Begin Tar 210104
	_GET_STR(Template, AlphaNumeric);
	//End tar 210104
	_GET_INT(Template, PasswordEntryKeypad);    // FZ 10102002, Alphanumberic Password
	_GET_INT(Template, UserIDEntryKeypad);
	_GET_STR(State, RapInitiateTransaction); // RFC 228102

	_GET_FLG(Utility, ExitButtonVisible); 
    _GET_STR(Utility, ExitPassword); 
    _GET_STR(Utility, ExitID); 
	_GET_STR(Utility, DiagID); 
    _GET_STR(Utility, DiagPassword); 
    _GET_STR(Utility, RebootID); 
    _GET_STR(Utility, RebootPassword); 
    _GET_STR(Utility, ShutdownID); 
    _GET_STR(Utility, ShutdownPassword); 
	_GET_STR(Utility, VolumeID); 
    _GET_STR(Utility, VolumePassword); 
    _GET_STR(Utility, TrainingDocument);
    _GET_STR(Utility, InternetExplorerEXE);

    _GET_FLG(State, TrainingProgramButtonVisible);

	//+4.2 provide subtle on-screen volume control
	_GET_FLG(Operations,ShowVolumeControl);	
	_GET_INT(Operations,VolumeControlIncrementLevel);			      	
	_GET_INT(TimeOut, VolumeControlTimer);
	//-4.2 provide subtle on-screen volume control

	//USSF START
    _GET_FLG(Operations, EnableUSSF);
	//USSF END

	// SR730 RFC380486
	_GET_STR(Operations, TransitionToScanAndBagContexts);
	ParseTransitionContexts();

	_GET_INT(TimeOut, SingleLanguageButtonAtAttractTimer);//RFQ SR681
	//+RFC 334227
	_GET_FLG( Operations,PrintDuplicateMsgonPrintLastReceipt );  
	trace(L0,_T("PrintDuplicateMsgonPrintLastReceipt = %d"), fOperationsPrintDuplicateMsgonPrintLastReceipt );
	//-RFC 334227
	_GET_FLG(Operations, ForceDelayedApprovalBeforeSuspend);
	trace(L0, _T("ForceDelayedApprovalBeforeSuspend = %d"), fOperationsForceDelayedApprovalBeforeSuspend);

	//SR81 begin
	_GET_FLG(Operations, ImmediateInterventionForRestrictedItems);
	_GET_FLG(Operations, ImmediateInterventionForVisualVerifyItems);
	//SR81 end
	
	//+SR719
	_GET_FLG(Operations, VolumeControlMuteBypass);
	//-SR719

	//+SR713
	_GET_FLG(Operations,SellBagsAtFinalize);
	_GET_STR(Operations,SellBagsItemCode);
	_GET_STR(Operations,BagPrice);
	_GET_FLG(Operations,RepeatSellBags);
	//-SR713 

	//+RFC 368222
    _GET_STR(Operations, ReceiptFileFormat);	  // load option co.csOperationsReceiptFileFormat
    trace(L0, _T("ReceiptFileFormat = %s"), csOperationsReceiptFileFormat);
	//-RFC 368222 

    _GET_STR(Operations, HeadCashierContext);	  
    trace(L0, _T("HeadCashierContext = %s"), csOperationsHeadCashierContext);

//	pApp->m_pszProfileName = csAppProfile;
	pApp->m_pszProfileName = 	pszAppProfile;

	return 0;
}

void ConfigObject::PullMovieFile( CString& csMovieFileName )
{
	if (csMovieFileName.GetLength())
	{
		if (0 == csMovieFileName.CompareNoCase(_T("N")))
			csMovieFileName.Empty();					//	clear name if it is "N"
		else
		{
			CString csDestNameWithPath = csConfigFilePath + _T("\\") + csMovieFileName;
			TBRC nTBCallStatus = tb.CopyFileFromServer(csMovieFileName,csDestNameWithPath,false);	// destination path is null
			CFileFind FileSearchObj;
			if (!FileSearchObj.FindFile(csDestNameWithPath))
			{
				// movie file doesn't exist, log it, don't load it and don't show it
				ScotError(INFO,SM_CAT_CONFIGURATION, SCOTAPP_PS_FILE, _T("%s"),csDestNameWithPath); // event log
				csMovieFileName.Empty();
				// trace it
			}
			else
				csMovieFileName = csDestNameWithPath;				// attract movie file exists,

			FileSearchObj.Close();
		}
	}
}

CString* ConfigObject::getMoviePlayOnAttract( void )	// return correct movie based on takeawaybelt or not
{
	if( !m_pcsMoviePlayOnAttract )						// then do run-once code
	{
		HKEY hKey;
		m_pcsMoviePlayOnAttract = &csStateMoviePlayOnAttract;	// default to non-takeawaybelt movie

		if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Devices\\TakeawayBelt\\TakeawayBelt"),
					 0, KEY_QUERY_VALUE, &hKey))
		{
			// Get the configuration value
			DWORD dwValueSize=10;
			_TCHAR  szValue[11];
			if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("Configure"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
			{
				if(szValue[0]==_T('Y') || szValue[0]==_T('y'))
					m_pcsMoviePlayOnAttract = &csStateMoviePlayOnAttractBelt;// update movie file pointer
			}
			::RegCloseKey(hKey);								// close registry if open succeeded
		}
	}
	return m_pcsMoviePlayOnAttract;								// return pointer to (possibly 0 lentgh) string
}

/**
 * The CustomerBagAllowed option is not valid for TAB systems.  This 
 * method disables the CustomerBagAllowed option if the system is 
 * TAB.
 * \param[in] bIsTAB - If true, this is a TAB system.  Otherwise non-TAB.
 * \note Method added for TAR 393731.
 */
void ConfigObject::InitCustomerBagAllowed(bool bIsTAB)	
{
	// If caller indicates that this is a TAB system and
	// CustomerBagAllowed is Y.  Then log a warning and disable the option.
  if(bIsTAB && fOperationsCustomerBagAllowed)
  {
	  ScotError(INFO, SM_CAT_CONFIGURATION, BAGBOX_CONFIG_ERROR);			  
	  fOperationsCustomerBagAllowed = false;
  }

  // Moved from ConfigObject initialization.
  if(! fStateDualLanguage)
  {
	  fStateSingleLanguageButtonAtAttract = false;
  }
  else if(fOperationsCustomerBagAllowed)
  {
	  fStateSingleLanguageButtonAtAttract = true;
  }
}

//***************************************************************************************//
// Function:		ParseTransitionContexts                                              //
//                                                                                       //
// Purpose:			SR730 RFC380486 - Iterates through a comma separated context list    //
//                  found under the TransitionToScanAndBagContexts option under          //
//                  scotopts.xxx. Loads the transition context list container with these //
//                  contexts.                                                            //
//                                                                                       //
// Parameters:		None.                                                                //
//                                                                                       //
// Returns:			None.                                                                //
//***************************************************************************************//
void ConfigObject::ParseTransitionContexts()
{
	CString csBuffer = _T("");
	CString csContextsData(csOperationsTransitionToScanAndBagContexts);
	int nContextIndex = 0;
	csaTransitionContextList.RemoveAll();
	
	int nDataSize = csContextsData.GetLength();
	for(int i = 0; i < nDataSize; i++)
	{
		switch(csContextsData[i])
		{
			case _T(','):
				csaTransitionContextList.Add(csBuffer);
				nContextIndex++;
				csBuffer.Empty();
				break;
			default:
				csBuffer += csContextsData[i];
				break;
		}
	}
	
	if(csBuffer.GetLength()) // Store the last element.
	{
		csaTransitionContextList.Add(csBuffer);
	}
}



