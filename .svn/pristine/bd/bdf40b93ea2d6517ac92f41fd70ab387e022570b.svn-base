#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/SMKeyValue.h 5     1/05/10 8:39p Cu185008 $
/*
 * @(#)SMKeyValue.h    $Revision: 5 $ $Date: 1/05/10 8:39p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/SMKeyValue.h $
 * 
 * 5     1/05/10 8:39p Cu185008
 * Migrating Core to G2.
 * 
 * 4     11/06/08 2:44p Sa250050
 * RFQ 2183 - DG35 - Patch G Migration
 * 
 * 3     10/29/08 4:50p Sm133076
 * Patch G Migration
 * 
 * 1     6/23/08 4:55p Sa250050
 * FastLane 4.0 E5.3 Build 291 - Core Source
 * 
 * 5     10/20/06 9:54a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 1:41p Dm185016
 * Moved to new Repository.
* 
* 40    1/23/05 5:21p Bd123405
* Merge 3.2 E, 4.0 A changes
* 
* 40    12/08/04 2:28p Tp151000
* TAR 287854
* 
* 39    12/07/04 2:12p Tk124825
* 
* 38    8/05/04 3:32p Dm185016
* TAR 276027 - Added handshaking for SkipBagging button.
* 
* 37    7/06/04 5:22p Dm185016
* 
* 36    6/29/04 10:34a Dm185016
* Another TAB related keyword for SC
* 
* 35    6/29/04 10:24a Dm185016
* Added more SC operations for TAB
* 
* 34    6/28/04 3:34p Dm185016
* Added takeaway belt definitions. Revision 102 of Security Manager API
* document.
* 
* 33    5/21/04 4:42p Dm185016
* Added keywords for void transaction.
* 
* 32    5/21/04 10:46a Dm185016
* New clear-rap-approval= keyword for SecurityExceptions
* 
* 31    5/21/04 9:14a Dm185016
* TAR 269207 - New Exception disposition
* 
* 30    5/19/04 1:41p Dm185016
* Added UPC keyword for Item Void
* 
* 29    5/19/04 9:53a Dm185016
* Support for Void Item API
* 
* 28    5/17/04 10:31a Dm185016
* New constants for error reporting during initialization.
* 
* 27    5/10/04 2:11p Dm185016
* Added TC operation=expired
* 
* 26    5/05/04 8:46a Dm185016
* Added lane-number to Initialize
* 
* 25    5/04/04 1:56p Dm185016
* Added tolerance-type to OnSecurityException event
* 
* 24    4/29/04 6:24p Dm185016
* Added new OnSecurityControl event
* 
* 23    4/28/04 1:25p Tk124825
* Added KEY_SC_RELOAD_OPTIONS_STATUS_TEXT
* 
* 22    4/27/04 1:31p Tp151000
* New Cancel Message in SMM
* 
* 21    4/27/04 1:18p Tp151000
* New Cancel Message in SMM
* 
* 20    4/20/04 3:06p Dm185016
* Added category=mismatch
* 
* 19    4/19/04 3:55p Tp151000
* Add Threshold Max Value to send to the application
* 
* 18    4/16/04 4:01p Dm185016
* Added weight-2x and category
* 
* 17    4/15/04 1:47p Dm185016
* Added flags=not-sellable to ItemDetails.
* 
* 16    4/13/04 1:15p Dm185016
* Added TransmissionControl operation=continue|finalize and tare values
* for OnAppControl
* 
* 15    4/12/04 2:56p Dm185016
* Added keywords for initialization error reporting
* 
* 14    4/05/04 2:28p Dm185016
* Added observed-weight KV
* 
* 13    4/02/04 10:25a Dm185016
* 
* 12    4/02/04 10:11a Dm185016
* Added new keys for server status
* 
* 11    4/01/04 1:48p Dm185016
* Added keywords for min/max tolerance
* 
* 10    4/01/04 12:47p Tp151000
* SMM
* 
* 9     3/31/04 10:48a Tp151000
* SMM
* 
* 8     3/29/04 2:22p Dm185016
* Changed keyword on SMM from weights= to weight
* 
* 7     3/29/04 11:00a Dm185016
* Fixes for default values
* 
* 6     3/26/04 11:35a Dm185016
* Added SMM query keywords
* 
* 5     3/25/04 3:12p Dm185016
* API changes for SMM
* 
* 4     3/19/04 2:51p Dm185016
* New API keywords
* 
* 2     2/24/04 3:25p Dm185016
* Added all other keys
* 
* 1     2/10/04 11:36a Dm185016
* New SecurityManager component
*/

