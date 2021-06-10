//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForApprovalBase.h
//
// TITLE: Class header for wait for approval state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WaitForApprovalBaseSTATE
#define _WaitForApprovalBaseSTATE

#ifndef _CPPUNIT
#include "SMOperatorPasswordState.h"
#else
#include "FakeSMOperatorPasswordStateBase.h"
#endif // _CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
SMWaitForApprovalBase : public STATE(OperatorPasswordState)
{
public:
    SMWaitForApprovalBase();
#ifndef _CPPUNIT
    SMWaitForApprovalBase(const bool fTV, // transaction verify
        const bool fVT, const bool bVerifySig=false);// void transaction
    
    SMStateBase *Deliver(const bool fTV, // transaction verify
        const bool fVT, const bool bVerifySig=false);
    virtual SMStateBase * Initialize(void);
    virtual SMStateBase * TimedOut(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual void UnInitialize(void);  //TAR198810
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase  *OnCouponSensor(void);	//TAR 386740
    virtual SMStateBase *SASecMgrParse( const MessageElement *me);
#endif //_CPPUNIT      
protected:
#ifndef _CPPUNIT
    virtual SMStateBase  *PSButtonHelp(void);
    virtual SMStateBase  *PSCancelKey(void);   // Override where needed
    virtual SMStateBase  *PSButton1();         // Store Sign In
    virtual SMStateBase  *OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase  *OnUnDeActivatedTheftTag(void);
#endif //_CPPUNIT    
    virtual void showScreen(bool passwordVerified);
    //private:
    bool fVoidTrx;
    bool m_bVerifySig ;
    
    int  nSaidCount;
    
    DECLARE_DYNCREATE(SMWaitForApprovalBase)// MFC Runtime class/object information
};

//DECLARE_STATE(WaitForApproval)

#endif
