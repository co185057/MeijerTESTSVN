#include "stdafx.h"
//#include <winsock.h>				// Winsock header file // CMal092799
#include <afxpriv.h>                // for USES_CONVERSION
#include <atlconv.h>

//#define WINSOCK_VERSION 0x0101		// Program requires Winsock version 1.1
#define INCL_WINSOCK_API_PROTOTYPES 1
#include <winsock2.h>				// Winsock header file // CMal092799
#include "qping.h"

#define NO_FLAGS 0					// No special flags specified
					// RFC 792 defines ICMP message values
#define ICMP_ECHO 8					// An ICMP echo message
#define ICMP_ECHOREPLY 0			// An ICMP echo reply message
#define	ICMP_HEADERSIZE 8			// ICMP header size ("echo messages" only)
#define ICMP_ID 6899				// Some random unique ID (actually means 6/8/99 :))

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

WORD InternetChksum(LPWORD lpwIcmpData, WORD wDataLength)
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

BOOL QuickPing(CString csMachineName, long PingRetry, long PingTDStart, long PingTDIncr)
{
	USES_CONVERSION;

  BOOL rc;
  WSADATA wsaData;						// Winsock implementation details
																// Local variables
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
	SOCKADDR_IN	sockAddrLocal;			// Socket address structures
	SOCKADDR_IN	sockAddrHost;			// 
	SOCKET hSocket;						// Socket handle (or descriptor)
	LPHOSTENT lpHostEntry;				// Internet host data structure
	LPPROTOENT lpProtocolEntry;			// Internet protocol data structure

	BYTE IcmpSendPacket[1024];			// Buffer space for data to send
	BYTE IcmpRecvPacket[4096];			// Buffer space for received data 

	struct icmp *pIcmpHeader;			// A pointer to the ICMP structure
	struct ip *pIpHeader;				// A pointer to the IP header structure
#ifdef _UNICODE
	LPCSTR lpszHostName(W2CA(csMachineName));
#else
	//old way
	LPCTSTR lpszHostName = csMachineName;					// A pointer to the time server host
#endif
	BOOL bRecvResult = FALSE;			// CMal092799 - reply from 'server'
		
	// Start the program here
	rc = WSAStartup(WINSOCK_VERSION, &wsaData);

  if (inet_addr(lpszHostName) == INADDR_NONE) // Check of IP addr - not name
  {
	  if ((lpHostEntry = gethostbyname(lpszHostName)) == NULL)
	  {
		  TRACE(_T("Could not get %s IP address. Error code: %d"), (LPTSTR)lpszHostName, WSAGetLastError());
          csMachineName.ReleaseBuffer();  
		  WSACleanup();
		  return(FALSE);
	  }
    
	  sockAddrLocal.sin_addr = *((LPIN_ADDR) *lpHostEntry->h_addr_list);
  }
  else  // We got an IP addr, not a name.  So just assign w/o looking up.
  {
    IN_ADDR tmp;
    tmp.s_addr = inet_addr(lpszHostName);
    sockAddrLocal.sin_addr = tmp;
  }

  sockAddrLocal.sin_family = AF_INET;

	// With a raw socket, the program must specify a protocol
	if ((lpProtocolEntry = getprotobyname("icmp")) == NULL)
		nProtocol = IPPROTO_ICMP;
	else
		nProtocol = lpProtocolEntry->p_proto;
	
	/* SET UP THE SOCKET ------------------------------------------- */
	/* Create a "raw" socket and specify ICMP as the protocol to use */
	/* ------------------------------------------------------------- */

	//if ((hSocket = socket(PF_INET, SOCK_RAW, nProtocol)) == INVALID_SOCKET) // CMal092799 replaced
	if ((hSocket = WSASocket(PF_INET, SOCK_RAW, nProtocol, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		TRACE(_T("Could not create a RAW socket."));

		WSACleanup();
		return(FALSE);
	}

	pIcmpHeader = (struct icmp *) IcmpSendPacket;	// Point at the data area
	pIcmpHeader->icmp_type = ICMP_ECHO;				// then fill in the data.
	pIcmpHeader->icmp_code = 0;						// Use the Sockman instance 
	pIcmpHeader->icmp_id = (WORD) ICMP_ID;			// Unique ID.
	pIcmpHeader->icmp_seq = 0;						// It's important to reset
	pIcmpHeader->icmp_cksum = 0;					// the checksum to zero.
		
	//Put tick count in the optional data area
	pdwTimeStamp = (PDWORD)&IcmpSendPacket[ICMP_HEADERSIZE];
  *pdwTimeStamp = GetTickCount();
	iPacketSize = ICMP_HEADERSIZE + sizeof(DWORD);
	pIcmpHeader->icmp_cksum = InternetChksum((LPWORD)pIcmpHeader, iPacketSize);
    
	if (pIcmpHeader->icmp_cksum !=0 )
	{   
		/* ----------------------------------------- */
		/* Send Msg to Server over Socket to server. */
		/* This seems to always work.    CMal092499  */
		/* ----------------------------------------- */
		iSentBytes = sendto(hSocket, (LPCSTR) IcmpSendPacket, iPacketSize, 
							NO_FLAGS, (LPSOCKADDR) &sockAddrLocal, sizeof(sockAddrLocal));

		if (iSentBytes == SOCKET_ERROR)
		{
      int iError = WSAGetLastError();
			closesocket(hSocket);

			TRACE(_T("The sendto() function returned a socket error %d."), iError);

			WSACleanup();
			return(FALSE);
		}
		
		if (iSentBytes != iPacketSize)
		{
			closesocket(hSocket);
			TRACE(_T("Wrong number of bytes sent: %d"), iSentBytes);

			WSACleanup();
			return(FALSE);
		}
		
		iHostAddrLength = sizeof(sockAddrHost);

		/* --------------------------------------------------------------------- */
		/* Get the reply back from the server (proves that server is 'alive')    */
		/* --------------------------------------------------------------------- */
		/* Rewrite this section of QPing to 'quickly' try to get the information */
		/* back from the Server.  And retry a few times if not successful.       */
		/* CMal092799 ---------------------------------------------------------- */ 

		int iVal = PingTDStart;
					    
		for (int i = 1; i <= PingRetry; i++)
		{
			// set the receive time to quickly retry milliseconds
			if (setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (LPCSTR)&iVal, sizeof(int) ) )
			{
				int iError = WSAGetLastError();
				TRACE(_T("QPing.cpp - setsockopt(SO_RCVTIMEO) %d"), iError);
				return FALSE;
			} // CMal092799
			
			// CMal092499 - retrieve reply over the socket
			iReceivedBytes = recvfrom(hSocket, (LPSTR) IcmpRecvPacket, 
				sizeof(IcmpRecvPacket), NO_FLAGS, (LPSOCKADDR) &sockAddrHost,
				&iHostAddrLength);

			if (iReceivedBytes > 0) // CMal092799 if positive then we communicated
			{
				bRecvResult = TRUE;
				break; // CMal092799 - if it worked, then don't try again
			}

			iVal += PingTDIncr; // CMal092799 - try a little long next time

		} 
		
		// CMal092799 - end of rewrite
	}
	else
	{
		closesocket(hSocket);
		TRACE(_T("Checksum computation error! Result was zero!"));
	
		WSACleanup();
		return(FALSE);
	}
    
	closesocket(hSocket);
	
	/* ------------------------------------------------------- */
	/* If we can't talk with the Server,                       */
	/* then iReceivedBytes will equal SOCKET_ERROR  CMal092799 */
	/* ------------------------------------------------------- */

	if (iReceivedBytes == SOCKET_ERROR)
	{   
		iSocketError = WSAGetLastError();
		if (iSocketError == 10004)
		{
			TRACE(_T("Ping operation for %s was cancelled.\n"), (LPTSTR)lpszHostName);
			dwRoundTrip = 0;

			WSACleanup();
			return(FALSE);
		}
		else
		{
			// And we'll leave QPing from this point - if no server exists  CMal092799
			TRACE(_T("Socket Error from recvfrom(): %d\n"), iSocketError);

			WSACleanup();
			return(FALSE);
		}
	}

	dwReturnTime = GetTickCount();
	dwRoundTrip = dwReturnTime - *pdwTimeStamp;

	// Point to the IP Header in the received packet
	pIpHeader = (struct ip *)IcmpRecvPacket;

	// Extract bits 4-7 and convert the number of 32-bit words to bytes
	iIPHeadLength = (pIpHeader->ip_verlen & 0x0F) * 4;  // get header length and times 4

	// Test the length to make sure an ICMP header was received
	if (iReceivedBytes < iIPHeadLength + ICMP_HEADERSIZE)
	{
		TRACE(_T("Received packet was too short."));

		WSACleanup();
		return(FALSE);
	}

	// Point to the ICMP message which immediately follows the IP header
	pIcmpHeader = (struct icmp *) (IcmpRecvPacket + iIPHeadLength);

	// Make sure this is an ICMP "echo reply"
	if (pIcmpHeader->icmp_type != ICMP_ECHOREPLY)
	{
		TRACE(_T("Received packet was not an echo reply to your ping.\n"));

		WSACleanup();
		return(FALSE);
	}

	// Make sure this program sent the packet
	if (pIcmpHeader->icmp_id != (WORD) ICMP_ID)
	{
		TRACE(_T("Received packet was not sent by this program.\n"));

		WSACleanup();
		return(FALSE);
	}

	WSACleanup();
	return TRUE; 
}
