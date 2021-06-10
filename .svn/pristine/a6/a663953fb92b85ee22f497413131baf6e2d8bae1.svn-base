//////////////////////////////////////////////////////////////////////
//
// BaseCurrentTransaction.cpp: implementation of the CBaseCurrentTransaction class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseCurrentTransaction.h"
#include "TraceSystem.h"
#include "TBUtilities.h"
#include "CustomerCurrentTransaction.h"


#define T_ID    _T("BASECurrentTransaction")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseCurrentTransaction::CBaseCurrentTransaction()
:m_pTBInProgress(NULL),
m_CurrentInProgressState(-1),
m_csLanguageUsed(_T(""))
{
    SetCurrentTransactionNumber(0);
    SetQuantityLimit(0);
    SetTransactionLimit(999999999); // set high until needed
    SetAssistModeStatus(TB_AMSTATUSMAYEXIT);
    m_csLoyaltyCardNumInTrx = _T("");
    m_pTBInProgress= CTBInProgress::instance();
    m_pTBInProgress->Initialize();
	DirtyTrx(FALSE);
}

CBaseCurrentTransaction::~CBaseCurrentTransaction()
{
}

/*************************************************************************
* ResetTransactionDetails - reset all base transaction data 
*
* Parameters: none
*
* Returns: void
*************************************************************************/
void CBaseCurrentTransaction::ResetTransactionDetails(void)
{
    tbtrace(TM_INFO,_T("ResetTransactionDetails()"));
    LoyaltyCardNumInTrx(_T(""));
	DirtyTrx(FALSE);
}

/*************************************************************************
* GetCurrentTransactionNumber, SetCurrentTransactionNumber - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentTransaction::GetCurrentTransactionNumber(void)
{
    return m_lTransactionNumber;
}

void CBaseCurrentTransaction::SetCurrentTransactionNumber(long lVal)
{
    m_lTransactionNumber = lVal;
}

/*************************************************************************
* GetQuantityLimit, SetQuantityLimit - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentTransaction::GetQuantityLimit(void)
{
    return m_lQuantityLimit;
}

void CBaseCurrentTransaction::SetQuantityLimit(long lVal)
{
    m_lQuantityLimit = lVal;
}

/*************************************************************************
* GetTransactionLimit, SetTransactionLimit - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentTransaction::GetTransactionLimit(void)
{
    return m_lTrxLimit;
}

void CBaseCurrentTransaction::SetTransactionLimit(long lVal)
{
    m_lTrxLimit = lVal;
}

/*************************************************************************
* GetAssistModeStatus, SetAssistModeStatus - accessors for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
long CBaseCurrentTransaction::GetAssistModeStatus(void)
{
    return m_lAssistModeStatus;
}

void CBaseCurrentTransaction::SetAssistModeStatus(long lVal)
{
    m_lAssistModeStatus = lVal;
}

/*************************************************************************
* SetCurrentInProgressState - sets the current TB InProgress state. If we 
*                             are actually changing states, then the current
*                             states exit routine will be called, and the new
*                             states entry routine will be called. This also
*                             copies the old state to the previous InProgress
*                             state.
*
* Parameters:
*  long nState - TBINPROGRESS is a long, so this is just the new state
*
* Returns: void
*************************************************************************/
void CBaseCurrentTransaction::SetCurrentInProgressState(long nState)
{
    tbtrace( TM_ENTRY, _T("SetCurrentInProgressState() from [%s] to [%s]"), 
        GetCurrentInProgressStateName(m_CurrentInProgressState), 
        GetCurrentInProgressStateName(nState));
    CString strTraceLine;

    long nCurState= GetCurrentInProgressState();
    long newState =  nState;
    
    // The exit handler is called before setting the state member.
    // the entry function is called after setting the state member.
    if(m_CurrentInProgressState != newState)
    {   // Call the exit function for the current state, if any.
        StateHandlerType pFunc;
        CMapStateToEntryFunction::iterator myIterator;
        myIterator= m_StateEntryHandlerMap.find(m_CurrentInProgressState);
        if(myIterator != m_StateEntryHandlerMap.end())
        {
            pFunc= (*myIterator).second.pExitFunction;
            // If it is NULL, that means we don't want to handle exit of this state         
            if(NULL != pFunc)
            {
                TBRC rc;
                (this->*pFunc)(NULL, &rc, 0); // At this time we are not interested in 
                // either the rc or the lparam
            }
        }
        
        m_PreviousInProgressState = m_CurrentInProgressState;
        m_CurrentInProgressState = newState;
        
        // Call the entry state for the new state, if any
        myIterator= m_StateEntryHandlerMap.find(m_CurrentInProgressState);
        if(myIterator != m_StateEntryHandlerMap.end())
        {
            pFunc= (*myIterator).second.pEntryFunction;
            // If it is NULL, that means we don't want to handle entry of this state
            // however, if entry and exit both are NULL, it will not harm, though
            // it doesn't seem to serve any purpose keeping bothe the pointers NULL.
            if(NULL != pFunc)
            {
                TBRC rc;
                (this->*pFunc)(NULL, &rc, 0); // At this time we are not interested in 
                // either the rc or the lparam
            }
        }
    }
    else // same state, so no handler handler functions here.
    {   
        m_PreviousInProgressState = m_CurrentInProgressState;
        m_CurrentInProgressState = newState;
    }
}

