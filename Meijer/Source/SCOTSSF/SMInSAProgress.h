//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInSAProgress.h
//
// Description: The InSAProgress state is used when the transition from 
//			TB_ITEMSOLD to the next Security event, which is the SASERVER WLBD
//			lookup return, and might take some time.  
//
// Author:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _INSAPROGRESSSTATE
#define _INSAPROGRESSSTATE

#include "SMInSAProgressBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMInSAProgress : public SMInSAProgressBase
{
public:
    // Constructor
    SMInSAProgress();
    virtual bool inProgress();
    virtual bool storeMode();
    virtual bool helpMode();
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *OnDataBaseInfoComplete(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *DMScanner(void);
    virtual SMStateBase  *DMCardReader(void);
    virtual SMStateBase  *OnBackToLGW(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *DMdf_EASNoMotion(void);
    virtual SMStateBase  *PSButtonHelp(void);

    DECLARE_DYNCREATE(SMInSAProgress)// MFC Runtime class/object information
};

#endif
