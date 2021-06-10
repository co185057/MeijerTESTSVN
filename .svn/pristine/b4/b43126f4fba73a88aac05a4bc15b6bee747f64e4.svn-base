// TCPWebConnect.h: interface for the CTCPWebConnectBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPWEBCONNECTBASE_H__40E2F99B_CC40_4550_A6FF_4095DA280FA5__INCLUDED_)
#define AFX_TCPWEBCONNECTBASE_H__40E2F99B_CC40_4550_A6FF_4095DA280FA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




#ifndef _CPPUNIT
#include "DllDefine.h"
#include "FLTCPConnectorU.tlh"
#else
#include "FakeCommon.h"
#include "FakeFLTCPConnector.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 CTCPWebConnectBase  
{
public:
	CTCPWebConnectBase();
	virtual ~CTCPWebConnectBase();

    virtual void Initialize();
    virtual void UnInitialize();
    virtual void Connect(int nDeviceID, BSTR bstrHostName, long lPortNum);
    virtual void Disconnect(int nDeviceID);
    virtual void Listen(int nDeviceID);

	virtual BOOL SendString(int nDeviceID, short sMsgFormat, short sMsgFormatExt, short appID, long lAppData, BSTR bstrSendString);
	virtual BOOL SendBytes(int nDeviceID, long lByteCount, short sMsgFormat, short sMsgFormatExt,  short appID, long lAppData, BYTE* pData);
    static BOOL CALLBACK TCPCallBack_Handler(const long Msg,
								 const WPARAM wParam,
								 const LPARAM lParam);
protected:
#ifndef _CPPUNIT
    FLTCP::IFLTCPConnectorPtr pObjConnect;
#else
    CFakeFLTCPConnector* pObjConnect;
#endif

};

#endif // !defined(AFX_TCPWEBCONNECTBASE_H__40E2F99B_CC40_4550_A6FF_4095DA280FA5__INCLUDED_)
