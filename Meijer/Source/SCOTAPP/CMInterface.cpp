//************************************************************************
// 
// Filename:  CMInterface.cpp
// 
// Purpose:   Wrapper class to access / instantiate external mode cash 
//            management DLL (XMode DLL)
// 
// Author:    NCR Corporation
//
// Copyright: Copyright (c) NCR Corp. 2010-2011
//
// Remarks:   N/A
// 
// Revision History
// 
// 1. 2011-07-08   NCR CDC - ISG   Added DLLIMPORT/DLLEXPORT to enable SSF
//      Customization and set public methods as virtual for forced
//      override by derived classes (SSCOP5.0_001 SSCOP-92/SSCOP-120)
// 2. <YYYY-MM-DD> <Author>        <Description>
// 
//************************************************************************ 

#include "StdAfx.h"

#include <assert.h>

#ifndef _CPPUNIT
#include "CMInterface.h"
#include "IDMWrapper.h"

#include "CommonServices.h"             // Need MsgObject
extern MsgObject mo;

#include "ScotMsg.h"                    // Need MessageElement.
#include "SMStateConstants.h"

#define COMP_ID ID_XM
#define T_ID _T("CMInter")
#include "MethodTrace.h"

// +-SCOADK-5902
bool CCMInterface::bTimerKilled = false;

/**
 * Function pointers to access exported XMode DLL functions.
 * Used by CCMInterface class.
 */
typedef bool (*XMINIT)(IDMWrapper &idm, IXMPrinter &printer, CPSX &psx, HWND hwnd);
typedef void (*XMUNINIT)(void);
typedef void (*XMCURRENCYDEPOSIT)(LPCSTR val, bool note=false);    
typedef void (*XMCURRENCYDISPENSE)(LPCSTR dstr);
typedef bool (*XMPROCESSCASHCOUNTS)(BSTR FAR *pCashCounts); //Report F53
typedef bool (*XMGETCASHCOUNTS)(CString &csDispenserCounts, CString &csAcceptorCounts); //TAR 448434
typedef long (*XMGETDISPENSERCAPACITY)(long denom); //TAR 450643

// TAR 449776:  Add new device event handler.
typedef void (*XMDEVICEEVENTEX)(MessageElement &rMsg, bool &bProcessed);

typedef bool (*XMHANDLEEVENT)(const MessageElement  * const pMsg);
typedef bool (*XMHANDLETBEVENT)(MessageElement *pMsg, 
                                bool &bProcessed); //TAR 448431
typedef bool (*XMHANDLEPSXEVENT)(LPCTSTR szControlName, 
                                 LPCTSTR szContextName, 
                                 LPCTSTR szEventName, 
                                 long buttonID);
typedef void (*XMENTERSTATE)(void);
typedef void (*XMEXITSTATE)(void);
typedef void (*XMGOTOBASESTATE)(void);
typedef void (*XMBALANCEUPDATE)(void);
typedef void (*XMEXITTOAPP)(void);
typedef void (*XMCANCELAUTHREQUEST)(void);

typedef void (*XMSETINTRX)(bool bInTrx);
typedef void (*XMSETCREDENTIALS)(LPCTSTR szId, LPCTSTR szPw);
typedef bool (*XMHANDLEFPEVENT)(PSEVT pEvent, long lParam, long wParam, bool bFPConnected);    //TAR 448426
// SR752+
typedef TBSTATE (*TBPROCESSMESSAGEHOOK)(PSMessageElement psMessage);    
typedef bool (*XMWAITFORTBRESPONSE)(void);  
// SR752-
typedef void (*XMSETAUTHENTICATIONREQUESTSUPPORTED)(void);
//+-SSCOADK-5902
typedef void (*XMDEPOSITINGFINISHED)(void);

