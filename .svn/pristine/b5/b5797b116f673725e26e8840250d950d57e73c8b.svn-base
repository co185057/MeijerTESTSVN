// FLTransporter.cpp: implementation of the CFLTransporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FLTransporter.h"
#include "FLReportingErrors.h"
#include "Common.h"

#define COMP_ID ID_RP               // Base state component
#define T_ID _T("CFLTransporter")


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
#define REPORT_REG  _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Reporting")
#define DATADIRECTORYMASK  _T("%DATA_DRIVE%\\scot\\report\\rpt_*.xml")
#define DATADIRECTORY _T("%DATA_DRIVE%\\scot\\report\\")  
#define LOCALHOST _T("127.0.0.1") 

extern bool bReportServerOnline; //RFC 262343


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFLTransporter::CFLTransporter()
: m_bInitialized(false),
  m_bTransportThreadActive(false),
  m_pTransporterThread(NULL)
{
	// Call InitializeTransporter() in constructer that ReportServer path is available at init time instead of first time report is generated //
	InitializeTransporter();
}

CFLTransporter::~CFLTransporter()
{
	DWORD dwCount;
	if (m_bTransportThreadActive)
	{
		try
		{
			// Suspend the thread so it can do no harm while we delete it.
			dwCount = SuspendThread(m_pTransporterThread->m_hThread);
			ASSERT(dwCount != (DWORD)-1);
			m_pTransporterThread = NULL;
		}
		catch (...)
		{
			// I really don't care, just don't want to die
		}
		m_bTransportThreadActive = false;
	}

}
///////////////////////////////////////////////////////////////////////////////
// Function:    TransportFiles                                       
// Class:       CFLTransporter                                              
// Description: This method stays in a loop for as long as the service is running. 
//              It first calls function InitializeTransporter to set up. Next it checks
//              to see if a file exists to be processed.  It not it waits for a directory 
///             change event in the report data directory and then retrieve the file name.  
//              When a file is found, CopyFile is called to move the file to the report engine.  
// Parameters:  NONE
// Returns:     long                                         
///////////////////////////////////////////////////////////////////////////////////
long CFLTransporter::TransportFiles()
{
	DWORD dwResult;
    trace(L6, _T("+CFLTransporter::TransportFiles"));

    if (!m_bTransportThreadActive && !SMState::IsStopping())
	{
		m_pTransporterThread = AfxBeginThread((AFX_THREADPROC)TransporterThreadEntry, 
								this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		_ASSERTE(m_pTransporterThread != NULL);

		if (m_pTransporterThread != NULL)
		{
			m_pTransporterThread->m_bAutoDelete = FALSE;
			dwResult = m_pTransporterThread->ResumeThread();
			ASSERT (dwResult != (DWORD) -1);
			if (dwResult == (DWORD)-1)
			{
			   trace(L4, _T(" CFLTransporter::TransportFiles  Resume Thread Failed"));
			}
		}
		else
		{
		   trace(L4, _T(" CFLTransporter::TransportFiles  Begin Thread Failed"));
		}
	}
    trace(L6, _T("-CFLTransporter::TransportFiles"));
    return (FLRPT_OK);
}

UINT CFLTransporter::TransporterThreadEntry(LPVOID *lpParam)
{
   CFLTransporter *pThis = (CFLTransporter*) lpParam;  //lint !e826 (converting pointer passed)
   pThis->TransporterThread();
   return 0;

}

void CFLTransporter::TransporterThread()
{
	m_bTransportThreadActive = true;
    trace(L6, _T("+CFLTransporter::TransporterThread"));
    WIN32_FIND_DATA FileData;   // Data structure describes the file found
    HANDLE hSearch;             // Search handle returned by FindFirstFile
    CString csSourceFileName;
    CString csDestinationFileName;

    CString csDataDirectoryMask = DATADIRECTORYMASK;
	GET_PARTITION_PATH(csDataDirectoryMask, csDataDirectoryMask.GetBuffer(_MAX_PATH));
	csDataDirectoryMask.ReleaseBuffer();

    CString csDataDirectory = DATADIRECTORY;
	GET_PARTITION_PATH(csDataDirectory, csDataDirectory.GetBuffer(_MAX_PATH));
	csDataDirectory.ReleaseBuffer();

	SCOTAPPErrorObject tteo(_T("SCOTAPP"));
	tteo.Initialize(&to);
    BOOL bFinished = FALSE;
    // If this is the first run, we need to initialize our parameters,
    // identifying the server location
    if (! m_bInitialized)
    {
        InitializeTransporter();
    }

    // Now copy the files

    // Start searching for .txt files in the root directory.
    while(!SMState::IsStopping() &&  
          ((hSearch = ::FindFirstFile(csDataDirectoryMask, &FileData)) != INVALID_HANDLE_VALUE))
    {
        // Do for all files
		bFinished = FALSE;  // Take care in case we come around a second time. TAR 325153
        while (!bFinished)
        {
            csSourceFileName = csDataDirectory;
            csSourceFileName += FileData.cFileName;
            csDestinationFileName = m_csServerSharePoint;
            csDestinationFileName += FileData.cFileName;

            if (! ::CopyFile((LPCTSTR)csSourceFileName, 
                                 (LPCTSTR) csDestinationFileName, 
                                 FALSE))
            {
                // Error occured
                #ifndef NO_DATACAP
                trace( L4, 
                    _T( "CFLTransporter::TransporterThread> CANT_COPY: %s to %s, E_FAIL"), 
							csSourceFileName, csDestinationFileName );
                #endif
				m_bTransportThreadActive = false;
                ::FindClose (hSearch);

				//RFC 262343 - start
				if(bReportServerOnline && !SMState::IsStopping())
				{
					tteo.Error(INFO, SM_CAT_INTERNAL_ERROR,FLR_REPORTSERVER_OFFLINE);
				}
				bReportServerOnline = false;
				//RFC 262343 - end

                return;
            }
            // Delete file if everything went well
            else
            {
				//RFC 262343 - start
				if (!bReportServerOnline && !SMState::IsStopping())
				{
					tteo.Error(INFO, SM_CAT_INTERNAL_INTERFACE,FLR_REPORTSERVER_NOW_ONLINE);
					bReportServerOnline  = true;
				}
				//RFC 262343 - end

                if (FALSE == ::DeleteFile((LPCTSTR)csSourceFileName))
                {
                    #ifndef NO_DATACAP
                    trace( L7, 
                        _T( "CFLTransporter::TransporterThread> FILE_NOT_DELETED: %s"), csSourceFileName );
                    #endif

					//TAR 348946 - start
					long lErr = GetLastError();
					long rc;
					_TCHAR sError[300];

					rc = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, lErr, 0, sError, 299, NULL);
					trace(L2, _T("Error %d(0x%x) on delete - %s"), lErr, lErr, sError);
					//TAR 348946 - end

					m_bTransportThreadActive = false;
                    ::FindClose (hSearch);
                    return;

                }
            }
            if (!::FindNextFile (hSearch, &FileData))
            {
                bFinished = TRUE;
            }
        }
        // Close the search handle.
		BOOL bResultCode = ::FindClose (hSearch);
	    ASSERT(bResultCode);
    }
	tteo.Uninitialize();
    trace(L6, _T("-CFLTransporter::TransporterThread"));
	m_bTransportThreadActive = false;
    return;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    InitializeTransporter                                       
// Class:       CFLTransporter                                              
// Description: Read the registry for destination server name & save server name
// Parameters:  NONE
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void CFLTransporter::InitializeTransporter()
{
    trace(L7, _T("+CFLTransporter::InitializeTransporter"));
    LONG     IRC;
    HKEY     hKey;
    DWORD    dwRegType;
    DWORD    dwDataSize = MAX_PATH;
    _TCHAR     szFilePath[MAX_PATH];
    CString  csServerName;

    // Read the registry for the destination server name
    // Read configuration info from the registry

    IRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, REPORT_REG, 0, KEY_QUERY_VALUE, &hKey);
    if (IRC==ERROR_SUCCESS) 
    { 
        // Registry found
        // Get send receive timeout
         IRC = ::RegQueryValueEx(hKey, _T("ReportServer"), 0, &dwRegType, (LPBYTE)szFilePath, 
                  &dwDataSize);
        if ((IRC==ERROR_SUCCESS) && (dwRegType==REG_SZ))
        {
            // Save the server name
            csServerName = szFilePath;
        }
		IRC = ::RegCloseKey(hKey);
		ASSERT (IRC == ERROR_SUCCESS);
    }
    if (csServerName == EMPTY_STRING)
    {
        csServerName = LOCALHOST;  // This is the universal default for local host 
    }
    m_csServerSharePoint.Format(_T("\\\\%s\\SCOTReport\\"), csServerName);
    m_bInitialized = true;
    trace(L7, _T("-CFLTransporter::InitializeTransporter"));
}
///////////////////////////////////////////////////////////////////////////////////
// End FLTransporter
//////////////////////////////////////////////////////////////////////////////////
