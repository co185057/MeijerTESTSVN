#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMLoaderRAP.h 3     1/12/10 11:58p Cu185008 $
/*
 * @(#)RCMLoaderRAP.h    $Revision: 3 $ $Date: 1/12/10 11:58p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMLoaderRAP.h $
 * 
 * 3     1/12/10 11:58p Cu185008
 * G2 Migration.
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     3/01/05 2:04p Dm185016
 * TAR 294587.  Added application state functionality.
*/
#include "RCMLoader.h"

#pragma warning( push )
#pragma warning( disable : 4251 )

class STATE_MACHINE_NAMESPACE::CXMLStateMachine;

using namespace fastlane;

namespace rap
{

using namespace std;
using namespace STATE_MACHINE_NAMESPACE;

class CRCMLoaderRAP
    : public CRCMLoader
{
public:
    CRCMLoaderRAP(HTRACE hTrace);
    virtual ~CRCMLoaderRAP(void) {};

    virtual void registerHandlers(void);

private:

    CRCMLoaderRAP();
};

}

#pragma warning( pop )
