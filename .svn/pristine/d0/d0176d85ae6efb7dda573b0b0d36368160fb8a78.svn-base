// CMal100901 : MAJOR UPDATE. almost same as Eric Kobres' version from RAP.
// Almost same as Qping.cpp from Core's VSS tree Core/Remote Approval/RAP
//
// EXCEPT: 
// 1) named qping2.cpp, calls quickping2, uses qping.h, and Traces have "SecPing" as 
// specific marker that message is from Security.  Also, added my comments from old ver.
// 2) QuickPing2 has a 5th parameter that allows PingByNameOnly
//
// Special note: that with this version the WSAStartup and WSACleanup happen in the
//               calling code.
// ----------------------------------------------------------------------------------
//
// Current known problems 
//
// 1. CMal071701 (Already TARed - 170822, 170856, 174873)
//    Assumes that the target will maintain the same IP Address, as it pings by Name
//    and if that's successful, then it will save the IP Addr, and then only ping
//    by IP Addr.  If the target machine is on DHCP and it goes up and down, this will
//    likely cause a problem because the IP Addr will not be guarenteed to be the same.
//    We did this fix because pinging by Name takes a VERY long time (30 sec) to timeout
//    but pinging by IP Addr is just a few seconds.  Microsoft is aware of this, but 
//    says that's the way that it is.
//
// ----------------------------------------------------------------------------------
//
// Fix History:
//
// A. CMal092799
//    Use Winsock2 instead of Winsock and Rewrite this section of QPing to 'quickly' try 
//    to get the information back from the Server.  And retry a few times if not
//    successful.
// B. CMal031400 
//    Winsock #define already in include file
// C. CMal041701
//    Ping by IP Address after Ping by Name is successful.  Known limitation specified
//    above is acceptable, because no customer is using DHCP.  Allows quicker recognition
//    of when Target machine is not available.
// D. CMal071801 - TAR 177768
//    QuickPing2 falsely returns failure because of CheckSum call based on date.  Dale
//    determined that this will fail one in every 65355 calls.  This call is not even
//    necessary, so Dale and I removed.  Also added the string "Security OCX - " 
//    to the front of all TRACE messages from Security's version of QuickPing.
// E. Eric Kobres rewrites QuickPing, and I adopt as Quickping2 almost as is.  See
//    small writeup at very top for actual changes.  CMal100901 and CMal101001
// F. 20050829 - QuickPing2() rewritten to not use raw sockets.  Change required for
//    SCOT to run without administrator privileges.  Compare to version 2 in 
//	  4.0 Patch C branch to see changes.  Added QPInitialize() - JV185012.
//    

// --------- 2.1.1 Patch E - very late version -----------
// CMal041701 : TAR fix 169653 - disconnect LAN in middle of trans causes slowdown
// --------- 2.2.0 ---------------------------------------
// CMal042301 : TAR fix 169865 - move TAR 169653 to 2.2
// --------- 2.3.0 -------------------------------------------------------------------
// CMal071801 : see above 'D' (same TAR as 177766 in 2.1.1 and 177767 in 2.2)
// ------------------------------------------------------------------------------------

#include "stdafx.h"
#include "common.h"
#include <winsock2.h>				// Winsock header file // CMal092799
#include "qping2.h"

#define NO_FLAGS 0					// No special flags specified
									// RFC 792 defines ICMP message values
#define ICMP_ECHO 8					// An ICMP echo message
#define ICMP_ECHOREPLY 0			// An ICMP echo reply message
#define	ICMP_HEADERSIZE 8			// ICMP header size ("echo messages" only)
#define ICMP_ID 0xECEC

#define MAX_SIZE  100
static int iCurrentArrayIndex = 0;
static WORD gwSequence = 0;

// For QuickPing2 - The following are for the port that QuickPing2() will try to connect to.
//  The default is 139, but this can be overridden using a registry setting (see QPInitialize()).
#define PORT_NETBIOS 139
#define CDBCLIENT_ROOT _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\ScotApp\\Controls\\ClientDBMgr\\")
DWORD gdwPingPort = PORT_NETBIOS;
DWORD gdwUseNewQuickPingMethod = 0;  // Default to the old tried-and-true ping method.

