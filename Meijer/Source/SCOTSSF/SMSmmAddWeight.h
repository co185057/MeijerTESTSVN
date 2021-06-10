//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmmAddWeight.h
//
// TITLE: Class header for store mode Security Maintenance Mode
//
// AUTHOR: Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMADDWEIGHTSTATE
#define _SMMADDWEIGHTSTATE

#include "SMSmmAddWeightBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMSmmAddWeight : public SMSmmAddWeightBase
{
public:
    SMSmmAddWeight();					  // constructor
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *SASMMBagScaleEvent(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton8(void);


    DECLARE_DYNCREATE(SMSmmAddWeight)// MFC Runtime class/object information
};

#endif
