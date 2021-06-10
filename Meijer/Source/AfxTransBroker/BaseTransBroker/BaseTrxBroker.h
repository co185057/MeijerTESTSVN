///////////////////////////////////////////////////////////////////////////
///
/// \file BaseTrxBroker.h
/// \brief Declaration file for the CBaseTrxBroker class
///
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BASETRXBROKER_H__INCLUDED_)
#define AFX_BASETRXBROKER_H__INCLUDED_

#include "transbroker.h"
// EventLogging not supported in Release 4.0 x of FastLane.  Re-enable for use with earlier release.
//#include "NCREventLogging.h"
#include "CustomerTBConnectPOS.h"

//////////////////////////////////////////////////////////////////////
///
/// CBaseTrxBroker is ...
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseTrxBroker : public CObject
{
public:
		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
    virtual ~CBaseTrxBroker();

		//////////////////////////////////////////////////////////////////////
		///
		/// launchPreScotStart is only used in _DEBUG env's. Checks option value
		///		and status flag before launching the bat file. To preserve
		///		backwards compatability, this then calls startPipeServer.
		///
		//////////////////////////////////////////////////////////////////////
    virtual void launchPreScotStart(void);  // should only be done in debug modes

		//////////////////////////////////////////////////////////////////////
		///
		/// launchPostScotStop is only used in _DEBUG env's. Checks option value
		///		and status flag before launching the bat file. To preserve
		///		backwards compatability, this then calls stopPipeServer.
		///
		//////////////////////////////////////////////////////////////////////
    virtual void launchPostScotStop(void);  // should only be done in debug modes

		//////////////////////////////////////////////////////////////////////
		///
		/// startPipeServer doesn't really start pipeserver as that should
		///		be located in the prescotstart bat file, but to preserver
		///		backwards compatibility, the name remains. This looks for
		///		the pipeserver window to ensure it gets started correctly.
		///
		//////////////////////////////////////////////////////////////////////
    virtual void startPipeServer(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// stopPipServer is only called when debug is run. This function does
		///		not actually stop anything, but rather just checks for the
		///		pipeserver window. The function name stays to preserver
		///		backwards compatibility.
		///
		//////////////////////////////////////////////////////////////////////
    virtual void stopPipeServer(void);


// EventLogging not supported in Release 4.0 x of FastLane.  Re-enable for use with earlier release.
//    virtual void logEvent ( EventClassEnum eventClass, EventTypeEnum  eventType, 
//        WORD eventCategory, ULONG eventID, WORD stringCount = 0,...);

    /******************************/
    //  TBAPI's
    /******************************/
		//////////////////////////////////////////////////////////////////////
		///
		/// TBCancelTransaction, Fastlane needs to cancel the transaction.
		///	This is a TBAPI where it the exposed interface points with Scotapp
		///
		///	@param BOOL fOverride - use override if needed (TRUE/FALSE)
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBCancelTransaction(const BOOL fOverride);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBCopyFileFromServer, Fastlane requests TB copy a file from the server
		///		to the Fastlane client terminal.
		///
		///	@param LPCTSTR szFileName - file name of file to be copied
		///	@param LPCTSTR szDestinationFilePath - complete path and filename of destination
		///	@param BOOL fForce - TRUE = force copy, FALSE = copy only if date or size diff
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBCopyFileFromServer(LPCTSTR szFileName, 
                                      LPCTSTR szDestinationFilePath, const BOOL fForce);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBCopyFileToServer, Fastlane requests TB copy a file from the client
		///		machine to the server.
		///
		///	@param LPCTSTR szFileName - complete filename (with path) of file to be copied
		///	@param LPCTSTR szSourceFilePath - complete path and filename of source location
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBCopyFileToServer(LPCTSTR szFileName, LPCTSTR szSourceFilePath);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBEnterDOB, Inform host application the Customer’s Date of Birth if needed
		///
		///	@param int nYear - valid year of customer DOB
		///	@param int nMonth - valid month of customer DOB
		/// @param int nDay - valid day of customer DOB
		/// @param int nCustomerAge - calculated customer age
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBEnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSendWeight, Inform host application of the weight currently on scale.
		///		This is only called while in assist mode when FL detects
		///		weight on the scale and the last return was TB_NEEDWEIGHT
		///
		///	@param long lWeight - the weight on the scale
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSendWeight(long lWeight);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBEnterItem, Called by FL to sell or void an item.
		///
		///	@param LPCTSTR szItemCode - item code as keyed in or extracted from barcode if 
		///		                     items is scanned
		///	@param BOOL fVoid - TRUE:void item, FALSE:sell item
		///	@param BOOL fOverride - TRUE: use override if needed; FALSE: no override
		///	@param long nQuantity - Quantity of the items to be entered into the transaction. 
		///													For non-quantity-required items, use the value 0.
		///	@param long nWeight - Weight associated with each item
		///	@param long nTareCode - Tare code associated with each item. TB calculates the 
		///													weight associated with this tare code automatically.
		///	@param long lPrice - Price of each item.
		/// @param BOOL fQuantityConfirmed - Used if nQuantity is above confirmation limit.
		/// @param SEnterItemDetails psMoreDetails - structure containing lUnitPrice, scan type
		///																					and multi-label barcode data
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBEnterItem(LPCTSTR szItemCode, const BOOL fVoid, 
                           const BOOL fOverride, const long nQuantity, 
                           const long nWeight, const long nTareCode, const long lPrice, 
						   const BOOL fQuantityConfirmed, PSEnterItemDetails psMoreDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBEnterTender, Called by FL to enter or void a tender. For additional info
		///                 about PIN, see ADK on SPINWorkingKey and TBPINENCODING
		///
		///	@param TBTENDERTYPE nTenderType - tender type from enum (TB_TENDER_CASH, etc)
		///	@param LONG lAmount - Amount of tender
		///	@param LPCTSTR szCardData - Card track data - read ADK for exact format
		///	@param int nPINLength - Length of the encrypted PIN block. 
		///	@param LPCTSTR szEncryptedPIN - Encrypted PIN block.
		///	@param BOOL fOverride - TRUE: use override if needed; FALSE: no override
		///	@param BOOL fVoid - TRUE: void tender; FALSE: enter tender
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBEnterTender(TBTENDERTYPE nTenderType, const LONG lAmount, 
                     LPCTSTR szCardData, const int nPINLength, LPCTSTR szEncryptedPIN,
					 const BOOL fOverride, const BOOL fVoid);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBEPSStartOfDay, FL requests TB perform a Start of Day - mostly used in
		///                   IBM TB's.
		///
		///	@param LPVOID lp - Data, the actual type of data depends on the type of implementation
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBEPSStartOfDay(LPVOID lp);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBEPSEndOfDay, FL requests TB perform an End of Day - mostly used in
		///                 IBM TB's.
		///
		///	@param LPVOID lp - Data, the actual type of data depends on the type of implementation
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBEPSEndOfDay(LPVOID lp);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBFinish, FL requests TB end the current sales session. TB_TRXEND state 
		///            returned by TB tells FastLane to call this function.
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBFinish(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGet2x20, Get 2x20 display message. This was originally used by FL to 
		///		         get the assist mode display when IBM only had a 2x20 display.
		///            This is either not used or hacked in most non-IBM TB's.
		///
		///	@param LPCTSTR szDisplayStr - populate this with the current 2x20 display
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGet2x20(LPCTSTR szDisplayStr);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetItemDetails, FL requests TB to populate a structure with pertinent
		///                    item details. This will be called with FL receives
		///                    TB_ITEMSOLD, or any other item sale related state event
		///                    (TB_NEEDQUANTITY, TB_NEEDWEIGHT, etc)
		///
		///	@param PSItemDetails psItemDetails - populate this struct with the item details
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetItemDetails(PSItemDetails psItemDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetItemExtendedDetails, Get extended detailed information on the last 
		///             item in the transaction. After confirming that the TB supports 
		///             this function, with TBIsAllowed, the application should call 
		///             this API when it receives TB_ITEMSOLD or any other item sale 
		///             related state event from TB.
		///
		///	@param PSItemExtendedDetails psItemExtendedDetails - populate this struct with the details
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetItemExtendedDetails(PSItemExtendedDetails psItemExtendedDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetTransactionLimit, A way to put limit on FastLane transaction total. 
		///              If TB does not want to put a limit on the transaction total, 
		///              it can return a very high value (Base TB returns 999999999)
		///              FastLane App calls this function to display the transaction 
		///              limit on the Attract screen.
		///
		///	@return long - value of transaction limit
		//////////////////////////////////////////////////////////////////////
    virtual long TBGetTransactionLimit(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetQuantityLimit, Get the quantity threshold per transaction. This is 
		///      a threshold that causes the system to ask the user for confirmation.
		///
		///	@return long - quantity threshold for user confirmation
		//////////////////////////////////////////////////////////////////////
    virtual long TBGetQuantityLimit(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetJournalData, Return the file handle of a file that contains journal 
		///        presentation data. If TB wants to print some transaction data on 
		///        the journal printer attached to the FastLane terminal, this 
		///        function should return a valid handle to a file that contains journal 
		///        data. The data is transparent to FastLane and is submitted to the 
		///        journal without any modifications. The journal is printed either on 
		///        request by TB (synchronous print request: TB_COMPLETEPRINT state) 
		///        or at the end of each transaction.
		///
		///	@return HFILE  - handle of a file (NULL = no journal data)
		//////////////////////////////////////////////////////////////////////
    virtual HFILE TBGetJournalData(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetPINWorkingKey, Get the PIN working key from the Host Credit module.
		///       FastLane Application creates a pin-block using this information and
		///       using the pin-block for PIN encryption/decryption.
		///
		///	@param PSPINWorkingKey psPINWorkingKey - Pointer to the SPINWorkingKey data structure.
		///       The application allocates memory for this structure. The TB function 
		///       fills in the parameter
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetPINWorkingKey(PSPINWorkingKey psPINWorkingKey);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetReceiptData, Return the file handle of a file that contains receipt 
		///        presentation data. If TB wants to print some transaction data on 
		///        receipt printer attached to FastLane terminal, this function should 
		///        return a valid handle to a file that contains receipt data. The 
		///        data is transparent to FastLane and is submitted to receipt printer 
		///        without any modifications. Receipt is printed either on request by 
		///        TB (synchronous print request: TB_COMPLETEPRINT state) or at the 
		///        end of each transaction.
		///
		///	@return HFILE  - handle of a customer receipt file (NULL = no receipt data)
		//////////////////////////////////////////////////////////////////////
    virtual HFILE TBGetReceiptData(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetSlipData, Return the file handle of a file that contains signature 
		///        slip presentation data.
		///
		///	@return HFILE  - handle of signature slip presentation fata (NULL = no data)
		//////////////////////////////////////////////////////////////////////
    virtual HFILE TBGetSlipData(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetTenderDetails, Get detailed information on the last tender in the 
		///                      transaction. The last tender can be accepted or declined 
		///                      (TB_TENDERACCEPTED or TB_TENDERDECLINED).
		///
		///	@param PSTenderDetails psTenderDetails - Pointer to the STenderDetails data structure
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetTenderDetails(PSTenderDetails psTenderDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetTerminalNumber, Return POS client terminal number. FastLane assumes 
		///                       positive terminal numbers only. Permits negative
		///                       numbers to be used to return error status.
		///
		///	@return long - terminal number
		//////////////////////////////////////////////////////////////////////
    virtual long TBGetTerminalNumber(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetCurrentTransactionNumber, The long value returned should be the 
		///            transaction number as determined by the POS. If the function 
		///            is not implemented inside of the transaction broker, then the 
		///            number -9999 is used for the transaction number. This number is 
		///            used solely for reporting purposes. The transaction number is 
		///            recorded in the reports. Base TB sets this to 0 and does not
		///            increment it!
		///
		///	@return long - transaction number
		//////////////////////////////////////////////////////////////////////
    virtual long TBGetCurrentTransactionNumber(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetTotalDetails, Get detailed information on the total in the transaction. 
		///                     FastLane calls this function after receiving TB_ITEMSOLD 
		///                     or TB_TOTAL state event.
		///
		///	@param PSTotalDetails psTotalDetails - Pointer to the STotalDetails data structure
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetTotalDetails(PSTotalDetails psTotalDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetLoyaltyCardNumber, Passes the loyalty card number information to 
		///       the FastLane Core Application. Called when TB_LOYALTYCARD is returned
		///       by the TB.
		///
		///	@param LPTSTR *pszLoyaltyCardNumber - ptr to the loyalty card number
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetLoyaltyCardNumber(LPTSTR *pszLoyaltyCardNumber);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBParseFoodStampTotal, Function to extract the Food Stamp Total of the purchase.
		///         At the end of a transaction, get the total that can be paid by food 
		///         stamps. First call TBRequestFoodStampEligibleTotal() which asks the host 
		///         system to send that the amount paid using food stamps. This function 
		///         TBParseFoodStampTotal() is then used to parse or extract the amount due.
		///
		///	@param PSTotalDetails psTotalDetails - A PSTotalDetails structure, one element of
		///                                  which holds the food stamp total
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBParseFoodStampTotal(PSTotalDetails psTotalDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetVersion, Return the version number and information of the TB.
		///                Version string conforms to the below format:
		///                  "FastLane TB Ver xx.xx.xx for Solution Name"
		///                This should be set using the CBaseCurrentOptions::SetVersionString
		///                function. Base TB does not currently set this.
		///
		///	@param LPTSTR szVersionString - Buffer to contain the version string
		///	@param long nBufferLength - Buffer length
		///	@return long - Length of the actual version string, including the NULL 
		///								character. If this is bigger than nBufferLength then the 
		///								buffer passed is not long enough to copy the entire version 
		///								string, but nBufferLength characters are still copied into 
		///								the buffer.
		//////////////////////////////////////////////////////////////////////
    virtual long TBGetVersion(LPTSTR szVersionString, long nBufferLength);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBIsAllowed, The application can use this function to query TB to see 
		///       whether or not a certain feature is allowable in the current state.
		///
		///	@param TBFEATURE nFeature - feature (TBFEATURE enum - ex: TB_FT_TRAININGMODE)
		///	@return BOOL - TRUE:permitted; FALSE:not permitted
		//////////////////////////////////////////////////////////////////////
    virtual BOOL TBIsAllowed(TBFEATURE nFeature);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBKeyPress, Send a key code. This is very useful in supporting the full 
		///              Store Mode POS keyboard that mimics the host application’s 
		///              functionality. This function is mainly used in the Assist Mode.
		///
		///	@param USHORT uKey - ASCII key value
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBKeyPress(const USHORT uKey);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBLoadOptions, FastLane App calls this function immediately after calling 
		///       TBInitialize() to request the host application to load its terminal 
		///       options. The host application normally loads its terminal options at 
		///       the start of the application. This function is provided so that while 
		///       the application is still running, you can still choose to reload options.
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBLoadOptions(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBLogErrorToServer, Log an error to the server. This feature may not be 
		///       available in all solutions. If the solution does not support this 
		///       feature, this function should always return TB_SUCCESS. 
		///
		///	@param LPCTSTR szErrorText - Full error text
		/// @param int nSeverity - Error severity, 1-5 (with 1 being the highest)
		///	@param int nComponentID - FastLane Component ID number
		///	@param LPCTSTR szComponentID - Component ID string, TS, PS
		///	@param LPCTSTR szModuleID - Module ID string
		///	@param int nModuleLineNumber - Module line number
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBLogErrorToServer(LPCTSTR szErrorText, const int nSeverity,
						   const int nComponentID, LPCTSTR szComponentID,
						   LPCTSTR szModuleID, const int nModuleLineNumber);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBRefreshState, Intended to refresh the current state of the host 
		///       application. This generates a display notification event, at which 
		///       time the application can call TBProcessMessage to determine the state 
		///       of the host application. Base TB does not implement this yet!
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBRefreshState(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBReleaseJournalData, Release the presentation data for the journal printer.
		///
		///	@param HFILE - Handle to journal data file
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBReleaseJournalData(HFILE hJournalData);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBReleaseReceiptData, Free the presentation data for the customer receipt.
		///
		///	@param HFILE - Handle to Receipt Data file
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBReleaseReceiptData(HFILE hReceiptData);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBReleaseSlipData, Free the presentation data for the signature slip.
		///
		///	@param HFILE - Handle to Signature Data file
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBReleaseSlipData(HFILE hSlipData);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBResumeTransaction, Resume the transaction suspended. This is always
		///       going to be solution specific, so no base implementation is done.
		///
		///	@param LPCTSTR szSuspendID - Suspended transaction ID
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBResumeTransaction(LPCTSTR szSuspendID);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSendMsg2Host, If none of the existing TB APIs address your need to 
		///       communicate with the host, use this function in SCOTSSF (FastLane 
		///       Solution Specific Functionality module) implementation. By default, 
		///       this function is not called by the FastLane application. This is 
		///       not required if TB has an extended set of TB APIs exposed to use 
		///       by SCOTSSF. This is not used in Base TB.
		///
		///	@param LPCTSTR szSend - Message to send/write, NULL for DWO reads
		///	@param int iMsgLen - message length
		///	@param TBDEVICEID nDeviceClass - Device ID
		///	@param UCHAR nFunctionCode - Function code
		///	@param long  lOptFileHandle - Optional handle for file DWO file I/O operations
		///	@param bool fOriginatePipeMsg - 
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSendMsg2Host(LPCTSTR szSend, const int iMsgLen,
					const TBDEVICEID nDeviceClass, const UCHAR nFunctionCode = 0,
					const long	lOptFileHandle = 0,	const bool fOriginatePipeMsg = true);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSetMonitorAll, TBAPI called to request TB track all display msgs
		///
		///	@param BOOL fMonitor - TRUE = entering assist mode, FALSE = exiting
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSetMonitorAll(BOOL fMonitor);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBStart, TBAPI called to start a transaction. FL should call this API
		///       before passing on any trx data. Violations of this agreement should
		///       be opened as TAR's against FL. TB can return TB_SUCCESS_IMMEDIATE to
		///       start a trx right away, or it can return TB_SUCESS which will cause 
		///       FL to wait for a TB_READY TBSTATE.
		///
		///	@param LPCTSTR szOverrideCode - The 2 character string denoting the ISO639
		///       language code for language that the customer has selected to use in 
		///       the transaction. NULL if dual language is not supported.
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBStart(LPCTSTR szOverrideCode);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSuspendTransaction, Suspend the current sales transaction.
		///
		///	@param BOOL fOverride - TRUE: use override if needed; FALSE: no override
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSuspendTransaction(const BOOL fOverride);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBValidateOperator, If this is a valid Operator and PW then return TRUE.
		///       Base TB does not have an implementation for this - solution specific.
		///
		///	@param LPCTSTR szOperatorID - Operator ID
		///	@param LPCTSTR szOperatorPw - Operator password
		///	@return BOOL - TRUE if a valid cashier and PW, FALSE otherwise
		//////////////////////////////////////////////////////////////////////
    virtual BOOL TBValidateOperator(LPCTSTR szOperatorID, LPCTSTR szOperatorPw);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBValidOperator, Validate Operator - not believed to be in use by FL
		///
		///	@param LPCTSTR szOperatorID - Operator ID
		///	@return BOOL - TRUE if a valid cashier, FALSE otherwise
		//////////////////////////////////////////////////////////////////////
    virtual BOOL TBValidOperator(LPCTSTR szOperatorID);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBValidPassword, Validate Password - not believed to be in use by FL.
		///
		///	@param LPCTSTR szOperatorPw - Operator PW
		///	@return BOOL - TRUE if a valid password, FALSE otherwise
		//////////////////////////////////////////////////////////////////////
    virtual BOOL TBValidPassword(LPCTSTR szOperatorPw);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBVerifyLoyaltyCard, Return true if the card data is of a loyalty 
		///       type card. If TB returns LC_VALID, then the FL will go into a 
		///       processing state, awaiting the TB_LOYALTYCARD or TB_LOYALTYINVALID
		///       return.
		///
		///	@param LPCTSTR szCardData - Card information, in format L1L2L3T1T2T3, 
		///       where L1, T1 are the track length and track data of track 1; L2, T2 are 
		///       for track 2; and L3, T3 are for track 3. Track lengths are declared as short integers.
		///	@param int nDataLen - data length
		///	@return TBLOYALTYCARDTYPE - - One of the following: LC_INVALID, LC_VALID, 
		///	       or LC_INSERTED_BEFORE
		//////////////////////////////////////////////////////////////////////
    virtual TBLOYALTYCARDTYPE TBVerifyLoyaltyCard(LPCTSTR szCardData, int nDataLen);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetAvailableFoodStampAmount, Function used to get the dollar amount of 
		///       Food Stamps remaining on a Food Stamp card that could be used against 
		///       a purchase once it has been determined that the balance on the card 
		///       is insufficient to pay for all food-stamp-eligible items. After 
		///       TB_TENDERFOODSTAMPINSUFFICIENT is received, this function is called 
		///       to find how much balance is remaining on the card. The user is informed 
		///       about insufficient funds and given a chance to cancel payment with 
		///       the Food Stamp card.
		///
		///	@param PSEBTAvailBalance psEBTAvailBalance - Contains the current balance on that 
		///       food stamp account
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetAvailableFoodStampAmount(PSEBTAvailBalance psEBTAvailBalance);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetAvailableBenefitAmount, Function used to get the dollar amount of 
		///       Cash Benefit remaining on a Cash Benefit card that could be used 
		///       against a purchase. After FastLane gets TB_TENDERCASHBENEFITINSUFFICIENT, 
		///       the application calls this function to find how much is remaining 
		///       on the card. The user is informed about insufficient funds and is given 
		///       a chance to cancel the payment with the benefit card.
		///
		///	@param PSEBTAvailBalance psEBTAvailBalance
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetAvailableBenefitAmount(PSEBTAvailBalance psEBTAvailBalance);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBRequestFoodStampEligibleTotal, Function used to request that the host 
		///       send the dollar amount that could be paid with Food Stamps. FL will
		///       follow up with a call to TBParseFoodStampEligibleTotal().
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBRequestFoodStampEligibleTotal();

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSendSignOn, FL requests an operator signon to the POS system. Not
		///       used currently in base FL app.
		///
		///	@return BRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSendSignOn(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSendSignOff, FL requests an operator signoff to the POS system. Not
		///       used currently in base FL app.
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSendSignOff(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSendLockOn, Do a manager key-lock on. Not currently used in core app
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSendLockOn(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSendLockOff, Do a manager key-lock off. Not currently used in core app
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSendLockOff(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBInitHostAppExit, Initiate host application exit procedures or sequences.
		///       This action is initiated by the FastLane user (can be in store mode). 
		///       This API exists to help solution-specific exit procedures.
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBInitHostAppExit(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBLaneClosed, Informs TB when FastLane enters or leaves Lane Closed screen.
		///       Appropriate TB action can be solution-specific. This is not a request
		///       from FL, but a statement of fact.
		///
		///	@param bool bEnter - Flag indicating whether entering (TRUE) or exiting (FALSE)
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBLaneClosed(const bool bEnter);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBEnterTenderState, Informs TB when FastLane enters or leaves the tender 
		///       state. Appropriate TB action can be solution-specific. This is not 
		///		    a request from FL, but a statement of fact.
		///
		///	@param bool bEnter - Flag indicating whether entering (TRUE) or exiting (FALSE)
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBEnterTenderState(const bool bEnter);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSendSigData, Sends captured signature data to TB if the SendSignatureDataToTB
		///       option in SCOTOPT.DAT is set to Y and the SignatureCapture option is
		///	      set to Y. Data is in 5992 format.
		///
		///	@param LPCTSTR CustomerName - The customer’s name
		///	@param LPCTSTR cSigData - The signature data in 5992 format
		/// @param DWORD nLength - The length of the signature data
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSendSigData(LPCTSTR CustomerName, LPCTSTR cSigData,  DWORD nLength);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSetTrainingModeState, FastLane App calls this function in order to enter 
		///       or leave training mode. After returning TB_SUCCESS to this call, TB 
		///	      should post a message to FastLane App to return either :
		///		    TB_ENTEREDTRAINING state if bTMState is true or
		///	      TB_LEFTTRAINING state if bTMState is false.
		///	
		///	@param bool bTMState - true: enter training mode; false: exit training mode
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSetTrainingModeState(const bool bTMState);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetRewardLineDetails, When TB returns TB_REWARDLINE, FL calls this
		///       function to collect the details of that reward line.
		///
		///	@param PSRewardLineDetails psRewardLineDetails - the reward line details struct
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetRewardLineDetails(PSRewardLineDetails psRewardLineDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBPrint, Permits FastLane App to send print requests to TB when TB 
		///       controls the printer
		///
		///	@param LPCTSTR szPrintFile - Fully qualified name of the file to be printed,
		///	       or NULL to tell TB to resume printing from the last error.
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBPrint(LPCTSTR szPrintFile);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSuspendTransactionEx, Suspend a sales transaction, and sends a list
		///       of suspend reasons to TB
		///
		///	@param BOOL fOverride - TRUE: use override if needed; FALSE: no override
		///	@param LPCTSTR szSuspendReasons - List of suspend reasons if any to be
		///	       appended to the receipt
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSuspendTransactionEx(const BOOL fOverride, LPCTSTR szSuspendReasons);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetPrinterError, Gets descriptive Printer error message. The FL 
		///       application calls this when it receives TB_PRINTER_VERIFY to get 
		///       the message to be displayed for the current printer error
		///	
		///	@param LPTSTR* psErrorMessage - Pointer to the buffer containing the message
		///	       string to be displayed
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetPrinterError(LPTSTR* psErrorMessage);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBEnterMICR, This function is called by the FastLane App to report 
		///       information retrieved from the MICR document. If MICR data could 
		///       not be retrieved, the parameter szRawData and other string parameters 
		///	      can be an empty string and the value of the long parameters is zero.
		///
		///	@param LPCTSTR szRawData - The raw MICR data from the MICR reader device
		///	@param LPCTSTR szTransitNumber - The bank’s transit (routing) number
		/// @param LPCTSTR szAccountNumber - The customer’s account number
		///	@param LPCTSTR szBankNumber - The bank’s ABA bank number
		///	@param LPCTSTR szSerialNumber - The sequence (check) number of the check
		///	@param LPCTSTR szEPC - The Extended Processing Code (EPC) on the check
		///	@param long lType - The type of check:
		///	       1  personal check
		///	       2  Business or Commercial check
		///	       99 Unknown check type
		///	@param long lCountryCode - The country code of the check:
		///	       1  United States check
		///        2  Canadian check (Canada)
		///	       3  Mexican check (Mexico)
		///	       99 Unknown check origination
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBEnterMICR(LPCTSTR szRawData, LPCTSTR szTransitNumber,
                    LPCTSTR szAccountNumber, LPCTSTR szBankNumber,
                    LPCTSTR szSerialNumber, LPCTSTR szEPC, long lType,
                    long lCountryCode);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetDocEndorsementInfo, This function is called by the FastLane application 
		///       to retrieve information about the next document to be endorsed. This 
		///       function must be called in response to a TB_ENDORSEDOCUMENT message
		///       from TB; however, it may be called at anytime.
		///
		///	@param long *plNumRemaining - Number of endorsements remaining
		///	@param LPTSTR *ppszDocDescription - Operator prompt or document description
		///	@param LPTSTR *ppszDocFront - Text to print on front of the document
		///	@param LPTSTR *ppszDocBack - Text to print on back of the document
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetDocEndorsementInfo(long *plNumRemaining, LPTSTR *ppszDocDescription,
                    LPTSTR *ppszDocFront, LPTSTR *ppszDocBack);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSetCashDrawerState, This is called by the FastLane App in response to 
		///       the state TB_NEWCASHDRAWERSTATE or when a cash drawer state changes.
		///
		///	@param long lState - A bitmapped field. The drawer should be opened if the bit 
		///	       is set. The drawer is closed or not present when the bit is cleared.
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSetCashDrawerState(long lState); 

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetExpectedCashDrawerState, This function is called when the FastLane 
		///       application needs identity of the state that TB expects the cash 
		///	      drawer to be. It can be called by the FastLane application at any 
		///       time after TB is initialized.
		///
		///	@param long *plExpectedState - A pointer to a bitmapped field. The drawer
		///       should be opened if the bit is set. The drawer is closed or not 
		///       present when the bit is cleared.
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetExpectedCashDrawerState(long *plExpectedState);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSetSecurityLevel, Called by the FastLane App when the security level 
		///       of the operator changes.
		///
		///	@param long lSecurityLevel - new security level from TBKEYLOCK enum
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSetSecurityLevel(long lSecurityLevel);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetExpectedSecurityLevel, Called by the FastLane application so that 
		///       it can learn what security level the TB currently wants to see.
		///
		///	@param long *plExpectedLevel - security level from TBKEYLOCK enum
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetExpectedSecurityLevel(long *plExpectedLevel);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetCurrentSecurityLevel, Called by the FastLane App so that the App can 
		///       determine the TB’s current security level.
		///
		///	@param long *plExpectedLevel - security level from TBKEYLOCK enum
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetCurrentSecurityLevel(long *plCurrentLevel);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBEndorsementCompleted, This function is called by the FastLane application 
		///       for each endorsement that has been printed. FastLane calls this API
		///       once for each TB_ENDORSEDOCUMENT state returned to it.
		///
		///	@param bool bSuppressSubsequent - If the bSuppressSubsequent flag is set, TB 
		///       must not send further TB_ENDORSEDOCUMENT states to FastLane for 
		///       any endorsements currently pending.
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBEndorsementCompleted(bool bSuppressSubsequent = false);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetAssistModeStatus, FL calls this function to determine if it is allowed
		///       to enter or exit assist mode. Currently, FL does not call this to see
		///       when it can enter AM, but it is called to see if it can exit. Base
		///       TB sets this to TB_AMSTATUSMAYEXIT and does not change it.
		///
		///	@param long *plAssistModeStatus - should be set to one of the following values:
		///       TB_AMSTATUSMAYEXIT Operator may exit Assist Mode
		///       TB_AMSTATUSMAYNOEXIT Operator may not exit Assist Mode
		///       TB_AMSTATUSMAYENTER Operator may enter Assist Mode
		///       TB_AMSTATUSMAYNOTENTER Operator may not enter Assist Mode
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetAssistModeStatus(long *plAssistModeStatus);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetMICRPrompt, This function is called by the FastLane App when it 
		///       receives a TB_NEEDMICR notification from the TB. It retrieves 
		///       information requested by TB about the MICR document.
		///
		/// @param LPTSTR *ppszMICRPrompt - A pointer to a string pointer that is allocated
		///       by TB. A prompt to be displayed to the operator to insert the MICR document.
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetMICRPrompt(LPTSTR *ppszMICRPrompt);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetMessageInfo,  Get custom message information. The application calls 
		///       this when it receives TB_MESSAGESCREEN state event from TB and also 
		///       when it gets TB_E_ITEMFAILED return code from TBEnterItem().
		///
		///	@param PSMessageInfo psMessageInfo - Pointer to the SMessageInfo data structure
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetMessageInfo(PSMessageInfo psMessageInfo);  

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetCustomerMessage, This function is needed if the DisplayCustomerMessage 
		///       option, located in SCOTOPTS file, is turned on and TB wants to provide 
		///       some customer specific or transaction specific message on the Scan and Bag
		///       screen of the FastLane Application. FastLane calls this function only 
		///       when TB returns TB_CUSTOMERMESSAGE state for TBProcessMessage call.
		///
		///	@param LPTSTR* psCustomerMessageText - TB has to allocate enough memory and fill
		///       the customer message into the buffer with a null termination, permitting 
		///       FastLane App to release the memory allocated by TB for this field.
		/// @param LPTSTR* psCustomerMessageAudio - TB has to allocate enough memory and fill
		///       the customer message audio file into the buffer with a null termination, 
		///       permitting FastLane App to release the memory allocated by TB for this 
		///       field. If the path is not included in the filename, FastLane assumes 
		///       that it is the current sound files’ path of FastLane App.
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetCustomerMessage(LPTSTR* psCustomerMessageText, LPTSTR* psCustomerMessageAudio);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetDataNeededView, Get configuration view data from TB. The application 
		///       calls this when it receives a TB_NEEDMOREDATA state event from TB. 
		///       The TB uses this mechanism when it wants Fastlane to display a data 
		///       entry screen.
		///
		///	@param PSDataNeededView psDataNeededView - Pointer to the SDataNeededView data structure
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetDataNeededView(PSDataNeededView psDataNeededView);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSendDataNeeded, Send the data entry information to the TB. The application 
		///       calls this as a result of receiving user input from the view generated 
		///       by the TBGetDataNeededView function.
		///
		///	@param LPCTSTR szDataNeededDetails - the first field represents the button pressed,
		///       the second field is any numeric data entered (pinpad or scanner), the 
		///       semicolon is used as the delimiter between fields
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSendDataNeeded(LPCTSTR szDataNeededDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBCreateReportEntry
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBCreateReportEntry(PSCreateReportEntry psCreateReportEntry);
		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetCurrentBusinessDate
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetCurrentBusinessDate(CString &csBusinessDate);
		//////////////////////////////////////////////////////////////////////
		///
		/// TBSetBusinessDate
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBSetBusinessDate(LPCTSTR szSetBusinessDate);
		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetGiftCardBalance
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBGetGiftCardBalance(long *plBalance);
		//////////////////////////////////////////////////////////////////////
		///
		/// TBDisableTenderButton
		///
		///	@return BOOL - TRUE to disable, FALSE to enable
		//////////////////////////////////////////////////////////////////////
    virtual BOOL TBDisableTenderButton(TBTENDERTYPE nTenderType);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBNeedStartOfDay, Deprecated - not used
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBNeedStartOfDay(TBSTARTOFDAYSTATE &eState);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBInitialize, This is not necessarily the first API called by SCOT! Used
		///                to initialize the connection points to POS and SCOT.
		///
		///	@param HWND hWnd - handle to the SCOT application window
		///	@param LPCTSTR szFilePath - path for config files
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBInitialize(HWND hWnd, LPCTSTR szFilePath);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBUninitialize, This will be the last API called as Fastlane shuts down
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBUninitialize(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBProcessMessage, Process the message posted by TB. TB analyzes the message 
		///       and returns the state of the host application, based on the presentation 
		///       data in the message. This function will farm out the work to the 
		///       specific msg device id class to actually process the message.
		///
		///	@param PSMessageElement psMessage - Pointer to the TB message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE TBProcessMessage(PSMessageElement psMessage);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBReleaseMessage, Free the message element posted by the TB and any
		///       memory associated with it.
		///
		///	@param PSMessageElement psMessage - Pointer to the message element as passed to
		///       the application as a notification message.
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBReleaseMessage(PSMessageElement psMessage);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetNextState, ### NEED TO ADD DESC ###
		///
		///	@param LPCTSTR
		///	@param LPCTSTR
		///	@param bool
		///	@param LPTSTR
		///	@param CDescriptorList
		///	@return TBRC
		//////////////////////////////////////////////////////////////////////
    virtual TBRC GetNextState(LPCTSTR lpLastStateName, LPCTSTR lpNextStateName,
                      bool *pbOverride, LPTSTR *lpOverrideStateName, CDescriptorList &dlParameterList);

		//////////////////////////////////////////////////////////////////////
		///
		/// PreprocessSignatureData, ### NEED TO ADD DESC ###
		///
		///	@param LPCTSTR
		///	@param BSTR
		///	@param LPCTSTR
		///	@param BOOL
		/// @return TBRC
		//////////////////////////////////////////////////////////////////////
    virtual TBRC PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetExtensionStateSetup, ### NEED TO ADD DESC ###
		///
		///	@param LPTSTR
		///	@param LPTSTR
		/// @param psPropChangeRequestList
		///	@return TBRC
		//////////////////////////////////////////////////////////////////////
		virtual TBRC GetExtensionStateSetup(LPTSTR szUICtx, LPTSTR szDevCtx, psPropChangeRequestList pExtChangeList);
		
		//////////////////////////////////////////////////////////////////////
		///
		/// NotifyExtensionStateEvent,  ### NEED TO ADD DESC ###
		///
		///	@param psExtStateEventNotifyIn
		///	@param psExtStateEventNotifyOut
		///	@param pEventNotifyOut
		///	@return TBSTATE
		//////////////////////////////////////////////////////////////////////
		virtual TBSTATE NotifyExtensionStateEvent(psExtStateEventNotifyIn pEventNotifyIn, psExtStateEventNotifyOut pEventNotifyOut);

		//////////////////////////////////////////////////////////////////////
		///
		/// InAssistMode, InAssistMode - accessors for member variable
		///
		///	@return BOOL
		//////////////////////////////////////////////////////////////////////
    virtual BOOL  InAssistMode( void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetInAssistMode, SetInAssistMode - set method for member variable
		///
		///	@param BOOL
		//////////////////////////////////////////////////////////////////////
    void SetInAssistMode( BOOL);


    // not api's
		//////////////////////////////////////////////////////////////////////
		///
		/// CreateMessageObjectByMessageId, this function creates a new object
		///       based on the message id passed in as a parameter, and returns a
		///       ptr to that object.
		///
		///	@param BYTE messageId - the id of the message object to be created
		///	@return CTBMsgObj * - a ptr to the new object, NULL if error
		//////////////////////////////////////////////////////////////////////
    virtual CTBMsgObj *CreateMessageObjectByMessageId(BYTE messageId);

		//////////////////////////////////////////////////////////////////////
		///
		/// CreateMessageObjectByTBDEVICEID, this function creates a new object
		///       based on the device id passed in as a parameter, and returns a
		///       ptr to that object.
		///
		///	@param TBDEVICEID nMsgSource - the device id of the object to be created
		///	@return CTBMsgObj * - a ptr to the new object, NULL if error
		//////////////////////////////////////////////////////////////////////
    virtual CTBMsgObj *CreateMessageObjectByTBDEVICEID(TBDEVICEID nMsgSource);

		//////////////////////////////////////////////////////////////////////
		///
		/// ReleaseMessageLParam, Frees the message element 
		///
		///	@param PSMessageElement psMessage - Pointer to the message element as passed to
		///       the application as a notification message.
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC ReleaseMessageLParam(PSMessageElement psMessage);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetTBRCName, returns a string representation of the TBRC (TB_SUCCESS,etc)
		///
		///	@param long key - long value of the TBRC 
		///	@return TCHAR * - string of TBRC
		//////////////////////////////////////////////////////////////////////
    virtual LPCTSTR GetTBRCName(long key);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetTBSTATEName, returns a string representation of the TBSTATE (TB_ITEMSOLD,etc)
		///
		/// @param long key - long value of the TBSTATE (TBSTATE is an enum)
		///	@return TCHAR * - string of TBSTATE
		//////////////////////////////////////////////////////////////////////
    virtual LPCTSTR GetTBSTATEName(long key);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetTBSTATEFromName, return the long value of the TBSTATE given its
		///       string representation.
		///
		///	@param LPCTSTR szTBState - string of TBSTATE name
		///	@return long - long value of TBSTATE (from enum)
		//////////////////////////////////////////////////////////////////////
    virtual long GetTBSTATEFromName(LPCTSTR szTBState);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBGetProperty, returns a CString value from the property specified in the
		///       parameter szProperty. Specify if this is a hook property or not using
		///       the bHookProperty parameter.
		///
		///	@param LPCTSTR szProperty - name of Property to retrieve the value from
		///	@param bool bHookProperty - is this a hook property, assumed false if not specified
		///	@return CString - value retrieved from property
		//////////////////////////////////////////////////////////////////////
    virtual CString TBGetProperty(LPCTSTR szProperty, bool bHookProperty =false);

		//////////////////////////////////////////////////////////////////////
		///
		/// TBSetProperty, set the value of a Property. Specify if this is a hook 
		///       property or not using the bHookProperty parameter.
		///
		///	@param LPCTSTR szProperty - name of Property to retrieve the value from
		///	@param LPCTSTR szValue - value to set the property with
		///	@param bool bHookProperty - is this a hook property, assumed false if not specified
		//////////////////////////////////////////////////////////////////////
    virtual void TBSetProperty(LPCTSTR szProperty, LPCTSTR szValue, bool bHookProperty =false);

		//////////////////////////////////////////////////////////////////////
		///
		/// SendTBCommand, ### ADD DESC ###
		///
		///	@param long
		///	@param va_list
		///	@return TBRC
		//////////////////////////////////////////////////////////////////////
		virtual TBRC SendTBCommand(long lCommandID, va_list pArgs);

		//////////////////////////////////////////////////////////////////////
		///
		/// TriggerMsgMap, triggers parsing of the message map
		///
		//////////////////////////////////////////////////////////////////////
    virtual void TriggerMsgMap();
		
		//////////////////////////////////////////////////////////////////////
		///
		/// SetFLState, this sends a return status message to FL. FL will ask TB to
		///       process the message and TB will then return the szState param as the
		///       new TBSTATE.
		///
		///	@param LPCTSTR szState - string of the TBSTATE that TB will eventually return to FL
		///	@return long - in Base TB, always TB_SUCCESS
		//////////////////////////////////////////////////////////////////////
    long SetFLState( LPCTSTR szState/*=NULL*/ );

		//////////////////////////////////////////////////////////////////////
		///
		/// GetCurrentInProgressState, get the current InProgress TB state
		///
		///	@return long - the value of the current InProgress TB state 
		//////////////////////////////////////////////////////////////////////
    long GetCurrentInProgressState();

		//////////////////////////////////////////////////////////////////////
		///
		/// GetCurrentInProgressState, returns the long value of the InProgress TB 
		///       state of the string value passed in as a parameter - note, this is
		///       not necessarily the current InProgress state.
		///
		///	@param LPCTSTR szStateName - string value of InProgress state to be looked up
		///	@return long - the value of the InProgress TB state
		//////////////////////////////////////////////////////////////////////
    long GetCurrentInProgressState(LPCTSTR szStateName);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetCurrentInProgressState, sets the current TB InProgress state to the
		///       value passed in
		///
		///	@param long nState - value of the new TB InProgress state
		//////////////////////////////////////////////////////////////////////
    void SetCurrentInProgressState(long nState);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetCurrentInProgressState, sets the current TB InProgress state to the
		///       value passed in
		///
		///	@param LPCTSTR szStateName - string of new TB InProgress state, this will be
		///       resolved to the long value
		//////////////////////////////////////////////////////////////////////
    void SetCurrentInProgressState(LPCTSTR szStateName);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetCurrentInProgressStateName, returns the string value of the InProgress 
		///       TB state of the long value passed in as a parameter - note, this is
		///       not necessarily the current InProgress state.
		///
		///	@param long key - value of TB InProgress state to be looked up
		///	@return LPCTSTR - string of the TB InProgress state
		//////////////////////////////////////////////////////////////////////
    LPCTSTR GetCurrentInProgressStateName(long key=0);

		//////////////////////////////////////////////////////////////////////
		///
		/// StoreMode, mutator for member veriable
		///
		///	@param bool
		//////////////////////////////////////////////////////////////////////
    void StoreMode(bool b);

		//////////////////////////////////////////////////////////////////////
		///
		/// IsInStoreMode, accessor for member variable
		///
		///	@return bool
		//////////////////////////////////////////////////////////////////////
    bool IsInStoreMode(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetPrimaryLanguage, ### ADD DESC ###
		///
		///	@param LPCTSTR
		//////////////////////////////////////////////////////////////////////
    void SetPrimaryLanguage(LPCTSTR szPrimaryLang);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetPrimaryLanguageUsed, ### ADD DESC ###
		///
		///	@return CString
		//////////////////////////////////////////////////////////////////////
    CString GetPrimaryLanguageUsed(void);
    
		//////////////////////////////////////////////////////////////////////
		///
		/// TBValidateHeadCashier, If this is a valid Cashier and PW then return TRUE.
		///       Base TB does not have an implementation for this - solution specific.
		///
		///	@param LPCTSTR szCashierID - Cashier ID
		///	@param LPCTSTR szCashierPw - Cashier password
		///	@return BOOL - TRUE if a valid cashier and PW, FALSE otherwise
		//////////////////////////////////////////////////////////////////////
    virtual BOOL TBValidateHeadCashier( LPCTSTR szCashierID, LPCTSTR szCashierPw );


		//////////////////////////////////////////////////////////////////////
		///
		/// TBEnterMICRWithType, Core API in support of MICR/Endorsement at FastLane
		///
		///	@param TBTENDERTYPE nTenderType - Tender Type
		///	@param LPCTSTR szRawData - Raw MICR data from the device
        /// @param LPCTSTR szTransitNumber - Bank Transit Number
        /// @param LPCTSTR szAccountNumber - Customer Account Number
        /// @param LPCTSTR szBankNumber - ABA Bank number
        /// @param LPCTSTR szSerialNumber - Check document number
        /// @param LPCTSTR szEPC - Extended Processing Code
        /// @param long lType - Type (1=Personal, 2=Business, 99= Unknown)
        /// @param long lCountryCode - Country Code (1=USA, 2=Canada, 3=Mexico, 99=Unknown)
		///	@return TBRC - 
		//////////////////////////////////////////////////////////////////////
    virtual TBRC TBEnterMICRWithType(
                       TBTENDERTYPE nTenderType,
                       LPCTSTR szRawData,         
                       LPCTSTR szTransitNumber,   
                       LPCTSTR szAccountNumber,   
                       LPCTSTR szBankNumber,      
                       LPCTSTR szSerialNumber,    
                       LPCTSTR szEPC,             
                       long lType,                
                       long lCountryCode);

	CString DecodeStringBase64(CString csEncodedData);
        
protected:
		//////////////////////////////////////////////////////////////////////
		///
		/// Contructor
		///
		//////////////////////////////////////////////////////////////////////
    CBaseTrxBroker();

private:

		//////////////////////////////////////////////////////////////////////
		///
		/// Hide Copy Const
		///
		//////////////////////////////////////////////////////////////////////
    CBaseTrxBroker(const CBaseTrxBroker &);              

		//////////////////////////////////////////////////////////////////////
		///
		/// Hide Assignment
		///
		//////////////////////////////////////////////////////////////////////
    CBaseTrxBroker & operator =(const CBaseTrxBroker &);

    bool m_bTBInit; ///< flag if TB has been initialized

    bool m_bPipeServerStarted; ///< flag if PipeServer has been started

    bool m_bPreScotStart_Active; ///< flag if PreScotStart.bat is active

    BOOL m_bInAssistMode;	///< flag if it's in assist mode

		CCustomerTBConnectPOS* m_pConnectPOS;	///< pointer to CustomerTBConnectPOS

    bool m_bStoreMode;	///< flag if it's in store mode

	void Decode(const void* data, const size_t len, LPSTR szDest);
	char* ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn, int sizein, int* sizeout);

};
#endif 
