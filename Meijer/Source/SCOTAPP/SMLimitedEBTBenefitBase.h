//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLimitedEBTBenefitBase.h
//
// TITLE: Class header for Limited EBT Benefit
//
//
// AUTHOR:    Michael Prather
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LimitedEBTBenefitBaseSTATE
#define _LimitedEBTBenefitBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMLimitedEBTBenefitBase : public SMCustomerModeState
{
protected:
  TBTENDERTYPE nTndrType;
	long lTenderLimit;

public:
  SMLimitedEBTBenefitBase(const TBTENDERTYPE nTyp,const long lLimit);
  SMLimitedEBTBenefitBase();
  SMStateBase *Deliver(const TBTENDERTYPE nTyp,const long lLimit);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);

  virtual SMStateBase  *OnWtDecrease(void);
  virtual void UnInitialize(void);		// RFC 330050
	
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  CString csCurContext;    // Tar 238987
	
  DECLARE_DYNCREATE(SMLimitedEBTBenefitBase)// MFC Runtime class/object information
};

//DECLARE_STATE(LimitedEBTBenefit)

#endif
