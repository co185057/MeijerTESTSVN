// TelnetClientIF.h : Declaration of the CTelnetClientIF

#ifndef __TELNETCLIENTIF_H_
#define __TELNETCLIENTIF_H_

//#import <TelnetX.DLL>  raw_interfaces_only, raw_native_types, no_namespace, named_guids 
#include <TelnetX.tlh>
#include "resource.h"       // main symbols
typedef BOOL  (CALLBACK *FPEVENTHANDLER)(UINT, WPARAM, LPARAM);

//lint -esym(1748, CTelnetClientIF) ATL requires multiple instances of same base class

/////////////////////////////////////////////////////////////////////////////
// CTelnetClientIF
class ATL_NO_VTABLE CTelnetClientIF : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTelnetClientIF, &CLSID_TelnetClientIF>,
	public IDispatchImpl<ITelnetClientIF, &IID_ITelnetClientIF, &LIBID_TELNETCLIENTSUPPORTLib>,
	public IDispatchImpl<ITelnetXComNotify, &IID_ITelnetXComNotify, &LIBID_MabryTelnetXCOMLib>
{
public:
	CTelnetClientIF();
    //lint -e(1510) Don't care about ATL base dtors.        
    ~CTelnetClientIF(); 

DECLARE_REGISTRY_RESOURCEID(IDR_TELNETCLIENTIF)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTelnetClientIF)
	COM_INTERFACE_ENTRY(ITelnetClientIF)
//DEL 	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY2(IDispatch, ITelnetClientIF)
	COM_INTERFACE_ENTRY(ITelnetXComNotify)
END_COM_MAP()

// ITelnetClientIF
public:
	STDMETHOD(Disconnect)();
	STDMETHOD(Connect)(/*[in]*/ BSTR bstrHostName, /*[in]*/ long lPortNum);
	STDMETHOD(TransmitString)(/*[in]*/ BSTR bstrSendString);
	STDMETHOD(ReceiveString)(/*[out]*/BSTR *bstrInputString, /*[in]*/long lCharCount);
	STDMETHOD(SetCallback)(/*[in]*/ VARIANT pfnCallbackAddr);
// ITelnetXComNotify
	STDMETHOD(Disconnected)(ITelnetXCom * Object);
	STDMETHOD(Received)(ITelnetXCom * Object, LONG BytesCount);
	STDMETHOD(Connected)(ITelnetXCom * )
	{
		return E_NOTIMPL;
	}
	STDMETHOD(Debug)(ITelnetXCom * , BSTR )
	{
		return E_NOTIMPL;
	}
	STDMETHOD(StateChanged)(ITelnetXCom * , TelnetXStatesEnum , TelnetXStatesEnum )
	{
		return E_NOTIMPL;
	}
	STDMETHOD(Prompt)(ITelnetXCom * )
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SpecialCode)(ITelnetXCom * , BSTR , BSTR * )
	{
		return E_NOTIMPL;
	}
	STDMETHOD(SSHCheckServerKey)(ITelnetXCom * , BSTR , SHORT , VARIANT_BOOL * )
	{
		return E_NOTIMPL;
	}
protected:
	ITelnetXComPtr m_pTelnetConnection;
	FPEVENTHANDLER m_pfnCallback;
	bool m_bDisconnecting;  // TAR 343530
};

#endif //__TELNETCLIENTIF_H_
