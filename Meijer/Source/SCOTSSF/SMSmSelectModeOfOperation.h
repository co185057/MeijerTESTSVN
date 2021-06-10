//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSelectModeOfOperation.h
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

#ifndef _SMSELECTMODEOFOPERATIONSTATE
#define _SMSELECTMODEOFOPERATIONSTATE

#include "SMSmSelectModeOfOperationBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmSelectModeOfOperation : public SMSmSelectModeOfOperationBase
{
public:
    SMSmSelectModeOfOperation();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual void   showScreen(const bool traceOn);
    virtual void   DisableButtons(void);
    virtual void   ReenableButtons(void);
    virtual void   SetDeviceEnabled(long nDeviceClass, bool fEnabled, bool fCheckHealth);


    DECLARE_DYNCREATE(SMSmSelectModeOfOperation)// MFC Runtime class/object information
};

#endif