#define KEY_DEVICE_NAME                             _T("device-name")
#define KEY_MISSING_FILE_NAME                       _T("missing-file-name")
#define KEY_ERROR_FILE_NAME                         _T("error-file-name")
#define KEY_ERROR_CODE                              _T("error-code")
#define KEY_DEVICE_ID                               _T("device-id")
#define KEY_MSG_TEXT_INDEX                          _T("msg-text-index")
#define KEY_MSG_TEXT                                _T("msg-text")
#define KEY_INITIALIZE_FAILURE_TEXT                 _T("failure-text")
#define KEY_MAXIMUM_SINGLE_QUANTITY                 _T("maximum-single-quantity")
#define KEY_UPC                                     _T("upc")
#define KEY_UPC_ENTRY                               _T("upc-entry")
#define KEY_UPC_ENTRY_SCAN                          _T("scan")
#define KEY_UPC_ENTRY_PICK                          _T("pick")
#define KEY_UPC_ENTRY_KEYED                         _T("keyed")
#define KEY_ITEM_ID                                 _T("item-id")
#define KEY_ITEM_SCAN_TYPE                          _T("scan-type")
#define KEY_ITEM_DETAILS_DESCRIPTION                _T("item-description")
#define KEY_ITEM_QUANTITY                           _T("quantity")
#define KEY_ITEM_DETAILS_SELLABLE                   _T("sellable")
#define KEY_ITEM_DETAILS_NOT_SELLABLE               _T("not-sellable")
#define KEY_ITEM_DETAILS_DEPT_ID                    _T("dept-id")
#define KEY_ITEM_DETAILS_WT_REQUIRED                _T("wt-required")
#define KEY_ITEM_DETAILS_ZERO_WT                    _T("zero-wt")
#define KEY_ITEM_DETAILS_BAGGING                    _T("bagging-required")
#define KEY_ITEM_DETAILS_NO_BAGGING                 _T("no-bagging-required")
#define KEY_ITEM_DETAILS_SUBSTITUTION_CHECK         _T("substitution-check-required")
#define KEY_ITEM_DETAILS_NO_SUBSTITUTION_CHECK      _T("no-substitution-check-required")
#define KEY_ITEM_DETAILS_WEIGHT                     _T("weight")
#define KEY_ITEM_DETAILS_GROSS_WEIGHT               _T("gross-weight")
#define KEY_ITEM_DETAILS_FLAGS                      _T("flags")
#define KEY_ITEM_DETAILS_PRICE_EMBEDDED             _T("price-embedded")
#define KEY_ITEM_DETAILS_PRICE_EMBEDDED_WT          _T("price-embedded-wt")
#define KEY_ITEM_DETAILS_TOLERANCE                  _T("tolerance")
#define KEY_ITEM_DETAILS_STD_DEVIATION              _T("std-deviation")
#define KEY_ITEM_DETAILS_DIMENSIONS                 _T("dimensions")
#define KEY_ITEM_DETAILS_ZEROUNITPRICE              _T("zero-unitprice")  //Tar400229
#define KEY_METRIC                                  _T("metric")
#define KEY_TOLERANCE_TYPE                          _T("tolerance-type")
#define KEY_TOLERANCE_TYPE_STD_DEV                  _T("std-dev")
#define KEY_TOLERANCE_TYPE_DEFAULT                  _T("default")
#define KEY_TOLERANCE_TYPE_FIXED                    _T("fixed")
#define KEY_TOLERANCE_TYPE_UNKNOWN                  _T("unknown")
#define KEY_TOLERANCE                               _T("tolerance")

#define KEY_OPERATION                               _T("operation")

#define KEY_IV_OPERATION                            KEY_OPERATION
#define KEY_IV_OPERATION_BEGIN                      _T("begin")
#define KEY_IV_OPERATION_END                        _T("end")
#define KEY_IV_ITEM_ID                              KEY_ITEM_ID
#define KEY_IV_QUANTITY                             KEY_ITEM_QUANTITY
#define KEY_IV_UPC                                  KEY_UPC

