// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/SetReturnCode.cpp 1     6/20/08 1:11p Sa250050 $
/*
 * @(#)SetReturnCode.cpp    $Revision: 1 $ $Date: 6/20/08 1:11p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/actions/SetReturnCode.cpp $
 * 
 * 1     6/20/08 1:11p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     3/28/05 5:09p Dm185016
 * Lint
 * 
 * 2     3/22/05 2:50p Dm185016
 * UNICODE
 * 
 * 1     1/26/05 2:28p Dm185016
 * Moved to new Repository.
// 
// 6     11/15/04 11:42a Dm185016
// Moved return code from action to message.
// 
// 5     10/29/04 9:28a Dm185016
// Removed reference to MessageObject.
// 
// 3     3/19/04 3:25p Dm185016
// Removed trace
// 
// 2     3/01/04 10:15a Dm185016
// Added Allocation/Release functions
// 
// 1     2/10/04 11:48a Dm185016
// New action to allow the state-machine to set a return code for
// synchronous messages.
*/
#include "StdAfx.h"
#include "SetReturnCode.h"

using namespace STATE_MACHINE_NAMESPACE;


CSetReturnCode::CSetReturnCode()
    : m_nRc(S_OK)
{
    SetName(_T("CSetReturnCode"));
    Comment.Value(_T("Resends the received message to the SM."));
}


CSetReturnCode::~CSetReturnCode()
{
}


bool CSetReturnCode::IsInternal() 
{
    return true; 
}


//-----------------------------------------------------------------------------
// Examines the operands from the XML, which is expected to have the form:
//
//  <action>
//      <action-name>set-return-code</action-name>
//      <parameter>
//          <value>rc-value</value>
//      </parameter>
//
//      ...
//
//  </action>
// The first <parameter> tag specifies the value for the return code of the synchronous call.
//-----------------------------------------------------------------------------
HRESULT CSetReturnCode::ParseOperands()
{ 
    HRESULT hr = S_OK;
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
    {
        ITRACE_ERROR(_T("Missing return code value for <action> named:  set-return-code"));
        return SECURITY_MISSING_RC;
    }
    StringType sRCValue = *iter;
    m_nRc = _tstoi(sRCValue.c_str());
    ITRACE_DEVELOP(_T("rc value=") + IString(m_nRc));

    return hr;
}


HRESULT CSetReturnCode::Execute(bool *pResult, IMessage &message)
{
    IMODTRACE_DEVELOP();

    *pResult = true;

    HRESULT hr = ParseOperands();
    if (hr == S_OK)
    {

        try
        {
            // Save the return code in the message
            SetSynchronousReturnCode(message, m_nRc);
        }
        catch (...)
        { ITRACE_DEVELOP(_T("Problem in SetReturnCode")); }
    }

    return hr;
}


template <>
IAction* CCreateable<CSetReturnCode, IAction>::NewInstance()
{
    return new CSetReturnCode;
}


template <>
void CCreateable<CSetReturnCode, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SetReturnCode)
DEFINE_ACTION_RELEASE_FXN(SetReturnCode)
