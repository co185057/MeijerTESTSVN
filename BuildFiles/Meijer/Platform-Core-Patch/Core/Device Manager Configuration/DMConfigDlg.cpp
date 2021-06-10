// DMConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DMConfig.h"
#include "common.h"
#include "DMConfigDlg.h"
#include "DMOptionDlg.h"
#include "DeviceChildDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL DeviceReadRegString( HKEY hKey, 
                          LPCTSTR pszValue, 
                          CString& sData )
{
  DWORD   dwType;
  DWORD   dwLength;
  sData.Empty();

  BOOL bResult = ::RegQueryValueEx(hKey, pszValue, 0, &dwType, 0, &dwLength) == ERROR_SUCCESS;
  if ( bResult != FALSE && dwType == REG_SZ)
  {
    LPTSTR pStr = sData.GetBuffer( dwLength );
    bResult = ::RegQueryValueEx(hKey, pszValue, 0, &dwType, (PBYTE)pStr, &dwLength) == ERROR_SUCCESS;
    sData.ReleaseBuffer();
  }
  return bResult;
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
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMConfigDlg dialog

CDMConfigDlg::CDMConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDMConfigDlg::IDD, pParent),
   m_sWorkingDirectory ( _T("c:\\scot\\scripts") )
{
	//{{AFX_DATA_INIT(CDMConfigDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDMConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMConfigDlg)
	DDX_Control(pDX, IDC_LIST_DEVICE, m_listDevices);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDMConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CDMConfigDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(IDM_ABOUTBOX, OnAboutbox)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECT, OnButtonUnselect)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_ALL_NONE, OnButtonAllNone)
	ON_BN_CLICKED(IDC_OPTION, OnOption)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMConfigDlg message handlers

BOOL CDMConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_cfgMenu.LoadMenu( IDR_MENU_CONFIG );
	m_listDevices.SetItemHeight( 0, 20 );
	SetMenu( &m_cfgMenu );
	ReadRegistry();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDMConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDMConfigDlg::OnPaint() 
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
HCURSOR CDMConfigDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/////////////////////////////////////////////////////////////////////////////
// CDevMgrCtrl::ReadRegistry
//
// Read and store the Device Manager registry information. hKey1 is used for
// the device class name (e.g. "Scanner"). hKey2 is used for the device
// specific entries under the device class names (e.g. "NCRWedgeScanner.1",
// "NCRScanner.1").
//

BOOL DeviceReadCommands(  HKEY hClassKey, 
                          LPCTSTR sKeyName, 
                          CStringArray& aCommands )
{
  aCommands.RemoveAll();
  HKEY hKey;
  long nRC = ::RegOpenKeyEx( hClassKey, sKeyName, 0, KEY_READ, &hKey);
  if ( nRC == ERROR_SUCCESS )
  {
    int nIndex = 1;
    for( ;; )
    {
      CString sValueName;
      CString sCommand;
      sValueName.Format( _T("%d"), nIndex++ );
      if ( DeviceReadRegString( hKey, sValueName, sCommand ) != FALSE )
		  {
			  // check that line is not empty
        if (sCommand.GetLength()) aCommands.Add( sCommand );
		  }
      else
        break;
    } 
  }
  return aCommands.GetSize();
}


