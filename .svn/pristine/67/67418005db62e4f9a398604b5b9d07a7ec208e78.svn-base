//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmGenericDelayedBase.cpp
//
// TITLE: Class implementation for update item state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMSmAssistMenu.h" 
#include "SMSmAuthorization.h"
#else
#include "FakeSMStateBase.h"
#endif //_CPPUNIT

#include "SMSmGenericDelayed.h"     
      
#include "IQueuedAction.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SmGenericDelayedBase")

IMPLEMENT_DYNCREATE(SMSmGenericDelayedBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmGenericDelayed)


DEFINE_TIMESTAMP

CString SMSmGenericDelayedBase::m_csLastInterventionID = _T("");

//////////////////////////////////////////
SMSmGenericDelayedBase::SMSmGenericDelayedBase()
: csCurContext(EMPTY_STRING)
{
	IMPLEMENT_TIMESTAMP
#ifndef _CPPUNIT
		saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
#endif //_CPPUNIT
}

////////////////////////////////////
SMStateBase *SMSmGenericDelayedBase::Initialize(void)
{
    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmGenericDelayed") );	// RFC 330050
    trace(L6,_T("SMSmGenericDelayedBase::Initialize - m_GenericDelayedInterventionStatus = %d"),
        m_GenericDelayedInterventionStatus);

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
        ClearCurrentGenericDelayedIntervention();

        SMStateBase *retState = STATE_NULL;
        if (!bStoreModeGoBack && !ps.RemoteMode())
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
        {
            m_GenericDelayedInterventionStatus = GENERIC_DELAYED_NOT_PROCESSING;
            //Set Frame so that OnReturnToShopping does not disable random SMButton8s
            //  OnReturnToShopping will return system to ScanAndBag if items have been 
            //  sold.  An OnReturnToShopping override can be created with this statement
            //  to change the behavior.
            //  if (fHasEnteredFinishState) {setAnchorState(BES_FINISH;}

            ps.SetFrame(_T("AM_Processing"));
            
            SMStateBase *retState = OnReturnToShopping();  //Note: This will return STATE_NULL when in RemoteMode
            if (!ps.RemoteMode())
            {
                if (isAnyApprovalRequired())  //retState will be 
                {
                    delete getRealStatePointer(retState);
                    RETURNSTATE(SmAuthorization)
                }
                else
                {
                    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode"));
                    return retState;
                }
            }  
            else
            {
                //bReturnToShoppingNow = true;
                TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode"));
                co.SetInAssistMenus(false);
                return OnReturnToShopping();
            }
        }
    }

	return STATE_NULL;							  
}

///////////////////////////////////
void SMSmGenericDelayedBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmGenericDelayed") );	// RFC 330050
}


