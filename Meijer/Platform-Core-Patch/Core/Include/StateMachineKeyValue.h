#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/StateMachineKeyValue.h 1     6/23/08 4:55p Sa250050 $
/*
 * @(#)SMKeyValue.h    $Revision: 1 $ $Date: 6/23/08 4:55p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/Include/StateMachineKeyValue.h $
 * 
 * 1     6/23/08 4:55p Sa250050
 * FastLane 4.0 E5.3 Build 291 - Core Source
 * 
 * 3     4/12/05 2:49p Dm185016
 * 
 * 2     4/12/05 11:44a Dm185016
 * TAR 296771
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
* 
* 2     1/10/05 3:56p Dm185016
* Split out common definitions.
* 
* 1     10/04/04 6:36p Dm185016
* Separated out common state machine key words.
*/

#define KEY_MISSING_FILE_NAME                       _T("missing-file-name")
#define KEY_ERROR_FILE_NAME                         _T("error-file-name")
#define KEY_ERROR_CODE                              _T("error-code")
#define KEY_MSG_TEXT_INDEX                          _T("msg-text-index")
#define KEY_MSG_TEXT                                _T("msg-text")
#define KEY_INITIALIZE_FAILURE_TEXT                 _T("failure-text")
#define KEY_MESSAGE_NAME                            _T("message-name")
#define KEY_OPERATION                               _T("operation")
#define KEY_IMMEDIATE                               _T("immediate")
#define KEY_COPY_PLIST                              _T("copy-plist")
#define KEY_STATE_NAME                              _T("state-name")
#define KEY_STATE_ID                                _T("state-id")