// List of exported CM functions that we want to call.
LPCSTR g_szCMFuncs[] =
{
    "XMCurrencyDeposited",
    "XMCurrencyDispensed",
    "XMDeviceEventEx",     // TAR 449776
    "XMProcessCashCounts",
    "XMGetCashCounts",
    "XMHandleEvent",
    "XMHandleTBEvent",
    "XMEnterState",
    "XMExitState",
    "XMGoToBaseState",
    "XMBalanceUpdate",
    "SetCredentials",
    "SetInTransaction",
    "XMHandlePSXEvent",
	"XMGetDispenserCapacity",
	"XMHandleFPEvent",     //TAR 448426
	"TBProcessMessageHook",     // SR752
    "XMWaitForTBResponse",      // SR752
    "XMExitToApp",
    "OnAuthenticationRequestCancelled",
    "SetAuthenticationRequestSupported",
    "XMDepositingFinished", //+-SSCOADK-5902
    NULL                   // Last entry should be NULL to signal end of list.
};

CCMInterface *CCMInterface::m_pInstance = NULL;
bool CCMInterface::m_bAllNotesTransferred = false;
CString CCMInterface::m_csPreviousControl1 = _T("");
CString CCMInterface::m_csPreviousControl2 = _T("");


CCMInterface::CCMInterface() :
    m_bInXM(false), 
    m_hdll(NULL),
    m_bXMOpenAttempted(false), 
    m_fpMap(NULL),
	m_bInTrx(false)
{
    assert(m_pInstance == NULL);

    // Make ourself the global instance so the static event handlers can
    // access function pointers.
    m_pInstance = this;   
}

CCMInterface::~CCMInterface()
{
    m_pInstance = NULL;
}

bool CCMInterface::IsOpen(void)
{
    return m_bXMOpenAttempted;
}

//Report F53+
bool CCMInterface::ProcessCashCounts(BSTR FAR *pCashCounts)
{
    TRACE_METHOD(_T("CCMInterface::ProcessCashCounts"));
    bool bSuccess = false;

    if(! m_hdll)
    {
        trace(L7, _T("-CCMInterface::ProcessCashCounts DLL NOT LOADED"));
        return bSuccess;
    }

    XMPROCESSCASHCOUNTS fp =(XMPROCESSCASHCOUNTS)m_fpMap["XMProcessCashCounts"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        bSuccess = (fp)(pCashCounts); //call to XM
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::ProcessCashCounts"), _T("XMProcessCashCounts"));
    }
    //-SSCOADK-5902
    return bSuccess;
}
//Report F53-

//+TAR 448434
bool CCMInterface::GetCMCashCounts(CString &csDispenserCounts, CString &csAcceptorCounts)
{
    TRACE_METHOD(_T("CCMInterface::GetCMCashCounts"));
    if(!m_hdll)
    {
        trace(L7, _T("-CCMInterface::GetCMCashCounts DLL NOT LOADED"));
        return false;
    }

    XMGETCASHCOUNTS fp =(XMGETCASHCOUNTS)m_fpMap["XMGetCashCounts"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        return (fp)(csDispenserCounts, csAcceptorCounts);
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::GetCMCashCounts"), _T("XMGetCashCounts"));
    }
    return false;
    //-SSCOADK-5902
}
//-TAR 448434

//+TAR 450643
long CCMInterface::GetDispenserCapacity(long denom)
{
    TRACE_METHOD(_T("CCMInterface::GetDispenserCapacity"));
    if(!m_hdll)
    {
        trace(L7, _T("-CCMInterface::GetDispenserCapacity DLL NOT LOADED"));
        return 0;
    }

    XMGETDISPENSERCAPACITY fp =(XMGETDISPENSERCAPACITY)m_fpMap["XMGetDispenserCapacity"];
    assert(fp != NULL);
    //+SSCOADK-5902
    long nResult = 0;
    if (fp != NULL)
    {
        nResult = (fp)(denom);
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::GetDispenserCapacity"), _T("XMGetDispenserCapacity"));
    }
    return nResult;
    //-SSCOADK-5902
}
//-TAR 450643

