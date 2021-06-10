#include "StdAfx.h"
#include "connectbase.h"

#define T_ID _T("ConnectBase(Mgr)")

CDataQueue CConnectBase::messageQueue;
CConnectionMgr CConnectBase::m_connMgr;
CConnectBase *CConnectBase::m_pPOSDestination= NULL;
CConnectBase *CConnectBase::m_pTBDestination= NULL;
bool CConnectBase::bTerminate=false;
HANDLE CConnectBase::m_hTerminate = NULL;

CRITICAL_SECTION CritSecConnMgr;

CConnectionMgr::CConnectionMgr()
{
   InitializeCriticalSection(&CritSecConnMgr);
   m_hSourceAckReceived= CreateEvent(NULL, FALSE, FALSE, NULL); // ack received autoreset event
   CConnectBase::m_hTerminate = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CConnectionMgr::~CConnectionMgr()
{
//	int key;
//	CConnectBase *pconn;
//	EnterCriticalSection(&CritSecConnMgr);
//	for (POSITION pos = m_conns.GetStartPosition(); pos!=NULL; ) {
//		m_conns.GetNextAssoc(pos, key, (void *&)pconn);
//		delete pconn;  
//	}
//	LeaveCriticalSection(&CritSecConnMgr);
   CloseHandle(m_hSourceAckReceived);
   CloseHandle(CConnectBase::m_hTerminate);
	DeleteCriticalSection(&CritSecConnMgr);
}

bool CConnectionMgr::AddReadConnection(CConnectBase *pconn)
{
	bool bretval=false;
	EnterCriticalSection(&CritSecConnMgr);
	if (pconn->m_connData.bReader) {
		POSITION pos=m_connReaders.Find(pconn);
		if (!pos) {
			m_connReaders.AddTail(pconn);
			bretval=true;
		}
		else
			ATLASSERT(false);  // ptr already added for this message Id!
	}
	LeaveCriticalSection(&CritSecConnMgr);
	return bretval;
}

bool CConnectionMgr::AddWriteConnection(BYTE messageId, CConnectBase *pconn)
{
	bool bretval=false;
	EnterCriticalSection(&CritSecConnMgr);
	if (!pconn->m_connData.bReader) {
		POSITION pos=m_conns[messageId].Find(pconn);
		if (!pos) {
			m_conns[messageId].AddTail(pconn);
			bretval=true;
		}
		else
			ATLASSERT(false);  // ptr already added for this message Id!
	}
	LeaveCriticalSection(&CritSecConnMgr);
	return bretval;
}

void CConnectionMgr::RemoveConnection(CConnectBase *pconn)
{
	EnterCriticalSection(&CritSecConnMgr);
	removeConnHelper(pconn);
	LeaveCriticalSection(&CritSecConnMgr);
}

// removeConnHelper
// does not use critical region. lock before calling
void CConnectionMgr::removeConnHelper(CConnectBase *pconn)
{
    if (pconn) {
	    if (pconn->m_connData.bReader) {
		    //reader connections
		    POSITION pos=m_connReaders.Find(pconn);
		    if (pos) 
			    m_connReaders.RemoveAt(pos);
	    } else {
		    // writer connections are stored for each msgId
		    for (BYTE i=0; i < MAX_MSGID_CONNECTIONS; i++) {
			    POSITION pos=m_conns[i].Find(pconn);
			    if (pos) 
				    m_conns[i].RemoveAt(pos);
		    }
	    }
    }
}

void CConnectionMgr::CloseReaders()
{
	EnterCriticalSection(&CritSecConnMgr);
	CConnectBase *pconn;
	POSITION pos=m_connReaders.GetHeadPosition();
	while (pos && ((pconn= m_connReaders.GetNext(pos)) != NULL)) 
		pconn->Close();
	LeaveCriticalSection(&CritSecConnMgr);
}

void CConnectionMgr::RemoveWriteConnHandle(HANDLE hSocket)
{
	EnterCriticalSection(&CritSecConnMgr);
	CAtlList <CConnectBase *> toDeleteConns;
	POSITION pos;
	CConnectBase *pConn=NULL;
	for (int MsgId=0; MsgId < MAX_MSGID_CONNECTIONS; MsgId++) {
		pos= m_conns[MsgId].GetHeadPosition();
		while ( pos && ((pConn=m_conns[MsgId].GetNext(pos)) != NULL)) {
			if (pConn->m_handle == hSocket) {
				if (toDeleteConns.Find(pConn) == NULL) // not in the toDelete list yet
					toDeleteConns.AddTail(pConn);
			}
		}
	}
	// cleanup the conns
	pos= toDeleteConns.GetHeadPosition();
	while ( pos && ((pConn=toDeleteConns.GetNext(pos)) != NULL)) {
		removeConnHelper(pConn);
		delete pConn;
	}
	toDeleteConns.RemoveAll();
	LeaveCriticalSection(&CritSecConnMgr);
}

void CConnectionMgr::RemoveAllWriteConns()
{
	EnterCriticalSection(&CritSecConnMgr);
	CAtlList <CConnectBase *> toDeleteConns;
	POSITION pos;
	CConnectBase *pConn=NULL;
	for (int MsgId=0; MsgId < MAX_MSGID_CONNECTIONS; MsgId++) {
		pos= m_conns[MsgId].GetHeadPosition();
		while ( pos && ((pConn=m_conns[MsgId].GetNext(pos)) != NULL)) {
			if (toDeleteConns.Find(pConn) == NULL) // not in the toDelete list yet
				toDeleteConns.AddTail(pConn);
		}
	}
	// cleanup all the conns
	pos= toDeleteConns.GetHeadPosition();
	while ( pos && ((pConn=toDeleteConns.GetNext(pos)) != NULL)) {
		removeConnHelper(pConn);
		delete pConn;
	}
	toDeleteConns.RemoveAll();
	LeaveCriticalSection(&CritSecConnMgr);
}

int CConnectionMgr::Send(CConnectBase *pConnSource, BYTE messageId, const void *ptr, int len, CTraceSystem *pts)
{
   return SendCount(pConnSource, messageId, ptr, len, pts);
}

int CConnectionMgr::SendCount(CConnectBase *pConnSource, BYTE messageId, const void *ptr, int len, CTraceSystem *pts)
{
	bool bFailure= false;
   int cb=0;
	// iterate and send to all subscribers
	EnterCriticalSection(&CritSecConnMgr);

	POSITION pos;
   CConnectBase *pConn;

   // Destination ACK:
   // before switching to send to a new target messageId, 
   // make sure that we request and process any ACKs from
   // the last destination msg id if required
   // This allows destination the needed extra time when necessary to consume the data
   // before a data can be delivered to a different destination
   if (m_lastMsgId && (messageId != m_lastMsgId)) {
	   pos= m_conns[m_lastMsgId].GetHeadPosition();
	   while ( pos && ((pConn=m_conns[m_lastMsgId].GetNext(pos)) != NULL)) {
         // we now have a pConn that we used in the previous send
         // See if it is an ACK connection type, and timeout hasn't expired 
         if (pConn->m_connData.bDestAck) {
            // This pConn wants to acknowledge that it consumed the last send.
            // Request and process ACK.
            unsigned long ulTimeLeft= m_ExpLastSend.Remaining();
            if (ulTimeLeft > 0) {   // we haven't timed out yet
               CExpiringInt expTimedAck;
               expTimedAck.Start();
               pConn->ProcessAck(pConnSource, m_lastMsgId, ulTimeLeft);
               if (pts)
                  pts->Trace(TM_DEBUG,_T("Last MsgId:%x, New MsgId:%x, ms until ACK Timeout:%d. ACK round trip:%d"), 
                              m_lastMsgId, messageId, ulTimeLeft, expTimedAck.Elapsed());
		      }
         }
	   }
   }
   // Source ACK:
   // If the source connection for this new message has changed, check if the previous source 
   // connection that delivered the last message wants to ACK before we can allow a different
   // source to route its message.
   // This allows a source to send a message and do some processing in the background while 
   // making sure that no other source can route messages until it ACKs.  Other 
   // sources may respond to the message but pipeserver will queue the message until the
   // previous source can ACK when processing is complete.
   if (m_pLastConnSource && (m_pLastConnSource != pConnSource)) {
      switch(m_pLastConnSource->m_connData.sourceAck) {
         case CConnectData::ACK_REQUEST_RECEIVE:
         case CConnectData::ACK_RECEIVE_ONLY:
            WaitForSourceAck(pts);
            break;
         case CConnectData::ACK_NONE:
         default:
            break;
      }
   }

	pos= m_conns[messageId].GetHeadPosition();
	while ( pos && ((pConn=m_conns[messageId].GetNext(pos)) != NULL)) {
      cb= pConn->SendCount(pConnSource, ptr, len, pts);
		if (cb <= 0) {
			bFailure=true;
			// connection send failed
			if (m_failedConns.Find(pConn) == NULL) // not in the failed list yet
				m_failedConns.AddTail(pConn);
		}
	}

	// cleanup any failed connections
	if (bFailure) {
		pos= m_failedConns.GetHeadPosition();
		while ( pos && ((pConn=m_failedConns.GetNext(pos)) != NULL)) {
			removeConnHelper(pConn);
			delete pConn;
		}
		m_failedConns.RemoveAll();
	}

   m_lastMsgId= messageId;
   m_ExpLastSend= 1; // set Expiring Int as a flag. Expires to 0.
	LeaveCriticalSection(&CritSecConnMgr);
   if (bFailure)
      pts->Trace(TM_ERROR, _T("Connection Send Failure"));
   return cb; // returns the count of bytes to the last receiver.
}

bool CConnectionMgr::MessageIDHasSubscribers(BYTE msgID)
{
    return (m_conns[msgID].GetCount() > 0);
}

void CConnectionMgr::WaitForSourceAck(CTraceSystem *pts)
{
   // the source connection that sent the previous message wants to acknowledge before
   // another source connection can route its data
   CExpiringInt expTimedAck;
   expTimedAck.Start();

   if (m_pLastConnSource->m_connData.sourceAck == CConnectData::ACK_REQUEST_RECEIVE) {
      // request an ACK packet by sending just a header.
      tRouteHeader hdr;
      hdr.messageID= m_lastMsgId;
      hdr.nLen= sizeof(hdr);
      hdr.nVersion= m_pLastConnSource->m_connData.nHeaderVersion;
      if (!m_pLastConnSource->Send(NULL, &hdr, sizeof(hdr))) { // send the ACK request
         pts->Trace(TM_ERROR, _T("SourceACK, Error sending ACK request"));
         return;
      }
   }
   // request is sent,  wait for the ACK response
   HANDLE aEvents[2];
	aEvents[0] = m_hSourceAckReceived;	         // ACK received event
	aEvents[1] = CConnectBase::m_hTerminate;	// terminate event
   switch (WaitForMultipleObjects(2, aEvents, FALSE, 10000)) {
		case WAIT_OBJECT_0: // Ack Received;
         if (pts)
            pts->Trace(TM_DEBUG, _T("SourceACK received in:%d"), expTimedAck.Elapsed());
         break;
      case WAIT_TIMEOUT:
         if (pts)
            pts->Trace(TM_ERROR, _T("SourceACK Timeout after waiting:%d"), 
                        expTimedAck.Elapsed());
         break;
		case WAIT_OBJECT_0+1: // shutdown
         if (pts)
            pts->Trace(TM_DEBUG, _T("Shutdown while waiting for SourceACK")); 
         break;
      default:
         if (pts)
            pts->Trace(TM_ERROR, _T("SourceACK Error. Elapsed:%d"), expTimedAck.Elapsed());
         break;
   }
}


/////////////////////////////////////////////////////////////////////////////////
//CConnectData
CConnectData::CConnectData()
{
	socketAddr="";
	socketPort=0;
	peerSocketAddr="";
	peerSocketPort=0;
	pipeName="";
	connId= SOURCEID_INVALID;
	hWorkerThread=0;
	bReader=true;
	nHeaderVersion=0;
	BytesRead=0;
	BytesWritten=0;
	ReadPasses=0;
	WritePasses=0;
   bDestAck=false;
   sourceAck=ACK_NONE;
}
CConnectData::~CConnectData()
{
}
   
// copy const  
CConnectData::CConnectData(CConnectData &oData)
{
	socketAddr=		oData.socketAddr;
	peerSocketAddr=	oData.peerSocketAddr;
	connId=			oData.connId;
	hWorkerThread=	oData.hWorkerThread;
	socketPort=		oData.socketPort;
	peerSocketPort=	oData.socketPort;
	pipeName=		oData.pipeName;
	bReader=	    oData.bReader;
	nHeaderVersion= oData.nHeaderVersion;
	BytesRead=		oData.BytesRead;
	BytesWritten=	oData.BytesWritten;
	ReadPasses=		oData.ReadPasses;
	WritePasses=	oData.WritePasses;
    bDestAck=      oData.bDestAck;
    sourceAck=     oData.sourceAck;
}

// assignment  
CConnectData& CConnectData::operator=(const CConnectData &oData)
{
	if (this!= &oData){
		socketAddr=		oData.socketAddr;
		peerSocketAddr=	oData.peerSocketAddr;
		connId=			oData.connId;
		hWorkerThread=	oData.hWorkerThread;
		socketPort=		oData.socketPort;
		peerSocketPort=	oData.peerSocketPort;
		pipeName=		oData.pipeName;
		bReader=		    oData.bReader;
		nHeaderVersion= oData.nHeaderVersion;
		BytesRead=		oData.BytesRead;
		BytesWritten=	oData.BytesWritten;
		ReadPasses=		oData.ReadPasses;
		WritePasses=	oData.WritePasses;
      bDestAck=      oData.bDestAck;
      sourceAck=     oData.sourceAck;
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////
// CConnectBase
CConnectBase::CConnectBase(void)
:	m_handle(INVALID_HANDLE_VALUE),
m_hWnd(NULL)
{
}

CConnectBase::CConnectBase(CConnectBase &oConn)
: m_handle(INVALID_HANDLE_VALUE),	// keep handle invalid. use attach for handle
m_connData(oConn.m_connData),
m_hWnd(oConn.m_hWnd)
{
}

CConnectBase &CConnectBase::operator =(const CConnectBase &oConn)
{
	if (this != &oConn) {
		m_connData = oConn.m_connData;
		m_handle = INVALID_HANDLE_VALUE;	// keep handle invalid. use attach for handle
		m_hWnd = oConn.m_hWnd;
	}
	return *this;
}

CConnectBase::~CConnectBase(void)
{
    m_connMgr.RemoveConnection(this);
	Close();
	if (m_hWnd)
		PostMessage(m_hWnd, WMU_CONNREMOVE, 0, (LPARAM) this);
}

bool CConnectBase::Initialize()
{
	return true;
}

void CConnectBase::Uninitialize()
{
	Close();
}

void CConnectBase::Sent(CConnectBase *pConnSource)
{
	if (m_hWnd)
		PostMessage(m_hWnd, WMU_CONNUPDATE, 0, (LPARAM) this);
   // remember the last source connection that initiated this routed message
   if (pConnSource)
      CConnectBase::m_connMgr.m_pLastConnSource= pConnSource;
}

void CConnectBase::Received()
{
	if (m_hWnd) 
		PostMessage(m_hWnd, WMU_CONNUPDATE, 0, (LPARAM) this);
}

void CConnectBase::SetNotifyHwnd(HWND hwnd)
{
	m_hWnd= hwnd;
}

void CConnectBase::GetPeerName()
{
}

bool CConnectBase::Attach(HANDLE handle)
{
	m_handle = handle;
	ATLASSERT(m_handle != INVALID_HANDLE_VALUE);

    GetPeerName();
	if (m_hWnd)
		PostMessage(m_hWnd, WMU_CONNINSERT, 0, (LPARAM) this);

	return (m_handle != INVALID_HANDLE_VALUE);
}

bool CConnectBase::CancelCommunication()
{
	return true;
}

bool CConnectBase::Accept()
{
	return true;
}

void CConnectBase::OnAccept(HANDLE)
{
}

int CConnectBase::Receive(void *lpBuf, int nBufLen, int nFlags, CTraceSystem *pts)
{
	return 0;
}

int CConnectBase::Send(CConnectBase *pConnSource, const void *lpBuf, int len, CTraceSystem *pts)
{
	return 0;
}

int CConnectBase::SendCount(CConnectBase *pConnSource, const void *lpBuf, int len, CTraceSystem *pts)
{
	return Send(pConnSource, lpBuf, len, pts);
}

bool CConnectBase::ReConnect()
{
	return true;
}

bool CConnectBase::Open(LPCTSTR szName, UINT nFlags)
{
	return true;
}

void CConnectBase::Create()
{
}

void CConnectBase::Close()
{
}

void CConnectBase::ReceiveAck(long msecs)
{
}

void CConnectBase::ProcessAck(CConnectBase *pConnSource, BYTE cmsgId, long msecs)
{
}
