//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSystemFunctionsBase.h
//
// TITLE: Class header for store mode System Functions state
//
// Description: This state inherits from StoreModeState.  The screen
//   is accessible from Assist Mode.
//
//
// AUTHOR:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmSystemFunctionsBaseSTATE
#define _SmSystemFunctionsBaseSTATE

#include "SMStoreModeState.h"
#include "SMStoreModeMenu.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmSystemFunctionsBase : public STATE(StoreModeMenu)
{
public:
	SMSmSystemFunctionsBase();
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton4(void);
  virtual SMStateBase  *PSButton5(void);
  //Begin_Suspend
  virtual SMStateBase  *PSButton6(void);
  //End_Suspend
  virtual SMStateBase  *PSButton7(void);
  virtual SMStateBase  *PSButton8(void);
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *DMEncryptorCompleted(void);	// master key complete was done
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);     // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI
//private:
  virtual void   showScreen(const bool traceOn);
  virtual long   CopyLogsToServer(void);
  virtual bool   inEncryptorMode(void);
  // TAR 313180 start
  virtual void   DisableButtons(void);
  virtual void   ReenableButtons(void);
  
  virtual SMStateBase *OnGoBackButton(void);

  virtual void ShowPrintBarcodeButton();
  virtual void ShowSecurityMaintButton();
  virtual void ShowModeOfOperationButton();
  virtual void ShowLoadOptionsButton();
  virtual void ShowUtilityButton();
	
private:
   bool   bButtonEnabled[8];
   bool   bButtonEnabledSet;
  // TAR 313180 end

  DECLARE_DYNCREATE(SMSmSystemFunctionsBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmSystemFunctions(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmSystemFunctionsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmSystemFunctions objects.  This function is called by
// RETURNSTATE(SmSystemFunctions) instead of calling new STATE(SmSystemFunctions) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmSystemFunctionsBase.cpp and SMSmSystemFunctions.cpp
// by using the macro IMPLEMENT_STATE(SmSystemFunctions)

//DECLARE_STATE(SmSystemFunctions)

#endif