bool CCMInterface::Open(void)
{
    TRACE_METHOD(_T("CCMInterface::Open"));

    assert(! m_bXMOpenAttempted);
    if(m_bXMOpenAttempted)  // TAR 376515 - Open already called once.
    {
        trace(L4, _T("WARNING: CCMInterface is already open!"));
        return false;
    }

    // Check if CM is enabled or not.  If not,then don't bother loading the
    // external DLL.
    if(! co.fLocaleCashManagementScreen)
    {
        trace(L6, _T("INFO: Cash management is not enabled."));
        return false;
    }

    // TAR 452451:  Don't attempt to load CM if currently in CARD_ONLY mode.
    if(co.IsDegradedModeOn() && 
       dm.GetDegradedModeType()==DEGRADEDMODE_CARD_ONLY)
    {
        trace(L6, 
              _T("INFO:  Not loading CM. CARD_ONLY degraded mode is active."));

        // Don't worry though.  We will try to load it again in 
        // SMSmMediaStatus.
        return false;
    }
    // -TAR 452451

    // SSCOP-4169:  If any of the cash devices are not loaded, then do not
    //              allow CM to load.
    if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_CASHACCEPTOR,0))
    {
        return false;
    }

    if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_COINACCEPTOR,0))		
    {
        return false;
    }

    if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_CASHCHANGER,0))
    {
        return false;
    }
    // SSCOP-4169

    if(! Initialize())
    {
        Close();
        return false;
    }

	// +TAR 450643
	 co.nCashCapacity.RemoveAll();
	 for (int i=0; i < co.nCashValueList.GetSize(); i++ )
	 {
		long lCMCapacity = GetDispenserCapacity(co.nCashValueList[i]);
		co.nCashCapacity.Add(lCMCapacity);
		trace(L6, _T("GetDispenserCapacity from CM: denom: %d; capacity: %d"), co.nCashValueList[i], lCMCapacity );
	 }
	// -TAR 450643
    SetAuthenticationRequestSupported();
    return true;
}

// TAR 450162:  Added internal Initialize method so that we can 
//              call Close if init does not succeed.
bool CCMInterface::Initialize(void)
{
    TRACE_METHOD(_T("CCMInterface::Initialize"));

    m_bXMOpenAttempted = true;

    // Try to determine appropriate version of XMode DLL.
    bool bInitOK = false;
    CString csTargdll;
    TCHAR modname[_MAX_PATH] = _T("");  
    if( GetModuleFileName(NULL, modname, _MAX_PATH) )
    {
        csTargdll = modname;
        csTargdll.MakeUpper();

        if( csTargdll.Find(_T("SCOTAPPUD.EXE")) != -1)
            csTargdll = _T("XModeUD.dll");
        else if( csTargdll.Find(_T("SCOTAPPU.EXE")) != -1)
            csTargdll = _T("XModeU.dll");
        else if( csTargdll.Find(_T("SCOTAPPD.EXE")) != -1 )
            csTargdll = _T("XModeD.dll");
        else
            csTargdll = _T("XMode.dll");
    }

    // Try to load the DLL.
    trace(L4, _T("Attempting to load DLL [%s]"), (LPCTSTR)csTargdll);

    m_hdll = LoadLibrary(csTargdll);
    if(! m_hdll)
    {
        trace(L4, _T("** FAILED TO LOAD EXTERNAL CASH MANAGEMENT MODULE [%s]. Cash management will be disabled. **"),
                 csTargdll);
        return false;
    }
    trace(L4, _T("Load succeeded."));
    
    // Try to initialize the DLL.
    XMINIT pfnInit;
    pfnInit = (XMINIT)(GetProcAddress(m_hdll, "InitializeCM"));
    
    if(pfnInit)
    {
        trace(L4, _T("** Obtaining DM and printer wrappers. **"));
        IDMWrapper *pDM = dm.GetDMWrapper();
        IXMPrinter *pPrinter = dm.GetPrinterObject(); 

        if(! (pDM && pPrinter))
        {
            trace(L4, _T("ERROR:  Failed to obtain wrappers!"));
            return false;
        }

        trace(L4, _T("** Attempting to initialize cash management **"));
        bInitOK = (pfnInit)(*pDM, *pPrinter , *ps.GetPSXObject(), mo.hw);

        trace(L4, _T("** Initialize cash management returned [%d] **"),bInitOK);
    }
    
    if(! bInitOK)
    {
        trace(L4,_T("** FAILED TO INITIALIZE EXTERNAL CASH MANAGEMENT MODULE [%s]. Cash management will be disabled. **"),
                 csTargdll);
        FreeLibrary(m_hdll);
        m_hdll = NULL;
        m_bXMOpenAttempted = false;
        
        return false;
    }
    else
    {
        trace(L4, _T("** INITIALIZATION OF CASH MANAGEMENT MODULE SUCCEEDED **"));
    }

    // TAR 449890:  Simplify adding new functions.  
    // Try to init function pointers.
    for(int i=0; g_szCMFuncs[i]; i++)
    {
        FARPROC fp = GetProcAddress(m_hdll, g_szCMFuncs[i]);
        assert(fp != NULL);
        //+SSCOADK-5902
        if(fp == NULL)
        {
            CString csFunc(g_szCMFuncs[i]);
            trace(L4, _T("ERROR:  Failed to get function pointer for [%s]"),
                  (LPCTSTR)csFunc);
            m_fpMap[g_szCMFuncs[i]] = NULL;
        }
        else
        {
            m_fpMap[g_szCMFuncs[i]] = fp;
        }
        //-SSCOADK-5902
    }
        
    assert(bInitOK);        // Should be from check above.
    CPSXInterface::Register(&EventHandler);
    return bInitOK;
}