#define COMP_ID ID_SM
#define T_ID _T("SigC")


struct HostName 
{
	CString csName;
	SOCKADDR_IN  client;
};

HostName ArrayOfHost[MAX_SIZE];     // Store host IP address for each machine.

struct ip 												// Structure for IP datagram header
	{
		BYTE ip_verlen;								// Version and header length
		BYTE ip_tos;								// Type of service
		WORD ip_len;								// Total packet length 
		UINT ip_id;									// Datagram identification 
		WORD ip_fragoff;							// Fragment offset 
		BYTE ip_ttl;								// Time to live 
		BYTE ip_proto;								// Protocol
		UINT ip_chksum;								// Checksum 
		IN_ADDR ip_src_addr;						// Source address 
		IN_ADDR ip_dst_addr;						// Destination address 
		BYTE ip_data[1];							// Variable length data area
	};
			
struct icmp												// Structure for an ICMP header
	{
		BYTE icmp_type;								// Type of message
		BYTE icmp_code;								// Type "sub code" (zero for echos)
		WORD icmp_cksum;							// 1's complement checksum
		WORD icmp_id;								// Unique ID (the instance handle)
		WORD icmp_seq;								// Tracks multiple pings
		BYTE icmp_data[1];							// The start of optional data
	};

WORD Chksum(LPWORD lpwIcmpData, WORD wDataLength)
{
	long	lSum;				// Store the summation
	WORD	wOddByte;		// Left over byte from the summation
	WORD	wAnswer;		// The 1's complement checksum
		
	lSum = 0L;

	while (wDataLength > 1)
	{
		lSum += *lpwIcmpData++;
		wDataLength -= 2;
	}

	// Handle the odd byte if necessary and make sure the top half is zero
	if (wDataLength == 1)
	{
		wOddByte = 0;
		*((LPBYTE) &wOddByte) = *(LPBYTE)lpwIcmpData;	// One byte only
		lSum += wOddByte;
	}

	// Add back the carry outs from the 16 bits to the low 16 bits
	lSum = (lSum >> 16) + (lSum & 0xffff);	// Add high-16 to low-16
	lSum += (lSum >> 16);										// Add carry
	wAnswer = (WORD)~lSum;									// 1's complement, then truncate 
																						// to 16 bits
	return(wAnswer);
}

char* ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn, int* sizeout)
{
  LPSTR pszOut = NULL;
  *sizeout=0;
  if (lpwszStrIn != NULL)
  {
	int nInputStrLen = wcslen (lpwszStrIn);

	// Double NULL Termination
	int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
	pszOut = new char [nOutputStrLen];

	if (pszOut)
	{
	  memset (pszOut, 0x00, nOutputStrLen);
	  WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
	}
    *sizeout=nOutputStrLen-2;
  }
  return pszOut;
}

