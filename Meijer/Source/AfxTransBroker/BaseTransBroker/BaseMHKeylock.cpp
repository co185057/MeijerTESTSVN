// BaseMHKeylock.cpp: implementation of the CBaseMHKeylock class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHKeylock.h"
#include "TraceSystem.h"
#include "BaseCurrentKeylock.h"
#include "BaseObjectManager.h"


#define T_ID  _T("CBaseMHKeylock")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseMHKeylock::CBaseMHKeylock()
{
}

CBaseMHKeylock::~CBaseMHKeylock()
{
}

/*************************************************************************
* GetCurrentSecurityLevel - Called by the FastLane App so that the App can 
*       determine the TB’s current security level.
*
* Parameters: 
*  long *plExpectedLevel - security level from TBKEYLOCK enum
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHKeylock::GetCurrentSecurityLevel(long *plCurrentLevel)
{
    tbtraceIO(_T("GetCurrentSecurityLevel"));
    TBRC rc = TB_SUCCESS;
    
    if (!plCurrentLevel)
    { // not valid pointer
        tbtrace( TM_WARNING, _T("GetCurrentSecurityLevel() invalid plCurrentLevel parameter\n"));
        TBASSERT( false);
        rc = TB_E_INVALIDPARAM;
    }
    else
    {
        *plCurrentLevel = CCustomerCurrentKeylock::instance()->getCurrentSecurityLevel();
        tbtrace( TM_INFO, _T("GetCurrentSecurityLevel( %ld)"), *plCurrentLevel);
    }
    return rc;
}

/*************************************************************************
* GetExpectedSecurityLevel - Called by the FastLane application so that 
*       it can learn what security level the TB currently wants to see.
*
* Parameters: 
*  long *plExpectedLevel - security level from TBKEYLOCK enum
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHKeylock::GetExpectedSecurityLevel(long *plExpectedLevel)
{
    tbtraceIO(_T("GetExpectedSecurityLevel"));
    TBRC rc = TB_SUCCESS;
    
    if (!plExpectedLevel)
    { // not valid pointer
        tbtrace( TM_WARNING, _T("GetExpectedSecurityLevel() invalid plExpectedLevel parameter"));
        TBASSERT( false);
        rc = TB_E_INVALIDPARAM;
    }
    else
    {
        *plExpectedLevel = CCustomerCurrentKeylock::instance()->getExpectedSecurityLevel();
        tbtrace( TM_INFO, _T("GetExpectedSecurityLevel( %ld)"), *plExpectedLevel);
    }
    return rc;
}

/*************************************************************************
* ProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. Dynamic cast to a KeylockMsg and call Refresh to
*       get data into the CurrentKeylock class.
*
* Parameters: 
*  CTBMsgObj *pMsgObj - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHKeylock::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_IGNORE;
    tbtraceIO(_T("ProcessMessage"));
	 CKeylockMsg *pMsg= dynamic_cast<CKeylockMsg *> (pMsgObj);
	 ASSERT(pMsg);
	 if (pMsg) {
	    CBaseCurrentKeylock* pCurr = CCustomerCurrentKeylock::instance();        
		 pCurr->Refresh(pMsg);
	 }
    return rc;
}

/*************************************************************************
* SendLockOff - Do a manager key-lock off. Not currently used in core app
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHKeylock::SendLockOff(void)
{
//    tbtrace( TM_ENTRY, _T("+-SendLockOff() - Empty Base Functionality"));
    return TB_SUCCESS;
}


/*************************************************************************
* SendLockOn - Do a manager key-lock on. Not currently used in core app
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHKeylock::SendLockOn(void)
{
//    tbtrace( TM_ENTRY, _T("+-SendLockOn() - Empty Base Functionality"));
    return TB_SUCCESS;
}

/*************************************************************************
* SendLockPosition - Allows TB to send a specific keylock position to the POS.
*       Base TB cannot implement this as it is POS-specific
*
* Parameters:
*  int nPosition - keylock position to send to POS
*
* Returns: void
*************************************************************************/
void CBaseMHKeylock::SendLockPosition(int nPosition)
{
//    tbtrace( TM_ENTRY, _T("+-SendLockPosition(%d) - Empty Base Functionality"), nPosition);
}

/*************************************************************************
* SetSecurityLevel - Called by the FastLane App when the security level 
*       of the operator changes.
*
* Parameters: 
*  long lSecurityLevel - new security level from TBKEYLOCK enum
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHKeylock::SetSecurityLevel( long lSecurityLevel)
{
    tbtrace( TM_ENTRY, _T("+SetSecurityLevel( %d)"), lSecurityLevel);
    TBRC rc = TB_SUCCESS;

    CBaseCurrentKeylock* pCurrKeylock = CCustomerCurrentKeylock::instance();
    pCurrKeylock->setCurrentSecurityLevel(lSecurityLevel);
    pCurrKeylock->setExpectedSecurityLevel(lSecurityLevel);
    
    switch (lSecurityLevel)
    {
    case TB_KEYLOCK_L: // L Position
        {
            tbtrace( TM_INFO, _T("SetSecurityLevel L Position SendLockPosition(L_POS) to the app."));
            SendLockPosition(L_POS);
            break;
        }
    case TB_KEYLOCK_N: // R/N Position
        {
            tbtrace( TM_INFO, _T("SetSecurityLevel R/N Position SendLockPosition(R_N_POS) to the app."));
            SendLockPosition(R_N_POS);
            break;
        }
    case TB_KEYLOCK_S: // S Position
        {
            tbtrace( TM_INFO, _T("SetSecurityLevel S Position SendLockPosition(S_POS) to the app."));
            SendLockPosition(S_POS);
            break;
        }
    case TB_KEYLOCK_X: // X/Ex Position
        {
            tbtrace( TM_INFO, _T("SetSecurityLevel X/Ex Position SendLockPosition(X_Ex_POS) to the app."));
            SendLockPosition(X_Ex_POS);
            break;
        }
    case TB_KEYLOCK_UNKNOWN:
    default :
        {
            tbtrace( TM_INFO, _T("SetSecurityLevel Invalid or Unknown Security Level."));
            TBASSERT( false);
            rc = TB_E_INVALIDPARAM;
            break;
        }
    }
    tbtrace( TM_ENTRY, _T("-SetSecurityLevel()"));
    return rc;
}
