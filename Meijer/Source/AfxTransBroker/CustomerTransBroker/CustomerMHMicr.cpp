// CustomerMHMicr.cpp: implementation of the CCustomerMHMicr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerMHMicr.h"
#include "CustomerTBProperties.h"
#include "BaseMessages.h"

#include "TraceSystem.h"

#define T_ID _T("CCustomerMHMicr")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCustomerMHMicr::CCustomerMHMicr()
{

}

CCustomerMHMicr::~CCustomerMHMicr()
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
*  LPCTSTR szAccountNumber - The Solution’s account number
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
TBRC CCustomerMHMicr::EnterMICR( LPCTSTR szRawData, LPCTSTR szTransitNumber, LPCTSTR szAccountNumber, 
                            LPCTSTR szBankNumber, LPCTSTR szSerialNumber, LPCTSTR szEPC, 
                            long lType, long lCountryCode)
{
    tbtraceIO(_T("EnterMICR"));

	if ( (NULL == szRawData) || (0 == _tcslen(szRawData)) )
	{
		// If a MICR Device Manager error code was passed, retrieve the string
        // to display in assist mode and set it to Title1_Prop
        long lErrorMsg = lCountryCode;  // error message packed in country code var.
        if( lErrorMsg )
        {
            int iID = 0;
            switch(lCountryCode)
            {
            case 801: iID = 9171; break; //DM_MICR_ERR_LINE_ERROR
            case 802: iID = 9172; break; //DM_MICR_ERR_NOCHECK
            case 803: iID = 9173; break; //DM_MICR_ERR_NO_DATA
            case 804: iID = 9174; break; //DM_MICR_ERR_BAD_DATA
            case 805: iID = 9175; break; //DM_MICR_ERR_CHECKJAM
            case 806: iID = 9176; break; //DM_MICR_ERR_COVER_OPEN
            case 807: iID = 9177; break; //DM_MICR_ERR_BADSIZE
            case 808: iID = 9178; break; //DM_MICR_ERR_CHECKDIGIT
            };
            CString csMsg = CBaseMessages::instance()->GetStringValueFromID(iID);
            tbtrace(TM_INFO, _T("Setting the MICR error %d for AM display: %s"), iID, csMsg);
            CCustomerTBProperties::instance()->SetHookProperty(
                _T("Title2_Prop"), csMsg);
        }
        lCountryCode = 0;
    }

    return CSolutionMHMicr::EnterMICR(szRawData, szTransitNumber, szAccountNumber, 
        szBankNumber, szSerialNumber, szEPC, lType, lCountryCode);
}

