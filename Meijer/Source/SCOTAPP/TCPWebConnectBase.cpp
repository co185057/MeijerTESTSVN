// TCPWebConnect.cpp: implementation of the CTCPWebConnectBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _CPPUNIT
#include "TestMacros.h"
#include "FakeCommon.h"
#endif //_CPPUNIT

#include "TCPWebConnectBase.h"
#include "FLTCPDefines.h"
#include "TCPConnectConstants.h"

#define COMP_ID ID_GP
#define T_ID    _T("TCPWebConnect")

#ifndef _CPPUNIT
#include "MethodTrace.h"
#endif //_CPPUNIT


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCPWebConnectBase::CTCPWebConnectBase()
{

}

CTCPWebConnectBase::~CTCPWebConnectBase()
{

}

void CTCPWebConnectBase::Initialize()
{
 
    TRACE_METHOD_AT_LEVEL(_T("CTCPWebConnectBase::Initialize"),7);
#ifndef _CPPUNIT
    HRESULT hr = pObjConnect.CreateInstance(__uuidof(FLTCP::TCPConnector));
#endif

    if (pObjConnect)
    {
        _variant_t vtPointer;
        FPTCPEVENTHANDLER fpnEventHandler;
        fpnEventHandler = TCPCallBack_Handler;
        vtPointer.plVal = (long *)fpnEventHandler;
        pObjConnect->SetCallback(vtPointer);
        trace(L6, _T("Successfully created FLTCPConnect object"));
    }
    trace(L6, _T("-Initialize()"));
}

void CTCPWebConnectBase::UnInitialize()
{
    TRACE_METHOD_AT_LEVEL(_T("CTCPWebConnectBase::UnInitialize"),7);
 
    if (pObjConnect != NULL)
    {
#ifndef _CPPUNIT
        pObjConnect.Release();
#else
        delete pObjConnect;
#endif
        pObjConnect = NULL;
    }
}

void CTCPWebConnectBase::Connect(int nDeviceID, BSTR bstrHostName, long lPortNum)
{
    TRACE_METHOD_AT_LEVEL(_T("CTCPWebConnectBase::Connect"),7);
    if (pObjConnect)
    {
        VARIANT_BOOL bDeviceExists = VARIANT_FALSE;
        pObjConnect->DeviceExists(&bDeviceExists, nDeviceID);
        if (bDeviceExists == VARIANT_FALSE)
        {
            pObjConnect->Connect(nDeviceID, bstrHostName, lPortNum);
            trace(L6, _T("CTCPWebConnectBase::Connect - Device %d connection created"), nDeviceID);
        }
        else
        {
            trace(L6, _T("CTCPWebConnectBase::Connect - Device %d connection already created"), nDeviceID);
        }
    }
}

void CTCPWebConnectBase::Disconnect(int nDeviceID)
{
    TRACE_METHOD_AT_LEVEL(_T("CTCPWebConnectBase::Disconnect"),7);
    if (pObjConnect)
    {
        VARIANT_BOOL bDeviceExists = VARIANT_FALSE;
        pObjConnect->DeviceExists(&bDeviceExists, nDeviceID);
        if (bDeviceExists == VARIANT_TRUE)
        {
            pObjConnect->Disconnect(nDeviceID);
            trace(L6, _T("CTCPWebConnectBase::Disconnect - Device %d connection disconnect"), nDeviceID);
        }
        else
        {
            trace(L6, _T("CTCPWebConnectBase::Disconnect - Device %d connection does not exist"), nDeviceID);
        }
    }
}

void CTCPWebConnectBase::Listen(int nDeviceID)
{
    TRACE_METHOD_AT_LEVEL(_T("CTCPWebConnectBase::Listen"),7);
    if (pObjConnect != NULL)
    {
        VARIANT_BOOL bDeviceExists = VARIANT_FALSE;
        pObjConnect->DeviceExists(&bDeviceExists, nDeviceID);
        if (bDeviceExists == VARIANT_TRUE)
        {
            pObjConnect->Listen(nDeviceID);
        }
        else
        {
            trace(L6, _T("CTCPWebConnectBase::Listen - Device %d connection does not exist"), nDeviceID);
        }
    }
}

BOOL CTCPWebConnectBase::SendString(int nDeviceID, short sMsgFormat, short sMsgFormatExt, short appID, long lAppData, BSTR bstrSendString)
{
    TRACE_METHOD_AT_LEVEL(_T("CTCPWebConnectBase::SendString"),7);
    BOOL retVal = FALSE;
    if (pObjConnect != NULL)
    {
        VARIANT_BOOL bDeviceExists = VARIANT_FALSE;
        pObjConnect->DeviceExists(&bDeviceExists, nDeviceID);
        if (bDeviceExists)
        {
          retVal = pObjConnect->SendString(nDeviceID, sMsgFormat, sMsgFormatExt, appID, lAppData, bstrSendString);
        }
        else
        {
            trace(L6, _T("CTCPWebConnectBase::SendString - Device %d connection does not exist"), nDeviceID);
        }
    }
    return retVal;
}

BOOL CTCPWebConnectBase::SendBytes(int nDeviceID, long lByteCount, short sMsgFormat, short sMsgFormatExt,  short appID, long lAppData, BYTE* pData)
{
    TRACE_METHOD_AT_LEVEL(_T("CTCPWebConnectBase::SendBytes"),7);
    BOOL retVal = FALSE;
    if (pObjConnect != NULL)
    {
        VARIANT_BOOL bDeviceExists = VARIANT_FALSE;
        pObjConnect->DeviceExists(&bDeviceExists, nDeviceID);
        if (bDeviceExists)
        {
            retVal = pObjConnect->SendBytes(nDeviceID, lByteCount, sMsgFormat, sMsgFormatExt, appID, lAppData, pData);
        }
        else
        {
            trace(L6, _T("CTCPWebConnectBase::SendBytes - Device %d connection does not exist"), nDeviceID);
        }
    }
    return retVal;
}

BOOL CALLBACK CTCPWebConnectBase::TCPCallBack_Handler(const long Msg,
                                 const WPARAM wParam,
                                 const LPARAM lParam)
{
  int nWhat = (int)wParam;
  long lParm = 0;
  long wParm = 0;

    if (TCP_DISCONNECTED == Msg)
    {
        // Disconnect
        mo.PostGPwithParams((GPEVT) GP_TCP_DISCONNECTED, wParam, lParam);
    }
    else if (TCP_CONNECTED == Msg)
    {
        mo.PostGPwithParams((GPEVT) GP_TCP_CONNECTED, wParam, lParam);
    }
    else if (TCP_RECEIVED == Msg)
    {
        
        BYTE* pMsg = (BYTE*) lParam;
        //BYTE* pMsgData = &pMsg[sizeof(structMsgHeader)];
        structMsgHeader* pHeader = (structMsgHeader*) pMsg;
        if (pHeader->sAppID == TCP_APPID_PRIMERDATA)
            return TRUE;
        BYTE* pMsgCpy = new BYTE[pHeader->length+sizeof(_TCHAR)];
        
        memset(pMsgCpy, 0, pHeader->length+sizeof(_TCHAR));
        
        memcpy(pMsgCpy, pMsg, pHeader->length);
        mo.PostGPwithParams((GPEVT) GP_TCP_RECEIVED, wParam, (LPARAM) pMsgCpy);
    }
    

    return TRUE;
}

