// dftrans.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <process.h>

#include "dftrans.h"

#include "MainFrm.h"
#include "dftransDoc.h"
#include "dftransView.h"
#include "OptionsDlg.h"
#include "VersionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_VIEWER          "notepad.exe"
#define DEFAULT_APPDEATH_TEXT   "!!!  SCOT App. restarted after Death  !!!"
#define DEFAULT_APPRESTART_TEXT "<<<  SCOT App. Reloaded  >>>"
#define DEFAULT_TRANSEND_TEXT   "---  End of SCOT Transaction  ---"
#define DEFAULT_LANECLOSED_TEXT "+++  SCOT Lane was Closed  +++"

#define DEFAULT_APPDEATH_COLOR    (RGB(255,0,0))
#define DEFAULT_APPRESTART_COLOR  (RGB(0,180,0))
#define DEFAULT_TRANSEND_COLOR    (RGB(0,0,0))
#define DEFAULT_LANECLOSED_COLOR  (RGB(0,0,255))

/////////////////////////////////////////////////////////////////////////////
// CDftransApp

BEGIN_MESSAGE_MAP(CDftransApp, CWinApp)
	//{{AFX_MSG_MAP(CDftransApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_REFRESH, OnFileRefresh)
	ON_UPDATE_COMMAND_UI(ID_FILE_REFRESH, OnUpdateFileRefresh)
	ON_COMMAND(ID_EDIT_OPTIONS, OnEditOptions)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDftransApp construction

CDftransApp::CDftransApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
  m_pListCtrl = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDftransApp object

CDftransApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDftransApp initialization

BOOL CDftransApp::InitInstance()
{
  // __asm int 3;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  CString strDate;
  strDate.LoadString(61593);

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("NCR\\SCOT\\CurrentVersion"));

  UpdateRegOptions(TRUE);

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	m_pDocTemplate = pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDftransDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDftransView));
	AddDocTemplate(pDocTemplate);

  EnableShellOpen();   
  RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

  if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
  {
    if (BaseName(cmdInfo.m_strFileName).Find(".stf") == -1)
    {
      LaunchDefaultViewer(cmdInfo.m_strFileName);
      return FALSE;
    }
  }
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
  m_pMainWnd->DragAcceptFiles();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_AppVersion;
	CString	m_csCopyright;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_AppVersion = _T("");
	m_csCopyright = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_APPVER, m_AppVersion);
	DDX_Text(pDX, IDC_COPYRIGHT, m_csCopyright);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDftransApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDftransApp commands

void CDftransApp::OnFileRefresh() 
{
  POSITION pos = m_pDocTemplate->GetFirstDocPosition();
  CDocument* pDoc = m_pDocTemplate->GetNextDoc(pos);
  CString docPath = pDoc->GetPathName();

  m_pDocTemplate->OpenDocumentFile(docPath);

  // Now call view's OnFileRefresh so the view can ensure the last
  // items in the list are visible.

  pos = pDoc->GetFirstViewPosition();
  CDftransView* pView = (CDftransView*) pDoc->GetNextView(pos);
  pView->OnFileRefresh();

}

void CDftransApp::OnUpdateFileRefresh(CCmdUI* pCmdUI) 
{
  POSITION pos = m_pDocTemplate->GetFirstDocPosition();
  CDocument* pDoc = m_pDocTemplate->GetNextDoc(pos);
  CString docPath = pDoc->GetPathName();

  pCmdUI->Enable((docPath.IsEmpty()) ? FALSE : TRUE);	
}

CString CDftransApp::BaseName(CString & path)
{
  return path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
}

CString CDftransApp::FileExtension(CString & path)
{
  return path.Right(path.GetLength() - path.ReverseFind('.') - 1);

}

void CDftransApp::OnEditOptions() 
{
  COptionsDlg dlg;
  
  dlg.m_LogFileViewer     = m_LogFileViewer;
  dlg.m_BakFileViewer     = m_BakFileViewer;
  dlg.m_MinPrintWarnItems = m_MinPrintWarnItems;

  dlg.m_csAppDeathText    = m_csAppDeathText;
  dlg.m_csAppRestartText  = m_csAppRestartText;
  dlg.m_csLaneClosedText  = m_csLaneClosedText;
  dlg.m_csTransEndText    = m_csTransEndText;

  dlg.m_AppDeathColor     = m_AppDeathColor;
  dlg.m_AppRestartColor   = m_AppRestartColor;
  dlg.m_LaneClosedColor   = m_LaneClosedColor;
  dlg.m_TransEndColor     = m_TransEndColor;

  if (dlg.DoModal() == IDOK)
  {
    m_LogFileViewer     = dlg.m_LogFileViewer;
    m_BakFileViewer     = dlg.m_BakFileViewer;
    m_MinPrintWarnItems = dlg.m_MinPrintWarnItems;

    m_csAppDeathText    = dlg.m_csAppDeathText;
    m_csAppRestartText  = dlg.m_csAppRestartText;
    m_csLaneClosedText  = dlg.m_csLaneClosedText;
    m_csTransEndText    = dlg.m_csTransEndText;

    m_AppDeathColor     = dlg.m_AppDeathColor;
    m_AppRestartColor   = dlg.m_AppRestartColor;
    m_LaneClosedColor   = dlg.m_LaneClosedColor;
    m_TransEndColor     = dlg.m_TransEndColor;

    UpdateRegOptions(FALSE);

    if (m_pListCtrl)
    {
      int nTopItem = m_pListCtrl->GetTopIndex();
      m_pListCtrl->RedrawItems(nTopItem, nTopItem + m_pListCtrl->GetCountPerPage());
      m_pListCtrl->UpdateWindow();
    }
  }
}

