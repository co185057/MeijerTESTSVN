//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSellBagsBase.h
//
// TITLE: Class header for Sell Bags state - SR713
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELLBAGSBASESTATE
#define _SELLBAGSBASESTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

SMSellBagsBase : public SMCustomerModeState
{

protected:

  long m_lQty;
  static CString m_csBagItemCode;
  static CString m_csBagPrice;

public:
  SMSellBagsBase();
  SMSellBagsBase(LPCTSTR sItemCode, const long lQty, LPCTSTR sPrice);
  SMStateBase *Deliver(LPCTSTR sItemCode,const long lQty, LPCTSTR sPrice);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase  *Initialize(void);
  virtual void UnInitialize(void);	
  
protected:
  virtual SMStateBase  *PSNumericKey(const long);
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSEnterKey(void);
  virtual SMStateBase  *PSButtonGoBack(void);
  virtual SMStateBase  *PSButton1(void);
  virtual SMStateBase  *PSButton2(void);
  virtual SMStateBase  *ConfirmQtyForBags(void);
  

  DECLARE_DYNCREATE(SMSellBagsBase)// MFC Runtime class/object information
		
};

//DECLARE_STATE(SellBags)

#endif
