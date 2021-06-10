//
// Copyright (c) 2001 Kresimir Petric
//


// You have no permission to use any of code from this class to anything else
// than inside Telnet/X, not even if you try to sit on your foot.

// Restriction does not apply to parts taken from Putty - you can do with it as stated in Putty license

#include "stdafx.h"
#include <wincrypt.h>

#define SSH_CIPHER_IDEA		1
#define SSH_CIPHER_DES		2
#define SSH_CIPHER_3DES		3
#define SSH_CIPHER_BLOWFISH	6

#define ASSERT(e, m) _ASSERT(e)
#define GET_32BIT(cp) \
    (((unsigned long)(_TUCHAR)(cp)[0] << 24) | \
    ((unsigned long)(_TUCHAR)(cp)[1] << 16) | \
    ((unsigned long)(_TUCHAR)(cp)[2] << 8) | \
    ((unsigned long)(_TUCHAR)(cp)[3]))

#define PUT_32BIT(cp, value) { \
    (cp)[0] = (_TUCHAR)((value) >> 24); \
    (cp)[1] = (_TUCHAR)((value) >> 16); \
    (cp)[2] = (_TUCHAR)((value) >> 8); \
    (cp)[3] = (_TUCHAR)(value); }


unsigned long crc32(const void *buf, size_t len);

#include "com.h"
#include "cstr.h"
#include "Cmsocket.h"
#include "telnetxcom.h"
#include "csocket.h"
//#include "blowfish.h"
#include "md5.h"
#include "CSSHSocket.h"

#define PROTOCOL_MAJOR_VERSION				1
#define PROTOCOL_MINOR_VERSION				5

unsigned long crc32(const void *buf, size_t len);
void des3_sesskey(_TUCHAR *key);
void des3_encrypt_blk(_TUCHAR *blk, int len);

HCRYPTPROV hCryptProv;
HCRYPTKEY hDESKey[2][3] = {{0,0,0},{0,0,0}}; /* global for now */
#define CSP MS_ENHANCED_PROV


#define SSH_MSG_NONE                            0       /* no message */
#define SSH_MSG_DISCONNECT                      1       /* cause (string) */
#define SSH_SMSG_PUBLIC_KEY                     2       /* ck,msk,srvk,hostk */
#define SSH_CMSG_SESSION_KEY                    3       /* key (MP_INT) */
#define SSH_CMSG_USER                           4       /* user (string) */
#define SSH_CMSG_AUTH_RHOSTS                    5       /* user (string) */
#define SSH_CMSG_AUTH_RSA                       6       /* modulus (MP_INT) */
#define SSH_SMSG_AUTH_RSA_CHALLENGE             7       /* int (MP_INT) */
#define SSH_CMSG_AUTH_RSA_RESPONSE              8       /* int (MP_INT) */
#define SSH_CMSG_AUTH_PASSWORD                  9       /* pass (string) */
#define SSH_CMSG_REQUEST_PTY                    10      /* TERM, tty modes */
#define SSH_CMSG_WINDOW_SIZE                    11      /* row,col,xpix,ypix */
#define SSH_CMSG_EXEC_SHELL                     12      /* */
#define SSH_CMSG_EXEC_CMD                       13      /* cmd (string) */
#define SSH_SMSG_SUCCESS                        14      /* */
#define SSH_SMSG_FAILURE                        15      /* */
#define SSH_CMSG_STDIN_DATA                     16      /* data (string) */
#define SSH_SMSG_STDOUT_DATA                    17      /* data (string) */
#define SSH_SMSG_STDERR_DATA                    18      /* data (string) */
#define SSH_CMSG_EOF                            19      /* */
#define SSH_SMSG_EXITSTATUS                     20      /* status (int) */
#define SSH_MSG_CHANNEL_OPEN_CONFIRMATION       21      /* channel (int) */
#define SSH_MSG_CHANNEL_OPEN_FAILURE            22      /* channel (int) */
#define SSH_MSG_CHANNEL_DATA                    23      /* ch,data (int,str) */
#define SSH_MSG_CHANNEL_CLOSE                   24      /* channel (int) */
#define SSH_MSG_CHANNEL_CLOSE_CONFIRMATION      25      /* channel (int) */

/* new channel protocol */
#define SSH_MSG_CHANNEL_INPUT_EOF               24
#define SSH_MSG_CHANNEL_OUTPUT_CLOSED           25

/*      SSH_CMSG_X11_REQUEST_FORWARDING         26         OBSOLETE */
#define SSH_SMSG_X11_OPEN                       27      /* channel (int) */
#define SSH_CMSG_PORT_FORWARD_REQUEST           28      /* p,host,hp (i,s,i) */
#define SSH_MSG_PORT_OPEN                       29      /* ch,h,p (i,s,i) */
#define SSH_CMSG_AGENT_REQUEST_FORWARDING       30      /* */
#define SSH_SMSG_AGENT_OPEN                     31      /* port (int) */
#define SSH_MSG_IGNORE                          32      /* string */
#define SSH_CMSG_EXIT_CONFIRMATION              33      /* */
#define SSH_CMSG_X11_REQUEST_FORWARDING         34      /* proto,data (s,s) */
#define SSH_CMSG_AUTH_RHOSTS_RSA                35      /* user,mod (s,mpi) */
#define SSH_MSG_DEBUG                           36      /* string */
#define SSH_CMSG_REQUEST_COMPRESSION            37      /* level 1-9 (int) */
#define SSH_CMSG_MAX_PACKET_SIZE                38      /* max_size (int) */

/* Support for TIS authentication server
   Contributed by Andre April <Andre.April@cediti.be>. */
