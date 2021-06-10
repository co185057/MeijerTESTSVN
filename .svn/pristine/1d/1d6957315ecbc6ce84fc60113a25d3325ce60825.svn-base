// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCManager.cpp 3     10/21/08 4:09p Sm133076 $
/*
 * @(#)RCManager.cpp    $Revision: 3 $ $Date: 10/21/08 4:09p $
 *
 * Copyright 2004 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/remote-console-manager/RCManager.cpp $
 * 
 * 3     10/21/08 4:09p Sm133076
 * Patch G Migration
 * 
 * 19    7/24/05 1:46p Jv185012
 * 5 partition modifications
 * 
 * 17    5/04/05 6:36p Dm185016
 * When ASSERT happens for error in reading XML file, it now displays the
 * error message in the dialog box.
 * 
 * 15    4/05/05 11:57a Dm185016
 * Added try/catch to catch mismatch error with PSX.
 * 
 * 14    3/30/05 2:37p Dm185016
 * TAR 297008
 * 
 * 13    3/22/05 4:07p Dm185016
 * UNICODE
 * 
 * 12    3/15/05 3:35p Dm185016
 * TAR 295676 - AcceptConnection is now handled by a separate thread to
 * prevent hanging the state machine thread.
 * 
 * 11    3/14/05 8:14p Dm185016
 * Added API statistics.
 * 
 * 10    3/11/05 1:08p Dm185016
 * Factored out RAP specific API features
 * 
 * 9     3/07/05 6:55p Dm185016
 * TAR 294609.
 * TAR 294610.
 * 
 * 8     3/02/05 1:46p Dm185016
 * TAR 294716.  Was setting current remote connection before releasing the
 * requested connection.  Caused a pointer to the newly released
 * connection to be added back when connection broken.
 * 
 * 7     2/28/05 6:02p Dm185016
 * Unicode fix
 * 
 * 6     2/28/05 4:22p Dm185016
 * lint
 * 
 * 5     2/21/05 8:10p Dm185016
 * TAR 293968
 * 
 * 4     2/18/05 4:35p Dm185016
 * TAR 293807
 * 
 * 3     2/15/05 11:29a Dm185016
 * TAR 293322.
 * 
 * 2     2/07/05 7:17p Dm185016
 * TAR 292647
 * 
 * 1     1/26/05 2:21p Dm185016
 * Moved to new Repository.
// 
// 11    1/18/05 3:47p Dm185016
// UNICODE support
// 
// 10    1/12/05 5:55p Dm185016
// Multiple RAP support.
// 
// 9     12/22/04 11:07a Dm185016
// HH Support.  Factored out remote data into new class CRemoteConnection.
// 
// 8     12/16/04 9:59a Dm185016
// Added assert-like popup for failure to load the XML.
// 
// 7     12/14/04 11:23a Dm185016
// Lane number no longer needed
// 
// 6     12/02/04 3:42p Dm185016
// 
// 5     11/16/04 11:34a Dm185016
// Input event made synchronous
// 
// 4     11/02/04 3:06p Dm185016
// Misspelled name of control
// 
// 3     10/29/04 9:01a Dm185016
// Added error checks for APIs.
// Split out RAP specific function.
// 
// 2     10/14/04 1:38p Dm185016
// New OnInputMethod Api.
// PSX now passed as handle.
// 
// 1     10/04/04 5:36p Dm185016
// New for 3.2 Patch E/4.0
*/
/*lint -esym(1763, fastlane::CRCManager::GetControl) */
/*lint -esym(1763, fastlane::CRCManager::GetCurrentRemoteConnection) */
/*lint -esym(1763, fastlane::CRCManager::currentRemoteConnection) */
/*lint -esym(1763, fastlane::CRCManager::currentRemoteConnection) */


#include "StdAfx.h"
#include "RCManager.h"
#include "SMError.h"
#include "RCMMessages.h"
#include "RCMError.h"
#include "RCMStateMachine.h"
#include "Configuration.h"
#include "KVParmList.h"
#include "RCMKeyValue.h"
#include "Registry.h"
#include "RCMRegistry.h"
#include "RCMConfigDefaults.h"
#include "RCMRecorder.h"
#include "RCMgrAPI.h"
#include "PSXMFC.h"
#include "verify.h"

