// CBaseMHOperator.cpp: implementation of the CBaseMHOperator class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHOperator.h"
#include "TraceSystem.h"

#define T_ID _T("CBaseMHOperator")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseMHOperator::CBaseMHOperator()
{
}

CBaseMHOperator::~CBaseMHOperator()
{
}

/*************************************************************************
* TBProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. Unknown what an operator message would be - base
*       TB will ignore it.
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHOperator::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_IGNORE;
    tbtrace( TM_ENTRY, _T("+-ProcessMessage()"));
    return rc;
}


/*************************************************************************
* SendSignOff - FL requests an operator signoff to the POS system. Not
*       used currently in base FL app.
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHOperator::SendSignOff()
{
//    tbtrace( TM_WARNING, _T("+-%SendSignOff() - Empty Base Functionality"));
    return TB_SUCCESS;
}

/*************************************************************************
* SendSignOn - FL requests an operator signon to the POS system. Not
*       used currently in base FL app.
*
* Parameters: none
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHOperator::SendSignOn()
{
//    tbtrace( TM_WARNING, _T("+-%SendSignOn() - Empty Base Functionality"));
    return TB_SUCCESS;
}

/*************************************************************************
* ValidOperator - Validate Operator - not believed to be in use by FL
*
* Parameters: 
*  LPCTSTR szOperatorID - Operator ID
*
* Returns: BOOL - TRUE if a valid cashier, FALSE otherwise
*************************************************************************/
bool CBaseMHOperator::ValidOperator(LPCTSTR szOperatorID)
{
    // Not used by FL now - just return true and exit.
    tbtrace(TM_WARNING, _T("ValidOperator() - Empty base logic returning true!"));
    return true;
}

/*************************************************************************
* ValidateOperator - If this is a valid Operator and PW then return TRUE.
*       Base TB does not have an implementation for this - solution specific.
*
* Parameters: 
*  LPCTSTR szOperatorID - Operator ID
*  LPCSTR szPassword - Operator password
*
* Returns: BOOL - TRUE if a valid cashier and PW, FALSE otherwise
*************************************************************************/
bool CBaseMHOperator::ValidateOperator(LPCTSTR szOperatorID, LPCTSTR szPassword)
{
    tbtrace(TM_ENTRY, _T("+-ValidateOperator(%s, %s)"), szOperatorID, szPassword);
    return true;
}


/*************************************************************************
* ValidPassword - Validate Password - not believed to be in use by FL
*
* Parameters: 
*  LPCTSTR szPassword - Operator PW
*
* Returns: BOOL - TRUE if a valid password, FALSE otherwise
*************************************************************************/
bool CBaseMHOperator::ValidPassword(LPCTSTR szPassword)
{
    // Not used by FL now - just return true and exit.
    tbtrace(TM_WARNING, _T("ValidPassword() - Empty base logic returning true!"));
    return true;
}
