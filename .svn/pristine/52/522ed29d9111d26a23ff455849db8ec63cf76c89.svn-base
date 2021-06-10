// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayControlParser.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayControlParser.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2006 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayControlParser.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 4     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 2     1/13/06 3:46p Tk124825
 * TAR 301329
 * 
 * 1     1/13/06 2:26p Dm185016
 * TAR 301329
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "DisplayControlParser.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "RCMKeyValue.h"
#include "RCMError.h"
#include "KVParmList.h"
#include "verify.h"

BEGIN_KV_HANDLER_CLASS(CDisplayControlParser::DCPParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayControlParser::DCPParms, KEY_EV_DATA_SOURCE, DataSource)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayControlParser::DCPParms, KEY_EV_DESCRIPTION, Desc)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayControlParser::DCPParms, KEY_EV_DESCRIPTION_DESCRIPTION, Description)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayControlParser::DCPParms, KEY_EV_INSTRUCTION_TEXT_VARIABLE_NAME, InstructionTextVariableName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayControlParser::DCPParms, KEY_EV_LANE_BUTTON_TEXT_CONTROL_NAME, LaneButtonTextControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayControlParser::DCPParms, KEY_EV_LANE_STATUS_1_CONTROL_NAME, LaneStatus1ControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayControlParser::DCPParms, KEY_EV_LANE_STATUS_2_CONTROL_NAME, LaneStatus2ControlName)
    DEFINE_KV_HANDLER_MAP_ENTRY(CDisplayControlParser::DCPParms, KEY_EV_LANE_STATUS_3_CONTROL_NAME, LaneStatus3ControlName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CDisplayControlParser::DCPParms, DataSource)
DEFINE_KV_CSTRING(CDisplayControlParser::DCPParms, Desc)
DEFINE_KV_CSTRING(CDisplayControlParser::DCPParms, Description)
DEFINE_KV_CSTRING(CDisplayControlParser::DCPParms, InstructionTextVariableName)
DEFINE_KV_CSTRING(CDisplayControlParser::DCPParms, LaneButtonTextControlName)
DEFINE_KV_CSTRING(CDisplayControlParser::DCPParms, LaneStatus1ControlName)
DEFINE_KV_CSTRING(CDisplayControlParser::DCPParms, LaneStatus2ControlName)
DEFINE_KV_CSTRING(CDisplayControlParser::DCPParms, LaneStatus3ControlName)
BEGIN_KV_HANDLER_CLASS_CLEAR(CDisplayControlParser::DCPParms)
    CLEAR_KV_CSTRING(DataSource)
    CLEAR_KV_CSTRING(Desc)
    CLEAR_KV_CSTRING(Description)
    CLEAR_KV_CSTRING(InstructionTextVariableName)
    CLEAR_KV_CSTRING(LaneButtonTextControlName)
    CLEAR_KV_CSTRING(LaneStatus1ControlName)
    CLEAR_KV_CSTRING(LaneStatus2ControlName)
    CLEAR_KV_CSTRING(LaneStatus3ControlName)
END_KV_HANDLER_CLASS_CLEAR()


// Get all control names from the xml
HRESULT CDisplayControlParser::ParseOperands()
{ 
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    CParameterList::iterator end    = parms.end();

    if (iter == end)
        return STATEM_MISSING_PARAMETER;
    
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
    if (hr != S_OK)
        return hr;

    return CPSXAction::NextOperands(iter, end);
}


const _TCHAR *CDisplayControlParser::GetDescription() const { return m_parms.GetDesc(); }
const _TCHAR *CDisplayControlParser::GetInstructionTextVariableName() const { return m_parms.GetInstructionTextVariableName(); }
const _TCHAR *CDisplayControlParser::GetLaneButtonTextControlName() const { return m_parms.GetLaneButtonTextControlName(); }
const _TCHAR *CDisplayControlParser::GetLaneStatus1ControlName() const { return m_parms.GetLaneStatus1ControlName(); }
const _TCHAR *CDisplayControlParser::GetLaneStatus2ControlName() const { return m_parms.GetLaneStatus2ControlName(); }
const _TCHAR *CDisplayControlParser::GetLaneStatus3ControlName() const { return m_parms.GetLaneStatus3ControlName(); }
