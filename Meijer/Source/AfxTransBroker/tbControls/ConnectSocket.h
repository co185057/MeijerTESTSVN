#pragma once
#include "connectbase.h"
#include "winsock2.h"

class CConnectSocket :
	public CConnectBase
{

public:
	CConnectSocket(void);
	CConnectSocket(LPCTSTR lpszSocketAddr, int nPort);
	virtual ~CConnectSocket(void);

	virtual bool Initialize();

	BOOL GetPeerName(CString& rPeerAddress, UINT& rPeerPort);
	BOOL GetSockName(CString& rSocketAddress, UINT& rSocketPort);
	BOOL AsyncSelect(long lEvent);

	bool Listen();
	bool Open();
	void Close();
	bool Connect();
    bool Connect(LPCTSTR lpszSocketAddr, int nPort);
	int  Receive(void* lpBuf, int nBufLen, int nFlags=0);
	int  ReceiveCount(void* lpBuf, int nBufLen, int nFlags=0);
	int  Send(CConnectBase *pConnSource, const void* lpBuf, int nBufLen);
    int  Send(const void* lpBuf, int nBufLen);
	int  SendCount(const void* lpBuf, int nBufLen);
	int  SendCount(CConnectBase *pConnSource, const void* lpBuf, int nBufLen);

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
   static bool m_bSocketsInitialized;

	void WSAError();

protected:
   void ClearBuffer(void);
};