BOOL GetQuickAddress(LPCTSTR lpszHostName, SOCKADDR_IN &sockAddrLocal)
{
	LPHOSTENT lpHostEntry;				// Internet host data structure
	sockAddrLocal.sin_family = AF_INET;

    CString csHostName = lpszHostName;
    int nDataLen = csHostName.GetLength();

    //need to convert to ANSI
    int myANSIDataSize = 0;
    LPTSTR myp = csHostName.GetBuffer(nDataLen);
#ifdef UNICODE
    char *myANSIdata = ConvertLPWSTRToLPSTR(myp,&myANSIDataSize);
#else  
    char *myANSIdata = new char[nDataLen+1];
    myANSIDataSize = nDataLen;
    strcpy(myANSIdata, myp);
#endif

	if(iCurrentArrayIndex == 0)
	{
		// extract the information from the Name/IP Addr
		// add to current index the new info	
		ArrayOfHost[iCurrentArrayIndex].csName = lpszHostName;

		if (inet_addr(myANSIdata) == INADDR_NONE) // Check of IP addr - not name
		{	  
			if ((lpHostEntry = gethostbyname(myANSIdata)) == NULL)
			{
				TRACE(_T("SecPing - Could not get %s IP address. Error code: %d\n"), (LPTSTR)lpszHostName, WSAGetLastError());

				delete [] myANSIdata;

				return(FALSE);
			}

			ArrayOfHost[iCurrentArrayIndex].client.sin_addr = *((LPIN_ADDR) *lpHostEntry->h_addr_list);
			sockAddrLocal.sin_addr = *((LPIN_ADDR) *lpHostEntry->h_addr_list);
		}
		else  // We got an IP addr, not a name.  So just assign w/o looking up.
		{
			IN_ADDR tmp;
			tmp.s_addr = inet_addr(myANSIdata);
			ArrayOfHost[iCurrentArrayIndex].client.sin_addr = tmp;
			sockAddrLocal.sin_addr = tmp;
		}

		iCurrentArrayIndex++; // =1
	}
	else
	{
		BOOL bFoundMatch = FALSE;
		// Maybe the Machine already exists, if Yes, then use the Addr
		// otherwise, add the new entry into the Array and find out the information
	  
		// STEP 1: // looking for existing entry in Array
		for(int i = 0; i < iCurrentArrayIndex; i++)
		{
			if (ArrayOfHost[i].csName == lpszHostName)
			{
				// we've found the match
				sockAddrLocal.sin_addr = ArrayOfHost[i].client.sin_addr; // Used
				bFoundMatch = TRUE;
				break; 
			}
			// else - just keep looking
		}

		// STEP 2: If match NOT found, then Add this one to the array
		if (!bFoundMatch)
		{
			// add to current index the new info	
			ArrayOfHost[iCurrentArrayIndex].csName = lpszHostName;

			if (inet_addr(myANSIdata) == INADDR_NONE) // Check of IP addr - not name
			{	  
				if ((lpHostEntry = gethostbyname(myANSIdata)) == NULL)
				{
					TRACE(_T("SecPing - Could not get %s IP address. Error code: %d\n"), (LPTSTR)lpszHostName, WSAGetLastError());
					delete [] myANSIdata;
					return(FALSE);
				}

				ArrayOfHost[iCurrentArrayIndex].client.sin_addr = *((LPIN_ADDR) *lpHostEntry->h_addr_list);
				sockAddrLocal.sin_addr = *((LPIN_ADDR) *lpHostEntry->h_addr_list); // Used
			}
			else  // We got an IP addr, not a name.  So just assign w/o looking up.
			{
				IN_ADDR tmp;
				tmp.s_addr = inet_addr(myANSIdata);
				ArrayOfHost[iCurrentArrayIndex].client.sin_addr = tmp;
				sockAddrLocal.sin_addr = tmp; // Used
			}

			// finally increment for next new entry
			if (iCurrentArrayIndex < MAX_SIZE)
				iCurrentArrayIndex++;
			else
			{
				// show outside of array index error
				TRACE(_T("SecPing - Beyond array index in QPing. Error code: %d\n"), WSAGetLastError());
				delete [] myANSIdata;
                return(FALSE);
			}
		}
	}

    delete [] myANSIdata;
	return TRUE;
}
/**
 * Function:		QPInitialize
 * Purpose:			Queries the registry for an alternate port number that QuickPing2 is 
 *					to use.  If no value is located, then a default of PORT_NETBIOS is used.
 *					Registry location is CDBCLIENT_ROOT.  The registry value is PingPort.
 *
 *					Queries the registry for method of ping to use.  If in non-admin mode, the
 *					new method is used.  Otherwise default to the old method.
 *					The registry value is NonAdminQuickPing.
 *
 *					Called by CClientDBMgrCtrl::InitInstance().
 *					
 * Parameters:		None.
 * Returns:			NA
 * Calls:
 * Side Effects:	Alters the global variables gdwPingPort and gdwUseNewQuickPingMethod.
 * Notes:			This functionality is added for flexibility in case QuickPing2() is 
 *					needed in the future and PORT_NETBIOS is not available.
 */
