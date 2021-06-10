//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SCOTAPP.h
//
// TITLE: main header file for the SCOTAPP application.
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
//  DcL-021298 integrating the Security Agent features
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

// SCOTAPP.h : main header file for the SCOTAPP application
//

#if !defined(AFX_SCOTAPP_H__C9027528_676E_11D1_82FB_F81607C1B845__INCLUDED_)
#define AFX_SCOTAPP_H__C9027528_676E_11D1_82FB_F81607C1B845__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif  // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"               // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCOTAPPApp:
// See SCOTAPP.cpp for the implementation of this class
//

class CSCOTAPPApp : public CWinApp
{
public:
  CSCOTAPPApp();
  CString m_SCOTWndName;	
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSCOTAPPApp)
public:
  virtual BOOL InitInstance();
  virtual BOOL ProcessShellCommand(CCommandLineInfo &);
  virtual int  ExitInstance();
  //}}AFX_VIRTUAL
	
  // Implementation
	
  //{{AFX_MSG(CSCOTAPPApp)
  // NOTE - the ClassWizard will add and remove member functions here.
  //  DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  virtual BOOL PreTranslateMessage( MSG* pMsg ); //tar 263887

private:
   HMODULE hDdraw;

   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif  // !defined(AFX_SCOTAPP_H__C9027528_676E_11D1_82FB_F81607C1B845__INCLUDED_)
