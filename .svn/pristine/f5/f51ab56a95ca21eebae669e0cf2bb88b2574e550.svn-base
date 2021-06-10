// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMgrAPI.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMgrAPI.cpp   $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMgrAPI.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 4     3/15/05 3:36p Dm185016
 * UNICODE
 * 
 * 3     2/28/05 4:23p Dm185016
 * lint
 * 
 * 2     2/18/05 9:20a Dm185016
 * TAR 293708.
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 6     1/18/05 3:47p Dm185016
// UNICODE support
// 
// 5     11/16/04 11:31a Dm185016
// Added event support
// 
// 4     10/29/04 9:02a Dm185016
// New OnInputEvent API
// 
// 3     10/14/04 1:39p Dm185016
// New OnInputEvent API.
// 
// 2     10/05/04 11:04a Dm185016
// Changed parameter for Initialize.  CPSX object now obtained via handle
// rather than thru pointer.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/

/*lint -save -e433 -e826 -e818 */

#include "stdafx.h"
#include "RCMgrAPI.h"
#include "RCMEvent.h"
#include "ApiRecorder.h"

using namespace fastlane;
using namespace utils;

bool CRCMgrAPI::m_bInitialized;
//StringType CRCMgrAPI::m_szItemOKEventName;

StringType CRCMgrAPI::m_szRequestApproval;
StringType CRCMgrAPI::m_szRequestSuspend;
StringType CRCMgrAPI::m_szCancelSuspend;
StringType CRCMgrAPI::m_szValidateOperator;
StringType CRCMgrAPI::m_szUnsolicitedData;
StringType CRCMgrAPI::m_szAssistMode;
StringType CRCMgrAPI::m_szRemoteConsoleEvent;


const _TCHAR *CRCMgrAPI::GetRecordName(const SRecord *pRecord)
{
    switch (pRecord->hdr.lApiNum)
    {
        case E_API_INITIALIZE :                     return _T("Initialize"); 
        case E_API_TERMINATE :                      return _T("Terminate"); 
        case E_API_CAN_APPROVE :                    return _T("CanApprove"); 
        case E_API_ON_APPROVAL :                    return _T("OnApproval"); 
        case E_API_ON_EVENT :                       return _T("OnEvent"); 
        case E_API_GET :                            return _T("Get"); 
        case E_API_SET :                            return _T("Set"); 
        case E_API_ON_CONTROL :                     return _T("OnControl"); 
        case E_API_GET_APPROVAL_INFO :              return _T("GetApprovalInfo"); 
        case E_API_ON_INPUT_EVENT :                 return _T("OnInputEvent"); 
        case E_API_EVENT_REQUEST_APPROVAL  :        return _T("OnRequestApproval"); 
        case E_API_EVENT_REQUEST_SUSPEND   :        return _T("OnRequestSuspend"); 
        case E_API_EVENT_CANCEL_SUSPEND    :        return _T("OnCancelSuspend"); 
        case E_API_EVENT_VALIDATE_OPERATOR :        return _T("OnValidateOperator"); 
        case E_API_EVENT_UNSOLICITED_DATA  :        return _T("OnUnsolicitedData"); 
        case E_API_EVENT_ASSIST_MODE       :        return _T("OnAssistMode"); 
        case E_API_EVENT_REMOTE_CONSOLE_EVENT  :    return _T("OnRemoteConsoleEvent"); 
        default :                                   return _T("<Unknown>");
    }
}