void QPInitialize(void)
{
	long lResult;
	CString csSubKey = CDBCLIENT_ROOT;
	CString csTrace;
	HKEY hKey = NULL;
	
	DWORD dwType	= REG_DWORD;
	DWORD dwSize	= sizeof(DWORD);

	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, csSubKey, 0, KEY_QUERY_VALUE, &hKey);

	if(lResult == ERROR_SUCCESS)
	{
        trace(L6,_T("QPInitialize() - Got access to registry."));
		// Query registry to see which ping method to use.  If non-admin is enabled, then
		// the new ping method must be used.
		lResult = RegQueryValueEx(hKey, _T("NonAdminQuickPing"), 0, &dwType, (BYTE *)&gdwUseNewQuickPingMethod, &dwSize);
		if(lResult != ERROR_SUCCESS)
		{
            trace(L6,_T("QPInitialize() - Using old method to ping."));
			gdwUseNewQuickPingMethod = 0;
		}

		// Get alternate port for QuickPing2 to utilize - if specified.
		if(gdwUseNewQuickPingMethod == 1)
		{
			lResult = RegQueryValueEx(hKey, _T("PingPort"), 0, &dwType, (BYTE *)&gdwPingPort, &dwSize);
			if(lResult != ERROR_SUCCESS)
			{
                trace(L6,_T("QPInitialize() - Did not find value for PingPort in registry."));
				gdwPingPort = PORT_NETBIOS;
			}
		}
	}

	RegCloseKey(hKey);

	csTrace.Format(_T("QPInitialize() - gdwUseNewQuickPingMethod = %d"), gdwUseNewQuickPingMethod);
    trace(L6,_T("%s"),csTrace);

	if(gdwUseNewQuickPingMethod == 1)
	{
		csTrace.Format(_T("QPInitialize() - Utilizing port %d for QuickPing2()"), gdwPingPort);
        trace(L6,_T("%s"),csTrace);
	}
}

/**
 * The old method of pinging - requires administrator privileges.
 * See QuickPing2().
 * This function will be used instead of the new method (QuickPingTCP) 
 * unless a registry setting is specified.
 * (see QPInitialize()).
 */
