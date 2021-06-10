#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMgrAPI.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMgrAPI.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMgrAPI.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     5/17/05 1:47p Dm185016
 * Changes for performance measurements.
 * 
 * 3     2/28/05 4:23p Dm185016
 * lint
 * 
 * 2     2/18/05 9:20a Dm185016
 * TAR 293708.
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 4     11/16/04 11:31a Dm185016
* Added event support
* 
* 3     10/14/04 1:39p Dm185016
* New OnInputEvent API.
* 
* 2     10/05/04 11:04a Dm185016
* Changed parameter for Initialize.  CPSX object now obtained via handle
* rather than thru pointer.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
*/

#include "RCMApi.h"
#include "RCMMessages.h"
#include "ApiRecorder.h"

//namespace utils 
//{
//    typedef struct SRecord;
//}
using namespace utils;

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace fastlane
{

static const LONG E_API_UNKNOWN                     = -1;
static const LONG E_API_INITIALIZE                  = 1;
static const LONG E_API_TERMINATE                   = 2;
static const LONG E_API_CAN_APPROVE                 = 3;
static const LONG E_API_ON_APPROVAL                 = 4;
static const LONG E_API_ON_EVENT                    = 5;
static const LONG E_API_GET                         = 6;
static const LONG E_API_SET                         = 7;
static const LONG E_API_ON_CONTROL                  = 8;
static const LONG E_API_GET_APPROVAL_INFO           = 9;
static const LONG E_API_ON_INPUT_EVENT              = 10;
static const LONG E_API_EVENT_REQUEST_APPROVAL      = 11;
static const LONG E_API_EVENT_REQUEST_SUSPEND       = 12;
static const LONG E_API_EVENT_CANCEL_SUSPEND        = 13;
static const LONG E_API_EVENT_VALIDATE_OPERATOR     = 14;
static const LONG E_API_EVENT_UNSOLICITED_DATA      = 15;
static const LONG E_API_EVENT_ASSIST_MODE           = 16;
static const LONG E_API_EVENT_REMOTE_CONSOLE_EVENT  = 17;

class CRCMEvent;

class RCMANAGER_API CRCMgrAPI
{

public:

    static SRecord *BuildEventRecord(const CRCMEvent *);
    static SRecord *BuildInitializeRecord(LONG, HTRACE);
    static SRecord *BuildTerminateRecord(void);
    static SRecord *BuildCanApproveRecord(LPCTSTR);
    static SRecord *BuildCanApproveRecord(LONG *);
    static SRecord *BuildGetApprovalInfoRecord(LPCTSTR);
    static SRecord *BuildGetApprovalInfoRecord(BSTR);
    static SRecord *BuildOnControlRecord(LPCTSTR);
    static SRecord *BuildOnControlRecord(BSTR);
    static SRecord *BuildOnEventRecord(LPCTSTR);
    static SRecord *BuildGetRecord(LPCTSTR);
    static SRecord *BuildGetRecord(BSTR);
    static SRecord *BuildSetRecord(LPCTSTR);
    static SRecord *BuildOnApprovalRecord(LPCTSTR);
    static SRecord *BuildOnInputEventRecord(LPCTSTR);

    static const _TCHAR *GetRecordName(const SRecord *);

private:

    CRCMgrAPI();
    CRCMgrAPI(const CRCMgrAPI &);

    static void Initialize(HTRACE hTrace);

    static bool m_bInitialized;

    static StringType m_szRequestApproval;
    static StringType m_szRequestSuspend;
    static StringType m_szCancelSuspend;
    static StringType m_szValidateOperator;
    static StringType m_szUnsolicitedData;
    static StringType m_szAssistMode;
    static StringType m_szRemoteConsoleEvent;

};


}

#pragma warning( pop )