void CCMInterface::Close(void)
{
    TRACE_METHOD(_T("CCMInterface::Close"));

    if(! m_bXMOpenAttempted)
    {
        trace(L4, _T("CCMInterface::Close> CALLED WITHOUT CALLING Open first!"));
        return;
    }

    m_bInXM = false;

    XMUNINIT pfnUnInit;

    if(! m_hdll)
    {
        m_bXMOpenAttempted = false;
        trace(L6, _T("-CCMInterface::Close: XMode module was not loaded."));
        return;     // DLL not loaded.
    }

    pfnUnInit = (XMUNINIT)GetProcAddress(m_hdll, "UnInitializeXM");

    if(pfnUnInit)
    {
        (pfnUnInit)();
    }
    else
    {
        trace(L4, _T("-CCMInterface::Close: FAILED TO CLOSE XMode module!"));
    }

    FreeLibrary(m_hdll);
    m_hdll = NULL;

    m_fpMap.clear();
    m_bXMOpenAttempted = false;

    CPSXInterface::Unregister(&EventHandler);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Method: CurrencyDeposited
// Parameter: 
// val = currency deposited
// note = Default is false if not specified. If the currency deposited is note,
//        then we need to specify it
////////////////////////////////////////////////////////////////////////////////////////////////
void CCMInterface::CurrencyDeposited(LPCTSTR val, bool note)
{
    TRACE_METHOD(_T("CCMInterface::CurrencyDeposited"));

    //SR700 - Defines a Macro that will convert CString to LPCSTR
    USES_CONVERSION;    

    if(! m_hdll)
    {
        trace(L7, _T("-CCMInterface::CurrencyDeposited DLL NOT LOADED"));
        return;
    }

    // +SCOADK-5902
    if (co.getTimeOutCMMoneyInsertedTimer() > 0)
    {
        if (!bTimerKilled)
        {
            trace(L6, _T("Killing CM_MONEY_INSERTED_TIMER..."));
            KillTimer(mo.hw, CM_MONEY_INSERTED_TIMER);
            bTimerKilled = true;
        }
        trace(L6, _T("Starting CM_MONEY_INSERTED_TIMER..."));
        SetTimer(mo.hw,CM_MONEY_INSERTED_TIMER, co.getTimeOutCMMoneyInsertedTimer(), &CMMoneyInsertedTimerTimeout);
        bTimerKilled = false;
    }
    // -SCOADK-5902

    XMCURRENCYDEPOSIT fp = (XMCURRENCYDEPOSIT)m_fpMap["XMCurrencyDeposited"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)(T2CA(val), note);      // Call method.
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::CurrencyDeposited"), _T("XMCurrencyDeposited"));
    }
    //-SSCOADK-5902
}

