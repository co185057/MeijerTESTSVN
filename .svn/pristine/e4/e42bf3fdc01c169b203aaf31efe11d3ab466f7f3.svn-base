///////////////////////////////////////////////////////////////////////////////////////////
//
//
// TITLE:   MobileCart.CPP
//          Manages the mobile cart
//
// AUTHOR:  Barbara Dawkins
// 
/////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////////////////////////////////

#define _WIN32_WINNT 0x0400
#include "stdafx.h"
#include "Common.h"
#include "TraceObject.h"
#include "MobileCart.h"
#include "MobileCartParser.h"
#include "FLTCPDefines.h"
#include "TCPConnectConstants.h"
#include "TCPConnectConstantsSSF.h"
#include "MobileCartAudit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMobileCartAudit mobileAudit;

#define COMP_ID ID_CU               // Main Procedure
#define T_ID _T("CMobileCart")

//DLLIMPORT extern CTCPWebConnect &tcpConnect;

//IMPLEMENT_DYNCREATE(CMobileCart, CObject)// MFC Runtime class/object information

///////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////

/**  
Constructor for CMobileCart object
*/
CMobileCart::CMobileCart()
{
    m_bSendFinishTransaction = true;   //UploadTicket Port
	
}

///////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////

/// Destructor
/**  
Destructor for CMobileCart object
*/
CMobileCart::~CMobileCart()
{
    //UploadTicket Port
    if (m_bInitialized)   //UploadTicket Port
        UnInitialize();
}


///////////////////////////////////////////////////////////////////////////////////
// UnInitialize for shutdown
///////////////////////////////////////////////////////////////////////////////////