void CDftransApp::UpdateRegOptions(BOOL bFromReg)
{
  if (bFromReg)
  {
    m_LogFileViewer     = GetProfileString("Settings", "LogFileViewer",     DEFAULT_VIEWER); 
    m_BakFileViewer     = GetProfileString("Settings", "BakFileViewer",     DEFAULT_VIEWER); 
    m_MinPrintWarnItems = GetProfileInt   ("Settings", "MinPrintWarnItems", 1000);

    m_csAppDeathText    = GetProfileString("Settings", "AppDeathText",   DEFAULT_APPDEATH_TEXT);
    m_csAppRestartText  = GetProfileString("Settings", "AppRestartText", DEFAULT_APPRESTART_TEXT);
    m_csTransEndText    = GetProfileString("Settings", "TransEndText",   DEFAULT_TRANSEND_TEXT);
    m_csLaneClosedText  = GetProfileString("Settings", "LaneClosedText", DEFAULT_LANECLOSED_TEXT);

    m_AppDeathColor   = (COLORREF) GetProfileInt("Settings", "AppDeathColor",   DEFAULT_APPDEATH_COLOR);
    m_AppRestartColor = (COLORREF) GetProfileInt("Settings", "AppRestartColor", DEFAULT_APPRESTART_COLOR);
    m_TransEndColor   = (COLORREF) GetProfileInt("Settings", "TransEndColor",   DEFAULT_TRANSEND_COLOR);
    m_LaneClosedColor = (COLORREF) GetProfileInt("Settings", "LaneClosedColor", DEFAULT_LANECLOSED_COLOR);
  }
  else
  {
    WriteProfileString("Settings", "LogFileViewer",     m_LogFileViewer);
    WriteProfileString("Settings", "BakFileViewer",     m_BakFileViewer);
    WriteProfileInt   ("Settings", "MinPrintWarnItems", m_MinPrintWarnItems);

    WriteProfileString("Settings", "AppDeathText",      m_csAppDeathText);
    WriteProfileString("Settings", "AppRestartText",    m_csAppRestartText);
    WriteProfileString("Settings", "TransEndText",      m_csTransEndText);
    WriteProfileString("Settings", "LaneClosedText",    m_csLaneClosedText);

    WriteProfileInt   ("Settings", "AppDeathColor",     (UINT) m_AppDeathColor);
    WriteProfileInt   ("Settings", "AppRestartColor",   (UINT) m_AppRestartColor);
    WriteProfileInt   ("Settings", "TransEndColor",     (UINT) m_TransEndColor);
    WriteProfileInt   ("Settings", "LaneClosedColor",   (UINT) m_LaneClosedColor);
  }
}

void CDftransApp::LaunchDefaultViewer(CString & fileName)
{
  CString cmd;
  CString ext = FileExtension(fileName);

  if (ext.CompareNoCase("STF") == 0)
    cmd.Format("%s \"%s\"", m_LogFileViewer, fileName);
  else if (ext.CompareNoCase("BAK") == 0)
    cmd.Format("%s \"%s\"", m_BakFileViewer, fileName);

  WinExec(cmd, SW_SHOW);

  return;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CVersionInfo verInfo;
  if (verInfo.Initialize(AfxGetInstanceHandle()))
  {
    CString appVersion = verInfo.GetVersionValue(VER_FILE_VER);
    int len = appVersion.GetLength();
    if (len > 0)
    {
      char* ptr = appVersion.GetBuffer(0);
      for (int i = 0; i < len; i++, ptr++)
        if (*ptr == ',')
          *ptr = '.';
    }

    m_AppVersion = "Version:  " + appVersion;
    m_csCopyright = verInfo.GetVersionValue(VER_LEGAL_COPYRIGHT);

    UpdateData(FALSE);
  }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDftransApp::SetListCtrl(CListCtrl *pListCtrl)
{
  m_pListCtrl = pListCtrl;
}