BOOL OldQuickPing2(CString csMachineName, long PingRetry, long PingTDStart, long PingTDIncr, bool bByNameOnly)
{
	BOOL rc;
	int iPacketSize;					// ICMP packet size 
	int iHostAddrLength;				// Host address length
	int iIPHeadLength;					// IP datagram header length
	int iReceivedBytes;					// Number of bytes received
	int iSentBytes;						// Number of bytes sent
	int nProtocol;						// ICMP protocol number
	int iSocketError;					// Stores any error codes
	PDWORD pdwTimeStamp;				// Tick count at transmission
	DWORD dwReturnTime;					// Tick count upon receipt of echo reply
	DWORD dwRoundTrip;					// Tick count for the round-trip
																	// Structures defined in WINSOCK.H
	SOCKET hSocket;						// Socket handle (or descriptor)
	LPPROTOENT lpProtocolEntry;			// Internet protocol data structure

	BYTE IcmpSendPacket[1024];			// Buffer space for data to send
	BYTE IcmpRecvPacket[4096];			// Buffer space for received data 
	SOCKADDR_IN	sockAddrHost;			// Socket address structure for host addr
	SOCKADDR_IN	sockAddrLocal;			// Socket address structure for local addr

	struct icmp *pIcmpHeader;			// A pointer to the ICMP structure
	struct ip *pIpHeader;				// A pointer to the IP header structure
	int iVal = PingTDStart;
	fd_set fdsRead;						// READ socket FDS
	fd_set fdsWrite;					// WRITE socket FDS
	timeval timeout;					// timeout for use with select()


	/* --------------------------------------------------------------------- */
	/* CMal101001 if special registry entry exists, then always ping by Name */
	/*            Do this by clearing out the structure that GetQuickAddress */
	/*            uses.  Told by the new 5th param to this call.             */
	/* --------------------------------------------------------------------- */
	if (bByNameOnly == TRUE)
	{
		IN_ADDR SockInEmpty;
        memset((void *)&SockInEmpty,0,sizeof(IN_ADDR));

		for(int ij = 0; ij < iCurrentArrayIndex; ij++)
		{
			// not very important to clear these, since putting the count of
			// things in this array to 0 in a second.  Do something to show intent.
			ArrayOfHost[ij].csName = _T("");
			// even thou not Init, has 0.0.0.0 for an IP address (seen in debugger)
			ArrayOfHost[ij].client.sin_addr = SockInEmpty; 
		}
		iCurrentArrayIndex = 0; // most important step
	}

	ZeroMemory(IcmpSendPacket, sizeof(IcmpSendPacket));
	ZeroMemory(IcmpRecvPacket, sizeof(IcmpRecvPacket));
	pIcmpHeader = (struct icmp *) IcmpSendPacket;			// Point at the data area
	pIcmpHeader->icmp_type = ICMP_ECHO;						// then fill in the data.
	pIcmpHeader->icmp_code = 0;								// Use the Sockman instance 
	pIcmpHeader->icmp_id = LOWORD(GetCurrentProcessId());	// Unique ID.

	if (!GetQuickAddress(csMachineName, sockAddrLocal))
		return FALSE;

	iHostAddrLength = sizeof(SOCKADDR_IN);

	// With a raw socket, the program must specify a protocol
	if ((lpProtocolEntry = getprotobyname("icmp")) == NULL)
		nProtocol = IPPROTO_ICMP;
	else
		nProtocol = lpProtocolEntry->p_proto;
	
	/* SET UP THE SOCKET ------------------------------------------- */
	/* Create a "raw" socket and specify ICMP as the protocol to use */
	/* ------------------------------------------------------------- */

	if ((hSocket = WSASocket(PF_INET, SOCK_RAW, nProtocol, NULL, 0, 0)) == INVALID_SOCKET)
	{
		trace(L6,_T("SecPing - Could not create a RAW socket.\n"));
		return(FALSE);
	}

	// For each retry attempt a ping -- but we'll exit as soon as one works
	for (int i = 1; i <= PingRetry; i++, iVal += PingTDIncr)
	{
		//Put tick count in the optional data area
		pdwTimeStamp = (PDWORD)&IcmpSendPacket[ICMP_HEADERSIZE];
		*pdwTimeStamp = GetTickCount();
		iPacketSize = ICMP_HEADERSIZE + sizeof(DWORD);

		pIcmpHeader->icmp_cksum = 0;	// init the checksum to zero.

		// This may just be superstition, but I've heard some net stacks
		// don't like ICMP packets with a 0 sumcheck.  I have not read 
		// RFC 792 in a long time, so if you are reading this and feeling
		// advnturous, have a look at the RFC and feel free to take this out
		// of the while loop:
		while (pIcmpHeader->icmp_cksum == 0)
		{
			pIcmpHeader->icmp_seq = ++gwSequence;
			pIcmpHeader->icmp_cksum = Chksum((LPWORD)pIcmpHeader, iPacketSize);
		}

		// convert Milliseconds to select() timeout.
		timeout.tv_sec = iVal / 1000;
		timeout.tv_usec = (iVal % 1000) * 1000;

		// Setup up write FDS
        FD_ZERO(&fdsWrite);        
		FD_SET(hSocket, &fdsWrite);
		rc = select(0, NULL, &fdsWrite, NULL, &timeout);

		if (rc > 0)
			iSentBytes = sendto(hSocket, (LPSTR) IcmpSendPacket, iPacketSize, 
							NO_FLAGS, (LPSOCKADDR) &sockAddrLocal, sizeof(sockAddrLocal));
		else if (rc < 0)
			{
				closesocket(hSocket);
				trace(L6,_T("SecPing - The select() function returned a write socket error %d.\n"), rc);
				return(FALSE);
			}
		else
			continue; // select returns 0 on timeout...

//		TRACE("SecPing - QuickPing2(): sendto() completed iteration %d w/ %d bytes sent.\n", i, iSentBytes);
			
		if (iSentBytes != iPacketSize)
		{
			closesocket(hSocket);
			trace(L6,_T("SecPing - Wrong number of bytes sent: %d\n"), iSentBytes);
			return(FALSE);
		}		

		// Note:  There is a bug in Windows NT which precludes the use of
		// SO_RCVTIMEO on a raw socket.  For this reason we MUST USE select()
		// to avoid blocking forever on reads. -ECK
        FD_ZERO(&fdsRead);        
		FD_SET(hSocket, &fdsRead);
		rc = select(0, &fdsRead, NULL, NULL, &timeout);

		if (rc > 0)
			iReceivedBytes = recvfrom(hSocket, (LPSTR) IcmpRecvPacket, 
					sizeof(IcmpRecvPacket), NO_FLAGS, (LPSOCKADDR) &sockAddrHost,
					&iHostAddrLength);
		else if (rc < 0)
			{
				closesocket(hSocket);
				trace(L6,_T("SecPing - The select() function returned a read socket error %d.\n"), rc);
				return(FALSE);
			}
		else
			continue; // select returns 0 on timeout...

//		TRACE("SecPing - QuickPing2(): recvfrom() iteration %d completed w/ %d bytes received.\n", i, iReceivedBytes);

		// Point to the IP Header in the received packet
		pIpHeader = (struct ip *)IcmpRecvPacket;

		// Extract bits 4-7 and convert the number of 32-bit words to bytes
		iIPHeadLength = (pIpHeader->ip_verlen & 0x0F) * 4;  // get header length and times 4

		// Test the length to make sure an ICMP header was received
		if (iReceivedBytes < iIPHeadLength + ICMP_HEADERSIZE)
		{
			trace(L6,_T("SecPing - Received packet was too short."));
			continue;
		}

		// Point to the ICMP message which immediately follows the IP header
		pIcmpHeader = (struct icmp *) (IcmpRecvPacket + iIPHeadLength);

		// Make sure this is an ICMP "echo reply"
		if (pIcmpHeader->icmp_type != ICMP_ECHOREPLY)
		{
			trace(L6,_T("SecPing - Received packet was not an echo reply.\n"));
			continue;
		}

		// Make sure this program sent the packet
		if (pIcmpHeader->icmp_id != LOWORD(GetCurrentProcessId()))
		{
			trace(L6,_T("SecPing - Received packet ID mismatch: expected 0x%0.4X but got 0x%0.4X.\n"), 
				LOWORD(GetCurrentProcessId()), pIcmpHeader->icmp_id);
		}

		if (gwSequence - pIcmpHeader->icmp_seq > i )
		{
			trace(L6,_T("SecPing - Received packet was stale - expected %d to %d but got %d.\n"), 
				gwSequence - i, gwSequence , pIcmpHeader->icmp_seq);
		}

		break; // CMal092799 - if it worked, then don't try again
	} 

	closesocket(hSocket);

	if (rc < 0)
	{
		iSocketError = WSAGetLastError();
		trace(L6,_T("SecPing - Socket Error from select()(): %d\n"), iSocketError);
		return(FALSE);
	}
	else if (rc == 0)
	{   
		trace(L6,_T("SecPing - Ping operation for %s timed out.\n"), csMachineName);
		dwRoundTrip = 0;
		return(FALSE);
	}

	dwReturnTime = GetTickCount();
	dwRoundTrip = dwReturnTime - *pdwTimeStamp;

	if (dwRoundTrip > iVal * 2)
		trace(L6,_T("SecPing - QuickPing2 took %d ms but we only allotted %d ms to winsock. (this is not an error)\n"), dwRoundTrip, iVal * 2);


//	TRACE("SecPing - -QuickPing2()\n");
	return TRUE; 
}

