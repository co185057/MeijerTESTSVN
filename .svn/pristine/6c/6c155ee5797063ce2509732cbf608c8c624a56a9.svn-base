#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/PSXConnectionThread.h 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)PSXConnectionThread.h    $Revision: 3 $ $Date: 10/21/08 4:09p $
 *
 * Copyright 2004, NCR Corporation.
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */


#include "RCMApi.h"
#include "SMConstants.h"
#include "RCManager.h"
#include "xerror.h"
#include "xthread.h"
#include "statem.h"

#define INVALID_THREAD_ID   (unsigned)0xFFFFFFFF

#pragma warning( push )
#pragma warning( disable : 4251 )

namespace STATE_MACHINE_NAMESPACE
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;


namespace fastlane
{

//lint --e{1735}        Virtual fxns with default args OK
class RCMANAGER_API CPSXConnectionThread
    : public CThreadMemberBase<CPSXConnectionThread>
{
public:

    friend class CThreadMemberBase<CPSXConnectionThread>;

    ///
    /// Default no-argument constructor
    ///
    CPSXConnectionThread(HTRACE hTrace);

    virtual ~CPSXConnectionThread();

    virtual void Start(IMessage &);

    void SetParmlist(const StringType &);


protected:

    unsigned Run();

    CRCManager &rcm() { return *CSingleton<CRCManager>::instance(); }

    CPSX &psx() { return rcm().psx(); }

private:
    
    CPSXConnectionThread();

    StringType m_sParmList;

    IMessage *m_pMsg;

};

}
#pragma warning( pop )
