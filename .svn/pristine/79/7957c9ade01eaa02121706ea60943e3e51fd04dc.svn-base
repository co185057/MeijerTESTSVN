// TelnetXCom.cpp : Implementation of CTelnetXCom
#include "stdafx.h"
#include "Com.h"
#include "global.h"
#include "TelnetXCom.h"
#include "version.h"
#include "global.h"
#include "cstr.h"
#include "cmsocket.h"
#include "csocket.h"
#include "csshsocket.h"

LPTSTR TelnetKeywords[]={_T("WILL"),_T("WON'T"),_T("DO"),_T("DON'T")};
#define oWILL	0
#define oWONT	1
#define oDO		2
#define oDONT	3
#define oSB		4
#define IAC		255

/////////////////////////////////////////////////////////////////////////////
// CTelnetXCom

CTelnetXCom::CTelnetXCom()
{
	m_Blocking=FALSE;
	m_BlockingMode = TrueBlocking;
	m_DebugMode=FALSE;
	m_Port=23;
	m_LastError=0;
	m_State=StateNotConnected;
	m_Timeout=30;
	m_SendBinary=FALSE;

	m_FirewallPort=23;
	m_FirewallType=FirewallTypeNone;

	m_InputBuffLen=0;
	m_NeedReadMore;
	m_InBuff=m_InputBuff;
	m_FirstOnSend=TRUE;
	m_FirstOnReceive=TRUE;
	m_BlockingDone=FALSE;

	m_TerminalType=Automatic;
	m_TerminalName=_T("UNKNOWN");

	m_DidAutoLogon = 0;
	m_AutoLogon = FALSE;
	m_Prompt.Empty();

	m_Socket=NULL;
	m_CanSetState=TRUE;

	m_Protocol=Telnet;
	m_Encryption=TripleDES;

	CComBSTR libname=_T("WSOCK32.DLL");
	put_LibraryName(libname);

	m_NotificationObject=NULL;

	if (m_Socket)
	{
		CString LocalHostName, LocalHostIP;
		if (m_Socket->GetHostName(LocalHostName, LocalHostIP))
		{
			m_LocalIP = LocalHostIP;
			m_LocalHost = LocalHostName;
		}
		else
		{
			m_LocalIP = _T("127.0.0.1");
			m_LocalHost = _T("localhost");
		}
	}
	else
		{
			m_LocalHost=_T("localhost");
			m_LocalIP=_T("127.0.0.1");
		}
}

CTelnetXCom::~CTelnetXCom()
{
	if (m_Socket)
		delete m_Socket;
	ClearOutBuffer();
}

ErrTable ErrLookupTable[] =
{
	{ 0, _T("No error.") },
	{ 13, _T("Type mismatch.") },
	{ 380, _T("Invalid Property Value.") },
	{ WSAEINTR, _T("system level interrupt interrupted socket operation.") },
	{ WSAEBADF, _T("Generic error for invalid format, bad format.") },
	{ WSAEACCES, _T("Generic error for access violation.") },
	{ WSAEFAULT, _T("Generic error for fault.") },
	{ WSAEINVAL, _T("Generic error for invalid format, entry, etc.") },
	{ WSAEMFILE, _T("Generic error for file error.") },
	{ 10025, _T("The IP address provided is not valid or the host specified by the IP does not exist.") },
	{ WSAENOTSOCK, _T("Invalid socket or not connected to remote.") },
	{ WSAEADDRINUSE, _T("The specified address is already in use.") },
	{ WSAEADDRNOTAVAIL, _T("The specified address is not available.") },
	{ WSAENETDOWN, _T("The connected network is not available.") },
	{ WSAENETUNREACH, _T("The connected network is not reachable.") },
	{ WSAENETRESET, _T("The connected network connection has been reset.") },
	{ WSAECONNABORTED, _T("The current connection has been aborted by the network or intermediate services.") },
	{ WSAECONNRESET, _T("The current socket connection has been reset.") },
	{ WSAENOTCONN, _T("The current socket has not been connected.") },
	{ WSAESHUTDOWN, _T("The connection has been shutdown.") },
	{ WSAETIMEDOUT, _T("The current connection has timedout.") },
	{ WSAECONNREFUSED, _T("The requested connection has been refused by the remote host.") },
	{ WSAENAMETOOLONG, _T("Specified host name is too long.") },
	{ WSAEHOSTDOWN, _T("Remote host is currently unavailable.") },
	{ WSAEHOSTUNREACH, _T("Remote host is currently unreachable. ") },
	{ WSASYSNOTREADY, _T("Remote system is not ready.") },
	{ WSAVERNOTSUPPORTED, _T("Current socket version not supported by application.") },
	{ WSANOTINITIALISED, _T("Socket API is not initialized.") },
	{ WSAEDISCON, _T("Socket has been disconnected.") },
	{ WSAHOST_NOT_FOUND , _T("Host not found.") },
	{ 20000 , _T("Cannot switch stacks in this time.") },
	{ 20001 , _T("Socket busy. Use Disconnect first.")},
	{ 20002 , _T("Operation invalid in this state.") },
	{ 20003 , _T("For RSH and SSH protocols you need to specify username and password.") },
	{ 20004 , _T("Server response contains invalid characters.") },
	{ 20005 , _T("For RExec protocol you must setup LogonCommand variable.") },
	{ 20006 , _T("You must set AutoLogon, LogonName and LogonPassword variables for this protocol.") },
	{ 20007 , _T("Not connected to server.") },
	{ 20008 , _T("Could not get free port between 512 and 1024.") },
	{ 20009 , _T("TelnetProxy is only available for Telnet protocol.") },
	{ 20010 , _T("Unknown variant type. Try to be more specific.") },
	{ -1, _T("Unrecognized Error.") }
};

LPTSTR GetErrorString(int err)
{
	LPErrTable p = ErrLookupTable;
	
	while (p->code != -1)
	{
		if (p->code == err)
			return p->msg;
		p++;
	}
	return p->msg;
}

ErrTable StateLookupTable[] =
{
	{ StateNotConnected, _T("Not connected to server")},
	{ StateConnecting, _T("Connecting to remote server")},
	{ StateConnected, _T("Connected - idle")},
	{ StateSendingLogonName, _T("Sending username to server")},
	{ StateSendingPassword, _T("Sending password to server")},
	{ StateReceivingData, _T("Receiving data from server")},
	{ StateReceivedPrompt, _T("Received command prompt")},
	{ StateSendingData, _T("Sending data to server")},
	{ StateDisconnecting, _T("Disconnecting from server")},
	{ -1, _T("Invalid state") }
};

TCHAR* GetStateString(int err)
{
	LPErrTable p = StateLookupTable;
	
	while (p->code != -1)
	{
		if (p->code == err)
			return p->msg;
		p++;
	}
	return p->msg;
}


HRESULT CTelnetXCom::CtlError(int err)
{
	m_LastError=err;
	return Error(GetErrorString(err), CLSID_TelnetXCom, CUSTOM_CTL_SCODE(err));
}