/**
 * Function:		QuickPingTCP
 * Purpose:			DCOM has a very long timeout in the event of a network error.  This 
 *					method is called prior to performing any DCOM action to make sure that the
 *					machine is accessible over the network.
 *					
 * Parameters:		sockAddr - Reference to a SOCKADDR_IN for the target machine.
 *					csMachineName - Constant CString reference to the machine being pinged 
 *					(used for logging).
 *					PingTimeout - Amount of time to wait for a response from the target machine.
 *					
 * Returns:			TRUE if the ping was successful.  FALSE otherwise.
 * Calls:			
 * Side Effects:	
 * Notes:			This function is typically called after scanning an item, so it must
 *					be very fast.  It must not hang if there is a network error.
 *
 *					This version of QuickPing2() attempts to open a TCP socket on the NetBios
 *					port (139).  If the connection succeeds, then the ping returns true.
 *
 *					Since SMB shares are currently utilized to copy the WLDB from the security
 *					server, this port should always be visible.  The ability to specify an 
 *					alternate port in the registry is provided, however (see QPInitialize()).
 *					If an alternate port is specified, there must be a listening TCP service
 *					on that port.
 */
BOOL QuickPingTCP(SOCKADDR_IN &sockAddr, const CString &csMachineName, long PingTimeout)
{
	SOCKET sock;
	int ret;						// Return codes of function calls.
	BOOL rc = FALSE;				// Return value.

	linger ling;					// To disable blocking mode on the socket.
	int iMode = 1;					// To disable blocking mode on the socket.

	fd_set fdsWrite;				// Write socket FDS
	fd_set fdsExc;					// Exception socket FDS
	timeval timeout;				// timeout for use with select()
	CString csTrace;

	// Note:  Must use WSA_FLAG_OVERLAPPED for the socket to be non-blocking.
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(sock == INVALID_SOCKET)
	{
		csTrace.Format(_T("QuickPingTCP() - ERROR: Could not create a socket [%d]."), WSAGetLastError());
        trace(L6,_T("%s"),csTrace);
		return(FALSE);
	}

	// Enable a hard disconnect for quick socket closure, since we are not sending any data.
	ling.l_onoff = 1;
	ling.l_linger = 0;
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling)) == SOCKET_ERROR) 
	{
        trace(L6,_T("QuickPingTCP() - ERROR:  Could not set SO_LINGER."));
	}

	// Make the socket non-blocking, so that connect will return immediately rather than
	// timeout if there is a network problem.
	ret = ioctlsocket(sock, FIONBIO, (u_long FAR*) &iMode);
	if(ret == SOCKET_ERROR)
	{
		csTrace.Format(_T("QuickPingTCP() - ERROR:  Could not enable non-blocking mode on socket. Error = %d"), WSAGetLastError());
        trace(L6,_T("%s"),csTrace);
		closesocket(sock);
		return(FALSE);
	}

	ret = connect( sock, (SOCKADDR*) &sockAddr, sizeof(sockAddr) );
	if(ret == 0)	// Connect successful - quit.
	{
		csTrace.Format(_T("QuickPingTCP() - Emulated ping of %s successful"), csMachineName);
        trace(L6,_T("%s"),csTrace);
		closesocket(sock);
		return(TRUE);
	}

	ret = WSAGetLastError();

	// ret == WSAEWOULDBLOCK means that the connection is still in progress.  In this case,
	// select() is utilized to wait for PingTimeout or until a
	// SYN/ACK is received.  Otherwise an error has occurred.
	if(ret != WSAEWOULDBLOCK)  
	{
		csTrace.Format(_T("QuickPingTCP() - ERROR:  Failed to connect to %s. Error code = [%d]"), csMachineName, ret);
        trace(L6,_T("%s"),csTrace);

		closesocket(sock);
		return(FALSE);
	}

	csTrace.Format(_T("QuickPingTCP() - Waiting [%d]"), PingTimeout);
    trace(L6,_T("%s"),csTrace);

	// convert Milliseconds to select() timeout.
	timeout.tv_sec = PingTimeout / 1000;
	timeout.tv_usec = (PingTimeout % 1000) * 1000;

	// Setup up FDS variables.
    FD_ZERO(&fdsWrite);        
    FD_SET(sock, &fdsWrite);
	FD_ZERO(&fdsExc);        
	FD_SET(sock, &fdsExc);

	ret = select(0, NULL, &fdsWrite, &fdsExc, &timeout);

	switch(ret)
	{
		case 0:					// timeout exceeded.
			csTrace.Format(_T("QuickPingTCP() - ERROR:  Timed out connecting to %s"), csMachineName);
            trace(L6,_T("%s"),csTrace);
			break;
		case SOCKET_ERROR:
			csTrace.Format(_T("QuickPingTCP() - ERROR:  Could not connect to %s -> Error = %d"), csMachineName, WSAGetLastError());
            trace(L6,_T("%s"),csTrace);
			break;
	}

	// Success ?
	if(ret > 0 && fdsExc.fd_count == 0)
	{
		csTrace.Format(_T("QuickPingTCP() - Emulated ping of %s successful"), csMachineName);
        trace(L6,_T("%s"),csTrace);
		rc = TRUE;
	}

	closesocket(sock);
	return(rc);
}

