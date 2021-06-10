// ACSHook.h : Declaration of the CACSHook
//
// Change history:
//
// POS97215 Work Request:61233 Name:Matt Condra    Date:May 20, 2014
// POS81632 Work Request:59313 Name:Matt Condra    Date:December 10, 2013
// POS14165 Work Request:13752 Name:Robert Susanto Date:May 23, 2011
// POS13219 Work Request:13752 Name:Robert Susanto Date:April 19, 2011
//
#pragma once
#include "resource.h"       // main symbols

#include "AppModel.h"
#include "ACSIOClient.h"
#include "Properties.h"
#include "Contexts.h"
#include "Controls.h"
#include <queue>

typedef [public, v1_enum] enum tagEntryFlag { ACSEX_OPER_KEYED = 0, ACSEX_OPER_SCANNED = 1 } ExEntryFlag;

// IACSHook
[
   object,
   uuid("E04A785D-8CF5-436B-A204-16DFE97667A7"),
   dual,	helpstring("IACSHook Interface"),
   pointer_default(unique)
]
__interface IACSHook : IDispatch
{
   [id(1), helpstring("method Connect")] HRESULT Connect(void);
   [id(2), helpstring("method Disconnect")] HRESULT Disconnect(void);
   [id(3), helpstring("method GetProperty")] HRESULT GetProperty([in] BSTR strName, [out, retval] BSTR* pVal);
   [id(4), helpstring("method SetProperty")] HRESULT SetProperty([in] BSTR strName, [in] BSTR newVal);
   [id(5), helpstring("method SendRawData")] HRESULT SendRawData([in] BSTR strData);
   [id(6), helpstring("method SendInput")] HRESULT SendInput([in] BSTR strInputName);
   [propget, id(7), helpstring("property States")] HRESULT States([out, retval] VARIANT* pVal);
   [propget, id(8), helpstring("property Inputs")] HRESULT Inputs([out, retval] VARIANT* pVal);
   [propget, id(9), helpstring("property Properties")] HRESULT Properties([out, retval] VARIANT* pVal);
   [id(10), helpstring("method SetCallback")] HRESULT SetCallback([in] VARIANT vCallbackProc);
   [id(11), helpstring("method SendSensitiveRawData")] HRESULT SendSensitiveRawData([in] BSTR strData);
   [id(12), helpstring("method SendControlMessage")] HRESULT SendControlMessage([in] VARIANT vArray);
   [id(13), helpstring("method ValidateOperator")] HRESULT ValidateOperator([in] BSTR strLogin, [in] BSTR strPassword, [out, retval] VARIANT_BOOL * pVal);
   [id(14), helpstring("method ValidateSupervisor")] HRESULT ValidateSupervisor([in] BSTR strLogin, [in] BSTR strPassword, [out, retval] VARIANT_BOOL * pVal);
   [propget, id(15), helpstring("property Controls")] HRESULT Controls([out, retval] VARIANT* pVal);
   [propget, id(16), helpstring("property Contexts")] HRESULT Contexts([out, retval] VARIANT* pVal);
   [id(17), helpstring("method SendAuxMsg")] HRESULT SendAuxMsg([in] BSTR strXmlData, [in] long lNotifyId);
   [id(19), helpstring("method ValidateEmployeeOperatorString")] HRESULT ExValidateEmployeeOperatorString([in] BSTR strID, [in] BSTR strPassword, [in] ExEntryFlag entryFlag, [out, retval] VARIANT_BOOL * pVal);
   [id(20), helpstring("method ValidateEmployeeSupervisorString")] HRESULT ExValidateEmployeeSupervisorString([in] BSTR strID, [in] BSTR strPassword, [in] ExEntryFlag entryFlag, [out, retval] VARIANT_BOOL * pVal);
   [id(21), helpstring("method ValidateEmployeeOperatorData")] HRESULT ExValidateEmployeeOperatorData([in] BSTR strID, [in] VARIANT vData, [in] ExEntryFlag entryFlag, [out, retval] VARIANT_BOOL * pVal);
   [id(22), helpstring("method ValidateEmployeeSupervisorData")] HRESULT ExValidateEmployeeSupervisorData([in] BSTR strID, [in] VARIANT vData, [in] ExEntryFlag entryFlag, [out, retval] VARIANT_BOOL * pVal);
   [id(23), helpstring("method ExGetTerminalBusinessDay")] HRESULT ExGetTerminalBusinessDay([in] long lTerminalID, [out] long * businessDay, [out, retval] VARIANT_BOOL * pVal);
   [id(24), helpstring("method ExGetEmplidFromBadge")] HRESULT ExGetEmplidFromBadge([in] BSTR strBadge, [out] BSTR * pstrEmplid, [out, retval] VARIANT_BOOL * pVal);
   [id(25), helpstring("method CheckInOutOperator")] HRESULT CheckInOutOperator([in] int nFunc, [in] BSTR strLogin, [in] BSTR strPassword, [in] int nNewRAPid, [out] int * currentRAPid, [out] int * offlineStatus, [out, retval] int * pVal); // POS81632
   [id(26), helpstring("method GetOperatorLastPwdChgTime")] HRESULT GetOperatorLastPwdChgTime([in] BSTR strID, [out] int * timeYear, [out] int * timeMonth, [out] int * timeDay, [out] int * timeHour, [out] int * timeMinute, [out] int * timeSecond, [out, retval] int * pVal); // POS97215
};


 // _IACSHookEvents
