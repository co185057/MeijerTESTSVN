//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:   SMUnDeActivatedItemApproval.h
//
// TITLE:  Class header for SMUnDeActivatedItemApproval state
//
// AUTHOR: Anurag Mishra
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SMUNDEACTIVATEDITEMAPPROVAL__
#define __SMUNDEACTIVATEDITEMAPPROVAL__

#include "SMUnDeActivatedItemApprovalBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMUnDeActivatedItemApproval : public SMUnDeActivatedItemApprovalBase
{
public:
    SMUnDeActivatedItemApproval();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
protected:
    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase * stateAfterPasswordVerify();

    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *OnRAPClearSecurityBtn(LPCTSTR lpszAuthorizationType);
    virtual SMStateBase  *OnUnDeActivatedTheftTag(void);
    virtual SMStateBase  *OnMatchedWt(void);

    DECLARE_DYNCREATE(SMUnDeActivatedItemApproval)// MFC Runtime class/object information
};

#endif
