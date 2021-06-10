// TCPWebConnect.h: interface for the CTCPWebConnect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPWEBCONNECT_H__40E2F99B_CC40_4550_A6FF_4095DA280FA5__INCLUDED_)
#define AFX_TCPWEBCONNECT_H__40E2F99B_CC40_4550_A6FF_4095DA280FA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TCPWebConnectBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 CTCPWebConnect  :  public CTCPWebConnectBase
{
public:
	CTCPWebConnect();
	virtual ~CTCPWebConnect();
    virtual void Initialize();
    virtual void UnInitialize();
    virtual void Connect(int nDeviceID, BSTR bstrHostName, long lPortNum);
    virtual void Disconnect(int nDeviceID);
    virtual void Listen(int nDeviceID);
	virtual BOOL SendBytes(int nDeviceID, long lByteCount, short sMsgFormat, short sMsgFormatExt,  short appID, long lAppData, BYTE* pData);

	BOOL SendString(int nDeviceID, short sMsgFormat, short sMsgFormatExt, short appID, long lAppData, BSTR bstrSendString);

};
#endif // !defined(AFX_TCPWEBCONNECT_H__40E2F99B_CC40_4550_A6FF_4095DA280FA5__INCLUDED_)
