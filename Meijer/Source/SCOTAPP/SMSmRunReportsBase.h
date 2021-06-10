//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRunReportsBase.h
//
// TITLE: Class header for store mode Run Reports state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmRunReportsBaseSTATE
#define _SmRunReportsBaseSTATE

#ifndef _CPPUNIT
#include "SMStoreModeState.h"
#include "SigReceiptDataFile.h"
#include "SMSmRunReportsBase.h"
//#import <TelnetClientSupport.dll> no_namespace
#include <TelnetClientSupport.tlh>
#else
#include "FakeSMStoreModeStateBase.h"
#endif


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmRunReportsBase : public STATE(StoreModeState)
{
public:
  SMSmRunReportsBase();
#ifndef _CPPUNIT
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
#endif

protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton1(void);  //for PrintReport module
  virtual SMStateBase  *PSButton8(void);  //for Go-back
  virtual long OpenPrintFile(void);
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);   // to disable and enable scroll buttons when 
#endif
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI	
#ifndef _CPPUNIT
  virtual void UnInitialize(void);		// RFC 330050
#endif

  DECLARE_DYNCREATE(SMSmRunReportsBase)// MFC Runtime class/object information
};
#endif
