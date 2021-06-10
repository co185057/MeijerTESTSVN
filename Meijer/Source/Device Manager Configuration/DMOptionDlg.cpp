// DMPlatformDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "common.h"
#include "DMOptionDlg.h"
#include "OptionChildDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL OptionReadRegString( HKEY hKey, 
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
// CDMOptionDlg dialog


CDMOptionDlg::CDMOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDMOptionDlg::IDD, pParent),
	m_sWorkingDirectory ( _T("c:\\scot\\scripts") )
{
	//{{AFX_DATA_INIT(CDMOptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	 	
}


void CDMOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMOptionDlg)
    DDX_Control(pDX, IDC_LIST_OPTION, m_listOptions);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDMOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CDMOptionDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDMOptionDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// CDMOptionDlg::ReadRegistry
//
// Read and store the Device Manager registry information. hKey1 is used for
// the device class name (e.g. "Scanner"). hKey2 is used for the device
// specific entries under the device class names (e.g. "NCRWedgeScanner.1",
// "NCRScanner.1").
//

BOOL OptionReadCommands(HKEY hClassKey, 
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
      if ( OptionReadRegString( hKey, sValueName, sCommand ) != FALSE )
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

void CDMOptionDlg::AddDevicesToList( LPCTSTR sClassName )
{
  const CString sClassKey = CString( DM_CONFIG_REG_PATH ) + CString( DM_CONFIG_OPTION ) + _T("\\") + sClassName;
  HKEY hKeyClass;
  long nRC = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, sClassKey, 0, KEY_READ, &hKeyClass);
  if ( nRC == ERROR_SUCCESS )
  {
    COptionCopiableStringArray aCommands;
    COptionChildDialog* pNewDialog = new COptionChildDialog( &m_listOptions );
    pNewDialog->m_sClassName = sClassName;
    pNewDialog->Create( pNewDialog->IDD, &m_listOptions );
    
    pNewDialog->GetDlgItem( IDC_RADIO_NO )->EnableWindow( OptionReadCommands( hKeyClass, _T("No"), aCommands ) );
    m_MapNoBatch.SetAt( CString(sClassName), aCommands );
    
    pNewDialog->GetDlgItem( IDC_RADIO_YES )->EnableWindow( OptionReadCommands( hKeyClass, _T("Yes"), aCommands ) );
    m_MapYesBatch.SetAt( CString(sClassName), aCommands );
    
    // add a listbox item
		// Listbox does not have the LBS_HASSTRINGS style, so the
		//  normal listbox string is used to store an RGB color
    m_listOptions.AddString( (LPCTSTR) pNewDialog);
    
    CString sCurrent;
    OptionReadRegString( hKeyClass, _T("CurrentState"), sCurrent );

    pNewDialog->m_nSelection = _ttoi( sCurrent );

    if ( pNewDialog->m_nSelection == 0 
      && pNewDialog->GetDlgItem( IDC_RADIO_NO )->IsWindowEnabled() == FALSE )
      pNewDialog->m_nSelection++;
    
    if ( pNewDialog->m_nSelection == 2 
      && pNewDialog->GetDlgItem( IDC_RADIO_YES )->IsWindowEnabled() == FALSE )
    pNewDialog->m_nSelection = -1;

    pNewDialog->UpdateData( FALSE );

    ::RegCloseKey( hKeyClass );
  }
  return;
}

void CDMOptionDlg::ReadRegistry()
{
	//***********************************************************************
	// Record Registry configuration of each supported device
	//***********************************************************************

	HKEY hKeyDevices;
	long nRC = ::RegOpenKeyEx(
  // HKEY_LOCAL_MACHINE, DM_CONFIG_OPTION_REG_PATH, 0, KEY_READ, &hKeyDevices);
  HKEY_LOCAL_MACHINE, CString( DM_CONFIG_REG_PATH ) + CString( DM_CONFIG_OPTION ) + _T("\\"), 0, KEY_READ, &hKeyDevices);
  
	if ( nRC == ERROR_SUCCESS )
  {
	  CString sWorkingDirectory;
		if ( OptionReadRegString( hKeyDevices, _T("WorkingDirectory"), sWorkingDirectory ) != FALSE 
			&& sWorkingDirectory.GetLength() >= sizeof( _T("C:\\") ) - 1 )
			m_sWorkingDirectory = sWorkingDirectory;
		DWORD nIndex = 0;
		TCHAR sClassName[50];
		while ( RegEnumKey( hKeyDevices, nIndex++, sClassName, sizeof( sClassName ) ) == ERROR_SUCCESS )
		{
			// Only Devices Entries
			AddDevicesToList( sClassName );
		}
	}
	::RegCloseKey( hKeyDevices );
	return;
}

BOOL CDMOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listOptions.SetItemHeight( 0, 20 );
	ReadRegistry();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDMOptionDlg::OnButtonApply() 
{
  int iCount=0;
  for ( int nIndex = 0; nIndex < m_listOptions.GetCount(); nIndex++ )
  {
    COptionCopiableStringArray aCommands;
    COptionChildDialog* pDlg = (COptionChildDialog*)m_listOptions.GetItemData(nIndex);
    pDlg->UpdateData( TRUE );
    
    if ( pDlg->m_nSelection == 1 )        // Action
    { 
      iCount++;
    }
  }
  if (iCount == 0)
  {
    MessageBox( _T("Nothing to do..."), _T("Batch Execution") );
    return;
  }
  if (iCount > 1)
  {
    MessageBox( _T("Only one action can be selected"), _T("Batch Execution") );
    return;
  }
  
  BOOL bOk = TRUE;  
  for ( nIndex = 0; nIndex < m_listOptions.GetCount(); nIndex++ )
  {
    COptionCopiableStringArray aCommands;
    COptionChildDialog* pDlg = (COptionChildDialog*)m_listOptions.GetItemData(nIndex);
    pDlg->UpdateData( TRUE );
    CString sLabel = pDlg->m_sClassName;
    //if ( pDlg->m_nSelection == 0 )      // No Action
    //   m_MapNoBatch.Lookup( sLabel, aCommands );
    //else 
    if ( pDlg->m_nSelection == 1 )        // Action
    {
       m_MapYesBatch.Lookup( sLabel, aCommands );
       if ( ExecuteCommand( sLabel, aCommands, m_sWorkingDirectory) == FALSE )
         bOk = FALSE;
       break;
    }
  }
  if ( bOk != FALSE )
  {
    MessageBox( _T("Advanced Options successfully updated.\n\nPlease, apply the Device Configuration again"), _T("Batch Execution") );
    CDialog::OnCancel();
  }
  return;
}


void CDMOptionDlg::OnPaint() 
{
	CDialog::OnPaint();  
}


void CDMOptionDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here

  CDialog::OnCancel();
}