STDMETHODIMP CTelnetXCom::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ITelnetXCom
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CTelnetXCom::get_Blocking(VARIANT_BOOL *pVal)
{
	*pVal=m_Blocking?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_Blocking(VARIANT_BOOL newVal)
{
	RuntimeCheck();

	m_Blocking=newVal?TRUE:FALSE;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_DebugMode(VARIANT_BOOL *pVal)
{
	*pVal=m_DebugMode?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_DebugMode(VARIANT_BOOL newVal)
{
	RuntimeCheck();



	m_DebugMode=newVal?TRUE:FALSE;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_BlockingMode(TelnetXBlockingModesEnum *pVal)
{
	*pVal=m_BlockingMode;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_BlockingMode(TelnetXBlockingModesEnum newVal)
{
	RuntimeCheck();

	m_BlockingMode=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_Host(BSTR *pVal)
{
	*pVal=m_Host.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_Host(BSTR newVal)
{
	RuntimeCheck();

	m_Host.Empty();
	m_Host=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_Port(short *pVal)
{
	*pVal=m_Port;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_Port(short newVal)
{
	RuntimeCheck();

	m_Port=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_LastError(short *pVal)
{
	*pVal=m_LastError;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_LastErrorString(BSTR *pVal)
{
	CComBSTR bstr;

	bstr=GetErrorString(m_LastError);
	*pVal=bstr.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_LibraryName(BSTR *pVal)
{
	*pVal=m_LibraryName.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_LibraryName(BSTR newVal)
{
	if (m_State != StateNotConnected)
		return CtlError(20000);

	m_LibraryName.Empty();
	m_LibraryName=newVal;

	if (m_Socket)
		delete m_Socket;
	m_Socket=NULL;

	CString tlib=m_LibraryName;
	m_Socket = new CSSHSocket(this,tlib.GetBuffer(tlib.GetLength()+1));
	m_Socket->m_Owner=this;
	tlib.ReleaseBuffer();
	m_Socket->m_fBlocking = FALSE;
	m_Socket->m_AcceptTimeout =	m_Socket->m_ConnectTimeout = m_Socket->m_ReceiveTimeout = m_Socket->m_SendTimeout = m_Timeout;
	
	DoFireDebug(_T("Recreated socket."));

	m_CanSend=FALSE;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_State(TelnetXStatesEnum *pVal)
{
	*pVal=m_State;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_StateString(TelnetXStatesEnum State, BSTR *pVal)
{
	CComBSTR bstr;

	bstr=GetStateString(State);
	*pVal=bstr.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_Timeout(short *pVal)

{
	*pVal=m_Timeout;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_Timeout(short newVal)
{
	RuntimeCheck();

	m_Timeout=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_Version(BSTR *pVal)
{
	_TCHAR szVersion[32];
	
#ifdef _DEBUG
	wsprintf(szVersion, _T("%s (Debug)"), VERSION_STRING);
#else
#ifdef _SHAREWARE
	wsprintf(szVersion, _T("%s (Sample)"), VERSION_STRING);
#else
	lstrcpy(szVersion, VERSION_STRING);
#endif
#endif
	
	CComBSTR bstr(szVersion);
	
	*pVal = bstr.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_Version(BSTR newVal)
{
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_LocalHost(BSTR *pVal)
{
	*pVal=m_LocalHost.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_LocalHost(BSTR newVal)
{
	RuntimeCheck();

	m_LocalHost.Empty();
	m_LocalHost=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_LocalIP(BSTR *pVal)
{
	*pVal=m_LocalIP.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_LocalIP(BSTR newVal)
{
	RuntimeCheck();

	m_LocalIP.Empty();
	m_LocalIP=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_FirewallHost(BSTR *pVal)
{
	*pVal=m_FirewallHost.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_FirewallHost(BSTR newVal)
{
	RuntimeCheck();

	m_FirewallHost.Empty();
	m_FirewallHost = newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_FirewallPort(short *pVal)
{
	*pVal=m_FirewallPort;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_FirewallPort(short newVal)
{
	RuntimeCheck();

	m_FirewallPort=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_FirewallLogonName(BSTR *pVal)
{
	*pVal=m_FirewallLogonName.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_FirewallLogonName(BSTR newVal)
{
	RuntimeCheck();

	m_FirewallLogonName.Empty();
	m_FirewallLogonName=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_FirewallPassword(BSTR *pVal)
{
	*pVal=m_FirewallPassword.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_FirewallPassword(BSTR newVal)
{


	RuntimeCheck();

	m_FirewallPassword.Empty();
	m_FirewallPassword=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_FirewallType(FirewallTypeEnum *pVal)
{
	*pVal=m_FirewallType;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_FirewallType(FirewallTypeEnum newVal)
{
	RuntimeCheck();

	m_FirewallType=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::Receive(VARIANT Type, VARIANT Count, VARIANT *retval)
{
	short num=0;
	VARIANT var;
	CComBSTR *bstr;

	RuntimeCheck();

	if (m_State<StateConnected)
		return CtlError(20007);


l1:

	VariantInit(&var);

	if (Count.vt==VT_I2)
		num=Count.iVal;	
	else
		if (Count.vt==(VT_I2 | VT_BYREF))
			num= *Count.piVal;
		else
		{
			VariantChangeType(&var, &Count,0,VT_I2);
			if (var.vt==VT_I2)
				num=var.iVal;
		}

	if (!num)
		num=m_InputBuffLen;


	long i=0;
	BOOL Found=FALSE;

	if (!m_SendBinary && m_Protocol==Telnet)
	{

		while (i<num && !Found) // search for next occurance
			if (m_InBuff[i]==255) 
				Found=TRUE;
			else
				i++;

	}
	else
		i=num;

	if (m_TerminalType==None) i=num;	

	while (m_Blocking && m_Socket && !i)
	{
		m_BlockingDone=FALSE;
		Block();
		if (m_InputBuffLen)
			goto l1;
	}
	if (m_Blocking && !m_Socket)
		return CtlError(m_LastError);


	// i holds number of bytes to transfer
	if (!i)
		retval->vt=VT_EMPTY;
	else
	{
		long h=0;

		VariantInit(&var);
		if (Type.vt==VT_I4)
			h=Type.lVal;
		else
			if (Type.vt==(VT_I4 | VT_BYREF))
				h = *Type.plVal;
			else
			{
				VariantChangeType(&var, &Type, 0, VT_I4);
				if (var.vt==VT_I4)
					h=var.lVal;
			}
		
		if (h==17 || h>=8192) //either vbByte or vbArray
		{
			SAFEARRAY *psa;
			SAFEARRAYBOUND rgsabound[1];
			_TCHAR HUGEP *data;

			rgsabound[0].lLbound=0;
			rgsabound[0].cElements=i;
		
			psa = SafeArrayCreate(VT_UI1,1,rgsabound);
			if (!psa)
				return CtlError(WSA_NOT_ENOUGH_MEMORY);

			SafeArrayAccessData(psa, (void HUGEP* FAR*)&data);
			memcpy(data, m_InBuff, i);
			SafeArrayUnaccessData(psa);
			
			retval->vt=VT_ARRAY | VT_UI1;
			retval->parray=psa;
		}
		else
		{
			long j;
			LPTSTR p;

			// get that amount of memory * 2
			p =(LPTSTR )malloc(i*2 + 1);
			if (!p)
				return CtlError(WSA_NOT_ENOUGH_MEMORY);
			j=::MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)m_InBuff, i, (LPWSTR)p, i*2 + 1);
			if (!j)
			{
				free(p);
				return CtlError(WSA_NOT_ENOUGH_MEMORY);
			}
			bstr = new CComBSTR(j, (LPOLESTR)p);
			free(p);
			retval->vt=VT_BSTR;
			retval->bstrVal=bstr->Copy();
			delete bstr;
		}

		m_InBuff+=i;
		m_InputBuffLen-=i;
	}

	if (m_InputBuffLen)
		::PostMessage(m_Socket->m_hwndSocket, WM_SOCKET_SSL_NOTIFY,0,0);
    if (m_NeedReadMore == TRUE)
    {
        m_NeedReadMore = FALSE;
        OnReceive(m_pLastSocket, m_LastError);
    }

	return S_OK;
}

STDMETHODIMP CTelnetXCom::Send(VARIANT newVal, short *retcount)
{
	int i;
	SAFEARRAY *psa;
	_TCHAR HUGEP *data;
	long j;
	LPTSTR p;

	RuntimeCheck();

	if (m_State<StateConnected)
		return CtlError(20007);

	if (!(newVal.vt & VT_ARRAY))
	{
		switch (newVal.vt)
		{
			case VT_BOOL: // boolean
				AddString((LPTSTR )&newVal.boolVal,1);
				break;
				
			case VT_UI1: // integer
				AddString((LPTSTR )&newVal.bVal,1);
				break;
				
			case VT_I2:
				AddString((LPTSTR )&newVal.iVal, sizeof(newVal.iVal));
				break;
				
			case VT_I4:
				AddString((LPTSTR )&newVal.lVal, sizeof(newVal.lVal));
				break;
				
			case VT_BSTR:
				i = SysStringLen(newVal.bstrVal);
				p=(LPTSTR )malloc(i+1);
				if (!p)
					return CtlError(WSA_NOT_ENOUGH_MEMORY);
				WideCharToMultiByte(CP_ACP,0,newVal.bstrVal,i,p,i,0,0);
				AddString(p,i);
				free(p);
				break;
				
			case VT_BYREF | VT_BSTR:
				i = SysStringLen(*newVal.pbstrVal);
				p=(LPTSTR )malloc(i+1);
				if (!p)
					return CtlError(WSA_NOT_ENOUGH_MEMORY);
				WideCharToMultiByte(CP_ACP,0,*newVal.pbstrVal,i,p,i,0,0);
				AddString(p,i);
				free(p);
				break;
				
			default:
				return CtlError(20010);
		}
	}
	else
	{
		if (V_ISBYREF(&newVal))
			psa = *(newVal.pparray);
		else
			psa = newVal.parray;

		SafeArrayAccessData(psa, (void HUGEP* FAR*)&data);
		switch (newVal.vt - VT_ARRAY)
		{
			case VT_UI1:	
			case VT_I2:
			case VT_I4:
			case VT_BOOL:
				break;
				
			default:
				SafeArrayUnaccessData(psa);
				return CtlError(20010);
		}
		
		j = psa->cbElements*psa->rgsabound[0].cElements; // amount of data to copy
		AddString((LPTSTR )psa->pvData, j);
		SafeArrayUnaccessData(psa);
	}

	SetState(StateSendingData);
	FlushOutBuffer();

	return S_OK;
}

BOOL CTelnetXCom::DoFireDebug(LPTSTR text)
{
	CComBSTR DebugMsg;
	
	if (m_DebugMode)
	{
		VARIANT_BOOL fCancel = VARIANT_FALSE;
		
		DebugMsg.Empty();
		DebugMsg = text;
		return FireDebug(DebugMsg.Copy());
	}
	
	return TRUE;
}

BOOL CTelnetXCom::AddString(LPTSTR buff, int i)
{
	tagOutBuffer *outbuf;
	//	int j;
	
	if (buff)
	{
		outbuf = new tagOutBuffer;
		outbuf->Data = (LPTSTR )malloc(i + 2);
		//		j = 0;
		memcpy(outbuf->Data, buff, i);
		//		while (j < i) outbuf->Data[j] = buff[j++];
		outbuf->Length = i;
		m_OutputBuffer.push_back(outbuf);
	}	
	return TRUE;
}

BOOL CTelnetXCom::SendString(LPTSTR text,...)
{
	va_list marker;
	_TCHAR buff[10240];
	
	
	if (text && text[0])
	{
		va_start(marker, text);
		wvsprintf(buff, text, marker);
		va_end(marker);
		
		if (buff[0])
			AddString(buff, _tcsclen(buff));
	}
	
	FlushOutBuffer();
	return TRUE;
}

void CTelnetXCom::FlushOutBuffer(void)
{
	int i;
	tagOutBuffer *outbuf;
	_TCHAR buff2[10241];
	i = 1;
	
	RefreshTimer();
	while (m_OutputBuffer.size() && i > 0)
	{
		outbuf = (tagOutBuffer *)*m_OutputBuffer.begin();
		if (!outbuf)
			return; // this may never happen, or we're doomed. 
		
		// let's raise event here
		if (m_DebugMode)
		{
			memcpy(buff2,outbuf->Data,outbuf->Length);
			buff2[outbuf->Length]=0;

			// remove \r\n
			if (outbuf->Length>0)
				if (buff2[outbuf->Length-1]<31)
					buff2[outbuf->Length-1]=0;
			if (outbuf->Length>1)
				if (buff2[outbuf->Length-2]<31)
					buff2[outbuf->Length-2]=0;

			if (!DoFireDebug(buff2))
				return;

			//if (IsBadCodePtr((FARPROC)this) || (!IsValidSocket(m_hSocket))) return;
//			if (!IsValidSocket(m_hSocket)) return;
		}
		
		SetState(StateSendingData);
		i = m_Socket->Send(outbuf->Data, outbuf->Length);
		if (i>0) // has data being sent?
		{
			if (i < outbuf->Length) // is all gone?
			{// no, redefine current buffer
				_tcscpy(buff2, outbuf->Data);
				free(outbuf->Data);
				outbuf->Data = (LPTSTR )malloc(outbuf->Length - i + 2);
				_tcscpy(outbuf->Data, &buff2[i]);
				outbuf->Length = outbuf->Length - i;
			}
			else
			{
				free(outbuf->Data);
				m_OutputBuffer.erase(m_OutputBuffer.begin());
				delete outbuf;
			}
		}
		else
			if (i == WSAEWOULDBLOCK)
				m_CanSend = FALSE;
	}

	SetState(StateConnected);
	RefreshTimer();
}

void CTelnetXCom::ClearOutBuffer(void)
{
	tagOutBuffer *outbuf;
	
	while (m_OutputBuffer.size() )
	{
		outbuf = (tagOutBuffer *)*m_OutputBuffer.begin();
		if (!outbuf)
			return; // this may never happen, or we're doomed. 
		free(outbuf->Data);
		m_OutputBuffer.erase(m_OutputBuffer.begin());
		delete outbuf;
	}
}

STDMETHODIMP CTelnetXCom::Connect(VARIANT Host, VARIANT Port)
{
	HRESULT	hr = S_OK;
	LPTSTR	p;
	CString	cstrHost;
	CString	cstrAddr;

	RuntimeCheck();


	if (m_Socket)
	{
		delete m_Socket;
		m_Socket=NULL;
	}
	
	if( m_State != StateNotConnected )
		return CtlError(20001);

	if (m_Protocol == RExec && !m_LogonCommand.Length())
		return CtlError(20005);

	if ((m_Protocol == SSH1 || m_Protocol == RSH || m_Protocol == RLogin || m_Protocol == RExec) && !m_AutoLogon)
		return CtlError(20006);

	if (m_Protocol != Telnet && m_FirewallType == FirewallTypeProxy)
		return CtlError(20009);

	ClearOutBuffer();

	if (Host.vt==VT_BSTR)


	{
		m_Host.Empty();
		m_Host = Host.bstrVal ;
	}
	else
		if (Host.vt== (VT_BSTR | VT_BYREF))
		{
			m_Host.Empty();
			m_Host = *Host.pbstrVal;
		}

	if (Port.vt==VT_I2)
		m_Port=Port.iVal;
	else
		{
			VARIANT var;
			VariantInit(&var);
			VariantChangeType(&var, &Port, 0, VT_I2);
			if (var.vt==VT_I2)
				m_Port=var.iVal;
		}

	CString tlib=m_LibraryName;
	if (!m_Socket)
	{
		m_Socket = new CSSHSocket(this,tlib.GetBuffer(tlib.GetLength()+1));
		m_Socket->m_Owner=this;
	}
	tlib.ReleaseBuffer();
		
	//
	// Open socket
	//
	if (m_Protocol==RLogin || m_Protocol == RExec || m_Protocol==RSH)
	{
		// for rlogin we need to grab ports below 1024
		int i=1023;
		BOOL success=FALSE;
		while (i>512 && !success)
		{
			success=m_Socket->Create(i);
			if (!success) i--;
		}

		if (!success)
			return CtlError(20008);
	}
	else
	{
		BOOL b;

		if( !m_Socket->Create() )
			return CtlError(m_Socket->GetLastError());

		b = TRUE;
		m_Socket->SetSockOpt( SO_DONTLINGER, &b, sizeof( BOOL ) );
	}

	m_Socket->m_AcceptTimeout=m_Socket->m_ConnectTimeout=m_Socket->m_ReceiveTimeout=m_Socket->m_SendTimeout=0;
	m_Socket->m_fBlocking=FALSE;
	m_Socket->AsyncSelect(FD_WRITE | FD_READ | FD_CONNECT | FD_CLOSE);


	if (!DoFireDebug(_T("Resolving Host to IP")))
		return CtlError(-1);
	//
	// Check host address and convert name to ip if required
	//
	CString m_strHost = m_Host;
	if (!m_FirewallType || m_FirewallType==FirewallTypeSocks4)
	{
		p = m_strHost.GetBuffer(m_strHost.GetLength() + 1);
		while( *p && ((*p >= _T('0') && *p <= _T('9')) || (*p == _T('.'))) )
			p++;
		if( *p )
		{
			struct hostent FAR * phost;
			phost = m_Socket->gethostbyname(m_strHost);
			if(phost != NULL )
			{
				cstrAddr = m_Socket->inet_ntoa((struct in_addr)*(struct in_addr *)phost->h_addr);
			} else {
				m_strHost.ReleaseBuffer();
				SetState( StateNotConnected );
				return CtlError(WSAHOST_NOT_FOUND);
			}
		} else {
			cstrAddr = m_strHost;
		}
		m_strHost.ReleaseBuffer();
	} else

		cstrAddr = m_strHost;
	
	// NEW for FIREWALL
	m_Socket->FWHost=m_FirewallHost;
	m_Socket->FWType=m_FirewallType;
	m_Socket->FWLogonName=m_FirewallLogonName;
	m_Socket->FWPassword=m_FirewallPassword;
	m_Socket->FWPort=m_FirewallPort;


//	if (!m_LogonName.Length())
//		return CtlError(20003);
	

	if (!DoFireDebug(_T("Initiating connection...")))
		return CtlError(-1);
	//
	// Connect
	//
	m_DidAutoLogon=0;

	m_Socket->m_Secured=m_Protocol;
	switch(m_Encryption)
	{
		case SingleDES:
			m_Socket->m_CipherType=2;
			break;

		default: // everything else
			m_Socket->m_CipherType=3; //tripleDES
	}
	m_Socket->m_Username=m_LogonName;
	m_Socket->m_Password=m_LogonPassword;
	m_Socket->m_TerminalType=m_TerminalName;
	m_Socket->debug=m_DebugMode?TRUE:FALSE;
	m_Socket->m_Command=m_LogonCommand;

	RefreshTimer();


	m_BlockingDone=FALSE;
	m_LastError=0;

	if( !m_Socket->Connect(cstrAddr.GetBuffer(0), m_Port) )
	{
		int err;
		
		//
		// WSAEWOULDBLOCK is returned for non-blocking connections,
		// any other errors are returned to the user.
		//
		if( (err = m_Socket->GetLastError()) != WSAEWOULDBLOCK )
		{
			SetState( StateNotConnected );
			return CtlError(err);
		}
	}

	SetState( StateConnecting );
	if ( m_Blocking && ( m_State == StateConnecting ))
	{
		Block();
		if (m_LastError)
			return CtlError(m_LastError);
	}

	return hr;
}

STDMETHODIMP CTelnetXCom::Disconnect()
{
	TelnetXStatesEnum st=m_State;


	RuntimeCheck();

	if (!DoFireDebug(_T("Manually disconnected.")))
		return CtlError(-1);
    CComBSTR libname=m_LibraryName;  //make copy to prevent deletion by put_LibraryName
	put_LibraryName(libname);
	if (SetState(StateNotConnected))
		if (!FireDisconnected())
			return CtlError(-1);

	if (m_Socket)
	{
		delete m_Socket;
		m_Socket=NULL;
	}
	return S_OK;
}

BOOL CTelnetXCom::SetState(TelnetXStatesEnum State)
{

	if (! m_CanSetState)
		return TRUE;

	if (State == m_State)
		return TRUE;

	TelnetXStatesEnum st=m_State;

	m_State=State;

	return FireStateChanged(m_State, st);
}

// socket stuff
void CTelnetXCom::OnAccept(CSocket* pSocket, int nErrorCode)
{
	
}
void CTelnetXCom::OnClose(CSocket* pSocket, int nErrorCode)
{
	if (nErrorCode)
		m_LastError=nErrorCode;

	m_BlockingDone=TRUE;

	m_CanSend=FALSE;
	if (!DoFireDebug(_T("OnClose called.")))
		return;

	if (!FireDisconnected())
		return;

	SetState(StateNotConnected);
	if (m_Socket)
	{
		delete m_Socket;
		m_Socket=NULL;
	}
}

void CTelnetXCom::OnConnect(CSocket* pSocket, int nErrorCode)
{
	if (pSocket!=m_Socket)
		return;

	RefreshTimer();

	if (nErrorCode)
		m_LastError=nErrorCode;

	if (!DoFireDebug(_T("OnConnected called.")))
		return;

	if (nErrorCode)
	{
		if (!SetState(StateNotConnected))
			return;
		
		if (m_Socket)
			::KillTimer(m_Socket->m_hwndSocket, 101);
		
		_TCHAR buff[1024];
		wsprintf(buff,_T("ERROR %d: %s"),nErrorCode, GetErrorString(nErrorCode));

		if (!DoFireDebug(buff))
			return;
        if (!FireDisconnected())
            return;

	}
	else
	{
		m_FirstOnSend=TRUE;

		if (m_Protocol == RLogin)
			m_DidAutoLogon=1; // for Rlogin auto send username
		else
			if (m_Protocol!=Telnet)
				m_DidAutoLogon=2; // for RSH & SSH logon during negotiation

		if (!SetState(StateConnected))
			return;
		
		if (!FireConnected())
			return;
	}
}

void CTelnetXCom::OnReceive(CSocket* pSocket, int nErrorCode)
{
	unsigned int i;

	if (!DoFireDebug(_T("OnReceive called.")))
		return;

	if (nErrorCode)
		m_LastError=nErrorCode;

	RefreshTimer();

	if (m_InputBuffLen)
    {
        m_pLastSocket = pSocket;
        m_LastError = nErrorCode;
		m_NeedReadMore=TRUE;
        return;
    }
	else
	{
		SetState(StateReceivingData);
		i=pSocket->Receive( m_InputBuff, sizeof( m_InputBuff ) - 1, 0 );
		if (i>0)
		{
			m_InBuff=m_InputBuff;
			m_InputBuffLen=i;
		}
	}
	if (i<0) i=0;

	m_InBuff[m_InputBuffLen]=0;

	if (m_FirstOnReceive && (m_Protocol == RExec || m_Protocol == RSH))
	{
		// we expect 0 to arrive
		if (!m_InputBuffLen || m_InputBuff[0])
			// something wrong has happened
		{
			m_Socket->Close();
			OnClose(m_Socket, 0);
			if (!DoFireDebug(_T("Server response contains invalid characters")))
				return;

			return;
		}
		// all ok, shuffle a bit
		memcpy(m_InputBuff, m_InputBuff+1, i-1);
		m_InputBuffLen--;
		m_InBuff[m_InputBuffLen]=0;
	}

	
	// if we're in binary mode, then there's nothing to implement
	i=0;
	BOOL Found=FALSE;
	if (!m_SendBinary)
	{

		while (i<m_InputBuffLen && !Found) // search for next occurance
		{
			if (m_Protocol == Telnet)
				if (m_InBuff[i]==255) 
					Found=TRUE;
				else
					i++;
			else
			if (m_Protocol == RLogin)
				if (m_InBuff[i]==2 || m_InBuff[i]==16)
					Found=TRUE;
				else
					i++;
			else
				i=m_InputBuffLen;
		}
	}
	else
		i=m_InputBuffLen;
	if (m_SendBinary && m_Protocol == RLogin && !Found)
	{
		while (i<m_InputBuffLen && !Found)
		if (m_InBuff[i]==32)
			Found=TRUE;
		else
			i++;
	}else
		i=m_InputBuffLen;

	if (m_TerminalType == None) // for terminaltype==none write everything to screen
	{
		i=m_InputBuffLen;
		Found=FALSE;
	}

	// if specialcodes are to be customized, fire even and let user decide what to do with them
	if (Found) // yes, there are 
	{
		m_CanSetState=FALSE;
		if (m_TerminalType != None)
		{	
			DoSpecialCode();
			i = m_InputBuffLen; // DEM 
			FlushOutBuffer();
		}
		m_CanSetState=TRUE;
	}
	if (!m_InputBuffLen) 
		return;

	if (m_AutoLogon && i)
//	if (false)
	{
		if (m_DidAutoLogon==0)
		{
			if (_tcsstr((LPTSTR )m_InBuff,_T("ogin:")) || _tcsstr((LPTSTR )m_InBuff,_T("sername:")))
			{
				SetState(StateSendingLogonName);
				m_CanSetState=FALSE;
				// send logonname
				m_InputBuffLen=0;
				CString a;
				LPTSTR b;
				a=m_LogonName;
				b=a.GetBuffer(0);
				SendString(b);
				a.ReleaseBuffer();
				SendString(_T("\r\n"));
				m_DidAutoLogon=1;
				i=0;
				m_CanSetState=TRUE;
			}
		}
		else
		if (m_DidAutoLogon==1)
		{
			if (_tcsstr((LPTSTR )m_InBuff,_T("assword:")))
			{
				SetState(StateSendingPassword);
				m_CanSetState=FALSE;
				// send password
				m_InputBuffLen=0;
				CString a;
				LPTSTR b;
				a=m_LogonPassword;
				b=a.GetBuffer(0);
				SendString(b);
				a.ReleaseBuffer();
				SendString(_T("\r\n"));
				m_DidAutoLogon=2;
				i=0;
				m_CanSetState=TRUE;
			}
		}
		else // actually, this is for LogonCommand
		if (m_DidAutoLogon==2)
		{
			if (m_AutoLogon && m_LogonCommand.Length() && m_Protocol==Telnet)
			{
				//SetState(StateSendingPassword);
				m_CanSetState=FALSE;
				// send password
				m_InputBuffLen=0;
				CString a;
				LPTSTR b;
				a=m_LogonCommand;
				b=a.GetBuffer(0);
				SendString(b);
				a.ReleaseBuffer();
				SendString(_T("\r\n"));
				SendString(_T("logout\r\n\r\n"));
				i=0;
				m_CanSetState=TRUE;
			}
			m_DidAutoLogon=3;
		}
/*		else
		if (m_DidAutoLogon==3)
		{
				//SetState(StateSendingPassword);
				m_CanSetState=FALSE;
				m_DidAutoLogon=3;
				// send password
				m_InputBuffLen=0;
				CString a;
				LPTSTR b;
				a="logout";
				b=a.GetBuffer(0);
				SendString(b);
				a.ReleaseBuffer();
				SendString("\r\n");
				FlushOutBuffer();
				m_DidAutoLogon=4;
				i=0;
				m_CanSetState=TRUE;
				// logoff
				::PostMessage(m_hwndSocket, WM_SOCKET_NOTIFY, 0,FD_CLOSE);
				return;
		}
*/	}

	m_CanSetState=TRUE;

	if (m_Prompt.Length() && i)
	{
		CString a;
		LPTSTR b, c;
		a=m_Prompt;
		b=a.GetBuffer(0);
		c=strstr((LPTSTR )m_InBuff,b);
		if (c)
		{
			SetState(StateReceivedPrompt);
			m_InputBuffLen=c-(LPTSTR )m_InBuff;
			if (m_InputBuffLen<0)m_InputBuffLen=0;
			// ok, we have prompt.. should we maybe execute something?
			
			if (!FirePrompt())
				return;

			i=m_InputBuffLen;
			//recalculate total bytes in buffer
			Found=FALSE;
			if (!m_SendBinary)
			{

				while (i<m_InputBuffLen && !Found) // search for next occurance
					if (m_InBuff[i]==255) 
						Found=TRUE;
					else
						i++;

			}
			else
				i=m_InputBuffLen;
		}
		a.ReleaseBuffer();
	}

	if (i)
	{
		m_BlockingDone=TRUE;
		if (!FireReceived(i))

			return;
	}

}

void CTelnetXCom::OnTimer(CSocket *pSocket)
{
	m_LastError=WSAETIMEDOUT;
	if (pSocket)
	{
		if (m_Timeout)
		{
			if (!DoFireDebug(_T("Timeout occurred")))
				return;
			//m_Timeouted=TRUE;
 			::PostMessage(pSocket->m_hwndSocket,WM_SOCKET_NOTIFY,0,FD_CLOSE);
			::KillTimer(pSocket->m_hwndSocket,101);
		}
		else
			RefreshTimer();
	}
}

void CTelnetXCom::OnDebug(CSocket *pSocket, LPTSTR msg)
{
	DoFireDebug(msg);
}

void CTelnetXCom::OnSend(CSocket* pSocket, int nErrorCode)
{
	int length, mylen;
	if (!DoFireDebug(_T("OnSend called.")))
		return;

	if (nErrorCode)
		m_LastError=nErrorCode;

	m_CanSend=TRUE;
	if (m_FirstOnSend && m_Protocol==RLogin)
	{
		m_FirstOnSend=FALSE;
		_TCHAR buff[1024];
		CString a=m_LogonName;
		LPTSTR b=a.GetBuffer(0);
		CString c=m_TerminalName;
		wsprintf(buff,_T("#%s#%s#%s/38400#"), b, b, c.GetBuffer(0));
		c.ReleaseBuffer();a.ReleaseBuffer();
		int i=strlen(buff);
		for (int j=0;j<i;j++)
			if (buff[j]==_T('#')) buff[j]=0;

		length=0;
		mylen=i;
		ImplementCustomCodes((_TUCHAR *)buff, &length, (_TUCHAR *)buff, &mylen); 
		
		AddString(buff, mylen);
		m_FirstOnReceive=TRUE;
	}
	if (m_FirstOnSend && m_Protocol==RSH)
	{
		m_FirstOnSend=FALSE;
		_TCHAR buff[1024];
		CString a=m_LogonName;
		CString c=m_LogonCommand;
		LPTSTR a1=a.GetBuffer(0);
		LPTSTR c1=c.GetBuffer(0);
		wsprintf(buff,_T("0#%s#%s#%s#"),a1, a1, c1);
		a.ReleaseBuffer();c.ReleaseBuffer();
		int i=strlen(buff);
		for (int j=0;j<i;j++)
			if (buff[j]==_T('#')) buff[j]=0;

		length=0;
		mylen=i;
		m_DidAutoLogon=1;
		ImplementCustomCodes((_TUCHAR *)buff, &length, (_TUCHAR *)buff, &mylen); 
		
		AddString(buff, mylen);
		m_FirstOnReceive=TRUE;
	}
	if (m_FirstOnSend && m_Protocol==RExec)
	{
		m_FirstOnSend=FALSE;
		_TCHAR buff[1024];
		CString a=m_LogonName;
		CString b=m_LogonPassword;
		CString c=m_LogonCommand;
		LPTSTR a1=a.GetBuffer(0);
		LPTSTR b1=b.GetBuffer(0);
		LPTSTR c1=c.GetBuffer(0);
		wsprintf(buff,_T("0#%s#%s#%s#"), a1, b1, c1);
		a.ReleaseBuffer();b.ReleaseBuffer();c.ReleaseBuffer();
		int i=strlen(buff);
		for (int j=0;j<i;j++)
			if (buff[j]==_T('#')) buff[j]=0;

		length=0;
		mylen=i;
		ImplementCustomCodes((_TUCHAR *)buff, &length, (_TUCHAR *)buff, &mylen); 
		
		AddString(buff, mylen);
		m_FirstOnReceive=TRUE;
	}

	FlushOutBuffer();
}

void CTelnetXCom::ImplementCustomCodes(_TUCHAR *inbuff, int *inlen, _TUCHAR *outbuf, int *outlen)
{
	BSTR ReceivedData, ResponseData;

	if (m_TerminalType == Custom)
	{
		ReceivedData=GetBstrFromStr((LPTSTR )inbuff,*inlen);
		ResponseData=GetBstrFromStr((LPTSTR )outbuf,*outlen);
		if (!FireSpecialCode(ReceivedData, &ResponseData))
			return;
		*outlen=GetStrFromBstr(ResponseData, (LPTSTR )outbuf);
	}

}

BSTR CTelnetXCom::GetBstrFromStr(LPTSTR str, int len)
{
	BSTR bstr;
	int i;
	
	i=::MultiByteToWideChar(CP_ACP, 0, str, len, NULL, 0);
	bstr = SysAllocStringLen(NULL,i);
	::MultiByteToWideChar(CP_ACP, 0, str, len, bstr, i);
	
	return bstr;
}

int CTelnetXCom::GetStrFromBstr(BSTR bstr, LPTSTR str)
{
	int i;
	i=::SysStringLen(bstr);
	if (str)
		i=::WideCharToMultiByte(CP_ACP,0,bstr,i,str,16384,0,0);

	return i;
}
void CTelnetXCom::DoSpecialCode(void)
{
	unsigned int i,j;
	int mylen, length;
	int iOutLength;            // DEM The final output length
	int iCurrentchar;
	_TUCHAR *a, *b;
	_TUCHAR *pOutPointer; // DEM Pointer to where we are in the output buffer
	_TUCHAR buff[1024];
	BOOL FirstSpecial=TRUE;
	
	a=m_InputBuff;
	pOutPointer = m_InputBuff;
	iOutLength = 0;
	iCurrentchar = 0;
	while (a<(m_InputBuff+m_InputBuffLen))
	{
		if (m_Protocol==Telnet)
		{
/*			if (FirstSpecial)
			{
				i=0;
				while (i<m_InputBuffLen && *(a)!=0xff)
				{
					i++;
					a++;  // DEM Advancing input buffer pointer
				}
				
			}
			else
			{
				if (*a!=0xff)
				{
					m_InputBuffLen=(a-m_InputBuff);
					memcpy(m_InputBuff, a, m_InputBuffLen);
					return;
				}
			}
			FirstSpecial=FALSE;
*/
			// DEM START Rewrite
			while ((unsigned int)(a - m_InputBuff) < m_InputBuffLen && *(a)!=0xff)
			{ 
				*pOutPointer = *a;  // Copy the character to the new buffer
				iOutLength ++;
				iCurrentchar ++;
				pOutPointer ++;
				a++;  
			}
			if ((unsigned int)(a - m_InputBuff) < m_InputBuffLen)
			{
				// we found something
				a++; // ignore 0xff
				switch (*a)

				{
					case 241: // maybe next time
					case 242:
					case 243:
					case 244:
					case 245:
					case 246:
					case 247:
					case 248:
					case 249:
						a++;
						break;

					case 251: // WILL
					case 252: // WONT
					case 253: // DO
					case 254: // DONT
						i=1;  // DEM ??
						j=(*a)-251;
						ProcessTelnetReceivedOption((*a)-251, a-1, 3);
						a+=2;
						break;

					case 250: //SB
						// copy request to new buffer
						b=a+1;
						i=0;
						b++;
						while (b[i]!=240 && i<m_InputBuffLen) 
							i++;

						// now examine what do we have
						ProcessTelnetReceivedOption(4, a-1, i+4);
						a=b+i+1; // set for next
						break;

					default:
						// ignore others
						break;
				}

			}
			else
				m_InputBuffLen = iOutLength;

		}
		if (m_Protocol==RLogin)
		{
			if (m_FirstOnReceive)
			{
			
				// we expect 0 to arrive
				if (i>1 || m_InputBuff[0])
					// something wrong has happened
				{
					m_Socket->Close();
					OnClose(m_Socket, 0);
					if (!DoFireDebug(_T("Server response contains invalid characters")))
						return;

					return;
				}
				unsigned short *buf2=(unsigned short *)buff;
				buf2[0]=0xffff;buf2[1]=0x7373;
				buf2[2]=buff[4]=25;buf2[3]=buff[5]=80;
				mylen=12;length=0;
				ImplementCustomCodes(a, &length, buff, &mylen); 
				AddString((LPTSTR )buff, 12);
				m_FirstOnReceive=FALSE;
				m_InputBuffLen=0;
				FlushOutBuffer();
				return;
			}
		}
	}
	// DEM START
	if (m_Protocol==Telnet)
	{
		m_InputBuffLen = iOutLength;
	}
	if (m_Protocol == RLogin)
	{
		// DEM END
		i=(a-m_InputBuff);
		if (i<m_InputBuffLen) // there's data left

		{
			m_InputBuffLen=i;
			memcpy(m_InputBuff,a,i);
		}
		else
			m_InputBuffLen=0;
	}

}

void CTelnetXCom::ProcessTelnetReceivedOption(int option, _TUCHAR *buffer, int length)
{
	_TUCHAR resbuf[1024];
	int mylen=3;
	CString str;

	switch (*(buffer+2))
	{
		case 24: // terminal type
			if (option==oDO)
			{

				// reply with WILL 
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfb\x18"));
				mylen=3;

				ImplementCustomCodes(buffer, &length, resbuf, &mylen); // if terminaltype=custom

				AddString((LPTSTR )resbuf,3);

			}
			else
			if (option==oWILL)
			{
				// reply with DO
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfd\x18"));
				mylen=3;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen); 
				AddString((LPTSTR )resbuf,3);
			}
			else
			if (option==oSB) // send
			{
				// send our info
				str=m_TerminalName;
				memcpy(resbuf,_T("\xff\xfa\x18\0"),4);
				memcpy(resbuf+4,str.GetBuffer(0),str.GetLength());
				memcpy(resbuf+4+str.GetLength(),_T("\xff\xf0"),2);
				str.ReleaseBuffer();
				mylen=str.GetLength()+6;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen); 
				AddString((LPTSTR )resbuf, mylen);
			}
			else
			{
				// we ignore this, just ask user if he wants something
				mylen=0;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen);
				if (mylen)
					AddString((LPTSTR )resbuf, mylen);

			}
			break;

		case 32: // terminal speed
			if (option==oDO)
			{
				// reply with WILL 
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfb\x20"));
				mylen=3;

				ImplementCustomCodes(buffer, &length, resbuf, &mylen); // if terminalspeed=custom

				AddString((LPTSTR )resbuf,3);
			}else
			if (option==oWILL)
			{
				// reply with DO
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfd\x20"));
				mylen=3;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen); 
				AddString((LPTSTR )resbuf,3);
			}else
			if (option==oSB) // send
			{
				// server sent us his terminal speed, reply with ours
				wsprintf((LPTSTR )resbuf,_T("\xff\xfa\x20\0"));
				wsprintf((LPTSTR )resbuf+4,_T("38400,38400\xff\xf0"));
				str.ReleaseBuffer();
				mylen=17;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen); 
				AddString((LPTSTR )resbuf, mylen);
			}else
			{
				// we ignore this, just ask user if he wants something - this means NO!
				mylen=0;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen);
				if (mylen)
					AddString((LPTSTR )resbuf, mylen);

			}


			break;
		case 35: // x-display-location
			if (option==oDO)
			{
				// reply with WONT
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfc\x23"));
				mylen=3;

				ImplementCustomCodes(buffer, &length, resbuf, &mylen); // if terminaltype=custom

				AddString((LPTSTR )resbuf,3);
			}else
			if (option==oWILL)
			{
				// reply with DONT
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfe\x23"));
				mylen=3;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen); // if terminaltype=custom
				AddString((LPTSTR )resbuf,3);
			}else
			{
				// we ignore this, just ask user if he wants something
				mylen=0;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen);
				if (mylen)
					AddString((LPTSTR )resbuf, mylen);

			}
			break;
		case 39: // echo
			if (option==oDO)
			{
				// reply with WILL  // DEM ?? This is won't
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfc\x27"));
				mylen=3;

				ImplementCustomCodes(buffer, &length, resbuf, &mylen); // if terminaltype=custom

				AddString((LPTSTR )resbuf,3);
			}else
			if (option==oWILL)
			{
				// reply with DO
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfd\x27"));
				mylen=3;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen); // if terminaltype=custom
				AddString((LPTSTR )resbuf,3);
			}else
			{
				// we ignore this, just ask user if he wants something
				mylen=0;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen);
				if (mylen)
					AddString((LPTSTR )resbuf, mylen);

			}
			break;
			// DEM START
		case 3: // Go ahead
			if (option==oDO)
			{
				// reply with WILL
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfb\x3"));
				mylen=3;

				ImplementCustomCodes(buffer, &length, resbuf, &mylen); // if terminaltype=custom

				AddString((LPTSTR )resbuf,3);
			}else
			if (option==oWILL)
			{
				// reply with DO
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfd\x3"));
				mylen=3;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen); // if terminaltype=custom
				AddString((LPTSTR )resbuf,3);
			}else
			{
				// we ignore this, just ask user if he wants something
				mylen=0;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen);
				if (mylen)
					AddString((LPTSTR )resbuf, mylen);

			}
			break;
		// DEM END

		case 1: // TRANSMIT_BINARY
			if (option==oDO)
			{
				// reply with WILL
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfc\x01"));
				mylen=3;

				ImplementCustomCodes(buffer, &length, resbuf, &mylen); // if terminaltype=custom

				AddString((LPTSTR )resbuf,3);
			}else
			if (option==oWILL)
			{
				// reply with DO
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfe\x01"));
				mylen=3;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen); 
				AddString((LPTSTR )resbuf,3);
			}else
			{
				// we ignore this, just ask user if he wants something
				mylen=0;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen);
				if (mylen)
					AddString((LPTSTR )resbuf, mylen);

			}
			break;

		case 31: // window size
			if (option==oDO)
			{
				// reply with WONT
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfc\x1f"));
				mylen=3;

				ImplementCustomCodes(buffer, &length, resbuf, &mylen); 

				AddString((LPTSTR )resbuf,3);
			}else
			if (option==oWILL)
			{
				// reply with DONT
				_tcscpy((LPTSTR )resbuf,_T("\xff\xfe\x1f"));
				mylen=3;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen); 
				AddString((LPTSTR )resbuf,3);
			}else
			{
				// we ignore this, just ask user if he wants something
				mylen=0;
				ImplementCustomCodes(buffer, &length, resbuf, &mylen);
				if (mylen)
					AddString((LPTSTR )resbuf, mylen);

			}
			break;


		default:
			// we're not interested in other
			if (option==oDO || option==oDONT)
				wsprintf((LPTSTR )resbuf,_T("\xff\xfc%c"), *(buffer+2));
			else
				wsprintf((LPTSTR )resbuf,_T("\xff\xfe%c"), *(buffer+2));

			mylen=3;
			ImplementCustomCodes(buffer, &length, resbuf, &mylen); 

			AddString((LPTSTR )resbuf, mylen);
			break;
	}


}





STDMETHODIMP CTelnetXCom::get_AutoLogon(VARIANT_BOOL *pVal)
{
	*pVal=m_AutoLogon?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_AutoLogon(VARIANT_BOOL newVal)
{
	RuntimeCheck();

	m_AutoLogon=newVal?TRUE:FALSE;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_LogonName(BSTR *pVal)
{
	*pVal=m_LogonName.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_LogonName(BSTR newVal)
{
	RuntimeCheck();

	m_LogonName.Empty();
	m_LogonName=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_LogonPassword(BSTR *pVal)
{
	*pVal=m_LogonPassword.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_LogonPassword(BSTR newVal)
{
	RuntimeCheck();

	m_LogonPassword.Empty();
	m_LogonPassword = newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_PromptString(BSTR *pVal)
{
	*pVal=m_Prompt.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_PromptString(BSTR newVal)
{
	RuntimeCheck();

	m_Prompt.Empty();
	m_Prompt = newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::About()
{
	MabryLicenseAbout();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_NotificationObject(ITelnetXComNotify **pVal)
{
	*pVal=m_NotificationObject;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_NotificationObject(ITelnetXComNotify *newVal)
{
	RuntimeCheck();

	m_NotificationObject=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_Protocol(ProtocolsEnum *pVal)
{
	*pVal=m_Protocol;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_Protocol(ProtocolsEnum newVal)
{
	RuntimeCheck();

	if (m_State != StateNotConnected)
		return CtlError(20001);
	m_Protocol=newVal;

	if (m_Protocol!=Telnet && !m_AutoLogon) // AutoLogon is required for SSH & RSH
			put_AutoLogon(TRUE);
	
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_Encryption(EncryptionsEnum *pVal)
{
	*pVal=m_Encryption;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_Encryption(EncryptionsEnum newVal)
{
	RuntimeCheck();

	if (m_State != StateNotConnected)
		return CtlError(20001);

	m_Encryption=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_TerminalName(BSTR *pVal)
{
	*pVal=m_TerminalName.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_TerminalName(BSTR newVal)
{
	RuntimeCheck();

	m_TerminalName.Empty();
	m_TerminalName=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_TerminalType(TelnetXTypesEnum *pVal)
{
	*pVal=m_TerminalType;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_TerminalType(TelnetXTypesEnum newVal)
{
	RuntimeCheck();

	m_TerminalType=newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_LicenseKey(BSTR *pVal)
{
	*pVal = m_LicenseKey.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_LicenseKey(BSTR newVal)
{
	m_LicenseKey.Empty();
	m_LicenseKey = newVal;
	return S_OK;
}

STDMETHODIMP CTelnetXCom::get_LogonCommand(BSTR *pVal)
{
	*pVal=m_LogonCommand.Copy();
	return S_OK;
}

STDMETHODIMP CTelnetXCom::put_LogonCommand(BSTR newVal)
{
	RuntimeCheck();

	m_LogonCommand.Empty();
	m_LogonCommand=newVal;
	return S_OK;
}

void CTelnetXCom::RefreshTimer(void)
{
	if (m_Socket)
	{
	 	::KillTimer(m_Socket->m_hwndSocket,101);
		::SetTimer(m_Socket->m_hwndSocket, 101, m_Timeout*1000, NULL);
	}
}

int CTelnetXCom::Block(void)
{

	while (!m_BlockingDone)
	{
		MSG msg;
		WaitMessage();
		while (::PeekMessage(&msg, NULL, WM_SOCKET_NOTIFY, WM_SOCKET_SSL_NOTIFY, PM_REMOVE))
		{
			// dispatch
			TranslateMessage(&msg);
			DispatchMessage(&msg);	
		} 
		
		if (::PeekMessage(&msg, NULL, WM_TIMER, WM_TIMER, PM_REMOVE))
		{
			if (msg.wParam == 101)// && msg.hwnd == m_Socket->m_hwndSocket) 
				m_BlockingDone = TRUE;				

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (m_BlockingMode == TrueBlocking)
		{
			// Service all paint messages
			//
			if (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			//
			// And any socket directed timer messages
		} else 	
		{
			//
			// Pseudo blocking, take everything
			//
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
		return 0;
}

BOOL CTelnetXCom::CheckServerKey(LPTSTR strkey, int length, int *retval)
{
	BSTR bstrkey=GetBstrFromStr(strkey, length);
	
	VARIANT_BOOL fCancel=VARIANT_FALSE;
	if (!FireSSHCheckServerKey(bstrkey, length, &fCancel))
		return FALSE;
	::SysFreeString(bstrkey);

	if (fCancel!=VARIANT_FALSE)
		return FALSE;

	return TRUE;
}