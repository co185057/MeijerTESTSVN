// CCustomerMHOperator.cpp: implementation of the CCustomerMHOperator class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionMHOperator.h"
#include "TraceSystem.h"
#include "CustomerTBConnectPOS.h"
#include "customerTrxBroker.h"

#define T_ID _T("CSolutionMHOperator")

CStringArray CSolutionMHOperator::csaOfflineOperators;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSolutionMHOperator::CSolutionMHOperator()
{
}

CSolutionMHOperator::~CSolutionMHOperator()
{
}

bool CSolutionMHOperator::ValidateSupervisor(LPCTSTR szOperatorID, LPCTSTR szPassword)
{
    tbtraceIO(_T("ValidateSupervisor"));
    bool rc = false;

    CCustomerTBConnectPOS *pConnectPOS = CCustomerTBConnectPOS::instance();
    CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();

    if(pConnectPOS->ACSValidateSupervisor(szOperatorID, szPassword))
    {
        rc = true;
    }

    return rc;
}

/*************************************************************************
* ValidateOperator - If this is a valid Operator and PW then return TRUE.
*
* Parameters: 
*  LPCTSTR szOperatorID - Operator ID
*  LPCSTR szOperatorPw - Operator password
*
* Returns: BOOL - TRUE if a valid cashier and PW, FALSE otherwise
*************************************************************************/
bool CSolutionMHOperator::ValidateOperator(LPCTSTR szOperatorID, LPCTSTR szPassword)
{
	bool rc = false;

    // This must be implemented at a customer/solution level. Base logic merely traces
    // the parameters, and returns true
    tbtraceIO(_T("ValidateOperator"));
    CCustomerTBConnectPOS *pConnectPOS = CCustomerTBConnectPOS::instance();
    CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();

    if(pConnectPOS->ACSValidateOperator(szOperatorID, szPassword))
    {
        rc = true;
    }

	// Did the validate fail?
	if (rc == false)
	{
		tbtrace( TM_INFO, _T("ValidateOperator Failed(%s;%s)."), szOperatorID, szPassword);
		// If so, are we offline?
		CString csOffline = Trx.TBGetProperty(_T("ACSRunningOffline"), true);
		if (csOffline.Find(_T("OFF-LINE")) > -1)
		{
			tbtrace( TM_INFO, _T("ACS Server Off-Line."));
			// If so, let's check the cached list of known good passwords.
			CString csCredentials = szOperatorID;
			csCredentials += _T("/");
			csCredentials += szPassword;
			// Check to see if already in list?
			rc = FindOfflineOperator(csCredentials);
			// If found it, then we will be returning true.
			if (rc == true)
			{
				tbtrace( TM_INFO, _T("Credentials Validated Against Cache."));
			}
			else
			{
				// Otherwise, still false.
				tbtrace( TM_INFO, _T("Credentials NOT Validated Against Cache."));
			}
		}
		else
		{
			tbtrace( TM_INFO, _T("ACS Server On-Line."));
		}
	}
	else
	{
		// Add to list of known good passwords for when we are offline.
		CString csCredentials = szOperatorID;
		csCredentials += _T("/");
		csCredentials += szPassword;
		// Check to see if already in list?
		if (FindOfflineOperator(csCredentials) == false)
		{
			// Add to the list
			tbtrace( TM_INFO, _T("Adding Credentials to Cache(%s;%s)."), szOperatorID, szPassword);
			csaOfflineOperators.Add(csCredentials);
		}
	}

    return rc;
}

bool CSolutionMHOperator::FindOfflineOperator(LPCTSTR csCredentials)
{
	bool rc = false;
	for(int i = 0; i < csaOfflineOperators.GetSize(); i++)
	{
		if (csaOfflineOperators[i] == csCredentials)
		{
			rc = true;
			break;
		}
	}
	return rc;
}