//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMOutOfServiceBase.h
//
// TITLE: Class header for wait for helpf for bad card state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _OutOfServiceBaseSTATE
#define _OutOfServiceBaseSTATE

#ifndef _CPPUNIT
#include "SMCustomerModeState.h"
#else
#include "FakeSMCustomerModeState.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMOutOfServiceBase : public SMCUSTOMERMODESTATE
{
public:
#ifndef _CPPUNIT
	SMOutOfServiceBase(const int);
#endif // _CPPUNIT
	SMOutOfServiceBase();
#ifndef _CPPUNIT
	SMStateBase *Deliver(const int);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase *Initialize(void);
  virtual void     UnInitialize(void);

#endif // _CPPUNIT
  virtual SMStateBase *InitializeAll(void);
  virtual SMStateBase *UnInitializeAll(void);

  virtual SMStateBase *ProcessMaintenance(void);

  // Touchscreen Motion sensor enables 7883 scanner motor
  // RFC 219283 (& 218445) DE/RF
  void MotionSensorStart();
  void MotionSensorEnd();
#ifndef _CPPUNIT
  static int MotionSensorThreadEntry(void *p);
  void MotionSensorThread();

  virtual long DMConfigureScaleDisplay(void);       // RFC 445940
	
#endif // _CPPUNIT
private:
  void SCDownload(int nTerminalNumber);  // TAR 287143
  bool	COInitialize(int nTerminalNumber, bool bLocal = false);	// caller should provide valid SCOT terminal number
  bool	SCOTTENDInitialize(int nTerminalNumber, bool bLocal = false);	// caller should provide valid SCOT terminal number
#ifndef _CPPUNIT
  SMStateBase* InitConfigData();	// call this to do a selective initialization required to reload options
  void UnInitConfigData();			// call this to do a selective uninitialization required to unload options

#endif // _CPPUNIT
protected:
#ifndef _CPPUNIT
  virtual void TBShow2x20(void);
#endif // _CPPUNIT
  virtual void UpdateTransactionCountRegistry(void);
#ifndef _CPPUNIT

  virtual SMStateBase * DMHandleStartupError(void);  // RFC 445940
  virtual long DMPrintVersion(void);                 // RFC 445940
  virtual long SecMgrInitialize(void);               // TAR 452531.
  

#endif // _CPPUNIT
//private:
  int nReason;
  bool m_bDoLegacyDeviceErrs;                        // TAR 452531

  DECLARE_DYNCREATE(SMOutOfServiceBase)// MFC Runtime class/object information

};

//DECLARE_STATE(OutOfService)

#endif