using namespace fastlane;
using namespace STATE_MACHINE_NAMESPACE;

_TCHAR szMachineName[MAX_COMPUTERNAME_LENGTH + 1];


CRCManager::CRCManager()
    : m_pStateMachine(NULL)
    , m_pXMLError(NULL)
    , m_pConfigMgr(NULL)
    , m_pPSX(NULL)
    , m_pControl(NULL)
    , m_pRecorder(NULL)
    , m_bServiceAvailable(false)
    , m_bStateMachineReady(false)
    , m_bExclusiveAccessGranted(false)
    , m_bExclusiveAccessReleaseRequired(true)
    , m_ulExceptionCount(0)
    , m_pCurrentRemoteConnection(NULL)
    , m_hrSMFailureReason(S_OK)
    , INIT_API_STATS(Initialize)
    , INIT_API_STATS(Terminate)
    , INIT_API_STATS(OnEvent)
    , INIT_API_STATS(OnControl)
    , INIT_API_STATS(Get)
    , INIT_API_STATS(Set)
    , INIT_API_STATS(OnInputEvent)
{
    DWORD dwMachineNameLength = MAX_COMPUTERNAME_LENGTH + 1;
    _TCHAR *pszMachineName = new _TCHAR[dwMachineNameLength];

    VERIFY_WIN32_AND_LOG(::GetComputerName(pszMachineName, &dwMachineNameLength));

    m_sComputerName = szMachineName;

    delete []pszMachineName;
}


CRCManager::~CRCManager()
{
    IMODTRACE_DEVELOP();

    ITRACE_DEVELOP(_T("Api Statistics:"));
    API_STATS(Terminate)
    API_STATS(Initialize)
    API_STATS(OnEvent)
    API_STATS(OnControl)
    API_STATS(OnInputEvent)
    API_STATS(Get)
    API_STATS(Set)

    try
    {
        delete m_pStateMachine;

        if (m_bServiceAvailable /*|| (m_pMainWnd != NULL)*/)
        {
            //delete m_pMainWnd;
            //m_pMainWnd = NULL;
            m_qEvents.EndService();
        }

        delete m_pConfigMgr;

        delete m_pRecorder;
        m_pRecorder = NULL;

        delete m_pPSX;

        m_pPSX = NULL;

        CRemoteConnectionsMap::iterator iter = m_RemoteConnections.begin();
        for (; iter != m_RemoteConnections.end(); iter++)
        {
            delete iter->second;
        }
        m_pCurrentRemoteConnection = NULL;

        delete m_pXMLError;
    }
    catch (...)
        { ITRACE_ERROR(_T("Exception caught in CRCManager::~CRCManager")); }

    m_pControl = NULL;
}


void CRCManager::BuildStateMachine()
{
    IMODTRACE_DEVELOP();

    m_bStateMachineReady = false;

    HRESULT hr = CoInitialize(NULL);
    switch (hr)
    {
        case S_FALSE :
            hr = S_OK;
            //lint -fallthrough
        case S_OK :
            break;
        default :
            {
                ISystemErrorInfo error(hr, _T("CoInitialize"));
                ITRACE_ERROR(error.text());
            }
    }

    if (hr == S_OK)
    {
        try
        {
            CRegistry registry;
            const _TCHAR *pSMName 
                = CRegistryValue<const _TCHAR *>::GetRegistryValue( registry
                                    , HKEY_RCM
                                    , REG_VALUE_STATE_MACHINE
                                    , NULL );

            if (pSMName[0] == 0)
            {
                IString sErrorText( HKEY_RCM _T("\\") REG_VALUE_STATE_MACHINE _T(" not specified in registry."));
                throw CSMError(sErrorText,
                               RCM_NO_XML_FILE_SPECIFIED);
            }

			TCHAR szSMName[_MAX_PATH] = _T("");
			if(ExpandEnvironmentStrings(pSMName, szSMName, _MAX_PATH))
			{
				pSMName = szSMName;
			}

            m_pStateMachine = GetStateMachine(pSMName);

            m_pStateMachine->Load();

            m_bStateMachineReady = true;
        }
        catch (const CSMError &e)
        {
            ITRACE_EXCEPTION(e);

            m_pXMLError = new CSMError(e);

            m_hrSMFailureReason = e.errorId();

            _ASSERTEX( _T("Error reading state machine XML file.  ") + IString(m_pXMLError->text()));

            _RPT0(_CRT_ERROR, _T("CSMError detected in building the state machine"));
        }
        catch (const exception &e)
        {
            ITRACE_EXCEPTION(e);

            _ASSERTEX( _T("Error reading state machine XML file.  See log for error description."));

            _RPT0(_CRT_ERROR, _T("exception detected in building the state machine"));
        }
    }
}


