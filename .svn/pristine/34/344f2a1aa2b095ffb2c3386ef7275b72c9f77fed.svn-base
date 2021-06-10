//************************************************************************
// 
// Filename:  CMInterface.h
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

#ifndef _CM_INTERFACE_H
#define _CM_INTERFACE_H


#include "ScotMsg.h"                    // Need MessageElement.
#include "Common.h"
#include "PSXInterface.h"
#include <string>
#include "FPMap.h"

class IDMWrapper;
class TraceObject;
class CPSX;

#ifdef SCOTSSF
class DLLIMPORT
#else
class DLLEXPORT
#endif
      CCMInterface
{
public:
    CCMInterface();
    ~CCMInterface();

    virtual bool Open(void);

    virtual void Close(void);                                   // Cloe XMode DLL
    virtual void CurrencyDeposited(LPCTSTR val, bool note=false);
    virtual void CurrencyDispensed(LPCTSTR dstr);

    /**
     * Pass device DM_STATUS, DM_ERROR, DM_DIRECTIO events down to CM.
     * \param[in] rMsg MessageElement for the event.
     * \param[out] bProcessed - Set to true by CM if the event was handled.
     *             false otherwise.
     */
    virtual void DeviceEvent(MessageElement &rMsg, bool &bProcessed);
    virtual void EnterCM(void);                         // Set Cash management active.
    virtual void ExitCM(void);                          // Set Cash management inactive.
    virtual void GoToBaseState(void);                   // Go to CM base state
    virtual void ExitToApp(void);
    virtual bool IsOpen(void);       // Return true if Open() called.

    virtual bool ProcessCashCounts(BSTR FAR *pResult); //Report F53
	virtual bool GetCMCashCounts(CString &csDispenserCounts, CString &csAcceptorCounts); //TAR 448434
	virtual long GetDispenserCapacity(long denom); //TAR 450643


    /**
     * Pass UI event down to be handled by CM module.
     */
    virtual void HandleEvent(const MessageElement const *pMsg) const;

    /** TAR 448431
     * Pass TB messages to CM module.
     */
    virtual void HandleTBEvent(MessageElement *pMsg, bool &bProcessed);

    /**
     * Pass raw PSX events to CM module.
     */
    static void HandlePSXEvent(LPCTSTR szControlName, 
                               LPCTSTR szContextName, 
                               LPCTSTR szEventName, 
                               long buttonID);

    /**
     * Send operator credentials down to CM module.
     */
    virtual void SetCredentials(LPCTSTR szId, LPCTSTR szPw);

    /**
     * Set in transaction down to CM module.
     */
    virtual void SetInTransaction(bool bInTrx);
	
	/**
     * Get in transaction
     */
    virtual bool GetInTransaction(); //TAR 450629

    /**
     * Handler to process CM UI events.
     */
    static bool EventHandler( CPSX *pPSX, PPSXEVENT pEvent );


	/**
     * Tells if the cashier has performed 'Transfer All Notes'.
     */
	virtual bool GetNotesTransferStatus(void);

	/**
     * Resets the notes transfer flag
     */
	virtual void ResetNotesTransferStatus(void);

    /**
     * Initialize the current CM state.
     */
    virtual void EnterState(void);

    /**
     * UnInitialize the current CM state.
     */
    virtual void ExitState(void);

     /**
     * Send Cash Balances to report server after a cash transaction.
     */
    virtual void BalanceUpdate(void);
	
	/**
     * Check if we are in CM
     */
    virtual bool IsInCashManagement(void);  //TAR 448426
    
    /**
     * Handle fingerprint event and pass it down to CM
     */
    virtual void HandleFPEvent(PSEVT pEvent, long lParam, long wParam, bool bFPConnected); //TAR 448426

	/** 
     * \brief This method allows the CM module to examine TB return states.
     * \note Added for SR752
     */
    virtual TBSTATE ProcessMessageHook(MessageElement *pMsg);

    /**
     * \brief Wait for TB's response before finalizing changes.
     * \param[in] void
     * \return true if CM is waiting for TB's response, false otherwise
     * \note Added for SR752
     */
    virtual bool WaitForTBResponse(void);
    
    /** 
     * \brief This method informs CM that authentication is cancelled.
     */
    virtual void CancelAuthenticationRequest(void);


    virtual bool IsCMEvent(PPSXEVENT pEvent) const;

    // +SCOADK-5902
    virtual void DepositingFinished(void);
    static VOID CALLBACK CCMInterface::CMMoneyInsertedTimerTimeout(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
    static bool bTimerKilled;
    // -SCOADK-5902

protected:
    virtual bool Initialize(void);
    virtual bool IsCMControlEvent(PPSXEVENT pEvent) const;

    virtual bool IsCMContextEvent(PPSXEVENT pEvent) const;

    bool  m_bInXM;                      // true when in external cash mgmnt.
    HINSTANCE m_hdll;                   // Handle to XMode DLL.

    CFPMap<std::string, FARPROC> m_fpMap;// Holds our function pointers.

    CCMInterface(const CCMInterface &rhs);        //copy constructor
    void operator=(const CCMInterface &rhs);    //assignment operator
    
    bool m_bXMOpenAttempted;            // Indicate if open attempted.

    static CCMInterface *m_pInstance;   // Used for our PSX event handler.
	bool m_bInTrx;						// //TAR 450629 true when in transaction
	static bool m_bAllNotesTransferred;
	static CString m_csPreviousControl1;
	static CString m_csPreviousControl2;
	
	 //+-SSCOADK-5902
    void LogFunctionNotFound(const CString &csMethodName, const CString &csFunctionName);
    /** 
    * \brief This method informs CM that authentication is supported.
    */
    virtual void SetAuthenticationRequestSupported(void);	
};

#endif //  _CM_INTERFACE_H