// +SCOADK-5902
VOID CALLBACK CCMInterface::CMMoneyInsertedTimerTimeout(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
    trace(L6, _T("+CCMInterface::DepositTimerTimeout"));
    if (!bTimerKilled)
    {
        KillTimer(hwnd, CM_MONEY_INSERTED_TIMER);
        bTimerKilled = true;
    }
    //Notify the XMode module to enable the exit button, if in cash management.
    m_pInstance->DepositingFinished();
    trace(L6, _T("-CCMInterface::DepositTimerTimeout"));
}
// -SCOADK-5902

////////////////////////////////////////////////////////////////////////////////////////////////
// Method: CurrencyDispensed
// Parameter:
// dstr = currency to be dispensed
////////////////////////////////////////////////////////////////////////////////////////////////
void CCMInterface::CurrencyDispensed(LPCTSTR dstr)
{
   TRACE_METHOD(_T("CCMInterface::CurrencyDispensed"));

    //SR700 - Defines a Macro that will convert CString to LPCSTR
    USES_CONVERSION;    

    if(! m_hdll)
    {
        trace(L7, _T("-CCMInterface::CurrencyDispensed DLL NOT LOADED"));
        return;
    }

    XMCURRENCYDISPENSE fp = (XMCURRENCYDISPENSE)m_fpMap["XMCurrencyDispensed"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)(T2CA(dstr));  // Call method.
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::CurrencyDispensed"), _T("XMCurrencyDispensed"));
    }
    //-SSCOADK-5902
}

void CCMInterface::DeviceEvent(MessageElement &rMsg, bool &bProcessed)
{
    TRACE_METHOD(_T("CCMInterface::DeviceEvent"));

    bProcessed = false;

    if(! m_hdll)
    {
        trace(L7, _T("-CCMInterface::DeviceEvent DLL NOT LOADED"));
        return;
    }

    XMDEVICEEVENTEX fp = (XMDEVICEEVENTEX)m_fpMap["XMDeviceEventEx"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)(rMsg, bProcessed);  // Call method.
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::DeviceEvent"), _T("XMDeviceEventEx"));
    }
    //-SSCOADK-5902
}

void CCMInterface::EnterCM(void)
{
    if(m_hdll == NULL)
        return;

    m_bInXM = true;
}

void CCMInterface::ExitCM(void)
{
    if(m_hdll == NULL)
    {
        return;
    }

    m_bInXM = false;
}

void CCMInterface::GoToBaseState(void)
{
    TRACE_METHOD(_T("CCMInterface::GoToBaseState"));
	
    if(m_hdll == NULL)
    {
        return;
    }

    XMGOTOBASESTATE fp = (XMGOTOBASESTATE)m_fpMap["XMGoToBaseState"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)();
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::GoToBaseState"), _T("XMGoToBaseState"));
    }
    //-SSCOADK-5902
}

void CCMInterface::HandleEvent(const MessageElement const *pMsg) const
{
    TRACE_METHOD(_T("CCMInterface::HandleEvent"));

    if(m_hdll == NULL)
    {
        trace(L7, _T("-CCMInterface::HandleEvent DLL NOT LOADED"));
        return;
    }

    XMHANDLEEVENT fp = (XMHANDLEEVENT)m_fpMap["XMHandleEvent"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)(pMsg);
    }
    else
    {
        m_pInstance->LogFunctionNotFound(_T("CCMInterface::HandleEvent"), _T("XMHandleEvent"));
    }
    //-SSCOADK-5902
}

