#include "StdAfx.h"
#include "TimerImpl.h"
#include "TimerHandler.h"

using namespace STATE_MACHINE_NAMESPACE;


CTimerImpl::CTimerImpl(int nId)
    : nId(nId)
//  , m_pCallback(NULL)
    , m_nTimeoutDuration(0)
    , m_bPeriodic(false)
{}


CTimerImpl::~CTimerImpl(void)
{}


DEFINE_STRING_PROPERTY(CTimerImpl, Name);
DEFINE_STRING_PROPERTY(CTimerImpl, Comment);

int CTimerImpl::GetId() const { return nId; }


const _TCHAR *CTimerImpl::GetComment() { return Comment.Value().c_str(); }


const _TCHAR *CTimerImpl::GetName() { return Name.Value().c_str(); }


const _TCHAR *CTimerImpl::GetTimeoutName() { return Name.Value().c_str(); }


int CTimerImpl::GetTimeoutDuration() { return m_nTimeoutDuration; }


void CTimerImpl::SetTimeoutDuration(int nTimeoutDuration)
{ 
    m_nTimeoutDuration = nTimeoutDuration;
}


int CTimerImpl::GetTimerHandle()
{ 
    return m_hTimer;
}


void CTimerImpl::SetTimerHandle(int hTimer)
{ 
    m_hTimer = hTimer;
}


bool CTimerImpl::IsPeriodic()
{ 
    return m_bPeriodic;
}


void CTimerImpl::SetPeriodic(bool bPeriodic)
{ 
    m_bPeriodic = bPeriodic;
}


StringType CTimerImpl::GetXMLOutput(CXMLBuilder *pXMLBuilder)
{
    if (pXMLBuilder == NULL)
        ((CXMLBuilder *)CTimerHandler::instance())->GetXMLOutput(this);
    else
        pXMLBuilder->GetXMLOutput(this);

    return (pXMLBuilder == NULL ? CTimerHandler::instance() : pXMLBuilder)->GetXMLOutput();
}