#define KEY_IZ_LANE_NUMBER                          _T("lane-number")

#define KEY_TC_OPERATION_BEGIN                      _T("begin")
#define KEY_TC_OPERATION_END                        _T("end")
#define KEY_TC_OPERATION_VOID_BEGIN                 _T("void-begin")
#define KEY_TC_OPERATION_VOID_END                   _T("void-end")
#define KEY_TC_OPERATION_FINALIZE                   _T("finalize")
#define KEY_TC_OPERATION_CONTINUE                   _T("continue")
#define KEY_TC_OPERATION_EXPIRED                    _T("expired")
#define KEY_TC_OPERATION_BALANCE_PAID				_T("balance-paid")

#define KEY_AC_OPERATION                            KEY_OPERATION
#define KEY_AC_SKIP_BAGGING                         _T("skip-bagging")
#define KEY_AC_ITEM_REMOVED                         _T("item-removed")
#define KEY_AC_BAG_BOX                              _T("bag-box")
#define KEY_AC_DONE_BTTN                            _T("done-bttn")//SR085
#define KEY_AC_CUST_HAS_BAG                         _T("customer-has-bag")//SR085
#define KEY_AC_APPROVE_WT                           _T("approve-wt")
#define KEY_AC_SET_APP_STATE                        _T("set-app-state")
#define KEY_AC_SET_APP_STATE_BEGIN                  _T("begin")
#define KEY_AC_SET_APP_STATE_END                    _T("end")
#define KEY_AC_HOLD                                 _T("hold")
#define KEY_AC_RESUME                               _T("resume")
#define KEY_AC_ITEM_EXPIRED                         _T("item-expired")
#define KEY_AC_CLEAR_BAG_AREA_BACKUP                _T("clear-bag-area-backup")
#define KEY_AC_SKIP_BAG_BTTN_ENABLE					_T("skip-bagging-enable") //SR778

#define KEY_EXCEPTION_ID                            _T("exception-id")
#define KEY_EXCEPTION_CATEGORY                      _T("category")
#define KEY_EXCEPTION_CATEGORY_WT_CHANGES           _T("wt-changes")
#define KEY_EXCEPTION_CATEGORY_IMMEDIATE_WT_CHANGES _T("immediate-wt-changes")
#define KEY_EXCEPTION_CATEGORY_THRESHOLD            _T("threshold")
#define KEY_EXCEPTION_CATEGORY_MISMATCH             _T("mismatch")
#define KEY_EXCEPTION_AUTHORIZATION                 _T("authorization")
#define KEY_EXCEPTION_AUTHORIZATION_AUTO            _T("auto")
#define KEY_EXCEPTION_AUTHORIZATION_RAP             _T("RAP")
#define KEY_EXCEPTION_AUTHORIZATION_LANE            _T("Lane")
#define KEY_EXCEPTION_OPERATION                     KEY_OPERATION
#define KEY_EXCEPTION_OPERATION_IGNORE              _T("ignore")
#define KEY_EXCEPTION_OPERATION_CLEAR               _T("clear")
#define KEY_EXCEPTION_OPERATION_CLEAR_DELAYED		_T("clear-delayed")
#define KEY_EXCEPTION_OPERATION_CLEAR_WITH_UPDATE   _T("clear-with-update")
#define KEY_EXCEPTION_OPERATION_CLEAR_IF_ITEM_MATCH _T("clear-if-item-match")
#define KEY_EXCEPTION_OPERATION_DEFER               _T("defer")

