// commServerDlg.cpp : Implementation of CcommServerDlg

#include "stdafx.h"
#include <atlcoll.h>
#include "commServerDlg.h"
#include "connectSocket.h"
#include "pipeServer.h"
 
#define TM_MODULE_ID TM_MODULE_2
#include "tracesystem.h"

#define T_ID  _T("CommServerDlg")
long WINAPI ServerListenThreadFn(CConnectBase *pconnServer);

int		CcommServerDlg::m_nHeaderVersion=0;
int		CcommServerDlg::m_nTraceOnReceive= 0;
bool	CcommServerDlg::m_bSendAllToTB(true);
bool    CcommServerDlg::bWaitingForSubscriber(false);

CString	CcommServerDlg::m_csTraceFileName(_T("c:\\scot\\bin\\pipeServer.log"));
CString	CcommServerDlg::m_csWaitForSubscriber(_T(""));

#define LIST_CONN		0
#define LIST_PASSES	1
#define LIST_RBYTES	2
#define LIST_WBYTES	3
#define LIST_VERSION	4

#define SOCKETREAD	0
#define SOCKETWRITE	1
#define TBREADPIPE	2
#define	TBWRITEPIPE	3
#define POSREADPIPE 4
#define POSWRITEPIPE 5

#define SERVERConnections 6
CConnectData dataConns[SERVERConnections];
CConnectBase *ServerConns[SERVERConnections]={0};

CcommServerDlg::CcommServerDlg()
{
}

LRESULT CcommServerDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<CcommServerDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	SetupUI();
	CConnectSocket::InitSockets();
	LoadRegistrySettings();
	SetupListeningThreads();

	return 1;  // Let the system set the focus
}

bool CcommServerDlg::WaitForSubscriber(BYTE msgID)
{
	CString cs;
    cs.Format(_T("%02X"), msgID);
    return (m_csWaitForSubscriber.Find(cs) > 0);
}

void CcommServerDlg::SetupUI()
{
	m_hList = GetDlgItem(IDC_LIST_CONN);

	LVCOLUMN lvc;
	lvc.mask=LVCF_TEXT | LVCF_WIDTH;

	lvc.pszText=_T("Connection");
	lvc.cx=125;
	ListView_InsertColumn(m_hList,LIST_CONN, &lvc);

	lvc.pszText=_T("Passes");
	lvc.cx=50;
	ListView_InsertColumn(m_hList,LIST_PASSES,&lvc);

	lvc.pszText=_T("R Bytes");
	lvc.cx=70;
	ListView_InsertColumn(m_hList,LIST_RBYTES,&lvc);

	lvc.pszText=_T("W Bytes");
	lvc.cx=70;
	ListView_InsertColumn(m_hList,LIST_WBYTES,&lvc);

	lvc.pszText=_T("Hdr");
	lvc.cx=42;
	ListView_InsertColumn(m_hList,LIST_VERSION,&lvc);
}

LRESULT CcommServerDlg::OnListInsert(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ListInsertItem((CConnectBase *) lParam);
	return S_OK;
}
LRESULT CcommServerDlg::OnListUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ListUpdateItem((CConnectBase *) lParam);
	return S_OK;
}
LRESULT CcommServerDlg::OnListRemove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ListRemoveItem((CConnectBase *) lParam);
	return S_OK;
}
LRESULT CcommServerDlg::OnListSetColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ListSetColor((COLORREF) lParam);
	return S_OK;
}

void CcommServerDlg::ListInsertItem(CConnectBase *pConn)
{
    CString cs;
	LV_ITEM lvi={0};
	lvi.lParam= (LPARAM)pConn;
	lvi.iItem=9999;
	lvi.iSubItem=0;
	lvi.iImage=
	lvi.mask= LVIF_TEXT | LVIF_PARAM;
	if (pConn->m_connData.socketPort) {
        cs.Format(_T("%d (%s)"),pConn->m_connData.socketPort,pConn->m_connData.peerSocketAddr);
		lvi.pszText= cs.GetBuffer(0);
	}
	else
		lvi.pszText= pConn->m_connData.pipeName.GetBuffer(0);
	ListView_InsertItem(m_hList, &lvi);
}