/// <summary>This function returns the configuration manager for the application. </summary>
CConfigManager *CRCManager::GetConfigMgr()
{
    if (m_pConfigMgr == NULL)
        m_pConfigMgr = new CConfigManager(GetTraceHandle());

    return m_pConfigMgr;
}


HRESULT CRCManager::LoadOptions()
{
    return S_OK;
}



CRCMStateMachine *CRCManager::GetStateMachine(const _TCHAR *pSMName)
{ 
    IMODTRACE_DEVELOP();

    return new CRCMStateMachine(pSMName, GetTraceHandle()); 
}


CRCMStateMachine *CRCManager::GetStateMachine()
{ 
    _ASSERTE(m_pStateMachine!=NULL);
    return m_pStateMachine; 
}


void CRCManager::SetControl(CRemoteConsoleMgrCtrl *pControl)
{
    m_pControl = pControl;
}


CRemoteConsoleMgrCtrl *CRCManager::GetControl() const
{
    _ASSERTE(m_pControl!=NULL);
    return m_pControl;
}


const _TCHAR *CRCManager::GetRemoteMachineName() const throw(...)
{
    return currentRemoteConnection().GetMachineName(); 
}


const _TCHAR *CRCManager::GetRemoteMachineName(const _TCHAR *pszRemoteConnectionName) const throw(...)
{
    CRemoteConnection &remoteConn = remoteConnection(pszRemoteConnectionName);

    return remoteConn.GetMachineName(); 
}


const _TCHAR *CRCManager::GetRemoteConnectionName() const throw(...)
{
    return currentRemoteConnection().GetRemoteConnectionName(); 
}


const _TCHAR *CRCManager::GetRemoteConnectionName(const _TCHAR *pszRemoteConnectionName) const throw(...)
{
    CRemoteConnection &remoteConn = remoteConnection(pszRemoteConnectionName);

    return remoteConn.GetRemoteConnectionName(); 
}


//void CRCManager::SetHandHeld(bool bHandHeld) { m_bHandHeld = bHandHeld; }
bool CRCManager::IsHandHeld() const throw(...) 
{
    return currentRemoteConnection().IsHandheld(); 
}


bool CRCManager::IsHandHeld(const _TCHAR *pszRemoteConnectionName) const throw(...) 
{
    CRemoteConnection &remoteConn = remoteConnection(pszRemoteConnectionName);

    return remoteConn.IsHandheld(); 
}


const _TCHAR *CRCManager::GetCurrentContext() const throw(...)
{
    return currentRemoteConnection().GetCurrentContext(); 
}


const _TCHAR *CRCManager::GetCurrentContext(const _TCHAR *pszRemoteConnectionName) const throw(...)
{
    CRemoteConnection &remoteConn = remoteConnection(pszRemoteConnectionName);

    return remoteConn.GetCurrentContext(); 
}


void CRCManager::SetCurrentContext(const _TCHAR *pszCurrentContext)
{
    return currentRemoteConnection().SetCurrentContext(pszCurrentContext); 
}


