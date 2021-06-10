//	Class : 

//		DMCashCount

//	Description :

//		For the Cash Recycler, the SCOTAPP reads cash count list from devices.
//		But many Cash/Coin Acceptor and Dispenser devices such as 5933,
//		Telequip and Coin Hopper do not have ability to provide accurate cash count.
//		The SCOTAPP will keep cash count list for Cash/Coin Acceptor and Dispenser in Phase 1. 

//		DMCashCount is used to keep track of these cash counts and it will store 4 cashcount lists
//		for Cash Acceptor, Cash Dispenser, Coin Acceptor and Coin Dispenser in the registry.

//		The CashManagement screen will read counts from this class,
//		instead of from device for Glory Recycler.

#include "stdafx.h"
#include "DMCashCount.h"

#include "common.h"
#include "CMInterface.h"


#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMcc")

#define INIT_ARRAY_SIZE 0
#define ARRAY_INCREMENT 1

//Registry Keys for Cash Counts
const CString ccsRegBranch = _T("Software\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\CashCount");
const CString ccsCashAcceptorCountRegEnt = _T("CashAcceptorCountList");
const CString ccsCashDispenserCountRegEnt = _T("CashDispenserCountList");
const CString ccsCoinAcceptorCountRegEnt = _T("CoinAcceptorCountList");
const CString ccsCoinDispenserCountRegEnt = _T("CoinDispenserCountList");

DMCashCount::DMCashCount() : m_pCM(NULL), m_bUpdateCashArrays(false)	
{
}

DMCashCount::~DMCashCount()
{
}

//Split a string into two strings at the occurance of a semi-colon.
//The two split strings do not contain the semi-colon.
bool DMCashCount::SplitString(BSTR* sOriginalString, BSTR* sFirstString, BSTR* sSecondString)
{
	CString csOriginalString(*sOriginalString);
	CString csFirstString = _T("");
	CString csSecondString = _T("");
    
    bool bRetVal = false;

	int iSColonPosition = 0;

	iSColonPosition = csOriginalString.Find(_T(";"),iSColonPosition);

    if (iSColonPosition != -1)
    {
	    csFirstString = csOriginalString.Left(iSColonPosition);

	    csSecondString = csOriginalString.Right(csOriginalString.GetLength() - iSColonPosition - 1);

	    *sFirstString = csFirstString.AllocSysString();

	    *sSecondString = csSecondString.AllocSysString();
        
        bRetVal = true;
    }
    else
    {
        if (csOriginalString.GetLength() > 0)
        {
            csFirstString = csOriginalString;
            bRetVal = true;
        }
        else
            sFirstString = NULL;
    }

	return bRetVal;
}

