//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRestrictedNotAllowedBase.h
//
// TITLE: Class header for Restricted Item State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RESTRICTEDNOTALLOWEDBASESTATE
#define _RESTRICTEDNOTALLOWEDBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMRestrictedNotAllowedBase : public SMCustomerModeState
{
public:
	SMRestrictedNotAllowedBase(const PSTEXT);
	SMRestrictedNotAllowedBase();
	SMStateBase *Deliver(const PSTEXT);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase *Initialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
  virtual void UnInitialize(void);		// RFC 330050
protected:
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSEnterKey(void);


//private:
	PSTEXT nMessage;
	
  DECLARE_DYNCREATE(SMRestrictedNotAllowedBase)// MFC Runtime class/object information
};

//DECLARE_STATE(RestrictedNotAllowed)

#endif
