// ACSHook.cpp : Implementation of CACSHook
//
// Change history:
//
// POS97215 Work Request:61233 Name:Matt Condra Date:May 20, 2014
// POS81632 Work Request:59313 Name:Matt Condra Date:December 10, 2013
// POS15617 Work Request:15196 Name:Matt Condra Date:July 20, 2011
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACSHook.h"
#include "DataKit.h"
#include "ACS.h"
#include "ACSExposedFunctions.h"
#include "Utils.h"
#include "Options.h"
#include "TB.h"
#include "CPSAPI.h"
#include "TraceImpl.h"
#include "ACSIOClientApi.h"
#include <iomanip>

using NCR::Util::endl;

/* Initialize one global utilities class */
CUtils Utils;

DWORD WINAPI ApplicationMonitorProc(LPVOID pvParam)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	_ASSERT(SUCCEEDED(hr));
	CACSHook & acsHook = *(CACSHook*)pvParam;
	DWORD dwRet = acsHook.ApplicationMonitor();
	CoUninitialize();
	return dwRet;
}

void WINAPI TBConnectCallbackProc(long lEvent, IDispatch * pObj)
{
	Trace trace(_T("TBConnectCallbackProc"), false);

	// Foreign thread, do as little as possible here and return!
	// convert the TBMsgObj to a XML TBGenMSg object and send it to TB for processinggg
	try
	{
		if ( lEvent != 0 )
		{
			TBC::ITBStreamPtr spInputStream = pObj;

			long lSize = 0;

			// NOTE:  The pointer below still belongs to spInputStream
			BYTE* pBuf = spInputStream->StreamOut(&lSize);

			CString csData;
			CString csTemp;
			csData.Empty();
			for( int i=0; i<lSize; i++ )
			{
				csTemp.Format(_T("%02X"), pBuf[i] );
				csData += csTemp;
			}

			Utils.ACSHook().FireEvent(CACSHook::ACSHookEvent::OnGenMsgEvent, lEvent, csData);
		}
	}
	catch(_com_error & cex)
	{
		_ASSERT(FALSE);
		trace(DCM_ERROR)
			<< _T("An exception occured while processing a tbControls event(callback). Error: [0x") << std::hex << cex.Error()
			<< std::dec << _T("]: ") << cex.ErrorMessage() << endl;
	}
}

using namespace NCR::Util;

CACSHook::CACSHook() :  
	m_hContextThread(NULL), 
	m_hContextSemaphore(NULL),
	m_hReaderThread(NULL),
	m_hDisconnectEvent(NULL), 
	m_hAppMonitorThread(NULL),
	m_hAppMonitorEvent(NULL), 
	m_pfnCallbackProc(NULL), 
	m_dwLastIO(GetTickCount()), 
	m_hClient(NULL), 
	m_hSeqClient(NULL),
	m_hAuxClient(NULL),
	m_strCurrentState(_T("")),
	m_hPOSapp(NULL),
	m_lAuxMsgSeq(0)
{
	Trace::InitLog(_T("ACSHookManager"));
	BOOL bRet = PSAPI::Load();
	_ASSERT(bRet);
	PROCESSMODULEINFO pmi = PSAPI::FindLoadedModuleInfo(GetCurrentProcessId(), _T("TransBroker"));

	if( _tcslen(pmi.szPathName) )
	{
		TB::Load(pmi.szPathName);
		Options::Initialize(TB::GetTerminalNumber());
	}
	else
	{
		Options::Initialize(1);
	}
	Trace trace(_T("CACSHook::CACSHook"));
}

CACSHook::~CACSHook()
{
	// explicit scope to destroy Trace class before ExitLog
	{ 
		Trace trace(_T("CACSHook::~CACSHook"));
		trace(DCM_INFO, _T("Unloading ACSFuncs..."));
		ACSFuncs::Unload();
		trace(DCM_INFO, _T("Unloading TB interface..."));
		TB::Unload();
		trace(DCM_INFO, _T("Unloading PSAPI..."));
		PSAPI::Unload();
	}
	Trace::ExitLog();
}

HRESULT CACSHook::FinalConstruct()
{
	Trace trace(_T("CACSHook::FinalConstruct"));

	trace(DCM_INFO, _T("Initializing CACSHook control object..."));

	try
	{
		// Initialize all the object references in the controller
		Utils.SetContextsObject(m_Contexts);
		Utils.SetControlsObject(m_Controls);
		Utils.SetPropertiesObject(m_Properties);
		Utils.SetACSHookObject(*this);

		if (!DataKit::IsInitialized())
		{
			DataKit::Initialize();

			if (!DataKit::IsInitialized())
			{
				trace(DCM_ERROR, _T("Failed to initialize the ACSIODataKit DLL."));
				return E_FAIL;
			}

			HRESULT hr = DataKit::LoadContextStructures();

			if (FAILED(hr))
			{
				trace(DCM_ERROR, _T("Failed to load context structures via ACSIODataKit DLL."));
				return hr;
			}


			hr = DataKit::LoadSequenceStructures();

			if (FAILED(hr))
			{
				trace(DCM_ERROR, _T("Failed to load sequence structures via ACSIODataKit DLL."));
				return hr;
			}
		}

		// Create merged document
		CString strFile = Options::OptionValue(ACS::OPT_APPMODEL, ACS::OPT_APPMODEL_DEF);

		BOOL bMerge = FALSE; // set to true if at least one file was merged.
		BOOL bRet = m_appModel.AppModelMergeBegin(strFile);
		if( bRet )
		{
			// see if there are any documents to merge
			for(int i = 2; bRet; i++)
			{
				// Get the next file to merge
				CString strOpt;
				CString strMergeFile;
				strOpt.Format(_T("%s_%d"), ACS::OPT_APPMODEL, i);
				strMergeFile = Options::OptionValue(strOpt);
				if( strMergeFile.GetLength() )
				{
					bMerge = TRUE;// flag that at least one merge has to occured
					bRet = m_appModel.AppModelMergeAdd(strMergeFile);
				}
				else
				{
					// no more files - break with a good result code
					break;
				}
			}

			// get final merged file output name otherwise use the original file
			if( bMerge && bRet )
			{
				strFile = Options::OptionValue(ACS::OPT_FINALAPPMODEL, ACS::OPT_FINALAPPMODEL_DEF);
				bRet = m_appModel.AppModelMergeEnd(strFile);
			}
		}

		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to create the merged Application Model."));
			return E_FAIL;
		}

		//
		// Load and verify the Application Model. Next, initialize the Application Model properties.
		//
		bRet = m_appModel.SetApplicationModel(strFile);
		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to load the Application Model."));
			return E_FAIL;
		}

		bRet = m_appModel.InitializeControls(m_Controls);
		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the Application Controls."));
			m_appModel.SetApplicationModel(_T(""));
			return E_FAIL;
		}

		bRet = m_appModel.InitializeProperties(m_Properties);
		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the Hook Properties."));
			m_appModel.SetApplicationModel(_T(""));
			return E_FAIL;
		}

		//
		// Initialize TB Controls for PipeServer communication
		//
		HRESULT hr = m_spTBConnect.CreateInstance(__uuidof(TBC::CtbConnect));
		if( FAILED(hr) )
		{
			trace(DCM_ERROR) 
				<< _T("Failed to create the tbConnect object. Error: 0x") << std::hex << hr << std::dec << endl;
			return E_FAIL;
		}

		hr = m_spKey.CreateInstance(__uuidof(TBC::CKeyboardObj));
		if( FAILED(hr) )
		{
			trace(DCM_ERROR) 
				<< _T("Failed to create the KeyboardObj object. Error: 0x") << std::hex << hr << std::dec << endl;
			return E_FAIL;
		}
		hr = m_spGenState.CreateInstance(__uuidof(TBC::CTBGenObj));
		if( FAILED(hr) )
		{
			m_spKey = NULL;
			trace(DCM_ERROR) 
				<< _T("Failed to create the GenMsg object for State event messages. Error: 0x") << std::hex << hr << std::dec << endl;
			return E_FAIL;
		}
		m_spGenState->IsXMLOutput = (BYTE)TRUE;
		m_spGenState->Validate = (BYTE)TRUE;
		m_spGenState->MsgId = (BYTE)'\xFE';

		trace(DCM_INFO, _T("CACSHook Object initialized Successfully...Firing DISCONNECTED State and waiting for Connect request from TB..."));
		//
		// Fire an event signaling that we are ready to connect to ACS.
		//
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED);
	}
	catch(_com_error & cex)
	{
		trace(DCM_ERROR) 
			<< _T("An exception occured while creating the ACS Hook Manager. Error: [0x") << std::hex << cex.Error() << std::dec
			<< _T("]: ") << cex.ErrorMessage()
			<< endl;
		m_spKey = NULL;
		m_spGenState = NULL;

		return E_FAIL;
	}

	return S_OK;
}