void CDMConfigDlg::AddDevicesToList( LPCTSTR sClassName )
{
  const CString sClassKey = CString( DM_CONFIG_REG_PATH ) + sClassName;
  HKEY hKeyClass;
  long nRC = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, sClassKey, 0, KEY_READ, &hKeyClass);
  if ( nRC == ERROR_SUCCESS )
  {
    CDeviceCopiableStringArray aCommands;
    CDeviceChildDialog* pNewDialog = new CDeviceChildDialog( &m_listDevices );
    pNewDialog->m_sClassName = sClassName;
    pNewDialog->Create( pNewDialog->IDD, &m_listDevices );

    CString sCurrent;
    DeviceReadRegString( hKeyClass, _T("CurrentState"), sCurrent );

    if (sCurrent != _T("9"))
    {
      pNewDialog->GetDlgItem( IDC_RADIO_NONE )->EnableWindow( DeviceReadCommands( hKeyClass, _T("None"), aCommands ) );
      m_MapNoneBatch.SetAt( CString(sClassName), aCommands );

      pNewDialog->GetDlgItem( IDC_RADIO_NULL )->EnableWindow( DeviceReadCommands( hKeyClass, _T("Null"), aCommands ) );
      m_MapNullBatch.SetAt( CString(sClassName), aCommands );

      pNewDialog->GetDlgItem( IDC_RADIO_ACTUAL )->EnableWindow( DeviceReadCommands( hKeyClass, _T("Actual"), aCommands ) );
      m_MapActualBatch.SetAt( CString(sClassName), aCommands );
    }
    else
    {
      DeviceReadCommands( hKeyClass, _T("None"), aCommands );
      pNewDialog->GetDlgItem( IDC_RADIO_NONE )->EnableWindow( TRUE );
      m_MapNoneBatch.SetAt( CString(sClassName), aCommands );

      pNewDialog->GetDlgItem( IDC_RADIO_NULL )->EnableWindow( FALSE );
      pNewDialog->GetDlgItem( IDC_RADIO_ACTUAL )->EnableWindow( FALSE );
      
    }
    
    // add a listbox item
		// Listbox does not have the LBS_HASSTRINGS style, so the
		// normal listbox string is used to store an RGB color

    m_listDevices.AddString( (LPCTSTR) pNewDialog);
   
    if (sCurrent != _T("9"))
    {
      pNewDialog->m_nSelection = _ttoi( sCurrent );
    }
    else
    {
      pNewDialog->m_nSelection = 0;
    }
    
    if ( pNewDialog->m_nSelection == 0 
      && pNewDialog->GetDlgItem( IDC_RADIO_NONE )->IsWindowEnabled() == FALSE )
      pNewDialog->m_nSelection++;
  
    if ( pNewDialog->m_nSelection == 1 
         && pNewDialog->GetDlgItem( IDC_RADIO_NULL )->IsWindowEnabled() == FALSE )
      pNewDialog->m_nSelection++;
  
    if ( pNewDialog->m_nSelection == 2 
      && pNewDialog->GetDlgItem( IDC_RADIO_ACTUAL )->IsWindowEnabled() == FALSE )
      pNewDialog->m_nSelection = -1;

    pNewDialog->UpdateData( FALSE );
    ::RegCloseKey( hKeyClass );
  }
  return;
}

void CDMConfigDlg::ReadRegistry()
{
	//***********************************************************************
	// Record Registry configuration of each supported device
	//***********************************************************************

  HKEY hKeyDevices;
	long nRC = ::RegOpenKeyEx(  HKEY_LOCAL_MACHINE, 
                              DM_CONFIG_REG_PATH, 
                              0, 
                              KEY_READ, 
                              &hKeyDevices);
  if ( nRC == ERROR_SUCCESS )
  {
    CString sWorkingDirectory;
    if ( DeviceReadRegString( hKeyDevices, _T("WorkingDirectory"), sWorkingDirectory ) != FALSE 
         && sWorkingDirectory.GetLength() >= sizeof( _T("C:\\") ) - 1 )
         m_sWorkingDirectory = sWorkingDirectory;
    DWORD nIndex = 0;
    TCHAR sClassName[50];
    while ( RegEnumKey( hKeyDevices, nIndex++, sClassName, sizeof( sClassName ) ) == ERROR_SUCCESS )
    {
      if(sClassName[0] != '-')
      AddDevicesToList( sClassName );
	  }
  }
  ::RegCloseKey( hKeyDevices );
  return;
}

