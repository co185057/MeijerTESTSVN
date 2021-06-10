//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMInvalidBarcodeBase.h
//
// TITLE: Class header for an Invalid 2D Barcode State
//
// AUTHOR:  Rhonah Entize
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _InvalidBarcodeBaseSTATE
#define _InvalidBarcodeBaseSTATE

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

SMInvalidBarcodeBase : public SMCustomerModeState
{
public:
	SMInvalidBarcodeBase();
    virtual SMStateBase *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase *TimedOut(void);
    virtual SMStateBase *PSButton1(void);
	
    DECLARE_DYNCREATE(SMInvalidBarcodeBase)// MFC Runtime class/object information
		
};

#endif