bool DMCashCount::Initialize(CCMInterface &cm) 
{
	trace(L6,_T("DMCashCount::Initialize()"));
	
    m_pCM = &cm; //TAR 448434
    
	// Initialise Member Variables
	caCashAcceptorDenomination.SetSize(INIT_ARRAY_SIZE,ARRAY_INCREMENT);
	caCashAcceptorCounts.SetSize(INIT_ARRAY_SIZE,ARRAY_INCREMENT);

	caCoinAcceptorDenomination.SetSize(INIT_ARRAY_SIZE,ARRAY_INCREMENT);
	caCoinAcceptorCounts.SetSize(INIT_ARRAY_SIZE,ARRAY_INCREMENT);

	caCashDispenserDenomination.SetSize(INIT_ARRAY_SIZE,ARRAY_INCREMENT);
	caCashDispenserCounts.SetSize(INIT_ARRAY_SIZE,ARRAY_INCREMENT);

	caCoinDispenserDenomination.SetSize(INIT_ARRAY_SIZE,ARRAY_INCREMENT);
	caCoinDispenserCounts.SetSize(INIT_ARRAY_SIZE,ARRAY_INCREMENT);
	
	//Check whether the registry is empty
	//If the registry contains values, read in those values.
	if (CheckRegistryEntries())
	{
		UpdateCashArrays();
        //test code for rp
        /*rp.CurrencyCashManagement( caCashAcceptorDenomination, 
            caCashAcceptorCounts,
            caCashAcceptorCounts,
            caCashDispenserDenomination, 
            caCashDispenserCounts,
            caCashDispenserCounts,
            caCoinAcceptorDenomination, 
            caCoinAcceptorCounts,
            caCoinAcceptorCounts,
            caCoinDispenserDenomination, 
            caCoinDispenserCounts,
            caCoinDispenserCounts,
            _T("9999"), _T("9999") );*/
	}
	else
	{
		//If the registry does not contains values, initialize the values
		//by obtaining the denomination lists from the cash devices.
		BSTR pCashCounts = SysAllocStringByteLen(NULL,200);
		BSTR pCashDispenserCounts = SysAllocStringByteLen(NULL,200);
		BSTR pCoinDispenserCounts = SysAllocStringByteLen(NULL,200);
		BSTR pCashAcceptorCounts = SysAllocStringByteLen(NULL,200);
		BSTR pCoinAcceptorCounts = SysAllocStringByteLen(NULL,200);
		
		BSTR pDummyCounts = SysAllocStringByteLen(NULL,200);
        CString csCashCount = _T("");
		//Get the Coin and Cash Denominations from the Cash Changer.
		BOOL pDiscrepancy = true;
		int n=0;

		while(dm.CashChangerGetCashCounts( &pCashCounts, &pDiscrepancy )!= OPOS_SUCCESS && n<30)  //device is not ready to give count, wait for 30 second
//		while(dm.GetCashChangerCurrencyCashList( &pCashCounts, )!= OPOS_SUCCESS && n<30)  //device is not ready to give count, wait for 30 second
		{
			trace(L0,_T("dm.CashChangerGetCashCounts() not ready n=%d."),n);
			Sleep(1000);
			n++;
		}

		if (n==30)
		{
			trace(L0,_T("dm.CashChangerGetCashCounts() failed. "));
		}
		
		// Format and extract the denomination list string returned from the device.
        if (SplitString(&pCashCounts,&pCoinDispenserCounts,&pCashDispenserCounts) == true)
        {
		    ExtractDenominations(&pCoinDispenserCounts,&caCoinDispenserDenomination);
		    ExtractDenominations(&pCashDispenserCounts,&caCashDispenserDenomination);
        }

		//Get the Cash Denominations from the Cash Acceptor.
		//dm.CashAcceptorReadBillCounts(&pCashCounts);
        dm.GetCashAcceptorCurrencyCashList(&csCashCount);
        pCashCounts = csCashCount.AllocSysString();
        //CDeviceManagerEx::GetCashAcceptorCurrencyCashList(DEVICEID0);
		// Format and extract the denomination list string returned from the device.
		// pDummyCounts is used for coins as we only want bill count.
        if (SplitString(&pCashCounts,&pDummyCounts,&pCashAcceptorCounts) == true)
        {
		    ExtractDenomFromDenomList(&pCashAcceptorCounts,&caCashAcceptorDenomination);
        }
		//Get the Cash Denominations from the Cash Acceptor.
		//dm.CoinAcceptorReadBillCounts(&pCashCounts);
        //csCashCount = CDeviceManagerEx::GetCoinAcceptorCoinList(DEVICEID0);
		// Format and extract the denomination list string returned from the device.
		// pDummyCounts is used for cash as we only want coin count.
        dm.GetCoinAcceptorCoinList(&csCashCount);
        pCashCounts = csCashCount.AllocSysString();
        if (SplitString(&pCashCounts,&pCoinAcceptorCounts,&pDummyCounts) == true)
        {
		    ExtractDenomFromDenomList(&pCashCounts,&caCoinAcceptorDenomination);
        }
        //Adjust the size of the Cash Count arrays to match the size of the denominations
	    caCashAcceptorCounts.SetSize(caCashAcceptorDenomination.GetSize(),ARRAY_INCREMENT);
        caCoinAcceptorCounts.SetSize(caCoinAcceptorDenomination.GetSize(),ARRAY_INCREMENT);
        caCashDispenserCounts.SetSize(caCashDispenserDenomination.GetSize(),ARRAY_INCREMENT);
	    caCoinDispenserCounts.SetSize(caCoinDispenserDenomination.GetSize(),ARRAY_INCREMENT);

        //Update the Cash Counts
		PutCashCountToRegistry(&caCashAcceptorCounts, &caCoinAcceptorCounts, &caCashDispenserCounts, &caCoinDispenserCounts);
		
		SysFreeString(pCashCounts);
		SysFreeString(pCashDispenserCounts);
		SysFreeString(pCoinDispenserCounts);
		SysFreeString(pCashAcceptorCounts);
		SysFreeString(pCoinAcceptorCounts);
		SysFreeString(pDummyCounts);
	}

	return true;
}


