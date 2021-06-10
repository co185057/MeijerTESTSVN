//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSwipeCardAsItem.h
//
// TITLE: Class header for Confirn Signature
//
// AUTHOR:    Stephen J. Chasko
//
// Copyright (c) NCR Corp. 2003
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SwipePhoneCardSTATE
#define _SwipePhoneCardSTATE

#include "SMSwipeCardBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif




SMSwipeCardAsItem : public SMCustomerModeState
{
    
public:
    SMSwipeCardAsItem();
    virtual void UnInitialize(void);
    SMStateBase *Initialize(void);

protected:
    
    SMStateBase *PSButtonGoBack(void); 
    
    DECLARE_DYNCREATE(SMSwipeCardAsItem)// MFC Runtime class/object information
private:
   
};


#endif