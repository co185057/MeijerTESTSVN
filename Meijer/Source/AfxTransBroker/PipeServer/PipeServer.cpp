// PipeServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PipeServer.h"
#define TM_MODULE_ID TM_MODULE_2
#include "tracesystem.h"

#include "commserverDlg.h"
#define T_ID _T("PipeServer")

long WINAPI PipeWorkerThreadFn(CConnectPipe *pPipe);

CPipeServer::CPipeServer(LPCTSTR lpszPipeName /*=NULL*/)
: CConnectPipe(lpszPipeName)
{
}

CPipeServer::~CPipeServer(void)
{
}

bool CPipeServer::Initialize()
{
	return __super::Initialize();
	//return Listen();
}

void CPipeServer::Uninitialize()
{
	__super::Uninitialize();
}

int NextAvailableSocketIndex()
{
	return 0;
}

void CPipeServer::OnAccept(HANDLE hPipe)
{
	CConnectPipe *pPipe= new CConnectPipe(*this); // use copy const
    pPipe->Attach(hPipe);	// change handle
	pPipe->m_connData.hWorkerThread= ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)PipeWorkerThreadFn,
											(void *)pPipe,0,0);
}

long WINAPI PipeWorkerThreadFn(CConnectPipe * pPipe)
{
	BYTE buf[BUFSIZE] = {0};
	DWORD cbBytesRead = 0;

    CoInitialize(NULL);
    CTraceSystem *pts= new CTraceSystem(CcommServerDlg::m_csTraceFileName);
    pts->Create();
    pts->SetModuleID(TM_MODULE_ID);
    pts->Trace(TM_ENTRY, _T("+PipeWorkerThreadFn"));

	tSourceRouting *psHdr= (tSourceRouting *)buf;
   psHdr->Source.SourceType= pPipe->m_connData.connId;

	// pipes do not use subscriptions
	if (pPipe->m_connData.bReader) { // start read loop
		CConnectBase::m_connMgr.AddReadConnection(pPipe);
		while(!CConnectBase::bTerminate) {
			if (pPipe->m_connData.nHeaderVersion==0) {
				cbBytesRead= pPipe->Receive(&buf[sizeof(tSourceRouting)], 
                                        BUFSIZE - sizeof(tSourceRouting));
				psHdr->Hdr.nLen= sizeof(tRouteHeader) + cbBytesRead;
				psHdr->Hdr.nVersion=0;
				psHdr->Hdr.messageID= 0;
				cbBytesRead+= sizeof(tSourceRouting);
			   Sleep(20);	// no header or length info. throttle between msgs to avoid break up
			} else {	// has the tRouteHeader in the data already, just add internal hdr size to it
                cbBytesRead = pPipe->Receive(&psHdr->Hdr.nLen, sizeof(psHdr->Hdr.nLen));
                if ((cbBytesRead != sizeof(psHdr->Hdr.nLen)) || (psHdr->Hdr.nLen > MAX_BUFSIZE))
                   break;

                cbBytesRead+= pPipe->Receive(&psHdr->Hdr.nVersion,//&buf[sizeof(tInternalSourceInfo)], 
                                            psHdr->Hdr.nLen - sizeof(psHdr->Hdr.nLen));
                pPipe->UpdateReceiveStats();
                if (cbBytesRead < (int) sizeof(tRouteHeader))
				       break;

                // check if this is a SourceACK response
                // Packets equal to the header size are ACKs
                if (cbBytesRead == (int) sizeof(tRouteHeader))
                   SetEvent(CConnectBase::m_connMgr.m_hSourceAckReceived);
                else 
				   cbBytesRead+= sizeof(tInternalSourceInfo);
			}
			if (cbBytesRead > (int) sizeof(tSourceRouting)) // do not push ACKs
   			CConnectBase::messageQueue.Push(new CDataMessage(pPipe, buf, cbBytesRead));
		}
		delete pPipe;
		ATLTRACE("Pipe Worker Reader Thread Exit");
	}
	else { // pipes do not use subscription list
		if (pPipe->m_connData.connId == SOURCEID_TBWRITEPIPE) {
			ATLTRACE("TBMONITOR pipe Connected");
			delete CConnectBase::m_pTBDestination;
			CConnectBase::m_pTBDestination= pPipe;
		}
		else if (pPipe->m_connData.connId == SOURCEID_POSWRITEPIPE) {
			ATLTRACE("POSMONITOR pipe Connected");
			delete CConnectBase::m_pPOSDestination;
			CConnectBase::m_pPOSDestination= pPipe;

			if (pPipe->m_connData.nHeaderVersion != 0) { // expect subscriptions
                
                // writer non-TB socket, identify the subscription list
                tRouteHeader *pHdr= (tRouteHeader *)buf;
                int cnt = pPipe->Receive( pHdr, BUFSIZE);
                pPipe->UpdateReceiveStats();
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
                            pPipe->m_connData.bDestAck= (((BYTE) buf[i] == 0x01) ? true: false);
                            i++;
                        }
                        for (; i< pHdr->nLen; i++){
                            CConnectBase::m_connMgr.AddWriteConnection((BYTE) buf[i], pPipe);
                        }
                        // if I have been waiting for this connection to send something, kick the write thread
                        if (CcommServerDlg::bWaitingForSubscriber)
                        {
                            CcommServerDlg::bWaitingForSubscriber=false;
                            PostMessage(pPipe->m_hWnd, WMU_SETCOLOR, NULL, RGB(0,0,0));
                            SetEvent(CConnectBase::messageQueue.GetDataEvent());
                        }
                    }
                    else {
                        pts->Trace(TM_ERROR,_T("Only Subscription Message Allowed. Closing Pipe"));
                        delete pPipe;   // no other receive allowed
                    }
                }
                else {
                    pts->Trace(TM_ERROR,_T("Subscription Message Error. Closing Pipe"));
                    delete pPipe;   // socket error
                }
            }
		}
	}
	return 0;
}
