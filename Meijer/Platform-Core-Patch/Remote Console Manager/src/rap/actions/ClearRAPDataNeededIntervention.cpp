// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearRAPDataNeededIntervention.cpp 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)ClearRAPDataNeededIntervention.cpp    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ClearRAPDataNeededIntervention.cpp $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 6     2/20/06 1:41p Bd123405
 * TAR 318517
 * 
 * 5     11/10/05 3:26p Tk124825
 * TAR 311902
 * 
 * 2     2/28/05 4:09p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
// 
// 4     1/13/05 1:27p Dm185016
// Removed trace.
// 
// 3     1/12/05 5:37p Dm185016
// Cleaned up trace logs.
// 
// 2     12/09/04 5:23p Dm185016
// TAR 287903.  Handler to clear rap-data-needed interventions.
// 
// 1     12/02/04 3:33p Dm185016
*/
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "ClearRAPDataNeededIntervention.h"
#include "Message.h"
#include "RCMKeyValue.h"


CClearRAPDataNeededIntervention *CClearRAPDataNeededIntervention::m_pCurrentInstance = NULL;


BEGIN_KV_HANDLER_CLASS(CClearRAPDataNeededIntervention::CRDNIParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CClearRAPDataNeededIntervention::CRDNIParms, KEY_EV_TYPE, Type)
	DEFINE_KV_HANDLER_MAP_ENTRY(CClearRAPDataNeededIntervention::CRDNIParms, KEY_COOKIE, Cookie) // TAR 311902
    DEFINE_KV_HANDLER_MAP_ENTRY(CClearRAPDataNeededIntervention::CRDNIParms, KEY_EV_VIEW, View)  // TAR 311902
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CClearRAPDataNeededIntervention::CRDNIParms, Type)
DEFINE_KV_INT(CClearRAPDataNeededIntervention::CRDNIParms, Cookie) // TAR 311902
DEFINE_KV_INT(CClearRAPDataNeededIntervention::CRDNIParms, View) // TAR 311902


void CClearRAPDataNeededIntervention::CRDNIParms::Clear()
{
    m_Type.Empty();
}


CClearRAPDataNeededIntervention::CClearRAPDataNeededIntervention()
{
    CRCMActionBase::SetName(_T("CClearRAPDataNeededIntervention"));
}


CClearRAPDataNeededIntervention::~CClearRAPDataNeededIntervention()
{
}


bool CClearRAPDataNeededIntervention::accept(CIntervention *pIntervention)
{
    crdni().m_parms.ParseKeys((const _TCHAR *)pIntervention->GetData(), pIntervention->GetTraceHandle());

    StringType sType = crdni().m_parms.GetType();

    if (sType == KEY_EV_TYPE_RAP_DATA_NEEDED)
	{ // + TAR 311902
		    CKeyValueParmList parmList;
    CKeyValue<const _TCHAR *>::Put(parmList
                        , KEY_RCEV_OPERATION
                        , KEY_RCEV_OPERATION_INTERVENTION_APPROVED);
    CKeyValue<INT>::Put(parmList
                        , KEY_RCEV_COOKIE
                        , crdni().m_parms.GetCookie());
	CKeyValue<INT>::Put(parmList
                        , KEY_RCEV_VIEW
                        , crdni().m_parms.GetView());


    rapmgr().FireEvent( new CRemoteConsoleEvent((const _TCHAR *)parmList) );
    return true; // - TAR 311902
	}

    return false;
}


HRESULT CClearRAPDataNeededIntervention::Execute(bool *pResult, IMessage &)
{
	m_pCurrentInstance = this;

    try
    {	// + TAR 311902
		if (rapmgr().IsImmediateInterventionPending()) 
		{
			CIntervention *pIntervention = rapmgr().GetNextImmediateIntervention();
			while (pIntervention != NULL)
			{
				accept(pIntervention);
				delete pIntervention;

				pIntervention = rapmgr().GetNextImmediateIntervention();
			}
		}
		if (rapmgr().IsDelayedInterventionPending())
		{
			rapmgr().RemoveDelayedInterventions(&accept);
		}
    }
    catch (...)
    { ITRACE_DOT_ERROR(); } // - TAR 311902

    *pResult = true;

    return S_OK;
}


template <>
IAction* CCreateable<CClearRAPDataNeededIntervention, IAction>::NewInstance()
{
    IAction *pAction = new CClearRAPDataNeededIntervention;

    return pAction;
}


template <>
void CCreateable<CClearRAPDataNeededIntervention, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(ClearRAPDataNeededIntervention)
DEFINE_ACTION_RELEASE_FXN(ClearRAPDataNeededIntervention)
