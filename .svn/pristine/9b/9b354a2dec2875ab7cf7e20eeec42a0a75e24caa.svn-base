//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRunReports.h
//
// TITLE: Class header for store mode RunReports state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMRunReportsSTATE
#define _SMRunReportsSTATE

#include "SMSmRunReportsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmRunReports : public SMSmRunReportsBase
{
public:
    SMSmRunReports();
 
    virtual SMStateBase  *Initialize(void);
    virtual void DMReceiptLine(const CString& csPrintLine);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton8(void);
    virtual long OpenPrintFile(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual void UnInitialize(void);

    DECLARE_DYNCREATE(SMSmRunReports)// MFC Runtime class/object information
};

#endif
