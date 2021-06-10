#include "StdAfx.h"
#include "connectbase.h"

#define T_ID "ConnectBase(Mgr)"

bool CConnectBase::bTerminate=false;
HANDLE CConnectBase::m_hTerminate = NULL;

/////////////////////////////////////////////////////////////////////////////////
//CConnectData 
CConnectData::CConnectData()
{
	socketAddr="";
	socketPort=0;
	pipeName="";
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
CConnectData::CConnectData(const CConnectData &oData)
{
	socketAddr=		oData.socketAddr;
	hWorkerThread=	oData.hWorkerThread;
	socketPort=		oData.socketPort;
	pipeName=		oData.pipeName;
	bReader=	      oData.bReader;
	nHeaderVersion=oData.nHeaderVersion;
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
		hWorkerThread=	oData.hWorkerThread;
		socketPort=		oData.socketPort;
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

bool CConnectBase::Attach(HANDLE handle)
{
	ATLASSERT(m_handle == INVALID_HANDLE_VALUE);
	m_handle = handle;
	ATLASSERT(m_handle != INVALID_HANDLE_VALUE);

	if (m_hWnd)
		PostMessage(m_hWnd, WMU_CONNINSERT, 0, (LPARAM) this);

	return (m_handle != INVALID_HANDLE_VALUE);
}

bool CConnectBase::Accept()
{
	return true;
}

void CConnectBase::OnAccept(HANDLE)
{
}

int CConnectBase::Receive(void *lpBuf, int nBufLen, int nFlags)
{
	return 0;
}

int CConnectBase::Send(CConnectBase *pConnSource, const void *lpBuf, int len)
{
	return 0;
}

int CConnectBase::SendCount(CConnectBase *pConnSource, const void *lpBuf, int len)
{
	return Send(pConnSource, lpBuf, len);
}

bool CConnectBase::ReConnect()
{
	return true;
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