DWORD CACSHook::ApplicationMonitor()
{
	Trace trace(_T("CACSHook::ApplicationMonitor"));

	CString strApp = Options::OptionValue(ACS::OPT_APPLICATIONNAME, ACS::OPT_APPLICATIONNAME_DEF);
	trace(DCM_INFO) 
		<< _T("Starting Application monitoring for '") << strApp.GetString()
		<< _T("' process...") << endl;

	DWORD interimWait = 10000;

	HANDLE hDummy = CreateEvent( NULL, FALSE, FALSE, NULL );
	HANDLE hEvents[] = { m_hAppMonitorEvent, hDummy };
	DWORD eventCount = sizeof(hEvents) / sizeof(hEvents[0]);
    static bool bResetPOSAppFlag = true;

	while(TRUE)
	{
		DWORD dwRet = MsgWaitForMultipleObjects(eventCount, hEvents, FALSE, interimWait, QS_ALLINPUT);
		//+CRD185924
		CString csQuickRestart = m_Properties.Get(_T("IsQuickRestart"));
		trace(DCM_INFO) << _T("IsQuickRestart = ") <<  csQuickRestart.GetBuffer(0) << endl;
		if( csQuickRestart == _T("1") )
		{
           trace(DCM_INFO)
				<< _T("POS quick restart inprogress. Avoid terminating the monitoring thread for '") << strApp.GetString()
				<< _T("' process.") << endl;

		   if((WAIT_OBJECT_0 + 1) == dwRet && m_hPOSapp)
               bResetPOSAppFlag = false;
			
		   if(!bResetPOSAppFlag)
		   {
			  FireStateChange(_T("DISCONNECTED_QUICK_RESTART"));
		      m_hPOSapp = NULL;
			  bResetPOSAppFlag = true;
		   }
		   
		   if(!m_hPOSapp)
		       dwRet = WAIT_TIMEOUT;		
		} //-CRD185924
		if( WAIT_TIMEOUT == dwRet )
		{
			if (!m_hPOSapp)
			{
				DWORD dwPOSID = PSAPI::GetFirstMatchProcessId(strApp.GetString());
				m_hPOSapp = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | SYNCHRONIZE, FALSE, dwPOSID);
				if ( m_hPOSapp )
				{
					m_Properties.Set(_T("IsQuickRestart"), _T("0"));
					hEvents[1] = m_hPOSapp;
					interimWait = INFINITE;
				}
			}
		}
		else if( WAIT_OBJECT_0 == dwRet ) //m_hAppMonitorEvent - terminate
		{
			trace(DCM_INFO)
				<< _T("Terminating Application Monitoring thread for '") << strApp.GetString()
				<< _T("' process.") << endl;
			break;
		}
		else if( (WAIT_OBJECT_0 + 1) == dwRet ) // m_hPOSapp has closed
		{
			trace(DCM_INFO)
				<< _T("Application '") << strApp.GetString()
				<< _T("' has terminated - firing notification.") << endl;
			// process has stopped running, fire an unexpected end and return
			FireStateChange(CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED);
			break;
		}
		else if( (WAIT_OBJECT_0 + eventCount) == dwRet) 
		{
			MSG msg = { 0 };
			while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{ 
				if( msg.message == WM_QUIT )
				{
					trace(DCM_INFO, _T("Received a WM_QUIT Message, exiting Context dispatcher..."));
					break;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);			
			}
		}
		else if (dwRet == WAIT_FAILED)
		{
			trace(DCM_INFO, _T("Unexpected ApplicationMonitor WAIT_FAILED return code. Exiting monitor..."));
			_ASSERT(FALSE);
			break;
		}
		else
		{
			DWORD failIndex = dwRet - WAIT_ABANDONED_0;
			trace(DCM_INFO, _T("Unexpected ApplicationMonitor WAIT_xxx return code. Exiting monitor..."));
			_ASSERT(FALSE);
			break;
		}
	}

	CloseHandle( hDummy );

	trace(DCM_INFO)
		<< _T("Terminating Application Monitoring thread for '") << strApp.GetString()
		<< _T("' process.") << endl;
	return 0;
}