#define SSH_CMSG_AUTH_TIS                       39      /* */
#define SSH_SMSG_AUTH_TIS_CHALLENGE             40      /* string */
#define SSH_CMSG_AUTH_TIS_RESPONSE              41      /* pass (string) */

/* Support for kerberos authentication by Glenn Machin and Dug Song
   <dugsong@umich.edu> */
#define SSH_CMSG_AUTH_KERBEROS                  42      /* string (KTEXT) */
#define SSH_SMSG_AUTH_KERBEROS_RESPONSE         43      /* string (KTEXT) */
#define SSH_CMSG_HAVE_KERBEROS_TGT              44      /* string (credentials) */

/* Reserved for official extensions, do not use these */
#define SSH_CMSG_RESERVED_START                 45
#define SSH_CMSG_RESERVED_END                   63




CSSHSocket::CSSHSocket( CTelnetXCom* pCtrl, LPTSTR libname)
: CSocket(pCtrl,libname)
{
	m_pCtrl = pCtrl;
	m_Secured=Telnet;
	m_State=SecuredStateNotConnected;
	m_InputBufferPos=0;
	m_OutputBufferPos=0;

	m_InputBuffer=m_InputBufferForReading;
	m_OutputBuffer=m_OutputBufferForWriting;

	memset(&pktin,0,sizeof(Packet));
	memset(&pktout,0,sizeof(Packet));

	m_Hostname.Empty();
	m_Port=0;

	m_Cipher=FALSE;
	//m_CipherType=SSH_CIPHER_IDEA;
	//m_CipherType=SSH_CIPHER_BLOWFISH;
	m_CipherType=SSH_CIPHER_3DES;
	//m_CipherType=SSH_CIPHER_DES;

//	m_Bf=NULL;

	debug=TRUE;

    if(CryptAcquireContext(&hCryptProv, _T("MabrySSH"), CSP, PROV_RSA_FULL,CRYPT_NEWKEYSET) == 0) 
	{
		if(GetLastError() == NTE_EXISTS) 
		{
			if(CryptAcquireContext(&hCryptProv, _T("MabrySSH"), CSP,PROV_RSA_FULL, 0) == 0) 
			{
					OnDebug(_T("Failed to acquire security context"));
					OnDebug(_T("Do you have high encryption installed?"));
					return; 
			}
		} 
		else
		{
					OnDebug(_T("Failed to acquire security context"));
					OnDebug(_T("Do you have high encryption installed?"));
					return; 
		}
    }
    return;


}

CSSHSocket::~CSSHSocket( void )
{

//	if (m_Bf)
//		delete m_Bf;

	if (pktin.data)
		free(pktin.data);
	if (pktout.data)
		free(pktout.data);
    int i, j;
    for(i=0; i<2; i++) {
	for(j=0; j<3; j++) {
	    if(hDESKey[i][j])
		CryptDestroyKey(hDESKey[i][j]);
	    hDESKey[i][j] = 0;
	}
    }
    if(hCryptProv)
	CryptReleaseContext(hCryptProv, 0);
    hCryptProv = 0;
}