void CCMInterface::HandlePSXEvent(LPCTSTR szControlName, 
                                LPCTSTR szContextName, 
                                LPCTSTR szEventName, 
                                long buttonID)
{
    TRACE_METHOD(_T("CCMInterface::HandlePSXEvent"));

    assert(m_pInstance != NULL);
    if(m_pInstance->m_hdll == NULL)
    {
        trace(L7, _T("-CCMInterface::HandlePSXEvent DLL NOT LOADED"));
        return;
    }

    XMHANDLEPSXEVENT fp = 
        (XMHANDLEPSXEVENT)m_pInstance->m_fpMap["XMHandlePSXEvent"];    
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)(szControlName, szContextName, szEventName, buttonID);
    }
    else
    {
        m_pInstance->LogFunctionNotFound(_T("CCMInterface::HandlePSXEvent"), _T("XMHandlePSXEvent"));
    }
    //-SSCOADK-5902
}

bool CCMInterface::IsCMControlEvent(PPSXEVENT pEvent) const
{
    return (pEvent->strControlName.Find(_T("XM")) == 0);
}

bool CCMInterface::IsCMContextEvent(PPSXEVENT pEvent) const
{
    bool bIsCMContext = (pEvent->strControlName.Find(_T("XM")) == 0);

    return (bIsCMContext && pEvent->strControlName == _T("Display"));
}

bool CCMInterface::IsCMEvent(PPSXEVENT pEvent) const
{
    return (IsCMControlEvent(pEvent) || IsCMContextEvent(pEvent));
}

bool CCMInterface::EventHandler(CPSX *pPSX, PPSXEVENT pEvent)
{
    assert(m_pInstance != NULL);
 
    // Shouldn't have been registered with PSX if not loaded.
    assert(m_pInstance->m_hdll != NULL); 
    if(m_pInstance->m_hdll == NULL)
    {
        return false;
    }
 
    TRACE_METHOD(_T("CCMInterface::EventHandler"));
    if(m_pInstance->IsCMEvent(pEvent))

    {
        long buttonID = 0;
        if( SUCCEEDED( VariantChangeType( &pEvent->vParam, &pEvent->vParam, 
                                          0, VT_I4 ) ) )
        {
            buttonID = pEvent->vParam.lVal;
        }
        else if( SUCCEEDED( VariantChangeType( &pEvent->vParam, &pEvent->vParam, 
                                          0, VT_BSTR ) ) )
        {
            CString *pTemp = new CString;
            *pTemp = OLE2T(pEvent->vParam.bstrVal);
            buttonID = (WPARAM) pTemp;
        }
    
 
        // Send raw event to CM module.
        HandlePSXEvent(pEvent->strControlName, pEvent->strContextName,
                       pEvent->strEventName, buttonID);

	
		if(!m_bAllNotesTransferred
			&& m_csPreviousControl2 == _T("XMButton6V6HW")
			&& m_csPreviousControl1 == _T("XMApplyChangesV6HW")
			&& pEvent->strControlName == _T("XMButton1V6HW")
			&& pEvent->strContextName == _T("XMConfirmActionV6HW")			
			&& pEvent->strEventName == _T("Click"))
		{
			trace(L6, _T("CCMInterface::EventHandler Transfer all notes initiated"));
			trace(L6, _T("Need to alert Cash Office on notes transfer"));
			m_bAllNotesTransferred = true;
			m_csPreviousControl1 = m_csPreviousControl2 = _T("");
		}
		if(pEvent->strEventName == _T("Click"))
		{
			m_csPreviousControl2 = m_csPreviousControl1;
			m_csPreviousControl1 = pEvent->strControlName;
		}
		
        if(!m_pInstance->IsCMContextEvent(pEvent))//Let Core process cm change context
        {
            return true;
        }
    }   
 
    return false;

}