void CACSHook::FireEvent(long lEvent, long lData, LPCTSTR szData1, LPCTSTR szData2)
{
	/* POS15617: Do not rely on _bstr_t deallocation logic, but use SysAllocString/SysFreeString to ensure memory is freed properly.
	if( m_pfnCallbackProc )
	{
		m_pfnCallbackProc(lEvent, lData, (szData1 ? _bstr_t(szData1).copy() : _bstr_t(OLESTR("")).copy()), (szData2 ? _bstr_t(szData2).copy() : _bstr_t(OLESTR("")).copy()));
	}
	OnEvent(lEvent, lData, (szData1 ? _bstr_t(szData1) : _bstr_t(OLESTR(""))), (szData2 ? _bstr_t(szData2) : _bstr_t(OLESTR(""))));
	*/

	CT2W wideParam1 = szData1 ? szData1 : _T("");
	CT2W wideParam2 = szData2 ? szData2 : _T("");

	BSTR param1 = SysAllocString(wideParam1);
	BSTR param2 = SysAllocString(wideParam2);

	if( m_pfnCallbackProc )
	{
		m_pfnCallbackProc(lEvent, lData, param1, param2);
	}

	OnEvent(lEvent, lData, param1, param2);

	SysFreeString(param1);
	SysFreeString(param2);
}

void CACSHook::FireStateChange(const _bstr_t & bstrState, const _bstr_t & bstrGroup, const _bstr_t & bstrIgnoreDuplicate, const _bstr_t & bstrTriggerDelay)
{
	Trace trace(_T("CACSHook::FireStateChange"), false);

	CCriticalSectionLock lock(m_csGenState, TRUE);
	try
	{
		m_spGenState->SetValue(OLESTR("PosState"), bstrState);
		m_spGenState->SetValue(OLESTR("Group"), bstrGroup);
		m_spGenState->SetValue(OLESTR("IgnoreDuplicate"), bstrIgnoreDuplicate);
		m_spGenState->SetValue(OLESTR("TriggerDelay"), bstrTriggerDelay);

		m_strCurrentState = (LPCTSTR)bstrState;

		if (Trace::IsLevel(DCM_DEBUG))
		{
			trace(DCM_DEBUG) 
				<< _T("state: \"") << (bstrState.length() ? (LPCTSTR)bstrState : _T("<null>"))
				<< _T("\", group: \"") << (bstrGroup.length() ? (LPCTSTR)bstrGroup : _T("<null>"))
				<< _T("\", ignore duplicate: \"") << (bstrIgnoreDuplicate.length() ? (LPCTSTR)bstrIgnoreDuplicate : _T("<null>"))
				<< _T("\", trigger delay: \"") << (bstrTriggerDelay.length() ? (LPCTSTR)bstrTriggerDelay : _T("<null>"))
				<< _T("\"") << endl;
		}

		FireEvent(ACSHookEvent::OnGenMsgEvent, 0, (LPCTSTR)m_spGenState->xml);
	}
	catch(_com_error & cex)
	{
		trace(DCM_ERROR) << _T("Failed to fire a state change for '") << (LPCTSTR)bstrState << _T("'. tbControls threw an exception: [0x") << std::hex << cex.Error() << std::dec << _T("]: ") << cex.ErrorMessage() << NCR::Util::endl;
	}
}

void CACSHook::FinalRelease() 
{
	Trace trace(_T("CACSHook::FinalRelease"));

	try
	{
		trace(DCM_INFO, _T("+CACSHook::FinalRelease"));
		DataKit::UnloadContextStructures();
		DataKit::UnloadSequenceStructures();
		DataKit::Uninitialize();
		
		Disconnect();

		m_spKey = NULL;
		m_spGenState = NULL;
		trace(DCM_INFO, _T("-CACSHook::FinalRelease"));
	}
	catch(_com_error & cex)
	{
		trace(DCM_ERROR) 
			<< _T("FinalRelease exception. Error [0x") << std::hex << cex.Error() << std::dec << cex.ErrorMessage() << endl;
	}
}

void CACSHook::SafeSendData(CString const& strData)
{
	Trace trace(_T("CACSHook::SafeSendData"));

	CCriticalSectionLock lock(m_csDataSend, TRUE);

	m_dwLastIO = GetTickCount();

	CT2A szData(strData.GetString());
	for(int i = 0; i < strData.GetLength(); i++)
	{
		trace(DCM_INFO)
			<< _T("SafeSendData: Sending character 0x") << (unsigned int)szData[i] << _T(" to keyboard.") << endl;
		m_spKey->KeyCode= (BYTE)szData[i];
		m_spKey->KeyEvent= TBC::KEY_UP;
		TBC::ITBStreamPtr spStream = m_spKey;
		m_spTBConnect->Send(spStream);
	}
}


