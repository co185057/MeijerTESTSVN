//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMTimeRestrictedItem.h
//
// TITLE: Class header for item not for time restricted item State
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TIMERESTRICTEDITEM
#define _TIMERESTRICTEDITEM

#include "SMTimeRestrictedItemBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMTimeRestrictedItem : public SMTimeRestrictedItemBase
{
public:
    SMTimeRestrictedItem();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual bool PSAssistMenuLegalForState();
    virtual SMStateBase *TimedOut(void);
protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSEnterKey(void);

    DECLARE_DYNCREATE(SMTimeRestrictedItem)// MFC Runtime class/object information
};

#endif