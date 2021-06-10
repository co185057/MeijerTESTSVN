// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMLoaderRAP.cpp 3     1/12/10 11:58p Cu185008 $
/*
 * @(#)RCMLoaderRAP.cpp    $Revision: 3 $ $Date: 1/12/10 11:58p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RCMLoaderRAP.cpp $
 * 
 * 3     1/12/10 11:58p Cu185008
 * G2 Migration.
 * 
 * 1     3/01/05 2:04p Dm185016
 * TAR 294587.  Added application state functionality.
*/

#include "StdAfx.h"
#include "RCMLoaderRAP.h"
#include "RAConstants.h"
#include "AppStateHnd.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;


CRCMLoaderRAP::CRCMLoaderRAP(HTRACE hTrace)
    : CRCMLoader(hTrace)
{
    CTOR_BEGIN(hTrace)

    IMODTRACE_DEVELOP();

    CRCMLoaderRAP::registerHandlers();

    CTOR_END()
}


void CRCMLoaderRAP::registerHandlers(void)
{
    IMODTRACE_DEVELOP();
 
    CApplicationStateHandler::instance()->SetLoader(this);
    
    SETUP_NULL_HANDLER( XP_APP_STATE_LIST );
    SETUP_HANDLER( XP_APP_STATE_DEFINITION, ApplicationStateHandler );
}
