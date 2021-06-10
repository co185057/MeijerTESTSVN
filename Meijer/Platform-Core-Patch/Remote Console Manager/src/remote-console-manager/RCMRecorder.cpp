// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMRecorder.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMRecorder.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMRecorder.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/

#include "StdAfx.h"
#include "RCMRecorder.h"
#include "RCMRegistry.h"

using namespace fastlane;


CRCMRecorder::CRCMRecorder(HTRACE hTrace) 
    : CApiRecorder(HKEY_RCM, hTrace) 
{
}


CRCMRecorder::~CRCMRecorder() 
{
}


const _TCHAR *CRCMRecorder::GetRecordName(const utils::SRecord *pRecord) const
{ return CRCMgrAPI::GetRecordName(pRecord); }


void CRCMRecorder::RecordAPI(SRecord *pRecord)
{
    CApiRecorder::RecordAPI(pRecord);

    delete [] (char *)pRecord;
}


void CRCMRecorder::RecordResult(SRecord *pRecord)
{
    CApiRecorder::RecordResult(pRecord);

    delete [] (char *)pRecord;
}
