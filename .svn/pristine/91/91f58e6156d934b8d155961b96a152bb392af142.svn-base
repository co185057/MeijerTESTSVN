//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmReportMenuBase.h
//
// TITLE: Class header for Reports state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmReportsMenuBaseSTATE
#define _SmReportsMenuBaseSTATE

#ifndef _CPPUNIT
#include "SMStoreModeState.h"
#include "SigReceiptDataFile.h"
#include "SMSmRunReportsBase.h"
#else
#include "FakeSMStoreModeStateBase.h"
#endif

#ifndef _CPPUNIT
enum REPORTSTATUS
{
    RP_NOTOPEN = 0,
    RP_OPENED,
    RP_CLOSED
};
#endif //_CPPUNIT


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmReportsMenuBase : public STATE(StoreModeState)
{
public:
  SMSmReportsMenuBase();
#ifndef _CPPUNIT
  virtual ~SMSmReportsMenuBase();
  virtual SMStateBase  *Initialize(void);  
  virtual void	 UnInitialize(void);
  // Open report file to print and close report file.
  // Reporting related routines.
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
#endif //_CPPUNIT
protected:
#ifndef _CPPUNIT
	CString m_csPartialString;
  virtual SMStateBase  *PSButton1(void);//Select
  virtual SMStateBase  *PSButton2(void);//PrintlastSignature
  virtual SMStateBase  *PSButton3(void);//Signature
  virtual SMStateBase  *PSButton4(void);//ReConnent 
  virtual SMStateBase  *PSButton7(void);//EODReportPeriod
  virtual SMStateBase  *PSButton8(void);//Goback-exit
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
  virtual bool  SigReceiptFileFound() ;                  // check if the s-receipt.scp exists
  virtual void  PrintSigReceiptFile() ; 
  CSigReceiptDataFile m_SigReceipt;
  HANDLE m_EventSource;
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);   // to disable and enable scroll buttons when 
#endif //_CPPUNIT
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI	
  //Handling of Character occupying double space
#ifndef _CPPUNIT
  virtual CString HandleColumnarLine(const CString &csData);
  virtual CString HandleCenterLine(const CString &csData);
  virtual CString HandleOther(const CString &csData);
#endif //_CPPUNIT

private:
//  CStdioFile m_csfCurrentReport;
#ifndef _CPPUNIT
  CFile m_csfCurrentReport;
  CString m_csFilePath;
  bool m_bReportCompletedSuccessfully;
  bool m_bProcessingMenus;
  bool m_bShowLastReceipt;
  bool m_bSignature;
  long m_lReportStatus;
  bool m_bSignaturesPrinted;
  bool m_bErrorLineReceived;
  bool m_bpressed;
  long m_lEODInterval;
  CTime *pctBusinessDate;
  CTime *pctCurrentDate;
  long m_lReportLineIndex;
  CString m_csErrorLine;
  bool m_bRunEOD;
  bool m_bProcessingReport;			//RFC 262343
  int m_nPrintReceiptBttnTextId;
#endif //_CPPUNIT

  DECLARE_DYNCREATE(SMSmReportsMenuBase)    // MFC Runtime class/object information
};

#endif
