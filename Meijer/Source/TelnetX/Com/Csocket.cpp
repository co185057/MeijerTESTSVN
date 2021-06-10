//
// Copyright (c) 1997-1998 Mabry Software, Inc.
//

#include "stdafx.h"

#define ASSERT(e, m) _ASSERT(e)


#include "com.h"
#include "cstr.h"
#include "global.h"
#include "Cmsocket.h"
#include "csocket.h"
#include "telnetxcom.h"
#include <stdio.h>




CSocket::CSocket( CTelnetXCom* pCtrl, LPTSTR libname)
: CMSocket(libname, &pWsaData)
{
	m_pCtrl = pCtrl;
	FWHost=_T("");
	FWType=FirewallTypeNone;
	FWLogonName=_T("");
	FWPassword=_T("");
	FWPort=0;
	State=FWStateNotConnected;
}

CSocket::~CSocket( void )
{
}

void CSocket::OnTimer(void)
{
	m_pCtrl->OnTimer(this);
}

void CSocket::OnAccept(int nErrorCode)
{
/*
	switch (FWType)
	{
		case FirewallTypeNone:
			break;

		case FirewallTypeSocks4:
			// do something
			break;

		case FirewallTypeSocks5:
			// do something
			break;

//		case FirewallTypeProxyUSER:
//			// do something
//			break;

//		case FirewallTypeProxySITE:
//			// do something
//			break;

//		case FirewallTypeProxyPROXY:
//			// do something
//			break;

//		case FirewallTypeProxyOPEN:
//			// do something
//			break;

//		case FirewallTypeProxyAuthUSER:
//			// do something
//			break;

		case FirewallTypePipe:
			// do something
			break;

		default:
			return;
	}
*/	
	m_pCtrl->OnAccept(this, nErrorCode);
}