void CRCManager::SetCurrentContext(const _TCHAR *pszRemoteConnectionName
                                 , const _TCHAR *pszCurrentContext) 
{
    CRemoteConnection &remoteConn = remoteConnection(pszRemoteConnectionName);

    return remoteConn.SetCurrentContext(pszCurrentContext); 
}


void CRCManager::AddRemoteConnection(PREMOTECONN pRConn)
{
    const _TCHAR *pszConnName = pRConn->GetRemoteConnectionName();

    // Check for duplicates.  If the connection is already in the collection, then
    // delete the new copy and return.
    CRemoteConnectionsMap::iterator iter = m_RemoteConnections.find(pszConnName);
    if (iter != m_RemoteConnections.end())
    {
        RemoveRemoteConnection(pszConnName);

        ITRACE_WARNING(_T("Duplicate remote connection name detected."));
    }

    // Add the new connection to the collection
    m_RemoteConnections.insert(std::make_pair(pszConnName, pRConn));

    // If we currently do not have a remote connection, then make this one the
    // current remote connection
    if (NULL == GetCurrentRemoteConnection())
        SetCurrentRemoteConnection(pRConn);
}


void CRCManager::RemoveRemoteConnection(const _TCHAR *pszName)
{
    // Get the remote connection by name
    PREMOTECONN pRConn = GetRemoteConnection(pszName);

    // If the connection object exists, then delete it and remove it from the collection.
    if (pRConn != NULL)
    {
        delete pRConn;
        m_RemoteConnections.erase(pszName);
    }

    if (pRConn == GetCurrentRemoteConnection())
        SetCurrentRemoteConnection(NULL);
}


bool CRCManager::IsRemoteConnectionAvailable() const
{
    return !m_RemoteConnections.empty();
}


PREMOTECONN CRCManager::GetRemoteConnection(const _TCHAR *pszName) const
{
    CRemoteConnectionsMap::const_iterator iter = m_RemoteConnections.find(pszName);
    if (iter == m_RemoteConnections.end())
        return NULL;

    return iter->second;
}


const CRemoteConnectionsMap &CRCManager::GetRemoteConnections() 
{ 
    return m_RemoteConnections; 
}


PREMOTECONN CRCManager::GetCurrentRemoteConnection() const { return m_pCurrentRemoteConnection; }


void CRCManager::SetCurrentRemoteConnection(PREMOTECONN pRConn) 
{ 
    m_pCurrentRemoteConnection = pRConn; 
    if (m_pCurrentRemoteConnection == NULL)
    {
        CRemoteConnectionsMap::const_iterator iter = m_RemoteConnections.begin();
        if (iter != m_RemoteConnections.end())
        {
            m_pCurrentRemoteConnection = iter->second;
            ITRACE_DEVELOP(_T("Current remote connection reset to ") + 
                           IString(m_pCurrentRemoteConnection->GetRemoteConnectionName()));
        }
        else
        {
            ITRACE_DEVELOP(_T("Current remote connection reset to NULL"));
        }
    }
}


void CRCManager::SetExclusiveAccessGranted(bool bEAGranted) { m_bExclusiveAccessGranted = bEAGranted; }
bool CRCManager::IsExclusiveAccessGranted() const { return m_bExclusiveAccessGranted; }


void CRCManager::SetExclusiveAccessReleaseRequired(bool bEAReleaseRequired) { m_bExclusiveAccessReleaseRequired = bEAReleaseRequired; }
bool CRCManager::IsExclusiveAccessReleaseRequired() const { return m_bExclusiveAccessReleaseRequired; }


CRemoteConnection &CRCManager::remoteConnection(const _TCHAR *pszName) const throw(...)
{
    CRemoteConnection *pRConn = GetRemoteConnection(pszName);
    if (pRConn == NULL)
        throw IException((const _TCHAR *) (_T("Unknown Remote Connection name:  ") + IString(pszName)),
                          RCM_UNKNOWN_REMOTE_CONNECTION );

    return *pRConn;
}


