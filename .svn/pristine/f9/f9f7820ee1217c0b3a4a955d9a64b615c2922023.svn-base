// SocketForTBDevice.cpp : implementation file
//

#include "stdafx.h"
#include "AFXPRIV.H"

#define T_ID _T("SocketForTBDevice")
#define TM_MODULE TM_MODULE_4
#define __SOCKETFORTB

#include "SocketForTBDevice.h"
#include "NCRTBDevice.h"
#include "TBDevice.h"
#include "TBService.h"
#include "TBOposIO.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _MAX_NAME_LEN 16
#define _MAX_NAME_LEN2 32		//will accept 31 characters including "," for separation


static CCriticalSection socketSendCriticSec;
extern _globalInfo g_info;
extern CTBDevice* pdeviceObjs[256];

long WINAPI HandleReceive(LPVOID lparam);

								//list devices in hex no leading "0x"
/////////////////////////////////////////////////////////////////////////////
// CSocketForTBDevice

int CSocketForTBDevice::m_nHeaderSpecVersion=0;
bool CSocketForTBDevice::m_bKeepSocketsOpen=false;

CSocketForTBDevice::CSocketForTBDevice()
{
    m_hSocket = INVALID_SOCKET;
//Get/Set registry values
	HKEY	hKey;
//	DWORD	PortNumber = 0;
	long	rc;
	DWORD disposition;
	LPCTSTR szAddressValue = _T("loopback");
	LPCTSTR szDeviceIDs = _T("10,30,3A,40,4A,54");
	LPCTSTR szAckNoAck = _T("1");
	LPCTSTR szTimeForPOStoProcessKey = _T("100");

//Registry access constants and default values
	const	HKEY		NCRTBDEVICE_KEY = HKEY_LOCAL_MACHINE;
	LPCTSTR NCRTBDEVICE_SUBKEY = _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB\\PipeServer");
	LPCTSTR NCRTBDEVICE_SUBKEY_2 = _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTTB\\SocketServerInfo");
	LPCTSTR	NCRTBDEVICE_SERVER_ADDRESS_VALUE = _T("ServerAddress");
	LPCTSTR NCRTBDEVICE_RECIEVE_FROM_DEVICEID = _T("ListenToDevices");
	LPCTSTR	NCRTBDEVICE_READ_PORT_VALUE = _T("SocketReadPort");
	LPCTSTR NCRTBDEVICE_ACKNOWLDGE_CAPABLE = _T("AckNoAck");
	LPCTSTR NCRTBDEVICE_TIMEFORPOSTOPROCESS_KEY = _T("TimeForPOStoProcessKey");

//Open the subkey (or create it if it does not exist)
	rc = RegCreateKeyEx( NCRTBDEVICE_KEY,    // handle of main key
		                   NCRTBDEVICE_SUBKEY_2, // addr of subkey name
		                   0,                      // reserved
		                   0,                      // no type for subkey value?
		                   REG_OPTION_NON_VOLATILE,    // will be saved across system reboots
		                   KEY_ALL_ACCESS,         // desired security access
		                   NULL,                   // default security, don't care
		                   &hKey,                  // subkey handle is returned
		                   &disposition );         // returns either REG_CREATED_NEW_KEY or
	                                             // REG_OPENED_EXISTING_KEY
	ASSERT(rc==ERROR_SUCCESS);

  // Treat the case where the subkey did not exist as if the value
  // were 0.  Set it to 5050
  if (disposition == REG_CREATED_NEW_KEY)
  {
	rc = RegSetValueEx( hKey,                  // subkey handle from creation/opening
			                  NCRTBDEVICE_SERVER_ADDRESS_VALUE, // addr of value name
			                  0,                     // reserved
			                  REG_SZ,             // value type
			                  (UCHAR *)szAddressValue,
			                  _tcslen(szAddressValue) * sizeof(TCHAR) + sizeof(TCHAR));	 // data length
	ASSERT(rc==ERROR_SUCCESS);

	rc = RegSetValueEx( hKey,                  // subkey handle from creation/opening
			                  NCRTBDEVICE_RECIEVE_FROM_DEVICEID, // addr of value name
			                  0,                     // reserved
			                  REG_SZ,             // value type
			                  (UCHAR *)szDeviceIDs, // must cast to this type
			                  _tcslen(szDeviceIDs) * sizeof(TCHAR) + sizeof(TCHAR));	 // data length
	ASSERT(rc==ERROR_SUCCESS);

	rc = RegSetValueEx( hKey,                  // subkey handle from creation/opening
			                  NCRTBDEVICE_ACKNOWLDGE_CAPABLE, // addr of value name
			                  0,                     // reserved
			                  REG_SZ,             // value type
			                  (UCHAR *)szAckNoAck, // must cast to this type
			                  _tcslen(szAckNoAck) * sizeof(TCHAR) + sizeof(TCHAR));	 // data length
	ASSERT(rc==ERROR_SUCCESS);

	rc = RegSetValueEx( hKey,                  // subkey handle from creation/opening
			                  NCRTBDEVICE_TIMEFORPOSTOPROCESS_KEY, // addr of value name
			                  0,                     // reserved
			                  REG_SZ,             // value type
			                  (UCHAR *)szTimeForPOStoProcessKey, // must cast to this type
			                  _tcslen(szTimeForPOStoProcessKey) * sizeof(TCHAR) + sizeof(TCHAR));	 // data length
	ASSERT(rc==ERROR_SUCCESS);

  }

	CString  csDefaultAddress = _T("ServerAddress");
	TCHAR szServerName[_MAX_NAME_LEN + 1];
	DWORD dwDataSize = sizeof(szServerName);
	rc = RegQueryValueEx(hKey,csDefaultAddress,NULL,NULL, (LPBYTE) szServerName, &dwDataSize);
	ASSERT(rc==ERROR_SUCCESS);
	m_csServerName = szServerName;

	CString  csDefaultDeviceIDs = _T("ListenToDevices");
	TCHAR szDeviceIDs2[_MAX_NAME_LEN2 + 1];
	dwDataSize = sizeof(szDeviceIDs2);
	rc = RegQueryValueEx(hKey,csDefaultDeviceIDs,NULL,NULL, (LPBYTE) szDeviceIDs2, &dwDataSize);
	ASSERT(rc==ERROR_SUCCESS);
	m_csDeviceIDs = szDeviceIDs2;

	CString csAckNoAck = _T("AckNoAck");
	TCHAR caAckNoAck[2];
	dwDataSize = sizeof(caAckNoAck);
	rc = RegQueryValueEx(hKey,csAckNoAck,NULL,NULL, (LPBYTE) caAckNoAck, &dwDataSize);
	ASSERT(rc==ERROR_SUCCESS);
	m_bAck = (caAckNoAck[0] == '1');

	CString csTimeForPOStoProcessKey = _T("TimeForPOStoProcessKey");
    TCHAR caTimeForPOStoProcessKey[_MAX_NAME_LEN2 + 1]={0};
	dwDataSize = sizeof(caTimeForPOStoProcessKey);
	rc = RegQueryValueEx(hKey,csTimeForPOStoProcessKey,NULL,NULL, (LPBYTE) caTimeForPOStoProcessKey, &dwDataSize);
	ASSERT(rc==ERROR_SUCCESS);
	CString csTemp = caTimeForPOStoProcessKey;
	if(csTemp.GetLength())
		m_iTimeForPOStoProcessKey = _ttoi(csTemp);
    else 
		m_iTimeForPOStoProcessKey = _ttoi(szTimeForPOStoProcessKey);

	rc = RegCloseKey(hKey);
	ASSERT(rc==ERROR_SUCCESS);

	//Open the subkey (or create it if it does not exist)
	rc = RegCreateKeyEx( NCRTBDEVICE_KEY,    // handle of main key
		                   NCRTBDEVICE_SUBKEY, // addr of subkey name
		                   0,                      // reserved
		                   0,                      // no type for subkey value?
		                   REG_OPTION_NON_VOLATILE,    // will be saved across system reboots
		                   KEY_ALL_ACCESS,         // desired security access
		                   NULL,                   // default security, don't care
		                   &hKey,                  // subkey handle is returned
		                   &disposition );         // returns either REG_CREATED_NEW_KEY or
	                                             // REG_OPENED_EXISTING_KEY

  if (rc != ERROR_SUCCESS) 
	//replace previous line with the following
	ASSERT(rc==ERROR_SUCCESS);
  //Get Server Address and Port Number values from registry

  if (disposition != REG_CREATED_NEW_KEY)
  {
	TCHAR szTemp[_MAX_NAME_LEN + 1];
	
	dwDataSize=sizeof(szTemp);
	rc = RegQueryValueEx(hKey,_T("SocketReadPort"),NULL,NULL, (LPBYTE) szTemp, &dwDataSize);
	ASSERT(rc==ERROR_SUCCESS);
	m_csPortNumber = szTemp;

	dwDataSize=sizeof(szTemp);
	rc = RegQueryValueEx(hKey,_T("SocketWritePort"),NULL,NULL, (LPBYTE) szTemp, &dwDataSize);
	ASSERT(rc==ERROR_SUCCESS);
	m_csPortNumberWrite = szTemp; 

	dwDataSize=sizeof(szTemp);
	rc = RegQueryValueEx(hKey,_T("HeaderSpecVersion"), NULL,NULL, (LPBYTE) szTemp, &dwDataSize);
	ASSERT(rc==ERROR_SUCCESS);
	m_nHeaderSpecVersion = _ttoi(szTemp);

  	dwDataSize=sizeof(szTemp);
	rc = RegQueryValueEx(hKey,_T("KeepSocketsOpen"), NULL,NULL, (LPBYTE) szTemp, &dwDataSize);
	if (rc==ERROR_SUCCESS)
	   m_bKeepSocketsOpen = (_ttoi(szTemp) != 0);

	rc = RegCloseKey(hKey);
	ASSERT(rc==ERROR_SUCCESS);
  }

  m_TBCountForKeyboardDataLeft = 0;

}

