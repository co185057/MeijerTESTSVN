// TelnetX.cpp : Implementation of CTelnetX

#include "stdafx.h"
#include "Ocx.h"
#include "TelnetX.h"


#define PropertyChanged(prop) \
	SetDirty(TRUE); \
	FireOnChanged(DISPID_##prop)

/////////////////////////////////////////////////////////////////////////////
// CTelnetX

STDMETHODIMP CTelnetX::get_Blocking(VARIANT_BOOL *pVal)
{
	return m_TelnetXCom.p->get_Blocking(pVal);
}

STDMETHODIMP CTelnetX::put_Blocking(VARIANT_BOOL newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_Blocking(newVal);
	PropertyChanged(blocking);
	return hr;
}

STDMETHODIMP CTelnetX::get_DebugMode(VARIANT_BOOL *pVal)
{
	return m_TelnetXCom.p->get_DebugMode(pVal);
}

STDMETHODIMP CTelnetX::put_DebugMode(VARIANT_BOOL newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_DebugMode(newVal);
	PropertyChanged(debugmode);
	return hr;
}

STDMETHODIMP CTelnetX::get_BlockingMode(TelnetXBlockingModesEnum *pVal)
{
	return m_TelnetXCom.p->get_BlockingMode(pVal);
}

STDMETHODIMP CTelnetX::put_BlockingMode(TelnetXBlockingModesEnum newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_BlockingMode(newVal);
	PropertyChanged(blockingmode);
	return hr;
}

STDMETHODIMP CTelnetX::get_Host(BSTR *pVal)
{
	return m_TelnetXCom.p->get_Host(pVal);
}

STDMETHODIMP CTelnetX::put_Host(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_Host(newVal);
	PropertyChanged(host);
	return hr;
}

STDMETHODIMP CTelnetX::get_Port(short *pVal)
{
	return m_TelnetXCom.p->get_Port(pVal);
}

STDMETHODIMP CTelnetX::put_Port(short newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_Port(newVal);
	PropertyChanged(port);
	return hr;
}

STDMETHODIMP CTelnetX::get_LastError(short *pVal)
{
	return m_TelnetXCom.p->get_LastError(pVal);
}

STDMETHODIMP CTelnetX::get_LastErrorString(BSTR *pVal)
{
	return m_TelnetXCom.p->get_LastErrorString(pVal);
}

STDMETHODIMP CTelnetX::get_LibraryName(BSTR *pVal)
{
	return m_TelnetXCom.p->get_LibraryName(pVal);
}

STDMETHODIMP CTelnetX::put_LibraryName(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_LibraryName(newVal);
	PropertyChanged(libraryname);
	return hr;
}

STDMETHODIMP CTelnetX::get_State(TelnetXStatesEnum *pVal)
{
	return m_TelnetXCom.p->get_State(pVal);
}

STDMETHODIMP CTelnetX::get_StateString(TelnetXStatesEnum State, BSTR *pVal)
{
	return m_TelnetXCom.p->get_StateString(State, pVal);
}

STDMETHODIMP CTelnetX::get_Timeout(short *pVal)
{
	return m_TelnetXCom.p->get_Timeout(pVal);
}

STDMETHODIMP CTelnetX::put_Timeout(short newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_Timeout(newVal);
	PropertyChanged(timeout);
	return hr;
}

STDMETHODIMP CTelnetX::get_Version(BSTR *pVal)
{
	return m_TelnetXCom.p->get_Version(pVal);
}

STDMETHODIMP CTelnetX::put_Version(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_Version(newVal);
	PropertyChanged(version);
	return hr;
}

STDMETHODIMP CTelnetX::get_LocalHost(BSTR *pVal)
{
	return m_TelnetXCom.p->get_LocalHost(pVal);
}

STDMETHODIMP CTelnetX::put_LocalHost(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_LocalHost(newVal);
	PropertyChanged(localhost);
	return hr;
}

STDMETHODIMP CTelnetX::get_LocalIP(BSTR *pVal)
{
	return m_TelnetXCom.p->get_LocalIP(pVal);
}

STDMETHODIMP CTelnetX::put_LocalIP(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_LocalIP(newVal);
	PropertyChanged(localip);
	return hr;
}

STDMETHODIMP CTelnetX::get_FirewallHost(BSTR *pVal)
{
	return m_TelnetXCom.p->get_FirewallHost(pVal);
}

STDMETHODIMP CTelnetX::put_FirewallHost(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_FirewallHost(newVal);
	PropertyChanged(firewallhost);
	return hr;
}

STDMETHODIMP CTelnetX::get_FirewallPort(short *pVal)
{
	return m_TelnetXCom.p->get_FirewallPort(pVal);
}

STDMETHODIMP CTelnetX::put_FirewallPort(short newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_FirewallPort(newVal);
	PropertyChanged(firewallport);
	return hr;
}

STDMETHODIMP CTelnetX::get_FirewallLogonName(BSTR *pVal)
{
	return m_TelnetXCom.p->get_FirewallLogonName(pVal);
}

STDMETHODIMP CTelnetX::put_FirewallLogonName(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_FirewallLogonName(newVal);
	PropertyChanged(firewalllogonname);
	return hr;
}

STDMETHODIMP CTelnetX::get_FirewallPassword(BSTR *pVal)
{
	return m_TelnetXCom.p->get_FirewallPassword(pVal);
}

STDMETHODIMP CTelnetX::put_FirewallPassword(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_FirewallPassword(newVal);
	PropertyChanged(firewallpassword);
	return hr;
}

STDMETHODIMP CTelnetX::get_FirewallType(FirewallTypeEnum *pVal)
{
	return m_TelnetXCom.p->get_FirewallType(pVal);
}

STDMETHODIMP CTelnetX::put_FirewallType(FirewallTypeEnum newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_FirewallType(newVal);
	PropertyChanged(firewalltype);
	return hr;
}

STDMETHODIMP CTelnetX::Receive(VARIANT Type, VARIANT Count, VARIANT *retval)
{
	return m_TelnetXCom.p->Receive(Type, Count, retval);
}

STDMETHODIMP CTelnetX::Send(VARIANT newVal, short *retcount)
{
	return m_TelnetXCom.p->Send(newVal, retcount);
}

STDMETHODIMP CTelnetX::Connect(VARIANT Host, VARIANT Port)
{
	return m_TelnetXCom.p->Connect(Host, Port);
}

STDMETHODIMP CTelnetX::Disconnect()
{
	return m_TelnetXCom.p->Disconnect();
}

STDMETHODIMP CTelnetX::get_AutoLogon(VARIANT_BOOL *pVal)
{
	return m_TelnetXCom.p->get_AutoLogon(pVal);
}

STDMETHODIMP CTelnetX::put_AutoLogon(VARIANT_BOOL newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_AutoLogon(newVal);
	PropertyChanged(autologon);
	return hr;
}

STDMETHODIMP CTelnetX::get_LogonName(BSTR *pVal)
{
	return m_TelnetXCom.p->get_LogonName(pVal);
}

STDMETHODIMP CTelnetX::put_LogonName(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_LogonName(newVal);
	PropertyChanged(logonname);
	return hr;
}

STDMETHODIMP CTelnetX::get_LogonPassword(BSTR *pVal)
{
	return m_TelnetXCom.p->get_LogonPassword(pVal);
}

STDMETHODIMP CTelnetX::put_LogonPassword(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_LogonPassword(newVal);
	PropertyChanged(logonpassword);
	return hr;
}

STDMETHODIMP CTelnetX::get_PromptString(BSTR *pVal)
{
	return m_TelnetXCom.p->get_PromptString(pVal);
}

STDMETHODIMP CTelnetX::put_PromptString(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_PromptString(newVal);
	PropertyChanged(prompt);
	return hr;
}

STDMETHODIMP CTelnetX::About()
{
	return m_TelnetXCom.p->About();
}

STDMETHODIMP CTelnetX::get_NotificationObject(ITelnetXNotify **pVal)
{
	*pVal=m_NotificationObject;
	return S_OK;
}

STDMETHODIMP CTelnetX::put_NotificationObject(ITelnetXNotify *newVal)
{
	m_NotificationObject=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetX::get_LicenseKey(BSTR *pVal)
{
	return m_TelnetXCom.p->get_LicenseKey(pVal);
}

STDMETHODIMP CTelnetX::put_LicenseKey(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_LicenseKey(newVal);
	PropertyChanged(licensekey);
	return hr;
}

STDMETHODIMP CTelnetX::get_Protocol(ProtocolsEnum *pVal)
{
	return m_TelnetXCom.p->get_Protocol(pVal);
}

STDMETHODIMP CTelnetX::put_Protocol(ProtocolsEnum newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_Protocol(newVal);
	PropertyChanged(protocol);
	return hr;
}

STDMETHODIMP CTelnetX::get_Encryption(EncryptionsEnum *pVal)
{
	return m_TelnetXCom.p->get_Encryption(pVal);
}

STDMETHODIMP CTelnetX::put_Encryption(EncryptionsEnum newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_Encryption(newVal);
	PropertyChanged(encryption);
	return hr;
}

STDMETHODIMP CTelnetX::get_LogonCommand(BSTR *pVal)
{
	return m_TelnetXCom.p->get_LogonCommand(pVal);
}

STDMETHODIMP CTelnetX::put_LogonCommand(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_LogonCommand(newVal);
	PropertyChanged(logoncommand);
	return hr;
}

STDMETHODIMP CTelnetX::get_TerminalName(BSTR *pVal)
{
	return m_TelnetXCom.p->get_TerminalName(pVal);
}

STDMETHODIMP CTelnetX::put_TerminalName(BSTR newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_TerminalName(newVal);
	PropertyChanged(terminalname);
	return hr;
}

STDMETHODIMP CTelnetX::get_TerminalType(TelnetXTypesEnum *pVal)
{
	return m_TelnetXCom.p->get_TerminalType(pVal);
}

STDMETHODIMP CTelnetX::put_TerminalType(TelnetXTypesEnum newVal)
{
	HRESULT hr=m_TelnetXCom.p->put_TerminalType(newVal);
	PropertyChanged(terminaltype);
	return hr;
}

