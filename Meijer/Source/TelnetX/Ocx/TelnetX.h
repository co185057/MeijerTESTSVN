// TelnetX.h : Declaration of the CTelnetX

#ifndef __TELNETX_H_
#define __TELNETX_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "OcxCP.h"
#include "..\com\mabrylic.h"


#ifdef _DEBUG
	#define _COMOBJECT_DIR	_T("..\com\Debug\TelnetX.dll")
#else
	#ifdef _SHAREWARE
		#define _COMOBJECT_DIR	_T("..\com\Sample\TelnetX.dll")
	#else
		#define _COMOBJECT_DIR	_T("..\com\Retail\TelnetX.dll")
	#endif
#endif

#import _COMOBJECT_DIR \
		raw_interfaces_only, raw_native_types, no_namespace, named_guids \
		exclude(_T("TelnetXStatesEnum")), \
		exclude(_T("TelnetXBlockingModesEnum")), \
		exclude(_T("TelnetXImplementSpecialCodes")), \
		exclude(_T("FirewallTypeEnum")), \
		exclude(_T("ProtocolsEnum")),\
		exclude(_T("EncryptionsEnum")), \
		exclude(_T("TelnetXTypesEnum"))


/////////////////////////////////////////////////////////////////////////////
// CTelnetX
class ATL_NO_VTABLE CTelnetX : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ITelnetX, &IID_ITelnetX, &LIBID_MabryTelnetXOCXLib>,
	public CComControl<CTelnetX>,
	public IPersistStreamInitImpl<CTelnetX>,
	public IOleControlImpl<CTelnetX>,
	public IOleObjectImpl<CTelnetX>,
	public IOleInPlaceActiveObjectImpl<CTelnetX>,
	public IViewObjectExImpl<CTelnetX>,
	public IOleInPlaceObjectWindowlessImpl<CTelnetX>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CTelnetX>,
	public IPersistStorageImpl<CTelnetX>,
	public IPersistPropertyBagImpl<CTelnetX>,
	public ISpecifyPropertyPagesImpl<CTelnetX>,
	public IQuickActivateImpl<CTelnetX>,
	public IDataObjectImpl<CTelnetX>,
	public IProvideClassInfo2Impl<&CLSID_TelnetX, &DIID__ITelnetXEvents, &LIBID_MabryTelnetXOCXLib>,
	public IPropertyNotifySinkCP<CTelnetX>,
	public CComCoClass<CTelnetX, &CLSID_TelnetX>,
	public IDispatchImpl<ITelnetXComNotify, &IID_ITelnetXComNotify, &LIBID_MabryTelnetXCOMLib>,
	public CProxy_ITelnetXEvents< CTelnetX >,
	public IObjectSafetyImpl< CTelnetX, INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA >,
	public CMabryLicense<CTelnetX, &CLSID_TelnetX>