CRemoteConnection &CRCManager::currentRemoteConnection() const throw(...)
{
    if (m_pCurrentRemoteConnection == NULL)
        throw IException( _T("NULL Remote Connection pointer:  "),
                          RCM_UNKNOWN_REMOTE_CONNECTION );

    return *m_pCurrentRemoteConnection;
}


const _TCHAR *CRCManager::GetMachineName() const { return m_sComputerName.c_str(); }


void CRCManager::FireEvent(CRCMEvent *pEvent)
{
    recorder().RecordAPI(CRCMgrAPI::BuildEventRecord(pEvent));

    m_qEvents.AddEvent( pEvent );
}


// CRCManagerCtrl message handlers
/*lint -save -e529 */
HRESULT CRCManager::Terminate()
{
    IMODTRACE_DEVELOP();

    API_TRACE(Terminate);

    _ASSERTE( _CrtCheckMemory() );

    recorder().RecordAPI(CRCMgrAPI::BuildTerminateRecord());

    if (!m_bStateMachineReady)
        return RCM_SM_NOT_INITIALIZED;

    //_ASSERTE( m_pMainWnd != NULL );

    //if ( m_pMainWnd == NULL )       //lint !e774    Invalid message since problem in Release
    //{
    //    _ASSERTE( _CrtCheckMemory() );
    //    return;
    //}

    // Tell the state machine to terminate all operations
    IMessage *pMsg;
    HRESULT hr = SendMsg(&pMsg, MSG_TERMINATE);
    if (hr != S_OK)
        return RCM_SM_MESSAGE_NOT_DEFINED;

    BSTR bstrResult = pMsg->GetText();
    CW2T strResult(bstrResult);
    ITRACE_DEVELOP(_T("Abort result:  ") + IString(strResult));
    ::SysFreeString(bstrResult);

    delete pMsg;

    m_qEvents.EndService();

    m_pStateMachine->Stop();

    m_bServiceAvailable = false;

    _ASSERTE( _CrtCheckMemory() );

    delete m_pPSX;

    m_pPSX = NULL;

    return S_OK;
}
/*lint -restore */


HRESULT CRCManager::PostMsg(const _TCHAR *pszName, const void *pData, size_t nDataLength)
{
    IMessage *pMsg;

    CRCMStateMachine &sm = *GetStateMachine();

    HRESULT hr = sm.GetMessage(&pMsg, pszName, pData, nDataLength);
    if ( hr == S_OK)
        hr = sm.PostMsg(*pMsg);
    else
        ITRACE_ERROR(_T("Failure to create message:  ") + IString(pszName));

    return hr;
}


HRESULT CRCManager::SendMsg(IMessage **ppMsg, const _TCHAR *pszName, const void *pData, size_t nDataLength)
{
    CRCMStateMachine &sm = *GetStateMachine();

    HRESULT hr = sm.GetMessage(ppMsg, pszName, pData, nDataLength);
    if ( hr == S_OK)
        hr = sm.SendMsg(**ppMsg);

    return hr;
}


