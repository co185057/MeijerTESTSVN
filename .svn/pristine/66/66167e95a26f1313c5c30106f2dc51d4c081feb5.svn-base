//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterIDBase.h
//
// TITLE: Class header for Enter ID state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERIDBASESTATE
#define _ENTERIDBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

SMEnterIDBase : public SMCustomerModeState
{

public:
   SMEnterIDBase();
   virtual SMStateBase  *Initialize(void);
   virtual void UnInitialize(void);		// RFC 330050

protected:
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSButtonGoBack(void);

  DECLARE_DYNCREATE(SMEnterIDBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(EnterID)

#endif
