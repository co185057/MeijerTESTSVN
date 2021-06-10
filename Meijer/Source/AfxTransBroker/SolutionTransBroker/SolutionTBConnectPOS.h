// SolutionTBConnectPOS.h: interface for the CSolutionTBConnectPOS class.
//
// Change history:
//
// POS277809 Work Request:73594 Name:Robert Susanto Date:July 12, 2016
// POS97215 Work Request:61233 Name:Matt Condra Date:May 20, 2014
// POS81632 Work Request:59313 Name:Matt Condra Date:December 10, 2013
// POS15617 Work Request:15196 Name:Matt Condra Date:July 20, 2011
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionTBConnectPOS_H__INCLUDED_)
#define AFX_SolutionTBConnectPOS_H__INCLUDED_

#include "SolutionTBDefs.h"
#include "BaseTBConnectPOS.h"
#include "SingleInstance.h"

class CSolutionTBConnectPOS : public CBaseTBConnectPOS
{
public:
    virtual ~CSolutionTBConnectPOS();
    CCriticalSection  m_Critical;

protected:
    CSolutionTBConnectPOS();
    virtual BOOL SendData(LPARAM szData);

private:
    CSolutionTBConnectPOS(const CSolutionTBConnectPOS &);              // hide copy const
    CSolutionTBConnectPOS & operator =(const CSolutionTBConnectPOS &); // hide assignment

public: 
	BOOL Connect(void);
	BOOL Disconnect(void);
	CString GetProperty(LPCTSTR szName);
	BOOL SetProperty(LPCTSTR szName, LPCTSTR szValue);
	BOOL SendControlMessage(VARIANT vArray);
    virtual void SendMsgToPOS(CTBMsgObj &obj);
	virtual BOOL SetSelectedItem(LPCTSTR szEntryID);
    virtual BOOL SendInput(LPARAM szInput);
    virtual bool InitEmulation();
    BOOL ACSValidateOperator(LPCTSTR szOperatorID, LPCTSTR szOperatorPw);
    BOOL ACSValidateSupervisor(LPCTSTR szOperatorID, LPCTSTR szSupervisorPw);
    int ACSCheckInOutOperator(int nFunc, LPCTSTR szOperatorID, LPCTSTR szOperatorPw, int nNewRAPid, int * currentRAPid, int * offlineStatus); // POS81632
    int ACSGetOperatorLastPwdChgTime(LPCTSTR szOperatorID, int * pYear, int * pMonth, int * pDay, int * pHour, int * pMinute, int * pSecond); // POS97215
    BOOL ACSExValidateEmployeeOperatorString(LPCTSTR szID, LPCTSTR szOperatorPw, ACS::ExEntryFlag entryFlag);
    BOOL ACSExValidateEmployeeSupervisorString(LPCTSTR szID, LPCTSTR szSupervisorPw, ACS::ExEntryFlag entryFlag);
    BOOL ACSExValidateEmployeeOperatorData(LPCTSTR szID, LPCTSTR szData, ACS::ExEntryFlag entryFlag);
    BOOL ACSExValidateEmployeeSupervisorData(LPCTSTR szID, LPCTSTR szData, ACS::ExEntryFlag entryFlag);
	BOOL ACSExGetBusinessDate(long terminal, long * currentBusinessDate);
	static void IsTerminating(BOOL bTerminating);
	static BOOL IsTerminating(void);
    static void CallbackLock();
    static void CallbackUnlock();
    BOOL SendAuxMsg(LPCTSTR pszAuxMsg, long lNotifyId);
    void StopHookThread(void);
	static bool ms_bIsItemsNeedTobeBuffered;
    static CString csPrev_bstr1; // POS15617

	//CRD 232268
	static CArray <CString, CString> csEASEntryIDList;
	static long lMobileAssistanceNeeded;

protected:
	ACS::IACSHookPtr GetHook(bool bForceMarshalled=false);	//POS277809

	ACS::IACSHookPtr m_spHook;
    IStream * m_pHookStream;
	DWORD m_dwHookThreadId;
	HANDLE m_hExitHookSignal;
	HANDLE m_hHookThread;
	HANDLE m_hInitHookSignal;

	enum ACSHookEvent
    {
        OnConnectEvent = 1,
        OnDisconnectEvent = 2,
        OnGenMsgEvent = 3,
        OnTraceEvent = 4
    };
    
	enum ACSHookTraceType
    {
        ttInfo = 1,
        ttWarning = 2,
        ttError = 3,
        ttDebug = 4
    };

	//
	// ACS Hook Callback
	//
    typedef void (WINAPI *LPACSHOOKCALLBACKPROC)(long lEvent, long lData, BSTR bstrData1, BSTR bstrData2);
	friend void WINAPI ACSHookCallbackProc(long lEvent, long lData, BSTR bstrData1, BSTR bstrData2);

	//
	// ACS Hook Creation Thread
	//
	friend DWORD WINAPI ACSHookThreadProc(PVOID pvParam);
	DWORD ACSHookThread(void);



};

#endif 