STDMETHODIMP CACSHook::Connect(void)
{
	Trace trace(_T("CACSHook::Connect"));
	trace(DCM_INFO,  _T("+CACSHook::Connect()"));

	trace(DCM_INFO,  _T("Getting ACSExposedFunctions DLL name..."));
	CString strDLL = Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF);

	trace(DCM_INFO,  _T("Loading the ACSExposedFunctions DLL..."));
	if (!ACSFuncs::Load(strDLL))
	{
		trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions DLL."));
		return E_FAIL;
	}

	/* Initialize Context Channel */
	trace(DCM_INFO, _T("Initializing ACSIOClient Context Channel..."));
	HRESULT hr = ACSIOInitialize(ACSIO_SINGLETHREADED, &m_hClient);
	trace(DCM_INFO)
		<< _T("   m_hClient = ") << m_hClient << endl;
	if( (hr != RPC_E_CHANGED_MODE) && FAILED(hr) )
	{
		trace(DCM_ERROR)
			<< _T("ACSIOInitialize failed with error 0x") << std::hex << hr << std::dec 
			<< _T(": ") << ACSIOClient::GetErrorText(m_hClient) << endl;
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED);
		return hr;
	}


	/* Initialize Sequence Channel */
	trace(DCM_INFO, _T("Initializing ACSIOClient Sequence Channel..."));
	hr = ACSIOInitialize(ACSIO_SINGLETHREADED, &m_hSeqClient);
	if( (hr != RPC_E_CHANGED_MODE) && FAILED(hr) )
	{
		trace(DCM_ERROR) 
			<< _T("ACSIOInitialize failed with error 0x") << std::hex << hr << std::dec 
			<< _T(": ") << ACSIOClient::GetErrorText(m_hSeqClient) << endl;
		ACSIOUninitialize(m_hClient);
		m_hClient = NULL;
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED);
		return hr;
	}

	//
	// Initialize Auxiliary Channel
	//
	trace(DCM_INFO, _T("Initializing ACSIOClient Auxiliary Channel..."));
	hr = ACSIOInitialize(ACSIO_SINGLETHREADED, &m_hAuxClient);
	if( (hr != RPC_E_CHANGED_MODE) && FAILED(hr) )
	{
		trace(DCM_ERROR)
			<< _T("ACSIOInitialize failed with error 0x") << std::hex << hr << std::dec << _T(": ") 
			<< ACSIOClient::GetErrorText(m_hAuxClient) << endl;
		ACSIOUninitialize(m_hClient);
		ACSIOUninitialize(m_hSeqClient);
		m_hClient = NULL;
		m_hSeqClient = NULL;
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED);
		return hr;
	}


	//
	// Connect to the context channel
	//
	trace(DCM_INFO, _T("Connecting to ACSIOClient Context Channel..."));
	hr = ACSIOConnectContextChannel(m_hClient);
	if( FAILED(hr) )
	{
		trace(DCM_ERROR)
			<< _T("ACSIOConnectContextChannel failed to connect to the context channel. Error 0x") 
			<< std::hex << hr << std::dec << _T(": ") << ACSIOClient::GetErrorText(m_hClient) << endl;
		ACSIOUninitialize(m_hClient);
		ACSIOUninitialize(m_hSeqClient);
		ACSIOUninitialize(m_hAuxClient);
		m_hClient = NULL;
		m_hSeqClient = NULL;
		m_hAuxClient = NULL;
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED);
		return hr;
	}

	/* start SOTF8062 */
   int queueEntries = Options::LongVal(Options::OptionValue(ACS::OPT_ACSIO_QUEUE_ENTRIES, ACS::OPT_ACSIO_QUEUE_ENTRIES_DEF));

	if( queueEntries > 0 )
	{
		hr = ACSIOSetMaxQueueEntries(m_hClient, queueEntries);
	}
	else
	{
		trace(DCM_ERROR, _T("Failed to retrieve a valid value for ACSIO queue entries. Value: '%d'"), queueEntries);
	}
	/* end SOTF8062 */

	//
	// Connect to the sequence channel
	//
	trace(DCM_INFO, _T("Connecting to ACSIOClient Sequence Channel..."));
	hr = ACSIOConnectSequenceChannel(m_hSeqClient);
	if( FAILED(hr) )
	{
		trace(DCM_ERROR)
			<< _T("ACSIOConnectSequenceChannel failed to connect to the sequence channel. Error 0x") 
			<< std::hex << hr << std::dec 
			<< _T(": ") << ACSIOClient::GetErrorText(m_hSeqClient) << endl;
		ACSIOUninitialize(m_hClient);
		ACSIOUninitialize(m_hSeqClient);
		ACSIOUninitialize(m_hAuxClient);
		m_hClient = NULL;
		m_hSeqClient = NULL;
		m_hAuxClient = NULL;
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED);
		return hr;
	}

	/* start SOTF8062 */
	if( queueEntries > 0 )
	{
		hr = ACSIOSetMaxQueueEntries(m_hSeqClient, queueEntries);
	}
	else
	{
		trace(DCM_ERROR, _T("Failed to retrieve a valid value for ACSIO queue entries. Value: '%d'"), queueEntries);
	}
	/* end SOTF8062 */

	//
	// Connect to the auxiliary channel
	//
	trace(DCM_INFO, _T("Connecting to ACSIOClient Auxiliary Channel..."));
	hr = ACSIOConnectAuxiliaryChannel(m_hAuxClient);
	if( FAILED(hr) )
	{
		trace(DCM_ERROR)
			<< _T("ACSIOConnectAuxiliaryChannel failed to connect to the auxiliary channel. Error 0x")
			<< std::hex << hr << std::dec 
			<< _T(": ") << ACSIOClient::GetErrorText(m_hAuxClient) << endl;
		ACSIOUninitialize(m_hClient);
		ACSIOUninitialize(m_hSeqClient);
		ACSIOUninitialize(m_hAuxClient);
		m_hClient = NULL;
		m_hSeqClient = NULL;
		m_hAuxClient = NULL;
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED);
		return hr;
	}

	//
	// Setup context reception event and begin listening to events
	// 
	trace(DCM_INFO, _T("Setup and begin listening for ACSIOClient Contexts..."));

	hr = ACSIOAdviseEx(m_hClient);

	if (FAILED(hr))
	{
		trace(DCM_ERROR)
			<< _T("ACSIOAdviseEx failed with error 0x") 
			<< std::hex << hr << std::dec << _T(": ") << ACSIOClient::GetErrorText(m_hClient) << endl;

		ACSIOUninitialize(m_hClient);
		ACSIOUninitialize(m_hSeqClient);
		ACSIOUninitialize(m_hAuxClient);
		m_hClient = NULL;
		m_hSeqClient = NULL;
		m_hAuxClient = NULL;
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED);
		return hr;
	}

	//
	// connect to pipeserver
	//
	trace(DCM_INFO, _T("Connecting to PipeServer..."));
	try
	{
		// Set the callback for events
		_variant_t vtCallback;
		vtCallback.ullVal = (ULONGLONG)TBConnectCallbackProc;
		vtCallback.vt = VT_UI8;
		m_spTBConnect->SetCallback(vtCallback);

		m_spTBConnect->SubscribeTo(Options::OptionValue(ACS::OPT_OPOS_LISTENTODEVICES, ACS::OPT_OPOS_LISTENTODEVICES_DEF).GetString());
		m_spTBConnect->Connect();
	}
	catch(_com_error & cex)
	{
		trace(DCM_ERROR)
			<< _T("Failed to connect to pipeserver. Error [0x") << 
			std::hex << cex.Error() << std::dec << _T("]: ") << cex.ErrorMessage() << endl;
		
		ACSIOUnadviseEx(m_hClient);

		ACSIOUninitialize(m_hClient);
		ACSIOUninitialize(m_hSeqClient);
		ACSIOUninitialize(m_hAuxClient);
		m_hClient = NULL;
		m_hSeqClient = NULL;
		m_hAuxClient = NULL;
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED_UNEXPECTED);
		return cex.Error();
	}

	//
	// Create the Context Listener, Dispatcher and Processor & disconnect event
	// Create the Application process watchdog
	//
	trace(DCM_INFO, _T("Creating the Context Listener, Dispatcher and Processor & disconnect event..."));
	m_hDisconnectEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	_ASSERT(m_hDisconnectEvent);

	m_hReaderThread = CreateThread(NULL, 0, ReadContextsProc, (PVOID)this, 0, NULL);
	_ASSERT(m_hReaderThread);

	m_hAppMonitorEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	_ASSERT(m_hAppMonitorEvent);

	m_hAppMonitorThread = CreateThread(NULL, 0, ApplicationMonitorProc, (PVOID)this, 0, NULL);
	_ASSERT(m_hAppMonitorThread);

	// 
	// Fire connection event
	//
	trace(DCM_INFO, _T("Firing OnConnectEvent..."));
	FireEvent(ACSHookEvent::OnConnectEvent, 0);

	trace(DCM_INFO, _T("Firing Connected State Change..."));
	FireStateChange(CAppModel::XML_STATE_CONNECTED);

	trace(DCM_INFO, _T("-CACSHook::Connect() - Completed successfully"));
	return S_OK;
}