void CSSHSocket::OnAccept(int nErrorCode)
{
	if (FWType!=FirewallTypeNone && State!=FWStateConnected)
	{
		CSocket::OnAccept(nErrorCode);
		return;
	}
	m_pCtrl->OnAccept(this, nErrorCode);
}
void CSSHSocket::OnClose(int nErrorCode)
{
	if (FWType!=FirewallTypeNone && State!=FWStateConnected)
	{
		CSocket::OnClose(nErrorCode);
		return;
	}
	m_pCtrl->OnClose(this, nErrorCode);
}
void CSSHSocket::OnConnect(int nErrorCode)
{
	if (FWType!=FirewallTypeNone && State!=FWStateConnected)
	{
		CSocket::OnConnect(nErrorCode);
		return;
	}

	if (!(m_Secured==SSH1 /*|| m_Secured==SSH2Protocol*/))
		m_pCtrl->OnConnect(this, nErrorCode);
}
void CSSHSocket::OnOutOfBandData(int nErrorCode)
{
	if (FWType!=FirewallTypeNone && State!=FWStateConnected)
	{
		CSocket::OnOutOfBandData(nErrorCode);
		return;
	}

	m_pCtrl->OnOutOfBandData(this,nErrorCode);
}
void CSSHSocket::OnReceive(int nErrorCode)
{
	int i,j;

	// first part deals with proxies that I couldn't move to CSocket
	if (FWType==FirewallTypeSSLProxy && m_Secured==SSH1 && State==FWStateConnecting)
	{
		_TCHAR buff[1024];
		CMSocket::AsyncSelect(0);
		i=CSocket::Receive(buff,12,0);
		if (i==SOCKET_ERROR || buff[9]!=_T('2') || buff[10]!=_T('0') || buff[11]!=_T('0'))
		{
			WSASetLastError(WSAECONNREFUSED);
			CSocket::Close();
			m_pCtrl->OnConnect(this,WSAECONNREFUSED);
			return;
		}
		j=0;
		do
		{
			i=CSocket::Receive(buff,1,0);
			if (buff[0]==_T('\n')) 
				j++;
		} while (i>0 && j<2);
		State=FWStateConnected;
		CMSocket::AsyncSelect(OrgAsyncEvent);
		//if (OrgAsyncEvent & FD_CONNECT) m_pCtrl->OnConnect(this,nErrorCode);
		//if (OrgAsyncEvent & FD_WRITE) m_pCtrl->OnSend(this,nErrorCode);
		//if (OrgAsyncEvent & FD_READ) m_pCtrl->OnSend(this,nErrorCode);
		// we will be called back
		return;
	}else
	if (FWType==FirewallTypeSocks4 && State==FWStateConnecting && m_Secured==SSH1)
	{
		i=CSocket::Receive(Socks4Data,8,0);
		if (i==SOCKET_ERROR || Socks4Data[1]!=90)
		{
			WSASetLastError(WSAECONNREFUSED);
			Close();
			m_pCtrl->OnConnect(this,WSAECONNREFUSED);
			return;
		}
		State=FWStateConnected;
		CMSocket::AsyncSelect(OrgAsyncEvent);
//		if (OrgAsyncEvent & FD_CONNECT) m_pCtrl->OnConnect(this,nErrorCode);
//		if (OrgAsyncEvent & FD_WRITE) m_pCtrl->OnSend(this,nErrorCode);
		// we will be called again
		return;	
	}
	else
	if (FWType==FirewallTypeSocks5 && State==FWStateSendingRequest && m_Secured==SSH1)
	{
		CMSocket::AsyncSelect(0);
		CSocket::Receive(Socks4Data,5,0);
		if (!Socks4Data[1])
		{
			switch(Socks4Data[3])
			{
				case 1:
					CSocket::Receive(Socks4Data,5,0);
					break;

				case 3:
					CSocket::Receive(Socks4Data,Socks4Data[4]+2,0);
					break;

				case 4:
					CSocket::Receive(Socks4Data,17,0);
					break;
			}
			State=FWStateConnected;
			CMSocket::AsyncSelect(OrgAsyncEvent);
//			if (OrgAsyncEvent & FD_CONNECT) m_pCtrl->OnConnect(this,nErrorCode);
//			if (OrgAsyncEvent & FD_WRITE) m_pCtrl->OnSend(this,nErrorCode);
			return;
			}
		m_pCtrl->OnConnect(this,WSAECONNREFUSED);
		return;
	}
	else
	if (FWType!=FirewallTypeNone && State!=FWStateConnected)
	{
		CSocket::OnReceive(nErrorCode);
		return;
	}

	if (m_Secured==SSH1 /*|| m_Secured==SSH2Protocol*/)
	{
		switch (m_State)
		{

			case SecuredStateConnecting:

				// get SSH version code
				if (ExchangeSSHVersionNumbers())
				{
					Close();
					break;
				}
				m_State=SecuredStateSentVersionNumber;
				break;

			case SecuredStateSentVersionNumber:
				// now I expect to receive SSH_MSG_PUBLIC_KEY
				i=ReadPacket();
				if (i)
				{
					// problems
					return;
				}
				if (pktin.type!=SSH_SMSG_PUBLIC_KEY)
				{
					// invalid packet type
					return;
				}
				CreateSSHKeypair();
				m_State=SecuredStateSentKeys;
				break;

			case SecuredStateSentKeys:
				i=ReadPacket();
				if (i)
				{
					return;
				}
				if (pktin.type!=SSH_SMSG_SUCCESS)
				{
					//eow, this is wrong!
					//MessageBeep(-1);
					return;
				}
				// send username
				CreateNewPacket(SSH_CMSG_USER, m_Username.GetLength()+4);
				pktout.body[0]=pktout.body[1]=pktout.body[2]=0;
				pktout.body[3]=m_Username.GetLength();
				memcpy(pktout.body+4, m_Username.GetBuffer(0),m_Username.GetLength());
				m_Username.ReleaseBuffer();
				SendPacket();
				m_State=SecuredStateSentUsername;
				break;

			case SecuredStateSentUsername:
				i=ReadPacket();
				if (i)
				{
					return;
				}
				if (pktin.type==SSH_SMSG_FAILURE)
				{
				// send password
					CreateNewPacket(SSH_CMSG_AUTH_PASSWORD, m_Password.GetLength()+4);
					pktout.body[0]=pktout.body[1]=pktout.body[2]=0;
					pktout.body[3]=m_Password.GetLength();
					memcpy(pktout.body+4, m_Password.GetBuffer(0),m_Password.GetLength());
					m_Password.ReleaseBuffer();
					SendPacket();
					//m_State=SecuredStateSentPassword;
					break;
				} // else password is ok
				if (pktin.type!=SSH_SMSG_SUCCESS)
					break; 
				// password is ok
				m_State=SecuredStateSentPassword;

			case SecuredStateSentPassword:
				// great, we're in.. request terminal
				i=m_TerminalType.GetLength();
				CreateNewPacket(SSH_CMSG_REQUEST_PTY, i+21);
		        pktout.body[0] = (i >> 24) & 0xFF;
				pktout.body[1] = (i >> 16) & 0xFF;
				pktout.body[2] = (i >> 8) & 0xFF;
				pktout.body[3] = i & 0xFF;
				memcpy(pktout.body+4, m_TerminalType.GetBuffer(0), i);
				m_TerminalType.ReleaseBuffer();
		        memset(pktout.body+4+i, 0, 17);
				pktout.body[7+i]=24;
				pktout.body[11+i]=80;
				SendPacket();
				m_State=SecuredStateRequestTerminal;
				break;

			case SecuredStateRequestTerminal:
				i=ReadPacket();
				if (i)
				{
					return;
				}
				if (pktin.type!=SSH_SMSG_SUCCESS)
					return;

				// yups, we got terminal also
				i=m_TerminalType.GetLength();
				if (!m_Command.GetLength())
					CreateNewPacket(SSH_CMSG_EXEC_SHELL,0);
				else
				{
					CreateNewPacket(SSH_CMSG_EXEC_CMD, m_Command.GetLength()+4);
					pktout.body[0]=pktout.body[1]=pktout.body[2]=0;
					pktout.body[3]=m_Command.GetLength();
					memcpy(pktout.body+4, m_Command.GetBuffer(0),m_Command.GetLength());
					m_Command.ReleaseBuffer();				
				}
				SendPacket();
				m_State=SecuredStateOk;
				m_pCtrl->OnConnect(this, 0);
				m_pCtrl->OnSend(this, 0);
				break;

			case SecuredStateOk:
				i=ReadPacket();
				if (i)
				{
					return;
				}
				if (pktin.type==SSH_SMSG_STDOUT_DATA)
				{
					memcpy(m_OutputBuffer,pktin.body+4,pktin.length-5);
					m_OutputBuffer+=pktin.length-5;
					m_OutputBufferPos+=pktin.length-5;
					m_pCtrl->OnReceive(this, 0);
				}
				break;
		}
	}
	else
		m_pCtrl->OnReceive(this, nErrorCode);
}

