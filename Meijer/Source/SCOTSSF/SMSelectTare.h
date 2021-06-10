//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSelectTare.h
//
// TITLE: Class header for select tare state
//
//
// AUTHOR:    Peter Denhoed
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTTARESTATE
#define _SELECTTARESTATE

#include "SMSelectTareBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSelectTare : public SMSelectTareBase
{
public:
    SMSelectTare();                   // constructor
    virtual SMStateBase  *Initialize(void);
    virtual bool PSAssistMenuLegalForState();
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSListLookup(CString);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *PSListScroll(const long);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSReceiptUp(void);
    virtual SMStateBase  *PSReceiptDown(void);
  


    DECLARE_DYNCREATE(SMSelectTare)// MFC Runtime class/object information

};

#endif