#define KEY_EV_APPLICATION_STATE					_T("Application-State")
#define KEY_SMM_UPC                                 KEY_UPC
#define KEY_SMM_OPERATION                           KEY_OPERATION
#define KEY_SMM_OPERATION_BEGIN                     _T("begin")
#define KEY_SMM_OPERATION_END                       _T("end")
#define KEY_SMM_OPERATION_QUERY                     _T("query")
#define KEY_SMM_OPERATION_ADD_WEIGHT                _T("add-weight")
#define KEY_SMM_OPERATION_ADD_ZERO                  _T("add-zero")
#define KEY_SMM_OPERATION_ADD_TOLERANCE             _T("add-tolerance")
#define KEY_SMM_OPERATION_DELETE_TOLERANCE          _T("delete-tolerance")
#define KEY_SMM_OPERATION_DELETE_ALL_WEIGHTS        _T("delete-all-weights")
#define KEY_SMM_OPERATION_CHANGE_WT_TOLERANCE       _T("change-wt-tolerance")
#define KEY_SMM_OPERATION_CHANGE_WT_REQUIRED        _T("change-wt-required")
#define KEY_SMM_OPERATION_CHANGE_SUBSTITUTION_CHECK _T("change-substitution-check")
#define KEY_SMM_TOLERANCE                           _T("tolerance")
#define KEY_SMM_TOLERANCE_TYPE                      _T("tolerance-type")
#define KEY_SMM_TOLERANCE_TYPE_GLOBAL               _T("global")
#define KEY_SMM_TOLERANCE_TYPE_ITEM                 _T("item")
#define KEY_SMM_TOLERANCE_TYPE_DEPARTMENT           _T("department")
#define KEY_SMM_FLAG_VALUE                          _T("flag-value")
#define KEY_SMM_FLAG_VALUE_DEFAULT                  _T("default")
#define KEY_SMM_PRICE_EMBEDDED                      _T("price-embedded")
#define KEY_SMM_PPWU                                _T("price-per-wt-unit")
#define KEY_SMM_OPERATION_CANCEL					_T("cancel")

#define KEY_SC_OPERATION                            KEY_OPERATION
#define KEY_SC_OPERATION_GET                        _T("get")
#define KEY_SC_OPERATION_SET                        _T("set")
#define KEY_SC_OPERATION_ABORT                      _T("abort")
#define KEY_SC_OPERATION_REINITIALIZE               _T("reinitialize")
#define KEY_SC_OPERATION_SERIALIZE                  _T("serialize")
#define KEY_SC_OPERATION_RELOAD_OPTIONS             _T("reload-options")
#define KEY_SC_OPERATION_QUERY_DEVICE_STATUS        _T("query-device-status")
#define KEY_SC_OPERATION_ASSIST_MODE_STOP           _T("stop-assist-mode")
#define KEY_SC_OPERATION_SUSPEND_ITEM               _T("suspend-item")
#define KEY_SC_OPERATION_FLUSH_ITEM                 _T("flush-item")
#define KEY_SC_PARAM                                _T("param")
#define KEY_SC_DEVICE_NAME                          _T("device-name")
#define KEY_SC_DEVICE_STATUS                        _T("device-status")
#define KEY_SC_DEVICE_STATUS_WORKING                _T("working")
#define KEY_SC_DEVICE_STATUS_NOT_WORKING            _T("not-working")
#define KEY_SC_RELOAD_OPTIONS_STATUS_TEXT			_T("reload-options-status-text")

