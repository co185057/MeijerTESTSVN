// tbConnect.cpp : Implementation of CtbConnect
#include "stdafx.h"
#include "tbConnect.h"
#include "itemobj.h"
#include "CashDrawerObj.h"
#include "safearrayVariant.h"
#include "ImplementMacros.h"

// CtbConnect

STDMETHODIMP CtbConnect::Connect(void)
{
   LoadRegistrySettings();
   StartReadSocket();
   StartWriteSocket();
   return S_OK;
}

STDMETHODIMP CtbConnect::ConnectAsTB(void)
{
   LoadRegistrySettings();
   StartReadSocket(true);
   StartWriteSocket(true);
   return S_OK;
}

void CtbConnect::LoadRegistrySettings(void)
{
	HKEY	hKey;
	long	rc;
	DWORD disposition;
	LPCTSTR szAddressValue = _T("loopback");
	LPCTSTR szDeviceIDs =_T("10,4A,4,54,7,9,A,B,D");
	CString PortNumber = _T("5050");
	LPCTSTR szAckNoAck = _T("1");

   if (!m_hWnd) {
      RECT rc = {0,0,0,0};
      // Don't have access to the container's window so just use the desktop.  
      CreateControlWindow(::GetDesktopWindow(), rc);
      _ASSERT (m_hWnd);
   }

//Registry access constants and default values
	LPCTSTR const NCRTBDEVICE_SUBKEY = _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB\\PipeServer");
	LPCTSTR const NCRTBDEVICE_SUBKEY_2 = _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB\\SocketServerInfo");
	LPCTSTR const	NCRTBDEVICE_SERVER_ADDRESS_VALUE = _T("ServerAddress");
	LPCTSTR const NCRTBDEVICE_RECIEVE_FROM_DEVICEID = _T("ListenToDevices");
	LPCTSTR const NCRTBDEVICE_ACKNOWLDGE_CAPABLE = _T("AckNoAck");

//Open the subkey (or create it if it does not exist)
	rc = RegCreateKeyEx( HKEY_LOCAL_MACHINE,    // handle of main key
		                   NCRTBDEVICE_SUBKEY_2, // addr of subkey name
		                   0,                      // reserved
		                   0,                      // no type for subkey value?
		                   REG_OPTION_NON_VOLATILE,    // will be saved across system reboots
		                   KEY_ALL_ACCESS,         // desired security access
		                   NULL,                   // default security, don't care
		                   &hKey,                  // subkey handle is returned
		                   &disposition );         // returns either REG_CREATED_NEW_KEY or
	                                             // REG_OPENED_EXISTING_KEY
	ATLASSERT(rc==ERROR_SUCCESS);

  // Treat the case where the subkey did not exist as if the value
  // were 0.  Set it to 5050
  if (disposition == REG_CREATED_NEW_KEY) {
	rc = RegSetValueEx( hKey,                  // subkey handle from creation/opening
			                  NCRTBDEVICE_SERVER_ADDRESS_VALUE, // addr of value name
			                  0,                     // reserved
			                  REG_SZ,             // value type
			                  (UCHAR *)szAddressValue,
			                  (DWORD)_tcslen(szAddressValue) * sizeof(TCHAR) + sizeof(TCHAR));	 // data length
	ATLASSERT(rc==ERROR_SUCCESS);
  }

	CString  csDefaultAddress = _T("ServerAddress");
	TCHAR szServerName[200];
	DWORD dwDataSize = sizeof(szServerName);
	rc = RegQueryValueEx(hKey,csDefaultAddress,NULL,NULL, (LPBYTE) szServerName, &dwDataSize);
	ATLASSERT(rc==ERROR_SUCCESS);
	m_csServerName = szServerName;

	/*
	CString  csDefaultDeviceIDs = "ListenToDevices";
	char szDeviceIDs2[200];
	dwDataSize = sizeof(szDeviceIDs2);
	rc = RegQueryValueEx(hKey,csDefaultDeviceIDs,NULL,NULL, (LPBYTE) szDeviceIDs2, &dwDataSize);
	ATLASSERT(rc==ERROR_SUCCESS);
	m_csDeviceIDs = szDeviceIDs2;

	CString csAckNoAck = "AckNoAck";
	char caAckNoAck[2];
	dwDataSize = sizeof(caAckNoAck);
	rc = RegQueryValueEx(hKey,csAckNoAck,NULL,NULL, (LPBYTE) caAckNoAck, &dwDataSize);
	ATLASSERT(rc==ERROR_SUCCESS);
	m_bAck = (caAckNoAck=="1");
	*/

	rc = RegCloseKey(hKey);
	ATLASSERT(rc==ERROR_SUCCESS);

	//Open the subkey (or create it if it does not exist)
	rc = RegCreateKeyEx( HKEY_LOCAL_MACHINE,    // handle of main key
		                   NCRTBDEVICE_SUBKEY, // addr of subkey name
		                   0,                      // reserved
		                   0,                      // no type for subkey value?
		                   REG_OPTION_NON_VOLATILE,    // will be saved across system reboots
		                   KEY_ALL_ACCESS,         // desired security access
		                   NULL,                   // default security, don't care
		                   &hKey,                  // subkey handle is returned
		                   &disposition );         // returns either REG_CREATED_NEW_KEY or
	                                             // REG_OPENED_EXISTING_KEY

	ATLASSERT(rc==ERROR_SUCCESS);
  //Get Server Address and Port Number values from registry

  if (disposition != REG_CREATED_NEW_KEY) {
	TCHAR szPortNumber[150];
	dwDataSize=sizeof(szPortNumber);
	rc = RegQueryValueEx(hKey, _T("SocketReadPort"), NULL, NULL, (LPBYTE) szPortNumber, &dwDataSize);
	ATLASSERT(rc==ERROR_SUCCESS);
	m_nPortNumberRead = _ttoi(szPortNumber);

	dwDataSize=sizeof(szPortNumber);
	rc = RegQueryValueEx(hKey,_T("SocketWritePort"),NULL,NULL, (LPBYTE) szPortNumber, &dwDataSize);
	ATLASSERT(rc==ERROR_SUCCESS);
	m_nPortNumberWrite = _ttoi(szPortNumber); 

 	dwDataSize=sizeof(szPortNumber);
	rc = RegQueryValueEx(hKey,_T("TBWritePipe"),NULL,NULL, (LPBYTE) szPortNumber, &dwDataSize);
	ATLASSERT(rc==ERROR_SUCCESS);
	m_nTBPortNumberWrite = _ttoi(szPortNumber); 

  	dwDataSize=sizeof(szPortNumber);
	rc = RegQueryValueEx(hKey,_T("TBReadPipe"),NULL,NULL, (LPBYTE) szPortNumber, &dwDataSize);
	ATLASSERT(rc==ERROR_SUCCESS);
	m_nTBPortNumberRead = _ttoi(szPortNumber); 

	rc = RegCloseKey(hKey);
	ATLASSERT(rc==ERROR_SUCCESS);
  }
}

