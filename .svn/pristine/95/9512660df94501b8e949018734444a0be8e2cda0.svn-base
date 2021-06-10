//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMItemNotForSale.h
//
// TITLE: Class header for item not for sale (recalled) State
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ITEMNOTFORSALESTATE
#define _ITEMNOTFORSALESTATE

#include "SMItemNotForSaleBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMItemNotForSale : public SMItemNotForSaleBase
{
public:
    SMItemNotForSale();

    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual bool PSAssistMenuLegalForState();
    virtual SMStateBase *TimedOut(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSEnterKey(void);


    DECLARE_DYNCREATE(SMItemNotForSale)// MFC Runtime class/object information
};

#endif