SRecord *CRCMgrAPI::BuildEventRecord(const CRCMEvent *pEvent)
{
    StringType recordname;
    try
    {
        CA2T sEventName(typeid(*pEvent).name());
        recordname = sEventName;
    }
    catch( const __non_rtti_object  &e )
        { pEvent->GetTraceObject().XTRACE_EXCEPTION(e); }
    catch( const bad_typeid &e )
        { pEvent->GetTraceObject().XTRACE_EXCEPTION(e); }

    const _TCHAR *pszParms = (*pEvent).GetParms();
    size_t nParmsLen = _tcslen(pszParms);

    size_t recordlen = sizeof(SRecordHdr) + (nParmsLen+1)*sizeof(_TCHAR);

    SRecord *pRecord                        = (SRecord*)new char[recordlen]; 
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    if (!m_bInitialized)
        Initialize(pEvent->GetTraceHandle());
    if (recordname == m_szRequestApproval)
        pRecord->hdr.lApiNum                = E_API_EVENT_REQUEST_APPROVAL;
    else if (recordname == m_szRequestSuspend)
        pRecord->hdr.lApiNum                = E_API_EVENT_REQUEST_SUSPEND;
    else if (recordname == m_szCancelSuspend)
        pRecord->hdr.lApiNum                = E_API_EVENT_CANCEL_SUSPEND;
    else if (recordname == m_szValidateOperator)
        pRecord->hdr.lApiNum                = E_API_EVENT_VALIDATE_OPERATOR;
    else if (recordname == m_szUnsolicitedData)
        pRecord->hdr.lApiNum                = E_API_EVENT_UNSOLICITED_DATA;
    else if (recordname == m_szAssistMode)
        pRecord->hdr.lApiNum                = E_API_EVENT_ASSIST_MODE;
    else if (recordname == m_szRemoteConsoleEvent)
        pRecord->hdr.lApiNum                = E_API_EVENT_REMOTE_CONSOLE_EVENT;
    else
        pRecord->hdr.lApiNum                = E_API_UNKNOWN;

    _tcscpy(pRecord->data, pszParms);
    return pRecord;
}


void CRCMgrAPI::Initialize(HTRACE hTrace)
{
    m_bInitialized                          = true;
    try
    {
        {
            CA2T sEventName(typeid(CRequestApprovalEvent).name());
            m_szRequestApproval = sEventName;
        }
        {
            CA2T sEventName(typeid(CRequestApprovalEvent).name());
            m_szRequestApproval = sEventName;
        }
        {
            CA2T sEventName(typeid(CRequestSuspendEvent).name());
            m_szRequestSuspend = sEventName;
        }
        {
            CA2T sEventName(typeid(CCancelSuspendEvent).name());
            m_szCancelSuspend = sEventName;
        }
        {
            CA2T sEventName(typeid(CValidateOperatorEvent).name());
            m_szValidateOperator = sEventName;
        }
        {
            CA2T sEventName(typeid(CUnsolicitedDataEvent).name());
            m_szUnsolicitedData = sEventName;
        }
        {
            CA2T sEventName(typeid(CAssistModeEvent).name());
            m_szAssistMode = sEventName;
        }
        {
            CA2T sEventName(typeid(CRemoteConsoleEvent).name());
            m_szRemoteConsoleEvent = sEventName;
        }
    }
    catch( const __non_rtti_object  &e )
    { 
        Trace &traceObject = Trace::GetTraceObject(hTrace);
        traceObject.XTRACE_EXCEPTION(e); 
        traceObject.XTRACE_ERROR(_T("Unable to translate event names"));
    }
    catch( const bad_typeid &e )
    { 
        Trace &traceObject = Trace::GetTraceObject(hTrace);
        traceObject.XTRACE_EXCEPTION(e); 
        traceObject.XTRACE_ERROR(_T("Unable to translate event names"));
    }
}


SRecord *CRCMgrAPI::BuildInitializeRecord(LONG lPSXHandle, HTRACE hTrace)
{
    unsigned int recordlen = sizeof(SRecordHdr) + sizeof(SInitialize);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_INITIALIZE;
    pRecord->hdr.nRecLength                 = recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    parms.initialize.lPSXHandle             = lPSXHandle; 
    parms.initialize.hTrace                 = hTrace; 
    return pRecord;
}


