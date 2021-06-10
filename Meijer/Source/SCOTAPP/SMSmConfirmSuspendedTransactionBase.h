//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmSuspendedTransactionBase.h
//
// TITLE: Class header for store mode ConfirmSuspendedTransaction state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmConfirmSuspendedTransactionBaseSTATE
#define _SmConfirmSuspendedTransactionBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmConfirmSuspendedTransactionBase : public STATE(StoreModeState)
{
public:
	SMSmConfirmSuspendedTransactionBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);   // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI	
  DECLARE_DYNCREATE(SMSmConfirmSuspendedTransactionBase)// MFC Runtime class/object information
};

//DECLARE_STATE(SmConfirmSuspendedTransaction)

#endif
