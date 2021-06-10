#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RAConstants.h 3     1/12/10 11:56p Cu185008 $
/*
 * @(#)RCMConstants.h    $Revision: 3 $ $Date: 1/12/10 11:56p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RAConstants.h $
 * 
 * 3     1/12/10 11:56p Cu185008
 * G2 Migration.
 * 
 * 1     3/01/05 2:04p Dm185016
 * TAR 294587.  Added application state functionality.
*/
#include "RCMConstants.h"

//-----------------------------------------------------------------------------
// Constants for XPath expressions
#define XP_APP_STATE_LIST               XP_STATE_MACHINE _T("/application-state-list")
#define XP_APP_STATE_DEFINITION         XP_APP_STATE_LIST _T("/application-state")
#define XP_APP_STATE_DEF_STATE_NAME     XP_APP_STATE_DEFINITION _T("/state-name")
#define XP_APP_STATE_DEF_COMMENT        XP_APP_STATE_DEFINITION _T("/comment")
#define XP_APP_STATE_DEF_PUSH_AM        XP_APP_STATE_DEFINITION _T("/push-assist-mode")
#define XP_APP_STATE_DEF_ENABLE_AM      XP_APP_STATE_DEFINITION _T("/enable-assist-mode")
#define XP_APP_STATE_DEF_DISABLE_AM     XP_APP_STATE_DEFINITION _T("/disable-assist-mode")
#define XP_APP_STATE_EXIT_RULE_LIST     XP_APP_STATE_DEFINITION _T("/exit-rule-list")
#define XP_APP_STATE_ENTRY_RULE_LIST    XP_APP_STATE_DEFINITION _T("/entry-rule-list")
