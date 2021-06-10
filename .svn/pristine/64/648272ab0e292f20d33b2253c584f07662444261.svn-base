// TestRCMDlg.cpp : implementation file
//

#include "stdafx.h"
#include <iostream>
#include "TestRCM.h"
#include "TestRCMDlg.h"
#include "verify.h"
#include "ApiRecorder.h"
#include "RCMMessages.h"
#include "RCMgrApi.h"
#ifdef TEST_PSXEVENTHANDLE
#include "psthread.h"
#include "psthreadold.h"
#endif
#include "PSEvt.h"
#include "PSXMfc.h"
#include "PSX Api.h"
#include "PSXInterface.h"
#include "comutil.h"
#include "RCMKeyValue.h"
#include "Registry.h"
#include "RCMRegistry.h"
#include ".\testrcmdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern HTRACE g_hTrace;

using namespace std;
using namespace fastlane;

_TCHAR szMachineName[MAX_COMPUTERNAME_LENGTH + 1];

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestRCMDlg dialog



CTestRCMDlg::CTestRCMDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTestRCMDlg::IDD, pParent)
    , m_pPSX(NULL)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestRCMDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_REMOTECONSOLEMGRCTRL1, m_ctrlRCM);
    DDX_Text(pDX, IDC_STATIC_EVENT_RESULT, m_sEventResult);
    DDX_Control(pDX, IDC_EDIT1, m_editMacroName);
    DDX_Control(pDX, IDC_BUTTON5, m_btnNextMacro);
    DDX_Control(pDX, IDC_BUTTON3, m_btnTestApi);
    DDX_Control(pDX, IDC_BUTTON4, m_btnLoadMacro);
    DDX_Control(pDX, IDC_BUTTON6, m_btnBrowse);
    DDX_Control(pDX, IDC_EDIT2, m_edtMacroText);
    DDX_Control(pDX, IDC_BUTTON2, m_btnTerminate);
    DDX_Control(pDX, IDC_BUTTON7, m_btnAutoPlay);
    DDX_Control(pDX, IDC_EDIT3, m_edtButtonName);
    DDX_Control(pDX, IDC_BUTTON9, m_btnButtonPress);
    DDX_Control(pDX, IDC_BUTTON8, m_btnSkip);
}

