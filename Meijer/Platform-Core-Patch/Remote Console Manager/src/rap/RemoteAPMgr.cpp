// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RemoteAPMgr.cpp 5     1/12/10 11:59p Cu185008 $
/*
 * @(#)RCManager.cpp    $Revision: 5 $ $Date: 1/12/10 11:59p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/RemoteAPMgr.cpp $
 * 
 * 5     1/12/10 11:59p Cu185008
 * G2 Migration.
 * 
 * 4     2/27/09 1:05p De200005
 * TAR390603
 * 
 * 3     10/21/08 4:04p Sm133076
 * Patch G Migration
 * 
 * 22    1/22/08 4:56a pl250020
 * TAR 352409 : Added for to handle new Suspension Decline Event.
 *
 * 21    11/29/07 4:33p Cb160006
 * TAR 364905
 *
 * 20    12/22/06 10:35a Jj100011
 * RFC 337360
 *
 * 19    10/20/06 9:50a Tn185011
 * Roll back to 244
 *
 * 17    5/31/05 11:32a Dm185016
 * Data Model Support
 * TAR 300883
 *
 * 17    5/20/05 3:29p Dm185016
 * TAR 301079
 *
 * 16    5/19/05 5:21p Dm185016
 * TAR 300190
 *
 * 15    5/02/05 2:58p Tp151000
 * TAR 299442
 *
 * 14    5/01/05 8:57a Tp151000
 * Performmance
 *
 * 13    4/15/05 9:11a Dm185016
 *
 * 12    4/14/05 10:05p Dm185016
 * TAR 298021
 *
 * 11    4/05/05 11:54a Dm185016
 * TAR 297356
 *
 * 10    3/25/05 9:59a Dm185016
 * TAR 296505
 *
 * 9     3/22/05 3:47p Dm185016
 * UNICODE
 *
 * 8     3/11/05 1:08p Dm185016
 * Factored out RAP specific API features
 *
 * 7     3/08/05 4:36p Tp151000
 * 295124
 *
 * TP & AQ
 *
 * 6     3/01/05 4:42p Dm185016
 *
 * 5     3/01/05 1:59p Dm185016
 * TAR 294587.  Added application state functionality.
 *
 * 4     2/28/05 4:05p Dm185016
 * lint
 *
 * 3     2/17/05 5:51p Dm185016
 * Replaced std::stack with std::list.
 *
 * 2     2/03/05 5:41p Dm185016
 * TAR 292378 - Converted single immediate intervention to use of stack
 *
 * 1     1/26/05 2:17p Dm185016
 * Moved to new Repository.
//
// 16    1/20/05 10:35a Dm185016
// Added handler for lane context switches.
//
// 15    1/19/05 2:07p Dm185016
// TAR 290775 - Operator not available on any but the one that handled the
// signon.
//
// 14    1/18/05 3:43p Dm185016
// UNICODE support
//
// 13    1/12/05 5:22p Dm185016
// TAR 290169 - Multiple interventions of the same kind were not clearing
// with single approval.
//
// 12    12/22/04 10:29a Dm185016
// TAR 288934 - priority of interventions is now respected.  Urgent
// interventions are added to the head of the Q.  deferred are still added
// at the tail.
//
// 11    12/14/04 11:16a Dm185016
// Removed unneccessary code for HH support
//
// 10    12/09/04 1:44p Dm185016
// TARs 287903, 287907
//
// 9     12/07/04 4:15p Dm185016
// Added tracing for lane name
//
// 8     12/02/04 3:31p Dm185016
// Added inside transaction indicator
//
// 7     11/30/04 4:17p Dm185016
// Added PushbackDelayedIntervention().
// Delayed intervention list now derived from std::list rather than
// std::queue.
//
// 6     11/22/04 3:42p Dm185016
// Added method to set the current intervention.
//
// 5     11/19/04 1:32p Dm185016
// Added current intervention field
//
// 4     11/18/04 1:36p Dm185016
// Added lane name to config file.
//
// 3     11/16/04 11:23a Dm185016
// More updates
//
// 2     10/29/04 8:52a Dm185016
// Added delayed intervention functions.
// Added config for RAP.ini
//
// 1     10/21/04 2:36p Dm185016
//
// 2     10/14/04 1:38p Dm185016
// New OnInputMethod Api.
// PSX now passed as handle.
//
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/


#include "StdAfx.h"
#include "RemoteAPMgr.h"
#include "SMError.h"
#include "RCMStateMachineRAP.h"
#include "RCMRecorder.h"
#include "RCMgrAPI.h"
#include "RCMError.h"
#include "Configuration.h"
#include "KVParmList.h"
#include "RCMKeyValue.h"
#include "RCMConfigDefaults.h"
#include "Operator.h"
#include "RAConnections.h"
#include "RAEvents.h"
#include "verify.h"
//#include "PSXMFC.h"

using namespace fastlane;
using namespace rap;
using namespace STATE_MACHINE_NAMESPACE;


CRemoteAPMgr::CRemoteAPMgr()
    : CRCManager()
    , m_pStateMachine(NULL)
    , m_pApplicationState(NULL)
    , m_pNullApplicationState(NULL)
    , m_CurrentIntervention(NULL)
    , m_pCurrentApproval(NULL)
    , m_pOperator(NULL)
    , m_pCurrentContextSwitch(NULL)
	, m_bHHAssistModeEnabled(true)
	, m_bAssistModeEnabled(true)
	, m_bAssistModeRequested(false)
    , m_bInputEventsEnabled(true)
    , m_bInsideTransaction(false)
    , m_bAllowSecurityApproval(true)
    , m_bAllowCouponApproval(true)
    , m_bAllowVoidApproval(true)
    , m_bAllowAgeRestrictionApproval(true)
    , m_bAllowVisualValidationApproval(true)
	,m_bAllowHopperSubstitutionApproval(true)
    , m_bAllowCallForHelpApproval(true)
    , m_bAllowOtherPaymentApproval(true)
	, m_bAllowSuspendApproval(true)
    , m_bAllowPriceRequiredApproval(true)
    , m_bAllowUnknownItemApproval(true)
    , m_bAllowRecalledItemApproval(true)
    , m_bAllowTimeRestrictedItemApproval(true)
    , m_bAllowSigCapApproval(true)
    , m_bAllowApplicationErrorApproval(true)
    , m_bAllowDeviceErrorApproval(true)
    , m_bAllowQuantityRestrictedItemApproval(true)
    , m_bAllowUndeactivatedItemApproval(true)
    , m_bAllowRestrictedNotAllowedApproval(true)
    , m_lReminderDisplayTime(60000)
    , m_lAssistModeControl(1)
    , m_lClearReceiptTime(15000)
    , m_bConfirmationForSuspendingTransaction(false)
    , m_bAutoAssistOnEnable(false)
    , m_bAllowParialAssistMode(true)
    , m_lItemCount(0)
    , m_eConnectionMode(RA_CONNECTION_MODE_CUSTOMER)
    , m_eCurrentTriliteColor(None)
    , m_presInterventionsListLock(new CResource(_T("OIL")))
	, m_bApprovalBeforeSuspend(false)
	, m_bAllowSuspensionDeclinedApproval(true)			// TAR 352409
{
    CSingleton<CRCManager>::SetInstance(this);

    m_pNullApplicationState     = newpooledobject CApplicationState;
    m_pApplicationState         = m_pNullApplicationState;

    m_sLaneName = _T("TestName");
}


CRemoteAPMgr::~CRemoteAPMgr()
{
    IMODTRACE_DEVELOP();
    delete m_presInterventionsListLock; //TR390603

    delete m_pNullApplicationState;
    m_pNullApplicationState = NULL;

    m_pApplicationState = NULL;

    delete m_CurrentIntervention;

    m_pCurrentApproval      = NULL;
    m_pOperator             = NULL;
    m_pCurrentContextSwitch = NULL;
}


HRESULT CRemoteAPMgr::LoadOptions()
{
    ITRACE_DEVELOP(_T("_____________________________________________________________"));
    ITRACE_DEVELOP(_T(__FUNCTION__) _T(":  Loading configuration parameters:"));

    HRESULT hr = CRCManager::LoadOptions();
    if (hr != S_OK)
        return hr;

    CConfigManager &cfgMgr = *CSingleton<CRemoteAPMgr>::instance()->GetConfigMgr();

    m_bAllowSecurityApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_SECURITY
                                , DEFAULT_APPROVAL_TYPE_SECURITY);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_SECURITY _T(":  ")
                   + IString(m_bAllowSecurityApproval));

    m_bAllowCouponApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_COUPON
                                , DEFAULT_APPROVAL_TYPE_COUPON);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_COUPON _T(":  ")
                   + IString(m_bAllowCouponApproval));

    m_bAllowVoidApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_VOID
                                , DEFAULT_APPROVAL_TYPE_VOID);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_VOID _T(":  ")
                   + IString(m_bAllowVoidApproval));

    m_bAllowAgeRestrictionApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_AGE_RESTRICTION
                                , DEFAULT_APPROVAL_TYPE_AGE_RESTRICTION);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_AGE_RESTRICTION _T(":  ")
                   + IString(m_bAllowAgeRestrictionApproval));

    m_bAllowVisualValidationApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_VISUAL_VALIDATION
                                , DEFAULT_APPROVAL_TYPE_VISUAL_VALIDATION);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_VISUAL_VALIDATION _T(":  ")
                   + IString(m_bAllowVisualValidationApproval));

	m_bAllowHopperSubstitutionApproval = CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_HOPPER_SUBSTITUTION
                                , DEFAULT_APPROVAL_TYPE_HOPPER_SUBSTITUTION);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_HOPPER_SUBSTITUTION _T(":  ")
                   + IString(m_bAllowHopperSubstitutionApproval));

    m_bAllowCallForHelpApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_CALL_FOR_HELP
                                , DEFAULT_APPROVAL_TYPE_CALL_FOR_HELP);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_CALL_FOR_HELP _T(":  ")
                   + IString(m_bAllowCallForHelpApproval));

    m_bAllowOtherPaymentApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_OTHER_PAYMENT
                                , DEFAULT_APPROVAL_TYPE_OTHER_PAYMENT);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_OTHER_PAYMENT _T(":  ")
                   + IString(m_bAllowOtherPaymentApproval));

    m_bAllowSuspendApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_SUSPEND
                                , DEFAULT_APPROVAL_TYPE_SUSPEND);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_SUSPEND _T(":  ")
                   + IString(m_bAllowSuspendApproval));

    m_bAllowPriceRequiredApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_PRICE_REQUIRED
                                , DEFAULT_APPROVAL_TYPE_PRICE_REQUIRED);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_PRICE_REQUIRED _T(":  ")
                   + IString(m_bAllowPriceRequiredApproval));

    m_bAllowUnknownItemApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_UNKNOWN_ITEM
                                , DEFAULT_APPROVAL_TYPE_UNKNOWN_ITEM);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_UNKNOWN_ITEM _T(":  ")
                   + IString(m_bAllowUnknownItemApproval));

    m_bAllowRecalledItemApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_RECALLED_ITEM
                                , DEFAULT_APPROVAL_TYPE_RECALLED_ITEM);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_RECALLED_ITEM _T(":  ")
                   + IString(m_bAllowRecalledItemApproval));

    m_bAllowTimeRestrictedItemApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM
                                , DEFAULT_APPROVAL_TYPE_TIME_RESTRICTED_ITEM);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_TIME_RESTRICTED_ITEM _T(":  ")
                   + IString(m_bAllowTimeRestrictedItemApproval));

    m_bAllowSigCapApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_SIG_CAP
                                , DEFAULT_APPROVAL_TYPE_SIG_CAP);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_SIG_CAP _T(":  ")
                   + IString(m_bAllowSigCapApproval));

    m_bAllowApplicationErrorApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_APPLICATION_ERROR
                                , DEFAULT_APPROVAL_TYPE_APPLICATION_ERROR);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_APPLICATION_ERROR _T(":  ")
                   + IString(m_bAllowApplicationErrorApproval));

    m_bAllowDeviceErrorApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_DEVICE_ERROR
                                , DEFAULT_APPROVAL_TYPE_DEVICE_ERROR);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_DEVICE_ERROR _T(":  ")
                   + IString(m_bAllowDeviceErrorApproval));

    m_bAllowQuantityRestrictedItemApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM
                                , DEFAULT_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_QUANTITY_RESTRICTED_ITEM _T(":  ")
                   + IString(m_bAllowQuantityRestrictedItemApproval));

    m_bAllowUndeactivatedItemApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM
                                , DEFAULT_APPROVAL_TYPE_UNDEACTIVATED_ITEM);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_UNDEACTIVATED_ITEM _T(":  ")
                   + IString(m_bAllowUndeactivatedItemApproval));

    m_bAllowRestrictedNotAllowedApproval =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED
                                , DEFAULT_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_RESTRICTED_NOT_ALLOWED _T(":  ")
                   + IString(m_bAllowRestrictedNotAllowedApproval));


    m_lReminderDisplayTime =
        CConfiguration<long>::Get(cfgMgr
                                , CONFIG_KEY_REMINDER_DISPLAY_TIME
                                , DEFAULT_REMINDER_DISPLAY_TIME);
    ITRACE_DEVELOP(CONFIG_KEY_REMINDER_DISPLAY_TIME _T(":  ")
                   + IString(m_lReminderDisplayTime));

    m_lAssistModeControl =
        CConfiguration<long>::Get(cfgMgr
                                , CONFIG_KEY_ASSIST_MODE_CONROL
                                , DEFAULT_ASSIST_MODE_CONROL);
    ITRACE_DEVELOP(CONFIG_KEY_ASSIST_MODE_CONROL _T(":  ")
                   + IString(m_lAssistModeControl));

    m_lClearReceiptTime =
        CConfiguration<long>::Get(cfgMgr
                                , CONFIG_KEY_CLEAR_RECEIPT_TIME
                                , DEFAULT_CLEAR_RECEIPT_TIME);
    ITRACE_DEVELOP(CONFIG_KEY_CLEAR_RECEIPT_TIME _T(":  ")
                   + IString(m_lClearReceiptTime));

    m_bConfirmationForSuspendingTransaction =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_CONFIRMATION_FOR_SUSPENDING_XACT
                                , DEFAULT_CONFIRMATION_FOR_SUSPENDING_XACT);
    ITRACE_DEVELOP(CONFIG_KEY_CONFIRMATION_FOR_SUSPENDING_XACT _T(":  ")
                   + IString(m_bConfirmationForSuspendingTransaction));

    m_bAutoAssistOnEnable =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_AUTO_ASSIST_ON_ENABLE
                                , DEFAULT_AUTO_ASSIST_ON_ENABLE);
    ITRACE_DEVELOP(CONFIG_KEY_AUTO_ASSIST_ON_ENABLE _T(":  ")
                   + IString(m_bAutoAssistOnEnable));

    m_bAllowParialAssistMode =
        CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_ALLOW_PARTIAL_ASSIST_MODE
                                , DEFAULT_ALLOW_PARTIAL_ASSIST_MODE);
    ITRACE_DEVELOP(CONFIG_KEY_ALLOW_PARTIAL_ASSIST_MODE _T(":  ")
                   + IString(m_bAllowParialAssistMode));

    m_sLaneName =
        CConfiguration<const _TCHAR *>::Get(cfgMgr
                                , CONFIG_KEY_LANE_NAME
                                , _T(""));
    ITRACE_DEVELOP(CONFIG_KEY_LANE_NAME _T(":  ")
                   + IString(m_sLaneName.c_str()));

	// +TAR 352409
	m_bAllowSuspensionDeclinedApproval =
	CConfiguration<bool>::Get(cfgMgr
                                , CONFIG_KEY_APPROVAL_TYPE_SUSPENSION_DECLINE
                                , DEFAULT_APPROVAL_TYPE_SUSPENSION_DECLINE);
    ITRACE_DEVELOP(CONFIG_KEY_APPROVAL_TYPE_SUSPENSION_DECLINE _T(":  ")
                   + IString(m_bAllowSuspensionDeclinedApproval));
	// -TAR 352409

    return S_OK;
}


CRCMStateMachine *CRemoteAPMgr::GetStateMachine(const _TCHAR *pSMName)
{
    IMODTRACE_DEVELOP();

    if (m_pStateMachine == NULL)
        m_pStateMachine = new CRCMStateMachineRAP(pSMName, GetTraceHandle());

    return m_pStateMachine;
}


CRCMStateMachineRAP *CRemoteAPMgr::GetStateMachineRAP()
{
    _ASSERTE(m_pStateMachine!=NULL);
    return m_pStateMachine;
}


unsigned long CRemoteAPMgr::GetNextItem() throw()
{
    m_lItemCount++;

    return m_lItemCount;
}


CApplicationState *CRemoteAPMgr::GetCurrentApplicationState() { return m_pApplicationState; }
void CRemoteAPMgr::SetCurrentApplicationState(CApplicationState *pAppState) { m_pApplicationState = pAppState; }


bool CRemoteAPMgr::HasOperator () const { return m_pOperator!=NULL; }
void CRemoteAPMgr::SetOperator (COperator *pOperator)
{
    delete m_pOperator;

    m_pOperator = pOperator;
}
const COperator &CRemoteAPMgr::GetOperator () const
{
    _ASSERTE(m_pOperator!=NULL);

    if (m_pOperator==NULL)
        throw IException(_T("No operator available"));
    return *m_pOperator;
}

void CRemoteAPMgr::SetConnectionMode(EConnectionModes eConnectionMode) { m_eConnectionMode = eConnectionMode; }
EConnectionModes CRemoteAPMgr::GetConnectionMode() const { return m_eConnectionMode; }


void CRemoteAPMgr::SetAssistModeEnabled(bool bAMEnabled) { m_bAssistModeEnabled = bAMEnabled; }
bool CRemoteAPMgr::IsAssistModeEnabled() const { return m_bAssistModeEnabled; }


void CRemoteAPMgr::SetHandheldAssistModeEnabled(bool bHHAMEnabled) { m_bHHAssistModeEnabled = bHHAMEnabled; }
bool CRemoteAPMgr::IsHandheldAssistModeEnabled() const { return m_bHHAssistModeEnabled; }


void CRemoteAPMgr::SetAssistModeRequested(bool bAMRequested) { m_bAssistModeRequested = bAMRequested; }
bool CRemoteAPMgr::IsAssistModeRequested() const { return m_bAssistModeRequested; }


void CRemoteAPMgr::SetAssistModeControl(long lValue) { m_lAssistModeControl = lValue; }
long CRemoteAPMgr::GetAssistModeControl() const { return m_lAssistModeControl; }


void CRemoteAPMgr::SetClearReceiptTime(long lValue) { m_lClearReceiptTime = lValue; }
long CRemoteAPMgr::GetClearReceiptTime() const { return m_lClearReceiptTime; }


void CRemoteAPMgr::SetReminderDisplayTime(long lValue) { m_lReminderDisplayTime = lValue; }
long CRemoteAPMgr::GetReminderDisplayTime() const { return m_lReminderDisplayTime; }


void CRemoteAPMgr::SetConfirmationForSuspendingTransaction(bool lValue) { m_bConfirmationForSuspendingTransaction = lValue; }
bool CRemoteAPMgr::IsConfirmationForSuspendingTransaction() const { return m_bConfirmationForSuspendingTransaction; }


void CRemoteAPMgr::SetAutoAssistOnEnable(bool lValue) { m_bAutoAssistOnEnable = lValue; }
bool CRemoteAPMgr::IsAutoAssistOnEnable() const { return m_bAutoAssistOnEnable; }


void CRemoteAPMgr::SetAllowParialAssistMode(bool lValue) { m_bAllowParialAssistMode = lValue; }
bool CRemoteAPMgr::IsAllowParialAssistMode() const { return m_bAllowParialAssistMode; }


void CRemoteAPMgr::SetInputEventsEnabled(bool lValue) { m_bInputEventsEnabled = lValue; }
bool CRemoteAPMgr::IsInputEventsEnabled() const { return m_bInputEventsEnabled; }


void CRemoteAPMgr::SetInsideTransaction(bool lValue) { m_bInsideTransaction = lValue; }
bool CRemoteAPMgr::IsInsideTransaction() const { return m_bInsideTransaction; }
bool CRemoteAPMgr::IsOutsideTransaction() const { return !m_bInsideTransaction; }


const _TCHAR *CRemoteAPMgr::GetCurrentLaneContext() const { return m_sCurrentLaneContext.c_str(); }
void CRemoteAPMgr::SetCurrentLaneContext(const _TCHAR *pszCurrentLaneContext) { m_sCurrentLaneContext = pszCurrentLaneContext; }

ETriliteColors CRemoteAPMgr::GetCurrentTriliteColor() const { return m_eCurrentTriliteColor ; }
void CRemoteAPMgr::SetCurrentTriliteColor(ETriliteColors newColor) { m_eCurrentTriliteColor = newColor; }
bool CRemoteAPMgr::IsCurrentTriliteColor(ETriliteColors newColor) const { return (m_eCurrentTriliteColor == newColor); }


void CRemoteAPMgr::SetPreviousInterventionColor(ETriliteColors newColor) { m_ePreviousInterventionColor = newColor; }
ETriliteColors CRemoteAPMgr::GetPreviousInterventionColor() const { return m_ePreviousInterventionColor; }


CContextSwitch *CRemoteAPMgr::GetCurrentContextSwitch() { return m_pCurrentContextSwitch; }
void CRemoteAPMgr::SetCurrentContextSwitch(CContextSwitch *pCS) { m_pCurrentContextSwitch = pCS; }


void CRemoteAPMgr::SetMinimumAge(int nMinimumAge)
{
	m_ValidBirthday = COleDateTime::GetCurrentTime();

    int nYear   = m_ValidBirthday.GetYear() - nMinimumAge;
    int nMonth  = m_ValidBirthday.GetMonth();
    int nDay    = m_ValidBirthday.GetDay();

    m_ValidBirthday.SetDate(nYear, nMonth, nDay);
}
const COleDateTime &CRemoteAPMgr::GetValidBirthday() const { return m_ValidBirthday; }


void CRemoteAPMgr::SetImmediateIntervention(CIntervention * pIntervention)
{
    m_ImmediateInterventions.push_front(pIntervention);
}


CIntervention * CRemoteAPMgr::GetImmediateIntervention() const
{
    return m_ImmediateInterventions.front();
}


bool CRemoteAPMgr::IsImmediateInterventionPending() const
{
    return !m_ImmediateInterventions.empty();
}


int CRemoteAPMgr::GetImmediateInterventionPendingCount() const { return (int)m_ImmediateInterventions.size(); }


CIntervention * CRemoteAPMgr::GetNextImmediateIntervention()
{
    if (m_ImmediateInterventions.empty())
        return NULL;

    CIntervention *pIntervention = m_ImmediateInterventions.front();

    m_ImmediateInterventions.pop_front();

    return pIntervention;
}


CIntervention * CRemoteAPMgr::PeekNextImmediateIntervention()
{
    return m_ImmediateInterventions.front();
}


CInterventionList &CRemoteAPMgr::GetImmediateInterventions()
{
    return m_ImmediateInterventions;
}

void CRemoteAPMgr::PushbackImmediateIntervention(CIntervention * pIntervention)
{
    SetImmediateIntervention(pIntervention);
}


void CRemoteAPMgr::RemoveImmediateIntervention(CIntervention * pIntervention)
{
    m_ImmediateInterventions.remove(pIntervention);
}


bool CRemoteAPMgr::IsDelayedInterventionPending()  //TR390603
{
	LockInterventionsList(); //TR390603
	bool bReturn = !m_Interventions.empty();
	UnlockInterventionsList(); //TR390603
	return bReturn;
}


bool CRemoteAPMgr::IsDelayedInterventionTypePending(const _TCHAR *pszInterventionType) const
{
    CInterventionMap::const_iterator iter = m_InterventionTypes.find(pszInterventionType);
	return (iter != m_InterventionTypes.end());
}


int CRemoteAPMgr::GetDelayedInterventionPendingCount() //TR390603
{
	LockInterventionsList(); //TR390603
	int iReturn = (int)m_Interventions.size();
	UnlockInterventionsList(); //TR390603
    return iReturn;
}


void CRemoteAPMgr::SaveInterventionType(CIntervention * pIntervention)
{
    InterventionParms parms;
    HRESULT hr = parms.ParseKeys((LPCTSTR)pIntervention->GetData(), GetTraceHandle());
    if (hr != S_OK)
        ITHROW(IException(_T("Error parsing intervention type"), hr))

    m_InterventionTypes.insert(make_pair((LPCTSTR)parms.GetType(), pIntervention));
}


void CRemoteAPMgr::SetNextDelayedIntervention(CIntervention * pIntervention, DWORD dwPriority)
{
	LockInterventionsList();//TR390603
    switch (dwPriority)
    {
        case RA_PRIORITY_DEFER :
            ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Saving deferred intervention"));
            m_Interventions.push_front(pIntervention);
            SaveInterventionType(pIntervention);
            break;
        case RA_PRIORITY_URGENT :
            ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Saving urgent intervention"));
            m_Interventions.push_front(pIntervention);
            SaveInterventionType(pIntervention);
            break;
        default :
            _ASSERTEX(_T(__FUNCTION__) _T("  Unknown priority detected"));
            break;
    }
	UnlockInterventionsList(); //TR390603
}


void CRemoteAPMgr::PushbackDelayedIntervention(CIntervention * pIntervention)
{
    SetNextDelayedIntervention(pIntervention, RA_PRIORITY_DEFER);
}


CIntervention * CRemoteAPMgr::GetNextDelayedIntervention()
{
	LockInterventionsList(); //TR390603
    if (m_Interventions.empty())
	{
		UnlockInterventionsList(); //TR390603
        return NULL;
	}
    

    CIntervention *pCurrentIntervention = m_Interventions.front();

    m_Interventions.pop_front();
    RemoveDelayedIntervention(pCurrentIntervention, false);

	UnlockInterventionsList(); //TR390603

    return pCurrentIntervention;
}


CIntervention * CRemoteAPMgr::GetInterventionByType(const _TCHAR *pszInterventionType)
{
	LockInterventionsList();//TR390603

    if (m_Interventions.empty())
	{
		UnlockInterventionsList(); //TR390603
        return NULL;
	}

    CIntervention *pCurrentIntervention = NULL;

    CInterventionMap::const_iterator iter = m_InterventionTypes.find(pszInterventionType);
	if (iter != m_InterventionTypes.end())
    {
        pCurrentIntervention = iter->second;
        m_Interventions.remove(pCurrentIntervention);

        m_InterventionTypes.erase(pszInterventionType);
    }

	UnlockInterventionsList(); //TR390603

    return pCurrentIntervention;
}


CIntervention * CRemoteAPMgr::PeekNextDelayedIntervention()
{
	LockInterventionsList(); //TR390603
    if (m_Interventions.empty())
	{
		UnlockInterventionsList(); //TR390603
        return NULL;
	}

    CIntervention *pCurrentIntervention = m_Interventions.front();
	UnlockInterventionsList(); //TR390603
    return pCurrentIntervention;
}


CInterventionList &CRemoteAPMgr::GetDelayedInterventions()
{
    return m_Interventions;
}


//TAR390603+
/*
void CRemoteAPMgr::RemoveDelayedInterventions(PACCEPT_FXN pFxn)
{
    CInterventionList::iterator iter = m_Interventions.begin();
    for (;iter != m_Interventions.end(); iter++)
    {
        CIntervention *pIntervention = *iter;

        // Use the function passed in as a filter for accepting this
        // exception as one that needs to be cleared.
        if ((*pFxn)(pIntervention))
        {
            // Remove the intervention
            RemoveDelayedIntervention(pIntervention);

            ITRACE_DEVELOP(_T("Cleared Intervention:  ") + IString(pIntervention->GetName()));

            // Free the storage for the intervention
            delete pIntervention;
        }
    }
}
*/
void CRemoteAPMgr::RemoveDelayedInterventions(PACCEPT_FXN pFxn)
    {
	  CInterventionList temp_interventions;			

  	  LockInterventionsList();//TR390603

        CInterventionList::iterator iter = m_Interventions.begin();
        for (;iter != m_Interventions.end(); iter++)
        {
            CIntervention *pIntervention = *iter;
    
            // Use the function passed in as a filter for accepting this
            // exception as one that needs to be cleared.
            if ((*pFxn)(pIntervention))
                // Remove the intervention
                temp_interventions.push_front(pIntervention);	
        }
        iter = temp_interventions.begin();					
        for (;iter != temp_interventions.end(); iter++)		
        {
            CIntervention *pIntervention = *iter;
    
            // Remove the intervention
            RemoveDelayedIntervention(pIntervention);
    
            ITRACE_DEVELOP(_T(__FUNCTION__) _T(".  Cleared Intervention:  ") + IString(pIntervention->GetName()));
    
            // Free the storage for the intervention
            delete pIntervention;
        }
		UnlockInterventionsList();//TR390603

    }

