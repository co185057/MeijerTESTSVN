#include "StdAfx.h"

#include "DMXAdapter.h"

#include "devmgr.h"     // CDeviceManagerEx class

// Copyright (c) NCR Corp. 2010

#define COMP_ID ID_DM
#define T_ID _T("DMXAdapter")
#include "MethodTrace.h"

//TAR 451615
#include "DMCashCount.h"
DLLEXPORT extern DMCashCount  dmcc;    // Global DM object

#define PROCESS_EXCEPTIONS(a)                                                               \
catch(COleDispatchException * pErr)                                                         \
{                                                                                           \
    trace(L1, _T("ERROR: COleDispatchException"));    \
    ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMXADAPTER_OLEDISPATCH_EXCEPTION,  \
              _T("%s|%s|%x"), msg, pErr->m_strDescription, pErr->m_scError);                \
                                                                                            \
    pErr->Delete();                                                                         \
    return a;                                                                               \
}                                                                                           \
catch(...)                                                                                  \
{                                                                                           \
    long le = GetLastError();                                                               \
    ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMXADAPTER_EXCEPTION,              \
                  _T("%s|%d"), msg, le);                                                    \
    return a;                                                                               \
}                                                                                           \


CDMXAdapter::CDMXAdapter(CDeviceManagerEx &devmgr,
                         TraceObject &to) : m_pDevMgr(&devmgr)
{
    TRACE_METHOD_AT_LEVEL(_T("CDMXAdapter::CDMXAdapter"),7);
}

bool CDMXAdapter::Initialize(void)
{
    TRACE_METHOD(_T("CDMXAdapter::Initialize"));
    
    return true;
}

void CDMXAdapter::TerminateDMWrapperThread()
{
    TRACE_METHOD_AT_LEVEL(_T("CDMXAdapter::TerminateDMWrapperThread"), 7);

    return;
}

void CDMXAdapter::UnInitialize(void)
{
    TRACE_METHOD_AT_LEVEL(_T("CDMXAdapter::UnInitialize"), 7);
}

CDMXAdapter::~CDMXAdapter()
{
    trace(L6, _T("CDMXAdapter::~CDMXAdapter"));

    try
    {
        UnInitialize();
    }
    catch(...)
    {
    }
}

bool CDMXAdapter::GetDeviceDescr(CString &result, long devClass, long devID )
{
    TRACE_METHOD_AT_LEVEL(_T("CDMXAdapter::GetDeviceDescr"), 7);
    _TCHAR msg[] = T_ID _T("::GetDeviceDescr");

    result.Empty();

    try
    {
        result = m_pDevMgr->GetDeviceDescription(devID, devClass);
    }
    PROCESS_EXCEPTIONS(false);

    return true;
}

bool CDMXAdapter::ReadCashCounts(CString &result, bool &bDiscrepancy, bool bUseCached /* = false */)
{
    TRACE_METHOD(_T("CDMXAdapter::ReadCashCounts"));

    if(bUseCached && ! m_sLastCashCount.IsEmpty())
    {
        trace(L4, _T(">> Returning cached cash count of [%s] <<"), (LPCTSTR)m_sLastCashCount);

        result = m_sLastCashCount;
        return true;
    }

    m_sLastCashCount.Empty();       // Obtain fresh value.

    BSTR pCashCounts = ::SysAllocStringByteLen(NULL,200);
    BOOL pDiscrepancy = FALSE;
    bool bRC = false;

    try
    {
        m_pDevMgr->CashChangerGetCashCounts(0, &pCashCounts, 
                                            &pDiscrepancy);

        result = pCashCounts;
        bDiscrepancy = pDiscrepancy;    // Add Glory CM Support
        m_sLastCashCount = result;

        trace(L4, _T("ReadCashCounts: [%s]"), (LPCTSTR)result);

        bRC = true;
    }
    catch(COleDispatchException *pErr)
    {
        pErr->Delete();

        trace(L1, _T("ERROR: COleDispatchException"));
        ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMXADAPTER_OLEDISPATCH_EXCEPTION, 
                  _T("%s|%s|%x"), _T("::ReadCashCounts"), pErr->m_strDescription, pErr->m_scError);   

        bRC = false;
    }
    catch(...)  
    {
        long le = GetLastError();
        ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMXADAPTER_EXCEPTION, 
                  _T("%s|%d"), _T("::ReadCashCounts"), le);
        
        bRC = false;
    }

    ::SysFreeString(pCashCounts);
    pCashCounts = NULL;                 

    return bRC;
}