{
public:
	CComQIPtr<ITelnetXCom> m_TelnetXCom;
	ITelnetXNotify* m_NotificationObject;


	CTelnetX()
	{
		USES_CONVERSION;
		SIZEL size = { 32, 32 };

		m_bWindowOnly = TRUE; 
		m_bAutoSize = TRUE;
		AtlPixelToHiMetric( &size, &m_sizeExtent );
		m_NotificationObject=NULL;
		m_bRequiresSave = TRUE;
	}

	~CTelnetX()
	{
		m_NotificationObject=NULL;
		m_TelnetXCom.Release();
	}


DECLARE_REGISTRY_RESOURCEID(IDR_TELNETX)
#ifndef DEBUG

	HRESULT CoCreateInstanceLic(CLSID clsid, CComBSTR& bstr, IUnknown** lpUnknown)
	{
		IClassFactory2  *pIUnknown = NULL;
		IUnknown		*pOuter   = NULL;
		
		HRESULT hr = 
			CoGetClassObject( clsid,
                            CLSCTX_INPROC_SERVER,
                            NULL,
                            IID_IClassFactory2,
                            (void**) &pIUnknown );      
		if(hr != S_OK)
			return hr;
		return pIUnknown->CreateInstanceLic( pOuter,
                                    NULL,
                                    IID_IUnknown,
#ifdef _SHAREWARE
									NULL,
#else
                                    bstr,
#endif
                                    (void**)lpUnknown );
	}

	HRESULT FinalConstruct()
	{
		HRESULT	hr;
		IUnknown* punk = NULL;

		if( (hr = CoCreateInstanceLic(CLSID_TelnetXCom, CComBSTR(_T("20010533")), &punk)) != S_OK )
			return hr;
		m_TelnetXCom = punk;
		if( m_TelnetXCom.p == NULL )
			return E_FAIL;
		m_TelnetXCom->put_NotificationObject(this);
		return S_OK;
	}
#else

	HRESULT FinalConstruct()
	{
		m_TelnetXCom.CoCreateInstance(CLSID_TelnetXCom);
		m_TelnetXCom.p->put_NotificationObject(this);
		return S_OK;
	}
#endif


DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTelnetX)
	COM_INTERFACE_ENTRY(ITelnetX)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IPersistPropertyBag)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY2(IDispatch, ITelnetX)
	COM_INTERFACE_ENTRY(ITelnetXComNotify)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_PROP_MAP(CTelnetX)
	PROP_ENTRY(_T("LicenseKey"),DISPID_licensekey,CLSID_NULL)
	PROP_PAGE(CLSID_TelnetXPropPage)
	PROP_ENTRY(_T("Host"),DISPID_host,CLSID_TelnetXPropPage)
	PROP_ENTRY(_T("Port"),DISPID_port,CLSID_TelnetXPropPage)
	PROP_ENTRY(_T("PromptString"),DISPID_prompt,CLSID_TelnetXPropPage)
	PROP_ENTRY(_T("Timeout"),DISPID_timeout,CLSID_TelnetXPropPage)
	PROP_ENTRY(_T("DebugMode"),DISPID_debugmode,CLSID_TelnetXPropPage)
	PROP_ENTRY(_T("Blocking"),DISPID_blocking,CLSID_TelnetXPropPage)
	PROP_ENTRY(_T("BlockingMode"),DISPID_blockingmode,CLSID_TelnetXPropPage)
	PROP_PAGE(CLSID_TelnetXProtocolPropPage)
	PROP_ENTRY(_T("Protocol"),DISPID_protocol,CLSID_TelnetXProtocolPropPage)
	PROP_ENTRY(_T("AutoLogon"),DISPID_autologon,CLSID_TelnetXProtocolPropPage)
	PROP_ENTRY(_T("LogonName"),DISPID_logonname,CLSID_TelnetXProtocolPropPage)
	PROP_ENTRY(_T("LogonPassword"),DISPID_logonpassword,CLSID_TelnetXProtocolPropPage)
	PROP_ENTRY(_T("LogonCommand"),DISPID_logoncommand,CLSID_TelnetXProtocolPropPage)
	PROP_ENTRY(_T("Encryption"),DISPID_encryption,CLSID_TelnetXProtocolPropPage)
	PROP_PAGE(CLSID_TelnetXFirewallPropPage)
	PROP_ENTRY(_T("FirewallType"),DISPID_firewalltype,CLSID_TelnetXFirewallPropPage)
	PROP_ENTRY(_T("FirewallHost"),DISPID_firewallhost,CLSID_TelnetXFirewallPropPage)
	PROP_ENTRY(_T("FirewallPort"),DISPID_firewallport,CLSID_TelnetXFirewallPropPage)
	PROP_ENTRY(_T("FirewallLogonName"),DISPID_firewalllogonname,CLSID_TelnetXFirewallPropPage)
	PROP_ENTRY(_T("FirewallPassword"),DISPID_firewallpassword,CLSID_TelnetXFirewallPropPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CTelnetX)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(DIID__ITelnetXEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CTelnetX)
	CHAIN_MSG_MAP(CComControl<CTelnetX>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] = 
		{
			&IID_ITelnetX,
		};
		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(DoVerb)(LONG iVerb,LPMSG lpmsg, IOleClientSite *pActiveSite, LONG lindex,   HWND hwndParent, LPCRECT lprcPosRect)
	{
		  if (iVerb == 0)
			 return IOleObjectImpl<CTelnetX>::DoVerb(OLEIVERB_PROPERTIES, lpmsg, pActiveSite,
										lindex,  hwndParent, lprcPosRect);
		  return IOleObjectImpl<CTelnetX>::DoVerb(iVerb, lpmsg, pActiveSite,
										lindex,  hwndParent, lprcPosRect);

	}

	STDMETHOD(InitNew)( IStorage* pStorage )
	{
		SetDirty( TRUE );
		return IPersistStorageImpl<CTelnetX>::InitNew(pStorage);
	}


// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

DECLARE_CLASSFACTORY2(CLicense)