BEGIN_MESSAGE_MAP(CTestRCMDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
    ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
    ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
END_MESSAGE_MAP()


// CTestRCMDlg message handlers

BOOL CTestRCMDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if (m_ctrlRCM.Create(NULL, NULL, CRect(0,0,0,0), this, IDC_REMOTECONSOLEMGRCTRL1 ) == FALSE )
    {
        //ITRACE_DEVELOP(_T("Failed to create the Remote Console Manager control."));
    }
    else
    {
        //ITRACE_DEVELOP(_T("Created the Remote Console Manager control!"));
    }

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
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here
    m_btnNextMacro.EnableWindow(FALSE);
    m_btnTestApi.EnableWindow(FALSE);
    m_btnButtonPress.EnableWindow(FALSE);
    m_edtButtonName.EnableWindow(FALSE);
    m_btnLoadMacro.EnableWindow(FALSE);
    m_editMacroName.EnableWindow(FALSE);
    m_btnBrowse.EnableWindow(FALSE);
    m_btnTerminate.EnableWindow(FALSE);
    m_btnAutoPlay.EnableWindow(FALSE);
    m_btnSkip.EnableWindow(FALSE);

    DWORD dwMachineNameLength = sizeof(szMachineName);
    GetComputerName(szMachineName, &dwMachineNameLength);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestRCMDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestRCMDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestRCMDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


static const _TCHAR *testkeys[] = 
{
    _T("SpaceKey"),             
    _T("ShiftKey1"),            
    _T("Backspace"),            
    _T("PeriodKey"),            
    _T("GreaterThanKey"),       
    _T("BackslashKey"),         
    _T("QuestionMarkKey"),      
    _T("LeftBracket"),          
    _T("LeftBrace"),            
    _T("RightBracket"),         
    _T("RightBrace"),           
    _T("SlashKey"),             
    _T("PipeKey"),              
    _T("EqualSignKey"),         
    _T("PlusSignKey"),          
    _T("ApostropheKey"),        
    _T("TildeKey"),             
    _T("AtSignKey"),            
    _T("ExclamationPointKey"),  
    _T("SharpKey"),             
    _T("DollarSignKey"),        
    _T("PercentSignKey"),       
    _T("AmpersandKey"),         
    _T("AsteriskKey"),          
    _T("OpenParenthesisKey"),   
    _T("CloseParenthesisKey"),  
    _T("MinusKey"),             
    _T("UnderscoreKey"),        
    _T("ColonKey"),             
    _T("SemicolonKey"),         
    _T("CommaKey"),             
    _T("LessThanKey"),          
    _T("SingleQuoteKey"),       
    _T("DoubleQuoteKey"),       
    _T("ButtonGoBack"),         
    _T("ButtonStoreLogIn"),     
    _T("ButtonHelp"),           
    _T("CMButton1"),            
    _T("SMButton1"),            
    _T("CMButton1Lg"),          
    _T("CMButton1Med"),         
    _T("CMButton1MidiList"),    
    _T("CMButton1StoreLogIn"),  
    _T("CMButton1Global"),      
    _T("CMButton1LookUp"),      
    _T("CMButton2"),            
    _T("SMButton2"),            
    _T("CMButton2Lg"),          
    _T("CMButton2Med"),         
    _T("CMButton2MidiList"),    
    _T("CMButton2StoreLogIn"),  
    _T("CMButton2KeyInCode"),   
    _T("CMButton3"),            
    _T("SMButton3"),            
    _T("CMButton3MidiList"),    
    _T("CMButton3Global"),      
    _T("CMButton3Med"),         
    _T("CMButton4"),            
    _T("SMButton4"),            
    _T("CMButton4MidiList"),    
    _T("CMButton4Lg"),          
    _T("CMButton4LgFinish"),    
    _T("CMButton5"),            
    _T("SMButton5"),            
    _T("CMButton5MidiList"),    
    _T("CMButton5Lg"),          
    _T("CMButton5LgFinish"),    
    _T("CMButton6"),            
    _T("SMButton6"),            
    _T("CMButton6MidiList"),    
    _T("CMButton6Lg"),          
    _T("CMButton6LgFinish"),    
    _T("CMButton7"),            
    _T("SMButton7"),            
    _T("CMButton7MidiList"),    
    _T("CMButton7Lg"),          
    _T("CMButton7LgFinish"),    
    _T("CMButton8"),            
    _T("SMButton8"),            
    _T("CMButton8MidiList"),    
    _T("CMButton8Lg"),          
    _T("CMReceiptScrollUp"),    
    _T("SMReceiptScrollUp"),    
    _T("CMReceiptScrollDown"),  
    _T("CMProduceSelectNext"),  
    _T("CMProduceSelectPrev"),  
    _T("CMTareDown"),           
    _T("CMTareUp"),             
    _T("NumericP1"),            
    _T("KeyBoardP1"),           
    _T("AlphaNumP1"),           
    _T("AlphaNumP2"),           
    _T("AlphaNumP4"),           
    _T("NumericP2"),            
    _T("KeyBoardP2"),           
    _T("NumericP3"),            
    _T("SMNumericKeyBoard"),    
    _T("NumericP4"),            
    _T("KeyBoardP4"),           
    _T("AlphaNumKey3"),         
    _T("KeyBoardP3"),           
    _T("RemoteButton1"),        
    _T("ButtonToggle"),         
    _T("ButtonMainMenu"),       
    _T("Menu1"), 
    _T("Menu2"), 
    _T("Menu3"), 
    _T("Menu4"), 
    _T("Menu5"), 
    _T("Menu6"), 
    _T("Menu7"), 
    _T("Menu8"), 
    _T("Menu9"), 
    _T("Menu10"),
    _T("Menu11"),
    _T("Menu12"),
    _T("Menu13"),
    _T("Menu14"),
    _T("Menu15"),
    _T("RemoteButton1"),
    _T("RemoteButton1"),
    NULL
};

#define MAX_TIMES   10
void CTestRCMDlg::OnBnClickedButton1()
{
    IMODTRACE_DEVELOP();
    
    LoadPSX();
    
    m_ctrlRCM.Initialize(m_pPSX->GetPSXHandle());

#ifdef TEST_PSXEVENTHANDLE
    // Prime the pump
    PSXEventHandlerNew( NULL, _T("Test"), testkeys[0], _T("Test"), UI::EVENTCLICK );

    clock_t start = clock();
    int total = 0;
    for (int i=0; i<MAX_TIMES; i++)
    {
        int cnt = 0;
        const _TCHAR *pStringName = testkeys[cnt];
        while (pStringName != NULL)
        {
            PSXEventHandlerNew( NULL, _T("Test"), pStringName, _T("Test"), UI::EVENTCLICK );
            pStringName = testkeys[cnt++];
            total++;
        }
    }
    clock_t time = clock() - start;

    start = clock();
    int total2 = 0;
    for (int i=0; i<MAX_TIMES; i++)
    {
        int cnt = 0;
        const _TCHAR *pStringName = testkeys[cnt];
        while (pStringName != NULL)
        {
            PSXEventHandlerOld( NULL, _T("Test"), pStringName, _T("Test"), UI::EVENTCLICK );
            pStringName = testkeys[cnt++];
            total2++;
        }
    }
    clock_t time2 = clock() - start;
    ITRACE_DEVELOP(_T("Total time:  ") + IString(time) + _T("  Count:  ") + IString(total) + _T(".  Avg:  ") + IString(time/total));
    ITRACE_DEVELOP(_T("Total time:  ") + IString(time2) + _T("  Count:  ") + IString(total2) + _T(".  Avg:  ") + IString(time2/total2));

    const _TCHAR *pStringName = testkeys[0];
    for (int cnt=1; pStringName != NULL; cnt++)
    {
        PSXEventHandlerNew( NULL, _T("Test"), pStringName, _T("Test"), UI::EVENTCLICK );
        PSXEventHandlerOld( NULL, _T("Test"), pStringName, _T("Test"), UI::EVENTCLICK );
        pStringName = testkeys[cnt];
    }
#endif

    m_btnTestApi.EnableWindow(TRUE);
    m_btnButtonPress.EnableWindow(TRUE);
    m_edtButtonName.EnableWindow(TRUE);
    m_btnLoadMacro.EnableWindow(TRUE);
    m_editMacroName.EnableWindow(TRUE);
    m_btnBrowse.EnableWindow(TRUE);
    m_btnTerminate.EnableWindow(TRUE);
}


void CTestRCMDlg::LoadPSX()
{
    IMODTRACE_DEVELOP();

    CPSXInterface::instance()->SetTraceObject(g_hTrace);
    CPSXInterface::instance()->SetControl(&m_ctrlRCM);

    CPSX::Initialize();

    m_pPSX = new CPSX( &CPSXInterface::EventHandler );                
    CRect rect( 0, 0, 640, 480 );
    if( PSX_SUCCESS != m_pPSX->CreateDisplay( rect, 0, NULL ) )
    {
        ITRACE_DEVELOP(_T("CreateDisplay Failed"));
        return;
    }    
        
    m_pPSX->SetDisplayView( DisplayStandard );
    //m_pPSX->SetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYVISIBLE, VARIANT_FALSE );

#if 0
    if( !_tgetenv( _T("SCOTPC") ) )
    {
        // Disable Cursor //
        m_pPSX->SetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYCURSOR, VARIANT_FALSE );

        // Make sure taskbar is not topmost //
        const DWORD ABM_SETSTATE = 0x0000000A;
        const TCHAR TASKBAR_WNDCLASS[] = _T("Shell_TrayWnd");
        APPBARDATA abd = { 0 };
        abd.cbSize = sizeof( APPBARDATA );
        abd.hWnd = ::FindWindow( TASKBAR_WNDCLASS, NULL );
        SHAppBarMessage( ABM_SETSTATE, &abd );
        
        // Put UI topmost //
        COleVariant v;
        if( PSX_SUCCESS == m_pPSX->GetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYHWND, v ) )
        {
            ::SetWindowPos( (HWND)v.lVal, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE );
        }
    }
