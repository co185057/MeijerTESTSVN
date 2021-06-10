//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPLAEnterWeight.h
//
// TITLE: Class header for lookup item(type item description) state
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLAENTERWEIGHTSTATE
#define _PLAENTERWEIGHTSTATE

#include "SMPLAEnterWeightBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMPLAEnterWeight : public SMPLAEnterWeightBase
{
public:
    SMPLAEnterWeight();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *OnPLATransition();

protected:
    virtual bool WaitForScaleRequired(bool bWeightChange = false);
    virtual SMStateBase  *DMScale(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *ProcessWeightOnScale(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSCancelKey(void);


    DECLARE_DYNCREATE(SMPLAEnterWeight) // MFC Runtime class/object information

};

#endif