///////////////////////////////////////////////////////////////////////////////////////////
//
//
// TITLE:   MobileCartBase.CPP
//          Manages the mobile cart
//
// AUTHOR:  Barbara Dawkins
// 
/////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////////////////////////////////

#define _WIN32_WINNT 0x0400
#include "stdafx.h"
#ifndef _CPPUNIT
#include "Common.h"
#include "TraceObject.h"
#else
#include "FakeCommon.h"
#endif
#include "MobileCartBase.h"
#include "MobileCartParser.h"
#include "FLTCPDefines.h"
#include "TCPConnectConstants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMP_ID ID_CU               // Main Procedure
#define T_ID _T("CMobileCartBase")

//DLLIMPORT extern CTCPWebConnect &tcpConnect;

IMPLEMENT_DYNCREATE(CMobileCartBase, CObject)// MFC Runtime class/object information

///////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////

/**  
Constructor for CMobileCartBase object
*/
CMobileCartBase::CMobileCartBase()
{
    m_bSendFinishTransaction = true;
}

///////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////

/// Destructor
/**  
Destructor for CMobileCartBase object
*/
CMobileCartBase::~CMobileCartBase()
{
#ifndef _CPPUNIT
    if (m_bInitialized)
        UnInitialize();
#endif
}


///////////////////////////////////////////////////////////////////////////////////
// UnInitialize for shutdown
///////////////////////////////////////////////////////////////////////////////////

/// UnInitialize
/**
Uninitialize the object and free all resources.
*/
void CMobileCartBase::UnInitialize()
{
    trace(L6, _T("+UnInitialize()"));
    if (m_bInitialized)
    {
            m_bInitialized = false;
    }
}

///////////////////////////////////////////////////////////////////////////////////
// Initialize
///////////////////////////////////////////////////////////////////////////////////

/// Initialize
/**
Initialize the object and allocate all required resources.
   @param[out] retVal Error code (Always zero for now)
*/
long CMobileCartBase::Initialize(CString csStoreNum, int nTerminalNum)
{
    long rc = 0;

    m_csStoreNum = csStoreNum;
    m_csTerminalNum.Format(_T("%03d"), nTerminalNum);
    trace(L6, _T("+Initialize()"));

    if (!m_bInitialized)
    {
        m_bInitialized = true;
    }
    else
        trace(L6, _T("Initialize - Already initialized"));

    trace(L6, _T("-Initialize() = %d"), rc);

    return rc;
   
}

void CMobileCartBase::SetSendFinishTransaction(bool bSendFinishTransaction)
{
    m_bSendFinishTransaction = bSendFinishTransaction;
}

///////////////////////////////////////////////////////////////////////////////////
// SendReadyForTransaction
///////////////////////////////////////////////////////////////////////////////////

///  SendReadyForTransaction
/**
This method is called by the application when it is ready to receive a mobile transaction
to process. The application displays a QRCode containing a GUID that the mobile app
will send with its cart to a server. This method initiates a read from the server for a
cart associated with the specified GUID. This read from the server will continue until
the application calls the NotReadyForTransaction method.
   @param[in] bstrCartGuid This specifies the GUID displayed by the application that will
   be associated with a mobile cart
*/
int CMobileCartBase::SendReadyForTransaction()
{
#ifndef _CPPUNIT
    trace(L6, _T("+SendReadyForTransaction()"));
    HRESULT hRes = tcpConnect.SendString(TCP_CONNECTION_1, MSG_FORMAT_XML, MSG_FMT1, 1, 1, _T(""));
    trace(L6, _T("-SendReadyForTransaction result = %d"), hRes);
#endif
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// NotReadyForTransaction
///////////////////////////////////////////////////////////////////////////////////

///  NotReadyForTransaction
/**
This method is called by the application when it is not ready to receive a mobile transaction
to process. It will cancel any outstanding read of the server for a mobile transaction cart
to process.
*/
void CMobileCartBase::NotReadyForTransaction()
{
#ifndef _CPPUNIT
    trace(L6, _T("+NotReadyForTransaction()"));
    HRESULT hRes = tcpConnect.SendString(TCP_CONNECTION_1, MSG_FORMAT_XML, MSG_FMT1, 1, 2, _T(""));
    trace(L6, _T("-NotReadyForTransaction result = %d"), hRes);
#endif
}

int CMobileCartBase::SendFinishTransaction(long lTotal, int nTransactionNumber)
{
    int rc = 0;
#ifndef _CPPUNIT

    if (!m_bSendFinishTransaction)
        return rc;

    trace(L6, _T("+SendFinishTransaction()"));
    CSingleLock lock(&m_CriticalRequest);
    lock.Lock();
    
    mobileCartParser.SetTotalDue(lTotal);
    mobileCartParser.SetPOSTransactioNum(nTransactionNumber);

    _bstr_t bstrUploadCart = mobileCartParser.GetFinishRequest();
    long len = SysStringLen(bstrUploadCart);

    if (len)
    {
        tcpConnect.SendBytes(TCP_CONNECTION_1, len+1,  MSG_FORMAT_XML, MSG_FMT1, 1, 3, (BYTE*) (char*) bstrUploadCart);
    }
    
    
    trace(L6, _T("-SendFinishTransaction()"));
    lock.Unlock();
#endif
    return rc;
}


_bstr_t CMobileCartBase::ProcessCart(_bstr_t bstrCartData)
{
    _bstr_t bstrResult = mobileCartParser.ProcessCart(bstrCartData); 
    return bstrResult;
}