//When the SCOTAPP dispenses changes,the APP will call CurrencyDispensed()
//to update the CashDispenser and Coin Dispenser cash counts, and store to registry. 
bool DMCashCount::CurrencyDispensed(LPCTSTR szDisp)
{
	if(m_pCM == NULL || szDisp == NULL)
	{
		return false;
	}

	m_pCM->CurrencyDispensed(szDisp);
    m_bUpdateCashArrays = true;		// Need to update the cash arrays!

	return true;
}

//When the SCOTAPP accepts changes,the APP will call CurrencyAccepted()
//to update the CashDispenser and Coin Dispenser cash counts, and store to registry. 
bool DMCashCount::CurrencyAccepted(bool bBill, CString csDen, int nQuantity)
{
	if(m_pCM == NULL || nQuantity <= 0)
	{
		return false;
	}
	for(int i=0; i<nQuantity; i++)
	{
		m_pCM->CurrencyDeposited(csDen, bBill);
	}
    m_bUpdateCashArrays = true;		// Need to update the cash arrays!
    return true;
}


//Find the first matching denomination passed in csDenom in the array caDenominations.
int DMCashCount::FindDenom(CString csDenom, CArray <CString,CString>* caDenominations)
{
	CString csCompare = (_T(""));
	
	int iArrayMax = caDenominations->GetSize() - 1;

	for (int iCount = 0; iCount <= iArrayMax; iCount++)
	{
		csCompare = caDenominations->GetAt(iCount);

		if (csCompare.CompareNoCase(csDenom) == 0)
			return iCount;
	}

	return -1;
}


//Public function to obtain the Cash Counts.
bool DMCashCount::SetCashCounts(CArray<int,int>* pCashAcceptorCount, CArray<int,int>* pCoinAcceptorCount, CArray<int,int>* pCashDispenserCount, CArray<int,int>* pCoinDispenserCount)
{
	bool bRetVal = false;
	int iSuccess = 0;
	int const iSuccessesNeeded = 1;
	
	//Initialize Cash Count Strings
	
	ClearCashArrays();

	caCashAcceptorCounts.Copy(*pCashAcceptorCount);
	caCoinAcceptorCounts.Copy(*pCoinAcceptorCount);
	caCashDispenserCounts.Copy(*pCashDispenserCount);
	caCoinDispenserCounts.Copy(*pCoinDispenserCount);

	iSuccess = PutCashCountToRegistry(&caCashAcceptorCounts, &caCoinAcceptorCounts, &caCashDispenserCounts, &caCoinDispenserCounts);

	if (iSuccess != iSuccessesNeeded)
	{
		trace(L6,_T("DMCashCount::SetCashCounts() Returned False"));
		return false;
	}
	else
	{
		trace(L6,_T("DMCashCount::SetCashCounts() Returned True"));
		return true;
	}
}