CSocketForTBDevice::~CSocketForTBDevice()
{
}


void CSocketForTBDevice::StartReadThread()
{
   DWORD dwBlocking= 0;
   ioctlsocket(m_hSocket, FIONBIO, &dwBlocking);
   g_info.g_hReceiveThread= ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)HandleReceive,
        							 (LPVOID) this,0,0);
}

long WINAPI HandleReceive(LPVOID lparam)
{
   /***
   Do not use DataCapture in this method.  It is not always initialized by
   this time and can cause a crash
   ***/
   CTraceSystem ts(_T("c:\\NCRTBDevice.log"), TM_MODULE);
   CSocketForTBDevice *pSocket= (CSocketForTBDevice *)lparam;
	BYTE pBuf[MAXIMUM_BUFFER_SIZE] = {0};
	int nPacketLen = 0;
    tSpecHeaderV01 specHdr;
   int nErrorCount=0;
   CoInitialize(NULL); // required for this new thread to create a Tracewriter obj 

   while(1)
	{
        // Blocking call will not return until data is ready
        int dwByteLen = pSocket->Receive((BYTE*)&specHdr, sizeof(tSpecHeaderV01), MSG_PEEK);

        if ( (SOCKET_ERROR == dwByteLen) || (0 == dwByteLen ) )
        {
            // abort reading on error or close
            if (SOCKET_ERROR == dwByteLen)
            {
                ts.Trace(TM_ERROR, _T("***Receive Socket Error, exiting receive!"));
            }
            break;
        }
        nPacketLen = specHdr.nLen;

        // Blocking call will not return until data is ready
        dwByteLen = pSocket->Receive( (BYTE*)pBuf, nPacketLen, 0 );
        if ( (SOCKET_ERROR == dwByteLen) || (0 == dwByteLen ) )
        {
            // abort reading on error or close
            if (SOCKET_ERROR == dwByteLen)
            {
                ts.Trace(TM_ERROR, _T("***Receive Socket Error, exiting receive!"));
            }
            break;
        }

		tSpecHeaderV01 *Temp = (tSpecHeaderV01 *)pBuf;

// NO LOGGING possible credit card numbers for HD
//         char szTmp[80];
//         sprintf(szTmp, "Received Data (Thread:%d)", GetCurrentThreadId());
////			ts.HexTrace(TM_DEBUG,szTmp, (UCHAR *)Temp, (ULONG)dwByteLen);

        //check to see if it is an Ack request start
		if(Temp->nLen == sizeof(tSpecHeaderV01)) {
		    if(Temp->messageID != TBMSG_KEYBOARD) {
            // non keyboard msg: acknowledge immediately
				SetEvent(g_info.hConsumed); 
				pSocket->HandleAck(pBuf, ts);
				continue;
			}
			else {
                // keyboard msg: wait for keyboard to consume previous keystrokes
				pSocket->HandleAck(pBuf, ts);
				continue;
			}
		}
		BYTE *ptr=(BYTE*)&Temp[1];

        int msgId;
        if (Temp->messageID == TBMSG_MICR2)
            msgId= TBMSG_MICR;
        else
            msgId= Temp->messageID;
        if (pdeviceObjs[msgId]) {
            if (CSocketForTBDevice::GetSpecVersion() > 1) // include the header if ver 2+
                pdeviceObjs[msgId]->ProcessRawData(pBuf, dwByteLen);
            else // strip the header out of the msg for ver 1.
                pdeviceObjs[msgId]->ProcessRawData(ptr, dwByteLen - sizeof(tSpecHeaderV01));
        } else
   		    ts.Trace(TM_ERROR,_T("**Message for Unknown or not created device:0x%x"), msgId);
    }
    ts.Trace(TM_INFO, _T("***Receive Thread End!***\n"));
    CoUninitialize();
    return 0;
}

