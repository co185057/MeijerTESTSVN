//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfServiceBase.h
//
// TITLE: Class header for wait for helpf for bad card state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _OutOfServiceBaseSTATE
#define _OutOfServiceBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMOutOfServiceBase : public SMCustomerModeState
{
public:
	SMOutOfServiceBase(const int);
	SMOutOfServiceBase();
	SMStateBase *Deliver(const int);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase *Initialize(void);
  virtual void     UnInitialize(void);

  virtual SMStateBase *InitializeAll(void);
  virtual SMStateBase *UnInitializeAll(void);
  virtual SMStateBase *ProcessMaintenance(void);

  // Touchscreen Motion sensor enables 7883 scanner motor
  // RFC 219283 (& 218445) DE/RF
  void MotionSensorStart();
  void MotionSensorEnd();
  static int MotionSensorThreadEntry(void *p);
  void MotionSensorThread();
	
private:
  void SCDownload(int nTerminalNumber);  // TAR 287143
  bool	COInitialize(int nTerminalNumber, bool bLocal = false);	// caller should provide valid SCOT terminal number
  bool	SCOTTENDInitialize(int nTerminalNumber, bool bLocal = false);	// caller should provide valid SCOT terminal number
  SMStateBase* InitConfigData();	// call this to do a selective initialization required to reload options
  void UnInitConfigData();			// call this to do a selective uninitialization required to unload options

protected:
  virtual void TBShow2x20(void);
  virtual void UpdateTransactionCountRegistry(void);

//private:
  int nReason;
	
  DECLARE_DYNCREATE(SMOutOfServiceBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(OutOfService)

#endif