//GetCashCountFromRegistry() will extract the cash counts for Cash Acceptor,
//Cash Dispenser, Coin Acceptor and Coin Dispenser. 
bool DMCashCount::GetCashCountFromRegistry(BSTR* pCashAcceptorCashCountString, BSTR* pCoinAcceptorCashCountString, BSTR* pCashDispenserCashCountString, BSTR* pCoinDispenserCashCountString)
{
	HKEY hKey;
	long iSuccess = 0;
	long lRetVal = 0;

	int const iSuccessesNeeded = 4;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, ccsRegBranch, 
		0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    if (lRetVal == ERROR_SUCCESS)
    {
		CString csConvertData = _T("");
		_TCHAR csDataSaved[500] = {NULL};
		DWORD  dwDataSize = sizeof(csDataSaved);
		
		lRetVal = ::RegQueryValueEx(hKey, ccsCashAcceptorCountRegEnt, 0, NULL, (LPBYTE)csDataSaved, &dwDataSize);
		
		if(lRetVal == ERROR_SUCCESS)
		{
			csConvertData = csDataSaved;
			
			if (csConvertData.GetLength() > 0)
			{
				SysFreeString(*pCashAcceptorCashCountString);
				*pCashAcceptorCashCountString = csConvertData.AllocSysString();
				iSuccess ++;
			}
			//Clear data strngs for next registry read.
			csConvertData = _T("");
			lRetVal = 0;

			int i = 0;
			while (csDataSaved[i])
			{
				csDataSaved[i] = 0;
				i++;
			}
			dwDataSize = sizeof(csDataSaved);
		}

		lRetVal = ::RegQueryValueEx(hKey, ccsCoinAcceptorCountRegEnt, 0, NULL, (LPBYTE)csDataSaved, &dwDataSize);

		if( lRetVal == ERROR_SUCCESS )
		{
			csConvertData = csDataSaved;
			
			if (csConvertData.GetLength() > 0)
			{
				SysFreeString(*pCoinAcceptorCashCountString);
				*pCoinAcceptorCashCountString = csConvertData.AllocSysString();
				iSuccess ++;
			}
			
			//Clear data strngs for next registry read.
			csConvertData = _T("");
			lRetVal = 0;

			int i = 0;
			while (csDataSaved[i])
			{
				csDataSaved[i] = 0;
				i++;
			}
			dwDataSize = sizeof(csDataSaved);
		}
		
		lRetVal = ::RegQueryValueEx(hKey, ccsCashDispenserCountRegEnt, 0, NULL, (LPBYTE)csDataSaved, &dwDataSize);

		if(lRetVal == ERROR_SUCCESS)
		{
			csConvertData = csDataSaved;
			
			if (csConvertData.GetLength() > 0)
			{
				SysFreeString(*pCashDispenserCashCountString);
				*pCashDispenserCashCountString = csConvertData.AllocSysString();
				iSuccess ++;
			}
			//Clear data strngs for next registry read.
			csConvertData = _T("");
			lRetVal = 0;

			int i = 0;
			while (csDataSaved[i])
			{
				csDataSaved[i] = 0;
				i++;
			}
			dwDataSize = sizeof(csDataSaved);
		}

		lRetVal = ::RegQueryValueEx(hKey, ccsCoinDispenserCountRegEnt, 0, NULL, (LPBYTE)csDataSaved, &dwDataSize);

		if(lRetVal == ERROR_SUCCESS)
		{
			csConvertData = csDataSaved;

			if (csConvertData.GetLength() > 0)
			{
				SysFreeString(*pCoinDispenserCashCountString);
				*pCoinDispenserCashCountString = csConvertData.AllocSysString();
				iSuccess ++;
			}
			//Clear data strngs for next registry read.
			csConvertData = _T("");
			lRetVal = 0;

			int i = 0;
			while (csDataSaved[i])
			{
				csDataSaved[i] = 0;
				i++;
			}
			dwDataSize = sizeof(csDataSaved);
		}
	}

	RegCloseKey(hKey);

	if (iSuccess != iSuccessesNeeded)
	{
		trace(L6,_T("DMCashCount::GetCashCountFromRegistry() Returned False"));
		return false;
	}
	else
	{
		trace(L6,_T("DMCashCount::GetCashCountFromRegistry() Returned True"));// TAR 328319
		return true;
	}
}

//PutCashCountToRegistry will set the cash counts for Cash Acceptor,
//Cash Dispenser, Coin Acceptor and Coin Dispenser. 
bool DMCashCount::PutCashCountToRegistry(BSTR* pCashAcceptorCashCountString, BSTR* pCoinAcceptorCashCountString, BSTR* pCashDispenserCashCountString, BSTR* pCoinDispenserCashCountString)
{
	long lRC;
	HKEY hKey;
	int iSuccess = 0;
    int const iSuccessesNeeded = 4;		// TAR 328319;  Changed from 5

//	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ccsRegBranch,
//			 0, KEY_ALL_ACCESS, &hKey);
    DWORD disposition; // like a return code

    lRC = ::RegCreateKeyEx( HKEY_LOCAL_MACHINE,   // handle of main key
		                   ccsRegBranch,// addr of subkey name
		                   0,                     // reserved
		                   0,                     // no type for subkey value?
		                   REG_OPTION_NON_VOLATILE,   // won't be saved across system reboots
		                   KEY_ALL_ACCESS,        // desired security access
		                   NULL,                  // default security, don't care
		                   &hKey,                 // subkey handle is returned
		                   &disposition );        // returns either REG_CREATED_NEW_KEY or
	                                            // REG_OPENED_EXISTING_KEY, we don't care


	if (lRC == ERROR_SUCCESS)
    {

		CString csData(*pCashAcceptorCashCountString);
       
		lRC = ::RegSetValueEx(hKey,
				ccsCashAcceptorCountRegEnt, 
				0, 
				REG_SZ,
				(LPBYTE)(LPCTSTR)csData,
				csData.GetLength() * sizeof(_TCHAR));

		if(lRC == ERROR_SUCCESS)
		{
			iSuccess ++;
		}

		csData = CString(*pCoinAcceptorCashCountString);

		lRC = ::RegSetValueEx(hKey,
				ccsCoinAcceptorCountRegEnt, 
				0, 
				REG_SZ,
				(LPBYTE)(LPCTSTR)csData,
				csData.GetLength() * sizeof(_TCHAR));

		if(lRC == ERROR_SUCCESS)
		{
			iSuccess ++;
		}

		csData = CString(*pCashDispenserCashCountString);

		lRC = ::RegSetValueEx(hKey,
				ccsCashDispenserCountRegEnt, 
				0, 
				REG_SZ,
				(LPBYTE)(LPCTSTR)csData,
				csData.GetLength() * sizeof(_TCHAR));

		if(lRC == ERROR_SUCCESS)
		{
			iSuccess ++;
		}

		csData = CString(*pCoinDispenserCashCountString);

		lRC = ::RegSetValueEx(hKey,
				ccsCoinDispenserCountRegEnt, 
				0, 
				REG_SZ,
				(LPBYTE)(LPCTSTR)csData,
				csData.GetLength() * sizeof(_TCHAR));

		if(lRC == ERROR_SUCCESS)
		{
			iSuccess ++;
		}

	}

	RegCloseKey(hKey);

	if (iSuccess != iSuccessesNeeded)
	{
		trace(L6,_T("DMCashCount::PutCashCountToRegistry() Returned False"));
		return false;
	}
	else
	{
		trace(L6,_T("DMCashCount::PutCashCountToRegistry() Returned True")); // TAR 328319
		return true;
	}
}

