#include "StdAfx.h"
#include "QueuedActionGenericDelayedIntervention.h"

#define COMP_ID ID_SM
#define T_ID _T("ShowMessage")

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#else
#include "FakeSMStateBase.h"
#include "DMTriColorLightConstants.h"
#endif //_CPPUNIT
#include "SMRAPDataNeeded.h"
#include "PSConfigData.h"
#include "DataNeededUtil.h"
#include "RCMKeyValue.h"

/*  
 *  Queued Action class for delayed message screen. 
 *  csData contains dataneeded view string.  Message screen text, title, and wave
 *  are extracted and used to fill in the CustomMessage object,  after which
 *  the CustomMessage state is returned.
 */

CQueuedActionGenericDelayedIntervention::CQueuedActionGenericDelayedIntervention(const CString &csData)
{
    trace(L6, _T("CQueuedActionGenericDelayedIntervention csData = [%s]"),(LPCTSTR)csData);

    m_DataNeededUtil = new CDataNeededUtil(csData);
    m_csActionString = csData;

    m_DataNeededUtil->GetString(m_csActionString);

    m_csID = m_DataNeededUtil->GetString(_T("ID"));

    m_csInterventionInstruction = m_DataNeededUtil->GetString(_T("INTERVENTIONINSTRUCTION"));
}

CQueuedActionGenericDelayedIntervention::~CQueuedActionGenericDelayedIntervention()
{
    delete m_DataNeededUtil;
}

SMStateBase* CQueuedActionGenericDelayedIntervention::HandleQueuedAction(SMStateBase* pCurrentState)
{
    pCurrentState->g_bTBNeedMoreData = true;  // TAR 238061
    CString csDesc = m_DataNeededUtil->GetDescription();
    if (csDesc.IsEmpty())
    {
        csDesc = _T(" ");
    }
    CREATE_AND_DISPENSE(RAPDataNeeded)(csDesc, DATANEEDED_MOREDATA, m_csActionString, pCurrentState->storeMode());
}

CString CQueuedActionGenericDelayedIntervention::GetInterventionInstruction() const
{
    return m_csInterventionInstruction;
}

CString CQueuedActionGenericDelayedIntervention::GetVideoPath()
{
    return m_DataNeededUtil->GetString(_T("VideoPath"));
}

int CQueuedActionGenericDelayedIntervention::GetReportingID()
{
    int iValue = 0;
    m_DataNeededUtil->GetInt(KEY_EV_COOKIE, iValue);
    return iValue;
}

void CQueuedActionGenericDelayedIntervention::SetReportingID(int iValue)
{
    CString csReportingID;
    csReportingID.Format(_T("%d"), iValue);
    m_DataNeededUtil->GetScotDataEntryObject()->InsertString(KEY_EV_COOKIE, csReportingID);
    m_DataNeededUtil->GetInt(KEY_EV_COOKIE, iValue);
}

DMTriColorLightColor CQueuedActionGenericDelayedIntervention::GetLightColor()
{
    DMTriColorLightColor retVal = DM_TRICOLORLIGHT_YELLOW;
    int nValue = 0;
    bool bResult = m_DataNeededUtil->GetInt(_T("TRILIGHTCOLOR"), nValue);
    if (bResult)
    {
        if (retVal >= DM_TRICOLORLIGHT_GREEN && retVal <= DM_TRICOLORLIGHT_RED)
        {
            retVal = (DMTriColorLightColor) nValue;
        }
    }
    return retVal;
}

DMTriColorLightState CQueuedActionGenericDelayedIntervention::GetLightState()
{
    DMTriColorLightState retVal = DM_TRICOLORLIGHT_ON;
    int nValue = 0;
    bool bResult = m_DataNeededUtil->GetInt(_T("TRILIGHTSTATE"), nValue);
    if (bResult)
    {
        if (retVal >= DM_TRICOLORLIGHT_OFF && retVal <= DM_TRICOLORLIGHT_BLINK_4HZ)
        {
            retVal = (DMTriColorLightState) nValue;
        }
    }
    return retVal;
}

