//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMGenericDelayed.cpp
//
// TITLE: Class impliementation for update item state
//
//
// AUTHOR:    Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMSmGenericDelayed.h"             // MGV this

#include "IQueuedAction.h"

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmGenericDelayed")

IMPLEMENT_DYNCREATE(SMSmGenericDelayed, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
SMSmGenericDelayed::SMSmGenericDelayed()
{
  IMPLEMENT_TIMESTAMP
}

SMStateBase * SMSmGenericDelayed::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmGenericDelayed") );	// RFC 330050
    trace(L6,_T("SMSmGenericDelayedBase::Initialize - m_GenericDelayedInterventionStatus = %d, bEASApproveFromVoid = %d"),
        m_GenericDelayedInterventionStatus, bEASApproveFromVoid);
	
    bStoreModeApprovals = false;
    g_bFromRAP = false;
	
    CheckAndCleanStateStack();
	
	SAWLDBScannerDisable();
	
    if (m_GenericDelayedInterventionStatus == GENERIC_DELAYED_STARTING)
    {
        m_csCurrentIntervention = m_GenericDelayedInterventions.GetLast();
        //IQueuedAction *pDelayedIntervention = m_GenericDelayedInterventions.GetLast();
        
        if(m_csCurrentIntervention != NULL)
        {
            m_csLastInterventionID = m_csCurrentIntervention->GetActionID();
            ra.OnGenericDelayedApproval(m_csCurrentIntervention->GetActionID(), m_csCurrentIntervention->GetReportingID(), ps.RemoteMode());
            if (ps.RemoteMode())
            {
                rp.SetOperator(ra.getOperatorID(ra.GetRemoteConnectionName()));
            }
            else
            {
                rp.SetOperator(csOperator);
            }
            rp.SetInterventionEndInProgress(true); 
			
            SMStateBase *pReturn = m_csCurrentIntervention->HandleQueuedAction(this);
            
            //delete pDelayedIntervention;
            //pDelayedIntervention = NULL;
            m_GenericDelayedInterventionStatus = GENERIC_DELAYED_PROCESSING;
            
            return pReturn;
        }
    }
    if (m_GenericDelayedInterventionStatus == GENERIC_DELAYED_COMPLETE ||
        m_GenericDelayedInterventionStatus == GENERIC_DELAYED_COMPLETING_APPROVALS)
    {

		SetHookProperty(_T("IsGenericDelayedProcessing"),_T("0"));	//CRD 232267

        ClearCurrentGenericDelayedIntervention();
		
        SMStateBase *retState = STATE_NULL;
       /* if (!bStoreModeGoBack && !ps.RemoteMode())
        {
            rp.SetOperator(csOperator);
            retState = HandleOutstandingInterventions();
        }

        if (retState != STATE_NULL)
        {
            m_GenericDelayedInterventionStatus = GENERIC_DELAYED_COMPLETING_APPROVALS;
            return retState;
        }
        else
        {*/
            m_GenericDelayedInterventionStatus = GENERIC_DELAYED_NOT_PROCESSING;
            //Set Frame so that OnReturnToShopping does not disable random SMButton8s
            //  OnReturnToShopping will return system to ScanAndBag if items have been 
            //  sold.  An OnReturnToShopping override can be created with this statement
            //  to change the behavior.
            //  if (fHasEnteredFinishState) {setAnchorState(BES_FINISH;}
			
            ps.SetFrame(_T("AM_Processing"));
            
            retState = OnReturnToShopping();  //Note: This will return STATE_NULL when in RemoteMode
            if (!ps.RemoteMode() && !bEASApproveFromVoid)
            {
               /* if (isAnyApprovalRequired())  //retState will be 
                {
                    delete getRealStatePointer(retState);
                    RETURNSTATE(SmAuthorization)
                }
                else
                {*/
                    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode"));
                    return retState;
                //}
            }  
            else if(!bEASApproveFromVoid)
            {
                //bReturnToShoppingNow = true;
                TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode"));
                co.SetInAssistMenus(false);
                return OnReturnToShopping();
            }
        //}
    }
	
	return STATE_NULL;
    //return SMSmGenericDelayedBase::Initialize();
}

void SMSmGenericDelayed::UnInitialize(void)
{
    SMSmGenericDelayedBase::UnInitialize();
}

bool SMSmGenericDelayed::PSAssistMenuLegalForState()
{
    return SMSmGenericDelayedBase::PSAssistMenuLegalForState();
}

