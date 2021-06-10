#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMRecorder.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMRecorder.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMRecorder.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 2     2/28/05 4:21p Dm185016
 * lint
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 1     10/04/04 5:36p Dm185016
* New for 3.2 Patch E/4.0
* 
* 4     6/30/04 2:49p Dm185016
* Exported class.
* 
* 3     5/17/04 1:02p Dm185016
* lint
* 
* 2     5/04/04 4:41p Dm185016
* Name of file for recorder now from registry
* 
* 1     3/19/04 2:59p Dm185016
* New mechanism for recording the API for playback.
*/

#include "ApiRecorder.h"
#include "RCMgrAPI.h"
//#include "Message.h"
#include <set>
//#include <iostream>
#include <fstream>

#pragma warning( push )
#pragma warning( disable : 4251 )

using namespace utils;

namespace fastlane
{

class RCMANAGER_API CRCMRecorder
    : public CApiRecorder
{

public:

    CRCMRecorder(HTRACE hTrace);
    virtual ~CRCMRecorder();

    virtual const _TCHAR *GetRecordName(const utils::SRecord *pRecord) const;
    void RecordAPI(SRecord *);
    void RecordResult(SRecord *);

private:

    CRCMRecorder();
    CRCMRecorder(const CRCMRecorder &);               // Hidden Copy construcor
    CRCMRecorder &operator =(const CRCMRecorder &);   // Hidden assignment operator

};

}

#pragma warning( pop )
