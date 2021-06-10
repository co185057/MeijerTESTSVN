//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidItemBase.h
//
// TITLE: Class header for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _VoidItemBaseSTATE
#define _VoidItemBaseSTATE

#ifdef _CPPUNIT
   #include "FakeSMCustomerModeState.h"
   #include "FakeSMStateBase.h"
#else
   #include "SMCustomerModeState.h"
#endif // _CPPUNIT


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMVoidItemBase : public SMCUSTOMERMODESTATE
{
public:
  SMVoidItemBase();
  #ifndef _CPPUNIT
  virtual SMStateBase *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual bool PSAssistMenuLegalForState() { return true; };
  #endif //_CPPUNIT
  virtual SMStateBase  *OnBackToLGW(void);
protected:
  virtual SMStateBase *PSReceiptDown(void); // at the ends of the list

  #ifndef _CPPUNIT
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *PSButton3(void);
  virtual SMStateBase *DMScanner(void);
  virtual SMStateBase *PSCancelKey(void);   // override where needed
  virtual SMStateBase *PSReceiptUp(void);   // to disable and enable scroll buttons when 

  //@@@INFOPOINT
  virtual SMStateBase *PSButtonGoBack(void);
  //@@@INFOPOINT
  
  virtual bool CheckForScrollUp(long lRcptPos);
  virtual bool CheckForScrollDown(long lRcptPos);
  #endif //CPPUNIT
  
  CString csCurContext;    // Tar 238987
  CString m_sCurrentReceipt; 
  CString m_sCurrentReceiptSrc;
  bool m_bIsFromMultiSelectPickList;
  DECLARE_DYNCREATE(SMVoidItemBase)// MFC Runtime class/object information
		
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMVoidItem(void);
// or
//    DLLEXPORT SMStateBase *makeSMVoidItemBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMVoidItem objects.  This function is called by
// RETURNSTATE(VoidItem) instead of calling new STATE(VoidItem) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMVoidItemBase.cpp and SMVoidItem.cpp
// by using the macro IMPLEMENT_STATE(VoidItem)

//DECLARE_STATE(VoidItem)

#endif