#endif

    // Set the environment variable for XML config file //
    //TCHAR szWork[_MAX_PATH] = _T("");
    //_tmakepath( szWork, SCOTDRIVE, SCOTPATH SCOTIMAGE, NULL, NULL );

    CRegistry registry;
	TCHAR szTmpPath[_MAX_PATH] = _T("");

    const _TCHAR *pMediaPath = CRegistryValue<const _TCHAR *>::GetRegistryValue( registry
                                           , HKEY_RCM_TEST
                                           , REG_VALUE_MEDIA_PATH
                                           , DEFAULT_RCM_MEDIA_PATH );

	if(ExpandEnvironmentStrings(pMediaPath, szTmpPath, _MAX_PATH))
	{
		pMediaPath = szTmpPath;
	}
    m_pPSX->SetEnvironmentVariable( _T("MediaPath"), pMediaPath );

    const _TCHAR *pConfigPath = CRegistryValue<const _TCHAR *>::GetRegistryValue( registry
                                           , HKEY_RCM_TEST
                                           , REG_VALUE_CONFIG_PATH
                                           , DEFAULT_RCM_CONFIG_PATH );
	if(ExpandEnvironmentStrings(pConfigPath, szTmpPath, _MAX_PATH))
	{
		pConfigPath = szTmpPath;
	}
    m_pPSX->SetEnvironmentVariable( _T("ConfigPath"), pConfigPath );
    
    // Text on the task bar //
    m_pPSX->SetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYTEXT, _T("NCR SCOT") );

	ExpandEnvironmentStrings(_T("%APP_DRIVE%\\scot\\config\\LaneRAP.xml"), szTmpPath, _MAX_PATH);
    PSXRC rc = m_pPSX->LoadConfigFile( szTmpPath );
    if( PSX_SUCCESS != rc )
    {
        trace( L0, _T("PS Initialize - unable to load XML UI configuration file. rc = %d"), rc );
        return;
    }

    // Start Remote Server
    rc = m_pPSX->StartServer( _T("FastLaneRemoteServer") );
    if( PSX_SUCCESS != rc )
    {
        trace( L6, _T("PS Initialize - PSX StartServer failed! rc = %d"), rc );
    }

    rc = m_pPSX->SetLoadStringCallback( LoadStringCallbackHandler );
    if( PSX_SUCCESS != rc )
    {
        trace( L6, _T("PS Initialize - PSX SetLoadStringCallback failed! rc = %d"), rc );
    }
}


