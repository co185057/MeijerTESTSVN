#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMMessages.h 4     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMMessages.h    $Revision: 4 $ $Date: 10/21/08 4:09p $
 *
 * Copyright 2003 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMMessages.h $
 * 
 * 4     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 16    10/17/08 7:17p Sc185102
 * SR678
 * 
 * 15    1/29/08 1:23p Cs185088
 * RFC - RAP Intervention in every Training Mode Transaction
 * 
 * 15    1/23/08 10:55p cs185088
 * RFC - RAP Intervention in every Training Mode Transaction
 *
 * 14    1/22/08 5:21a pl250020
 * TAR 352409 : Added for new Suspension decline Event.
 *
 * 13    12/22/06 10:38a Jj100011
 * RFC 337360
 *
 * 10    4/05/05 11:58a Dm185016
 * TAR 297356
 *
 * 9     3/17/05 2:41p Dm185016
 * TAR 296081
 *
 * 8     3/15/05 3:36p Dm185016
 * TAR 295676 - AcceptConnection is now handled by a separate thread to
 * prevent hanging the state machine thread.
 *
 * 7     3/10/05 11:46a Tp151000
 * TAR 295124
 * TP & AQ
 *
 *
 * 6     3/03/05 3:14p Dm185016
 * TAR 291700
 *
 * 5     2/28/05 4:21p Dm185016
 * lint
 *
 * 4     2/17/05 5:54p Dm185016
 *
 * 3     2/14/05 7:08p Dm185016
 * Release Exclusive Access Event support
 *
 * 2     1/27/05 2:38p Dm185016
 * New messages for 4.0 RAP dialogs.
 *
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
*
* 13    1/20/05 10:36a Dm185016
* Added handler for lane context switches.
*
* 12    12/22/04 11:03a Dm185016
* Added message for switch context async event.
*
* 11    12/16/04 9:58a Dm185016
* Messages to support clearing the receipt and suspend transaction.
*
* 10    12/14/04 11:22a Dm185016
* TAR 288478.  New remote approval message.
*
* 9     12/02/04 3:41p Dm185016
* More messages.
*
* 8     11/30/04 4:28p Dm185016
* New messages added for verify signature and cancel transaction generic.
*
* 7     11/19/04 1:39p Dm185016
*
* 6     11/18/04 1:40p Dm185016
* New messages for start transaction functions
*
* 5     11/16/04 11:32a Dm185016
* Added missing message
*
* 4     10/29/04 8:58a Dm185016
* More new messages\.
*
* 3     10/14/04 1:36p Dm185016
*
* 2     10/05/04 11:05a Dm185016
* Changed parameter for Initialize.  CPSX object now obtained via handle
* rather than thru pointer.
*
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

