//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmUtilityBase.h
//
// TITLE: Class header for store mode Utility state
//
//
// AUTHOR:  Feng Zhang
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmUtilityBaseSTATE
#define _SmUtilityBaseSTATE

#include "SMStoreModeState.h"
#include "SMOperatorPasswordState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif   //STATE(StoreModeState)//,
  SMSmUtilityBase : public  STATE(OperatorPasswordState)
{
public:
	SMSmUtilityBase();
  virtual SMStateBase  *Initialize(void);
  virtual bool  storeMode()  { return true; }
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton4(void);
  virtual SMStateBase  *PSButton5(void);
  virtual SMStateBase  *PSButton6(void);
  virtual SMStateBase * PSButtonGoBack();
  virtual SMStateBase * TimedOut(void);
  //Button6, 7 is blank
  virtual SMStateBase  *PSButton8(void);
  virtual SMStateBase *PSReceiptUp(void);     // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
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


  int m_nButtonPressed;  //Which button pressed on Utility screen
  static bool m_ButtonDisable;    //Disable all buttons. Default is false

  DECLARE_DYNCREATE(SMSmUtilityBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmUtility(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmUtilityBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmUtility objects.  This function is called by
// RETURNSTATE(SmUtility) instead of calling new STATE(SmUtility) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmUtilityBase.cpp and SMSmUtility.cpp
// by using the macro IMPLEMENT_STATE(SmUtility)

//DECLARE_STATE(SmUtility)

#endif