[
   dispinterface,
   uuid("967F4276-3B76-48CC-BD51-FC6FED9A3E3C"),
   helpstring("_IACSHookEvents Interface")
]
__interface IACSHookEvents
{
   [id(1), helpstring("method OnEvent")] void OnEvent([in] LONG lEvent, LONG lData, BSTR szData1, BSTR szData2);
};


// CACSHook

[
   coclass,
   threading("Apartment"),
   support_error_info("IACSHook"),
   event_source("com"),
   vi_progid("ACSHookManager.ACSHook"),
   progid("ACSHookManager.ACSHook.1"),
   version(1.0),
   uuid("ED78D3F3-599D-404F-A8E0-12282AB6405C"),
   helpstring("ACSHook Class")
]
class ATL_NO_VTABLE CACSHook : 
   public IACSHook                                                                      
{
public:
   CACSHook();
   ~CACSHook();

   __event __interface IACSHookEvents;

   DECLARE_PROTECT_FINAL_CONSTRUCT()

   HRESULT FinalConstruct();
   void FinalRelease();

public: // tracing and other routines accessed by other classes
   enum ACSHookEvent
   {
      OnConnectEvent = 1,
      OnDisconnectEvent = 2,
      OnGenMsgEvent = 3,
      OnTraceEvent = 4  // no longer supported
   };

   enum tbConnectReceiveEvent
   {
      ReceiveEvent				= 0,
      ReceiveItemObjEvent		= 1,
      ReceiveCashDrawerObjEvent	= 2,
      ReceiveTotalObjEvent		= 3,
      ReceiveTenderObjEvent		= 4,
      ReceiveRewardObjEvent		= 5,
      ReceiveScannerObjEvent	= 6,
      ReceiveKeyboardObjEvent	= 7,
      ReceiveKeylockObjEvent	= 8,
      ReceiveMSRObjEvent		= 9,
      ReceiveMICRObjEvent		= 10,
      ReceivePrinterObjEvent	= 11,
      ReceivePinpadObjEvent		= 12,
      ReceiveScaleObjEvent		= 13,
      ReceiveCustomObjEvent		= 14,
      ReceiveTBGenObjEvent		= 15
   };


   typedef void (WINAPI *LPACSHOOKCALLBACKPROC)(long lEvent, long lData, BSTR bstrData1, BSTR bstrData2);

   void FireEvent(long lEvent, long lData, LPCTSTR szData1 = NULL, LPCTSTR szData2 = NULL);
   void FireStateChange(const _bstr_t & bstrState, 
      const _bstr_t & bstrGroup = OLESTR(""),
      const _bstr_t & bstrIgnoreDuplicate = OLESTR(""), 
      const _bstr_t & bstrTriggerDelay = OLESTR(""));



   HRESULT InternalSendInput(BSTR strInputName);

   void SafeSendData(CString const& strData);
   inline DWORD GetIdlePeriod() { return GetTickCount() - m_dwLastIO; }

   // Build and send Control messages back to Transaction Broker
   bool SendControlToTB(LPCTSTR pszControl);
   // Build and send Control messages back to Application
   bool SendControlToApp(LPCTSTR pszControl);

   std::vector<BYTE> GetBytesFromVariant(VARIANT & vData);


protected:
   HANDLE m_hContextEvent;
   HANDLE m_hContextThread;
   HANDLE m_hDisconnectEvent;
   HANDLE m_hAppMonitorEvent;
   HANDLE m_hAppMonitorThread;
   HANDLE m_hPOSapp;
   LPACSHOOKCALLBACKPROC m_pfnCallbackProc;
   DWORD m_dwLastIO;
   HCLIENT m_hClient;
   HCLIENT m_hSeqClient;
   HCLIENT m_hAuxClient;
   CString m_strCurrentState;
   long m_lAuxMsgSeq;

   // tbControls objects
   TBC::ItbConnectPtr    m_spTBConnect;

   TBC::IKeyboardObjPtr m_spKey;
   CCriticalSection m_csKey;

   TBC::ITBGenObjPtr m_spGenState;
   CCriticalSection m_csGenState;

   // serialize sending data to TB in case it's not
   CCriticalSection m_csGenControl;

   CAppModel m_appModel;
   CProperties m_Properties;
   CContexts m_Contexts;
   CControls m_Controls;
   CCriticalSection m_csDataSend;

	CCriticalSection m_csContextQueue;
	//pos14165 managing the pointer of VARIANT instead of using local copy
	typedef std::pair<BSTR, VARIANT*> ContextPair;
	// e pos14165

	std::queue<ContextPair> m_contextQueue;

	HANDLE m_hReaderThread;

	HANDLE m_hContextSemaphore;

	/* Context reader */
   friend DWORD WINAPI ReadContextsProc(LPVOID pvParam);
   DWORD ReadContexts(void);

   // context dispatcher
   friend DWORD WINAPI DispatchContextsProc(LPVOID pvParam);
   DWORD DispatchContexts(void);
   void TerminateContextListenerProcessor(void);

   // context processor
   friend void CALLBACK ProcessContextsProc(LPSTR pszContext, void* pvParam);
   //POS14165 don't use the _bstr_t and _variant_t as the _variant_t will make a variantcopy of the ContextData that is sensitive to memory corruption
   void ProcessContexts(const BSTR contextName, const VARIANT& contextData);

   // TBConnect TB Message processor
   friend void WINAPI TBConnectCallbackProc(long lEvent, IDispatch * pObj);


   // Application Monitor proc
   friend DWORD WINAPI ApplicationMonitorProc(LPVOID pvParam);
   DWORD ApplicationMonitor(void);
   void TerminateApplicationMonitor(void);

   // IACSHook Methods
public:
   STDMETHOD(Connect)(void);
   STDMETHOD(Disconnect)(void);
   STDMETHOD(GetProperty)(BSTR strName, BSTR* pVal);
   STDMETHOD(SetProperty)(BSTR strName, BSTR newVal);
   STDMETHOD(SendRawData)(BSTR strData);
   STDMETHOD(SendInput)(BSTR strKeyName);
   STDMETHOD(SetCallback)(VARIANT vCallProc);
   STDMETHOD(get_States)(VARIANT* pVal);
   STDMETHOD(get_Inputs)(VARIANT* pVal);
   STDMETHOD(get_Properties)(VARIANT* pVal);
   STDMETHOD(get_Controls)(VARIANT* pVal);
   STDMETHOD(get_Contexts)(VARIANT* pVal);
   STDMETHOD(SendSensitiveRawData)(BSTR strData);
   STDMETHOD(SendControlMessage)(VARIANT vArray);
   STDMETHOD(ValidateOperator)(BSTR strLogin, BSTR strPassword, VARIANT_BOOL * pVal);
   STDMETHOD(ValidateSupervisor)(BSTR strLogin, BSTR strPassword, VARIANT_BOOL * pVal);
   STDMETHOD(SendAuxMsg)(BSTR strXmlData, long lNotifyId);


   // API supporting EMPLID entry, badge scan and biometric authentication
   STDMETHOD(ExValidateEmployeeOperatorString)(BSTR strID, BSTR strPassword, ExEntryFlag entryFlag, VARIANT_BOOL * pVal);
   STDMETHOD(ExValidateEmployeeSupervisorString)(BSTR strID, BSTR strPassword, ExEntryFlag entryFlag, VARIANT_BOOL * pVal);
   STDMETHOD(ExValidateEmployeeOperatorData)(BSTR strID, VARIANT vData, ExEntryFlag entryFlag, VARIANT_BOOL * pVal);
   STDMETHOD(ExValidateEmployeeSupervisorData)(BSTR strID, VARIANT vData, ExEntryFlag entryFlag, VARIANT_BOOL * pVal);
   STDMETHOD(ExGetTerminalBusinessDay)(long lTerminalID, long * businessDay, VARIANT_BOOL * pVal);
   STDMETHOD(ExGetEmplidFromBadge)(BSTR strBadge, BSTR* pstrEmplid, VARIANT_BOOL * pVal);

   // POS81632
   STDMETHOD(CheckInOutOperator)(int nFunc, BSTR strLogin, BSTR strPassword, int nNewRAPid, int * currentRAPid, int * offlineStatus, int * pVal);

   // POS97215
   STDMETHOD(GetOperatorLastPwdChgTime)(BSTR strID, int * timeYear, int * timeMonth, int * timeDay, int * timeHour, int * timeMinute, int * timeSecond, int * pVal);

};

