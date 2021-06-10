//
// Copyright (c) 1997-2002 Mabry Software, Inc. All Rights Reserved.
//
#ifndef __CSOCKET_DEFINITION
#define __CSOCKET_DEFINITION

typedef enum 
{
	FirewallConnect=0,
	FirewallDisconnect=1,
	FirewallReceive=2,
	FirewallSend=3
} FirewallEvent;



class CTelnetXCom;

class CSocket : public CMSocket
{
	
public:
	CSocket( CTelnetXCom* pCtrl, LPTSTR libname);
	~CSocket( void );
	CTelnetXCom *m_pCtrl;
	WSADATA pWsaData;

	
	CString FWLogonName,FWPassword,FWHost;
	short	FWPort;
	FirewallTypeEnum FWType;

public:
	typedef enum
	{	
		FWStateNotConnected=0,
		FWStateConnecting,
		FWStateSendingAuth,
		FWStateSendingRequest,
		FWStateConnected
	} FWStates;
	
	FWStates		State;
	long			OrgAsyncEvent;
	int				OrgSocketType;
	char			m_Host[4];
	CString			m_OriginalHost;
	short			m_Port;
	unsigned char	Socks4Data[1024];

public:
	// overrides 
	BOOL Create(UINT nSocketPort = 0, int nSocketType=SOCK_STREAM,
		long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE,
		LPTSTR lpszSocketAddress = NULL);
	BOOL AsyncSelect(long lEvent);
	BOOL Connect(LPTSTR lpszHostAddress, UINT nHostPort);

private:
	BOOL CSocket::TranslateAddress( LPTSTR lpszAddr, in_addr * inetAddr );

protected:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnTimer(void);
};


#endif