/*************************************************************************
* GetCurrentInProgressState - finds the long or TBINPROGRESS value of a 
*                             TB state from its string name
*
* Parameters:
*  LPCTSTR szStateName - contains the string TB state name
*
* Returns: the long value of the TB state
*************************************************************************/
long CBaseCurrentTransaction::GetCurrentInProgressState(LPCTSTR szStateName)
{
    return m_pTBInProgress->GetKey(szStateName);
}

/*************************************************************************
* SetCurrentInProgressState - provides the ability to set the current TB
*                             TB state by passing the TB string name.
*
* Parameters:
*  LPCTSTR szStateName - contains the string TB state name
*
* Returns: void
*************************************************************************/
void CBaseCurrentTransaction::SetCurrentInProgressState(LPCTSTR szStateName)
{
    SetCurrentInProgressState(m_pTBInProgress->GetKey(szStateName));
}

/*************************************************************************
* GetCurrentInProgressState - return the TBINPROGRESS TB state
*
* Parameters: none
*
* Returns: the current TB state
*************************************************************************/
long CBaseCurrentTransaction::GetCurrentInProgressState(void)
{
    return m_CurrentInProgressState;
}

/*************************************************************************
* GetCurrentInProgressStateName - given the key (the InProgress state num)
*                                 this will find the string name of the state
*
* Parameters:
*  long key - TB state number
*
* Returns: LPCTSTR - the string representation of the state name
*************************************************************************/
LPCTSTR CBaseCurrentTransaction::GetCurrentInProgressStateName(long key)
{
    if (key == 0)
        key= GetCurrentInProgressState();
    return m_pTBInProgress->GetName(key);
}

/*************************************************************************
* GetPreviousInProgressState - return the previous TBINPROGRESS TB state
*
* Parameters: none
*
* Returns: the previous TB state
*************************************************************************/
long CBaseCurrentTransaction::GetPreviousInProgressState(void)
{
    return m_PreviousInProgressState;
}

/*************************************************************************
* LoyaltyCardNumInTrx - accessors for member data, gets and sets data
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
CString CBaseCurrentTransaction::LoyaltyCardNumInTrx(void)
{
    return m_csLoyaltyCardNumInTrx;
}

void CBaseCurrentTransaction::LoyaltyCardNumInTrx(CString csNewVal)
{
    m_csLoyaltyCardNumInTrx = csNewVal;
}

/*************************************************************************
* GetLoyaltyCardNumber - from TBAPI, Fastlane requesting loyalty num
*
* Parameters:
*  LPTSTR *pszLoyaltyCardNumber - will contain loyalty number
*
* Returns: TBRC
*************************************************************************/
TBRC CBaseCurrentTransaction::GetLoyaltyCardNumber(LPTSTR *pszLoyaltyCardNumber)
{
    tbtraceIO(_T("GetLoyaltyCardNumber()"));
    CopyString(*pszLoyaltyCardNumber, LoyaltyCardNumInTrx());
    return TB_SUCCESS;
}

/*************************************************************************
* GetLanguageUsed, SetLanguageUsed - accessors for the member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
CString CBaseCurrentTransaction::GetLanguageUsed(void)
{
    return m_csLanguageUsed;
}

void CBaseCurrentTransaction::SetLanguageUsed(LPCTSTR szLangCode)
{
    //szLangCode could be NULL
    if (szLangCode == NULL)
        m_csLanguageUsed.Empty();
    else
        m_csLanguageUsed = szLangCode;
}

/*************************************************************************
* DirtyTrx - accessors for the member variable. A Dirty Transaction flag is
*			kept to know when something must be done to complete an empty trx.
*			A non-dirty trx requires no additional processing when ending the
*			trx. You would not enter a tender state in clean trx's, for example.
*			At the very least, trx's become dirty when items are sold. They
*			can also become dirty when loyalty cards are scanned.
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
void CBaseCurrentTransaction::DirtyTrx(BOOL bNewVal)
{
	m_bDirtyTrx=bNewVal;
}

BOOL CBaseCurrentTransaction::DirtyTrx(void) const 
{
	return m_bDirtyTrx;
}
