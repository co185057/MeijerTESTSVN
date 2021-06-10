//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInvalidBarcode.h
//
// TITLE:	Class header for 2D Invalid Barcode state
//
// AUTHOR:	Aiqin Hou
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INVALIDBARCODE
#define _INVALIDBARCODE

#include "SMInvalidBarcodeBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMInvalidBarcode : public SMInvalidBarcodeBase
{
public:
    SMInvalidBarcode();
    virtual SMStateBase *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *PSButton1(void);

    DECLARE_DYNCREATE(SMInvalidBarcode)// MFC Runtime class/object information		
};

#endif


