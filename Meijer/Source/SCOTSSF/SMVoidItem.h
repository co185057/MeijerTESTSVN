//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMVoidItem.h
//
// TITLE: Class header for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _VOIDITEMSTATE
#define _VOIDITEMSTATE

#include "SMVoidItemBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMVoidItem : public SMVoidItemBase
{
public:
    SMVoidItem();
    virtual SMStateBase *Initialize(void);
    virtual void     UnInitialize(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual bool PSAssistMenuLegalForState();
    virtual SMStateBase  *OnBackToLGW(void);

protected:
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSButton3(void);
    virtual SMStateBase *DMScanner(void);
    virtual SMStateBase *PSCancelKey(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual SMStateBase *PSButtonGoBack(void);
    virtual bool CheckForScrollUp(long lRcptPos);
    virtual bool CheckForScrollDown(long lRcptPos);

	SMStateBase  *TimedOut(void);
	bool m_bPrevCancelButtonStatus;

    DECLARE_DYNCREATE(SMVoidItem)// MFC Runtime class/object information

};

#endif