SRecord *CRCMgrAPI::BuildTerminateRecord()
{
    unsigned int recordlen = sizeof(SRecordHdr);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_TERMINATE;
    pRecord->hdr.nRecLength                 = recordlen; 
    return pRecord;
}
//
//
//SRecord *CRCMgrAPI::BuildInitializeRecord(long lCookie, const _TCHAR *pszParms)
//{
//    size_t nParmsLen = 0;
//    size_t recordlen = sizeof(SRecordHdr) + sizeof(SSecurityInitialize);
//    if (pszParms != NULL)
//    {
//        nParmsLen = _tcslen(pszParms)*sizeof(_TCHAR) + sizeof(_TCHAR);
//        recordlen += nParmsLen;
//    }
//
//    SRecord *pRecord                        = (SRecord*)new char[recordlen];
//    pRecord->hdr.lApiNum                    = E_API_INITIALIZE;
//    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 
//
//    pRecord->parms.initialize.lCookie       = lCookie;
//    pRecord->parms.initialize.szParms[0]    = 0;
//    if (pszParms != NULL)
//        _tcscpy(pRecord->parms.initialize.szParms, pszParms);
//
//    return pRecord;
//}
//
//
//SRecord *CRCMgrAPI::BuildInitializeRecord(BSTR bstrResult)
//{
//    size_t recordlen = sizeof(SRecordHdr) + sizeof(wchar_t);
//    std::wstring wszResult;
//    if (bstrResult != NULL)
//    {
//        wszResult = bstrResult;
//        recordlen += wszResult.size() * sizeof(wchar_t);
//    }
//
//    SRecord *pRecord                        = (SRecord*)new char[recordlen];
//    pRecord->hdr.lApiNum                    = E_API_INITIALIZE;
//    pRecord->hdr.nRecLength                 = (ULONG)recordlen;
//
//    if (bstrResult != NULL)
//        wcscpy((wchar_t *)&pRecord->parms, wszResult.c_str());  //lint !e534 ignoring return value
//    else
//        memset((void *)&pRecord->parms, 0, 2);
//
//    return pRecord;
//}


SRecord *CRCMgrAPI::BuildCanApproveRecord(const _TCHAR *pszParms)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(SCanApprove);
    if (pszParms != NULL)
        recordlen += _tcslen(pszParms) * sizeof(_TCHAR);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_CAN_APPROVE;
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    parms.canApprove.lResult                = -1;

    if (pszParms != NULL)
        _tcscpy(parms.canApprove.szParms, pszParms);
    else
        parms.canApprove.szParms[0] = 0;

    return pRecord;
}


SRecord *CRCMgrAPI::BuildCanApproveRecord(LONG *pResult)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(SCanApprove);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_CAN_APPROVE;
    pRecord->hdr.SetResult();
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    parms.canApprove.lResult                = *pResult;
    parms.canApprove.szParms[0]             = 0;

    return pRecord;
}


SRecord *CRCMgrAPI::BuildOnApprovalRecord(const _TCHAR *pszParms)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(_TCHAR);
    if (pszParms != NULL)
        recordlen += _tcslen(pszParms) * sizeof(_TCHAR);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_ON_APPROVAL;
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    if (pszParms != NULL)
        _tcscpy(parms.onApproval.szParms, pszParms);
    else
        parms.onApproval.szParms[0] = 0;

    return pRecord;
}


SRecord *CRCMgrAPI::BuildOnEventRecord(const _TCHAR *pszParms)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(_TCHAR);
    if (pszParms != NULL)
        recordlen += _tcslen(pszParms) * sizeof(_TCHAR);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_ON_EVENT;
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    if (pszParms != NULL)
        _tcscpy(parms.onEvent.szParms, pszParms);
    else
        parms.onEvent.szParms[0] = 0;

    return pRecord;
}


SRecord *CRCMgrAPI::BuildGetRecord(const _TCHAR *pszParms)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(_TCHAR);
    if (pszParms != NULL)
        recordlen += _tcslen(pszParms) * sizeof(_TCHAR);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_GET;
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    if (pszParms != NULL)
        _tcscpy(parms.get.szParms, pszParms);
    else
        parms.get.szParms[0] = 0;

    return pRecord;
}


SRecord *CRCMgrAPI::BuildGetRecord(BSTR bstrResult)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(wchar_t);
    std::wstring wszResult;
    if (bstrResult != NULL)
    {
        wszResult = bstrResult;
        recordlen += wszResult.size() * sizeof(wchar_t);
    }

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_GET;
    pRecord->hdr.SetResult();
    pRecord->hdr.nRecLength                 = (ULONG)recordlen;

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    if (bstrResult != NULL)
        wcscpy((wchar_t *)&parms, wszResult.c_str());  //lint !e534 ignoring return value
    else
        memset((void *)&parms, 0, 2);

    return pRecord;
}


SRecord *CRCMgrAPI::BuildSetRecord(const _TCHAR *pszParms)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(_TCHAR);
    if (pszParms != NULL)
        recordlen += _tcslen(pszParms) * sizeof(_TCHAR);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_SET;
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    if (pszParms != NULL)
        _tcscpy(parms.set.szParms, pszParms);
    else
        parms.set.szParms[0] = 0;

    return pRecord;
}