//TAR390603-

BEGIN_KV_HANDLER_CLASS(CRemoteAPMgr::InterventionParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CRemoteAPMgr::InterventionParms, KEY_EV_TYPE, Type)
    DEFINE_KV_HANDLER_MAP_ENTRY(CRemoteAPMgr::InterventionParms, KEY_EV_PRIORITY, Priority)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CRemoteAPMgr::InterventionParms, Type)
DEFINE_KV_CSTRING(CRemoteAPMgr::InterventionParms, Priority)
BEGIN_KV_HANDLER_CLASS_CLEAR(CRemoteAPMgr::InterventionParms)
    CLEAR_KV_CSTRING(Type)
    CLEAR_KV_CSTRING(Priority)
END_KV_HANDLER_CLASS_CLEAR()


void CRemoteAPMgr::RemoveDelayedIntervention(CIntervention * pIntervention, bool bRemoveFromList)
{
	LockInterventionsList(); //TR390603
    if (bRemoveFromList)
        m_Interventions.remove(pIntervention);

    InterventionParms parms;
    HRESULT hr = parms.ParseKeys((LPCTSTR)pIntervention->GetData(), GetTraceHandle());
    if (hr != S_OK)
    {
        CRCMErrorInfo error(hr);
        ITHROW(IException(error))
    }

    m_InterventionTypes.erase((LPCTSTR)parms.GetType());
	UnlockInterventionsList(); //TR390603
}


