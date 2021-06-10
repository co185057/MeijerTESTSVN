// BaseCurrentTotals.cpp: implementation of the CBaseCurrentTotals class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentTotals.h"
#include "TraceSystem.h"
#include "TBUtilities.h"

#define T_ID  _T("CBaseCurrentTotals")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseCurrentTotals::CBaseCurrentTotals():
m_lSubTotal(0),
m_lChangeDue(0),
m_lBalanceDue(0),
m_lAvailableBenefitAmount(0),
m_lAvailableFoodStampAmount(0)
{
}

CBaseCurrentTotals::~CBaseCurrentTotals()
{
}

/*************************************************************************
* Refresh - casts a CTotalDetailsMsg to a this*, get msg data into class
*
* Parameters: 
*  pMsg  - CTotalDetailsMsg *
*
* Returns: void
*************************************************************************/
void CBaseCurrentTotals::Refresh(CTotalDetailsMsg *pMsg)
{
    tbtraceIO(_T("Refresh()"));
    // lock if other threads can get here
    delete [] m_pExtensionBuf;	// delete old pointer
    *((CTotalDetailsMsg *) this) = *pMsg;  // USE DEFAULT COPY 
    m_pExtensionBuf= pMsg->GetAndForgetExtensionData();// take ownership of this pointer.
    // unlock
}

/*************************************************************************
* ResetTotalDetails - reset member variables assocaited with class
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseCurrentTotals::ResetTotalDetails()
{
    Reset(); // resets CTotalDetailsMsg
    
    SetSubTotal(0);
    SetChangeDue(0);
    SetBalanceDue(0);
    SetAvailableBenefitAmount(0);
}

/*************************************************************************
* GetSubTotal, SetSubTotal - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentTotals::GetSubTotal()
{
    return m_lSubTotal;
}

void CBaseCurrentTotals::SetSubTotal(long lVal)
{
    m_lSubTotal = lVal;
}

/*************************************************************************
* GetChangeDue, SetChangeDue - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentTotals::GetChangeDue()
{
    return m_lChangeDue;
}

void CBaseCurrentTotals::SetChangeDue(long lVal)
{
    m_lChangeDue = lVal;
}

/*************************************************************************
* GetBalanceDue, SetBalanceDue - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentTotals::GetBalanceDue()
{
    return m_lBalanceDue;
}

void CBaseCurrentTotals::SetBalanceDue(long lVal)
{
    m_lBalanceDue = lVal;
}

/*************************************************************************
* GetAvailableBenefitAmount, SetAvailableBenefitAmount - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentTotals::GetAvailableBenefitAmount()
{
    return m_lAvailableBenefitAmount;
}

void CBaseCurrentTotals::SetAvailableBenefitAmount(long lVal)
{
    m_lAvailableBenefitAmount = lVal;
}