SRecord *CRCMgrAPI::BuildOnControlRecord(const _TCHAR *pszParms)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(_TCHAR);
    if (pszParms != NULL)
        recordlen += _tcslen(pszParms) * sizeof(_TCHAR);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_ON_CONTROL;
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    if (pszParms != NULL)
        _tcscpy(parms.onControl.szParms, pszParms);
    else
        parms.onControl.szParms[0] = 0;

    return pRecord;
}


SRecord *CRCMgrAPI::BuildOnControlRecord(BSTR bstrResult)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(wchar_t);
    std::wstring wszResult;
    if (bstrResult != NULL)
    {
        wszResult = bstrResult;
        recordlen += wszResult.size() * sizeof(wchar_t);
    }

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_ON_CONTROL;
    pRecord->hdr.SetResult();
    pRecord->hdr.nRecLength                 = (ULONG)recordlen;

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    if (bstrResult != NULL)
        wcscpy((wchar_t *)&parms, wszResult.c_str());  //lint !e534 ignoring return value
    else
        memset((void *)&parms, 0, 2);

    return pRecord;
}


SRecord *CRCMgrAPI::BuildGetApprovalInfoRecord(const _TCHAR *pszParms)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(_TCHAR);
    if (pszParms != NULL)
        recordlen += _tcslen(pszParms) * sizeof(_TCHAR);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_GET_APPROVAL_INFO;
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    if (pszParms != NULL)
        _tcscpy(parms.getApprovalInfo.szParms, pszParms);
    else
        parms.getApprovalInfo.szParms[0] = 0;

    return pRecord;
}


SRecord *CRCMgrAPI::BuildGetApprovalInfoRecord(BSTR bstrResult)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(wchar_t);
    std::wstring wszResult;
    if (bstrResult != NULL)
    {
        wszResult = bstrResult;
        recordlen += wszResult.size() * sizeof(wchar_t);
    }

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_GET_APPROVAL_INFO;
    pRecord->hdr.SetResult();
    pRecord->hdr.nRecLength                 = (ULONG)recordlen;

    if (bstrResult != NULL)
        wcscpy((wchar_t *)pRecord->data, wszResult.c_str());  //lint !e534 ignoring return value
    else
        memset((void *)pRecord->data, 0, sizeof(wchar_t));

    return pRecord;
}


SRecord *CRCMgrAPI::BuildOnInputEventRecord(const _TCHAR *pszParms)
{
    size_t recordlen = sizeof(SRecordHdr) + sizeof(_TCHAR);
    if (pszParms != NULL)
        recordlen += _tcslen(pszParms) * sizeof(_TCHAR);

    SRecord *pRecord                        = (SRecord*)new char[recordlen];
    pRecord->hdr.lApiNum                    = E_API_ON_INPUT_EVENT;
    pRecord->hdr.nRecLength                 = (ULONG)recordlen; 

    UMessageParms &parms                    = *(UMessageParms *)&(pRecord->data[0]);
    if (pszParms != NULL)
        _tcscpy(parms.onEvent.szParms, pszParms);
    else
        parms.onEvent.szParms[0] = 0;

    return pRecord;
}
//
//
//SRecord *CRCMgrAPI::BuildSecurityControlRecord(BSTR bstrResult)
//{
//    size_t recordlen = sizeof(SRecordHdr) + sizeof(wchar_t);
//    std::wstring wszResult;
//    if (bstrResult != NULL)
//    {
//        wszResult = bstrResult;
//        recordlen += wszResult.size() * sizeof(wchar_t);
//    }
//
//    SRecord *pRecord                        = (SRecord*)new char[recordlen];
//    pRecord->hdr.lApiNum                    = E_API_SECURITY_CONTROL;
//    pRecord->hdr.nRecLength                 = (ULONG)recordlen;
//
//    if (bstrResult != NULL)
//        wcscpy((wchar_t *)&pRecord->parms, wszResult.c_str());  //lint !e534 ignoring return value
//    else
//        memset((void *)&pRecord->parms, 0, 2);
//
//
//    return pRecord;
//}

/*lint -restore */
