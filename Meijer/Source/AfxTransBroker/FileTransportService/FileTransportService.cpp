////////////////////////////////////////////////////////////
// FileTransportService.cpp : Implementation of WinMain
//
// Description
// A Windows Service used to transport files between SMB based resources.  
// Supports moving and copying files. Full tracing in Windows Application 
// Event Viewer and self registration through command line switches.
// 
// Author: Steve Antonakakis
//         FastLane Senior Software Engineer
//
// Written: November-2004
////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "FTSDefine.h"

// The module attribute causes WinMain to be automatically implemented for you
[ module(SERVICE, uuid = "{0C817706-D515-4E46-BFB1-D2E6304CA861}", 
         name = "FileTransportService", 
         helpstring = "File Transport Service 1.0 Type Library", 
         resource_name="IDS_SERVICENAME") ]
class CFileTransportService
{
protected:
    HANDLE m_hWorkerThread;
    HANDLE m_hShutdown;

    friend DWORD WINAPI WorkerThreadProc(LPVOID pvParam)
    {
        CFileTransportService & fts = *(CFileTransportService*)pvParam;

        CString strError = _T("");
        // 
        // Initialize the COM apartment.
        //
        HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        if( FAILED(hr) )
        {
            strError.Format(_T("Failed to initialize Worker thread COM apartment. HRESULT = 0x%08X"), hr);
            fts.LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
            return RETCODE_WORKERCOMPLETE;
        }

        //
        // initially set the timeout to one second, subsequently it will be retrieved from file
        // if the file retrieval fails then set to the non-first default value
        //
        DWORD dwTimeout = DEFAULT_TIMEOUT_FIRST; 
        BOOL bFirstLoop = TRUE;
        while( fts.WaitForNextTimeoutEvent(dwTimeout) )
        {
            // 
            // Once the first wait is executed. Set the new default period and proceed.
            //
            if( bFirstLoop )
            {
                bFirstLoop = FALSE;
                dwTimeout = DEFAULT_TIMEOUT;
            }
            
            fts.LogEvent(_T("File Transport Service monitoring has started."));

            CString strFile = fts.GetConfigFilePath();

            //
            // load the FileTransportService XML file
            //
            MSXML2::IXMLDOMDocumentPtr spDoc(__uuidof(MSXML2::DOMDocument40));
            VARIANT_BOOL vb = spDoc->load((LPCTSTR)strFile);
            if( VARIANT_TRUE == vb )
            {
                fts.GetNextTimeoutPeriod(spDoc, dwTimeout);

                try
                {   
                    //
                    // Validate XML configuration structure
                    //
                    if( spDoc->selectSingleNode(XML_ROOT) != NULL )
                    {
                        if( spDoc->selectSingleNode(XML_MONITORS) != NULL )
                        {
                            fts.EnumerateAndPerformFileTransports(spDoc);
                        }
                        else
                        {
                            strError.Format(_T("Configuration file node is missing: '%s'."), (LPCTSTR)XML_MONITORS);
                            fts.LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
                        }
                    }
                    else
                    {
                        strError.Format(_T("Configuration file node is missing: '%s'."), (LPCTSTR)XML_ROOT);
                        fts.LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
                    }
                }
                catch(_com_error & cex)
                {
                    strError.Format(_T("Configuration file error executing transports in file '%s'. Error: %s"), strFile, cex.ErrorMessage());
                }
            }
            else
            {
                strError.Format(_T("Failed to load the config file '%s'."), strFile);
                fts.LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
            }
            fts.LogEvent(_T("File Transport Service monitoring has completed. Will run again in %.3f hours."), dwTimeout/3600000.0);
        }
        CoUninitialize();
        return RETCODE_WORKERCOMPLETE;
    }

protected:
    BOOL GetNextTimeoutPeriod(MSXML2::IXMLDOMDocumentPtr spDoc, DWORD & dwTimeout)
    {
        BOOL bRet = FALSE;
        CString strError;
        //
        // Get the next timeout period
        //
        MSXML2::IXMLDOMAttributePtr spAttr = spDoc->selectSingleNode(XML_MONITORS_ATTR_PERIOD);
        if( spAttr != NULL )
        {
            try
            {
                _variant_t vt = spAttr->nodeValue;
                vt.ChangeType(VT_UI4);
                dwTimeout = vt.ulVal*1000;
                bRet = TRUE;
            }
            catch(_com_error)
            {
                strError.Format(
                    _T("An invalid monitor period '%s' is set in the config file. Keeping existing period of %d seconds."),
                    (LPCTSTR)_bstr_t(spAttr->nodeValue),
                    dwTimeout/1000
                    );
                LogEventEx(0, strError, EVENTLOG_WARNING_TYPE);
            }
        }
        else
        {
            strError.Format(_T("A period is not set in the config file. Keeping existing period of %d seconds"), dwTimeout/1000);
            LogEventEx(0, strError, EVENTLOG_WARNING_TYPE);
        }
        return bRet;
    }

