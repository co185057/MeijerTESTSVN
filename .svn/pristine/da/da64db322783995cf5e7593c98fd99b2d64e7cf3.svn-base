// mywnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyWindow window
class CFireeventCtrl;
class CMyWindow : public CWnd
{
// Construction
public:
	LRESULT OnFireEventForThread(WPARAM wParam, LPARAM lParam);
	CFireeventCtrl *m_pControl;
	HWND Create();
	CMyWindow(CFireeventCtrl *pCtrl);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyWindow)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyWindow)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