void CSSHSocket::OnTimer(void)
{
	m_pCtrl->OnTimer(this);
}

void CSSHSocket::OnDebug(LPTSTR msg)
{
	if (debug)
		m_pCtrl->OnDebug(this,msg);
}

void CSSHSocket::OnSend(int nErrorCode)
{
	if (FWType!=FirewallTypeNone && State!=FWStateConnected)
	{
		CSocket::OnSend(nErrorCode);
		return;
	}

	if (!(m_Secured==SSH1 /*|| m_Secured==SSH2Protocol*/))
		m_pCtrl->OnSend(this, nErrorCode);
}

BOOL CSSHSocket::Connect(LPTSTR lpszHostAddress, UINT nHostPort)
{
	m_Hostname=lpszHostAddress;
	m_Port=nHostPort;
	m_State=SecuredStateConnecting;
	return CSocket::Connect(lpszHostAddress,nHostPort);
}

int CSSHSocket::Send( const void* lpBuf, int nBufLen, int nFlags)
{
	int i;

	if (m_Secured==SSH1 /*|| m_Secured==SSH2Protocol*/)
	{
		if (m_State!=SecuredStateOk)
			return -1;

		CreateNewPacket(SSH_CMSG_STDIN_DATA, 4+nBufLen);
		i=nBufLen;
		pktout.body[3]=i%256;i=i>>8;
		pktout.body[2]=i%256;i=i>>8;
		pktout.body[1]=i%256;i=i>>8;
		pktout.body[0]=i;
		memcpy(pktout.body+4, lpBuf, nBufLen);
		SendPacket();
		return nBufLen;
	}
	else
		return CSocket::Send(lpBuf, nBufLen, nFlags);

}

int CSSHSocket::Receive(void* lpBuf, int nBufLen, int nFlags)
{
	int i;


	if (m_Secured==SSH1 /*|| m_Secured==SSH2Protocol*/)
	{
		if (m_State!=SecuredStateOk)
			return 0;

		i=min(nBufLen, m_OutputBufferPos);

		if (i<0)
			return 0;

		memcpy(lpBuf, m_OutputBuffer-i, i);
		m_OutputBuffer-=i;
		m_OutputBufferPos-=i;

		if (m_OutputBufferPos<0)
		{
			m_OutputBufferPos=0;
			m_OutputBuffer=m_OutputBufferForWriting;
		}
		return i;
	}
	return CSocket::Receive(lpBuf, nBufLen, nFlags);

}


void CSSHSocket::CreateNewPacket(int type, int len)
{
    int pad, biglen;

    len += 5;			       /* type and CRC */
    pad = 8 - (len%8);
    biglen = len + pad;

    pktout.length = len-5;
    if (pktout.maxlen < biglen) 
	{
		pktout.maxlen = biglen;
		if (pktout.data)
			free(pktout.data);
		pktout.data = (_TUCHAR *)(pktout.data == NULL ? malloc(biglen+4) : realloc(pktout.data, biglen+4));
		if (!pktout.data)
			OnDebug(_T("Not enough memory"));
    }
    pktout.body = pktout.data+4+pad+1;
    pktout.type = type;
}

void CSSHSocket::SendPacket(void) 
{
    int pad, len, biglen, i;
    unsigned long crc;

    pktout.body[-1] = pktout.type;
/*
    if (ssh1_compressing) {
	_TUCHAR *compblk;
	int complen;
	zlib_compress_block(pktout.body-1, pktout.length+1,
			    &compblk, &complen);
	ssh1_pktout_size(complen-1);
	memcpy(pktout.body-1, compblk, complen);
	sfree(compblk);
    }
*/
    len = pktout.length + 5;	       /* type and CRC */
    pad = 8 - (len%8);
    biglen = len + pad;
	pktout.body[-1] = pktout.type;

    for (i=0; i<pad; i++)
	pktout.data[i+4] = RandomByte();

    crc = crc32(pktout.data+4, biglen-4);
    
    pktout.data[biglen+0] = (_TUCHAR) ((crc >> 24) & 0xFF);
    pktout.data[biglen+1] = (_TUCHAR) ((crc >> 16) & 0xFF);
    pktout.data[biglen+2] = (_TUCHAR) ((crc >> 8) & 0xFF);
    pktout.data[biglen+3] = (_TUCHAR) (crc & 0xFF);

    pktout.data[0] = (len >> 24) & 0xFF;
    pktout.data[1] = (len >> 16) & 0xFF;
    pktout.data[2] = (len >> 8) & 0xFF;
    pktout.data[3] = len & 0xFF;

    if(m_Cipher)
		switch(m_CipherType)
		{
			case SSH_CIPHER_DES:
				DESencryptblk(pktout.data+4, biglen);
				break;

			case SSH_CIPHER_3DES:
				DES3encryptblk(pktout.data+4, biglen);
				break;

//			case SSH_CIPHER_BLOWFISH:
//				m_Bf->EncryptBlk(pktout.data+4, biglen);
//				break;
		}

	CSocket::Send(pktout.data,biglen+4);
}

