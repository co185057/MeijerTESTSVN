// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ControlHnd.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ControlHnd.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ControlHnd.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     2/28/05 4:25p Dm185016
 * lint
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
#include "ControlHnd.h"
#include "ControlImpl.h"
#include "RCMConstants.h"
#include "RCMStateMachine.h"
#include "RCMLoader.h"
#include "RCMError.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace fastlane;
using namespace STATE_MACHINE_NAMESPACE;


CControlHandler::CControlHandler(void) 
    : pLoader(NULL)
{
}


CControlHandler::~CControlHandler(void)
{
}


void CControlHandler::registerHandlers(void)
{
}


DEFINE_START_ELEMENT(CControlHandler) 
{
    //IMODTRACE_DEVELOP();

    StringType name = XMLUtilities::getAttribute(
                            (const wchar_t *)CH_NAMESPACE_URI,
                            CH_NAMESPACE_URI_LEN,
                            (const wchar_t *)ATTR_DISPLAY_CONTROLS_CONTROL_NAME,
                            (int)ATTR_DISPLAY_CONTROLS_CONTROL_NAME_LENGTH,
                            pAttributes);
    if (name.size() == 0)
        return RCM_MISSING_ATTRIBUTE;

    //ITRACE_DEVELOP(_T(" Control name:  ") + IString(name.c_str()));

    StringType type = XMLUtilities::getAttribute(
                            (const wchar_t *)CH_NAMESPACE_URI,
                            CH_NAMESPACE_URI_LEN,
                            (const wchar_t *)ATTR_DISPLAY_CONTROLS_CONTROL_TYPE,
                            (int)ATTR_DISPLAY_CONTROLS_CONTROL_TYPE_LENGTH,
                            pAttributes);
    if (type.size() == 0)
        return RCM_MISSING_ATTRIBUTE;

    m_pContent = new CControlImpl(GetTraceHandle(), name, type);

    _ASSERTE(pLoader != NULL);
    CRCMStateMachine *pSM = dynamic_cast<CRCMStateMachine *>(pLoader->GetStateMachine());
    _ASSERTE(pSM != NULL);

    //ITRACE_DEVELOP(_T(" Control type:  ") + IString(type.c_str()));

    if (type == ATTR_DISPLAY_CONTROLS_CONTROL_TYPE_BUTTONLIST)
    {
        ITRACE_DEVELOP(_T(" Processing ") ATTR_DISPLAY_CONTROLS_CONTROL_TYPE_BUTTONLIST);
        control().SetIgnored(false);
        return pSM->Add(&control());
    }

    else if (type == ATTR_DISPLAY_CONTROLS_CONTROL_TYPE_BUTTON)
    {
        ITRACE_DEVELOP(_T(" Processing ") ATTR_DISPLAY_CONTROLS_CONTROL_TYPE_BUTTON);
        control().SetIgnored(false);
        return pSM->Add(&control());
    }

    else
    {
        ITRACE_DEVELOP(_T(" Ignoring ") + IString(type.c_str()));
        control().SetIgnored(true);
    }

    return S_OK;
}


DEFINE_END_ELEMENT(CControlHandler)
{
    //IMODTRACE_DEVELOP();

    _ASSERTE(m_pContent != NULL);

    if (control().IsIgnored())
        delete m_pContent;

    m_pContent = NULL;

    return S_OK;
}


void CControlHandler::GetXMLOutput(CXMLContent * /* pContent */, CXMLBuilder * /* pTheBuilder */)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    //CApplicationState &state = *(CApplicationState *)pContent;
}