void CRemoteAPMgr::SetCurrentIntervention(CIntervention *pCurrentIntervention)
{
    if (pCurrentIntervention != NULL)
        ITRACE_DEVELOP(_T(__FUNCTION__) _T("  Intervention:  ") + IString(pCurrentIntervention->ToString().c_str()));

    m_CurrentIntervention = pCurrentIntervention;
}


CIntervention * CRemoteAPMgr::GetCurrentIntervention()
{
    return m_CurrentIntervention;
}


CApproval &CRemoteAPMgr::GetCurrentApproval()
{
    _ASSERTE(m_pCurrentApproval!=NULL);

    return *m_pCurrentApproval;
}


void CRemoteAPMgr::SetCurrentApproval(PApproval pApproval)
{
    delete m_pCurrentApproval;

    m_pCurrentApproval = pApproval;
}


void CRemoteAPMgr::SetEntryText(const _TCHAR *szNewText) { m_sEntryText = szNewText; }
bool CRemoteAPMgr::HasEntryText() const { return !m_sEntryText.empty(); }
const _TCHAR *CRemoteAPMgr::GetEntryText() const { return m_sEntryText.c_str(); }
void CRemoteAPMgr::AppendText(const _TCHAR *sText) { m_sEntryText += sText; }
void CRemoteAPMgr::ResetEntryText() { m_sEntryText.clear(); }


