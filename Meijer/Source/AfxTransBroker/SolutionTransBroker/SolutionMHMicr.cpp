// SolutionMHMicr.cpp: implementation of the CSolutionMHMicr class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionMHMicr.h"
#include "TraceSystem.h"
#include "CustomerMHOptions.h"
#include "MicrMsg.h"
#include "CustomerTBScotMsg.h"
#include "CustomerTBConnectPOS.h"

#define T_ID _T("CSolutionMHMicr")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSolutionMHMicr::CSolutionMHMicr()
{
}

CSolutionMHMicr::~CSolutionMHMicr()
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
TBRC CSolutionMHMicr::EnterMICR( LPCTSTR szRawData, LPCTSTR szTransitNumber, LPCTSTR szAccountNumber, 
                            LPCTSTR szBankNumber, LPCTSTR szSerialNumber, LPCTSTR szEPC, 
                            long lType, long lCountryCode)
{
    tbtraceIO(_T("EnterMICR"));

	if ( (NULL == szRawData) || (0 == _tcslen(szRawData)) )
	{
		tbtrace( TM_INFO, _T("Bad Micr!!  Send it to the POS. "));
		CMicrMsg badMICR;
		badMICR.AccountNumber(_T(""));
		badMICR.BankNumber(_T(""));
		badMICR.Data(_T(""));
		badMICR.CheckType( CMicrMsg::CheckTypes::CHECK_PERSONAL );
		badMICR.CountryCode( CMicrMsg::CountryCodes::CC_US );
		badMICR.SerialNumber( _T("") );
		CCustomerTBConnectPOS::instance()->SendMsgToPOS( badMICR );
		// ACS POS requires an enter key to initiate processing of a MICR

		CCustomerTBConnectPOS::instance()->SendSequence( CString(_T("{ENTER}")) );
	}
	else
	{
        
        long lMICRWaitTime;
        CString csOptValue = CCustomerMHOptions::instance()->OptionValue("MICRWaitTime");

        lMICRWaitTime = _ttol(csOptValue);
        if(lMICRWaitTime == 0)
        {
            lMICRWaitTime = 200;
        }

		// OPOS MICR only requires the raw data.
		CMicrMsg theMsg;
		theMsg.AccountNumber( szAccountNumber );
		theMsg.BankNumber( szBankNumber );
		theMsg.CheckType( (CMicrMsg::CheckTypes) lType );
		theMsg.CountryCode( (CMicrMsg::CountryCodes) lCountryCode );
		theMsg.Data( szRawData );
		theMsg.SerialNumber( szSerialNumber );

        // ACS POS requires an enter key to initiate processing of a MICR
		CCustomerTBConnectPOS::instance()->SendSequence( CString(_T("{ENTER}")) );

        Sleep(lMICRWaitTime);
		// No place to put szEPC ???
		CCustomerTBConnectPOS::instance()->SendMsgToPOS( theMsg );

	}

    return TB_SUCCESS;
}


/*************************************************************************
* EnterMICRWithType - This function is called by the FastLane App to report 
*       information retrieved from the MICR document. If MICR data could 
*       not be retrieved, the parameter szRawData and other string parameters 
*       can be an empty string and the value of the long parameters is zero.
*
* Parameters: 
*  TBTENDERTYPE nTenderType - Check tender type
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
TBRC CSolutionMHMicr::EnterMICRWithType( TBTENDERTYPE nTenderType, LPCTSTR szRawData, LPCTSTR szTransitNumber, 
                                LPCTSTR szAccountNumber, LPCTSTR szBankNumber, LPCTSTR szSerialNumber, 
                                LPCTSTR szEPC, long lType, long lCountryCode)
{
    tbtraceIO(_T("EnterMICRWithType"));

	if ( (NULL == szRawData) || (0 == _tcslen(szRawData)) )
	{
		tbtrace( TM_INFO, _T("Bad Micr!!  Send it to the POS. "));
		CMicrMsg badMICR;
		badMICR.AccountNumber(_T(""));
		badMICR.BankNumber(_T(""));
		badMICR.Data(_T(""));
		badMICR.CheckType( CMicrMsg::CheckTypes::CHECK_PERSONAL );
		badMICR.CountryCode( CMicrMsg::CountryCodes::CC_US );
		badMICR.SerialNumber( _T("") );
		CCustomerTBConnectPOS::instance()->SendMsgToPOS( badMICR );
		// ACS POS requires an enter key to initiate processing of a MICR

		CCustomerTBConnectPOS::instance()->SendSequence( CString(_T("{ENTER}")) );
	}
	else
	{
		// OPOS MICR only requires the raw data.
		CMicrMsg theMsg;
		theMsg.AccountNumber( szAccountNumber );
		theMsg.BankNumber( szBankNumber );
		theMsg.CheckType( (CMicrMsg::CheckTypes) lType );
		theMsg.CountryCode( (CMicrMsg::CountryCodes) lCountryCode );
		theMsg.Data( szRawData );
		theMsg.SerialNumber( szSerialNumber );
		// No place to put szEPC ???
		CCustomerTBConnectPOS::instance()->SendMsgToPOS( theMsg );

		// ACS POS requires an enter key to initiate processing of a MICR

		CCustomerTBConnectPOS::instance()->SendSequence( CString(_T("{ENTER}")) );
	}

    return TB_SUCCESS;
}
