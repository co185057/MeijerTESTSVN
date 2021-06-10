//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidAndRemoveItem.h
//
// TITLE: Class header for store mode void and remove voided item state
//
//
// AUTHOR:  Virgilio Esquilla, Jr.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMVOIDANDREMOVEITEMSTATE
#define _SMVOIDANDREMOVEITEMSTATE

#include "SMSmVoidAndRemoveItemBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmVoidAndRemoveItem : public SMSmVoidAndRemoveItemBase
{
public:
    SMSmVoidAndRemoveItem();									  // constructor
    virtual SMStateBase  *Initialize(void);
protected:
    virtual void UnInitialize(void);
    SMStateBase  *PSButton1(void);

    DECLARE_DYNCREATE(SMSmVoidAndRemoveItem)// MFC Runtime class/object information

};

#endif