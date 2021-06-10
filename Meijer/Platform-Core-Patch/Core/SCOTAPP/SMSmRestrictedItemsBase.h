//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRestrictedItemsBase.h
//
// TITLE: Class header for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmRestrictedItemsBaseSTATE
#define _SmRestrictedItemsBaseSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmRestrictedItemsBase : public STATE(StoreModeState)
{
public:
          SMSmRestrictedItemsBase();		     // constructor
  virtual SMStateBase * Initialize(void);        // required override
  virtual void UnInitialize(void);		// RFC 330050
	
protected:

  virtual SMStateBase  *PSButton1(void);         // override for button 1
  virtual SMStateBase  *PSButton2(void);         // override for button 2

	//SR81 begin
	virtual SMStateBase  *PSButton3(void);       // override for button 3
	//SR81 end

  virtual SMStateBase  *PSButton8(void);         //
  virtual SMStateBase  *PSNumericKey(const long);	// a key 0-9 was pressed
  virtual SMStateBase  *PSEnterKey(void);        // override for enter key
  virtual SMStateBase  *PSClearKey(void);        // override for clear key
  // NewUI
  virtual SMStateBase *PSReceiptUp(void);   // to disable and enable scroll buttons when 
  virtual SMStateBase *PSReceiptDown(void);   // to disable and enable scroll buttons when 
  virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);
  CString csCurContext;    // Tar 238987

  // NewUI
  DECLARE_DYNCREATE(SMSmRestrictedItemsBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmRestrictedItems(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmRestrictedItemsBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmRestrictedItems objects.  This function is called by
// RETURNSTATE(SmRestrictedItems) instead of calling new STATE(SmRestrictedItems) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmRestrictedItemsBase.cpp and SMSmRestrictedItems.cpp
// by using the macro IMPLEMENT_STATE(SmRestrictedItems)

//DECLARE_STATE(SmRestrictedItems)

#endif