//-----------------------------------------------------------------------------
// Constants for Message Names.  These values are used in the XML
#define MSG_INITIALIZE                      _T("Initialize")
#define MSG_TERMINATE                       _T("Terminate")
#define MSG_CAN_APPROVE_COMMAND             _T("Can Approve Command")
#define MSG_GET_APPROVAL_COMMAND            _T("Get Approval Command")
#define MSG_APPROVAL_COMMAND                _T("Approval Command")
#define MSG_CONTROL_COMMAND                 _T("Control Command")
#define MSG_EVENT_COMMAND                   _T("Event Command")
#define MSG_GET_COMMAND                     _T("Get Command")
#define MSG_SET_COMMAND                     _T("Set Command")
#define MSG_INPUT_EVENT_COMMAND             _T("Input Event Command")
#define MSG_ADD_OTHER_EVENT                 _T("Add Other Event")
#define MSG_SECURITY_EVENT                  _T("Security Event")
#define MSG_COUPON_EVENT                    _T("Coupon Event")
#define MSG_VOID_EVENT                      _T("Void Event")
#define MSG_AGE_RESTRICTION_EVENT           _T("Age Restriction Event")
#define MSG_VISUAL_VALIDATION_EVENT         _T("Visual Validation Event")
#define MSG_HOPPER_SUBSTITUTION_EVENT         _T("Hopper Substitution Event")
#define MSG_CALL_FOR_HELP_EVENT             _T("Call For Help Event")
#define MSG_CALL_FOR_HELP_OUTSIDE_XACT_EVENT    _T("Call For Help Outside Transaction Event")
#define MSG_OTHER_PAYMENT_EVENT             _T("Other Payment Event")
#define MSG_SUSPEND_EVENT                   _T("Suspend Event")
#define MSG_PRICE_REQUIRED_EVENT            _T("Price Required Event")
#define MSG_UNKNOWN_ITEM_EVENT              _T("Unknown Item Event")
#define MSG_UNKNOWN_PRICE_EVENT             _T("Unknown Price Event")
#define MSG_RECALLED_ITEM_EVENT             _T("Recalled Item Event")
#define MSG_TIME_RESTRICTED_ITEM_EVENT      _T("Time Restricted Item Event")
#define MSG_SIG_CAP_EVENT                   _T("Sig Cap Event")
#define MSG_APPLICATION_ERROR_EVENT         _T("Application Error Event")
#define MSG_DEVICE_ERROR_EVENT              _T("Device Error Event")
#define MSG_CHECK_ON_LANE_EVENT             _T("Check on Lane Event")
#define MSG_CHECK_ON_LANE_RAP_EVENT         _T("Check on Lane Rap Event")
#define MSG_WLDB_FAILURE_EVENT              _T("WLDB Failure Event")
#define MSG_MULTIPACKITEM                   _T("Multipack Item Event")
#define MSG_OUTOFSYNC_STOP                  _T("Out Of Sync Stop Event")
#define MSG_OUTOFSYNC_ASSIST                _T("Out Of Sync Assist Event")
#define MSG_RAPDATANEEDED_NOT_HANDLED_EVENT _T("RAPDataNeeded Not Handled Event")
#define MSG_QUANTITY_RESTRICTED_ITEM_EVENT  _T("Quantity Restricted Item Event")
#define MSG_UNDEACTIVATED_ITEM_EVENT        _T("Undeactivated Item Event")
#define MSG_CAN_SUSPEND_NOW_EVENT           _T("Can Suspend Now Event")
#define MSG_CANCEL_APPLICATION_ERROR_EVENT  _T("Cancel Application Error Event")
#define MSG_ASSISTED_TENDER_EVENT           _T("Assisted Tender Event")
#define MSG_BAG_AREA_BACKUP_EVENT           _T("Bag Area Backup Event")
#define MSG_SAFETY_COVER_OPEN_EVENT         _T("Safety Cover Open Event")
#define MSG_ASSISTED_ENDORSEMENT_EVENT      _T("Assisted Endorsement Event")
#define MSG_MEDIA_LOW_EVENT                 _T("Media Low Event")
#define MSG_MEDIA_EVENT                     _T("Media Event")
#define MSG_ENABLE_NUM_ITEMS_EVENT          _T("Enable NumItems Event")
#define MSG_DISABLE_NUM_ITEMS_EVENT         _T("Disable NumItems Event")
#define MSG_INITIALIZE_UI                   _T("Initialize UI")
#define MSG_START_XACT_ENTER_AM             _T("Start Transaction Enter AM Event")
#define MSG_START_XACT                      _T("Start Transaction Event")
#define MSG_CANCEL_XACT                     _T("Cancel Transaction Event")
#define MSG_CANCEL_XACT_GENERIC             _T("Cancel Transaction Generic Event")
#define MSG_NO_RAP_DATA_NEEDED_EVENT        _T("No RAP Data Needed Event")
#define MSG_RAP_DATA_NEEDED_EVENT           _T("RAP Data Needed Event")
#define MSG_NORMAL_ITEM_EVENT               _T("Normal Item Event")
#define MSG_SWITCH_CONTEXT_EVENT            _T("Switch Context Event")
#define MSG_SWITCH_LANE_CONTEXT_EVENT       _T("Switch Lane Context Event")
#define MSG_OUT_OF_SERVICE_EVENT            _T("Out Of Service Event")
#define MSG_RETURN_TO_SERVICE_EVENT         _T("Return To Service Event")
#define MSG_READY_TO_COLLECT_EVENT          _T("Ready To Collect Event")
#define MSG_READY_TO_SUSPEND_EVENT          _T("Ready To Suspend Event")
#define MSG_VERIFY_SIGNATURE_EVENT          _T("Verify Signature Event")
#define MSG_CLOSE_LANE_EVENT                _T("Close Lane Event")
#define MSG_REMOTE_DATA_EVENT               _T("Remote Data Event")
#define MSG_CONFIRM_SUSPEND_EVENT           _T("Confirm Suspend Event")
#define MSG_PRICE_NOT_MATCH_EVENT           _T("Price Not Match Event")
#define MSG_AM_SIZE_TO_FULLSCREEEN          _T("Assist Mode Size To FullScreen")
#define MSG_IMMEDIATE_INTERVENTION          _T("Immediate Intervention")
#define MSG_RESTRICTED_NOT_ALLOWED_EVENT    _T("Restricted Not Allowed Event")
#define MSG_TRAINING_MODE_TRANSACTION_EVENT _T("Training Mode Transaction Event")   
#define MSG_SUSPENSION_DECLINE_EVENT		_T("SuspensionDecline Event")	// TAR 352409
#define MSG_COUPON_LIMIT_EVENT 				_T("Max Coupon Event")					//SR678
#define MSG_COUPON_LIMIT_FINALIZATION_EVENT	_T("Max Coupon Finalization Event")	//SR678

