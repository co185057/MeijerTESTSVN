#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMLoader.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMLoader.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMLoader.h $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 2     2/28/05 4:20p Dm185016
 * lint
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
* 
* 2     11/16/04 11:32a Dm185016
* Context switch support
* 
* 1     10/29/04 8:55a Dm185016
*/
#include "SMLoader.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

class STATE_MACHINE_NAMESPACE::CXMLStateMachine;

namespace fastlane
{

using namespace std;
using namespace STATE_MACHINE_NAMESPACE;

class CRCMLoader
    : public CSMLoader
{
public:
    CRCMLoader(HTRACE hTrace);
    virtual ~CRCMLoader(void) {};

    virtual void registerHandlers(void);

private:

    CRCMLoader();
};

}

#pragma warning( pop )
