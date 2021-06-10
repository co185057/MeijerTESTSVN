// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMLoader.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCMLoader.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCMLoader.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 5     5/27/08 10:35a Bc185047
 * RCM Patch G Issue – Eliminate RCM’s dependence on XML format
 * 
 * Removed unecessary tag handlers.
 * 
 * 2     1/27/05 2:37p Dm185016
 * New keyword in LaneRAP.xml.
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 6     1/13/05 3:08p Dm185016
// New keywords added to LaneRAP.xml
// 
// 5     12/14/04 11:22a Dm185016
// Removed lane configuration
// 
// 4     11/19/04 1:38p Dm185016
// Added Strikethrough xpath
// 
// 3     11/16/04 11:32a Dm185016
// Context switch support
// 
// 2     11/04/04 10:52a Dm185016
// More LaneRAP.xml changes
// 
// 1     10/29/04 8:55a Dm185016
*/

#include "StdAfx.h"
#include "RCMLoader.h"
#include "RCMConstants.h"
#include "ControlHnd.h"
#include "ContextHnd.h"
#include "ContextSwitchHnd.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace fastlane;


CRCMLoader::CRCMLoader(HTRACE hTrace)
    : CSMLoader(hTrace)
{
    CTOR_BEGIN(hTrace)

    IMODTRACE_DEVELOP();

    CRCMLoader::registerHandlers();

    CTOR_END()
}


void CRCMLoader::registerHandlers(void)
{
    IMODTRACE_DEVELOP();
 
    CContextSwitchHandler::instance()->SetLoader(this);
    CControlHandler::instance()->SetLoader(this);
    CContextHandler::instance()->SetLoader(this);

	SETUP_NULL_HANDLER( XP_LANE_CONFIGS );

	SETUP_NULL_HANDLER(  XP_DISPLAY );
    SETUP_NULL_HANDLER(  XP_DISPLAY_INCLUDES );
    SETUP_NULL_HANDLER(  XP_DISPLAY_INCLUDES_INCLUDE );

	SETUP_NULL_HANDLER( XP_CONTEXT_SWITCH_LIST );
    SETUP_HANDLER( XP_CONTEXT_SWITCH_DEFINITION, ContextSwitchHandler );
}