void CDMConfigDlg::OnFileSave() 
{
  static _TCHAR BASED_CODE szFilter[] = _T("Batch Files (*.bat)|All Files (*.*)|*.*||");
  CFileDialog fileDialog( FALSE, 
                          _T("bat"), 
                          NULL, 
                          OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST, 
                          szFilter );
  int nResult = fileDialog.DoModal( );
  if ( nResult == IDOK )
  {
    try
    {
      CStdioFile fDest( fileDialog.GetPathName( ),  CFile::modeReadWrite | CFile::modeCreate );
      CString sTemp;
      sTemp.Format( _T("rem %s Custom Batch Configuration\n\n"), fileDialog.GetFileName() );
      fDest.WriteString( sTemp );
      sTemp.Format( _T("pushd %s\n"), m_sWorkingDirectory );
      fDest.WriteString( sTemp );
      for ( int nIndex = 0; nIndex < m_listDevices.GetCount(); nIndex++ )
      {
        CDeviceCopiableStringArray aCommands;
        CDeviceChildDialog* pDlg = (CDeviceChildDialog*)m_listDevices.GetItemData(nIndex);
        pDlg->UpdateData( TRUE );
        CString sLabel = pDlg->m_sClassName;
        if ( pDlg->m_nSelection == 0 )      // none
        m_MapNoneBatch.Lookup( sLabel, aCommands );
        else 
          if ( pDlg->m_nSelection == 1 )      // emulated
               m_MapNullBatch.Lookup( sLabel, aCommands );
          else
               m_MapActualBatch.Lookup( sLabel, aCommands );
        for ( int nIndex = 0; nIndex < aCommands.GetSize(); nIndex++ )
        {
          sTemp.Format( _T("%s\n"), aCommands[ nIndex ] );
          fDest.WriteString( sTemp );
        }
      }
      fDest.WriteString( _T("popd\n") );
    }
    catch ( CFileException* ex )
    {
      CString sCause;
      sCause.Format( _T("0x%x"), ex->m_cause ); 
      MessageBox( CString( _T("Write Failure: ") ) + sCause, _T("File Error") );
      ex->Delete();
    }
  }
  return;
}

void CDMConfigDlg::OnFileExit() 
{
	OnOK();
  return;	
}

void CDMConfigDlg::OnAboutbox() 
{
  CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CDMConfigDlg::OnButtonApply() 
{
  BOOL bOk = TRUE;
  for ( int nIndex = 0; nIndex < m_listDevices.GetCount(); nIndex++ )
  {
    CDeviceCopiableStringArray aCommands;
    CDeviceChildDialog* pDlg = (CDeviceChildDialog*)m_listDevices.GetItemData(nIndex);
    pDlg->UpdateData( TRUE );
    CString sLabel = pDlg->m_sClassName;
    if ( pDlg->m_nSelection == 0 )      // none
       m_MapNoneBatch.Lookup( sLabel, aCommands );
    else if ( pDlg->m_nSelection == 1 )      // emulated
       m_MapNullBatch.Lookup( sLabel, aCommands );
    else
       m_MapActualBatch.Lookup( sLabel, aCommands );
    if ( ExecuteCommand( sLabel, aCommands, m_sWorkingDirectory) == FALSE )
       bOk = FALSE;
  }
  if ( bOk != FALSE )
    MessageBox( _T("Configuration successfully updated."), _T("Batch Execution") );
  return;
}	

void CDMConfigDlg::OnButtonSelect() 
{
  for ( int nIndex = 0; nIndex < m_listDevices.GetCount(); nIndex++ )
  {
    CDeviceChildDialog* pDlg = (CDeviceChildDialog*)m_listDevices.GetItemData(nIndex);
    if ( pDlg->GetDlgItem( IDC_RADIO_NULL )->IsWindowEnabled() != FALSE )
    {
      pDlg->m_nSelection = 1;
      pDlg->UpdateData( FALSE );
    }
  }
  return;
}

void CDMConfigDlg::OnButtonAllNone() 
{
  for ( int nIndex = 0; nIndex < m_listDevices.GetCount(); nIndex++ )
  {
    CDeviceChildDialog* pDlg = (CDeviceChildDialog*)m_listDevices.GetItemData(nIndex);
    if ( pDlg->GetDlgItem( IDC_RADIO_NONE )->IsWindowEnabled() != FALSE )
    {
      pDlg->m_nSelection = 0;
      pDlg->UpdateData( FALSE );
    }
  }
  return;
}

void CDMConfigDlg::OnButtonUnselect() 
{
  for ( int nIndex = 0; nIndex < m_listDevices.GetCount(); nIndex++ )
  {
    CDeviceChildDialog* pDlg = (CDeviceChildDialog*)m_listDevices.GetItemData(nIndex);
    if ( pDlg->GetDlgItem( IDC_RADIO_ACTUAL )->IsWindowEnabled() != FALSE )
    {
      pDlg->m_nSelection = 2;
      pDlg->UpdateData( FALSE );
    }
  }
  return;
}

void CDMConfigDlg::OnOption() 
{
	// TODO: Add your control notification handler code here
	CDMOptionDlg dlg;
	int nResponse = dlg.DoModal();
  m_listDevices.ResetContent();
  ReadRegistry();
}

void CDMConfigDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
  CDialog::OnCancel();
}
