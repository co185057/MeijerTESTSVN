//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmUpdateItem.h
//
// TITLE: Class header for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmUpdateItemSTATE
#define _SmUpdateItemSTATE

#include "SMSmUpdateItemBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmUpdateItem : public SMSmUpdateItemBase
{
public:
    SMSmUpdateItem();
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase *Initialize(void);
    virtual void     UnInitialize(void);
    virtual bool PSAssistMenuLegalForState();
protected:
    virtual SMStateBase *OnChangeItem(void);
    virtual SMStateBase *PSCancelKey(void);
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton8(void);
    virtual SMStateBase *PSNumericKey(const long);
    virtual SMStateBase *PSClearKey(void);
    virtual SMStateBase *PSEnterKey(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);

    DECLARE_DYNCREATE(SMSmUpdateItem)// MFC Runtime class/object information

};

#endif