void CRemoteAPMgr::SetLaneName (const _TCHAR *pszLaneName)
{
    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  pszLaneName:  \"") + IString(pszLaneName) + _T("\""));

    m_sLaneName = pszLaneName;
}
const _TCHAR *CRemoteAPMgr::GetLaneName () const { return m_sLaneName.c_str(); }


HRESULT CRemoteAPMgr::PostMsg(CIntervention * pIntervention, const void * /* pData */, size_t /* nDataLength */ )
{
    CRCMStateMachine *pSm = GetStateMachineRAP();
    _ASSERTE(pSm!=NULL);

    // TODO:  Need to add code to update the data with the parameters passed in

    _ASSERTE(pIntervention!=NULL);

    HRESULT hr = pSm->PostMsg(*dynamic_cast<IMessage *>(pIntervention));

    return hr;
}


bool CRemoteAPMgr::CanApprove(LPCTSTR szApprovalName) const
{
    StringType xpathExpression = CONFIG_KEY_APPROVALS;
    xpathExpression += _T("/");
    xpathExpression += szApprovalName;
    ITRACE_DEVELOP(_T("xpathExpression:  ") + IString(xpathExpression.c_str()));

    bool bRetValue =
        CConfiguration<bool>::Get(*CRCManager::instance()->GetConfigMgr()
                                , xpathExpression.c_str()
                                , true);
    ITRACE_DEVELOP(IString(xpathExpression.c_str()) + _T(":  ")
                   + IString(bRetValue));

    return bRetValue;
}