void CCMInterface::SetCredentials(LPCTSTR szId, LPCTSTR szPw)
{
    TRACE_METHOD(_T("CCMInterface::SetCredentials"));

    if(m_hdll == NULL)
    {
        return;
    }

    XMSETCREDENTIALS fp = (XMSETCREDENTIALS)m_fpMap["SetCredentials"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)(szId, szPw);
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::SetCredentials"), _T("SetCredentials"));
    }
    //-SSCOADK-5902
}

void CCMInterface::SetInTransaction(bool bInTrx)
{
    TRACE_METHOD(_T("CCMInterface::SetInTransaction"));

	m_bInTrx = bInTrx; //TAR 450629
}

//TAR 450629
bool CCMInterface::GetInTransaction()
{
    TRACE_METHOD(_T("CCMInterface::GetInTransaction"));

	return m_bInTrx;
}


void CCMInterface::EnterState(void)
{
    TRACE_METHOD(_T("CCMInterface::EnterState"));
    if(m_hdll == NULL)
    {
        return;
    }
    
    XMENTERSTATE fp = (XMENTERSTATE)m_fpMap["XMEnterState"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)();
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::EnterState"), _T("XMEnterState"));
    }
    //-SSCOADK-5902
    EnterCM();
}

void CCMInterface::ExitState(void)
{
    TRACE_METHOD(_T("CCMInterface::ExitState"));
    if(m_hdll == NULL)
    {
        return;
    }

    
    XMEXITSTATE fp = (XMEXITSTATE)m_fpMap["XMExitState"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)();
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::ExitState"), _T("XMExitState"));
    }
    //-SSCOADK-5902
}

void CCMInterface::BalanceUpdate(void)
{
    TRACE_METHOD(_T("CCMInterface::BalanceUpdate"));
    if(m_hdll == NULL)
    {
        return;
    }

    XMBALANCEUPDATE fp = (XMBALANCEUPDATE)m_fpMap["XMBalanceUpdate"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)();
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::BalanceUpdate"), _T("XMBalanceUpdate"));
    }
    //-SSCOADK-5902
}

// Disable copies. 
CCMInterface::CCMInterface(const CCMInterface &rhs) : m_fpMap(NULL)
{
    throw "ERROR";
}

void CCMInterface::operator=(const CCMInterface &rhs) ///< Disable copies.
{
    if(this == &rhs)
        return;

    throw "ERROR";
} 

void CCMInterface::HandleTBEvent(MessageElement *pMsg, bool &bProcessed)
{
    if(m_hdll == NULL)
    {
        trace(L7, _T("CCMInterface::HandleTBEvent DLL NOT LOADED"));
        return;
    }

    TRACE_METHOD(_T("CCMInterface::HandleTBEvent"));

    
    XMHANDLETBEVENT fp = (XMHANDLETBEVENT )m_fpMap["XMHandleTBEvent"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)(pMsg, bProcessed);
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::HandleTBEvent"), _T("XMHandleTBEvent"));
    }
    //-SSCOADK-5902
}

//TAR 448426
bool CCMInterface::IsInCashManagement(void)
{
    trace(L7, _T("CCMInterface::IsInCashManagement %d"), m_bInXM);

    return m_bInXM;
}

//TAR 448426
void CCMInterface::HandleFPEvent(PSEVT pEvent, long lParam, long wParam, bool bFPConnected)
{
    TRACE_METHOD(_T("CCMInterface::HandleFPEvent"));

    if(m_hdll == NULL)
    {
        return;
    }

    XMHANDLEFPEVENT fp = (XMHANDLEFPEVENT )m_fpMap["XMHandleFPEvent"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)(pEvent, lParam, wParam, bFPConnected);
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::HandleFPEvent"), _T("XMHandleFPEvent"));
    }
    //-SSCOADK-5902
}

