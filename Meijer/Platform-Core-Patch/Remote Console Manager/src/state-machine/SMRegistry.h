#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMRegistry.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)RCMRegistry.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/SMRegistry.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 1     10/29/04 9:24a Dm185016
*/

#define HKEY_SM                                     _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\StateMachine")

#define REG_VALUE_ON_MISSING_HANDLER                _T("Missing.Handler")

#define VALUE_HANDLER_FAIL                          _T("fail")
#define VALUE_HANDLER_PASS                          _T("pass")