bool CDMXAdapter::CashChangerDispenseByCount(const CString &dispstr, long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::CashChangerDispenseByCount"));
    bool bRC = false;

    trace(L6, _T("Dispense str [%s]"), (LPCTSTR)dispstr);

    try{
        m_pDevMgr->CashChangerDispense(devID, (LPCTSTR)dispstr);
        bRC = true;
    }
    catch(COleDispatchException *pErr)
    {
        pErr->Delete();

        trace(L1, _T("ERROR: COleDispatchException"));
        ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMXADAPTER_OLEDISPATCH_EXCEPTION, 
                  _T("%s|%s|%x"), _T("::CashChangerDispenseByCount"), pErr->m_strDescription, 
                  pErr->m_scError);   

        bRC = false;
    }
    catch(...)
    {
        long le = GetLastError();
        ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMXADAPTER_EXCEPTION, 
                  _T("%s|%d"), _T("::CashChangerDispenseByCount"), le);
        
        bRC = false;
    }

    return bRC;
}

bool CDMXAdapter::GetCashChangerDispensedCashList(CString &result, long devID) 
{
    TRACE_METHOD(_T("CDMXAdapter::GetCashChangerDispensedCashList"));
    _TCHAR msg[] = T_ID _T("::GetCashChangerDispensedCashList");

    result.Empty();
    bool bRC = false;

    try
    {
        result = m_pDevMgr->GetCashChangerDispensedCashList(devID);

        trace(L6, _T("GetCashChangerDispensedCashList: [%s]"), (LPCTSTR)result);

		// +TAR 451615
		// Call CurrencyDispensed() to update the CashDispenser 
		// and Coin Dispenser cash counts, and store to registry.
		if( ! result.IsEmpty())
		{
			dmcc.CurrencyDispensed(result);
		}
		
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::CoinAcceptorResetCoinCounts(long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::CoinAcceptorResetCoinCounts"));
    _TCHAR msg[] = T_ID _T("::CoinAcceptorResetCoinCounts");

    bool bRC = false;

    try
    {
        m_pDevMgr->CoinAcceptorResetCoinCounts(devID);
		bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::CashAcceptorResetBillCounts(long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::CashAcceptorResetBillCounts"));
    _TCHAR msg[] = T_ID _T("::CashAcceptorResetBillCounts");

    bool bRC = false;

    try
    {
        m_pDevMgr->CashAcceptorResetBillCounts(devID);
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::CoinAcceptorReadCoinCounts(CString &result)
{
    TRACE_METHOD(_T("CDMXAdapter::CoinAcceptorReadCoinCounts"));
    _TCHAR msg[] = T_ID _T("::CoinAcceptorReadCoinCounts");

    // WARNING:  Method implementation just for testing!!  Please 
    //           examine code closely for any bugs.
    
    result.Empty();
    bool bRC = false;

    try
    {
        BSTRBlob bsCounts;

        m_pDevMgr->CoinAcceptorReadCoinCounts(0, bsCounts.GetBSTRptr());
        result = (BSTR)bsCounts;
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::CashAcceptorReadBillCounts(CString &result)
{
    TRACE_METHOD(_T("CDMXAdapter::CashAcceptorReadBillCounts"));
    _TCHAR msg[] = T_ID _T("::CashAcceptorReadBillCounts");
    
    result.Empty();
    bool bRC = false;

    try
    {
        BSTRBlob bsCashCounts;

        m_pDevMgr->CashAcceptorReadBillCounts(0, bsCashCounts.GetBSTRptr());
        result = (BSTR) bsCashCounts;

        trace(L6, _T("CashAcceptorReadBillCounts : [%s]"), (LPCTSTR)result);

        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::Enable(bool enable, long deviceClass, long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::Enable"));
    _TCHAR msg[] = T_ID _T("::Enable");

    bool bRC = false;

    try
    { 
        //TAR 448440
        m_pDevMgr->Enable(devID, deviceClass, enable);
        bRC = true;
    }

    catch(COleDispatchException * pErr)                                                         
    {
        if (enable==false && deviceClass==16 && pErr->m_scError==0x6f)
        {
             trace(L1, _T("Ignore coin acceptor's busy error."));   
        }
        else
        {
           trace(L1, _T("ERROR: COleDispatchException"));    
           ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMXADAPTER_OLEDISPATCH_EXCEPTION,  
              _T("%s|%s|%x"), msg, pErr->m_strDescription, pErr->m_scError );                 
        }
                                                                                             
         pErr->Delete();                                                                          
         return bRC;                                                                                
    }                                                                                            
    catch(...)                                                                                   
    {                                                                                            
        long le = GetLastError();                                                                
        ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMXADAPTER_EXCEPTION,               
                  _T("%s|%d"), msg, le);                                                    
        return bRC;                                                                                
    }  

    return bRC;
}

bool CDMXAdapter::GetCashAcceptorCurrencyCashList(CString &result, long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::GetCashAcceptorCurrencyCashList"));
    _TCHAR msg[] = T_ID _T("::GetCashAcceptorCurrencyCashList");

    result.Empty();
    bool bRC = false;

    try
    {
        result = m_pDevMgr->GetCashAcceptorCurrencyCashList(devID);
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::GetCoinAcceptorEnabled(long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::GetCoinAcceptorEnabled"));
    _TCHAR msg[] = T_ID _T("::GetCoinAcceptorEnabled");

    bool bRC = false;

    try
    {
        bRC = m_pDevMgr->GetCoinAcceptorEnabled(devID);
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::SetCashAcceptorCurrencyAcceptList(const CString &newList, 
                                                long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::SetCashAcceptorCurrencyAcceptList"));
    _TCHAR msg[] = T_ID _T("::SetCashAcceptorCurrencyAcceptList");

    bool bRC = false;

    try
    {
        m_pDevMgr->SetCashAcceptorCurrencyAcceptList(devID, newList);
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::GetCoinAcceptorCoinList(CString &result, long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::GetCoinAcceptorCoinList"));
    _TCHAR msg[] = T_ID _T("::GetCoinAcceptorCoinList");

    result.Empty();
    bool bRC = false;

    try
    {
        result = m_pDevMgr->GetCoinAcceptorCoinList(devID);
        trace(L6, _T("GetCoinAcceptorCoinList: [%s]"), (LPCTSTR)result);

        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::CashChangerSetCashCounts(LPCTSTR cashCounts, long devID) 
{
    TRACE_METHOD(_T("CDMXAdapter::CashChangerSetCashCounts"));
    _TCHAR msg[] = T_ID _T("::CashChangerSetCashCounts");

    ASSERT(cashCounts != NULL);

    bool bRC = false;

    trace(L6, _T("CashChangerSetCashCounts:  CashCount String: [%s]"), cashCounts);

    try
    {
        m_pDevMgr->CashchangerSetCashCounts(devID, cashCounts);
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

long CDMXAdapter::DirectIO(long cmnd, long *pdata, CString &sdata, 
                         long devClass, long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::DirectIO"));
    _TCHAR msg[] = T_ID _T("::DirectIO");

    long lRC = OPOS_SUCCESS;
    BSTR bsData;
    ASSERT(pdata != NULL);

    if (pdata == NULL)
        return OPOS_E_FAILURE;
    
    try
    {
        bsData = sdata.AllocSysString();
        lRC = m_pDevMgr->DirectIO(devID, devClass, cmnd, pdata, &bsData);
        sdata = (BSTR) bsData;
    }
    PROCESS_EXCEPTIONS(lRC);
    SysFreeString(bsData);

    return lRC;
}

long CDMXAdapter::GetResultCodeExtended(long devClass, long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::GetResultCodeExtended"));
    _TCHAR msg[] = T_ID _T("::GetResultCodeExtended");

    long lRC = OPOS_SUCCESS;

    try
    {
        lRC = m_pDevMgr->GetResultCodeExtended(devClass, devID);
    }
    PROCESS_EXCEPTIONS(lRC);

    return lRC;
}

// Added CheckHealth to allow cash management do reset BNR.
long CDMXAdapter::CheckHealth(long level, long devClass, long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::CheckHealth"));
    _TCHAR msg[] = T_ID _T("::CheckHealth");

    //SR877 Handle checkhealth return code correctly.
    long lRC = OPOS_E_FAILURE;
    try
    {
        m_pDevMgr->CheckHealth(devClass, devID, level);
        lRC = OPOS_SUCCESS;
    }
    PROCESS_EXCEPTIONS(lRC);

    return lRC;
}


bool CDMXAdapter::GetCashChangerLowIndicatorList(CString &result, long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::GetCashChangerLowIndicatorList"));
    _TCHAR msg[] = T_ID _T("::GetCashChangerLowIndicatorList");

    result.Empty();
    bool bRC = false;

    try
    {
        result = m_pDevMgr->GetCashChangerLowIndicatorList(devID);
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::GetCashChangerCurrencyContainerMap(CString &result, long devID)
{
    TRACE_METHOD(_T("CDMXAdapter::GetCashChangerCurrencyContainerMap"));
    _TCHAR msg[] = T_ID _T("::GetCashChangerCurrencyContainerMap");

    result.Empty();
    bool bRC = false;

    try
    {
        result = m_pDevMgr->GetCashChangerCurrencyContainerMap(devID);
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::SetCMState(bool bNewValue)
{
    TRACE_METHOD(_T("CDMXAdapter::SetCMState"));
    _TCHAR msg[] = T_ID _T("::SetCMState");

    bool bRC = false;

    try
    {
        m_pDevMgr->SetCMState(bNewValue);
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::CashChangerSetPurgedCounts(LPCTSTR cashCounts, long devID) 
{
    TRACE_METHOD(_T("CDMXAdapter::CashChangerSetPurgedCounts"));
    _TCHAR msg[] = T_ID _T("::CashChangerSetPurgedCounts");

    bool bRC = false;

    try
    {
        m_pDevMgr->CashChangerSetPurgedCounts(devID, cashCounts);
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}


bool CDMXAdapter::CashChangerGetPurgedCounts(CString &result)
{
    TRACE_METHOD(_T("CDMXAdapter::CashChangerGetPurgedCounts"));
    _TCHAR msg[] = T_ID _T("::CashChangerGetPurgedCounts");

    result.Empty();
    bool bRC = false;
    BOOL bDiscrepancy = false;
    
    try
    {
        BSTRBlob bsPurgedCount;
        m_pDevMgr->CashChangerGetPurgedCounts(0, bsPurgedCount.GetBSTRptr(), &bDiscrepancy);
        result = (BSTR) bsPurgedCount;
        bRC = true;
    }
    PROCESS_EXCEPTIONS(bRC);

    return bRC;
}

bool CDMXAdapter::CashChangerRemoveCoins(long* plData, const CString &sData, long devID)
{
	trace(L6, _T("CDMXAdapter::CashChangerRemoveCoins"));
	_TCHAR msg[] = T_ID _T("::CashChangerRemoveCoins");

	bool bRC = false;
	BSTR bsData;
	if (plData == NULL)
		return false;
		
	try
	{
		bsData = sData.AllocSysString();
		m_pDevMgr->CashChangerReplenishCoins(devID, plData, &bsData);

		bRC = true;
	}
	PROCESS_EXCEPTIONS(bRC);
	SysFreeString(bsData);

	return bRC;
}
//TAR 448440
long CDMXAdapter::SetCoinAcceptorEnableCoins(bool bNewValue)
{
    TRACE_METHOD(_T("CDMXAdapter::SetCoinAcceptorEnableCoins"));
    _TCHAR msg[] = T_ID _T("::SetCoinAcceptorEnableCoins");

    return dm.SetCoinAcceptorEnableCoins(bNewValue);     
}

//TAR 448426
bool CDMXAdapter::EnableFPReader(bool bEnable)
{
    TRACE_METHOD(_T("CDMXAdapter::EnableFPReader"));
    _TCHAR msg[] = T_ID _T("::EnableFPReader");

    bool rc = true;

    if(bEnable)
    {
        try
        {
            trace(L6, _T("FT_GetFingerprint() with ret= %d"), 
                FT_GetFingerprint() ); 
        }
        catch(CException* e)                                         
        {
            _TCHAR szWork[255];
            memset(szWork, 0, 255);
            e->GetErrorMessage( szWork, sizeof( szWork ) );
            trace(L6,_T("Exception in FT_GetFingerprint(): %s"), szWork);
            e->Delete();
            rc = false;
        }
        catch(...)
        {
            long le = GetLastError();   
            trace(L6, _T("Exception in FT_GetFingerprint(): %d"), le);
            rc= false;
        }
    }
    else
    {
        FT_Abort();
    }

    return rc;
}

// SR752+
bool CDMXAdapter::PostDMEvent(long lDevClass, long lStatusErrorCode)
{
    TRACE_METHOD(_T("CDMXAdapter::PostDMEvent"));
    _TCHAR msg[] = T_ID _T("::PostDMEvent");

    bool bReturn = false;

    if(dm.IsDeviceConfigured(lDevClass,0))
    {
        trace(L6, 
            _T("Generating a device error event for DEVICE_CLASS %d.  Error: %d"), lDevClass, lStatusErrorCode);

        mo.PostDM(DM_ERROR, lDevClass, 0, lStatusErrorCode, NULL);
        bReturn = true;
    }

    return bReturn;
}
// SR752-

// Add Glory CM Support
bool CDMXAdapter::ReadCassetteCounts(CString &result)
{
    TRACE_METHOD(_T("CDMXAdapter::ReadCassetteCounts"));

    return true;
}

// The following were implemented to avoid lint warnings.
// Disable default constructors & assignment.
CDMXAdapter::CDMXAdapter() : m_pDevMgr(NULL)
{
    throw "ERROR";
}

CDMXAdapter::CDMXAdapter(const CDMXAdapter &rhs) :  ///< Disable copies.
    m_pDevMgr(NULL)
{
    throw "ERROR";
}

void CDMXAdapter::operator=(const CDMXAdapter &rhs) ///< Disable copies.
{
    if(this == &rhs)
        return;

    throw "ERROR";
} //lint !e1539

BSTRBlob::BSTRBlob() : m_bsData(NULL)
{
    m_bsData = ::SysAllocString(NULL);
}

BSTRBlob::~BSTRBlob()
{
    ::SysFreeString(m_bsData);
}

BSTRBlob::operator BSTR()
{
    return m_bsData;
}

BSTR * BSTRBlob::GetBSTRptr(void) 
{
    return &m_bsData;
}