#define MSG_REMOTE_APPROVAL                     _T("Remote Approval")
#define MSG_ADD_OTHER_APPROVAL                  _T("Add Other Approval")
#define MSG_SECURITY_APPROVAL                   _T("Security Approval")
#define MSG_COUPON_APPROVAL                     _T("Coupon Approval")
#define MSG_VOID_APPROVAL                       _T("Void Approval")
#define MSG_AGE_RESTRICTION_APPROVAL            _T("Age Restriction Approval")
#define MSG_VISUAL_VALIDATION_APPROVAL          _T("Visual Validation Approval")
#define MSG_HOPPER_SUBSTITUTION_APPROVAL          _T("Hopper Substitution Approval")
#define MSG_CALL_FOR_HELP_APPROVAL              _T("Call For Help Approval")
#define MSG_OTHER_PAYMENT_APPROVAL              _T("Other Payment Approval")
#define MSG_SUSPEND_APPROVAL                    _T("Suspend Approval")
#define MSG_PRICE_REQUIRED_APPROVAL             _T("Price Required Approval")
#define MSG_UNKNOWN_ITEM_APPROVAL               _T("Unknown Item Approval")
#define MSG_RECALLED_ITEM_APPROVAL              _T("Recalled Item Approval")
#define MSG_TIME_RESTRICTED_ITEM_APPROVAL       _T("Time Restricted Item Approval")
#define MSG_SIG_CAP_APPROVAL                    _T("Sig Cap Approval")
#define MSG_APPLICATION_ERROR_APPROVAL          _T("Application Error Approval")
#define MSG_DEVICE_ERROR_APPROVAL               _T("Device Error Approval")
#define MSG_QUANTITY_RESTRICTED_ITEM_APPROVAL   _T("Quantity Restricted Item Approval")
#define MSG_UNDEACTIVATED_ITEM_APPROVAL         _T("Undeactivated Item Approval")
#define MSG_RESTRICTED_NOT_ALLOWED_APPROVAL     _T("Restricted Not Allowed Approval")
#define MSG_SUSPENSION_DECLINE_APPROVAL			_T("SuspensionDecline Approval")	// TAR 352409

#define MSG_SET_XACT_STATE                      _T("Set Transaction State")
#define MSG_ENTER_STORE_MODE                    _T("Enter Store Mode")
#define MSG_EXIT_STORE_MODE                     _T("Exit Store Mode")
#define MSG_XACT_END                            _T("Transaction End")
#define MSG_XACT_BEGIN                          _T("Transaction Begin")
#define MSG_VALIDATE_OPERATOR                   _T("Validate Operator")
#define MSG_SET_LIGHT_STATE                     _T("Set Light State")
#define MSG_ENABLE_ASSIST_MODE_BUTTON           _T("Enable Assist Mode Button")
#define MSG_DISABLE_ASSIST_MODE_BUTTON          _T("Disable Assist Mode Button")
#define MSG_ENABLE_HH_ASSIST_MODE_BUTTON        _T("Enable Handheld Assist Mode Button")
#define MSG_DISABLE_HH_ASSIST_MODE_BUTTON       _T("Disable Handheld Assist Mode Button")

#define MSG_NORMAL_ITEM_EVENT_ALERT             _T("Normal Item Event - Alert")
#define MSG_NORMAL_ITEM_EVENT_POS_MESSAGE       _T("Normal Item Event - POS Message")
#define MSG_CONNECTION_COMPLETE                 _T("Connection Complete")

//
//-----------------------------------------------------------------------------


/*lint -e{756, 768} */
namespace fastlane
{


typedef struct SInitialize
{
    LONG lPSXHandle;
    HTRACE hTrace;
} _SInitialize;


typedef struct STerminate
{
} _STerminate;


typedef struct SCanApprove
{
    LONG lResult;
    _TCHAR szParms[1];
} _SCanApprove;


typedef struct SGetApprovalInfo
{
    _TCHAR szParms[1];
} _SGetApprovalInfo;


typedef struct SOnControl
{
    _TCHAR szParms[1];
} _SOnControl;


typedef struct SOnEvent
{
    _TCHAR szParms[1];
} _SOnEvent;


typedef struct SGet
{
    _TCHAR szParms[1];
} _SGet;


typedef struct SSet
{
    _TCHAR szParms[1];
} _SSet;


typedef struct SOnApproval
{
    _TCHAR szParms[1];
} _SOnApproval;


typedef struct SOnInputEvent
{
    _TCHAR szParms[1];
} _SOnInputEvent;


typedef union _UMessageParms
{
    SInitialize         initialize;
    STerminate          terminate;
    SCanApprove         canApprove;
    SGetApprovalInfo    getApprovalInfo;
    SOnControl          onControl;
    SOnEvent            onEvent;
    SGet                get;
    SSet                set;
    SOnApproval         onApproval;
    SOnInputEvent       onInputEvent;
} UMessageParms;

}