/*
unsigned long CSSHSocket::CRC32(const void *buf, size_t len) 
{
    unsigned long crcword = 0L;
    const _TUCHAR *p = (const _TUCHAR *) buf;
    while (len--) {
        unsigned long newbyte = *p++;
        newbyte ^= crcword & 0xFFL;
        crcword = (crcword >> 8) ^ crc32_table[newbyte];
    }
    return crcword;
}
*/
int CSSHSocket::ExchangeSSHVersionNumbers(void)
{
	int vMajor, vMinor;
	int i;
	_TCHAR vRemote[256];

	i=CSocket::Receive(m_InputBuffer, 16383-m_InputBufferPos,0);
	if (i<1)
		return -1; //bad protocol

	m_InputBuffer[i]=0;

	i=sscanf((LPTSTR )m_InputBuffer,_T("SSH-%d.%d-%s"), &vMajor, &vMinor, vRemote);
	if (i!=3)
		return -1; // bad protocol

	if (vMajor<1)
		return -2; // version mismatch

	if (vMajor==1 && !vMinor)
		return -3; // doesn't support 1.0

//	if (vMajor>1 || (vMajor==1 && vMinor>5))
//		return -4; // newer version;

	_TCHAR buff[256];
	wsprintf(buff,_T("SSH-%d.%d-Mabry\n\0"), PROTOCOL_MAJOR_VERSION, PROTOCOL_MINOR_VERSION);
	CSocket::Send(buff, _tcsclen(buff),0);
	return 0; // all ok
}

int CSSHSocket::DecryptPacket(void)
{
	long len, biglen;
	int i, pad;

	for (i = len = 0; i < 4; i++) len = (len << 8) + m_InputBufferForReading[i];
	pad = 8 - (len%8);
	biglen = len + pad;

//	if (pktin.data)
//		free(pktin.data);
	pktin.data = (_TUCHAR *)(pktin.data == NULL ? malloc(biglen) : realloc(pktin.data, biglen));
	if (!pktin.data)
		return -1; // out of memory

	pktin.length=biglen-4-pad;

	if(debug)
	{
		if(m_Cipher) OnDebug(_T("RECEIVED ciphertext:"));
			else OnDebug(_T("RECEIVED plaintext:"));
		_TCHAR c=m_InputBufferForReading[biglen+4];
		m_InputBufferForReading[biglen+4]=0;
		OnDebug((LPTSTR )m_InputBufferForReading);
		m_InputBufferForReading[biglen+4]=c;
	   }

	if (m_Cipher)
	    switch (m_CipherType) 
		{
			case SSH_CIPHER_DES: 
				DESdecryptblk((_TUCHAR *)m_InputBufferForReading+4, biglen);
				break;

			case SSH_CIPHER_3DES:
				DES3decryptblk((_TUCHAR *)m_InputBufferForReading+4, biglen);
				break;

//			case SSH_CIPHER_BLOWFISH: 
//				m_Bf->DecryptBlk((unsigned char *)m_InputBufferForReading+4, biglen);
//				break;
    }
		
	if(debug)
	{
		OnDebug(_T("RECEIVED plaintext:"));
		_TCHAR c=m_InputBufferForReading[biglen+4];
		m_InputBufferForReading[biglen+4]=0;
		OnDebug((LPTSTR )m_InputBufferForReading);
		m_InputBufferForReading[biglen+4]=c;
	} 

	memcpy(pktin.data, m_InputBufferForReading+4, biglen);
	pktin.type = pktin.data[pad];
	pktin.body = pktin.data+pad+1;

	i=m_InputBufferPos-biglen-4;
	if (i<0) i=0;
	if (i>0)
	{
		_TCHAR buff[16384];
		memcpy(buff,m_InputBufferForReading+biglen+4,i);
		memcpy(m_InputBufferForReading,buff,i);
		m_InputBufferPos=i;
		m_InputBuffer=m_InputBufferForReading+i;
	}
	else
	{
		m_InputBufferPos=0;
		m_InputBuffer=m_InputBufferForReading;
	}
	return 0;
}

int CSSHSocket::ReadPacket(void)
{
	int i;

		i=CSocket::Receive(m_InputBuffer, 16383-m_InputBufferPos,0);	
		if (i<1 && !m_InputBufferPos)
		{
			OnDebug(_T("Nothing received"));
			return -1;
		}
		if (i>0)
		{
			m_InputBuffer+=i;
			m_InputBufferPos+=i;
		}
		if (DecryptPacket())

		{
			OnDebug(_T("Could not decrypt packet"));
			return -1;
		}
		if (m_InputBufferPos)
		{
			::PostMessage(m_hwndSocket, WM_SOCKET_SSL_NOTIFY, 0,0);
		}
		
		if (pktin.type==SSH_MSG_DEBUG)
		{
			i=pktin.body[3];
			pktin.body[i+4]=0;
			OnDebug((LPTSTR )pktin.body+4);
			return -2;
		}
		if (pktin.type==SSH_SMSG_STDERR_DATA)
		{
			i=pktin.body[3];
			pktin.body[i+4]=0;
			OnDebug(_T("ERROR!"));
			OnDebug((LPTSTR )pktin.body+4);
			return -3;		
		}
		if (pktin.type==SSH_SMSG_EXITSTATUS)
		{
			::PostMessage(m_hwndSocket, WM_SOCKET_NOTIFY, 0,FD_CLOSE);
		}

		return 0;
}

