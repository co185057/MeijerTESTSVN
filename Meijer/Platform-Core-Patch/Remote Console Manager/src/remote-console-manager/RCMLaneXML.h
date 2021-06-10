#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMLaneXML.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMLaneXML.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMLaneXML.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 2     2/04/05 9:39a Dm185016
 * TAR 292311
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 4     11/30/04 4:23p Dm185016
* Added new type for rap only.
* 
* 3     11/16/04 11:32a Dm185016
* New state variables
* 
* 2     10/29/04 8:58a Dm185016
* 
* 1     10/14/04 1:35p Dm185016
*/

#define COLOR_BLACK         _T("Black")
#define COLOR_YELLOW        _T("Yellow")
#define COLOR_WHITE         _T("White")
#define COLOR_RED           _T("Red")
#define COLOR_BLUE          _T("Blue")
#define COLOR_PURPLE        _T("Purple")
#define COLOR_GRAY          _T("Gray")
#define COLOR_DARKGRAY      _T("DarkGray")

#define COLOR_TEXT_BLACK            _T("Black")
#define COLOR_TEXT_WHITE            _T("White")

#define COLOR_BACKGROUND_RED        _T("Red")
#define COLOR_BACKGROUND_YELLOW     _T("Yellow")
#define COLOR_BACKGROUND_GREEN      _T("Green")
#define COLOR_BACKGROUND_GREY       _T("Gray")

const _TCHAR RECEIPT_VAR_ITEM_MSG[]         = _T("ITEM_RAP_ONLY_HILITE");
const _TCHAR TOP_LIST_ITEM_MSG[]            = _T("ITEM_SMTEXT");
const _TCHAR RECEIPT_VAR_ITEM_SELECTABLE[]  = _T("ITEM_SELECTABLE");

//-----------------------------------------------------------------------------
// Transaction Variables defined in LaneRAP.xml
const _TCHAR XACT_VARIABLE_ECHO[]           = _T("Echo");
const _TCHAR XACT_VARIABLE_LANE_STATE_1[]   = _T("LaneStateWindow1");
const _TCHAR XACT_VARIABLE_LANE_STATE_2[]   = _T("LaneStateWindow2");
const _TCHAR XACT_VARIABLE_LANE_STATE_3[]   = _T("LaneStateWindow3");
const _TCHAR XACT_VARIABLE_ITEM_AMOUNT[]    = _T("ItemAmount");
const _TCHAR XACT_VARIABLE_TOTAL_AMOUNT[]   = _T("SMTotalAmount");

//-----------------------------------------------------------------------------
// String table ids
const _TCHAR STRING_TABLE_INVALID_BIRTHDATE_FORMAT[]    = _T("Echo");