    _bstr_t GetTextAttribute(MSXML2::IXMLDOMNodePtr spNode, _bstr_t & bstrAttr)
    {
        try
        {
            return spNode->attributes->getNamedItem(bstrAttr)->nodeValue.bstrVal;
        }
        catch(_com_error)
        {
            return OLESTR("");
        }
    }

    void EnumerateAndPerformFileTransports(MSXML2::IXMLDOMDocumentPtr spDoc)
    {
        CString strError = _T("");
        //
        // Get the collection of monitor items and execute the actions
        //
        MSXML2::IXMLDOMNodeListPtr spMonitors = spDoc->selectNodes(XML_MONITORS_MONITOR);

        if( spMonitors != NULL )
        {
            spMonitors->reset();

            // loop through each and spawn a copy/move session.
            MSXML2::IXMLDOMNodePtr spMonitor;
            while( (spMonitor = spMonitors->nextNode()) != NULL )
            {   
                if( IsShutdownRequested() )
                {
                    LogEventEx(0, 
                        _T("A service STOP request has been issued. The current monitor cycle has been interrupted."), 
                        EVENTLOG_WARNING_TYPE);
                    break;
                }

                CString strSrcMask = _T("");
                CString strDest = _T("");
                CString strMove = _T("");
                CString strSrcUser = _T("");
                CString strSrcPass = _T("");
                CString strDestUser = _T("");
                CString strDestPass = _T("");

                BOOL bMove = FALSE;
                try
                {
                    //
                    // Get the source mask
                    //
                    strSrcMask = (LPCTSTR)_bstr_t(spMonitor->attributes->getNamedItem(XML_MONITOR_SRC)->nodeValue);
                    strDest = (LPCTSTR)_bstr_t(spMonitor->attributes->getNamedItem(XML_MONITOR_DEST)->nodeValue);
                    strMove = (LPCTSTR)_bstr_t(spMonitor->attributes->getNamedItem(XML_MONITOR_MOVE)->nodeValue);
                    strSrcUser = (LPCTSTR)GetTextAttribute(spMonitor, XML_MONITOR_SRCUSER);
                    strSrcPass = (LPCTSTR)GetTextAttribute(spMonitor, XML_MONITOR_SRCPASS);
                    strDestUser = (LPCTSTR)GetTextAttribute(spMonitor, XML_MONITOR_DESTUSER);
                    strDestPass = (LPCTSTR)GetTextAttribute(spMonitor, XML_MONITOR_DESTPASS);

                    strMove.MakeUpper();
                    bMove = (strMove == _T("YES") || strMove == _T("1") || strMove == "Y");
                }
                catch(_com_error & cex)
                {
                    strError.Format(_T("Source, Destination and Move attributes were not set properpy for one monitor. Skipping."));
                    LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
                    continue;
                }

                //
                // Generate the source folder excluding mask and 
                // validate the source
                //
                TCHAR szTemp[PATH_SIZE] = {0};
                _tcscpy(szTemp, strSrcMask);
                ATLPath::RemoveFileSpec(szTemp);
                ATLPath::AddBackslash(szTemp);
                CString strSrc = szTemp;

                //
                // If the source is a UNC path and remote, then 
                // the service will attempt to authenticate with
                // provided credentials
                //
                if( ATLPath::IsUNC(strSrc) )
                {
                    AuthenticateToRemoteShare(strSrc, strSrcUser, strSrcPass);
                }

                if( !ATLPath::IsDirectory(strSrc) )
                {
                    DWORD dwError = GetLastError();
                    strError.Format(_T("The source folder specified is invalid: '%s'. Skipping this monitor entry. Last Error: %d"), strSrc, dwError);
                    LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
                    continue;
                }

                //
                // If the destination is a UNC path and remote, then 
                // the service will attempt to authenticate with
                // provided credentials
                //
                if( ATLPath::IsUNC(strDest) )
                {
                    AuthenticateToRemoteShare(strDest, strDestUser, strDestPass);
                }

                //
                // Validate the destination
                //
                if( !ATLPath::IsDirectory(strDest) )
                {
                    DWORD dwError = GetLastError();
                    strError.Format(_T("The destination folder specified is invalid: '%s'. Skipping this monitor entry. Last Error: %d"), strDest, dwError);
                    LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
                    continue;
                }

                //
                // Add destination backslash
                //
                strDest.Trim();
                _tcscpy(szTemp, strDest);
                ATLPath::AddBackslash(szTemp);
                strDest = szTemp;

                // 
                // retrieve a static list of source files
                //
                CSimpleArray<CString> strFileList;
                strFileList.RemoveAll();
                RetrieveSrcFileList(strSrcMask, strFileList);

                if( strFileList.GetSize() )
                {
                    for(int i = 0; i < strFileList.GetSize(); i++)
                    {
                        if( IsShutdownRequested() )
                        {
                            LogEventEx(0, 
                                _T("A service STOP request has been issued. The current monitor cycle has been interrupted."), 
                                EVENTLOG_WARNING_TYPE);
                            break;
                        }
                        PerformFileTransport(strFileList[i], strSrc, strDest, bMove);
                    }
                }
            }
        }
        else
        {
            LogEvent(_T("No monitors specified for file transfer."));
        }
    }