//Private function to set the Cash Counts.
bool DMCashCount::PutCashCountToRegistry(CArray<int,int>* pCashAcceptorCount, CArray<int,int>* pCoinAcceptorCount, CArray<int,int>* pCashDispenserCount, CArray<int,int>* pCoinDispenserCount)
{
	bool bRetVal = false;
	int iSuccess = 0;
	int const iSuccessesNeeded = 5;
	
	//Initialize Cash Count Strings
	BSTR pCashAcceptorCashCountString = SysAllocStringByteLen(NULL,200);
	BSTR pCoinAcceptorCashCountString = SysAllocStringByteLen(NULL,200);
	BSTR pCashDispenserCashCountString = SysAllocStringByteLen(NULL,200);
	BSTR pCoinDispenserCashCountString = SysAllocStringByteLen(NULL,200);
	
	//Format Cash Count Strings
	bRetVal = WriteCashString(&pCashAcceptorCashCountString, &caCashAcceptorDenomination, pCashAcceptorCount);
	if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}

	bRetVal = WriteCashString(&pCoinAcceptorCashCountString, &caCoinAcceptorDenomination, pCoinAcceptorCount);
	if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}

	bRetVal = WriteCashString(&pCashDispenserCashCountString, &caCashDispenserDenomination, pCashDispenserCount);
	if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}

	bRetVal = WriteCashString(&pCoinDispenserCashCountString, &caCoinDispenserDenomination, pCoinDispenserCount);
	if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}

	//Write Cash Count Strings to the registry
	bRetVal = PutCashCountToRegistry(&pCashAcceptorCashCountString, &pCoinAcceptorCashCountString, &pCashDispenserCashCountString, &pCoinDispenserCashCountString);
    if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}

	SysFreeString(pCashAcceptorCashCountString);
	SysFreeString(pCoinAcceptorCashCountString);
	SysFreeString(pCashDispenserCashCountString);
	SysFreeString(pCoinDispenserCashCountString);

	if (iSuccess != iSuccessesNeeded)
	{
		trace(L6,_T("DMCashCount::SetCashCounts() Returned False"));
		return false;
	}
	else
	{
		trace(L6,_T("DMCashCount::SetCashCounts() Returned True"));// TAR 328319
		return true;
	}
}
//Extracts the counts from a string psInput into an array pcaOutput
bool DMCashCount::ExtractCounts(BSTR* psInput, CArray<int,int>* pcaOutput)
{
	int iCommaOccurance = 0;
	int iColonOccurance = 0;
	
	CString csInput(*psInput);

	CString csCurrencyCount(_T(""));
	
	while (iCommaOccurance < (csInput.GetLength() - 1))
	{
		iColonOccurance = csInput.Find(_T(':'),iColonOccurance);
		
		if (iColonOccurance == -1)
			iColonOccurance = 0;

		iCommaOccurance = csInput.Find(_T(','),iCommaOccurance);
		if (iCommaOccurance == -1)
			iCommaOccurance = csInput.GetLength();

		csCurrencyCount = csInput.Mid(iColonOccurance + 1, iCommaOccurance - iColonOccurance - 1);
		
		int lCurrencyCount = _ttoi(csCurrencyCount);
			
		pcaOutput->Add(lCurrencyCount);
		
		csCurrencyCount = _T("");
		lCurrencyCount = 0;

		iColonOccurance ++;
		iCommaOccurance ++;
	}

	return true;

}