HRESULT CRemoteAPMgr::CanApprove(LPCTSTR bstrInParms, LONG* pResult)
{
    IMODTRACE_DEVELOP();

    //API_TRACE(RCMControl);

    _ASSERTE( _CrtCheckMemory() );

    if (!m_bStateMachineReady)
        return RCM_SM_NOT_INITIALIZED;

    recorder().RecordAPI(CRCMgrAPI::BuildCanApproveRecord(bstrInParms));

    IMessage *pMsg;
    HRESULT hr = SendMsg(&pMsg, MSG_CAN_APPROVE_COMMAND, bstrInParms, _tcslen(bstrInParms)); //lint !e534

    CString text = pMsg->GetText();
    if (text == _T("TRUE"))
        *pResult = TRUE;
    else
        *pResult = FALSE;

    recorder().RecordResult(CRCMgrAPI::BuildCanApproveRecord(pResult));

    _ASSERTE( _CrtCheckMemory() );

    delete pMsg;

    return hr;
}


HRESULT CRemoteAPMgr::GetApprovalInfo(LPCTSTR bstrInParms, BSTR* bstrOutParms)
{
    IMODTRACE_DEVELOP();

    //API_TRACE(RCMControl);

    _ASSERTE( _CrtCheckMemory() );

    if (!m_bStateMachineReady)
        return RCM_SM_NOT_INITIALIZED;

    recorder().RecordAPI(CRCMgrAPI::BuildGetApprovalInfoRecord(bstrInParms));

    // Free any string passed in
    ::SysFreeString(*bstrOutParms);

    IMessage *pMsg;
    HRESULT hr = SendMsg(&pMsg, MSG_GET_APPROVAL_COMMAND, bstrInParms, _tcslen(bstrInParms)); //lint !e534
ITRACE_DEVELOP(_T("SendMsg returned:  ") + IString(hr).d2x());

    *bstrOutParms = pMsg->GetText();

    recorder().RecordResult(CRCMgrAPI::BuildGetApprovalInfoRecord(pMsg->GetText()));

    _ASSERTE( _CrtCheckMemory() );

    delete pMsg;

    return hr;
}


