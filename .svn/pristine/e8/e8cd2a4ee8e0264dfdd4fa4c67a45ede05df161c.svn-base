#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/remote-console-manager/RCMKeyValue.h 5     1/25/10 8:18p Cu185008 $
/*
 * @(#)RemoteAppMgrKeyValue.h    $Revision: 5 $ $Date: 1/25/10 8:18p $
 *
 * Copyright 2004 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/remote-console-manager/RCMKeyValue.h $
 * 
 * 5     1/25/10 8:18p Cu185008
 * G2 Migration.
 * 
 * 4     4/30/09 2:11p Sm133076
 * 381428 Meijer lab 4.32.6 FL Reports: Daily Summary Report by Cashier
 * are reporting incorrect operator ID
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 30    2/14/08 2:48p Mm185096
 * added new const definitions for SR683
 * 
 * 29    2/14/08 11:05a Mm185096
 * added new macro definition for new action SetTextColor
 * 
 * 28    1/18/08 9:55a pl250020
 * TAR 352409: Added new evnt for Suspension Decline 
 *
 * 27    11/29/07 4:07p Cb160006
 * TAR 364905
 *
 * 26    12/22/06 10:38a Jj100011
 * RFC 337360
 *
 * 25    8/22/06 4:55p Mb185050
 * TAR 330580 - THD - toggle issue at RAP & shift key
 *
 * 24    6/24/05 1:41p Dm185016
 * Added capability for customer created rap-data-needed contexts.
 *
 * 22    5/09/05 4:43p Dm185016
 * Added sound support for RAP interventions
 *
 * 21    4/29/05 2:15p Cb160006
 * TAR 299443
 *
 * 20    4/27/05 5:38p Tp151000
 * TAR 299443
 *
 * 19    4/15/05 9:18a Dm185016
 * Merge from Patch B
 *
 * 17    4/14/05 9:56p Dm185016
 * TAR 298021
 *
 * 16    4/04/05 10:50a Dm185016
 * TAR 297356
 *
 * 15    3/30/05 11:46a Dm185016
 * TAR 297001
 *
 * 14    3/17/05 2:41p Dm185016
 * TAR 296081
 *
 * 13    3/08/05 4:36p Tp151000
 * 295124
 *
 * TP & AQ
 *
 * 12    3/08/05 10:25a Dm185016
 *
 * 11    3/07/05 6:56p Dm185016
 * Added indication of current exclusive access per Dan.
 *
 * 10    3/03/05 3:14p Dm185016
 * TAR 291700
 *
 * 9     3/02/05 4:51p Dm185016
 * TAR 294890.
 *
 * 8     2/28/05 4:19p Dm185016
 * lint
 *
 * 7     2/21/05 8:09p Dm185016
 * TAR 293968
 *
 * 6     2/18/05 11:27a Dm185016
 *
 * 5     2/18/05 8:27a Dm185016
 *
 * 4     2/14/05 7:07p Dm185016
 * Release Exclusive Access Event support
 *
 * 3     2/10/05 5:28p Dm185016
 *
 * 2     2/03/05 5:42p Tp151000
 * TAR 292361
 *
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
*
* 29    1/20/05 10:36a Dm185016
* Added handler for lane context switches.
*
* 28    1/15/05 6:53p Dm185016
* Support for signon from scanner data.
*
* 27    1/13/05 5:29p Dm185016
* Remote scanner data support
* Time server support
*
* 26    1/13/05 1:30p Dm185016
* TAR 290304
*
* 25    1/12/05 5:53p Dm185016
* Multiple RAP support.
*
* 24    1/10/05 3:17p Dm185016
*
* 23    1/05/05 2:02p Tk124825
* TAR 287909 - Added the new  keys for total amount, item, item value,
* and visible
*
* 22    12/31/04 9:21a Dm185016
* TAR 288930 - Added key for string-id in XML
*
* 21    12/22/04 11:03a Dm185016
* TAR 288934 - Added priority keys.  Made switch context an event.
*
* 20    12/16/04 3:33p Dm185016
*
* 19    12/14/04 11:21a Dm185016
* TAR 288478.  New keys for clearing button text when remote event
* received.
* New keys for HH support.
*
* 18    12/09/04 1:47p Dm185016
* TAR 287907 Added definitions for scrolling buttons
*
* 17    12/06/04 4:10p Dm185016
* Added reload options feature
*
* 16    12/02/04 3:41p Dm185016
* More key values.
*
* 15    11/30/04 4:23p Dm185016
* Added reporting functions.
*
* 14    11/29/04 7:14p Dm185016
* Added definitions for old RAP protocol.
*
* 13    11/22/04 3:59p Dm185016
* Added definition for receipt-source
*
* 12    11/19/04 1:38p Dm185016
* more constants
*
* 11    11/18/04 1:39p Dm185016
*
* 10    11/16/04 11:31a Dm185016
* More definitions
*
* 9     11/12/04 8:54a Dm185016
* Added defines for SwitchContext
*
* 8     11/11/04 9:50a Dm185016
* Changes for new OnAssistModeEvent
*
* 7     11/08/04 10:43a Dm185016
* Added old button names for compatibility with SCOTAPP.
*
* 6     11/02/04 3:07p Dm185016
* More updates
*
* 5     10/26/04 1:51p Dm185016
* Numerous fixes
*
* 4     10/14/04 1:35p Dm185016
* New values for xact state.
*
* 3     10/08/04 1:32p Dm185016
* Added OnControl(operation=connection)
*
* 2     10/07/04 6:13p Dm185016
* Added InputEvent values.
*
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

//lint -e551

#include "StateMachineKeyValue.h"

#define KEY_TYPE                                                            _T("type")
#define KEY_OP_ID                                                           _T("operator")
#define KEY_OP_PASSWORD                                                     _T("password")
#define KEY_DESCRIPTION                                                     _T("description")
#define KEY_ID                                                              _T("id")
#define KEY_AUX_DATA                                                        _T("aux-data")
#define KEY_REMOTE_CONNECTION_NAME                                          _T("remote-connection-name")
#define KEY_UNSOLICITED_DATA                                                _T("unsolicited-data")
#define KEY_VALID_VALUES                                                    _T("valid-values")
#define KEY_LANE_NAME                                                       _T("lane-name")
#define KEY_BUTTON_NAME                                                     _T("button-name")
#define KEY_CONTROL_NAME                                                    _T("control-name")
#define KEY_CONTEXT_NAME                                                    _T("context-name")
#define KEY_CONTEXT_NAME_ALL                                                _T("all")
#define KEY_VIEW                                                            _T("view")
#define KEY_RECEIPT_SOURCE                                                  _T("receipt-source")
#define KEY_REASON                                                          _T("reason")
#define KEY_COOKIE                                                          _T("cookie")
#define KEY_LOOKUP_TABLE_NAME                                               _T("lookup-table-name")
#define KEY_MSG_NAME                                                        _T("message-name")
#define KEY_STATE_NAME                                                      _T("state-name")
#define KEY_SOUND_NAME                                                      _T("sound-name")
#define KEY_SOUND_NAME_RED_INTERVENTION                                     _T("red-intervention")
#define KEY_SOUND_NAME_YELLOW_INTERVENTION                                  _T("yellow-intervention")
#define KEY_SOUND_NAME_NONE                                                 _T("none")
#define KEY_PRIORITY                                                        _T("priority")
#define KEY_COLOR_NAME                                                      _T("color-name")

const _TCHAR KEY_OPERATION_ACQUIRE[]                                        = _T("acquire");
const _TCHAR KEY_OPERATION_RELEASE[]                                        = _T("release");
const _TCHAR KEY_OPERATION_DISABLE_ALL_CONTEXTS[]                           = _T("disable-all-contexts");
const _TCHAR KEY_OPERATION_ENABLE_ALL_CONTEXTS[]                            = _T("enable-all-contexts");

#define KEY_APPROVAL_TYPE_DATA_NEEDED                                       _T("rap-data-needed")
#define KEY_APPROVAL_TYPE_SECURITY                                          _T("security")
#define KEY_APPROVAL_TYPE_COUPON                                            _T("coupon")
#define KEY_APPROVAL_TYPE_VOID                                              _T("void")
#define KEY_APPROVAL_TYPE_AGE_RESTRICTION                                   _T("age-restriction")
#define KEY_APPROVAL_TYPE_VISUAL_VALIDATION                                 _T("visual-validation")
#define KEY_APPROVAL_TYPE_HOPPER_SUBSTITUTION                               _T("hopper-substitution")
#define KEY_APPROVAL_TYPE_CALL_FOR_HELP                                     _T("call-for-help")
#define KEY_APPROVAL_TYPE_OTHER_PAYMENT                                     _T("other-payment")
#define KEY_APPROVAL_TYPE_SUSPEND                                           _T("suspend")
#define KEY_APPROVAL_TYPE_PRICE_REQUIRED                                    _T("price-required")
#define KEY_APPROVAL_TYPE_UNKNOWN_ITEM                                      _T("unknown-item")
#define KEY_APPROVAL_TYPE_UNKNOWN_PRICE                                     _T("unknown-price")
#define KEY_APPROVAL_TYPE_RECALLED_ITEM                                     _T("recalled-item")
#define KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM                              _T("time-restricted-item")
#define KEY_APPROVAL_TYPE_SIG_CAP                                           _T("sig-cap")
#define KEY_APPROVAL_TYPE_APPLICATION_ERROR                                 _T("application-error")
#define KEY_APPROVAL_TYPE_DEVICE_ERROR                                      _T("device-error")
#define KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM                          _T("quantity-restricted-item")
#define KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM                                _T("undeactivated-item")
#define KEY_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED                            _T("restricted-not-allowed")
#define KEY_APPROVAL_TYPE_REMOTE                                            _T("remote")
#define KEY_APPROVAL_TYPE_STALE                                             _T("stale")
#define KEY_APPROVAL_TYPE_DATANEEDED                                        _T("data-needed")
#define KEY_APPROVAL_TYPE_UNKNOWN                                           _T("unknown")
#define KEY_APPROVAL_TYPE_OTHER                                             _T("other")
#define KEY_APPROVAL_TYPE_COLLECTION                                        _T("collection")
#define KEY_APPROVAL_TYPE_READY_TO_COLLECT                                  _T("ready-to-collect")
#define KEY_APPROVAL_TYPE_EFT                                               _T("eft")
#define KEY_APPROVAL_TYPE_OUTOFSERVICE                                      _T("out-of-service")
#define KEY_APPROVAL_TYPE_SUSPENSIONDECLINED					    		_T("suspension-decline")	// TAR 352409

#define KEY_AI_TYPE                                                         KEY_TYPE
#define KEY_AI_TYPE_ALL_APPROVALS                                           _T("*")

#define KEY_AP_TYPE                                                         KEY_TYPE
#define KEY_AP_OP_ID                                                        KEY_OP_ID
#define KEY_AP_OP_PASSWORD                                                  KEY_OP_PASSWORD
#define KEY_AP_APPROVAL_CODE                                                _T("approval-code")
#define KEY_AP_AUX_DATA                                                     KEY_AUX_DATA
#define KEY_AP_COOKIE                                                       KEY_COOKIE
#define KEY_AP_HANDLE                                                       _T("handle")

#define KEY_CA_TYPE                                                         KEY_TYPE
#define KEY_CA_OP_ID                                                        KEY_OP_ID
#define KEY_CA_OP_PASSWORD                                                  KEY_OP_PASSWORD
#define KEY_CA_EVENT_ID                                                     _T("event-id")
#define KEY_CA_APPROVAL_GRANTED                                             _T("approval-granted")

#define KEY_CT_OPERATION                                                    KEY_OPERATION
#define KEY_CT_OPERATION_SET_XACT_STATE                                     _T("set-transaction-state")
#define KEY_CT_OPERATION_ENTER_STORE_MODE                                   _T("enter-store-mode")
#define KEY_CT_OPERATION_EXIT_STORE_MODE                                    _T("exit-store-mode")
#define KEY_CT_OPERATION_XACT_END                                           _T("end")
#define KEY_CT_OPERATION_XACT_BEGIN                                         _T("begin")
#define KEY_CT_OPERATION_VALIDATE_OPERATOR                                  _T("validate-operator")
#define KEY_CT_OPERATION_CONNECT                                            _T("connect")
#define KEY_CT_OPERATION_DISCONNECT                                         _T("disconnect")
#define KEY_CT_OPERATION_DISCONNECT_SERVER                                  _T("disconnect-server")
#define KEY_CT_OPERATION_RELOAD_OPTIONS                                     _T("reload-options")
#define KEY_CT_OPERATION_ACQUIRE_EXCLUSIVE_ACCESS                           _T("acquire-exclusive-access")
#define KEY_CT_OPERATION_RELEASE_EXCLUSIVE_ACCESS                           _T("release-exclusive-access")
#define KEY_CT_OPERATION_SIGNON                                             _T("sign-on")
#define KEY_CT_OPERATION_SIGNOFF                                            _T("sign-off")
#define KEY_CT_OPERATION_SET_LIGHT_STATE                                    _T("set-light-state")
#define KEY_CT_OPERATION_SET_LIGHT_STATE_GREEN                              _T("green")
#define KEY_CT_OPERATION_SET_LIGHT_STATE_YELLOW                             _T("yellow")
#define KEY_CT_OPERATION_SET_LIGHT_STATE_RED                                _T("red")
#define KEY_CT_OPERATION_SCANDATA                                           _T("scanner-data")
#define KEY_CT_OPERATION_SCANTYPE                                           _T("scanner-type")
#define KEY_CT_OPERATION_SCANDATALABEL                                      _T("scanner-data-label")
#define KEY_CT_USERID                                                       _T("UserId")
#define KEY_CT_OP_ID                                                        KEY_OP_ID
#define KEY_CT_OP_PASSWORD                                                  KEY_OP_PASSWORD
#define KEY_CT_CONTEXT                                                      _T("context")
#define KEY_CT_SUCCESS                                                      _T("success")
#define KEY_CT_OP_VALID                                                     _T("valid")
#define KEY_CT_DESCRIPTION                                                  KEY_DESCRIPTION
#define KEY_CT_STATE                                                        _T("state")
#define KEY_CT_VALID                                                        _T("valid")
#define KEY_CT_REMOTE_CONNECTION_NAME                                       KEY_REMOTE_CONNECTION_NAME
#define KEY_CT_TIME_REQUEST                                                 _T("set-rap-time")
#define KEY_CT_USE_MESSAGE                                                  _T("use-message")
#define KEY_CT_ACQUIRE_INDICATION                                           _T("acquire")
#define KEY_CT_ASSIST_DENIED												_T("assist-denied")
#define KEY_CT_ASSIST_ALLOWED                                               _T("assist-granted")
#define KEY_CT_OPERATION_SIGNON_AUTO                                        _T("sign-on-auto")
const _TCHAR KEY_CT_VARIABLE_NAME[]                                         = _T("variable-name");
const _TCHAR KEY_CT_LOOKUP_TABLE_NAME[]                                     = KEY_LOOKUP_TABLE_NAME;

const _TCHAR KEY_CT_OPERATION_INTERVENTION[]                                = _T("intervention");
const _TCHAR KEY_CT_INTERVENTION_TYPE[]                                     = _T("intervention-type");
const _TCHAR KEY_CT_INTERVENTION_TYPE_IMMEDIATE[]                           = _T("immediate");
const _TCHAR KEY_CT_INTERVENTION_TYPE_DELAYED[]                             = _T("delayed");
const _TCHAR KEY_CT_INTERVENTION_TYPE_NONE[]                                = _T("none");
const _TCHAR KEY_CT_INTERVENTION_TIME[]                                     = _T("intervention-time");
const _TCHAR KEY_CT_OPERATION_SET_XACT_STATE_INACTIVE[]                     = _T("inactive");
const _TCHAR KEY_CT_OPERATION_SET_XACT_STATE_ITEMIZATION[]                  = _T("itemization");
const _TCHAR KEY_CT_OPERATION_SET_XACT_STATE_TENDER[]                       = _T("tender");
const _TCHAR KEY_CT_OPERATION_SET_XACT_STATE_WAIT_FOR_APPROVAL[]            = _T("wait-for-approval");
const _TCHAR KEY_CT_OPERATION_SET_XACT_STATE_DATA_NEEDED[]                  = _T("data-needed");
const _TCHAR KEY_CT_OPERATION_SET_XACT_STATE_BAG_ITEM[]                     = _T("bag-item");
const _TCHAR KEY_CT_OPERATION_SET_XACT_STATE_PICKING_UP_ITEMS[]             = _T("picking-up-items");
const _TCHAR KEY_CT_OPERATION_SET_XACT_STATE_SECURITY_VIOLATION[]           = _T("security-violation");

const _TCHAR KEY_EV_TYPE[]                                                  = KEY_TYPE;
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT[]                                  = _T("add-other-event");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_MEDIA[]                            = _T("COMMAND=MEDIA");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_ENABLE_NUM_ITEMS[]                 = _T("COMMAND=EnableNumItems");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_DISABLE_NUM_ITEMS[]                = _T("COMMAND=DisableNumItems");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_UIINITIALIZE[]                     = _T("COMMAND=UIInitialize");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODETOGGLETOMAIN[]           = _T("COMMAND=AssistModeToggleToMain");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODETOGGLETOMAINEXIT[]       = _T("COMMAND=AssistModeToggleToMainExit");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODESIZETOFULLSCREEEN[]      = _T("COMMAND=AssistModeSizeToFullScreen");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODESIZETOPARTIALSCREEN[]    = _T("COMMAND=AssistModeSizeToPartialScreen");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODESIZERESTORE[]            = _T("COMMAND=AssistModeRestore");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODETOGGLETOPARTIAL[]        = _T("COMMAND=AssistModeToggleToPartial");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_ASSISTMODEREFRESHLANESTATUS[]      = _T("COMMAND=AssistModeRefreshLaneStatus");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_RAPINITIATETRANSACTIONASSISTMODE[] = _T("COMMAND=RAPInitiateTransactionAssistMode");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_RAPINITIATETRANSACTIONDATANEEDED[] = _T("COMMAND=RapInitiateTransactionDataNeeded");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_RAPINITIATELANECLOSE[]             = _T("RAPInitiateLaneClose");
//const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_RAPINITIATELANECLOSE[]             = _T("COMMAND=RAPInitiateLaneClose");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_START_XACT_ENTER_AM[]              = _T("COMMAND=StartTransactionEnterAM");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_START_XACT[]                       = _T("COMMAND=StartTransaction");
const _TCHAR KEY_EV_TYPE_ADD_OTHER_EVENT_[]                                 = _T("COMMAND=AssistMode");
const _TCHAR KEY_EV_TYPE_SECURITY[]                                         = KEY_APPROVAL_TYPE_SECURITY;
const _TCHAR KEY_EV_TYPE_COUPON[]                                           = KEY_APPROVAL_TYPE_COUPON;
const _TCHAR KEY_EV_TYPE_VOID[]                                             = KEY_APPROVAL_TYPE_VOID;
const _TCHAR KEY_EV_TYPE_AGE_RESTRICTION[]                                  = KEY_APPROVAL_TYPE_AGE_RESTRICTION;
const _TCHAR KEY_EV_TYPE_VISUAL_VALIDATION[]                                = KEY_APPROVAL_TYPE_VISUAL_VALIDATION;
const _TCHAR KEY_EV_TYPE_HOPPER_SUBSTITUTION[]                              = KEY_APPROVAL_TYPE_HOPPER_SUBSTITUTION;
const _TCHAR KEY_EV_TYPE_CALL_FOR_HELP[]                                    = KEY_APPROVAL_TYPE_CALL_FOR_HELP;
const _TCHAR KEY_EV_TYPE_OTHER_PAYMENT[]                                    = KEY_APPROVAL_TYPE_OTHER_PAYMENT;
const _TCHAR KEY_EV_TYPE_SUSPEND[]                                          = KEY_APPROVAL_TYPE_SUSPEND;
const _TCHAR KEY_EV_TYPE_PRICE_REQUIRED[]                                   = KEY_APPROVAL_TYPE_PRICE_REQUIRED;
const _TCHAR KEY_EV_TYPE_UNKNOWN_ITEM[]                                     = KEY_APPROVAL_TYPE_UNKNOWN_ITEM;
const _TCHAR KEY_EV_TYPE_RECALLED_ITEM[]                                    = KEY_APPROVAL_TYPE_RECALLED_ITEM;
const _TCHAR KEY_EV_TYPE_TIME_RESTRICTED_ITEM[]                             = KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM;
const _TCHAR KEY_EV_TYPE_SIG_CAP[]                                          = KEY_APPROVAL_TYPE_SIG_CAP;
const _TCHAR KEY_EV_TYPE_APPLICATION_ERROR[]                                = KEY_APPROVAL_TYPE_APPLICATION_ERROR;
const _TCHAR KEY_EV_TYPE_DEVICE_ERROR[]                                     = KEY_APPROVAL_TYPE_DEVICE_ERROR;
const _TCHAR KEY_EV_TYPE_QUANTITY_RESTRICTED_ITEM[]                         = KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM;
const _TCHAR KEY_EV_TYPE_UNDEACTIVATED_ITEM[]                               = KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM;
const _TCHAR KEY_EV_TYPE_RESTRICTED_NOT_ALLOWED[]                           = KEY_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED;
const _TCHAR KEY_EV_TYPE_REMOTE[]                                           = KEY_APPROVAL_TYPE_REMOTE;
const _TCHAR KEY_EV_TYPE_SUSPENSIONDECLINED[]					    		= KEY_APPROVAL_TYPE_SUSPENSIONDECLINED;  // TAR 352409
const _TCHAR KEY_EV_TYPE_MEDIA_LOW[]                                        = _T("media-low");
const _TCHAR KEY_EV_TYPE_CAN_SUSPEND_NOW[]                                  = _T("can-suspend-now");
const _TCHAR KEY_EV_TYPE_CANCEL_APPLICATION_ERROR[]                         = _T("cancel-application-error");
const _TCHAR KEY_EV_TYPE_NO_RAP_DATA_NEEDED[]                               = _T("no-rap-data-needed");
const _TCHAR KEY_EV_TYPE_RAP_DATA_NEEDED[]                                  = _T("rap-data-needed");
const _TCHAR KEY_EV_TYPE_NORMAL_ITEM[]                                      = _T("normal-item");
const _TCHAR KEY_EV_TYPE_OUT_OF_SERVICE[]                                   = _T("out-of-service");
const _TCHAR KEY_EV_TYPE_RETURN_TO_SERVICE[]                                = _T("return-to-service");
const _TCHAR KEY_EV_TYPE_READY_TO_COLLECT[]                                 = KEY_APPROVAL_TYPE_READY_TO_COLLECT;
const _TCHAR KEY_EV_TYPE_READY_TO_SUSPEND[]                                 = _T("ready-to-suspend");
const _TCHAR KEY_EV_TYPE_ENTER_STORE_MODE[]                                 = _T("enter-store-mode");
const _TCHAR KEY_EV_TYPE_EXIT_STORE_MODE[]                                  = _T("exit-store-mode");
const _TCHAR KEY_EV_TYPE_SWITCH_CONTEXT[]                                   = _T("switch-context");
const _TCHAR KEY_EV_TYPE_SWITCH_LANE_CONTEXT[]                              = _T("switch-lane-context");
const _TCHAR KEY_EV_TYPE_REMOTE_DATA[]                                      = _T("remote-data");
const _TCHAR KEY_EV_COOKIE[]                                                = KEY_COOKIE;
const _TCHAR KEY_EV_DESCRIPTION[]                                           = KEY_DESCRIPTION;
const _TCHAR KEY_EV_MINIMUM_AGE[]                                           = _T("minimum-age");
const _TCHAR KEY_EV_VOIDED[]                                                = _T("voided");
const _TCHAR KEY_EV_DOB_NOT_NEEDED[]                                        = _T("dob-not-needed");
const _TCHAR KEY_EV_CATEGORY[]                                              = _T("category");
const _TCHAR KEY_EV_EVENT_ID[]                                              = _T("event-id");
const _TCHAR KEY_EV_AMOUNT[]                                                = _T("amount");
const _TCHAR KEY_EV_NEED_APPROVAL[]                                         = _T("need-approval");
const _TCHAR KEY_EV_VIEW_TEXT[]                                             = _T("view-text");
const _TCHAR KEY_EV_VIEW[]                                                  = KEY_VIEW;
const _TCHAR KEY_EV_CONCLUSION[]                                            = _T("conclusion");
const _TCHAR KEY_EV_LEVEL[]                                                 = _T("level");
const _TCHAR KEY_EV_VALUE[]                                                 = _T("value");
const _TCHAR KEY_EV_GROSS[]                                                 = _T("gross");
const _TCHAR KEY_EV_SIG_CAP_DATA[]                                          = _T("sig-cap-data");
const _TCHAR KEY_EV_LENGTH[]                                                = _T("length");
const _TCHAR KEY_EV_KEYED[]                                                 = _T("keyed");
const _TCHAR KEY_EV_PRIORITY[]                                              = KEY_PRIORITY;
const _TCHAR KEY_EV_PRIORITY_URGENT[]                                       = _T("urgent");
const _TCHAR KEY_EV_PRIORITY_DELAYED[]                                      = _T("delayed");
const _TCHAR KEY_EV_BUTTON_COMMAND[]                                        = _T("button-command");
const _TCHAR KEY_EV_APPROVAL_CODE[]                                         = _T("approval-code");
const _TCHAR KEY_EV_LANE_BUTTON_CONTROL_NAME[]                              = _T("lane-button");
const _TCHAR KEY_EV_LANE_BUTTON_TEXT_CONTROL_NAME[]                         = _T("lane-button-text");
const _TCHAR KEY_EV_LANE_STATUS_1_CONTROL_NAME[]                            = _T("lane-status1-text");
const _TCHAR KEY_EV_LANE_STATUS_2_CONTROL_NAME[]                            = _T("lane-status2-text");
const _TCHAR KEY_EV_LANE_STATUS_3_CONTROL_NAME[]                            = _T("lane-status3-text");
const _TCHAR KEY_EV_LANE_STATUS_BUTTON_CONTROL_NAME[]                       = _T("lane-status-button");
const _TCHAR KEY_EV_ASSIST_MODE_BUTTON_CONTROL_NAME[]                       = _T("assist-mode-button");
const _TCHAR KEY_EV_INSTRUCTION_TEXT_VARIABLE_NAME[]                        = _T("instruction-text-variable");
const _TCHAR KEY_EV_TRANSACTION_VARIABLE_NAME[]                             = _T("transaction-variable");
const _TCHAR KEY_EV_STRING_ID[]                                             = _T("string-id");
const _TCHAR KEY_EV_RECEIPT_CONTROL_NAME[]                                  = _T("receipt-control");
const _TCHAR KEY_EV_ITEM_COUNT_TEXT_CONTROL_NAME[]                          = _T("item-count-text");
const _TCHAR KEY_EV_TOTAL_AMOUNT_TEXT_CONTROL_NAME[]                        = _T("total-amount-text");
const _TCHAR KEY_EV_ITEM_COUNT_STATIC_TEXT_CONTROL_NAME[]                   = _T("item-count-static-text");
const _TCHAR KEY_EV_TOTAL_AMOUNT_STATIC_TEXT_CONTROL_NAME[]                 = _T("total-amount-static-text");
const _TCHAR KEY_EV_CONTROL_VISIBLE[]										= _T("visible");
const _TCHAR KEY_EV_DATA_SOURCE[]                                           = _T("data-source");
const _TCHAR KEY_EV_SCROLL_TYPE[]                                           = _T("scroll-type");
const _TCHAR KEY_EV_SCROLL_TYPE_PAGE_UP[]                                   = _T("page-up");
const _TCHAR KEY_EV_SCROLL_TYPE_PAGE_DOWN[]                                 = _T("page-down");
const _TCHAR KEY_EV_RELEASE_NEEDED[]                                        = _T("release-needed");
const _TCHAR KEY_EV_OPERATION[]                                             = KEY_OPERATION;
const _TCHAR KEY_EV_LOOKUP_TABLE_NAME[]                                     = KEY_LOOKUP_TABLE_NAME;
const _TCHAR KEY_EV_AUX_DATA[]                                              = KEY_AUX_DATA;
const _TCHAR KEY_EV_COMMAND[]                                               = _T("COMMAND");

const _TCHAR KEY_EV_TYPE_ENABLE_SECURITY[]                                  = _T("enable-security");
const _TCHAR KEY_EV_TYPE_DISABLE_SECURITY[]                                 = _T("disable-security");

const _TCHAR KEY_EV_DESCRIPTION_DESCRIPTION[]           = _T("DESCRIPTION");
const _TCHAR KEY_EV_DESCRIPTION_LANE_BUTTON_TEXT[]      = _T("LANEBUTTONTEXT");
const _TCHAR KEY_EV_DESCRIPTION_LANE_STATUS_LINE_1[]    = _T("LSWND1");
const _TCHAR KEY_EV_DESCRIPTION_LANE_STATUS_LINE_2[]    = _T("LSWND2");
const _TCHAR KEY_EV_DESCRIPTION_LANE_STATUS_LINE_3[]    = _T("LSWND3");

const _TCHAR KEY_EV_ENABLE[]										= _T("Enable");

#define KEY_IE_LPARAM                                                       _T("lparam")
#define KEY_IE_WPARAM                                                       _T("wparam")
#define KEY_IE_EVENT_ID                                                     _T("event-id")
#define KEY_IE_CONTROL_NAME                                                 _T("control-name")
#define KEY_IE_CONTEXT_NAME                                                 KEY_CONTEXT_NAME
#define KEY_IE_EVENT_NAME                                                   _T("event-name")
#define KEY_IE_REMOTE_CONNECTION_NAME                                       KEY_REMOTE_CONNECTION_NAME
#define KEY_IE_REMOTE_XML_NAME                                              _T("remote-xml-name")
#define KEY_IE_REMOTE_HANDHELD                                              _T("handheld")
#define KEY_IE_REMOTE_START_CONTEXT                                         _T("start-context")

const _TCHAR KEY_GET_ID[]                                                   = KEY_ID;
const _TCHAR KEY_GET_ID_CURRENT_APPROVAL_CODE[]                             = _T("current-approval-code");
const _TCHAR KEY_GET_ID_RAP_DATA_AVAILABLE[]                                = _T("rap-data-available");
const _TCHAR KEY_GET_ID_RAP_DATA[]                                          = _T("rap-data");
const _TCHAR KEY_GET_ID_PREVIOUS_VIEW[]                                     = _T("previous-view");
const _TCHAR KEY_GET_ID_RAP_DATA_PENDING_COUNT[]                            = _T("rap-data-pending-count");
const _TCHAR KEY_GET_ID_CONNECTION_MODE[]                                   = _T("connection-mode");
const _TCHAR KEY_GET_ID_REQUEST_ENABLE_ASSIST_MODE[]                        = _T("request-enable-assist-mode");
const _TCHAR KEY_GET_ID_REQUEST_ENABLE_HH_ASSIST_MODE[]                     = _T("request-enable-handheld-assist-mode");
const _TCHAR KEY_GET_ID_CURRENT_REMOTE_CONNECTION_HANDHELD[]                = _T("remote-connection-handheld");
const _TCHAR KEY_GET_ID_CURRENT_REMOTE_CONNECTION_NAME[]                    = _T("remote-connection-name");
const _TCHAR KEY_GET_VALUE[]                                                = _T("value");
const _TCHAR KEY_GET_REMOTE_CONNECTION_NAME[]                               = KEY_REMOTE_CONNECTION_NAME;

#define KEY_QUERY_ID                                                        KEY_ID
#define KEY_QUERY_ID_CURRENT_APPROVAL_CODE                                  _T("current-approval-code")
#define KEY_QUERY_ID_RAP_DATA                                               _T("rap-data")
#define KEY_QUERY_ID_RAP_DATA_AVAILABLE                                     _T("rap-data-available")
#define KEY_QUERY_ID_RAP_DATA_PENDING_COUNT                                 _T("rap-data-pending-count")
#define KEY_QUERY_ID_PREVIOUS_VIEW                                          _T("previous-view")
#define KEY_QUERY_ID_REQUEST_ENABLE_ASSIST_MODE                             _T("request-enable-assist-mode")
#define KEY_QUERY_ID_CAN_SUSPEND_NOW                                        _T("can-suspend-now")

#define KEY_SET_ID                                                          KEY_ID
#define KEY_SET_ID_XACT_TOTAL                                               _T("transaction-total")
#define KEY_SET_ID_REQUEST_ENABLE_ASSIST_MODE                               _T("request-enable-assist-mode")
#define KEY_SET_ID_REQUEST_ENABLE_HH_ASSIST_MODE                            _T("request-enable-handheld-assist-mode")
#define KEY_SET_CMD_VALUE                                                   _T("value")
#define KEY_SET_ID_FORCE_APPROVAL_BEFORE_SUSPEND							_T("force-approval-before-suspend")

#define KEY_AEV_OP_ID                                                       KEY_OP_ID
#define KEY_AEV_OP_PASSWORD                                                 KEY_OP_PASSWORD
#define KEY_AEV_COOKIE                                                      KEY_COOKIE
#define KEY_AEV_TYPE                                                        _T("type")
#define KEY_AEV_CATEGORY                                                    _T("category")
#define KEY_AEV_PENDING_APPROVALS                                           _T("pending-approvals")
#define KEY_AEV_APPROVAL_CODE                                               _T("approval-code")
#define KEY_AEV_APPROVAL_CODE_APPROVED                                      _T("approved")
#define KEY_AEV_APPROVAL_CODE_VERIFY_APPROVED                               _T("verify-approved")
#define KEY_AEV_APPROVAL_CODE_REDO                                          _T("redo")
#define KEY_AEV_AUX_DATA                                                    KEY_AUX_DATA
#define KEY_AEV_VIEW                                                        KEY_VIEW
#define KEY_AEV_REMOTE_CONNECTION_NAME										KEY_REMOTE_CONNECTION_NAME

#define KEY_SREV_REASON                                                     KEY_REASON

#define KEY_AMEV_OPERATION                                                  KEY_OPERATION
#define KEY_AMEV_OPERATION_ENTER_AM                                         _T("enter-assist-mode")
#define KEY_AMEV_OPERATION_EXIT_AM                                          _T("exit-assist-mode")
#define KEY_AMEV_OPERATION_ENTER_SM                                         _T("enter-store-mode")
#define KEY_AMEV_OPERATION_EXIT_SM                                          _T("exit-store-mode")
#define KEY_AMEV_OPERATION_ENTER_REPORTING                                  _T("enter-reporting-mode")
#define KEY_AMEV_OPERATION_EXIT_REPORTING                                   _T("exit-reporting-mode")
#define KEY_AMEV_OPERATION_ENTER_DATA_NEEDED                                _T("enter-data-needed-mode")
#define KEY_AMEV_OPERATION_EXIT_DATA_NEEDED                                 _T("exit-data-needed-mode")
#define KEY_AMEV_MACHINE_NAME						                        _T("machine-name")
#define KEY_AMEV_CONTEXT_NAME						                        KEY_CONTEXT_NAME
#define KEY_AMEV_REMOTE_CONNECTION_NAME                                     KEY_REMOTE_CONNECTION_NAME

#define KEY_RCEV_OPERATION                                                  KEY_OPERATION
#define KEY_RCEV_OPERATION_SEND_CLIENT_MACHINE_NAME                         _T("send-client-machine-name")
#define KEY_RCEV_OPERATION_INTERVENTION_APPROVED                            _T("intervention-approved")
#define KEY_RCEV_CLIENT_MACHINE_NAME                                        _T("client-machine-name")
#define KEY_RCEV_COOKIE                                                     KEY_COOKIE
#define KEY_RCEV_VIEW                                                       KEY_VIEW
#define KEY_RCEV_TYPE                                                       KEY_TYPE

//-----------------------------------------------------------------------------------
// Constant names used in the old rap data needed protocol
const _TCHAR KEY_RAP_DATA_NEEDED_TOPCAPTION[]                               = _T("TOPCAPTION");
const _TCHAR KEY_RAP_DATA_NEEDED_BOTTOMCAPTION[]                            = _T("BOTTOMCAPTION");
const _TCHAR KEY_RAP_DATA_NEEDED_SUMMARYINSTRUCTION[]                       = _T("SUMMARYINSTRUCTION");
const _TCHAR KEY_RAP_DATA_NEEDED_DEFAULTBUTTON[]                            = _T("DEFAULTBUTTON");
const _TCHAR KEY_RAP_DATA_NEEDED_BUTTON0[]                                  = _T("BUTTON0");
const _TCHAR KEY_RAP_DATA_NEEDED_BUTTON1[]                                  = _T("BUTTON1");
const _TCHAR KEY_RAP_DATA_NEEDED_BUTTON2[]                                  = _T("BUTTON2");
const _TCHAR KEY_RAP_DATA_NEEDED_BUTTON3[]                                  = _T("BUTTON3");
const _TCHAR KEY_RAP_DATA_NEEDED_BUTTON4[]                                  = _T("BUTTON4");
const _TCHAR KEY_RAP_DATA_NEEDED_BUTTON5[]                                  = _T("BUTTON5");
const _TCHAR KEY_RAP_DATA_NEEDED_BUTTON6[]                                  = _T("BUTTON6");
const _TCHAR KEY_RAP_DATA_NEEDED_BUTTON7[]                                  = _T("BUTTON7");

const _TCHAR KEY_RAP_DATA_NEEDED_SECTION[]                                  = _T("SECTION");
const _TCHAR KEY_RAP_DATA_NEEDED_SECTION_CANCELXACTGENERIC[]                = _T("CancelTransactionGeneric");

//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// Button names used by the old rap system but propageted into SCOTAPP code.
// Passed in auxilliary data field
static const _TCHAR KEY_OLD_BUTTONNAME_CCLEARSECURITY[]            = _T("cClearSecurity;");
static const size_t KEY_OLD_BUTTONNAME_LENGTH_CCLEARSECURITY       = (sizeof(KEY_OLD_BUTTONNAME_CCLEARSECURITY)-1);

static const _TCHAR KEY_OLD_BUTTONNAME_CAPPROVEANDUPDATE[]         = _T("cApprove&Update;");
static const size_t KEY_OLD_BUTTONNAME_LENGTH_CAPPROVEANDUPDATE    = (sizeof(KEY_OLD_BUTTONNAME_CAPPROVEANDUPDATE)-1);

static const _TCHAR KEY_OLD_BUTTONNAME_CGOBACK[]                   = _T("cGoBack;");
static const size_t KEY_OLD_BUTTONNAME_LENGTH_CGOBACK              = (sizeof(KEY_OLD_BUTTONNAME_CGOBACK)-1);

static const _TCHAR KEY_OLD_BUTTONNAME_CAPPROVENOUPDATE[]          = _T("cApproveNoUpdate;");
static const size_t KEY_OLD_BUTTONNAME_LENGTH_CAPPROVENOUPDATE     = (sizeof(KEY_OLD_BUTTONNAME_CAPPROVENOUPDATE)-1);

static const _TCHAR KEY_OLD_BUTTONNAME_CLIGHTITEM[]                = _T("cLightItem;");
static const size_t KEY_OLD_BUTTONNAME_LENGTH_CLIGHTITEM           = (sizeof(KEY_OLD_BUTTONNAME_CLIGHTITEM)-1);

static const _TCHAR KEY_OLD_BUTTONNAME_CITEMNOTBAGGED[]            = _T("cItemNotBagged;");
static const size_t KEY_OLD_BUTTONNAME_LENGTH_CITEMNOTBAGGED       = (sizeof(KEY_OLD_BUTTONNAME_CITEMNOTBAGGED)-1);

static const _TCHAR KEY_OLD_BUTTONNAME_CAPPROVEVOID[]            = _T("cApproveVoid;");
static const size_t KEY_OLD_BUTTONNAME_LENGTH_CAPPROVEVOID       = (sizeof(KEY_OLD_BUTTONNAME_CAPPROVEVOID)-1);
//-------------------------------------------------------------------------------------

#define RCM_MSG_CONFIGURATION_LOADED                896