// SR752+
TBSTATE CCMInterface::ProcessMessageHook(MessageElement *pMsg)
{
    TRACE_METHOD(_T("CCMInterface::ProcessMessageHook"));
        
    if(m_hdll == NULL)
    {
        return TB_UNKNOWN;
    }

    TBPROCESSMESSAGEHOOK fp = (TBPROCESSMESSAGEHOOK)m_fpMap["TBProcessMessageHook"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        return (fp)(pMsg);
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::ProcessMessageHook"), _T("TBProcessMessageHook"));
    }
    return TB_UNKNOWN;
    //-SSCOADK-5902
}


bool CCMInterface::WaitForTBResponse(void)
{
    TRACE_METHOD(_T("CCMInterface::WaitForTBResponse"));
    bool bReturn = false;

    if(TBIsAllowed(TB_FT_CASHOPERATION))
    {
        if(m_hdll == NULL)
        {
            bReturn = false;
        }

        XMWAITFORTBRESPONSE fp = (XMWAITFORTBRESPONSE)m_fpMap["XMWaitForTBResponse"];
        assert(fp != NULL);
        //+SSCOADK-5902
        if (fp != NULL)
        {
            bReturn = (fp)();
        }
        else
        {
            LogFunctionNotFound(_T("CCMInterface::WaitForTBResponse"), _T("XMWaitForTBResponse"));
        }
        //-SSCOADK-5902
    }

    return bReturn;
}
// SR752-

void CCMInterface::ExitToApp(void)
{
    TRACE_METHOD(_T("CCMInterface::ExitToApp"));
	
    if(m_hdll == NULL)
    {
        return;
    }

    XMEXITTOAPP fp = (XMEXITTOAPP)m_fpMap["XMExitToApp"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)();
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::ExitToApp"), _T("XMExitToApp"));
    }
    //-SSCOADK-5902
}

void CCMInterface::CancelAuthenticationRequest(void)
{
    TRACE_METHOD(_T("CCMInterface::CancelAuthenticationRequest"));
	
    if(m_hdll == NULL)
    {
        return;
    }

    XMCANCELAUTHREQUEST fp = (XMCANCELAUTHREQUEST)m_fpMap["OnAuthenticationRequestCancelled"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)();
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::CancelAuthenticationRequest"), _T("OnAuthenticationRequestCancelled"));
    }
    //-SSCOADK-5902
}

void CCMInterface::SetAuthenticationRequestSupported(void)
{
    TRACE_METHOD(_T("CCMInterface::SetAuthenticationRequestSupported"));
	
    if(m_hdll == NULL)
    {
        return;
    }

    XMSETAUTHENTICATIONREQUESTSUPPORTED fp = (XMSETAUTHENTICATIONREQUESTSUPPORTED)m_fpMap["SetAuthenticationRequestSupported"];
    assert(fp != NULL);
    //+SSCOADK-5902
    if (fp != NULL)
    {
        (fp)();
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::SetAuthenticationRequestSupported"), _T("SetAuthenticationRequestSupported"));
    }
    //-SSCOADK-5902
}

bool CCMInterface::GetNotesTransferStatus(void)
{	
	return m_bAllNotesTransferred;
}

void CCMInterface::ResetNotesTransferStatus(void)
{	
	m_bAllNotesTransferred = false;
}
//+SSCOADK-5902
void CCMInterface::DepositingFinished(void)
{
    TRACE_METHOD(_T("CCMInterface::DepositingFinished"));
	
    if(m_hdll == NULL)
    {
        return;
    }

    XMDEPOSITINGFINISHED fp = (XMDEPOSITINGFINISHED)m_fpMap["XMDepositingFinished"];
    assert(fp != NULL);
    if (fp != NULL)
    {
        (fp)();
    }
    else
    {
        LogFunctionNotFound(_T("CCMInterface::DepositingFinished"), _T("XMDepositingFinished"));
    }
}

void CCMInterface::LogFunctionNotFound(const CString &csMethodName, const CString &csFunctionName)
{
    trace(L4, _T("%s>  function pointer for %s is NULL."), (LPCTSTR)csMethodName, (LPCTSTR)csFunctionName);
}
//-SSCOADK-5902
#endif //_CPPUNIT