void CSocket::OnClose(int nErrorCode)
{
	switch (FWType)
	{
		case FirewallTypeNone:
			break;

		case FirewallTypeSocks4:
		case FirewallTypeSocks5:
		case FirewallTypeSSLProxy:
		case FirewallTypePipe:
		case FirewallTypeProxy:
			if (State!=FWStateConnected)
			{
				m_pCtrl->OnConnect(this, WSAEADDRNOTAVAIL);
				return;
			}
			break;

		default:
			return;
	}
	
	m_pCtrl->OnClose(this, nErrorCode);

}
void CSocket::OnConnect(int nErrorCode)
{
	switch (FWType)
	{
		case FirewallTypeNone:
			break;

		case FirewallTypeSocks4:
		case FirewallTypeSocks5:
		case FirewallTypeSSLProxy:
		case FirewallTypePipe:
		case FirewallTypeProxy:
			if (!nErrorCode) return;
			break;

		default:
			return;
	}
	
	m_pCtrl->OnConnect(this, nErrorCode);

}
void CSocket::OnOutOfBandData(int nErrorCode)
{
/*	switch (FWType)
	{
		case FirewallTypeNone:
			break;

		case FirewallTypeSocks4:
			// do something
			break;

		case FirewallTypeSocks5:
			// do something
			break;

//		case FirewallTypeProxyUSER:
//			// do something
//			break;

//		case FirewallTypeProxySITE:
//			// do something
//			break;

//		case FirewallTypeProxyPROXY:
//			// do something
//			break;

//		case FirewallTypeProxyOPEN:
//			// do something
//			break;

//		case FirewallTypeProxyAuthUSER:
//			// do something
//			break;

		case FirewallTypePipe:
			// do something
			break;

		default:
			return;
	}
*/	
	m_pCtrl->OnOutOfBandData(this, nErrorCode);
}
void CSocket::OnReceive(int nErrorCode)
{
	int i,j;

	switch (FWType)
	{
		case FirewallTypeNone:
			break;

		case FirewallTypeSocks4:
			if (State==FWStateConnecting)
			{
				i=CSocket::Receive(Socks4Data,8,0);
				if (i==SOCKET_ERROR || Socks4Data[1]!=90)
				{
					WSASetLastError(WSAECONNREFUSED);
					Close();
					m_pCtrl->OnConnect(this,WSAECONNREFUSED);
					return;
				}
				State=FWStateConnected;
				CMSocket::AsyncSelect(OrgAsyncEvent);

				if (OrgAsyncEvent & FD_CONNECT) 
					m_pCtrl->OnConnect(this,nErrorCode);

				if (OrgAsyncEvent & FD_WRITE) 
					m_pCtrl->OnSend(this,nErrorCode);
				return;
			}
			// do something
			break;

		case FirewallTypeSocks5:
			if (State==FWStateConnecting)
			{
				i=CSocket::Receive(Socks4Data,2,0);
				if (i==SOCKET_ERROR || Socks4Data[1]==255)
				{
					WSASetLastError(WSAECONNREFUSED);
					Close();
					m_pCtrl->OnConnect(this,WSAECONNREFUSED);
					return;
				}
				if (Socks4Data[1]==0)
				{
				Socks4Data[0]=5;
				Socks4Data[1]=1;
				Socks4Data[2]=0;
				Socks4Data[3]=3;
				j=4;
				if (m_OriginalHost.GetLength())
				{
					Socks4Data[4]=(_TCHAR)m_OriginalHost.GetLength();
					memcpy(&Socks4Data[5],(LPTSTR)(LPCTSTR)m_OriginalHost,m_OriginalHost.GetLength());
					j=m_OriginalHost.GetLength()+1;
//					Socks4Data[4+j++]=0;
				} 
					else j=m_OriginalHost.GetLength();
				i=ntohs(m_Port);
				memcpy(&Socks4Data[4+j],&i,2);
				CSocket::Send(Socks4Data,6+j,0);
				State=FWStateSendingRequest;
				return;
				}

				if (Socks4Data[1]==2)
				{
					// username/password auth.
					Socks4Data[0]=1;
					i=FWLogonName.GetLength();
					Socks4Data[1]=(_TUCHAR)i;
					_tcscpy((LPTSTR)&Socks4Data[2],(LPTSTR)(LPCTSTR)FWLogonName);
					j=FWPassword.GetLength();
					Socks4Data[i+2]=(_TUCHAR) j;
					_tcscpy((LPTSTR)&Socks4Data[3+i],(LPTSTR)(LPCTSTR)FWPassword);
					CSocket::Send(Socks4Data,i+j+3,0);
					State=FWStateSendingAuth;
					return;					
				}
			}
			if (State==FWStateSendingAuth)
			{
				i=CSocket::Receive(Socks4Data,2,0);
				if (i==SOCKET_ERROR || Socks4Data[1])
				{
					WSASetLastError(WSAECONNREFUSED);
					Close();
					m_pCtrl->OnConnect(this,WSAECONNREFUSED);
					return;
				}

				Socks4Data[0]=5;
				Socks4Data[1]=1;
				Socks4Data[2]=0;
				Socks4Data[3]=3;
				j=4;
				if (m_OriginalHost.GetLength())
				{
					Socks4Data[4]=(_TCHAR)m_OriginalHost.GetLength();
					memcpy(&Socks4Data[5],(LPTSTR)(LPCTSTR)m_OriginalHost,m_OriginalHost.GetLength());
					j=m_OriginalHost.GetLength()+1;
//					Socks4Data[4+j++]=0;
				} 
					else j=m_OriginalHost.GetLength();
				i=ntohs(m_Port);
				memcpy(&Socks4Data[4+j],&i,2);
				CSocket::Send(Socks4Data,6+j,0);
				State=FWStateSendingRequest;
				return;
			}

			if (State==FWStateSendingRequest)
			{	// we connected successfully

//				IOCtl(FIONREAD,&param);
//				if (param) 
//				{
//					if (param>100) param=100;
				CSocket::Receive(Socks4Data,5,0);
					if (!Socks4Data[1])
					{
						switch(Socks4Data[3])
						{
							case 1:
								Receive(Socks4Data,5,0);
								break;

							case 3:
								Receive(Socks4Data,Socks4Data[4]+2,0);
								break;

							case 4:
								Receive(Socks4Data,17,0);
								break;
						}
						State=FWStateConnected;
						CMSocket::AsyncSelect(OrgAsyncEvent);
						if (OrgAsyncEvent & FD_CONNECT) 
							m_pCtrl->OnConnect(this,nErrorCode);
						if (OrgAsyncEvent & FD_WRITE) 
							m_pCtrl->OnSend(this,nErrorCode);
						return;
					}
//				}
				m_pCtrl->OnConnect(this,WSAECONNREFUSED);
				return;
			}
			break;
/*
		case FirewallTypeProxy:
			if (State==FWStateConnecting)
			{
				Receive(Socks4Data,1024,0);
				if (m_Port!=23)
					wsprintf((LPTSTR)Socks4Data,"%s %u\r\n",m_OriginalHost.GetBuffer(0), m_Port);
				else
					wsprintf((LPTSTR)Socks4Data,"%s\r\n",m_OriginalHost.GetBuffer(0));
				m_OriginalHost.ReleaseBuffer();
				Send((LPTSTR)Socks4Data,_tcsclen((LPTSTR)Socks4Data),0);
				State=FWStateSendingRequest;
				return;
			} 
			else
			if (State==FWStateSendingRequest)
			{
						State=FWStateConnected;
						AsyncSelect(OrgAsyncEvent);
						if (OrgAsyncEvent & FD_CONNECT) m_pCtrl->OnConnect(this,nErrorCode);
						if (OrgAsyncEvent & FD_WRITE) m_pCtrl->OnSend(this,nErrorCode);
			}
			else
			if (State!=FWStateConnected) return;
			break;
*/
		case FirewallTypeProxy:
			if (State==FWStateConnecting)
			{
/*				_TCHAR buff[1024];
				i=CSocket::Receive(buff,1024,0);

				if (i==SOCKET_ERROR || buff[9]!='2' || buff[10]!='0' || buff[11]!='0')
				{
					WSASetLastError(WSAECONNREFUSED);
					CSocket::Close();
					m_pCtrl->OnConnect(this,WSAECONNREFUSED);
					return;
				}
*/				State=FWStateConnected;
				CMSocket::AsyncSelect(OrgAsyncEvent);
				if (OrgAsyncEvent & FD_CONNECT) 
					m_pCtrl->OnConnect(this,nErrorCode);

				if (OrgAsyncEvent & FD_WRITE) 
					m_pCtrl->OnSend(this,nErrorCode);

				if (OrgAsyncEvent & FD_READ) 
					m_pCtrl->OnSend(this,nErrorCode);
				return;
			}
			break;

		case FirewallTypeSSLProxy:
			if (State==FWStateConnecting)
			{
				_TCHAR buff[1024];
				CMSocket::AsyncSelect(0);
				i=CSocket::Receive(buff,14,0);
				if (i==SOCKET_ERROR || buff[9]!=_T('2') || buff[10]!=_T('0') || buff[11]!=_T('0'))
				{
					WSASetLastError(WSAECONNREFUSED);
					CSocket::Close();
					m_pCtrl->OnConnect(this,WSAECONNREFUSED);
					return;
				}
				j=0;
				do
				{
					i=CSocket::Receive(buff,1,0);
					if (buff[0]==_T('\n')) 
						j++;
				} while (i>0 && j<2);
				State=FWStateConnected;
				CMSocket::AsyncSelect(OrgAsyncEvent);
				if (OrgAsyncEvent & FD_CONNECT) 
					m_pCtrl->OnConnect(this,nErrorCode);
				if (OrgAsyncEvent & FD_WRITE) 
					m_pCtrl->OnSend(this,nErrorCode);
				if (OrgAsyncEvent & FD_READ) 
					m_pCtrl->OnSend(this,nErrorCode);
				return;
			}
			break;

		case FirewallTypePipe:
			// do something
			break;

		default:
			return;
	}

	m_pCtrl->OnReceive(this, nErrorCode);
}
void CSocket::OnSend(int nErrorCode)
{
	int i;

	switch (FWType)
	{
		case FirewallTypeNone:
			break;

		case FirewallTypeSocks4:
			if (State==FWStateConnecting) 
			{
				i=ntohs(m_Port);
				memcpy(&Socks4Data[2],&i,2);
				memcpy(&Socks4Data[4],(void *)(LPCTSTR)m_Host,4);
				Socks4Data[0]=4;
				Socks4Data[1]=1;
				Socks4Data[8]=0;
				CSocket::Send(Socks4Data,9,0);
				return;
			}
			// do something
			break;

		case FirewallTypeSocks5:
			if (State==FWStateConnecting)
			{
				Socks4Data[0]=5;
				Socks4Data[1]=5;
				Socks4Data[2]=0;
				Socks4Data[3]=1;
				Socks4Data[4]=2;
				Socks4Data[5]=3;
				Socks4Data[6]=128;
				CSocket::Send(Socks4Data,7,0);
				return;
			}
			// do something
			break;


		case FirewallTypeProxy:
			if (State==FWStateConnecting)
			{
				CString req=m_OriginalHost;
				if (m_Port!=23)
				{
					req+=_T(" ");
					_TCHAR buff[80];
					wsprintf(buff,_T("%d"),m_Port);
					req+=buff;
				}
				req+=_T("\r\n");
				LPTSTR a=req.GetBuffer(req.GetLength()+1);
				CSocket::Send(a, req.GetLength(),0);
				req.ReleaseBuffer();
				return;
			}
			break;

		case FirewallTypeSSLProxy:
			if (State==FWStateConnecting)
			{

				_TCHAR buff[1024];


				CString req=_T("CONNECT ");
				req+=m_OriginalHost;
				req+=_T(":");
				wsprintf(buff,_T("%u"),m_Port);
				req+=buff;
				req+=_T(" HTTP/1.0\r\nHost: ");
				req+=m_OriginalHost;
				req+=_T("\r\n\r\n");
				LPTSTR a=req.GetBuffer(req.GetLength()+1);
				CSocket::Send(a, req.GetLength(),0);
				req.ReleaseBuffer();
				return;
			}
			break;


		case FirewallTypePipe:
			// do something
			break;

		default:
			return;
	}

	
	m_pCtrl->OnSend(this, nErrorCode);
}