HRESULT CRemoteAPMgr::OnApproval(LPCTSTR bstrParms)
{
    _ASSERTE( _CrtCheckMemory() );

    //API_TRACE(VoidItem);

    IMODTRACE_DEVELOP();

    if (!m_bStateMachineReady)
        return RCM_SM_NOT_INITIALIZED;

    recorder().RecordAPI(CRCMgrAPI::BuildOnApprovalRecord(bstrParms));

    HRESULT hr = CRCManager::PostMsg(MSG_APPROVAL_COMMAND, const_cast<LPTSTR>(bstrParms), _tcslen(bstrParms)+1);
ITRACE_DEVELOP(_T("PostMsg returned:  ") + IString(hr).d2x());
    VERIFY((hr==S_OK) || (hr==S_FALSE));

    _ASSERTE( _CrtCheckMemory() );

    return S_OK;
}


const _TCHAR *CRemoteAPMgr::GetDataModelName() const
{
    return _T("CRemoteAPMgr");
}
void CRemoteAPMgr::SetForceApprovalBeforeSuspend(bool bApprovalBeforeSuspend) { m_bApprovalBeforeSuspend = bApprovalBeforeSuspend; }
bool CRemoteAPMgr::IsForceApprovalBeforeSuspend() const { return m_bApprovalBeforeSuspend; }