//Extracts the denominations from a string psInput into an array pcaOutput
bool DMCashCount::ExtractDenominations(BSTR* psInput, CArray<CString,CString>* pcaOutput)
{

	int iCommaOccurance = 0;
	int iColonOccurance = 0;
	
	CString csInput(*psInput);
	
	while (iColonOccurance !=  -1)
	{

		CString csDenomination(_T(""));

		iColonOccurance = csInput.Find(_T(':'),iColonOccurance);
		
		if (iColonOccurance == -1)
			continue;

		if (iCommaOccurance != 0)
			iCommaOccurance = csInput.Find(_T(','),iCommaOccurance);

		if (iCommaOccurance == -1)
			iCommaOccurance = csInput.GetLength();
		
		if (iCommaOccurance == 0)
			csDenomination = csInput.Mid(iCommaOccurance, iColonOccurance - iCommaOccurance);
		else
			csDenomination = csInput.Mid(iCommaOccurance + 1, iColonOccurance - iCommaOccurance - 1);

		pcaOutput->Add(csDenomination);
		
		iColonOccurance ++;
		iCommaOccurance ++;
	}

	return true;

}
//Creates a string psOutput containing the counts and denominations from the arrays
//that are passed into the function.
bool DMCashCount::WriteCashString(BSTR* psOutputString, CArray<CString,CString>* pcaInputDenom, CArray<int,int>* pcaInputCount)
{
	bool bRetVal = true;
	
	const CString csDenomSeperator = _T(":");
	const CString csCountSeperator = _T(",");
	CString csOutput = _T("");

	int iDenomArrayMax = pcaInputDenom->GetSize() - 1;
	int iCountArrayMax = pcaInputCount->GetSize() - 1;

	for (int iCount = 0; iCount <= iDenomArrayMax; iCount++)
	{
		CString csBuildStr;
		csBuildStr += pcaInputDenom->GetAt(iCount);
		
		if (iCount > iCountArrayMax)
			csBuildStr += _T(":0");
		else
		{
			_TCHAR buffer[20] = {NULL};
			_itot(pcaInputCount->GetAt(iCount),buffer,10);
			csBuildStr += _T(":");
			csBuildStr += buffer;
		}
		if (iCount < iDenomArrayMax)
			csBuildStr += _T(",");
		
		csOutput += csBuildStr;
	}

	SysFreeString(*psOutputString);
	*psOutputString = csOutput.AllocSysString();
	
	return bRetVal;
}

//Checks whether the registry values for Cash Acceptor,
//Cash Dispenser, Coin Acceptor and Coin Dispenser and returns true if 
//each contains entry is not zero length.
bool DMCashCount::CheckRegistryEntries()
{
	HKEY hKey;
	int iSuccess = 0;
	int const iSuccessesNeeded = 4;
	long lRetVal = 0;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, ccsRegBranch, 
		0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		CString csConvertData = _T("");
		_TCHAR csDataSaved[100] = {NULL};
		DWORD  dwDataSize = sizeof(csDataSaved);

		
		lRetVal = ::RegQueryValueEx(hKey, ccsCashAcceptorCountRegEnt, 0, NULL, (LPBYTE)csDataSaved, &dwDataSize);

		if(lRetVal == ERROR_SUCCESS)
		{
			csConvertData = csDataSaved;
			
			if (csConvertData.GetLength() > 0)
			{
				iSuccess ++;
			}

			//Clear data strngs for next registry read.
			csConvertData = _T("");
			lRetVal = 0;

			int i = 0;
			while (csDataSaved[i])
			{
				csDataSaved[i] = 0;
				i++;
			}
			dwDataSize = sizeof(csDataSaved);
		}
		
		lRetVal = ::RegQueryValueEx(hKey, ccsCoinAcceptorCountRegEnt, 0, NULL, (LPBYTE)csDataSaved, &dwDataSize);
		if(lRetVal == ERROR_SUCCESS)
		{
			csConvertData = csDataSaved;

			if (csConvertData.GetLength() > 0)
			{
				iSuccess ++;
			}
			//Clear data strngs for next registry read.
			csConvertData = _T("");
			lRetVal = 0;

			int i = 0;
			while (csDataSaved[i])
			{
				csDataSaved[i] = 0;
				i++;
			}
			dwDataSize = sizeof(csDataSaved);
		}

		lRetVal = ::RegQueryValueEx(hKey, ccsCashDispenserCountRegEnt, 0, NULL, (LPBYTE)csDataSaved, &dwDataSize);

		if(lRetVal == ERROR_SUCCESS)
		{
			csConvertData = csDataSaved;
			
			if (csConvertData.GetLength() > 0)
			{
				iSuccess ++;
			}
			//Clear data strngs for next registry read.
			csConvertData = _T("");
			lRetVal = 0;

			int i = 0;
			while (csDataSaved[i])
			{
				csDataSaved[i] = 0;
				i++;
			}
			dwDataSize = sizeof(csDataSaved);
		}

		lRetVal = ::RegQueryValueEx(hKey, ccsCoinDispenserCountRegEnt, 0, NULL, (LPBYTE)csDataSaved, &dwDataSize);

		if(lRetVal == ERROR_SUCCESS)
		{
			csConvertData = csDataSaved;

			if (csConvertData.GetLength() > 0)
			{
				iSuccess ++;
			}
			//Clear data strngs for next registry read.
			csConvertData = _T("");
			lRetVal = 0;

			int i = 0;
			while (csDataSaved[i])
			{
				csDataSaved[i] = 0;
				i++;
			}
			dwDataSize = sizeof(csDataSaved);
		}
	}

	RegCloseKey(hKey);

	if (iSuccess != iSuccessesNeeded)
	{
		trace(L6,_T("DMCashCount::CheckRegistryEntries() Returned False"));
		return false;
	}
	else
	{
		trace(L6,_T("DMCashCount::CheckRegistryEntries() Returned True"));
		return true;
	}
}