BOOL CSocket::Create(UINT nSocketPort, int nSocketType, long lEvent, LPTSTR lpszSocketAddress)
{
	if (nSocketType!=SOCK_STREAM && FWType!=FirewallTypeNone) return FALSE;

	OrgAsyncEvent=lEvent;
	OrgSocketType=nSocketType;
	
	return CMSocket::Create(nSocketPort,nSocketType,lEvent,lpszSocketAddress);
}

BOOL CSocket::AsyncSelect(long lEvent)
{
	if (OrgSocketType!=SOCK_STREAM && FWType!=FirewallTypeNone) return FALSE;
	OrgAsyncEvent=lEvent;
	if (FWType>0  && State!=StateConnected) 
			lEvent|=FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE;
	return CMSocket::AsyncSelect(lEvent);
}

BOOL CSocket::TranslateAddress( LPTSTR lpszAddr, in_addr * inetAddr )
{
	LPTSTR p;
	struct hostent FAR *RemoteHost;

	p = lpszAddr;

	while( *p && ((*p >= _T('0') && *p <= _T('9')) || (*p == _T('.'))) ) 
		p++;

	if( *p == 0 )
		inetAddr->s_addr = inet_addr(lpszAddr);
	else
	{
		RemoteHost = gethostbyname( lpszAddr );
		if ( !RemoteHost ) 
			return FALSE; // host could not be resolved

		inetAddr->s_addr = *(DWORD*)(*RemoteHost->h_addr_list);
	} 

	return TRUE;
}

BOOL CSocket::Connect(LPTSTR lpszHostAddress, UINT nHostPort)
{
	struct in_addr inetAddr;

	if ( FWType == FirewallTypeNone )
			return CMSocket::Connect( lpszHostAddress, nHostPort );

	if (( OrgSocketType != SOCK_STREAM ) || ( FWHost.GetLength() == 0 ))
		return FALSE;

	if ( TranslateAddress( lpszHostAddress, &inetAddr ) == FALSE )
		return FALSE;

	memcpy( m_Host, (&inetAddr.S_un.S_addr), 4 );
	m_Port = nHostPort;
	m_OriginalHost = lpszHostAddress;

	State=FWStateConnecting;

	if ( m_fBlocking == FALSE )
		return CMSocket::Connect((LPTSTR )(LPCTSTR)FWHost, FWPort );
	else
	{
		if ( CMSocket::Connect((LPTSTR )(LPCTSTR)FWHost, FWPort ) == FALSE )
			return FALSE;

		while ( State == FWStateConnecting )
		{
			MSG msg;

			if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg); 
			}
		}

		return ( State == FWStateConnected );
	}
}