/**
 * Function:		QuickPing2
 * Purpose:			DCOM has a very long timeout in the event of a network error.  This 
 *					method is called prior to performing any DCOM action to make sure that the
 *					machine is accessible over the network.
 *					
 * Parameters:		csMachineName - The name of the machine to ping.
 *					PingRetry - Number of times to attempt.
 *					PingTDStart - Timeout starting value.
 *					PingTDIncr - Amount to increment the timeout each retry.
 *					bByNameOnly - If true, perform a DNS lookup each time the function is 
 *						called (possibly slow).  If false, use a cached value (IP could be 
 *						incorrect in DHCP environments).
 *					
 * Returns:			TRUE if the ping was successful.  FALSE otherwise.
 * Calls:			QuickPingTCP() if gdwUseNewQuickPingMethod is set, OldQuickPing2() otherwise.
 * Side Effects:	
 * Notes:			This function is typically called after scanning an item, so it must
 *					be very fast.  It must not hang if there is a network error.
 *
 *					Previously, this function perfomed an ICMP ping.  When ScotApp runs
 *					without administrator privileges, however, the function ceases to work.
 *					The reason is that the use of ICMP requires the creation of a raw
 *					socket, which requires admin privileges.  Several alternatives were
 *					tried:
 *					1) Using _tsystem() to call the windows ping.exe directly.  This
 *					   was far too slow.
 *					2) Using IcmpSendEcho().  This function is not available on NT.
 *					3) Using NetShareGetInfo().  Too slow in the case of a network problem.
 *
 *					QuickPingTCP() is used if running without administrator privileges
 *					(global variable gdwUseNewQuickPingMethod == 1).  Otherwise, the old ICMP
 *					ping method is used.
 */