HRESULT CRCManager::Initialize(LONG lPSXHandle, HTRACE hTrace)
{
    // Do not allow initialization to proceed more than once
    if (m_bStateMachineReady)
        return RCM_SM_ALREADY_INITIALIZED;

    // Setup the tracing object passed in from the OCX
    SetTraceObject(hTrace);

    IMODTRACE_DEVELOP();

    // Build the API Recorder
    m_pRecorder = new CRCMRecorder(hTrace);
    _ASSERTE( _CrtCheckMemory() );

    recorder().RecordAPI(CRCMgrAPI::BuildInitializeRecord(lPSXHandle, hTrace));

    ITRACE_DEVELOP(_T(__FUNCTION__) _T("  lPSXHandle:  ") + IString(lPSXHandle));

    // Save the PSX instance pointer for use later
    _ASSERTE(lPSXHandle != 0);
    try
    {
        m_pPSX = CPSX::FromHandle( lPSXHandle );
        if (m_pPSX == NULL)
        {
            ITRACE_DEVELOP(_T("Invalid handle received.  lPSXHandle:  ") + IString(lPSXHandle));
            return E_FAIL;
        }
    }

    // If this call fails, then we have a mismatch between us and the PSX.
    // e.g. we are ASCII and PSX is UNICODE
    catch (...)
    {
        ITRACE_ERROR(_T("Version mismatch between RCM and PSX"));

        return RCM_PSX_VERSION_MISMATCH;
    }

    //Build the state machine from the XML file
    BuildStateMachine();
    if (!m_bStateMachineReady)
    {
        _ASSERTE( _CrtCheckMemory() );
        return E_FAIL;
    }

    // Start processing messages in the state machine
    m_pStateMachine->Start();

    // Start the event handler for transfering events to the correct thread
    m_qEvents.BeginService( this );

    //ASSERT( m_pMainWnd == NULL );

    //if ( m_pMainWnd != NULL )       //lint !e774    Really problem in Release
    //    return E_FAIL;

    //m_pMainWnd = new CSmParentWnd( );

    m_bServiceAvailable = true;

    //m_nAdviseCount++;

    _ASSERTE( _CrtCheckMemory() );
    if (!_CrtCheckMemory())
        ITRACE_DEVELOP(_T("Initialize first assert failed"));
    
    HRESULT hr;
	
  //  if (lCookie > m_nAdviseCount)
  //  {
  //      _ASSERTE( _CrtCheckMemory() );
		//return SECURITY_INVALID_COOKIE;
  //  }

    //recorder().RecordAPI(CRCMgrAPI::BuildInitializeRecord(bstrInParms));

    // Send an Initialize message to the state machine to perform the remainder
    // of the initialization steps.
    if (m_bStateMachineReady)
    {
        IMessage *pMsg;
        hr = SendMsg(&pMsg, MSG_INITIALIZE);

        delete pMsg;
    }
    else
    {
        CKeyValueParmList parmList;
        parmList.NewKVPair(KEY_MISSING_FILE_NAME, m_pStateMachine->GetURL());
        _bstr_t bstrParmList((const _TCHAR *)parmList); 

        hr = RCM_SM_INIT_FAILED;
    }

    //recorder().RecordResult(CRCMAPI::BuildInitializeRecord(*pbstrValues));

    _ASSERTE( _CrtCheckMemory() );
    if (!_CrtCheckMemory())
        ITRACE_DEVELOP(_T("Initialize second assert failed"));

    ITRACE_DEVELOP(_T("<< ") _T(__FUNCTION__));

    return hr;
}


HRESULT CRCManager::OnControl(LPCTSTR bstrInParms, BSTR* bstrOutParms)
{
    IMODTRACE_DEVELOP();

    API_TRACE(OnControl);

    _ASSERTE( _CrtCheckMemory() );

    if (!m_bStateMachineReady)
        return RCM_SM_NOT_INITIALIZED;

    recorder().RecordAPI(CRCMgrAPI::BuildOnControlRecord(bstrInParms));

    // Free any string passed in
    ::SysFreeString(*bstrOutParms);

    IMessage *pMsg;
    HRESULT hr = SendMsg(&pMsg, MSG_CONTROL_COMMAND, bstrInParms, _tcslen(bstrInParms)); //lint !e534

    *bstrOutParms = pMsg->GetText();

    recorder().RecordResult(CRCMgrAPI::BuildOnControlRecord(pMsg->GetText()));

    _ASSERTE( _CrtCheckMemory() );

    delete pMsg;

    return hr;
}


HRESULT CRCManager::OnEvent(LPCTSTR bstrParms)
{
    IMODTRACE_DEVELOP();

    API_TRACE(OnEvent);

    _ASSERTE( _CrtCheckMemory() );

    if (!m_bStateMachineReady)
        return RCM_SM_NOT_INITIALIZED;

    recorder().RecordAPI(CRCMgrAPI::BuildOnEventRecord(bstrParms));

    HRESULT hr = PostMsg(MSG_EVENT_COMMAND, const_cast<LPTSTR>(bstrParms), _tcslen(bstrParms)+1);
ITRACE_DEVELOP(_T("PostMsg returned:  ") + IString(hr).d2x());
    VERIFY((hr==S_OK) || (hr==S_FALSE));

    _ASSERTE( _CrtCheckMemory() );

    return S_OK;
}


