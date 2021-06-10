// SMTypeJobInfo.h: interface for the SMTypeJobInfo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _JOBINFOBASESTATE
#define _JOBINFOBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
SMEnterAlphaNumericInfoBase : public SMCustomerModeState
{
public:
	SMEnterAlphaNumericInfoBase();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *PSAlphaNumericKey(const long); //TAR247048
  virtual SMStateBase  *PSAlphaKey(const long);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSSpaceKey(void);
  virtual SMStateBase  *DMScanner(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  
  DECLARE_DYNCREATE(SMEnterAlphaNumericInfoBase)// MFC Runtime class/object information
		
};

//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMLookUpItem(void);
// or
//    DLLEXPORT SMStateBase *makeSMLookUpItemBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMLookUpItem objects.  This function is called by
// RETURNSTATE(LookUpItem) instead of calling new STATE(LookUpItem) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMLookUpItemBase.cpp and SMLookUpItem.cpp
// by using the macro IMPLEMENT_STATE(LookUpItem)

//DECLARE_STATE(EnterAlphaNumericInfo)

#endif
