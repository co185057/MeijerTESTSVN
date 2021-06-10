#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMConstants.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)SMConstants.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMConstants.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 11    5/31/05 11:36a Dm185016
 * String variables in new Data Model Support
 * 
 * 9     5/03/05 4:05p Dm185016
 * Cleaned up tracing of state machine
 * 
 * 8     4/22/05 8:13a Dm185016
 * New DataCap constant to control miscellaneous tracing in State Machine.
 * 
 * 6     3/28/05 5:08p Dm185016
 * Lint
 * 
 * 5     3/17/05 2:44p Dm185016
 * <if>, <else-if>, <else> support
 * 
 * 4     3/03/05 3:17p Dm185016
 * TAR 291700 - Added values for value-type
 * 
 * 3     2/28/05 3:53p Dm185016
 * lint
 * 
 * 2     2/14/05 7:09p Dm185016
 * Release Exclusive Access Event support
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 13    12/22/04 11:19a Dm185016
* Added <otherwise> tag.
* 
* 12    11/09/04 3:34p Dm185016
* Support for state lookup tables
* 
* 11    10/29/04 9:22a Dm185016
* Added <select-action>
* 
* 10    10/04/04 6:40p Dm185016
* Added beginnings of <if> and <else> tags
* 
* 8     6/11/04 10:58a Dm185016
* Data capture values.
* 
* 7     4/30/04 1:02p Dm185016
* Deleted unused tags
* 
* 6     4/21/04 3:09p Dm185016
* Add unicode fixes.  Added //include
* 
* 5     2/24/04 3:33p Dm185016
* Separated out message definitions, since they are application specific
* 
* 4     2/10/04 11:42a Dm185016
* New action format
* 
* 3     1/27/04 1:59p Dm185016
* Action Library support
* 
* 2     1/16/04 9:08a Dm185016
* New Action format, Timers, Constants
* 
* 1     1/06/04 8:58a Dm185016
* First version
*/

//-----------------------------------------------------------------------------
// This defines the namespace name for all components of the State Machine
#define STATE_MACHINE_NAMESPACE StateMachine
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Constants for XPath expressions
#define XP_STATE_MACHINE                 _T("/state-machine")

#define XP_INCLUDE                       _T("//include")

#define XP_LIBRARY_LIST                 XP_STATE_MACHINE _T("/library-list")
#define XP_LIBRARY                      XP_LIBRARY_LIST _T("/library")
#define XP_LIBRARY_NAME                 XP_LIBRARY _T("/library-name")
#define XP_LIBRARY_COMMENT              XP_LIBRARY _T("/comment")

#define XP_VAR_LIST                     XP_STATE_MACHINE _T("/variable-list")
#define XP_VARIABLE                     XP_VAR_LIST _T("/variable")
#define XP_VAR_NAME                     XP_VARIABLE _T("/var-name")
#define XP_VAR_COMMENT                  XP_VARIABLE _T("/comment")
#define XP_VAR_INITIAL_VALUE            XP_VARIABLE _T("/initial-value")
#define XP_VAR_MAX_VALUE                XP_VARIABLE _T("/max-value")
#define XP_VAR_MIN_VALUE                XP_VARIABLE _T("/min-value")

#define XP_CONSTANT_LIST                XP_STATE_MACHINE _T("/constant-list")
#define XP_CONSTANT                     XP_CONSTANT_LIST _T("/constant")
#define XP_CONSTANT_NAME                XP_CONSTANT _T("/constant-name")
#define XP_CONSTANT_COMMENT             XP_CONSTANT _T("/comment")
#define XP_CONSTANT_VALUE               XP_CONSTANT _T("/constant-value")

#define XP_TIMER_LIST                XP_STATE_MACHINE _T("/timer-list")
#define XP_TIMER                     XP_TIMER_LIST _T("/timer")
#define XP_TIMER_NAME                XP_TIMER _T("/timer-name")
#define XP_TIMER_COMMENT             XP_TIMER _T("/comment")

