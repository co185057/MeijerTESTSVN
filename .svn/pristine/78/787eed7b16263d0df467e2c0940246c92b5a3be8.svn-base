// BaseMHMicr.cpp: implementation of the CBaseMHMicr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHMicr.h"
#include "TraceSystem.h"
#include "micrMsg.h"
#include "basecurrentMicr.h"

#include "BaseObjectManager.h"

#define T_ID  _T("CBaseMHMicr")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseMHMicr::CBaseMHMicr()
{
    m_bMICR = false;
}

CBaseMHMicr::~CBaseMHMicr()
{
    
}

/*************************************************************************
* EnterMICR - This function is called by the FastLane App to report 
*       information retrieved from the MICR document. If MICR data could 
*       not be retrieved, the parameter szRawData and other string parameters 
*       can be an empty string and the value of the long parameters is zero.
*
* Parameters: 
*  LPCTSTR szRawData - The raw MICR data from the MICR reader device
*  LPCTSTR szTransitNumber - The bank’s transit (routing) number
*  LPCTSTR szAccountNumber - The customer’s account number
*  LPCTSTR szBankNumber - The bank’s ABA bank number
*  LPCTSTR szSerialNumber - The sequence (check) number of the check
*  LPCTSTR szEPC - The Extended Processing Code (EPC) on the check
*  long lType - The type of check:
*       1  personal check
*       2  Business or Commercial check
*       99 Unknown check type
*  long lCountryCode - The country code of the check:
*       1  United States check
*       2  Canadian check (Canada)
*       3  Mexican check (Mexico)
*       99 Unknown check origination
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHMicr::EnterMICR( LPCTSTR szRawData, LPCTSTR szTransitNumber, LPCTSTR szAccountNumber, 
                            LPCTSTR szBankNumber, LPCTSTR szSerialNumber, LPCTSTR szEPC, 
                            long lType, long lCountryCode)
{
    tbtraceIO(_T("EnterMICR"));
    
    try {
        tbtrace(TM_INFO, _T("szRawData[%s], szTransitNumber[%s], szAccountNumber[%s]"),
            szRawData, szTransitNumber, szAccountNumber);
        tbtrace(TM_INFO, _T("szBankNumber[%s], szSerialNumber[%s], szEPC[%s], lType[%d], lCountryCode[%d]"),
            szBankNumber, szSerialNumber, szEPC, lType, lCountryCode);
    }
    catch(...){tbtrace(TM_ERROR, _T("Error - logging EnterMICR parameters threw exception!"));}
    
    return TB_SUCCESS;
}

/*************************************************************************
* GetMICRPrompt - This function is called by the FastLane App when it 
*       receives a TB_NEEDMICR notification from the TB. It retrieves 
*       information requested by TB about the MICR document.
*
* Parameters: 
*  LPTSTR *ppszMICRPrompt - A pointer to a string pointer that is allocated
*       by TB. A prompt to be displayed to the operator to insert the MICR document.
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHMicr::GetMICRPrompt( LPTSTR *ppszMICRPrompt)
{
    tbtrace( TM_ENTRY, _T("+CBaseMHMicr::GetMICRPrompt()"));
    TBRC rc = TB_SUCCESS;
    
    if ((!_CrtIsValidPointer( ppszMICRPrompt, sizeof( void*), TRUE)) || (*ppszMICRPrompt))
    { // not valid string pointer or string pointer not null
        tbtrace( TM_WARNING, _T("CBaseMHMicr::GetMICRPrompt() invalid ppszMICRPrompt parameter\n"));
        TBASSERT( false);
        rc = TB_E_INVALIDPARAM;
    }
    else if (!IsMICR())
    { // set pointer to string to NULL - indicates cancel MICR at RAP 
        tbtrace( TM_INFO, _T("Cancel MICR at RAP"));
        *ppszMICRPrompt = NULL;
    }
    else
    { // prompt to insert check should be displayed in AM - send null string
        tbtrace( TM_INFO, _T("sending null prompt for MICR at RAP - use POS display in AM"));
        *ppszMICRPrompt = new TCHAR[1];
        **ppszMICRPrompt = '\0';
    }
    tbtrace( TM_ENTRY, _T("-CBaseMHMicr::GetMICRPrompt()"));
    return rc;
}

/*************************************************************************
* ProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. Dynamic cast to a MicrMsg object and call 
*       Refresh() to get data into CurrentMicr class
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHMicr::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_IGNORE;
    LPCTSTR  lpThisName = _T("ProcessMessage()");
    tbtrace( TM_ENTRY, _T("+-%s"), lpThisName);
    CMicrMsg *pMsg= dynamic_cast<CMicrMsg *> (pMsgObj);
    ASSERT(pMsg);
    if (pMsg) {
        CBaseCurrentMicr* pCurr = CCustomerCurrentMicr::instance();
        pCurr->Refresh(pMsg);
    }
    return rc;
}

/*************************************************************************
* IsMICR, SetMICR - accessor functions for member variable
*
* Parameters: N/A
*
* Returns: N/A
*************************************************************************/
bool CBaseMHMicr::IsMICR( void)
{
    return m_bMICR;
}

void CBaseMHMicr::SetMICR( bool b)
{
    m_bMICR = b;
}

/*************************************************************************
* EnterMICR - This function is called by the FastLane App to report 
*       information retrieved from the MICR document. If MICR data could 
*       not be retrieved, the parameter szRawData and other string parameters 
*       can be an empty string and the value of the long parameters is zero.
*
* Parameters: 
*  TBTENDERTYPE nTenderType - The check tender type
*  LPCTSTR szRawData - The raw MICR data from the MICR reader device
*  LPCTSTR szTransitNumber - The bank’s transit (routing) number
*  LPCTSTR szAccountNumber - The customer’s account number
*  LPCTSTR szBankNumber - The bank’s ABA bank number
*  LPCTSTR szSerialNumber - The sequence (check) number of the check
*  LPCTSTR szEPC - The Extended Processing Code (EPC) on the check
*  long lType - The type of check:
*       1  personal check
*       2  Business or Commercial check
*       99 Unknown check type
*  long lCountryCode - The country code of the check:
*       1  United States check
*       2  Canadian check (Canada)
*       3  Mexican check (Mexico)
*       99 Unknown check origination
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHMicr::EnterMICRWithType( TBTENDERTYPE nTenderType, LPCTSTR szRawData, LPCTSTR szTransitNumber, 
                                    LPCTSTR szAccountNumber, LPCTSTR szBankNumber, LPCTSTR szSerialNumber, 
                                    LPCTSTR szEPC, long lType, long lCountryCode)
{
    tbtraceIO(_T("EnterMICRWithType"));
    
    try {
        tbtrace(TM_INFO, _T("nTenderType[%d], szRawData[%s], szTransitNumber[%s], szAccountNumber[%s]"),
            nTenderType, szRawData, szTransitNumber, szAccountNumber);
        tbtrace(TM_INFO, _T("szBankNumber[%s], szSerialNumber[%s], szEPC[%s], lType[%d], lCountryCode[%d]"),
            szBankNumber, szSerialNumber, szEPC, lType, lCountryCode);
    }
    catch(...){tbtrace(TM_ERROR, _T("Error - logging EnterMICRWithType parameters threw exception!"));}
    
    return TB_SUCCESS;
}

