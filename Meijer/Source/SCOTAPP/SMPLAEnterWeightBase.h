//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMPLAEnterWeightBase.h
//
// TITLE: Class header for VegiCam enter weight state
//
//
// AUTHOR:  David Moore
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLAENTERWEIGHTBASESTATE
#define _PLAENTERWEIGHTBASESTATE

#ifdef _CPPUNIT
   #include "FakeSMCustomerModeState.h"
#else
   #include "SMCustomerModeState.h"
#endif // _CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMPLAEnterWeightBase : public SMCUSTOMERMODESTATE
{
public:
	SMPLAEnterWeightBase();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *OnPLATransition();

protected:
    static SMPLAEnterWeightBase* m_pEnterWeight;	
    CString csCurContext;    
    virtual bool WaitForScaleRequired(bool bWeightChange = false);
    virtual SMStateBase  *DMScale(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *ProcessWeightOnScale(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSCancelKey(void);
    bool weightchanged;

  DECLARE_DYNCREATE(SMPLAEnterWeightBase)// MFC Runtime class/object information
		
};

#endif