#include "StdAfx.h"
#include "socketserver.h"

#define TM_MODULE_ID TM_MODULE_2
#include "tracesystem.h"

#include "commserverDlg.h"
#define T_ID _T("SocketServer")

long WINAPI SocketWorkerThreadFn(CConnectSocket *pSocketItem);

CSocketServer::CSocketServer(void)
{
}

CSocketServer::CSocketServer(LPCTSTR lpszSocketAddr, int nPort)
:CConnectSocket(lpszSocketAddr, nPort)
{
}

CSocketServer::~CSocketServer(void)
{
}

bool CSocketServer::Initialize()
{
    __super::Initialize();
    return Listen();
}

void CSocketServer::OnAccept(HANDLE handle)
{
    CConnectSocket *pSocket= new CConnectSocket(*this); // use copy const

    pSocket->Attach(handle);	// change the handle;
    // create a worker thread to work on this socket handle
    pSocket->m_connData.hWorkerThread= ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SocketWorkerThreadFn,
        (void *)pSocket, 0, 0);
}

long WINAPI SocketWorkerThreadFn(CConnectSocket *pSocket)
{
    BYTE buf[BUFSIZE]={0};
    int nPacketLen = 0;
    int cnt;

    CoInitialize(NULL);
    CTraceSystem *pts= new CTraceSystem(CcommServerDlg::m_csTraceFileName);
    pts->Create();
    pts->SetModuleID(TM_MODULE_ID);
    pts->Trace(TM_ENTRY, _T("+SocketWorkerThread"));

    if (pSocket->m_connData.bReader) { // start read loop
        tSourceRouting *psHdr;
        CConnectBase::m_connMgr.AddReadConnection(pSocket);
        BYTE *pbuffer = NULL;

        while(!CConnectBase::bTerminate) {
            if (CcommServerDlg::m_nHeaderVersion == 0) { // no headers in packets
                cnt= pSocket->Receive(&buf[sizeof(tSourceRouting)], BUFSIZE - sizeof(tSourceRouting));
                if (cnt > 0) {
                    psHdr= (tSourceRouting *)buf;
                    psHdr->Source.SourceType= pSocket->m_connData.connId;
                    psHdr->Hdr.nLen= sizeof(tRouteHeader) + cnt;
                    psHdr->Hdr.nVersion=0;
                    psHdr->Hdr.messageID= 0;

                    if (CcommServerDlg::m_nTraceOnReceive)
                        pts->HexTrace(TM_WARNING, _T("No Header. Tracing on Receive"), 
                        &buf[sizeof(tSourceRouting)], 
                        cnt);
                    cnt+= sizeof(tSourceRouting);
                    CConnectBase::messageQueue.Push(new CDataMessage( pSocket, buf, cnt));
                }
                else {
                    pts->Trace(TM_ERROR, _T("HeaderVersion 0 error - Closing socket."));
                    break;
                }
            }
            else {
                // peek for the packet length (first ULONG of packet) 
                cnt = pSocket->ReceiveCount( &nPacketLen, sizeof(psHdr->Hdr.nLen), MSG_PEEK, pts);
                if (cnt != sizeof(psHdr->Hdr.nLen)) {
                    pts->Trace(TM_ERROR, _T("could not retrieve packet length. Closing socket.(%d)"), cnt);
                    break;
                }
                if (nPacketLen > MAX_BUFSIZE) {
                    pts->Trace(TM_ERROR, _T("packet length in received hdr too large:%d. packet next. Closing socket."),
                                nPacketLen);
                    pbuffer = new BYTE[ nPacketLen + sizeof(psHdr->Source)];
                    cnt = pSocket->Receive(pbuffer, nPacketLen, 0, pts);
                    pts->HexTrace(TM_ERROR, _T("Problem packet:"), (UCHAR*)pbuffer, cnt);
                    delete [] pbuffer;
                    break;
                }
                if (nPacketLen >= BUFSIZE) // dynamic allocation
                    pbuffer = new BYTE[ nPacketLen + sizeof(psHdr->Source)];
                else
                    pbuffer = buf;

                psHdr = (tSourceRouting *)pbuffer;
                psHdr->Source.SourceType = pSocket->m_connData.connId;

                cnt = pSocket->ReceiveCount( &psHdr->Hdr, nPacketLen, 0, pts);

                if (CcommServerDlg::m_nTraceOnReceive)
                    pts->HexTrace(TM_WARNING, _T("Tracing on Receive"), (UCHAR*)&psHdr->Hdr, cnt);

                if (cnt >= (int) sizeof(tRouteHeader)) {
                    // check if this is a SourceACK response
                    // Packets equal to the header size are ACKs
                    if (cnt == (int) sizeof(tRouteHeader)) {
                        SetEvent(CConnectBase::m_connMgr.m_hSourceAckReceived);
                    } else if (psHdr->Hdr.messageID == MSGID_SUBSCRIPTION) { // could be an ACK subscription 
                        // first byte after a subscription id is the Source ACK/NoAck flag
                        BYTE *pData= (BYTE *)&psHdr->Hdr;
                        pSocket->m_connData.sourceAck= (CConnectData::tAcknowledgeType) pData[sizeof(tRouteHeader)];
                    } else {
                        cnt+= sizeof(psHdr->Source);
                        CConnectBase::messageQueue.Push(new CDataMessage( pSocket, pbuffer, cnt));
                    }   
                }
                else {
                    pts->Trace(TM_ERROR, _T("packet size smaller than header:%d.packet next."), cnt);
                    pts->HexTrace(TM_ERROR, _T("Problem packet:"), (UCHAR*)&psHdr->Hdr, cnt);
                }
                if (nPacketLen >= BUFSIZE) // dynamic allocation.
                    delete [] pbuffer;
            }
        }
        delete pSocket;
        ATLTRACE("Socket Worker Reader Thread Exit");
    }
    else { 
        if (pSocket->m_connData.connId == SOURCEID_TBWRITEPIPE) {
            ATLTRACE("TBMONITOR socket Connected");
            delete CConnectBase::m_pTBDestination;
            CConnectBase::m_pTBDestination= pSocket;
        }
        else {
            // writer non-TB socket, identify the subscription list
            tRouteHeader *pHdr= (tRouteHeader *)buf;
            cnt = pSocket->ReceiveCount( &nPacketLen, sizeof(pHdr->nLen), MSG_PEEK, pts);
            if (cnt != sizeof(pHdr->nLen)) {
                Sleep(250); // try one more time
                cnt = pSocket->ReceiveCount( &nPacketLen, sizeof(pHdr->nLen), MSG_PEEK, pts);
            }
            pts->Trace(TM_INFO,_T("Port:%d, Subscription Msg packet len:%d"), pSocket->m_connData.socketPort, nPacketLen);
            if ((cnt == sizeof(pHdr->nLen)) && (nPacketLen <= (MAX_MSGID_CONNECTIONS+ sizeof(tRouteHeader)+1))) {
                cnt = pSocket->ReceiveCount( pHdr, nPacketLen, 0, pts);
                if (cnt > 0)
                    pts->HexTrace(TM_INFO,_T("Subscription Message"), buf, cnt);
                if ((cnt != SOCKET_ERROR) && (cnt > (int) sizeof(tRouteHeader))) {
                    ATLASSERT (pHdr->nLen >= sizeof(tRouteHeader));
                    if (pHdr->messageID == MSGID_SUBSCRIPTION) {
                        // first byte after a subscription id is ACK/NoAck flag
                        // then it's the list of messageIds to listen to
                        unsigned int i=sizeof(tRouteHeader);
                        if (((BYTE) buf[i] == 0x00) || ((BYTE) buf[i] == 0x01)) {
                            // first byte is an ACK flag (0/1). Otherwise, treat like a messageId
                            pSocket->m_connData.bDestAck= (((BYTE) buf[i] == 0x01) ? true: false);
                            i++;
                        }
                        for (; i< pHdr->nLen; i++){
                            CConnectBase::m_connMgr.AddWriteConnection((BYTE) buf[i], pSocket);
                        }
                        WSAAsyncSelect((SOCKET)pSocket->m_handle, pSocket->m_hWnd, WMU_CONNCLOSEANDREMOVE, FD_CLOSE);
                        // if I have been waiting for this connection to send something, kick the write thread
                        if (CcommServerDlg::bWaitingForSubscriber)
                        {
                            CcommServerDlg::bWaitingForSubscriber=false;
                            PostMessage(pSocket->m_hWnd, WMU_SETCOLOR, NULL, RGB(0,0,0));
                            SetEvent(CConnectBase::messageQueue.GetDataEvent());
                        }

                    }
                    else {
                        pts->Trace(TM_ERROR,_T("Port %d Only Subscription Message Allowed. Closing Socket"), pSocket->m_connData.socketPort);
                        delete pSocket;   // no other receive allowed on this socket
                    }
                }
                else {
                    pts->Trace(TM_ERROR,_T("Port %d Subscription Message Error. Closing Socket"), pSocket->m_connData.socketPort);
                    delete pSocket;   // socket error
                }
            }
            else {
                cnt = pSocket->Receive( pHdr, nPacketLen);
                if (cnt > 0)
                    pts->HexTrace(TM_ERROR,_T("Subscription Message"), buf, cnt);

                pts->Trace(TM_ERROR,_T("Port %d Subscription Message Error. Closing Socket"), pSocket->m_connData.socketPort);
                delete pSocket;   // socket error
            }
        }
    }
    pts->Trace(TM_ENTRY, _T("-SocketWorkerThread"));
    delete pts;
    CoUninitialize();
    return(0);
};