#define XP_STATE_LIST                   XP_STATE_MACHINE _T("/state-list")
#define XP_STATE_DEFINITION             XP_STATE_LIST _T("/state-definition")
#define XP_STATE_NAME                   XP_STATE_DEFINITION _T("/state-name")
#define XP_STATE_COMMENT                XP_STATE_DEFINITION _T("/comment")
#define XP_STATE_INITIAL                XP_STATE_DEFINITION _T("/initial")

#define XP_STATE_TABLE                  XP_STATE_MACHINE _T("/state-table")
#define XP_STATE_TABLE_STATE            XP_STATE_TABLE _T("/state")

#define XP_LOOKUP_TABLE                 XP_STATE_MACHINE _T("/lookup-table")
#define XP_LOOKUP_TABLE_LOOKUP_ENTRY    XP_LOOKUP_TABLE _T("/lookup-entry")

#define XP_MSG_LIST                     XP_STATE_MACHINE _T("/msg-list")
#define XP_MSG_DEFINITION               XP_MSG_LIST _T("/msg-definition")
#define XP_MSG_NAME                     XP_MSG_DEFINITION _T("/msg-name")
#define XP_MSG_COMMENT                  XP_MSG_DEFINITION _T("/comment")
#define XP_MSG_VALUE                    XP_MSG_DEFINITION _T("/msg-value")

#define XP_ACTION_LIST                  XP_STATE_MACHINE _T("/action-list")
#define XP_ACTION                       XP_ACTION_LIST _T("/action")
#define XP_ACTION_NAME                  XP_ACTION _T("/action-name")
#define XP_ACTION_COMMENT               XP_ACTION _T("/comment")

#define XP_RULE_LIST                                XP_STATE_MACHINE _T("/rules")
#define XP_RULE_STATE                               XP_RULE_LIST _T("/state")
#define XP_RULE_STATE_NAME                          XP_RULE_STATE _T("/state-name")
#define XP_RULE_STATE_GLOBAL                        XP_RULE_STATE _T("/global")

#define XP_RULE_STATE_EXIT_RULE_LIST                XP_RULE_STATE _T("/exit-rule-list")
#define XP_RULE_STATE_ENTRY_RULE_LIST               XP_RULE_STATE _T("/entry-rule-list")
#define XP_RULE_STATE_MSG                           XP_RULE_STATE _T("/message")
#define XP_RULE_STATE_MSG_NAME                      XP_RULE_STATE_MSG _T("/message-name")
#define XP_RULE_STATE_MSG_ACTION_RULE_LIST          XP_RULE_STATE_MSG _T("/message-rule-list")
#define XP_RULE_STATE_MSG_RULE                      _T("//rule")
#define XP_RULE_STATE_MSG_IF                        _T("//if")
#define XP_RULE_STATE_MSG_ELSE                      _T("//else")
#define XP_RULE_STATE_MSG_ELSEIF                    _T("//else-if")
#define XP_RULE_STATE_MSG_SELECT                    _T("//select")
#define XP_RULE_STATE_MSG_SELECT_WHEN               XP_RULE_STATE_MSG_SELECT _T("/when")
#define XP_RULE_STATE_MSG_SELECT_OTHERWISE          XP_RULE_STATE_MSG_SELECT _T("/otherwise")
#define XP_RULE_STATE_MSG_ACTION                    XP_RULE_STATE_MSG_RULE _T("/action")
#define XP_RULE_STATE_MSG_ACTION_NAME               XP_RULE_STATE_MSG_ACTION _T("/action-name")
#define XP_RULE_STATE_MSG_ACTION_PARM               XP_RULE_STATE_MSG_ACTION _T("/parameter")
#define XP_RULE_STATE_MSG_ACTION_PARM_VALUE         XP_RULE_STATE_MSG_ACTION_PARM _T("/value")
#define XP_RULE_STATE_MSG_ACTION_FAILURE_TRANS      XP_RULE_STATE_MSG_RULE _T("/failure-transition")
#define XP_RULE_STATE_MSG_ACTION_FAILURE_ACTION     XP_RULE_STATE_MSG_RULE _T("/on-failure-action")
#define XP_RULE_STATE_MSG_ACTION_EXC_TRANS          XP_RULE_STATE_MSG_RULE _T("/exception-transition")
#define XP_RULE_STATE_MSG_ACTION_EXC_ACTION         XP_RULE_STATE_MSG_RULE _T("/on-exception-action")
#define XP_RULE_STATE_MSG_RULE_SUCCESS_TRANS            XP_RULE_STATE_MSG_ACTION_RULE_LIST _T("/success-transition")
#define XP_RULE_STATE_MSG_RULE_ACTION_FAILURE_TRANS     XP_RULE_STATE_MSG_ACTION_RULE_LIST _T("/failure-transition")
#define XP_RULE_STATE_MSG_RULE_ACTION_FAILURE_ACTION    XP_RULE_STATE_MSG_ACTION_RULE_LIST _T("/on-failure-action")
#define XP_RULE_STATE_MSG_RULE_ACTION_EXC_TRANS         XP_RULE_STATE_MSG_ACTION_RULE_LIST _T("/exception-transition")
#define XP_RULE_STATE_MSG_RULE_ACTION_EXC_ACTION        XP_RULE_STATE_MSG_ACTION_RULE_LIST _T("/on-exception-action")

