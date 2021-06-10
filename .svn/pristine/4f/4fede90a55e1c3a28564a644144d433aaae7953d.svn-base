//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmUpdateItemBase.h
//
// TITLE: Class header for update item state
//
//
// AUTHOR:    Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmUpdateItemBaseSTATE
#define _SmUpdateItemBaseSTATE

#ifndef _CPPUNIT
#include "SMStoreModeMenu.h"   
#else
#include "FakeSMStoreModeMenuBase.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmUpdateItemBase : public STATE(StoreModeState)
{
public:
#ifndef _CPPUNIT
  SMSmUpdateItemBase();
  SMSmUpdateItemBase(long lUpdateType);
  SMStateBase *Deliver(long lUpdateType);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase *Initialize(void);
  virtual void     UnInitialize(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
#endif
protected:
#ifndef _CPPUNIT
  virtual SMStateBase *OnChangeItem(void);
  virtual SMStateBase *PSCancelKey(void);   // override where needed
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton8(void);
  virtual SMStateBase *PSNumericKey(const long);
  virtual SMStateBase *PSClearKey(void);
  virtual SMStateBase *PSEnterKey(void);
  virtual SMStateBase *PSReceiptUp(void);
#endif

  virtual SMStateBase *PSReceiptDown(void);
 
#ifndef _CPPUNIT
  long m_lUpdateType;
  bool m_bCoupon;
  int m_nCrateableScreenAnswer;
  CString csCurContext;    // Tar 238987
#endif
  DECLARE_DYNCREATE(SMSmUpdateItemBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmUpdateItem(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmUpdateItemBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmUpdateItem objects.  This function is called by
// RETURNSTATE(SmUpdateItem) instead of calling new STATE(SmUpdateItem) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmUpdateItemBase.cpp and SMSmUpdateItem.cpp
// by using the macro IMPLEMENT_STATE(UpdateItem)

//DECLARE_STATE(SmUpdateItem)

#endif