void CACSHook::TerminateApplicationMonitor(void)
{
	Trace trace(_T("CACSHook::TerminateApplicationMonitor"));

	// fire the application monitor event so it can stop monitoring
	if( m_hAppMonitorEvent )
	{
		SetEvent(m_hAppMonitorEvent);
	}

	if( m_hAppMonitorThread )
	{
		DWORD dwCode = 0;
		const DWORD dwTimeout = 2000;
		DWORD dwTime = GetTickCount();
		do
		{
			trace(DCM_INFO, _T("Waiting for the ACS Application Monitor thread to complete..."));
			GetExitCodeThread(m_hAppMonitorThread, &dwCode);
			Sleep(100);
		}
		while( STILL_ACTIVE == dwCode && ((GetTickCount() - dwTime) < dwTimeout) );

		// If thread is still active, kill it
		if( STILL_ACTIVE == dwCode )
		{
			trace(DCM_WARNING)
				<< _T("ACS Application Monitor thread did not die within ") 
				<< dwTimeout << _T(" ms as expected. TerminateThread is being called.") << endl;
			TerminateThread(m_hAppMonitorThread,-1);
		}
		CloseHandle(m_hAppMonitorThread);
		m_hAppMonitorThread = NULL;
	}
	if( m_hAppMonitorEvent )
	{
		CloseHandle(m_hAppMonitorEvent);
		m_hAppMonitorEvent = NULL;
	}
	if (m_hPOSapp)
		CloseHandle(m_hPOSapp);
	m_hPOSapp = NULL;
}

void CACSHook::TerminateContextListenerProcessor(void)
{
	Trace trace(_T("CACSHook::TerminateContextListenerProcessor"));

	trace(DCM_INFO, _T("+CACSHook::TerminateContextListenerProcessor"));
	// Wait up to a 2 seconds for Thread completion
	// if it does not complete gracefully in 2 seconds then
	// kill it by force and continue
	//
	if (m_hReaderThread)
	{
		DWORD dwCode = 0;
		const DWORD dwTimeout = 2000;

		trace(DCM_INFO, _T("Waiting for the ACS Context Listener & Processor thread to complete..."));
		DWORD dwWait = WaitForSingleObject(m_hReaderThread, dwTimeout);
		GetExitCodeThread(m_hReaderThread, &dwCode);

		// If thread is still active, kill it
		if( STILL_ACTIVE == dwCode )
		{
			trace(DCM_WARNING)
				<< _T("ACS Context Listener & Processor thread did not die within ")
				<< dwTimeout << _T(" ms as expected. TerminateThread is being called.") << endl;
			TerminateThread(m_hReaderThread, -1);
		}
		CloseHandle(m_hReaderThread);
		m_hReaderThread = NULL;
	}

	if( m_hDisconnectEvent )
	{
		CloseHandle(m_hDisconnectEvent);
		m_hDisconnectEvent = NULL;
	}
	trace(DCM_INFO, _T("-CACSHook::TerminateContextListenerProcessor"));
}

STDMETHODIMP CACSHook::Disconnect(void)
{
	Trace trace(_T("CACSHook::Disconnect"));
	trace(DCM_INFO, _T("+CACSHook::Disconnect"));

	trace(DCM_INFO, _T("Terminating Application Monitoring thread..."));
	TerminateApplicationMonitor();	

	//
	// Check if we're already connected 
	//
	if( !m_hClient && !m_hSeqClient && !m_hAuxClient )
	{	
		trace(DCM_WARNING, _T("We are already disconnected from ACSIO.  Ignoring request."));
		return S_FALSE;
	}

	trace(DCM_INFO, _T("Disconnecting from ACSIO Server..."));
	HRESULT hrRet = S_OK;

	if( m_hClient )
	{
		ACSIOUnadviseEx(m_hClient);
		ACSIOUninitialize(m_hClient);
		m_hClient = NULL;
		trace(DCM_INFO,  _T("Disconnect: m_hClient == NULL"));
	}

	if( m_hSeqClient )
	{
		ACSIOUninitialize(m_hSeqClient);
		m_hSeqClient = NULL;
		trace(DCM_INFO, _T("Disconnect: m_hSeqClient == NULL"));
	}

	if( m_hAuxClient )
	{
		ACSIOUninitialize(m_hAuxClient);
		m_hAuxClient = NULL;
		trace(DCM_INFO, _T("Disconnect: m_hAuxClient == NULL"));

	}


	// fire a disconnect request
	if( m_hDisconnectEvent )
	{
		SetEvent(m_hDisconnectEvent);
	}

	trace(DCM_INFO, _T("Terminating Context Listener & Processor thread..."));
	TerminateContextListenerProcessor();

	try
	{
		trace(DCM_INFO, _T("Disconnecting from PipeServer..."));
		m_spTBConnect->Disconnect();
		_variant_t vtCallback;
		vtCallback.ulVal= 0;
		vtCallback.vt = VT_UI4;
		m_spTBConnect->SetCallback(vtCallback);
	}
	catch(_com_error & cex)
	{
		trace(DCM_ERROR)
			<< _T("Failed to disconnect from pipeserver. Error [0x")
			<< std::hex << cex.Error() << _T("]: ") << cex.ErrorMessage() << endl;
		hrRet = cex.Error();
	}

	FireEvent(ACSHookEvent::OnDisconnectEvent, 0);

	FireStateChange(CAppModel::XML_STATE_DISCONNECTED);

	trace(DCM_INFO, _T("-CACSHook::Disconnect"));
	return hrRet;
}

STDMETHODIMP CACSHook::GetProperty(BSTR strName, BSTR* pVal)
{
	if( !strName || !pVal ) return E_POINTER;

	*pVal = _bstr_t(m_Properties.Get(strName)).copy();

	return S_OK;
}

STDMETHODIMP CACSHook::SetProperty(BSTR strName, BSTR newVal)
{
	Trace trace(_T("CACSHook::SetProperty"));

	if( !strName || !newVal ) return E_POINTER;

	COLE2T szName(strName);
	COLE2T szVal(newVal);

	trace(DCM_INFO)
		<< _T("Setting property '") << (TCHAR*)szName
		<< _T("' to value '") << (TCHAR*)szVal
		<< _T("'.") << endl;

	BOOL bRet = m_Properties.Set(szName, szVal);

	return bRet ? S_OK : E_FAIL;
}