int CSSHSocket::CreateSSHKeypair(void)
{
	int i, j, len;
    _TUCHAR *rsabuf, *keystr1, *keystr2;
    _TUCHAR cookie[8];
    struct RSAKey servkey, hostkey;
    MD5 md5c;
    static unsigned long supported_ciphers_mask, supported_auths_mask;
    static int tried_publickey;
    static _TUCHAR session_id[16];
    static _TCHAR username[100];

    OnDebug(_T("Received public keys"));

    memcpy(cookie, pktin.body, 8);

    i = MakeRSAKey(pktin.body+8, &servkey, &keystr1);
    j = MakeRSAKey(pktin.body+8+i, &hostkey, &keystr2);

	if (debug)
	{
		_TCHAR logmsg[80];
		OnDebug(_T("Host key fingerprint is:"));
		_tcscpy(logmsg, _T("      "));
//		hostkey.comment = NULL;
//		rsa_fingerprint(logmsg+strlen(logmsg), sizeof(logmsg)-strlen(logmsg),&hostkey);
		OnDebug(logmsg);
    }

    supported_ciphers_mask = GET_32BIT(pktin.body+12+i+j);
    supported_auths_mask = GET_32BIT(pktin.body+16+i+j);

    md5c.Update(keystr2, hostkey.bytes);
    md5c.Update(keystr1, servkey.bytes);
    md5c.Update(pktin.body, 8);
    md5c.Final(session_id);

    for (i=0; i<32; i++)
	session_key[i] = RandomByte();

    len = (hostkey.bytes > servkey.bytes ? hostkey.bytes : servkey.bytes);

    rsabuf = (_TUCHAR *)malloc(len);
    if (!rsabuf)
	{
		OnDebug(_T("Out of memory!"));
		free(servkey.modulus);
		free(hostkey.modulus);
		return 0;
	}
	memset(rsabuf,0,len);

    /*
     * Verify the host key.
     */
    {
        /*
         * First format the key into a string.
         */

        int len = RSAStrLen(&hostkey);
        LPTSTR keystr = (LPTSTR )malloc(len);
		int retval=0;
        if (!keystr)
		{
			OnDebug(_T("Out of memory!"));
			free(servkey.modulus);
			free(hostkey.modulus);
			return 0;
		}
        RSAStrFmt(keystr, &hostkey);
		// now send this key to telnetxcom
		if (!((CTelnetXCom*)m_Owner)->CheckServerKey(keystr, len,&retval))
		{
			// user didn't accept this key
			free(keystr);
			::PostMessage(m_hwndSocket, WM_SOCKET_NOTIFY, 0,FD_CLOSE);
			Close();
			free(servkey.modulus);
			free(hostkey.modulus);
			return FALSE;
		}
        free(keystr);  
	}

    for (i=0; i<32; i++) {
	rsabuf[i] = session_key[i];
	if (i < 16)
	    rsabuf[i] ^= session_id[i];
    }

    if (hostkey.bytes > servkey.bytes) {
		RSAEncrypt(rsabuf, 32, &servkey);
		RSAEncrypt(rsabuf, servkey.bytes, &hostkey);
    } else {
		RSAEncrypt(rsabuf, 32, &hostkey);
		RSAEncrypt(rsabuf, hostkey.bytes, &servkey);
    }

    OnDebug(_T("Encrypted session key"));

    if ((supported_ciphers_mask & (1 << m_CipherType)) == 0) 
	{
		OnDebug(_T("Selected cipher not supported, falling back to 3DES"));
		m_CipherType = SSH_CIPHER_3DES;
    }

	if (debug)
	    switch (m_CipherType) 
		{
			case SSH_CIPHER_3DES: 
				OnDebug(_T("Using 3DES encryption"));
				break;

			case SSH_CIPHER_DES:
				OnDebug(_T("Using single-DES encryption")); 
				break;

//			case SSH_CIPHER_BLOWFISH: 
//				OnDebug("Using Blowfish encryption"); 
//				break;
    }

	CreateNewPacket(SSH_CMSG_SESSION_KEY, len+15);
    pktout.body[0] = m_CipherType;
    memcpy(pktout.body+1, cookie, 8);
    pktout.body[9] = (len*8) >> 8;
    pktout.body[10] = (len*8) & 0xFF;
    memcpy(pktout.body+11, rsabuf, len);
    pktout.body[len+11] = pktout.body[len+12] = 0;
    pktout.body[len+13] = pktout.body[len+14] = 0;

    OnDebug(_T("Sending encrypted session key"));
    SendPacket();
    OnDebug(_T("Done"));

    OnDebug(_T("Trying to enable encryption"));


    free(rsabuf);

	m_Cipher=TRUE;
	switch (m_CipherType) 
	{
		case SSH_CIPHER_3DES: 
			DES3sesskey(session_key);
			break;

		case SSH_CIPHER_DES:
			DESsesskey(session_key);
			break;

//		case SSH_CIPHER_BLOWFISH: 
//			m_Bf=new CBlowfish;
//			m_Bf->SessKey(session_key);
//			break;

		default:
			m_Cipher=FALSE;
	}

	free(servkey.modulus);
	free(hostkey.modulus);

	return 1;
}

int CSSHSocket::RandomByte(void) 
{
    _TUCHAR b;
    if(!CryptGenRandom(hCryptProv, 1, &b))
		OnDebug(_T("Random number generator failure"));
    return b;
}


/* ---------------------------------------------------------*
 * DES encryption / decryption functions                    *
 * ---------------------------------------------------------*/


