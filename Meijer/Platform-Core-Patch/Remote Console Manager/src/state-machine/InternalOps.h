#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/InternalOps.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)InternalOps.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/InternalOps.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     5/31/05 11:35a Dm185016
 * TAR 301203
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 8     10/29/04 9:22a Dm185016
* Added goto-state action.
* 
* 6     6/11/04 10:56a Dm185016
* New wait-timer action.
* 
* 5     4/30/04 1:02p Dm185016
* Added send-msg
* 
* 4     3/25/04 3:15p Dm185016
* New push-state pop-state
* 
* 3     2/10/04 11:41a Dm185016
* New set-return-code internal action
* 
* 2     1/27/04 2:00p Dm185016
* resend-msg Internal action
* 
* 1     1/16/04 9:04a Dm185016
* New file containing definitions of the internal functions
*/

#define INCREMENT_VARIABLE      _T("increment-variable")
#define DECREMENT_VARIABLE      _T("decrement-variable")
#define ASSIGN_VARIABLE         _T("assign-variable")
#define EVALUATE_EXPRESSION     _T("evaluate-expression")
#define START_TIMER             _T("start-timer")
#define STOP_TIMER              _T("stop-timer")
#define WAIT_TIMER              _T("wait-timer")
#define IS_TIMER_TIMING         _T("is_timer_timing")
#define RESEND_MSG              _T("resend-msg")
#define SEND_MSG                _T("send-msg")
#define SET_RETURN_CODE         _T("set-return-code")
#define PUSH_STATE              _T("push-state")
#define POP_STATE               _T("pop-state")
#define GOTO_STATE              _T("goto-state")
#define CLEAN_STATE              _T("clean-state")
