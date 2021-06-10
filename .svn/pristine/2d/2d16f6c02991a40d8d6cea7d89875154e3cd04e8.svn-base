//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  FILE:    DMInterface.h
//
//  TITLE:   DMX Window
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMINTERFACE_H__0CE36FC0_4A54_11D1_8AE4_4A5572000000__INCLUDED_)
#define AFX_DMINTERFACE_H__0CE36FC0_4A54_11D1_8AE4_4A5572000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "UiThread.h"
#include "DispatchHelper.h"


#define WMU_CREATE_DEVICE WM_USER + 9

/////////////////////////////////////////////////////////////////////////////
// DMInterface window

class DMInterface : public CWnd, private CThreadWndIF
{
   public:
   // Constructor
      DMInterface( class CDeviceManagerEx& rDMX, CmDataCapture& cmDc);
      virtual ~DMInterface();
      BOOL Initialize( BOOL bNewThread = TRUE );
      void UnInitialize( void );

  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(DMInterface)
public:
  //}}AFX_VIRTUAL
  DECLARE_EVENTSINK_MAP()
	
  // Generated message map functions
public:	
  //{{AFX_MSG(DMInterface)
	afx_msg void OnTimer(UINT nIDEvent);
  //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
   // These are non-standard, so declare them outside the //}}AFX_MSG
   afx_msg BOOL OnOposErrorEvent( UINT nCtrlId, long nResultCode, 
      long nResultCodeExtended, long nErrorLocus, long FAR* pnErrorResponse );
   afx_msg BOOL OnOposOutputCompleteEvent( UINT nCtrlId, long nOutputId );
   afx_msg BOOL OnOposStatusUpdateEvent( UINT nCtrlId, long nStatus );
   afx_msg BOOL OnOposDataEvent( UINT nCtrlId, long nStatus );
   afx_msg BOOL OnOposDirectIOEvent( UINT nCtrlId, long nEventId, long* plData, BSTR* pbstrData );
private:
   DECLARE_MARSHAL_CONTROL( )
   LONG OnCreateDevice( UINT pDevice, LONG );
private:
   // CThreadWndIF Overrides
   virtual BOOL InitWindow( void );
   virtual void ExitWindow( void ); 
private:
   CUiThread               m_uiThread;
   mutable CmDataCapture&  m_cmDc;
   CDeviceManagerEx&       m_rDMX;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_DMINTERFACE_H__0CE36FC0_4A54_11D1_8AE4_4A5572000000__INCLUDED_)
