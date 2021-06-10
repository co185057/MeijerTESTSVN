// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayInstructionText.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayInstructionText.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayInstructionText.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     1/13/06 3:46p Tk124825
 * TAR 301329
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 1     1/07/05 2:27p Dm185016
// TAR 287222, TAR 288789
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisplayInstructionText.h"
#include "RAPMessages.h"
#include "RCMKeyValue.h"
#include "verifypsx.h"
#include "psxmfc.h"

BEGIN_KV_HANDLER_CLASS(CDisplayInstructionText::DITParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayInstructionText::DITParms, KEY_EV_INSTRUCTION_TEXT_VARIABLE_NAME, InstructionTextVariable)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayInstructionText::DITParms, KEY_CONTEXT_NAME, ContextName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CDisplayInstructionText::DITParms, InstructionTextVariable)
DEFINE_KV_CSTRING(CDisplayInstructionText::DITParms, ContextName)


void CDisplayInstructionText::DITParms::Clear()
{
    m_InstructionTextVariable.Empty();
    m_ContextName.Empty();
}


// Get all control names from the xml
HRESULT CDisplayInstructionText::ParseOperands()
{ 
    HRESULT hr = S_OK;

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;
    
    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return hr;
}


HRESULT CDisplayInstructionText::DisplayInstructionText()
{
	HRESULT hr = ParseOperands();
    if (hr != S_OK)
		return hr;

    StringType sOutput;

    hr = FormatInstructionText(sOutput, GetInstructionText());
    if (hr != S_OK)
        return hr;

    hr = SetTransactionVariable(m_parms.GetInstructionTextVariable(), sOutput.c_str());

    return hr;
}


const _TCHAR *CDisplayInstructionText::GetInstructionText()
{   
    HRESULT hr;

    CString sText;

    VERIFY_PSX(hr, psx().GetCustomDataVar(RAP_INSTRUCTIONTEXT
                                        , sText
                                        , _T("")
                                        , (const _TCHAR *)m_parms.GetContextName()));
    m_sText = GetString( sText );

    return m_sText;
}
