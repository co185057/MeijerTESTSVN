//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSelectModeOfOperationBase.h
//
// TITLE: Class header for store mode Select Mode of Operation state
//
// Description: This state inherits from StoreModeState.  The screen
//   is accessible from Assist Mode.
//
//
// AUTHOR:  Renato Taer
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmSelectModeOfOperationBaseSTATE
#define _SmSelectModeOfOperationBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmSelectModeOfOperationBase : public STATE(StoreModeState)
{
public:
	SMSmSelectModeOfOperationBase();
	virtual SMStateBase  *Initialize(void);
	virtual void UnInitialize(void);		// RFC 330050

  
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton4(void);
  virtual SMStateBase  *PSButton8(void);

  // NewUId
  virtual SMStateBase *PSReceiptUp(void);     // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI
//private:
  virtual void   showScreen(const bool traceOn);
  // TAR 313180 start
  virtual void   DisableButtons(void);
  virtual void   ReenableButtons(void);
  virtual void   SetDeviceEnabled(bool fEnabled);

	
protected:
   bool   m_bButtonEnabled[8];
   bool   m_bButtonEnabledSet;
  // TAR 313180 end

  

  DECLARE_DYNCREATE(SMSmSelectModeOfOperationBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmSelectModeOfOperation(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmSelectModeOfOperationBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmSelectModeOfOperation objects.  This function is called by
// RETURNSTATE(SmSelectModeOfOperation) instead of calling new STATE(SmSelectModeOfOperation) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmSelectModeOfOperationBase.cpp and SMSmSelectModeOfOperation.cpp
// by using the macro IMPLEMENT_STATE(SmSelectModeOfOperation)

//DECLARE_STATE(SmSelectModeOfOperation)

#endif
