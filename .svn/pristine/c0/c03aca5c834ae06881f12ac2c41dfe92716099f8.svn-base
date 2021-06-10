#pragma once
#include "connectbase.h"

class CConnectPipe :
	public CConnectBase
{
public:
	CConnectPipe(LPCTSTR lpszPipeName= NULL);
	virtual ~CConnectPipe(void);

	virtual bool Initialize();
	virtual int Receive(void *lpBuf, int nBufLen, int nFlags=0);
	virtual int Send(CConnectBase *pConnSource, const void *lpBuf, int nBufLen, CTraceSystem *pts=NULL);

	virtual bool CConnectPipe::Accept();
	virtual void Close();
   void UpdateReceiveStats();	

	HANDLE CreatePipe();
public:
	CString m_PipeName;
};
