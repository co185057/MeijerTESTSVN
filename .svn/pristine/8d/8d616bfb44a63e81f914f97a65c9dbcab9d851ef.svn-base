#if !defined(AFX_DISPATCHHELPER_H__38D11973_A43A_11D4_9A0B_00A0C9EDD467__INCLUDED_)
#define AFX_DISPATCHHELPER_H__38D11973_A43A_11D4_9A0B_00A0C9EDD467__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Include special MFC implementation header
// VS 6.0 users need occimpl.h - Add: Tools | Options | Directories | Include
// C:\Program Files\Microsoft Visual Studio\VC98\MFC\SRC

#if _MFC_VER < 0x0700 
#include "occimpl.h"
#else
#include <afxocc.h>
#endif

// DispatchHelper.h : header file
//
const long WMU_INVOKE_HELPER =	WM_USER + 10;  // Special Marshalling
const long WMU_SET_PROPERTY  =	WM_USER + 11;  // Special Marshalling
const long WMU_DESTROY_WND   =   WM_USER + 12;  // Special Marshalling

#define DECLARE_MARSHAL_CONTROL( )                    \
   LONG OnInvokeHelper( UINT pMarshalData, LONG );    \
   LONG OnSetProperty( UINT pMarshalData, LONG );     \
   LONG OnDestroyDevice( UINT hWnd, LONG );

#define IMPLEMENT_MARSHAL_CONTROL( n )                \
LONG n::OnInvokeHelper( UINT pMarshalData, LONG )     \
   { ((CDispatchData*)pMarshalData)->InvokeHelper(  ); \
   return 0; }                                        \
LONG n::OnSetProperty(UINT pMarshalData, LONG )       \
   { ((CDispatchData*)pMarshalData)->SetProperty( );   \
   return 0;}                                         \
LONG n::OnDestroyDevice( UINT hWnd, LONG )            \
   {   return ::DestroyWindow( (HWND)hWnd );   }

#define IMPLEMENT_MARSHAL_MESSAGES( )                 \
   ON_MESSAGE( WMU_INVOKE_HELPER, OnInvokeHelper )    \
   ON_MESSAGE( WMU_SET_PROPERTY, OnSetProperty )      \
   ON_MESSAGE( WMU_DESTROY_WND, OnDestroyDevice )     \

class CDispatchData
   {
   public:
      CDispatchData( COleControlSite*  pCtrlSite,
         DISPID dwDispID, WORD wFlags,
	   	VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, va_list argList )
         : m_pCtrlSite( pCtrlSite ),
         m_dwDispID( dwDispID ),
         m_wFlags( wFlags ),
         m_vtData( vtRet ),
         m_pvRet( pvRet ),
         m_pbParamInfo( pbParamInfo ),
         m_argList( argList ),
         m_pException( NULL )
         {
         }
      CDispatchData( COleControlSite*  pCtrlSite,
         DISPID dwDispID, VARTYPE vtProp, va_list argList )
         : m_pCtrlSite( pCtrlSite ),
         m_dwDispID( dwDispID ),
         m_wFlags( 0 ),
         m_vtData( vtProp ),
         m_pvRet( NULL ),
         m_pbParamInfo( NULL ),
         m_argList( argList ),
         m_pException( NULL )
         {
         }
      void InvokeHelper( void )
         {
         ASSERT( m_pException == NULL );
         try
            {
	         m_pCtrlSite->InvokeHelperV(
               m_dwDispID, m_wFlags, m_vtData, m_pvRet, m_pbParamInfo, m_argList);
            }
         catch (CException* ex)
            {
            m_pException = ex;
            }
         return;
         }
      void SetProperty( void )
         {
         ASSERT( m_pException == NULL );
         try
            {
            m_pCtrlSite->SetPropertyV( m_dwDispID, m_vtData, m_argList );
            }
         catch (CException* ex)
            {
            m_pException = ex;
            }
         return;
         }
      void CheckExceptions( void )
         {
         if ( m_pException != NULL )
            {
            CException* ex = m_pException;
            m_pException = NULL;
            throw ex;
            }
         return;
         }
   private:
      const DISPID      m_dwDispID;
      const WORD        m_wFlags;
      const VARTYPE     m_vtData;
      void*             m_pvRet;
      const BYTE*       m_pbParamInfo;
      va_list           m_argList;
      COleControlSite*  m_pCtrlSite;
      CException*       m_pException;
   };


/////////////////////////////////////////////////////////////////////////////
// CDispatchHelper window

class CDispatchHelper : public CWnd
   {
	protected: 
      DECLARE_DYNCREATE( CDispatchHelper )
   // Construction
   public:
	   CDispatchHelper();

   // Attributes
   public:

   // Operations
   public:

   // Overrides
	   // ClassWizard generated virtual function overrides
	   //{{AFX_VIRTUAL(CDispatchHelper)
	   //}}AFX_VIRTUAL

   // Implementation
   public:
	   virtual ~CDispatchHelper();

	   // Generated message map functions
   protected:
	   //{{AFX_MSG(CDispatchHelper)
		   // NOTE - the ClassWizard will add and remove member functions here.
	   //}}AFX_MSG
	   DECLARE_MESSAGE_MAP()
   protected:
   	void AFX_CDECL InvokeHelper(DISPID dwDispID, WORD wFlags,
	   	VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, ...);
   	void GetProperty(DISPID dwDispID, VARTYPE vtProp, void* pvProp) const;
   	void AFX_CDECL SetProperty(DISPID dwDispID, VARTYPE vtProp, ...);
   public:
	   BOOL CreateControl(REFCLSID clsid, LPCTSTR pszWindowName, DWORD dwStyle,
		   const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist=NULL,
		   BOOL bStorage=FALSE, BSTR bstrLicKey=NULL);
	   BOOL CreateControl(LPCTSTR pszClass, LPCTSTR pszWindowName, DWORD dwStyle,
		   const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist=NULL,
		   BOOL bStorage=FALSE, BSTR bstrLicKey=NULL);
   protected:
      CWnd* m_pParentWnd;
   private:
      DWORD m_dwThreadId;
   };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPATCHHELPER_H__38D11973_A43A_11D4_9A0B_00A0C9EDD467__INCLUDED_)
