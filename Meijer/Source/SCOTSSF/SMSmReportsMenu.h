//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:	SMSmReportsMenu.h
//
// TITLE:	Class header for reports menu state
//
//
// AUTHOR:  Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMREPORTSMENUSTATE
#define _SMREPORTSMENUSTATE


#include "SMSmReportsMenuBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmReportsMenu : public SMSmReportsMenuBase
{
public:
    SMSmReportsMenu();
    virtual SMStateBase  *Initialize(void);  
    virtual void	 UnInitialize(void);
    virtual void WriteRecord(CString csData);
    virtual void OpenFile(void);
    virtual void CloseFile(void);
    virtual void GoOffline(void);
    virtual void LogError(DWORD eventID, 
        _bstr_t &functionName, 
        WORD eventType, 
        long errorCode = -1,
        long errorCode2 = -1,
        BSTR Description = NULL);
    virtual void LogComError(_com_error &e, 
        _bstr_t &functionName);
    virtual void LogOLEError(COleDispatchException *e, 
        _bstr_t &functionName);
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton7(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *OnRPReceived(long byteCount);
    virtual SMStateBase  *OnRPDisconnect();
    virtual SMStateBase  *SendRPDisconnect();
    virtual SMStateBase  *SendRPConnect();
    virtual long AddToReport(const CString& text, const CString &csOption);
    virtual void ParseString(CString csInString);
    virtual void ParseReportLine(CString &csReportLine);
    virtual bool ParsePrompt(CString &csReportLine, bool &bKeepGoing);
    virtual bool ParseMenuLine(CString &csReportLine, bool &bKeepGoing);
    virtual bool ParseErrorLine(CString &csReportLine);
    virtual bool ParseStartReport(CString &csReportLine);
    virtual bool  SigReceiptFileFound() ;
    virtual void  PrintSigReceiptFile() ;
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual CString HandleColumnarLine(const CString &csData);
    virtual CString HandleCenterLine(const CString &csData);
    virtual CString HandleOther(const CString &csData);

    DECLARE_DYNCREATE(SMSmReportsMenu)// MFC Runtime class/object information
};

#endif


