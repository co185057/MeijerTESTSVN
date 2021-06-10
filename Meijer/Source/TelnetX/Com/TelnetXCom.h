// TelnetXCom.h : Declaration of the CTelnetXCom

#ifndef __TELNETXCOM_H_
#define __TELNETXCOM_H_

#include "resource.h"       // main symbols
#include "ComCP.h"
#include "global.h"
#include <atlctl.h>
#include "mabrylic.h"


class CSocket;
class CSSHSocket;


/////////////////////////////////////////////////////////////////////////////
// CTelnetXCom
class ATL_NO_VTABLE CTelnetXCom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTelnetXCom, &CLSID_TelnetXCom>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CTelnetXCom>,
	public IDispatchImpl<ITelnetXCom, &IID_ITelnetXCom, &LIBID_MabryTelnetXCOMLib>,
	public CMabryLicense<CTelnetXCom, &CLSID_TelnetXCom>,
	public IObjectSafetyImpl< CTelnetXCom, INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA >,
	public CProxy_ITelnetXComEvents< CTelnetXCom >
{
public:
    CSocket                             *m_pLastSocket;
	CSSHSocket							*m_Socket;
	BOOL								m_Blocking;
	TelnetXBlockingModesEnum			m_BlockingMode;
	BOOL								m_DebugMode;
	CComBSTR							m_Host;
	short								m_Port;
	short								m_LastError;
	CComBSTR							m_LibraryName;
	TelnetXStatesEnum					m_State;
	short								m_Timeout;
	CComBSTR							m_LocalHost;
	CComBSTR							m_LocalIP;

	BOOL								m_AutoLogon;
	int									m_DidAutoLogon;
	CComBSTR							m_Prompt;
	CComBSTR							m_LogonName, m_LogonPassword;
	CComBSTR							m_LogonCommand;
	CComBSTR							m_FirewallHost,
										m_FirewallLogonName,
										m_FirewallPassword;
	short								m_FirewallPort;
	FirewallTypeEnum					m_FirewallType;

	typeOutputBuffer					m_OutputBuffer;
	BOOL								m_CanSend;
	BOOL								m_SendBinary;

	TelnetXTypesEnum					m_TerminalType;
	CComBSTR							m_TerminalName;

	_TUCHAR 						m_InputBuff[16385];
	unsigned char					   *m_InBuff;
	unsigned int						m_InputBuffLen;
	BOOL								m_FirstOnSend;
	BOOL								m_FirstOnReceive;

	BOOL								m_NeedReadMore;
	BOOL								m_BlockingDone;
	BOOL								m_CanSetState;
	ProtocolsEnum						m_Protocol;
	EncryptionsEnum						m_Encryption;

	ITelnetXComNotify				   *m_NotificationObject;

	CTelnetXCom();
	~CTelnetXCom();

DECLARE_REGISTRY_RESOURCEID(IDR_TELNETXCOM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DECLARE_CLASSFACTORY2(CLicense)

BEGIN_COM_MAP(CTelnetXCom)
	COM_INTERFACE_ENTRY(ITelnetXCom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CTelnetXCom)
CONNECTION_POINT_ENTRY(DIID__ITelnetXComEvents)
END_CONNECTION_POINT_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// ITelnetXCom
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
	STDMETHOD(get_NotificationObject)(/*[out, retval]*/ ITelnetXComNotify * *pVal);
	STDMETHOD(put_NotificationObject)(/*[in]*/ ITelnetXComNotify * newVal);
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


	//
	// Socket callbacks
	//
	void OnAccept(CSocket* pSocket, int nErrorCode);
	void OnClose(CSocket* pSocket, int nErrorCode);
	void OnConnect(CSocket* pSocket, int nErrorCode);
	void OnOutOfBandData(CSocket* pSocket, int nErrorCode){};
	void OnReceive(CSocket* pSocket, int nErrorCode);
	void OnSend(CSocket* pSocket, int nErrorCode);
	void OnTimer(CSocket *pSocket);
	void OnDebug(CSocket *pSocket, LPTSTR msg);



	HRESULT CtlError(int err);
	BOOL SetState(TelnetXStatesEnum State);
	BOOL DoFireDebug(LPTSTR text);
	BOOL AddString(LPTSTR buff, int i);
	BOOL SendString(LPTSTR text,...);
	void FlushOutBuffer(void);
	void ClearOutBuffer(void);
	void ImplementCustomCodes(_TUCHAR *inbuff, int *inlen, _TUCHAR *outbuf, int *outlen);
	void DoSpecialCode(void);
	BSTR GetBstrFromStr(LPTSTR str, int len);
	int GetStrFromBstr(BSTR bstr, LPTSTR str);
	void ProcessTelnetReceivedOption(int option, _TUCHAR *buffer, int length);
	void RefreshTimer(void);
	int Block(void);
	BOOL CheckServerKey(LPTSTR strkey, int length, int *retval);

	BOOL FireConnected(void)
	{
		long olddwRef;
		this->AddRef();

		if( m_NotificationObject == NULL )
			Fire_Connected();
		else
			m_NotificationObject->Connected(this);
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireDisconnected(void)
	{
		long olddwRef;
		this->AddRef();

		if( m_NotificationObject == NULL )
			Fire_Disconnected();
		else
			m_NotificationObject->Disconnected(this);
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireReceived(long BytesCount)
	{
		long olddwRef;
		this->AddRef();


		if( m_NotificationObject == NULL )
			Fire_Received(BytesCount);
		else
			m_NotificationObject->Received(this, BytesCount);
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireDebug(BSTR DebugMsg)
	{
		long olddwRef;
		this->AddRef();

		if( m_NotificationObject == NULL )
			Fire_Debug(DebugMsg);
		else
			m_NotificationObject->Debug(this, DebugMsg);
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireStateChanged(TelnetXStatesEnum NewState, TelnetXStatesEnum OldState)
	{
		long olddwRef;
		this->AddRef();

		if( m_NotificationObject == NULL )
			Fire_StateChanged(NewState, OldState);
		else
			m_NotificationObject->StateChanged(this, NewState, OldState);
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FirePrompt(void)
	{
		long olddwRef;
		this->AddRef();

		if( m_NotificationObject == NULL )
			Fire_Prompt();
		else
			m_NotificationObject->Prompt(this);
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireSpecialCode(BSTR ReceivedData, BSTR *ResponseData)
	{
		long olddwRef;
		this->AddRef();

		if( m_NotificationObject == NULL )
			Fire_SpecialCode(ReceivedData, ResponseData);
		else
			m_NotificationObject->SpecialCode(this, ReceivedData, ResponseData);
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
	BOOL FireSSHCheckServerKey(BSTR ServerKey, short Length, VARIANT_BOOL *Cancel)
	{
		long olddwRef;
		this->AddRef();

		if( m_NotificationObject == NULL )
			Fire_SSHCheckServerKey(ServerKey, Length, Cancel);
		else
			m_NotificationObject->SSHCheckServerKey(this, ServerKey, Length, Cancel);
		olddwRef=m_dwRef-1;
		this->Release();
		if (!olddwRef)
			return FALSE;

		return TRUE;
	}
};

#endif //__TELNETXCOM_H_
