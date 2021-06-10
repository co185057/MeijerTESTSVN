//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmUtility.h
//
// TITLE: Class header for store mode Utility state
//
// Description: This state inherits from StoreModeState.  
//
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMUtilitySTATE
#define _SMUtilitySTATE

#include "SMSmUtilityBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmUtility : public SMSmUtilityBase
{
public:
    SMSmUtility();
    virtual SMStateBase  *Initialize(void);
    virtual bool  storeMode();
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton5(void);
    virtual SMStateBase  *PSButton6(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase * PSButtonGoBack();
    virtual SMStateBase * TimedOut(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual bool getOperatorPassword();
    virtual void  showScreen(bool passwordVerified);
    virtual void UtilityExitFastLane(void);
    virtual void UtilityGetDiagFiles(void);
    virtual void UtilityRebootEntireSystem(void);
    virtual void UtilityShutdownEntireSystem(void);
    virtual void UtilityVolumeControl(void);
    virtual void UtilityMultimediaTrainingProgram(void);
    virtual bool UtilityValidateOperator(CString csOperator, CString csPassword);
    virtual SMStateBase * stateAfterPasswordVerify(void);

    DECLARE_DYNCREATE(SMSmUtility)// MFC Runtime class/object information
};

#endif