void CcommServerDlg::ListSetColor(COLORREF color)
{
    ListView_SetTextColor(m_hList, color);
    ListView_RedrawItems(m_hList, 0, ListView_GetItemCount(m_hList)-1);
}

void CcommServerDlg::ListUpdateItem(CConnectBase *pConn)
{
	LV_FINDINFO lvfi;
	lvfi.flags= LVFI_PARAM;
	lvfi.lParam= (LPARAM) pConn;
	int index= ListView_FindItem(m_hList, -1, &lvfi);
	if (index >=0) {
		TCHAR buf[30];
		_itot_s(pConn->m_connData.BytesRead, buf, 30, 10);
		ListView_SetItemText(m_hList, index, LIST_RBYTES, buf);

		_itot_s(pConn->m_connData.BytesWritten, buf, 30, 10);
		ListView_SetItemText(m_hList, index, LIST_WBYTES, buf);

		_itot_s(pConn->m_connData.ReadPasses + pConn->m_connData.WritePasses, 
           buf, 30, 10);
		ListView_SetItemText(m_hList, index, LIST_PASSES, buf);
		_itot_s(pConn->m_connData.nHeaderVersion, buf, 30, 10);
		ListView_SetItemText(m_hList, index, LIST_VERSION, buf);
	}
}

void CcommServerDlg::ListRemoveItem(CConnectBase *pConn)
{
	LV_FINDINFO lvfi;
	lvfi.flags= LVFI_PARAM;
	lvfi.lParam= (LPARAM) pConn;
	int index= ListView_FindItem(m_hList, -1, &lvfi);
	if (index >=0) {
		ListView_DeleteItem(m_hList, index);
	}
}

LRESULT CcommServerDlg::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShutDown();
	return 0;
}

void CcommServerDlg::ShutDown()
{
	// set hTerminate event to end threads that wait on it (writer thread)
    CConnectBase::bTerminate=true;
    SetEvent(CConnectBase::m_hTerminate);

	// reader threads are blocked on reads, so they do not see the hTerminate event
	// tell the CConnectionMgr to clean 
	CConnectBase::m_connMgr.CloseReaders();
	// now reader threads will continue and exit
	ShowWindow(SW_HIDE);
	Sleep(1000);

	// terminate all listening threads first to stop accepting connections
	for (int i=0; i< SERVERConnections; i++) {
		if (ServerConns[i]) {
			// listening server connections are ok to delete. They are only used to accept connections
			// deleting closes the connection so any thread waiting with accept() will be released
			delete ServerConns[i]; 
		}
	}
	WSACleanup();
	PostQuitMessage(0);
	DestroyWindow();

}

void CcommServerDlg::SetupListeningThreads()
{ 
	for (int i=0; i<SERVERConnections; i++) {
		CConnectBase *pConn=NULL;
		if (dataConns[i].socketPort)	// this is a socket connection.
			pConn= new CSocketServer();
		else if (!dataConns[i].pipeName.IsEmpty()) // this is a pipe connection
			pConn= new CPipeServer();
		if (pConn) {
			pConn->m_connData = dataConns[i];
			pConn->Initialize();
			pConn->SetNotifyHwnd(this->m_hWnd);

			dataConns[i].hWorkerThread= ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ServerListenThreadFn,
											(void *)pConn,CREATE_SUSPENDED, NULL);
			ServerConns[i]= pConn;
			ResumeThread(dataConns[i].hWorkerThread);
		}
	}
	HANDLE handle= ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CcommServerDlg::mainWriterThreadFn,
								 (void *)this,0,0);
	CloseHandle(handle);
}

long WINAPI ServerListenThreadFn(CConnectBase *pconnServer)
{
	pconnServer->Accept();	// accept loop
	ATLTRACE("Listening Thread Exiting\n");
	return(0);
}