void CSSHSocket::DES3sesskey(_TUCHAR *key) {
    int i, j;
    for(i = 0; i < 2; i++) {
	for(j = 0; j < 3; j++) {
	    hDESKey[i][j] = create_des_key(key + (j * 8));
	}
    }
}


void CSSHSocket::DES3encryptblk(_TUCHAR *blk, int len) {

    DWORD dlen;
    dlen = len;

    if(CryptEncrypt(hDESKey[0][0], 0, FALSE, 0, blk, &dlen, len + 8) == 0)
	OnDebug(_T("Error encrypting block"));
    if(CryptDecrypt(hDESKey[0][1], 0, FALSE, 0, blk, &dlen) == 0)
	OnDebug(_T("Error encrypting block"));
    if(CryptEncrypt(hDESKey[0][2], 0, FALSE, 0, blk, &dlen, len + 8) == 0)
	OnDebug(_T("Error encrypting block"));
}


void CSSHSocket::DES3decryptblk(_TUCHAR *blk, int len) {
    DWORD dlen;
    dlen = len;

    if(CryptDecrypt(hDESKey[1][2], 0, FALSE, 0, blk, &dlen) == 0)
	OnDebug(_T("Error decrypting block"));
    if(CryptEncrypt(hDESKey[1][1], 0, FALSE, 0, blk, &dlen, len + 8) == 0)
	OnDebug(_T("Error decrypting block"));
    if(CryptDecrypt(hDESKey[1][0], 0, FALSE, 0, blk, &dlen) == 0)
	OnDebug(_T("Error decrypting block"));
}


void CSSHSocket::DESsesskey(_TUCHAR *key) {
    int i;
    for(i = 0; i < 2; i++) {
	hDESKey[i][0] = create_des_key(key);
    }
}


void CSSHSocket::DESencryptblk(_TUCHAR *blk, int len) {
    DWORD dlen;
    dlen = len;
    if(CryptEncrypt(hDESKey[0][0], 0, FALSE, 0, blk, &dlen, len + 8) == 0)
	OnDebug(_T("Error encrypting block"));
}


void CSSHSocket::DESdecryptblk(_TUCHAR *blk, int len) {
    DWORD dlen;
    dlen = len;
    if(CryptDecrypt(hDESKey[1][0], 0, FALSE, 0, blk, &dlen) == 0)
	OnDebug(_T("Error decrypting block"));
}

