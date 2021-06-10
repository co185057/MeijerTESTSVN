#pragma once
//#include "atlcoll.h"
//#include "expiringInt.h"

//#define TM_MODULE_ID TM_MODULE_2
//#include "tracesystem.h"

#include "TBMsgObj.h"

#define WMU_CONNINSERT	WM_USER+ 1
#define WMU_CONNUPDATE	WM_USER+ 2
#define WMU_CONNREMOVE	WM_USER+ 3

#define MSGID_SUBSCRIPTION 0xFF

#define BUFSIZE_MULTIPLE 3  // number of 4k blocks in buffers
#define BUFSIZE 4096 * BUFSIZE_MULTIPLE   // size of static buffer to receive packet
#define MAX_BUFSIZE  BUFSIZE * 4          // max allowed length of a single packet
#define PIPE_TIMEOUT 1000


class CConnectData {
public:
	CConnectData();
	~CConnectData();
	
enum tConnectStatus
{
   STATUS_OK,
   STATUS_FAILED
};

enum tAcknowledgeType
{
   ACK_NONE = 0,
   ACK_REQUEST_RECEIVE = 1,
   ACK_RECEIVE_ONLY =2
};

   CConnectData(const CConnectData &);                 // copy const  
   CConnectData& operator=(const CConnectData &);// assignment  
	
	CString	socketAddr;
	int		socketPort;
	CString	pipeName;
	HANDLE    hWorkerThread;
	bool		bReader;
   bool     bDestAck;   // the destination connection wants to acknowledges before we route to another destination
   tAcknowledgeType  sourceAck; // this source wants to acknowledge before we route from another source
   tConnectStatus status;
	int		nHeaderVersion;
	LONG	BytesRead;
	LONG	BytesWritten;
	LONG	ReadPasses;
	LONG	WritePasses;
};

class CConnectBase
{
public:
	CConnectBase(void);
	CConnectBase(CConnectBase &oConn); // copy const
	CConnectBase & operator=(const CConnectBase &);	// assignment

	virtual ~CConnectBase(void);
	
	virtual bool Initialize();
	virtual void Uninitialize();
	
	virtual void Close();
	virtual bool ReConnect();

	virtual int  Send(CConnectBase *pConnSource, const void* lpBuf, int nBufLen);
   virtual int  SendCount(CConnectBase *pConnSource, const void* lpBuf, int nBufLen);
	virtual int  Receive(void* lpBuf, int nBufLen, int nFlags=0);

	virtual bool Accept();					// called to wait for connection
	virtual void OnAccept(HANDLE hConn);	// callback with accepted connection handle

   virtual void ReceiveAck(long msecs);
   virtual void ProcessAck(CConnectBase *pConnSource, BYTE cmsgId, long msecs);
	
   bool Attach(HANDLE handle);
	void Sent(CConnectBase *pConnSource);
	void Received();
	void SetNotifyHwnd(HWND hwnd);

public:
	HANDLE			m_handle;
	CConnectData	m_connData;
	HWND			m_hWnd;
	static bool bTerminate;
	static HANDLE m_hTerminate;

};