long WINAPI CcommServerDlg::mainWriterThreadFn(void *dlg)
{
    CDataMessage *pMsg;
    BYTE *pMsgData;
    tSourceRouting *psHdr;
    TCHAR szTitle[MAX_PATH];
    int cb;

    CoInitialize(NULL);
    CTraceSystem ts(CcommServerDlg::m_csTraceFileName, TM_MODULE_ID);
    ts.Create();
    ts.Trace(TM_ENTRY, _T("mainWriterThread()+"));

    HANDLE aEvents[2];
    aEvents[0] = CConnectBase::messageQueue.GetDataEvent();	// data available event
    aEvents[1] = CConnectBase::m_hTerminate;				// terminate event

    while (!CConnectBase::bTerminate) {
        switch (WaitForMultipleObjects(2, aEvents, FALSE, INFINITE)) {
            case WAIT_OBJECT_0: 
                {  // there is a message in the queue
                    DWORD dwDataLen = 0;
                    while (pMsg = CConnectBase::messageQueue.Pop()) {
                        pMsgData = (BYTE *) pMsg->Detach( &dwDataLen );
                        if (!pMsgData) {
                            delete pMsg;
                            continue;
                        }
                        psHdr= (tSourceRouting *) pMsgData;
                        // any source can route
                        if (CcommServerDlg::m_nHeaderVersion == 0) {	// default to POS destination
                            // HeaderVersion 0 means there is no header information
                            while (!CConnectBase::m_pPOSDestination && !CConnectBase::bTerminate) {
                                ts.Trace(TM_DEBUG, _T("Msg ready for POS. POS Write Pipe Not connected yet"));
                                Sleep(5000);
                            }
                            //remove all header info 
                            cb= CConnectBase::m_pPOSDestination->SendCount(pMsg->m_pConnSource,
                                &pMsgData[sizeof(tSourceRouting)], 
                                dwDataLen - sizeof(tSourceRouting), &ts);
                            _stprintf_s(szTitle, MAX_PATH, _T("Routed. Send size:%d, Sent:%d, routed in:%d ms"), 
                                dwDataLen - sizeof(tSourceRouting), 
                                cb, pMsg->m_lifeTime.Elapsed());
                            ts.HexTrace(TM_DEBUG, szTitle, 
                                (UCHAR*)&pMsgData[sizeof(tSourceRouting)], 
                                dwDataLen - sizeof(tSourceRouting));
                        }
                        else {
                            // if this is a non-TB source and TB wants all messages without subscribing
                            // then send it to TB anyway.
                            if ( CcommServerDlg::m_bSendAllToTB &&
                                (psHdr->Source.SourceType != SOURCEID_TBREADPIPE) ) {
                                    // any non TB source goes to TB
                                    while (!CConnectBase::m_pTBDestination && !CConnectBase::bTerminate) {
                                        ts.Trace(TM_DEBUG, _T("Msg ready for TB. TB Write Pipe Not connected yet"));
                                        Sleep(5000);
                                    }
                                    if (CConnectBase::m_pTBDestination) {
                                        //if (CcommServerDlg::m_nHeaderVersion > 0) {
                                            // Keep the spec header, remove the internal pipeserver header.
                                            cb= CConnectBase::m_pTBDestination->SendCount(pMsg->m_pConnSource,
                                                &pMsgData[sizeof(tInternalSourceInfo)], 
                                                dwDataLen - sizeof(tInternalSourceInfo), &ts);
                                            _stprintf_s(szTitle, MAX_PATH,_T("To TB. Send size:%d, Sent:%d, routed in:%d ms"), 
                                                dwDataLen - sizeof(tInternalSourceInfo), 
                                                cb, pMsg->m_lifeTime.Elapsed());
                                            ts.HexTrace(TM_DEBUG, szTitle, 
                                                (UCHAR*)&pMsgData[sizeof(tInternalSourceInfo)], 
                                                dwDataLen - sizeof(tInternalSourceInfo));
                                        //}
                                    }
                            }
                            else if (CConnectBase::m_connMgr.MessageIDHasSubscribers(psHdr->Hdr.messageID))
                            {
                                // Must keep header information. Just remove the internal additional header info
                                // iterate and send to all subscribers
                                cb= CConnectBase::m_connMgr.SendCount(pMsg->m_pConnSource,
                                    psHdr->Hdr.messageID, 
                                    &pMsgData[sizeof(tInternalSourceInfo)], 
                                    dwDataLen - sizeof(tInternalSourceInfo), &ts);
                                _stprintf_s(szTitle, MAX_PATH,_T("Routed. Send size:%d, Sent:%d, routed in:%d ms"), 
                                    dwDataLen - sizeof(BYTE), 
                                    cb, pMsg->m_lifeTime.Elapsed());
                                ts.HexTrace(TM_DEBUG, szTitle, 
                                    (UCHAR*)&pMsgData[sizeof(tInternalSourceInfo)], 
                                    dwDataLen - sizeof(tInternalSourceInfo));
                            }
                            else
                            {
                                if (dlg && ((CcommServerDlg*)dlg)->WaitForSubscriber(psHdr->Hdr.messageID))
                                {
                                    if (((CcommServerDlg*)dlg)->m_hWnd)
                                        ((CcommServerDlg*)dlg)->PostMessage(WMU_SETCOLOR, 0, (LPARAM) RGB(255,0,0));
                                    ((CcommServerDlg*)dlg)->bWaitingForSubscriber=true;
                                    // return to the front of the queue
                                    ts.Trace(TM_WARNING, _T("Must wait for MsgId:%x subscribers. Returning to head of queue"), psHdr->Hdr.messageID);
                                    pMsg->Attach(pMsgData, dwDataLen);
                                    CConnectBase::messageQueue.PushFront(pMsg);
                                    // don't delete these
                                    pMsgData=NULL;
                                    pMsg=NULL;
                                    // Can't do anything anymore until we get a subscriber for this msg.
                                    // The write thread will set the event again when the subscriber comes.
                                    ResetEvent(CConnectBase::messageQueue.GetDataEvent());
                                    break;
                                }
                            }
                        }

                        delete pMsg;
                        delete [] pMsgData;
                    }
                    break;
                }
            default: // termination or error
                // bTerminate should be true, and we'll exit
                break;
        } 
    }
    ts.Trace(TM_ENTRY, _T("mainWriterThread()-"));
    CConnectBase::m_connMgr.RemoveAllWriteConns();
    CoUninitialize();
    return(0);
}


