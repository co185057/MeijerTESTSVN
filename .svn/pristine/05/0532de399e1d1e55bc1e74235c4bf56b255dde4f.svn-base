//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    CollectorBase.h
//
// TITLE:   Collect data for Reports Purposes
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// CollectorBase
// This class collects and writes various data for FastLane Reports.
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CCOLLECTORBASE
#define _CCOLLECTORBASE
#include "CmClass.h"
#include "FLTransporter.h"
#ifndef NO_LOGGING
#include "ReportingErrors.h"
#endif


 
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 CCollectorBase  : public CObject
{
public:
    CCollectorBase();
    virtual ~CCollectorBase();
    virtual long TransactionCompleted(CString csTerminalNumber);
    void LogError(DWORD eventID, 
                 _bstr_t &functionName, 
                 WORD eventType, 
                 long errorCode = -1,
                 long errorCode2 = -1,
                 BSTR Description = NULL);

    void LogComError(_com_error &e, 
                     _bstr_t &functionName);
    void LogOLEError(COleDispatchException *e, 
                     _bstr_t &functionName);

	inline CString GetServerSharePoint() const { return m_Transporter.GetServerSharePoint(); }
	inline CString GetServerName() const { return m_Transporter.GetServerName(); }

protected:
    virtual long WriteReportData(BSTR bxml);
    virtual long CreateDOM();
    MSXML2::IXMLDOMDocumentPtr  m_pNewTransacationDOM;
    bool m_bTransactionCompleted;
    bool m_bCoInitialized;
    HANDLE m_EventSource;
    CFLTransporter m_Transporter;

DECLARE_DYNCREATE(CCollectorBase)// MFC Runtime class/object information
};

#endif