HRESULT CACSHook::InternalSendInput(BSTR strInputName)
{
	Trace trace(_T("CACSHook::InternalSendInput"));

	if( !strInputName ) return E_POINTER;

	_bstr_t tmp = strInputName;
	NCR::Util::tstring inputName = (LPCTSTR)tmp;

	//
	// Is it input text? If so, then send an OPOS keyboard press
	//
	if( m_appModel.IsInputText(inputName) )
	{
		LPCTSTR ptext = m_appModel.GetInputTextData(inputName);

		if (ptext)
		{
			trace(DCM_INFO) << _T("SendInput Text: '") << inputName << _T("'") << endl;
			SafeSendData(ptext);
		}
		else
		{
			trace(DCM_ERROR) << _T("SendInput Text: '") << inputName << _T("' - This is not a valid Application Input.") << endl;
		}
	}
	//
	// Is it an input sequence?  If so, then use the ACSIO Sequence channel to send the data
	//
	else if (m_appModel.IsInputSequence(inputName))
	{
		/* The "_ENTERED" suffix is not accepted by the new ACSIODataKit 
		interface, but some unmodified ApplicationModel.XML documents may 
		be lurking in the wild. Remove the suffix if found. */
		static TCHAR entered[] = _T("_ENTERED");
		static const int enteredLen = sizeof(entered) / sizeof(entered[0]);

		CString sInputName = strInputName;
		int suffixPos = sInputName.Find(entered, sInputName.GetLength() - enteredLen);

		if (suffixPos >= 0)
		{
			sInputName = sInputName.Left(suffixPos);
		}


		CStringA ascInputName = CT2CA(sInputName);
		DataKit::SequenceBuffer sequence(ascInputName);

		trace(DCM_INFO) 
			<< _T("SendInput Sequence: '") << inputName.c_str()
			<< _T("'") << endl;

		BOOL bRet = m_appModel.PopulateInputSequenceData(inputName, sequence);

		if( bRet )
		{
			int iId = sequence.ID();
			trace(DCM_DEBUG, _T("Send Input with id '%d' through ACSIO Sequence channel."), iId);

			HRESULT hr = ACSIOSendSequence(
				m_hSeqClient, 
				iId, 
				sequence);

			if( FAILED(hr) )
			{
				trace(DCM_ERROR) 
					<< _T("SendInput: ACSIOSendSequence call failed. Error [0x") 
					<< std::hex << hr << std::dec << _T("]: ") 
					<< ACSIOClient::GetErrorText(m_hSeqClient).GetString() << endl;
			}
		}
		else
		{
			trace(DCM_ERROR)
				<< _T("SendInput: '") << COLE2T(strInputName)
				<< _T("' - Failed to Create the Input Sequence data block.") << endl;
		}
	}
	else 
	{
		trace(DCM_ERROR)
			<< _T("SendInput: '") << COLE2T(strInputName)
			<< _T("' - This is neither a Text or Sequence Application Input.") << endl;
	}

	return S_OK;
}

STDMETHODIMP CACSHook::SendSensitiveRawData(BSTR strData)
{
	return SendRawData(strData);
}


STDMETHODIMP CACSHook::SendRawData(BSTR strData)
{
	Trace trace(_T("CACSHook::SendRawData"));

	if( !strData ) return E_POINTER;

	CString strKeys = COLE2T(strData);

	trace(DCM_INFO)
		<< _T("SendRawData Sequence: '") << strKeys.GetString()
		<< _T("'") << endl;

	for(int i = 0; i < strKeys.GetLength(); i++)
	{
		HRESULT hr = InternalSendInput(_bstr_t(strKeys.Mid(i, 1).GetString()));
		if( FAILED(hr) )
		{
			return hr;
		}
	}

	return S_OK;
}

STDMETHODIMP CACSHook::SendInput(BSTR strInputName) 
{
	return InternalSendInput(strInputName);
}

STDMETHODIMP CACSHook::SendAuxMsg(BSTR strXmlData, long lNotifyId)
{
	Trace trace(_T("CACSHook::SendAuxMsg"));

	if( !strXmlData ) return E_POINTER;

	// wrap the message in the AuxMsg envelope
	CString auxMsg;
	HRESULT hr;

	// wrap the sequence number every 1000 messages
	m_lAuxMsgSeq = (m_lAuxMsgSeq + 1) % 1000;
	auxMsg.Format(_T("<AuxMsg MsgSeq=\"%d\">\n%s\n</AuxMsg>"), m_lAuxMsgSeq, (LPCTSTR)strXmlData);

	// Send the msg to the auxiliary channel
	int iBytes = (auxMsg.GetLength() + 1) * sizeof(TCHAR);  // an extra character for NULL terminator
	trace(DCM_INFO)
		<< _T("Sending AuxMsg data (") << iBytes
		<< _T(" bytes)...") << endl;

	ACSIO_AUXMSG_HEADER header = { sizeof(ACSIO_AUXMSG_HEADER) };
	header.payloadID = lNotifyId;
	header.payloadSize = iBytes;

	std::vector<BYTE> buffer (sizeof(header) + iBytes);
	memcpy(&buffer[0], &header, sizeof(header));
	memcpy(&buffer[sizeof(header)], (LPBYTE)auxMsg.GetString(), auxMsg.GetLength() * sizeof(TCHAR));

	hr = ACSIOSendRaw(
		m_hAuxClient, 
		sizeof(header) + iBytes, 
		&buffer[0]);

	if (FAILED(hr) )
	{
		trace(DCM_ERROR)
			<< _T("SendAuxMsg: ACSIOSendRaw call failed. Error [0x")
			<< std::hex << hr << std::dec << _T(": ") << ACSIOClient::GetErrorText(m_hAuxClient).GetString()
			<< endl;
	}
	return S_OK;
}


STDMETHODIMP CACSHook::SetCallback(VARIANT vCallbackProc)
{
	_ASSERT( (V_VT(&vCallbackProc) & VT_UI4) || (V_VT(&vCallbackProc) & VT_UI8) );

	BOOL bFirst = (m_pfnCallbackProc == NULL);

	m_pfnCallbackProc = (LPACSHOOKCALLBACKPROC)vCallbackProc.ullVal;
	if( bFirst )
	{
		FireStateChange(CAppModel::XML_STATE_DISCONNECTED);
	}

	return S_OK;
}

