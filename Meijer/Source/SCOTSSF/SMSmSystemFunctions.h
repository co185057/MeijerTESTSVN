//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSystemFunctions.h
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

#ifndef _SMSYSTEMFUNCTIONSSTATE
#define _SMSYSTEMFUNCTIONSSTATE

#include "SMSmSystemFunctionsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmSystemFunctions : public SMSmSystemFunctionsBase
{
public:
    SMSmSystemFunctions();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
	virtual SMStateBase	 *IdleTimer(const long);   // idle between messages

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton5(void);
    virtual SMStateBase  *PSButton6(void);
    virtual SMStateBase  *PSButton7(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *DMEncryptorCompleted(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual void   showScreen(const bool traceOn);
    virtual long   CopyLogsToServer(void);
    virtual bool   inEncryptorMode(void);
    virtual void   DisableButtons(void);
    virtual void   ReenableButtons(void);
    virtual SMStateBase *OnGoBackButton(void);
    virtual void ShowPrintBarcodeButton();
    virtual void ShowSecurityMaintButton();
    virtual void ShowModeOfOperationButton();
    virtual void ShowLoadOptionsButton();
    virtual void ShowUtilityButton();

    DECLARE_DYNCREATE(SMSmSystemFunctions)// MFC Runtime class/object information
};

#endif