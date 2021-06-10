//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    MobileCart.h
//
// TITLE:   Receives mobile cart from cloud and updates it as required
//
// AUTHOR:  Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// MobileCart
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CMobileCart
#define _CMobileCart

#include <map>
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

//#include "xmlHttpEvent.h"


#include <GTOU.tlh>
//class CMobileCartParser;

#include "MobileCartBase.h"


#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 CMobileCart  : public CMobileCartBase
{
public:
    CMobileCart();
    ~CMobileCart();
    virtual long Initialize(CString csStoreNum, int nTerminalNum);
    virtual void UnInitialize();
    virtual int SendReadyForTransaction();
    virtual void NotReadyForTransaction();
    virtual int SendFinishTransaction(long lTotal, int nTransactionNumber);
    virtual _bstr_t ProcessCart(_bstr_t bstrCartData);
    void SetSendFinishTransaction(bool bSendFinishTransaction);
    void SendTransactionStatus(_bstr_t bstrStatusString);
	virtual _bstr_t NoSOAPForYou(_bstr_t bstrCartData);
	virtual void ExtractTransactionIdFromCartData(_bstr_t bstrCartData);
	virtual CString GetTransactionId();

	//Start CRD 224739
	virtual void ExtractCardNumberFromCartData(_bstr_t bstrCartData);
	virtual CString GetCardNumber();
	//End CRD 224739

	//CRT 299891
	virtual void SetAbortingAudit(bool abort);
	virtual bool GetAbortingAudit();


protected:
    bool m_bInitialized;           // Initialization complete
    CCriticalSection m_CriticalRequest;
    CString m_csStoreNum;
    CString m_csTerminalNum;
    bool m_bSendFinishTransaction;
	bool m_bAbortingAudit;

	CString csTransactionId;	//CRD 177093
	CString csCardNumber;		//CRD 224739	

    //DECLARE_DYNCREATE(CMobileCart)// MFC Runtime class/object information
};
    //service soap headers to connect to acs server for suspend/resume.
    const TCHAR SOAP_HEADER[] = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?><SuspendTransaction xmlns=\"http://tempuri.org/\"><v_strXmlRequestDoc>");
	const TCHAR SOAP_FOOTER[] = _T("</v_strXmlRequestDoc><r_strXmlResponseDoc/><v_strApplicationID>TestApp</v_strApplicationID></SuspendTransaction>");
    //original version of the service. Service runs on ssco now. 
    //const TCHAR SOAP_HEADER[] = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><soap:Body><SuspendTransaction xmlns=\"http://tempuri.org/\"><v_strXmlRequestDoc>");
	//const TCHAR SOAP_FOOTER[] = _T("</v_strXmlRequestDoc><r_strXmlResponseDoc /><v_strApplicationID>TestApp</v_strApplicationID></SuspendTransaction></soap:Body></soap:Envelope>");
    
#endif