// ITelnetX
public:
	STDMETHOD(get_LogonCommand)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LogonCommand)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_TerminalType)(/*[out, retval]*/ TelnetXTypesEnum *pVal);
	STDMETHOD(put_TerminalType)(/*[in]*/ TelnetXTypesEnum newVal);
	STDMETHOD(get_TerminalName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_TerminalName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Encryption)(/*[out, retval]*/ EncryptionsEnum *pVal);
	STDMETHOD(put_Encryption)(/*[in]*/ EncryptionsEnum newVal);
	STDMETHOD(get_Protocol)(/*[out, retval]*/ ProtocolsEnum *pVal);
	STDMETHOD(put_Protocol)(/*[in]*/ ProtocolsEnum newVal);
	STDMETHOD(get_LicenseKey)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LicenseKey)(/*[in]*/ BSTR newVal);
	STDMETHOD(About)();
	STDMETHOD(get_PromptString)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_PromptString)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_LogonPassword)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LogonPassword)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_LogonName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LogonName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_AutoLogon)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_AutoLogon)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(Send)(VARIANT Data, short *retcount);
	STDMETHOD(Receive)(/*[in, optional]*/ VARIANT Type, /*[in,optional]*/ VARIANT Count,  VARIANT *retval);
	STDMETHOD(get_LocalIP)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LocalIP)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_LocalHost)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LocalHost)(/*[in]*/ BSTR newVal);
	STDMETHOD(Disconnect)();
	STDMETHOD(Connect)(/*[in, optional]*/ VARIANT Host, /*[in, optional]*/ VARIANT Port);
	STDMETHOD(get_FirewallType)(/*[out, retval]*/ FirewallTypeEnum *pVal);
	STDMETHOD(put_FirewallType)(/*[in]*/ FirewallTypeEnum newVal);
	STDMETHOD(get_FirewallPassword)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FirewallPassword)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FirewallLogonName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FirewallLogonName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FirewallPort)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_FirewallPort)(/*[in]*/ short newVal);
	STDMETHOD(get_FirewallHost)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FirewallHost)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Version)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Version)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Timeout)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_Timeout)(/*[in]*/ short newVal);
	STDMETHOD(get_StateString)(TelnetXStatesEnum State, /*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_State)(/*[out, retval]*/ TelnetXStatesEnum *pVal);
	STDMETHOD(get_LibraryName)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_LibraryName)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_LastErrorString)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_LastError)(/*[out, retval]*/ short *pVal);
	STDMETHOD(get_Port)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_Port)(/*[in]*/ short newVal);
	STDMETHOD(get_Host)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Host)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_BlockingMode)(/*[out, retval]*/ TelnetXBlockingModesEnum *pVal);
	STDMETHOD(put_BlockingMode)(/*[in]*/ TelnetXBlockingModesEnum newVal);
	STDMETHOD(get_DebugMode)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_DebugMode)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_Blocking)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Blocking)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_NotificationObject)(/*[out, retval]*/ ITelnetXNotify * *pVal);
	STDMETHOD(put_NotificationObject)(/*[in]*/ ITelnetXNotify * newVal);

	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
	HBITMAP hbm;

	if ( hbm = LoadBitmap( _Module.GetModuleInstance(), MAKEINTRESOURCE(IDB_TELNETXBIG)))
	{
		BITMAP	bitmap;
		HBITMAP	hbmSave;
		HDC		hdcOut;

		GetObject(hbm, sizeof( BITMAP ), &bitmap );

		hdcOut = CreateCompatibleDC( di.hdcDraw );

		hbmSave = (HBITMAP)SelectObject(hdcOut, hbm );

		BitBlt( di.hdcDraw, di.prcBounds->left, di.prcBounds->top,
			bitmap.bmWidth, bitmap.bmHeight, hdcOut, 0, 0,
			SRCCOPY );
		SelectObject(hdcOut, hbm);
		DeleteObject(hdcOut);
		DeleteObject(hbm);
	}
	return S_OK;
	}

	STDMETHOD(Debug)(ITelnetXCom *Object,BSTR DebugMsg)
	{
		if (m_NotificationObject == NULL) 
			Fire_Debug(DebugMsg);
		else
			m_NotificationObject->Debug(this, DebugMsg);
		return S_OK;
	}
	STDMETHOD(Connected)(ITelnetXCom *Object)
	{
		if (m_NotificationObject == NULL) 
			Fire_Connected();
		else
			m_NotificationObject->Connected(this);
		return S_OK;
	}
	STDMETHOD(Disconnected)(ITelnetXCom *Object)
	{
		if (m_NotificationObject == NULL) 
			Fire_Disconnected();
		else
			m_NotificationObject->Disconnected(this);
		return S_OK;
	}
	STDMETHOD(Received)(ITelnetXCom *Object, long BytesCount)
	{
		if (m_NotificationObject == NULL) 
			Fire_Received(BytesCount);
		else
			m_NotificationObject->Received(this, BytesCount);
		return S_OK;
	}
	STDMETHOD(StateChanged)(ITelnetXCom *Object, TelnetXStatesEnum NewState, TelnetXStatesEnum OldState)
	{
		if (m_NotificationObject == NULL) 
			Fire_StateChanged(NewState, OldState);
		else
			m_NotificationObject->StateChanged(this, NewState, OldState);
		return S_OK;
	}
	STDMETHOD(Prompt)(ITelnetXCom *Object)
	{
		if (m_NotificationObject == NULL) 
			Fire_Prompt();
		else
			m_NotificationObject->Prompt(this);
		return S_OK;
	}
	STDMETHOD(SpecialCode)(ITelnetXCom *Object, BSTR ReceivedData, BSTR *ResponseData)
	{
		if (m_NotificationObject == NULL) 
			Fire_SpecialCode(ReceivedData, ResponseData);
		else
			m_NotificationObject->SpecialCode(this, ReceivedData, ResponseData);
		return S_OK;
	}
	STDMETHOD(SSHCheckServerKey)(ITelnetXCom *Object, BSTR ServerKey, short Length, VARIANT_BOOL *Cancel)
	{
		if (m_NotificationObject == NULL) 
			Fire_SSHCheckServerKey(ServerKey, Length, Cancel);
		else
			m_NotificationObject->SSHCheckServerKey(this, ServerKey, Length, Cancel);
		return S_OK;
	}
};

#endif //__TELNETX_H_
