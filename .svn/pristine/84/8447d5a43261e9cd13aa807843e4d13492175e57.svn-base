// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/PSXConnectionThread.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)PSXConnectionThread.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
 *
 * Copyright 2005, NCR Corporation.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */

/*lint -save -e666 */

#include "stdafx.h"
#include "PSXConnectionThread.h"
#include "RCMStateMachine.h"
#include "RCMMessages.h"
#include "Loggable.h"
#include "Message.h"
#include "Action.h"
#include "PSXMfc.h"
#include "verifypsx.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace fastlane;


/**
* Default no-argument constructor
*/
CPSXConnectionThread::CPSXConnectionThread(HTRACE hTrace)
    : CThreadMemberBase<CPSXConnectionThread>(true)
    , m_pMsg(NULL)
{
    CTOR_BEGIN(hTrace)
    IMODTRACE_DEVELOP();
    CTOR_END()
    // DO NOT put any more initialization steps after this.
}


void CPSXConnectionThread::Start(IMessage &message)
{
    _ASSERTE(!m_sParmList.empty());

    m_pMsg = &rcm().GetStateMachine()->MakeAsynchCopyEx(message, MSG_CONNECTION_COMPLETE);

    _ASSERTE(m_pMsg!=NULL);

    CThreadBase::Start();
}


CPSXConnectionThread::~CPSXConnectionThread()
{
    IMODTRACE_DEVELOP();

    delete m_pMsg;
}

void CPSXConnectionThread::SetParmlist(const StringType &sParmlist)
{
    m_sParmList = sParmlist;
}


#pragma warning( push )
#pragma warning( disable : 4311 )
unsigned CPSXConnectionThread::Run()
{
    HRESULT hr = S_OK;
    {
        IMODTRACE_DEVELOP();

        try
        {
            StringType sRemoteConnectionName = CRemoteConnection::GetRemoteConnectionName(m_pMsg);

            ITRACE_DEVELOP(IString(_T(__FUNCTION__)) + _T("  Accepting connection from ") + sRemoteConnectionName.c_str());

            // Accept the connection
            _variant_t vParm = m_sParmList.c_str();

            clock_t ctStart = clock();

            VERIFY_PSX(hr,  psx().AcceptConnection( sRemoteConnectionName.c_str()
                                                , vParm
                                                , TRUE ) );

            long lTime = clock() - ctStart;
            ITRACE_DEVELOP(IString(_T(__FUNCTION__)) + _T("  AcceptConnection took ") + IString(lTime) );

            if (hr == S_OK)
            {
                rcm().GetStateMachine()->PostMsg(*m_pMsg, false);
                m_pMsg = NULL;
            }
        }
        catch (const IException &exc)
        {
            ITRACE_EXCEPTION(exc);

            _ASSERTE(0);
        }
        catch (...)
        {
            ITRACE_DOT_ERROR();

            _ASSERTE(0);
        }
    }

    return hr;
}
#pragma warning( pop )

/*lint -restore */
