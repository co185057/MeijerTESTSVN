// PopupDlg.cpp : implementation file
//
// Change History:
//
// 07/2001	tn.		SCOT 2.4 - Add SMA support for RAP station
// 07/2001	tn.		SCOT 2.4 - Add remote utility support for RAP station
//
//************************************************************************

#include "stdafx.h"
#include "Popup.h"
#include "PopupDlg.h"
#include "afxtempl.h"
#include "StdFastlane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STEAL_FOCUS _T("/FOCUS") // Text to be used as the command line for enabling steal focus

static int currPos = 1;
BOOL bOK=FALSE;  //FL 4.0 Patch B Support for new country
BOOL bStealFocus	= FALSE;  
/////////////////////////////////////////////////////////////////////////////
// CPopupDlg dialog

CPopupDlg::CPopupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupDlg)
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOAD, m_ctrlProgress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPopupDlg, CDialog)
	//{{AFX_MSG_MAP(CPopupDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON, OnButton)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

bool IsFileUnicode(LPCTSTR lpInFile)
{
    TCHAR sInFile[_MAX_PATH] = _T("");
    GetPartitionPath( lpInFile, sInFile );
    DWORD	  dwSize;

	HANDLE	  _hFile=NULL;
	HANDLE	  _hMapFile = NULL;
	PBYTE	  _Buff;

    dwSize = 0L; /* use full size for input file	*/
					/* MapFile returns actual size	*/
			       

    /* calculate an upper bound for the size of the output file and
     * memorymap it.
     */

	_Buff = NULL;

    /* Open a file for read, or create a file for read/write */

    _hFile = (HANDLE) CreateFile(	sInFile,
	 							    GENERIC_READ,
								    0,              // exclusive access
								    0L,             // no security 
								    OPEN_EXISTING,
								    FILE_ATTRIBUTE_NORMAL,
								    0L);            // no template file 
    if( (DWORD)_hFile == 0xFFFFFFFF) 
	{
		TRACE(_T("Fail to open the file %s \n"), sInFile);
		return false;
	}

    /* Create a File mapping that is either read or read/write */

    _hMapFile = CreateFileMapping(	_hFile,
									NULL,		// no security attributes
									PAGE_READONLY,
									0L,		// Hi size DWORD
									dwSize, 	// Lo size DWORD
									NULL);        // no name 
    if (!_hMapFile) 
	{
		TRACE(_T("Fail to CreateFileMapping for the file %s \n"), sInFile);
		CloseHandle(_hFile);			// close the file
		return false;
	}


    /* Create a Mapped View that is either read or read/write */

    _Buff = (PBYTE) MapViewOfFile(	_hMapFile,
									FILE_MAP_READ,
									0L,		    // zero offset 
									0L,
									dwSize);
    if (!_Buff) 
	{
		TRACE(_T("Fail to MapViewOfFile for the file %s \n"), sInFile);
		if (!_hMapFile) 
			CloseHandle(_hMapFile);       // close the handle to the file mapping 
		CloseHandle(_hFile);			// close the file
		return false;
	}

    /* retain the size of the file opened */

    dwSize = GetFileSize( _hFile, NULL);
	bool ret = false;

    /* format and print error messages from GetLastError */
    if( _Buff && dwSize )
	{
		 if (IsTextUnicode(_Buff, dwSize, NULL ))
			 ret = true;
		 else
			 ret = false;
		 
	}

	if (_Buff) 
		UnmapViewOfFile(_Buff);     //unmap the output file
	if (_hMapFile) 
		CloseHandle(_hMapFile);       // close the handle to the file mapping 
	if (_hFile)
		CloseHandle(_hFile);			// close the file

	return ret;

}

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg message handlers