    BOOL MakeFileWriteable(const CString strFile)
    {
        BOOL bRet = TRUE;
        // is the file readonly?
        DWORD dwAttr = GetFileAttributes(strFile);
        if( dwAttr & FILE_ATTRIBUTE_READONLY )
        {
            // reset the read-only flag
            dwAttr &= ~FILE_ATTRIBUTE_READONLY;
            bRet = SetFileAttributes(strFile, dwAttr);
        }
        return bRet;
    }

    void PerformFileTransport(const CString & strFile, const CString & strSrc, const CString & strDest, BOOL bMove)
    {
        CString strError = _T("");
        CString strMsg;
        strMsg.Format(_T("File: '%s', Source: '%s', Destination: '%s'."), strFile, strSrc, strDest);

        // does the destination file exist?
        // if it does, then make sure that at the very least the read-only flag is not set
        if( ATLPath::FileExists(strDest + strFile) )
        {
            if( !MakeFileWriteable(strDest + strFile) )
            {
                strError.Format(
                    _T("Destination file already exists and we failed to clear the destination ") 
                    _T("file's read-only flag. It's possible the transport may fail. %s"),
                    strMsg
                    );
                LogEventEx(0, strError, EVENTLOG_WARNING_TYPE);
            }
        }

        // copy the file over
        if( CopyFile(strSrc + strFile, strDest + strFile, FALSE) )
        {
            // was the file transported successfully?
            if( ATLPath::FileExists(strDest + strFile) )
            {
                if( !MakeFileWriteable(strDest + strFile) )
                {
                    strError.Format(_T("Failed to clear the destination file's read-only flag. %s"), strMsg);
                    LogEventEx(0, strError, EVENTLOG_WARNING_TYPE);
                }

                if( bMove )
                {
                    if( !MakeFileWriteable(strSrc + strFile) )
                    {
                        strError.Format(_T("Failed to clear the source file's read-only flag. %s"), strMsg);
                        LogEventEx(0, strError, EVENTLOG_WARNING_TYPE);
                    }

                    // the file was copied successfully and we now need to delete the source file.
                    if( DeleteFile(strSrc + strFile) )
                    {
                        LogEvent(_T("File was moved successfully. %s"), (LPCTSTR)strMsg);
                    }
                    else
                    {
                        strError.Format(_T("File copied but failed to delete source file. %s"), strMsg);
                        LogEventEx(0, strError, EVENTLOG_WARNING_TYPE);
                    }
                }
                else
                {
                    LogEvent(_T("File has been copied successfully. %s"), (LPCTSTR)strMsg);
                }
            }
            else
            {
                strError.Format(_T("Failed to verify the file was copied. %s"), strMsg);
                LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
            }
        }
        else
        {
            strError.Format(_T("Failed to copy file. %s"), strMsg);
            LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
        }
    }