STDMETHODIMP CACSHook::SendControlMessage(VARIANT vArray)
{
	Trace trace(_T("CACSHook::SendControlMessage"));

	if( (vArray.vt != (VT_ARRAY | VT_UI1)) || (vArray.parray == NULL) ) return E_INVALIDARG;

	CCriticalSectionLock lock(m_csDataSend, TRUE);
	trace(DCM_INFO, _T("Sending Control Message from TB to POS"));

	try
	{
		TBC::ITBStreamPtr spTBObj;
		TBC::ITBGenObjPtr spGen;
		HRESULT hr= spGen.CreateInstance(__uuidof(TBC::CTBGenObj));
		if(FAILED(hr))
		{
			trace(DCM_ERROR,  _T("Failed to create ITBGenObj"));
			return E_FAIL;
		}
        
        // function will return bytes in array - supports base64, hexstring pairs and byte array
        std::vector<BYTE> data = GetBytesFromVariant(vArray);
        trace(DCM_INFO, _T("Dump of serial data being sent to POS using tbControls:"));
        Utils.HexTrace(DCM_INFO, &data[0], (long)data.size());

		spTBObj= spGen;
		spTBObj->ParseStream(vArray);

		TBC::ITBStreamPtr spStream = spTBObj;
		m_spTBConnect->Send(spStream);
		m_dwLastIO = GetTickCount();
		return S_OK;
	}
	catch(_com_error & cex)
	{
		trace(DCM_ERROR)
			<< _T("SendControlMessage failed to send the control message. Error [0x")
			<< std::hex << cex.Error() << std::dec << _T("]: ") << cex.ErrorMessage() << endl;
		return E_NOTIMPL;
	}
}

STDMETHODIMP CACSHook::ValidateOperator(BSTR strLogin, BSTR strPassword, VARIANT_BOOL * pVal)
{
	Trace trace(_T("CACSHook::ValidateOperator"));

	if( !strLogin || !strPassword || !pVal ) return E_POINTER;

	if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));
		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}

	BOOL bRet = ACSFuncs::ValidateOperator(COLE2T(strLogin), COLE2T(strPassword));
	*pVal = bRet ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CACSHook::ValidateSupervisor(BSTR strLogin, BSTR strPassword, VARIANT_BOOL * pVal)
{
	Trace trace(_T("CACSHook::ValidateSupervisor"));

	if( !strLogin || !strPassword || !pVal ) return E_POINTER;

	if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));

		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}

	BOOL bRet = ACSFuncs::ValidateSupervisor(COLE2T(strLogin), COLE2T(strPassword));
	*pVal = bRet ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

// POS81632
STDMETHODIMP CACSHook::CheckInOutOperator(int nFunc, BSTR strLogin, BSTR strPassword, int nNewRAPid, int * currentRAPid, int * offlineStatus, int * pVal)
{
	Trace trace(_T("CACSHook::CheckInOutOperator"));

	if( !strLogin || !strPassword || !pVal ) return E_POINTER;

	if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));
		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}

	int nRet = ACSFuncs::CheckInOutOperator(nFunc, COLE2T(strLogin), COLE2T(strPassword), nNewRAPid, currentRAPid, offlineStatus);
	*pVal = nRet;
	return S_OK;
}

// POS97215
STDMETHODIMP CACSHook::GetOperatorLastPwdChgTime(BSTR strID, int * timeYear, int * timeMonth, int * timeDay, int * timeHour, int * timeMinute, int * timeSecond, int * pVal)
{
	Trace trace(_T("CACSHook::GetOperatorLastPwdChgTime"));

	if( !strID || !pVal ) return E_POINTER;

	if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));
		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}

	int nRet = ACSFuncs::GetOperatorLastPwdChgTime(COLE2T(strID), timeYear, timeMonth, timeDay, timeHour, timeMinute, timeSecond);
	*pVal = nRet;
	return S_OK;
}


STDMETHODIMP CACSHook::ExValidateEmployeeOperatorString(BSTR strID, BSTR strPassword, ExEntryFlag entryFlag, VARIANT_BOOL * pVal)
{
	Trace trace(_T("CACSHook::ExValidateEmployeeOperatorString"));

	if( !strID || !strPassword || !pVal ) return E_POINTER;

	if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));

		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}

    BOOL bRet = ACSFuncs::ExValidateEmployeeOperatorString(COLE2T(strID), COLE2T(strPassword), (ACSFuncs::ACSExEntryFlag)entryFlag);
	*pVal = bRet ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CACSHook::ExValidateEmployeeSupervisorString(BSTR strID, BSTR strPassword, ExEntryFlag entryFlag, VARIANT_BOOL * pVal)
{
	Trace trace(_T("CACSHook::ExValidateEmployeeSupervisorString"));

	if( !strID || !strPassword || !pVal ) return E_POINTER;

	if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));

		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}

    BOOL bRet = ACSFuncs::ExValidateEmployeeSupervisorString(COLE2T(strID), COLE2T(strPassword), (ACSFuncs::ACSExEntryFlag)entryFlag);
	*pVal = bRet ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CACSHook::ExValidateEmployeeOperatorData(BSTR strID, VARIANT vData, ExEntryFlag entryFlag, VARIANT_BOOL * pVal)
{
	Trace trace(_T("CACSHook::ExValidateEmployeeOperatorData"));

	if( !strID || vData.vt == VT_EMPTY || !pVal ) return E_POINTER;

	if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));

		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}

    // Convert variant contents to biometric data
    std::vector<BYTE> data = GetBytesFromVariant(vData);

    BOOL bRet = ACSFuncs::ExValidateEmployeeOperatorData(COLE2T(strID), &data[0], (long)data.size(), (ACSFuncs::ACSExEntryFlag)entryFlag);
	*pVal = bRet ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CACSHook::ExValidateEmployeeSupervisorData(BSTR strID, VARIANT vData, ExEntryFlag entryFlag, VARIANT_BOOL * pVal)
{
	Trace trace(_T("CACSHook::ExValidateEmployeeSupervisorData"));

	if( !strID || vData.vt == VT_EMPTY || !pVal ) return E_POINTER;

	if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));

		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}

    // Convert variant contents to biometric data
    std::vector<BYTE> data = GetBytesFromVariant(vData);
   
    BOOL bRet = ACSFuncs::ExValidateEmployeeSupervisorData(COLE2T(strID), &data[0], (long)data.size(), (ACSFuncs::ACSExEntryFlag)entryFlag);
	*pVal = bRet ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}


