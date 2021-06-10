// RCManagerCtrl.cpp : Implementation of the CRCManagerCtrl ActiveX Control class.

#include "stdafx.h"
#include "RCManagerFactory.h"
#include "RCManager.h"
#include "RemoteAPMgr.h"

using namespace fastlane;
using namespace rap;

CRCManager *CRCManagerFactory::m_pRCManager = NULL;
CRCManager *CRCManagerFactory::GetRCManagerInstance()
{
    //IMODTRACE_DEVELOP();

    if (m_pRCManager == NULL)
    {
        m_pRCManager = CSingleton<CRemoteAPMgr>::instance();
    }

    return m_pRCManager;
}



