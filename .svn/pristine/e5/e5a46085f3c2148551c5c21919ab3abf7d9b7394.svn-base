//	Class : 

//		DMCashCount

//	Description :

//		For the Cash Recycler, the SCOTAPP reads cash count list from devices.
//		But many Cash/Coin Acceptor and Dispenser devices such as 5933,
//		Telequip and Coin Hopper do not have ability to provide accurate cash count.
//		The SCOTAPP will keep cash count list for Cash/Coin Acceptor and Dispenser in Phase 1. 

//		DMCashCount is used to keep track of these cash counts and it will store 2 cashcount lists
//		for Acceptor and Dispenser counts in the registry.

//		The CashManagement screen will read counts from this class,
//		instead of from device for Glory Recycler.

#ifndef _DMCashCount
#define _DMCashCount

//#include <afx.h>
////
#include "DllDefine.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCMInterface;

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
    DMCashCount
{

public:

	DMCashCount();
	virtual ~DMCashCount();

	bool Initialize(CCMInterface &cm); //TAR 448434

	bool CurrencyDispensed(LPCTSTR szDisp);
	bool CurrencyAccepted(bool bBill, CString csDen, int nQuantity);
	
	bool GetCashCounts(CArray<int,int>* pCashAcceptorCount, CArray<int,int>* pCoinAcceptorCount, CArray<int,int>* pCashDispenserCount, CArray<int,int>* pCoinDispenserCount);
	bool SetCashCounts(CArray<int,int>* pCashAcceptorCount, CArray<int,int>* pCoinAcceptorCount, CArray<int,int>* pCashDispenserCount, CArray<int,int>* pCoinDispenserCount);

	bool GetCashDenoms(CArray<CString,CString>* pCashAcceptorDenomination, CArray<CString,CString>* pCoinAcceptorDenomination, CArray<CString,CString>* pCashDispenserDenomination, CArray<CString,CString>* pCoinDispenserDenomination);
	bool UpdateCashArrays();	
	
private:
    DMCashCount(const DMCashCount &rhs);        //copy constructor
    DMCashCount & operator=(const DMCashCount &rhs);    //assignment operator

	CArray <CString,CString> caCashAcceptorDenomination;
	CArray <int,int> caCashAcceptorCounts;

	CArray <CString,CString> caCoinAcceptorDenomination;
	CArray <int,int> caCoinAcceptorCounts;

	CArray <CString,CString> caCashDispenserDenomination;
	CArray <int,int> caCashDispenserCounts;

	CArray <CString,CString> caCoinDispenserDenomination;
	CArray <int,int> caCoinDispenserCounts;

	bool ExtractCounts(BSTR* psInput, CArray<int,int>* pcaOutput);
	bool ExtractDenominations(BSTR* psInput, CArray<CString,CString>* pcaOutput);
    bool ExtractDenomFromDenomList(BSTR* psInput, CArray<CString,CString>* pcaOutput);

	bool GetCashCountFromRegistry(BSTR* pCashAcceptorCashCountString, BSTR* pCoinAcceptorCashCountString, BSTR* pCashDispenserCashCountString, BSTR* pCoinDispenserCashCountString);
	bool PutCashCountToRegistry(BSTR* pCashAcceptorCashCountString, BSTR* pCoinAcceptorCashCountString, BSTR* pCashDispenserCashCountString, BSTR* pCoinDispenserCashCountString);

	bool WriteCashString(BSTR* psOutput, CArray<CString,CString>* pcaOutputDenom, CArray<int,int>* pcaOutputCount);

	void ClearCashArrays();
	void ClearDenominationArrays();
	
	CCMInterface *m_pCM; //TAR 448434
	bool m_bUpdateCashArrays;		 // Set to true if the counts in cash arrays need to be updated.
	
int FindDenom(CString csDenom, CArray<CString,CString>* pcaDenominations);

	bool CheckRegistryEntries();

	bool SplitString(BSTR* sOriginalString, BSTR* sFirstString, BSTR* sSecondString);

	bool PutCashCountToRegistry(CArray<int,int>* pCashAcceptorCount, CArray<int,int>* pCoinAcceptorCount, CArray<int,int>* pCashDispenserCount, CArray<int,int>* pCoinDispenserCount);
};

#endif _DMCashCount