// TOposDlgThread.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// TOposDlgThread thread
template<class DEVICE_TYPE, class DIALOG_TYPE>
class TOposDlgThread : public CWinThread
{
protected:
	TOposDlgThread();           // protected constructor used by dynamic creation
// Attributes
public:
	void Enable( BOOL bEnable );
   DIALOG_TYPE* GetDlg( void ) { return m_pDlg; }

// Operations
public:
   virtual ~TOposDlgThread() { }
   void Start( DEVICE_TYPE* pDevice );
   void Stop( void );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TOposDlgThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
   DIALOG_TYPE*      m_pDlg;
   DEVICE_TYPE*      m_pDevice;
private:
   CEvent            m_evComplete;
};

/////////////////////////////////////////////////////////////////////////////


template<class DEVICE_TYPE, class DIALOG_TYPE>
TOposDlgThread<DEVICE_TYPE, DIALOG_TYPE>::TOposDlgThread(
   void )
   : m_pDlg( NULL ),
   m_pDevice( NULL )
   {
   }


template<class DEVICE_TYPE, class DIALOG_TYPE>
BOOL TOposDlgThread<DEVICE_TYPE, DIALOG_TYPE>::InitInstance()
   {
   ASSERT( m_pDevice != NULL );
   ASSERT( m_pDlg == NULL );
   m_pDlg = new DIALOG_TYPE( CWnd::GetDesktopWindow( ), m_pDevice); 
   BOOL bResult = m_pDlg->Create( DIALOG_TYPE::IDD, CWnd::GetDesktopWindow() );
   m_pDlg->EnableWindow( TRUE);
   m_pDlg->ShowWindow( SW_SHOW );
   ASSERT( bResult != FALSE );
   m_pMainWnd = m_pDlg;
   m_evComplete.SetEvent(); 
	return TRUE;
   }

template<class DEVICE_TYPE, class DIALOG_TYPE>
int TOposDlgThread<DEVICE_TYPE, DIALOG_TYPE>::ExitInstance()
   {
   ASSERT( m_pDlg != NULL );
	m_pDlg->DestroyWindow();
   delete m_pDlg;
   m_pDlg = NULL;
   m_evComplete.SetEvent(); 
	return CWinThread::ExitInstance();
   }

/////////////////////////////////////////////////////////////////////////////
// TOposDlgThread message handlers


template<class DEVICE_TYPE, class DIALOG_TYPE>
void TOposDlgThread<DEVICE_TYPE, DIALOG_TYPE>::Enable( BOOL bEnable )
   {
   ASSERT( m_pDlg != NULL );
   if ( bEnable == FALSE )
      m_pDlg->ShowWindow( SW_MINIMIZE );
   m_pDlg->ShowWindow( bEnable ? /* m_pDevice->GetShowWindow() */ SW_SHOWNORMAL : SW_HIDE );
   return;
   }


template<class DEVICE_TYPE, class DIALOG_TYPE>
void TOposDlgThread<DEVICE_TYPE, DIALOG_TYPE>::Start( 
   DEVICE_TYPE* pDevice)
   { 
   ASSERT( pDevice != NULL );    // Must pass Device
   ASSERT( m_pDevice == NULL );  // One call only
   m_pDevice = pDevice;
   m_bAutoDelete = FALSE;
   ResumeThread();
   CSingleLock( &m_evComplete, TRUE );
   ASSERT( m_pDlg != NULL );
   m_pDlg->SetWindowText( m_pDevice->GetWindowCaption() );
   return;
   }



template<class DEVICE_TYPE, class DIALOG_TYPE>
void TOposDlgThread<DEVICE_TYPE, DIALOG_TYPE>::Stop(
   void )
   {
   PostThreadMessage( WM_QUIT, 0, 0 );
   CSingleLock( &m_evComplete, TRUE );
   delete this;
   return;
   }


