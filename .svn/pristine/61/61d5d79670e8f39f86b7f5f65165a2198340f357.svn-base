//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidedItems.h
//
// TITLE: Class header for store mode Voided Items state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMVOIDEDITEMSSTATE
#define _SMVOIDEDITEMSSTATE

#include "SMSmVoidedItemsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmVoidedItems : public SMSmVoidedItemsBase
{
public:
    SMSmVoidedItems();							  // constructor

    virtual SMStateBase  *Initialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton8(void);

    DECLARE_DYNCREATE(SMSmVoidedItems)// MFC Runtime class/object information
};

#endif