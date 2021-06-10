#include "stdafx.h"
#include "GloryAdapter.h"
#include "Common.h"
#include "DMX/CashRecyclerWrapper.h"
#include "assert.h"

#define COMP_ID ID_XM
#define T_ID _T("GloryAdapter")

#include "MethodTrace.h"

#define PROCESS_EXCEPTIONS(a)                                                               \
catch(COleDispatchException * pErr)                                                         \
{                                                                                           \
    trace(L1, _T("ERROR: COleDispatchException"));    \
    ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_GLORYADAPTER_OLEDISPATCH_EXCEPTION,\
              _T("%s|%s|%x"), msg, pErr->m_strDescription, pErr->m_scError);                \
                                                                                            \
    pErr->Delete();                                                                         \
    return a;                                                                               \
}                                                                                           \
catch(...)                                                                                  \
{                                                                                           \
    long le = GetLastError();                                                               \
    ScotError(RETURN, SM_CAT_INTERNAL_INTERFACE, SCOTAPP_GLORYADAPTER_EXCEPTION,            \
                  _T("%s|%d"), msg, le);                                                    \
    return a;                                                                               \
}                                                                                           \

 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGloryAdapter::CGloryAdapter(CDeviceManagerEx &devmgr, 
							 CCashRecyclerWrapper *pRecycler, IDMWrapper *pWrapper ) : 
        m_pDevMgr(&devmgr), m_pRecycler(pRecycler), m_pWrapper(pWrapper)
{
    TRACE_METHOD(_T("CGloryAdapter::CGloryAdapter"));

    assert(m_pRecycler != NULL && m_pDevMgr != NULL && m_pWrapper != NULL ); 

}

CGloryAdapter::~CGloryAdapter()
{
    TRACE_METHOD(_T("CGloryAdapter::~CGloryAdapter"));

    try
    {
        UnInitialize();
    }
    catch(...)
    {
    }
}

bool CGloryAdapter::Initialize(void)
{
    TRACE_METHOD(_T("CGloryAdapter::Initialize"));

    return true;
}


void CGloryAdapter::UnInitialize(void)
{
    TRACE_METHOD(_T("CGloryAdapter::UnInitialize"));

    delete m_pWrapper;
    m_pWrapper = NULL;
}


bool CGloryAdapter::GetDeviceDescr(CString &result, long devClass, long devID )
{
    TRACE_METHOD(_T("CGloryAdapter::GetDeviceDescr"));
    _TCHAR msg[] = T_ID _T("::GetDeviceDescr");

    result = dm.ReadCashRecyclerDeviceModel();

    return true;
}

bool CGloryAdapter::ReadCashCounts(CString &result, bool &bDiscrepancy, bool bUseCached)    
{
    TRACE_METHOD(_T("CGloryAdapter::ReadCashCounts"));
    _TCHAR msg[] = T_ID _T("::ReadCashCounts");

    if(bUseCached && ! m_csLastCashCount.IsEmpty())
    {
        trace(L4, _T(">> Returning cached cash count of [%s] <<"), (LPCTSTR)m_csLastCashCount);

        result = m_csLastCashCount;
        return true;
    }

    m_csLastCashCount.Empty();
    

    BSTR pCashCounts = ::SysAllocStringByteLen(NULL,200);
    BOOL bDiscrepancyError = FALSE; 

    try
    {
        m_pRecycler->ReadCashCounts(&pCashCounts, &bDiscrepancyError);   

        result = pCashCounts;
        bDiscrepancy = bDiscrepancyError;    
        m_csLastCashCount = result;
    }
    PROCESS_EXCEPTIONS(false);

    SysFreeString(pCashCounts);

    trace(L4, _T("ReadCashCounts: [%s]"), (LPCTSTR)result);

    return true;
}

bool CGloryAdapter::CoinAcceptorReadCoinCounts(CString &result)
{
    TRACE_METHOD(_T("CGloryAdapter::CoinAcceptorReadCoinCounts"));

    result.Empty();     // Clear the result before returning to be safe

    return false;       // Glory does not have coin acceptor.
}

