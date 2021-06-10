// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/LaneConfigurationHnd.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)LaneConfigurationHnd.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/LaneConfigurationHnd.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 1     1/26/05 2:25p Dm185016
 * Moved to new Repository.
// 
// 2     11/16/04 11:46a Dm185016
// Cleaned up tracing
// 
// 1     10/29/04 9:18a Dm185016
*/
#include "StdAfx.h"
#include "LaneConfigurationHnd.h"
#include "RCMConstants.h"
#include "RCMStateMachine.h"
#include "RemoteAPMgr.h"
#include "RCMLoader.h"
#include "RCMError.h"

using namespace fastlane;
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;


CLaneConfigurationHandler::CLaneConfigurationHandler(void) 
    : pLoader(NULL)
{
}


CLaneConfigurationHandler::~CLaneConfigurationHandler(void)
{
}


void CLaneConfigurationHandler::registerHandlers(void)
{
}


DEFINE_START_ELEMENT(CLaneConfigurationHandler) 
{
    StringType sNumLanes = XMLUtilities::getAttribute(
                            (const wchar_t *)CH_NAMESPACE_URI,
                            CH_NAMESPACE_URI_LEN,
                            (const wchar_t *)ATTR_LANE_CONFIGS_LANE_CONFIG_NUMBER_OF_LANES,
                            (int)ATTR_LANE_CONFIGS_LANE_CONFIG_NUMBER_OF_LANES_LENGTH,
                            pAttributes);
    if (sNumLanes.size() == 0)
        return RCM_MISSING_ATTRIBUTE;

    ITRACE_DEVELOP(_T(" Number Lanes:  ") + IString(sNumLanes.c_str()));

    StringType sContextName = XMLUtilities::getAttribute(
                            (const wchar_t *)CH_NAMESPACE_URI,
                            CH_NAMESPACE_URI_LEN,
                            (const wchar_t *)ATTR_LANE_CONFIGS_LANE_CONFIG_CONTEXT_NAME,
                            (int)ATTR_LANE_CONFIGS_LANE_CONFIG_CONTEXT_NAME_LENGTH,
                            pAttributes);
    if (sContextName.size() == 0)
        return RCM_MISSING_ATTRIBUTE;
    ITRACE_DEVELOP(_T(" Context name:  ") + IString(sContextName.c_str()));

    _ASSERTE(pLoader != NULL);
    CRCMStateMachine *pSM = dynamic_cast<CRCMStateMachine *>(pLoader->GetStateMachine());
    _ASSERTE(pSM != NULL);

	return CSingleton<CRemoteAPMgr>::instance()->AddLaneConfiguration(IString(sNumLanes.c_str()).asInt(), sContextName.c_str());

    return S_OK;
}


void CLaneConfigurationHandler::GetXMLOutput(CXMLContent * /* pContent */, CXMLBuilder * /* pTheBuilder */)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    //CApplicationState &state = *(CApplicationState *)pContent;
}