static BYTE PrivateKeyWithExponentOfOne[] =
{
    0x07, 0x02, 0x00, 0x00, 0x00, 0xA4, 0x00, 0x00,
    0x52, 0x53, 0x41, 0x32, 0x00, 0x02, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xAB, 0xEF, 0xFA, 0xC6,
    0x7D, 0xE8, 0xDE, 0xFB, 0x68, 0x38, 0x09, 0x92,
    0xD9, 0x42, 0x7E, 0x6B, 0x89, 0x9E, 0x21, 0xD7,
    0x52, 0x1C, 0x99, 0x3C, 0x17, 0x48, 0x4E, 0x3A,
    0x44, 0x02, 0xF2, 0xFA, 0x74, 0x57, 0xDA, 0xE4,
    0xD3, 0xC0, 0x35, 0x67, 0xFA, 0x6E, 0xDF, 0x78,
    0x4C, 0x75, 0x35, 0x1C, 0xA0, 0x74, 0x49, 0xE3,
    0x20, 0x13, 0x71, 0x35, 0x65, 0xDF, 0x12, 0x20,
    0xF5, 0xF5, 0xF5, 0xC1, 0xED, 0x5C, 0x91, 0x36,
    0x75, 0xB0, 0xA9, 0x9C, 0x04, 0xDB, 0x0C, 0x8C,
    0xBF, 0x99, 0x75, 0x13, 0x7E, 0x87, 0x80, 0x4B,
    0x71, 0x94, 0xB8, 0x00, 0xA0, 0x7D, 0xB7, 0x53,
    0xDD, 0x20, 0x63, 0xEE, 0xF7, 0x83, 0x41, 0xFE,
    0x16, 0xA7, 0x6E, 0xDF, 0x21, 0x7D, 0x76, 0xC0,
    0x85, 0xD5, 0x65, 0x7F, 0x00, 0x23, 0x57, 0x45,
    0x52, 0x02, 0x9D, 0xEA, 0x69, 0xAC, 0x1F, 0xFD,
    0x3F, 0x8C, 0x4A, 0xD0,

    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x64, 0xD5, 0xAA, 0xB1,
    0xA6, 0x03, 0x18, 0x92, 0x03, 0xAA, 0x31, 0x2E,
    0x48, 0x4B, 0x65, 0x20, 0x99, 0xCD, 0xC6, 0x0C,
    0x15, 0x0C, 0xBF, 0x3E, 0xFF, 0x78, 0x95, 0x67,
    0xB1, 0x74, 0x5B, 0x60,

    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

HCRYPTKEY CSSHSocket::create_des_key(_TUCHAR *key) {

    HCRYPTKEY hSessionKey, hPrivateKey;
    DWORD dlen = 8;
    BLOBHEADER *pbh;
    _TCHAR buf[sizeof(BLOBHEADER) + sizeof(ALG_ID) + 256];

    /*
     * Need special private key to encrypt session key so we can
     * import session key, since only encrypted session keys can be
     * imported
     */
    if(CryptImportKey(hCryptProv, PrivateKeyWithExponentOfOne,
		      sizeof(PrivateKeyWithExponentOfOne),
		      0, 0, &hPrivateKey) == 0)
	return 0;

    /* now encrypt session key using special private key */
    memcpy(buf + sizeof(BLOBHEADER) + sizeof(ALG_ID), key, 8);
    if(CryptEncrypt(hPrivateKey, 0, TRUE, 0,(_TUCHAR *)buf + sizeof(BLOBHEADER) + sizeof(ALG_ID),&dlen, 256) == 0)
	return 0;

    /* build session key blob */
    pbh = (BLOBHEADER*)buf;
    pbh->bType = SIMPLEBLOB;
    pbh->bVersion = 0x02;
    pbh->reserved = 0;
    pbh->aiKeyAlg = CALG_DES;
    *((ALG_ID*)(buf+sizeof(BLOBHEADER))) = CALG_RSA_KEYX;

    /* import session key into key container */
    if(CryptImportKey(hCryptProv, (_TUCHAR *)buf,dlen + sizeof(BLOBHEADER) + sizeof(ALG_ID),hPrivateKey, 0, &hSessionKey) == 0)
	return 0;

    if(hPrivateKey)
	CryptDestroyKey(hPrivateKey);

    return hSessionKey;

}

int CSSHSocket::MakeRSAKey(_TUCHAR *data, struct RSAKey *result,_TUCHAR **keystr) 
{

    _TUCHAR *p = data;
    int i;
    int w, b;

    /* get size (bits) of modulus */
    result->bits = 0;
    for(i=0; i<4; i++)
	result->bits = (result->bits << 8) + *p++;

    /* get size (bits) of public exponent */
    w = 0;
    for (i=0; i<2; i++)
	w = (w << 8) + *p++;
    b = (w+7)/8;   /* bits -> bytes */

    /* convert exponent to DWORD */
    result->exponent = 0;
    for (i=0; i<b; i++)
	result->exponent = (result->exponent << 8) + *p++;

    /* get size (bits) of modulus */
    w = 0;
    for (i=0; i<2; i++)
	w = (w << 8) + *p++;
    result->bytes = b = (w+7)/8;   /* bits -> bytes */

    /* allocate buffer for modulus & copy it */
    result->modulus = (_TUCHAR *)malloc(b);
    memcpy(result->modulus, p, b);

    /* update callers pointer */
    if (keystr) *keystr = p;       /* point at key string, second time */

    return (p - data) + b;
}

void CSSHSocket::RSAEncrypt(_TUCHAR *data, int length, struct RSAKey *rsakey) {

    int i;
    _TUCHAR *pKeybuf, *pKeyin;
    HCRYPTKEY hRsaKey;
    PUBLICKEYSTRUC *pBlob;
    RSAPUBKEY *pRPK;
    _TUCHAR *buf;
    DWORD dlen;
    DWORD bufsize;

    /* allocate buffer for public key blob */
    if((pBlob = (struct _PUBLICKEYSTRUC *)malloc(sizeof(PUBLICKEYSTRUC) + sizeof(RSAPUBKEY) + rsakey->bytes)) == NULL)
	OnDebug(_T("Out of memory"));

    /* allocate buffer for message encryption block */
    bufsize = (length + rsakey->bytes) << 1;
    if((buf = (_TUCHAR *)malloc(bufsize)) == NULL)
	OnDebug(_T("Out of memory"));

    /* construct public key blob from host public key */
    pKeybuf = ((_TUCHAR*)pBlob) + sizeof(PUBLICKEYSTRUC) +
	sizeof(RSAPUBKEY);
    pKeyin = ((_TUCHAR*)rsakey->modulus);
    /* change big endian to little endian */
    for(i=0; i<rsakey->bytes; i++)
	pKeybuf[i] = pKeyin[rsakey->bytes-i-1];
    pBlob->bType = PUBLICKEYBLOB;
    pBlob->bVersion = 0x02;
    pBlob->reserved = 0;
    pBlob->aiKeyAlg = CALG_RSA_KEYX;
    pRPK = (RSAPUBKEY*)(((_TUCHAR*)pBlob) + sizeof(PUBLICKEYSTRUC));
    pRPK->magic = 0x31415352; /* "RSA1" */
    pRPK->bitlen = rsakey->bits;
    pRPK->pubexp = rsakey->exponent;

    /* import public key blob into key container */
    if(CryptImportKey(hCryptProv, (const _TUCHAR *)pBlob, sizeof(PUBLICKEYSTRUC)+sizeof(RSAPUBKEY)+rsakey->bytes,0, 0, &hRsaKey) == 0)
	OnDebug(_T("Error importing RSA key!"));

    /* copy message into buffer */
    memcpy(buf, data, length);
    dlen = length;

    /* using host public key, encrypt the message */
    if(CryptEncrypt(hRsaKey, 0, TRUE, 0, buf, &dlen, bufsize) == 0)
	OnDebug(_T("Error encrypting using RSA key!"));

    /*
     * For some strange reason, Microsoft CryptEncrypt using public
     * key, returns the cyphertext in backwards (little endian)
     * order, so reverse it!
     */
    for(i = 0; i < (int)dlen; i++)
	data[i] = buf[dlen - i - 1]; /* make it big endian */

    CryptDestroyKey(hRsaKey);
    free(buf);
    free(pBlob);
}

int CSSHSocket::RSAStrLen(struct RSAKey *key) 
{
    return 2 * (sizeof(DWORD) + key->bytes) + 10;
}


void CSSHSocket::RSAStrFmt(LPTSTR str, struct RSAKey *key) 
{

    int len = 0, i;

    wsprintf(str+len, _T("%04x"), key->exponent);
    len += _tcsclen(str+len);

    str[len++] = _T('/');
    for (i=1; i<key->bytes; i++) {
	wsprintf(str+len, _T("%02x"), key->modulus[i]);
	len += _tcsclen(str+len);
    }
    str[len] = _T('\0');
}
