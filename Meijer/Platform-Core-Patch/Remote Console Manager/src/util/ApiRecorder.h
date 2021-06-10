#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/ApiRecorder.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)RCMRecorder.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/util/ApiRecorder.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     5/17/05 1:45p Dm185016
 * Changes for performance measurements.
 * 
 * 4     5/16/05 6:11p Dm185016
 * Added timestamp for performance measurements.
 * 
 * 2     2/28/05 3:29p Dm185016
 * lint
 * 
 * 1     1/26/05 2:32p Dm185016
 * Moved to new Repository.
* 
* 4     11/15/04 11:29a Dm185016
* Replaced C++ IO streams with CreateFile/WriteFile/CloseHandle sequence.
* 
* 3     10/29/04 9:32a Dm185016
* Added methods to check/set for result
* 
* 2     10/14/04 1:49p Dm185016
* Fixed compile problem between VS 2002 and VS 2003.
* 
* 1     10/04/04 6:53p Dm185016
*/

#include "Utils.h"

#include <fstream>

#pragma warning( push )
#pragma warning( disable : 4251 )

#define API_RECORD_FILE _T("api.dat")
#define REG_VALUE_RECORD                            _T("Record")
#define REG_VALUE_RECORD_NAME                       _T("Record.Name")

namespace utils
{


typedef struct _SFileHdr
{
} SFileHdr;


typedef struct _SRecordHdr
{
    LONG    lApiNum;
    ULONG   nRecLength;

    FILETIME time;

    bool IsResult() const { return (((ULONG)lApiNum & 0x80000000) != 0); }
    void SetResult() { lApiNum = (LONG)((ULONG)lApiNum | 0x80000000); }

} SRecordHdr;


typedef struct _SRecord
{
    SRecordHdr hdr;
    _TCHAR data[1];

    size_t GetDataSize() const { return hdr.nRecLength - sizeof(SRecordHdr); }
    const _TCHAR *GetData() const { return data; }

} SRecord;


class UTILS_API CApiRecorder
    : public CBaseObject
{

public:

    CApiRecorder(const _TCHAR *, HTRACE);
    virtual ~CApiRecorder();

    bool IsRecording() const;

    virtual const _TCHAR *GetRecordName(const SRecord *) const=0;

    virtual void RecordAPI(SRecord *);
    virtual void RecordResult(SRecord *);

private:

    CApiRecorder();
    CApiRecorder(const CApiRecorder &);               // Hidden Copy construcor
    CApiRecorder &operator =(const CApiRecorder &);   // Hidden assignment operator

private:

    bool m_bRecording;

    StringType m_sFileName;

    HANDLE m_hFile;
};

}

#pragma warning( pop )
