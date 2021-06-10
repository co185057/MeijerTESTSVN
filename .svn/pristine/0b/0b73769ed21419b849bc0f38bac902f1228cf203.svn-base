//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmEditAddBase.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmmEditAddBaseSTATE
#define _SmmEditAddBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmmEditAddBase : public STATE(StoreModeState)
{
public:
  SMSmmEditAddBase();							// constructor
  virtual SMStateBase  *Initialize(void);       // required override
  virtual void	 UnInitialize(void);

protected:
  virtual SMStateBase  *DMScanner(void);         // override for DM scanner
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton4(void);
  virtual SMStateBase  *PSButton5(void);
  virtual SMStateBase  *PSButton6(void);
  virtual SMStateBase  *PSButton8(void);
  //*** NewUI ***//
  virtual SMStateBase *PSReceiptUp(void);   
  virtual SMStateBase *PSReceiptDown(void);
  //*** end of NewUI ***//

  DECLARE_DYNCREATE(SMSmmEditAddBase)// MFC Runtime class/object information
};

//DECLARE_STATE(SmmEditAdd)

#endif
