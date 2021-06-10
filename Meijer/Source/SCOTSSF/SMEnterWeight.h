//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMEnterWeight.h
//
// TITLE: Class header for enter weight state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ENTERWEIGHTSTATE
#define _ENTERWEIGHTSTATE

#include "SMEnterWeightBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMEnterWeight : public SMEnterWeightBase
{
public:
    SMEnterWeight();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual bool PSAssistMenuLegalForState();
protected:
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSCancelKey(void);
    virtual SMStateBase  *DMScale(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *ProcessWeightOnScale();
    virtual void HandleEnterWeightTimedOut();
    virtual SMStateBase  *TimedOut(void);
    virtual bool ShowAcceptWeightBtnAndStartEnterWeightTimer(bool bWeightChange = false);

    DECLARE_DYNCREATE(SMEnterWeight)// MFC Runtime class/object information
		
};

#endif