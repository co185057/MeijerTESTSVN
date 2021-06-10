//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForApprovalBase.h
//
// TITLE: Class header for wait for approval state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WaitForApprovalBaseSTATE
#define _WaitForApprovalBaseSTATE

#include "SMOperatorPasswordState.h"
//#include "SMSmAbort.h"				// Tar 194454 RF 020702
//#include "SMSmAuthorization.h"		// Tar 194454 RF 020702

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMWaitForApprovalBase : public STATE(OperatorPasswordState)
{
public:
	SMWaitForApprovalBase(const bool fTV, // transaction verify
	                  const bool fVT, const bool bVerifySig=false);// void transaction
	SMWaitForApprovalBase();
	SMStateBase *Deliver(const bool fTV, // transaction verify
	                  const bool fVT, const bool bVerifySig=false);
  virtual SMStateBase * Initialize(void);
  virtual SMStateBase * TimedOut(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
  virtual SMStateBase  *OnWtDecrease(void);
  virtual void UnInitialize(void);  //TAR198810
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *OnCouponSensor(void);	//TAR 386740
	
protected:
  virtual SMStateBase  *PSButtonHelp(void);
  virtual SMStateBase  *PSCancelKey(void);   // Override where needed
  virtual SMStateBase  *PSButton1();         // Store Sign In
  virtual SMStateBase  *OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual void showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual SMStateBase  *OnUnDeActivatedTheftTag(void);

//private:
  bool fVoidTrx;
  bool m_bVerifySig ;

  int  nSaidCount;
	
  DECLARE_DYNCREATE(SMWaitForApprovalBase)// MFC Runtime class/object information
};

//DECLARE_STATE(WaitForApproval)

#endif