void CTestRCMDlg::OnBnClickedButton2()
{
    delete m_pPSX;
    
    CPSX::Uninitialize();

    // TODO: Add your control notification handler code here
    m_ctrlRCM.Terminate();

    m_btnNextMacro.EnableWindow(FALSE);
    m_btnTestApi.EnableWindow(FALSE);
    m_btnButtonPress.EnableWindow(FALSE);
    m_edtButtonName.EnableWindow(FALSE);
    m_btnLoadMacro.EnableWindow(FALSE);
    m_editMacroName.EnableWindow(FALSE);
    m_btnBrowse.EnableWindow(FALSE);
    m_btnTerminate.EnableWindow(FALSE);
    m_btnAutoPlay.EnableWindow(FALSE);
    m_btnSkip.EnableWindow(FALSE);
}

void CTestRCMDlg::OnBnClickedButton3()
{
    // TODO: Add your control notification handler code here
    LONG lResult;
    HRESULT hr = m_ctrlRCM.CanApprove(_T("TestApproval"), &lResult);
    cout << _T("CanApprove hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    BSTR testBstr = NULL;
    StringType cmd = _T("type=add-other-event;description=COMMAND=MEDIAAll currency levels are normal&semi;Receipt Printer: Normal (15)&semi&semi;Journal Printer: Normal (15)&semi;Cash Acceptor: Cassette Inserted (60)&semi;Coin Acceptor: Cassette Inserted (80)&semi;");
    hr = m_ctrlRCM.OnEvent(cmd.c_str());
    cout << _T("OnEvent hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    cmd = _T("type=normal-item;description=DESCRIPTION=&semi;LANEBUTTONTEXT=&semi;LSWND1=Welcome!&semi;LSWND2=&semi;LSWND3=&semi;");
    hr = m_ctrlRCM.OnEvent(cmd.c_str());
    cout << _T("OnEvent hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    cmd = _T("operation=set-light-state;green=0;yellow=0;red=1");
    hr = m_ctrlRCM.OnControl(cmd.c_str(), &testBstr);
    cout << _T("OnControl hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    cmd = _T("operation=exit-store-mode");
    hr = m_ctrlRCM.OnControl(cmd.c_str(), &testBstr);
    cout << _T("OnControl hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    cmd = _T("id=request-enable-assist-mode;value=true");
    hr = m_ctrlRCM.Get(cmd.c_str(), &testBstr);
    cout << _T("Get hr:  ") << (const _TCHAR *)IString(hr).d2x() << _T(".  testBstr:  ") << testBstr << endl;

    cmd = _T("operation=set-light-state;green=0;yellow=0;red=0");
    hr = m_ctrlRCM.OnControl(cmd.c_str(), &testBstr);
    cout << _T("OnControl hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    cmd = _T("type=add-other-event;description=COMMAND=UIInitialize&semi;ASSISTMODEFILENAME=wSMKBDIBM4680&semi;");
    hr = m_ctrlRCM.OnEvent(cmd.c_str());
    cout << _T("OnEvent hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    cmd = _T("type=normal-item;description=[Alert]POS Message: ");
    hr = m_ctrlRCM.OnEvent(cmd.c_str());
    cout << _T("OnEvent hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    cmd = _T("operation=set-light-state;green=1;yellow=0;red=0");
    hr = m_ctrlRCM.OnControl(cmd.c_str(), &testBstr);
    cout << _T("OnControl hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    hr = m_ctrlRCM.Get(_T("TestApproval"), &testBstr);
    cout << _T("Get hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;

    hr = m_ctrlRCM.Set(_T("test=event"));
    cout << _T("Set hr:  ") << (const _TCHAR *)IString(hr).d2x() << endl;
}


BOOL CTestRCMDlg::LoadStringCallbackHandler( UINT nId, CString& strReturnedString )
{
    CString csTemp;
    csTemp = _T("Test String");
    //csTemp = ps.GetPSText(nId);
    //csTemp = ps.ConvertToEscapeSequence(csTemp, _T("\\t"), _T("\t"));
    //csTemp = ps.ConvertToEscapeSequence(csTemp, _T("\\n"), _T("\n"));
    strReturnedString = csTemp;
    return true;
}  


void CTestRCMDlg::OnBnClickedButton4()
{
    IMODTRACE_DEVELOP();

    _TCHAR buffer[MAX_PARM_LIST+1];
    int nBytesCopied = m_editMacroName.GetLine(0, buffer, MAX_PARM_LIST);
    //m_editMacroName.Clear();
    buffer[nBytesCopied] = 0;

    try
    {
        CFile macroFile(buffer, CFile::modeRead | CFile::shareDenyNone);

        size_t ullLength = (size_t)macroFile.GetLength();

        m_pszMacroBuffer = new char[ullLength+1];
        m_pszCurrentRecord  = m_pszMacroBuffer;
        m_pszEndRecord      = m_pszMacroBuffer + ullLength;

        macroFile.Read(m_pszMacroBuffer, (UINT)ullLength);

        m_pszMacroBuffer[ullLength] = 0;

        macroFile.Close();

        m_btnNextMacro.EnableWindow(TRUE);
        m_btnAutoPlay.EnableWindow(TRUE);
        m_btnSkip.EnableWindow(TRUE);

        DisplayNextMacro();
    }
    catch( CFileException *pEx)
    {
        // Simply show an error message to the user.
        pEx->ReportError();
    }

}


void CTestRCMDlg::OnBnClickedButton5()
{
    ExecuteNextMacro();
}


bool CTestRCMDlg::ExecuteNextMacro()
{
    if (m_pszCurrentRecord >= m_pszEndRecord)
        return false;

    SRecord *pRecord        = (SRecord *)m_pszCurrentRecord;
    UMessageParms *pParms   = (UMessageParms *)(m_pszCurrentRecord + sizeof(SRecordHdr));

    long lResult;
    BSTR bstrOutParms       = NULL;

    switch (pRecord->hdr.lApiNum)
    {
        // Ignore these two.  They are handled by other buttons
        case E_API_INITIALIZE :
        case E_API_TERMINATE :
            break;
        case E_API_CAN_APPROVE :
            m_ctrlRCM.CanApprove(pParms->canApprove.szParms, &lResult);
            break;
        case E_API_ON_APPROVAL :
            m_ctrlRCM.OnApproval(pParms->onApproval.szParms);
            break;
        case E_API_ON_EVENT :
            m_ctrlRCM.OnEvent(pParms->onEvent.szParms);
            break;
        case E_API_GET :
            m_ctrlRCM.Get(pParms->get.szParms, &bstrOutParms);
            ::SysFreeString(bstrOutParms);
            break;
        case E_API_SET :
            m_ctrlRCM.Set(pParms->set.szParms);
            break;
        case E_API_ON_CONTROL :
            m_ctrlRCM.OnControl(pParms->onControl.szParms, &bstrOutParms);
            ::SysFreeString(bstrOutParms);
            break;
        case E_API_GET_APPROVAL_INFO :
            m_ctrlRCM.GetApprovalInfo(pParms->getApprovalInfo.szParms, &bstrOutParms);
            ::SysFreeString(bstrOutParms);
            break;
        case E_API_ON_INPUT_EVENT :
            m_ctrlRCM.OnInputEvent(pParms->onInputEvent.szParms);
            break;
    }

    FindNextMacro(pRecord);

    if (m_pszCurrentRecord >= m_pszEndRecord)
    {
        m_btnNextMacro.EnableWindow(FALSE);
        m_btnAutoPlay.EnableWindow(FALSE);
        m_btnSkip.EnableWindow(FALSE);
    }
    else
        DisplayNextMacro();

	return (m_pszCurrentRecord >= m_pszEndRecord);
}


void CTestRCMDlg::FindNextMacro(SRecord *pRecord)
{
    m_pszCurrentRecord += pRecord->hdr.nRecLength;
    SRecord *pNextRecord        = (SRecord *)m_pszCurrentRecord;

    while ((m_pszCurrentRecord < m_pszEndRecord) && pNextRecord->hdr.IsResult())
    {
        m_pszCurrentRecord += pNextRecord->hdr.nRecLength;
        pNextRecord        = (SRecord *)m_pszCurrentRecord;
    }
}


void CTestRCMDlg::DisplayNextMacro()
{
    if (m_pszCurrentRecord >= m_pszEndRecord)
    {
        m_edtMacroText.SetWindowText(_T(""));
        return;
    }

    SRecord *pRecord        = (SRecord *)m_pszCurrentRecord;
    UMessageParms *pParms   = (UMessageParms *)(m_pszCurrentRecord + sizeof(SRecordHdr));
    IString msg;

    switch (pRecord->hdr.lApiNum)
    {
        // Ignore these two.  They are handled by other buttons
        case E_API_INITIALIZE :
            m_edtMacroText.SetWindowText(_T("Initialize"));
            break;
        case E_API_TERMINATE :
            m_edtMacroText.SetWindowText(_T("Terminate"));
            break;
        case E_API_CAN_APPROVE :
            msg = _T("CanApprove:  ");
            msg += pParms->canApprove.szParms;
            m_edtMacroText.SetWindowText((const _TCHAR *)msg);
            break;
        case E_API_ON_APPROVAL :
            msg = _T("OnApproval:  ");
            msg += pParms->onApproval.szParms;
            m_edtMacroText.SetWindowText((const _TCHAR *)msg);
            break;
        case E_API_ON_EVENT :
            msg = _T("OnEvent:  ");
            msg += pParms->onEvent.szParms;
            m_edtMacroText.SetWindowText((const _TCHAR *)msg);
            break;
        case E_API_GET :
            msg = _T("Get:  ");
            msg += pParms->get.szParms;
            m_edtMacroText.SetWindowText((const _TCHAR *)msg);
            break;
        case E_API_SET :
            msg = _T("Set:  ");
            msg += pParms->set.szParms;
            m_edtMacroText.SetWindowText((const _TCHAR *)msg);
            break;
        case E_API_ON_CONTROL :
            msg = _T("OnControl:  ");
            msg += pParms->onControl.szParms;
            m_edtMacroText.SetWindowText((const _TCHAR *)msg);
            break;
        case E_API_GET_APPROVAL_INFO :
            msg = _T("GetApprovalInfo:  ");
            msg += pParms->getApprovalInfo.szParms;
            m_edtMacroText.SetWindowText((const _TCHAR *)msg);
            break;
        case E_API_ON_INPUT_EVENT :
            msg = _T("OnInputEvent:  ");
            msg += pParms->onInputEvent.szParms;
            m_edtMacroText.SetWindowText((const _TCHAR *)msg);
            break;
    }
}



void CTestRCMDlg::OnBnClickedButton6()
{
    _TCHAR szFilters[]= _T("API Macro Files (*.dat)|*.dat||");

    CFileDialog fileDlg(TRUE
                      , _T(".dat")
                      , NULL
                      , (OFN_FILEMUSTEXIST | OFN_HIDEREADONLY 
                       | OFN_PATHMUSTEXIST)
                      , szFilters);

    if (IDOK == fileDlg.DoModal())
    {
        ITRACE_DEVELOP(_T("Obtaining file name ..."));
        CString sFileName = fileDlg.GetPathName();
        m_editMacroName.SetWindowText(sFileName);
    }
}

void CTestRCMDlg::OnBnClickedButton7()
{
    // TODO: Add your control notification handler code here
    // Play back all the API's until either an error is detected
    // or the end of the playback sequence is reached
	do 
	{
		ITRACE_DEVELOP(_T("Executing next macro"));
	} while(ExecuteNextMacro());
}


BEGIN_EVENTSINK_MAP(CTestRCMDlg, CDialog)
    ON_EVENT(CTestRCMDlg, IDC_REMOTECONSOLEMGRCTRL1, 1, OnRequestApproval, VTS_BSTR)
    ON_EVENT(CTestRCMDlg, IDC_REMOTECONSOLEMGRCTRL1, 2, OnRequestSuspend, VTS_BSTR)
    ON_EVENT(CTestRCMDlg, IDC_REMOTECONSOLEMGRCTRL1, 3, OnCancelSuspend, VTS_BSTR)
    ON_EVENT(CTestRCMDlg, IDC_REMOTECONSOLEMGRCTRL1, 4, OnValidateOperator, VTS_BSTR)
    ON_EVENT(CTestRCMDlg, IDC_REMOTECONSOLEMGRCTRL1, 5, OnUnsolicitedData, VTS_BSTR)
    ON_EVENT(CTestRCMDlg, IDC_REMOTECONSOLEMGRCTRL1, 6, OnAssistMode, VTS_BSTR)
    ON_EVENT(CTestRCMDlg, IDC_REMOTECONSOLEMGRCTRL1, 7, OnRemoteConsoleEvent, VTS_BSTR)
END_EVENTSINK_MAP()


void CTestRCMDlg::OnRequestApproval(LPCTSTR bstrParms)
{
    m_sEventResult = "OnRequestApproval was called.  ";
    m_sEventResult += bstrParms;
    UpdateData(FALSE);
}


void CTestRCMDlg::OnRequestSuspend(LPCTSTR bstrParms)
{
    m_sEventResult = "OnRequestSuspend:  ";
    m_sEventResult += bstrParms;
    UpdateData(FALSE);
}


void CTestRCMDlg::OnCancelSuspend(LPCTSTR bstrParms)
{
    m_sEventResult = "OnCancelSuspend:  ";
    m_sEventResult += bstrParms;
    UpdateData(FALSE);
}


void CTestRCMDlg::OnValidateOperator(LPCTSTR bstrParms)
{
    m_sEventResult = "OnValidateOperator:  ";
    m_sEventResult += bstrParms;
    UpdateData(FALSE);
}


void CTestRCMDlg::OnUnsolicitedData(LPCTSTR bstrParms)
{
    m_sEventResult = "OnUnsolicitedData:  ";
    m_sEventResult += bstrParms;
    UpdateData(FALSE);
}


void CTestRCMDlg::OnAssistMode(LPCTSTR bstrParms)
{
    m_sEventResult = "OnAssistMode:  ";
    m_sEventResult += bstrParms;
    UpdateData(FALSE);
}


void CTestRCMDlg::OnRemoteConsoleEvent(LPCTSTR bstrParms)
{
    m_sEventResult = "OnRemoteConsoleEvent:  ";
    m_sEventResult += bstrParms;
    UpdateData(FALSE);
}

void CTestRCMDlg::OnBnClickedButton8()
{
    if (m_pszCurrentRecord >= m_pszEndRecord)
    {
        m_edtMacroText.SetWindowText(_T(""));
        return;
    }

    SRecord *pRecord        = (SRecord *)m_pszCurrentRecord;
    FindNextMacro(pRecord);

    if (m_pszCurrentRecord >= m_pszEndRecord)
    {
        m_btnNextMacro.EnableWindow(FALSE);
        m_btnAutoPlay.EnableWindow(FALSE);
        m_btnSkip.EnableWindow(FALSE);
    }
    else
        DisplayNextMacro();
}


void CTestRCMDlg::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
    _TCHAR buffer[MAX_PARM_LIST+1];
    int nBytesCopied = m_edtButtonName.GetLine(0, buffer, MAX_PARM_LIST);
    m_edtButtonName.Clear();
    buffer[nBytesCopied] = 0;

    IString remoteName(_T("RAP."));
    remoteName += szMachineName;

    CKeyValueParmList parmList;
    CKeyValue<long>::Put(parmList, KEY_IE_LPARAM, 0);
    CKeyValue<long>::Put(parmList, KEY_IE_WPARAM, 0);
    CKeyValue<long>::Put(parmList, KEY_IE_EVENT_ID, PS_KEYSTROKE);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_CONTROL_NAME, buffer);
	CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_CONTEXT_NAME, _T("Test"));
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_EVENT_NAME, UI::EVENTCLICK);
    CKeyValue<const _TCHAR*>::Put(parmList, KEY_IE_REMOTE_CONNECTION_NAME, (const _TCHAR *)remoteName);

    _bstr_t bstrParms = (const _TCHAR *)parmList;

    m_ctrlRCM.OnInputEvent(bstrParms);
    m_sEventResult = "OnInputEvent";
    UpdateData(FALSE);
}
