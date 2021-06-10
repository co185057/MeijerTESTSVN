//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidAndRemoveItemBase.h
//
// TITLE: Class header for store mode void and remove voided item state
//
//
// AUTHOR:  Virgilio Esquilla, Jr.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSmVoidAndRemoveItemBaseSTATE
#define _SMSmVoidAndRemoveItemBaseSTATE

#ifdef _CPPUNIT
	#include "FakeSMStoreModeStateBase.h"
#else
	#include "SMStoreModeState.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMSmVoidAndRemoveItemBase :   public STATE(StoreModeState)
{
public:
  SMSmVoidAndRemoveItemBase();			         // constructor
  virtual SMStateBase  *Initialize(void);        // required override

#ifndef _CPPUNIT
protected:
  virtual void UnInitialize(void);
  SMStateBase  *PSButton1(void);         // override for button 1  
#endif //_CPPUNIT
  
  DECLARE_DYNCREATE(SMSmVoidAndRemoveItemBase)// MFC Runtime class/object information
		
};

#endif