BOOL CPopupDlg::OnInitDialog()
{
	bool b_SCOTC = false;
	int x = 0;
	int y = 0;


	CDialog::OnInitDialog();
   	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    // TAR 226471 Screen 
	// If the sytem is a c-series, then display popup on the 7401.
	TCHAR szSCOTSeries[20];
    CString csIni = _T("%APP_DRIVE%\\scot\\config\\Switch.ini");
    TCHAR sIni[_MAX_PATH] = _T("");
    GetPartitionPath( csIni, sIni );
    csIni = sIni;
    GetPrivateProfileString (_T("ProjectType"),_T("Project"),_T("Project"), szSCOTSeries,
    sizeof (szSCOTSeries),csIni);
    //GetPrivateProfileString (_T("ProjectType"),_T("Project"),_T("Project"), szSCOTSeries,
    //sizeof (szSCOTSeries),_T("c:\\scot\\config\\Switch.ini"));

    if(_tcsicmp(szSCOTSeries, _T("SCOT-C"))==0)   // Project = SCOT-C (ASCOT)
        b_SCOTC = true;

    _TINT xScreen = GetSystemMetrics(SM_CXFULLSCREEN);
    _TINT yScreen = GetSystemMetrics(SM_CYSCREEN);
    if (xScreen > 800 && !b_SCOTC) 
    {
		CRect rect;
        this->GetDlgItem(IDC_BUTTON)->GetWindowRect(&rect);
        this->GetDlgItem(IDC_BUTTON)->MoveWindow(rect.left + rect.left*.28,
			                                     rect.top + rect.top*.28,
												 rect.Width() + rect.Width()*.28,
												 rect.Height() + rect.Height()*.28);


		CRect rect3;
        this->GetDlgItem(IDC_PROGRESS_DOWNLOAD)->GetWindowRect(&rect3);
        this->GetDlgItem(IDC_PROGRESS_DOWNLOAD)->MoveWindow(rect3.left + rect3.left*.28,
			                                     rect3.top + rect3.top*.28,
												 rect3.Width() + rect3.Width()*.28,
												 rect3.Height() + rect3.Height()*.28);

        CRect rect2;
		this->GetDlgItem(IDC_STATIC)->GetWindowRect(&rect2);
        this->GetDlgItem(IDC_STATIC)->MoveWindow(rect2.left + rect2.left*.28,
			                                     rect2.top  + rect2.top*.28, 
			                                     rect2.Width() + rect2.Width()*.28, 
												 rect2.Height()+ rect2.Height()*.28);
    }
	else if (b_SCOTC)
    {
       x = 800;
	   y = 0;
    }
	// -------------------------------------------------
	// Get Command Line
	// -------------------------------------------------
	LPTSTR lpCmdLine = GetCommandLine();

	// Parse Command Line
	CString sCMDLine;
	sCMDLine = lpCmdLine;
	sCMDLine.TrimLeft();
	sCMDLine.TrimRight();
	BOOL bSetMsg;
	BOOL bSetMsgFromFile;
	BOOL bProgressBar;

	BOOL bSmallScreen = FALSE; //RFC #282631

	int nMaxRange=1000;

	int argc=0;
	// 10 args Max...
	TCHAR argv[20][255];

	argv[0][0]=0x00;

	int iFindSpace=999;
	CString sTempString;
		
	sTempString=sCMDLine;
	while (iFindSpace>0)
	{
		iFindSpace = sTempString.FindOneOf(_T(" "));
		if (iFindSpace>0)
			_tcscpy(argv[argc], sTempString.Left(iFindSpace));					
		else
			_tcscpy(argv[argc], sTempString);					
		sTempString = sCMDLine.Right(sTempString.GetLength()-iFindSpace-1);								
		argc++;		
		if (argc>19) iFindSpace=0;
	}

	bSetMsg=FALSE;
	bSetMsgFromFile		= FALSE;
	bProgressBar		= FALSE;
	BOOL bPending		= FALSE;
	BOOL bConfirm		= FALSE;
	
	

	BOOL bKill = FALSE;
	CArray <CString, CString &> sInputFiles;
	CString strMsgTxt;
	CString strPendingTxt;
	CString strConfirmTxt;

	if (argc>=2)
	{
		for (int k=1; k < argc; k++)
		{
			if (_tcsicmp(argv[k],_T("/MSG"))==0) 
			{
				bSetMsg=TRUE;
			}
			else if (_tcsicmp(argv[k],_T("/f"))==0) 
			{	
				bSetMsgFromFile=TRUE;
				//for (int j=k+1; j<argc; j++)
				//{
					//CString tempArgv(argv[j]);
					//sInputFiles.Add(tempArgv);
					//k++;
				//}
				
			}
			else if (_tcsicmp(argv[k],_T("/s"))==0)   //+RFC #282631
			{
				bSmallScreen =  TRUE;
			}										  //-RFC #282631	
			else if (_tcsicmp(argv[k],_T("/p"))==0) 
			{
				bProgressBar=TRUE;
				CString tmp(argv[k+1]);
				if (!tmp.IsEmpty())
					nMaxRange = _ttoi(argv[k+1]);
				k++;
				
			}
			//FL 4.0 Patch B Support for new country
			else if (_tcsicmp(argv[k],_T("/k"))==0) 
			{
				bOK=TRUE;
			}
			//-FL 4.0 Patch B Support for new country
			else if (bSetMsgFromFile)
			{
                TCHAR tempArg[ _MAX_PATH ] = _T("");
                GetPartitionPath( argv[k], tempArg );
                CString tempArgv(tempArg);
                //CString tempArgv(argv[k]);
                sInputFiles.Add(tempArgv);
			}
			else if (_tcsicmp(argv[k],_T("PENDING"))==0) 
				bPending = TRUE;
			else if (_tcsicmp(argv[k],_T("CONFIRMATION"))==0) 
				bConfirm = TRUE;
			else if (_tcsicmp(argv[k],_T("KILL"))==0) 
				bKill = TRUE;
			else if (_tcsicmp(argv[k], STEAL_FOCUS )==0) 
				bStealFocus = TRUE;
			else if (bSetMsg)
			{
				strMsgTxt += argv[k];
				strMsgTxt += _T(" ");
			}
			else if (bPending)
			{
				strPendingTxt += argv[k];
				strPendingTxt += _T(" ");
			}
			else if (bConfirm)
			{
				strConfirmTxt += argv[k];
				strConfirmTxt += _T(" ");
			}
		}
	}
	
	if (bProgressBar)
	{
		SetTimer( 1, 1000, NULL ); // one seconds
		m_ctrlProgress.SetRange( 0, nMaxRange );
		currPos = 1;
	}

	if (bStealFocus) 
		// Set the timer only if "Steal Focus" functionality has been enabled 
	{
		SetTimer( 2, 2000, NULL ); // two seconds	
	}

	if (bSetMsgFromFile)
	{
		CString strTxt = _T("");
		CStdioFile cs1;
		CFileException  ex;
		BOOL bOpen = false;
		bool bUnicodeFile= false;
	 for (int k=0; k < sInputFiles.GetSize(); k++)
	 {
		if (!IsFileUnicode(sInputFiles[k]))
			bOpen=cs1.Open(sInputFiles[k],CFile::modeRead | CFile::typeText, &ex);
		else
		{
			bUnicodeFile = true;
			bOpen=cs1.Open(sInputFiles[k],CFile::modeRead | CFile::typeBinary, &ex);
		}

		if (!bOpen)
		{
		  TCHAR szError[1024];
		  ex.GetErrorMessage(szError, 1024);
          TRACE(szError);
		}
		else
		{
			CString csLine;
			while (cs1.ReadString(csLine))
			{
				//Tar 249419, strip off book-mark-order for unicode files
				if ((bUnicodeFile == true) && !(csLine.IsEmpty()))
				{
					if ( csLine[0] == 0xFEFF) 
						csLine = csLine.Mid(1);
				}
				strTxt = strTxt + csLine;
			} 
			strTxt = strTxt + _T("  ");
		}
		cs1.Close();
	 }

		this->SetDlgItemText(IDC_STATIC, strTxt);
		this->GetDlgItem(IDC_BUTTON)->SetFocus();

		if (!bProgressBar)
		{
			this->GetDlgItem(IDC_PROGRESS_DOWNLOAD)->ShowWindow(SW_HIDE);
		}
		EnableWindow(true);
		
		if ( bSmallScreen )		//+RFC #282631	
		{
			CWnd *m_pCWnd = AfxGetApp()->m_pMainWnd;
			xScreen = GetSystemMetrics(SM_CXFULLSCREEN) * .65;
			yScreen = GetSystemMetrics(SM_CYSCREEN) * .4;
			_TINT xScreenStatic = xScreen * .86;
			_TINT yScreenStatic = yScreen * .57;
			if ( !b_SCOTC )
			{
				m_pCWnd->SetWindowPos( &wndTopMost, 0, 0, xScreen, yScreen, SWP_SHOWWINDOW );
			}else
			{
				m_pCWnd->SetWindowPos( &wndTopMost, xScreen, 0, xScreen, yScreen, SWP_SHOWWINDOW );
			}
			
			CRect rect4;
			m_pCWnd->GetClientRect( &rect4 );
			this->GetDlgItem(IDC_STATIC)->MoveWindow(((rect4.Width() - xScreenStatic ) / 2), ((rect4.Height() - yScreenStatic )/2) , xScreenStatic , yScreenStatic );
			m_pCWnd->CenterWindow( NULL );
			
			CFont *cFont = new CFont();
			LOGFONT lf;
			this->GetDlgItem(IDC_STATIC)->GetFont()->GetLogFont( &lf );
			CDC *dc = this->GetDlgItem(IDC_STATIC)->GetWindowDC();
			lf.lfHeight = -MulDiv(GetSystemMetrics(SM_CXFULLSCREEN) * .03, dc->GetDeviceCaps(LOGPIXELSY), 72);
			if (cFont->CreateFontIndirect( &lf ) != NULL )
			{
				this->GetDlgItem(IDC_STATIC)->SetFont( cFont );
			}

		}else//-RFC #282631			
		{
			AfxGetApp()->m_pMainWnd->SetWindowPos(&wndTopMost, x, y,xScreen,yScreen,SWP_SHOWWINDOW);
		}
		//+FL 4.0 Patch B Support for new country
		if (bOK)  // show OK button
		{
			if (strConfirmTxt.IsEmpty()) 
					strConfirmTxt = _T("The Device CF9520 Coin Acceptor is too old and not supported for Mexico Currency,please get the device replaced. Press OK, the Application will load but failed due to this device issue!");
			strConfirmTxt=strTxt;
			this->SetDlgItemText(IDC_STATIC, strConfirmTxt);
			if (!bProgressBar)
			{
				this->GetDlgItem(IDC_PROGRESS_DOWNLOAD)->ShowWindow(SW_HIDE);
			}
			this->GetDlgItem(IDC_BUTTON)->ShowWindow(SW_SHOW);
			this->GetDlgItem(IDC_BUTTON)->SetFocus();
			EnableWindow(true);			
		}
		//-FL 4.0 Patch B Support for new country
		return FALSE;  // return TRUE  unless you set the focus to a control
	}
	if (bSetMsg)
	{
		
		this->SetDlgItemText(IDC_STATIC, strMsgTxt);
		this->GetDlgItem(IDC_BUTTON)->SetFocus();
		if (!bProgressBar)
		{
			this->GetDlgItem(IDC_PROGRESS_DOWNLOAD)->ShowWindow(SW_HIDE);
		}
		EnableWindow(true);

		AfxGetApp()->m_pMainWnd->SetWindowPos(&wndTopMost, x, y,xScreen,yScreen,SWP_SHOWWINDOW);
		return FALSE;  // return TRUE  unless you set the focus to a control
	}

	// -end---------------------------------------------------------------------------------

	// tn. SCOT 2.4 - supporting remote utility on RAP station
	HKEY  hMachineKey = HKEY_LOCAL_MACHINE;
	HKEY  hNamesKey;
	BOOL  bRAP_PC = FALSE; 
	TCHAR szRAPTerminal[100];

	// Open The loadcontroller key in registry
	if (RegOpenKeyEx(hMachineKey, _T("Software\\NCR\\SCOT\\CurrentVersion\\Load Controller"), 
                  0, KEY_QUERY_VALUE, &hNamesKey) == ERROR_SUCCESS)
	{
		DWORD dwDataSize = sizeof (szRAPTerminal), dwDataType;
		dwDataSize = sizeof (szRAPTerminal);
		if(RegQueryValueEx(hNamesKey, _T("RAPTerminal"), 0, &dwDataType, 
                     (LPBYTE) szRAPTerminal, &dwDataSize) != ERROR_SUCCESS)
		{
			 _tcscpy(szRAPTerminal, _T("No"));
		}

		if (!_tcsnicmp(szRAPTerminal, _T("YES"), 3))
		{
			// set flag to run Popup.exe on RAP station 
			bRAP_PC = TRUE;
		}

		RegCloseKey(hNamesKey);
        RegCloseKey(hMachineKey);
	}

	// tn. SCOT 2.4 - supporting SMA
	

	if (bConfirm)  // popup confirmation screen
	{
		if (strConfirmTxt.IsEmpty()) 
				strConfirmTxt = _T("Please press Reboot button after all machines are updated!");
		this->SetDlgItemText(IDC_STATIC, strConfirmTxt);
		if (!bProgressBar)
		{
			this->GetDlgItem(IDC_PROGRESS_DOWNLOAD)->ShowWindow(SW_HIDE);
		}
		this->GetDlgItem(IDC_BUTTON)->ShowWindow(SW_SHOW);
		this->GetDlgItem(IDC_BUTTON)->SetFocus();
		EnableWindow(true);
	}

	else if (bPending)  // popup pending screen
	{
		if (strPendingTxt.IsEmpty()) 
			strPendingTxt = _T("Update pending, please wait!");

		if (!bProgressBar)
		{
			this->GetDlgItem(IDC_PROGRESS_DOWNLOAD)->ShowWindow(SW_HIDE);
		}
		this->SetDlgItemText(IDC_STATIC, strPendingTxt);
		this->GetDlgItem(IDC_BUTTON)->SetFocus();
		EnableWindow(true);
	}

	else if (bKill) // kill popupscreen
	{
		EnableWindow(true);
		if (!bProgressBar)
		{
			this->GetDlgItem(IDC_PROGRESS_DOWNLOAD)->ShowWindow(SW_HIDE);
		}
		OnOK();
	}

	else
	// shutdown SCOT (or RAP) App remotely
	{
		TCHAR szSCOTShutDown[100], szRAPShutDown[100];
        CString csIni = _T("%APP_DRIVE%\\scot\\config\\SENDScot.ini");
        TCHAR sIni[_MAX_PATH] = _T("");
        GetPartitionPath( csIni, sIni );
        csIni = sIni;
        GetPrivateProfileString (_T("Messages"),_T("SCOTShutdownPopup"),_T("SCOTShutdownPopup"), szSCOTShutDown,
	        sizeof(szSCOTShutDown),csIni);
        csIni = _T("%APP_DRIVE%\\scot\\config\\SENDScot.ini");
        GetPartitionPath( csIni, sIni );
        csIni = sIni;
        GetPrivateProfileString (_T("Messages"),_T("RAPShutdownPopup"),_T("RAPShutdownPopup"), szRAPShutDown,
	        sizeof(szRAPShutDown),csIni);
//         GetPrivateProfileString (_T("Messages"),_T("SCOTShutdownPopup"),_T("SCOTShutdownPopup"), szSCOTShutDown,
	//         sizeof(szSCOTShutDown),_T("c:\\scot\\config\\SENDScot.ini"));
//         GetPrivateProfileString (_T("Messages"),_T("RAPShutdownPopup"),_T("RAPShutdownPopup"), szRAPShutDown,
	//         sizeof(szRAPShutDown),_T("c:\\scot\\config\\SENDScot.ini"));
		if (bRAP_PC)
			this->SetDlgItemText(IDC_STATIC, szRAPShutDown);
		else
			this->SetDlgItemText(IDC_STATIC, szSCOTShutDown);
		this->GetDlgItem(IDC_BUTTON)->SetFocus();
		if (!bProgressBar)
		{
			this->GetDlgItem(IDC_PROGRESS_DOWNLOAD)->ShowWindow(SW_HIDE);
		}
		EnableWindow(false);
	}


    // TAR 226471
    if(b_SCOTC)       // Project = SCOT-C (ASCOT)
		AfxGetApp()->m_pMainWnd->SetWindowPos(&wndTopMost, 800,0,800,600,SWP_SHOWWINDOW);
	else
		AfxGetApp()->m_pMainWnd->SetWindowPos(&wndTopMost, x, y, xScreen,yScreen,SWP_SHOWWINDOW);

	return FALSE;    // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPopupDlg::OnPaint() 
{
	 
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	} 
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPopupDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPopupDlg::OnButton() 
{
	    HANDLE hToken;
	    TOKEN_PRIVILEGES oTokentPriv;
		//FL 4.0 Patch B Support for new country
		if(bOK)
		{

			OnOK();
		}
		//FL 4.0 Patch B Support for new country
		if (!bOK) //FL 4.0 Patch B Support for new country
		{
			if (!OpenProcessToken(GetCurrentProcess(), 
						   TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
			{
				AfxMessageBox(_T("Fail to open process"));
				OnOK();
			}

			if(!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
							   &oTokentPriv.Privileges[0].Luid))
			{
				AfxMessageBox(_T("Fail to LookupPrivilegeValue"));
				OnOK();
			}

			oTokentPriv.PrivilegeCount = 1;
			oTokentPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			if(!AdjustTokenPrivileges(
				hToken, FALSE, &oTokentPriv, 0, (PTOKEN_PRIVILEGES) NULL, 0))
			{
				AfxMessageBox(_T("Fail to AdjustTokenPrivileges"));
				OnOK();
			}

			if(!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, NULL))
			{
				AfxMessageBox(_T("Fail to ExitWindowsEx"));
				OnOK();
			}
			else  
			{
			  OnOK();
			}
		}//FL 4.0 Patch B Support for new country
}


void CPopupDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
		case 1:
		{
			currPos ++;
			m_ctrlProgress.SetPos( currPos );
		
		break;
		}
		case 2:
		{
			// Get the foreground window. Check and see if it is our window or not.
			// If we have the focus then kill the timer.
			// If we dont have the focus then check to see if it is a console window. 
			// If it is a console window then minimize it so that we get the focus. 
			// Otherwise, attach input processing mechanism to the thread of the window which has the 
			// focus so that we can steal the focus. After getting the focus detach 
			// the input processin mechanism with that thread

			HWND hWnd=::GetForegroundWindow();

			CWnd* wndPtrpopup= AfxGetMainWnd();
			if(m_hWnd != hWnd )		
			{
				TCHAR strClass[MAX_PATH];
				bool bCmdWindow = false;

				//CWnd* pActiveWnd = FromHandle(hWnd);
				GetClassName(hWnd, strClass, sizeof(strClass));
				if (0 == _tcsicmp(strClass, _T("ConsoleWindowClass")))
				{
					bCmdWindow = true;
				}

				if(bCmdWindow)
				{
					::PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
				}
				else
				{

					AttachThreadInput(
									GetWindowThreadProcessId(hWnd,NULL),
									GetCurrentThreadId(),
									TRUE);

					wndPtrpopup->SetActiveWindow();
					wndPtrpopup->SetForegroundWindow();
					wndPtrpopup->SetFocus(); //Just playing safe

					


					AttachThreadInput(	
									GetWindowThreadProcessId(hWnd,NULL),
									GetCurrentThreadId(),
									FALSE);
				}
				
			}
			else
			{
				// Popup has the focus. Kill the timer. The timer will be started once the 
				//window become inactive 
				KillTimer(2);
			}
			break;
		
		}
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CPopupDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{

		case WM_ACTIVATEAPP:
		if((wParam==FALSE) ) 
		//If the application become inactive then start the timer
		{
			if (bStealFocus) 
			// Start the timer only if enabled by the /FOCUS commandline parameter
			{
				SetTimer( 2, 2000, NULL ); // two seconds	
			}
			
		}
		break;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}