void CcommServerDlg::QueryKey(HKEY hKey) 
{ 
/**
#define MAX_VALUE_NAME 200
    CHAR     achKey[MAX_PATH]; 
    CHAR     achClass[MAX_PATH] = "";  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // length of class string 
    DWORD    cSubKeys;                 // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
 
    DWORD i; 
    DWORD retCode; 
    DWORD cchValue = MAX_VALUE_NAME; 
 
    // Get the class name and the value count. 
    RegQueryInfoKey(hKey,        // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // length of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
    // Enumerate the child keys, until RegEnumKeyEx fails. 

	DWORD	 dwMAX_PATH = sizeof(achClass)/sizeof(TCHAR);

	for (i = 0, retCode = ERROR_SUCCESS; retCode == ERROR_SUCCESS; i++) { 
        retCode = RegEnumKeyEx(hKey, 
                     i, 
                     achKey, 
                     &dwMAX_PATH, 
                     NULL, 
                     NULL, 
                     NULL, 
                     &ftLastWriteTime); 
		if (retCode == (DWORD)ERROR_SUCCESS) {
//			connNames[i]= achKey;
//			totalConnections=i;
		}
    } 
***/
} 

void CcommServerDlg::LoadRegistrySettings()
{
	LONG     lRC;
	HKEY     hKey;
	DWORD dwDataSize;
	TCHAR buf[MAX_PATH];

	CString csbaseKey= _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB\\PipeServer");

	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR) csbaseKey, 0, KEY_QUERY_VALUE, &hKey);
	if(lRC) {
		ATLASSERT(0);
		return ;
	}