/// UnInitialize
/**
Uninitialize the object and free all resources.
*/
void CMobileCart::UnInitialize()
{
    //UploadTicket Port
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
//+UploadTicket Port
long CMobileCart::Initialize(CString csStoreNum, int nTerminalNum)
{
    long rc = 0;

    m_csStoreNum = csStoreNum;
    m_csTerminalNum.Format(_T("%03d"), nTerminalNum);

    trace(L6, _T("+Initialize()"));

	m_bAbortingAudit = false;

    if (!m_bInitialized)
    {
        m_bInitialized = true;
    }
    else
        trace(L6, _T("Initialize - Already initialized"));

    trace(L6, _T("-Initialize() = %d"), rc);

    return rc;
   
}

void CMobileCart::SetSendFinishTransaction(bool bSendFinishTransaction)
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
*/
int CMobileCart::SendReadyForTransaction()
{

    trace(L6, _T("+SendReadyForTransaction()"));
    HRESULT hRes = tcpConnect.SendString(TCP_CONNECTION_1, MSG_FORMAT_XML, MSG_FMT1, 1, TCP_MOBILECARTMSG_READYFORTRANSACTION, _bstr_t(_T("")));
    trace(L6, _T("-SendReadyForTransaction result = %d"), hRes);

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
void CMobileCart::NotReadyForTransaction()
{

    trace(L6, _T("+NotReadyForTransaction()"));
    HRESULT hRes = tcpConnect.SendString(TCP_CONNECTION_1, MSG_FORMAT_XML, MSG_FMT1, 1, TCP_MOBILECARTMSG_NOTREADYFORTRANSACTION, _bstr_t(_T("")));
    trace(L6, _T("-NotReadyForTransaction result = %d"), hRes);

}


int CMobileCart::SendFinishTransaction(long lTotal, int nTransactionNumber)
{
    trace(L6, _T("+CMobileCart::SendFinishTransaction(). lTotal=%d, nTransactionNumber=%d"), lTotal, nTransactionNumber);
    int rc = 0;

    trace(L6, _T("+SendFinishTransaction() m_bSendFinishTransaction = %d"), m_bSendFinishTransaction);
    if (!m_bSendFinishTransaction)
        return rc;


    CSingleLock lock(&m_CriticalRequest);
    lock.Lock();
    
    mobileCartParser.SetTotalDue(lTotal);
    mobileCartParser.SetPOSTransactioNum(nTransactionNumber);

    _bstr_t bstrUploadCart = mobileCartParser.GetFinishRequest();
    long len = SysStringLen(bstrUploadCart);

    if (len)
    {
        tcpConnect.SendBytes(TCP_CONNECTION_1, len+1,  MSG_FORMAT_XML, MSG_FMT1, 1, TCP_MOBILECARTMSG_UPLOADCARTINFO, (BYTE*) (char*) bstrUploadCart);
    }
      
    trace(L6, _T("-CMobileCart::SendFinishTransaction()"));
    lock.Unlock();

    return rc;
}


_bstr_t CMobileCart::ProcessCart(_bstr_t bstrCartData)
{
    trace(L6, _T("+ProcessCart()"));

    _bstr_t bstrResult = mobileCartParser.ProcessCart(bstrCartData); 
    if (_tcscmp(bstrResult, MOBILE_RESCAN_REQUIRED) == 0)
    {
        mobileAudit.Initialize(bstrCartData);
    }
	else
	{
		
		trace(L6, _T("+ProcessCart() NoSOAP bstrCartData len: %d"), bstrCartData.length());
		//trace(L6, _T("+ProcessCart() no soap: %s"), bstrCartData);

		//No Audit, send soap to POS Server
		_bstr_t soapForPOS = NoSOAPForYou(bstrCartData);
		
		long len = SysStringLen(soapForPOS);
		int intlen = soapForPOS.length();
		
		CString cStringSoapForPOS = (LPCTSTR) soapForPOS;
		int intcslen = cStringSoapForPOS.GetLength();		
	
		trace(L6, _T("+ProcessCart() no soap len: %d intlen: %d, intcslen: %d"), len, intlen, intcslen);

		
		//trace(L6, _T("+ProcessCart() About to print Dump: "));
		//traceStack(L6);


		ExtractTransactionIdFromCartData(bstrCartData);	//CRD 177093
		ExtractCardNumberFromCartData(bstrCartData);	//CRD 224739

		//msg id for FastLaneSvc
		int TCP_MOBILECARTMSG_SEND_TO_POS = 100;

		BYTE* pData = (BYTE*) (char*) soapForPOS;
		int pDataLen = intcslen * sizeof(char);

		trace(L6, _T("CMobileCart pDataLen: %d, pData[%d]: %d, pData[%d]: %d, pData[%d]: %d, pData[%d]: %d"),
			pDataLen,
			pDataLen - 4, pData[pDataLen - 4], 
			pDataLen - 3, pData[pDataLen - 3],
			pDataLen - 2, pData[pDataLen - 2],
			pDataLen - 1, pData[pDataLen - 1]);

		//CRD 299891
        if(!m_bAbortingAudit)
        {				
            //send xml to FastLaneSvc
            // NOTICE: The intcslen is not passing a +1
            BOOL bSuccess = FALSE; 
            bSuccess = tcpConnect.SendBytes(
                TCP_CONNECTION_1, 
                intcslen,
                MSG_FORMAT_XML,
                MSG_FMT1,
                1, 
                TCP_MOBILECARTMSG_SEND_TO_POS, 
                pData);
            
            trace(L6, _T("CMobileCart::TCP_MOBILECARTMSG_SEND_TO_POS - sent. without -1."));
        }
        else
        {
            trace(L6, _T("CMobileCart::Service Check Abort Not sending to XML to ACS."));            
        }

	}
    return bstrResult;
}




void CMobileCart::SendTransactionStatus(_bstr_t bstrStatusString)
{
	// Start CRD SSCOIA-39227
    trace(L6, _T("+SendTransactionStatus(%s) - NOT Sending string to FL Services"), (LPCTSTR) bstrStatusString);
    //HRESULT hRes = tcpConnect.SendString(TCP_CONNECTION_1, MSG_FORMAT_XML, MSG_FMT1, 1, TCP_MOBILECARTMSG_SSCO_TRANSACTION_STATUS, _bstr_t(_T("")));
    //trace(L6, _T("-SendTransactionStatus result = %d"), hRes);
	// End CRD SSCOIA-39227

}
//-UploadTicket Port

_bstr_t CMobileCart::NoSOAPForYou(_bstr_t bstrCartData){
	CString csCartDataTemp;
	csCartDataTemp = (LPCTSTR) bstrCartData;
	//int foo = csCartDataTemp.Replace(_T("<"), _T("&lt;"));
	//int bar = csCartDataTemp.Replace(_T(">"), _T("&gt;"));
	
	csCartDataTemp.Format(_T("%s%s%s"), SOAP_HEADER, csCartDataTemp,SOAP_FOOTER);
	trace(L6, _T("CMobileCart::No SOAP for you: %s"), csCartDataTemp);
	
	return csCartDataTemp;

}




void CMobileCart::ExtractTransactionIdFromCartData(_bstr_t bstrCartData)
{
	CString csCartDataTemp;
	csCartDataTemp = (LPCTSTR) bstrCartData;
 
	int nPosStart = csCartDataTemp.Find(_T("TransactionNumber")) + 18;
	int nPosEnd =  csCartDataTemp.Find(_T("</TransactionNumber"));

	csTransactionId.Format(_T("%s"), csCartDataTemp.Mid(nPosStart, nPosEnd-nPosStart));
	trace(L6, _T("CMobileCart::TransactionId: %s"), csTransactionId);
}

CString CMobileCart::GetTransactionId()
{
	CString csIDtemp = csTransactionId;
	//length must be 5
	int nLen = csIDtemp.GetLength();
	int iDiff = 5 - nLen;
	if(iDiff > 0)
	{
		CString csZero;
		for (int i = 0; i < iDiff; i++)
		{
			csZero.Format(_T("%s0"), csZero);
		}				
		csIDtemp.Format(_T("%s%s"), csZero, csIDtemp);
	}
	else
	{
		csIDtemp.Mid(0,5);
	}
	
	trace(L6, _T("CMobileCart::GetTransactionId - %s"), csIDtemp);

	return csIDtemp;
}

//Start CRD 224739
void CMobileCart::ExtractCardNumberFromCartData(_bstr_t bstrCartData)
{
	CString csCartDataTemp;
	csCartDataTemp = (LPCTSTR) bstrCartData;
	
	int nPosStart = csCartDataTemp.Find(_T("CardNumber")) + 11;
	int nPosEnd =  csCartDataTemp.Find(_T("</CardNumber"));
	
	csCardNumber.Format(_T("%s"), csCartDataTemp.Mid(nPosStart, nPosEnd-nPosStart));
	trace(L6, _T("CMobileCart::csCardNumber: %s"), csCardNumber);
}

CString CMobileCart::GetCardNumber()
{
	return csCardNumber;
}
//End CRD 224739


// CRD 299891
void CMobileCart::SetAbortingAudit(bool b_abort)
{
	m_bAbortingAudit = b_abort;
}

bool CMobileCart::GetAbortingAudit()
{
	return m_bAbortingAudit;
}
// CRD 299891	