#define XP_RULE_STATE_MSG_RULE_EXC_ACTION                        XP_RULE_STATE_MSG_RULE _T("/on-exception-action")
#define XP_RULE_STATE_MSG_RULE_EXC_ACTION_NAME                   XP_RULE_STATE_MSG_RULE_EXC_ACTION _T("/action-name")
#define XP_RULE_STATE_MSG_RULE_EXC_ACTION_PARM                   XP_RULE_STATE_MSG_RULE_EXC_ACTION _T("/parameter")
#define XP_RULE_STATE_MSG_RULE_EXC_ACTION_PARM_VALUE             XP_RULE_STATE_MSG_RULE_EXC_ACTION_PARM _T("/value")

#define XP_RULE_STATE_MSG_RULE_FAILURE_ACTION                    XP_RULE_STATE_MSG_RULE _T("/on-failure-action")
#define XP_RULE_STATE_MSG_RULE_FAILURE_ACTION_NAME               XP_RULE_STATE_MSG_RULE_FAILURE_ACTION _T("/action-name")
#define XP_RULE_STATE_MSG_RULE_FAILURE_ACTION_PARM               XP_RULE_STATE_MSG_RULE_FAILURE_ACTION _T("/parameter")
#define XP_RULE_STATE_MSG_RULE_FAILURE_ACTION_PARM_VALUE         XP_RULE_STATE_MSG_RULE_FAILURE_ACTION_PARM _T("/value")