//Clear out the member arrays
void DMCashCount::ClearCashArrays()
{
	caCashAcceptorCounts.RemoveAll();
	caCoinAcceptorCounts.RemoveAll();
	caCashDispenserCounts.RemoveAll();
	caCoinDispenserCounts.RemoveAll();
}

void DMCashCount::ClearDenominationArrays()
{
	caCashAcceptorDenomination.RemoveAll();
	caCoinAcceptorDenomination.RemoveAll();
	caCashDispenserDenomination.RemoveAll();
	caCoinDispenserDenomination.RemoveAll();
}


bool DMCashCount::UpdateCashArrays()
{
	bool bRetVal = false;
	int iSuccess = 0;
	int const iSuccessesNeeded = 9;
	CString csCoinDispenserCounts, csCashDispenserCounts, csCoinAcceptorCounts, 
			csCashAcceptorCounts, csDispenserCounts, csAcceptorCounts;
	int scolon;

	BSTR pCashAcceptorCashCountString;
	BSTR pCoinAcceptorCashCountString;
	BSTR pCashDispenserCashCountString;
	BSTR pCoinDispenserCashCountString;

	//Clear the current stored values
	ClearCashArrays();
	ClearDenominationArrays();

    //+TAR 448434
    //instead of getting the cashcount in Scot’s registry, redirect 
    //the call to XM to get the correct cashcount
    if (m_pCM)
    {
        bRetVal = m_pCM->GetCMCashCounts(csDispenserCounts, csAcceptorCounts);
        trace(L6,_T("AcceptorCounts : [%s], DispenserCounts : [%s]"), 
                    csAcceptorCounts, csDispenserCounts);
    }

    if(bRetVal == true)
    {
        //we need to separate the coin from cash counts
        //then we need to convert CString to BSTR.
        //get the coin acceptor counts
        scolon = csAcceptorCounts.Find(_T(';'));
        csCoinAcceptorCounts = csAcceptorCounts.Mid(0, scolon);
        pCoinAcceptorCashCountString = csCoinAcceptorCounts.AllocSysString();

        //get the cash acceptor counts
        csCashAcceptorCounts = csAcceptorCounts.Mid(scolon+1, csAcceptorCounts.GetLength());
        pCashAcceptorCashCountString = csCashAcceptorCounts.AllocSysString();

        //get the coin acceptor counts
        scolon = csDispenserCounts.Find(_T(';'));
        csCoinDispenserCounts = csDispenserCounts.Mid(0, scolon);
        pCoinDispenserCashCountString = csCoinDispenserCounts.AllocSysString();

        //get the cash acceptor counts
        csCashDispenserCounts = csDispenserCounts.Mid(scolon+1, csDispenserCounts.GetLength());
        pCashDispenserCashCountString = csCashDispenserCounts.AllocSysString();

        trace(L6,_T("DMCashCount::UpdateCashArrays() Get CashCounts successful"));
        iSuccess++;
        bRetVal = false;
    }
    else
    {
        trace(L6,_T("DMCashCount::UpdateCashArrays() Failed to read CashCounts"));
        return false;
    }
    //-TAR 448434
	
    //Extract the counts to the appropriate CArray.
    bRetVal = ExtractCounts(&pCashAcceptorCashCountString,&caCashAcceptorCounts);
    if (bRetVal == true)
    {
        iSuccess++;
        bRetVal = false;
    }

	bRetVal = ExtractCounts(&pCoinAcceptorCashCountString,&caCoinAcceptorCounts);
	if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}

	bRetVal = ExtractCounts(&pCashDispenserCashCountString,&caCashDispenserCounts);
	if (bRetVal == true)
	{
        iSuccess++;
		bRetVal = false;
	}
	
	bRetVal = ExtractCounts(&pCoinDispenserCashCountString,&caCoinDispenserCounts);
	if (bRetVal == true)
	{
        iSuccess++;
		bRetVal = false;
	}

	//Extract the denominations to the appropriate CArray.
	bRetVal = ExtractDenominations(&pCashAcceptorCashCountString,&caCashAcceptorDenomination);
	if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}

	bRetVal = ExtractDenominations(&pCoinAcceptorCashCountString,&caCoinAcceptorDenomination);
	if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}
	bRetVal = ExtractDenominations(&pCashDispenserCashCountString,&caCashDispenserDenomination);

	if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}
	bRetVal = ExtractDenominations(&pCoinDispenserCashCountString,&caCoinDispenserDenomination);

	if (bRetVal == true)
	{
		iSuccess++;
		bRetVal = false;
	}

	SysFreeString(pCashAcceptorCashCountString);
	SysFreeString(pCoinAcceptorCashCountString);
	SysFreeString(pCashDispenserCashCountString);
	SysFreeString(pCoinDispenserCashCountString);

	if (iSuccess != iSuccessesNeeded)
	{
		trace(L6,_T("DMCashCount::UpdateCashArrays() Unsuccessful"));
		return false;
	}
	else
	{
		trace(L6,_T("DMCashCount::UpdateCashArrays() Successful"));
        m_bUpdateCashArrays = false;		// Updated!
		return true;
	}
}