    void RetrieveSrcFileList(LPCTSTR szFileMask, CSimpleArray<CString> & strFileList)
    {
        //
        // Perform the file find
        //
        WIN32_FIND_DATA wfd = {0};
        HANDLE hFind = FindFirstFile(szFileMask, &wfd);
        
        if( INVALID_HANDLE_VALUE != hFind )
        {
            //
            // Generate the source folder excluding mask
            //
            TCHAR szTemp[PATH_SIZE];
            _tcscpy(szTemp, szFileMask);
            ATLPath::RemoveFileSpec(szTemp);
            ATLPath::AddBackslash(szTemp);
            CString strDir = szTemp;

            do
            {
                // ignore folders
                if( !ATLPath::IsDirectory(strDir + wfd.cFileName) )
                {
                    strFileList.Add(wfd.cFileName);
                }
            }
            while( FindNextFile(hFind, &wfd) );
            FindClose(hFind);
        }
    }


    CString GetConfigFilePath()
    {
        //
        // open the registry and fetch the location of the FileTransportService.xml file
        //
        CRegKey key(HKEY_LOCAL_MACHINE);
        CString strRet = _T("");

        if( ERROR_SUCCESS == key.Open(HKEY_LOCAL_MACHINE, REG_CONFIGFILE_PATH, KEY_READ) )
        {
            TCHAR szVal[PATH_SIZE];
            ULONG ulBufSize = PATH_SIZE;
            if( ERROR_SUCCESS == key.QueryStringValue(REG_CONFIGFILE_VALUENAME, szVal, &ulBufSize) )
            {
                strRet = szVal;
            }
            else
            {
                CString strError;
                strError.Format(
                    _T("Registry entry specifying configuration file is not available. HKLM\\%s\\%s"),
                    REG_CONFIGFILE_PATH,
                    REG_CONFIGFILE_VALUENAME);
                LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
            }
        }
        return strRet;
    }
    
    BOOL IsShutdownRequested()
    {
        if( !m_hShutdown )
        {
            return TRUE;
        }
        else
        {
            // Waiting zero will return whether or not the shutdown signal has been set.
            return !WaitForNextTimeoutEvent(0);
        }
    }

    BOOL WaitForNextTimeoutEvent(DWORD dwTimeout)
    {
        DWORD dwRet = WaitForSingleObject(m_hShutdown, dwTimeout);
        if( WAIT_OBJECT_0 == dwRet )
        {
            return FALSE;
        }
        else if( WAIT_TIMEOUT == dwRet )
        {
            return TRUE;
        }
        else
        {
            LogEvent(_T("Received an invalid timeout loop event: 0x%08X."), dwRet);
            return FALSE;
        }
    }

    void WaitForWorkerThreadCompletion()
    {
        DWORD dwCode = 0;
        BOOL bRet = FALSE;
        do
        {   
            // check every second for completion of the worker thread
            Sleep(1000);
            bRet = GetExitCodeThread(m_hWorkerThread, &dwCode);
        }
        while( bRet && (dwCode != RETCODE_WORKERCOMPLETE) );
    }