// This method will call the pRecycler ReadDropBoxCounts().
bool CGloryAdapter::CashAcceptorReadBillCounts(CString &result) 
{
    TRACE_METHOD(_T("CGloryAdapter::CashAcceptorReadBillCounts"));
    _TCHAR msg[] = T_ID _T("::CashAcceptorReadBillCounts");

    BSTR pCashCounts = ::SysAllocStringByteLen(NULL,200);
    try
    {
        dm.CashAcceptorReadBillCounts(&pCashCounts);
        result = pCashCounts;
    }
    PROCESS_EXCEPTIONS(false);

    SysFreeString(pCashCounts);

    trace(L6, _T("CashAcceptorReadBillCounts : [%s]"), (LPCTSTR)result);
    return true;
}

bool CGloryAdapter::Enable(bool enable, long deviceClass, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::Enable"));
    _TCHAR msg[] = T_ID _T("::Enable");

    try
    {
         // +SSCOB-2631
        if(!enable && deviceClass == DMCLASS_CASHRECYCLER)
        {
            CString csDepositedAmount;
            dm.CashRecyclerStopDeposit(csDepositedAmount);
        }
        // -SSCOB-2631

        dm.Enable(deviceClass, devID, enable);

        // Special call for Glory to properly set device state.
        m_pRecycler->SetDeviceEnabled(enable);
    }
    PROCESS_EXCEPTIONS(false);
    return true;
}
  
bool CGloryAdapter::CashChangerDispenseByCount(const CString &dispstr, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::CashChangerDispenseByCount"));
    _TCHAR msg[] = T_ID _T("::CashChangerDispenseByCount");
    trace(L6, _T("Dispense str [%s]"), (LPCTSTR)dispstr);

    try
    {
        dm.CashChangerDispense((LPCTSTR)dispstr);
    }
    PROCESS_EXCEPTIONS(false);
    
    return true;
}

bool CGloryAdapter::GetCashChangerDispensedCashList(CString &result, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::GetCashChangerDispensedCashList"));
    _TCHAR msg[] = T_ID _T("::GetCashChangerDispensedCashList");

    try
    {
        result = dm.CashChangerGetDispensedCashList(); 
    }
    PROCESS_EXCEPTIONS(false);

    return true;
}

bool CGloryAdapter::CoinAcceptorResetCoinCounts(long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::CoinAcceptorResetCoinCounts"));

    return true;    // Glory does not have coin acceptor.
}

bool CGloryAdapter::CashAcceptorResetBillCounts(long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::CashAcceptorResetBillCounts"));

    return true;
}

bool CGloryAdapter::GetCashAcceptorCurrencyCashList(CString &result, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::GetCashAcceptorCurrencyCashList"));
    _TCHAR msg[] = T_ID _T("::GetCashAcceptorCurrencyCashList");
    try
    {
        dm.CashRecyclerGetCurrencyCashList(result);
    }
    PROCESS_EXCEPTIONS(false);

    return true;
}

bool CGloryAdapter::GetCoinAcceptorEnabled(long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::GetCoinAcceptorEnabled"));
  
    return false;     // Glory does not have coin acceptor.
}

bool CGloryAdapter::SetCashAcceptorCurrencyAcceptList(const CString &newList, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::SetCashAcceptorCurrencyAcceptList"));
    _TCHAR msg[] = T_ID _T("::SetCashAcceptorCurrencyAcceptList");
    try
    {
        dm.SetCashAcceptorCurrencyAcceptList((LPCTSTR)newList);
    }
    PROCESS_EXCEPTIONS(false);

    return true;
}

bool CGloryAdapter::GetCoinAcceptorCoinList(CString &result, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::GetCoinAcceptorCoinList"));

    result.Empty(); // Clear the result before returning 

    return true;    // Glory does not have coin acceptor.
}

bool CGloryAdapter::CashChangerSetCashCounts(LPCTSTR cashCounts, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::CashChangerSetCashCounts"));

    return true;
}

long CGloryAdapter::DirectIO(long cmnd, long *pdata, CString &sdata, 
                  long devClass, long devID)

{
    TRACE_METHOD(_T("CGloryAdapter::DirectIO"));
    _TCHAR msg[] = T_ID _T("::DirectIO");

    long lRC = 0;

    assert(pdata != NULL);

    if (pdata == NULL)
        return false;

	BSTR bsData = sdata.AllocSysString();
    try
    {
        lRC = m_pRecycler->DirectIO(cmnd,pdata,&bsData);
        sdata = (BSTR) bsData;
    }
    PROCESS_EXCEPTIONS(lRC);

	SysFreeString(bsData);

    return lRC;
}