HRESULT CRCManager::Get(LPCTSTR bstrInParms, BSTR* bstrOutParms)
{
    IMODTRACE_DEVELOP();

    API_TRACE(Get);

    _ASSERTE( _CrtCheckMemory() );

    if (!m_bStateMachineReady)
        return RCM_SM_NOT_INITIALIZED;

    recorder().RecordAPI(CRCMgrAPI::BuildGetRecord(bstrInParms));

    // Free any string passed in
    ::SysFreeString(*bstrOutParms);

    IMessage *pMsg;
    HRESULT hr = SendMsg(&pMsg, MSG_GET_COMMAND, bstrInParms, _tcslen(bstrInParms)); //lint !e534
ITRACE_DEVELOP(_T("SendMsg returned:  ") + IString(hr).d2x());

    *bstrOutParms = pMsg->GetText();

    recorder().RecordResult(CRCMgrAPI::BuildGetRecord(pMsg->GetText()));

    _ASSERTE( _CrtCheckMemory() );

    delete pMsg;

    return hr;
}


HRESULT CRCManager::Set(LPCTSTR bstrParms)
{
    _ASSERTE( _CrtCheckMemory() );

    API_TRACE(Set);

    IMODTRACE_DEVELOP();

    if (!m_bStateMachineReady)
        return RCM_SM_NOT_INITIALIZED;

    recorder().RecordAPI(CRCMgrAPI::BuildSetRecord(bstrParms));

    HRESULT hr = PostMsg(MSG_SET_COMMAND, const_cast<LPTSTR>(bstrParms), _tcslen(bstrParms)+1);
ITRACE_DEVELOP(_T("PostMsg returned:  ") + IString(hr).d2x());
    VERIFY((hr==S_OK) || (hr==S_FALSE));

    _ASSERTE( _CrtCheckMemory() );

    return S_OK;
}


HRESULT CRCManager::OnInputEvent(LPCTSTR bstrParms)
{
    IMODTRACE_DEVELOP();

    API_TRACE(OnInputEvent);

    _ASSERTE( _CrtCheckMemory() );

    if (!m_bStateMachineReady)
        return RCM_SM_NOT_INITIALIZED;

    recorder().RecordAPI(CRCMgrAPI::BuildOnInputEventRecord(bstrParms));

    IMessage *pMsg;
    HRESULT hr = SendMsg(&pMsg, MSG_INPUT_EVENT_COMMAND, const_cast<LPTSTR>(bstrParms), _tcslen(bstrParms)+1); //lint !e534
ITRACE_DEVELOP(_T("SendMsg returned:  ") + IString(hr).d2x());

    VERIFY((hr==S_OK) || (hr==S_FALSE));

    _ASSERTE( _CrtCheckMemory() );

    return hr;
}


/// <summary>
/// Gets a list of RCM Exceptions that are currently outstanding.
/// </summary>
/// <value>
/// A list of RCM Exceptions that are currently outstanding.
/// </value>
BSTR CRCManager::GetErrorText(void)
{
    //API_TRACE(GetErrorInfo);

    _ASSERTE( _CrtCheckMemory() );

    _bstr_t bstrResult;
    if (m_pXMLError != NULL)
        bstrResult = m_pXMLError->what();

    _ASSERTE( _CrtCheckMemory() );

    return bstrResult.Detach();
}


/// <summary>
/// Gets a list of RCM Exceptions that are currently outstanding.
/// </summary>
/// <value>
/// A list of RCM Exceptions that are currently outstanding.
/// </value>
CXMLErrorInfo *CRCManager::GetErrorInfo(void)
{
    if (m_pXMLError == NULL)
        return NULL;

    return dynamic_cast<CXMLErrorInfo *>(m_pXMLError->errorInfo());
}
