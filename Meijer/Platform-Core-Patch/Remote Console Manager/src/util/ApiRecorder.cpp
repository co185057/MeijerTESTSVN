// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/ApiRecorder.cpp 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)ApiRecorder.cpp    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/ApiRecorder.cpp $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     7/24/05 1:46p Jv185012
 * 5 partition modifications
 * 
 * 4     5/16/05 6:11p Dm185016
 * Added timestamp for performance measurements.
 * 
 * 2     2/28/05 3:29p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
// 
// 3     1/18/05 4:00p Dm185016
// UNICODE support
// 
// 2     11/15/04 11:29a Dm185016
// Replaced C++ IO streams with CreateFile/WriteFile/CloseHandle sequence.
// 
// 1     10/04/04 6:53p Dm185016
*/
//lint --e{732, 1775}

#include "StdAfx.h"
#include "ApiRecorder.h"
#include "Registry.h"
#include "verify.h"

using namespace utils;


CApiRecorder::CApiRecorder(const _TCHAR *pszRegKey, HTRACE hTrace)
    : CBaseObject(hTrace)
    , m_bRecording(false)
{
    CRegistry registry;
    m_bRecording = 
        CRegistryValue<bool>::GetRegistryValue( registry
                                    , pszRegKey
                                    , REG_VALUE_RECORD
                                    , false);

    m_sFileName = 
        CRegistryValue<const _TCHAR *>::GetRegistryValue( registry
                                    , pszRegKey
                                    , REG_VALUE_RECORD_NAME
                                    , API_RECORD_FILE);

	TCHAR szFileName[_MAX_PATH] = _T("");
	if(ExpandEnvironmentStrings(m_sFileName.c_str(), szFileName, _MAX_PATH))
	{
		m_sFileName = szFileName;
	}

    ITRACE_DEVELOP(m_bRecording ? _T(__FUNCTION__) _T("Recording ...") : _T(__FUNCTION__) _T("Recording not in progress."));

    m_hFile = ::CreateFile(m_sFileName.c_str()
                         , GENERIC_WRITE
                         , 0
                         , NULL
                         , CREATE_ALWAYS
                         , FILE_ATTRIBUTE_NORMAL
                         , NULL);
    if ( INVALID_HANDLE_VALUE == m_hFile )
    {
        ITHROWSYSTEMERROR(::GetLastError(), _T("CreateFile"), IException::unrecoverable);
    }
}



/*lint -save -e1740 */
CApiRecorder::~CApiRecorder()
{
    IMODTRACE_DEVELOP();

    VERIFY_WIN32_AND_LOG(::CloseHandle(m_hFile));

    m_hFile = INVALID_HANDLE_VALUE;
}
/*lint -restore */


bool CApiRecorder::IsRecording() const { return m_bRecording; }


void CApiRecorder::RecordAPI(SRecord *pRecord)
{
    SYSTEMTIME time;
    ::GetSystemTime(&time);

    if (0 == ::SystemTimeToFileTime(&time, &(pRecord->hdr.time)))
    {
        ISystemErrorInfo error( _T("SystemTimeToFileTime") );
        ITRACE_ERROR((const _TCHAR*)error);
    }

    ITRACE_DEVELOP(_T("Api:  ") + IString(GetRecordName(pRecord)));
    ITRACE_DUMP(_T("Api Record:"), (const _TCHAR *)pRecord, (int)pRecord->hdr.nRecLength);

    DWORD dwBytesWritten;
    if (m_bRecording)
    {
        if (FALSE == ::WriteFile(m_hFile
                                , (const _TCHAR *)pRecord
                                , (int)pRecord->hdr.nRecLength
                                , &dwBytesWritten
                                , NULL) )
        {
            ITHROWSYSTEMERROR(::GetLastError(), _T("WriteFile"), IException::unrecoverable);
        }
    }
}


void CApiRecorder::RecordResult(SRecord *pRecord)
{
    SYSTEMTIME time;
    ::GetSystemTime(&time);

    if (0 == ::SystemTimeToFileTime(&time, &(pRecord->hdr.time)))
    {
        ISystemErrorInfo error( _T("SystemTimeToFileTime") );
        ITRACE_ERROR((const _TCHAR*)error);
    }

    ITRACE_DEVELOP(_T("Api:  ") + IString(GetRecordName(pRecord)));
    ITRACE_DUMP(_T("Api Result:"), (const _TCHAR *)pRecord, (int)pRecord->hdr.nRecLength);

    DWORD dwBytesWritten;
    if (m_bRecording)
    {
        if (FALSE == ::WriteFile(m_hFile
                                , (const _TCHAR *)pRecord
                                , (int)pRecord->hdr.nRecLength
                                , &dwBytesWritten
                                , NULL) )
        {
            ITHROWSYSTEMERROR(::GetLastError(), _T("WriteFile"), IException::unrecoverable);
        }
    }
}
