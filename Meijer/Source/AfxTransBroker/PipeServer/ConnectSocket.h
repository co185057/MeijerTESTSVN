#pragma once
#include "connectbase.h"
#include "winsock2.h"
#include "tracesystem.h"

class CConnectSocket :
	public CConnectBase
{

public:
	CConnectSocket(void);
	CConnectSocket(LPCTSTR lpszSocketAddr, int nPort);
	virtual ~CConnectSocket(void);

	virtual bool Initialize();
	void Create();

	virtual void GetPeerName();
	BOOL GetSockName();
	BOOL AsyncSelect(long lEvent);

	bool Listen();
	bool Open();
	void Close();
	bool Connect();
	int  Receive(void* lpBuf, int nBufLen, int nFlags=0, CTraceSystem *pts=NULL);
	int  ReceiveCount(void* lpBuf, int nBufLen, int nFlags=0, CTraceSystem *pts=NULL);
	int  WaitReceive(void* lpBuf, int nCount, int msecs, int nFlags=0);
	int  Send(CConnectBase *pConnSource, const void* lpBuf, int nBufLen, CTraceSystem *pts=NULL);
	int  SendCount(CConnectBase *pConnSource, const void* lpBuf, int nBufLen, CTraceSystem* pts=NULL);

	virtual bool Accept();
	virtual void OnAccept(HANDLE hSocket);

   virtual void ReceiveAck(long msec);
   virtual void ProcessAck(CConnectBase *pConnSource, BYTE cmsgId, long msecs);

	virtual void OnReceive(int /*nErrorCode*/);
	virtual void OnSend(int /*nErrorCode*/);
	virtual void OnOutOfBandData(int /*nErrorCode*/);
	virtual void OnConnect(int /*nErrorCode*/);
	virtual void OnClose(int /*nErrorCode*/);

	static bool InitSockets();
	void WSAError(CTraceSystem *pts=NULL);

protected:
   void ClearBuffer(void);
};