#define KEY_OSE_EXCEPTION_ID                        _T("exception-id")
#define KEY_OSE_ITEM_ID                             _T("item-id")
#define KEY_OSE_ITEM_DESCRIPTION                    _T("item-description")
#define KEY_OSE_WEIGHT_DELTA                        _T("weight-delta")
#define KEY_OSE_WEIGHT_ACTUAL                       _T("weight-actual")
#define KEY_OSE_WEIGHT_EXPECTED                     _T("weight-expected")
#define KEY_OSE_TOLERANCE_TYPE                      KEY_TOLERANCE_TYPE
#define KEY_OSE_TOLERANCE_TYPE_STD_DEV              KEY_TOLERANCE_TYPE_STD_DEV
#define KEY_OSE_TOLERANCE_TYPE_DEFAULT              KEY_TOLERANCE_TYPE_DEFAULT
#define KEY_OSE_TOLERANCE_TYPE_FIXED                KEY_TOLERANCE_TYPE_FIXED
#define KEY_OSE_TOLERANCE                           KEY_TOLERANCE
#define KEY_OSE_WEIGHT_2X                           _T("weight-2x")
#define KEY_OSE_EXC_TYPE                            _T("exception-type")
#define KEY_OSE_EXC_TYPE_MISMATCHED                 _T("mismatched")
#define KEY_OSE_EXC_TYPE_UNEX_INCREASE              _T("unexpected-increase")
#define KEY_OSE_EXC_TYPE_UNEX_DECREASE              _T("unexpected-decrease")
#define KEY_OSE_EXC_TYPE_SKIP_BAGGING_THRESHOLD     _T("skip-bagging-threshold")
#define KEY_OSE_EXC_TYPE_ITEM_REMOVED_THRESHOLD     _T("item-removed-threshold")
#define KEY_OSE_EXC_TYPE_BAG_BOX_THRESHOLD          _T("bag-box-threshold")
#define KEY_OSE_EXC_TYPE_NEW_ITEM_THRESHOLD         _T("new-item-threshold")
#define KEY_OSE_EXC_TYPE_ZERO_WT_THRESHOLD          _T("zero-wt-threshold")
#define KEY_OSE_EXC_TYPE_INVALID_BAG                _T("invalid-bag")//SR085
#define KEY_OSE_EXCEPTION_MSG_INDEX                 _T("exception-msg-index")
#define KEY_OSE_TRACE_DESCRIPTION                   _T("trace-description")
#define KEY_OSE_DISPOSITION                         _T("disposition")
#define KEY_OSE_DISPOSITION_DELAYED                 _T("delayed")
#define KEY_OSE_DISPOSITION_FORGIVEN                _T("forgiven")
#define KEY_OSE_DISPOSITION_OUTSTANDING             _T("outstanding")
#define KEY_OSE_DISPOSITION_CLEARED                 _T("cleared")
#define KEY_OSE_DISPOSITION_NO_INTERVENTION_REQ     _T("no-intervention-required")
#define KEY_OSE_METRIC                              KEY_METRIC
#define KEY_OSE_THRESHOLD_MAX						_T("threshold-max")
#define KEY_OSE_CLEAR_RAP_APPROVAL                  _T("clear-rap-approval")
#define KEY_OSE_MULTI_PACK_ITEM                     _T("multi-pack-item")
#define KEY_OSEC_EXCEPTION_ID                       _T("exception-id")

#define KEY_OSM_OPERATION                           KEY_OPERATION
#define KEY_OSM_OPERATION_QUERY                     _T("query")
#define KEY_OSM_OPERATION_UPDATE                    _T("update")
#define KEY_OSM_UPC                                 _T("upc")
#define KEY_OSM_WEIGHT_COUNT                        _T("weight-count")
#define KEY_OSM_WEIGHT                              _T("weight")
#define KEY_OSM_OBSERVED_WEIGHT                     _T("observed-weight")
#define KEY_OSM_TOLERANCE_TYPE                      KEY_TOLERANCE_TYPE
#define KEY_OSM_TOLERANCE_TYPE_STD_DEV              KEY_TOLERANCE_TYPE_STD_DEV
#define KEY_OSM_TOLERANCE_TYPE_DEFAULT              KEY_TOLERANCE_TYPE_DEFAULT
#define KEY_OSM_TOLERANCE_TYPE_FIXED                KEY_TOLERANCE_TYPE_FIXED
#define KEY_OSM_TOLERANCE                           KEY_TOLERANCE
#define KEY_OSM_METRIC                              KEY_METRIC
#define KEY_OSM_MIN_TOLERANCE                       _T("min-tolerance")
#define KEY_OSM_MAX_TOLERANCE                       _T("max-tolerance")
#define KEY_OSM_SUBSTITUTION_CHECK                  _T("substitution-check")
#define KEY_OSM_WT_REQUIRED                         _T("wt-required")
#define KEY_OSM_TALLY                               _T("tally")
#define KEY_OSM_APPROVED                            _T("approved")
#define KEY_OSM_DATE                                _T("date")
#define KEY_OSM_YES                                 _T("y")
#define KEY_OSM_NO                                  _T("n")
#define KEY_OSM_DEFAULT_YES                         _T("default-y")
#define KEY_OSM_DEFAULT_NO                          _T("default-n")
#define KEY_OSM_ERROR_TYPE							_T("error-type")
#define KEY_OSM_ERROR_TYPE_INVALID_TOLERANCE        _T("invalid-tolerance")
#define KEY_OSM_ERROR_TYPE_SERVER_OFFLINE           _T("server-offline")
#define KEY_OSM_SERVER_STATUS						_T("server-status")
#define KEY_OSM_SERVER_STATUS_OFFLINE               _T("offline")
#define KEY_OSM_SERVER_STATUS_ONLINE                _T("online")

