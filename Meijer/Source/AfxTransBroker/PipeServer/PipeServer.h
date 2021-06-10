#pragma once

#include "resource.h"

#include "connectPipe.h"

class CPipeServer : public CConnectPipe
{
public:
	CPipeServer(LPCTSTR lpszPipeName=NULL);
	~CPipeServer();
	virtual bool Initialize();
	virtual void Uninitialize();

	virtual void OnAccept(HANDLE hPipe);
};