    BOOL AuthenticateToRemoteShare(CString strUNC, CString strUser, CString strPass)
    {
        BOOL bRet = TRUE;
       
        // just in case

        strUNC.Trim();
        //
        // first get the root UNC share 
        //
        TCHAR szTemp[PATH_SIZE] = {0};
        _tcscpy(szTemp, strUNC);
        ATLPath::StripToRoot(szTemp);
        CString strRoot = szTemp;

        // 
        // next see if the username already contains the computername or domain name.
        // if it doesn't you'll need to append the computer name
        //
        if( strUser.Find(_T("\\")) == -1 )
        {
            int iPos = strUNC.Mid(2).Find(_T("\\"));
            _ASSERT(iPos != -1);
            CString strServer = strUNC.Mid(2, iPos+1);
            strUser = strServer + strUser;
        }

        NETRESOURCE nr = { 0 };
        nr.dwType = RESOURCETYPE_ANY;
        nr.lpLocalName = NULL;
        nr.lpRemoteName = CT2W(strRoot);
        nr.lpProvider = NULL;

        DWORD dwRet = WNetAddConnection2(&nr, strPass, strUser, 0);
        if( dwRet != NO_ERROR )
        {
            CString strError = _T("");
            strError.Format(_T("Error authenticating user '%s' on '%s'. Error: %d"), strUser, strRoot, dwRet);
            LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
            bRet = FALSE;
        }
        return bRet;
    }

public:

	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the
		// appropriate security settings for your service.
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.
		//CoInitializeSecurity(NULL, 
		return S_OK;
	}

    virtual HRESULT PreMessageLoop(int nShowCmd) throw()
    {
        // Method returns S_FALSE in the case where there are no COM objects to be registered
        // Unfortunately returning S_FALSE will fail the service and it won'r run RunMessageLoop()
        // Thuse we need to override and ensure that S_OK is returned if the call succeeds with S_FALSE.
        HRESULT hr = __super::PreMessageLoop(nShowCmd);

        return SUCCEEDED(hr) ? S_OK : hr;
    }

    virtual void RunMessageLoop() throw()
    {
        //
        // Spawn the worker thread
        //
        DWORD dwId = 0;
        m_hShutdown = CreateEvent(NULL, TRUE, FALSE, NULL);
        m_hWorkerThread = CreateThread(NULL, 0, WorkerThreadProc,this, 0, &dwId);

        if( !m_hShutdown || !m_hWorkerThread )
        {
            CString strError;
            strError.Format(_T("Failed to initialize the worker thread.  Shutdown event: 0x%08X. Worker Thread: 0x%08X"), (DWORD)m_hShutdown, (DWORD)m_hWorkerThread);
            LogEventEx(0, strError, EVENTLOG_ERROR_TYPE);
        }
        __super::RunMessageLoop();

        //
        // Once the worker thread is complete, the service can end gracefully.
        //
        WaitForWorkerThreadCompletion();
    }

    HRESULT PostMessageLoop() throw()
    {
        // 
        // Cleanup worker thread code
        //
        if( m_hShutdown )
        {
            CloseHandle(m_hShutdown);
            m_hShutdown = NULL;
        }
        // we should only get here if the thread has been 
        // terminated either gracefully or brute force
        m_hWorkerThread = NULL;

        return __super::PostMessageLoop();
    }


    virtual void OnStop() throw()
    {
        if( !SetEvent(m_hShutdown) )
        {
            LogEventEx(0,_T("Failed to signal worker thread to terminate.  Attempting brute force."), EVENTLOG_WARNING_TYPE);
            if( !TerminateThread(m_hWorkerThread,RETCODE_WORKERCOMPLETE) )
            {
                LogEventEx(0, _T("Brute force worker thread termination failed."), EVENTLOG_ERROR_TYPE);
            }
        }
        __super::OnStop();
    }
};
