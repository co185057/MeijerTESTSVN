//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmAddWeightBase.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmmAddWeightBaseSTATE
#define _SmmAddWeightBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmmAddWeightBase : public STATE(StoreModeState)
{
public:
  SMSmmAddWeightBase();							// constructor
  virtual SMStateBase  *Initialize(void);       // required override
  virtual SMStateBase  *SASMMBagScaleEvent(void);
  virtual void UnInitialize(void);
	
protected:
  virtual SMStateBase  *PSButton8(void);        // go back

  DECLARE_DYNCREATE(SMSmmAddWeightBase)// MFC Runtime class/object information
};

//DECLARE_STATE(SmmAddWeight)

#endif
