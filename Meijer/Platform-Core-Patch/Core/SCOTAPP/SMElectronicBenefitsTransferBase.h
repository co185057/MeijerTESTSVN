//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMElectronicBenefitsTransferBase.H
//
// TITLE:    Class header for Electronic Benefits Transfer State
//
//
// AUTHOR:   Michael Prather
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ElectronicBenefitsTransferBaseSTATE
#define _ElectronicBenefitsTransferBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMElectronicBenefitsTransferBase : public SMCustomerModeState
{
private:
	bool fCashBack;

public:
  SMElectronicBenefitsTransferBase(const bool);
  SMElectronicBenefitsTransferBase();
  SMStateBase *Deliver(const bool);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

  virtual SMStateBase  *OnWtDecrease(void);
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *PSCancelKey(void);
  virtual void UnInitialize(void);		// RFC 330050

  bool bShowFoodStamps;
	
  DECLARE_DYNCREATE(SMElectronicBenefitsTransferBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(ElectronicBenefitsTransfer)

#endif
