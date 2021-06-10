/////////////////////////////////////////////////////////////////////////////
//
// OposDeviceThread.h : header file
//
//   Declaration of OposDeviceThread methods.
//
//   Copyright (c) 1998 NCR.  All rights reserved.
//
//   Date                   Modification                          Author
//---------|---------------------------------------------------|-------------
// 98/04/15 Initial version.                                      T. Burk
//
// This class replaces the Dialog thread template
// Now the device and the dialog are combined into a single class, 
// making the implementation simpler.  Two classes do not act as one. They are one!


/////////////////////////////////////////////////////////////////////////////
// COposDeviceThread thread
#ifndef _OPOS_DEVICE_THREAD_H_
#define _OPOS_DEVICE_THREAD_H_

class COposDeviceThread : private CWinThread
   {
   public:

	   COposDeviceThread( UINT nIDTemplate, CmDataCapture& cmDc );
   // Attributes
   public:
   // Operations
   public:
      virtual ~COposDeviceThread() { }
      BOOL Start( CDialog* pDialog, int nShowWindow = SW_SHOWMINNOACTIVE );
      void Stop( void );
   // Overrides
	   // ClassWizard generated virtual function overrides
	   //{{AFX_VIRTUAL(COposDeviceThread)
	   public:
	   virtual BOOL InitInstance();
	   virtual int ExitInstance();
	   //}}AFX_VIRTUAL

   // Implementation
   protected:
      CDialog*          m_pDlg;
      UINT              m_nIDTemplate;
      int               m_nShowWindow;
   private:
      CEvent            m_evComplete;
      CmDataCapture&    m_cmDc;

   };

/////////////////////////////////////////////////////////////////////////////

#endif