int CtbConnect::StartReadSocket(bool bConnectAsTB /*=false*/)
{
   // NOTE: OUR Read socket is the pipeserver Write socket, so use the writesocket
   if (bConnectAsTB) {
      // TB does not need to subscribe
		if (!m_ReadSocket.Initialize())
			ATLASSERT(false);
	//      ts2.Trace(TM_ERROR,"**StartSocketClient Socket create failed!!");
      m_ReadSocket.Connect(m_csServerName, m_nTBPortNumberWrite);
      Sleep(300);
      StartReadThread();
   }
	else {
		if (m_csDeviceIDs.IsEmpty()) // no subscription required
			return 0;	
		if (!m_ReadSocket.Initialize())
			ATLASSERT(false);
	//      ts2.Trace(TM_ERROR,"**StartSocketClient Socket create failed!!");
		if (m_ReadSocket.Connect(m_csServerName, m_nPortNumberWrite)) {
			CString csMessage;
			if (m_bAck)
				csMessage = _T("1,") + m_csDeviceIDs;
			else
				csMessage = _T("0,") + m_csDeviceIDs;

			int iLength = csMessage.GetLength();
			if (iLength > 0)	{
				int iComa = 0;
				int iNumberOfDevices = 0;
				do	{
					iComa = csMessage.Find(_T(","), iComa);
					iNumberOfDevices++;
				} while (iComa++ != -1);

				int iTotallength = sizeof(tSpecHeaderV01)+ iNumberOfDevices;
				ATLASSERT(iTotallength > sizeof(tSpecHeaderV01));

				tRouteHeader *pPipeHdr= (tRouteHeader*) new BYTE[iTotallength];

				BYTE *ptr=(BYTE*)&pPipeHdr[1];

				pPipeHdr->nLen = iTotallength;
				pPipeHdr->messageID=0xFF;
				pPipeHdr->nVersion=1;

				CString csMessage2;
				int iDeviceID = 0;
				int iLength2 = iLength;

				for(int i=0; i<= (iNumberOfDevices); i++)	{
					if((iComa = csMessage.Find(_T(","), 0)) == -1) break;
					csMessage2 = csMessage.Left(iComa);
					csMessage = csMessage.Right(iLength - (iComa+1));
					iLength -= (csMessage2.GetLength() + 1);
					_stscanf(csMessage2, _T("%x"),&iDeviceID);
					ptr[i] = iDeviceID;
				}

				if(csMessage.GetLength() >= 1) {
					_stscanf(csMessage,_T("%x"),&iDeviceID);
					ptr[i] = iDeviceID;
				}
			
				Sleep(300);
				m_ReadSocket.Send((BYTE *)pPipeHdr,iTotallength);

				delete [] (BYTE*)pPipeHdr;
				StartReadThread();
			}
		}
	}
   return 0;
}

