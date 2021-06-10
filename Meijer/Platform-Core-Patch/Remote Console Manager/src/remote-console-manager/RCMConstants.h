#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMConstants.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMConstants.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMConstants.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 6     5/27/08 10:37a Bc185047
 * RCM Patch G Issue – Eliminate RCM’s dependence on XML format
 * 
 * Removed unecessary hardcoded tag definitions.
 * 
 * 3     2/07/05 7:17p Dm185016
 * Handler for new <HHInstructionText> tag.
 * 
 * 2     1/27/05 2:37p Dm185016
 * New keyword in LaneRAP.xml.
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 6     1/13/05 3:08p Dm185016
* New keyword added to LaneRAP.xml
* 
* 5     1/07/05 10:48a Dm185016
* TAR 288789
* 
* 4     11/19/04 1:37p Dm185016
* Added Strikethrough xpath expression
* 
* 3     11/16/04 11:29a Dm185016
* Adedd context switch table
* 
* 2     11/04/04 10:52a Dm185016
* More LaneRAP.xml changes
* 
* 1     10/29/04 8:55a Dm185016
*/
#include "SMConstants.h"

//-----------------------------------------------------------------------------
// Constants for XPath expressions
#define XP_LANE_CONFIGS					XP_STATE_MACHINE _T("/lane-configurations")
#define XP_LANE_CONFIGS_LANE_CONFIG		XP_LANE_CONFIGS _T("/lane-configuration")

#define XP_CONTEXT_SWITCH_LIST          XP_STATE_MACHINE _T("/context-switch-list")
#define XP_CONTEXT_SWITCH_DEFINITION    XP_CONTEXT_SWITCH_LIST _T("/context-switch")
#define XP_CONTEXT_SWITCH_NAME          XP_CONTEXT_SWITCH_DEFINITION _T("/context-name")
#define XP_CONTEXT_SWITCH_COMMENT       XP_CONTEXT_SWITCH_DEFINITION _T("/comment")
#define CONTEXT_SWITCH_PUSH_IC          _T("push-input-controls-setting")
#define XP_CONTEXT_SWITCH_PUSH_IC       XP_CONTEXT_SWITCH_DEFINITION _T("/") CONTEXT_SWITCH_PUSH_IC
#define CONTEXT_SWITCH_DISABLE_IC       _T("disable-input-controls")
#define XP_CONTEXT_SWITCH_DISABLE_IC    XP_CONTEXT_SWITCH_DEFINITION _T("/") CONTEXT_SWITCH_DISABLE_IC
#define CONTEXT_SWITCH_ENABLE_IC        _T("enable-input-controls")
#define XP_CONTEXT_SWITCH_ENABLE_IC     XP_CONTEXT_SWITCH_DEFINITION _T("/") CONTEXT_SWITCH_ENABLE_IC

#define XP_DISPLAY                      XP_STATE_MACHINE _T("/Display")
#define XP_DISPLAY_INCLUDES             XP_DISPLAY _T("/Includes")
#define XP_DISPLAY_INCLUDES_INCLUDE     XP_DISPLAY_INCLUDES _T("/Include")

#define ATTR_LANE_CONFIGS_LANE_CONFIG_NUMBER_OF_LANES			L"number-of-lanes"
#define ATTR_LANE_CONFIGS_LANE_CONFIG_NUMBER_OF_LANES_LENGTH	(sizeof(ATTR_LANE_CONFIGS_LANE_CONFIG_NUMBER_OF_LANES)/sizeof(wchar_t)-1)
#define ATTR_LANE_CONFIGS_LANE_CONFIG_CONTEXT_NAME				L"context-name"
#define ATTR_LANE_CONFIGS_LANE_CONFIG_CONTEXT_NAME_LENGTH		(sizeof(ATTR_LANE_CONFIGS_LANE_CONFIG_CONTEXT_NAME)/sizeof(wchar_t)-1)
#define ATTR_DISPLAY_CONTROLS_CONTROL_NAME              		L"Name"
#define ATTR_DISPLAY_CONTROLS_CONTROL_NAME_LENGTH       		(sizeof(ATTR_DISPLAY_CONTROLS_CONTROL_NAME)/sizeof(wchar_t)-1)
#define ATTR_DISPLAY_CONTROLS_CONTROL_TYPE              		L"Type"
#define ATTR_DISPLAY_CONTROLS_CONTROL_TYPE_LENGTH       		(sizeof(ATTR_DISPLAY_CONTROLS_CONTROL_TYPE)/sizeof(wchar_t)-1)
#define ATTR_DISPLAY_CONTROLS_CONTROL_TYPE_BUTTONLIST   		_T("ButtonList")
#define ATTR_DISPLAY_CONTROLS_CONTROL_TYPE_BUTTON       		_T("Button")