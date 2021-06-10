//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRestrictedItems.h
//
// TITLE: Class header for store mode Restricted Items state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMRESTRICTEDITEMSSTATE
#define _SMRESTRICTEDITEMSSTATE

#include "SMSmRestrictedItemsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmRestrictedItems : public SMSmRestrictedItemsBase
{
public:
    SMSmRestrictedItems();					  // constructor
    virtual SMStateBase * Initialize(void);
    virtual void UnInitialize(void);
	SMStateBase *DMScanner(void);

protected:
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
	virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);
    virtual SMStateBase  *OnWtIncreaseNotAllowed();
    virtual SMStateBase  *OnWtDecrease();

    DECLARE_DYNCREATE(SMSmRestrictedItems)// MFC Runtime class/object information
};

#endif
