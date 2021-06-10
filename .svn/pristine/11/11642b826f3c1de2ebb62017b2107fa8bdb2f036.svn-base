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

#ifndef _CPPUNIT
    #include "SMStoreModeState.h"
#else
    #include "FakeSMStoreModeStateBase.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmSelectModeOfOperationBase : public STATE(StoreModeState)
{
public:
#ifndef _CPPUNIT
	SMSmSelectModeOfOperationBase();
	virtual SMStateBase  *Initialize(void);
	virtual void UnInitialize(void);		// RFC 330050
#endif // _CPPUNIT
  
	
protected:
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton1(void);
#endif // _CPPUNIT
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButton4(void);
  virtual SMStateBase  *PSButton8(void);

  // NewUId
  virtual SMStateBase *PSReceiptUp(void);     // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  // NewUI
//private:
#endif
  virtual void   showScreen(const bool traceOn);
#ifndef _CPPUNIT
  // TAR 313180 start
  virtual void   DisableButtons(void);
  virtual void   ReenableButtons(void);
  virtual void   SetDeviceEnabled(long nDeviceClass, bool fEnabled, bool fCheckHealth); //Tar 450827

	
protected:
   bool   m_bButtonEnabled[8];
   bool   m_bButtonEnabledSet;
  // TAR 313180 end
#endif // _CPPUNIT
  

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