BEGIN_PROPERTIES_TABLE(CRemoteAPMgr)
    DEFINE_BOOL_PROPERTY(AssistModeRequested)
    DEFINE_BOOL_PROPERTY(HandheldAssistModeEnabled)
END_PROPERTIES_TABLE()


BEGIN_DATA_MODEL(CRemoteAPMgr, RemoteAPMgr)
    DATA_MODEL_BOOL_PROPERTY(CRemoteAPMgr, RemoteAPMgr, AssistModeEnabled)
    DATA_MODEL_BOOL_PROPERTY(CRemoteAPMgr, RemoteAPMgr, HandheldAssistModeEnabled)
END_DATA_MODEL()
//TR390603+
void CRemoteAPMgr::LockInterventionsList(unsigned long ulTimeout)
{
    IMODTRACE_DEVELOP();

    m_presInterventionsListLock->Lock(ulTimeout);      //lint !e534
}


void CRemoteAPMgr::UnlockInterventionsList()
{
    IMODTRACE_DEVELOP();

    try
    {
        m_presInterventionsListLock->Unlock();    //lint !e534
    }
    catch (ISystemError &se)
    {
        ITRACE_EXCEPTION(se);
        throw;
    }
    catch (...)
    {
        ITRACE_DOT_ERROR();
        throw;
    }
}
//TR390603-