void CSocketForTBDevice::HandleAck(BYTE *AckToSend, CTraceSystem& ts)
{
    ts.Trace(TM_ENTRY, _T("+HandleAck"));
    if (m_bAck) 
    {
        DWORD dwWait;
        ts.Trace(TM_INFO, _T("Waiting for g_info.hConsumed"));
        dwWait = WaitForSingleObject( g_info.hConsumed, 30000 ); //wait for POS to get last keystroke or 30 seconds
        if ( dwWait == WAIT_OBJECT_0 ) 
        {    
            tSpecHeaderV01 *Temp = (tSpecHeaderV01 *)AckToSend;
            ts.Trace(TM_INFO, _T("Waiting for time remaining %d"), g_info.TimerForSO.Remaining() );
            Sleep(g_info.TimerForSO.Remaining());
            ts.Trace(TM_INFO, _T("sending Ack") );
            TBSend((BYTE *)Temp, sizeof(tSpecHeaderV01));
        }
    }
    ts.Trace(TM_ENTRY, _T("-HandleAck"));
}

int CSocketForTBDevice::TBSend(const void *pBuf, int iLength)
{
   CSingleLock sl( &socketSendCriticSec, false );
   sl.Lock();                        //enter critical section
   int cb = send(m_hSocket, (LPSTR)pBuf, iLength, 0);
   sl.Unlock();                      //exit critical section
   return cb;
}


