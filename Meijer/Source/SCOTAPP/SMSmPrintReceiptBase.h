//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmPrintReceiptBase.h
//
// TITLE: Class header for store mode Print Receipt state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmPrintReceiptBaseSTATE
#define _SmPrintReceiptBaseSTATE

#include "SMStoreModeState.h"
#include "SMSmReportsMenuBase.h"


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmPrintReceiptBase : public STATE(StoreModeState)
{
public:
  SMSmPrintReceiptBase();
  virtual ~SMSmPrintReceiptBase();
  virtual SMStateBase  *Initialize(void);
  virtual void	 UnInitialize(void);

protected:
  virtual SMStateBase  *PSButton1(void);  
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton8(void); 

  DECLARE_DYNCREATE(SMSmPrintReceiptBase)// MFC Runtime class/object information
};
#endif
