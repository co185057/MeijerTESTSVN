//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmEnterBirthdateBase.h
//
// TITLE: Class header for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmEnterBirthdateBaseSTATE
#define _SmEnterBirthdateBaseSTATE

#include "SMStoreModeState.h"
#include "SMSmRestrictedItems.h"

#ifdef _SCOTSSF_

class DLLIMPORT
#else
class DLLEXPORT
#endif

SMSmEnterBirthdateBase : public STATE(SmRestrictedItems)
{

public:
          SMSmEnterBirthdateBase();		     // constructor
  virtual SMStateBase * Initialize(void);        // required override
  virtual void          UnInitialize(void);
	
protected:

  virtual SMStateBase  *PSButton8(void);         //
  virtual SMStateBase  *PSEnterKey(void);        // override for enter key
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSNumericKey(const long);	// a key 0-9 was pressed

  boolean bShowApproveButton;

  DECLARE_DYNCREATE(SMSmEnterBirthdateBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmEnterBirthdate(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmEnterBirthdateBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmEnterBirthdate objects.  This function is called by
// RETURNSTATE(SmEnterBirthdate) instead of calling new STATE(SmEnterBirthdate) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmEnterBirthdateBase.cpp and SMSmEnterBirthdate.cpp
// by using the macro IMPLEMENT_STATE(SmEnterBirthdate)

//DECLARE_STATE(SmEnterBirthdate)

#endif
