//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSellBags.h
//
// TITLE: Class header for Sell Bags state - added for SR713
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELLBAGSSTATE
#define _SELLBAGSSTATE

#include "SMSellBagsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
SMSellBags : public SMSellBagsBase
{
public:
    SMSellBags();
    SMSellBags(LPCTSTR sItemCode, const long lQty, LPCTSTR sPrice);
    SMStateBase *Deliver(LPCTSTR sItemCode, const long lQty, LPCTSTR sPrice);

    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);	
  
protected:
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSBackSpaceKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *ConfirmQtyForBags(void);



  DECLARE_DYNCREATE(SMSellBags)// MFC Runtime class/object information
		
};

#endif