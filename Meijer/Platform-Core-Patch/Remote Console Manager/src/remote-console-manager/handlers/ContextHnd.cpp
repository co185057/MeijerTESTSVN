// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ContextHnd.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ContextHnd.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/handlers/ContextHnd.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     5/27/08 10:32a Bc185047
 * RCM Patch G Issue – Eliminate RCM’s dependence on XML format
 * 
 * Removed unecessary tag handlers.
 * 
 * 7     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 3     2/28/05 4:25p Dm185016
 * lint
 * 
 * 2     2/07/05 7:18p Dm185016
 * New handler for <HHInstructionText> tag in LaneRAP.xml.
 * 
 * 1     1/26/05 2:25p Dm185016
 * Moved to new Repository.
// 
// 4     1/07/05 10:49a Dm185016
// TAR 288789 - Added handler for lead thru text name in LaneRAP.xml
// 
// 3     11/16/04 11:46a Dm185016
// Additional xpath needed
// 
// 2     11/04/04 2:16p Dm185016
// 
// 1     10/29/04 9:18a Dm185016
*/
#include "StdAfx.h"
#include "ContextHnd.h"
#include "ContextImpl.h"
#include "RCMConstants.h"
#include "RCMStateMachine.h"
#include "RCMLoader.h"
#include "RCMError.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace fastlane;
using namespace STATE_MACHINE_NAMESPACE;


CContextHandler::CContextHandler(void) 
    : pLoader(NULL)
{
}


CContextHandler::~CContextHandler(void)
{
}


void CContextHandler::registerHandlers(void)
{
    //IMODTRACE_DEVELOP();
}


DEFINE_START_ELEMENT(CContextHandler) 
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

    m_pContent = new CContextImpl(GetTraceHandle(), name);

    return S_OK;
}


DEFINE_END_ELEMENT(CContextHandler)
{
    //IMODTRACE_DEVELOP();

    _ASSERTE(m_pContent != NULL);
    _ASSERTE(pLoader != NULL);
    CRCMStateMachine *pSM = dynamic_cast<CRCMStateMachine *>(pLoader->GetStateMachine());
    _ASSERTE(pSM != NULL);

    //return pSM->Add(&context());
    return pSM->Add((CContextImpl *)(m_pContent));
}


DEFINE_CHARACTERS(CContextHandler, DefaultButton)
{
    HRESULT hr = S_OK;
    StringType szValue = XMLUtilities::getString(pwchChars, cchChars);
    try
    {
        CContextHandler::instance()->SetDefaultButton(szValue);
    }
    catch (const IException  &exc)
    {
        ITRACE_EXCEPTION(exc);

        hr = exc.errorId();
    }

    return hr;
}


//DEFINE_HANDLER(CContextHandler, View, context)
DEFINE_CHARACTERS(CContextHandler, View)
{
    /*
    IMODTRACE_DEVELOP();
    */
    StringType szValue = GET_CHARACTERS();
    /*
    ITRACE_DEVELOP(_T("szValue:  ") + szValue);
    */
    /*
    BASE::instance()->INSTANCE().ID.Convert(szValue);
    */
    CContextImpl *pContext = (CContextImpl *)CContextHandler::instance()->m_pContent;
    pContext->SaveView(szValue);
    return S_OK;
}


void CContextHandler::SetDefaultButton(const StringType &szMsgName)
{
    //IMODTRACE_DEVELOP();

    //context().DefaultButton.Value(szMsgName);
    CContextImpl *pContext = (CContextImpl *)m_pContent;
    pContext->DefaultButton.Value(szMsgName);
}


void CContextHandler::GetXMLOutput(CXMLContent * /* pContent */, CXMLBuilder * /* pTheBuilder */)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    //CApplicationState &state = *(CApplicationState *)pContent;
}
