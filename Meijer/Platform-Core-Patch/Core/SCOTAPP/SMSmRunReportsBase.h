//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRunReportsBase.h
//
// TITLE: Class header for store mode Run Reports state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmRunReportsBaseSTATE
#define _SmRunReportsBaseSTATE

#include "SMStoreModeState.h"
#include "SigReceiptDataFile.h"
#include "SMSmReportsMenuBase.h"

//#import <TelnetClientSupport.dll> no_namespace
#include <TelnetClientSupport.tlh>

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmRunReportsBase : public STATE(StoreModeState)
{
public:
  SMSmRunReportsBase();
  virtual ~SMSmRunReportsBase();
  virtual SMStateBase  *Initialize(void);
  long PrintReport();
  CString GetNameOfFileToPrint(void);   
  void  ReleaseReportPrint (void);
  void ClosePrintFile(void);
  HFILE GetReportPrint(void); 
  CString ReportFileToPrint(void){return m_csReportFileToPrint;};
  void SetReportFileToPrint(CString name){m_csReportFileToPrint=name;};
  CStdioFile m_csfTmp;
  CString m_csReportFileToPrint;
  virtual void DMReceiptLine(const CString& csPrintLine); //397105

protected:
  virtual SMStateBase  *PSButton1(void);  //for PrintReport module
  virtual SMStateBase  *PSButton8(void);  //for Go-back
  virtual long OpenPrintFile(void);
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);   // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI	
  virtual void UnInitialize(void);		// RFC 330050

  DECLARE_DYNCREATE(SMSmRunReportsBase)// MFC Runtime class/object information
};
#endif
