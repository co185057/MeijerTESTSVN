#pragma once
#include "atlcoll.h"
#include "DataQueue.h"
#include "expiringInt.h"

#define TM_MODULE_ID TM_MODULE_2
#include "tracesystem.h"

#define WMU_CONNINSERT	WM_USER+ 1
#define WMU_CONNUPDATE	WM_USER+ 2
#define WMU_CONNREMOVE	WM_USER+ 3
#define WMU_CONNCLOSEANDREMOVE WM_USER+ 4
#define WMU_SETCOLOR    WM_USER+5

#pragma pack(1)
typedef struct {
	ULONG	nLen;
	ULONG	nVersion;
	BYTE	messageID;
} tSpecHeaderV01;

#define tRouteHeader tSpecHeaderV01 

//class CConnectBase;

typedef struct {
   BYTE SourceType;
} tInternalSourceInfo;

typedef struct {
	tInternalSourceInfo Source;
	tRouteHeader Hdr;
} tSourceRouting;
#pragma pack()


#define MSGID_SUBSCRIPTION 0xFF

#define BUFSIZE_MULTIPLE 3  // number of 4k blocks in buffers
#define BUFSIZE 4096 * BUFSIZE_MULTIPLE   // size of static buffer to receive packet
#define MAX_BUFSIZE  BUFSIZE * 4          // max allowed length of a single packet
#define PIPE_TIMEOUT 1000

#define MAX_MSGID_CONNECTIONS 255

class CConnectionMgr
{
public:
	CConnectionMgr();
	virtual ~CConnectionMgr();

	void 	RemoveConnection(CConnectBase *pconn);
	void  RemoveWriteConnHandle(HANDLE hSocket);
	bool	AddWriteConnection(BYTE messageId,  CConnectBase *pconnInfo);
	bool	AddReadConnection(CConnectBase *pconnInfo);
	int	Send(CConnectBase *pConnSource, BYTE messageId, const void *ptr, int len, CTraceSystem *pts= NULL);
	int	SendCount(CConnectBase *pConnSource, BYTE messageId, const void *ptr, int len, CTraceSystem *pts=NULL);
	void	RemoveAllWriteConns();
	void	CloseReaders();

    bool MessageIDHasSubscribers(BYTE msgID);

   void SetACKTimeout(int timeoutValMs)
   {
      m_ExpLastSend.SetExpiration(timeoutValMs);
   };
   CConnectBase *m_pLastConnSource;
   HANDLE m_hSourceAckReceived;
private:
	void removeConnHelper(CConnectBase *pconn);

   CAtlList<CConnectBase *> m_conns[MAX_MSGID_CONNECTIONS];	// multiple subscribers for max BYTE value
   CAtlList<CConnectBase *> m_connReaders;	// reader connections
	CAtlList<CConnectBase *> m_failedConns;   // manage failed connections
   BYTE         m_lastMsgId;
   CExpiringInt m_ExpLastSend;
   void WaitForSourceAck(CTraceSystem *pts);
};

enum tSourceID
{
	SOURCEID_INVALID		=0,
	SOURCEID_TBREADPIPE,
	SOURCEID_TBWRITEPIPE,
	SOURCEID_POSREADPIPE,
	SOURCEID_POSWRITEPIPE,
	SOURCEID_OTHER
};

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

   CConnectData(CConnectData &);                 // copy const  
   CConnectData& operator=(const CConnectData &);// assignment  
	
	CString	peerSocketAddr;
	int		peerSocketPort;
	CString	socketAddr;
	int		socketPort;
	CString	pipeName;
	tSourceID	connId;
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
	virtual void Create();
	
	virtual bool Open(LPCTSTR szFileName, UINT nOpenFlags);
	virtual void Close();
	virtual bool ReConnect();
	virtual void GetPeerName();

	virtual int  Send(CConnectBase *pConnSource, const void* lpBuf, int nBufLen, CTraceSystem *pts=NULL);
   virtual int  SendCount(CConnectBase *pConnSource, const void* lpBuf, int nBufLen, CTraceSystem *pts=NULL);
	virtual int  Receive(void* lpBuf, int nBufLen, int nFlags=0, CTraceSystem *pts=NULL);

	virtual bool Accept();					// called to wait for connection
	virtual void OnAccept(HANDLE hConn);	// callback with accepted connection handle

	virtual bool CancelCommunication();

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
	static CDataQueue messageQueue;
	static CConnectionMgr m_connMgr;
	static CConnectBase *m_pTBDestination;
	static CConnectBase *m_pPOSDestination;
	static bool bTerminate;
	static HANDLE m_hTerminate;

protected:
};