//Public function to obtain the Cash Counts.
bool DMCashCount::GetCashCounts(CArray<int,int>* pCashAcceptorCount, CArray<int,int>* pCoinAcceptorCount, CArray<int,int>* pCashDispenserCount, CArray<int,int>* pCoinDispenserCount)
{
	if(m_bUpdateCashArrays)
	{
		UpdateCashArrays();
	}

	pCashAcceptorCount->Copy(caCashAcceptorCounts);
	pCoinAcceptorCount->Copy(caCoinAcceptorCounts);
	pCashDispenserCount->Copy(caCashDispenserCounts);
	pCoinDispenserCount->Copy(caCoinDispenserCounts);

	trace(L6,_T("DMCashCount::GetCashCounts"));

	return true;
}


//Public function to obtain the Cash Counts.
bool DMCashCount::GetCashDenoms(CArray<CString,CString>* pCashAcceptorDenomination, CArray<CString,CString>* pCoinAcceptorDenomination, CArray<CString,CString>* pCashDispenserDenomination, CArray<CString,CString>* pCoinDispenserDenomination)
{
    if(m_bUpdateCashArrays)
	{
		UpdateCashArrays();
	}

	pCashAcceptorDenomination->Copy(caCashAcceptorDenomination);
	pCoinAcceptorDenomination->Copy(caCoinAcceptorDenomination);
	pCashDispenserDenomination->Copy(caCashDispenserDenomination);
	pCoinDispenserDenomination->Copy(caCoinDispenserDenomination);

	trace(L6,_T("DMCashCount::GetCashDenoms"));

	return true;
}

//Extracts the denominations from a string psInput into an array pcaOutput
bool DMCashCount::ExtractDenomFromDenomList(BSTR* psInput, CArray<CString,CString>* pcaOutput)
{


	int iCommaOccurance = 0;
	int iNextCommaOccurance = 0;

	CString csInput(*psInput);
	
	while (iNextCommaOccurance <= csInput.GetLength())
	{

		CString csDenomination(_T(""));
        
        iNextCommaOccurance = csInput.Find(_T(','),iNextCommaOccurance);

		if (iNextCommaOccurance == -1)
			iNextCommaOccurance = csInput.GetLength();
		
		csDenomination = csInput.Mid(iCommaOccurance, iNextCommaOccurance - iCommaOccurance);

		pcaOutput->Add(csDenomination);
		
		iNextCommaOccurance ++;
		iCommaOccurance = iNextCommaOccurance;
	}

	return true;

}