int CSocketForTBDevice::GetSpecVersion()
{
   return m_nHeaderSpecVersion;
}

bool CSocketForTBDevice::GetKeepSocketsOpen()
{
   return m_bKeepSocketsOpen;
}

//int CSocketForTBDevice::Receive(BYTE *buf, int len, int flags )
//{
//    return recv(m_hSocket, (char*)buf, len, flags);
//}

int CSocketForTBDevice::Receive(BYTE* lpBuf, int nCount, int nFlags )
{
   /*
   **  A read() from a socket could return without reading all the bytes.
   **  This function retries until all bytes are read.
   */
	int nRead;
	int nLeft = nCount;
	PBYTE pBuf = (PBYTE)lpBuf;
	while(nLeft > 0) {
		nRead = recv((SOCKET)m_hSocket, (char *)pBuf, nLeft, nFlags);
		int ntries=0;
		while ( (nRead == SOCKET_ERROR) && 
			    ((WSAGetLastError() == WSAEWOULDBLOCK) ||
			     (WSAGetLastError() == WSAEINPROGRESS)) &&
				 (ntries++ <100)
			  ) 
		{
			Sleep(80); // try again
			nRead = recv((SOCKET)m_hSocket, (char *)pBuf, nLeft, nFlags);
		}
		if (nRead == SOCKET_ERROR){
			return WSAGetLastError()*-1;
		} else if (nRead == 0) {
			return nCount - nLeft;
		}
        if (nFlags & MSG_PEEK) {    // we re-read entire buffer if MSG_PEEK
            if (nRead == nCount)
                nLeft=0; //done
        }
        else {
		    nLeft -= nRead;
		    pBuf += nRead;
        }
	}
	nRead= nCount - nLeft;
    return nRead;
}