#define KEY_ODF_TYPE                                _T("type")
#define KEY_ODF_DEVICE_NAME                         _T("device-name")
#define KEY_ODF_DEVICE_ID                           _T("device-id")
#define KEY_ODF_ERROR_CODE                          _T("error-code")
#define KEY_ODF_EXT_ERROR_CODE                      _T("ext-error-code")
#define KEY_ODF_ERROR_TEXT                          _T("error-text")

#define KEY_OAC_OPERATION                           KEY_OPERATION
#define KEY_OAC_OPERATION_SKIP_BAGGING              _T("skip-bagging")
#define KEY_OAC_OPERATION_ITEM_REMOVED              _T("item-removed")
#define KEY_OAC_OPERATION_BAG_BOX                   _T("bag-box")
#define KEY_OAC_TARE                                _T("tare")
#define KEY_OAC_TARE_WEIGHTED_ITEM                  _T("weighted-item")
#define KEY_OAC_TARE_TARE_ONLY                      _T("tare-only")
#define KEY_OAC_TARE_WLDB_ITEM                      _T("wldb-item")
#define KEY_OAC_TARE_NONE                           _T("none")
#define KEY_OAC_OPERATION_CUST_BAG                  _T("customer-bag")//SR085
#define KEY_OAC_TARE_VALID_CUST_BAG                 _T("valid-customer-bag")//SR085
#define KEY_OAC_TARE_INVALID_CUST_BAG                 _T("invalid-customer-bag")//SR085

#define KEY_OSC_OPERATION                           KEY_OPERATION
#define KEY_OSC_OPERATION_UNEX_INCREASE             _T("unexpected-increase")
#define KEY_OSC_OPERATION_DISABLE_SCANNER           _T("disable-scanner")
#define KEY_OSC_OPERATION_ENABLE_SCANNER            _T("enable-scanner")
#define KEY_OSC_OPERATION_BAG_AREA_BACKUP           _T("bag-area-backup")
#define KEY_OSC_OPERATION_BAG_AREA_CLEAR            _T("bag-area-clear")
#define KEY_OSC_OPERATION_ASSIST_MODE_STOP_COMPLETE _T("stop-assist-mode-complete")
#define KEY_OSC_OPERATION_SKIP_BAGGING_ALLOWED      _T("skip-bagging-allowed")
#define KEY_OSC_OPERATION_SKIP_BAGGING_DISALLOWED   _T("skip-bagging-disallowed")
#define KEY_OSC_WEIGHT_OBSERVED                     _T("weight-observed")
#define KEY_OSC_OPERATION_GOTO_SCANANDBAG           _T("goto-scanandbag") //410299

#define KEY_OIL_EXCEPTION_ID                        _T("exception-id")
#define KEY_OIL_WEIGHT                              _T("weight")
#define KEY_OIL_TEXT                                _T("text")

#define KEY_DEVICES_CLASS                           _T("class")
#define KEY_DEVICES_TYPE                            _T("type")
#define KEY_DEVICES_UNIT                            _T("unit")
#define KEY_DEVICES_STATUS                          _T("status")
#define KEY_DEVICES_STATE                           _T("state")
#define KEY_DEVICES_ID                              _T("id")

#define VALUE_DEVICE_CONFIGURATION                  _T("Configuration")
#define VALUE_DEVICE_WTINFO                         _T("WTInfo")
#define VALUE_DEVICE_SS                             _T("SmartScale")
#define VALUE_DEVICE_DBMGRCLIENT                    _T("DBMgrClient")
#define VALUE_DEVICE_TAKEAWAY_BELT                  _T("TakeawayBelt")
#define VALUE_DEVICE_ALL                            _T("*")

#define SM_MSG_CONFIGURATION_LOADED                 896
#define SM_MSG_WTINFO_LOADED                        890
#define SM_MSG_SS_LOADED                            893
#define SM_MSG_DBMGRCLIENT_LOADED                   1344
#define SM_MSG_TAKEAWAY_BELT_LOADED                 983
#define SM_MSG_INITIALIZATION_COMPLETE              1344
#define SM_MSG_CONFIGURATION_UNLOADED               2000
#define SM_MSG_WTINFO_UNLOADED                      2001
#define SM_MSG_SS_UNLOADED                          2002
#define SM_MSG_DBMGRCLIENT_UNLOADED                 2003
#define SM_MSG_TAKEAWAY_BELT_UNLOADED               2004
