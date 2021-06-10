// ScotAppBusyStatus.cpp: implementation of the ScotAppBusyStatus class.
//
//////////////////////////////////////////////////////////////////////
//Implemented for RFC 263404

#include "stdafx.h"
#include "Common.h"
#include "ScotAppBusyStatus.h"
#include "ScotBusyStatus.h"
#include "ErrorObject.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ScotAppBusyStatus")


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ScotAppBusyStatus::ScotAppBusyStatus(LPCTSTR sAppName)
:m_sAppName(sAppName), m_lLeaseMinutes(0), m_bBusy(false)
{
}

ScotAppBusyStatus::~ScotAppBusyStatus()
{
}

void ScotAppBusyStatus::Initialize()
{
    trace(L6, _T("+ScotAppBusyStatus::Initialize"));
    trace(L6, _T("Initialized ScotBusy object, return code = %x"), InitApplBusy());
}

//////////////////////////////////////////////////////////////////////
// Method:     ClearBusyStatus
// Purpose:    Immediately clears RSM busy status for m_sAppName 
// Parameters: None
// Return:     N/A
// Comments:   ClearApplBusy does not have any harmful effects 
//             if it is called when the 
//             application has not been marked as busy.
//////////////////////////////////////////////////////////////////////
void ScotAppBusyStatus::ClearBusyStatus()
{
    m_lLeaseMinutes=0;
    m_bBusy = false;
    trace(L6, _T("+ScotAppBusyStatus::ClearBusyStatus"));
    if (m_lLeaseMinutes > 0)
    {
        trace(L6, _T("ClearBusyStatus(%s) returned = %x"), m_sAppName, ClearApplBusy(m_sAppName));
    }
    else
    {
        trace(L6, _T("ClearBusyStatus: AppBusy not enabled, doing nothing"));
    }
}

//////////////////////////////////////////////////////////////////////
// Method:     GetSystemBusyStatus
// Purpose:    Retrieves the RSM overall busy status 
// Parameters: None
// Returns:    Number of applications that are busy, or
//             0xFFFFFFFF if number cannot be retrieved.
//////////////////////////////////////////////////////////////////////
long ScotAppBusyStatus::GetSystemBusyStatus()
{
    trace(L6, _T("+ScotAppBusyStatus::GetSystemBusyStatus()"));
    long lBusyCount = GetApplBusyStatus();
    trace(L6, _T("GetSystemBusyStatus returning count of %x"), lBusyCount);
    return lBusyCount;
}

//////////////////////////////////////////////////////////////////////
// Method:  RenewBusyStatus
// Purpose: Renew a lease only if it is close to expiring.  Used by
//          Scotapp's IdleTimer function to renew a lease during 
//          transactions or while in store mode.  Uses the lease
//          time saved during the last SetBusyStatus call to renew the
//          lease.
// Parameters: None
// Return:  N/A
//////////////////////////////////////////////////////////////////////
void ScotAppBusyStatus::RenewBusyStatus()
{
    CTimeSpan tTimeElapsed;
    trace(L9, _T("+ScotAppBusyStatus::RenewBusyStatus()"));
    try
    {
        if (!m_bBusy)
        {
            trace(L9, _T("-ScotAppBusyStatus::RenewBusyStatus(): App not busy"));
            return;
        }        
        tTimeElapsed = CTime::GetCurrentTime() - m_LastRenewalTime;
        
        long lTimeLeftInLease = (m_lLeaseMinutes * 60) - tTimeElapsed.GetTotalSeconds();
        if ( lTimeLeftInLease < 30)
        {
            long lResult = SetApplBusy(m_sAppName, m_lLeaseMinutes); 
            if (lResult == BUSY_SUCCESS)
            {
                m_LastRenewalTime = CTime::GetCurrentTime();
            }
            else
            {
                m_bBusy = false; //Can't verify busy lease anymore
            }
            trace(L9, _T("-ScotAppBusyStatus::RenewBusyStatus() lease renewed"));
        }
        else
        {
            trace(L9, _T("-ScotAppBusyStatus::RenewBusyStatus() %d seconds left, not renewing"));
        }
    }
    catch(...)
    {
    }
    return;
}

//////////////////////////////////////////////////////////////////////
// Method:  SetBusyStatus
// Purpose: Create an RSM application busy lease
// Parameters: Number of minutes the application will be considered busy
// Return:  N/A
//////////////////////////////////////////////////////////////////////
void ScotAppBusyStatus::SetBusyStatus(long nExpirationMinutes)
{
    long lResult;

    m_lLeaseMinutes = nExpirationMinutes;
    if (m_lLeaseMinutes > 0)
    {
        trace(L6, _T("+ScotAppBusyStatus::SetBusyStatus(%d minutes)"), nExpirationMinutes);
        try
        {
            lResult = SetApplBusy(m_sAppName, nExpirationMinutes);
            if(lResult == BUSY_SUCCESS)
            {
                m_bBusy = true;
                m_lLeaseMinutes = nExpirationMinutes;
                m_LastRenewalTime = CTime::GetCurrentTime();
            }
            trace(L6, _T("SetBusyStatus(%s, %d) returned %x"), m_sAppName, nExpirationMinutes, lResult);
        }
        catch(...)
        {
        }
    }
    else
    {
        m_bBusy = false;
    }
}