int CtbConnect::StartWriteSocket(bool bConnectAsTB /*=false*/)
{
   // start the outgoing socket
   if (m_WriteSocket.Initialize()) {
      // NOTE: the OUR Write socket is the pipeserver READ socket, so use the read socket
      int nPort= (bConnectAsTB ? m_nTBPortNumberRead : m_nPortNumberRead);
      if (! m_WriteSocket.Connect(m_csServerName, nPort)) {
         ATLASSERT(false);
//		   ts2.Trace( TM_INFO, _T("toTBSocket socket connected to PipeServer" ) );
      }
//      else
//		   ts2.Trace( TM_ERROR, _T("toTBSocket Failed to connect to PipeServer" ) );
   } 
//   else
//		ts2.Trace( TM_ERROR, _T("Failed to create toTBSocket" ) );
   return 0;
}

void CtbConnect::StartReadThread(void)
{
	HANDLE handle= ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CtbConnect::ReceiveThreadFn,
								 (void *)this,0,0);
	CloseHandle(handle);
}

STDMETHODIMP CtbConnect::Send(ITBStream * pObj)
{
   LONG cb=0;
   BYTE *pBuf=NULL; 

   HRESULT hr= pObj->StreamOut(&cb, &pBuf);
	//lock 
   int sendResult=m_WriteSocket.SendCount(pBuf, cb);
	//unlock
   delete [] pBuf;
   return S_OK;

}


long WINAPI CtbConnect::ReceiveThreadFn(LPVOID lparam)
{
   BYTE *pbuffer;
   tRouteHeader *pHdr;
	int nPacketLen = 0;
   int cnt;
//   CoInitialize(NULL);
   
   CtbConnect *ptbConn= (CtbConnect *)lparam;
	while(1)	{
      // peek for the packet length (first ULONG of packet) 
      cnt = ptbConn->m_ReadSocket.ReceiveCount( &nPacketLen, sizeof(pHdr->nLen), MSG_PEEK);
      if (cnt < (int) sizeof(pHdr->nLen))
         break;
      if (nPacketLen > MAX_BUFSIZE) {
         ATLTRACE("packet length in received hdr too large. Closing socket.");
         break;
      }

	  // Allocate a buffer to hold the message.  
	  // This buffer will be deleted by CtbConnect::OnReceiveEvent when the event is processed.
      pbuffer = new BYTE[ nPacketLen ];
	  if (pbuffer == NULL)
	  {
		  CString csTemp;
		  ATLTRACE("failed to allocate receive buffer. Closing socket.");
		  break;
	  }
      pHdr= (tRouteHeader *)pbuffer;

      cnt = ptbConn->m_ReadSocket.ReceiveCount( pHdr, nPacketLen);
		if (cnt < (int) sizeof(tRouteHeader))
		{
			delete[] pbuffer;
			break;
		}

      //check to see if it is an Ack request start
		if(pHdr->nLen == sizeof(tRouteHeader)) {
//>>>TODO
// trigger AckRequested event
      // continue;
		}

      if (cnt == SOCKET_ERROR)
         ATLASSERT(false);
      ptbConn->PostMessage(WM_RECEIVE_EVENT, (WPARAM)pHdr,(LPARAM)cnt);

//        ptbConn->_ItbConnectEvents_Receive();
   }
//   CoUninitialize();
   return 0;
}

