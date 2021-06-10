#pragma once
 // $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/algorithms/ClearInterventions.h 3     10/21/08 4:06p Sm133076 $
/*
 * @(#)ClearInterventions.h    $Revision: 3 $ $Date: 10/21/08 4:06p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/algorithms/ClearInterventions.h $
 * 
 * 3     10/21/08 4:06p Sm133076
 * Patch G Migration
 * 
 * 5     6/23/06 8:31p Lt185007
 * tar migration 300190
 * 
 * 2     3/22/05 2:49p Dm185016
 * UNICODE
 * 
 * 1     1/26/05 2:20p Dm185016
 * Moved to new Repository.
* 
* 2     1/18/05 3:45p Dm185016
* Removed trace.
* 
* 1     12/09/04 1:46p Dm185016
* TAR 287903
*/

#include "SMConstants.h"
#include "KVParmList.h"
#include "RCMKeyValue.h"
#include "RemoteAPMgr.h"

// Must be included prior to function body
#include "algorithms/AlgorithmBegin.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
};
using namespace STATE_MACHINE_NAMESPACE;


namespace fastlane
{


// Signature for functions returning T if the given exception should be accepted or F
// if not.  These are used by the ClearInterventions algorithm.
typedef bool (*PACCEPT_EXC_FXN)(IAction *pAction, const CIntervention *);

class CSecurityStateMachine;
class CSecurityManager;

const static int FLAGS_CLEAR_DELETE_EXCEPTION   = 0x00000001;
const static int FLAGS_CLEAR_REMOVE_EXCEPTION   = 0x00000002;
const static int FLAGS_CLEAR_REPORT_ONLY        = 0x00000000;


template <class T>
HRESULT ClearInterventions(T *pAction
                      , PACCEPT_EXC_FXN pAcceptExceptionFxn)
{
    CRemoteAPMgr &rapmgr = pAction->rapmgr();

    // If we have any delayed interventions, then clear those first
    if (rapmgr.IsDelayedInterventionPending())
    {
    
    // Traverse all current exceptions in the transaction.  Those that
    // meet the criteria tested for by the filter function, will be cleared.
    CInterventionList interventions 
        = rapmgr.GetDelayedInterventions();
    CInterventionList::iterator iter = interventions.begin();
    for (;iter != interventions.end(); iter++)
    {
        CIntervention *pIntervention = *iter;

        // Use the function passed in as a filter for accepting this
        // exception as one that needs to be cleared.
        if ((*pAcceptExceptionFxn)(pAction, pIntervention))
        {
            // Remove the intervention
            rapmgr.RemoveDelayedIntervention(pIntervention);

            ITRACE_DEVELOP(_T("Cleared Intervention:  ") + IString(pIntervention->GetName()));

            // Free the storage for the intervention
            delete pIntervention;
        }
    }
    }
    // If we have any immediate interventions, then clear those
    if (rapmgr.IsImmediateInterventionPending())
    {
    
        // Traverse all current exceptions in the transaction.  Those that
        // meet the criteria tested for by the filter function, will be cleared.
        CInterventionList interventions 
            = rapmgr.GetImmediateInterventions();
        CInterventionList::iterator iter = interventions.begin();
        for (;iter != interventions.end(); iter++)
        {
            CIntervention *pIntervention = *iter;

            // Use the function passed in as a filter for accepting this
            // exception as one that needs to be cleared.
            if ((*pAcceptExceptionFxn)(pAction, pIntervention))
            {
                // Remove the intervention
                rapmgr.RemoveImmediateIntervention(pIntervention);

                ITRACE_DEVELOP(_T("Cleared Intervention:  ") + IString(pIntervention->GetName()));

                // Free the storage for the intervention
                delete pIntervention;
            }
        }
    }

    return S_OK;
}


};

// The following include must be placed last in this header file
#include "algorithms/AlgorithmEnd.h"