//
// Close
// Derived from MFC CAsyncSocket source
//
void CSocketForTBDevice::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		VERIFY(SOCKET_ERROR != closesocket(m_hSocket));
		m_hSocket = INVALID_SOCKET;
	}
}

//
// GetLastError
// Derived from MFC CAsyncSocket source
//
int CSocketForTBDevice::GetLastError()
{ 
    return WSAGetLastError(); 
}

//
// Create
// Derived from MFC CAsyncSocket source
//
BOOL CSocketForTBDevice::Create(UINT nSocketPort, int nSocketType,
	LPCTSTR lpszSocketAddress)
{
	if (Socket(nSocketType))
	{
		if (Bind(nSocketPort,lpszSocketAddress))
			return TRUE;
		int nResult = GetLastError();
		Close();
		WSASetLastError(nResult);
	}
	return FALSE;
}

//
// Socket
// Derived from MFC CAsyncSocket source
//
BOOL CSocketForTBDevice::Socket(int nSocketType, int nProtocolType, int nAddressFormat)
{
	ASSERT(m_hSocket == INVALID_SOCKET);

	m_hSocket = socket(nAddressFormat,nSocketType,nProtocolType);
	if (m_hSocket != INVALID_SOCKET)
	{
		return TRUE;
	}
	return FALSE;
}


//
// Bind
// Derived from MFC CAsyncSocket source
//
BOOL CSocketForTBDevice::Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress)
{
	USES_CONVERSION;

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = T2A((LPTSTR)lpszSocketAddress);
	sockAddr.sin_family = AF_INET;

	if (lpszAscii == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		DWORD lResult = inet_addr(lpszAscii);
		if (lResult == INADDR_NONE)
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
		sockAddr.sin_addr.s_addr = lResult;
	}

	sockAddr.sin_port = htons((u_short)nSocketPort);

	return Bind((SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

//
// Bind
// Derived from MFC CAsyncSocket source
//
BOOL CSocketForTBDevice::Bind(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{ 
    return (SOCKET_ERROR != bind(m_hSocket, lpSockAddr, nSockAddrLen)); 
}

//
// IOCtl
// Derived from MFC CAsyncSocket source
//
BOOL CSocketForTBDevice::IOCtl(long lCommand, DWORD* lpArgument)
{
    return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument)); 
}

//
// Connect
// Derived from MFC CAsyncSocket source
//
BOOL CSocketForTBDevice::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	USES_CONVERSION;

	ASSERT(lpszHostAddress != NULL);

	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = T2A((LPTSTR)lpszHostAddress);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAscii);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	return Connect((SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

//
// Connect
// Derived from MFC CAsyncSocket source
//
BOOL CSocketForTBDevice::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{ 
    return connect(m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR; 
}

//
// Send
// Derived from MFC CAsyncSocket source
//
int CSocketForTBDevice::Send(const void* lpBuf, int nBufLen, int nFlags)
{
   char *ptr;
   int   n, wc=0;
   /*  A write() to a socket could return without writing all the bytes.
   **  This function retries until all bytes are written.
   */
   for(n = nBufLen, ptr = (char *)lpBuf ; n > 0 ; n -= wc, ptr += wc){
      if((wc = send((SOCKET)m_hSocket, ptr, n, 0)) == SOCKET_ERROR){ 
         int ntries=0;
         while ((wc == SOCKET_ERROR) && 
                (WSAGetLastError() == WSAEWOULDBLOCK) &&
     				 (ntries++ <100)) {
            Sleep(50); // try again
            wc = send((SOCKET)m_hSocket, ptr, n, nFlags);
         }
         if(wc == SOCKET_ERROR) {
            return -1;
         }
      }
      else if(wc <= 0) {
         return -1;
      }
   }
   return wc;
}