long CGloryAdapter::GetResultCodeExtended(long devClass, long devID) 
{
    TRACE_METHOD(_T("CGloryAdapter::GetResultCodeExtended"));
    _TCHAR msg[] = T_ID _T("::GetResultCodeExtended");

    long lRC = OPOS_SUCCESS;
    try
    {
        lRC = m_pRecycler->GetResultCodeExtended();
    }
    PROCESS_EXCEPTIONS(lRC);

    return lRC;
}

long CGloryAdapter::CheckHealth(long level, long devClass, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::CheckHealth"));
    _TCHAR msg[] = T_ID _T("::CheckHealth");

    long lRC = OPOS_SUCCESS;
    try
    {
        lRC = m_pRecycler->CheckHealth(level);
    }
    PROCESS_EXCEPTIONS(lRC);

    return lRC;
}

bool CGloryAdapter::GetCashChangerLowIndicatorList(CString &result, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::GetCashChangerLowIndicatorList"));
    _TCHAR msg[] = T_ID _T("::GetCashChangerLowIndicatorList");

    try
    {
        result = m_pRecycler->GetLowIndicatorList();
    }
    PROCESS_EXCEPTIONS(false);

    return true;
}

bool CGloryAdapter::GetCashChangerCurrencyContainerMap(CString &result, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::GetCashChangerCurrencyContainerMap"));
    _TCHAR msg[] = T_ID _T("::GetCashChangerCurrencyContainerMap");

    try
    {
        result = m_pRecycler->GetCurrencyContainerMap();
    }
    PROCESS_EXCEPTIONS(false);

    return true;
}

bool CGloryAdapter::SetCMState(bool bNewValue)
{
    TRACE_METHOD(_T("CGloryAdapter::SetCMState"));

    return true;
}

bool CGloryAdapter::CashChangerSetPurgedCounts(LPCTSTR cashCounts, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::CashChangerSetPurgedCounts"));

    return true;
}

bool CGloryAdapter::CashChangerGetPurgedCounts(CString &result)
{
    TRACE_METHOD(_T("CGloryAdapter::CashChangerGetPurgedCounts"));

    result.Empty();     // Clear the result before returning

    return true;
}

void CGloryAdapter::TerminateDMWrapperThread()
{
    TRACE_METHOD(_T("CGloryAdapter::TerminateDMWrapperThread"));

    return;
}   

bool CGloryAdapter::CashChangerRemoveCoins(long* plData, const CString &csDump, long devID)
{
    TRACE_METHOD(_T("CGloryAdapter::CashChangerRemoveCoins"));

    delete plData;      // Delete before returning
    plData = NULL;

    return true;
}

long CGloryAdapter::SetCoinAcceptorEnableCoins(bool bNewValue)
{
    TRACE_METHOD(_T("CGloryAdapter::SetCoinAcceptorEnableCoins"));
    _TCHAR msg[] = T_ID _T("::SetCoinAcceptorEnableCoins");

    try
    {
        dm.SetCoinAcceptorEnableCoins(bNewValue);
    }
    PROCESS_EXCEPTIONS(false);

    return 0;
}

bool CGloryAdapter::EnableFPReader(bool bEnable)
{
    TRACE_METHOD(_T("CGloryAdapter::EnableFPReader"));

    return m_pWrapper->EnableFPReader(bEnable);
}

bool CGloryAdapter::PostDMEvent(long lDevClass, long lStatusErrorCode)
{
    TRACE_METHOD(_T("CGloryAdapter::PostDMEvent"));

    return true;
}

bool CGloryAdapter::ReadCassetteCounts(CString &result)
{
    TRACE_METHOD(_T("CGloryAdapter::ReadCassetteCounts"));
    _TCHAR msg[] = T_ID _T("::ReadCassetteCounts");

	BSTR pCashCounts = ::SysAllocStringByteLen(NULL,200);
    try
    {
        m_pRecycler->ReadCassetteCounts(&pCashCounts);
        result = pCashCounts;
    }
    PROCESS_EXCEPTIONS(false);

	SysFreeString(pCashCounts);

    return true;
}