// Attribute names
#define XP_ATTR_EXPRESSION                                          L"expression"
const wchar_t XP_SELECT_ACTION_ATTR_EXPRESSION[]                    = XP_ATTR_EXPRESSION;
const wchar_t XP_STATE_TABLE_ATTR_KEY[]                             = L"key";
const wchar_t XP_STATE_TABLE_ATTR_STATE_NAME[]                      = L"state-name";
const wchar_t XP_STATE_TABLE_ATTR_STATE_TABLE_NAME[]                = L"name";
const wchar_t XP_LOOKUP_TABLE_ATTR_LOOKUP_TABLE_NAME[]              = L"name";
const wchar_t XP_LOOKUP_TABLE_ATTR_KEY_TYPE[]                       = L"key-type";
const wchar_t XP_LOOKUP_TABLE_ATTR_VALUE_TYPE[]                     = L"value-type";
const wchar_t XP_LOOKUP_TABLE_ATTR_VALUE_TYPE_STRING[]              = L"string";
const wchar_t XP_LOOKUP_TABLE_ATTR_VALUE_TYPE_INT[]                 = L"int";
const wchar_t XP_LOOKUP_TABLE_ATTR_ENTRY_VALUE[]                    = L"value";
const wchar_t XP_LOOKUP_TABLE_ATTR_ENTRY_KEY[]                      = L"key";
const wchar_t XP_IF_ACTION_ATTR_EXPRESSION[]                        = XP_ATTR_EXPRESSION;
const wchar_t XP_VARIABLE_NAME_ATTR[]                               = L"name";
const wchar_t XP_VARIABLE_TYPE_ATTR[]                               = L"type";
const _TCHAR XP_VARIABLE_TYPE_ATTR_NONE[]                          = _T("none");
const _TCHAR XP_VARIABLE_TYPE_ATTR_BOOL[]                          = _T("bool");
const _TCHAR XP_VARIABLE_TYPE_ATTR_INT[]                           = _T("int");
const _TCHAR XP_VARIABLE_TYPE_ATTR_LONG[]                          = _T("long");
const _TCHAR XP_VARIABLE_TYPE_ATTR_STRING[]                        = _T("string");

//#define XP_RULE_STATE_MSG_ACTION                     _T("//action")
//#define XP_RULE_STATE_MSG_ACTION_NAME               XP_RULE_STATE_MSG_ACTION _T("/action-name")
//#define XP_RULE_STATE_MSG_ACTION_PARM               XP_RULE_STATE_MSG_ACTION _T("/parameter")
//#define XP_RULE_STATE_MSG_ACTION_PARM_VALUE         XP_RULE_STATE_MSG_ACTION_PARM _T("/value")
//#define XP_RULE_STATE_MSG_ACTION_FAILURE_TRANS      XP_RULE_STATE_MSG_ACTION _T("/failure-transition")
//#define XP_RULE_STATE_MSG_ACTION_FAILURE_ACTION     XP_RULE_STATE_MSG_ACTION _T("/on-failure-action")
//#define XP_RULE_STATE_MSG_ACTION_EXC_TRANS          XP_RULE_STATE_MSG_ACTION _T("/exception-transition")
//#define XP_RULE_STATE_MSG_ACTION_EXC_ACTION         XP_RULE_STATE_MSG_ACTION _T("/on-exception-action")
//#define XP_RULE_STATE_MSG_SUCCESS_TRANS             XP_RULE_STATE_MSG _T("/success-transition")
//#define XP_RULE_STATE_MSG_FAILURE_TRANS             XP_RULE_STATE_MSG _T("/failure-transition")
//#define XP_RULE_STATE_MSG_EXC_TRANS                 XP_RULE_STATE_MSG _T("/exception-transition")
//
//-----------------------------------------------------------------------------


#define MAX_ITF 0xffff

namespace STATE_MACHINE_NAMESPACE
{

const HRESULT ERROR_ACTION_NOT_DEFINED  = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, MAX_ITF);
const HRESULT ERROR_NO_RULES_DEFINED    = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, MAX_ITF-1);

const int INVALID_STATE                 = -1;
const int INVALID_ACTION                = -1;
const int INVALID_MSG                   = -1;


const int REMAIN_THE_SAME_STATE             = INVALID_STATE-1;
const int ANY_STATE                         = 0;

const int NULL_ACTION                       = 0;

const int ANY_MSG                           = INVALID_MSG-1;

//----------------------------------------------------
// Masks available for Data Capture
//
enum DataCaptureMasks
{
    eDCXPath            = 0x00000100,     // Use to suppress xpath tracing during init
    eDCSMEntryExit      = 0x00000200,     // Use to suppress entry/exit tracing
    eDCSMDefault        = 0x00000400,     // Used to suppress all other traces
    eDCSMExecution      = 0x00008000      // Used to suppress all other traces
} ;

}

#define ANY_MSG_TEXT                         _T("*")
#define DEFAULT_GLOBAL_STATE_NAME            _T("$global")
