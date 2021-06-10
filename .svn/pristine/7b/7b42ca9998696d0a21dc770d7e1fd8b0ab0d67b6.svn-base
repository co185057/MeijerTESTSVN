// MarshalHelper.cpp : implementation file
//

#include "stdafx.h"
#include "DispatchHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDispatchHelper

// The constructor sets the Thread ID, even though this may be wrong if the wrapper is 
// created on a different thread than where the component will run. However, when the 
// control is created using CreateControl, then the Thread ID will be set. 
// PRB: When you are using this class with the Wrapper created on thread A, but the window
// is created on thread B, and neither of the CreateControl mathods is called (e.g. this occurs when 
// the control window is created as part of a CDialog and the creation is implicit by the MFC framework)
// Then there is no way to determine on which thread the control was created. 
// Example: OPOS CO is on a dialog (defined in the resource file). The C++ class for the dialog is 
// created on one thread, but the m_hWnd is created on a different thread. The wrapper class' m_hWnd
// id set when DDX_Control calls CWndAttach. We have no way of catching this and hence, the thread id 
// is set when the wrapper object is constructed, and is not the thread where the m_hWnd is created. 
// I have an idea on how to solve this, but since I don't think we will see this in the FLAP, I'm not
// going to address this. 
// One place you would see something like this is in the device emulators design. But that would 
// only happen if an emulator dialog loaded another ActiveX control on the dialog - not likely. 

IMPLEMENT_DYNCREATE( CDispatchHelper, CWnd )

CDispatchHelper::CDispatchHelper()
   : m_dwThreadId( GetCurrentThreadId() ),
   m_pParentWnd( NULL )
   {
   }

CDispatchHelper::~CDispatchHelper()
   {
   // We cannot let DestroyWindow Execute on the wrong thread. So Marshal it to the correct thread
   if ( m_dwThreadId != GetCurrentThreadId() )
      {
      if ( m_pParentWnd == NULL )
         m_pParentWnd = GetParent();
      if ( m_pParentWnd != NULL
         && m_hWnd != NULL )
         {
         m_pParentWnd->SendMessage( WMU_DESTROY_WND, (WPARAM)m_hWnd  );
         m_hWnd = NULL;
         }
      }
   return;
   }


BEGIN_MESSAGE_MAP(CDispatchHelper, CWnd)
	//{{AFX_MSG_MAP(CDispatchHelper)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDispatchHelper::GetProperty(
   DISPID   dwDispID, 
   VARTYPE  vtProp,
	void*    pvProp) const
   {
	ASSERT(m_pCtrlSite != NULL);    // not an OLE control (not yet, at least)

	if (m_pCtrlSite != NULL)
      {
	   const_cast<CDispatchHelper*>(this)->InvokeHelper(dwDispID, DISPATCH_PROPERTYGET,
		   vtProp, pvProp, NULL);
      }
   return;
   }

void AFX_CDECL CDispatchHelper::SetProperty(
   DISPID   dwDispID, 
   VARTYPE  vtProp, 
   ...)
   {
	ASSERT(m_pCtrlSite != NULL);    // not an OLE control (not yet, at least)

	if (m_pCtrlSite != NULL)
      {
	   va_list argList;    // really only one arg, but...
	   va_start(argList, vtProp);
      CDispatchData mData( m_pCtrlSite, dwDispID, vtProp, argList );
      if ( m_dwThreadId != GetCurrentThreadId() )
         {
         if ( m_pParentWnd == NULL )
            m_pParentWnd = GetParent();
         if ( m_pParentWnd != NULL )
            m_pParentWnd->SendMessage( WMU_SET_PROPERTY, (UINT)&mData );
         }
      else
         mData.SetProperty( );
	   va_end(argList);
      mData.CheckExceptions( );
      }
   return;
   }


void AFX_CDECL CDispatchHelper::InvokeHelper(
   DISPID      dwDispID, 
   WORD        wFlags,
	VARTYPE     vtRet, 
   void*       pvRet, 
   const BYTE* pbParamInfo, 
   ... )
   {
	ASSERT(m_pCtrlSite != NULL);    // not an OLE control (not yet, at least)

	if (m_pCtrlSite != NULL)
      {
	   va_list argList;
	   va_start(argList, pbParamInfo);
      CDispatchData mData( m_pCtrlSite, dwDispID, wFlags, vtRet, pvRet, pbParamInfo, argList );
      if ( m_dwThreadId != GetCurrentThreadId() )
         {
         if ( m_pParentWnd == NULL )
            m_pParentWnd = GetParent();
         if ( m_pParentWnd != NULL )
            m_pParentWnd->SendMessage( WMU_INVOKE_HELPER, (UINT)&mData );
         }
      else
         mData.InvokeHelper(  );
	   va_end(argList);
      mData.CheckExceptions( );
      }
   return;
   }


BOOL CDispatchHelper::CreateControl(
   LPCTSTR     pszClass, 
   LPCTSTR     pszWindowName, 
   DWORD       dwStyle,
	const RECT& rect, 
   CWnd*       pParentWnd, 
   UINT        nID, 
   CFile*      pPersist,
	BOOL        bStorage, 
   BSTR        bstrLicKey )
   {
   m_pParentWnd = pParentWnd;
   m_dwThreadId = GetCurrentThreadId();
   return CWnd::CreateControl( pszClass, pszWindowName, dwStyle, rect, 
      pParentWnd, nID, pPersist, bStorage, bstrLicKey );
   }

BOOL CDispatchHelper::CreateControl( 
   REFCLSID    clsid, 
   LPCTSTR     lpszWindowName,
   DWORD       dwStyle, 
   const RECT& rect, 
   CWnd*       pParentWnd, 
   UINT        nID,
   CFile*      pPersist, 
   BOOL        bStorage, 
   BSTR        bstrLicKey )
   {
   m_pParentWnd = pParentWnd;
   m_dwThreadId = GetCurrentThreadId();
   return CWnd::CreateControl( clsid, lpszWindowName, dwStyle, rect, 
      pParentWnd, nID, pPersist, bStorage, bstrLicKey );
   }


/////////////////////////////////////////////////////////////////////////////
// CDispatchHelper message handlers