BOOL QuickPing2(CString csMachineName, long PingRetry, long PingTDStart, long PingTDIncr, bool bByNameOnly)
{
	SOCKADDR_IN	sockAddr;
	int iVal = PingTDStart;

	// Use old ping method unless specified otherwise (see QPInitialize()).
	if(gdwUseNewQuickPingMethod == 0)
	{
		return OldQuickPing2(csMachineName, PingRetry, PingTDStart, PingTDIncr, bByNameOnly);
	}

	/* --------------------------------------------------------------------- */
	/* CMal101001 if special registry entry exists, then always ping by Name */
	/*            Do this by clearing out the structure that GetQuickAddress */
	/*            uses.  Told by the new 5th param to this call.             */
	/* --------------------------------------------------------------------- */
	if (bByNameOnly == TRUE)
	{
		for(int ij = 0; ij < iCurrentArrayIndex; ij++)
		{
			// not very important to clear these, since putting the count of
			// things in this array to 0 in a second.  Do something to show intent.
			ArrayOfHost[ij].csName = _T("");
			// even though not Init, has 0.0.0.0 for an IP address (seen in debugger)
			ArrayOfHost[ij].client.sin_addr.s_addr = 0UL; 
		}
		iCurrentArrayIndex = 0; // most important step
	}

	if (!GetQuickAddress(csMachineName, sockAddr))
	{
		return FALSE;
	}
	sockAddr.sin_port = htons((u_short)gdwPingPort);
	
	// Use the new TCP ping.  Break out as soon as a ping is successful.
	for(int i = 1; i <= PingRetry; i++, iVal += PingTDIncr)
	{
		if(QuickPingTCP(sockAddr, csMachineName, iVal) == TRUE)
		{
			return TRUE;
		}
	}
	return FALSE;	// Failed after all attempts.
}
