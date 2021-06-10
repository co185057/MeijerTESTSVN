//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectContainer.h
//
// TITLE: Class header for void item state
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTCONTAINERSTATE
#define _SELECTCONTAINERSTATE

#include "SMSelectContainerBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSelectContainer : public SMSelectContainerBase
{
public:
    SMSelectContainer();

    virtual SMStateBase *Initialize(void);
    virtual bool PSAssistMenuLegalForState();
    virtual void UnInitialize(void);
protected:
    virtual SMStateBase *PSButton1(void);
    virtual SMStateBase *PSButton2(void);
    virtual SMStateBase *PSButton3(void);
    virtual SMStateBase *PSButton4(void);
    virtual SMStateBase *PSButtonGoBack(void);
    virtual SMStateBase  *PSCancelKey(void);
 


  DECLARE_DYNCREATE(SMSelectContainer)// MFC Runtime class/object information

};

#endif