//	QueryKey(hKey);

	dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("HeaderSpecVersion"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS)
		m_nHeaderVersion= _ttoi(buf);

	buf[0]=NULL;
	dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("SendAllToTB"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS)
		m_bSendAllToTB= (buf[0]==_T('1')) || (buf[0]==_T('y')) || (buf[0]==_T('Y'));

	dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("TraceOnReceive"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS)
		m_nTraceOnReceive= _ttoi(buf);

	dwDataSize= sizeof(buf);
   _tcscpy_s(buf, MAX_PATH,_T("5000"));
	lRC = RegQueryValueEx(hKey, _T("ACKTimeout"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
   CConnectBase::m_connMgr.SetACKTimeout(_ttoi(buf));

   dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("SocketReadPort"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS){
		dataConns[SOCKETREAD].bReader=true;
		dataConns[SOCKETREAD].socketPort= _ttoi(buf);
		dataConns[SOCKETREAD].connId= SOURCEID_OTHER;
		dataConns[SOCKETREAD].nHeaderVersion= m_nHeaderVersion;
	}
	dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("SocketWritePort"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS){
		dataConns[SOCKETWRITE].bReader=false;
		dataConns[SOCKETWRITE].socketPort= _ttoi(buf);
		dataConns[SOCKETWRITE].connId= SOURCEID_OTHER;
		dataConns[SOCKETWRITE].nHeaderVersion= m_nHeaderVersion;
	}
	dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("TBReadPipe"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS){
      if (_ttoi(buf) > 0)
   		dataConns[TBREADPIPE].socketPort= _ttoi(buf);
      else
   		dataConns[TBREADPIPE].pipeName= buf;
		dataConns[TBREADPIPE].bReader= true;
		dataConns[TBREADPIPE].connId= SOURCEID_TBREADPIPE;
		dataConns[TBREADPIPE].nHeaderVersion= m_nHeaderVersion;
	}
	dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("TBWritePipe"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS){
      if (_ttoi(buf) > 0)
   		dataConns[TBWRITEPIPE].socketPort= _ttoi(buf);
      else
   		dataConns[TBWRITEPIPE].pipeName= buf;
		dataConns[TBWRITEPIPE].bReader= false;
		dataConns[TBWRITEPIPE].connId= SOURCEID_TBWRITEPIPE;
		dataConns[TBWRITEPIPE].nHeaderVersion= m_nHeaderVersion;
	}
	dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("POSReadPipe"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS){
		dataConns[POSREADPIPE].pipeName= buf;
		dataConns[POSREADPIPE].bReader= true;
		dataConns[POSREADPIPE].connId= SOURCEID_POSREADPIPE;
		dataConns[POSREADPIPE].nHeaderVersion= m_nHeaderVersion;
	}
	dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("POSWritePipe"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS){
		dataConns[POSWRITEPIPE].pipeName= buf;
		dataConns[POSWRITEPIPE].bReader= false;
		dataConns[POSWRITEPIPE].connId= SOURCEID_POSWRITEPIPE;
		dataConns[POSWRITEPIPE].nHeaderVersion= m_nHeaderVersion;
	}

	dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("TraceFileName"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS)
		m_csTraceFileName= buf;

    dwDataSize= sizeof(buf);
	lRC = RegQueryValueEx(hKey, _T("WaitForSubscriber"), NULL, NULL, (LPBYTE)buf, &dwDataSize);
	if(lRC == ERROR_SUCCESS){
        m_csWaitForSubscriber= buf;
	}

	RegCloseKey(hKey);
	return;
}  

LRESULT CcommServerDlg::OnEnChangeEdit1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CAxDialogImpl<CcommServerDlg>::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	return 0;
}

LRESULT CcommServerDlg::OnLvnItemchangedConnList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	return 0;
}

LRESULT CcommServerDlg::OnSocketResetAndRemove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// a Write socket has been reset.
	// wParam is the handle
    CTraceSystem ts(CcommServerDlg::m_csTraceFileName, TM_MODULE_ID);
    ts.Create();
    ts.Trace(TM_WARNING, _T("FD_CLOSE received from this socket.  Closing."));
	CConnectBase::m_connMgr.RemoveWriteConnHandle((HANDLE) wParam);
	return 0;
}
