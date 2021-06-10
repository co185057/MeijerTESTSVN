//
// Copyright (c) 2000 Kresimir Petric, Mabry Software
//
#ifndef __SSH_SOCKET
#define __SSH_SOCKET


typedef enum SecuredStates
{
	SecuredStateNotConnected,
	SecuredStateConnecting,
	SecuredStateSentVersionNumber,
	SecuredStateSentKeys,
	SecuredStateSentUsername,
	SecuredStateSentPassword,
	SecuredStateRequestTerminal,
	SecuredStateExecShell,
	SecuredStateOk
} SecuredStates;


// external funcs needed here
class MD5;

struct Packet {
    long length;
    int type;
    _TUCHAR *data;
    _TUCHAR *body;
    long savedpos;
    long maxlen;
};

struct RSAKey {
    int bits;
    int bytes;
    unsigned long exponent;
    _TUCHAR *modulus;
    LPTSTR comment;
};



class CSSHSocket : public CSocket
{
	
public:
	CSSHSocket( CTelnetXCom* pCtrl, LPTSTR libname);
	~CSSHSocket( void );
	CTelnetXCom *m_pCtrl;

	ProtocolsEnum			m_Secured;
	SecuredStates			m_State;

	struct Packet			pktin,
							pktout;
	BOOL					debug;
	unsigned char			session_key[32];
	CString					m_Hostname;
	int						m_Port;

	BOOL					m_Cipher;
	int						m_CipherType;
	CString					m_Username;
	CString					m_Password;
	CString					m_TerminalType;
	CString					m_Command;
	void					*m_Owner;


protected:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnTimer(void);
	virtual void OnDebug(LPTSTR msg);

public:
	BOOL Connect(LPTSTR lpszHostAddress, UINT nHostPort);
	int Send(const void* lpBuf, int nBufLen, int nFlags=0);
	int Receive(void* lpBuf, int nBufLen, int nFlags=0);


// stuff for SSL

	// variables
	unsigned char			m_InputBufferForReading[16384];
	unsigned char			m_OutputBufferForWriting[16384];
	unsigned char			*m_InputBuffer;
	unsigned char			*m_OutputBuffer;
public:
	unsigned int			m_InputBufferPos;
	unsigned int			m_OutputBufferPos;
protected:

	void CreateNewPacket(int type, int len);
	void SendPacket(void);
	unsigned long CRC32(const void *buf, size_t len);
	int ExchangeSSHVersionNumbers(void);
	int ReadPacket(void);
	int DecryptPacket(void);
	int CreateSSHKeypair(void);
	int MakeRSAKey(_TUCHAR *data, struct RSAKey *result,_TUCHAR **keystr);
	int RandomByte(void);
	void RSAEncrypt(_TUCHAR *data, int length, struct RSAKey *rsakey);
	int RSAStrLen(struct RSAKey *key);
	void RSAStrFmt(LPTSTR str, struct RSAKey *key);
	void DES3sesskey(_TUCHAR *key);
	void DES3encryptblk(_TUCHAR *blk, int len);
	void DES3decryptblk(_TUCHAR *blk, int len);
	void DESsesskey(_TUCHAR *key);
	void DESencryptblk(_TUCHAR *blk, int len);
	void DESdecryptblk(_TUCHAR *blk, int len);
	HCRYPTKEY create_des_key(_TUCHAR *key);
};

#endif