LRESULT CtbConnect::OnReceiveEvent(UINT uMsg, WPARAM pData, LPARAM cb, BOOL& bHandled)
{
   tRouteHeader *pHdr   = (tRouteHeader *)pData;
   BYTE         *pbuffer= (BYTE *)pData;
   bHandled=true;
   
   m_critSectReceiveEvent.Lock();
   CComPtr<ITBStream> pStream;
	if ((pHdr->nVersion == VERSION_GENOBJ_BIN) || (pHdr->nVersion == VERSION_GENOBJ_XML)) 
	{

		//PARSE_AND_NOTIFY(TBGenObj);
		HRESULT hr = CoCreateInstance(__uuidof(CTBGenObj), NULL, CLSCTX_INPROC_SERVER, __uuidof(ITBStream), (void**) &pStream); \
		if (pStream)
		{
			pStream->ParseBuffer(pbuffer);
			ITBGenObj *pInterface;
			pStream->QueryInterface(__uuidof(ITBGenObj), (void**) &pInterface);
			if( m_pfnEventCallbackProc )
			{
				IDispatchPtr spDisp = pInterface;
				m_pfnEventCallbackProc(ReceiveTBGenObjEvent, spDisp);
				spDisp = NULL;
			}
			_ItbConnectEvents_ReceiveTBGenObj(pInterface);
			pInterface->Release();
			pStream.Release();
		}
	}
	else {
		switch(pHdr->messageID) {
		case TBMSG_ITEM:
			PARSE_AND_NOTIFY(ItemObj);
			break;
		case TBMSG_SCANNER:
			PARSE_AND_NOTIFY(ScannerObj);
			break;
		case TBMSG_CASH_DRAWER:
			PARSE_AND_NOTIFY(CashDrawerObj);
			break;
		case TBMSG_TOTAL:
			PARSE_AND_NOTIFY(TotalObj);
			break;
		case TBMSG_TENDER:
			PARSE_AND_NOTIFY(TenderObj);
			break;
		case TBMSG_REWARD:
			PARSE_AND_NOTIFY(RewardObj);
			break;
		case TBMSG_KEYBOARD:
			PARSE_AND_NOTIFY(KeyboardObj);
			break;
		case TBMSG_KEYLOCK:
			PARSE_AND_NOTIFY(KeylockObj);
			break;
		case TBMSG_MSR:
			PARSE_AND_NOTIFY(MSRObj);
			break;
		case TBMSG_MICR:
			PARSE_AND_NOTIFY(MICRObj);
			break;
		case TBMSG_PRINTER:
			PARSE_AND_NOTIFY(PrinterObj);
			break;
		case TBMSG_PINPAD:
			PARSE_AND_NOTIFY(PinpadObj);
			break;
		case TBMSG_SCALE:
			PARSE_AND_NOTIFY(ScaleObj);
			break;
		case TBMSG_CUSTOM:
			PARSE_AND_NOTIFY(CustomObj);
			break;

		default:
			break;
		}
	}
   m_critSectReceiveEvent.Unlock();

   // This buffer is always dynamically created in CtbConnect::ReceiveThreadFn
   delete [] pbuffer;

   return 0;
}

STDMETHODIMP CtbConnect::SetAck(LONG nAck)
{
	m_bAck= (nAck != 0);
	return S_OK;
}

STDMETHODIMP CtbConnect::SubscribeTo(BSTR szCommaDelimHexDeviceIds)
{
	m_csDeviceIDs= szCommaDelimHexDeviceIds;
	return S_OK;
}

STDMETHODIMP CtbConnect::Disconnect(void)
{
	m_WriteSocket.Close();
	m_ReadSocket.Close();
	return S_OK;
}

STDMETHODIMP CtbConnect::SetCallback(VARIANT vCallbackProc)
{
    _ASSERT( (V_VT(&vCallbackProc) & VT_UI4) || (V_VT(&vCallbackProc) & VT_UI8) );
	
	m_pfnEventCallbackProc = (LPEVENTCALLBACKPROC)vCallbackProc.ullVal;
	
    return S_OK;
}

