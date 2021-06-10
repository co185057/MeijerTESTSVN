#pragma once
#include "connectsocket.h"

class CSocketServer : public CConnectSocket
{
public:
	CSocketServer(void);
	CSocketServer(LPCTSTR lpszSocketAddr, int nPort);
	virtual ~CSocketServer(void);

	virtual bool Initialize();
	virtual void OnAccept(HANDLE hSocket);

};
