//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCrateableItem.h
//
// TITLE: Class header for purchase crateable item state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CRATEABLEITEMSTATE
#define _CRATEABLEITEMSTATE

#include "SMCrateableItemBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCrateableItem : public SMCrateableItemBase
{
public:
    SMCrateableItem();

    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual bool PSAssistMenuLegalForState();

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);

    DECLARE_DYNCREATE(SMCrateableItem)// MFC Runtime class/object information
		
};

#endif