std::vector<BYTE> CACSHook::GetBytesFromVariant(VARIANT & vData)
{
    Trace trace(_T("CACSHook::GetBytesFromVariant"));
    std::vector<BYTE> data = std::vector<BYTE>();
    
    // Variant may contain a byte array, a hex encoded string or a base64 encoded string - all should be supoorted.

    if( vData.vt == (VT_ARRAY | VT_UI1) )
    {
        trace(DCM_INFO, _T("Byte array received."));
        
        // assume one dimensional SAFEARRAY
        CComSafeArray<BYTE> sa(V_ARRAY(&vData));
        for(int i = sa.GetLowerBound(); i <= sa.GetUpperBound(); i++)
        {
            data.push_back(sa[i]);
        }
    }
    else if( vData.vt == VT_BSTR )
    {
        trace(DCM_INFO)
			  << _T("String received: ") << (LPCTSTR)_bstr_t(vData) << endl;
        _variant_t vByteData;
        try
        {
            // Decode Base64 encoded string and package into a byte array
            MSXML2::IXMLDOMDocumentPtr spDoc(__uuidof(MSXML2::FreeThreadedDOMDocument60));
            MSXML2::IXMLDOMElementPtr spElem = spDoc->createElement(_T("b64"));
            spDoc->appendChild(spElem);
            spElem->PutdataType(_T("bin.base64"));
            spElem->text = _bstr_t(vData);
            vByteData = spElem->nodeTypedValue;
        }
        catch(_com_error & cex)
        {
            trace(DCM_ERROR)
					<< _T("An error occurred while processing the biometric string: [0x")
					<< std::hex << cex.Error() << std::dec << _T("] ") << cex.ErrorMessage() << endl;
        }
        // assume one dimensional SAFEARRAY
        CComSafeArray<BYTE> sa(V_ARRAY(&vByteData));
        for(int i = sa.GetLowerBound(); i <= sa.GetUpperBound(); i++)
        {
            data.push_back(sa[i]);
        }
    }
  
    return data;
}

bool CACSHook::SendControlToApp(LPCTSTR pszControl)
{
	Trace trace(_T("CACSHook::SendControlToApp"));
	trace(DCM_INFO)
		<< _T("Sending ") << pszControl
		<< _T(" control to Application...") << endl;
	CCriticalSectionLock lock(m_csGenControl, TRUE);
	bool bRet = false;

	// fill the gen object with the control information
	PCONTROL pCtrl = m_Controls.GetControl(pszControl);
	if( pCtrl )
	{
		try
		{
			pCtrl->spGenCtrl->Reset(TBC::RESET_FIELD_DATA);
			// setup the message header info
			POSITION pos = pCtrl->fieldMap.GetStartPosition();
			PCONTROLFIELDMAPITEM pcfmi = NULL;
			while( pos && (pcfmi = pCtrl->fieldMap.GetNext(pos)) )
			{
				CString strName = pcfmi->m_key;
				CString strValue = pcfmi->m_value.strLastSavedValue;

				if (trace.IsLevel(DCM_DEBUG))
				{
					trace(DCM_DEBUG)
						<< _T("GenControl SetValue('") << strName.GetString()
						<< _T("', '") << strValue.GetString()
						<< _T("')") << endl;
				}

				pCtrl->spGenCtrl->SetValue(strName.GetString(), strValue.GetString());
			}

			TBC::ITBStreamPtr spStream = pCtrl->spGenCtrl;
			m_spTBConnect->Send(spStream);
			m_dwLastIO = GetTickCount();
			bRet = true;
		}
		catch(_com_error & cex)
		{
			trace(DCM_ERROR)
				<< _T("SendControlToApp(") << pszControl
				<< _T(") failed. tbControls threw an exception: Error [0x")
				<< std::hex << cex.Error() << std::dec << _T("]: ") << cex.ErrorMessage() << endl;
			bRet = false;
		}
	}

	return bRet;
}


bool CACSHook::SendControlToTB(LPCTSTR pszControl)
{
	Trace trace(_T("CACSHook::SendControlToTB"));
	trace(DCM_INFO) 
		<< _T("Sending ") << pszControl
		<< _T(" control to TB...") << endl;
	CCriticalSectionLock lock(m_csGenControl, TRUE);
	bool bRet = false;

	// fill the gen object with the control information
	PCONTROL pCtrl = m_Controls.GetControl(pszControl);
	if( pCtrl )
	{
		try
		{
			pCtrl->spGenCtrl->Reset(TBC::RESET_FIELD_DATA);
			// setup the message header info
			POSITION pos = pCtrl->fieldMap.GetStartPosition();
			PCONTROLFIELDMAPITEM pcfmi = NULL;
			while( pos && (pcfmi = pCtrl->fieldMap.GetNext(pos)) )
			{
				CString strName = pcfmi->m_key;
				CString strValue = pcfmi->m_value.strLastSavedValue;
				if (trace.IsLevel(DCM_DEBUG))
				{
					trace(DCM_DEBUG)
						<< _T("GenControl SetValue('") << strName.GetString()
						<< _T("', '") << strValue.GetString()
						<< _T("')") << endl;
				}
				pCtrl->spGenCtrl->SetValue(strName.GetString(), strValue.GetString());
			}
			FireEvent(ACSHookEvent::OnGenMsgEvent, 0, (LPCTSTR)pCtrl->spGenCtrl->xml);
			bRet = true;
		}
		catch(_com_error & cex)
		{
			trace(DCM_ERROR)
				<< _T("SendControlToTB(") << pszControl
				<< _T(") failed. tbControls threw an exception: Error [0x")
				<< std::hex << cex.Error() << _T("]: ") << cex.ErrorMessage() << endl;
			bRet = false;
		}
	}

	return bRet;
}


STDMETHODIMP CACSHook::ExGetTerminalBusinessDay(long lTerminalID, long * businessDay, VARIANT_BOOL * pVal)
{
	Trace trace(_T("CACSHook::GetTerminalBusinessDay"));


	if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));

		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}
 
    BOOL bRet = ACSFuncs::ExGetTerminalBusinessDay(lTerminalID, businessDay);
	*pVal = bRet ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CACSHook::ExGetEmplidFromBadge(BSTR strBadge, BSTR* pstrEmplid, VARIANT_BOOL * pVal)
{
    Trace trace(_T("CACSHook::ExGetEmplidFromBadge"));
	
    if( !ACSFuncs::IsLoaded() )
	{
		//
		// Load the exposed functions library
		//
		BOOL bRet = ACSFuncs::Load(Options::OptionValue(ACS::OPT_ACSEXFUNCSDLL, ACS::OPT_ACSEXFUNCSDLL_DEF));

		if( !bRet )
		{
			trace(DCM_ERROR, _T("Failed to initialize the ACSExposedFunctions Client DLL."));
			return E_NOINTERFACE;
		}
	}
 
    TCHAR szEmplid[MAX_PATH] = {0};
    
    BOOL bRet = ACSFuncs::ExGetEmplidFromBadge(COLE2T(strBadge),szEmplid);
    
    if( *pstrEmplid )
    {
        ::SysFreeString(*pstrEmplid); // deallocate if applicable to avoid mem leaks
    }

    if( bRet )
    {
        *pstrEmplid = ::SysAllocString(CT2OLE(szEmplid));
    }
    else
    {
        *pstrEmplid = ::SysAllocString(OLESTR(""));
    }

	*pVal = bRet ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;

}
