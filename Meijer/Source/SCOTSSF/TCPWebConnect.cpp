// TCPWebConnect.cpp: implementation of the CTCPWebConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DllDefine.h"
#include "FLTCPDefines.h"
#include "TCPConnectConstants.h"
#include "TCPWebConnect.h"
#include "CommonServices.h" 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCPWebConnect::CTCPWebConnect()
{

}

CTCPWebConnect::~CTCPWebConnect()
{

}

void CTCPWebConnect::Initialize()
{
    CTCPWebConnectBase::Initialize();
}

void CTCPWebConnect::UnInitialize()
{
    CTCPWebConnectBase::UnInitialize();
}

void CTCPWebConnect::Connect(int nDeviceID, BSTR bstrHostName, long lPortNum)
{
    CTCPWebConnectBase::Connect(nDeviceID, bstrHostName, lPortNum);
}

void CTCPWebConnect::Disconnect(int nDeviceID)
{
    CTCPWebConnectBase::Disconnect(nDeviceID);
}

void CTCPWebConnect::Listen(int nDeviceID)
{
    CTCPWebConnectBase::Listen(nDeviceID);
}

BOOL CTCPWebConnect::SendString(int nDeviceID, short sMsgFormat, short sMsgFormatExt, short appID, long lAppData, BSTR bstrSendString)
{
    return CTCPWebConnectBase::SendString(nDeviceID, sMsgFormat, sMsgFormatExt, appID, lAppData, bstrSendString);
}

BOOL CTCPWebConnect::SendBytes(int nDeviceID, long lByteCount, short sMsgFormat, short sMsgFormatExt,  short appID, long lAppData, BYTE* pData)
{
    return CTCPWebConnectBase::SendBytes(nDeviceID, lByteCount, sMsgFormat, sMsgFormatExt, appID, lAppData, pData);
}

