//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVisualItemsBase.h
//
// TITLE: Class header for store mode Coupon Items state
//
//
// AUTHOR:  gary miller
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmVisualItemsBaseSTATE
#define _SmVisualItemsBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmVisualItemsBase : public STATE(StoreModeState)
{
public:
	SMSmVisualItemsBase();							  // constructor
  virtual SMStateBase  *Initialize(void);        // required override
	
protected:
  virtual SMStateBase  *PSButton1(void);         // override for button 1
  
	//SR81 begin
	virtual SMStateBase  *PSButton2(void);       // override for button 2
	//SR81 end

  virtual SMStateBase  *PSButton8(void);         // override for button 8
//  virtual SMStateBase  *PSCancelKey(void);       // override for cancel key ----> removed per tar 121487
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);      // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);    // to disable and enable scroll buttons when 

  // NewUI
	
  DECLARE_DYNCREATE(SMSmVisualItemsBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmVisualItems(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmVisualItemsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmVisualItems objects.  This function is called by
// RETURNSTATE(SmVisualItems) instead of calling new STATE(SmVisualItems) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmVisualItemsBase.cpp and SMSmVisualItems.cpp
// by using the macro IMPLEMENT_STATE(SmVisualItems)

//DECLARE_STATE(SmVisualItems)

#endif
