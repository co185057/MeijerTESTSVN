#if !defined(AFX_UITHREAD_H__9B690B76_4A18_41AF_B87C_AE8EDAF4CA72__INCLUDED_)
#define AFX_UITHREAD_H__9B690B76_4A18_41AF_B87C_AE8EDAF4CA72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <CmDataCapture.h>

// UiThread.h : header file
//

// This is the only UI thread that we will ever need in the MFC world.
// Derive your windows based component from CThreadWndIF
// Declare a member variable of type CUiThread in your window class
// Override CThreadWndIF::InitInstance - construct your window(s)
// Override CThreadWndIF::ExitInstance - destroy your window(s)
// Call CUiThread::Start and CUiThread::Stop to create/destroy your window(s)


class CThreadWndIF
   {
   public:
      virtual BOOL InitWindow( void ) = 0;
      virtual void ExitWindow( void ) = 0; 

      virtual ~CThreadWndIF() {}
   };


/////////////////////////////////////////////////////////////////////////////
// CUiThread thread

class CUiThread : public CWinThread
{
//	DECLARE_DYNCREATE(CUiThread)
protected:
//	CUiThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUiThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUiThread();
protected:
	// Generated message map functions
	//{{AFX_MSG(CUiThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
   CUiThread( CmDataCapture& cmDc );
   BOOL Start( CThreadWndIF* pWindow, CWnd* pMainWnd );
   void Stop( void );
private:
   mutable  CmDataCapture& m_cmDc;
   CThreadWndIF*  m_pWndIF;
   BOOL           m_bInitResult;
   CEvent         m_evInitComplete;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UITHREAD_H__9B690B76_4A18_41AF_B87C_AE8EDAF4CA72__INCLUDED_)
