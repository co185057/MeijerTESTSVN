//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditWtExpBase.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmmEditWtExpBaseSTATE
#define _SmmEditWtExpBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmmEditWtExpBase : public STATE(StoreModeState)
{
public:
  SMSmmEditWtExpBase();							 // constructor
  virtual SMStateBase * Initialize(void);        // required override
  virtual void     UnInitialize(void);			 // required override
	
protected:
  virtual SMStateBase  *PSButton1(void);         // button 1
  virtual SMStateBase  *PSButton2(void);         // button 2
  virtual SMStateBase  *PSButton3(void);         // button 3
  virtual SMStateBase  *PSButton4(void);         // button 3
  virtual SMStateBase  *PSButton8(void);         // go back

  DECLARE_DYNCREATE(SMSmmEditWtExpBase)// MFC Runtime class/object information
};

//DECLARE_STATE(SmmEditWtExp)

#endif
