//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMProduceFavoritesBase.h
//
// TITLE: Class header for produce favorites state
//
// AUTHOR:    Ehrhardt, Dale
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PRODUCEFAVORITESBASESTATE
#define _PRODUCEFAVORITESBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
SMProduceFavoritesBase : public SMCustomerModeState
{
public:
	SMProduceFavoritesBase();
  virtual SMStateBase  *Initialize(void);
  virtual void     UnInitialize(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnSkipBaggingAllowed(void);

protected:
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSListLookup(CString);// override for list/lookups
// NewUI
  virtual SMStateBase  *PSListLookupTab(LPCTSTR);
  virtual SMStateBase  *PSListScroll(const long);
// NewUI
  virtual SMStateBase  *DMScanner(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *PSButton3(void);
  virtual SMStateBase  *PSButton4(void);

  virtual SMStateBase  *TimedOut(void);
  
  DECLARE_DYNCREATE(SMProduceFavoritesBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(ProduceFavorites)

#endif
